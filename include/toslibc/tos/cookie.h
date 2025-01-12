// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#ifndef TOS_COOKIE_H
#define TOS_COOKIE_H

#include <stdint.h>

#include <tos/system-variable.h>

/**
 * struct cookie - Cookie Jar entry
 * @uid: unsigned integer identification
 * @cid: character identification
 * @value: value
 */
struct cookie {
	union {
		uint32_t uid;
		char cid[4];
	};
	uint32_t value;
};

#define COOKIE_UIDS(cid)						\
	cid(_CPU, '_CPU')						\
	cid(_VDO, '_VDO')						\
	cid(_MCH, '_MCH')						\
	cid(_SWI, '_SWI')						\
	cid(_SND, '_SND')						\
	cid(_FPU, '_FPU')

#define COOKIES(c)							\
	c(_CPU, 68000,	     	 0)					\
	c(_CPU, 68010,	     	10)					\
	c(_CPU, 68020,	     	20)					\
	c(_CPU, 68030,	     	30)					\
									\
	c(_VDO, ST,		(0 << 16) |  0)				\
	c(_VDO, STE,		(1 << 16) |  0)				\
	c(_VDO, TT,		(2 << 16) |  0)				\
	c(_VDO, FALCON,		(3 << 16) |  0)				\
									\
	c(_MCH, ST,		(0 << 16) |  0)				\
	c(_MCH, STE,		(1 << 16) |  0)				\
	c(_MCH, ST_BOOK,	(1 << 16) |  8)				\
	c(_MCH, MEGA_STE,	(1 << 16) | 16)				\
	c(_MCH, TT,		(2 << 16) |  0)				\
	c(_MCH, FALCON,		(3 << 16) |  0)				\
									\
	c(_SND, PSG,		1 << 0)					\
	c(_SND, DMA8,		1 << 1)					\
	c(_SND, DMAREC,		1 << 2)					\
	c(_SND, DMA16,		1 << 3)					\
	c(_SND, DSP,		1 << 4)

enum {
#define DEFINE_COOKIE_UID(id_, uid_)					\
	COOKIE_ ## id_ = uid_,
COOKIE_UIDS(DEFINE_COOKIE_UID)
};

enum {
#define DEFINE_COOKIE_VALUE(id_, label_, value_)			\
	COOKIE_ ## id_ ## _ ## label_ = value_,
COOKIES(DEFINE_COOKIE_VALUE)
	COOKIE__FPU_NONE = 0,
};

int cookie_jar_size();

struct __cookie_cid { char s[5]; };

static inline struct __cookie_cid __cookie_sid(struct cookie c)
{
	return (struct __cookie_cid) { {
		c.cid[0],
		c.cid[1],
		c.cid[2],
		c.cid[3],
		'\0'
	} };
}

#define cookie_sid(c) (__cookie_sid(c).s)

#define for_each_cookie(cookie)						\
	for ((cookie) = __system_variables->_p_cookies; (cookie)->uid; (cookie)++)

#endif /* TOS_COOKIE_H */
