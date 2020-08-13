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

/*-----------------------------------------------------------
* Implementation of functions defined in portable.h for the RISC-V RV32 port.
*----------------------------------------------------------*/

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Standard includes. */
#include "string.h"

#ifdef configCLINT_BASE_ADDRESS
    #warning The configCLINT_BASE_ADDRESS constant has been deprecated.  configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS are currently being derived from the (possibly 0) configCLINT_BASE_ADDRESS setting.  Please update to define configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS dirctly in place of configCLINT_BASE_ADDRESS.  See https: /*www.freertos.org/Using-FreeRTOS-on-RISC-V.html */
#endif

#ifndef configMTIME_BASE_ADDRESS
    #warning configMTIME_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtime register then set configMTIME_BASE_ADDRESS to the mapped address.  Otherwise set configMTIME_BASE_ADDRESS to 0.  See https: /*www.freertos.org/Using-FreeRTOS-on-RISC-V.html */
#endif

#ifndef configMTIMECMP_BASE_ADDRESS
    #warning configMTIMECMP_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtimecmp register then set configMTIMECMP_BASE_ADDRESS to the mapped address.  Otherwise set configMTIMECMP_BASE_ADDRESS to 0.  See https: /*www.freertos.org/Using-FreeRTOS-on-RISC-V.html */
#endif

/* Let the user override the pre-loading of the initial LR with the address of
 * prvTaskExitError() in case it messes up unwinding of the stack in the
 * debugger. */
#ifdef configTASK_RETURN_ADDRESS
    #define portTASK_RETURN_ADDRESS    configTASK_RETURN_ADDRESS
#else
    #define portTASK_RETURN_ADDRESS    prvTaskExitError
#endif

/* The stack used by interrupt service routines.  Set configISR_STACK_SIZE_WORDS
 * to use a statically allocated array as the interrupt stack.  Alternative leave
 * configISR_STACK_SIZE_WORDS undefined and update the linker script so that a
 * linker variable names __freertos_irq_stack_top has the same value as the top
 * of the stack used by main.  Using the linker script method will repurpose the
 * stack that was used by main before the scheduler was started for use as the
 * interrupt stack after the scheduler has started. */
#ifdef configISR_STACK_SIZE_WORDS
    static __attribute__( ( aligned( 16 ) ) ) StackType_t xISRStack[ configISR_STACK_SIZE_WORDS ] = { 0 };
    const StackType_t xISRStackTop = ( StackType_t ) &( xISRStack[ configISR_STACK_SIZE_WORDS & ~portBYTE_ALIGNMENT_MASK ] );

/* Don't use 0xa5 as the stack fill bytes as that is used by the kernerl for
 * the task stacks, and so will legitimately appear in many positions within
 * the ISR stack. */
    #define portISR_STACK_FILL_BYTE    0xee
#else
    extern const uint32_t __freertos_irq_stack_top[];
    const StackType_t xISRStackTop = ( StackType_t ) __freertos_irq_stack_top;
#endif

/* misa bit mapping.
 * These are mapped the same for RV32 and RV64.
 */
#define MISA_EXTENSION_U			( 1UL << 20UL )

/* mstatus bit mapping.
 * These are mapped the same for RV32 and RV64.
 * Most embedded profiles have M-mode and optionally U-mode.
 * U-mode mstatus.mpp = 00 and M-mode mstatus.mpp = 11
 */
#define MSTATUS_MPRV_BIT_MASK 		( 1UL << 17UL )
#define MSTATUS_MPP_BITS_MASK		( 3UL << 11UL )
#define MSTATUS_MPIE_BIT_MASK		( 1UL << 7UL )
#define MSTATUS_UPIE_BIT_MASK		( 1UL << 4UL )
#define MSTATUS_MIE_BIT_MASK		( 1UL << 3UL )
#define MSTATUS_UIE_BIT_MASK		( 1UL << 0UL )


/*
 * Setup the timer to generate the tick interrupts.  The implementation in this
 * file is weak to allow application writers to change the timer used to
 * generate the tick interrupt.
 */
void vPortSetupTimerInterrupt( void ) __attribute__( ( weak ) );

/*-----------------------------------------------------------*/

/* Used to program the machine timer compare register. */
uint64_t ullNextTime = 0ULL;
const uint64_t * pullNextTime = &ullNextTime;
const size_t uxTimerIncrementsForOneTick = ( size_t ) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ); /* Assumes increment won't go over 32-bits. */
uint32_t const ullMachineTimerCompareRegisterBase = configMTIMECMP_BASE_ADDRESS;
volatile uint64_t * pullMachineTimerCompareRegister = NULL;

/* Set configCHECK_FOR_STACK_OVERFLOW to 3 to add ISR stack checking to task
 * stack checking.  A problem in the ISR stack will trigger an assert, not call the
 * stack overflow hook function (because the stack overflow hook is specific to a
 * task stack, not the ISR stack). */
#if defined( configISR_STACK_SIZE_WORDS ) && ( configCHECK_FOR_STACK_OVERFLOW > 2 )
    #warning This path not tested, or even compiled yet.

    static const uint8_t ucExpectedStackBytes[] =
    {
        portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, \
        portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, \
        portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, \
        portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, \
        portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE
    }; \

    #define portCHECK_ISR_STACK()    configASSERT( ( memcmp( ( void * ) xISRStack, ( void * ) ucExpectedStackBytes, sizeof( ucExpectedStackBytes ) ) == 0 ) )
#else /* if defined( configISR_STACK_SIZE_WORDS ) && ( configCHECK_FOR_STACK_OVERFLOW > 2 ) */
    /* Define the function away. */
    #define portCHECK_ISR_STACK()
#endif /* configCHECK_FOR_STACK_OVERFLOW > 2 */

/*-----------------------------------------------------------*/

#if ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 )

    void vPortSetupTimerInterrupt( void )
    {
        uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;
        volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( ( configMTIME_BASE_ADDRESS ) + 4UL ); /* 8-byte typer so high 32-bit word is 4 bytes up. */
        volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( configMTIME_BASE_ADDRESS );
        volatile uint32_t ulHartId;

        __asm volatile ( "csrr %0, mhartid" : "=r" ( ulHartId ) );

        pullMachineTimerCompareRegister = ( volatile uint64_t * ) ( ullMachineTimerCompareRegisterBase + ( ulHartId * sizeof( uint64_t ) ) );

        do
        {
            ulCurrentTimeHigh = *pulTimeHigh;
            ulCurrentTimeLow = *pulTimeLow;
        } while( ulCurrentTimeHigh != *pulTimeHigh );

        ullNextTime = ( uint64_t ) ulCurrentTimeHigh;
        ullNextTime <<= 32ULL; /* High 4-byte word is 32-bits up. */
        ullNextTime |= ( uint64_t ) ulCurrentTimeLow;
        ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
        *pullMachineTimerCompareRegister = ullNextTime;

        /* Prepare the time to use after the next tick interrupt. */
        ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
    }

#endif /* ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIME_BASE_ADDRESS != 0 ) */
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
    extern void xPortStartFirstTask( void );

    #if ( configASSERT_DEFINED == 1 )
        {
            volatile uint32_t mtvec = 0;

            /* Check the least significant two bits of mtvec are 00 - indicating
             * single vector mode. */
            __asm volatile ( "csrr %0, mtvec" : "=r" ( mtvec ) );
            configASSERT( ( mtvec & 0x03UL ) == 0 );

            /* Check alignment of the interrupt stack - which is the same as the
             * stack that was being used by main() prior to the scheduler being
             * started. */
            configASSERT( ( xISRStackTop & portBYTE_ALIGNMENT_MASK ) == 0 );

            #ifdef configISR_STACK_SIZE_WORDS
                {
                    memset( ( void * ) xISRStack, portISR_STACK_FILL_BYTE, sizeof( xISRStack ) );
                }
            #endif /* configISR_STACK_SIZE_WORDS */
        }
    #endif /* configASSERT_DEFINED */

    /* If there is a CLINT then it is ok to use the default implementation
     * in this file, otherwise vPortSetupTimerInterrupt() must be implemented to
     * configure whichever clock is to be used to generate the tick interrupt. */
    vPortSetupTimerInterrupt();

    #if ( ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 ) )
        {
            /* Enable mtime and external interrupts.  1<<7 for timer interrupt, 1<<11
             * for external interrupt.  _RB_ What happens here when mtime is not present as
             * with pulpino? */
            __asm volatile ( "csrs mie, %0" ::"r" ( 0x880 ) );
        }
    #else
        {
            /* Enable external interrupts. */
            __asm volatile ( "csrs mie, %0" ::"r" ( 0x800 ) );
        }
    #endif /* ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 ) */

    xPortStartFirstTask();

    /* Should not get here as after calling xPortStartFirstTask() only tasks
     * should be executing. */
    return pdFAIL;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
    /* Not implemented. */
    for( ; ; )
    {
    }
}

/*-----------------------------------------------------------*/

BaseType_t vPortIsUserModeSupported( void )
{
	volatile uint32_t misa;
	BaseType_t retval;

	/* Alternative to reading misa.U bit, could use mstatus.MPP WARL property
	 * to check whether U-mode is supported. */

	__asm volatile ( "csrr %0, misa" : "=r" ( misa ) );

	retval = ( ( misa & MISA_EXTENSION_U ) == MISA_EXTENSION_U ) ? pdTRUE : pdFALSE;
	return retval;
}

/*-----------------------------------------------------------*/

void vPortDropPrivilege( void )
{
	volatile uint32_t mstatus;

	__asm volatile ( "csrr %0, mstatus" : "=r" ( mstatus ) );

	/* Set mstatus.MPP to U-mode, thus when mret is executed U-mode is restored. */
	mstatus &= ~MSTATUS_MPP_BITS_MASK;

	/* Preserve M-mode interrupt setting by set/clear mstatus.MPIE.
	 * Thus when mret is executed mstatus.MIE is restored. */
	if ( mstatus & MSTATUS_MIE_BIT_MASK )
	{
		mstatus |= MSTATUS_MPIE_BIT_MASK;
	}
	else
	{
		mstatus &= ~MSTATUS_MPIE_BIT_MASK;
	}

	/* Preserve U-mode interrupt setting by set/clear mstatus.UPIE.
	 * Thus when mret is executed mstatus.UIE is restored.
	 * User-level interrupts are primarily intended to support secure embedded
	 * systems with only M-mode and U-mode present.
	 * User-level interrupts require ISA N extension. */
	if ( mstatus & MSTATUS_UIE_BIT_MASK )
	{
		mstatus |= MSTATUS_UPIE_BIT_MASK;
	}
	else
	{
		mstatus &= ~MSTATUS_UPIE_BIT_MASK;
	}

	__asm volatile ( "csrw mstatus, %0" ::"r" ( mstatus ) );
}

/*-----------------------------------------------------------*/

void vPortSwitchToUserMode( void )
{

	/* When a trap is taken into M-mode, mepc is written with the address of the
	 * instruction that was interrupted or that encountered the exception. Write
	 * U-mode entry point address, thus when mret is called execution resumes
	 * from entry point.
	 *
	 * Save ra before function call since ra is caller save register. */
	__asm volatile ( "csrw mepc, ra" );

	/* Update mstatus register. */
	vPortDropPrivilege();

	__asm volatile ( "mret" );
}

/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/

void vPortPrivilegeAdjustment(void)
{
	extern uint32_t __syscalls_flash_start__;
	extern uint32_t __syscalls_flash_end__;
	uint32_t ulAddressStart = (size_t)&__syscalls_flash_start__ ;
	uint32_t ulAddressEnd = (size_t)&__syscalls_flash_end__;

	volatile uint32_t mepc;
	volatile uint32_t mstatus;

	__asm volatile ( "csrr %0, mepc" : "=r" ( mepc ) );

	if ( mepc >= ulAddressStart && mepc <= ulAddressEnd )
	{
		__asm volatile ( "csrr %0, mstatus" : "=r" ( mstatus ) );

		/* Set mstatus.MPP to M-mode, thus when mret is executed M-mode is restored. */
		mstatus |= MSTATUS_MPP_BITS_MASK;

		/* Preserve M-mode interrupt setting by set/clear mstatus.MPIE.
		 * Thus when mret is executed mstatus.MIE is restored. */
		if ( mstatus & MSTATUS_MIE_BIT_MASK )
		{
			mstatus |= MSTATUS_MPIE_BIT_MASK;
		}
		else
		{
			mstatus &= ~MSTATUS_MPIE_BIT_MASK;
		}

		/* Preserve U-mode interrupt setting by set/clear mstatus.UPIE.
		 * Thus when mret is executed mstatus.UIE is restored.
		 * User-level interrupts are primarily intended to support secure embedded
		 * systems with only M-mode and U-mode present.
		 * User-level interrupts require ISA N extension. */
		if ( mstatus & MSTATUS_UIE_BIT_MASK )
		{
			mstatus |= MSTATUS_UPIE_BIT_MASK;
		}
		else
		{
			mstatus &= ~MSTATUS_UPIE_BIT_MASK;
		}

		__asm volatile ( "csrw mstatus, %0" ::"r" ( mstatus ) );
	}
	else
	{
		/* The call is originated outside of the allowed range. Do not drop privilege.*/
	}

	/* ECALL causes the receiving privilege mode’s epc register to be set to the
	 * address of the ECALL instruction itself. Thus we need to return to the following
	 * instruction. And we know for sure that ecall is a 32-bit instruction. */
	mepc += 4;
	__asm volatile ( "csrw mepc, %0" ::"r" ( mepc ) );

	/* This function is executed as part of the interrupt handler.
	 * Preserve ra and sp, And mret is called at the end of the handler, thus
	 * no need to call mret here. */
}

/*-----------------------------------------------------------*/

void vPortStoreTaskMPUSettings( xMPU_SETTINGS * xMPUSettings,
                                const struct xMEMORY_REGION * const xRegions,
                                StackType_t * pxBottomOfStack,
                                uint32_t ulStackDepth )
{

}
