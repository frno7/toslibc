// SPDX-License-Identifier: GPL-2.0

#ifndef TOSLIBC_TOOL_SECTION_H
#define TOSLIBC_TOOL_SECTION_H

#include <stdbool.h>

#include "tool/elf32.h"
#include "tool/file.h"

bool null_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool text_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool data_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool bss_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool rela_text_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool rela_data_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool symtab_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool strtab_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

bool shstrtab_section(Elf_Shdr *shdr, Elf_Ehdr *ehdr);

uint32_t section_size(const struct file *f,
	const char *name, bool (*section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr));

void append_section(struct file *tf, struct file *ef,
	const char *name, bool (*section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr));

void append_sections_text_data(struct file *tf, struct file *ef);

#endif /* TOSLIBC_TOOL_SECTION_H */
