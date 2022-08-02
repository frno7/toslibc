# SPDX-License-Identifier: GPL-2.0
#
# See the file INSTALL for installation instructions.

CFLAGS += -g -O2 -Wall -fPIC -Iinclude -D_GNU_SOURCE

ifdef TARGET_COMPILE
TARGET_CC = $(TARGET_COMPILE)gcc
TARGET_LD = $(TARGET_COMPILE)ld
TARGET_AR = $(TARGET_COMPILE)ar
endif

ifeq "$(S)" "1"
S_CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined	\
	  -fsanitize-address-use-after-scope -fstack-check
endif

DEP_CFLAGS += $(CFLAGS) $(BASIC_CFLAGS)
ALL_CFLAGS += $(DEP_CFLAGS) $(HAVE_CFLAGS) $(S_CFLAGS)

.PHONY: all
all:

include lib/Makefile
include tool/Makefile
include example/Makefile

.PHONY: version
version:
	@script/version

.PHONY: clean
clean:
	$(QUIET_RM)$(RM) -f *.a */*.o* */*/*.o*				\
		$(TOSLIBC_VERSION_SRC) $(TOSLINK)			\
		*/*.PRG */*.TOS */*.TTP					\
		GPATH GRTAGS GTAGS

.PHONY: gtags
gtags:
	$(QUIET_GEN)gtags

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

BASIC_CFLAGS += -Wp,-MD,$(@D)/$(@F).d -MT $(@D)/$(@F)

ifneq "$(MAKECMDGOALS)" "clean"
    DEP_FILES := $(shell find . -name '*'.d -printf '%P\n' | sort)
    $(if $(DEP_FILES),$(eval include $(DEP_FILES)))
endif
