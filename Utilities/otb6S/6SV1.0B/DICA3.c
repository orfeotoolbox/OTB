/* DICA3.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    dica3(a,inu) >*/
/* Subroutine */ int dica3_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)4.1135e-5,(
	    float).13491,(float).019511,(float)-8.8592e-5,(float).017169,(
	    float)-8.6383e-5,(float)7620.,(float)7630.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)7630.,(float)7640.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    7640.,(float)7650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)7650.,(float)7660.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)7660.,(float)7670.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7670.,
	    (float)7680.,(float)6.9843e-6,(float).05869,(float).017996,(float)
	    -8.495e-5,(float).014986,(float)-7.9255e-5,(float)7680.,(float)
	    7690.,(float)4.4856e-5,(float).1161,(float).012313,(float)
	    -6.1208e-5,(float).0094917,(float)-5.6817e-5,(float)7690.,(float)
	    7700.,(float)2.1119e-4,(float).14823,(float).0058288,(float)
	    -3.5255e-5,(float).0029752,(float)-3.0772e-5,(float)7700.,(float)
	    7710.,(float)6.8368e-4,(float).18822,(float)2.1812e-4,(float)
	    -1.3229e-5,(float)-.0027425,(float)-8.0771e-6,(float)7710.,(float)
	    7720.,(float)8.0401e-4,(float).20648,(float)-.0032887,(float)
	    5.0708e-8,(float)-.0062117,(float)5.94e-6,(float)7720.,(float)
	    7730.,(float)3.6897e-4,(float).20612,(float)-.0045166,(float)
	    4.7173e-6,(float)-.0074494,(float)1.0697e-5,(float)7730.,(float)
	    7740.,(float).0011094,(float).31021,(float)-.0022536,(float)
	    -1.0224e-6,(float)-.0033444,(float)2.1129e-6,(float)7740.,(float)
	    7750.,(float)6.5848e-4,(float).26193,(float).0027594,(float)
	    -2.1278e-5,(float)2.5217e-4,(float)-1.8776e-5,(float)7750.,(float)
	    7760.,(float)7.3155e-5,(float).30739,(float).013041,(float)
	    -6.319e-5,(float).010499,(float)-6.0136e-5,(float)7760.,(float)
	    7770.,(float)1.9363e-5,(float).19417,(float).014647,(float)
	    -7.1772e-5,(float).011659,(float)-6.5892e-5,(float)7770.,(float)
	    7780.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)7780.,(float)7790.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)7790.,(float)7800.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)7800.,(float)7810.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)7810.,(float)7820.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)7820.,(float)7830.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)7830.,(float)7840.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    7840.,(float)7850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)7850.,(float)7860.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)7860.,(float)7870.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7870.,
	    (float)7880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7880.,(float)7890.,(float)3.719e-5,(float).1891,(
	    float)3.2484e-4,(float)-1.3576e-5,(float)-.0026862,(float)
	    -8.2867e-6,(float)7890.,(float)7900.,(float)9.4656e-5,(float).363,
	    (float)-.0020395,(float)-4.2752e-6,(float)-.0049939,(float)
	    1.2049e-6,(float)7900.,(float)7910.,(float)7.3888e-5,(float)
	    .33612,(float)-.0027854,(float)-1.4358e-6,(float)-.005628,(float)
	    3.7427e-6,(float)7910.,(float)7920.,(float)3.5986e-5,(float)
	    .14439,(float)-.0040916,(float)3.0556e-6,(float)-.0070423,(float)
	    9.0581e-6,(float)7920.,(float)7930.,(float)7.729e-5,(float).22754,
	    (float)-.0015562,(float)-6.2734e-6,(float)-.0044592,(float)
	    -1.0627e-6,(float)7930.,(float)7940.,(float)1.8388e-5,(float)
	    .092491,(float).0027526,(float)-2.4014e-5,(float)-3.4204e-4,(
	    float)-1.8092e-5,(float)7940.,(float)7950.,(float)1.9936e-7,(
	    float).0010968,(float).001703,(float)-2.0156e-5,(float)-.0013872,(
	    float)-1.4222e-5,(float)7950.,(float)7960.,(float)4.9455e-7,(
	    float).0019615,(float)-.0015846,(float)-6.8674e-6,(float)-.004292,
	    (float)-1.7132e-6,(float)7960.,(float)7970.,(float)2.7828e-7,(
	    float).0013177,(float)-.0036989,(float)1.1926e-6,(float)-.0065515,
	    (float)6.9563e-6,(float)7970.,(float)7980.,(float)3.8372e-7,(
	    float).0017475,(float)-.0038411,(float)1.7875e-6,(float)-.0067043,
	    (float)7.5942e-6,(float)7980.,(float)7990.,(float)7.3276e-7,(
	    float).003011,(float)-6.7794e-4,(float)-9.4857e-6,(float)
	    -.0034111,(float)-5.1641e-6,(float)7990.,(float)8e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8e3,(
	    float)8010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8010.,(float)8020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8020.,(float)8030.,(float)
	    3.7029e-8,(float)3.6096e-4,(float).012109,(float)-6.2089e-5,(
	    float).0089059,(float)-5.5639e-5,(float)8030.,(float)8040.,(float)
	    2.1335e-7,(float).0010751,(float).0088149,(float)-4.8284e-5,(
	    float).0057133,(float)-4.2623e-5,(float)8040.,(float)8050.,(float)
	    4.6462e-7,(float).001086,(float).0049284,(float)-3.291e-5,(float)
	    .0017555,(float)-2.6942e-5,(float)8050.,(float)8060.,(float)
	    1.5991e-6,(float).0018385,(float)6.4633e-4,(float)-1.5477e-5,(
	    float)-.0022392,(float)-1.0372e-5,(float)8060.,(float)8070.,(
	    float)1.7752e-6,(float).0016397,(float)-.0024966,(float)
	    -3.4666e-6,(float)-.0051909,(float)1.8847e-6,(float)8070.,(float)
	    8080.,(float)2.4423e-5,(float).26254,(float).02579,(float)
	    -7.7107e-5,(float).025403,(float)-1.1405e-4,(float)8080.,(float)
	    8090.,(float)6.1857e-5,(float).23309,(float).021977,(float)
	    -7.7807e-5,(float).020482,(float)-9.5402e-5,(float)8090.,(float)
	    8100.,(float)1.3473e-4,(float).23293,(float).019374,(float)
	    -8.4779e-5,(float).016814,(float)-8.2971e-5,(float)8100.,(float)
	    8110.,(float)3.3293e-4,(float).42794,(float).016857,(float)
	    -7.4779e-5,(float).01563,(float)-7.4735e-5,(float)8110.,(float)
	    8120.,(float)4.5545e-4,(float).42084,(float).014131,(float)
	    -6.4833e-5,(float).011587,(float)-6.2246e-5,(float)8120.,(float)
	    8130.,(float)3.9267e-4,(float).4459,(float).01422,(float)
	    -6.6328e-5,(float).010179,(float)-5.8346e-5,(float)8130.,(float)
	    8140.,(float).0014095,(float).66221,(float).012879,(float)
	    -6.3481e-5,(float).010259,(float)-5.9679e-5,(float)8140.,(float)
	    8150.,(float).0025744,(float).34109,(float).0081434,(float)
	    -3.5365e-5,(float).010242,(float)-4.4763e-5,(float)8150.,(float)
	    8160.,(float).0053482,(float).12345,(float).0021618,(float)
	    -2.1398e-5,(float)-8.9174e-4,(float)-1.5761e-5,(float)8160.,(
	    float)8170.,(float).0085974,(float).12902,(float)-.0011939,(float)
	    -8.269e-6,(float)-.0041809,(float)-2.5012e-6,(float)8170.,(float)
	    8180.,(float).011093,(float).20887,(float)-.0036354,(float)
	    1.357e-6,(float)-.006594,(float)7.4093e-6,(float)8180.,(float)
	    8190.,(float).0090124,(float).22951,(float)-.0041975,(float)
	    3.4952e-6,(float)-.0071581,(float)9.6182e-6,(float)8190.,(float)
	    8200.,(float).022977,(float).54634,(float)5.4679e-4,(float)
	    -7.4452e-6,(float)3.2715e-5,(float)-7.7515e-6,(float)8200.,(float)
	    8210.,(float)4.0967e-5,(float).33171,(float).029828,(float)
	    -1.2884e-4,(float).027502,(float)-1.2751e-4,(float)8210.,(float)
	    8220.,(float)1.3117e-4,(float).36661,(float).024705,(float)
	    -1.0516e-4,(float).023688,(float)-1.1029e-4,(float)8220.,(float)
	    8230.,(float)4.1427e-4,(float).33236,(float).018964,(float)
	    -8.1986e-5,(float).018612,(float)-8.8547e-5,(float)8230.,(float)
	    8240.,(float).0011268,(float).30228,(float).013786,(float)
	    -6.0455e-5,(float).014251,(float)-6.6968e-5,(float)8240.,(float)
	    8250.,(float).0038631,(float).3115,(float).0080823,(float)
	    -3.8679e-5,(float).0084273,(float)-4.2578e-5,(float)8250.,(float)
	    8260.,(float).0060039,(float).26991,(float).0040499,(float)
	    -2.3093e-5,(float).0051036,(float)-2.751e-5,(float)8260.,(float)
	    8270.,(float).014968,(float).24443,(float)-6.0624e-4,(float)
	    -8.9134e-6,(float)-.0014888,(float)-7.5779e-6,(float)8270.,(float)
	    8280.,(float).015831,(float).42226,(float)-.0020274,(float)
	    6.2203e-6,(float)-1.636e-4,(float)-2.1524e-6,(float)8280.,(float)
	    8290.,(float).0086272,(float).42161,(float)-.0028717,(float)
	    1.6391e-5,(float)5.8996e-4,(float)1.5552e-5,(float)8290.,(float)
	    8300.,(float).038978,(float).60506,(float)-1.8963e-4,(float)
	    -4.6658e-6,(float)-.0011243,(float)-4.8784e-6,(float)8300.,(float)
	    8310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)8310.,(float)8320.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)8320.,(float)8330.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)8330.,(float)8340.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)8340.,(float)8350.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)8350.,(float)8360.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)8360.,(float)8370.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    8370.,(float)8380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)8380.,(float)8390.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)8390.,(float)8400.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8400.,
	    (float)8410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8410.,(float)8420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8420.,(float)8430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8430.,(
	    float)8440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8440.,(float)8450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8450.,(float)8460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8460.,(
	    float)8470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8470.,(float)8480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8480.,(float)8490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8490.,(
	    float)8500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8500.,(float)8510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8510.,(float)8520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8520.,(
	    float)8530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8530.,(float)8540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8540.,(float)8550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8550.,(
	    float)8560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8560.,(float)8570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8570.,(float)8580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8580.,(
	    float)8590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8590.,(float)8600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8600.,(float)8610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8610.,(
	    float)8620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8620.,(float)8630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8630.,(float)8640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8640.,(
	    float)8650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8650.,(float)8660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8660.,(float)8670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8670.,(
	    float)8680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8680.,(float)8690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8690.,(float)8700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8700.,(
	    float)8710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8710.,(float)8720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8720.,(float)8730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8730.,(
	    float)8740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8740.,(float)8750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8750.,(float)8760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8760.,(
	    float)8770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8770.,(float)8780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8780.,(float)8790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8790.,(
	    float)8800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8800.,(float)8810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8810.,(float)8820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8820.,(
	    float)8830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8830.,(float)8840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8840.,(float)8850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8850.,(
	    float)8860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8860.,(float)8870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8870.,(float)8880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8880.,(
	    float)8890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8890.,(float)8900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8900.,(float)8910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8910.,(
	    float)8920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8920.,(float)8930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8930.,(float)8940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8940.,(
	    float)8950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8950.,(float)8960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8960.,(float)8970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8970.,(
	    float)8980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8980.,(float)8990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8990.,(float)9e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9e3,(
	    float)9010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9010.,(float)9020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9020.,(float)9030.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9030.,(
	    float)9040.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9040.,(float)9050.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9050.,(float)9060.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9060.,(
	    float)9070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9070.,(float)9080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9080.,(float)9090.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9090.,(
	    float)9100.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9100.,(float)9110.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9110.,(float)9120.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9120.,(
	    float)9130.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9130.,(float)9140.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9140.,(float)9150.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9150.,(
	    float)9160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9160.,(float)9170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9170.,(float)9180.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9180.,(
	    float)9190.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9190.,(float)9200.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9200.,(float)9210.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9210.,(
	    float)9220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9220.,(float)9230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9230.,(float)9240.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9240.,(
	    float)9250.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9250.,(float)9260.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9260.,(float)9270.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9270.,(
	    float)9280.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9280.,(float)9290.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9290.,(float)9300.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9300.,(
	    float)9310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9310.,(float)9320.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9320.,(float)9330.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9330.,(
	    float)9340.,(float)9.9593e-6,(float).060237,(float).0097616,(
	    float)-5.2148e-5,(float).0066534,(float)-4.6124e-5,(float)9340.,(
	    float)9350.,(float)4.3567e-5,(float).12051,(float).0054505,(float)
	    -3.4277e-5,(float).0024633,(float)-2.9032e-5,(float)9350.,(float)
	    9360.,(float)8.8924e-5,(float).12477,(float).0012273,(float)
	    -1.774e-5,(float)-.0018429,(float)-1.1984e-5,(float)9360.,(float)
	    9370.,(float)1.5573e-4,(float).16489,(float)-.0021342,(float)
	    -4.4631e-6,(float)-.005046,(float)1.1731e-6,(float)9370.,(float)
	    9380.,(float)9.4382e-5,(float).17579,(float)-.0041243,(float)
	    3.1944e-6,(float)-.0071235,(float)9.3601e-6,(float)9380.,(float)
	    9390.,(float)2.1829e-4,(float).27491,(float)-.0034678,(float)
	    8.2722e-7,(float)-.0064858,(float)7.0242e-6,(float)9390.,(float)
	    9400.,(float)2.27e-4,(float).36616,(float).0019245,(float)
	    -1.579e-5,(float)-1.5673e-4,(float)-1.5436e-5,(float)9400.,(float)
	    9410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)9410.,(float)9420.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)9420.,(float)9430.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)9430.,(float)9440.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)9440.,(float)9450.,(float)4.0618e-5,(float).33672,(float)
	    .016922,(float)-7.9759e-5,(float).013854,(float)-7.4271e-5,(float)
	    9450.,(float)9460.,(float)8.026e-5,(float).35567,(float).013366,(
	    float)-6.6285e-5,(float).010448,(float)-6.0815e-5,(float)9460.,(
	    float)9470.,(float)1.7445e-4,(float).15326,(float).0081302,(float)
	    -4.4538e-5,(float).0056112,(float)-4.1022e-5,(float)9470.,(float)
	    9480.,(float)3.3041e-4,(float).26053,(float).0049592,(float)
	    -2.831e-5,(float).0049032,(float)-3.3415e-5,(float)9480.,(float)
	    9490.,(float)8.9723e-4,(float).1582,(float)-1.1481e-4,(float)
	    -1.225e-5,(float)-.0031268,(float)-6.7086e-6,(float)9490.,(float)
	    9500.,(float)8.7248e-4,(float).13522,(float)-.0029599,(float)
	    -1.3666e-6,(float)-.005813,(float)4.3278e-6,(float)9500.,(float)
	    9510.,(float)5.7391e-4,(float).20153,(float)-.0043771,(float)
	    4.1806e-6,(float)-.0073125,(float)1.0212e-5,(float)9510.,(float)
	    9520.,(float).002106,(float).33852,(float)-.0026269,(float)
	    -1.9082e-6,(float)-.0056393,(float)3.824e-6,(float)9520.,(float)
	    9530.,(float)8.7766e-4,(float).35363,(float).0044041,(float)
	    -2.4109e-5,(float).0031707,(float)-2.6057e-5,(float)9530.,(float)
	    9540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)9540.,(float)9550.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)9550.,(float)9560.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)9560.,(float)9570.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)9570.,(float)9580.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)9580.,(float)9590.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)9590.,(float)9600.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    9600.,(float)9610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)9610.,(float)9620.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)9620.,(float)9630.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9630.,
	    (float)9640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9640.,(float)9650.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9650.,(float)9660.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9660.,(
	    float)9670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9670.,(float)9680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9680.,(float)9690.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9690.,(
	    float)9700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9700.,(float)9710.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9710.,(float)9720.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9720.,(
	    float)9730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9730.,(float)9740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9740.,(float)9750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9750.,(
	    float)9760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9760.,(float)9770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9770.,(float)9780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9780.,(
	    float)9790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9790.,(float)9800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9800.,(float)9810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9810.,(
	    float)9820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9820.,(float)9830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9830.,(float)9840.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9840.,(
	    float)9850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9850.,(float)9860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9860.,(float)9870.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9870.,(
	    float)9880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9880.,(float)9890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9890.,(float)9900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9900.,(
	    float)9910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9910.,(float)9920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9920.,(float)9930.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9930.,(
	    float)9940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9940.,(float)9950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9950.,(float)9960.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9960.,(
	    float)9970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9970.,(float)9980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9980.,(float)9990.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9990.,(
	    float)1e4,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)1e4,(float)10010.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10010.,(float)10020.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10020.,(float)10030.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10030.,(float)10040.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10040.,(float)
	    10050.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10050.,(float)10060.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10060.,(float)10070.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10070.,(float)10080.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10080.,(float)10090.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10090.,(float)
	    10100.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10100.,(float)10110.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10110.,(float)10120.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10120.,(float)10130.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10130.,(float)10140.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10140.,(float)
	    10150.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10150.,(float)10160.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10160.,(float)10170.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10170.,(float)10180. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     carbon dioxide (7620 - 10170 cm-1) */

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
} /* dica3_ */

#ifdef __cplusplus
	}
#endif
