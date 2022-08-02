# SPDX-License-Identifier: GPL-2.0

TOSLINK := tool/toslink

TOSLINK_CFLAGS = $(BASIC_CFLAGS) $(S_CFLAGS) $(CFLAGS)

TOSLIBC_VERSION_SRC := tool/version.c

.PHONY: $(shell script/version --check $(TOSLIBC_VERSION_SRC))
$(TOSLIBC_VERSION_SRC):
	$(QUIET_GEN)script/version $@

TOSLINK_VERSION_OBJ := $(dir $(TOSLINK))version.o

TOSLINK_OBJ = $(TOSLINK).o $(addprefix $(dir $(TOSLINK)),		\
	elf32.o								\
	file.o								\
	memory.o							\
	print.o)

ALL_OBJ += $(TOSLINK_VERSION_OBJ) $(TOSLINK_OBJ)

$(TOSLINK_VERSION_OBJ): $(TOSLIBC_VERSION_SRC)
	$(QUIET_CC)$(CC) $(TOSLINK_CFLAGS) -c -o $@ $<

$(TOSLINK_OBJ): %.o: %.c
	$(QUIET_CC)$(CC) $(TOSLINK_CFLAGS) -c -o $@ $<

$(TOSLINK): $(TOSLINK_OBJ) $(TOSLINK_VERSION_OBJ)
	$(QUIET_LINK)$(CC) $(TOSLINK_CFLAGS) -o $@ $^
