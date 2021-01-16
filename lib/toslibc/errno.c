// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2020 Fredrik Noring
 */

#include <errno.h>
#include <tos/error.h>

int errno;

int errno_for_tos_error(int toserr)
{
	switch (toserr) {
#define _TOS_ERROR_ERRNO(identifier_, number_, errno_, description_)	\
	case TOS_E##identifier_: return errno_;
_TOS_ERRORS(_TOS_ERROR_ERRNO)
	}

	return 0 < toserr && toserr < 1000 ? EINVAL : toserr;
}

const char *tos_error_message(int toserr)
{
	switch (toserr) {
#define _TOS_ERROR_MSG(identifier_, number_, errno_, description_)	\
	case TOS_E##identifier_: return description_;
_TOS_ERRORS(_TOS_ERROR_MSG)
	}

	return "Unknown error";
}
