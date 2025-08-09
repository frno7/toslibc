# SPDX-License-Identifier: GPL-2.0

TOSLIBC_example_dir := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_EXAMPLE_PRG = $(addprefix $(TOSLIBC_example_dir),		\
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
TOSLIBC_EXAMPLE_ELF = $(TOSLIBC_EXAMPLE_PRG:%=%.elf)

ALL_OBJ += $(TOSLIBC_EXAMPLE_OBJ) $(TOSLIBC_EXAMPLE_ELF)

$(TOSLIBC_EXAMPLE_OBJ): %.o: %.c
	$(QUIET_CC)$(TARGET_CC) $(TOSLIBC_PROGRAM_CFLAGS) -c -o $@ $<

define TOSLIBC_EXAMPLE_OBJ_target
$(1).elf: $(call lowercase,$(basename $(1))).o				\
	$$(TOSLIBC) $$(TOSLIBC_SCRIPT_PRG_LD) $$(TOSLIBC_SCRIPT_SNDH_LD)
endef

$(foreach e,$(TOSLIBC_EXAMPLE_PRG),					\
	$(eval $(call TOSLIBC_EXAMPLE_OBJ_target,$(e))))

$(filter-out %.SNDH.elf,$(TOSLIBC_EXAMPLE_ELF)):			\
	TOSLIBC_EXAMPLE_LDFLAGS += $(TOSLIBC_PROGRAM_LDFLAGS)

$(filter %.SNDH.elf,$(TOSLIBC_EXAMPLE_ELF)):				\
	TOSLIBC_EXAMPLE_LDFLAGS += $(TOSLIBC_SNDH_LDFLAGS)

$(TOSLIBC_EXAMPLE_ELF):
	$(QUIET_LINK)$(TARGET_LD) $(TOSLIBC_EXAMPLE_LDFLAGS) -o $@ $< $(TOSLIBC)

$(TOSLIBC_EXAMPLE_PRG): $(TOSLINK)

$(TOSLIBC_EXAMPLE_PRG): %: %.elf
	$(QUIET_LINK)$(TOSLINK) -o $@ $<

TOSLIBC_EXAMPLE_MAKEFILE = $(TOSLIBC_example_dir)Makefile.example

OTHER_CLEAN += $(TOSLIBC_EXAMPLE_PRG)

ifeq (example/,$(TOSLIBC_example_dir))

.PHONY: install-example-src
install-example-src: $(TOSLIBC_EXAMPLE_SRC)
	$(INSTALL) -d $(DESTDIR)$(exampledir)
	$(INSTALL) -m 644 $^ $(DESTDIR)$(exampledir)

.PHONY: install-example-makefile
install-example-makefile: $(TOSLIBC_EXAMPLE_MAKEFILE)
	mkdir tmp && cp $^ tmp/Makefile
	$(INSTALL) -d $(DESTDIR)$(exampledir)
	$(INSTALL) -m 644 tmp/Makefile $(DESTDIR)$(exampledir)
	rm tmp/Makefile && rmdir tmp

.PHONY: install-example-prg
install-example-prg: $(TOSLIBC_EXAMPLE_PRG)
	$(INSTALL) -d $(DESTDIR)$(exampledir)
	$(INSTALL) $^ $(DESTDIR)$(exampledir)

.PHONY: install-example
install-example: install-example-src install-example-makefile install-example-prg

endif
