/* DICA1.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    dica1(a,inu) >*/
/* Subroutine */ int dica1_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)1.1446e-5,(
	    float).0020117,(float)-.0041334,(float)3.2304e-6,(float)-.0069982,
	    (float)9.0084e-6,(float)2500.,(float)2510.,(float)1.9234e-5,(
	    float).0019311,(float)-.0017326,(float)-5.8646e-6,(float)
	    -.0045311,(float)-6.0352e-7,(float)2510.,(float)2520.,(float)
	    9.202e-6,(float).0017952,(float).0034861,(float)-2.526e-5,(float)
	    5.8777e-4,(float)-2.0915e-5,(float)2520.,(float)2530.,(float)
	    1.6374e-6,(float).0018281,(float).010418,(float)-4.3535e-5,(float)
	    .0074404,(float)-4.1207e-5,(float)2530.,(float)2540.,(float)
	    1.9079e-7,(float).0010034,(float).011048,(float)-9.739e-6,(float)
	    .010458,(float)-3.7165e-5,(float)2540.,(float)2550.,(float)
	    3.4315e-9,(float)2.4238e-5,(float).0054443,(float)-3.5215e-5,(
	    float).00216,(float)-2.8602e-5,(float)2550.,(float)2560.,(float)
	    8.2488e-9,(float)1.2211e-4,(float).026474,(float)-1.1937e-4,(
	    float).023595,(float)-1.1358e-4,(float)2560.,(float)2570.,(float)
	    6.4587e-7,(float).0014539,(float).017175,(float)-7.7312e-5,(float)
	    .015125,(float)-7.6621e-5,(float)2570.,(float)2580.,(float)
	    5.4268e-6,(float).0015777,(float).0078126,(float)-4.2151e-5,(
	    float).0051843,(float)-3.9096e-5,(float)2580.,(float)2590.,(float)
	    2.0993e-5,(float).0024666,(float).0015156,(float)-1.4469e-5,(
	    float).0014551,(float)-1.2874e-5,(float)2590.,(float)2600.,(float)
	    3.1892e-5,(float).0032261,(float)-.0024271,(float)1.4275e-6,(
	    float)-.0024514,(float)2.5977e-6,(float)2600.,(float)2610.,(float)
	    1.1919e-5,(float).0029607,(float)-.0037722,(float)7.2919e-6,(
	    float)-.004064,(float)7.442e-6,(float)2610.,(float)2620.,(float)
	    3.5396e-5,(float).0031915,(float)-.0024421,(float)-5.4413e-7,(
	    float)-.0029857,(float)7.7772e-7,(float)2620.,(float)2630.,(float)
	    2.2174e-5,(float).0031839,(float).002243,(float)-1.6625e-5,(float)
	    .0017446,(float)-1.7477e-5,(float)2630.,(float)2640.,(float)
	    4.9654e-6,(float).0031706,(float).0096026,(float)-4.1529e-5,(
	    float).0087311,(float)-4.451e-5,(float)2640.,(float)2650.,(float)
	    5.9768e-7,(float).0013719,(float).0076702,(float)1.3005e-5,(float)
	    .0090078,(float)-2.4905e-5,(float)2650.,(float)2660.,(float)
	    1.3394e-7,(float)3.2465e-4,(float).0037114,(float)-2.6108e-5,(
	    float)9.3326e-4,(float)-2.2404e-5,(float)2660.,(float)2670.,(
	    float)9.101e-9,(float)7.1741e-5,(float).010072,(float)-5.3553e-5,(
	    float).0068917,(float)-4.7305e-5,(float)2670.,(float)2680.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2680.,
	    (float)2690.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2690.,(float)2700.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2700.,(float)2710.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2710.,(
	    float)2720.,(float)3.0498e-7,(float).0012211,(float).011385,(
	    float)-5.7015e-5,(float).008676,(float)-5.3344e-5,(float)2720.,(
	    float)2730.,(float)2.3044e-6,(float).0018735,(float).0031625,(
	    float)-2.3857e-5,(float)4.2583e-4,(float)-2.0332e-5,(float)2730.,(
	    float)2740.,(float)4.587e-6,(float).0020082,(float)-.0021166,(
	    float)-4.3444e-6,(float)-.0048776,(float)8.714e-7,(float)2740.,(
	    float)2750.,(float)1.8151e-6,(float).001778,(float)-.0043591,(
	    float)3.9928e-6,(float)-.0074161,(float)1.0271e-5,(float)2750.,(
	    float)2760.,(float)4.3155e-6,(float).0021845,(float)-.0035667,(
	    float)1.0325e-6,(float)-.0064484,(float)6.9079e-6,(float)2760.,(
	    float)2770.,(float)3.5601e-6,(float).0015853,(float)-2.4397e-4,(
	    float)-1.1623e-5,(float)-.0030932,(float)-6.6664e-6,(float)2770.,(
	    float)2780.,(float)1.4023e-6,(float).0017421,(float).0052527,(
	    float)-3.1584e-5,(float).0022672,(float)-2.6978e-5,(float)2780.,(
	    float)2790.,(float)1.9517e-7,(float).0010843,(float).011752,(
	    float)-5.0614e-5,(float).0090578,(float)-5.0929e-5,(float)2790.,(
	    float)2800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2800.,(float)2810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2810.,(float)2820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2820.,(
	    float)2830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2830.,(float)2840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2840.,(float)2850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2850.,(
	    float)2860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2860.,(float)2870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2870.,(float)2880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2880.,(
	    float)2890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2890.,(float)2900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2900.,(float)2910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2910.,(
	    float)2920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2920.,(float)2930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2930.,(float)2940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2940.,(
	    float)2950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2950.,(float)2960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2960.,(float)2970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)2970.,(
	    float)2980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)2980.,(float)2990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)2990.,(float)3e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3e3,(
	    float)3010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3010.,(float)3020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3020.,(float)3030.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3030.,(
	    float)3040.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3040.,(float)3050.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3050.,(float)3060.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3060.,(
	    float)3070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3070.,(float)3080.,(float)3.3052e-6,(float)
	    .058651,(float).034989,(float)-1.529e-4,(float).031987,(float)
	    -1.4721e-4,(float)3080.,(float)3090.,(float)4.1503e-5,(float)
	    .20207,(float).026206,(float)-1.1436e-4,(float).023672,(float)
	    -1.1213e-4,(float)3090.,(float)3100.,(float)8.7906e-5,(float)
	    .21783,(float).018422,(float)-8.5123e-5,(float).015304,(float)
	    -7.985e-5,(float)3100.,(float)3110.,(float)2.9489e-5,(float)
	    .12859,(float).014066,(float)-6.9404e-5,(float).011044,(float)
	    -6.3254e-5,(float)3110.,(float)3120.,(float)3.1118e-5,(float)
	    .2981,(float).025033,(float)-1.1155e-4,(float).022143,(float)
	    -1.0719e-4,(float)3120.,(float)3130.,(float)1.4832e-4,(float)
	    .7106,(float).018309,(float)-7.0285e-5,(float).015857,(float)
	    -7.3742e-5,(float)3130.,(float)3140.,(float)5.0912e-4,(float)
	    .39631,(float).016927,(float)-7.7076e-5,(float).014306,(float)
	    -7.4481e-5,(float)3140.,(float)3150.,(float).0018569,(float)
	    .28375,(float).0076498,(float)-3.6249e-5,(float).0067089,(float)
	    -3.6791e-5,(float)3150.,(float)3160.,(float).0023217,(float).3497,
	    (float).0010351,(float)-1.0688e-5,(float)9.4905e-4,(float)
	    -6.7349e-6,(float)3160.,(float)3170.,(float)4.761e-4,(float)
	    .41947,(float).0019347,(float)1.6317e-5,(float).0033594,(float)
	    -7.9441e-6,(float)3170.,(float)3180.,(float)4.6778e-4,(float)
	    .77616,(float)7.9129e-4,(float)2.3675e-5,(float)9.1006e-4,(float)
	    4.8023e-6,(float)3180.,(float)3190.,(float).0030862,(float).25667,
	    (float)-.0014396,(float)-4.1338e-6,(float)-.0027351,(float)
	    6.4924e-6,(float)3190.,(float)3200.,(float).0034062,(float).18499,
	    (float).003259,(float)-2.4779e-5,(float)2.1425e-4,(float)
	    -1.9664e-5,(float)3200.,(float)3210.,(float).0014366,(float)
	    .17452,(float).010436,(float)-5.221e-5,(float).0077713,(float)
	    -4.9107e-5,(float)3210.,(float)3220.,(float)2.6116e-4,(float)
	    .16068,(float).020114,(float)-8.9663e-5,(float).017964,(float)
	    -8.8666e-5,(float)3220.,(float)3230.,(float)3.408e-5,(float)
	    .26237,(float).031854,(float)-1.3724e-4,(float).029565,(float)
	    -1.3533e-4,(float)3230.,(float)3240.,(float)3.8044e-5,(float)
	    .18154,(float).023462,(float)-1.0477e-4,(float).020484,(float)
	    -1.0031e-4,(float)3240.,(float)3250.,(float)2.3642e-5,(float)
	    .12587,(float).01793,(float)-8.3993e-5,(float).014725,(float)
	    -7.8063e-5,(float)3250.,(float)3260.,(float)1.3555e-6,(float)
	    .0021856,(float).0043607,(float)-2.9254e-5,(float).0012372,(float)
	    -2.4119e-5,(float)3260.,(float)3270.,(float)3.5631e-4,(float)
	    .86373,(float).015491,(float)-6.0741e-5,(float).014051,(float)
	    -6.1126e-5,(float)3270.,(float)3280.,(float)3.3068e-4,(float)
	    .41597,(float).01708,(float)-4.5243e-5,(float).018249,(float)
	    -6.1327e-5,(float)3280.,(float)3290.,(float).001398,(float).43415,
	    (float).020779,(float)-8.71e-5,(float).019046,(float)-8.4016e-5,(
	    float)3290.,(float)3300.,(float).0054038,(float).37316,(float)
	    .013012,(float)-5.4513e-5,(float).014582,(float)-6.1135e-5,(float)
	    3300.,(float)3310.,(float).011865,(float).37215,(float).005333,(
	    float)-2.4891e-5,(float).0077872,(float)-2.3154e-5,(float)3310.,(
	    float)3320.,(float).0081717,(float).45864,(float)7.0369e-4,(float)
	    -2.5399e-6,(float).0032045,(float)-2.1463e-6,(float)3320.,(float)
	    3330.,(float).0067313,(float).55963,(float)-.0020245,(float)
	    2.6702e-6,(float)-.0024299,(float)1.777e-5,(float)3330.,(float)
	    3340.,(float).013254,(float).74729,(float)-.001815,(float)
	    5.4501e-6,(float).0013181,(float)-4.3636e-7,(float)3340.,(float)
	    3350.,(float).036823,(float).36421,(float)-2.01e-4,(float)
	    -5.8593e-6,(float).0013324,(float)-2.3987e-6,(float)3350.,(float)
	    3360.,(float).035439,(float).40306,(float).0054434,(float)
	    -2.6546e-5,(float).0067677,(float)-2.8288e-5,(float)3360.,(float)
	    3370.,(float).0090957,(float).48115,(float).014512,(float)
	    -6.1088e-5,(float).016771,(float)-6.5673e-5,(float)3370.,(float)
	    3380.,(float).0015199,(float).65754,(float).023451,(float)-8.6e-5,
	    (float).024073,(float)-8.9238e-5,(float)3380.,(float)3390.,(float)
	    1.3838e-4,(float).96599,(float).027438,(float)-3.7378e-5,(float)
	    .026069,(float)-7.2896e-5,(float)3390.,(float)3400.,(float)
	    3.1321e-4,(float).32432,(float).014691,(float)-4.5546e-5,(float)
	    .01786,(float)-4.056e-5,(float)3400.,(float)3410.,(float)
	    9.6318e-4,(float).34642,(float).017708,(float)-6.6986e-5,(float)
	    .020408,(float)-5.3348e-5,(float)3410.,(float)3420.,(float)
	    6.1782e-4,(float).30724,(float).02599,(float)-8.9579e-5,(float)
	    .025999,(float)-9.8228e-5,(float)3420.,(float)3430.,(float)
	    4.0506e-4,(float).30755,(float).038698,(float)-1.6162e-4,(float)
	    .035406,(float)-1.5721e-4,(float)3430.,(float)3440.,(float)
	    .0011183,(float).2365,(float).034497,(float)-1.4952e-4,(float)
	    .032465,(float)-1.4642e-4,(float)3440.,(float)3450.,(float)
	    .0018653,(float).34796,(float).029814,(float)-1.2999e-4,(float)
	    .026883,(float)-1.245e-4,(float)3450.,(float)3460.,(float)
	    .0035542,(float).40556,(float).017369,(float)-5.7102e-5,(float)
	    .019546,(float)-4.6731e-5,(float)3460.,(float)3470.,(float)
	    .0090605,(float).50199,(float).012281,(float)2.333e-7,(float)
	    .01992,(float)2.7162e-5,(float)3470.,(float)3480.,(float).008421,(
	    float).7135,(float).011986,(float)7.348e-5,(float).024636,(float)
	    8.1574e-5,(float)3480.,(float)3490.,(float).0060886,(float).9612,(
	    float).023574,(float)1.0867e-4,(float).044783,(float)-7.2014e-5,(
	    float)3490.,(float)3500.,(float).035622,(float)1.6535,(float)
	    .012077,(float)1.3532e-4,(float).02212,(float)8.0854e-5,(float)
	    3500.,(float)3510.,(float).078238,(float)1.3688,(float).016785,(
	    float)1.4511e-4,(float).032327,(float)-1.2296e-5,(float)3510.,(
	    float)3520.,(float).17292,(float)1.6314,(float).028411,(float)
	    -4.0866e-6,(float).033279,(float)-8.391e-5,(float)3520.,(float)
	    3530.,(float).58094,(float)1.4177,(float).030478,(float)-9.228e-5,
	    (float).032074,(float)-1.1676e-4,(float)3530.,(float)3540.,(float)
	    1.9973,(float)1.0662,(float).026332,(float)-1.0563e-4,(float)
	    .027668,(float)-1.086e-4,(float)3540.,(float)3550.,(float)5.8904,(
	    float).85364,(float).020626,(float)-8.7922e-5,(float).022034,(
	    float)-8.7919e-5,(float)3550.,(float)3560.,(float)16.608,(float)
	    .82681,(float).016296,(float)-7.197e-5,(float).016508,(float)
	    -6.8914e-5,(float)3560.,(float)3570.,(float)33.94,(float).67293,(
	    float).011756,(float)-5.5546e-5,(float).012667,(float)-5.0727e-5,(
	    float)3570.,(float)3580.,(float)105.89,(float).40519,(float)
	    .0053354,(float)-3.05e-5,(float).00642,(float)-2.7269e-5,(float)
	    3580.,(float)3590.,(float)286.58,(float).37875,(float)3.1591e-4,(
	    float)-7.6033e-6,(float).0017571,(float)-6.9331e-6,(float)3590.,(
	    float)3600.,(float)258.81,(float).31632,(float)-.0031299,(float)
	    5.3577e-6,(float)-.0014554,(float)1.6811e-5,(float)3600.,(float)
	    3610.,(float)153.98,(float).23899,(float)-.0042487,(float)
	    5.6295e-6,(float)-.0038259,(float)2.864e-5,(float)3610.,(float)
	    3620.,(float)381.54,(float).2369,(float)-.0021079,(float)
	    -4.2102e-6,(float)-.0046108,(float)4.6494e-6,(float)3620.,(float)
	    3630.,(float)189.08,(float).24104,(float).003487,(float)
	    -2.4525e-5,(float).0012111,(float)-1.869e-5,(float)3630.,(float)
	    3640.,(float)22.631,(float).27596,(float).013843,(float)
	    -6.0817e-5,(float).013983,(float)-5.5436e-5,(float)3640.,(float)
	    3650.,(float).789,(float).45834,(float).0288,(float)-9.2261e-5,(
	    float).033526,(float)-1.1634e-4,(float)3650.,(float)3660.,(float)
	    3.5516,(float).24747,(float).021606,(float)-9.1483e-5,(float)
	    .025091,(float)-7.7825e-5,(float)3660.,(float)3670.,(float)26.382,
	    (float).26344,(float).013128,(float)-5.9351e-5,(float).015813,(
	    float)-4.5315e-5,(float)3670.,(float)3680.,(float)114.51,(float)
	    .25517,(float).0061726,(float)-3.2903e-5,(float).0088361,(float)
	    -1.856e-5,(float)3680.,(float)3690.,(float)375.87,(float).29958,(
	    float)6.1303e-4,(float)-1.0823e-5,(float).0025691,(float)
	    2.5921e-6,(float)3690.,(float)3700.,(float)399.12,(float).32406,(
	    float)-.0024055,(float)3.4271e-6,(float)8.3356e-4,(float)
	    1.4624e-5,(float)3700.,(float)3710.,(float)209.7,(float).49024,(
	    float)-.0029825,(float)1.1251e-5,(float)9.8088e-4,(float)
	    1.7825e-5,(float)3710.,(float)3720.,(float)651.86,(float).43325,(
	    float)-.0025584,(float)-7.4576e-7,(float)-.0013717,(float)
	    1.1441e-5,(float)3720.,(float)3730.,(float)360.94,(float).49364,(
	    float).0033219,(float)-1.9819e-5,(float).0045993,(float)
	    -1.8704e-5,(float)3730.,(float)3740.,(float)57.048,(float).71411,(
	    float).013964,(float)-5.8323e-5,(float).015441,(float)-6.5072e-5,(
	    float)3740.,(float)3750.,(float)1.6419,(float).94961,(float)
	    .031167,(float)-1.2796e-4,(float).03253,(float)-1.3294e-4,(float)
	    3750.,(float)3760.,(float).0072097,(float).71242,(float).053044,(
	    float)-2.1219e-4,(float).055695,(float)-2.2364e-4,(float)3760.,(
	    float)3770.,(float)3.2929e-4,(float).16488,(float).043688,(float)
	    -1.8453e-4,(float).042346,(float)-1.8165e-4,(float)3770.,(float)
	    3780.,(float).0014501,(float).17127,(float).036748,(float)
	    -1.5788e-4,(float).035331,(float)-1.524e-4,(float)3780.,(float)
	    3790.,(float).0047585,(float).19488,(float).031054,(float)
	    -1.364e-4,(float).028516,(float)-1.3041e-4,(float)3790.,(float)
	    3800.,(float).0056312,(float).2178,(float).027563,(float)
	    -1.2299e-4,(float).02536,(float)-1.1514e-4,(float)3800.,(float)
	    3810.,(float).0025663,(float).23975,(float).026455,(float)
	    -1.1697e-4,(float).025771,(float)-1.0379e-4,(float)3810.,(float)
	    3820.,(float).0073473,(float).2611,(float).028264,(float)
	    -1.2512e-4,(float).026543,(float)-1.1735e-4,(float)3820.,(float)
	    3830.,(float).0041625,(float).3064,(float).0335,(float)-1.4328e-4,
	    (float).032396,(float)-1.4197e-4,(float)3830.,(float)3840.,(float)
	    6.2707e-4,(float).49963,(float).042723,(float)-1.7265e-4,(float)
	    .041198,(float)-1.7864e-4,(float)3840.,(float)3850.,(float)
	    5.2281e-5,(float).443,(float).041789,(float)-1.3546e-4,(float)
	    .041381,(float)-1.6812e-4,(float)3850.,(float)3860.,(float)
	    1.1349e-4,(float).48522,(float).040194,(float)-1.3982e-4,(float)
	    .03909,(float)-1.6958e-4,(float)3860.,(float)3870.,(float)
	    1.1733e-4,(float).49615,(float).044541,(float)-1.6207e-4,(float)
	    .043254,(float)-1.8688e-4,(float)3870.,(float)3880.,(float)
	    2.1994e-5,(float).36694,(float).051435,(float)-1.7841e-4,(float)
	    .051058,(float)-2.162e-4,(float)3880.,(float)3890.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3890.,(
	    float)3900.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3900.,(float)3910.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)3910.,(float)3920.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)3920.,(
	    float)3930.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)3930.,(float)3940.,(float)1.0712e-5,(float).11242,
	    (float).038222,(float)-1.6405e-4,(float).035612,(float)-1.6058e-4,
	    (float)3940.,(float)3950.,(float)2.226e-5,(float).088337,(float)
	    .032098,(float)-1.4078e-4,(float).029142,(float)-1.356e-4,(float)
	    3950.,(float)3960.,(float)6.0145e-5,(float).1215,(float).02614,(
	    float)-1.1441e-4,(float).023109,(float)-1.101e-4,(float)3960.,(
	    float)3970.,(float)1.1812e-4,(float).5338,(float).023839,(float)
	    -9.6178e-5,(float).023506,(float)-1.0676e-4,(float)3970.,(float)
	    3980.,(float)6.12e-5,(float).32611,(float).020895,(float)
	    -7.6119e-5,(float).020369,(float)-8.7589e-5,(float)3980.,(float)
	    3990.,(float)3.019e-4,(float).84191,(float).024861,(float)
	    -9.352e-5,(float).024385,(float)-1.0546e-4,(float)3990.,(float)
	    4e3,(float).0013735,(float).6758,(float).014099,(float)-6.4565e-5,
	    (float).011933,(float)-5.7363e-5,(float)4e3,(float)4010.,(float)
	    .0017095,(float).28901,(float).013726,(float)-6.7497e-5,(float)
	    .010543,(float)-6.096e-5,(float)4010.,(float)4020.,(float)
	    .0020687,(float).45821,(float).021277,(float)-8.6802e-5,(float)
	    .020845,(float)-9.0012e-5,(float)4020.,(float)4030.,(float)0.,(
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
	    float)0.,(float)0.,(float)0.,(float)4410.,(float)4420.,(float)
	    2.629e-5,(float).099522,(float)-.0026652,(float)-2.6042e-6,(float)
	    -.0055447,(float)3.2186e-6,(float)4420.,(float)4430.,(float)
	    8.0883e-5,(float).18978,(float)5.7466e-4,(float)-1.4607e-5,(float)
	    -.0025665,(float)-8.7838e-6,(float)4430.,(float)4440.,(float)
	    5.526e-5,(float).18005,(float).0065537,(float)-3.7323e-5,(float)
	    .0036814,(float)-3.3174e-5,(float)4440.,(float)4450.,(float)
	    8.255e-6,(float).05991,(float).012867,(float)-6.4515e-5,(float)
	    .0097575,(float)-5.8512e-5,(float)4450.,(float)4460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4460.,(
	    float)4470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4470.,(float)4480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4480.,(float)4490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4490.,(
	    float)4500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4500.,(float)4510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)4510.,(float)4520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)4520.,(
	    float)4530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)4530.,(float)4540.,(float)5.6586e-6,(float)
	    .058006,(float).021852,(float)-1.003e-4,(float).018901,(float)
	    -9.4764e-5,(float)4540.,(float)4550.,(float)6.0995e-5,(float)
	    .17176,(float).013472,(float)-6.3857e-5,(float).010919,(float)
	    -6.1333e-5,(float)4550.,(float)4560.,(float)1.6173e-4,(float)
	    .1827,(float).0055264,(float)-3.3401e-5,(float).0025266,(float)
	    -2.8677e-5,(float)4560.,(float)4570.,(float)1.342e-4,(float)
	    .18669,(float)8.6662e-5,(float)-1.2871e-5,(float)-.0031886,(float)
	    -6.3395e-6,(float)4570.,(float)4580.,(float)7.7677e-6,(float)
	    .035973,(float)-.0017804,(float)3.5792e-6,(float)-.004378,(float)
	    4.7097e-6,(float)4580.,(float)4590.,(float)2.1446e-4,(float)
	    .52918,(float)-.0026414,(float)1.9171e-8,(float)-.0051868,(float)
	    3.1486e-6,(float)4590.,(float)4600.,(float)4.9732e-4,(float)
	    .19678,(float)-.0015023,(float)-6.4944e-6,(float)-.0045248,(float)
	    -6.0552e-7,(float)4600.,(float)4610.,(float)5.7844e-4,(float)
	    .21566,(float).0036076,(float)-2.5498e-5,(float)6.874e-4,(float)
	    -2.1166e-5,(float)4610.,(float)4620.,(float)1.836e-4,(float)
	    .19472,(float).010909,(float)-4.5759e-5,(float).0094535,(float)
	    -5.3705e-5,(float)4620.,(float)4630.,(float)2.1722e-5,(float)
	    .066168,(float).010212,(float)2.4083e-5,(float).016411,(float)
	    -6.7231e-5,(float)4630.,(float)4640.,(float)1.5836e-5,(float)
	    .0025231,(float)-.0037387,(float)1.6847e-6,(float)-.0066855,(
	    float)7.7758e-6,(float)4640.,(float)4650.,(float)2.8194e-5,(float)
	    .055835,(float).011719,(float)7.8341e-5,(float).028162,(float)
	    -6.5915e-5,(float)4650.,(float)4660.,(float)2.9246e-5,(float)
	    .15846,(float).033601,(float)-1.2318e-4,(float).031988,(float)
	    -1.4227e-4,(float)4660.,(float)4670.,(float)6.866e-5,(float)
	    .23768,(float).030956,(float)-1.3481e-4,(float).028087,(float)
	    -1.3066e-4,(float)4670.,(float)4680.,(float)2.5776e-5,(float)
	    .14238,(float).026879,(float)-1.1219e-4,(float).025142,(float)
	    -1.1785e-4,(float)4680.,(float)4690.,(float)6.7605e-5,(float)
	    .28782,(float).028103,(float)-9.7067e-5,(float).028478,(float)
	    -1.1197e-4,(float)4690.,(float)4700.,(float)9.4449e-5,(float)
	    .58934,(float).03528,(float)-9.4266e-5,(float).037047,(float)
	    -1.3314e-4,(float)4700.,(float)4710.,(float)2.1985e-4,(float)
	    .94207,(float).026856,(float)1.5457e-5,(float).034398,(float)
	    -7.9603e-5,(float)4710.,(float)4720.,(float)5.9268e-4,(float)
	    .92654,(float).019629,(float)7.9413e-5,(float).02922,(float)
	    -3.244e-5,(float)4720.,(float)4730.,(float).001207,(float).99725,(
	    float).019414,(float)1.0014e-4,(float).026778,(float)-2.2067e-5,(
	    float)4730.,(float)4740.,(float).0022806,(float)1.2176,(float)
	    .028561,(float)-2.1574e-5,(float).030485,(float)-7.5567e-5,(float)
	    4740.,(float)4750.,(float).0043327,(float)1.3486,(float).028595,(
	    float)-6.6405e-5,(float).027851,(float)-7.4726e-5,(float)4750.,(
	    float)4760.,(float).0077428,(float)1.4486,(float).030758,(float)
	    -1.1612e-4,(float).029991,(float)-1.2747e-4,(float)4760.,(float)
	    4770.,(float).025636,(float)1.0146,(float).025412,(float)
	    -1.0692e-4,(float).02508,(float)-1.0479e-4,(float)4770.,(float)
	    4780.,(float).071004,(float).80049,(float).019034,(float)
	    -8.2075e-5,(float).019591,(float)-8.1415e-5,(float)4780.,(float)
	    4790.,(float).12168,(float).79844,(float).014488,(float)
	    -6.4424e-5,(float).0152,(float)-5.8903e-5,(float)4790.,(float)
	    4800.,(float).076746,(float)1.2201,(float).015267,(float)
	    -5.7662e-5,(float).014334,(float)-5.5273e-5,(float)4800.,(float)
	    4810.,(float).25656,(float).72698,(float).012794,(float)
	    -6.0697e-5,(float).011343,(float)-5.448e-5,(float)4810.,(float)
	    4820.,(float).71618,(float).47943,(float).0075299,(float)
	    -3.4485e-5,(float).0086269,(float)-4.1288e-5,(float)4820.,(float)
	    4830.,(float)1.9439,(float).28552,(float)6.0337e-4,(float)
	    -1.0585e-5,(float).0017705,(float)-3.3465e-6,(float)4830.,(float)
	    4840.,(float)2.2409,(float).24124,(float)-.0032243,(float)
	    7.7209e-7,(float)-.0036357,(float)1.7134e-5,(float)4840.,(float)
	    4850.,(float)1.023,(float).23661,(float)-.0044285,(float)
	    5.6593e-6,(float)-.0054571,(float)1.8603e-5,(float)4850.,(float)
	    4860.,(float)2.9838,(float).24972,(float)-.0026005,(float)
	    -2.2445e-6,(float)-.0048507,(float)6.708e-6,(float)4860.,(float)
	    4870.,(float)1.7331,(float).24569,(float).002513,(float)
	    -2.0912e-5,(float)9.7879e-5,(float)-1.4658e-5,(float)4870.,(float)
	    4880.,(float).23009,(float).24263,(float).012134,(float)
	    -5.5883e-5,(float).011873,(float)-4.6579e-5,(float)4880.,(float)
	    4890.,(float).0073766,(float).50576,(float).021012,(float)
	    1.3113e-5,(float).033128,(float)-7.4753e-5,(float)4890.,(float)
	    4900.,(float).0053301,(float)1.1415,(float).02315,(float)
	    8.1743e-5,(float).032613,(float)-4.9011e-5,(float)4900.,(float)
	    4910.,(float).019881,(float)1.1163,(float).030658,(float)
	    -8.9121e-5,(float).031493,(float)-1.049e-4,(float)4910.,(float)
	    4920.,(float).13274,(float).8018,(float).023959,(float)-9.6864e-5,
	    (float).026465,(float)-1.057e-4,(float)4920.,(float)4930.,(float)
	    .54198,(float).58029,(float).016095,(float)-6.7361e-5,(float)
	    .019209,(float)-7.4544e-5,(float)4930.,(float)4940.,(float)2.3931,
	    (float).43634,(float).0083244,(float)-3.8869e-5,(float).01091,(
	    float)-4.0486e-5,(float)4940.,(float)4950.,(float)6.364,(float)
	    .38463,(float).0024225,(float)-1.7014e-5,(float).0051156,(float)
	    -1.3677e-5,(float)4950.,(float)4960.,(float)11.174,(float).37051,(
	    float)-.0020924,(float)-1.8861e-6,(float)1.9797e-4,(float)
	    8.4038e-6,(float)4960.,(float)4970.,(float)4.7425,(float).4397,(
	    float)-.0023967,(float)1.1771e-5,(float).0015888,(float)1.1027e-5,
	    (float)4970.,(float)4980.,(float)13.215,(float).42301,(float)
	    -.002886,(float)4.5346e-6,(float)-.0018435,(float)1.2941e-5,(
	    float)4980.,(float)4990.,(float)10.682,(float).28898,(float)
	    9.1279e-4,(float)-1.3741e-5,(float)2.9048e-4,(float)-3.723e-6,(
	    float)4990.,(float)5e3,(float)1.9873,(float).27401,(float)
	    .0096347,(float)-4.5675e-5,(float).0089294,(float)-4.3606e-5,(
	    float)5e3,(float)5010.,(float).025245,(float).25842,(float)
	    .027802,(float)-1.1266e-4,(float).028517,(float)-1.1312e-4,(float)
	    5010.,(float)5020.,(float).0010773,(float).39566,(float).019467,(
	    float)-9.0504e-6,(float).021521,(float)-4.662e-5,(float)5020.,(
	    float)5030.,(float).0026445,(float).46517,(float).022864,(float)
	    -5.1981e-6,(float).021923,(float)-4.8242e-5,(float)5030.,(float)
	    5040.,(float).0089353,(float).34218,(float).025304,(float)
	    -8.7147e-5,(float).020721,(float)-6.3342e-5,(float)5040.,(float)
	    5050.,(float).066176,(float).21908,(float).017982,(float)
	    -8.0841e-5,(float).017736,(float)-7.414e-5,(float)5050.,(float)
	    5060. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<       integer inu,j,k,i >*/
/*     carbon dioxide (2500 - 5050 cm-1) */

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
} /* dica1_ */

#ifdef __cplusplus
	}
#endif
