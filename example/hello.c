// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <stdlib.h>

#include <tos/gemdos.h>

int main(int argc, char *argv[])
{
	printf("Hello, TOS.\n");

	gemdos_cnecin();

	return EXIT_SUCCESS;
}
