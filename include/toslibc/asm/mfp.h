// SPDX-License-Identifier: LGPL-2.1

#ifndef TOSLIBC_ASM_MFP_H
#define TOSLIBC_ASM_MFP_H

#include <stdint.h>

#include <asm/io.h>

#define MFP_REGISTERS(mfp)						\
	mfp( 0, gpip,  GPIP,  "General-purpose I/O data register")	\
	mfp( 1, aer,   AER,   "Active edge register")			\
	mfp( 2, ddr,   DDR,   "Data direction register")		\
	mfp( 3, iera,  IERA,  "Interrupt enable register A")		\
	mfp( 4, ierb,  IERB,  "Interrupt enable register B")		\
	mfp( 5, ipra,  IPRA,  "Interrupt pending register A")		\
	mfp( 6, iprb,  IPRB,  "Interrupt pending register B")		\
	mfp( 7, isra,  ISRA,  "Interrupt in-service register A")	\
	mfp( 8, isrb,  ISRB,  "Interrupt in-service register B")	\
	mfp( 9, imra,  IMRA,  "Interrupt mask register A")		\
	mfp(10, imrb,  IMRB,  "Interrupt mask register B")		\
	mfp(11, vr,    VR,    "Vector register")			\
	mfp(12, tacr,  TACR,  "Timer A control register")		\
	mfp(13, tbcr,  TBCR,  "Timer B control register")		\
	mfp(14, tcdcr, TCDCR, "Timers C and D control register")	\
	mfp(15, tadr,  TADR,  "Timer A data register")			\
	mfp(16, tbdr,  TBDR,  "Timer B data register")			\
	mfp(17, tcdr,  TCDR,  "Timer C data register")			\
	mfp(18, tddr,  TDDR,  "Timer D data register")			\
	mfp(19, scr,   SCR,   "Synchronous character register")		\
	mfp(20, ucr,   UCR,   "USART control register")			\
	mfp(21, rsr,   RSR,   "Receiver status register")		\
	mfp(22, tsr,   TSR,   "Transmitter status register")		\
	mfp(23, udr,   UDR,   "USART data register")

enum mfp_reg {
#define MFP_REG_ENUM(register, symbol, label, description)		\
	MFP_REG_##label = register,
MFP_REGISTERS(MFP_REG_ENUM)
};

enum mfp_addr {
	MFP_ADDR_BASE = 0xfffa00,
#define MFP_ADDR_ENUM(register, symbol, label, description)		\
	MFP_ADDR_##label = MFP_ADDR_BASE + 1 + 2 * register,
MFP_REGISTERS(MFP_ADDR_ENUM)
};

#define MFP_CTRL_DIV(div)						\
	div(4)								\
	div(10)								\
	div(16)								\
	div(50)								\
	div(64)								\
	div(100)							\
	div(200)

enum mfp_ctrl {
	MFP_CTRL_STOP,
#define MFP_CTRL_DIV_ENUM(div)						\
	MFP_CTRL_DIV_##div,
MFP_CTRL_DIV(MFP_CTRL_DIV_ENUM)
};

#define MFP_DEFINE_PORT(type)						\
struct mfp_##type {							\
	union {								\
		struct {						\
			uint8_t mono_monitor_detect : 1;		\
			uint8_t ring_indicator : 1;			\
			uint8_t fdc_hdc : 1;				\
			uint8_t acia : 1;				\
			uint8_t blitter_done : 1;			\
			uint8_t clear_to_send : 1;			\
			uint8_t data_carrier_detect : 1;		\
			uint8_t centronics_busy : 1;			\
		};							\
		uint8_t u8;						\
	};								\
}

#define MFP_CHANNELS_A(c)						\
	c(7, mono_monitor_detect)					\
	c(6, ring_indicator)						\
	c(5, timer_a)							\
	c(4, rx_buffer_full)						\
	c(3, rx_error)							\
	c(2, tx_buffer_empty)						\
	c(1, tx_error)							\
	c(0, timer_b)

#define MFP_CHANNELS_B(c)						\
	c(7, fdc_hdc)							\
	c(6, acia)							\
	c(5, timer_c)							\
	c(4, timer_d)							\
	c(3, blitter_ready)						\
	c(2, clear_to_send)						\
	c(1, data_carrier_detect)					\
	c(0, centronics_busy)

#define MFP_CHANNEL_U8(bit, symbol)					\
	uint8_t symbol : 1;

#define MFP_CHANNEL_U16(bit, symbol)					\
	uint16_t symbol : 1;

#define MFP_DEFINE_IRA(type)						\
struct mfp_##type {							\
	union {								\
		struct {						\
			MFP_CHANNELS_A(MFP_CHANNEL_U8)			\
		};							\
		uint8_t u8;						\
	};								\
}

#define MFP_DEFINE_IRB(type)						\
struct mfp_##type {							\
	union {								\
		struct {						\
			MFP_CHANNELS_B(MFP_CHANNEL_U8)			\
		};							\
		uint8_t u8;						\
	};								\
}

#define MFP_DEFINE_IR(type)						\
struct mfp_##type {							\
	union {								\
		struct {						\
			MFP_CHANNELS_A(MFP_CHANNEL_U16)			\
			MFP_CHANNELS_B(MFP_CHANNEL_U16)			\
		};							\
		struct {						\
			struct mfp_##type##a a;				\
			struct mfp_##type##b b;				\
		};							\
		uint8_t u16;						\
	};								\
}

#define MFP_DEFINE_TABCR(type)						\
struct mfp_##type {							\
	union {								\
		struct {						\
			uint8_t : 3;					\
			uint8_t reset : 1;				\
			uint8_t event : 1;				\
			uint8_t ctrl : 3;				\
		};							\
		uint8_t u8;						\
	};								\
}

struct mfp_tcdcr {
	union {
		struct {
			uint8_t : 1;
			uint8_t tc_ctrl : 3;
			uint8_t : 1;
			uint8_t td_ctrl : 3;
		};
		uint8_t u8;
	};
};

/* Virtual timer C control register for struct mfp_tcdcr. */
struct mfp_tccr {
	union {
		struct {
			uint8_t : 1;
			uint8_t ctrl : 3;
			uint8_t : 4;
		};
		uint8_t u8;
	};
};

/* Virtual timer D control register for struct mfp_tcdcr. */
struct mfp_tdcr {
	union {
		struct {
			uint8_t : 5;
			uint8_t ctrl : 3;
		};
		uint8_t u8;
	};
};

#define MFP_DEFINE_TDR(type)						\
struct mfp_##type {							\
	union {								\
		struct {						\
			uint8_t count;					\
		};							\
		uint8_t u8;						\
	};								\
}

MFP_DEFINE_PORT(gpip);
MFP_DEFINE_PORT(aer);
MFP_DEFINE_PORT(ddr);

MFP_DEFINE_IRA(iera);
MFP_DEFINE_IRB(ierb);
MFP_DEFINE_IRA(ipra);
MFP_DEFINE_IRB(iprb);
MFP_DEFINE_IRA(isra);
MFP_DEFINE_IRB(isrb);
MFP_DEFINE_IRA(imra);
MFP_DEFINE_IRB(imrb);

MFP_DEFINE_IR(ier);
MFP_DEFINE_IR(ipr);
MFP_DEFINE_IR(isr);
MFP_DEFINE_IR(imr);

MFP_DEFINE_TABCR(tacr);
MFP_DEFINE_TABCR(tbcr);

MFP_DEFINE_TDR(tadr);
MFP_DEFINE_TDR(tbdr);
MFP_DEFINE_TDR(tcdr);
MFP_DEFINE_TDR(tddr);

#define MFP_DEFINE_RDWR_IEMRAB(symbol, label)				\
static inline uint8_t mfp_rd8_##symbol()				\
{									\
	return iord8(MFP_ADDR_##label);					\
}									\
									\
static inline struct mfp_##symbol mfp_rd_##symbol()			\
{									\
	return (struct mfp_##symbol) { .u8 = mfp_rd8_##symbol() };	\
}									\
									\
static inline void mfp_set8_##symbol(uint8_t mask)			\
{									\
	ioor8(mask, MFP_ADDR_##label);					\
}									\
									\
static inline void mfp_set_##symbol(struct mfp_##symbol symbol)		\
{									\
	mfp_set8_##symbol(symbol.u8);					\
}									\
									\
static inline void mfp_clr8_##symbol(uint8_t mask)			\
{									\
	ioand8(~mask, MFP_ADDR_##label);				\
}									\
									\
static inline void mfp_clr_##symbol(struct mfp_##symbol symbol)		\
{									\
	mfp_clr8_##symbol(symbol.u8);					\
}

MFP_DEFINE_RDWR_IEMRAB(iera, IERA);
MFP_DEFINE_RDWR_IEMRAB(ierb, IERB);
MFP_DEFINE_RDWR_IEMRAB(imra, IMRA);
MFP_DEFINE_RDWR_IEMRAB(imrb, IMRB);

#define mfp_sets_iera(...) mfp_set_iera((struct mfp_iera) __VA_ARGS__)
#define mfp_sets_ierb(...) mfp_set_ierb((struct mfp_ierb) __VA_ARGS__)
#define mfp_sets_imra(...) mfp_set_imra((struct mfp_imra) __VA_ARGS__)
#define mfp_sets_imrb(...) mfp_set_imrb((struct mfp_imrb) __VA_ARGS__)

#define mfp_clrs_iera(...) mfp_clr_iera((struct mfp_iera) __VA_ARGS__)
#define mfp_clrs_ierb(...) mfp_clr_ierb((struct mfp_ierb) __VA_ARGS__)
#define mfp_clrs_imra(...) mfp_clr_imra((struct mfp_imra) __VA_ARGS__)
#define mfp_clrs_imrb(...) mfp_clr_imrb((struct mfp_imrb) __VA_ARGS__)

#define MFP_DEFINE_RDWR_IEMR(symbol, label)				\
static inline struct mfp_##symbol mfp_rd_##symbol()			\
{									\
	/* FIXME: Can this be optimised with a 32-bit read? */		\
	return (struct mfp_##symbol) {					\
		.a = mfp_rd_##symbol##a(),				\
		.b = mfp_rd_##symbol##b()				\
	};								\
}									\
									\
static inline void mfp_set16_##symbol(uint16_t mask)			\
{									\
	/* FIXME: Is the bitorder correct? */				\
	const uint8_t a = mask >> 8;					\
	const uint8_t b = mask & 0xff;					\
									\
	/* FIXME: Can this be optimised with a 32-bit write? */		\
	if (a) mfp_set8_##symbol##a(a);					\
	if (b) mfp_set8_##symbol##b(b);					\
}									\
									\
static inline void mfp_set_##symbol(struct mfp_##symbol symbol)		\
{									\
	/* FIXME: Can this be optimised with a 32-bit write? */		\
	if (symbol.a.u8) mfp_set_##symbol##a(symbol.a);			\
	if (symbol.b.u8) mfp_set_##symbol##b(symbol.b);			\
}									\
									\
static inline void mfp_clr16_##symbol(uint16_t mask)			\
{									\
	/* FIXME: Is the bitorder correct? */				\
	const uint8_t a = mask >> 8;					\
	const uint8_t b = mask & 0xff;					\
									\
	/* FIXME: Can this be optimised with a 32-bit write? */		\
	if (a) mfp_clr8_##symbol##a(a);					\
	if (b) mfp_clr8_##symbol##b(b);					\
}									\
									\
static inline void mfp_clr_##symbol(struct mfp_##symbol symbol)		\
{									\
	/* FIXME: Can this be optimised with a 32-bit write? */		\
	if (symbol.a.u8) mfp_clr_##symbol##a(symbol.a);			\
	if (symbol.b.u8) mfp_clr_##symbol##b(symbol.b);			\
}

#define mfp_sets_ier(...) mfp_set_ier((struct mfp_ier) __VA_ARGS__)
#define mfp_sets_imr(...) mfp_set_imr((struct mfp_imr) __VA_ARGS__)

#define mfp_clrs_ier(...) mfp_clr_ier((struct mfp_ier) __VA_ARGS__)
#define mfp_clrs_imr(...) mfp_clr_imr((struct mfp_imr) __VA_ARGS__)

MFP_DEFINE_RDWR_IEMR(ier, IER);
MFP_DEFINE_RDWR_IEMR(imr, IMR);

#define MFP_DEFINE_CLR_IPSRAB(symbol, label)				\
static inline void mfp_clr8_##symbol(uint8_t mask)			\
{									\
	iowr8(~mask, MFP_ADDR_##label);					\
}									\
									\
static inline void mfp_clr_##symbol(struct mfp_##symbol symbol)		\
{									\
	mfp_clr8_##symbol(symbol.u8);					\
}

MFP_DEFINE_CLR_IPSRAB(ipra, IPRA)
MFP_DEFINE_CLR_IPSRAB(iprb, IPRB)
MFP_DEFINE_CLR_IPSRAB(isra, ISRA)
MFP_DEFINE_CLR_IPSRAB(isrb, ISRB)

#define mfp_clrs_ipra(...) mfp_clr_ipra((struct mfp_ipra) __VA_ARGS__)
#define mfp_clrs_iprb(...) mfp_clr_iprb((struct mfp_iprb) __VA_ARGS__)
#define mfp_clrs_isra(...) mfp_clr_isra((struct mfp_isra) __VA_ARGS__)
#define mfp_clrs_isrb(...) mfp_clr_isrb((struct mfp_isrb) __VA_ARGS__)

#define MFP_DEFINE_CLR_IPSR(symbol, label)				\
static inline void mfp_clr16_##symbol(uint16_t mask)			\
{									\
	/* FIXME: Is the bitorder correct? */				\
	const uint8_t a = mask >> 8;					\
	const uint8_t b = mask & 0xff;					\
									\
	/* FIXME: Can this be optimised with a 32-bit write? */		\
	if (a) mfp_clr8_##symbol##a(a);					\
	if (b) mfp_clr8_##symbol##b(b);					\
}									\
									\
static inline void mfp_clr_##symbol(struct mfp_##symbol symbol)		\
{									\
	/* FIXME: Can this be optimised with a 32-bit write? */		\
	if (symbol.a.u8) mfp_clr_##symbol##a(symbol.a);			\
	if (symbol.b.u8) mfp_clr_##symbol##b(symbol.b);			\
}

#define mfp_clrs_ipr(...) mfp_clr_ipr((struct mfp_ipr) __VA_ARGS__)
#define mfp_clrs_isr(...) mfp_clr_isr((struct mfp_isr) __VA_ARGS__)

MFP_DEFINE_CLR_IPSR(ipr, IPR);
MFP_DEFINE_CLR_IPSR(isr, ISR);

#define MFP_DEFINE_RDWR_U8S(symbol, label)				\
static inline uint8_t mfp_rd8_##symbol()				\
{									\
	return iord8(MFP_ADDR_##label);					\
}									\
									\
static inline struct mfp_##symbol mfp_rd_##symbol()			\
{									\
	return (struct mfp_##symbol) { .u8 = mfp_rd8_##symbol() };	\
}									\
									\
static inline void mfp_wr8_##symbol(uint8_t symbol)			\
{									\
	iowr8(symbol, MFP_ADDR_##label);				\
}									\
									\
static inline void mfp_wr_##symbol(struct mfp_##symbol symbol)		\
{									\
	mfp_wr8_##symbol(symbol.u8);					\
}

MFP_DEFINE_RDWR_U8S(tacr, TACR)
MFP_DEFINE_RDWR_U8S(tbcr, TBCR)

MFP_DEFINE_RDWR_U8S(tcdcr, TCDCR)

MFP_DEFINE_RDWR_U8S(tadr, TADR)
MFP_DEFINE_RDWR_U8S(tbdr, TBDR)
MFP_DEFINE_RDWR_U8S(tcdr, TCDR)
MFP_DEFINE_RDWR_U8S(tddr, TDDR)

#define mfp_wrs_tacr(...) mfp_wr_tacr((struct mfp_tacr) __VA_ARGS__)
#define mfp_wrs_tbcr(...) mfp_wr_tbcr((struct mfp_tbcr) __VA_ARGS__)

#define mfp_wrs_tcdcr(...) mfp_wr_tcdcr((struct mfp_tcdcr) __VA_ARGS__)

#define mfp_wrs_tadr(...) mfp_wr_tadr((struct mfp_tadr) __VA_ARGS__)
#define mfp_wrs_tbdr(...) mfp_wr_tbdr((struct mfp_tbdr) __VA_ARGS__)
#define mfp_wrs_tcdr(...) mfp_wr_tcdr((struct mfp_tcdr) __VA_ARGS__)
#define mfp_wrs_tddr(...) mfp_wr_tddr((struct mfp_tddr) __VA_ARGS__)

static inline struct mfp_tccr mfp_rd_tccr()
{
	return (struct mfp_tccr) { .ctrl = mfp_rd_tcdcr().tc_ctrl };
}

static inline struct mfp_tdcr mfp_rd_tdcr()
{
	return (struct mfp_tdcr) { .ctrl = mfp_rd_tcdcr().td_ctrl };
}

static inline void mfp_wr_tccr(struct mfp_tccr tccr)
{
	ioxor8((mfp_rd_tccr().u8 ^ tccr.u8) & 0x70, MFP_ADDR_TCDCR);
}

static inline void mfp_wr_tdcr(struct mfp_tdcr tdcr)
{
	ioxor8((mfp_rd_tdcr().u8 ^ tdcr.u8) & 0x07, MFP_ADDR_TCDCR);
}

#define mfp_wrs_tccr(...) mfp_wr_tccr((struct mfp_tccr) __VA_ARGS__)
#define mfp_wrs_tdcr(...) mfp_wr_tdcr((struct mfp_tdcr) __VA_ARGS__)

#endif /* TOSLIBC_ASM_MFP_H */
