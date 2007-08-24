/* WAVA1.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       SUBROUTINE wava1(a,inu) >*/
/* Subroutine */ int wava1_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)5.2155e-5,(
	    float).1088,(float).024708,(float)5.6434e-5,(float).028126,(float)
	    -3.6504e-5,(float)2500.,(float)2510.,(float)2.6024e-4,(float)
	    .21216,(float).025876,(float)3.0026e-5,(float).030504,(float)
	    -6.2253e-5,(float)2510.,(float)2520.,(float)1.2221e-4,(float)
	    .091374,(float).023862,(float)-7.9891e-5,(float).020651,(float)
	    -8.5449e-5,(float)2520.,(float)2530.,(float)6.8892e-5,(float)
	    .12703,(float).030103,(float)-6.0355e-5,(float).031571,(float)
	    -9.5671e-5,(float)2530.,(float)2540.,(float)9.5684e-5,(float)
	    .11204,(float).033051,(float)-9.6752e-5,(float).030485,(float)
	    -1.1273e-4,(float)2540.,(float)2550.,(float)1.0008e-4,(float)
	    .092789,(float).016702,(float)4.1104e-5,(float).021654,(float)
	    -1.2175e-5,(float)2550.,(float)2560.,(float)1.1515e-4,(float)
	    .072392,(float).028437,(float)-8.9454e-5,(float).028734,(float)
	    -1.022e-4,(float)2560.,(float)2570.,(float)1.098e-4,(float)
	    .093023,(float).023611,(float)1.0889e-5,(float).02688,(float)
	    -5.0398e-5,(float)2570.,(float)2580.,(float)1.0952e-4,(float)
	    .07579,(float).029206,(float)-9.8206e-5,(float).030307,(float)
	    -1.1103e-4,(float)2580.,(float)2590.,(float)1.1941e-4,(float)
	    .070428,(float).019144,(float)-2.8423e-5,(float).019587,(float)
	    -5.4379e-5,(float)2590.,(float)2600.,(float)8.0498e-5,(float)
	    .061889,(float).024778,(float)-5.5252e-5,(float).028205,(float)
	    -1.2627e-4,(float)2600.,(float)2610.,(float)7.2792e-5,(float)
	    .062776,(float).026682,(float)-5.8101e-5,(float).029166,(float)
	    -1.1148e-4,(float)2610.,(float)2620.,(float)1.6616e-4,(float)
	    .015773,(float).023141,(float)-6.0394e-5,(float).024947,(float)
	    -1.1178e-4,(float)2620.,(float)2630.,(float)1.8043e-4,(float)
	    .087686,(float).026248,(float)-3.0398e-5,(float).03174,(float)
	    -1.367e-4,(float)2630.,(float)2640.,(float)1.3339e-4,(float)
	    .05328,(float).012701,(float)-4.6874e-5,(float).012628,(float)
	    -6.7487e-5,(float)2640.,(float)2650.,(float)4.115e-5,(float)
	    .0075778,(float).011257,(float)1.1038e-4,(float).036362,(float)
	    -1.1284e-4,(float)2650.,(float)2660.,(float)6.1395e-5,(float)
	    .026074,(float).016798,(float)1.0755e-4,(float).035481,(float)
	    -1.3778e-4,(float)2660.,(float)2670.,(float)3.5062e-4,(float)
	    .080662,(float).021065,(float)-3.9968e-5,(float).021602,(float)
	    -8.7339e-5,(float)2670.,(float)2680.,(float)2.6391e-4,(float)
	    .11483,(float).02727,(float)-4.0691e-5,(float).029928,(float)
	    -1.1046e-4,(float)2680.,(float)2690.,(float)8.741e-4,(float)
	    .06007,(float).021732,(float)-8.76e-5,(float).020645,(float)
	    -9.6557e-5,(float)2690.,(float)2700.,(float)3.0675e-5,(float)
	    .02297,(float).0094212,(float)7.8691e-6,(float).015402,(float)
	    -7.4597e-5,(float)2700.,(float)2710.,(float)2.1928e-4,(float)
	    .13351,(float).022082,(float)-4.5472e-5,(float).02546,(float)
	    -9.6549e-5,(float)2710.,(float)2720.,(float)3.4904e-4,(float)
	    .095073,(float).013969,(float)1.2276e-6,(float).016415,(float)
	    -6.5204e-5,(float)2720.,(float)2730.,(float).002136,(float)
	    .061072,(float).018908,(float)-7.653e-5,(float).019801,(float)
	    -7.7251e-5,(float)2730.,(float)2740.,(float)1.3372e-4,(float)
	    .10806,(float).025683,(float)-4.923e-5,(float).025729,(float)
	    -8.9387e-5,(float)2740.,(float)2750.,(float).0021697,(float)
	    .062188,(float).018593,(float)-8.0474e-5,(float).016349,(float)
	    -8.1422e-5,(float)2750.,(float)2760.,(float)4.503e-5,(float)
	    .014902,(float).011376,(float)2.086e-4,(float).04688,(float)
	    -1.3018e-4,(float)2760.,(float)2770.,(float)3.9413e-4,(float)
	    .075433,(float).019773,(float)3.0322e-5,(float).02086,(float)
	    -4.5326e-5,(float)2770.,(float)2780.,(float).0048935,(float)
	    .096746,(float).019042,(float)-7.2945e-5,(float).016358,(float)
	    -7.6368e-5,(float)2780.,(float)2790.,(float)5.6197e-5,(float)
	    .026178,(float).017977,(float)1.4035e-4,(float).032163,(float)
	    -7.1639e-5,(float)2790.,(float)2800.,(float).0017459,(float)
	    .049818,(float).012334,(float)-5.2052e-5,(float).013169,(float)
	    -4.5266e-5,(float)2800.,(float)2810.,(float).013747,(float)
	    .048325,(float).012754,(float)-6.181e-5,(float).011564,(float)
	    -4.9886e-5,(float)2810.,(float)2820.,(float)8.2805e-4,(float)
	    .084185,(float).021588,(float)3.0162e-6,(float).021186,(float)
	    -3.7441e-5,(float)2820.,(float)2830.,(float).011866,(float)
	    .034247,(float).0094911,(float)-5.0331e-5,(float).0068889,(float)
	    -4.5062e-5,(float)2830.,(float)2840.,(float).0053887,(float)
	    .10669,(float).021019,(float)-5.3528e-5,(float).021717,(float)
	    -5.4515e-5,(float)2840.,(float)2850.,(float)1.0411e-4,(float)
	    .11175,(float).031522,(float)3.5658e-5,(float).036103,(float)
	    -8.82e-5,(float)2850.,(float)2860.,(float).0079899,(float).07314,(
	    float).02926,(float)-1.0778e-4,(float).03194,(float)-1.11e-4,(
	    float)2860.,(float)2870.,(float).024725,(float).10335,(float)
	    .008848,(float)-3.2902e-5,(float).010195,(float)-1.929e-5,(float)
	    2870.,(float)2880.,(float)5.7422e-4,(float).18372,(float).046405,(
	    float)-1.7178e-4,(float).046094,(float)-1.887e-4,(float)2880.,(
	    float)2890.,(float).067995,(float).089991,(float).016432,(float)
	    -5.4992e-5,(float).019457,(float)-5.5113e-5,(float)2890.,(float)
	    2900.,(float).03635,(float).17982,(float).015309,(float)7.9551e-7,
	    (float).018904,(float)-3.6064e-5,(float)2900.,(float)2910.,(float)
	    .02725,(float).14127,(float).030783,(float)-1.2895e-4,(float)
	    .031036,(float)-1.2355e-4,(float)2910.,(float)2920.,(float)
	    .002652,(float).19136,(float).041919,(float)-1.6834e-4,(float)
	    .042452,(float)-1.753e-4,(float)2920.,(float)2930.,(float).52888,(
	    float).269,(float).012546,(float)-3.0896e-5,(float).013568,(float)
	    -4.2996e-5,(float)2930.,(float)2940.,(float).053771,(float).17608,
	    (float).020209,(float)-6.024e-5,(float).025687,(float)-8.3758e-5,(
	    float)2940.,(float)2950.,(float).32492,(float).1684,(float)
	    .021364,(float)-9.2341e-5,(float).020449,(float)-8.8117e-5,(float)
	    2950.,(float)2960.,(float).61749,(float).1805,(float).013955,(
	    float)-6.0114e-5,(float).014337,(float)-5.833e-5,(float)2960.,(
	    float)2970.,(float)1.5421,(float).13158,(float).0096584,(float)
	    -4.1781e-5,(float).0089471,(float)-3.729e-5,(float)2970.,(float)
	    2980.,(float)1.6668,(float).2273,(float).0099338,(float)
	    -3.7288e-5,(float).01043,(float)-3.702e-5,(float)2980.,(float)
	    2990.,(float)1.5158,(float).13985,(float).011565,(float)
	    -5.1421e-5,(float).011056,(float)-4.3391e-5,(float)2990.,(float)
	    3e3,(float)1.6503,(float).17713,(float).0095447,(float)-3.7275e-5,
	    (float).0086134,(float)-3.6851e-5,(float)3e3,(float)3010.,(float)
	    6.5433,(float).21911,(float).0060474,(float)-3.289e-5,(float)
	    .0053084,(float)-2.7435e-5,(float)3010.,(float)3020.,(float)
	    2.2497,(float).12464,(float)8.6253e-4,(float)-6.1061e-6,(float)
	    .0037153,(float)4.0466e-6,(float)3020.,(float)3030.,(float)10.011,
	    (float).23151,(float).0027571,(float)-1.9224e-5,(float).0021442,(
	    float)-1.1081e-5,(float)3030.,(float)3040.,(float)2.7015,(float)
	    .12619,(float).0016271,(float)-1.3887e-5,(float).001636,(float)
	    -1.9543e-8,(float)3040.,(float)3050.,(float)5.2767,(float).13242,(
	    float).001288,(float)-1.3169e-5,(float).0012577,(float)-3.613e-6,(
	    float)3050.,(float)3060.,(float)9.1595,(float).12205,(float)
	    -.0012513,(float)-4.0326e-6,(float)-.0018414,(float)2.4599e-6,(
	    float)3060.,(float)3070.,(float)4.6857,(float).15647,(float)
	    -4.3308e-5,(float)-6.9326e-6,(float)-8.1377e-4,(float)-3.1694e-6,(
	    float)3070.,(float)3080.,(float)3.2521,(float).14435,(float)
	    .0035016,(float)-2.0102e-5,(float).0037489,(float)-1.4446e-5,(
	    float)3080.,(float)3090.,(float)8.6678,(float).20679,(float)
	    -.0014866,(float)2.2765e-6,(float)-.001046,(float)1.7536e-6,(
	    float)3090.,(float)3100.,(float)9.0277,(float).2295,(float)
	    -.0010574,(float)-1.1212e-6,(float)-6.8088e-4,(float)-3.9181e-8,(
	    float)3100.,(float)3110.,(float)12.148,(float).38295,(float)
	    4.9769e-4,(float)-4.4194e-6,(float)4.3116e-4,(float)-4.0708e-6,(
	    float)3110.,(float)3120.,(float)9.4078,(float).34012,(float)
	    -3.175e-5,(float)2.1043e-6,(float).0022348,(float)1.6614e-7,(
	    float)3120.,(float)3130.,(float)6.1004,(float).077373,(float)
	    -.004677,(float)1.1726e-5,(float)-5.7011e-4,(float)3.3047e-5,(
	    float)3130.,(float)3140.,(float).64298,(float).083113,(float)
	    -9.0344e-4,(float)7.4815e-6,(float).0068034,(float)3.8978e-5,(
	    float)3140.,(float)3150.,(float).31468,(float).22047,(float)
	    .0087793,(float)-1.3249e-5,(float).014974,(float)-2.9267e-5,(
	    float)3150.,(float)3160.,(float).84773,(float).34913,(float)
	    .0023565,(float)-7.7347e-7,(float).004167,(float)2.4148e-6,(float)
	    3160.,(float)3170.,(float)6.7043,(float).10518,(float)-.0049662,(
	    float)1.3112e-5,(float)-9.2458e-4,(float)3.1899e-5,(float)3170.,(
	    float)3180.,(float)3.3487,(float).17575,(float)-.0036206,(float)
	    1.2779e-5,(float)5.0643e-5,(float)2.6882e-5,(float)3180.,(float)
	    3190.,(float)7.1051,(float).15577,(float)-.0032039,(float)
	    5.7362e-6,(float)-.0027319,(float)2.0608e-5,(float)3190.,(float)
	    3200.,(float)4.9187,(float).094692,(float)-.0034658,(float)
	    9.9349e-6,(float)6.5696e-4,(float)2.9105e-5,(float)3200.,(float)
	    3210.,(float)11.08,(float).15706,(float)-.0036162,(float)
	    6.4921e-6,(float)-.002575,(float)1.9544e-5,(float)3210.,(float)
	    3220.,(float)6.4326,(float).26178,(float)-3.2818e-4,(float)
	    3.2261e-6,(float)5.6524e-4,(float)5.0713e-6,(float)3220.,(float)
	    3230.,(float)4.8444,(float).21447,(float)-.0019614,(float)
	    1.5381e-5,(float).0019305,(float)2.4092e-5,(float)3230.,(float)
	    3240.,(float)8.8873,(float).19618,(float)-.0011584,(float)
	    1.5371e-6,(float)5.7401e-4,(float)5.5356e-6,(float)3240.,(float)
	    3250.,(float)6.7011,(float).11596,(float)3.1879e-4,(float)
	    -3.2082e-7,(float).0027244,(float)9.5927e-6,(float)3250.,(float)
	    3260.,(float)3.2951,(float).21537,(float).0018132,(float)
	    -9.3642e-6,(float).0034721,(float)-1.0415e-5,(float)3260.,(float)
	    3270.,(float)11.905,(float).31637,(float)-1.7961e-5,(float)
	    7.0218e-6,(float).0027392,(float)8.7434e-8,(float)3270.,(float)
	    3280.,(float)4.8139,(float).2732,(float).0023884,(float)2.6589e-5,
	    (float).0043504,(float)5.7992e-6,(float)3280.,(float)3290.,(float)
	    6.9428,(float).3268,(float).0028073,(float)-7.0232e-6,(float)
	    .0032805,(float)-8.5722e-6,(float)3290.,(float)3300.,(float)
	    5.2753,(float).19806,(float).0027307,(float)-1.4565e-6,(float)
	    .0049125,(float)-1.2966e-5,(float)3300.,(float)3310.,(float)
	    4.7208,(float).23262,(float).002983,(float)2.6911e-5,(float)
	    .0064942,(float)-7.7622e-6,(float)3310.,(float)3320.,(float)
	    3.2466,(float).37221,(float).0099704,(float)-2.8634e-5,(float)
	    .011158,(float)-3.6092e-5,(float)3320.,(float)3330.,(float)4.2083,
	    (float).17584,(float).013723,(float)-3.9504e-5,(float).013219,(
	    float)-4.4922e-5,(float)3330.,(float)3340.,(float).78485,(float)
	    .29505,(float).018153,(float)-7.6368e-5,(float).018172,(float)
	    -6.9561e-5,(float)3340.,(float)3350.,(float)4.0763,(float).2731,(
	    float).011974,(float)-3.8706e-5,(float).010563,(float)-3.3592e-5,(
	    float)3350.,(float)3360.,(float)12.275,(float).42572,(float)
	    .0045309,(float)5.9958e-6,(float).0056864,(float)-1.5831e-5,(
	    float)3360.,(float)3370.,(float).94088,(float).2913,(float)
	    .012712,(float)-2.3725e-5,(float).01375,(float)-2.6806e-5,(float)
	    3370.,(float)3380.,(float)5.802,(float).18758,(float).0056963,(
	    float)-9.9475e-6,(float).0091903,(float)-1.2922e-5,(float)3380.,(
	    float)3390.,(float)11.279,(float).22153,(float).010955,(float)
	    -4.55e-5,(float).010347,(float)-3.3646e-5,(float)3390.,(float)
	    3400.,(float)3.0915,(float).29101,(float).0076973,(float)
	    -3.1164e-6,(float).0079615,(float)-1.2231e-5,(float)3400.,(float)
	    3410.,(float)2.746,(float).24709,(float).01353,(float)-3.9278e-5,(
	    float).015939,(float)-3.5039e-5,(float)3410.,(float)3420.,(float)
	    14.49,(float).18486,(float).0089652,(float)-3.5228e-5,(float)
	    .009544,(float)-2.5256e-5,(float)3420.,(float)3430.,(float)4.152,(
	    float).37165,(float).0094576,(float)-9.1648e-6,(float).01319,(
	    float)-1.9085e-5,(float)3430.,(float)3440.,(float)35.28,(float)
	    .29267,(float).0077335,(float)-3.3655e-5,(float).0086235,(float)
	    -3.439e-5,(float)3440.,(float)3450.,(float)1.6073,(float).33722,(
	    float).016402,(float)1.5852e-5,(float).018964,(float)-3.2271e-5,(
	    float)3450.,(float)3460.,(float)13.605,(float).31569,(float)
	    .0095057,(float)-1.5485e-5,(float).0094813,(float)-1.6751e-5,(
	    float)3460.,(float)3470.,(float)7.5164,(float).27307,(float)
	    .0075889,(float)-4.7962e-6,(float).012249,(float)-2.3811e-5,(
	    float)3470.,(float)3480.,(float)33.484,(float).31131,(float)
	    .0077627,(float)-1.1503e-5,(float).0094019,(float)-2.0136e-5,(
	    float)3480.,(float)3490.,(float)13.815,(float).23917,(float)
	    .010592,(float)-3.1935e-5,(float).012433,(float)-3.6436e-5,(float)
	    3490.,(float)3500.,(float)61.974,(float).36287,(float).0075577,(
	    float)2.6848e-6,(float).010181,(float)-2.3323e-5,(float)3500.,(
	    float)3510.,(float)14.812,(float).37195,(float).008261,(float)
	    -1.8654e-5,(float).010113,(float)-3.1482e-5,(float)3510.,(float)
	    3520.,(float)83.223,(float).39932,(float).0076795,(float)
	    5.0656e-7,(float).0082892,(float)-2.0998e-5,(float)3520.,(float)
	    3530.,(float)21.464,(float).29835,(float).014148,(float)
	    -4.3852e-5,(float).013193,(float)-5.1897e-5,(float)3530.,(float)
	    3540.,(float)78.956,(float).41357,(float).012512,(float)
	    -2.8052e-5,(float).0093167,(float)-3.188e-5,(float)3540.,(float)
	    3550.,(float)57.829,(float).33002,(float).0050941,(float)
	    -8.3644e-6,(float).0066338,(float)-1.927e-5,(float)3550.,(float)
	    3560.,(float)264.95,(float).48627,(float).0094242,(float)
	    -2.4386e-5,(float).0071083,(float)-3.052e-5,(float)3560.,(float)
	    3570.,(float)48.65,(float).32647,(float).003423,(float)-1.2164e-5,
	    (float).0023536,(float)-9.56e-6,(float)3570.,(float)3580.,(float)
	    278.65,(float).3935,(float).010521,(float)-4.8325e-5,(float)
	    .0076691,(float)-3.9509e-5,(float)3580.,(float)3590.,(float)
	    160.87,(float).35038,(float).0056295,(float)-1.9831e-5,(float)
	    .0040519,(float)-1.575e-5,(float)3590.,(float)3600.,(float)337.87,
	    (float).23399,(float).0064862,(float)-3.0846e-5,(float).0044382,(
	    float)-2.4648e-5,(float)3600.,(float)3610.,(float)576.3,(float)
	    .34952,(float).0047866,(float)-2.6607e-5,(float).0014788,(float)
	    -1.5623e-5,(float)3610.,(float)3620.,(float)753.87,(float).18159,(
	    float).0042931,(float)-2.8734e-5,(float).0024163,(float)
	    -2.2384e-5,(float)3620.,(float)3630.,(float)135.67,(float).19692,(
	    float).0012097,(float)-8.0313e-6,(float)-4.4021e-4,(float)
	    5.3739e-7,(float)3630.,(float)3640.,(float)776.59,(float).21673,(
	    float).0021798,(float)-2.0291e-5,(float)5.0596e-4,(float)
	    -1.5749e-5,(float)3640.,(float)3650.,(float)657.22,(float).1435,(
	    float).001212,(float)-1.6829e-5,(float)-2.0471e-4,(float)
	    -9.1773e-6,(float)3650.,(float)3660.,(float)229.37,(float).088535,
	    (float)-3.47e-4,(float)-8.0698e-6,(float)7.053e-4,(float)
	    -5.8648e-7,(float)3660.,(float)3670.,(float)1746.9,(float).21306,(
	    float)-.0011718,(float)-7.1619e-6,(float)-.0032694,(float)
	    -2.8781e-7,(float)3670.,(float)3680.,(float)833.45,(float).086211,
	    (float)-.0027517,(float)-7.8552e-7,(float)-.0033472,(float)
	    1.3487e-5,(float)3680.,(float)3690.,(float)324.07,(float).27669,(
	    float)-.0021659,(float)4.4909e-7,(float)-.0016982,(float)
	    1.0818e-5,(float)3690.,(float)3700.,(float)485.79,(float).23163,(
	    float)-.0033766,(float)7.1088e-6,(float)-.0016229,(float)
	    1.3215e-5,(float)3700.,(float)3710.,(float)693.99,(float).19031,(
	    float)-.0023753,(float)1.1342e-5,(float)2.795e-4,(float)1.1827e-5,
	    (float)3710.,(float)3720.,(float)309.49,(float).32694,(float)
	    .0051146,(float)-3.3747e-6,(float).0055153,(float)-1.4764e-5,(
	    float)3720.,(float)3730.,(float)1205.8,(float).33098,(float)
	    2.6244e-4,(float)4.4389e-6,(float)4.3858e-4,(float)-5.8687e-6,(
	    float)3730.,(float)3740.,(float)2387.3,(float).35395,(float)
	    -.0013313,(float)-2.6877e-6,(float)-.0025997,(float)-1.5668e-6,(
	    float)3740.,(float)3750.,(float)1331.8,(float).24255,(float)
	    -.0023524,(float)5.7434e-7,(float)-.0030581,(float)2.0511e-6,(
	    float)3750.,(float)3760.,(float)462.66,(float).18423,(float)
	    -.002022,(float)1.4083e-6,(float)-.0011856,(float)1.3407e-6,(
	    float)3760.,(float)3770.,(float)221.29,(float).15206,(float)
	    -.0045078,(float)1.2626e-5,(float)-.0025579,(float)1.1787e-5,(
	    float)3770.,(float)3780.,(float)72.994,(float).26021,(float)
	    -9.4598e-4,(float)5.8721e-6,(float).0020193,(float)1.4281e-6,(
	    float)3780.,(float)3790.,(float)243.54,(float).11623,(float)
	    -.0039723,(float)1.0003e-5,(float)-.0024749,(float)1.1674e-5,(
	    float)3790.,(float)3800.,(float)1610.2,(float).13844,(float)
	    -.0050654,(float)8.5459e-6,(float)-.0061217,(float)1.4867e-5,(
	    float)3800.,(float)3810.,(float)925.09,(float).095941,(float)
	    -.0040019,(float)4.2438e-6,(float)-.0041671,(float)1.3408e-5,(
	    float)3810.,(float)3820.,(float)878.65,(float).19813,(float)
	    -.0032809,(float)1.3909e-6,(float)-.0048774,(float)6.0213e-6,(
	    float)3820.,(float)3830.,(float)1445.5,(float).15082,(float)
	    -.0027746,(float)-6.1623e-7,(float)-.004156,(float)6.6755e-6,(
	    float)3830.,(float)3840.,(float)368.8,(float).2433,(float)
	    4.8895e-4,(float)-1.2565e-5,(float)-3.1389e-4,(float)-8.6255e-6,(
	    float)3840.,(float)3850.,(float)2068.8,(float).2275,(float)
	    -8.8059e-4,(float)-7.9102e-6,(float)-.0024751,(float)-3.6839e-6,(
	    float)3850.,(float)3860.,(float)634.35,(float).19602,(float)
	    .0017316,(float)-1.7618e-5,(float)2.7542e-4,(float)-9.9025e-6,(
	    float)3860.,(float)3870.,(float)660.01,(float).16073,(float)
	    .0019181,(float)-1.8141e-5,(float)8.7743e-4,(float)-1.3268e-5,(
	    float)3870.,(float)3880.,(float)596.2,(float).22715,(float)
	    .0044999,(float)-2.8285e-5,(float).0029743,(float)-2.3179e-5,(
	    float)3880.,(float)3890.,(float)497.07,(float).23257,(float)
	    .0049027,(float)-2.8299e-5,(float).0031352,(float)-2.2089e-5,(
	    float)3890.,(float)3900.,(float)476.01,(float).20656,(float)
	    .0070284,(float)-3.7624e-5,(float).0056038,(float)-3.3998e-5,(
	    float)3900.,(float)3910.,(float)97.812,(float).12575,(float)
	    .012028,(float)-5.8026e-5,(float).010886,(float)-5.051e-5,(float)
	    3910.,(float)3920.,(float)154.07,(float).2759,(float).0075519,(
	    float)-2.7169e-5,(float).0059888,(float)-2.6593e-5,(float)3920.,(
	    float)3930.,(float)75.906,(float).19815,(float).014672,(float)
	    -6.2916e-5,(float).010813,(float)-4.7324e-5,(float)3930.,(float)
	    3940.,(float)91.761,(float).19338,(float).015621,(float)
	    -6.9646e-5,(float).014264,(float)-6.8568e-5,(float)3940.,(float)
	    3950.,(float)23.976,(float).28167,(float).0049108,(float)
	    1.7525e-5,(float).0072419,(float)-1.0423e-5,(float)3950.,(float)
	    3960.,(float)14.037,(float).20425,(float).017638,(float)
	    -2.7195e-5,(float).013686,(float)-3.6754e-5,(float)3960.,(float)
	    3970.,(float)15.37,(float).33316,(float).0098866,(float)2.0342e-5,
	    (float).011243,(float)-2.1419e-5,(float)3970.,(float)3980.,(float)
	    5.4881,(float).22417,(float).0075532,(float)3.9091e-5,(float)
	    .010416,(float)-2.6476e-6,(float)3980.,(float)3990.,(float)8.3126,
	    (float).20066,(float).013204,(float)-8.1155e-7,(float).014493,(
	    float)-3.2804e-5,(float)3990.,(float)4e3,(float)8.7033,(float)
	    .12823,(float).0053683,(float)-1.0088e-5,(float).0083956,(float)
	    -9.9974e-6,(float)4e3,(float)4010.,(float)7.1987,(float).21615,(
	    float).0019288,(float)1.9286e-5,(float).006975,(float)2.1036e-5,(
	    float)4010.,(float)4020.,(float)7.374,(float).10603,(float)
	    .0021924,(float)-1.68e-5,(float).0031402,(float)6.6314e-10,(float)
	    4020.,(float)4030.,(float)1.2253,(float).20951,(float).0061999,(
	    float)3.6357e-5,(float).012788,(float)-6.0887e-6,(float)4030.,(
	    float)4040.,(float)7.9524,(float).12691,(float).0039534,(float)
	    -2.3885e-5,(float).0036525,(float)-1.0242e-5,(float)4040.,(float)
	    4050.,(float).51733,(float).11847,(float).011888,(float)
	    -3.4371e-5,(float).016178,(float)-2.9656e-5,(float)4050.,(float)
	    4060.,(float)1.8317,(float).094248,(float).0053224,(float)
	    -2.5376e-5,(float).005784,(float)-6.4186e-6,(float)4060.,(float)
	    4070.,(float)2.7229,(float).12453,(float).0093697,(float)
	    -3.7263e-5,(float).0090599,(float)-2.2481e-5,(float)4070.,(float)
	    4080.,(float)3.4742,(float).19542,(float).0072771,(float)
	    -3.4478e-5,(float).0043552,(float)-2.4708e-5,(float)4080.,(float)
	    4090.,(float).08819,(float).11131,(float).0125,(float)-3.4253e-5,(
	    float).016631,(float)-2.1563e-5,(float)4090.,(float)4100.,(float)
	    4.472,(float).16534,(float).0066915,(float)-3.536e-5,(float)
	    .0054523,(float)-3.2561e-5,(float)4100.,(float)4110.,(float)
	    .017698,(float).14435,(float).0099143,(float)3.28e-5,(float)
	    .020212,(float)3.5978e-6,(float)4110.,(float)4120.,(float).49468,(
	    float).13443,(float).0087298,(float)-3.5276e-5,(float).0068006,(
	    float)-3.1378e-5,(float)4120.,(float)4130.,(float)2.2881,(float)
	    .15905,(float).011097,(float)-4.923e-5,(float).01025,(float)
	    -4.8021e-5,(float)4130.,(float)4140.,(float).62111,(float).16054,(
	    float).011288,(float)-5.0025e-5,(float).010196,(float)-4.7296e-5,(
	    float)4140.,(float)4150.,(float).60454,(float).14755,(float)
	    .014727,(float)-4.9548e-5,(float).014191,(float)-5.8716e-5,(float)
	    4150.,(float)4160.,(float).093094,(float).068954,(float).004002,(
	    float)-3.9015e-6,(float).0055718,(float)-1.3126e-6,(float)4160.,(
	    float)4170.,(float).27101,(float).093272,(float).013113,(float)
	    -5.6084e-5,(float).012707,(float)-5.2567e-5,(float)4170.,(float)
	    4180.,(float).5398,(float).067639,(float).016122,(float)
	    -7.5048e-5,(float).013265,(float)-6.515e-5,(float)4180.,(float)
	    4190.,(float).44133,(float).17104,(float).016673,(float)
	    -7.1388e-5,(float).014609,(float)-6.461e-5,(float)4190.,(float)
	    4200.,(float).36918,(float).11239,(float).013785,(float)-5.548e-5,
	    (float).014404,(float)-5.8254e-5,(float)4200.,(float)4210.,(float)
	    .13276,(float).12746,(float).017403,(float)-5.447e-5,(float)
	    .013863,(float)-4.8485e-5,(float)4210.,(float)4220.,(float)
	    .080464,(float).086998,(float).022044,(float)-9.2895e-5,(float)
	    .019048,(float)-8.7985e-5,(float)4220.,(float)4230.,(float)
	    .063692,(float).14834,(float).022806,(float)-8.4692e-5,(float)
	    .023548,(float)-9.253e-5,(float)4230.,(float)4240.,(float).01811,(
	    float).12119,(float).018389,(float)-2.2676e-6,(float).013683,(
	    float)-2.6036e-5,(float)4240.,(float)4250.,(float).209,(float)
	    .16597,(float).019432,(float)-7.8011e-5,(float).019282,(float)
	    -8.4089e-5,(float)4250.,(float)4260.,(float).049819,(float).06211,
	    (float).026704,(float)-1.1099e-4,(float).025655,(float)-1.0067e-4,
	    (float)4260.,(float)4270.,(float).051444,(float).1311,(float)
	    .014568,(float)-3.4291e-5,(float).014016,(float)-4.9479e-5,(float)
	    4270.,(float)4280.,(float).010759,(float).15106,(float).027993,(
	    float)-1.0013e-4,(float).023418,(float)-8.4057e-5,(float)4280.,(
	    float)4290.,(float).005931,(float).085611,(float).036412,(float)
	    -1.5063e-4,(float).034796,(float)-1.4138e-4,(float)4290.,(float)
	    4300.,(float).060294,(float).13145,(float).023253,(float)
	    -8.8313e-5,(float).022949,(float)-9.0705e-5,(float)4300.,(float)
	    4310.,(float).0087499,(float).11428,(float).024739,(float)
	    -5.6984e-5,(float).024765,(float)-7.2542e-5,(float)4310.,(float)
	    4320.,(float).0043984,(float).17658,(float).035844,(float)
	    -1.1719e-4,(float).030706,(float)-1.0295e-4,(float)4320.,(float)
	    4330.,(float).0031592,(float).14762,(float).014237,(float)
	    6.5312e-6,(float).014523,(float)-1.9336e-5,(float)4330.,(float)
	    4340.,(float).0022605,(float).13901,(float).023941,(float)
	    1.0579e-5,(float).023751,(float)-4.5551e-5,(float)4340.,(float)
	    4350.,(float)8.4338e-8,(float)1.5285e-4,(float).0047057,(float)
	    -2.5631e-5,(float).0022316,(float)-2.3738e-5,(float)4350.,(float)
	    4360.,(float)9.5411e-8,(float)1.1284e-4,(float).0051111,(float)
	    -2.6317e-5,(float).0030958,(float)-2.5483e-5,(float)4360.,(float)
	    4370.,(float)1.5708e-8,(float)4.7653e-5,(float).0099036,(float)
	    -4.9633e-5,(float).0072007,(float)-4.4307e-5,(float)4370.,(float)
	    4380.,(float)7.2646e-5,(float).073368,(float).017942,(float)
	    1.2698e-5,(float).012584,(float)-3.4551e-5,(float)4380.,(float)
	    4390.,(float)5.9152e-5,(float).041171,(float).00839,(float)
	    -4.5951e-5,(float).0057949,(float)-4.0723e-5,(float)4390.,(float)
	    4400.,(float)9.0913e-5,(float).0094376,(float).037251,(float)
	    -1.6131e-4,(float).034629,(float)-1.5582e-4,(float)4400.,(float)
	    4410.,(float)3.4561e-8,(float)4.7569e-5,(float).0015514,(float)
	    -1.0274e-5,(float)6.5701e-7,(float)-1.2526e-5,(float)4410.,(float)
	    4420.,(float)4.3107e-4,(float).054262,(float).024045,(float)
	    -4.9839e-5,(float).016547,(float)-4.4636e-5,(float)4420.,(float)
	    4430.,(float)3.1142e-4,(float).038963,(float).014134,(float)
	    -6.8927e-5,(float).011537,(float)-6.369e-5,(float)4430.,(float)
	    4440.,(float)1.4945e-8,(float)7.8332e-5,(float).0089337,(float)
	    -4.3145e-5,(float).0069004,(float)-4.2442e-5,(float)4440.,(float)
	    4450.,(float).0016802,(float).051222,(float).015502,(float)
	    -2.3447e-5,(float).010002,(float)-3.2528e-5,(float)4450.,(float)
	    4460.,(float)1.0007e-4,(float).066862,(float).02459,(float)
	    -9.8504e-5,(float).022882,(float)-1.0233e-4,(float)4460.,(float)
	    4470.,(float).0030025,(float).051153,(float).01923,(float)
	    -8.107e-5,(float).015535,(float)-6.8267e-5,(float)4470.,(float)
	    4480.,(float)8.6831e-5,(float).040458,(float).036221,(float)
	    -1.5713e-4,(float).03335,(float)-1.5083e-4,(float)4480.,(float)
	    4490.,(float).0080902,(float).1275,(float).016863,(float)
	    -7.6478e-5,(float).01678,(float)-7.7918e-5,(float)4490.,(float)
	    4500.,(float).0022317,(float).15229,(float).012535,(float)
	    -1.3795e-5,(float).010875,(float)-3.7123e-5,(float)4500.,(float)
	    4510.,(float).019169,(float).11664,(float).010716,(float)
	    -5.1479e-5,(float).0082124,(float)-4.6198e-5,(float)4510.,(float)
	    4520.,(float).0065788,(float).15986,(float).012038,(float)
	    -5.1957e-5,(float).010074,(float)-5.1345e-5,(float)4520.,(float)
	    4530.,(float).03291,(float).11768,(float).0084067,(float)
	    -4.3158e-5,(float).0067305,(float)-4.0106e-5,(float)4530.,(float)
	    4540.,(float).044345,(float).15763,(float).0035365,(float)
	    -2.4308e-5,(float)7.9048e-4,(float)-1.7012e-5,(float)4540.,(float)
	    4550.,(float).027331,(float).16819,(float).0042859,(float)
	    -2.1639e-5,(float).0038192,(float)-2.5423e-5,(float)4550.,(float)
	    4560.,(float).020407,(float).09609,(float).001546,(float)
	    -1.6323e-5,(float)6.6197e-4,(float)-9.1244e-6,(float)4560.,(float)
	    4570.,(float).057143,(float).18848,(float).0030453,(float)
	    -1.8516e-5,(float).0016176,(float)-1.8614e-5,(float)4570.,(float)
	    4580.,(float).052929,(float).13102,(float)-.0010515,(float)
	    -4.5621e-6,(float)-.0023271,(float)2.7682e-6,(float)4580.,(float)
	    4590.,(float).028557,(float).14681,(float)-4.8301e-4,(float)
	    -6.3151e-6,(float)-.0019647,(float)-5.3522e-6,(float)4590.,(float)
	    4600.,(float).0046383,(float).13391,(float).0088609,(float)
	    -2.6493e-5,(float).0082741,(float)-3.2453e-5,(float)4600.,(float)
	    4610.,(float).02773,(float).10824,(float)-.0034789,(float)
	    5.4132e-6,(float)-.0030391,(float)8.0678e-6,(float)4610.,(float)
	    4620.,(float).051267,(float).16778,(float)-.0015076,(float)
	    -7.4663e-7,(float)-.0025263,(float)3.707e-6,(float)4620.,(float)
	    4630.,(float).0129,(float).13459,(float)4.1658e-4,(float)
	    7.4475e-7,(float).0012458,(float)-7.7586e-6,(float)4630.,(float)
	    4640.,(float).044954,(float).38311,(float)-.0018643,(float)
	    4.6026e-6,(float)-.0027134,(float)4.4958e-6,(float)4640.,(float)
	    4650.,(float).0048639,(float).10343,(float).0025659,(float)
	    -1.6961e-5,(float).0023819,(float)-1.4778e-5,(float)4650.,(float)
	    4660.,(float).0012082,(float).18004,(float).014733,(float)
	    -5.2646e-5,(float).014275,(float)-6.0508e-5,(float)4660.,(float)
	    4670.,(float).0013469,(float).34467,(float).012177,(float)
	    -2.3395e-5,(float).010966,(float)-3.6299e-5,(float)4670.,(float)
	    4680.,(float).0068991,(float).1607,(float)6.4738e-4,(float)
	    3.7071e-6,(float).0032901,(float)-2.0981e-6,(float)4680.,(float)
	    4690.,(float).027452,(float).060959,(float)-.0051851,(float)
	    1.2668e-5,(float)-.0041528,(float)2.6305e-5,(float)4690.,(float)
	    4700.,(float).017796,(float).15523,(float)-.0027265,(float)
	    3.7407e-6,(float)-.0033733,(float)1.0358e-5,(float)4700.,(float)
	    4710.,(float).011446,(float).17796,(float)-6.8324e-4,(float)
	    6.8406e-6,(float)-.0010826,(float)5.9431e-6,(float)4710.,(float)
	    4720.,(float).029309,(float).13652,(float)-.0017282,(float)
	    6.762e-6,(float)5.6764e-4,(float)1.3237e-5,(float)4720.,(float)
	    4730.,(float).021395,(float).18395,(float)-.0015768,(float)
	    1.4383e-5,(float)1.5897e-4,(float)8.796e-6,(float)4730.,(float)
	    4740.,(float).0025077,(float).18622,(float).0042912,(float)
	    -4.9559e-6,(float).0045676,(float)-2.6247e-6,(float)4740.,(float)
	    4750.,(float).031587,(float).13943,(float)-.0012088,(float)
	    -8.9815e-8,(float)-1.571e-4,(float)2.0359e-5,(float)4750.,(float)
	    4760.,(float).034566,(float).28253,(float).0016858,(float)
	    -8.995e-6,(float)9.752e-4,(float)-8.1082e-6,(float)4760.,(float)
	    4770.,(float).020369,(float).14591,(float)-.0014421,(float)
	    1.9399e-5,(float).0039665,(float)1.4338e-5,(float)4770.,(float)
	    4780.,(float).0081447,(float).12195,(float).0013964,(float)
	    3.2025e-5,(float).0037728,(float)2.0537e-5,(float)4780.,(float)
	    4790.,(float).019803,(float).14625,(float)9.6298e-4,(float)
	    -2.9783e-6,(float).0041118,(float)-9.7923e-7,(float)4790.,(float)
	    4800.,(float).0052146,(float).17691,(float).014543,(float)
	    -3.1749e-5,(float).0149,(float)-4.4049e-5,(float)4800.,(float)
	    4810.,(float).035065,(float).21617,(float)-.0021492,(float)
	    2.3072e-5,(float).001178,(float)2.4416e-5,(float)4810.,(float)
	    4820.,(float).012656,(float).15792,(float).0083705,(float)
	    -2.1219e-5,(float).0082684,(float)-2.8775e-5,(float)4820.,(float)
	    4830.,(float).014126,(float).4985,(float).0096621,(float)
	    2.3197e-5,(float).011163,(float)-2.0765e-5,(float)4830.,(float)
	    4840.,(float).023704,(float).35541,(float).0055808,(float)
	    -1.0952e-6,(float).0068854,(float)-1.3359e-5,(float)4840.,(float)
	    4850.,(float).032166,(float).23632,(float)-2.3289e-4,(float)
	    7.945e-6,(float).0017163,(float)2.8957e-6,(float)4850.,(float)
	    4860.,(float).026468,(float).24812,(float).008326,(float)
	    8.9107e-6,(float).010638,(float)-2.4848e-5,(float)4860.,(float)
	    4870.,(float).012607,(float).28278,(float).014341,(float)
	    -1.7161e-5,(float).017744,(float)-4.3433e-5,(float)4870.,(float)
	    4880.,(float).0019484,(float).13452,(float).014299,(float)
	    -5.8644e-5,(float).012622,(float)-5.8891e-5,(float)4880.,(float)
	    4890.,(float).019835,(float).43138,(float).016502,(float)
	    -3.7834e-5,(float).015564,(float)-5.3844e-5,(float)4890.,(float)
	    4900.,(float).031673,(float).23676,(float).018724,(float)
	    -6.2795e-5,(float).017963,(float)-6.7476e-5,(float)4900.,(float)
	    4910.,(float).020383,(float).1686,(float).02082,(float)-9.2373e-5,
	    (float).018807,(float)-8.5676e-5,(float)4910.,(float)4920.,(float)
	    .034221,(float).37729,(float).014636,(float)9.8132e-6,(float)
	    .015033,(float)-3.2546e-5,(float)4920.,(float)4930.,(float)
	    .041381,(float).17341,(float).004227,(float)2.6778e-5,(float)
	    .0073624,(float)-6.193e-6,(float)4930.,(float)4940.,(float)
	    .050322,(float).24574,(float).01506,(float)-6.0731e-5,(float)
	    .01171,(float)-4.9448e-5,(float)4940.,(float)4950.,(float).11355,(
	    float).43168,(float).012506,(float)-2.3567e-5,(float).010508,(
	    float)-3.2884e-5,(float)4950.,(float)4960.,(float).073646,(float)
	    .28063,(float).01551,(float)-6.3821e-5,(float).012136,(float)
	    -5.1762e-5,(float)4960.,(float)4970.,(float).0021754,(float).2329,
	    (float).025115,(float)1.3402e-6,(float).023138,(float)-4.9298e-5,(
	    float)4970.,(float)4980.,(float).20462,(float).37006,(float)
	    .013871,(float)-5.7344e-5,(float).011137,(float)-4.9726e-5,(float)
	    4980.,(float)4990.,(float).15479,(float).14505,(float).012101,(
	    float)-5.5517e-5,(float).011913,(float)-3.8561e-5,(float)4990.,(
	    float)5e3,(float).1627,(float).14157,(float).012656,(float)
	    -4.9554e-5,(float).01273,(float)-4.7918e-5,(float)5e3,(float)
	    5010.,(float).20283,(float).16538,(float).0098358,(float)
	    -4.1905e-5,(float).0091599,(float)-3.3109e-5,(float)5010.,(float)
	    5020.,(float).32847,(float).20501,(float).010455,(float)
	    -3.0987e-5,(float).011634,(float)-2.2216e-5,(float)5020.,(float)
	    5030.,(float).4088,(float).1568,(float).0097922,(float)-4.2397e-5,
	    (float).0098076,(float)-3.8651e-5,(float)5030.,(float)5040.,(
	    float).27012,(float).21951,(float).012867,(float)-1.8768e-6,(
	    float).012462,(float)-2.155e-5,(float)5040.,(float)5050.,(float)
	    .5761,(float).16605,(float).0061804,(float)-3.0533e-5,(float)
	    .0069772,(float)-1.7735e-5,(float)5050.,(float)5060. };

    integer i__;

/*<       real a(8) >*/
/*<       real acr(8,256) >*/
/*<       integer inu,j,k,i >*/

/*     WATER VAPOR (2500 - 5040 CM-1) */

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
} /* wava1_ */

#ifdef __cplusplus
	}
#endif
