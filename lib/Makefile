# SPDX-License-Identifier: GPL-2.0

TOSLIBC_VERSION_SRC := lib/version.c

.PHONY: $(shell script/version --check $(TOSLIBC_VERSION_SRC))
$(TOSLIBC_VERSION_SRC):
	$(QUIET_GEN)script/version $@

TOSLIBC := lib/toslibc.a

TOSLIBC_CFLAGS = $(BASIC_CFLAGS) -march=68000 -fno-PIC -nostdlib	\
	-ffunction-sections -fdata-sections				\
	-isystem include/toslibc -D_TOSLIBC_SOURCE $(TARGET_CFLAGS)

TOSLIBC_SRC =								\
	lib/aes.c							\
	lib/bios.S							\
	lib/close.c							\
	lib/crt0.S							\
	lib/ctype.c							\
	lib/dprintf.c							\
	lib/errno.c							\
	lib/exit.c							\
	lib/free.c							\
	lib/gemdos.S							\
	lib/lseek.c							\
	lib/malloc.c							\
	lib/memcpy.c							\
	lib/memset.c							\
	lib/open.c							\
	lib/printf.c							\
	lib/ptermres.c							\
	lib/putchar.c							\
	lib/puts.c							\
	lib/read.c							\
	lib/realloc.c							\
	lib/snprintf.c							\
	lib/start.c							\
	lib/strcmp.c							\
	lib/strerror.c							\
	lib/strlen.c							\
	lib/strncmp.c							\
	lib/strncpy.c							\
	lib/strntoimax.c						\
	lib/strntoumax.c						\
	lib/strsplit.c							\
	lib/strtoimax.c							\
	lib/strtol.c							\
	lib/strtoll.c							\
	lib/strtoul.c							\
	lib/strtoull.c							\
	lib/strtoumax.c							\
	lib/supexecarg.c						\
	lib/system-variable.c						\
	lib/vdi.c							\
	lib/vdprintf.c							\
	lib/vprintf.c							\
	lib/vsnprintf.c							\
	lib/write.c							\
	lib/xbios.S							\
	lib/xbra.c							\
	lib/xgemdos.S							\
	lib/__divsi3.c							\
	lib/__modsi3.c							\
	lib/__muldi3.c							\
	lib/__mulsi3.S							\
	lib/__udivdi3.c							\
	lib/__udivmoddi4.c						\
	lib/__udivmodsi4.c						\
	lib/__udivsi3.c							\
	lib/__umoddi3.c

lib/memcpy.o								\
lib/memset.o: TOSLIBC_CFLAGS += -fno-tree-loop-distribute-patterns

define TOSLIBC_OBJ_target
TOSLIBC_OBJ += $(basename $(1)).o
$(basename $(1)).o: $(1)
	$$(QUIET_CC)$$(TARGET_CC) $$(TOSLIBC_CFLAGS) -c -o $$@ $$<
endef

$(foreach f,$(TOSLIBC_SRC),$(eval $(call TOSLIBC_OBJ_target,$(f))))

ALL_OBJ += $(TOSLIBC_OBJ)

$(TOSLIBC): $(TOSLIBC_OBJ)
	$(QUIET_AR)$(TARGET_AR) rcs $@ $(TOSLIBC_OBJ)
