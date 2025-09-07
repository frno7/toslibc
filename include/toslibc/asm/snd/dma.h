// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_SND_DMA_H
#define TOSLIBC_ASM_SND_DMA_H

#include <asm/io.h>

#define SND_DMA_REGISTERS(reg)						\
	reg( 0, ctrl,      CTRL,      "Sound DMA control")		\
	reg( 1, basehi,	   BASEHI,    "Frame base address high byte")	\
	reg( 2, basemi,	   BASEMI,    "Frame base address middle byte")	\
	reg( 3, baselo,	   BASELO,    "Frame base address low byte")	\
	reg( 4, counterhi, COUNTERHI, "Frame address counter high byte") \
	reg( 5, countermi, COUNTERMI, "Frame address counter middle byte") \
	reg( 6, counterlo, COUNTERLO, "Frame address counter low byte")	\
	reg( 7, endhi,     ENDHI,     "Frame end address high byte")	\
	reg( 8, endmi,     ENDMI,     "Frame end address middle byte")	\
	reg( 9, endlo,     ENDLO,     "Frame end address low byte")	\
	reg(16, mode,      MODE,      "Sound mode control")

enum snd_dma_reg {
#define SND_DMA_REG_ENUM(register, symbol, label, description)		\
	SND_DMA_ADDR_##label = 0xff8900 | (register << 1),
SND_DMA_REGISTERS(SND_DMA_REG_ENUM)
};

struct snd_dma_ctrl {
	union {
		struct {
			uint16_t : 14;
			uint16_t play_repeat : 1;
			uint16_t play : 1;
		};
		uint16_t u16;
	};
};

struct snd_dma_mode {
	union {
		struct {
			uint16_t : 8;
			uint16_t format : 2;
			uint16_t : 4;
			uint16_t rate : 2;
		};
		uint16_t u16;
	};
};

#define SND_DMA_MODE_FORMAT(m)						\
	m(0, STEREO8)							\
	m(1, STEREO16)							\
	m(2, MONO8)

enum {
#define SND_DMA_MODE_FORMAT_ENUM(value, format)				\
	SND_DMA_MODE_FORMAT_##format = value,
SND_DMA_MODE_FORMAT(SND_DMA_MODE_FORMAT_ENUM)
};

#define SND_DMA_MODE_RATE(r)						\
	r(0, 6258)							\
	r(1, 12517)							\
	r(2, 25033)							\
	r(3, 50066)

enum {
#define SND_DMA_MODE_FREQUENCY_ENUM(value, frequency)			\
	SND_DMA_MODE_FREQUENCY_##frequency = value,
SND_DMA_MODE_RATE(SND_DMA_MODE_FREQUENCY_ENUM)
};

#define SND_DMA_REG_RD16_WR16(symbol, label)				\
	static inline uint16_t snd_dma_rd16_##symbol()			\
	{								\
		return iord16(SND_DMA_ADDR_##label);			\
	}								\
	static inline void snd_dma_wr16_##symbol(uint16_t value)	\
	{								\
		return iowr16(value, SND_DMA_ADDR_##label);		\
	}
SND_DMA_REG_RD16_WR16(ctrl, CTRL)
SND_DMA_REG_RD16_WR16(mode, MODE)

#define SND_DMA_REG_RD8_WR8(symbol, label)				\
	static inline uint8_t snd_dma_rd8_##symbol()			\
	{								\
		return iord8(SND_DMA_ADDR_##label + 1);			\
	}								\
	static inline void snd_dma_wr8_##symbol(uint8_t value)		\
	{								\
		return iowr8(value, SND_DMA_ADDR_##label + 1);		\
	}
SND_DMA_REG_RD8_WR8(basehi,    BASEHI)
SND_DMA_REG_RD8_WR8(basemi,    BASEMI)
SND_DMA_REG_RD8_WR8(baselo,    BASELO)
SND_DMA_REG_RD8_WR8(counterhi, COUNTERHI)
SND_DMA_REG_RD8_WR8(countermi, COUNTERMI)
SND_DMA_REG_RD8_WR8(counterlo, COUNTERLO)
SND_DMA_REG_RD8_WR8(endhi,     ENDHI)
SND_DMA_REG_RD8_WR8(endmi,     ENDMI)
SND_DMA_REG_RD8_WR8(endlo,     ENDLO)

static inline struct snd_dma_ctrl snd_dma_rd_ctrl()
{
	return (struct snd_dma_ctrl) { .u16 = snd_dma_rd16_ctrl() };
}

static inline void snd_dma_wr_ctrl(struct snd_dma_ctrl ctrl)
{
	snd_dma_wr16_ctrl(ctrl.u16);
}

#define snd_dma_wrs_ctrl(...) snd_dma_wr_ctrl((struct snd_dma_ctrl) __VA_ARGS__)

static inline uint32_t snd_dma_rd32_base()
{
	return (snd_dma_rd8_basehi() << 16) |
	       (snd_dma_rd8_basemi() <<  8) |
	        snd_dma_rd8_baselo();
}

static inline void *snd_dma_rd_base()
{
	return (void *)snd_dma_rd32_base();
}

static inline void snd_dma_wr32_base(uint32_t addr)
{
	snd_dma_wr8_basehi(addr >> 16);
	snd_dma_wr8_basemi(addr >>  8);
	snd_dma_wr8_baselo(addr);
}

static inline void snd_dma_wr_base(const void *buffer)
{
	snd_dma_wr32_base((uint32_t)buffer);
}

static inline uint32_t snd_dma_rd32_count()
{
	return (snd_dma_rd8_counthi() << 16) |
	       (snd_dma_rd8_countmi() <<  8) |
	        snd_dma_rd8_countlo();
}

static inline void *snd_dma_rd_count()
{
	return (void *)snd_dma_rd32_count();
}

static inline uint32_t snd_dma_rd32_end()
{
	return (snd_dma_rd8_endhi() << 16) |
	       (snd_dma_rd8_endmi() <<  8) |
	        snd_dma_rd8_endlo();
}

static inline void *snd_dma_rd_end()
{
	return (void *)snd_dma_rd32_end();
}

static inline void snd_dma_wr32_end(uint32_t addr)
{
	snd_dma_wr8_endhi(addr >> 16);
	snd_dma_wr8_endmi(addr >>  8);
	snd_dma_wr8_endlo(addr);
}

static inline void snd_dma_wr_end(const void *buffer)
{
	snd_dma_wr32_end((uint32_t)buffer);
}

static inline struct snd_dma_mode snd_dma_rd_mode()
{
	return (struct snd_dma_mode) { .u16 = snd_dma_rd16_mode() };
}

static inline void snd_dma_wr_mode(struct snd_dma_mode mode)
{
	snd_dma_wr16_mode(mode.u16);
}

#define snd_dma_wrs_mode(...) snd_dma_wr_mode((struct snd_dma_mode) __VA_ARGS__)

#endif /* TOSLIBC_ASM_SND_DMA_H */
