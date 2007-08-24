/* MOCA4.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    moca4(a,inu) >*/
/* Subroutine */ int moca4_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)10180.,(float)
	    10190.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10190.,(float)10200.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10200.,(float)10210.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10210.,(float)10220.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10220.,(float)10230.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10230.,(float)
	    10240.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10240.,(float)10250.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10250.,(float)10260.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10260.,(float)10270.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10270.,(float)10280.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10280.,(float)
	    10290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10290.,(float)10300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10300.,(float)10310.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10310.,(float)10320.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10320.,(float)10330.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10330.,(float)
	    10340.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10340.,(float)10350.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10350.,(float)10360.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10360.,(float)10370.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10370.,(float)10380.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10380.,(float)
	    10390.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10390.,(float)10400.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10400.,(float)10410.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10410.,(float)10420.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10420.,(float)10430.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10430.,(float)
	    10440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10440.,(float)10450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10450.,(float)10460.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10460.,(float)10470.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10470.,(float)10480.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10480.,(float)
	    10490.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10490.,(float)10500.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10500.,(float)10510.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10510.,(float)10520.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10520.,(float)10530.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10530.,(float)
	    10540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10540.,(float)10550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10550.,(float)10560.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10560.,(float)10570.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10570.,(float)10580.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10580.,(float)
	    10590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10590.,(float)10600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10600.,(float)10610.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10610.,(float)10620.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10620.,(float)10630.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10630.,(float)
	    10640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10640.,(float)10650.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10650.,(float)10660.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10660.,(float)10670.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10670.,(float)10680.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10680.,(float)
	    10690.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10690.,(float)10700.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10700.,(float)10710.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10710.,(float)10720.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10720.,(float)10730.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10730.,(float)
	    10740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10740.,(float)10750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10750.,(float)10760.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10760.,(float)10770.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10770.,(float)10780.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10780.,(float)
	    10790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10790.,(float)10800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10800.,(float)10810.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10810.,(float)10820.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10820.,(float)10830.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10830.,(float)
	    10840.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10840.,(float)10850.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10850.,(float)10860.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10860.,(float)10870.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10870.,(float)10880.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10880.,(float)
	    10890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10890.,(float)10900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10900.,(float)10910.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10910.,(float)10920.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10920.,(float)10930.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10930.,(float)
	    10940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10940.,(float)10950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10950.,(float)10960.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10960.,(float)10970.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10970.,(float)10980.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10980.,(float)
	    10990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10990.,(float)1.1e4,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)1.1e4,(float)11010.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11010.,(
	    float)11020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)11020.,(float)11030.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)11030.,(float)11040.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11040.,(float)11050.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11050.,(float)11060.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11060.,(float)
	    11070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11070.,(float)11080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11080.,(float)11090.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11090.,(float)11100.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11100.,(float)11110.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11110.,(float)
	    11120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11120.,(float)11130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11130.,(float)11140.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11140.,(float)11150.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11150.,(float)11160.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11160.,(float)
	    11170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11170.,(float)11180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11180.,(float)11190.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11190.,(float)11200.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11200.,(float)11210.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11210.,(float)
	    11220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11220.,(float)11230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11230.,(float)11240.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11240.,(float)11250.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11250.,(float)11260.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11260.,(float)
	    11270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11270.,(float)11280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11280.,(float)11290.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11290.,(float)11300.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11300.,(float)11310.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11310.,(float)
	    11320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11320.,(float)11330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11330.,(float)11340.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11340.,(float)11350.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11350.,(float)11360.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11360.,(float)
	    11370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11370.,(float)11380.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11380.,(float)11390.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11390.,(float)11400.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11400.,(float)11410.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11410.,(float)
	    11420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11420.,(float)11430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11430.,(float)11440.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11440.,(float)11450.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11450.,(float)11460.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11460.,(float)
	    11470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11470.,(float)11480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11480.,(float)11490.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11490.,(float)11500.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11500.,(float)11510.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11510.,(float)
	    11520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11520.,(float)11530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11530.,(float)11540.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11540.,(float)11550.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11550.,(float)11560.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11560.,(float)
	    11570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11570.,(float)11580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11580.,(float)11590.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11590.,(float)11600.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11600.,(float)11610.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11610.,(float)
	    11620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11620.,(float)11630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11630.,(float)11640.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11640.,(float)11650.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11650.,(float)11660.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11660.,(float)
	    11670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11670.,(float)11680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11680.,(float)11690.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11690.,(float)11700.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11700.,(float)11710.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11710.,(float)
	    11720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11720.,(float)11730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11730.,(float)11740.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11740.,(float)11750.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11750.,(float)11760.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11760.,(float)
	    11770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11770.,(float)11780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11780.,(float)11790.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11790.,(float)11800.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11800.,(float)11810.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11810.,(float)
	    11820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11820.,(float)11830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11830.,(float)11840.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11840.,(float)11850.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11850.,(float)11860.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11860.,(float)
	    11870.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11870.,(float)11880.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11880.,(float)11890.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11890.,(float)11900.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11900.,(float)11910.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11910.,(float)
	    11920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11920.,(float)11930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11930.,(float)11940.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11940.,(float)11950.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11950.,(float)11960.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11960.,(float)
	    11970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11970.,(float)11980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11980.,(float)11990.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11990.,(float)1.2e4,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)1.2e4,(float)12010.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)12010.,(float)
	    12020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12020.,(float)12030.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12030.,(float)12040.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12040.,(float)12050.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12050.,(float)12060.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12060.,(float)
	    12070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12070.,(float)12080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12080.,(float)12090.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12090.,(float)12100.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12100.,(float)12110.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12110.,(float)
	    12120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12120.,(float)12130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12130.,(float)12140.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12140.,(float)12150.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12150.,(float)12160.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12160.,(float)
	    12170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12170.,(float)12180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12180.,(float)12190.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12190.,(float)12200.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12200.,(float)12210.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12210.,(float)
	    12220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12220.,(float)12230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12230.,(float)12240.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12240.,(float)12250.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12250.,(float)12260.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12260.,(float)
	    12270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12270.,(float)12280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12280.,(float)12290.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12290.,(float)12300.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12300.,(float)12310.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12310.,(float)
	    12320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12320.,(float)12330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12330.,(float)12340.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12340.,(float)12350.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12350.,(float)12360.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12360.,(float)
	    12370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12370.,(float)12380.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12380.,(float)12390.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12390.,(float)12400.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12400.,(float)12410.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12410.,(float)
	    12420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12420.,(float)12430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12430.,(float)12440.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12440.,(float)12450.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12450.,(float)12460.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12460.,(float)
	    12470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12470.,(float)12480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12480.,(float)12490.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12490.,(float)12500.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12500.,(float)12510.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12510.,(float)
	    12520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12520.,(float)12530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12530.,(float)12540.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12540.,(float)12550.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12550.,(float)12560.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12560.,(float)
	    12570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12570.,(float)12580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12580.,(float)12590.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12590.,(float)12600.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12600.,(float)12610.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12610.,(float)
	    12620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12620.,(float)12630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12630.,(float)12640.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12640.,(float)12650.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12650.,(float)12660.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12660.,(float)
	    12670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12670.,(float)12680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12680.,(float)12690.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12690.,(float)12700.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12700.,(float)12710.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12710.,(float)
	    12720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12720.,(float)12730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12730.,(float)12740. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     carbon monoxide (10180 - 12730 cm-1) */

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
} /* moca4_ */

#ifdef __cplusplus
	}
#endif
