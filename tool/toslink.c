// SPDX-License-Identifier: GPL-2.0
/*
 * Link programs for Atari TOS
 *
 * Copyright (C) 2022 Fredrik Noring
 */

#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "internal/compare.h"

#include "tool/elf32.h"
#include "tool/file.h"
#include "tool/option.h"
#include "tool/prg.h"
#include "tool/print.h"
#include "tool/section.h"
#include "tool/symbol.h"
#include "tool/tool.h"

char progname[] = "toslink";

#define PRG_HEADER_SIZE 28

static void check_symtab(struct file *f)
{
	Elf_Ehdr *ehdr = (Elf_Ehdr *)f->data;
	Elf_Shdr *shdr;
	Elf_Sym *sym;
	size_t errs = 0;

	elf_for_each_sym (sym, shdr, ehdr) {
		const char *name = elf_symbol(sym, shdr, ehdr);

		if (sym->st_shndx == SHN_UNDEF && *name) {
			pr_error("%s: undefined reference to `%s'",
				f->path, name);
			errs++;
		}
	}

	if (errs > 0)
		exit(EXIT_FAILURE);
}

static void check_program(struct file *f)
{
	struct {
		const char *name;
		const bool (* const section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr);
		const int align;
		const int min;
		const int max;
		int n;
	} sections[] = {
		{ "",           null_section,      0, 0, 1 },
		{ ".text",      text_section,      4, 1, 1 },
		{ ".rela.text", rela_text_section, 0, 0, 1 },
		{ ".rela.data", rela_data_section, 0, 0, 1 },
		{ ".data",      data_section,      4, 0, 1 },
		{ ".bss",       bss_section,       4, 0, 1 },
		{ ".symtab",    symtab_section,    0, 0, 1 },
		{ ".strtab",    strtab_section,    0, 0, 1 },
		{ ".shstrtab",  shstrtab_section,  0, 0, 1 },
	};
	Elf_Ehdr *ehdr = (Elf_Ehdr *)f->data;
	Elf_Shdr *shdr;

	if (ehdr->e_entry != 0)
		pr_fatal_error("%s: entry address nonzero 0x%x",
			f->path, ehdr->e_entry);

	elf_for_each_section (shdr, ehdr) {
		for (int i = 0; i < ARRAY_SIZE(sections); i++) {
			if (!sections[i].section(shdr, ehdr))
				continue;

			if (strcmp(elf_section_name(shdr, ehdr), sections[i].name) != 0)
				pr_warn("%s: unexpected section name \"%s\", expected \"%s\"",
					f->path, elf_section_name(shdr, ehdr),
					sections[i].name);

			if (sections[i].align) {
				if (shdr->sh_offset % sections[i].align)
					pr_fatal_error("%s: unaligned section offset %u",
						f->path, shdr->sh_offset);

				if (shdr->sh_size % sections[i].align)
					pr_fatal_error("%s: unaligned section size %u",
						f->path, shdr->sh_size);
			}

			sections[i].n++;

			goto next;
		}

		pr_fatal_error("%s: unexpected section type %d flags %x name \"%s\"",
			f->path, shdr->sh_type, shdr->sh_flags,
			elf_section_name(shdr, ehdr));
next:
	}

	for (int i = 0; i < ARRAY_SIZE(sections); i++)
		if (sections[i].n < sections[i].min ||
		    sections[i].n > sections[i].max)
		pr_fatal_error("%s: unexpected section count %d name \"%s\", min %d, max %d",
			f->path, sections[i].n, sections[i].name,
			sections[i].min, sections[i].max);

	check_symtab(f);
}

static size_t append_header(struct file *tf, struct file *ef)
{
	const struct prg_header prg_header = {
		.magic       = prg_header_magic,
		.text_size   = section_size(ef, text_section),
		.data_size   = section_size(ef, data_section),
		.bss_size    = section_size(ef, bss_section),
		.symbol_size = symbol_size(ef, symtab_section)
	};

	BUILD_BUG_ON(sizeof(struct prg_header) != PRG_HEADER_SIZE);

	file_append(tf, (const void *)&prg_header, sizeof(prg_header));

	return sizeof(prg_header) +
	       prg_header.text_size +
	       prg_header.data_size +
	       prg_header.symbol_size;
}

static Elf_Sym *sym_for_rela(Elf_Rela *rela, Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	Elf_Shdr *shdr_symtab = &elf_first_section(ehdr)[shdr->sh_link];
	const uint32_t i = ELF_R_SYM(rela->r_info);
	uint8_t *b = elf_first_ent(shdr_symtab, ehdr);
	Elf_Sym *sym = (Elf_Sym *)&b[shdr_symtab->sh_entsize * i];

	return sym;
}

static void reloc_code(const uint32_t addr, const uint32_t r,
	const int type, struct file *ef, struct file *tf)
{
	const size_t size =
		type == R_68K_PC8  ? 1 :
		type == R_68K_PC16 ? 2 :
		type == R_68K_PC32 ? 4 :
		type == R_68K_32   ? 4 : 0;

	const ssize_t offset =
		type == R_68K_PC8  ? -addr :
		type == R_68K_PC16 ? -addr :
		type == R_68K_PC32 ? -addr : 0;

	uint8_t *b = tf->data;

	if (!size)
		pr_fatal_error("%s: unknown relocation type %d", ef->path, type);

	for (size_t i = 0; i < size; i++) {
		const size_t k = PRG_HEADER_SIZE + addr + i;

		if (b[k])
			pr_fatal_error("%s: nonzero code at %x", ef->path, k);

		b[k] = ((r + offset) >> (8 * (size - 1 - i))) & 0xff;
	}

}

static uint32_t reloc_entry(uint32_t *reloc,
	const uint32_t addr, struct file *tf)
{
	size_t size = 0;

	if (!*reloc) {
		const struct BE_STORAGE { uint32_t addr; } first = { addr };

		BUILD_BUG_ON(sizeof(first) != 4);

		file_append(tf, (const void *)&first, sizeof(first));

		size += 4;
	} else
		for (uint32_t d = addr - *reloc; d > 0;) {
			const uint8_t r = d <= 254 ? d : 1;

			file_append(tf, &r, sizeof(r));

			d -= min(d, 254u);
			size++;
		}

	*reloc = addr;

	return size;
}

static size_t append_relocations(struct file *tf, struct file *ef,
	uint32_t *reloc, bool (*section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr))
{
	if (!section) {
		const uint32_t z = 0;
		const size_t s = *reloc ? 1 : 4;

		file_append(tf, &z, s);

		return s;
	}

	Elf_Ehdr *ehdr = (Elf_Ehdr *)ef->data;
	Elf_Shdr *shdr_rela;
	size_t size = 0;

	elf_for_each_rela (shdr_rela, ehdr) {
	       Elf_Shdr *shdr = &elf_first_section(ehdr)[shdr_rela->sh_info];
	       Elf_Rela *rela;

	       if (!section(shdr, ehdr))
			continue;

	       elf_for_each_ent (rela, shdr_rela, ehdr) {
			const int type = ELF_R_TYPE(rela->r_info);
			const uint32_t addr = shdr->sh_addr + rela->r_offset;

			if (addr <= *reloc || addr % 2 != 0)
				pr_fatal_error("%s: malformed relocations %x, %zx",
					ef->path, addr, *reloc);

			Elf_Sym *sym = sym_for_rela(rela, shdr_rela, ehdr);
			const uint32_t r = rela->r_addend + sym->st_value +
				elf_first_section(ehdr)[sym->st_shndx].sh_addr;

			reloc_code(addr, r, type, ef, tf);

			if (type == R_68K_PC8  ||
			    type == R_68K_PC16 ||
			    type == R_68K_PC32)
				continue;

			size += reloc_entry(reloc, addr, tf);
	       }
	}

	return size;
}

static void link_program(struct file *tf, struct file *ef)
{
	size_t size = append_header(tf, ef);
	uint32_t reloc = 0;

	append_sections_text_data(tf, ef);

	append_symbols(tf, ef);

	size += append_relocations(tf, ef, &reloc, text_section);
	size += append_relocations(tf, ef, &reloc, data_section);
	size += append_relocations(tf, ef, &reloc, NULL);

	if (tf->size != size)
		pr_fatal_error("%s: size mismatch %zu != %zu",
			ef->path, tf->size, size);
}

int main(int argc, char **argv)
{
	int status = EXIT_FAILURE;

	parse_options(argc, argv);

	struct file ef = file_read(option.input);
	if (!file_valid(ef))
		pr_fatal_errno(option.input);

	if (!elf_identify(ef.data, ef.size))
		pr_fatal_error("%s: invalid ELF file", option.input);

	struct file tf = { };

	check_program(&ef);
	link_program(&tf, &ef);

	if (!file_rename(option.output, &tf))
		pr_fatal_errno(option.output);

	if (!file_write(tf, 0755))
		pr_fatal_errno(tf.path);

	status = EXIT_SUCCESS;

	file_free(tf);
	file_free(ef);

	return status;
}
