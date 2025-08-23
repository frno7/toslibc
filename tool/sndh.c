// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2025 Fredrik Noring
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "internal/compare.h"
#include "internal/struct.h"

#include "tool/file.h"
#include "tool/option.h"
#include "tool/prg.h"
#include "tool/print.h"
#include "tool/sndh.h"
#include "tool/relocation.h"
#include "tool/section.h"
#include "tool/symbol.h"
#include "tool/tool.h"

#define SNDH_SECTION_HEADER_SIZE 16

struct sndh_section_header {
	uint32_t text_size;
	uint32_t data_size;
	uint32_t bss_size;
	uint32_t reloc_size;
} BE_STORAGE __attribute__((__packed__));

struct sndh {
	struct link_header link_header;
	struct sndh_section_header sndh_section_header;

	size_t size;
};

static size_t append_bra_w(struct file *tf, int offset)
{
	const uint8_t bra_w[4] = {
		0x60,
		0x00,
		(offset >> 8) & 0xff,
		(offset >> 0) & 0xff,
	};
	const size_t size = sizeof(bra_w);

	file_append(tf, bra_w, size);

	return size;
}

static size_t append_text(struct file *tf, const char *text)
{
	const size_t size = strlen(text);

	file_append(tf, text, size);

	return size;
}

static size_t append_even(struct file *tf, size_t offset)
{
	const uint8_t align[1] = { 0 };
	const size_t size = offset & 1;

	if (tf)
		file_append(tf, align, size);

	return size;
}

static size_t append_sndh_title(struct file *tf, struct file *ef)
{
	const size_t size = section_size(ef, ".sndh.title", data_section);

	if (!size)
		return 0;

	if (tf) {
		file_append(tf, "TITL", 4);
		append_section(tf, ef, ".sndh.title", data_section);
	}

	return 4 + size + append_even(tf, size);
}

static int sndh_tune_count_(struct file *ef)
{
	const struct section s =
		section_data(ef, ".sndh.tune.count", data_section);
	const uint8_t *b = s.data;

	return s.size == 2 ? (b[0] << 8) | b[1] : 0;
}

static int sndh_tune_names_count_(struct file *ef)
{
	const struct section s =
		section_data(ef, ".sndh.tune.names", data_section);
	const char *c = s.data;
	int n = 0;

	/* List of strings ends with "\0\0" since each entry ends with "\0". */
	for (size_t i = 0; i + 1 < s.size; i++)
		n += c[i] == '\0';

	return n;
}

static int sndh_tune_count(struct file *ef)
{
	struct {
		const char * const name;
		int (*f)(struct file *ef);
	} sections[] = {
		{ ".sndh.tune.count", sndh_tune_count_ },
		{ ".sndh.tune.names", sndh_tune_names_count_ },
	};
	int n = 0;
	int k = 0;

	for (size_t i = 0; i < ARRAY_SIZE(sections); i++) {
		const int m = sections[i].f(ef);

		if (!m) {
			continue;
		} else if (!n) {
			n = m;
			k = i;
		} else if (n != m)
			pr_fatal_error("inconsistent tune count: %d (%s) != %d (%s)",
				n, sections[k].name, m, sections[i].name);
	}

	return n;
}

static size_t append_sndh_tune_count(struct file *tf, struct file *ef)
{
	const int n = sndh_tune_count(ef);
	const char count[6] = {
		'#', '#',
		((n / 10) % 10) + '0',
		((n /  1) % 10) + '0',
		'\0', '\0'
	};

	if (!n)
		return 0;
	if (tf)
		file_append(tf, count, sizeof(count));

	return sizeof(count);
}

static size_t append_sndh_tune_names(struct file *tf, struct file *ef)
{
	const int n = sndh_tune_count(ef);
	const struct section s =
		section_data(ef, ".sndh.tune.names", data_section);
	const char *c = s.data;
	size_t offset = 0;
	size_t size = 4;

	if (!n || s.size <= 1)
		return 0;
	if (tf)
		append_text(tf, "!#SN");

	for (size_t i = 0; i < n && offset < s.size; i++) {
		const uint8_t data[2] = {
			((4 + n * 2 + offset) >> 8) & 0xff,
			((4 + n * 2 + offset) >> 0) & 0xff
		};

		if (tf)
			file_append(tf, data, sizeof(data));
		size += sizeof(data);

		offset += strlen(&c[offset]) + 1;
	}

	if (tf)
		file_append(tf, s.data, offset);
	size += offset;

	return size + append_even(tf, size);
}

static size_t append_sndh_timer(struct file *tf, struct file *ef)
{
	const struct section s =
		section_data(ef, ".sndh.timer", data_section);
	const uint8_t *b = s.data;

	if (!s.size)
		return 0;
	if (s.size != 4)
		pr_fatal_error("%s: .sndh.timer size mismatch %zu != 4",
			ef->path, s.size);

	char timer[6] = { b[0], b[1] };
	const int n = (b[2] << 8) | b[3];
	int i = 2;

	if (b[0] != '!' || b[1] != 'V')
		timer[i++] = ((n / 100) % 10) + '0';

	timer[i++] = ((n /  10) % 10) + '0';
	timer[i++] = ((n /   1) % 10) + '0';

	if (tf)
		file_append(tf, timer, sizeof(timer));

	return sizeof(timer);
}

static size_t append_sndh_metadata(struct file *tf, struct file *ef)
{
	if (tf)
		append_text(tf, "SNDH");

	const size_t title_length = append_sndh_title(tf, ef);
	const size_t count_length = append_sndh_tune_count(tf, ef);
	const size_t names_length = append_sndh_tune_names(tf, ef);
	const size_t timer_length = append_sndh_timer(tf, ef);

	if (tf)
		append_text(tf, "HDNS");

	return 4 + count_length
		 + title_length
		 + names_length
		 + timer_length
		 + 4;
}

static size_t sndh_metadata_size(struct file *ef)
{
	return append_sndh_metadata(NULL, ef);
}

static void append_sndh_bra_w_size_init_exit_play(
	struct file *tf, const struct link_header *link_header)
{
	append_bra_w(tf, link_header->size + 10);
	append_bra_w(tf, link_header->size -  0);
	append_bra_w(tf, link_header->size - 10);
}

static struct sndh append_sndh_header(struct file *tf, struct file *ef)
{
	const size_t sndh_bra_w_size_init_exit_play = 3 * 4;
	const struct link_header link_header = {
		.size = sndh_bra_w_size_init_exit_play +
			sndh_metadata_size(ef) +
			sizeof(struct sndh_section_header)
	};
	const struct sndh_section_header sndh_section_header = {
		.text_size  = section_size(ef, ".text", text_section),
		.data_size  = section_size(ef, ".data", data_section),
		.bss_size   = section_size(ef, ".bss",  bss_section),
		.reloc_size = relocation_size(ef, &link_header)
	};
	const struct sndh sndh = {
		.link_header = link_header,
		.sndh_section_header = sndh_section_header,
		.size = link_header.size +
			sndh_section_header.text_size +
			sndh_section_header.data_size +
			/* Relocation and BSS sections overlap to save space. */
			max(sndh_section_header.reloc_size,
			    sndh_section_header.bss_size)
	};

	BUILD_BUG_ON(sizeof(struct sndh_section_header) != SNDH_SECTION_HEADER_SIZE);

	append_sndh_bra_w_size_init_exit_play(tf, &link_header);
	append_sndh_metadata(tf, ef);

	if (tf)
		file_append(tf, (const void *)&sndh_section_header,
			sizeof(sndh_section_header));

	return sndh;
}

void append_section_bss(struct file *tf,
	const struct sndh_section_header *sndh_section_header)
{
	ssize_t size = (ssize_t)sndh_section_header->bss_size -
		       (ssize_t)sndh_section_header->reloc_size;
	static const uint8_t zero[256] = { };

	while (size > 0) {
		const ssize_t n = min_t(ssize_t, size, ARRAY_SIZE(zero));

		file_append(tf, zero, n);

		size -= n;
	}
}

void link_sndh(struct file *tf, struct file *ef)
{
	const struct sndh sndh = append_sndh_header(tf, ef);

	append_sections_text_data(tf, ef);
	append_relocations_text_data(tf, ef, &sndh.link_header);
	append_section_bss(tf, &sndh.sndh_section_header);

	if (tf->size != sndh.size)
		pr_fatal_error("%s: size mismatch %zu != %zu",
			ef->path, tf->size, sndh.size);
}

static bool has_suffix(const char *string, const char *suffix)
{
	const size_t string_len = strlen(string);
	const size_t suffix_len = strlen(suffix);

	return string_len >= suffix_len &&
	       strcmp(&string[string_len - suffix_len], suffix) == 0;
}

bool has_sndh_extension(const char *path)
{
	return has_suffix(path, "sndh") || has_suffix(path, "snd") ||
	       has_suffix(path, "SNDH") || has_suffix(path, "SND");
}
