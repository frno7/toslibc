// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <stdlib.h>
#include <string.h>

#include "tool/elf32.h"
#include "tool/print.h"
#include "tool/section.h"
#include "tool/verify.h"

static void verify_symtab(struct file *f)
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

static void verify_sections(struct file *f)
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
}

void verify_elf(struct file *f)
{
	if (!file_valid(f))
		pr_fatal_errno(f->path);

	if (!elf_identify(f->data, f->size))
		pr_fatal_error("%s: invalid ELF file", f->path);

	verify_sections(f);
	verify_symtab(f);
}
