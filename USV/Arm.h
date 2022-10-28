/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
 ============================================================================================================================================
 Author      : samlu (samlu@vciash.com)
 Description : The definition of data types and architecture dependent declaration.
 Status      : RELEASE
 ============================================================================================================================================
 */
/** @defgroup __ARCHITECTURE__H_ Architecture
 *  It includes the architecture-dependent declaration, developer should modify this file 
 *  and recompile kernel once porting to other architectures.
 *  @{
 */
/** @defgroup __ARCHITECTURE_DECLARATION__H_ Architecture declaration ARM
 *  The definition about fundamental data type and architecture dependent declaration will be described here.
 *  @{
 */

/*-------------------------------------------------------------------------------------------------------------------------------------------
 *
 *                                                                Constants
 *
 *-----------------------------------------------------------------------------------------------------------------------------------------*/
/** @name Fundamental data type
 *  @{
 */
/* 
 * File:   Arm.h
 * Author: Administrator
 *
 * Created on 2016年11月28日, 下午3:29
 */

#ifndef ARM_H
#define ARM_H

/** 8-bit signed. */
#define CHAR char

/** 8-bit unsigned. */
#define UCHAR unsigned char

/** 8-bit signed. */
#define INT8 signed char

/** 8-bit unsigned. */
#define UINT8 unsigned char

/** 8-bit unsigned. */
#define BYTE unsigned char

/** 16-bit signed. */
#define INT16 signed short int

/** 16-bit unsigned. */
#define UINT16 unsigned short int

/** 32-bit signed. */
#define INT32 signed int

/** 32-bit unsigned. */
#define UINT32 unsigned int

/** 64-bit unsigned. */
#define UINT64 unsigned long long

/** 64-bit signed. */
#define INT64 signed long long

/** IEEE floating point. */
#define FLOAT float

/** IEEE double point. */
#define DOUBLE double

/** Pointer. */
#define POINTER int*

/** Boolean - false. */
//#define FALSE 0

/** Boolean - true. */
//#define TRUE  1

typedef enum                     // boolean
{ FALSE = 0, TRUE } BOOL;
/** @}*/


/** @name GCC compiler attribute
 *  @{
 */
/** Public function. */
#define DLL_PUBLIC __attribute__( ( visibility( "default" ) ) ) 

/** Private function. */
#define DLL_LOCAL __attribute__( ( visibility( "hidden" ) ) )

/** Inline function. */
#define INLINE __attribute__( ( always_inline ) ) inline
/** @}*/




/*-------------------------------------------------------------------------------------------------------------------------------------------
 *
 *                                                          Structure Definitions
 *
 *-----------------------------------------------------------------------------------------------------------------------------------------*/
/** 8-bit array. */
typedef struct {
    /** Lowest bit. */
    unsigned b0 : 1;
    
    /** Second bit. */
    unsigned b1 : 1;
    
    /** Third bit. */
    unsigned b2 : 1;
    
    /** Forth bit. */
    unsigned b3 : 1;
    
    /** Fifth bit. */
    unsigned b4 : 1;
    
    /** Sixth bit. */
    unsigned b5 : 1;
    
    /** Seventh bit. */
    unsigned b6 : 1;
    
    /** Highest bit. */
    unsigned b7 : 1;
} __attribute__ ( ( packed ) ) BITARRAY8;


/** 16-bit array. */
typedef struct {
    /** Lowest bit. */
    unsigned b0  : 1;
    
    /** Second bit. */
    unsigned b1  : 1;
    
    /** Third bit. */
    unsigned b2  : 1;
    
    /** Forth bit. */
    unsigned b3  : 1;
    
    /** Fifth bit. */
    unsigned b4  : 1;
    
    /** Sixth bit. */
    unsigned b5  : 1;
    
    /** Seventh bit. */
    unsigned b6  : 1;
    
    /** Eighth bit. */
    unsigned b7  : 1;
    
    /** Ninth bit. */
    unsigned b8  : 1;
    
    /** Tenth bit. */
    unsigned b9  : 1;
    
    /** Eleventh bit. */
    unsigned b10 : 1;
    
    /** Twelfth bit. */
    unsigned b11 : 1;
    
    /** Thirteenth bit. */
    unsigned b12 : 1;
    
    /** Fourteenth bit. */
    unsigned b13 : 1;
    
    /** Fifteenth bit. */
    unsigned b14 : 1;
    
    /** Highest bit. */
    unsigned b15 : 1;
} __attribute__ ( ( packed ) ) BITARRAY16;


/** 32-bit array. */
typedef struct {
    /** Lowest bit. */
    unsigned b0  : 1;

    /** Second bit. */
    unsigned b1  : 1;
    
    /** Third bit. */
    unsigned b2  : 1;
    
    /** Forth bit. */
    unsigned b3  : 1;
    
    /** Fifth bit. */
    unsigned b4  : 1;
    
    /** Sixth bit. */
    unsigned b5  : 1;
    
    /** Seventh bit. */
    unsigned b6  : 1;
    
    /** Eighth bit. */
    unsigned b7  : 1;
    
    /** Ninth bit. */
    unsigned b8  : 1;
    
    /** Tenth bit. */
    unsigned b9  : 1;
    
    /** Eleventh bit. */
    unsigned b10 : 1;
    
    /** Twelfth bit. */
    unsigned b11 : 1;
    
    /** Thirteenth bit. */
    unsigned b12 : 1;
    
    /** Fourteenth bit. */
    unsigned b13 : 1;
    
    /** Fifteenth bit. */
    unsigned b14 : 1;
    
    /** Sixteenth bit. */
    unsigned b15 : 1;
    
    /** Seventeenth bit. */
    unsigned b16 : 1;
    
    /** Eighteenth bit. */
    unsigned b17 : 1;
    
    /** Nineteenth bit. */
    unsigned b18 : 1;
    
    /** Twentieth bit. */
    unsigned b19 : 1;
    
    /** Twenty-first bit. */
    unsigned b20 : 1;
    
    /** Twenty-second bit. */
    unsigned b21 : 1;
    
    /** Twenty-third bit. */
    unsigned b22 : 1;
    
    /** Twenty-forth bit. */
    unsigned b23 : 1;
    
    /** Twenty-fifth bit. */
    unsigned b24 : 1;
    
    /** Twenty-sixth bit. */
    unsigned b25 : 1;
    
    /** Twenty-seventh bit. */
    unsigned b26 : 1;
    
    /** Twenty-eighth bit. */
    unsigned b27 : 1;
    
    /** Twenty-ninth bit. */
    unsigned b28 : 1;
    
    /** Thirtieth bit. */
    unsigned b29 : 1;
    
    /** Thirty-first bit. */
    unsigned b30 : 1;
    
    /** Highest bit. */
    unsigned b31 : 1;
} __attribute__ ( ( packed ) ) BITARRAY32;


/** 64-bit array. */
typedef struct {
    /** Lowest bit. */
    unsigned b0  : 1;

    /** Second bit. */
    unsigned b1  : 1;
    
    /** Third bit. */
    unsigned b2  : 1;
    
    /** Forth bit. */
    unsigned b3  : 1;
    
    /** Fifth bit. */
    unsigned b4  : 1;
    
    /** Sixth bit. */
    unsigned b5  : 1;
    
    /** Seventh bit. */
    unsigned b6  : 1;
    
    /** Eighth bit. */
    unsigned b7  : 1;
    
    /** Ninth bit. */
    unsigned b8  : 1;
    
    /** Tenth bit. */
    unsigned b9  : 1;
    
    /** Eleventh bit. */
    unsigned b10 : 1;
    
    /** Twelfth bit. */
    unsigned b11 : 1;
    
    /** Thirteenth bit. */
    unsigned b12 : 1;
    
    /** Fourteenth bit. */
    unsigned b13 : 1;
    
    /** Fifteenth bit. */
    unsigned b14 : 1;
    
    /** Sixteenth bit. */
    unsigned b15 : 1;
    
    /** Seventeenth bit. */
    unsigned b16 : 1;
    
    /** Eighteenth bit. */
    unsigned b17 : 1;
    
    /** Nineteenth bit. */
    unsigned b18 : 1;
    
    /** Twentieth bit. */
    unsigned b19 : 1;
    
    /** Twenty-first bit. */
    unsigned b20 : 1;
    
    /** Twenty-second bit. */
    unsigned b21 : 1;
    
    /** Twenty-third bit. */
    unsigned b22 : 1;
    
    /** Twenty-forth bit. */
    unsigned b23 : 1;
    
    /** Twenty-fifth bit. */
    unsigned b24 : 1;
    
    /** Twenty-sixth bit. */
    unsigned b25 : 1;
    
    /** Twenty-seventh bit. */
    unsigned b26 : 1;
    
    /** Twenty-eighth bit. */
    unsigned b27 : 1;
    
    /** Twenty-ninth bit. */
    unsigned b28 : 1;
    
    /** Thirtieth bit. */
    unsigned b29 : 1;
    
    /** Thirty-first bit. */
    unsigned b30 : 1;
    
    /** Thirty-second bit. */
    unsigned b31 : 1;
    
    /** Thirty-third bit. */
    unsigned b32 : 1;
    
    /** Thirty-forth bit. */
    unsigned b33 : 1;
    
    /** Thirty-fifth bit. */
    unsigned b34 : 1;
    
    /** Thirty-sixth bit. */
    unsigned b35 : 1;
    
    /** Thirty-seventh bit. */
    unsigned b36 : 1;
    
    /** Thirty-eighth bit. */
    unsigned b37 : 1;
    
    /** Thirty-ninth bit. */
    unsigned b38 : 1;
    
    /** Fortieth bit. */
    unsigned b39 : 1;
    
    /** Forty-first bit. */
    unsigned b40 : 1;
    
    /** Forty-second bit. */
    unsigned b41 : 1;
    
    /** Forty-third bit. */
    unsigned b42 : 1;
    
    /** Forty-forth bit. */
    unsigned b43 : 1;
    
    /** Forty-fifth bit. */
    unsigned b44 : 1;
    
    /** Forty-sixth bit. */
    unsigned b45 : 1;
    
    /** Forty-seventh bit. */
    unsigned b46 : 1;
    
    /** Forty-eighth bit. */
    unsigned b47 : 1;
    
    /** Forty-ninth bit. */
    unsigned b48 : 1;
    
    /** Fiftieth bit. */
    unsigned b49 : 1;
    
    /** Fifty-first bit. */
    unsigned b50 : 1;
    
    /** Fifty-second bit. */
    unsigned b51 : 1;
    
    /** Fifty-third bit. */
    unsigned b52 : 1;
    
    /** Fifty-forth bit. */
    unsigned b53 : 1;
    
    /** Fifty-fifth bit. */
    unsigned b54 : 1;
    
    /** Fifty-sixth bit. */
    unsigned b55 : 1;
    
    /** Fifty-seventh bit. */
    unsigned b56 : 1;
    
    /** Fifty-eighth bit. */
    unsigned b57 : 1;
    
    /** Fifty-ninth bit. */
    unsigned b58 : 1;
    
    /** Sixtieth bit. */
    unsigned b59 : 1;
    
    /** Sixty-first bit. */
    unsigned b60 : 1;
    
    /** Sixty-second bit. */
    unsigned b61 : 1;
    
    /** Sixty-third bit. */
    unsigned b62 : 1;
    
    /** Highest bit. */
    unsigned b63 : 1;
} __attribute__ ( ( packed ) ) BITARRAY64;


/** Byte based structure for 16 bits. */
typedef union {
    /** 8-bit unsigned array. */
    UINT8  uc[ 2 ];
    
    /** 8-bit signed array. */
    INT8   sc[ 2 ];
    
    /** 16-bit unsigned. */
    UINT16 ui;
    
    /** 16-bit signed. */
    INT16  si;
    
    /** 8-bit unsigned array. */
    UINT8  u8[ 2 ];
    
    /** 8-bit signed array. */
    INT8   s8[ 2 ];
    
    /** 16-bit unsigned. */
    UINT16 u16;
    
    /** 16-bit signed. */
    INT16  s16;
    
    /** bit array. */
    BITARRAY16 ba;
} BYTEBASE16;


/** Byte based structure for 32 bits. */
typedef union {
    /** 8-bit unsigned array. */
    UINT8  u8[ 4 ];
    
    /** 8-bit signed array. */
    INT8   s8[ 4 ];
    
    /** 16-bit unsigned array. */
    UINT16 u16[ 2 ];
    
    /** 16-bit signed array. */
    INT16  s16[ 2 ];
    
    /** 32-bit unsigned. */
    UINT32 u32;
    
    /** 32-bit signed. */
    INT32  s32;
    
    /** IEEE floating point. */
    FLOAT  f32;
    
    /** 8-bit unsigned array. */
    UINT8  uc[ 4 ];
    
    /** 8-bit signed array. */
    INT8   sc[ 4 ];
    
    /** 16-bit unsigned array. */
    UINT16 ui[ 2 ];
    
    /** 16-bit signed array. */
    INT16  si[ 2 ];
    
    /** 32-bit unsigned. */
    UINT32 ul;
    
    /** 32-bit signed. */
    INT32  sl;
    
    /** IEEE floating point. */
    FLOAT  f;
    
    /** bit array. */
    BITARRAY32 ba;
} BYTEBASE32;


/** Byte based structure for 64 bits. */
typedef union {
    /** 8-bit unsigned array. */
    UINT8  u8[ 8 ];
    
    /** 8-bit signed array. */
    INT8   s8[ 8 ];
    
    /** 16-bit unsigned array. */
    UINT16 u16[ 4 ];
    
    /** 16-bit signed array. */
    INT16  s16[ 4 ];
    
    /** 32-bit unsigned array. */
    UINT32 u32[ 2 ];
    
    /** 32-bit signed array. */
    INT32  s32[ 2 ];
    
    /** IEEE floating point array. */
    FLOAT  f32[ 2 ];
    
    /** 64-bit unsigned. */
    UINT64 u64;
    
    /** 64-bit signed. */
    INT64  s64;    
    
    /** 8-bit unsigned array. */
    UINT8  uc[ 8 ];
    
    /** 8-bit signed array. */
    INT8   sc[ 8 ];
    
    /** 16-bit unsigned array. */
    UINT16 ui[ 4 ];
    
    /** 16-bit signed array. */
    INT16  si[ 4 ];
    
    /** 32-bit unsigned array. */
    UINT32 ul[ 2 ];
    
    /** 32-bit signed array. */
    INT32  sl[ 2 ];
    
    /** IEEE floating point array. */
    FLOAT  f[ 2 ];
    
    /** bit array. */
    BITARRAY64 ba;
} BYTEBASE64;


#endif /* ARM_H */

