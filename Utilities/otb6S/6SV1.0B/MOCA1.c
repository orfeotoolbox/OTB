/* MOCA1.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    moca1(a,inu) >*/
/* Subroutine */ int moca1_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)2500.,(float)2510.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    2510.,(float)2520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)2520.,(float)2530.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)2530.,(float)2540.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2540.,
	    (float)2550.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2550.,(float)2560.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2560.,(float)2570.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2570.,(
	    float)2580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2580.,(float)2590.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2590.,(float)2600.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2600.,(
	    float)2610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2610.,(float)2620.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2620.,(float)2630.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2630.,(
	    float)2640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2640.,(float)2650.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2650.,(float)2660.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2660.,(
	    float)2670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2670.,(float)2680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2680.,(float)2690.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2690.,(
	    float)2700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2700.,(float)2710.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2710.,(float)2720.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2720.,(
	    float)2730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2730.,(float)2740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2740.,(float)2750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2750.,(
	    float)2760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2760.,(float)2770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2770.,(float)2780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2780.,(
	    float)2790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2790.,(float)2800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2800.,(float)2810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2810.,(
	    float)2820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2820.,(float)2830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2830.,(float)2840.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2840.,(
	    float)2850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2850.,(float)2860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2860.,(float)2870.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2870.,(
	    float)2880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2880.,(float)2890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2890.,(float)2900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2900.,(
	    float)2910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2910.,(float)2920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2920.,(float)2930.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2930.,(
	    float)2940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2940.,(float)2950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2950.,(float)2960.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2960.,(
	    float)2970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2970.,(float)2980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2980.,(float)2990.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2990.,(
	    float)3e3,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3e3,(float)3010.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3010.,(float)3020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3020.,(
	    float)3030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3030.,(float)3040.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3040.,(float)3050.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3050.,(
	    float)3060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3060.,(float)3070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3070.,(float)3080.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3080.,(
	    float)3090.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3090.,(float)3100.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3100.,(float)3110.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3110.,(
	    float)3120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3120.,(float)3130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3130.,(float)3140.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3140.,(
	    float)3150.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3150.,(float)3160.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3160.,(float)3170.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3170.,(
	    float)3180.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3180.,(float)3190.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3190.,(float)3200.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3200.,(
	    float)3210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3210.,(float)3220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3220.,(float)3230.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3230.,(
	    float)3240.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3240.,(float)3250.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3250.,(float)3260.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3260.,(
	    float)3270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3270.,(float)3280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3280.,(float)3290.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3290.,(
	    float)3300.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3300.,(float)3310.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3310.,(float)3320.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3320.,(
	    float)3330.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3330.,(float)3340.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3340.,(float)3350.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3350.,(
	    float)3360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3360.,(float)3370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3370.,(float)3380.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3380.,(
	    float)3390.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3390.,(float)3400.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3400.,(float)3410.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3410.,(
	    float)3420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3420.,(float)3430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3430.,(float)3440.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3440.,(
	    float)3450.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3450.,(float)3460.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3460.,(float)3470.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3470.,(
	    float)3480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3480.,(float)3490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3490.,(float)3500.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3500.,(
	    float)3510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3510.,(float)3520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3520.,(float)3530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3530.,(
	    float)3540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3540.,(float)3550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3550.,(float)3560.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3560.,(
	    float)3570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3570.,(float)3580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3580.,(float)3590.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3590.,(
	    float)3600.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3600.,(float)3610.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3610.,(float)3620.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3620.,(
	    float)3630.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3630.,(float)3640.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3640.,(float)3650.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3650.,(
	    float)3660.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3660.,(float)3670.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3670.,(float)3680.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3680.,(
	    float)3690.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3690.,(float)3700.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3700.,(float)3710.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3710.,(
	    float)3720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3720.,(float)3730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3730.,(float)3740.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3740.,(
	    float)3750.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3750.,(float)3760.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3760.,(float)3770.,(float)
	    2.6188e-23,(float)7.5648e-7,(float).14705,(float)-5.9469e-4,(
	    float).14469,(float)-5.9147e-4,(float)3770.,(float)3780.,(float)
	    1.7237e-22,(float)7.5958e-7,(float).13909,(float)-5.6296e-4,(
	    float).13671,(float)-5.596e-4,(float)3780.,(float)3790.,(float)
	    3.0601e-22,(float)3.9946e-7,(float).13415,(float)-5.447e-4,(float)
	    .13135,(float)-5.3906e-4,(float)3790.,(float)3800.,(float)
	    2.5148e-21,(float)7.6406e-7,(float).12775,(float)-5.1775e-4,(
	    float).12533,(float)-5.1417e-4,(float)3800.,(float)3810.,(float)
	    2.5745e-19,(float)6.6661e-7,(float).10289,(float)-4.1172e-4,(
	    float).10359,(float)-4.1229e-4,(float)3810.,(float)3820.,(float)
	    2.3471e-18,(float)8.5834e-7,(float).095064,(float)-3.8496e-4,(
	    float).093786,(float)-3.8192e-4,(float)3820.,(float)3830.,(float)
	    4.4267e-18,(float)6.1131e-7,(float).090479,(float)-3.6372e-4,(
	    float).091063,(float)-3.6128e-4,(float)3830.,(float)3840.,(float)
	    3.7544e-17,(float)9.6342e-7,(float).083552,(float)-3.3748e-4,(
	    float).083109,(float)-3.3494e-4,(float)3840.,(float)3850.,(float)
	    6.6043e-17,(float)4.5762e-6,(float).080581,(float)-2.9237e-4,(
	    float).1186,(float)-1.9899e-4,(float)3850.,(float)3860.,(float)
	    5.0104e-16,(float)3.0566e-6,(float).073204,(float)-2.8334e-4,(
	    float).10085,(float)-1.1404e-4,(float)3860.,(float)3870.,(float)
	    2.5102e-15,(float)5.6505e-6,(float).066558,(float)-2.4745e-4,(
	    float).10411,(float)-1.0858e-4,(float)3870.,(float)3880.,(float)
	    1.179e-14,(float)7.1883e-6,(float).060355,(float)-2.1477e-4,(
	    float).099433,(float)-1.0798e-4,(float)3880.,(float)3890.,(float)
	    1.7518e-14,(float)1.0491e-5,(float).058888,(float)-1.6207e-4,(
	    float).09819,(float)-2.0655e-4,(float)3890.,(float)3900.,(float)
	    1.2334e-13,(float)8.2478e-5,(float).061162,(float)-4.6855e-5,(
	    float).099456,(float)-3.0433e-4,(float)3900.,(float)3910.,(float)
	    5.9987e-13,(float)1.9476e-4,(float).061687,(float)1.0764e-5,(
	    float).093436,(float)-3.2996e-4,(float)3910.,(float)3920.,(float)
	    1.7948e-12,(float)9.9622e-5,(float).052116,(float)1.2152e-5,(
	    float).087699,(float)-2.7025e-4,(float)3920.,(float)3930.,(float)
	    8.3226e-12,(float)3.291e-4,(float).054625,(float)5.1416e-5,(float)
	    .089131,(float)-2.3629e-4,(float)3930.,(float)3940.,(float)
	    2.0276e-11,(float)2.1725e-4,(float).045681,(float)6.3672e-5,(
	    float).093594,(float)-1.044e-4,(float)3940.,(float)3950.,(float)
	    9.452e-11,(float)4.2081e-4,(float).049658,(float)5.7601e-5,(float)
	    .079407,(float)-1.9603e-4,(float)3950.,(float)3960.,(float)
	    2.0604e-10,(float)4.2113e-4,(float).043092,(float)7.9797e-5,(
	    float).08456,(float)-1.0065e-4,(float)3960.,(float)3970.,(float)
	    9.2559e-10,(float)5.2769e-4,(float).046842,(float)2.13e-5,(float)
	    .070955,(float)-1.5981e-4,(float)3970.,(float)3980.,(float)
	    4.0093e-9,(float).0016525,(float).048905,(float)-4.3479e-5,(float)
	    .080584,(float)-1.3211e-4,(float)3980.,(float)3990.,(float)
	    7.2188e-9,(float).0049631,(float).047903,(float)3.506e-5,(float)
	    .087341,(float)-2.2515e-4,(float)3990.,(float)4e3,(float)
	    3.5244e-8,(float).0026406,(float).044217,(float)-4.3905e-5,(float)
	    .074799,(float)-1.3539e-4,(float)4e3,(float)4010.,(float)
	    1.4041e-7,(float).0058689,(float).045795,(float)-7.4645e-5,(float)
	    .073346,(float)-1.7741e-4,(float)4010.,(float)4020.,(float)
	    4.8242e-7,(float).0031728,(float).03946,(float)-9.4315e-5,(float)
	    .063747,(float)-1.0552e-4,(float)4020.,(float)4030.,(float)
	    1.5614e-6,(float).0029789,(float).03481,(float)-9.4517e-5,(float)
	    .058578,(float)-7.6764e-5,(float)4030.,(float)4040.,(float)
	    2.3212e-6,(float).011038,(float).038571,(float)-4.0092e-6,(float)
	    .064826,(float)-1.8611e-4,(float)4040.,(float)4050.,(float)
	    1.0679e-5,(float).014942,(float).035924,(float)2.0954e-7,(float)
	    .05826,(float)-1.6362e-4,(float)4050.,(float)4060.,(float)
	    2.5314e-5,(float).0076112,(float).028757,(float)-2.7274e-6,(float)
	    .050639,(float)-1.0468e-4,(float)4060.,(float)4070.,(float)
	    8.2488e-5,(float).019238,(float).031099,(float)3.2981e-5,(float)
	    .049127,(float)-1.4923e-4,(float)4070.,(float)4080.,(float)
	    1.5838e-4,(float).010696,(float).024633,(float)3.8132e-5,(float)
	    .04326,(float)-9.7166e-5,(float)4080.,(float)4090.,(float)
	    5.3014e-4,(float).027912,(float).028944,(float)3.4086e-5,(float)
	    .042425,(float)-1.3608e-4,(float)4090.,(float)4100.,(float)
	    .0019236,(float).031464,(float).026171,(float)2.5248e-5,(float)
	    .036412,(float)-1.1618e-4,(float)4100.,(float)4110.,(float)
	    .002417,(float).018774,(float).023171,(float)3.1047e-5,(float)
	    .032949,(float)-9.3254e-5,(float)4110.,(float)4120.,(float)
	    .0095957,(float).039252,(float).027808,(float)-5.0845e-5,(float)
	    .030866,(float)-1.1461e-4,(float)4120.,(float)4130.,(float)
	    .029222,(float).041309,(float).025476,(float)-7.0834e-5,(float)
	    .025927,(float)-9.9072e-5,(float)4130.,(float)4140.,(float)
	    .080521,(float).042144,(float).023591,(float)-9.2915e-5,(float)
	    .022029,(float)-9.0905e-5,(float)4140.,(float)4150.,(float).21163,
	    (float).043454,(float).019801,(float)-8.4058e-5,(float).017719,(
	    float)-7.6225e-5,(float)4150.,(float)4160.,(float).20177,(float)
	    .023178,(float).017214,(float)-7.6243e-5,(float).015553,(float)
	    -6.4832e-5,(float)4160.,(float)4170.,(float).7632,(float).046725,(
	    float).01416,(float)-6.3818e-5,(float).012147,(float)-5.4565e-5,(
	    float)4170.,(float)4180.,(float)1.5947,(float).047792,(float)
	    .010708,(float)-5.0352e-5,(float).0083233,(float)-4.2778e-5,(
	    float)4180.,(float)4190.,(float)5.2669,(float).073957,(float)
	    .0067643,(float)-3.4467e-5,(float).004319,(float)-2.8069e-5,(
	    float)4190.,(float)4200.,(float)6.4202,(float).051382,(float)
	    .0036772,(float)-2.2614e-5,(float)9.742e-4,(float)-1.6763e-5,(
	    float)4200.,(float)4210.,(float)9.2445,(float).052627,(float)
	    .0014938,(float)-1.3915e-5,(float)-.0010491,(float)-6.9012e-6,(
	    float)4210.,(float)4220.,(float)11.75,(float).05381,(float)
	    -3.2681e-4,(float)-6.6562e-6,(float)-.0028054,(float)1.0375e-6,(
	    float)4220.,(float)4230.,(float)12.919,(float).055506,(float)
	    -.0017838,(float)-8.5608e-7,(float)-.0042665,(float)7.1579e-6,(
	    float)4230.,(float)4240.,(float)16.381,(float).089081,(float)
	    -.003049,(float)4.1985e-6,(float)-.0056615,(float)1.1648e-5,(
	    float)4240.,(float)4250.,(float)5.0278,(float).066105,(float)
	    -.0038255,(float)7.3053e-6,(float)-.0061865,(float)1.7191e-5,(
	    float)4250.,(float)4260.,(float)5.3208,(float).065464,(float)
	    -.0039797,(float)7.8726e-6,(float)-.006508,(float)1.6403e-5,(
	    float)4260.,(float)4270.,(float)18.844,(float).088395,(float)
	    -.0034205,(float)5.6382e-6,(float)-.0061631,(float)1.2216e-5,(
	    float)4270.,(float)4280.,(float)24.188,(float).081391,(float)
	    -.0020758,(float)3.3792e-7,(float)-.004851,(float)6.276e-6,(float)
	    4280.,(float)4290.,(float)20.85,(float).078388,(float)7.1143e-5,(
	    float)-8.1227e-6,(float)-.0026924,(float)-2.6258e-6,(float)4290.,(
	    float)4300.,(float)16.326,(float).099662,(float).003472,(float)
	    -2.1159e-5,(float)8.2032e-4,(float)-1.6489e-5,(float)4300.,(float)
	    4310.,(float)5.9676,(float).090468,(float).0087017,(float)
	    -4.1571e-5,(float).0061894,(float)-3.7687e-5,(float)4310.,(float)
	    4320.,(float)1.4674,(float).080452,(float).015334,(float)
	    -6.7528e-5,(float).012977,(float)-6.4486e-5,(float)4320.,(float)
	    4330.,(float).26137,(float).087098,(float).023749,(float)
	    -9.9448e-5,(float).02192,(float)-9.8971e-5,(float)4330.,(float)
	    4340.,(float).017052,(float).094008,(float).036064,(float)
	    -1.4594e-4,(float).035363,(float)-1.4924e-4,(float)4340.,(float)
	    4350.,(float)1.3974e-4,(float).088019,(float).056495,(float)
	    -2.2628e-4,(float).057375,(float)-2.2878e-4,(float)4350.,(float)
	    4360.,(float)2.5708e-10,(float).017736,(float).10803,(float)
	    -4.4019e-4,(float).10523,(float)-4.3455e-4,(float)4360.,(float)
	    4370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)4370.,(float)4380.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)4380.,(float)4390.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)4390.,(float)4400.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)4400.,(float)4410.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)4410.,(float)4420.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)4420.,(float)4430.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    4430.,(float)4440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)4440.,(float)4450.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)4450.,(float)4460.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4460.,
	    (float)4470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4470.,(float)4480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4480.,(float)4490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4490.,(
	    float)4500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4500.,(float)4510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4510.,(float)4520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4520.,(
	    float)4530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4530.,(float)4540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4540.,(float)4550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4550.,(
	    float)4560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4560.,(float)4570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4570.,(float)4580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4580.,(
	    float)4590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4590.,(float)4600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4600.,(float)4610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4610.,(
	    float)4620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4620.,(float)4630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4630.,(float)4640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4640.,(
	    float)4650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4650.,(float)4660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4660.,(float)4670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4670.,(
	    float)4680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4680.,(float)4690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4690.,(float)4700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4700.,(
	    float)4710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4710.,(float)4720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4720.,(float)4730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4730.,(
	    float)4740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4740.,(float)4750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4750.,(float)4760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4760.,(
	    float)4770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4770.,(float)4780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4780.,(float)4790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4790.,(
	    float)4800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4800.,(float)4810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4810.,(float)4820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4820.,(
	    float)4830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4830.,(float)4840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4840.,(float)4850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4850.,(
	    float)4860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4860.,(float)4870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4870.,(float)4880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4880.,(
	    float)4890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4890.,(float)4900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4900.,(float)4910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4910.,(
	    float)4920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4920.,(float)4930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4930.,(float)4940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4940.,(
	    float)4950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4950.,(float)4960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4960.,(float)4970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4970.,(
	    float)4980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4980.,(float)4990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4990.,(float)5e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5e3,(
	    float)5010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5010.,(float)5020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5020.,(float)5030.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5030.,(
	    float)5040.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5040.,(float)5050.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5050.,(float)5060. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     carbon monoxide (2500 - 5050 cm-1) */

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
} /* moca1_ */

#ifdef __cplusplus
	}
#endif
