// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <asm/mfp.h>

#include "internal/build-assert.h"

void mfp_build_bug_on()
{
	BUILD_BUG_ON(sizeof(struct mfp_gpip) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_aer) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_ddr) != 1);

	BUILD_BUG_ON(sizeof(struct mfp_iera) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_ierb) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_ipra) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_iprb) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_isra) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_isrb) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_imra) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_imrb) != 1);

	BUILD_BUG_ON(sizeof(struct mfp_ier) != 2);
	BUILD_BUG_ON(sizeof(struct mfp_ipr) != 2);
	BUILD_BUG_ON(sizeof(struct mfp_isr) != 2);
	BUILD_BUG_ON(sizeof(struct mfp_imr) != 2);

	BUILD_BUG_ON(sizeof(struct mfp_tacr) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_tbcr) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_tccr) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_tdcr) != 1);

	BUILD_BUG_ON(sizeof(struct mfp_tadr) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_tbdr) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_tcdr) != 1);
	BUILD_BUG_ON(sizeof(struct mfp_tddr) != 1);
}
