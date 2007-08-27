/* OZON1.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    ozon1(a,inu) >*/
/* Subroutine */ int ozon1_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float).062007,(float)
	    2.4365,(float)-5.9503e-4,(float)-8.1198e-6,(float)-.0039418,(
	    float)-2.4624e-6,(float)2500.,(float)2510.,(float).023839,(float)
	    2.3534,(float).0037377,(float)-6.15e-6,(float).0015592,(float)
	    -1.2727e-5,(float)2510.,(float)2520.,(float).0090127,(float)
	    1.2172,(float)-.0014733,(float)-4.7053e-6,(float)-.0042092,(float)
	    -1.5909e-6,(float)2520.,(float)2530.,(float).0058863,(float)1.1,(
	    float)3.3397e-4,(float)-1.3388e-5,(float)-.0024956,(float)
	    -9.7955e-6,(float)2530.,(float)2540.,(float).0021326,(float)
	    .57054,(float).0025704,(float)-2.3597e-5,(float)-2.6961e-4,(float)
	    -1.9566e-5,(float)2540.,(float)2550.,(float)2.6378e-4,(float)
	    .095762,(float).0021816,(float)-2.4506e-5,(float)-8.9441e-4,(
	    float)-1.8362e-5,(float)2550.,(float)2560.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)2560.,(float)2570.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    2570.,(float)2580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)2580.,(float)2590.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)2590.,(float)2600.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2600.,
	    (float)2610.,(float)1.2538e-4,(float).06037,(float).017057,(float)
	    -8.3526e-5,(float).013989,(float)-7.7656e-5,(float)2610.,(float)
	    2620.,(float).0033172,(float)1.2606,(float).015787,(float)
	    -6.9353e-5,(float).013123,(float)-6.8937e-5,(float)2620.,(float)
	    2630.,(float).008713,(float)2.2667,(float).014103,(float)
	    -5.7967e-5,(float).011482,(float)-5.9919e-5,(float)2630.,(float)
	    2640.,(float).0172,(float)3.144,(float).011768,(float)-4.8179e-5,(
	    float).0088246,(float)-4.9702e-5,(float)2640.,(float)2650.,(float)
	    .034158,(float)3.5816,(float).0099884,(float)-4.184e-5,(float)
	    .0074214,(float)-4.4219e-5,(float)2650.,(float)2660.,(float)
	    .058424,(float)3.8732,(float).0072077,(float)-3.3325e-5,(float)
	    .004529,(float)-3.3709e-5,(float)2660.,(float)2670.,(float)
	    .091154,(float)4.3262,(float).0058558,(float)-2.8338e-5,(float)
	    .0036931,(float)-2.8993e-5,(float)2670.,(float)2680.,(float)
	    .14108,(float)4.4142,(float).0030363,(float)-1.8897e-5,(float)
	    8.3735e-4,(float)-1.8016e-5,(float)2680.,(float)2690.,(float)
	    .19337,(float)5.4007,(float).0022914,(float)-1.3229e-5,(float)
	    5.9045e-4,(float)-1.4091e-5,(float)2690.,(float)2700.,(float)
	    .22397,(float)5.654,(float)4.0913e-4,(float)-5.5214e-8,(float)
	    -4.337e-4,(float)-1.0921e-6,(float)2700.,(float)2710.,(float)
	    .25652,(float)5.9001,(float).0016172,(float)5.8755e-6,(float)
	    .0013629,(float)-1.6939e-6,(float)2710.,(float)2720.,(float)
	    .26204,(float)6.3198,(float).0046886,(float)1.25e-5,(float)
	    .0044002,(float)-9.3401e-6,(float)2720.,(float)2730.,(float)
	    1.1785,(float)7.1393,(float).0037596,(float)-7.5943e-6,(float)
	    .0028497,(float)-1.6442e-5,(float)2730.,(float)2740.,(float)
	    1.2094,(float)5.2539,(float).0056671,(float)-2.7298e-5,(float)
	    .0028397,(float)-2.2464e-5,(float)2740.,(float)2750.,(float)
	    2.6976,(float)4.6649,(float).0025634,(float)-2.0935e-5,(float)
	    3.1356e-4,(float)-1.7037e-5,(float)2750.,(float)2760.,(float)
	    5.2789,(float)4.5914,(float)-7.3997e-4,(float)-9.2529e-6,(float)
	    -.0021265,(float)-8.3858e-6,(float)2760.,(float)2770.,(float)
	    5.611,(float)6.1724,(float)-.0032253,(float)2.5797e-6,(float)
	    -.0028278,(float)8.7898e-8,(float)2770.,(float)2780.,(float)4.928,
	    (float)9.4156,(float)-.0020334,(float)1.9294e-6,(float)-.0030249,(
	    float)-1.069e-6,(float)2780.,(float)2790.,(float)9.8978,(float)
	    7.256,(float)-.0024986,(float)-1.5096e-6,(float)-.0036608,(float)
	    -5.8064e-7,(float)2790.,(float)2800.,(float)3.3206,(float)9.3181,(
	    float).0047633,(float)-2.5074e-5,(float).0053638,(float)
	    -3.2113e-5,(float)2800.,(float)2810.,(float).02474,(float)2.6388,(
	    float).0070889,(float)-3.2967e-5,(float).0046677,(float)
	    -3.4491e-5,(float)2810.,(float)2820.,(float).034545,(float)2.9892,
	    (float).0049198,(float)-2.638e-5,(float).0026057,(float)
	    -2.5038e-5,(float)2820.,(float)2830.,(float).04698,(float)4.234,(
	    float).0050308,(float)-1.5985e-5,(float).0030829,(float)
	    -2.1189e-5,(float)2830.,(float)2840.,(float).07243,(float)5.0065,(
	    float).0059166,(float)-2.0703e-5,(float).0028776,(float)
	    -2.3414e-5,(float)2840.,(float)2850.,(float).11034,(float)4.0562,(
	    float).0040989,(float)-2.3368e-5,(float)8.42e-4,(float)-1.8356e-5,
	    (float)2850.,(float)2860.,(float).12579,(float)3.6106,(float)
	    .0011211,(float)-1.2332e-5,(float)-8.604e-4,(float)-1.0593e-5,(
	    float)2860.,(float)2870.,(float).095405,(float)4.2611,(float)
	    .0010479,(float)-5.3844e-6,(float)-6.5774e-4,(float)-8.3477e-6,(
	    float)2870.,(float)2880.,(float).092029,(float)4.3877,(float)
	    .0013349,(float)-8.8914e-6,(float)-8.978e-4,(float)-9.601e-6,(
	    float)2880.,(float)2890.,(float).085314,(float)2.2957,(float)
	    -.0010376,(float)-3.9555e-7,(float)-.0023459,(float)-3.1759e-6,(
	    float)2890.,(float)2900.,(float).1913,(float)2.1329,(float)
	    9.3145e-5,(float)-1.0935e-5,(float)-.0023306,(float)-7.0411e-6,(
	    float)2900.,(float)2910.,(float).14327,(float)3.0363,(float)
	    .0049245,(float)-1.9969e-5,(float).0019913,(float)-1.9177e-5,(
	    float)2910.,(float)2920.,(float).062081,(float)2.9232,(float)
	    .0028349,(float)4.5083e-6,(float).0026887,(float)-6.9673e-6,(
	    float)2920.,(float)2930.,(float).054536,(float)2.5833,(float)
	    .003389,(float)5.9711e-6,(float).0039633,(float)1.706e-6,(float)
	    2930.,(float)2940.,(float).067558,(float)4.1622,(float).012948,(
	    float)1.8636e-5,(float).013881,(float)-2.7246e-5,(float)2940.,(
	    float)2950.,(float).10694,(float)4.2481,(float).019447,(float)
	    -4.2948e-5,(float).016908,(float)-6.0648e-5,(float)2950.,(float)
	    2960.,(float).23821,(float)4.1323,(float).020561,(float)
	    -8.5072e-5,(float).016888,(float)-7.7856e-5,(float)2960.,(float)
	    2970.,(float).64222,(float)3.762,(float).01732,(float)-8.1718e-5,(
	    float).014625,(float)-7.652e-5,(float)2970.,(float)2980.,(float)
	    1.63,(float)3.7899,(float).013216,(float)-6.5623e-5,(float)
	    .011621,(float)-6.3803e-5,(float)2980.,(float)2990.,(float)3.8611,
	    (float)4.0234,(float).0091789,(float)-4.9247e-5,(float).0082148,(
	    float)-4.8334e-5,(float)2990.,(float)3e3,(float)8.4985,(float)
	    4.366,(float).0053518,(float)-3.3739e-5,(float).0046322,(float)
	    -3.3282e-5,(float)3e3,(float)3010.,(float)16.437,(float)4.6428,(
	    float).0018372,(float)-1.9254e-5,(float).0014965,(float)
	    -1.9554e-5,(float)3010.,(float)3020.,(float)26.227,(float)5.2685,(
	    float)-.0010368,(float)-6.92e-6,(float)-.0010546,(float)
	    -7.8474e-6,(float)3020.,(float)3030.,(float)29.674,(float)6.9683,(
	    float)-.0027894,(float)1.5655e-6,(float)-.0027673,(float)
	    3.4286e-7,(float)3030.,(float)3040.,(float)24.306,(float)9.5097,(
	    float)-.002074,(float)6.2838e-6,(float)-.0020776,(float)2.179e-6,(
	    float)3040.,(float)3050.,(float)63.521,(float)9.8031,(float)
	    -.0012075,(float)-2.994e-6,(float)-.0022793,(float)-5.3779e-6,(
	    float)3050.,(float)3060.,(float)1.1941,(float)5.4609,(float)
	    .0015524,(float)-9.5161e-6,(float)-9.6618e-4,(float)-7.6711e-6,(
	    float)3060.,(float)3070.,(float).92867,(float)5.355,(float)
	    -6.2368e-4,(float)-3.643e-6,(float)-.0029524,(float)-1.6001e-6,(
	    float)3070.,(float)3080.,(float).67841,(float)5.2135,(float)
	    5.235e-4,(float)6.0726e-6,(float)-3.7339e-4,(float)-2.3662e-6,(
	    float)3080.,(float)3090.,(float)3.1429,(float)5.2297,(float)
	    .0017001,(float)-8.0369e-6,(float)-2.527e-4,(float)-9.2399e-6,(
	    float)3090.,(float)3100.,(float).55537,(float)4.0641,(float)
	    -8.4176e-4,(float)3.74e-7,(float)-.0020788,(float)-8.3774e-7,(
	    float)3100.,(float)3110.,(float).62683,(float)4.1264,(float)
	    -6.4674e-4,(float)-8.4882e-7,(float)-5.9275e-4,(float)-1.4998e-6,(
	    float)3110.,(float)3120.,(float).75072,(float)5.6505,(float)
	    .0024151,(float)-8.8833e-6,(float).0024005,(float)-1.4915e-5,(
	    float)3120.,(float)3130.,(float).63692,(float)5.2929,(float)
	    .0052702,(float)-2.0599e-5,(float).0049282,(float)-2.9353e-5,(
	    float)3130.,(float)3140.,(float).6931,(float)5.7878,(float)
	    .0069797,(float)-3.6776e-5,(float).0052917,(float)-3.67e-5,(float)
	    3140.,(float)3150.,(float)1.0593,(float)5.0675,(float).0040985,(
	    float)-2.5996e-5,(float).0032401,(float)-2.8071e-5,(float)3150.,(
	    float)3160.,(float)1.7493,(float)4.5943,(float)-1.8037e-4,(float)
	    -8.6286e-6,(float)2.5807e-4,(float)-1.0009e-5,(float)3160.,(float)
	    3170.,(float)2.0855,(float)5.9045,(float)-.0022784,(float)
	    1.4682e-6,(float)-.0024776,(float)-2.9964e-6,(float)3170.,(float)
	    3180.,(float)1.3759,(float)5.4986,(float)-.0034094,(float)
	    2.6712e-6,(float)-.0053592,(float)5.6885e-6,(float)3180.,(float)
	    3190.,(float)3.923,(float)8.3024,(float)-.0018506,(float)
	    -3.5742e-7,(float)-.0024786,(float)-1.1732e-6,(float)3190.,(float)
	    3200.,(float)1.1841,(float)5.1604,(float).0039237,(float)
	    -2.2108e-5,(float).0038274,(float)-2.7628e-5,(float)3200.,(float)
	    3210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)3210.,(float)3220.,(float)5.5594e-4,(float).13449,(float)
	    1.458e-4,(float)-1.5828e-5,(float)-.0029023,(float)-1.0057e-5,(
	    float)3220.,(float)3230.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)3230.,(float)3240.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)3240.,(float)3250.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    3250.,(float)3260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)3260.,(float)3270.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)3270.,(float)3280.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3280.,
	    (float)3290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3290.,(float)3300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3300.,(float)3310.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3310.,(
	    float)3320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3320.,(float)3330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3330.,(float)3340.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3340.,(
	    float)3350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3350.,(float)3360.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3360.,(float)3370.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3370.,(
	    float)3380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3380.,(float)3390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3390.,(float)3400.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3400.,(
	    float)3410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3410.,(float)3420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3420.,(float)3430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3430.,(
	    float)3440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3440.,(float)3450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3450.,(float)3460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3460.,(
	    float)3470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3470.,(float)3480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3480.,(float)3490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3490.,(
	    float)3500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3500.,(float)3510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3510.,(float)3520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3520.,(
	    float)3530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3530.,(float)3540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3540.,(float)3550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3550.,(
	    float)3560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3560.,(float)3570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3570.,(float)3580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3580.,(
	    float)3590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3590.,(float)3600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3600.,(float)3610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3610.,(
	    float)3620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3620.,(float)3630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3630.,(float)3640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3640.,(
	    float)3650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3650.,(float)3660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3660.,(float)3670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3670.,(
	    float)3680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3680.,(float)3690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3690.,(float)3700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3700.,(
	    float)3710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3710.,(float)3720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3720.,(float)3730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3730.,(
	    float)3740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3740.,(float)3750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3750.,(float)3760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3760.,(
	    float)3770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3770.,(float)3780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3780.,(float)3790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3790.,(
	    float)3800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3800.,(float)3810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3810.,(float)3820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3820.,(
	    float)3830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3830.,(float)3840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3840.,(float)3850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3850.,(
	    float)3860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3860.,(float)3870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3870.,(float)3880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3880.,(
	    float)3890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3890.,(float)3900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3900.,(float)3910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3910.,(
	    float)3920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3920.,(float)3930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3930.,(float)3940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3940.,(
	    float)3950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3950.,(float)3960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3960.,(float)3970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3970.,(
	    float)3980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3980.,(float)3990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3990.,(float)4e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4e3,(
	    float)4010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4010.,(float)4020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4020.,(float)4030.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4030.,(
	    float)4040.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4040.,(float)4050.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4050.,(float)4060.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4060.,(
	    float)4070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4070.,(float)4080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4080.,(float)4090.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4090.,(
	    float)4100.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4100.,(float)4110.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4110.,(float)4120.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4120.,(
	    float)4130.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4130.,(float)4140.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4140.,(float)4150.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4150.,(
	    float)4160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4160.,(float)4170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4170.,(float)4180.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4180.,(
	    float)4190.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4190.,(float)4200.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4200.,(float)4210.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4210.,(
	    float)4220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4220.,(float)4230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4230.,(float)4240.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4240.,(
	    float)4250.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4250.,(float)4260.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4260.,(float)4270.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4270.,(
	    float)4280.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4280.,(float)4290.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4290.,(float)4300.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4300.,(
	    float)4310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4310.,(float)4320.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4320.,(float)4330.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4330.,(
	    float)4340.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4340.,(float)4350.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4350.,(float)4360.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4360.,(
	    float)4370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4370.,(float)4380.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4380.,(float)4390.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4390.,(
	    float)4400.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4400.,(float)4410.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4410.,(float)4420.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4420.,(
	    float)4430.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4430.,(float)4440.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4440.,(float)4450.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4450.,(
	    float)4460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4460.,(float)4470.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4470.,(float)4480.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4480.,(
	    float)4490.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4490.,(float)4500.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4500.,(float)4510.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4510.,(
	    float)4520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4520.,(float)4530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4530.,(float)4540.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4540.,(
	    float)4550.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4550.,(float)4560.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4560.,(float)4570.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4570.,(
	    float)4580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4580.,(float)4590.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4590.,(float)4600.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4600.,(
	    float)4610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4610.,(float)4620.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4620.,(float)4630.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4630.,(
	    float)4640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4640.,(float)4650.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4650.,(float)4660.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4660.,(
	    float)4670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4670.,(float)4680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4680.,(float)4690.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4690.,(
	    float)4700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4700.,(float)4710.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4710.,(float)4720.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4720.,(
	    float)4730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4730.,(float)4740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4740.,(float)4750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4750.,(
	    float)4760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4760.,(float)4770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4770.,(float)4780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4780.,(
	    float)4790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4790.,(float)4800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4800.,(float)4810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4810.,(
	    float)4820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4820.,(float)4830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4830.,(float)4840.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4840.,(
	    float)4850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4850.,(float)4860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4860.,(float)4870.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4870.,(
	    float)4880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4880.,(float)4890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4890.,(float)4900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4900.,(
	    float)4910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4910.,(float)4920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4920.,(float)4930.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4930.,(
	    float)4940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4940.,(float)4950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4950.,(float)4960.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4960.,(
	    float)4970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4970.,(float)4980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4980.,(float)4990.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4990.,(
	    float)5e3,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5e3,(float)5010.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5010.,(float)5020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5020.,(
	    float)5030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5030.,(float)5040.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5040.,(float)5050.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5050.,(
	    float)5060. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     ozone (2500 - 5040 cm-1) */

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
} /* ozon1_ */

#ifdef __cplusplus
	}
#endif
