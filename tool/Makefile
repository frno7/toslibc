# SPDX-License-Identifier: GPL-2.0

TOSLINK := tool/toslink

all: $(TOSLINK)

TOSLINK_CFLAGS += $(ALL_CFLAGS)

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
