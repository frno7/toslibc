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

TOSLIBC_PC = $(TOSLIBC_SCRIPT_SUBDIR)toslibc.pc

$(TOSLIBC_PC): $(TOSLIBC_SCRIPT_SUBDIR)pkg
	$(QUIET_GEN)$< $@

TOSLIBC_SCRIPT_PRG_LD = $(TOSLIBC_SCRIPT_SUBDIR)prg.ld

$(TOSLIBC_SCRIPT_PRG_LD): $(TOSLIBC_SCRIPT_SUBDIR)target-prg.ld
	$(QUIET_GEN)$< $@

.PHONY: script
script: $(TOSLIBC_SCRIPT_TARGET_CC) $(TOSLIBC_SCRIPT_TARGET_LD)		\
	$(TOSLIBC_PC) $(TOSLIBC_SCRIPT_PRG_LD)

OTHER_CLEAN += $(TOSLIBC_SCRIPT_TARGET_CC) $(TOSLIBC_SCRIPT_TARGET_LD)	\
	$(TOSLIBC_PC) $(TOSLIBC_SCRIPT_PRG_LD)

.PHONY: install-linker-script
install-linker-script: $(TOSLIBC_SCRIPT_TARGET_LD)
	$(INSTALL) -d -m 755 $(DESTDIR)$(binutilsbindir)
	$(INSTALL) $< $(DESTDIR)$(binutilsbindir)

.PHONY: install-compiler-script
install-compiler-script: $(TOSLIBC_SCRIPT_TARGET_CC)
	$(INSTALL) -d $(DESTDIR)$(compilerbindir)
	$(INSTALL) $< $(DESTDIR)$(compilerbindir)

.PHONY: install-pkg-config
install-pkg-config: $(TOSLIBC_PC)
	$(INSTALL) -d -m 755 $(DESTDIR)$(pkgdir)
	$(INSTALL) -m 644 $^ $(DESTDIR)$(pkgdir)

.PHONY: install-prg.ld-script
install-prg.ld-script: $(TOSLIBC_SCRIPT_PRG_LD)
	$(INSTALL) -d -m 755 $(DESTDIR)$(ldscriptdir)
	$(INSTALL) -m 644 $< $(DESTDIR)$(ldscriptdir)
