# SPDX-License-Identifier: GPL-2.0

TOSLIBC_script_dir := $(dir $(lastword $(MAKEFILE_LIST)))

TOSLIBC_VERSION := $(shell $(TOSLIBC_script_dir)version)
TOSLIBC_VERSION_MINOR := $(shell echo '$(TOSLIBC_VERSION)' | sed 's/-.*$$//')
TOSLIBC_VERSION_MAJOR := $(shell echo '$(TOSLIBC_VERSION)' | sed 's/\..*$$//')

export TOSLIBC_VERSION_MINOR TOSLIBC_VERSION_MAJOR

TOSLIBC_SCRIPT_TARGET_CC = $(TOSLIBC_script_dir)$(target)-cc
TOSLIBC_SCRIPT_TARGET_LD = $(TOSLIBC_script_dir)$(target)-ld

$(TOSLIBC_SCRIPT_TARGET_CC): $(TOSLIBC_script_dir)target-cc
	$(QUIET_GEN)$< $@

$(TOSLIBC_SCRIPT_TARGET_LD): $(TOSLIBC_script_dir)target-ld
	$(QUIET_GEN)$< $@

TOSLIBC_PC = $(TOSLIBC_script_dir)toslibc.pc

$(TOSLIBC_PC): $(TOSLIBC_script_dir)pkg
	$(QUIET_GEN)$< $@

TOSLIBC_SCRIPT_PRG_LD = $(TOSLIBC_script_dir)prg.ld

$(TOSLIBC_SCRIPT_PRG_LD): $(TOSLIBC_script_dir)target-prg.ld
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
	$(INSTALL) -d -m 755 $(DESTDIR)$(pkgconfigdir)
	$(INSTALL) -m 644 $^ $(DESTDIR)$(pkgconfigdir)

.PHONY: install-prg.ld-script
install-prg.ld-script: $(TOSLIBC_SCRIPT_PRG_LD)
	$(INSTALL) -d -m 755 $(DESTDIR)$(ldscriptdir)
	$(INSTALL) -m 644 $< $(DESTDIR)$(ldscriptdir)
