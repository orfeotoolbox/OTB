/* MOCA2.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    moca2(a,inu) >*/
/* Subroutine */ int moca2_(real *a, integer *inu)
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
	    float)5500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5500.,(float)5510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5510.,(float)5520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5520.,(
	    float)5530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5530.,(float)5540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5540.,(float)5550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5550.,(
	    float)5560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5560.,(float)5570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5570.,(float)5580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5580.,(
	    float)5590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5590.,(float)5600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5600.,(float)5610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5610.,(
	    float)5620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5620.,(float)5630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5630.,(float)5640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5640.,(
	    float)5650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5650.,(float)5660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5660.,(float)5670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5670.,(
	    float)5680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5680.,(float)5690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5690.,(float)5700.,(float)
	    4.6683e-26,(float)3.9946e-7,(float).15004,(float)-6.0825e-4,(
	    float).14724,(float)-6.0261e-4,(float)5700.,(float)5710.,(float)
	    1.2312e-25,(float)3.9946e-7,(float).14595,(float)-5.9188e-4,(
	    float).14315,(float)-5.8625e-4,(float)5710.,(float)5720.,(float)
	    1.1266e-24,(float)7.5927e-7,(float).13909,(float)-5.6295e-4,(
	    float).13671,(float)-5.5959e-4,(float)5720.,(float)5730.,(float)
	    2.0103e-24,(float)3.9946e-7,(float).13415,(float)-5.447e-4,(float)
	    .13135,(float)-5.3906e-4,(float)5730.,(float)5740.,(float)
	    4.9063e-24,(float)3.9946e-7,(float).13038,(float)-5.2961e-4,(
	    float).12758,(float)-5.2397e-4,(float)5740.,(float)5750.,(float)
	    3.9255e-23,(float)7.6518e-7,(float).12413,(float)-5.0331e-4,(
	    float).12169,(float)-4.9966e-4,(float)5750.,(float)5760.,(float)
	    6.3305e-23,(float)3.9946e-7,(float).11954,(float)-4.8626e-4,(
	    float).11675,(float)-4.8063e-4,(float)5760.,(float)5770.,(float)
	    1.5847e-21,(float)1.0092e-6,(float).10516,(float)-4.1519e-4,(
	    float).10467,(float)-4.2332e-4,(float)5770.,(float)5780.,(float)
	    3.6811e-21,(float)7.1016e-7,(float).09985,(float)-3.9815e-4,(
	    float).098651,(float)-4.013e-4,(float)5780.,(float)5790.,(float)
	    3.2319e-20,(float)1.2898e-6,(float).092607,(float)-3.6764e-4,(
	    float).092132,(float)-3.7318e-4,(float)5790.,(float)5800.,(float)
	    5.6403e-20,(float)6.4883e-7,(float).087374,(float)-3.4877e-4,(
	    float).086636,(float)-3.5122e-4,(float)5800.,(float)5810.,(float)
	    1.6009e-19,(float)9.5335e-7,(float).0849,(float)-3.3381e-4,(float)
	    .084844,(float)-3.4257e-4,(float)5810.,(float)5820.,(float)
	    1.0493e-18,(float)1.0941e-6,(float).076482,(float)-3.0762e-4,(
	    float).080072,(float)-2.5402e-4,(float)5820.,(float)5830.,(float)
	    1.8849e-18,(float)2.3089e-6,(float).073551,(float)-2.7995e-4,(
	    float).10134,(float)-1.0464e-4,(float)5830.,(float)5840.,(float)
	    1.2274e-17,(float)1.8503e-6,(float).065955,(float)-2.6262e-4,(
	    float).08719,(float)-6.1536e-5,(float)5840.,(float)5850.,(float)
	    1.9475e-17,(float)2.1482e-6,(float).062641,(float)-2.3947e-4,(
	    float).092133,(float)-5.5475e-5,(float)5850.,(float)5860.,(float)
	    4.2834e-17,(float)5.2914e-6,(float).060443,(float)-2.1227e-4,(
	    float).10022,(float)-9.1581e-5,(float)5860.,(float)5870.,(float)
	    2.5814e-16,(float)2.709e-6,(float).053613,(float)-2.0674e-4,(
	    float).078473,(float)-2.4556e-5,(float)5870.,(float)5880.,(float)
	    3.4582e-16,(float)2.4018e-6,(float).049817,(float)-1.8578e-4,(
	    float).083893,(float)-9.9109e-6,(float)5880.,(float)5890.,(float)
	    1.9766e-15,(float)2.6621e-6,(float).044522,(float)-1.7276e-4,(
	    float).070531,(float)1.7855e-5,(float)5890.,(float)5900.,(float)
	    7.0047e-15,(float)5.7066e-6,(float).039496,(float)-1.4181e-4,(
	    float).075654,(float)5.7712e-6,(float)5900.,(float)5910.,(float)
	    8.6757e-15,(float)3.3052e-5,(float).040851,(float)-4.9822e-5,(
	    float).088823,(float)-2.0654e-4,(float)5910.,(float)5920.,(float)
	    4.0761e-14,(float)1.4076e-5,(float).033721,(float)-8.1073e-5,(
	    float).073952,(float)-7.155e-5,(float)5920.,(float)5930.,(float)
	    1.1802e-13,(float)1.8315e-5,(float).029401,(float)-5.6811e-5,(
	    float).072527,(float)-6.1682e-5,(float)5930.,(float)5940.,(float)
	    1.2517e-13,(float)3.208e-5,(float).029645,(float)5.2262e-6,(float)
	    .075673,(float)-1.605e-4,(float)5940.,(float)5950.,(float)
	    5.3299e-13,(float)8.3885e-5,(float).028648,(float)5.1037e-5,(
	    float).076701,(float)-1.1804e-4,(float)5950.,(float)5960.,(float)
	    1.2176e-12,(float)4.3932e-5,(float).023072,(float)3.9649e-5,(
	    float).070157,(float)-2.5205e-5,(float)5960.,(float)5970.,(float)
	    2.6812e-12,(float)7.4724e-5,(float).021392,(float)7.8492e-5,(
	    float).0689,(float)-5.7739e-5,(float)5970.,(float)5980.,(float)
	    5.2852e-12,(float)8.1623e-5,(float).018441,(float)9.2123e-5,(
	    float).072474,(float)3.8196e-5,(float)5980.,(float)5990.,(float)
	    1.2137e-11,(float)2.5414e-4,(float).024162,(float)1.7686e-4,(
	    float).065473,(float)-1.1219e-4,(float)5990.,(float)6e3,(float)
	    2.022e-11,(float)1.7439e-4,(float).021342,(float)1.7061e-4,(float)
	    .061647,(float)-4.9481e-5,(float)6e3,(float)6010.,(float)
	    7.0256e-11,(float)3.9013e-4,(float).025354,(float)1.739e-4,(float)
	    .056294,(float)-1.0119e-4,(float)6010.,(float)6020.,(float)
	    9.1715e-11,(float)3.8456e-4,(float).027846,(float)1.3835e-4,(
	    float).055857,(float)-8.5738e-5,(float)6020.,(float)6030.,(float)
	    3.5812e-10,(float)8.9526e-4,(float).036811,(float)-1.9691e-5,(
	    float).060026,(float)-5.1263e-5,(float)6030.,(float)6040.,(float)
	    5.1951e-10,(float)6.5082e-4,(float).034472,(float)-3.8106e-5,(
	    float).057771,(float)-1.7166e-5,(float)6040.,(float)6050.,(float)
	    2.3396e-9,(float).0012585,(float).037673,(float)-1.3867e-4,(float)
	    .056388,(float)-3.7053e-5,(float)6050.,(float)6060.,(float)
	    3.1627e-9,(float).0012183,(float).033941,(float)-1.1427e-4,(float)
	    .058265,(float)-1.4231e-5,(float)6060.,(float)6070.,(float)
	    1.3747e-8,(float).0010905,(float).030224,(float)-1.1494e-4,(float)
	    .045888,(float)-1.243e-5,(float)6070.,(float)6080.,(float)
	    1.6746e-8,(float).0011055,(float).0271,(float)-9.4052e-5,(float)
	    .048795,(float)1.7267e-6,(float)6080.,(float)6090.,(float)
	    6.9627e-8,(float).0014531,(float).023474,(float)-8.7211e-5,(float)
	    .042687,(float)2.9187e-5,(float)6090.,(float)6100.,(float)
	    1.789e-7,(float).0023713,(float).020122,(float)-5.984e-5,(float)
	    .042924,(float)9.3171e-6,(float)6100.,(float)6110.,(float)
	    1.6918e-7,(float).002373,(float).018564,(float)-3.2576e-5,(float)
	    .047913,(float)-5.6001e-6,(float)6110.,(float)6120.,(float)
	    6.1497e-7,(float).0020751,(float).014671,(float)-3.6986e-5,(float)
	    .03447,(float)3.2655e-5,(float)6120.,(float)6130.,(float)
	    1.3012e-6,(float).0059809,(float).014081,(float)1.2706e-5,(float)
	    .041059,(float)-1.4713e-5,(float)6130.,(float)6140.,(float)
	    2.3687e-6,(float).0034824,(float).010334,(float)1.187e-5,(float)
	    .031822,(float)2.134e-5,(float)6140.,(float)6150.,(float)
	    4.1168e-6,(float).0049295,(float).0084797,(float)3.3745e-5,(float)
	    .032297,(float)2.8091e-5,(float)6150.,(float)6160.,(float)
	    7.275e-6,(float).015527,(float).012669,(float)9.9217e-5,(float)
	    .036777,(float)-5.5634e-5,(float)6160.,(float)6170.,(float)
	    9.7222e-6,(float).0091547,(float).010267,(float)9.5535e-5,(float)
	    .030194,(float)-2.5674e-5,(float)6170.,(float)6180.,(float)
	    1.7703e-5,(float).012682,(float).0090194,(float)1.0461e-4,(float)
	    .028528,(float)-8.8439e-6,(float)6180.,(float)6190.,(float)
	    3.2557e-5,(float).038848,(float).022453,(float)5.1526e-5,(float)
	    .032595,(float)-1.0114e-4,(float)6190.,(float)6200.,(float)
	    3.6378e-5,(float).026446,(float).022869,(float)1.5344e-5,(float)
	    .029472,(float)-7.8062e-5,(float)6200.,(float)6210.,(float)
	    1.5091e-4,(float).045107,(float).027472,(float)-9.5379e-5,(float)
	    .027231,(float)-1.0428e-4,(float)6210.,(float)6220.,(float)
	    4.4635e-4,(float).044617,(float).022992,(float)-8.3379e-5,(float)
	    .022135,(float)-8.6546e-5,(float)6220.,(float)6230.,(float)
	    4.645e-4,(float).025101,(float).020065,(float)-7.4635e-5,(float)
	    .019585,(float)-7.1162e-5,(float)6230.,(float)6240.,(float)
	    .0018567,(float).045449,(float).017769,(float)-7.6382e-5,(float)
	    .015878,(float)-6.8709e-5,(float)6240.,(float)6250.,(float)
	    .0042909,(float).047419,(float).01417,(float)-6.399e-5,(float)
	    .012295,(float)-5.5119e-5,(float)6250.,(float)6260.,(float)
	    .0037371,(float).024446,(float).011714,(float)-5.4834e-5,(float)
	    .0096092,(float)-4.6244e-5,(float)6260.,(float)6270.,(float)
	    .012658,(float).047613,(float).0091389,(float)-4.4311e-5,(float)
	    .006717,(float)-3.7584e-5,(float)6270.,(float)6280.,(float)
	    .022981,(float).050782,(float).0062294,(float)-3.2686e-5,(float)
	    .0039945,(float)-2.4418e-5,(float)6280.,(float)6290.,(float)
	    .03751,(float).052128,(float).0036805,(float)-2.2546e-5,(float)
	    .0014034,(float)-1.3971e-5,(float)6290.,(float)6300.,(float)
	    .054636,(float).053047,(float).0014952,(float)-1.3854e-5,(float)
	    -7.4579e-4,(float)-4.3465e-6,(float)6300.,(float)6310.,(float)
	    .070262,(float).053883,(float)-3.2751e-4,(float)-6.6345e-6,(float)
	    -.0026603,(float)2.8956e-6,(float)6310.,(float)6320.,(float)
	    .078204,(float).055207,(float)-.0017859,(float)-8.6799e-7,(float)
	    -.0043181,(float)7.5236e-6,(float)6320.,(float)6330.,(float)
	    .10057,(float).088588,(float)-.0030519,(float)4.1846e-6,(float)
	    -.0057894,(float)1.0937e-5,(float)6330.,(float)6340.,(float)
	    .031333,(float).065185,(float)-.0038296,(float)7.2424e-6,(float)
	    -.006643,(float)1.2937e-5,(float)6340.,(float)6350.,(float)
	    .033845,(float).06492,(float)-.0039811,(float)7.8455e-6,(float)
	    -.0067871,(float)1.3512e-5,(float)6350.,(float)6360.,(float)
	    .12178,(float).088198,(float)-.0034193,(float)5.6304e-6,(float)
	    -.0062387,(float)1.1334e-5,(float)6360.,(float)6370.,(float)
	    .15914,(float).081345,(float)-.0020733,(float)3.2783e-7,(float)
	    -.0048764,(float)5.9258e-6,(float)6370.,(float)6380.,(float)
	    .17605,(float).10382,(float)4.7396e-4,(float)-9.4825e-6,(float)
	    -.0022623,(float)-4.3366e-6,(float)6380.,(float)6390.,(float)
	    .10257,(float).11943,(float).0050797,(float)-2.6879e-5,(float)
	    .002564,(float)-2.3075e-5,(float)6390.,(float)6400.,(float)
	    .025341,(float).11996,(float).012613,(float)-5.5092e-5,(float)
	    .010613,(float)-5.3927e-5,(float)6400.,(float)6410.,(float)
	    .0020638,(float).13697,(float).024524,(float)-9.7954e-5,(float)
	    .025258,(float)-9.9948e-5,(float)6410.,(float)6420.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6420.,(
	    float)6430.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6430.,(float)6440.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6440.,(float)6450.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6450.,(
	    float)6460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6460.,(float)6470.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6470.,(float)6480.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6480.,(
	    float)6490.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6490.,(float)6500.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6500.,(float)6510.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6510.,(
	    float)6520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6520.,(float)6530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6530.,(float)6540.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6540.,(
	    float)6550.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6550.,(float)6560.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6560.,(float)6570.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6570.,(
	    float)6580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6580.,(float)6590.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6590.,(float)6600.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6600.,(
	    float)6610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6610.,(float)6620.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6620.,(float)6630.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6630.,(
	    float)6640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6640.,(float)6650.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6650.,(float)6660.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6660.,(
	    float)6670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6670.,(float)6680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6680.,(float)6690.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6690.,(
	    float)6700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6700.,(float)6710.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6710.,(float)6720.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6720.,(
	    float)6730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6730.,(float)6740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6740.,(float)6750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6750.,(
	    float)6760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6760.,(float)6770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6770.,(float)6780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6780.,(
	    float)6790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6790.,(float)6800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6800.,(float)6810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6810.,(
	    float)6820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6820.,(float)6830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6830.,(float)6840.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6840.,(
	    float)6850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6850.,(float)6860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6860.,(float)6870.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6870.,(
	    float)6880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6880.,(float)6890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6890.,(float)6900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6900.,(
	    float)6910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6910.,(float)6920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6920.,(float)6930.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6930.,(
	    float)6940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6940.,(float)6950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6950.,(float)6960.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6960.,(
	    float)6970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6970.,(float)6980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6980.,(float)6990.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6990.,(
	    float)7e3,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7e3,(float)7010.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7010.,(float)7020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7020.,(
	    float)7030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7030.,(float)7040.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7040.,(float)7050.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7050.,(
	    float)7060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
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
	    float)0.,(float)7240.,(float)7250.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7250.,(float)7260.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7260.,(
	    float)7270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7270.,(float)7280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7280.,(float)7290.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7290.,(
	    float)7300.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7300.,(float)7310.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7310.,(float)7320.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7320.,(
	    float)7330.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7330.,(float)7340.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7340.,(float)7350.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7350.,(
	    float)7360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7360.,(float)7370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7370.,(float)7380.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7380.,(
	    float)7390.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7390.,(float)7400.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7400.,(float)7410.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7410.,(
	    float)7420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7420.,(float)7430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7430.,(float)7440.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7440.,(
	    float)7450.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7450.,(float)7460.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7460.,(float)7470.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7470.,(
	    float)7480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7480.,(float)7490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7490.,(float)7500.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7500.,(
	    float)7510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7510.,(float)7520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7520.,(float)7530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7530.,(
	    float)7540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7540.,(float)7550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7550.,(float)7560.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7560.,(
	    float)7570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7570.,(float)7580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7580.,(float)7590.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7590.,(
	    float)7600.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7600.,(float)7610.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7610.,(float)7620. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     carbon monoxide (5060 - 7610 cm-1) */

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
} /* moca2_ */

#ifdef __cplusplus
	}
#endif
