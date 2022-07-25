// SPDX-License-Identifier: GPL-2.0
/*
 * 32-bit executable and linkable format (ELF)
 *
 * Copyright (C) 2019 Fredrik Noring
 */

#ifndef TOSLIBC_TOOL_ELF32_H
#define TOSLIBC_TOOL_ELF32_H

#define ENDIAN_STORAGE BE_STORAGE

#include "elfst.h"

typedef Elf32_Half    Elf_Half;
typedef Elf32_Word    Elf_Word;
typedef Elf32_Sword   Elf_Sword;
typedef Elf32_Xword   Elf_Xword;
typedef Elf32_Sxword  Elf_Sxword;
typedef Elf32_Addr    Elf_Addr;
typedef Elf32_Off     Elf_Off;
typedef Elf32_Section Elf_Section;
typedef Elf32_Versym  Elf_Versym;

typedef Elf32_Ehdr    Elf_Ehdr;
typedef Elf32_Shdr    Elf_Shdr;
typedef Elf32_Sym     Elf_Sym;
typedef Elf32_Syminfo Elf_Syminfo;
typedef Elf32_Rel     Elf_Rel;
typedef Elf32_Rela    Elf_Rela;

#define ELF_ST_BIND(st_info) ELF32_ST_BIND(st_info)
#define ELF_ST_TYPE(st_info) ELF32_ST_TYPE(st_info)

#define ELF_R_TYPE(r_info) ELF32_R_TYPE(r_info)
#define ELF_R_SYM(r_info)  ELF32_R_SYM(r_info)

#include "elf.h"

#endif /* TOSLIBC_TOOL_ELF32_H */
