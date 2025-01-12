# SPDX-License-Identifier: GPL-2.0

TOSLIBC_EXAMPLE_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_EXAMPLE_PRG = $(addprefix $(TOSLIBC_EXAMPLE_SUBDIR),		\
	ALERT.PRG							\
	COOKIE.TOS							\
	HELLO.TOS							\
	WINDOW.PRG							\
	XBRA.PRG)

TOSLIBC_EXAMPLE_CFLAGS = $(BASIC_CFLAGS) -march=68000 -fno-PIC		\
	-nostdlib -ffunction-sections -fdata-sections			\
	-isystem $(TOSLIBC_LIB_SUBDIR)../include/toslibc		\
	-I$(TOSLIBC_LIB_SUBDIR)../include				\
	-D_TOSLIBC_SOURCE $(TARGET_CFLAGS)

TOSLIBC_EXAMPLE_LDFLAGS = --relocatable --gc-sections --strip-all	\
	--entry _start --script=script/prg.ld $(TARGET_LDFLAGS)

lowercase ?= $(shell echo "$(1)" | tr '[:upper:]' '[:lower:]')

TOSLIBC_EXAMPLE_SRC := $(foreach e,$(TOSLIBC_EXAMPLE_PRG),		\
	$(call lowercase,$(basename $(e))).c)
TOSLIBC_EXAMPLE_OBJ = $(TOSLIBC_EXAMPLE_SRC:%.c=%.o)

ALL_OBJ += $(TOSLIBC_EXAMPLE_OBJ) $(TOSLIBC_EXAMPLE_PRG:%=%.o)

$(TOSLIBC_EXAMPLE_OBJ): %.o: %.c
	$(QUIET_CC)$(TARGET_CC) $(TOSLIBC_EXAMPLE_CFLAGS) -c -o $@ $<

define TOSLIBC_EXAMPLE_OBJ_target
$(1).o: $(call lowercase,$(basename $(1))).o $$(TOSLIBC)
endef

$(foreach e,$(TOSLIBC_EXAMPLE_PRG),					\
	$(eval $(call TOSLIBC_EXAMPLE_OBJ_target,$(e))))

$(TOSLIBC_EXAMPLE_PRG:%=%.o):
	$(QUIET_LINK)$(TARGET_LD) $(TOSLIBC_EXAMPLE_LDFLAGS) -o $@ $^

$(TOSLIBC_EXAMPLE_PRG): $(TOSLINK)

$(TOSLIBC_EXAMPLE_PRG): %: %.o
	$(QUIET_LINK)$(TOSLINK) -o $@ $<

OTHER_CLEAN += $(TOSLIBC_EXAMPLE_PRG)
