// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_SNDH_H
#define TOSLIBC_ASM_SNDH_H

#define sndh_title(title) const char _sndh_title[]			\
	__attribute__((section(".sndh.title")))	= title

#define sndh_tune_count(count) const unsigned char _sndh_tune_count[2]	\
	__attribute__((section(".sndh.tune.count"))) = {		\
		((count) >> 8) & 0xff,					\
		((count) >> 0) & 0xff					\
	}

#define SNDH_TUNE_DEFINE_NAME_vn(value, name)				\
	name "\0"

#define SNDH_TUNE_DEFINE_NAME_vtn(value, time, name)			\
	name "\0"

/* Each entry ends with "\0", so the whole section ends with "\0\0". */
#define sndh_tune_names(type, entries, kind)				\
	__attribute__((section(".sndh.tune.names")))			\
	const char _sndh_tune_names[] =					\
		entries(SNDH_TUNE_DEFINE_NAME_##kind)

#define SNDH_TUNE_DEFINE_TIME_vtn(value, time,  name)			\
	{								\
		((time) >> 8) & 0xff,					\
		((time) >> 0) & 0xff					\
	},

#define sndh_tune_times(type, entries, kind)				\
	__attribute__((section(".sndh.tune.times")))			\
	const unsigned char _sndh_tune_times[][2] = {			\
		entries(SNDH_TUNE_DEFINE_TIME_##kind)			\
	}

#define SNDH_TUNE_DEFINE_VALUE_vn(value, name)				\
	value,

#define SNDH_TUNE_DEFINE_VALUE_vtn(value, time,  name)			\
	value,

#define sndh_tune_types(type, entries, kind)				\
	const type _sndh_tune_values[] = {				\
		entries(SNDH_TUNE_DEFINE_VALUE_##kind)			\
	}

#define sndh_tune_value_names(type, entries)				\
	sndh_tune_names(type, entries, vn);				\
	sndh_tune_types(type, entries, vn)

#define sndh_tune_value_time_names(type, entries)			\
	sndh_tune_names(type, entries, vtn);				\
	sndh_tune_times(type, entries, vtn);				\
	sndh_tune_types(type, entries, vtn)

#define sndh_tune_select_value(tune)					\
	(_sndh_tune_values[(tune) - 1])

enum {
	SNDH_TIMER_VBL = ('!' << 8) | 'V',
	SNDH_TIMER_A   = ('T' << 8) | 'A',
	SNDH_TIMER_B   = ('T' << 8) | 'B',
	SNDH_TIMER_C   = ('T' << 8) | 'C',
	SNDH_TIMER_D   = ('T' << 8) | 'D',
};

#define sndh_timer(timer, frequency) const unsigned char _sndh_timer[4]	\
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
