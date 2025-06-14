# SPDX-License-Identifier: GPL-2.0
#
# See the file INSTALL for installation instructions.

target = m68k-atari-tos-gnu
prefix = $(HOME)/.local/usr/$(target)
datarootdir = $(prefix)/share
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
binutilsbindir = $(bindir)
compilerbindir = $(bindir)
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1
includedir = $(prefix)/include
libdir = $(exec_prefix)/lib
pkgconfigdir = $(libdir)/pkgconfig
ldscriptdir = $(libdir)/script
testdir = $(datarootdir)/toslibc/test
exampledir = $(datarootdir)/toslibc/example

target_prefix = $(target)-

export prefix binutilsbindir compilerbindir				\
	includedir libdir ldscriptdir target_prefix

CFLAGS = -g

ifdef TARGET_COMPILE
TARGET_CC = $(TARGET_COMPILE)gcc
TARGET_LD = $(TARGET_COMPILE)ld
TARGET_AR = $(TARGET_COMPILE)ar
endif

TARGET_CFLAGS = $(CFLAGS)

export TARGET_CC TARGET_LD

COMPILER_ALIASES = cc:gcc
BINUTILS_SYMLINKS = ar as nm objcopy objdump pkg-config			\
		    ranlib readelf size strings strip

INSTALL = install

ifeq (1,$(S))
S_CFLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined	\
	  -fsanitize-address-use-after-scope
endif

DEP_CFLAGS = -Wp,-MD,$(@D)/$(@F).d -MT $(@D)/$(@F)
BASIC_CFLAGS = -O2 -Wall -D_GNU_SOURCE $(DEP_CFLAGS)

.PHONY: all
all:

parentdir ?= $(dir ${1:/=})
lowercase ?= $(shell echo "$(1)" | tr '[:upper:]' '[:lower:]')

include script/Makefile
include lib/Makefile
include tool/Makefile

TOSLIBC_PROGRAM_BASIC_CFLAGS = -march=68000 -fno-PIC -nostdlib
TOSLIBC_PROGRAM_BASIC_LDFLAGS = --relocatable --gc-sections --strip-all --entry _start

export TOSLIBC_PROGRAM_BASIC_CFLAGS TOSLIBC_PROGRAM_BASIC_LDFLAGS

TOSLIBC_PROGRAM_CFLAGS = $(BASIC_CFLAGS) $(TOSLIBC_PROGRAM_BASIC_CFLAGS)\
	-ffunction-sections -fdata-sections				\
	-isystem $(TOSLIBC_lib_dir)../include/toslibc			\
	-I$(TOSLIBC_lib_dir)../include					\
	-D_TOSLIBC_SOURCE $(TARGET_CFLAGS)

TOSLIBC_PROGRAM_LDFLAGS = $(TOSLIBC_PROGRAM_BASIC_LDFLAGS)		\
	-nostdlib -Llib --script=$(TOSLIBC_SCRIPT_PRG_LD) $(TARGET_LDFLAGS)

include example/Makefile
include test/Makefile

all: $(TOSLINK)

ifdef TARGET_CC
all: $(TOSLIBC) test example
endif

.PHONY: install
install: install-lib install-binutils install-compiler			\
	 install-test install-example

ALL_DEP = $(sort $(ALL_OBJ:%=%.d))

.PHONY: version
version:
	@script/version

.PHONY: gtags
gtags:
	$(QUIET_GEN)gtags
OTHER_CLEAN += GPATH GRTAGS GTAGS

.PHONY: clean
clean:
	$(QUIET_RM)$(RM) $(ALL_OBJ) $(ALL_DEP) $(OTHER_CLEAN)

V             = @
Q             = $(V:1=)
QUIET_AR      = $(Q:@=@echo    '  AR      '$@;)
QUIET_AS      = $(Q:@=@echo    '  AS      '$@;)
QUIET_CC      = $(Q:@=@echo    '  CC      '$@;)
QUIET_GEN     = $(Q:@=@echo    '  GEN     '$@;)
QUIET_LINK    = $(Q:@=@echo    '  LD      '$@;)
QUIET_RM      = $(Q:@=@echo    '  RM      '$@;)
QUIET_CHECK   = $(Q:@=@echo    '  CHECK   '$@;)
QUIET_TEST    = $(Q:@=@echo    '  TEST    '$@;)

$(eval -include $(ALL_DEP))
