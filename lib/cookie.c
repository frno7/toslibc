// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <tos/cookie.h>
#include <tos/system-variable.h>

int cookie_jar_size()
{
	volatile struct cookie *cookie = __system_variables->_p_cookies;

	if (!cookie)
		return -1;

	while (cookie->uid)
		cookie++;

	return cookie->value;
}
