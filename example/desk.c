// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>

#include <tos/aes.h>

int main(int argc, char *argv[])
{
	static struct aes aes;

	const int16_t ap_id = aes_appl_init(&aes);
	if (ap_id == -1)
		return EXIT_FAILURE;

	const int16_t menu_id = aes_menu_register(&aes, ap_id, " Hello");
	if (menu_id == -1)
		return EXIT_FAILURE;

	for (;;) {
		struct aes_mesag msg = { };

		aes_evnt_mesag(&aes, &msg);

		switch (msg.type) {
		case AES_AC_OPEN:
			if (msg.ac_open.id != menu_id)
				continue;

			aes_form_alertf(&aes, 1, "[%d][%s][ OK ]",
				AES_FORM_ICON_EXCLAMATION,
				"Hello, accessory.");
			break;

		case AES_AC_CLOSE:
			if (msg.ac_close.id != menu_id)
				continue;
			break;
		}
	}
}
