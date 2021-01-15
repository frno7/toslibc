// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2021 Fredrik Noring
 */

#ifndef _TOSLIBC_TOS_VDI_CALL_H
#define _TOSLIBC_TOS_VDI_CALL_H

#define __VDI_CALL(call)						\
	call(  1,  0, V_OPNWK)						\
	call(  2,  0, V_CLSWK)						\
	call(  3,  0, V_CLRWK)						\
	call(  4,  0, V_UPDWK)						\
	call(  5,  1, VQ_CHCELLS)					\
	call(  5,  2, V_EXIT_CUR)					\
	call(  5,  3, V_ENTER_CUR)					\
	call(  5,  4, V_CURUP)						\
	call(  5,  5, V_CURDOWN)					\
	call(  5,  6, V_CURRIGHT)					\
	call(  5,  7, V_CURLEFT)					\
	call(  5,  8, V_CURHOME)					\
	call(  5,  9, V_EEOS)						\
	call(  5, 10, V_EEOL)						\
	call(  5, 11, VS_CURADDRESS)					\
	call(  5, 12, V_CURTEXT)					\
	call(  5, 13, V_RVON)						\
	call(  5, 14, V_RVOFF)						\
	call(  5, 15, VQ_CURADDRESS)					\
	call(  5, 16, VQ_TABSTATUS)					\
	call(  5, 17, V_HARDCOPY)					\
	call(  5, 18, V_DSPCUR)						\
	call(  5, 19, V_RMCUR)						\
	call(  6,  0, V_PLINE)						\
	call(  7,  0, V_PMARKER)					\
	call(  8,  0, V_GTEXT)						\
	call(  9,  0, V_FILLAREA)					\
	call( 10,  0, V_CELLARRAY)					\
	call( 11,  1, V_BAR)						\
	call( 11,  2, V_ARC)						\
	call( 11,  3, V_PIESLICE)					\
	call( 11,  4, V_CIRCLE)						\
	call( 11,  5, V_ELLIPSE)					\
	call( 11,  6, V_ELLARC)						\
	call( 11,  7, V_ELLPIE)						\
	call( 11,  8, V_RBOX)						\
	call( 11,  9, V_RFBOX)						\
	call( 11, 10, V_JUSTIFIED)					\
	call( 12,  0, VST_HEIGHT)					\
	call( 13,  0, VST_ROTATION)					\
	call( 14,  0, VS_COLOR)						\
	call( 15,  0, VSL_TYPE)						\
	call( 16,  0, VSL_WIDTH)					\
	call( 17,  0, VSL_COLOR)					\
	call( 18,  0, VSM_TYPE)						\
	call( 19,  0, VSM_HEIGHT)					\
	call( 20,  0, VSM_COLOR)					\
	call( 21,  0, VST_FONT)						\
	call( 22,  0, VST_COLOR)					\
	call( 23,  0, VSF_INTERIOR)					\
	call( 24,  0, VSF_STYLE)					\
	call( 25,  0, VSF_COLOR)					\
	call( 26,  0, VQ_COLOR)						\
	call( 27,  0, VQ_CELLARRAY)					\
	call( 28,  0, V_LOCATOR)					\
	call( 29,  0, V_VALUATOR)					\
	call( 30,  0, V_CHOICE)						\
	call( 31,  0, V_STRING)						\
	call( 32,  0, VSWR_MODE)					\
	call( 33,  0, VSIN_MODE)					\
	call( 35,  0, VQL_ATTRIBUTES)					\
	call( 36,  0, VQM_ATTRIBUTES)					\
	call( 37,  0, VQF_ATTRIBUTES)					\
	call( 38,  0, VQT_ATTRIBUTES)					\
	call( 39,  0, VST_ALIGNMENT)					\
	call(100,  0, V_OPNVWK)						\
	call(101,  0, V_CLSVWK)						\
	call(102,  0, VQ_EXTND)						\
	call(103,  0, V_CONTOURFILL)					\
	call(104,  0, VSF_PERIMETER)					\
	call(105,  0, V_GET_PIXEL)					\
	call(106,  0, VST_EFFECTS)					\
	call(107,  0, VST_POINT)					\
	call(108,  0, VSL_ENDS)						\
	call(109,  0, VRO_CPYFM)					\
	call(110,  0, VR_TRNFM)						\
	call(111,  0, VSC_FORM)						\
	call(112,  0, VSF_UDPAT)					\
	call(113,  0, VSL_UDSTY)					\
	call(114,  0, VR_RECFL)						\
	call(115,  0, VQIN_MODE)					\
	call(116,  0, VQT_EXTENT)					\
	call(117,  0, VQT_WIDTH)					\
	call(118,  0, VEX_TIMV)						\
	call(119,  0, VST_LOAD_FONTS)					\
	call(120,  0, VST_UNLOAD_FONTS)					\
	call(121,  0, VRT_CPYFM)					\
	call(122,  0, V_SHOW_C)						\
	call(123,  0, V_HIDE_C)						\
	call(124,  0, VQ_MOUSE)						\
	call(125,  0, VEX_BUTV)						\
	call(126,  0, VEX_MOTV)						\
	call(127,  0, VEX_CURV)						\
	call(128,  0, VQ_KEY_S)						\
	call(129,  0, VS_CLIP)						\
	call(130,  0, VQT_NAME)						\
	call(131,  0, VQT_FONTINFO)

#endif /* _TOSLIBC_TOS_VDI_CALL_H */
