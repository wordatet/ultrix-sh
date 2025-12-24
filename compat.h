/*
 * compat.h - Linux compatibility shims for Ultrix 4.2 shell
 */

#ifndef COMPAT_H
#define COMPAT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <sys/ioctl.h>

#ifndef OPEN_MAX
#define OPEN_MAX 256
#endif

/* Linux/POSIX replacement for FIOCLEX */
#ifndef FIOCLEX
#define FIOCLEX 0x5451
#endif

/* Avoid conflicts with libc */
#define setenv sh_setenv

#define free sh_free

/* Terminal compatibility shims for gtty/stty */
struct sgttyb { int dummy; };
#define gtty(fd, b) (isatty(fd) ? 0 : -1)
#define stty(fd, b) 0

#endif /* COMPAT_H */
