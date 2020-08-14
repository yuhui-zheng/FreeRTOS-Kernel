/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* Syscall request number. */

/*-----------------------------------------------------------*/
/* ecall calling convention.
 * Similar to pk, using a7 to pass in syscall number, a0-a5 for arguments, and a0 for return. */

#ifndef __FREERTOS_RISC_V_SYSCALL_NUMBER_H__
#define __FREERTOS_RISC_V_SYSCALL_NUMBER_H__

/* Check is done from lowest number to highest. */
#define SYSCALL_SELF_YIELD				(0U)		/* Used by portYIELD. */
#define SYSCALL_ENABLE_M_IRQ			(1U)		/* Enable M-mode interrupts. */
#define SYSCALL_DISABLE_M_IRQ			(2U)		/* Disable M-mode interrupts. */
#define SYSCALL_ACCESS_REQUEST			(3U)		/* Used by MPU wrappers to grant access to kernel APIs. */
#define SYSCALL_REMOVE_ACCESS			(4U)		/* Used by MPU wrappers to remove access to kernel API. */



#endif /* __FREERTOS_RISC_V_SYSCALL_NUMBER_H__ */
