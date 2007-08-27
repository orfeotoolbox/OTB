/* NIOX5.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    niox5(a,inu) >*/
/* Subroutine */ int niox5_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)12740.,(float)
	    12750.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12750.,(float)12760.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12760.,(float)12770.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12770.,(float)12780.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12780.,(float)12790.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12790.,(float)
	    12800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12800.,(float)12810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12810.,(float)12820.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12820.,(float)12830.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12830.,(float)12840.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12840.,(float)
	    12850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12850.,(float)12860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12860.,(float)12870.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12870.,(float)12880.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12880.,(float)12890.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12890.,(float)
	    12900.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12900.,(float)12910.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12910.,(float)12920.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12920.,(float)12930.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12930.,(float)12940.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12940.,(float)
	    12950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12950.,(float)12960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12960.,(float)12970.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12970.,(float)12980.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12980.,(float)12990.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12990.,(float)
	    1.3e4,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)1.3e4,(float)13010.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)13010.,(float)13020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13020.,(
	    float)13030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)13030.,(float)13040.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)13040.,(float)13050.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13050.,(float)13060.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13060.,(float)13070.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13070.,(float)
	    13080.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13080.,(float)13090.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13090.,(float)13100.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13100.,(float)13110.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13110.,(float)13120.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13120.,(float)
	    13130.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13130.,(float)13140.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13140.,(float)13150.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13150.,(float)13160.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13160.,(float)13170.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13170.,(float)
	    13180.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13180.,(float)13190.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13190.,(float)13200.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13200.,(float)13210.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13210.,(float)13220.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13220.,(float)
	    13230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13230.,(float)13240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13240.,(float)13250.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13250.,(float)13260.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13260.,(float)13270.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13270.,(float)
	    13280.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13280.,(float)13290.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13290.,(float)13300.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13300.,(float)13310.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13310.,(float)13320.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13320.,(float)
	    13330.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13330.,(float)13340.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13340.,(float)13350.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13350.,(float)13360.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13360.,(float)13370.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13370.,(float)
	    13380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13380.,(float)13390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13390.,(float)13400.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13400.,(float)13410.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13410.,(float)13420.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13420.,(float)
	    13430.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13430.,(float)13440.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13440.,(float)13450.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13450.,(float)13460.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13460.,(float)13470.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13470.,(float)
	    13480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13480.,(float)13490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13490.,(float)13500.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13500.,(float)13510.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13510.,(float)13520.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13520.,(float)
	    13530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13530.,(float)13540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13540.,(float)13550.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13550.,(float)13560.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13560.,(float)13570.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13570.,(float)
	    13580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13580.,(float)13590.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13590.,(float)13600.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13600.,(float)13610.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13610.,(float)13620.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13620.,(float)
	    13630.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13630.,(float)13640.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13640.,(float)13650.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13650.,(float)13660.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13660.,(float)13670.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13670.,(float)
	    13680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13680.,(float)13690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13690.,(float)13700.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13700.,(float)13710.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13710.,(float)13720.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13720.,(float)
	    13730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13730.,(float)13740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13740.,(float)13750.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13750.,(float)13760.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13760.,(float)13770.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13770.,(float)
	    13780.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13780.,(float)13790.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13790.,(float)13800.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13800.,(float)13810.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13810.,(float)13820.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13820.,(float)
	    13830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13830.,(float)13840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13840.,(float)13850.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13850.,(float)13860.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13860.,(float)13870.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13870.,(float)
	    13880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13880.,(float)13890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13890.,(float)13900.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13900.,(float)13910.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13910.,(float)13920.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13920.,(float)
	    13930.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13930.,(float)13940.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13940.,(float)13950.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13950.,(float)13960.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13960.,(float)13970.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13970.,(float)
	    13980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13980.,(float)13990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13990.,(float)1.4e4,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)1.4e4,(
	    float)14010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)14010.,(float)14020.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)14020.,(float)14030.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14030.,(float)14040.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14040.,(float)14050.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14050.,(float)
	    14060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14060.,(float)14070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14070.,(float)14080.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14080.,(float)14090.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14090.,(float)14100.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14100.,(float)
	    14110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14110.,(float)14120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14120.,(float)14130.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14130.,(float)14140.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14140.,(float)14150.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14150.,(float)
	    14160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14160.,(float)14170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14170.,(float)14180.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14180.,(float)14190.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14190.,(float)14200.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14200.,(float)
	    14210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14210.,(float)14220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14220.,(float)14230.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14230.,(float)14240.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14240.,(float)14250.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14250.,(float)
	    14260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14260.,(float)14270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14270.,(float)14280.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14280.,(float)14290.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14290.,(float)14300.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14300.,(float)
	    14310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14310.,(float)14320.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14320.,(float)14330.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14330.,(float)14340.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14340.,(float)14350.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14350.,(float)
	    14360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14360.,(float)14370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14370.,(float)14380.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14380.,(float)14390.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14390.,(float)14400.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14400.,(float)
	    14410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14410.,(float)14420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14420.,(float)14430.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14430.,(float)14440.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14440.,(float)14450.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14450.,(float)
	    14460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14460.,(float)14470.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14470.,(float)14480.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14480.,(float)14490.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14490.,(float)14500.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14500.,(float)
	    14510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14510.,(float)14520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14520.,(float)14530.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14530.,(float)14540.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14540.,(float)14550.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14550.,(float)
	    14560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14560.,(float)14570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14570.,(float)14580.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14580.,(float)14590.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14590.,(float)14600.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14600.,(float)
	    14610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14610.,(float)14620.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14620.,(float)14630.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14630.,(float)14640.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14640.,(float)14650.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14650.,(float)
	    14660.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14660.,(float)14670.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14670.,(float)14680.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14680.,(float)14690.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14690.,(float)14700.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14700.,(float)
	    14710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14710.,(float)14720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14720.,(float)14730.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14730.,(float)14740.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14740.,(float)14750.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14750.,(float)
	    14760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14760.,(float)14770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14770.,(float)14780.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14780.,(float)14790.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14790.,(float)14800.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14800.,(float)
	    14810.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14810.,(float)14820.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14820.,(float)14830.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14830.,(float)14840.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14840.,(float)14850.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14850.,(float)
	    14860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14860.,(float)14870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14870.,(float)14880.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14880.,(float)14890.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14890.,(float)14900.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14900.,(float)
	    14910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14910.,(float)14920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14920.,(float)14930.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14930.,(float)14940.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14940.,(float)14950.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14950.,(float)
	    14960.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14960.,(float)14970.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14970.,(float)14980.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14980.,(float)14990.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14990.,(float)1.5e4,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)1.5e4,(float)
	    15010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15010.,(float)15020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15020.,(float)15030.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15030.,(float)15040.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15040.,(float)15050.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15050.,(float)
	    15060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15060.,(float)15070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15070.,(float)15080.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15080.,(float)15090.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15090.,(float)15100.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15100.,(float)
	    15110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15110.,(float)15120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15120.,(float)15130.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15130.,(float)15140.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15140.,(float)15150.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15150.,(float)
	    15160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15160.,(float)15170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15170.,(float)15180.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15180.,(float)15190.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15190.,(float)15200.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15200.,(float)
	    15210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15210.,(float)15220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15220.,(float)15230.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15230.,(float)15240.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15240.,(float)15250.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15250.,(float)
	    15260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15260.,(float)15270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15270.,(float)15280.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15280.,(float)15290.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15290.,(float)15300. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     nitrous oxide (12740 - 15290 cm-1) */

/*<    >*/
    /* Parameter adjustments */
    --a;

    /* Function Body */
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/

/*<       do i=1,8 >*/
    for (i__ = 1; i__ <= 8; ++i__) {
/*<       a(i)=acr(i,inu) >*/
	a[i__] = acr[i__ + (*inu << 3) - 9];
/*<       enddo >*/
    }

/*<       return >*/
    return 0;
/*<       end >*/
} /* niox5_ */

#ifdef __cplusplus
	}
#endif
