# SPDX-License-Identifier: GPL-2.0

TOSLIBC_TEST_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_TEST_SUITE_PRG = $(addprefix $(TOSLIBC_TEST_SUBDIR), SUITE.TOS)

TOSLIBC_TEST_SUITE_H = $(TOSLIBC_TEST_SUBDIR)../include/test/suite.h

.PHONY: test
test: $(TOSLIBC_TEST_SUITE_PRG)

TOSLIBC_TEST_SRC := $(addprefix $(TOSLIBC_TEST_SUBDIR),			\
	fopen.c								\
	fread.c								\
	fwrite.c							\
	lseek.c								\
	open.c								\
	rmdir.c								\
	stat.c								\
	strchr.c							\
	strcmp.c							\
	unlink.c)
TOSLIBC_TEST_OBJ = $(TOSLIBC_TEST_SRC:%.c=%.o)

TOSLIBC_TEST_SUITE_SRC := $(foreach e,$(TOSLIBC_TEST_SUITE_PRG),	\
	$(call lowercase,$(basename $(e))).c)
TOSLIBC_TEST_SUITE_OBJ = $(TOSLIBC_TEST_SUITE_SRC:%.c=%.o)

ALL_OBJ += $(TOSLIBC_TEST_OBJ)						\
	   $(TOSLIBC_TEST_SUITE_OBJ)					\
	   $(TOSLIBC_TEST_SUITE_PRG:%=%.o)

$(TOSLIBC_TEST_SUITE_H): $(TOSLIBC_TEST_SRC)
	$(QUIET_GEN)$(TOSLIBC_TEST_SUBDIR)../script/test-suite -o $@ $^

$(TOSLIBC_TEST_SUITE_OBJ): $(TOSLIBC_TEST_SUITE_H)

$(TOSLIBC_TEST_OBJ) $(TOSLIBC_TEST_SUITE_OBJ): %.o: %.c
	$(QUIET_CC)$(TARGET_CC) $(TOSLIBC_PROGRAM_CFLAGS) -c -o $@ $<

define TOSLIBC_TEST_OBJ_target
$(1).o: $(call lowercase,$(basename $(1))).o $$(TOSLIBC) $$(TOSLIBC_SCRIPT_PRG_LD)
endef

$(TOSLIBC_TEST_SUITE_PRG:%=%.o): $(TOSLIBC_TEST_OBJ)
$(foreach e,$(TOSLIBC_TEST_SUITE_PRG),					\
	$(eval $(call TOSLIBC_TEST_OBJ_target,$(e))))

$(TOSLIBC_TEST_SUITE_PRG:%=%.o):
	$(QUIET_LINK)$(TARGET_LD) $(TOSLIBC_PROGRAM_LDFLAGS) -o $@	\
		$(TOSLIBC_TEST_OBJ) $(TOSLIBC_TEST_SUITE_OBJ) $(TOSLIBC)

$(TOSLIBC_TEST_SUITE_PRG): $(TOSLINK)

$(TOSLIBC_TEST_SUITE_PRG): %: %.o
	$(QUIET_LINK)$(TOSLINK) -o $@ $<

TOSLIBC_TEST_SUITE_DATA = test/test.txt

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

OTHER_CLEAN += $(TOSLIBC_TEST_SUITE_H) $(TOSLIBC_TEST_SUITE_PRG)
