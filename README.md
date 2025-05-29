![compilation workflow](https://github.com/frno7/toslibc/actions/workflows/compilation.yml/badge.svg)

TOS/libc is a 32-bit
[C standard library](https://en.wikipedia.org/wiki/C_standard_library)
to compile programs for [Atari TOS](https://en.wikipedia.org/wiki/Atari_TOS)
and the [Atari ST](https://en.wikipedia.org/wiki/Atari_ST) family of computers.

Many, if not most, other libc for Atari TOS are 16-bit. There are two
significant advantages with a 32-bit libc:

- the [GNU compiler collection](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)
  (GCC) for the `m68k-elf` target works unmodified also for Atari TOS;

- modern C programs and libraries often assume the C integer type `int` is
  at least 32 bits, since that has been the case for most modern operating
  systems in use during at least 25 years.

The are also two potentially significant disadvantages compared with a 16-bit
libc: lower performance and greater memory use. However, with judicious
optimisations such losses can often be recovered.

Programs linked with TOS/libc can use the `--gc-sections` option to the
GNU linker and thereby drastically reduce their size. The supplied example
programs compile to a few kilobytes each, for instance, which make them
several times smaller than had the whole TOS/libc been linked.

TOS/libc includes interfaces for the following Atari TOS specific libraries:

- BIOS (basic input/output system),
- XBIOS (extended BIOS),
- GEMDOS (graphics environment manager disk operating system),
- XGEMDOS (extended GEMDOS),
- AES (application environment services),
- VDI (virtual device interface).

The AES and VDI interfaces are part of the
[GEM desktop environment](https://en.wikipedia.org/wiki/GEM_(desktop_environment)).

# Examples

The [`example`](https://github.com/frno7/toslibc/tree/main/example) directory
contains applications of TOS/libc:

![ALERT.PRG](https://raw.githubusercontent.com/frno7/toslibc/main/example/alert.png)
![HELLO.PRG](https://raw.githubusercontent.com/frno7/toslibc/main/example/hello.png)
![WINDOW.PRG](https://raw.githubusercontent.com/frno7/toslibc/main/example/window.png)
![XBRA.PRG](https://raw.githubusercontent.com/frno7/toslibc/main/example/xbra.png)

# How to download

- [Gentoo Linux](https://en.wikipedia.org/wiki/Gentoo_Linux) has a
  [`crossdev`](https://wiki.gentoo.org/wiki/Crossdev) package that installs
  `m68k-elf-gcc` with the command `crossdev -s1 -t m68k-elf`. After that, a
  compiler with TOS/libc can be installed with the
  [`sys-devel/m68k-atari-tos-gnu-gcc`](https://github.com/frno7/gentoo.overlay/tree/main/sys-devel/m68k-atari-tos-gnu-gcc) package.
- The [`gentoo-m68k`](https://github.com/frno7/gentoo-m68k) repository has
  installation scripts and a
  [Docker](https://en.wikipedia.org/wiki/Docker_(software)) configuration for
  building Atari TOS programs with GitHub workflows (mentioned below).
- [Mac OS](https://en.wikipedia.org/wiki/MacOS)
  [Homebrew](https://en.wikipedia.org/wiki/Homebrew_(package_manager)) has
  both [`m68k-elf-gcc`](https://formulae.brew.sh/formula/m68k-elf-gcc) and
  [`toslibc`](https://github.com/kareandersen/homebrew-toslibc) packages.

Precompiled
[`example`](https://github.com/frno7/toslibc/tree/main/example)
applications are available for download under the GitHub
[actions](https://github.com/frno7/toslibc/actions) tab. These are
automatically built with
[`.github/workflows/compilation.yml`](https://github.com/frno7/toslibc/blob/main/.github/workflows/compilation.yml).

# How to build

Review the file [`INSTALL`](https://github.com/frno7/toslibc/blob/main/INSTALL)
for installation instructions. A preinstalled m68k/GCC compiler is required.
Assuming the compiler is `m68k-elf-gcc`, build TOS/libc with
`make TARGET_COMPILE=m68k-elf-` to produce a `lib/toslibc.a`
[static library](https://en.wikipedia.org/wiki/Static_library),
an `m68k-atari-tos-gnu-gcc` compiler, example applications,
and a test suite `test/SUITE.TOS`.

Type `make TARGET_COMPILE=m68k-elf- install` to install everything, by
default in `~/.local/usr/m68k-atari-tos-gnu`. Set `prefix` to change the
directory, for example
`make TARGET_COMPILE=m68k-elf- prefix=$HOME/some/place/else install`.
More specific subtargets than `install` are also available, for instance
`install-lib`, `install-binutils`, `install-compiler`, `install-test`,
`install-example`, and so on. Set `DESTDIR` for
[staged installs](https://www.gnu.org/prep/standards/html_node/DESTDIR.html).

Add `$prefix/bin` to your `PATH`. It normally has a combination of scripts
and symlinks to the `m68k-elf-gcc` compiler, similar to

```
lrwxrwxrwx    20 m68k-atari-tos-gnu-ar -> /usr/bin/m68k-elf-ar
lrwxrwxrwx    20 m68k-atari-tos-gnu-as -> /usr/bin/m68k-elf-as
-rwxr-xr-x   132 m68k-atari-tos-gnu-cc
lrwxrwxrwx    21 m68k-atari-tos-gnu-gcc -> m68k-atari-tos-gnu-cc
-rwxr-xr-x   467 m68k-atari-tos-gnu-ld
lrwxrwxrwx    20 m68k-atari-tos-gnu-nm -> /usr/bin/m68k-elf-nm
lrwxrwxrwx    25 m68k-atari-tos-gnu-objcopy -> /usr/bin/m68k-elf-objcopy
lrwxrwxrwx    25 m68k-atari-tos-gnu-objdump -> /usr/bin/m68k-elf-objdump
lrwxrwxrwx    28 m68k-atari-tos-gnu-pkg-config -> /usr/bin/m68k-elf-pkg-config
lrwxrwxrwx    24 m68k-atari-tos-gnu-ranlib -> /usr/bin/m68k-elf-ranlib
lrwxrwxrwx    25 m68k-atari-tos-gnu-readelf -> /usr/bin/m68k-elf-readelf
lrwxrwxrwx    22 m68k-atari-tos-gnu-size -> /usr/bin/m68k-elf-size
lrwxrwxrwx    25 m68k-atari-tos-gnu-strings -> /usr/bin/m68k-elf-strings
lrwxrwxrwx    23 m68k-atari-tos-gnu-strip -> /usr/bin/m68k-elf-strip
-rwxr-xr-x 84104 m68k-atari-tos-gnu-toslink
```

# How to verify

Atari TOS example applications and a `Makefile` are installed in
`$prefix/share/toslibc/example`. Go to that directory and type
`make clean && make`, as in

```bash
$ make clean && make
rm -f ALERT.PRG COOKIE.TOS HELLO.TOS WINDOW.PRG XBRA.PRG
m68k-atari-tos-gnu-gcc -O2 -Wall -o ALERT.PRG alert.c
m68k-atari-tos-gnu-gcc -O2 -Wall -o COOKIE.TOS cookie.c
m68k-atari-tos-gnu-gcc -O2 -Wall -o HELLO.TOS hello.c
m68k-atari-tos-gnu-gcc -O2 -Wall -o WINDOW.PRG window.c
m68k-atari-tos-gnu-gcc -O2 -Wall -o XBRA.PRG xbra.c
```

A test suite for TOS/libc is installed in `$prefix/share/toslibc/test`.

The examples and the test suite can run on Atari ST hardware or an emulator.

# How it works

TOS/libc automatically converts between 32-bit and 16-bit integers for
Atari TOS. The main complication is the `trap` system call with its stack
arguments, which must handle
[reentrancy](https://en.wikipedia.org/wiki/Reentrancy_(computing)). This
conversion is done in
[`include/toslibc/tos/trap.S`](https://github.com/frno7/toslibc/tree/main/include/toslibc/tos/trap.S).
Integers in structures for Atari TOS are generally defined as `int16_t` or
`uint16_t`. [Bit fields](https://en.wikipedia.org/wiki/Bit_field) are generally
used to improve notation with modern C standards.

A standard, unmodified, m68k/GCC compiler is used to obtain
[ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format) object
files, having a specific structure defined in
[`script/prg.ld`](https://github.com/frno7/toslibc/blob/main/script/prg.ld).
The `tool/m68k-atari-tos-gnu-toslink` linker is used to produce the final
Atari TOS program files.
