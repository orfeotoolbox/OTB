/* METH2.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    meth2(a,inu) >*/
/* Subroutine */ int meth2_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)5060.,(float)5070.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    5070.,(float)5080.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)5080.,(float)5090.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)5090.,(float)5100.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5100.,
	    (float)5110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5110.,(float)5120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5120.,(float)5130.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5130.,(
	    float)5140.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5140.,(float)5150.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5150.,(float)5160.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5160.,(
	    float)5170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5170.,(float)5180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5180.,(float)5190.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5190.,(
	    float)5200.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5200.,(float)5210.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5210.,(float)5220.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5220.,(
	    float)5230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5230.,(float)5240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5240.,(float)5250.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5250.,(
	    float)5260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5260.,(float)5270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5270.,(float)5280.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5280.,(
	    float)5290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5290.,(float)5300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5300.,(float)5310.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5310.,(
	    float)5320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5320.,(float)5330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5330.,(float)5340.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5340.,(
	    float)5350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5350.,(float)5360.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5360.,(float)5370.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5370.,(
	    float)5380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5380.,(float)5390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5390.,(float)5400.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5400.,(
	    float)5410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5410.,(float)5420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5420.,(float)5430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5430.,(
	    float)5440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5440.,(float)5450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5450.,(float)5460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5460.,(
	    float)5470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5470.,(float)5480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5480.,(float)5490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5490.,(
	    float)5500.,(float).50197,(float).25265,(float).0033519,(float)
	    -2.4607e-5,(float)6.8955e-4,(float)-2.0482e-5,(float)5500.,(float)
	    5510.,(float)2.3899,(float).60596,(float)2.7671e-5,(float)
	    -1.2307e-5,(float)-.0033058,(float)-5.2945e-6,(float)5510.,(float)
	    5520.,(float)2.4379,(float).30699,(float)-6.0867e-4,(float)
	    -9.0704e-6,(float)-.0032892,(float)-5.0115e-6,(float)5520.,(float)
	    5530.,(float)2.1592,(float).38949,(float)-.0023556,(float)
	    -3.3022e-6,(float)-.0052838,(float)2.4513e-6,(float)5530.,(float)
	    5540.,(float)2.3029,(float).15736,(float)-.0035795,(float)
	    2.1673e-6,(float)-.005968,(float)6.0863e-6,(float)5540.,(float)
	    5550.,(float)1.954,(float).11711,(float)-.0044087,(float)5.103e-6,
	    (float)-.0068665,(float)9.1701e-6,(float)5550.,(float)5560.,(
	    float)1.195,(float).29396,(float)-.0010618,(float)3.4067e-6,(
	    float)-.0024231,(float)-2.582e-6,(float)5560.,(float)5570.,(float)
	    4.8095,(float)1.2465,(float).0019344,(float)-1.5456e-5,(float)
	    -6.8788e-4,(float)-1.287e-5,(float)5570.,(float)5580.,(float)
	    11.674,(float)1.5114,(float)-.0025504,(float)8.25e-7,(float)
	    -.0045912,(float)2.2777e-6,(float)5580.,(float)5590.,(float)
	    2.3702,(float).84024,(float)-8.2688e-4,(float)2.9123e-6,(float)
	    -.0020134,(float)-5.0547e-6,(float)5590.,(float)5600.,(float)
	    3.4064,(float)1.1326,(float)-.0025002,(float)8.9997e-7,(float)
	    -.0048837,(float)2.6082e-6,(float)5600.,(float)5610.,(float)
	    1.7392,(float).31991,(float)-.0045126,(float)8.6603e-6,(float)
	    -.0061278,(float)9.3747e-6,(float)5610.,(float)5620.,(float)4.348,
	    (float).58786,(float)-.0043137,(float)4.6334e-6,(float)-.0070133,(
	    float)9.529e-6,(float)5620.,(float)5630.,(float)6.6586,(float)
	    1.8023,(float)-.0019866,(float)-1.5163e-6,(float)-.0039533,(float)
	    -8.5993e-7,(float)5630.,(float)5640.,(float)4.3959,(float).91267,(
	    float)-.0018359,(float)-2.6379e-6,(float)-.0045115,(float)
	    1.3137e-6,(float)5640.,(float)5650.,(float)2.9732,(float).90097,(
	    float)-4.5783e-4,(float)-8.654e-6,(float)-.0035958,(float)
	    -2.8318e-6,(float)5650.,(float)5660.,(float)2.7758,(float).82876,(
	    float).0019221,(float)-1.4165e-5,(float)-.0010447,(float)
	    -1.1375e-5,(float)5660.,(float)5670.,(float)1.4345,(float).92016,(
	    float).0046313,(float)-1.9911e-5,(float).0018272,(float)
	    -2.0246e-5,(float)5670.,(float)5680.,(float)1.0486,(float).4465,(
	    float)-3.8086e-4,(float)5.6985e-6,(float)-.0018929,(float)
	    -1.1857e-6,(float)5680.,(float)5690.,(float).94797,(float).35596,(
	    float)-2.2904e-4,(float)8.4588e-6,(float)-.0016889,(float)
	    -3.7956e-7,(float)5690.,(float)5700.,(float).93528,(float).41431,(
	    float).00128,(float)-8.7918e-7,(float)-.0014444,(float)-4.7052e-6,
	    (float)5700.,(float)5710.,(float)1.6454,(float).57474,(float)
	    .0026463,(float)-1.3124e-5,(float)-3.7027e-4,(float)-1.2452e-5,(
	    float)5710.,(float)5720.,(float)2.0351,(float).34637,(float)
	    .0026986,(float)-1.9613e-5,(float)-4.6781e-4,(float)-1.4709e-5,(
	    float)5720.,(float)5730.,(float)2.2445,(float).69714,(float)
	    .0019373,(float)-1.6752e-5,(float)-8.5242e-4,(float)-1.2956e-5,(
	    float)5730.,(float)5740.,(float)3.2153,(float).75956,(float)
	    .0013362,(float)-1.4189e-5,(float)-.0010894,(float)-1.1571e-5,(
	    float)5740.,(float)5750.,(float)3.4596,(float).73536,(float)
	    .0012876,(float)-1.2547e-5,(float)-4.537e-4,(float)-1.2912e-5,(
	    float)5750.,(float)5760.,(float)4.9414,(float)1.0526,(float)
	    6.4547e-4,(float)-1.0024e-5,(float)-9.6857e-4,(float)-1.1592e-5,(
	    float)5760.,(float)5770.,(float)7.5119,(float)1.41,(float)
	    2.8819e-4,(float)-7.444e-6,(float)-.0012786,(float)-1.0307e-5,(
	    float)5770.,(float)5780.,(float)7.5235,(float)1.323,(float)
	    -4.9407e-4,(float)-6.8773e-6,(float)-.002629,(float)-6.638e-6,(
	    float)5780.,(float)5790.,(float)10.241,(float)1.6323,(float)
	    -.0013776,(float)-5.3214e-6,(float)-.0037904,(float)-2.3185e-6,(
	    float)5790.,(float)5800.,(float)9.1281,(float).93597,(float)
	    -.0035876,(float)2.3809e-6,(float)-.0059879,(float)5.9495e-6,(
	    float)5800.,(float)5810.,(float)3.9872,(float).6767,(float)
	    -.0044503,(float)6.8501e-6,(float)-.0067178,(float)9.7535e-6,(
	    float)5810.,(float)5820.,(float)2.9318,(float).24918,(float)
	    -.0051233,(float)7.7265e-6,(float)-.0076555,(float)1.1663e-5,(
	    float)5820.,(float)5830.,(float)3.6596,(float).91227,(float)
	    -.0026988,(float)2.3063e-6,(float)-.0047124,(float)2.5366e-6,(
	    float)5830.,(float)5840.,(float)6.9904,(float).93296,(float)
	    -.0037966,(float)6.1051e-6,(float)-.0055025,(float)7.3047e-6,(
	    float)5840.,(float)5850.,(float)12.815,(float)1.9354,(float)
	    -.0021432,(float)8.1298e-7,(float)-.0042246,(float)2.2902e-6,(
	    float)5850.,(float)5860.,(float)10.246,(float)2.1238,(float)
	    4.6316e-5,(float)-6.0726e-6,(float)-.00223,(float)-5.3836e-6,(
	    float)5860.,(float)5870.,(float)9.5647,(float)1.9821,(float)
	    .0012796,(float)-7.4626e-6,(float)-9.2616e-4,(float)-1.0886e-5,(
	    float)5870.,(float)5880.,(float)4.8776,(float)1.7308,(float)
	    .0012461,(float)-8.1397e-6,(float)-5.6127e-5,(float)-1.3882e-5,(
	    float)5880.,(float)5890.,(float)5.7337,(float).87446,(float)
	    .0038286,(float)-1.704e-5,(float)6.941e-4,(float)-1.573e-5,(float)
	    5890.,(float)5900.,(float)7.472,(float).73641,(float).0028865,(
	    float)-2.0558e-5,(float)-6.6892e-4,(float)-1.358e-5,(float)5900.,(
	    float)5910.,(float)7.0944,(float).66112,(float).0024095,(float)
	    -2.1458e-5,(float)-9.9935e-4,(float)-1.3636e-5,(float)5910.,(
	    float)5920.,(float)8.1168,(float).58477,(float).0013314,(float)
	    -1.7142e-5,(float)-.0010021,(float)-1.3588e-5,(float)5920.,(float)
	    5930.,(float)10.972,(float).47138,(float)-3.0541e-4,(float)
	    -9.6826e-6,(float)-.0014478,(float)-9.657e-6,(float)5930.,(float)
	    5940.,(float)7.3055,(float).38828,(float)-.0015833,(float)
	    -3.9472e-6,(float)-.0024534,(float)-5.2105e-6,(float)5940.,(float)
	    5950.,(float).18257,(float).15141,(float).0032545,(float)
	    -2.5286e-5,(float)3.2305e-4,(float)-1.9965e-5,(float)5950.,(float)
	    5960.,(float)8.803,(float).28605,(float)-.0031163,(float)
	    1.3762e-6,(float)-.0040606,(float)1.0755e-6,(float)5960.,(float)
	    5970.,(float)6.8729,(float).20493,(float)-.0040682,(float)
	    5.468e-6,(float)-.004767,(float)4.3804e-6,(float)5970.,(float)
	    5980.,(float)2.9955,(float).47987,(float)-.002474,(float)
	    9.9034e-6,(float)-8.4087e-4,(float)-7.5046e-6,(float)5980.,(float)
	    5990.,(float)14.364,(float)1.4135,(float).0066597,(float)
	    -3.1277e-5,(float).0045742,(float)-3.1988e-5,(float)5990.,(float)
	    6e3,(float)65.167,(float).97529,(float)-.0019616,(float)
	    -2.2633e-6,(float)-.0046527,(float)1.0271e-6,(float)6e3,(float)
	    6010.,(float)3.5647,(float).20344,(float)-.0052252,(float)
	    1.0688e-5,(float)-.0057674,(float)8.0346e-6,(float)6010.,(float)
	    6020.,(float)3.4141,(float).14357,(float)-.0054304,(float)
	    9.5186e-6,(float)-.0072736,(float)1.143e-5,(float)6020.,(float)
	    6030.,(float)7.635,(float).4332,(float)-.0031157,(float)1.3276e-5,
	    (float)-.001277,(float)1.7678e-7,(float)6030.,(float)6040.,(float)
	    16.118,(float).28374,(float)-.0041664,(float)4.9599e-6,(float)
	    -.0056931,(float)5.7765e-6,(float)6040.,(float)6050.,(float)
	    19.177,(float).33833,(float)-.0035167,(float)1.7094e-6,(float)
	    -.0061198,(float)6.6957e-6,(float)6050.,(float)6060.,(float)16.85,
	    (float).88879,(float)-.002146,(float)-3.2028e-6,(float)-.0042847,(
	    float)2.7277e-7,(float)6060.,(float)6070.,(float)21.077,(float)
	    .25352,(float)-9.3983e-4,(float)-9.0824e-6,(float)-.0038178,(
	    float)-2.813e-6,(float)6070.,(float)6080.,(float)14.763,(float)
	    .2437,(float)5.8706e-4,(float)-1.4814e-5,(float)-.0030455,(float)
	    -5.893e-6,(float)6080.,(float)6090.,(float)12.16,(float).31237,(
	    float).002521,(float)-2.1431e-5,(float)-.0011996,(float)
	    -1.1964e-5,(float)6090.,(float)6100.,(float)10.601,(float).42935,(
	    float).0043443,(float)-2.7697e-5,(float)3.1712e-4,(float)
	    -1.7641e-5,(float)6100.,(float)6110.,(float)7.7832,(float).54373,(
	    float).0057906,(float)-2.6532e-5,(float).001438,(float)-1.9125e-5,
	    (float)6110.,(float)6120.,(float)5.7902,(float)1.1869,(float)
	    .0054008,(float)-1.8503e-5,(float).0023381,(float)-2.0741e-5,(
	    float)6120.,(float)6130.,(float)4.0292,(float).58128,(float)
	    .0079694,(float)-2.4981e-5,(float).0032731,(float)-2.2237e-5,(
	    float)6130.,(float)6140.,(float)3.0446,(float).82071,(float)
	    .0077456,(float)-1.3929e-5,(float).0053212,(float)-2.5965e-5,(
	    float)6140.,(float)6150.,(float)1.9431,(float).98831,(float)
	    .006064,(float)-2.2019e-5,(float).0032998,(float)-2.4905e-5,(
	    float)6150.,(float)6160.,(float).97862,(float).77724,(float)
	    .0073198,(float)-3.3144e-5,(float).0043341,(float)-3.23e-5,(float)
	    6160.,(float)6170.,(float).41035,(float).31003,(float).0044331,(
	    float)-2.7381e-5,(float).0019194,(float)-2.471e-5,(float)6170.,(
	    float)6180.,(float).19122,(float).12211,(float).0046368,(float)
	    -3.0314e-5,(float).0017315,(float)-2.5376e-5,(float)6180.,(float)
	    6190.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)6190.,(float)6200.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)6200.,(float)6210.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)6210.,(float)6220.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)6220.,(float)6230.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)6230.,(float)6240.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)6240.,(float)6250.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    6250.,(float)6260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)6260.,(float)6270.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)6270.,(float)6280.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6280.,
	    (float)6290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6290.,(float)6300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6300.,(float)6310.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6310.,(
	    float)6320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6320.,(float)6330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6330.,(float)6340.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6340.,(
	    float)6350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6350.,(float)6360.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6360.,(float)6370.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6370.,(
	    float)6380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6380.,(float)6390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6390.,(float)6400.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6400.,(
	    float)6410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6410.,(float)6420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6420.,(float)6430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6430.,(
	    float)6440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6440.,(float)6450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6450.,(float)6460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6460.,(
	    float)6470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6470.,(float)6480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6480.,(float)6490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6490.,(
	    float)6500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6500.,(float)6510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6510.,(float)6520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6520.,(
	    float)6530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6530.,(float)6540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6540.,(float)6550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6550.,(
	    float)6560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6560.,(float)6570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6570.,(float)6580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6580.,(
	    float)6590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6590.,(float)6600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6600.,(float)6610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6610.,(
	    float)6620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6620.,(float)6630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6630.,(float)6640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6640.,(
	    float)6650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6650.,(float)6660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6660.,(float)6670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6670.,(
	    float)6680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6680.,(float)6690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6690.,(float)6700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6700.,(
	    float)6710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6710.,(float)6720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6720.,(float)6730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6730.,(
	    float)6740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6740.,(float)6750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6750.,(float)6760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6760.,(
	    float)6770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6770.,(float)6780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6780.,(float)6790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6790.,(
	    float)6800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6800.,(float)6810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6810.,(float)6820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6820.,(
	    float)6830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6830.,(float)6840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6840.,(float)6850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6850.,(
	    float)6860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6860.,(float)6870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6870.,(float)6880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6880.,(
	    float)6890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6890.,(float)6900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6900.,(float)6910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6910.,(
	    float)6920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6920.,(float)6930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6930.,(float)6940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6940.,(
	    float)6950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6950.,(float)6960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6960.,(float)6970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6970.,(
	    float)6980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6980.,(float)6990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6990.,(float)7e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7e3,(
	    float)7010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7010.,(float)7020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7020.,(float)7030.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7030.,(
	    float)7040.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7040.,(float)7050.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7050.,(float)7060.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7060.,(
	    float)7070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7070.,(float)7080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7080.,(float)7090.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7090.,(
	    float)7100.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7100.,(float)7110.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7110.,(float)7120.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7120.,(
	    float)7130.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7130.,(float)7140.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7140.,(float)7150.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7150.,(
	    float)7160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7160.,(float)7170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7170.,(float)7180.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7180.,(
	    float)7190.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7190.,(float)7200.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7200.,(float)7210.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7210.,(
	    float)7220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7220.,(float)7230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7230.,(float)7240.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7240.,(
	    float)7250.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7250.,(float)7260.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7260.,(float)7270.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7270.,(
	    float)7280.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7280.,(float)7290.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7290.,(float)7300.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7300.,(
	    float)7310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7310.,(float)7320.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7320.,(float)7330.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7330.,(
	    float)7340.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7340.,(float)7350.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7350.,(float)7360.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7360.,(
	    float)7370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7370.,(float)7380.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7380.,(float)7390.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7390.,(
	    float)7400.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7400.,(float)7410.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7410.,(float)7420.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7420.,(
	    float)7430.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7430.,(float)7440.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7440.,(float)7450.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7450.,(
	    float)7460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7460.,(float)7470.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7470.,(float)7480.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7480.,(
	    float)7490.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7490.,(float)7500.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7500.,(float)7510.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7510.,(
	    float)7520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7520.,(float)7530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7530.,(float)7540.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7540.,(
	    float)7550.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7550.,(float)7560.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7560.,(float)7570.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7570.,(
	    float)7580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7580.,(float)7590.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7590.,(float)7600.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7600.,(
	    float)7610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7610.,(float)7620. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     methane (5060 - 7610 cm-1) */

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
} /* meth2_ */

#ifdef __cplusplus
	}
#endif
