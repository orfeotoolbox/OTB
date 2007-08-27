/* METH3.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    meth3(a,inu) >*/
/* Subroutine */ int meth3_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)7620.,(float)7630.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    7630.,(float)7640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)7640.,(float)7650.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)7650.,(float)7660.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7660.,
	    (float)7670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7670.,(float)7680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7680.,(float)7690.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7690.,(
	    float)7700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7700.,(float)7710.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7710.,(float)7720.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7720.,(
	    float)7730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7730.,(float)7740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7740.,(float)7750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7750.,(
	    float)7760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7760.,(float)7770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7770.,(float)7780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7780.,(
	    float)7790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7790.,(float)7800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7800.,(float)7810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7810.,(
	    float)7820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7820.,(float)7830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7830.,(float)7840.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7840.,(
	    float)7850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7850.,(float)7860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7860.,(float)7870.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7870.,(
	    float)7880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7880.,(float)7890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7890.,(float)7900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7900.,(
	    float)7910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7910.,(float)7920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7920.,(float)7930.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7930.,(
	    float)7940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7940.,(float)7950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7950.,(float)7960.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7960.,(
	    float)7970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7970.,(float)7980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7980.,(float)7990.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7990.,(
	    float)8e3,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8e3,(float)8010.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8010.,(float)8020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8020.,(
	    float)8030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8030.,(float)8040.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8040.,(float)8050.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8050.,(
	    float)8060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8060.,(float)8070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8070.,(float)8080.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8080.,(
	    float)8090.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8090.,(float)8100.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8100.,(float)8110.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8110.,(
	    float)8120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8120.,(float)8130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8130.,(float)8140.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8140.,(
	    float)8150.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8150.,(float)8160.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8160.,(float)8170.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8170.,(
	    float)8180.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8180.,(float)8190.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8190.,(float)8200.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8200.,(
	    float)8210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8210.,(float)8220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8220.,(float)8230.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8230.,(
	    float)8240.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8240.,(float)8250.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8250.,(float)8260.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8260.,(
	    float)8270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8270.,(float)8280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8280.,(float)8290.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8290.,(
	    float)8300.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8300.,(float)8310.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8310.,(float)8320.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8320.,(
	    float)8330.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8330.,(float)8340.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8340.,(float)8350.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8350.,(
	    float)8360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8360.,(float)8370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8370.,(float)8380.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8380.,(
	    float)8390.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8390.,(float)8400.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8400.,(float)8410.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8410.,(
	    float)8420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8420.,(float)8430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8430.,(float)8440.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8440.,(
	    float)8450.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8450.,(float)8460.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8460.,(float)8470.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8470.,(
	    float)8480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8480.,(float)8490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8490.,(float)8500.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8500.,(
	    float)8510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8510.,(float)8520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8520.,(float)8530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8530.,(
	    float)8540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8540.,(float)8550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8550.,(float)8560.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8560.,(
	    float)8570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8570.,(float)8580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8580.,(float)8590.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8590.,(
	    float)8600.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8600.,(float)8610.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8610.,(float)8620.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8620.,(
	    float)8630.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8630.,(float)8640.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8640.,(float)8650.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8650.,(
	    float)8660.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8660.,(float)8670.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8670.,(float)8680.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8680.,(
	    float)8690.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8690.,(float)8700.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8700.,(float)8710.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8710.,(
	    float)8720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8720.,(float)8730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8730.,(float)8740.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8740.,(
	    float)8750.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8750.,(float)8760.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8760.,(float)8770.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8770.,(
	    float)8780.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8780.,(float)8790.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8790.,(float)8800.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8800.,(
	    float)8810.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8810.,(float)8820.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8820.,(float)8830.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8830.,(
	    float)8840.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8840.,(float)8850.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8850.,(float)8860.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8860.,(
	    float)8870.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8870.,(float)8880.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8880.,(float)8890.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8890.,(
	    float)8900.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8900.,(float)8910.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8910.,(float)8920.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8920.,(
	    float)8930.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8930.,(float)8940.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8940.,(float)8950.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8950.,(
	    float)8960.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8960.,(float)8970.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8970.,(float)8980.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8980.,(
	    float)8990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8990.,(float)9e3,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9e3,(float)9010.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9010.,(
	    float)9020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9020.,(float)9030.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9030.,(float)9040.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9040.,(
	    float)9050.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9050.,(float)9060.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9060.,(float)9070.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9070.,(
	    float)9080.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9080.,(float)9090.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9090.,(float)9100.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9100.,(
	    float)9110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9110.,(float)9120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9120.,(float)9130.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9130.,(
	    float)9140.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9140.,(float)9150.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9150.,(float)9160.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9160.,(
	    float)9170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9170.,(float)9180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9180.,(float)9190.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9190.,(
	    float)9200.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9200.,(float)9210.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9210.,(float)9220.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9220.,(
	    float)9230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9230.,(float)9240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9240.,(float)9250.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9250.,(
	    float)9260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9260.,(float)9270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9270.,(float)9280.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9280.,(
	    float)9290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9290.,(float)9300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9300.,(float)9310.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9310.,(
	    float)9320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9320.,(float)9330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9330.,(float)9340.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9340.,(
	    float)9350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9350.,(float)9360.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9360.,(float)9370.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9370.,(
	    float)9380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9380.,(float)9390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9390.,(float)9400.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9400.,(
	    float)9410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9410.,(float)9420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9420.,(float)9430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9430.,(
	    float)9440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9440.,(float)9450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9450.,(float)9460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9460.,(
	    float)9470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9470.,(float)9480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9480.,(float)9490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9490.,(
	    float)9500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9500.,(float)9510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9510.,(float)9520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9520.,(
	    float)9530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9530.,(float)9540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9540.,(float)9550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9550.,(
	    float)9560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9560.,(float)9570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9570.,(float)9580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9580.,(
	    float)9590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9590.,(float)9600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9600.,(float)9610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9610.,(
	    float)9620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9620.,(float)9630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9630.,(float)9640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9640.,(
	    float)9650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9650.,(float)9660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9660.,(float)9670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9670.,(
	    float)9680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9680.,(float)9690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9690.,(float)9700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9700.,(
	    float)9710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9710.,(float)9720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9720.,(float)9730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9730.,(
	    float)9740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9740.,(float)9750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9750.,(float)9760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9760.,(
	    float)9770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9770.,(float)9780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9780.,(float)9790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9790.,(
	    float)9800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9800.,(float)9810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9810.,(float)9820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9820.,(
	    float)9830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9830.,(float)9840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9840.,(float)9850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9850.,(
	    float)9860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9860.,(float)9870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9870.,(float)9880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9880.,(
	    float)9890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9890.,(float)9900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9900.,(float)9910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9910.,(
	    float)9920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9920.,(float)9930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9930.,(float)9940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9940.,(
	    float)9950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9950.,(float)9960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9960.,(float)9970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9970.,(
	    float)9980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9980.,(float)9990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9990.,(float)1e4,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)1e4,(
	    float)10010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)10010.,(float)10020.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)10020.,(float)10030.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10030.,(float)10040.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10040.,(float)10050.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10050.,(float)
	    10060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10060.,(float)10070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10070.,(float)10080.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10080.,(float)10090.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10090.,(float)10100.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10100.,(float)
	    10110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10110.,(float)10120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10120.,(float)10130.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10130.,(float)10140.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10140.,(float)10150.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10150.,(float)
	    10160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10160.,(float)10170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10170.,(float)10180. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     methane (7620 - 10170 cm-1) */

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
} /* meth3_ */

#ifdef __cplusplus
	}
#endif
