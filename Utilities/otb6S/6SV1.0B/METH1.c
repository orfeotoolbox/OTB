/* METH1.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    meth1(a,inu) >*/
/* Subroutine */ int meth1_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)1.4454,(float)
	    .47807,(float).0052823,(float)-3.0056e-5,(float).002903,(float)
	    -2.686e-5,(float)2500.,(float)2510.,(float)8.7736,(float).49348,(
	    float)3.8511e-4,(float)-6.0533e-6,(float)1.0891e-4,(float)
	    -9.3895e-6,(float)2510.,(float)2520.,(float)5.7188,(float).51082,(
	    float)3.239e-4,(float)-7.2399e-6,(float)1.6424e-4,(float)
	    -1.0608e-5,(float)2520.,(float)2530.,(float)4.5159,(float).73333,(
	    float)5.5152e-4,(float)-2.5526e-6,(float)2.6319e-4,(float)
	    -1.1149e-5,(float)2530.,(float)2540.,(float)6.2089,(float).57459,(
	    float)-5.1671e-5,(float)-3.2924e-6,(float)-.0012056,(float)
	    -7.7167e-6,(float)2540.,(float)2550.,(float)9.8841,(float).5111,(
	    float)-.0015575,(float)1.7182e-6,(float)-.001203,(float)
	    -3.8007e-6,(float)2550.,(float)2560.,(float)7.6169,(float).80719,(
	    float)1.2985e-4,(float)-5.7517e-6,(float)-.0011789,(float)
	    -6.7688e-6,(float)2560.,(float)2570.,(float)7.3766,(float).91538,(
	    float)-6.9626e-4,(float)-9.8403e-8,(float)-.0010951,(float)
	    -5.6422e-6,(float)2570.,(float)2580.,(float)7.9669,(float)1.0249,(
	    float)-.0020553,(float)6.0631e-6,(float)-.0025043,(float)
	    1.2199e-6,(float)2580.,(float)2590.,(float)12.942,(float)1.8227,(
	    float).0016813,(float)-1.019e-5,(float)8.4177e-4,(float)
	    -1.3848e-5,(float)2590.,(float)2600.,(float)11.671,(float).93909,(
	    float)-.0023388,(float)7.1737e-6,(float)-.0030848,(float)
	    1.0079e-5,(float)2600.,(float)2610.,(float)7.682,(float).22382,(
	    float)-.0036797,(float)1.7419e-5,(float)-.002062,(float)2.0955e-5,
	    (float)2610.,(float)2620.,(float)5.628,(float).56037,(float)
	    -.0012398,(float)1.026e-5,(float)-.001454,(float)8.1367e-6,(float)
	    2620.,(float)2630.,(float)14.127,(float)1.2843,(float)7.6229e-5,(
	    float)5.7331e-6,(float)-6.8637e-4,(float)-4.6206e-6,(float)2630.,(
	    float)2640.,(float)9.0425,(float)1.3886,(float).0042412,(float)
	    2.3021e-6,(float).0040176,(float)-2.0017e-5,(float)2640.,(float)
	    2650.,(float)10.155,(float).90252,(float)9.0385e-6,(float)
	    1.3215e-5,(float)-4.9853e-4,(float)-1.2678e-6,(float)2650.,(float)
	    2660.,(float)10.154,(float).79137,(float).0031844,(float)
	    -3.2869e-6,(float).0011834,(float)-1.1313e-5,(float)2660.,(float)
	    2670.,(float)11.725,(float).70879,(float)-.0014287,(float)
	    1.8146e-6,(float)-.0025033,(float)-1.0406e-7,(float)2670.,(float)
	    2680.,(float)14.948,(float).92692,(float).0062395,(float)
	    -3.3167e-5,(float).0031727,(float)-2.6064e-5,(float)2680.,(float)
	    2690.,(float)26.279,(float).87829,(float).0028361,(float)
	    -1.4791e-5,(float).0014018,(float)-1.4682e-5,(float)2690.,(float)
	    2700.,(float)7.1745,(float).92595,(float).0030055,(float)
	    -2.7951e-6,(float).0045235,(float)-2.2596e-5,(float)2700.,(float)
	    2710.,(float)27.757,(float).81447,(float).0030565,(float)
	    -1.9569e-5,(float).0025866,(float)-2.0025e-5,(float)2710.,(float)
	    2720.,(float)36.518,(float).82512,(float).0018262,(float)
	    -1.5755e-5,(float).0012027,(float)-1.7464e-5,(float)2720.,(float)
	    2730.,(float)7.1564,(float).77634,(float).0051047,(float)
	    -2.7255e-5,(float).003528,(float)-2.6912e-5,(float)2730.,(float)
	    2740.,(float)53.441,(float).64417,(float)-7.6181e-5,(float)
	    -1.012e-5,(float)-8.2299e-4,(float)-9.7201e-6,(float)2740.,(float)
	    2750.,(float)38.926,(float).7434,(float)-6.9195e-4,(float)
	    -4.3135e-6,(float)2.1091e-4,(float)-6.9939e-6,(float)2750.,(float)
	    2760.,(float)15.709,(float)1.3287,(float).0030102,(float)
	    -4.8796e-6,(float).0052525,(float)-2.2331e-5,(float)2760.,(float)
	    2770.,(float)54.733,(float)1.246,(float)-.0010987,(float)
	    5.2563e-6,(float).002214,(float)-1.2844e-5,(float)2770.,(float)
	    2780.,(float)47.55,(float)1.5791,(float)8.4509e-4,(float)
	    -2.0192e-6,(float).0011472,(float)-1.6086e-5,(float)2780.,(float)
	    2790.,(float)66.52,(float)1.2817,(float)-2.9084e-4,(float)
	    -6.0279e-6,(float)-.0013817,(float)-7.9406e-6,(float)2790.,(float)
	    2800.,(float)54.178,(float).80208,(float)-.0020375,(float)
	    1.3886e-6,(float)-.002404,(float)9.4209e-6,(float)2800.,(float)
	    2810.,(float)53.2,(float).65974,(float)-7.1708e-4,(float)
	    8.8569e-6,(float)-2.5373e-4,(float)1.13e-5,(float)2810.,(float)
	    2820.,(float)148.4,(float).84587,(float)-.0016143,(float)
	    2.0628e-6,(float)-9.7561e-4,(float)9.6287e-6,(float)2820.,(float)
	    2830.,(float)45.29,(float)1.3591,(float)-2.3074e-4,(float)
	    1.8732e-5,(float).0020323,(float)6.678e-6,(float)2830.,(float)
	    2840.,(float)32.141,(float)1.1794,(float)7.5732e-4,(float)
	    5.0043e-5,(float).005454,(float)1.3952e-6,(float)2840.,(float)
	    2850.,(float)52.919,(float).85346,(float).0010685,(float)
	    4.9698e-5,(float).0029632,(float)1.729e-5,(float)2850.,(float)
	    2860.,(float)92.855,(float)1.2724,(float).0034021,(float)
	    3.1892e-5,(float).0020937,(float)7.5591e-6,(float)2860.,(float)
	    2870.,(float)115.15,(float)1.6724,(float).0068873,(float)
	    1.0637e-5,(float).0045477,(float)-1.5654e-5,(float)2870.,(float)
	    2880.,(float)149.81,(float)1.2641,(float).0081325,(float)
	    -1.1177e-6,(float).0053865,(float)-1.8572e-5,(float)2880.,(float)
	    2890.,(float)225.46,(float).83931,(float).011197,(float)
	    -4.4342e-5,(float).0080886,(float)-3.4016e-5,(float)2890.,(float)
	    2900.,(float)337.19,(float).68846,(float).0074267,(float)
	    -2.7002e-5,(float).0051288,(float)-1.7032e-5,(float)2900.,(float)
	    2910.,(float)593.38,(float).59397,(float).0062712,(float)
	    -3.0983e-5,(float).0039217,(float)-1.5025e-5,(float)2910.,(float)
	    2920.,(float)814.37,(float).44567,(float).0050972,(float)
	    -3.2548e-5,(float).0037255,(float)-1.8409e-5,(float)2920.,(float)
	    2930.,(float)1008.,(float).42373,(float).002722,(float)-2.2438e-5,
	    (float).001776,(float)-8.5011e-6,(float)2930.,(float)2940.,(float)
	    1273.8,(float).39048,(float)9.1833e-4,(float)-1.5902e-5,(float)
	    3.8752e-4,(float)-6.11e-6,(float)2940.,(float)2950.,(float)1822.6,
	    (float).32354,(float)-8.3613e-4,(float)-9.1286e-6,(float)
	    -.0012748,(float)6.4576e-7,(float)2950.,(float)2960.,(float)
	    1144.9,(float).28404,(float)-.0022237,(float)-3.1413e-6,(float)
	    -.0015533,(float)4e-6,(float)2960.,(float)2970.,(float)1422.2,(
	    float).24694,(float)-.0034946,(float)2.0028e-6,(float)-.0022609,(
	    float)9.6681e-6,(float)2970.,(float)2980.,(float)1129.9,(float)
	    .25225,(float)-.0043236,(float)6.0179e-6,(float)-.0023072,(float)
	    8.9383e-6,(float)2980.,(float)2990.,(float)382.43,(float).28365,(
	    float)-.0047369,(float)1.1124e-5,(float).0018248,(float)1.9257e-5,
	    (float)2990.,(float)3e3,(float)231.12,(float)2.8616,(float)
	    .0085402,(float)4.3691e-5,(float).016112,(float)-4.2673e-5,(float)
	    3e3,(float)3010.,(float)12795.,(float)1.9671,(float)-2.1206e-4,(
	    float)-1.4984e-6,(float)-3.9307e-4,(float)-5.8544e-6,(float)3010.,
	    (float)3020.,(float)578.69,(float).79629,(float)-.0043645,(float)
	    1.3898e-5,(float)7.3408e-4,(float)-4.5626e-6,(float)3020.,(float)
	    3030.,(float)434.57,(float).096576,(float)-.0055811,(float)
	    1.294e-5,(float).0011172,(float)2.1736e-5,(float)3030.,(float)
	    3040.,(float)899.49,(float).11536,(float)-.0052201,(float)
	    1.0059e-5,(float)-.0015874,(float)2.2277e-5,(float)3040.,(float)
	    3050.,(float)2131.4,(float).12202,(float)-.0045842,(float)
	    6.3207e-6,(float)-.003267,(float)2.194e-5,(float)3050.,(float)
	    3060.,(float)2451.4,(float).19437,(float)-.0035989,(float)
	    2.0635e-6,(float)-.0037611,(float)1.5519e-5,(float)3060.,(float)
	    3070.,(float)1860.9,(float).31324,(float)-.0022827,(float)
	    -2.6816e-6,(float)-.0014644,(float)8.4665e-6,(float)3070.,(float)
	    3080.,(float)2857.1,(float).3194,(float)-8.3645e-4,(float)
	    -9.0192e-6,(float)-7.6302e-4,(float)2.417e-6,(float)3080.,(float)
	    3090.,(float)1935.6,(float).28235,(float)9.2008e-4,(float)
	    -1.5863e-5,(float).0010109,(float)-1.3517e-6,(float)3090.,(float)
	    3100.,(float)1498.4,(float).32994,(float).0028983,(float)
	    -2.3966e-5,(float).0020895,(float)-9.8936e-6,(float)3100.,(float)
	    3110.,(float)1250.8,(float).36564,(float).005137,(float)
	    -3.2908e-5,(float).0037408,(float)-1.8191e-5,(float)3110.,(float)
	    3120.,(float)818.19,(float).45356,(float).0075913,(float)
	    -4.2727e-5,(float).0055871,(float)-2.645e-5,(float)3120.,(float)
	    3130.,(float)483.02,(float).62289,(float).010573,(float)
	    -5.4032e-5,(float).0075195,(float)-3.888e-5,(float)3130.,(float)
	    3140.,(float)405.16,(float)1.0372,(float).01439,(float)-6.8744e-5,
	    (float).011121,(float)-5.8648e-5,(float)3140.,(float)3150.,(float)
	    80.532,(float).8961,(float).01798,(float)-6.9816e-5,(float)
	    .012298,(float)-5.1675e-5,(float)3150.,(float)3160.,(float)41.171,
	    (float).85486,(float).021624,(float)-8.2706e-5,(float).015717,(
	    float)-6.4082e-5,(float)3160.,(float)3170.,(float)18.087,(float)
	    .82972,(float).024369,(float)-8.2555e-5,(float).018128,(float)
	    -6.8246e-5,(float)3170.,(float)3180.,(float)7.8303,(float).7783,(
	    float).024083,(float)-4.5258e-5,(float).018125,(float)-5.3074e-5,(
	    float)3180.,(float)3190.,(float)5.5218,(float)1.2503,(float)
	    .023406,(float)-1.2671e-6,(float).019648,(float)-4.075e-5,(float)
	    3190.,(float)3200.,(float).69216,(float).51644,(float).01424,(
	    float)-8.731e-6,(float).01086,(float)-3.2732e-5,(float)3200.,(
	    float)3210.,(float).30939,(float).2177,(float).0082095,(float)
	    -4.4565e-5,(float).0058697,(float)-4.1343e-5,(float)3210.,(float)
	    3220.,(float).22568,(float).37985,(float).012091,(float)
	    -6.0525e-5,(float).0090838,(float)-5.5067e-5,(float)3220.,(float)
	    3230.,(float).18766,(float).2799,(float).01283,(float)-6.1005e-5,(
	    float).010043,(float)-5.7718e-5,(float)3230.,(float)3240.,(float)
	    .063205,(float).18476,(float).014107,(float)-6.7485e-5,(float)
	    .011333,(float)-6.3191e-5,(float)3240.,(float)3250.,(float)
	    .028821,(float).059172,(float).014986,(float)-7.1903e-5,(float)
	    .012123,(float)-6.7129e-5,(float)3250.,(float)3260.,(float)0.,(
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
	    float)0.,(float)3690.,(float)3700.,(float).014928,(float).049339,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3700.,(float)3710.,(float).086139,(float).13757,
	    (float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3710.,(float)3720.,(float).067083,(float).12495,
	    (float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3720.,(float)3730.,(float).020862,(float)
	    .049275,(float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3730.,(float)3740.,(float).067784,(float).11899,
	    (float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3740.,(float)3750.,(float).15027,(float).18565,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3750.,(float)3760.,(float).31581,(float).26221,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3760.,(float)3770.,(float).48086,(float).2992,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3770.,(float)3780.,(float).59376,(float).29431,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3780.,(float)3790.,(float)1.8375,(float).36994,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3790.,(float)3800.,(float)2.5355,(float).40322,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3800.,(float)3810.,(float)2.7002,(float).43597,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3810.,(float)3820.,(float)6.1651,(float).55369,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3820.,(float)3830.,(float)4.0461,(float).57732,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3830.,(float)3840.,(float)6.3365,(float).67727,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3840.,(float)3850.,(float)9.3866,(float).95498,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3850.,(float)3860.,(float)20.329,(float)1.0079,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3860.,(float)3870.,(float)3.8488,(float).60704,(
	    float)-.0030055,(float)4.6922e-6,(float)-.0036436,(float)
	    -2.5739e-6,(float)3870.,(float)3880.,(float)11.688,(float).69118,(
	    float)-.0033148,(float)3.0917e-6,(float)-.0045244,(float)
	    8.2466e-7,(float)3880.,(float)3890.,(float)15.787,(float).75496,(
	    float)-3.761e-4,(float)-9.5219e-6,(float)-.0028616,(float)
	    -7.036e-6,(float)3890.,(float)3900.,(float)12.339,(float)1.2573,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3900.,(float)3910.,(float)4.4035,(float)1.1712,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3910.,(float)3920.,(float)1.452,(float)1.1029,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3920.,(float)3930.,(float).94805,(float).51649,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3930.,(float)3940.,(float)1.3512,(float).57924,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3940.,(float)3950.,(float)1.2982,(float).5633,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3950.,(float)3960.,(float)1.1898,(float).7034,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3960.,(float)3970.,(float)1.2669,(float).70992,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3970.,(float)3980.,(float)1.1486,(float).75703,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3980.,(float)3990.,(float).94294,(float).67691,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)3990.,(float)4e3,(float)1.1722,(float).71803,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4e3,(float)4010.,(float)1.6614,(float)1.1434,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4010.,(float)4020.,(float)1.3169,(float).99418,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4020.,(float)4030.,(float)1.6675,(float)1.5586,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4030.,(float)4040.,(float)2.2754,(float)1.5263,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4040.,(float)4050.,(float)3.3696,(float)1.614,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4050.,(float)4060.,(float)4.4078,(float)1.7299,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4060.,(float)4070.,(float)6.4093,(float)1.6235,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4070.,(float)4080.,(float)8.8553,(float)1.445,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4080.,(float)4090.,(float)8.5641,(float)1.5633,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4090.,(float)4100.,(float)16.356,(float)1.9485,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4100.,(float)4110.,(float)15.552,(float)1.5099,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4110.,(float)4120.,(float)23.823,(float)2.7996,(
	    float).0010215,(float)-1.7301e-5,(float)-.0020195,(float)
	    -1.1177e-5,(float)4120.,(float)4130.,(float)18.843,(float)1.4075,(
	    float).0058105,(float)-2.6748e-5,(float)6.4011e-4,(float)
	    -1.7934e-5,(float)4130.,(float)4140.,(float)7.1651,(float).45187,(
	    float).0070593,(float)-2.9644e-5,(float).0039603,(float)
	    -2.9724e-5,(float)4140.,(float)4150.,(float)37.171,(float).44157,(
	    float).0068418,(float)-3.8862e-5,(float).0036076,(float)
	    -3.2195e-5,(float)4150.,(float)4160.,(float)38.158,(float).32296,(
	    float).0043474,(float)-2.9319e-5,(float).00185,(float)-2.5238e-5,(
	    float)4160.,(float)4170.,(float)65.621,(float).45796,(float)
	    .0022774,(float)-2.0417e-5,(float)8.223e-5,(float)-1.7919e-5,(
	    float)4170.,(float)4180.,(float)79.347,(float).33603,(float)
	    -8.4769e-4,(float)-8.4263e-6,(float)-.0034772,(float)-3.6512e-6,(
	    float)4180.,(float)4190.,(float)65.038,(float).4819,(float)
	    -.0014827,(float)-1.4444e-6,(float)-.0022137,(float)-5.6866e-6,(
	    float)4190.,(float)4200.,(float)86.982,(float).77363,(float)
	    -7.1575e-4,(float)2.9104e-7,(float)-.0017062,(float)-5.7132e-6,(
	    float)4200.,(float)4210.,(float)309.82,(float)1.6801,(float)
	    -8.9037e-4,(float)-1.3593e-6,(float)-.0021551,(float)-4.6701e-6,(
	    float)4210.,(float)4220.,(float)81.077,(float).588,(float)
	    -.0012356,(float)-7.661e-7,(float)-.0026793,(float)-4.4252e-6,(
	    float)4220.,(float)4230.,(float)89.975,(float).39899,(float)
	    -.0039748,(float)5.6557e-6,(float)-.0048805,(float)4.5246e-6,(
	    float)4230.,(float)4240.,(float)148.96,(float).80968,(float)
	    -.002216,(float)-1.5823e-6,(float)-.0039631,(float)-2.9253e-7,(
	    float)4240.,(float)4250.,(float)155.94,(float)1.2514,(float)
	    4.5507e-4,(float)-8.2566e-6,(float)-.0014417,(float)-8.5226e-6,(
	    float)4250.,(float)4260.,(float)100.6,(float)1.2829,(float)
	    .0020903,(float)-1.6069e-5,(float)-3.6824e-4,(float)-1.5144e-5,(
	    float)4260.,(float)4270.,(float)117.04,(float)1.5739,(float)
	    5.5952e-4,(float)-2.1104e-6,(float)-8.6496e-4,(float)-9.3941e-6,(
	    float)4270.,(float)4280.,(float)72.794,(float)1.549,(float)
	    -6.9688e-4,(float)-3.1516e-6,(float)-.0027541,(float)-2.9267e-6,(
	    float)4280.,(float)4290.,(float)55.269,(float).72223,(float)
	    -.0034302,(float)5.1174e-6,(float)-.0052484,(float)7.0355e-6,(
	    float)4290.,(float)4300.,(float)59.856,(float)1.1762,(float)
	    -5.5565e-4,(float)2.0772e-7,(float)-.0019773,(float)-5.0966e-6,(
	    float)4300.,(float)4310.,(float)218.17,(float)2.2377,(float)
	    -.0013492,(float)-1.8427e-6,(float)-.0029966,(float)-3.2329e-6,(
	    float)4310.,(float)4320.,(float)10.284,(float).071846,(float)
	    -.0057696,(float)9.8901e-6,(float)-.0087876,(float)1.5909e-5,(
	    float)4320.,(float)4330.,(float)96.725,(float).88048,(float)
	    -.0045462,(float)6.1508e-6,(float)-.0069991,(float)9.6787e-6,(
	    float)4330.,(float)4340.,(float)111.26,(float)1.6893,(float)
	    -.002444,(float)5.0136e-7,(float)-.0044752,(float)1.1244e-6,(
	    float)4340.,(float)4350.,(float)102.83,(float)1.8062,(float)
	    -3.868e-4,(float)-5.0005e-6,(float)-.0021043,(float)-6.8556e-6,(
	    float)4350.,(float)4360.,(float)89.61,(float)1.1851,(float)
	    -6.654e-4,(float)2.3556e-6,(float)-.001897,(float)-3.4232e-6,(
	    float)4360.,(float)4370.,(float)66.668,(float).73012,(float)
	    -.0022103,(float)2.3323e-6,(float)-.0030787,(float)-5.6056e-7,(
	    float)4370.,(float)4380.,(float)81.907,(float).54144,(float)
	    -.0025213,(float)-8.9597e-7,(float)-.0047474,(float)1.5149e-6,(
	    float)4380.,(float)4390.,(float)90.229,(float).49784,(float)
	    -.0023921,(float)-1.0143e-6,(float)-.0043793,(float)4.6547e-7,(
	    float)4390.,(float)4400.,(float)82.849,(float).85204,(float)
	    -1.0412e-4,(float)-8.1641e-6,(float)-.0020032,(float)-8.0151e-6,(
	    float)4400.,(float)4410.,(float)44.643,(float).57527,(float)
	    .0023103,(float)-1.4133e-5,(float)8.589e-4,(float)-1.8551e-5,(
	    float)4410.,(float)4420.,(float)69.032,(float).78506,(float)
	    3.9855e-4,(float)-1.0098e-5,(float)-.0010159,(float)-1.0449e-5,(
	    float)4420.,(float)4430.,(float)58.683,(float).96123,(float)
	    .0024151,(float)-1.8621e-5,(float)9.2225e-4,(float)-1.9737e-5,(
	    float)4430.,(float)4440.,(float)37.181,(float)1.0126,(float)
	    .0046217,(float)-2.9919e-5,(float).0020589,(float)-2.6447e-5,(
	    float)4440.,(float)4450.,(float)42.607,(float)1.0332,(float)
	    .0047981,(float)-3.088e-5,(float).0014909,(float)-2.4304e-5,(
	    float)4450.,(float)4460.,(float)33.113,(float).94984,(float)
	    .0043677,(float)-2.6166e-5,(float).0012754,(float)-2.2104e-5,(
	    float)4460.,(float)4470.,(float)32.668,(float).8529,(float)
	    .0023495,(float)-1.622e-5,(float)-4.7342e-4,(float)-1.3768e-5,(
	    float)4470.,(float)4480.,(float)23.216,(float).8354,(float)
	    .0022982,(float)-7.3604e-6,(float)6.8926e-4,(float)-1.3485e-5,(
	    float)4480.,(float)4490.,(float)13.719,(float).6592,(float)
	    .0014982,(float)-6.2753e-6,(float)-3.7161e-4,(float)-1.1492e-5,(
	    float)4490.,(float)4500.,(float)16.799,(float).41229,(float)
	    -7.848e-4,(float)1.7545e-6,(float)-.001216,(float)-7.1041e-6,(
	    float)4500.,(float)4510.,(float)10.085,(float).14021,(float)
	    -.0037976,(float)6.6108e-6,(float)-.0050669,(float)6.8711e-6,(
	    float)4510.,(float)4520.,(float)3.1009,(float).087599,(float)
	    -.0043682,(float)1.0148e-5,(float)-.0057555,(float)1.0223e-5,(
	    float)4520.,(float)4530.,(float)18.983,(float)1.5932,(float)
	    .0014519,(float)-9.1544e-6,(float)-9.7256e-4,(float)-1.0139e-5,(
	    float)4530.,(float)4540.,(float)63.83,(float)2.0016,(float)
	    -.0011189,(float)-2.1104e-6,(float)-.0028048,(float)-3.8802e-6,(
	    float)4540.,(float)4550.,(float)3.1441,(float).067773,(float)
	    -.0040447,(float)1.4471e-5,(float)-.0037016,(float)5.8757e-6,(
	    float)4550.,(float)4560.,(float)2.1782,(float).029389,(float)
	    -.005929,(float)1.0501e-5,(float)-.00897,(float)1.6625e-5,(float)
	    4560.,(float)4570.,(float)4.0086,(float).078646,(float)-.0053825,(
	    float)8.3305e-6,(float)-.0083558,(float)1.4185e-5,(float)4570.,(
	    float)4580.,(float)1.3954,(float).11032,(float)-.0040678,(float)
	    3.1725e-6,(float)-.0071674,(float)9.4743e-6,(float)4580.,(float)
	    4590.,(float)8.2827,(float).13524,(float)-.0044855,(float)
	    4.8983e-6,(float)-.0072447,(float)9.9023e-6,(float)4590.,(float)
	    4600.,(float)8.1728,(float).18866,(float)-.0033132,(float)
	    4.1656e-7,(float)-.0059898,(float)5.0092e-6,(float)4600.,(float)
	    4610.,(float)6.4947,(float).31086,(float)-.0016438,(float)
	    -5.9966e-6,(float)-.0043498,(float)-1.4967e-6,(float)4610.,(float)
	    4620.,(float)6.3479,(float).34741,(float)1.5663e-4,(float)
	    -1.2499e-5,(float)-.0026034,(float)-7.9833e-6,(float)4620.,(float)
	    4630.,(float)2.4602,(float).20366,(float).0017162,(float)
	    -1.8956e-5,(float)-9.66e-4,(float)-1.4741e-5,(float)4630.,(float)
	    4640.,(float)2.0994,(float).2061,(float).004364,(float)-2.6098e-5,
	    (float).0019605,(float)-2.4954e-5,(float)4640.,(float)4650.,(
	    float)1.6635,(float).21606,(float).0038987,(float)-2.7059e-5,(
	    float).001059,(float)-2.2496e-5,(float)4650.,(float)4660.,(float)
	    .9148,(float).11908,(float).0055341,(float)-3.5022e-5,(float)
	    .002491,(float)-2.9056e-5,(float)4660.,(float)4670.,(float)0.,(
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
/*     methane (2500 - 5050 cm-1) */

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
} /* meth1_ */

#ifdef __cplusplus
	}
#endif
