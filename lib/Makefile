# SPDX-License-Identifier: GPL-2.0

TOSLIBC_VERSION_SRC := lib/version.c

.PHONY: $(shell script/version --check $(TOSLIBC_VERSION_SRC))
$(TOSLIBC_VERSION_SRC):
	$(QUIET_GEN)script/version $@

TOSLIBC := toslibc.a

ifdef TARGET_CC
all: $(TOSLIBC)
endif

TOSLIBC_CFLAGS =  $(DEP_CFLAGS) -march=68000 -fno-PIC -nostdlib		\
	-ffunction-sections -fdata-sections				\
	-isystem include/toslibc -D_TOSLIBC_SOURCE

TOSLIBC_S_SRC =								\
	lib/crt0.S							\
	lib/bios.S							\
	lib/gemdos.S							\
	lib/xbios.S							\
	lib/xgemdos.S							\
	lib/__mulsi3.S

TOSLIBC_C_SRC =								\
	lib/aes.c							\
	lib/close.c							\
	lib/ctype.c							\
	lib/dprintf.c							\
	lib/errno.c							\
	lib/exit.c							\
	lib/free.c							\
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
	lib/xbra.c							\
	lib/__divsi3.c							\
	lib/__modsi3.c							\
	lib/__muldi3.c							\
	lib/__udivdi3.c							\
	lib/__udivmoddi4.c						\
	lib/__udivmodsi4.c						\
	lib/__udivsi3.c							\
	lib/__umoddi3.c

TOSLIBC_S_OBJ = $(TOSLIBC_S_SRC:%.S=%.o)
TOSLIBC_C_OBJ = $(TOSLIBC_C_SRC:%.c=%.o)

TOSLIBC_OBJ = $(TOSLIBC_S_OBJ) $(TOSLIBC_C_OBJ)

ALL_OBJ += $(TOSLIBC_OBJ)

lib/memcpy.o								\
lib/memset.o: TOSLIBC_CFLAGS += -fno-tree-loop-distribute-patterns

$(TOSLIBC_S_OBJ): %.o : %.S
	$(QUIET_CC)$(TARGET_CC) $(TOSLIBC_CFLAGS) -c -o $@ $<

$(TOSLIBC_C_OBJ): %.o : %.c
	$(QUIET_CC)$(TARGET_CC) $(TOSLIBC_CFLAGS) -c -o $@ $<

$(TOSLIBC): $(TOSLIBC_OBJ)
	$(QUIET_AR)$(TARGET_AR) rcs $@ $(TOSLIBC_OBJ)
