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

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code that tailors the port to a specific RISC-V chip:
 *
 * + FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S contains the code that
 *   is common to all currently supported RISC-V chips.  There is only one
 *   portASM.S file because the same file is built for all RISC-V target chips.
 *
 * + Header files called freertos_risc_v_chip_specific_extensions.h contain the
 *   code that tailors the FreeRTOS kernel's RISC-V port to a specific RISC-V
 *   chip.  There are multiple freertos_risc_v_chip_specific_extensions.h files
 *   as there are multiple RISC-V chip implementations.
 *
 * !!!NOTE!!!
 * TAKE CARE TO INCLUDE THE CORRECT freertos_risc_v_chip_specific_extensions.h
 * HEADER FILE FOR THE CHIP IN USE.  This is done using the assembler's (not the
 * compiler's!) include path.  For example, if the chip in use includes a core
 * local interrupter (CLINT) and does not include any chip specific register
 * extensions then add the path below to the assembler's include path:
 * FreeRTOS\Source\portable\GCC\RISC-V-RV32\chip_specific_extensions\RV32I_CLINT_no_extensions
 *
 */


#ifndef __FREERTOS_RISC_V_EXTENSIONS_H__
#define __FREERTOS_RISC_V_EXTENSIONS_H__

#define portasmHAS_SIFIVE_CLINT           1
#define portasmHAS_MTIME                  1


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


#define portasmADDITIONAL_CONTEXT_SIZE    10 /* For 8 PMP entries. */

.macro portasmINCREMENT_MEPC
    /* ECALL causes the receiving privilege mode’s epc register to be set to the
     * address of the ECALL instruction itself. Thus to return to the following
     * instruction, increment mepc register. +4 directly, since we know for sure
     * that ecall is a 32-bit instruction. */
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0
    .endm

.macro portasmSAVE_ADDITIONAL_REGISTERS
   addi sp, sp, -( portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE ) /* Make room for the additional registers. */

   csrr t0, pmpcfg0
   csrr t1, pmpcfg1

   store_x t0, 1 * portWORD_SIZE( sp )
   store_x t1, 2 * portWORD_SIZE( sp )

   csrr t0, pmpaddr0
   csrr t1, pmpaddr1
   csrr t2, pmpaddr2
   csrr t3, pmpaddr3

   store_x t0, 3 * portWORD_SIZE( sp )
   store_x t1, 4 * portWORD_SIZE( sp )
   store_x t2, 5 * portWORD_SIZE( sp )
   store_x t3, 6 * portWORD_SIZE( sp )


   csrr t0, pmpaddr4
   csrr t1, pmpaddr5
   csrr t2, pmpaddr6
   csrr t3, pmpaddr7

   store_x t0, 7 * portWORD_SIZE( sp )
   store_x t1, 8 * portWORD_SIZE( sp )
   store_x t2, 9 * portWORD_SIZE( sp )
   store_x t3, 10 * portWORD_SIZE( sp )
   .endm


.macro portasmRESTORE_ADDITIONAL_REGISTERS
   load_x t0, 1 * portWORD_SIZE( sp ) /* Load additional registers into accessible temporary registers. */
   load_x t1, 2 * portWORD_SIZE( sp )

   csrw pmpcfg0, t0
   csrw pmpcfg1, t1

   load_x t0, 3 * portWORD_SIZE( sp )
   load_x t1, 4 * portWORD_SIZE( sp )
   load_x t2, 5 * portWORD_SIZE( sp )
   load_x t3, 6 * portWORD_SIZE( sp )

   csrw pmpaddr0, t0
   csrw pmpaddr1, t1
   csrw pmpaddr2, t2
   csrw pmpaddr3, t3

   load_x t0, 7 * portWORD_SIZE( sp )
   load_x t1, 8 * portWORD_SIZE( sp )
   load_x t2, 9 * portWORD_SIZE( sp )
   load_x t3, 10 * portWORD_SIZE( sp )

   csrw pmpaddr4, t0
   csrw pmpaddr5, t1
   csrw pmpaddr6, t2
   csrw pmpaddr7, t3

   addi sp, sp, ( portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE ) /* Remove space added for additional registers. */

   .endm

#endif /* __FREERTOS_RISC_V_EXTENSIONS_H__ */
