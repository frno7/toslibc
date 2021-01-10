// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>

#include <tos/aes.h>
#include <tos/xbra.h>
#include <tos/xgemdos.h>

struct calls {
	size_t aes;
	size_t vdi;
};

DEFINE_XBRA("xmpl", xbra_example);

static bool enter(uint32_t vector, struct xbra_regs *regs, void *arg)
{
	const uint16_t fn = regs->d[0];
	struct calls *c = arg;

	switch (fn) {
	case xgemdos_trap_aes: c->aes++; break;
	case xgemdos_trap_vdi: c->vdi++; break;
	}

	return true;	/* Proceed with the original exception. */
}

int main(int argc, char *argv[])
{
	static struct aes aes;
	struct calls c = { };

	xbra_example_init(34 /* XGEMDOS */, enter, NULL, &c);

	const int16_t ap_id = aes_appl_init(&aes);
	if (ap_id == -1)
		return EXIT_FAILURE;

	aes_form_alertf(&aes, 1, "[%d][Hello!][ OK ]",
		AES_FORM_ICON_EXCLAMATION);

	aes_form_alertf(&aes, 1,
		"[%d][There have now been |"
		     "%zu calls to AES and |"
		     "%zu calls to VDI. ][ OK ]",
		AES_FORM_ICON_NONE, c.aes, c.vdi);

	aes_appl_exit(&aes);

	xbra_example_exit();

	return EXIT_SUCCESS;
}
