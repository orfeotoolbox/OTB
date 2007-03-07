
/* PROTOMAS.H */

#ifndef PROTOMASKS_H

#define PROTOMASKS_H

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __STDC__
#define var_start(x,y) va_start(x,y)

/* These macros expand into ANSI prototypes */
#define FUN0()		(void)
#define EXT0()		(void)

#define FUN1(t1,a1)	(t1 a1)
#define EXT1(t1)	(t1)
#define FUN1N(t1,a1)	(t1 a1, ...)
#define EXT1N(t1)	(t1, ...)

#define FUN2(t1,a1,t2,a2)	(t1 a1,t2 a2)
#define EXT2(t1, t2)		(t1, t2)
#define FUN2N(t1,a1,t2,a2)	(t1 a1,t2 a2, ...)
#define EXT2N(t1, t2)		(t1, t2, ...)

#define FUN3(t1,a1,t2,a2,t3,a3)	(t1 a1, t2 a2, t3 a3)
#define EXT3(t1, t2, t3)	(t1, t2, t3)
#define FUN3N(t1,a1,t2,a2,t3,a3)(t1 a1, t2 a2, t3 a3, ...)
#define EXT3N(t1, t2, t3)	(t1, t2, t3, ...)

#define FUN4(t1,a1,t2,a2,t3,a3,t4,a4)	(t1 a1, t2 a2, t3 a3, t4 a4)
#define EXT4(t1, t2, t3, t4)		(t1, t2, t3, t4)

#define FUN5(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5)
#define EXT5(t1, t2, t3, t4, t5)		(t1, t2, t3, t4, t5)

#define FUN6(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6)
#define EXT6(t1, t2, t3, t4, t5, t6)			(t1, t2, t3, t4, t5, t6)

#define FUN7(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7)
#define EXT7(t1, t2, t3, t4, t5, t6, t7)		(t1, t2, t3, t4, t5, t6, t7)

#define FUN8(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7, t8 a8)
#define EXT8(t1, t2, t3, t4, t5, t6, t7, t8)			(t1, t2, t3, t4, t5, t6, t7, t8)

#define FUN9(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7, t8 a8, t9 a9)
#define EXT9(t1, t2, t3, t4, t5, t6, t7, t8, t9)			(t1, t2, t3, t4, t5, t6, t7, t8, t9)

#define FUN10(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7, t8 a8, t9 a9, t10 a10)
#define EXT10(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10)				(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10)

#define FUN11(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10,t11,a11)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7, t8 a8, t9 a9, t10 a10, t11 a11)
#define EXT11(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11)				(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11)

#define FUN12(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10,t11,a11,t12,a12)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7, t8 a8, t9 a9, t10 a10, t11 a11, t12 a12)
#define EXT12(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12)				(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12)

#define FUN13(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10,t11,a11,t12,a12,t13,a13)	(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7, t8 a8, t9 a9, t10 a10, t11 a11, t12 a12, t13 a13)
#define EXT13(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13)				(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13)


#else
/* Non-ANSI */
#define var_start(x,y) va_start(x)

/* These macros expand into old-style function definitions */

#define FUN0()	()
#define EXT0()	()

#define FUN1(t1,a1)	(a1) t1 a1;
#define EXT1(t1)	()
#define FUN1N(t1,a1)	(a1,va_alist) t1 a1; va_dcl
#define EXT1N(t1)	()

#define FUN2(t1,a1,t2,a2)	(a1, a2) t1 a1; t2 a2;
#define EXT2(t1, t2)		()
#define FUN2N(t1,a1,t2,a2,va_alist) (a1, a2) t1 a1; t2 a2; va_dcl
#define EXT2N(t1, t2)		()

#define FUN3(t1,a1,t2,a2,t3,a3) (a1, a2, a3) t1 a1; t2 a2; t3 a3;
#define EXT3(t1, t2, t3)	()
#define FUN3N(t1,a1,t2,a2,t3,a3) (a1, a2, a3, va_alist) t1 a1; t2 a2; t3 a3; va_dcl
#define EXT3N(t1, t2, t3)	()

#define FUN4(t1,a1,t2,a2,t3,a3,t4,a4)	(a1, a2, a3, a4) t1 a1; t2 a2; t3 a3; t4 a4;
#define EXT4(t1, t2, t3, t4)		()

#define FUN5(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5) (a1, a2, a3, a4, a5) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5;
#define EXT5(t1, t2, t3, t4, t5)	()

#define FUN6(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6) (a1, a2, a3, a4, a5, a6) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6;
#define EXT6(t1, t2, t3, t4, t5, t6)	()

#define FUN7(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7) (a1, a2, a3, a4, a5, a6, a7) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6; t7 a7;
#define EXT7(t1, t2, t3, t4, t5, t6, t7)	()

#define FUN8(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8) (a1, a2, a3, a4, a5, a6, a7, a8) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6; t7 a7; t8 a8;
#define EXT8(t1, t2, t3, t4, t5, t6, t7, t8)	()

#define FUN9(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9) (a1, a2, a3, a4, a5, a6, a7, a8, a9) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6; t7 a7; t8 a8; t9 a9;
#define EXT9(t1, t2, t3, t4, t5, t6, t7, t8, t9)	()

#define FUN10(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10) (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6; t7 a7; t8 a8; t9 a9; t10 a10;
#define EXT10(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10)	()

#define FUN11(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10,t11,a11) (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6; t7 a7; t8 a8; t9 a9; t10 a10; t11 a11;
#define EXT11(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11)	()

#define FUN12(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10,t11,a11,t12,a12) (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6; t7 a7; t8 a8; t9 a9; t10 a10; t11 a11; t12 a12;
#define EXT12(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12)	()

#define FUN13(t1,a1,t2,a2,t3,a3,t4,a4,t5,a5,t6,a6,t7,a7,t8,a8,t9,a9,t10,a10,t11,a11,t12,a12,t13,a13) (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) t1 a1; t2 a2; t3 a3; t4 a4; t5 a5; t6 a6; t7 a7; t8 a8; t9 a9; t10 a10; t11 a11; t12 a12; t13 a13;
#define EXT13(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13)	()

#endif

#ifdef __cplusplus
}
#endif
#endif /* ProtoMasks_h */
