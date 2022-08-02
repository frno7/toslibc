# SPDX-License-Identifier: GPL-2.0

ifneq "x$(CROSS_COMPILE)" "x"

EXAMPLE :=								\
	example/ALERT.PRG						\
	example/HELLO.TOS						\
	example/WINDOW.PRG						\
	example/XBRA.PRG

all: $(EXAMPLE)

PRG_CFLAGS += $(DEP_CFLAGS) -march=68000 -fno-PIC -nostdlib		\
	-ffunction-sections -fdata-sections -isystem include/toslibc	\
	-D_TOSLIBC_SOURCE

PRG_LDFLAGS += --relocatable --gc-sections --strip-all --entry _start	\
	--script=script/prg.ld

lowercase = $(shell echo "$(1)" | tr '[:upper:]' '[:lower:]')

EXAMPLE_SRC := $(foreach e,$(EXAMPLE),$(call lowercase,$(basename $(e))).c)
EXAMPLE_OBJ += $(patsubst %.c, %.o, $(EXAMPLE_SRC))

$(EXAMPLE_OBJ): %.o : %.c
	$(QUIET_CC)$(CROSS_COMPILE)gcc $(PRG_CFLAGS) -c -o $@ $<

define EXAMPLE_OBJ_target
$(1).o: $(call lowercase,$(basename $(1))).o $$(TOSLIBC)
endef

$(foreach e,$(EXAMPLE),$(eval $(call EXAMPLE_OBJ_target,$(e))))

$(patsubst %, %.o, $(EXAMPLE)):
	$(QUIET_LINK)$(CROSS_COMPILE)ld $(PRG_LDFLAGS) -o $@ $^

$(EXAMPLE): $(TOSLINK)

$(EXAMPLE): % : %.o
	$(QUIET_LINK)$(TOSLINK) -o $@ $<

endif
