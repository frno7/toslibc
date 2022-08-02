# SPDX-License-Identifier: GPL-2.0

TOSLIBC_LIB_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_VERSION_SRC := $(TOSLIBC_LIB_SUBDIR)version.c

.PHONY: $(shell script/version --check $(TOSLIBC_VERSION_SRC))
$(TOSLIBC_VERSION_SRC):
	$(QUIET_GEN)script/version $@

TOSLIBC := $(TOSLIBC_LIB_SUBDIR)toslibc.a

TOSLIBC_CFLAGS = $(BASIC_CFLAGS) -march=68000 -fno-PIC -nostdlib	\
	-ffunction-sections -fdata-sections				\
	-isystem include/toslibc -D_TOSLIBC_SOURCE $(TARGET_CFLAGS)

TOSLIBC_SRC = $(addprefix $(TOSLIBC_LIB_SUBDIR),			\
	aes.c								\
	bios.S								\
	close.c								\
	crt0.S								\
	ctype.c								\
	dprintf.c							\
	errno.c								\
	exit.c								\
	free.c								\
	gemdos.S							\
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
	xbios.S								\
	xbra.c								\
	xgemdos.S							\
	__divsi3.c							\
	__modsi3.c							\
	__muldi3.c							\
	__mulsi3.S							\
	__udivdi3.c							\
	__udivmoddi4.c							\
	__udivmodsi4.c							\
	__udivsi3.c							\
	__umoddi3.c)

$(TOSLIBC_LIB_SUBDIR)memcpy.o						\
$(TOSLIBC_LIB_SUBDIR)memset.o:						\
	TOSLIBC_CFLAGS += -fno-tree-loop-distribute-patterns

define TOSLIBC_OBJ_target
TOSLIBC_OBJ += $(basename $(1)).o
$(basename $(1)).o: $(1)
	$$(QUIET_CC)$$(TARGET_CC) $$(TOSLIBC_CFLAGS) -c -o $$@ $$<
endef

$(foreach f,$(TOSLIBC_SRC),$(eval $(call TOSLIBC_OBJ_target,$(f))))

ALL_OBJ += $(TOSLIBC_OBJ)

$(TOSLIBC): $(TOSLIBC_OBJ)
	$(QUIET_AR)$(TARGET_AR) rcs $@ $(TOSLIBC_OBJ)
