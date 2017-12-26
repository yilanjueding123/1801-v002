#ifndef __PROJECT_H__
#define __PROJECT_H__

/* Pseudo header files for customer level plateform define */
#include "customer.h"

//---------------------------------------------------------------------------
#define	GPL32_A                     0x1                                    //
#define	GPL32_B                     0x2                                    //
#define	GPL32_C                     0x3                                    //
#define	GPL32_GP10                 0x10                                    //
#define MCU_VERSION          GPL32_GP10                                    //
//---------------------------------------------------------------------------

/*=== Board ID Config ===*/
//---------------------------------------------------------------------------
#define BOARD_TURNKEY_BASE      0x10000000                                 //
#define BOARD_EMU_BASE          0x00000000                                 //
                                                                           //
#define BOARD_EMU_V1_0          (BOARD_EMU_BASE + 0x10)                    //
#define BOARD_EMU_V2_0          (BOARD_EMU_BASE + 0x20)                    //
#define BOARD_TK35_V1_0         (BOARD_TURNKEY_BASE + 0x10)                //
#define BOARD_TK_V1_0           (BOARD_TURNKEY_BASE + 0x20)                //
#define BOARD_TK_V2_0           (BOARD_TURNKEY_BASE + 0x30)                //
#define BOARD_TK_V3_0           (BOARD_TURNKEY_BASE + 0x40)                //
#define BOARD_TK_V4_0           (BOARD_TURNKEY_BASE + 0x50)                //
#define BOARD_TK_V4_1           (BOARD_TURNKEY_BASE + 0x60)                //
#define BOARD_TK_V4_3           (BOARD_TURNKEY_BASE + 0x70)                //
#define BOARD_TK_A_V1_0         (BOARD_TURNKEY_BASE + 0x80)                //
#define BOARD_GPY0200_EMU_V2_0  (BOARD_TURNKEY_BASE + 0x90)                //
#define BOARD_TK_V4_4           (BOARD_TURNKEY_BASE + 0xA0)                //
#define BOARD_TK_A_V2_0         (BOARD_TURNKEY_BASE + 0xB0)                //
#define BOARD_TYPE              BOARD_EMU_V2_0                             //
//---------------------------------------------------------------------------

/*=== DEBUG Devices ===*/
//---------------------------------------------------------------------------
#define CONSOLE_DBG_MSG         DBG_MESSAGE                                  //
                                                                           //
//enable UART , if console mode debug need                                 //
#if (CONSOLE_DBG_MSG==CUSTOM_ON)                                           //
	#define SUPPORT_UART            CUSTOM_ON                              //
#elif (CONSOLE_DBG_MSG==CUSTOM_OFF)                                        //                             //
	#define SUPPORT_UART            CUSTOM_OFF                             //
#endif                                                                     //
//---------------------------------------------------------------------------



// Project definitions and config
#define _PROJ_PLATFORM			0
#define _PROJ_TURNKEY           1
#define _PROJ_TYPE              _PROJ_TURNKEY
// IC definitions and config
#define GPL32500A			    0
#define GPL32600A               1
#define GPL32900A               2
#define _IC_TYPE                GPL32600A
// Operating System definitions and config
#define _OS_NONE                0
#define _OS_UCOS2               1
#define	_OPERATING_SYSTEM       _OS_UCOS2

// Driver layer module definitions and config


// GPLib layer module definitions and config
//#define DBG_PRINT				if (0) print_string
#if DBG_MESSAGE
	#define DBG_PRINT	print_string
#else
	#define DBG_PRINT	if (0) print_string
#endif

#if MINI_DVR_BOARD_VERSION <= GPL32680_MINI_DVR_EMU_V2_0 || MINI_DVR_BOARD_VERSION == GPL32680_MINI_DVR_DEMO_BOARD
	#define SUPPORT_JTAG					CUSTOM_OFF
#else
	#define SUPPORT_JTAG					CUSTOM_OFF
#endif

// Global definitions
#define TRUE                    1
#define FALSE                   0
#define NULL                    0
#define STATUS_OK               0
#define STATUS_FAIL             -1

typedef char                    CHAR;   // By default, char is unsigned. It can be changed to signed by compiler option
typedef unsigned char           BOOLEAN;
typedef unsigned char           INT8U;
typedef signed   char           INT8S;
typedef unsigned short          INT16U;
typedef signed   short          INT16S;
typedef unsigned int            INT32U;
typedef signed   int            INT32S;
typedef float                   FP32;
typedef	long long		INT64S;	
typedef	unsigned long long	INT64U;	
typedef double                  FP64;

#ifndef __ALIGN_DEFINE__
#define __ALIGN_DEFINE__
    #ifdef __ADS_COMPILER__
        #define ALIGN32 __align(32)
        #define ALIGN16 __align(16)
        #define ALIGN8 __align(8)
        #define ALIGN4 __align(4)
    #else
        #ifdef __CS_COMPILER__
        #define ALIGN32 __attribute__ ((aligned(32)))
        #define ALIGN16 __attribute__ ((aligned(16)))
        #define ALIGN8 __attribute__ ((aligned(8)))
        #define ALIGN4 __attribute__ ((aligned(4)))
        #else
        #define ALIGN32 __align(32)
        #define ALIGN16 __align(16)
        #define ALIGN8 __align(8)
        #define ALIGN4 __align(4)
        #endif
    #endif
#endif


#ifndef __CS_COMPILER__
    #define ASM(asm_code)  __asm {asm_code}  /*ADS embeded asm*/
    #define IRQ            __irq
    #define PACKED 			__packed
#else
    #define ASM(asm_code)  asm(#asm_code);  /*ADS embeded asm*/
    #define IRQ            __attribute__ ((interrupt))
    #define PACKED	   __attribute__((__packed__))
#endif

#endif      // __PROJECT_H__
