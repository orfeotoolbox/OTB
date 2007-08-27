/* DICA2.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    dica2(a,inu) >*/
/* Subroutine */ int dica2_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float).37011,(float)
	    .18132,(float).0098385,(float)-4.992e-5,(float).0096965,(float)
	    -3.9497e-5,(float)5060.,(float)5070.,(float)1.7202,(float).2316,(
	    float).0029954,(float)-2.2435e-5,(float).0029757,(float)
	    -9.2488e-6,(float)5070.,(float)5080.,(float)3.3606,(float).25416,(
	    float)-.0016977,(float)-4.0846e-6,(float)-.0013656,(float)
	    1.1658e-5,(float)5080.,(float)5090.,(float)1.8282,(float).30223,(
	    float)-.0032772,(float)9.4805e-6,(float)-5.335e-4,(float)
	    2.1209e-5,(float)5090.,(float)5100.,(float)2.9024,(float).38026,(
	    float)-.0030989,(float)7.2616e-6,(float)-.001377,(float)1.615e-5,(
	    float)5100.,(float)5110.,(float)4.2563,(float).41711,(float)
	    -1.4453e-4,(float)-8.0697e-6,(float)4.2978e-4,(float)-2.7285e-6,(
	    float)5110.,(float)5120.,(float)1.1106,(float).56558,(float)
	    .0071956,(float)-3.6594e-5,(float).0079066,(float)-3.7847e-5,(
	    float)5120.,(float)5130.,(float).30634,(float).97715,(float)
	    .015099,(float)-6.7211e-5,(float).015152,(float)-6.5294e-5,(float)
	    5130.,(float)5140.,(float).11213,(float).77801,(float).019757,(
	    float)-8.6279e-5,(float).020188,(float)-8.4289e-5,(float)5140.,(
	    float)5150.,(float).014584,(float)1.0474,(float).030585,(float)
	    -1.2981e-4,(float).030136,(float)-1.2999e-4,(float)5150.,(float)
	    5160.,(float).001482,(float).96349,(float).039762,(float)
	    -1.6303e-4,(float).039329,(float)-1.6879e-4,(float)5160.,(float)
	    5170.,(float)7.185e-5,(float).75521,(float).050597,(float)
	    -1.9183e-4,(float).049189,(float)-2.096e-4,(float)5170.,(float)
	    5180.,(float)2.9909e-5,(float).14908,(float).03608,(float)
	    -1.1815e-4,(float).034793,(float)-1.453e-4,(float)5180.,(float)
	    5190.,(float)1.2504e-4,(float).18326,(float).032726,(float)
	    -1.4282e-4,(float).029813,(float)-1.3805e-4,(float)5190.,(float)
	    5200.,(float)2.1512e-4,(float).19978,(float).02843,(float)
	    -1.2662e-4,(float).025525,(float)-1.2103e-4,(float)5200.,(float)
	    5210.,(float)9.6169e-5,(float).20142,(float).026447,(float)
	    -1.1912e-4,(float).023511,(float)-1.1308e-4,(float)5210.,(float)
	    5220.,(float)1.9738e-4,(float).21048,(float).02716,(float)
	    -1.2182e-4,(float).024213,(float)-1.1582e-4,(float)5220.,(float)
	    5230.,(float)2.6384e-4,(float).44256,(float).028679,(float)
	    -1.144e-4,(float).024161,(float)-1.056e-4,(float)5230.,(float)
	    5240.,(float)3.6492e-4,(float).82665,(float).01703,(float)
	    -5.0383e-5,(float).015395,(float)-5.7885e-5,(float)5240.,(float)
	    5250.,(float)1.5039e-4,(float).27488,(float).013098,(float)
	    -6.1988e-5,(float).010623,(float)-5.4594e-5,(float)5250.,(float)
	    5260.,(float)2.4449e-4,(float).45805,(float).01877,(float)
	    -8.2525e-5,(float).016371,(float)-8.1515e-5,(float)5260.,(float)
	    5270.,(float)6.037e-5,(float).43685,(float).016157,(float)
	    -6.0387e-5,(float).013838,(float)-6.3959e-5,(float)5270.,(float)
	    5280.,(float)4.5624e-4,(float)1.0732,(float).012236,(float)
	    -4.3571e-5,(float).011139,(float)-5.5369e-5,(float)5280.,(float)
	    5290.,(float)9.4288e-4,(float).71418,(float).0033079,(float)
	    -9.3343e-6,(float).0045345,(float)-2.5858e-5,(float)5290.,(float)
	    5300.,(float).0024083,(float).75842,(float).0033829,(float)
	    4.6857e-7,(float).0043964,(float)-1.9712e-5,(float)5300.,(float)
	    5310.,(float).011861,(float).7448,(float)-.0016809,(float)
	    -1.8723e-6,(float)-.0036883,(float)4.435e-6,(float)5310.,(float)
	    5320.,(float).005669,(float).26992,(float)-.0026897,(float)
	    -1.9753e-6,(float)-.0056893,(float)3.9128e-6,(float)5320.,(float)
	    5330.,(float).0043472,(float).36618,(float).0037666,(float)
	    -2.1625e-5,(float).0021182,(float)-2.2907e-5,(float)5330.,(float)
	    5340.,(float)6.5931e-5,(float).17111,(float).021978,(float)
	    -9.6238e-5,(float).020336,(float)-9.699e-5,(float)5340.,(float)
	    5350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)5350.,(float)5360.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)5360.,(float)5370.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)5370.,(float)5380.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)5380.,(float)5390.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)5390.,(float)5400.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)5400.,(float)5410.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    5410.,(float)5420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)5420.,(float)5430.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)5430.,(float)5440.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5440.,
	    (float)5450.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5450.,(float)5460.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5460.,(float)5470.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5470.,(
	    float)5480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5480.,(float)5490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5490.,(float)5500.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5500.,(
	    float)5510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5510.,(float)5520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5520.,(float)5530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5530.,(
	    float)5540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5540.,(float)5550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5550.,(float)5560.,(float)
	    6.8899e-6,(float).12901,(float).032092,(float)-1.4141e-4,(float)
	    .029106,(float)-1.3565e-4,(float)5560.,(float)5570.,(float)
	    5.4438e-6,(float).10241,(float).030061,(float)-1.3353e-4,(float)
	    .027225,(float)-1.2783e-4,(float)5570.,(float)5580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5580.,(
	    float)5590.,(float)1.3218e-5,(float).23142,(float).031168,(float)
	    -1.3734e-4,(float).028262,(float)-1.3193e-4,(float)5590.,(float)
	    5600.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)5600.,(float)5610.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)5610.,(float)5620.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)5620.,(float)5630.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)5630.,(float)5640.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)5640.,(float)5650.,(float)3.3959e-6,(
	    float).061663,(float).034194,(float)-1.4999e-4,(float).031058,(
	    float)-1.438e-4,(float)5650.,(float)5660.,(float)1.0828e-5,(float)
	    .12604,(float).03117,(float)-1.3759e-4,(float).028105,(float)
	    -1.3178e-4,(float)5660.,(float)5670.,(float)1.7083e-5,(float)
	    .16767,(float).028148,(float)-1.2567e-4,(float).025263,(float)
	    -1.1999e-4,(float)5670.,(float)5680.,(float)4.9699e-6,(float)
	    .072931,(float).026646,(float)-1.1993e-4,(float).02372,(float)
	    -1.1402e-4,(float)5680.,(float)5690.,(float)3.2495e-5,(float)
	    .33854,(float).028198,(float)-1.2521e-4,(float).025185,(float)
	    -1.1947e-4,(float)5690.,(float)5700.,(float)8.5544e-6,(float)
	    .12346,(float).03299,(float)-1.4471e-4,(float).029937,(float)
	    -1.3907e-4,(float)5700.,(float)5710.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)5710.,(float)5720.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5720.,
	    (float)5730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5730.,(float)5740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5740.,(float)5750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5750.,(
	    float)5760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5760.,(float)5770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5770.,(float)5780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5780.,(
	    float)5790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5790.,(float)5800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5800.,(float)5810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5810.,(
	    float)5820.,(float)1.0801e-7,(float)6.4763e-4,(float).00504,(
	    float)-3.331e-5,(float).0018319,(float)-2.715e-5,(float)5820.,(
	    float)5830.,(float)3.6558e-7,(float).0011712,(float).0012766,(
	    float)-1.7918e-5,(float)-.0017271,(float)-1.2476e-5,(float)5830.,(
	    float)5840.,(float)6.5048e-7,(float).0015865,(float)-.0022804,(
	    float)-3.9603e-6,(float)-.0050092,(float)1.3558e-6,(float)5840.,(
	    float)5850.,(float)2.8928e-7,(float).0011091,(float)-.0041564,(
	    float)3.1815e-6,(float)-.0071328,(float)9.2288e-6,(float)5850.,(
	    float)5860.,(float)8.7451e-7,(float).0023908,(float)-.003329,(
	    float)2.1406e-7,(float)-.0062303,(float)6.1286e-6,(float)5860.,(
	    float)5870.,(float)6.7089e-7,(float).0021602,(float).0012514,(
	    float)-1.6242e-5,(float)-.0014507,(float)-1.275e-5,(float)5870.,(
	    float)5880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5880.,(float)5890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5890.,(float)5900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5900.,(
	    float)5910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5910.,(float)5920.,(float)1.7501e-8,(float)
	    1.3034e-4,(float).0051578,(float)-3.4109e-5,(float).0018735,(
	    float)-2.7495e-5,(float)5920.,(float)5930.,(float)6.9507e-7,(
	    float).0034287,(float)4.4624e-4,(float)-1.3702e-5,(float)
	    -.0024553,(float)-8.9919e-6,(float)5930.,(float)5940.,(float)
	    7.8354e-7,(float).0027679,(float)-.002211,(float)-4.0762e-6,(
	    float)-.0050605,(float)1.5266e-6,(float)5940.,(float)5950.,(float)
	    1.7028e-5,(float).28847,(float).029444,(float)-1.0994e-4,(float)
	    .027638,(float)-1.257e-4,(float)5950.,(float)5960.,(float)1.43e-5,
	    (float).23307,(float).025314,(float)-6.3115e-5,(float).025428,(
	    float)-1.0758e-4,(float)5960.,(float)5970.,(float)2.9931e-5,(
	    float).32693,(float).031262,(float)-1.0549e-4,(float).030096,(
	    float)-1.315e-4,(float)5970.,(float)5980.,(float)1.6591e-4,(float)
	    .85547,(float).027749,(float)-1.2007e-4,(float).025471,(float)
	    -1.1947e-4,(float)5980.,(float)5990.,(float)4.2245e-4,(float)
	    .43143,(float).020385,(float)-9.2124e-5,(float).018281,(float)
	    -8.9908e-5,(float)5990.,(float)6e3,(float)9.3274e-4,(float).46433,
	    (float).015249,(float)-7.1885e-5,(float).01312,(float)-6.839e-5,(
	    float)6e3,(float)6010.,(float)7.0225e-4,(float).54757,(float)
	    .013086,(float)-5.8065e-5,(float).011855,(float)-5.6321e-5,(float)
	    6010.,(float)6020.,(float)6.928e-4,(float).65125,(float).013222,(
	    float)-5.6472e-5,(float).011874,(float)-5.5027e-5,(float)6020.,(
	    float)6030.,(float).0015664,(float).5897,(float).015019,(float)
	    -7.1875e-5,(float).012178,(float)-6.7231e-5,(float)6030.,(float)
	    6040.,(float).0038079,(float).38374,(float).0089238,(float)
	    -3.815e-5,(float).0095406,(float)-4.6524e-5,(float)6040.,(float)
	    6050.,(float).011401,(float).22142,(float).0011962,(float)
	    -1.4969e-5,(float)5.0672e-4,(float)-5.0173e-6,(float)6050.,(float)
	    6060.,(float).015986,(float).20314,(float)-.0028719,(float)
	    -1.5218e-6,(float)-.0057785,(float)4.2132e-6,(float)6060.,(float)
	    6070.,(float).0065575,(float).20869,(float)-.0045237,(float)
	    4.7582e-6,(float)-.0074277,(float)1.0726e-5,(float)6070.,(float)
	    6080.,(float).016542,(float).20732,(float)-.0033062,(float)
	    1.2692e-7,(float)-.0062117,(float)5.9729e-6,(float)6080.,(float)
	    6090.,(float).0148,(float).21832,(float)5.2489e-4,(float)
	    -1.4021e-5,(float)-.002348,(float)-9.405e-6,(float)6090.,(float)
	    6100.,(float).0037777,(float).221,(float).0079105,(float)
	    -4.1125e-5,(float).0056565,(float)-3.9694e-5,(float)6100.,(float)
	    6110.,(float)2.3785e-4,(float).20223,(float).019267,(float)
	    -7.498e-5,(float).018479,(float)-8.4052e-5,(float)6110.,(float)
	    6120.,(float)8.7992e-6,(float).013627,(float).0023267,(float)
	    9.2669e-5,(float).014502,(float)8.1104e-5,(float)6120.,(float)
	    6130.,(float)4.6902e-5,(float).41886,(float).0248,(float)
	    7.0996e-5,(float).036122,(float)-1.3184e-4,(float)6130.,(float)
	    6140.,(float)1.7614e-4,(float).70325,(float).034157,(float)
	    -1.2548e-4,(float).032692,(float)-1.4479e-4,(float)6140.,(float)
	    6150.,(float)5.1423e-4,(float).765,(float).030511,(float)
	    -1.3248e-4,(float).028126,(float)-1.2927e-4,(float)6150.,(float)
	    6160.,(float).0016425,(float).80783,(float).024399,(float)
	    -1.0406e-4,(float).023798,(float)-1.0618e-4,(float)6160.,(float)
	    6170.,(float).0047297,(float).7047,(float).018441,(float)
	    -8.1354e-5,(float).017633,(float)-8.284e-5,(float)6170.,(float)
	    6180.,(float).012112,(float).7774,(float).014404,(float)
	    -6.4815e-5,(float).014093,(float)-6.4401e-5,(float)6180.,(float)
	    6190.,(float).029946,(float).46202,(float).0077033,(float)
	    -3.8421e-5,(float).0091624,(float)-3.7333e-5,(float)6190.,(float)
	    6200.,(float).084779,(float).36412,(float).0025097,(float)
	    -1.7677e-5,(float).0033111,(float)-1.9653e-5,(float)6200.,(float)
	    6210.,(float).14928,(float).35083,(float)-.0016626,(float)
	    6.5627e-7,(float)-1.0556e-5,(float)3.0708e-6,(float)6210.,(float)
	    6220.,(float).055916,(float).2564,(float)-.0037859,(float)
	    9.6465e-6,(float)-4.846e-4,(float)2.9685e-5,(float)6220.,(float)
	    6230.,(float).14147,(float).24788,(float)-.0037163,(float)
	    1.7896e-6,(float)-.006379,(float)1.0289e-5,(float)6230.,(float)
	    6240.,(float).16605,(float).27773,(float)3.1594e-4,(float)
	    -1.2404e-5,(float)-.0024142,(float)-8.6644e-6,(float)6240.,(float)
	    6250.,(float).026015,(float).2642,(float).0096177,(float)
	    -4.5715e-5,(float).0081902,(float)-4.718e-5,(float)6250.,(float)
	    6260.,(float)3.4429e-4,(float).24315,(float).027429,(float)
	    -1.0196e-4,(float).028346,(float)-1.1603e-4,(float)6260.,(float)
	    6270.,(float)4.6627e-5,(float).19149,(float).036426,(float)
	    -1.3678e-4,(float).034831,(float)-1.5131e-4,(float)6270.,(float)
	    6280.,(float)2.1373e-4,(float).25683,(float).031955,(float)
	    -1.3913e-4,(float).029335,(float)-1.3555e-4,(float)6280.,(float)
	    6290.,(float).0013513,(float).24863,(float).022352,(float)
	    -9.6861e-5,(float).021515,(float)-9.9991e-5,(float)6290.,(float)
	    6300.,(float).0054489,(float).20135,(float).014377,(float)
	    -6.5562e-5,(float).015352,(float)-6.4813e-5,(float)6300.,(float)
	    6310.,(float).027679,(float).22784,(float).0074243,(float)
	    -3.7863e-5,(float).0085371,(float)-3.3611e-5,(float)6310.,(float)
	    6320.,(float).078593,(float).25925,(float).0020493,(float)
	    -1.5599e-5,(float).0043199,(float)-6.7814e-6,(float)6320.,(float)
	    6330.,(float).14691,(float).31965,(float)-.0018417,(float)
	    -2.6752e-7,(float)-4.3593e-4,(float)7.2456e-6,(float)6330.,(float)
	    6340.,(float).060904,(float).38372,(float)-.0026072,(float)
	    1.0806e-5,(float)3.4105e-5,(float)6.1858e-6,(float)6340.,(float)
	    6350.,(float).16958,(float).41812,(float)-.0032411,(float)
	    1.9629e-6,(float)-.0029726,(float)8.5775e-6,(float)6350.,(float)
	    6360.,(float).14604,(float).4663,(float).0014212,(float)
	    -1.3457e-5,(float).0019636,(float)-1.361e-5,(float)6360.,(float)
	    6370.,(float).031358,(float).68476,(float).010997,(float)
	    -4.6694e-5,(float).011746,(float)-5.4252e-5,(float)6370.,(float)
	    6380.,(float).0010642,(float).83438,(float).027721,(float)
	    -1.1549e-4,(float).027342,(float)-1.2081e-4,(float)6380.,(float)
	    6390.,(float)1.0993e-5,(float).084985,(float).011868,(float)
	    1.2012e-4,(float).014373,(float)3.5831e-5,(float)6390.,(float)
	    6400.,(float)6.8548e-5,(float).19065,(float)-1.396e-4,(float)
	    -1.1926e-5,(float)-.0033005,(float)-5.8719e-6,(float)6400.,(float)
	    6410.,(float)5.8606e-5,(float).18176,(float).0053969,(float)
	    -3.2912e-5,(float).0024598,(float)-2.8413e-5,(float)6410.,(float)
	    6420.,(float)1.3107e-5,(float).089518,(float).011918,(float)
	    -6.0253e-5,(float).0088864,(float)-5.4787e-5,(float)6420.,(float)
	    6430.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)6430.,(float)6440.,(float)1.17e-5,(float).080752,(float)
	    .030987,(float)-1.3581e-4,(float).028483,(float)-1.3216e-4,(float)
	    6440.,(float)6450.,(float)1.5147e-4,(float).133,(float).021388,(
	    float)-9.5943e-5,(float).019129,(float)-9.4039e-5,(float)6450.,(
	    float)6460.,(float).0010569,(float).14161,(float).012722,(float)
	    -6.2067e-5,(float).010104,(float)-5.8766e-5,(float)6460.,(float)
	    6470.,(float).0062046,(float).17678,(float).0050148,(float)
	    -3.1545e-5,(float).002256,(float)-2.7623e-5,(float)6470.,(float)
	    6480.,(float).012909,(float).15821,(float)-1.1368e-4,(float)
	    -1.2254e-5,(float)-.0031262,(float)-6.7108e-6,(float)6480.,(float)
	    6490.,(float).018519,(float).26278,(float)-.0033124,(float)
	    1.2436e-6,(float)-.0049708,(float)1.187e-5,(float)6490.,(float)
	    6500.,(float).0094276,(float).28543,(float)-.0037172,(float)
	    9.5584e-6,(float)-.0028297,(float)1.7915e-5,(float)6500.,(float)
	    6510.,(float).02326,(float).34557,(float)-.0016092,(float)
	    -1.5397e-6,(float)-.0015497,(float)2.6953e-6,(float)6510.,(float)
	    6520.,(float).011602,(float).45777,(float).0040857,(float)
	    -2.3186e-5,(float).003463,(float)-2.4006e-5,(float)6520.,(float)
	    6530.,(float).0026251,(float)1.0461,(float).0096811,(float)
	    -3.5495e-5,(float).0054387,(float)-2.7418e-5,(float)6530.,(float)
	    6540.,(float).0013999,(float).87933,(float).011802,(float)
	    -4.3414e-5,(float).0093028,(float)-3.5381e-5,(float)6540.,(float)
	    6550.,(float).0010684,(float).70255,(float).015871,(float)
	    -6.462e-5,(float).012638,(float)-5.6493e-5,(float)6550.,(float)
	    6560.,(float)1.6155e-4,(float).42392,(float).025507,(float)
	    -1.1133e-4,(float).023436,(float)-1.1063e-4,(float)6560.,(float)
	    6570.,(float)1.8042e-5,(float).32854,(float).030046,(float)
	    -1.3213e-4,(float).02717,(float)-1.2714e-4,(float)6570.,(float)
	    6580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)6580.,(float)6590.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)6590.,(float)6600.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)6600.,(float)6610.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)6610.,(float)6620.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)6620.,(float)6630.,(float)1.6128e-5,(
	    float).088896,(float).01184,(float)-5.9958e-5,(float).0088458,(
	    float)-5.4627e-5,(float)6630.,(float)6640.,(float)5.7685e-5,(
	    float).1197,(float).0066894,(float)-3.9133e-5,(float).0037276,(
	    float)-3.4034e-5,(float)6640.,(float)6650.,(float)1.6743e-4,(
	    float).15492,(float).0016277,(float)-1.8979e-5,(float)-.0013919,(
	    float)-1.3601e-5,(float)6650.,(float)6660.,(float)2.3995e-4,(
	    float).19763,(float)-.0015517,(float)-2.5448e-6,(float)-.0034273,(
	    float)4.3033e-7,(float)6660.,(float)6670.,(float).0011561,(float)
	    .75988,(float)-.001204,(float)-2.7363e-6,(float)-.0037117,(float)
	    -3.8924e-7,(float)6670.,(float)6680.,(float)2.3092e-4,(float)
	    .26967,(float)-.0038326,(float)2.1748e-6,(float)-.0067421,(float)
	    8.1124e-6,(float)6680.,(float)6690.,(float)1.8913e-4,(float)
	    .29419,(float)2.3018e-4,(float)-1.1781e-5,(float)-.0022434,(float)
	    -9.0419e-6,(float)6690.,(float)6700.,(float)4.262e-6,(float)
	    .033197,(float).008741,(float)-4.0921e-5,(float).0057336,(float)
	    -3.8266e-5,(float)6700.,(float)6710.,(float)9.8135e-7,(float)
	    .0038989,(float).019411,(float)-9.0203e-5,(float).01638,(float)
	    -8.4877e-5,(float)6710.,(float)6720.,(float)2.4238e-6,(float)
	    .0042762,(float).015022,(float)-7.3077e-5,(float).01219,(float)
	    -6.7965e-5,(float)6720.,(float)6730.,(float)4.5088e-6,(float)
	    .0043488,(float).010996,(float)-5.6685e-5,(float).0085436,(float)
	    -5.3188e-5,(float)6730.,(float)6740.,(float)1.0584e-5,(float)
	    .0026967,(float).0057298,(float)-3.4851e-5,(float).0039372,(float)
	    -3.384e-5,(float)6740.,(float)6750.,(float)2.4861e-5,(float)
	    .0051111,(float).0029099,(float)-1.5549e-5,(float).0043761,(float)
	    -2.6982e-5,(float)6750.,(float)6760.,(float)3.9077e-5,(float)
	    .0028826,(float)-.001771,(float)-1.6701e-6,(float)-.001197,(float)
	    3.3e-6,(float)6760.,(float)6770.,(float)2.3656e-5,(float).0021453,
	    (float)-.004179,(float)3.1183e-6,(float)-.0071966,(float)9.311e-6,
	    (float)6770.,(float)6780.,(float)4.577e-5,(float).0032492,(float)
	    -.003795,(float)1.7261e-6,(float)-.006782,(float)7.9425e-6,(float)
	    6780.,(float)6790.,(float)6.1088e-5,(float).0055679,(float)
	    .001493,(float)-1.1871e-5,(float)7.2114e-4,(float)-1.3903e-5,(
	    float)6790.,(float)6800.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)6800.,(float)6810.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)6810.,(float)6820.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    6820.,(float)6830.,(float)9.2277e-7,(float).029726,(float).049361,
	    (float)-2.108e-4,(float).046239,(float)-2.0451e-4,(float)6830.,(
	    float)6840.,(float)1.4134e-5,(float).25624,(float).046651,(float)
	    -1.979e-4,(float).043955,(float)-1.9411e-4,(float)6840.,(float)
	    6850.,(float)7.2012e-5,(float).62577,(float).042787,(float)
	    -1.8123e-4,(float).0404,(float)-1.7929e-4,(float)6850.,(float)
	    6860.,(float)1.8633e-4,(float).5831,(float).038378,(float)
	    -1.6303e-4,(float).035924,(float)-1.6051e-4,(float)6860.,(float)
	    6870.,(float)5.7256e-4,(float).7495,(float).03396,(float)
	    -1.4641e-4,(float).03134,(float)-1.4291e-4,(float)6870.,(float)
	    6880.,(float).0014919,(float).79461,(float).029669,(float)
	    -1.2898e-4,(float).027598,(float)-1.2671e-4,(float)6880.,(float)
	    6890.,(float).003259,(float).69112,(float).02466,(float)
	    -1.0873e-4,(float).022993,(float)-1.093e-4,(float)6890.,(float)
	    6900.,(float).0079821,(float).66219,(float).020779,(float)
	    -9.1999e-5,(float).020053,(float)-9.48e-5,(float)6900.,(float)
	    6910.,(float).019955,(float).70298,(float).016653,(float)
	    -7.4032e-5,(float).017622,(float)-7.4061e-5,(float)6910.,(float)
	    6920.,(float).047678,(float).44625,(float).011512,(float)
	    -5.6613e-5,(float).010983,(float)-5.0893e-5,(float)6920.,(float)
	    6930.,(float).072667,(float).31454,(float).0067346,(float)
	    -3.8595e-5,(float).0058565,(float)-4.0764e-5,(float)6930.,(float)
	    6940.,(float).21391,(float).45263,(float).0039143,(float)
	    -1.9313e-5,(float).0056575,(float)-2.8872e-5,(float)6940.,(float)
	    6950.,(float).3773,(float).2493,(float)-.0011236,(float)
	    -3.7053e-6,(float)4.1967e-4,(float)5.3393e-6,(float)6950.,(float)
	    6960.,(float).29585,(float).17647,(float)-.0038687,(float)
	    2.2126e-6,(float)-.0068173,(float)8.2438e-6,(float)6960.,(float)
	    6970.,(float).30531,(float).23118,(float)-.004195,(float)
	    3.5322e-6,(float)-.0069672,(float)1.113e-5,(float)6970.,(float)
	    6980.,(float).77834,(float).54008,(float)5.3815e-4,(float)
	    -7.5276e-6,(float)-1.0156e-4,(float)-7.935e-6,(float)6980.,(float)
	    6990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)6990.,(float)7e3,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)7e3,(float)7010.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)7010.,(float)7020.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    7020.,(float)7030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)7030.,(float)7040.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)7040.,(float)7050.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7050.,
	    (float)7060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7060.,(float)7070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7070.,(float)7080.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7080.,(
	    float)7090.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7090.,(float)7100.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7100.,(float)7110.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7110.,(
	    float)7120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7120.,(float)7130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7130.,(float)7140.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7140.,(
	    float)7150.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7150.,(float)7160.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7160.,(float)7170.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7170.,(
	    float)7180.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7180.,(float)7190.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7190.,(float)7200.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7200.,(
	    float)7210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7210.,(float)7220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7220.,(float)7230.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7230.,(
	    float)7240.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7240.,(float)7250.,(float)9.7362e-6,(float)
	    .061285,(float).004498,(float)-3.1185e-5,(float).0013664,(float)
	    -2.5022e-5,(float)7250.,(float)7260.,(float)5.4706e-5,(float)
	    .18852,(float)2.4251e-4,(float)-1.332e-5,(float)-.0027302,(float)
	    -8.1259e-6,(float)7260.,(float)7270.,(float)6.1563e-5,(float)
	    .20629,(float)-.0032814,(float)2.2e-8,(float)-.0062079,(float)
	    5.925e-6,(float)7270.,(float)7280.,(float)1.4699e-5,(float)
	    .075241,(float)-.0044219,(float)4.3348e-6,(float)-.0074089,(float)
	    1.0364e-5,(float)7280.,(float)7290.,(float)7.0542e-5,(float)
	    .20329,(float)-.002882,(float)-1.4802e-6,(float)-.0057837,(float)
	    4.2344e-6,(float)7290.,(float)7300.,(float)5.4492e-5,(float)
	    .21457,(float).0013783,(float)-1.721e-5,(float)-.0014841,(float)
	    -1.2786e-5,(float)7300.,(float)7310.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)7310.,(float)7320.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7320.,
	    (float)7330.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7330.,(float)7340.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7340.,(float)7350.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7350.,(
	    float)7360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7360.,(float)7370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7370.,(float)7380.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7380.,(
	    float)7390.,(float)4.0391e-5,(float).34559,(float).016271,(float)
	    -7.7571e-5,(float).013286,(float)-7.2256e-5,(float)7390.,(float)
	    7400.,(float)4.8302e-5,(float).37526,(float).012898,(float)
	    -6.4691e-5,(float).010001,(float)-5.8901e-5,(float)7400.,(float)
	    7410.,(float)2.6773e-5,(float).14702,(float).016569,(float)
	    -7.6331e-5,(float).013558,(float)-7.16e-5,(float)7410.,(float)
	    7420.,(float)2.0816e-4,(float).52374,(float).010994,(float)
	    -5.4458e-5,(float).0090105,(float)-5.4076e-5,(float)7420.,(float)
	    7430.,(float)4.8129e-4,(float).23329,(float).0042161,(float)
	    -2.5147e-5,(float).0034174,(float)-2.5567e-5,(float)7430.,(float)
	    7440.,(float).0010907,(float).19364,(float)-.0012077,(float)
	    -7.8171e-6,(float)-.0041544,(float)-2.4136e-6,(float)7440.,(float)
	    7450.,(float)7.8191e-4,(float).2075,(float)-.0039131,(float)
	    2.4015e-6,(float)-.0069105,(float)8.5956e-6,(float)7450.,(float)
	    7460.,(float)7.115e-4,(float).19688,(float)-.0041668,(float)
	    3.3692e-6,(float)-.0070951,(float)9.4003e-6,(float)7460.,(float)
	    7470.,(float).0014863,(float).25817,(float)-.0012495,(float)
	    -7.1361e-6,(float)-.0041323,(float)-2.2309e-6,(float)7470.,(float)
	    7480.,(float)4.7705e-4,(float).2487,(float).005832,(float)
	    -3.2625e-5,(float).0036125,(float)-3.1486e-5,(float)7480.,(float)
	    7490.,(float)2.3552e-5,(float).11333,(float).017332,(float)
	    -8.0926e-5,(float).014655,(float)-7.7124e-5,(float)7490.,(float)
	    7500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)7500.,(float)7510.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)7510.,(float)7520.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)7520.,(float)7530.,
	    (float)7.1378e-6,(float).057272,(float).023828,(float)-1.0818e-4,(
	    float).020923,(float)-1.0279e-4,(float)7530.,(float)7540.,(float)
	    5.5634e-5,(float).11302,(float).017308,(float)-8.0844e-5,(float)
	    .014641,(float)-7.7072e-5,(float)7540.,(float)7550.,(float)
	    3.874e-4,(float).29038,(float).010833,(float)-4.9887e-5,(float)
	    .010414,(float)-5.4606e-5,(float)7550.,(float)7560.,(float)
	    9.6863e-4,(float).27281,(float).0053932,(float)-2.8655e-5,(float)
	    .0052931,(float)-3.3358e-5,(float)7560.,(float)7570.,(float)
	    .0023248,(float).26964,(float)4.149e-4,(float)-1.1296e-5,(float)
	    -6.0618e-5,(float)-1.1765e-5,(float)7570.,(float)7580.,(float)
	    .0029909,(float).2796,(float)-.0030237,(float)8.4962e-7,(float)
	    -.0041122,(float)3.9569e-6,(float)7580.,(float)7590.,(float)
	    .001504,(float).43911,(float)-.0026031,(float)1.2211e-5,(float)
	    -.0012549,(float)4.2399e-6,(float)7590.,(float)7600.,(float)
	    .004448,(float).35168,(float)-.0020068,(float)-1.1923e-6,(float)
	    -.0024644,(float)4.0855e-6,(float)7600.,(float)7610.,(float)
	    .0020548,(float).3146,(float).0041619,(float)-2.4632e-5,(float)
	    .0023465,(float)-2.5042e-5,(float)7610.,(float)7620. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     carbon dioxide (5060 - 7610 cm-1) */

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
} /* dica2_ */

#ifdef __cplusplus
	}
#endif
