TOS/libc is a 32-bit
[C standard library](https://en.wikipedia.org/wiki/C_standard_library)
to [cross-compile](https://en.wikipedia.org/wiki/Cross_compiler) programs and
libraries for [Atari TOS](https://en.wikipedia.org/wiki/Atari_TOS) and the
[Atari ST](https://en.wikipedia.org/wiki/Atari_ST) family of computers.

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

# How to build

Review the file [INSTALL](https://github.com/frno7/toslibc/blob/main/INSTALL)
for installation instructions. A working GCC/m68k cross-compiler is required.
Assuming the compiler is `m68k-elf-gcc`, build TOS/libc with
`make CROSS_COMPILE=m68k-elf-` to produce a `toslibc.a`
[static library](https://en.wikipedia.org/wiki/Static_library),
and example applications.

Precompiled example applications are available for download under the Github
[actions](https://github.com/frno7/toslibc/actions) tab. These are
automatically built with
[`.github/workflows/compilation.yml`](https://github.com/frno7/toslibc/blob/main/.github/workflows/compilation.yml).

# How it works

TOS/libc automatically converts 32-bit integers to 16-bit integers for
Atari TOS. The main complication is the `trap` system call with its stack
arguments, which must handle
[reentrancy](https://en.wikipedia.org/wiki/Reentrancy_(computing)). This
conversion is done in
[`include/toslibc/tos/trap.S`](https://github.com/frno7/toslibc/tree/main/include/toslibc/tos/trap.S).
Integers in structures for Atari TOS are generally defined as `int16_t` or
`uint16_t`. [Bit fields](https://en.wikipedia.org/wiki/Bit_field) are generally
used to improve notation with modern C standards.

A standard, unmodified, GCC/m68k package is used to compile
[ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format) object
files, having a specific structure defined in
[`script/prg.ld`](https://github.com/frno7/toslibc/blob/main/script/prg.ld).
The `tool/toslink` linker is used to produce the final Atari TOS program files.
