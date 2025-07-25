// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2019 Fredrik Noring
 */

#ifndef TOSLIBC_TOOL_PRG_H
#define TOSLIBC_TOOL_PRG_H

#include <stdint.h>

#include "internal/struct.h"

enum { prg_header_magic = 0x601a };

/**
 * struct prg_header_flags - program header flags
 * @ttrammem: set to allow malloc requests from alternative RAM, otherwise
 * 	standard RAM
 * @ttramload: set to allow the program to load into alternative RAM, otherwise
 * 	load into standard RAM
 * @fastload: set to clear only the BSS area on program load, otherwise clear
 * 	the entire heap
 */
struct prg_header_flags
{
	uint32_t : 29;
	uint32_t ttrammem : 1;
	uint32_t ttramload : 1;
	uint32_t fastload : 1;
} BE_STORAGE;

/**
 * struct prg_header - program header
 * @magic: %prg_header_magic
 * @text_size: size of text segment in bytes
 * @data_size: size of data segment in bytes
 * @bss_size: size of BSS segment in bytes
 * @symbol_size: size of symbol table in bytes
 * @reserved: reserved and unused
 * @flags: program header flags
 * @absflag: zero indicates there is a relocation table
 *
 * Note that some versions of TOS handle programs with @relocate being
 * nonzero incorrectly. For compatibility, have it always be zero and
 * place an empty relocation table instead.
 */
struct prg_header {
	uint16_t magic;
	uint32_t text_size;
	uint32_t data_size;
	uint32_t bss_size;
	uint32_t symbol_size;
	uint32_t reserved;
	struct prg_header_flags flags;
	uint16_t absflag;
} BE_STORAGE __attribute__((__packed__));

#endif /* TOSLIBC_TOOL_PRG_H */
