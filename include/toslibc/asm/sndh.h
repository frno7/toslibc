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

enum {
	SNDH_TIMER_VBL = ('!' << 8) | 'V',
	SNDH_TIMER_A   = ('T' << 8) | 'A',
	SNDH_TIMER_B   = ('T' << 8) | 'B',
	SNDH_TIMER_C   = ('T' << 8) | 'C',
	SNDH_TIMER_D   = ('T' << 8) | 'D',
};

#define sndh_timer(timer, frequency) const uint8_t _sndh_timer[4]	\
	__attribute__((section(".sndh.timer"))) = {			\
		((timer)     >> 8) & 0xff,				\
		((timer)     >> 0) & 0xff,				\
		((frequency) >> 8) & 0xff,				\
		((frequency) >> 0) & 0xff				\
	}

void sndh_init(int tune);
void sndh_exit();
void sndh_play();

#endif /* TOSLIBC_ASM_SNDH_H */
