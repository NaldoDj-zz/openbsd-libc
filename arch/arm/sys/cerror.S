/*	$OpenBSD: cerror.S,v 1.3 2004/02/07 06:02:07 drahn Exp $	*/
/*	$NetBSD: cerror.S,v 1.5 2003/08/07 16:42:04 agc Exp $	*/

/*-
 * Copyright (c) 1990 The Regents of the University of California.
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
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: @(#)cerror.s	5.1 (Berkeley) 4/23/90
 */

#include "SYS.h"

ASENTRY(CERROR)
#ifdef _REENTRANT
	stmfd	sp!, {r4, lr}
	mov	r4, r0
	bl	PIC_SYM(_C_LABEL(__errno), PLT)
	str	r4, [r0]
	mvn	r0, #0x00000000
	mvn	r1, #0x00000000
	ldmfd	sp!, {r4, pc}
#else
#ifdef PIC
	/* Setup the GOT */
	ldr	r3, .Lgot
	ldr	r1, .Lerrno
.L1:
	add	r3, pc, r3
	ldr	r1, [r3, r1]
#else
	ldr	r1, .Lerrno
#endif /* PIC */
	str	r0, [r1]
	mvn	r0, #0x00000000
	mvn	r1, #0x00000000
	mov	pc, lr

#ifdef PIC
	.align	0
.Lgot:
	.word	_C_LABEL(_GLOBAL_OFFSET_TABLE_) - (.L1+8)
#endif /* PIC */

	.globl	_C_LABEL(errno)

.Lerrno:
	.word	PIC_SYM(_C_LABEL(errno), GOT)
#endif /* _REENTRANT */