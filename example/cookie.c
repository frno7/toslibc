// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <stdlib.h>

#include <tos/cookie.h>
#include <tos/gemdos.h>
#include <tos/xbios.h>

static void super(void *arg)
{
	volatile struct cookie *cookie;

	for_each_cookie (cookie)
		printf("cookie %s %lx\r\n", cookie_sid(*cookie), cookie->value);

	printf("cookie jar size %d\r\n", cookie_jar_size());

	gemdos_cnecin();
}

int main(int argc, char *argv[])
{
	xbios_supexecarg(super, NULL);

	return EXIT_SUCCESS;
}
