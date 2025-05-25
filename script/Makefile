# SPDX-License-Identifier: GPL-2.0

TOSLIBC_SCRIPT_SUBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_VERSION := $(shell $(TOSLIBC_SCRIPT_SUBDIR)version)
TOSLIBC_VERSION_MINOR := $(shell echo '$(TOSLIBC_VERSION)' | sed 's/-.*$$//')
TOSLIBC_VERSION_MAJOR := $(shell echo '$(TOSLIBC_VERSION)' | sed 's/\..*$$//')

export TOSLIBC_VERSION_MINOR TOSLIBC_VERSION_MAJOR

TOSLIBC_SCRIPT_TARGET_CC = $(TOSLIBC_SCRIPT_SUBDIR)$(target)-cc
TOSLIBC_SCRIPT_TARGET_LD = $(TOSLIBC_SCRIPT_SUBDIR)$(target)-ld

$(TOSLIBC_SCRIPT_TARGET_CC): $(TOSLIBC_SCRIPT_SUBDIR)target-cc
	$(QUIET_GEN)$< $@

$(TOSLIBC_SCRIPT_TARGET_LD): $(TOSLIBC_SCRIPT_SUBDIR)target-ld
	$(QUIET_GEN)$< $@

OTHER_CLEAN += $(TOSLIBC_SCRIPT_TARGET_CC) $(TOSLIBC_SCRIPT_TARGET_LD)

.PHONY: install-prg.ld
install-prg.ld:
	$(INSTALL) -d -m 755 $(DESTDIR)$(ldscriptdir)
	$(INSTALL) -m 644 script/prg.ld $(DESTDIR)$(ldscriptdir)

.PHONY: install-compiler-script
install-compiler-script: $(TOSLIBC_SCRIPT_TARGET_CC) $(TOSLIBC_SCRIPT_TARGET_LD)
	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL) $^ $(DESTDIR)$(bindir)
