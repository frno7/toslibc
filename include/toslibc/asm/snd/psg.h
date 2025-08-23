// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_SND_PSG_H
#define TOSLIBC_ASM_SND_PSG_H

#include <asm/io.h>
#include <asm/machine.h>
#include <asm/math.h>

#define PSG_ADDR_RD_SELECT	0xff8800
#define PSG_ADDR_WR		0xff8802

#define PSG_REGISTERS(reg)						\
	reg( 0, plo_a,   PLO_A,   "Period of channel A fine tone")	\
	reg( 1, phi_a,   PHI_A,   "Period of channel A rough tone")	\
	reg( 2, plo_b,   PLO_B,   "Period of channel B fine tone")	\
	reg( 3, phi_b,   PHI_B,   "Period of channel B rough tone")	\
	reg( 4, plo_c,   PLO_C,   "Period of channel C fine tone")	\
	reg( 5, phi_c,   PHI_C,   "Period of channel C rough tone")	\
	reg( 6, noise,   NOISE,   "Period of noise")			\
	reg( 7, iomix,   IOMIX,   "I/O port and mixer settings")	\
	reg( 8, level_a, LEVEL_A, "Level of channel A")			\
	reg( 9, level_b, LEVEL_B, "Level of channel B")			\
	reg(10, level_c, LEVEL_C, "Level of channel C")			\
	reg(11, plo_env, PLO_ENV, "Period of envelope fine")		\
	reg(12, phi_env, PHI_ENV, "Period of envelope rough")		\
	reg(13, shape,   SHAPE,   "Shape of envelope")			\
	reg(14, io_a,    IO_A,    "Data of I/O port A")			\
	reg(15, io_b,    IO_B,    "Data of I/O port B")

enum {
#define PSG_REG_ENUM(register_, symbol_, label_, description_)		\
	PSG_REG_##label_ = register_,
PSG_REGISTERS(PSG_REG_ENUM)
};

enum {
	PSG_IOMIX_OFF     = 0b111111,
	PSG_IOMIX_TONE_A  = 0b111110,
	PSG_IOMIX_TONE_B  = 0b111101,
	PSG_IOMIX_TONE_C  = 0b111011,
	PSG_IOMIX_NOISE_A = 0b110111,
	PSG_IOMIX_NOISE_B = 0b101111,
	PSG_IOMIX_NOISE_C = 0b011111
};

static inline uint8_t psg_rd(uint8_t reg)
{
	iowr8(reg, PSG_ADDR_RD_SELECT);

	return iord8(PSG_ADDR_RD_SELECT);
}

static inline void psg_wr(uint8_t value, uint8_t reg)
{
	iowr8(reg,   PSG_ADDR_RD_SELECT);
	iowr8(value, PSG_ADDR_WR);
}

static inline uint16_t psg_tone_period_freq(uint32_t freq)
{
	return DIV_ROUND_CLOSEST_U32(ATARI_STE_PSG_CLK, 16 * freq);
}

#define PSG_DEFINE_CHANNEL_F(ch, CH)					\
static inline void psg_wr_period_##ch(uint16_t period)			\
{									\
	psg_wr(period & 0xff, PSG_REG_PLO_##CH);			\
	psg_wr(period >> 8,   PSG_REG_PHI_##CH);			\
}									\
									\
static inline void psg_wr_freq_##ch(uint16_t freq)			\
{									\
	psg_wr_period_##ch(psg_tone_period_freq(freq));			\
}									\
									\
static inline void psg_wr_level_##ch(uint8_t level)			\
{									\
	psg_wr(level, PSG_REG_LEVEL_##CH);				\
}

PSG_DEFINE_CHANNEL_F(a, A)
PSG_DEFINE_CHANNEL_F(b, B)
PSG_DEFINE_CHANNEL_F(c, C)

static inline void psg_wr_iomix(uint8_t iomix)
{
	psg_wr(iomix, PSG_REG_IOMIX);
}

#endif /* TOSLIBC_ASM_SND_PSG_H */
