# SPDX-License-Identifier: GPL-2.0

TOSLIBC_TOOL_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLINK = $(TOSLIBC_TOOL_SUBDIR)$(target)-toslink

TOSLINK_CFLAGS = $(BASIC_CFLAGS) $(S_CFLAGS) $(CFLAGS)			\
	-I$(TOSLIBC_TOOL_SUBDIR)../include

TOSLIBC_VERSION_SRC = $(TOSLIBC_TOOL_SUBDIR)version.c
TOSLIBC_VERSION_SCRIPT = $(TOSLIBC_TOOL_SUBDIR)../script/version	\
	--directory $(TOSLIBC_TOOL_SUBDIR)..

.PHONY: $(shell $(TOSLIBC_VERSION_SCRIPT) --check $(TOSLIBC_VERSION_SRC))
$(TOSLIBC_VERSION_SRC):
	$(QUIET_GEN)$(TOSLIBC_VERSION_SCRIPT) $@

TOSLINK_SRC = $(addprefix $(TOSLIBC_TOOL_SUBDIR),			\
	elf32.c								\
	file.c								\
	memory.c							\
	print.c								\
	toslink.c)							\
	$(TOSLIBC_VERSION_SRC)

TOSLINK_OBJ = $(TOSLINK_SRC:%.c=%.o)

ALL_OBJ += $(TOSLINK_OBJ)

$(TOSLINK_OBJ): %.o: %.c
	$(QUIET_CC)$(CC) $(TOSLINK_CFLAGS) -c -o $@ $<

$(TOSLINK): $(TOSLINK_OBJ)
	$(QUIET_LINK)$(CC) $(TOSLINK_CFLAGS) -o $@ $^

.PHONY: tool
tool: $(TOSLINK)

.PHONY: install-toslink
install-toslink: $(TOSLINK)
	$(INSTALL) -d $(DESTDIR)$(binutilsbindir)
	$(INSTALL) $(TOSLINK) $(DESTDIR)$(binutilsbindir)

.PHONY: binutils
binutils: $(TOSLIBC_SCRIPT_TARGET_LD) $(TOSLINK)

.PHONY: compiler
compiler: $(TOSLIBC_SCRIPT_TARGET_CC)

.PHONY: install-binutils
install-binutils: install-toslink install-linker-script			\
	install-binutils-symlink install-prg.ld-script

.PHONY: install-compiler
install-compiler: install-compiler-script install-compiler-alias

.PHONY: install-binutils-symlink
install-binutils-symlink:
	$(INSTALL) -d $(DESTDIR)$(binutilsbindir)
	@script/install-compiler-symlink -v $(binutilsbindir) $(BINUTILS_SYMLINKS)

.PHONY: install-compiler-alias
install-compiler-alias: install-compiler-script
	$(INSTALL) -d $(DESTDIR)$(compilerbindir)
	@script/install-compiler-symlink -v -a $(compilerbindir) $(COMPILER_ALIASES)

OTHER_CLEAN += $(TOSLINK) $(TOSLIBC_VERSION_SRC)
