// SPDX-License-Identifier: LGPL-2.1

#ifndef _TOSLIBC_SYS_STAT_H
#define _TOSLIBC_SYS_STAT_H

#include <sys/types.h>

typedef uint16_t mode_t;

/**
 * enum stat_bit_fields - stat file type bit fields
 * @S_IFMT:   bit mask for the file type bit field
 * @S_IFSOCK: socket
 * @S_IFLNK:  symbolic link
 * @S_IFREG:  regular file
 * @S_IFBLK:  block device
 * @S_IFDIR:  directory
 * @S_IFCHR:  character device
 * @S_IFIFO:  FIFO
 */
enum stat_bit_fields {
	S_IFMT   = 0170000,
	S_IFSOCK = 0140000,
	S_IFLNK  = 0120000,
	S_IFREG  = 0100000,
	S_IFBLK  = 0060000,
	S_IFDIR  = 0040000,
	S_IFCHR  = 0020000,
	S_IFIFO  = 0010000,
};

#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)
#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)

/**
 * struct stat - file status
 * @st_mode: file type and mode
 * @st_size: total size, in bytes
 */
struct stat {
	mode_t st_mode;
	off_t st_size;
};

int stat(const char *pathname, struct stat *statbuf);

/*
 * lstat() is identical to stat(), except that if pathname is a symbolic
 * link, then it returns  information about the link itself, not the file
 * that the link refers to.
 */
static inline int lstat(const char *pathname, struct stat *statbuf)
{
	return stat(pathname, statbuf);  /* Atari TOS has no symlinks. */
}

#endif /* _TOSLIBC_SYS_STAT_H */
