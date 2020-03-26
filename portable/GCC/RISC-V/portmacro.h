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


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#if __riscv_xlen == 64
	#define portSTACK_TYPE			uint64_t
	#define portBASE_TYPE			int64_t
	#define portUBASE_TYPE			uint64_t
	#define portMAX_DELAY 			( TickType_t ) 0xffffffffffffffffUL
	#define portPOINTER_SIZE_TYPE 	uint64_t
#elif __riscv_xlen == 32
	#define portSTACK_TYPE	uint32_t
	#define portBASE_TYPE	int32_t
	#define portUBASE_TYPE	uint32_t
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#else
	#error Assembler did not define __riscv_xlen
#endif


typedef portSTACK_TYPE StackType_t;
typedef portBASE_TYPE BaseType_t;
typedef portUBASE_TYPE UBaseType_t;
typedef portUBASE_TYPE TickType_t;

/* Legacy type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short

/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
not need to be guarded with a critical section. */
#define portTICK_TYPE_IS_ATOMIC 1
/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#ifdef __riscv64
	#error This is the RV32 port that has not yet been adapted for 64.
	#define portBYTE_ALIGNMENT			16
#else
	#define portBYTE_ALIGNMENT			16
#endif
/*-----------------------------------------------------------*/


/* Supported privilege levels by embedded profile implementations.
It's likely embedded systems only supports Machine mode and/or User mode. */
#define portCONST_PRIVILEGE_USER		0UL
#define portCONST_PRIVILEGE_MACHINE		3UL

/* Machine status register (mstatus) related bit definitions.
RV32 and RV64 share the same definitions for below. */
#define portREG_MSTATUS_UIE_BIT			( 1UL )
#define portREG_MSTATUS_MIE_BIT			( 1UL << 3UL )

#define portREG_MSTATUS_MPP_BITS		( 3UL << 11UL )
#define portREG_MSTATUS_MPIE_BIT		( 1UL << 7UL )

/* Read-modify-write mstatus register to place caller task into user mode.
 Once the task is in user mode, it cannot return to machine mode. refer to
 https://www.freertos.org/portSWITCH_TO_USER_MODE.html */
void vPortSwitchToUserMode( void );
#define portSWITCH_TO_USER_MODE() vPortSwitchToUserMode();

/* The number of PMP entries.
 An implementation can have up to portCONST_PMP_ABSOLUTE_MAX_ENTRIES PMP regions.
 An implementation supports in total portCONST_PMP_PORT_MAX_ENTRIES PMP regions. */
#define portCONST_PMP_ABSOLUTE_MAX_ENTRIES	16UL	/* RISC-V supports up to 16 PMP entries. */
#define portCONST_PMP_PORT_MAX_ENTRIES		8UL		/* Implementation specific value. */

#if portCONST_PMP_PORT_MAX_ENTRIES > portCONST_PMP_ABSOLUTE_MAX_ENTRIES
	#error RISC-V can only support up to portCONST_PMP_ABSOLUTE_MAX_ENTRIES PMP entries.
#endif

/* PMP configuration register.
| bit7 | bit6-5      | bit4-3           | bit2      | bit1  | bit0 |
| Lock | 0 (padding) | Address matching | eXecution | Write | Read |
Lock: if locked, writes to the configuration register and associated address registers are ignored.
Address matching: see portCONST_PMP_ADDRESS_MODE_*.
eXecution, Write, Read: when set,  */

/* PMP lock configuration register.
 Locked PMP entries may only be unlocked with a system reset.*/
#define portCONST_PMP_LOCK_SET 				( 1UL << 7UL )

/* PMP address matching mode configuration. */
#define portCONST_PMP_ADDRESS_MODE_OFF		( 0UL << 3UL )	/* Null region (disabled). */
#define portCONST_PMP_ADDRESS_MODE_TOR		( 1UL << 3UL )	/* Top of range. */
#define portCONST_PMP_ADDRESS_MODE_NA4		( 2UL << 3UL )	/* Naturally aligned four-byte region. */
#define portCONST_PMP_ADDRESS_MODE_NAPOT	( 3UL << 3UL )	/* Naturally aligned power-of-two region, >=8bytes. */

/* PMP access type configuration.
 When corresponding bit is set, access of the type is allowed.
 The combination of R=0 && W=1 is reserved for future use. */
#define portCONST_PMP_ACCESS_EXECUTABLE		( 1UL << 2UL )
#define portCONST_PMP_ACCESS_WRITABLE		( 1UL << 1UL )
#define portCONST_PMP_ACCESS_READABLE		( 1UL )

/* PMP configuration initial setting -- allow all access. */
#define portCONST_PMP_CONFIG_INIT			( portCONST_PMP_ADDRESS_MODE_NAPOT | portCONST_PMP_ACCESS_EXECUTABLE | portCONST_PMP_ACCESS_WRITABLE | portCONST_PMP_ACCESS_READABLE)

/* PMP address initial value -- 0x0. */
#define portCONST_PMP_ADDRESS_INIT			( 1UL << (31UL + __riscv_xlen / 64UL ) * 32UL )


/*-----------------------------------------------------------*/


/* Scheduler utilities. */
extern void vTaskSwitchContext( void );
#define portYIELD() __asm volatile( "ecall" );
#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired ) vTaskSwitchContext()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/


/* Critical section management. */
#define portCRITICAL_NESTING_IN_TCB					1
extern void vTaskEnterCritical( void );
extern void vTaskExitCritical( void );

#define portSET_INTERRUPT_MASK_FROM_ISR() 0
#define portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedStatusValue ) ( void ) uxSavedStatusValue
#define portDISABLE_INTERRUPTS()	__asm volatile( "csrc mstatus, %0" :: "i" (portREG_MSTATUS_MIE_BIT) : "memory" )
#define portENABLE_INTERRUPTS()		__asm volatile( "csrs mstatus, %0" :: "i" (portREG_MSTATUS_MIE_BIT) : "memory" )
#define portENTER_CRITICAL()	vTaskEnterCritical()
#define portEXIT_CRITICAL()		vTaskExitCritical()

/*-----------------------------------------------------------*/

/* Architecture specific optimisations. */
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#endif

#if( configUSE_PORT_OPTIMISED_TASK_SELECTION == 1 )

	/* Check the configuration. */
	#if( configMAX_PRIORITIES > 32 )
		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
	#endif

	/* Store/clear the ready priorities in a bit map. */
	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )

	/*-----------------------------------------------------------*/

	#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31UL - __builtin_clz( uxReadyPriorities ) )

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */


/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site.  These are
not necessary for to use this port.  They are defined so the common demo files
(which build with all the ports) will build. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

/*-----------------------------------------------------------*/

#define portNOP() __asm volatile 	( " nop " )

#define portINLINE	__inline

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE inline __attribute__(( always_inline))
#endif

#define portMEMORY_BARRIER() __asm volatile( "" ::: "memory" )
/*-----------------------------------------------------------*/


/* configCLINT_BASE_ADDRESS is a legacy definition that was replaced by the
configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS definitions.  For
backward compatibility derive the newer definitions from the old if the old
definition is found. */
#if defined( configCLINT_BASE_ADDRESS ) && !defined( configMTIME_BASE_ADDRESS ) && ( configCLINT_BASE_ADDRESS == 0 )
	/* Legacy case where configCLINT_BASE_ADDRESS was defined as 0 to indicate
	there was no CLINT.  Equivalent now is to set the MTIME and MTIMECMP
	addresses to 0. */
	#define configMTIME_BASE_ADDRESS 	( 0 )
	#define configMTIMECMP_BASE_ADDRESS ( 0 )
#elif defined( configCLINT_BASE_ADDRESS ) && !defined( configMTIME_BASE_ADDRESS )
	/* Legacy case where configCLINT_BASE_ADDRESS was set to the base address of
	the CLINT.  Equivalent now is to derive the MTIME and MTIMECMP addresses
	from the CLINT address. */
	#define configMTIME_BASE_ADDRESS 	( ( configCLINT_BASE_ADDRESS ) + 0xBFF8UL )
	#define configMTIMECMP_BASE_ADDRESS ( ( configCLINT_BASE_ADDRESS ) + 0x4000UL )
#elif !defined( configMTIME_BASE_ADDRESS ) || !defined( configMTIMECMP_BASE_ADDRESS )
	#error configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  Set them to zero if there is no MTIME (machine time) clock.  See https://www.freertos.org/Using-FreeRTOS-on-RISC-V.html
#endif



#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

