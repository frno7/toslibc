// SPDX-License-Identifier: LGPL-2.1

#ifndef _TOSLIBC_STDIO_H
#define _TOSLIBC_STDIO_H

#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>

#include <tos/gemdos.h>

#define EOF (-1)

int dprintf(int fd, const char *format, ...)
	__attribute__((format(printf, 2, 3)));

int printf(const char *format, ...)
	__attribute__((format(printf, 1, 2)));

int putchar(int c);

int puts(const char *s);

int vdprintf(int fd, const char *format, va_list ap);

int vprintf(const char *format, va_list ap);

int vsnprintf(char *str, size_t size, const char *format, va_list ap);

int snprintf(char *str, size_t size, const char *format, ...)
	__attribute__((format(printf, 3, 4)));

typedef struct { int pseudo_fd; } FILE;

static inline int fileno(FILE *stream)
{
	return (int)stream;
}

int __fopen_mode_flags(const char *mode);

FILE *fopen(const char *pathname, const char *mode);

static inline int fclose(FILE *stream)
{
	return close(fileno(stream)) != -1 ? 0 : EOF;
}

static inline int fseek(FILE *stream, long offset, int whence)
{
	return lseek(fileno(stream), offset, whence) != -1 ? 0 : -1;
}

static inline long ftell(FILE *stream)
{
	return lseek(fileno(stream), 0, SEEK_CUR);
}

static inline size_t fread(void *ptr, size_t size, size_t n, FILE *stream)
{
	const ssize_t r = read(fileno(stream), ptr, n * size);

	return r == -1 ? 0 : r / size;
}

static inline size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream)
{
	const ssize_t w = write(fileno(stream), ptr, n * size);

	return w == -1 ? 0 : w / size;
}

#endif /* _TOSLIBC_STDIO_H */
