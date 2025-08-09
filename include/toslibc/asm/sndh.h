// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_SNDH_H
#define TOSLIBC_ASM_SNDH_H

#define sndh_title(title) const char _sndh_title[]			\
	__attribute__((section(".sndh.title")))	= title

#define sndh_tune_count(count) const uint8_t _sndh_tune_count[2]	\
	__attribute__((section(".sndh.tune.count"))) = {		\
		((count) >> 8) & 0xff,					\
		((count) >> 0) & 0xff					\
	}

#define SNDH_TUNE_DEFINE_TITLE(value, title)				\
	title "\0"

/* Each entry ends with "\0", so the whole section ends with "\0\0". */
#define sndh_tune_names(type, names)					\
	__attribute__((section(".sndh.tune.names")))			\
	const char _sndh_tune_names[] =					\
		names(SNDH_TUNE_DEFINE_TITLE)

#define SNDH_TUNE_DEFINE_VALUE(value, title)				\
	value,

#define sndh_tune_types(type, names)					\
	const type _sndh_tune_values[] = {				\
		names(SNDH_TUNE_DEFINE_VALUE)				\
	}

#define sndh_tune_value_names(type, names)				\
	sndh_tune_names(type, names);					\
	sndh_tune_types(type, names)

#define sndh_tune_select_value(tune)					\
	(_sndh_tune_values[(tune) - 1])

void sndh_init(int tune);
void sndh_exit();
void sndh_play();

#endif /* TOSLIBC_ASM_SNDH_H */
