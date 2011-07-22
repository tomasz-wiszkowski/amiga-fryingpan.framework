/*
 * types.h
 *
 *  Created on: 09-10-2010
 *      Author: error
 */

#ifndef LIBC_TYPES_H_
#define LIBC_TYPES_H_

#ifdef __AMIGAOS4__
#include <exec/types.h>
#else
typedef unsigned long long 	uint64; /**< @brief unsigned 64bit integer */
typedef unsigned int	   	uint32; /**< @brief unsigned 32bit integer */
typedef unsigned short     	uint16; /**< @brief unsigned 16bit integer */
typedef unsigned char      	uint8;  /**< @brief unsigned 8bit integer  */
typedef signed long long   	int64;  /**< @brief signed 64bit integer */
typedef signed int	   	   	int32;  /**< @brief signed 32bit integer */
typedef signed short       	int16;  /**< @brief signed 16bit integer */
typedef signed char       	int8;   /**< @brief signed 8bit integer  */
#endif

typedef unsigned long      	iptr;  	/**< @brief type where sizeof(iptr) = sizeof(void*) */
typedef iptr		   		sized_iptr;

/* platform specific includes */
#if defined(__AROS__)
typedef uint32 size_t;
#elif defined(__mc68000)
typedef iptr size_t;
typedef iptr IPTR;
#elif defined(__AMIGAOS4__)
typedef unsigned int size_t;
typedef iptr IPTR;
#elif defined(__MORPHOS__)
typedef uint32 size_t;
#else
#error no size_t defined
#endif

#define PACKED __attribute__((packed))

#endif /* LIBC_TYPES_H_ */
