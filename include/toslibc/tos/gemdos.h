// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2019 Fredrik Noring
 */

#ifndef _TOSLIBC_TOS_GEMDOS_H
#define _TOSLIBC_TOS_GEMDOS_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#include "dta.h"
#include "gemdos-call.h"

/**
 * struct _date - GEMDOS date
 * @year: year since 1980
 * @month: month
 * @day: day
 */
struct _date {
	uint16_t day : 5;
	uint16_t month : 4;
	uint16_t year : 7;
};

/**
 * struct _time - GEMDOS time
 * @hour: hour
 * @minute: minute
 * @second: second divided by 2
 */
struct _time {
	uint16_t second : 5;
	uint16_t minute : 6;
	uint16_t hour : 5;
};

/**
 * struct _datetime - GEMDOS date and time
 * @date: GEMDOS date
 * @time: GEMDOS time
 */
struct _datetime {
	struct _date date;
	struct _time time;
};

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#define S_READ		0
#define S_WRITE		1
#define S_READWRITE	2

#define GSH_CONIN	 0
#define GSH_CONOUT	 1
#define GSH_AUX		 2
#define GSH_PRN		 3
#define GSH_BIOSCON	-1
#define GSH_BIOSAUX	-2
#define GSH_BIOSPRN	-3
#define GSH_MIDIIN	-4
#define GSH_MIDIOUT	-5

#define FA_INQUIRE	0
#define FA_SET		1

#define FA_READONLY	0
#define FA_HIDDEN	1
#define FA_SYSTEM	2
#define FA_VOLUME	3
#define FA_DIR		4
#define FA_ARCHIVE	5

#define FA_ISREADONLY(attr)	(((attr) & (1 << FA_READONLY)) != 0)
#define FA_ISHIDDEN(attr)	(((attr) & (1 << FA_HIDDEN))   != 0)
#define FA_ISSYSTEM(attr)	(((attr) & (1 << FA_SYSTEM))   != 0)
#define FA_ISVOLUME(attr)	(((attr) & (1 << FA_VOLUME))   != 0)
#define FA_ISDIR(attr)		(((attr) & (1 << FA_DIR))      != 0)
#define FA_ISARCHIVE(attr)	(((attr) & (1 << FA_ARCHIVE))  != 0)

#define GEMDOS_DECLARE(opcode_, rtype_, name_, ptypes_, ...)		\
rtype_ gemdos_##name_(__VA_ARGS__);
GEMDOS_CALL(GEMDOS_DECLARE)

void __NORETURN gemdos_ptermres(int16_t code);

#endif /* _TOSLIBC_TOS_GEMDOS_H */
