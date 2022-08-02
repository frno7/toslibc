# SPDX-License-Identifier: GPL-2.0
#
# See the file INSTALL for installation instructions.

CFLAGS = -g

ifdef TARGET_COMPILE
TARGET_CC = $(TARGET_COMPILE)gcc
TARGET_LD = $(TARGET_COMPILE)ld
TARGET_AR = $(TARGET_COMPILE)ar
endif

TARGET_CFLAGS = $(CFLAGS)

ifeq "$(S)" "1"
S_CFLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined	\
	  -fsanitize-address-use-after-scope -fstack-check
endif

DEP_CFLAGS = -Wp,-MD,$(@D)/$(@F).d -MT $(@D)/$(@F)
BASIC_CFLAGS = -O2 -Wall -Iinclude -D_GNU_SOURCE $(DEP_CFLAGS)

.PHONY: all
all:

include lib/Makefile
include tool/Makefile
include example/Makefile

ALL_DEP = $(sort $(ALL_OBJ:%=%.d))

.PHONY: version
version:
	@script/version

.PHONY: clean
clean:
	$(QUIET_RM)$(RM) -f $(ALL_OBJ) $(ALL_DEP)			\
		$(TOSLIBC) $(TOSLIBC_VERSION_SRC)			\
		$(TOSLINK)						\
		$(EXAMPLE)						\
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

$(eval -include $(ALL_DEP))
