// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal/version.h"

#include "tool/option.h"
#include "tool/print.h"
#include "tool/tool.h"

struct option_ option;

static void help(FILE *file)
{
	fprintf(file,
"usage: %s [-h | --help] <-o <outfile>> <infile>\n"
"\n"
"options:\n"
"    -h, --help              display this help and exit\n"
"    --version               display version and exit\n"
"\n"
"    -o, --output <outfile>  name of the program produced by %s\n"
"\n",
		progname, progname);
}

static void NORETURN help_exit(void)
{
	help(stdout);
	exit(EXIT_SUCCESS);
}

static void NORETURN version_exit(void)
{
	printf("%s version %s\n", progname, toslibc_version());

	exit(EXIT_SUCCESS);
}

void parse_options(int argc, char **argv)
{
#define OPT(option) (strcmp(options[index].name, (option)) == 0)

	static const struct option options[] = {
		{ "help",    no_argument,       NULL, 0 },
		{ "version", no_argument,       NULL, 0 },
		{ "output",  required_argument, NULL, 0 },
		{ NULL, 0, NULL, 0 }
	};

	argv[0] = progname;	/* For better getopt_long error messages. */

	for (;;) {
		int index = 0;

		switch (getopt_long(argc, argv, "ho:", options, &index)) {
		case -1:
			if (argc < optind + 1)
				pr_fatal_error("no input file");
			if (argc > optind + 1)
				pr_fatal_error("too many input files");
			option.input = argv[optind];
			return;

		case 0:
			if (OPT("help"))
				help_exit();
			else if (OPT("version"))
				version_exit();
			else if (OPT("output"))
				option.output = optarg;
			break;

		case 'h':
			help_exit();

		case 'o':
			option.output = optarg;
			break;

		case '?':
			exit(EXIT_FAILURE);
		}
	}

#undef OPT
}
