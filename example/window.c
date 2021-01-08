// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <stdlib.h>

#include <tos/gemdos.h>
#include <tos/aes.h>
#include <tos/vdi.h>

static void wm_redraw(struct aes *aes,
	int16_t vdi_id, int16_t win_id,
	const struct aes_bar win_redr)
{
	struct aes_bar win_work = aes_wind_get_workxywh(aes, win_id);
	struct aes_bar win_draw;

	vdi_vsf_color(vdi_id, 0);

	aes_wind_update(aes, AES_WIND_BEG_UPDATE);
	aes_graf_mouse(aes, AES_GRAF_MOUSE_OFF, NULL);

	aes_wind_for_each_xywh (win_draw, aes, win_id) {
		const struct aes_bar win_clip =
			aes_bar_intersect(win_draw, win_redr);

		vdi_vs_clip_on(vdi_id, vdi_bar_from_aes(win_clip));
		vdi_v_bar(vdi_id, vdi_bar_from_aes(win_work));
	}

	aes_graf_mouse(aes, AES_GRAF_MOUSE_ON, NULL);
	aes_wind_update(aes, AES_WIND_END_UPDATE);

	vdi_vs_clip_off(vdi_id);
}

static bool event(struct aes *aes, const int16_t vdi_id)
{
	struct aes_mesag msg = { };

	aes_evnt_mesag(aes, &msg);

	switch (msg.type) {
	case AES_WM_REDRAW:
		wm_redraw(aes, vdi_id, msg.wm_redraw.id, msg.wm_redraw.b);
		break;

	case AES_WM_CLOSED:
		return false;

	case AES_WM_FULLED:
		aes_wind_set_currxywh(aes, msg.wm_fulled.id,
			aes_wind_get_fullxywh(aes, msg.wm_fulled.id));
		break;

	case AES_WM_SIZED:
		aes_wind_set_currxywh(aes, msg.wm_sized.id, msg.wm_sized.b);
		break;

	case AES_WM_MOVED:
		aes_wind_set_currxywh(aes, msg.wm_moved.id, msg.wm_moved.b);
		break;
	}

	return true;
}

int main(int argc, char *argv[])
{
	static struct aes aes;

	const int16_t ap_id = aes_appl_init(&aes);
	if (ap_id == -1)
		return EXIT_FAILURE;

	aes_graf_mouse(&aes, AES_GRAF_MOUSE_ARROW, NULL);

	const int16_t vdi_id = vdi_v_opnvwk(&aes, NULL, NULL, NULL);
	const struct aes_bar desktop_work =
		aes_wind_get_workxywh(&aes, AES_WIND_ID_DESKTOP);
	const struct aes_bar win_bar = {
		.p = {
			.x =  desktop_work.p.x + 20,
			.y =  desktop_work.p.y + 10
		},
		.r = {
			.w = 200,
			.h = 100
		}
	};

	const int win_id = aes_wind_create(&aes,
		AES_WC_NAME   |
		AES_WC_CLOSER |
		AES_WC_FULLER |
		AES_WC_MOVER  |
		AES_WC_SIZER, desktop_work);

	aes_wind_open(&aes, win_id, win_bar);

	for (;;)
		if (!event(&aes, vdi_id))
			break;

	aes_wind_close(&aes, win_id);

	aes_wind_delete(&aes, win_id);

	vdi_v_clsvwk(vdi_id);

	aes_appl_exit(&aes);

	return EXIT_SUCCESS;
}
