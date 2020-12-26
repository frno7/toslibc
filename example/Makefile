# SPDX-License-Identifier: GPL-2.0

ifneq "x$(CROSS_COMPILE)" "x"

PRG_CFLAGS += $(DEP_CFLAGS) -march=68000 -fno-PIC -nostdlib		\
	-ffunction-sections -fdata-sections -isystem include/toslibc	\
	-D_TOSLIBC_SOURCE

PRG_LDFLAGS += --relocatable --gc-sections --strip-all --entry _start	\
	--script=script/prg.ld

PRG_VLINKFLAGS += -b ataritos -tos-fastload -e _start

EXAMPLE_SRC := $(wildcard example/*.c)
EXAMPLE_OBJ += $(patsubst %.c, %.o, $(EXAMPLE_SRC))

$(EXAMPLE_OBJ): %.o : %.c
	$(QUIET_CC)$(CROSS_COMPILE)gcc $(PRG_CFLAGS) -c -o $@ $<

example/HELLO.TOS.o: example/hello.o $(TOSLIBC)
	$(QUIET_LINK)$(CROSS_COMPILE)ld $(PRG_LDFLAGS) -o $@ $^

example/HELLO.TOS: example/HELLO.TOS.o
	$(QUIET_LINK)vlink $(PRG_VLINKFLAGS) -o $@ $^

endif
