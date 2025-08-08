// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <string.h>

#include "tool/section.h"

bool null_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return shdr->sh_type == SHT_NULL;
}

bool text_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return (shdr->sh_type == SHT_PROGBITS) &&
	       (shdr->sh_flags & SHF_EXECINSTR) &&
	       (shdr->sh_flags & SHF_ALLOC);
}

bool data_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return (shdr->sh_type == SHT_PROGBITS) &&
	       (shdr->sh_flags & SHF_ALLOC) &&
	      !(shdr->sh_flags & SHF_EXECINSTR);
}

bool bss_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return (shdr->sh_type == SHT_NOBITS) &&
	       (shdr->sh_flags & SHF_ALLOC) &&
	       (shdr->sh_flags & SHF_WRITE) &&
	      !(shdr->sh_flags & SHF_EXECINSTR);
}

bool rela_text_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return (shdr->sh_type == SHT_RELA) &&
	       (shdr->sh_flags & SHF_INFO_LINK) &&
	       text_section(&elf_first_section(ehdr)[shdr->sh_info], ehdr);
}

bool rela_data_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return (shdr->sh_type == SHT_RELA) &&
	       (shdr->sh_flags & SHF_INFO_LINK) &&
	       data_section(&elf_first_section(ehdr)[shdr->sh_info], ehdr);
}

bool symtab_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return shdr->sh_type == SHT_SYMTAB;
}

bool strtab_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return (shdr->sh_type == SHT_STRTAB) &&
	       strcmp(elf_section_name(shdr, ehdr), ".strtab") == 0;
}

bool shstrtab_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return (shdr->sh_type == SHT_STRTAB) &&
	       strcmp(elf_section_name(shdr, ehdr), ".shstrtab") == 0;
}

uint32_t section_size(const struct file *f,
	const char *name, bool (*section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr))
{
	Elf_Ehdr *ehdr = (Elf_Ehdr *)f->data;
	Elf_Shdr *shdr;
	uint32_t size = 0;

	elf_for_each_section (shdr, ehdr)
		if (strcmp(elf_section_name(shdr, ehdr), name) == 0 &&
		    section(shdr ,ehdr))
			size += shdr->sh_size;

	return size;
}

static void append_section(struct file *tf, struct file *ef,
	const char *name, bool (*section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr))
{
	Elf_Ehdr *ehdr = (Elf_Ehdr *)ef->data;
	Elf_Shdr *shdr;
	const uint8_t *b = ef->data;

	elf_for_each_section (shdr, ehdr)
		if (strcmp(elf_section_name(shdr, ehdr), name) == 0 &&
		    section(shdr ,ehdr))
			file_append(tf, &b[shdr->sh_offset], shdr->sh_size);
}

void append_sections_text_data(struct file *tf, struct file *ef)
{
	append_section(tf, ef, ".text", text_section);
	append_section(tf, ef, ".data", data_section);
}
