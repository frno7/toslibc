# SPDX-License-Identifier: GPL-2.0

TOSLIBC_EXAMPLE_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_EXAMPLE_PRG = $(addprefix $(TOSLIBC_EXAMPLE_SUBDIR),		\
	ALERT.PRG							\
	COOKIE.TOS							\
	HELLO.TOS							\
	WINDOW.PRG							\
	XBRA.PRG)

.PHONY: example
example: $(TOSLIBC_EXAMPLE_PRG)

TOSLIBC_EXAMPLE_SRC := $(foreach e,$(TOSLIBC_EXAMPLE_PRG),		\
	$(call lowercase,$(basename $(e))).c)
TOSLIBC_EXAMPLE_OBJ = $(TOSLIBC_EXAMPLE_SRC:%.c=%.o)

ALL_OBJ += $(TOSLIBC_EXAMPLE_OBJ) $(TOSLIBC_EXAMPLE_PRG:%=%.o)

$(TOSLIBC_EXAMPLE_OBJ): %.o: %.c
	$(QUIET_CC)$(TARGET_CC) $(TOSLIBC_PROGRAM_CFLAGS) -c -o $@ $<

define TOSLIBC_EXAMPLE_OBJ_target
$(1).o: $(call lowercase,$(basename $(1))).o $$(TOSLIBC)
endef

$(foreach e,$(TOSLIBC_EXAMPLE_PRG),					\
	$(eval $(call TOSLIBC_EXAMPLE_OBJ_target,$(e))))

$(TOSLIBC_EXAMPLE_PRG:%=%.o):
	$(QUIET_LINK)$(TARGET_LD) $(TOSLIBC_PROGRAM_LDFLAGS) -o $@ $^

$(TOSLIBC_EXAMPLE_PRG): $(TOSLINK)

$(TOSLIBC_EXAMPLE_PRG): %: %.o
	$(QUIET_LINK)$(TOSLINK) -o $@ $<

OTHER_CLEAN += $(TOSLIBC_EXAMPLE_PRG)
