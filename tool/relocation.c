// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include "internal/compare.h"

#include "tool/elf32.h"
#include "tool/print.h"
#include "tool/relocation.h"
#include "tool/section.h"
#include "tool/tool.h"

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

size_t append_relocations_text_data(struct file *tf, struct file *ef)
{
	uint32_t reloc = 0;
	size_t size = 0;

	size += append_relocations(tf, ef, &reloc, text_section);
	size += append_relocations(tf, ef, &reloc, data_section);
	size += append_relocations(tf, ef, &reloc, NULL);

	return size;
}
