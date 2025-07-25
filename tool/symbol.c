// SPDX-License-Identifier: GPL-2.0
/*
 * DRI and GST symbols table
 *
 * Copyright (C) 2025 Fredrik Noring
 */

#include <string.h>

#include "internal/compare.h"

#include "tool/section.h"
#include "tool/symbol.h"

enum {
	dri_symbol_type_bss  = 0x0100,
	dri_symbol_type_text = 0x0200,
	dri_symbol_type_data = 0x0400,
	gst_symbol_type      = 0x0048,
};

struct dri_symbol {
	char name[8];
	uint16_t type;
	uint32_t offset;
} BE_STORAGE __attribute__((__packed__));

struct gst_symbol {
	char name1[8];
	uint16_t type;
	uint32_t offset;
	char name2[14];
} BE_STORAGE __attribute__((__packed__));

static bool gst_symbol(Elf_Sym *sym, Elf_Shdr *shdr, Elf_Ehdr *ehdr)
{
	return strlen(elf_symbol(sym, shdr, ehdr)) >
	       sizeof_member(struct dri_symbol, name);
}

static uint16_t dri_symbol_type(Elf_Sym *sym, Elf_Ehdr *ehdr)
{
	if (ELF_ST_BIND(sym->st_info) != STB_GLOBAL)
		return 0;

	Elf_Shdr *shdr = &elf_first_section(ehdr)[sym->st_shndx];

	return text_section(shdr, ehdr) ? dri_symbol_type_text :
	       data_section(shdr, ehdr) ? dri_symbol_type_data :
	        bss_section(shdr, ehdr) ? dri_symbol_type_bss  : 0;
}

uint32_t symbol_size(const struct file *f,
	bool (*section)(Elf_Shdr *shdr, Elf_Ehdr *ehdr))
{
	Elf_Ehdr *ehdr = (Elf_Ehdr *)f->data;
	Elf_Shdr *shdr;
	Elf_Sym *sym;
	uint32_t size = 0;

	elf_for_each_sym (sym, shdr, ehdr)
		if (dri_symbol_type(sym, ehdr))
			size += gst_symbol(sym, shdr, ehdr) ?
				sizeof(struct gst_symbol) :
				sizeof(struct dri_symbol);

	return size;
}

void append_symbols(struct file *tf, struct file *ef)
{
	BUILD_BUG_ON(sizeof(struct dri_symbol) != 14);
	BUILD_BUG_ON(sizeof(struct gst_symbol) != 28);

	Elf_Ehdr *ehdr = (Elf_Ehdr *)ef->data;
	Elf_Shdr *shdr;
	Elf_Sym *sym;

	elf_for_each_sym (sym, shdr, ehdr) {
		const uint16_t dri_type = dri_symbol_type(sym, ehdr);
		if (!dri_type)
			continue;

		const char *s = elf_symbol(sym, shdr, ehdr);
		const size_t n = strlen(s);

		if (n <= sizeof_member(struct dri_symbol, name)) {
			struct dri_symbol dri = {
				.type = dri_type,
				.offset = sym->st_value
			};

			memcpy(dri.name, s, n);
			file_append(tf, (const void *)&dri, sizeof(dri));
		} else {
			struct gst_symbol gst = {
				.type = dri_type | gst_symbol_type,
				.offset = sym->st_value
			};

			memcpy(gst.name1, s, sizeof(gst.name1));
			memcpy(gst.name2, &s[sizeof(gst.name1)],
				min(n - sizeof(gst.name1), sizeof(gst.name2)));
			file_append(tf, (const void *)&gst, sizeof(gst));
		}
	}
}
