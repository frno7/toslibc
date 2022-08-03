# SPDX-License-Identifier: GPL-2.0

TOSLIBC_TOOL_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLINK := $(TOSLIBC_TOOL_SUBDIR)toslink

TOSLINK_CFLAGS = $(TOSLIBC_CFLAGS) $(S_CFLAGS) $(CFLAGS)

TOSLIBC_VERSION_SRC := $(TOSLIBC_TOOL_SUBDIR)version.c

.PHONY: $(shell script/version --check $(TOSLIBC_VERSION_SRC))
$(TOSLIBC_VERSION_SRC):
	$(QUIET_GEN)script/version $@

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

OTHER_CLEAN += $(TOSLINK) $(TOSLIBC_VERSION_SRC)
