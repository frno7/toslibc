// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_SND_DMA_H
#define TOSLIBC_ASM_SND_DMA_H

#include <asm/io.h>

#define SND_DMA_REGISTERS(reg)						\
	reg( 0, ctrl,    CTRL,    "Sample control")			\
	reg( 1, starthi, STARTHI, "Sample start address high byte")	\
	reg( 2, startmi, STARTMI, "Sample start address middle byte")	\
	reg( 3, startlo, STARTLO, "Sample start address low byte")	\
	reg( 4, counthi, COUNTHI, "Sample count address high byte")	\
	reg( 5, countmi, COUNTMI, "Sample count address middle byte")	\
	reg( 6, countlo, COUNTLO, "Sample count address low byte")	\
	reg( 7, endhi,   ENDHI,   "Sample start address high byte")	\
	reg( 8, endmi,   ENDMI,   "Sample start address middle byte")	\
	reg( 9, endlo,   ENDLO,   "Sample start address low byte")	\
	reg(16, mode,    MODE,    "Sample mode")

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
			uint16_t mode : 2;
			uint16_t : 4;
			uint16_t frequency : 2;
		};
		uint16_t u16;
	};
};

#define SND_DMA_MODE(m)							\
	m(0, STEREO8)							\
	m(1, STEREO16)							\
	m(2, MONO8)

enum {
#define SND_DMA_MODE_ENUM(value, mode)					\
	SND_DMA_MODE_##mode = value,
SND_DMA_MODE(SND_DMA_MODE_ENUM)
};

#define SND_DMA_FREQUENCY(f)						\
	f(0, 6258)							\
	f(1, 12517)							\
	f(2, 25033)							\
	f(3, 50066)

enum {
#define SND_DMA_FREQUENCY_ENUM(value, frequency)			\
	SND_DMA_FREQUENCY_##frequency = value,
SND_DMA_FREQUENCY(SND_DMA_FREQUENCY_ENUM)
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
SND_DMA_REG_RD8_WR8(starthi, STARTHI)
SND_DMA_REG_RD8_WR8(startmi, STARTMI)
SND_DMA_REG_RD8_WR8(startlo, STARTLO)
SND_DMA_REG_RD8_WR8(counthi, COUNTHI)
SND_DMA_REG_RD8_WR8(countmi, COUNTMI)
SND_DMA_REG_RD8_WR8(countlo, COUNTLO)
SND_DMA_REG_RD8_WR8(endhi,   ENDHI)
SND_DMA_REG_RD8_WR8(endmi,   ENDMI)
SND_DMA_REG_RD8_WR8(endlo,   ENDLO)

static inline struct snd_dma_ctrl snd_dma_rd_ctrl()
{
	return (struct snd_dma_ctrl) { .u16 = snd_dma_rd16_ctrl() };
}

static inline void snd_dma_wr_ctrl(struct snd_dma_ctrl ctrl)
{
	snd_dma_wr16_ctrl(ctrl.u16);
}

#define snd_dma_wrs_ctrl(...) snd_dma_wr_ctrl((struct snd_dma_ctrl) __VA_ARGS__)

static inline uint32_t snd_dma_rd32_start()
{
	return (snd_dma_rd8_starthi() << 16) |
	       (snd_dma_rd8_startmi() <<  8) |
	        snd_dma_rd8_startlo();
}

static inline void *snd_dma_rd_start()
{
	return (void *)snd_dma_rd32_start();
}

static inline void snd_dma_wr32_start(uint32_t addr)
{
	snd_dma_wr8_starthi(addr >> 16);
	snd_dma_wr8_startmi(addr >>  8);
	snd_dma_wr8_startlo(addr);
}

static inline void snd_dma_wr_start(void *buffer)
{
	snd_dma_wr32_start((uint32_t)buffer);
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

static inline void snd_dma_wr_end(void *buffer)
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
