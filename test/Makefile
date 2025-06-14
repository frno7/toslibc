# SPDX-License-Identifier: GPL-2.0

TOSLIBC_test_dir := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_TEST_SUITE_PRG = $(addprefix $(TOSLIBC_test_dir), SUITE.TOS)

TOSLIBC_TEST_SUITE_H = $(call parentdir, $(TOSLIBC_test_dir))include/test/suite.h

.PHONY: test
test: $(TOSLIBC_TEST_SUITE_PRG)

TOSLIBC_TEST_SUITE_SRC :=						\
	$(call lowercase, $(basename $(TOSLIBC_TEST_SUITE_PRG))).c
TOSLIBC_TEST_SUITE_OBJ = $(TOSLIBC_TEST_SUITE_SRC:%.c=%.o)
TOSLIBC_TEST_SUITE_ELF = $(TOSLIBC_TEST_SUITE_PRG:%=%.elf)

TOSLIBC_TEST_SRC := $(filter-out $(TOSLIBC_TEST_SUITE_SRC),		\
	$(wildcard $(TOSLIBC_test_dir)*.c))
TOSLIBC_TEST_OBJ = $(TOSLIBC_TEST_SRC:%.c=%.o)

ALL_OBJ += $(TOSLIBC_TEST_OBJ)						\
	   $(TOSLIBC_TEST_SUITE_OBJ)					\
	   $(TOSLIBC_TEST_SUITE_ELF)

TOSLIBC_TEST_SUITE_SCRIPT :=						\
	$(call parentdir, $(TOSLIBC_test_dir))script/test-suite

$(TOSLIBC_TEST_SUITE_H): $(TOSLIBC_TEST_SRC)
	$(QUIET_GEN)$(TOSLIBC_TEST_SUITE_SCRIPT) -o $@ $^

$(TOSLIBC_TEST_SUITE_OBJ): $(TOSLIBC_TEST_SUITE_H)

$(TOSLIBC_TEST_OBJ) $(TOSLIBC_TEST_SUITE_OBJ): %.o: %.c
	$(QUIET_CC)$(TARGET_CC) $(TOSLIBC_PROGRAM_CFLAGS) -c -o $@ $<

define TOSLIBC_TEST_OBJ_target
$(1).elf: $(call lowercase,$(basename $(1))).o $$(TOSLIBC) $$(TOSLIBC_SCRIPT_PRG_LD)
endef

$(TOSLIBC_TEST_SUITE_ELF): $(TOSLIBC_TEST_OBJ)
$(foreach e,$(TOSLIBC_TEST_SUITE_PRG),					\
	$(eval $(call TOSLIBC_TEST_OBJ_target,$(e))))

$(TOSLIBC_TEST_SUITE_ELF):
	$(QUIET_LINK)$(TARGET_LD) $(TOSLIBC_PROGRAM_LDFLAGS) -o $@	\
		$(TOSLIBC_TEST_OBJ) $(TOSLIBC_TEST_SUITE_OBJ) $(TOSLIBC)

$(TOSLIBC_TEST_SUITE_PRG): $(TOSLINK)

$(TOSLIBC_TEST_SUITE_PRG): %: %.elf
	$(QUIET_LINK)$(TOSLINK) -o $@ $<

TOSLIBC_TEST_SUITE_DATA = test/test.txt

OTHER_CLEAN += $(TOSLIBC_TEST_SUITE_H) $(TOSLIBC_TEST_SUITE_PRG)

ifeq (test/,$(TOSLIBC_test_dir))

.PHONY: install-test-data
install-test-data: $(TOSLIBC_TEST_SUITE_DATA)
	$(INSTALL) -d $(DESTDIR)$(testdir)
	$(INSTALL) -m 644 $^ $(DESTDIR)$(testdir)

.PHONY: install-test-prg
install-test-prg: $(TOSLIBC_TEST_SUITE_PRG)
	$(INSTALL) -d $(DESTDIR)$(testdir)
	$(INSTALL) $^ $(DESTDIR)$(testdir)

.PHONY: install-test
install-test: install-test-data install-test-prg

endif
