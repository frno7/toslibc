// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>

#include <tos/aes.h>

int main(int argc, char *argv[])
{
	static struct aes aes;

	const int16_t ap_id = aes_appl_init(&aes);
	if (ap_id == -1)
		return EXIT_FAILURE;

	aes_form_alert(&aes, 1, "[1][Hello, GEM.][ OK ]");

	aes_appl_exit(&aes);

	return EXIT_SUCCESS;
}
