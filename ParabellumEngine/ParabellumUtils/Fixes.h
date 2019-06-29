//****************************************************************************************************/
//
//	Filename : Fixes.h
//
//	Descritpion : Provides pre/su-fixes used in ParabellumUtils
//
//					
//
//
//****************************************************************************************************/



#ifndef __PRAGMAONCE_PARABELLUMUTILS_FIXES_H__
#define __PRAGMAONCE_PARABELLUMUTILS_FIXES_H__


// shortcut for PArabellumFramework namespace
#define PFNS	ParabellumFramework

// null pointer
#define NULLPTR	nullptr


// prefixes for function parametrs

#define _IN_	//const		// read only parametr
#define _OUT_				// write only parametr
#define _IO_				// read/write parametr
#define _FUNC_				// pointer to a function / lambda / functor
#define _KEEP_				// pointer will be saved somewhere in the function/class, so can not be deleted later

#define	_ARRAY_				// we have to deal with an array
#define _THREAD_CRITICAL_	// thread critical section
#define _THREAD_ATOMIC_		// function/variable MUST be atomic


// Epsilon for calculations
#define EFEPSILON	0.00001f		// float epsilon
#define EDEPSILON	0.00000001		// double epsilon


#define EEQ_ZERO(val) ( (val < EFEPSILON) &&  (val > -EFEPSILON) )

// Bit position
#define EBYTENO_0	0
#define EBYTENO_1	1
#define EBYTENO_2	2
#define EBYTENO_3	4
#define EBYTENO_4	8
#define EBYTENO_5	16
#define EBYTENO_6	32
#define EBYTENO_7	64
#define EBYTENO_8	128
#define EBYTENO_9	256
#define EBYTENO_10	512
#define EBYTENO_11	1024
#define EBYTENO_12	2048
#define EBYTENO_13	4096
#define EBYTENO_14	8192
#define EBYTENO_15	16384
#define EBYTENO_16	32768
#define EBYTENO_17	1 << 17
#define EBYTENO_18	1 << 18
#define EBYTENO_19	1 << 19
#define EBYTENO_20	1 << 20
#define EBYTENO_21	1 << 21
#define EBYTENO_22	1 << 22
#define EBYTENO_23	1 << 23
#define EBYTENO_24	1 << 24
#define EBYTENO_25	1 << 25
#define EBYTENO_26	1 << 26
#define EBYTENO_27	1 << 27
#define EBYTENO_28	1 << 28
#define EBYTENO_29	1 << 29
#define EBYTENO_30	1 << 30
#define EBYTENO_31	1 << 31

#define EB_1YOUNGBYTES 1		// 00000001
#define EB_2YOUNGBYTES 3		// 00000011
#define EB_3YOUNGBYTES 7		// 00000111
#define EB_4YOUNGBYTES 15		// 00001111
#define EB_5YOUNGBYTES 31		// 00011111
#define EB_6YOUNGBYTES 63		// 00111111
#define EB_7YOUNGBYTES 127		// 01111111

#define EB_1OLDBYTES 128		// 10000000
#define EB_2OLDBYTES 192		// 11000000
#define EB_3OLDBYTES 224		// 11100000
#define EB_4OLDBYTES 240		// 11110000
#define EB_5OLDBYTES 248		// 11111000
#define EB_6OLDBYTES 252		// 11111100
#define EB_7OLDBYTES 254		// 11111110



// units
#define _UNIT_RAD_
#define _UNIT_EULER_
#define _UNIT_CENTIMETER_
#define _UNIT_METER_
#define _UNIT_MILISECOND_
#define _UNIT_SECOND_
#define _UNIT_MINUTE_
#define _UNIT_HOUR_
#define _UNIT_DAY_
#define _UNIT_MONTH_
#define _UNIT_YEAR_




#endif