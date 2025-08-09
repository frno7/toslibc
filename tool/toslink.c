// SPDX-License-Identifier: GPL-2.0
/*
 * Link programs for Atari TOS
 *
 * Copyright (C) 2022 Fredrik Noring
 */

#include <stdio.h>
#include <stdlib.h>

#include "tool/file.h"
#include "tool/option.h"
#include "tool/print.h"
#include "tool/program.h"
#include "tool/sndh.h"
#include "tool/tool.h"
#include "tool/verify.h"

char progname[] = "toslink";

int main(int argc, char **argv)
{
	parse_options(argc, argv);

	struct file ef = file_read(option.input);
	struct file tf = { };

	verify_elf(&ef);

	if (has_sndh_extension(option.output))
		link_sndh(&tf, &ef);
	else
		link_program(&tf, &ef);

	if (!file_rename(option.output, &tf))
		pr_fatal_errno(option.output);

	if (!file_write(tf, 0755))
		pr_fatal_errno(tf.path);

	file_free(tf);
	file_free(ef);

	return EXIT_SUCCESS;
}
