# SPDX-License-Identifier: GPL-2.0

TOSLIBC_lib_dir := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC := $(TOSLIBC_lib_dir)libc.a

TOSLIBC_LIB_CFLAGS = $(BASIC_CFLAGS) -march=68000 -fno-PIC		\
	-nostdlib -ffunction-sections -fdata-sections			\
	-isystem $(TOSLIBC_lib_dir)../include/toslibc			\
	-I$(TOSLIBC_lib_dir)../include					\
	-D_TOSLIBC_SOURCE $(TARGET_CFLAGS)

TOSLIBC_LIB_SRC = $(addprefix $(TOSLIBC_lib_dir),			\
	crt0.S								\
	sndhrt0.S							\
	bios.S								\
	gemdos.S							\
	xbios.S								\
	xgemdos.S							\
	__mulsi3.S							\
	aes.c								\
	close.c								\
	cookie.c							\
	ctype.c								\
	dprintf.c							\
	errno.c								\
	exit.c								\
	fopen.c								\
	fpgnulib.c							\
	free.c								\
	lseek.c								\
	malloc.c							\
	memcpy.c							\
	memmove.c							\
	memset.c							\
	mkdir.c								\
	open.c								\
	printf.c							\
	ptermres.c							\
	putchar.c							\
	puts.c								\
	read.c								\
	realloc.c							\
	rmdir.c								\
	snprintf.c							\
	start.c								\
	stat.c								\
	strchr.c							\
	strcmp.c							\
	strerror.c							\
	strlen.c							\
	strncmp.c							\
	strncpy.c							\
	strntoimax.c							\
	strntoumax.c							\
	strsplit.c							\
	strtoimax.c							\
	strtol.c							\
	strtoll.c							\
	strtoul.c							\
	strtoull.c							\
	strtoumax.c							\
	supexecarg.c							\
	system-variable.c						\
	unlink.c							\
	vdi.c								\
	vdprintf.c							\
	vprintf.c							\
	vsnprintf.c							\
	write.c								\
	xbra.c								\
	__divsi3.c							\
	__modsi3.c							\
	__muldi3.c							\
	__udivdi3.c							\
	__udivmoddi4.c							\
	__udivmodsi4.c							\
	__udivsi3.c							\
	__umoddi3.c)

$(TOSLIBC_lib_dir)memcpy.o						\
$(TOSLIBC_lib_dir)memset.o:						\
	TOSLIBC_LIB_CFLAGS += -fno-tree-loop-distribute-patterns

define TOSLIBC_OBJ_target
TOSLIBC_LIB_OBJ += $(basename $(1)).o
$(basename $(1)).o: $(1)
	$$(QUIET_CC)$$(TARGET_CC) $$(TOSLIBC_LIB_CFLAGS) -c -o $$@ $$<
endef

$(foreach f,$(TOSLIBC_LIB_SRC),$(eval $(call TOSLIBC_OBJ_target,$(f))))

ALL_OBJ += $(TOSLIBC_LIB_OBJ)

$(TOSLIBC): $(TOSLIBC_LIB_OBJ)
	$(QUIET_AR)$(TARGET_AR) rcs $@ $^

OTHER_CLEAN += $(TOSLIBC)

ifeq (lib/,$(TOSLIBC_lib_dir))

.PHONY: lib
lib: $(TOSLIBC)

.PHONY: install-lib
install-lib: install-lib-include install-lib-static install-pkg-config

.PHONY: install-lib-include
install-lib-include:
	$(INSTALL) -d -m 755 $(DESTDIR)$(includedir)
	$(INSTALL) -d -m 755 $(DESTDIR)$(includedir)/asm
	$(INSTALL) -d -m 755 $(DESTDIR)$(includedir)/asm/snd
	$(INSTALL) -d -m 755 $(DESTDIR)$(includedir)/sys
	$(INSTALL) -d -m 755 $(DESTDIR)$(includedir)/tos
	$(INSTALL) -m 644 $(wildcard include/toslibc/*.h) $(DESTDIR)$(includedir)
	$(INSTALL) -m 644 $(wildcard include/toslibc/asm/*.h) $(DESTDIR)$(includedir)/asm
	$(INSTALL) -m 644 $(wildcard include/toslibc/asm/snd/*.h) $(DESTDIR)$(includedir)/asm/snd
	$(INSTALL) -m 644 $(wildcard include/toslibc/sys/*.h) $(DESTDIR)$(includedir)/sys
	$(INSTALL) -m 644 $(wildcard include/toslibc/tos/*.h) $(DESTDIR)$(includedir)/tos

.PHONY: install-lib-static
install-lib-static: $(TOSLIBC)
	$(INSTALL) -d -m 755 $(DESTDIR)$(libdir)
	$(INSTALL) $(TOSLIBC) $(DESTDIR)$(libdir)

endif
