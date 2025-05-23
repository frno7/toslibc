# SPDX-License-Identifier: GPL-2.0

TOSLIBC_LIB_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC := $(TOSLIBC_LIB_SUBDIR)toslibc.a

TOSLIBC_LIB_CFLAGS = $(BASIC_CFLAGS) -march=68000 -fno-PIC		\
	-nostdlib -ffunction-sections -fdata-sections			\
	-isystem $(TOSLIBC_LIB_SUBDIR)../include/toslibc		\
	-I$(TOSLIBC_LIB_SUBDIR)../include				\
	-D_TOSLIBC_SOURCE $(TARGET_CFLAGS)

TOSLIBC_LIB_SRC = $(addprefix $(TOSLIBC_LIB_SUBDIR),			\
	crt0.S								\
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
	free.c								\
	lseek.c								\
	malloc.c							\
	memcpy.c							\
	memset.c							\
	open.c								\
	printf.c							\
	ptermres.c							\
	putchar.c							\
	puts.c								\
	read.c								\
	realloc.c							\
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

$(TOSLIBC_LIB_SUBDIR)memcpy.o						\
$(TOSLIBC_LIB_SUBDIR)memset.o:						\
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
