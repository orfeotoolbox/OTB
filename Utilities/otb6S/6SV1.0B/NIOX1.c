/* NIOX1.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    niox1(a,inu) >*/
/* Subroutine */ int niox1_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)2.0198,(float)
	    1.2223,(float).021725,(float)-7.4064e-5,(float).021102,(float)
	    -6.8716e-5,(float)2500.,(float)2510.,(float)5.563,(float).51358,(
	    float).018526,(float)-8.1387e-5,(float).020173,(float)-7.5293e-5,(
	    float)2510.,(float)2520.,(float)30.587,(float).41845,(float)
	    .010994,(float)-5.2858e-5,(float).012658,(float)-4.4443e-5,(float)
	    2520.,(float)2530.,(float)127.44,(float).46867,(float).0044706,(
	    float)-2.7536e-5,(float).0057879,(float)-1.6691e-5,(float)2530.,(
	    float)2540.,(float)288.02,(float).51762,(float)-2.6998e-4,(float)
	    -8.7651e-6,(float).0012264,(float)2.1451e-6,(float)2540.,(float)
	    2550.,(float)353.69,(float).7476,(float)-.0029604,(float)
	    4.5093e-6,(float)-.0013209,(float)1.2765e-5,(float)2550.,(float)
	    2560.,(float)185.24,(float)1.0395,(float)-.0026365,(float)
	    1.2028e-5,(float)6.176e-4,(float)1.0222e-5,(float)2560.,(float)
	    2570.,(float)464.55,(float).85773,(float)-.0024108,(float)
	    -1.72e-6,(float)-7.7777e-4,(float)5.7019e-6,(float)2570.,(float)
	    2580.,(float)293.83,(float)1.0741,(float).0031043,(float)
	    -2.1797e-5,(float).0037703,(float)-2.0707e-5,(float)2580.,(float)
	    2590.,(float)55.672,(float)1.7813,(float).013484,(float)
	    -6.2711e-5,(float).013188,(float)-6.339e-5,(float)2590.,(float)
	    2600.,(float)6.6505,(float)2.3452,(float).023437,(float)
	    -9.9109e-5,(float).023591,(float)-1.0503e-4,(float)2600.,(float)
	    2610.,(float).41623,(float)2.2528,(float).035914,(float)
	    -1.5047e-4,(float).035732,(float)-1.5387e-4,(float)2610.,(float)
	    2620.,(float).01271,(float)1.0271,(float).047982,(float)
	    -1.9811e-4,(float).047474,(float)-2.0344e-4,(float)2620.,(float)
	    2630.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)2630.,(float)2640.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)2640.,(float)2650.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)2650.,(float)2660.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)2660.,(float)2670.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)2670.,(float)2680.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)2680.,(float)2690.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    2690.,(float)2700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)2700.,(float)2710.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)2710.,(float)2720.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2720.,
	    (float)2730.,(float).018198,(float).18592,(float).025778,(float)
	    -1.1646e-4,(float).02282,(float)-1.1152e-4,(float)2730.,(float)
	    2740.,(float).21623,(float).86664,(float).021368,(float)-9.617e-5,
	    (float).018994,(float)-9.4763e-5,(float)2740.,(float)2750.,(float)
	    .9723,(float)1.0815,(float).015101,(float)-7.1229e-5,(float)
	    .012988,(float)-7.0769e-5,(float)2750.,(float)2760.,(float)3.1871,
	    (float)1.6537,(float).0095979,(float)-4.1092e-5,(float).0109,(
	    float)-5.0556e-5,(float)2760.,(float)2770.,(float)9.1207,(float)
	    2.0456,(float).0045146,(float)-1.7223e-5,(float).0068327,(float)
	    -3.3329e-5,(float)2770.,(float)2780.,(float)15.769,(float)2.4147,(
	    float).00151,(float)2.7306e-6,(float).004717,(float)-2.1798e-5,(
	    float)2780.,(float)2790.,(float)63.944,(float)2.2532,(float)
	    -9.2395e-4,(float)-1.4987e-6,(float)-.0017516,(float)-2.476e-6,(
	    float)2790.,(float)2800.,(float)13.465,(float).81561,(float)
	    -.0029579,(float)7.8199e-6,(float)-.0016132,(float)7.282e-6,(
	    float)2800.,(float)2810.,(float)15.397,(float).59138,(float)
	    -4.591e-4,(float)-9.3657e-6,(float)-.0015437,(float)-2.9788e-6,(
	    float)2810.,(float)2820.,(float)3.8011,(float).48019,(float)
	    .0069255,(float)-3.8126e-5,(float).0047344,(float)-3.7097e-5,(
	    float)2820.,(float)2830.,(float).17619,(float).34619,(float)
	    .019824,(float)-8.9371e-5,(float).017957,(float)-8.9565e-5,(float)
	    2830.,(float)2840.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)2840.,(float)2850.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)2850.,(float)2860.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2860.,
	    (float)2870.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2870.,(float)2880.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2880.,(float)2890.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2890.,(
	    float)2900.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2900.,(float)2910.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2910.,(float)2920.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2920.,(
	    float)2930.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2930.,(float)2940.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2940.,(float)2950.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2950.,(
	    float)2960.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2960.,(float)2970.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2970.,(float)2980.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2980.,(
	    float)2990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2990.,(float)3e3,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3e3,(float)3010.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3010.,(
	    float)3020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3020.,(float)3030.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3030.,(float)3040.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3040.,(
	    float)3050.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3050.,(float)3060.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3060.,(float)3070.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3070.,(
	    float)3080.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3080.,(float)3090.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3090.,(float)3100.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3100.,(
	    float)3110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3110.,(float)3120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3120.,(float)3130.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3130.,(
	    float)3140.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3140.,(float)3150.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3150.,(float)3160.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3160.,(
	    float)3170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3170.,(float)3180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3180.,(float)3190.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3190.,(
	    float)3200.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3200.,(float)3210.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3210.,(float)3220.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3220.,(
	    float)3230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3230.,(float)3240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3240.,(float)3250.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3250.,(
	    float)3260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3260.,(float)3270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3270.,(float)3280.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3280.,(
	    float)3290.,(float).031935,(float).33591,(float).031875,(float)
	    -1.4104e-4,(float).028977,(float)-1.3626e-4,(float)3290.,(float)
	    3300.,(float).29281,(float).7467,(float).024844,(float)-1.1101e-4,
	    (float).022436,(float)-1.0879e-4,(float)3300.,(float)3310.,(float)
	    1.3054,(float).88331,(float).018331,(float)-8.5898e-5,(float)
	    .015557,(float)-8.2041e-5,(float)3310.,(float)3320.,(float)4.3559,
	    (float).97784,(float).012392,(float)-6.2543e-5,(float).0096436,(
	    float)-5.8644e-5,(float)3320.,(float)3330.,(float)10.74,(float)
	    .87778,(float).0059037,(float)-3.4711e-5,(float).0041535,(float)
	    -3.5594e-5,(float)3330.,(float)3340.,(float)23.794,(float).67907,(
	    float)5.4208e-5,(float)-1.1592e-5,(float)-8.2054e-4,(float)
	    -1.2774e-5,(float)3340.,(float)3350.,(float)27.447,(float).86607,(
	    float)-.0018645,(float)4.7336e-6,(float)-.001502,(float)
	    -1.8166e-6,(float)3350.,(float)3360.,(float)12.839,(float).92051,(
	    float)-.0018449,(float)1.8928e-5,(float)-3.1436e-5,(float)
	    6.4319e-6,(float)3360.,(float)3370.,(float)33.503,(float).561,(
	    float)-.0027786,(float)-1.9224e-7,(float)-.0032856,(float)
	    1.1355e-5,(float)3370.,(float)3380.,(float)20.6,(float).44389,(
	    float).0016967,(float)-1.9484e-5,(float)-.0011701,(float)
	    -1.5371e-5,(float)3380.,(float)3390.,(float)3.8609,(float).51395,(
	    float).010469,(float)-4.8303e-5,(float).010318,(float)-4.3382e-5,(
	    float)3390.,(float)3400.,(float).29988,(float).99187,(float)
	    .032066,(float)-1.0701e-4,(float).031782,(float)-1.3149e-4,(float)
	    3400.,(float)3410.,(float)1.1046,(float).96114,(float).03056,(
	    float)-1.2026e-4,(float).031203,(float)-1.3653e-4,(float)3410.,(
	    float)3420.,(float)5.3792,(float).96792,(float).023315,(float)
	    -9.51e-5,(float).024421,(float)-1.0864e-4,(float)3420.,(float)
	    3430.,(float)25.745,(float).89943,(float).015618,(float)
	    -6.5963e-5,(float).017309,(float)-7.84e-5,(float)3430.,(float)
	    3440.,(float)90.538,(float).79321,(float).009142,(float)
	    -4.2087e-5,(float).010653,(float)-5.0451e-5,(float)3440.,(float)
	    3450.,(float)302.63,(float).81416,(float).0031369,(float)
	    -1.9699e-5,(float).0046971,(float)-2.3774e-5,(float)3450.,(float)
	    3460.,(float)451.74,(float).71592,(float)-.0010912,(float)
	    -4.691e-6,(float)7.436e-4,(float)-4.31e-6,(float)3460.,(float)
	    3470.,(float)395.99,(float).95008,(float)-.0034795,(float)
	    5.0213e-6,(float)-.0012061,(float)8.3238e-6,(float)3470.,(float)
	    3480.,(float)391.94,(float)1.0413,(float)-.0024518,(float)
	    9.7005e-6,(float)3.1609e-4,(float)6.168e-6,(float)3480.,(float)
	    3490.,(float)753.96,(float).84036,(float)-9.1794e-4,(float)
	    -4.5133e-6,(float)-2.3983e-4,(float)-6.0537e-7,(float)3490.,(
	    float)3500.,(float)216.44,(float).71621,(float).00633,(float)
	    -3.2276e-5,(float).0065565,(float)-3.1312e-5,(float)3500.,(float)
	    3510.,(float)1.8617,(float).41652,(float).026193,(float)
	    -1.1186e-4,(float).025872,(float)-1.1548e-4,(float)3510.,(float)
	    3520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)3520.,(float)3530.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)3530.,(float)3540.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)3540.,(float)3550.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)3550.,(float)3560.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)3560.,(float)3570.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)3570.,(float)3580.,(
	    float).11329,(float).32221,(float).011785,(float)-5.9055e-5,(
	    float).009293,(float)-5.6411e-5,(float)3580.,(float)3590.,(float)
	    .6324,(float).37454,(float).0039372,(float)-2.85e-5,(float)
	    .0010518,(float)-2.435e-5,(float)3590.,(float)3600.,(float)1.3646,
	    (float).37234,(float)-.0012358,(float)-8.9975e-6,(float)-.0042592,
	    (float)-3.4218e-6,(float)3600.,(float)3610.,(float)1.1991,(float)
	    .43663,(float)-.0040429,(float)1.6771e-6,(float)-.007273,(float)
	    8.322e-6,(float)3610.,(float)3620.,(float).84759,(float).39017,(
	    float)-.0045304,(float)3.5248e-6,(float)-.0077172,(float)
	    1.0072e-5,(float)3620.,(float)3630.,(float)1.7247,(float).41455,(
	    float)-.0023639,(float)-4.6172e-6,(float)-.0054164,(float)
	    1.1967e-6,(float)3630.,(float)3640.,(float).9314,(float).40923,(
	    float).002508,(float)-2.2795e-5,(float)-3.8062e-4,(float)
	    -1.8608e-5,(float)3640.,(float)3650.,(float).17321,(float).37301,(
	    float).010558,(float)-5.3495e-5,(float).0082291,(float)-5.1685e-5,
	    (float)3650.,(float)3660.,(float).0028992,(float).031363,(float)
	    .018626,(float)-8.9141e-5,(float).015463,(float)-8.2773e-5,(float)
	    3660.,(float)3670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)3670.,(float)3680.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)3680.,(float)3690.,(float)
	    .052016,(float).2333,(float).023008,(float)-1.0429e-4,(float)
	    .020466,(float)-1.014e-4,(float)3690.,(float)3700.,(float).40077,(
	    float).43751,(float).015625,(float)-7.0109e-5,(float).014974,(
	    float)-7.3666e-5,(float)3700.,(float)3710.,(float)2.1822,(float)
	    .57186,(float).0083501,(float)-3.9798e-5,(float).0081924,(float)
	    -4.424e-5,(float)3710.,(float)3720.,(float)6.6262,(float).62697,(
	    float).0026483,(float)-1.7431e-5,(float).0025461,(float)
	    -2.1437e-5,(float)3720.,(float)3730.,(float)11.503,(float).6936,(
	    float)-.001529,(float)-2.0568e-6,(float)-.0018176,(float)
	    -4.1926e-6,(float)3730.,(float)3740.,(float)5.547,(float).70735,(
	    float)-.0036265,(float)5.5723e-6,(float)-.0039987,(float)
	    4.3869e-6,(float)3740.,(float)3750.,(float)10.823,(float).8515,(
	    float)-.0028437,(float)4.704e-6,(float)-.0029039,(float)1.4795e-6,
	    (float)3750.,(float)3760.,(float)13.145,(float).83989,(float)
	    2.3467e-4,(float)-7.519e-6,(float)3.7066e-4,(float)-1.0406e-5,(
	    float)3760.,(float)3770.,(float)3.9192,(float).87428,(float)
	    .0073134,(float)-3.2311e-5,(float).0084021,(float)-3.9596e-5,(
	    float)3770.,(float)3780.,(float).56238,(float).59784,(float)
	    .018114,(float)-8.3716e-5,(float).016001,(float)-8.226e-5,(float)
	    3780.,(float)3790.,(float)1.486,(float).24352,(float).011201,(
	    float)-5.7941e-5,(float).0083855,(float)-5.3563e-5,(float)3790.,(
	    float)3800.,(float)5.7846,(float).28312,(float).0051402,(float)
	    -3.3968e-5,(float).0020967,(float)-2.8941e-5,(float)3800.,(float)
	    3810.,(float)15.534,(float).38502,(float)2.6866e-4,(float)
	    -1.3504e-5,(float)-.0010262,(float)-5.4149e-6,(float)3810.,(float)
	    3820.,(float)20.107,(float).46654,(float)-.0028471,(float)
	    4.7046e-7,(float)-.0034091,(float)7.0565e-6,(float)3820.,(float)
	    3830.,(float)9.6329,(float).68399,(float)-.0033535,(float)
	    1.0816e-5,(float)-.0031945,(float)9.1488e-6,(float)3830.,(float)
	    3840.,(float)28.782,(float).76658,(float)-.0028915,(float)
	    1.4121e-6,(float)-.0036898,(float)2.7782e-6,(float)3840.,(float)
	    3850.,(float)22.068,(float).72888,(float).0013903,(float)
	    -1.6991e-5,(float)-3.5656e-4,(float)-1.6534e-5,(float)3850.,(
	    float)3860.,(float)4.1416,(float)1.4182,(float).010989,(float)
	    -5.3426e-5,(float).0089392,(float)-5.1526e-5,(float)3860.,(float)
	    3870.,(float)1.386,(float)1.0616,(float).014465,(float)-7.035e-5,(
	    float).011642,(float)-6.6421e-5,(float)3870.,(float)3880.,(float)
	    .21981,(float).827,(float).023073,(float)-1.0318e-4,(float)
	    .020793,(float)-1.0169e-4,(float)3880.,(float)3890.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3890.,(
	    float)3900.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3900.,(float)3910.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3910.,(float)3920.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3920.,(
	    float)3930.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3930.,(float)3940.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3940.,(float)3950.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3950.,(
	    float)3960.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3960.,(float)3970.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3970.,(float)3980.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3980.,(
	    float)3990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3990.,(float)4e3,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4e3,(float)4010.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4010.,(
	    float)4020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4020.,(float)4030.,(float).046381,(float).22687,(
	    float).0024345,(float)-2.3816e-5,(float)-7.0852e-4,(float)
	    -1.7981e-5,(float)4030.,(float)4040.,(float).11207,(float).33988,(
	    float)-.0013925,(float)-8.515e-6,(float)-.0044315,(float)
	    -2.7839e-6,(float)4040.,(float)4050.,(float).26494,(float).78086,(
	    float)7.5352e-4,(float)-1.0882e-5,(float)-.00225,(float)
	    -8.0414e-6,(float)4050.,(float)4060.,(float).52355,(float).95211,(
	    float)-.003229,(float)-8.4991e-7,(float)-.0065316,(float)
	    5.7606e-6,(float)4060.,(float)4070.,(float).20288,(float).51222,(
	    float)-.0019356,(float)-5.8786e-6,(float)-.0049461,(float)
	    -5.0049e-7,(float)4070.,(float)4080.,(float).047528,(float).22508,
	    (float).002968,(float)-2.5911e-5,(float)-1.9533e-4,(float)
	    -2.0081e-5,(float)4080.,(float)4090.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)4090.,(float)4100.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4100.,
	    (float)4110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4110.,(float)4120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4120.,(float)4130.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4130.,(
	    float)4140.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4140.,(float)4150.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4150.,(float)4160.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4160.,(
	    float)4170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4170.,(float)4180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4180.,(float)4190.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4190.,(
	    float)4200.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4200.,(float)4210.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4210.,(float)4220.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4220.,(
	    float)4230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4230.,(float)4240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4240.,(float)4250.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4250.,(
	    float)4260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4260.,(float)4270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4270.,(float)4280.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4280.,(
	    float)4290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4290.,(float)4300.,(float).010261,(float).065002,(
	    float).0034485,(float)-2.8392e-5,(float)1.7191e-4,(float)
	    -2.183e-5,(float)4300.,(float)4310.,(float).084185,(float).33064,(
	    float)1.571e-4,(float)-1.4472e-5,(float)-.0028838,(float)
	    -8.9966e-6,(float)4310.,(float)4320.,(float).11106,(float).39358,(
	    float)-.0032422,(float)-1.3748e-6,(float)-.006363,(float)
	    4.8743e-6,(float)4320.,(float)4330.,(float).58854,(float)1.3088,(
	    float)-9.7669e-4,(float)7.0381e-7,(float)-.0031832,(float)
	    2.6227e-6,(float)4330.,(float)4340.,(float).25725,(float)1.0051,(
	    float).006086,(float)4.6822e-5,(float).0059645,(float)6.3569e-6,(
	    float)4340.,(float)4350.,(float).46043,(float)1.0822,(float)
	    .014451,(float)-3.4551e-5,(float).010783,(float)-4.3114e-5,(float)
	    4350.,(float)4360.,(float)1.0879,(float).79314,(float).015344,(
	    float)-7.4899e-5,(float).012327,(float)-6.9661e-5,(float)4360.,(
	    float)4370.,(float)2.8336,(float).83143,(float).010372,(float)
	    -5.4787e-5,(float).0077026,(float)-5.1069e-5,(float)4370.,(float)
	    4380.,(float)5.6315,(float).55232,(float).0048246,(float)
	    -3.1963e-5,(float).0030343,(float)-3.1434e-5,(float)4380.,(float)
	    4390.,(float)11.627,(float).66242,(float).0012226,(float)
	    -1.4669e-5,(float)9.6618e-4,(float)-1.906e-5,(float)4390.,(float)
	    4400.,(float)18.357,(float).81274,(float)-.0015078,(float)
	    2.8307e-6,(float)3.021e-4,(float)-3.645e-6,(float)4400.,(float)
	    4410.,(float)8.1517,(float).53727,(float)-.0041588,(float)
	    8.2656e-6,(float)-.0039017,(float)1.9754e-5,(float)4410.,(float)
	    4420.,(float)21.947,(float).50379,(float)-.0036693,(float)
	    3.4765e-7,(float)-.0068581,(float)6.819e-6,(float)4420.,(float)
	    4430.,(float)21.321,(float).70812,(float).0014866,(float)
	    -1.5888e-5,(float)-7.1958e-4,(float)-1.5047e-5,(float)4430.,(
	    float)4440.,(float).76039,(float).46213,(float).015813,(float)
	    -7.1293e-5,(float).014807,(float)-7.4172e-5,(float)4440.,(float)
	    4450.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)4450.,(float)4460.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)4460.,(float)4470.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)4470.,(float)4480.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)4480.,(float)4490.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)4490.,(float)4500.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)4500.,(float)4510.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    4510.,(float)4520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)4520.,(float)4530.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)4530.,(float)4540.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4540.,
	    (float)4550.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4550.,(float)4560.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4560.,(float)4570.,(float)
	    .0027208,(float).031363,(float).019608,(float)-9.3071e-5,(float)
	    .016446,(float)-8.6703e-5,(float)4570.,(float)4580.,(float)
	    .075316,(float).2673,(float).013852,(float)-6.7928e-5,(float)
	    .01126,(float)-6.4664e-5,(float)4580.,(float)4590.,(float).26478,(
	    float).24916,(float).0075261,(float)-4.3549e-5,(float).0044923,(
	    float)-3.8501e-5,(float)4590.,(float)4600.,(float).94057,(float)
	    .32245,(float).0019994,(float)-2.1568e-5,(float)-.0010356,(float)
	    -1.639e-5,(float)4600.,(float)4610.,(float)1.6283,(float).34503,(
	    float)-.002067,(float)-5.9246e-6,(float)-.0051076,(float)
	    -7.9763e-8,(float)4610.,(float)4620.,(float)1.1361,(float).39643,(
	    float)-.0042841,(float)2.5773e-6,(float)-.007504,(float)9.2053e-6,
	    (float)4620.,(float)4630.,(float)1.3595,(float).42824,(float)
	    -.0043267,(float)2.7527e-6,(float)-.0075382,(float)9.366e-6,(
	    float)4630.,(float)4640.,(float)2.345,(float).5039,(float)
	    -.0013021,(float)-8.1896e-6,(float)-.0042803,(float)-3.1004e-6,(
	    float)4640.,(float)4650.,(float).73444,(float).57859,(float)
	    .006112,(float)-3.3071e-5,(float).0044533,(float)-3.3529e-5,(
	    float)4650.,(float)4660.,(float).038422,(float).23935,(float)
	    .02365,(float)-1.0589e-4,(float).020573,(float)-1.0116e-4,(float)
	    4660.,(float)4670.,(float).16643,(float).21059,(float).017844,(
	    float)-8.4429e-5,(float).015076,(float)-8.0351e-5,(float)4670.,(
	    float)4680.,(float).66361,(float).33032,(float).012376,(float)
	    -5.9085e-5,(float).01164,(float)-5.974e-5,(float)4680.,(float)
	    4690.,(float)2.4086,(float).4322,(float).0069608,(float)
	    -3.6434e-5,(float).0066271,(float)-3.8146e-5,(float)4690.,(float)
	    4700.,(float)5.7007,(float).46363,(float).0024374,(float)-1.85e-5,
	    (float).0021804,(float)-2.0294e-5,(float)4700.,(float)4710.,(
	    float)11.243,(float).58097,(float)-.0013313,(float)-4.1463e-6,(
	    float)-.001735,(float)-4.8882e-6,(float)4710.,(float)4720.,(float)
	    7.8568,(float).66135,(float)-.0035465,(float)4.3994e-6,(float)
	    -.0041602,(float)4.5248e-6,(float)4720.,(float)4730.,(float)
	    9.4234,(float).73404,(float)-.0035684,(float)4.612e-6,(float)
	    -.0040476,(float)4.3652e-6,(float)4730.,(float)4740.,(float)
	    18.463,(float)1.0659,(float)1.1174e-6,(float)-7.5689e-6,(float)
	    -1.4938e-4,(float)-1.035e-5,(float)4740.,(float)4750.,(float)
	    2.9519,(float).92577,(float).009335,(float)-4.0974e-5,(float)
	    .010093,(float)-5.0109e-5,(float)4750.,(float)4760.,(float)0.,(
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
	    float)4950.,(float).057081,(float).26504,(float)2.1749e-4,(float)
	    -1.4991e-5,(float)-.0028657,(float)-9.2093e-6,(float)4950.,(float)
	    4960.,(float).089281,(float).35365,(float)-.0029091,(float)
	    -2.6962e-6,(float)-.0059925,(float)3.4121e-6,(float)4960.,(float)
	    4970.,(float).46911,(float)1.1088,(float)-.0015343,(float)
	    -4.5097e-6,(float)-.0043859,(float)-9.5143e-7,(float)4970.,(float)
	    4980.,(float).1858,(float).71004,(float)2.2264e-4,(float)
	    5.4082e-6,(float)-.0022981,(float)3.4446e-7,(float)4980.,(float)
	    4990.,(float).30113,(float).63162,(float).0034248,(float)
	    -2.4398e-5,(float)-2.019e-4,(float)-1.8173e-5,(float)4990.,(float)
	    5e3,(float).54259,(float).33048,(float)1.4963e-4,(float)
	    -1.4443e-5,(float)-.0028875,(float)-8.9818e-6,(float)5e3,(float)
	    5010.,(float).69468,(float).35696,(float)-.0031719,(float)
	    -1.6864e-6,(float)-.0062635,(float)4.4718e-6,(float)5010.,(float)
	    5020.,(float).31258,(float).41285,(float)-.0047292,(float)
	    4.2957e-6,(float)-.0079123,(float)1.0784e-5,(float)5020.,(float)
	    5030.,(float).9629,(float).50181,(float)-.0034905,(float)
	    -3.0499e-7,(float)-.0066561,(float)6.056e-6,(float)5030.,(float)
	    5040.,(float).75559,(float).535,(float).0012181,(float)-1.6925e-5,
	    (float)-.0015139,(float)-1.3577e-5,(float)5040.,(float)5050.,(
	    float).089652,(float).35481,(float).010413,(float)-5.3691e-5,(
	    float).0078717,(float)-5.0854e-5,(float)5050.,(float)5060. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     nitrous oxide (2500 - 5050 cm-1) */

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
} /* niox1_ */

#ifdef __cplusplus
	}
#endif
