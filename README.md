TOS/libc is a 32-bit
[C standard library](https://en.wikipedia.org/wiki/C_standard_library)
to [cross-compile](https://en.wikipedia.org/wiki/Cross_compiler) programs and
libraries for [Atari TOS](https://en.wikipedia.org/wiki/Atari_TOS) and the
[Atari ST](https://en.wikipedia.org/wiki/Atari_ST) family of computers.

Many, if not most, other libc for Atari TOS are 16-bit. There are two
significant advantages with a 32-bit libc:

- The [GNU compiler collection](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)
  (GCC) for the [m68k](https://en.wikipedia.org/wiki/m68k)/Linux target works
  unmodified also for Atari TOS;

- modern C programs and libraries often assume the C integer type `int` is
  at least 32 bits, since that has been the case for most modern operating
  systems in use during at least 25 years.

The are also two potentially significant disadvantages compared with a 16-bit
libc: lower performance and greater memory use. However, with judicious
optimisations such losses can often be recovered.
