/*	$OpenBSD: brk.S,v 1.1 2005/04/01 10:54:27 mickey Exp $	*/

/*
 * Copyright (c) 1999 Michael Shalayeff
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR OR HIS RELATIVES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF MIND, USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "SYS.h"

#ifdef SYSLIBC_SCCS
	.text
	.asciz "$OpenBSD: brk.S,v 1.1 2005/04/01 10:54:27 mickey Exp $"
	.align	4
#endif

	.import	__curbrk, data
	.import	end, data

	.data
	.export	minbrk, data
minbrk
	.long	end

SYSENTRY(brk,0)
#ifdef PIC
	addil	LT%minbrk, r19
	ldd	RT%minbrk(r1), t1
	ldd	0(t1), t2
#else
	ldil	L%minbrk, t1
	ldd	R%minbrk(t1), t2
#endif
	sub,<<	t2, arg0, r0
	copy	t2, arg0
	std	arg0, HPPA_FRAME_ARG(0)(sp)

	SYSCALL(break)

	ldd	HPPA_FRAME_ARG(0)(sp), arg0
#ifdef PIC
	addil	LT%__curbrk, r19
	ldd	RT%__curbrk(r1), t1
	bv	r0(rp)
	std     arg0, 0(t1)
#else
	ldil	L%__curbrk, t1
	bv	r0(rp)
	std	arg0, R%__curbrk(t1)
#endif
SYSEXIT(brk)

	.end
