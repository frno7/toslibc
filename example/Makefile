# SPDX-License-Identifier: GPL-2.0

EXAMPLE_PRG :=								\
	example/ALERT.PRG						\
	example/HELLO.TOS						\
	example/WINDOW.PRG						\
	example/XBRA.PRG

ifdef TARGET_CC
all: $(EXAMPLE_PRG)
endif

EXAMPLE_CFLAGS += $(BASIC_CFLAGS) -march=68000 -fno-PIC -nostdlib	\
	-ffunction-sections -fdata-sections -isystem include/toslibc	\
	-D_TOSLIBC_SOURCE $(TARGET_CFLAGS)

EXAMPLE_LDFLAGS += --relocatable --gc-sections --strip-all		\
	--entry _start --script=script/prg.ld $(TARGET_LDFLAGS)

lowercase = $(shell echo "$(1)" | tr '[:upper:]' '[:lower:]')

EXAMPLE_SRC := $(foreach e,$(EXAMPLE_PRG),$(call lowercase,$(basename $(e))).c)
EXAMPLE_OBJ += $(EXAMPLE_SRC:%.c=%.o)

ALL_OBJ += $(EXAMPLE_OBJ) $(EXAMPLE_PRG:%=%.o)

$(EXAMPLE_OBJ): %.o : %.c
	$(QUIET_CC)$(TARGET_CC) $(EXAMPLE_CFLAGS) -c -o $@ $<

define EXAMPLE_OBJ_target
$(1).o: $(call lowercase,$(basename $(1))).o $$(TOSLIBC)
endef

$(foreach e,$(EXAMPLE_PRG),$(eval $(call EXAMPLE_OBJ_target,$(e))))

$(EXAMPLE_PRG:%=%.o):
	$(QUIET_LINK)$(TARGET_LD) $(EXAMPLE_LDFLAGS) -o $@ $^

$(EXAMPLE_PRG): $(TOSLINK)

$(EXAMPLE_PRG): % : %.o
	$(QUIET_LINK)$(TOSLINK) -o $@ $<
