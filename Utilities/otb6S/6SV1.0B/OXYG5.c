/* OXYG5.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    oxyg5(a,inu) >*/
/* Subroutine */ int oxyg5_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)12740.,(float)
	    12750.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12750.,(float)12760.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12760.,(float)12770.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12770.,(float)12780.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12780.,(float)12790.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12790.,(float)
	    12800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12800.,(float)12810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12810.,(float)12820.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12820.,(float)12830.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12830.,(float)12840.,(float)1.4615e-8,(
	    float).036385,(float).0669,(float)-2.676e-4,(float).064873,(float)
	    -2.6352e-4,(float)12840.,(float)12850.,(float)1.7551e-8,(float)
	    .018194,(float).063056,(float)-2.5222e-4,(float).061029,(float)
	    -2.4814e-4,(float)12850.,(float)12860.,(float)5.5571e-8,(float)
	    .034936,(float).060516,(float)-2.4079e-4,(float).058832,(float)
	    -2.3863e-4,(float)12860.,(float)12870.,(float)1.1652e-7,(float)
	    .036823,(float).057157,(float)-2.2751e-4,(float).055426,(float)
	    -2.2511e-4,(float)12870.,(float)12880.,(float)2.2758e-7,(float)
	    .037524,(float).05406,(float)-2.1528e-4,(float).052277,(float)
	    -2.1261e-4,(float)12880.,(float)12890.,(float)4.1844e-7,(float)
	    .044301,(float).051439,(float)-2.03e-4,(float).050656,(float)
	    -2.0073e-4,(float)12890.,(float)12900.,(float)1.1196e-6,(float)
	    .064417,(float).048398,(float)-1.9212e-4,(float).047119,(float)
	    -1.8843e-4,(float)12900.,(float)12910.,(float)1.3443e-6,(float)
	    .056296,(float).046024,(float)-1.7943e-4,(float).045942,(float)
	    -1.7773e-4,(float)12910.,(float)12920.,(float)1.9719e-6,(float)
	    .064603,(float).044605,(float)-1.7094e-4,(float).044515,(float)
	    -1.7298e-4,(float)12920.,(float)12930.,(float)2.6027e-6,(float)
	    .060089,(float).042943,(float)-1.6604e-4,(float).04177,(float)
	    -1.6646e-4,(float)12930.,(float)12940.,(float)5.7954e-6,(float)
	    .10762,(float).040374,(float)-1.577e-4,(float).038909,(float)
	    -1.5614e-4,(float)12940.,(float)12950.,(float)4.3014e-6,(float)
	    .084704,(float).042188,(float)-1.5762e-4,(float).040357,(float)
	    -1.5977e-4,(float)12950.,(float)12960.,(float)7.9614e-6,(float)
	    .097284,(float).041471,(float)-1.6264e-4,(float).038436,(float)
	    -1.5588e-4,(float)12960.,(float)12970.,(float)1.7998e-5,(float)
	    .071534,(float).03844,(float)-1.5372e-4,(float).036186,(float)
	    -1.4871e-4,(float)12970.,(float)12980.,(float)2.7733e-5,(float)
	    .06553,(float).0349,(float)-1.3823e-4,(float).034326,(float)
	    -1.4018e-4,(float)12980.,(float)12990.,(float)8.7111e-5,(float)
	    .082142,(float).031863,(float)-1.2227e-4,(float).033009,(float)
	    -1.3017e-4,(float)12990.,(float)1.3e4,(float)5.9912e-5,(float)
	    .064608,(float).031064,(float)-1.1475e-4,(float).036751,(float)
	    -1.2672e-4,(float)1.3e4,(float)13010.,(float)2.6987e-4,(float)
	    .037736,(float).026012,(float)-1.039e-4,(float).024588,(float)
	    -9.7152e-5,(float)13010.,(float)13020.,(float)5.9914e-4,(float)
	    .037261,(float).022365,(float)-8.9458e-5,(float).020337,(float)
	    -8.5375e-5,(float)13020.,(float)13030.,(float).0012384,(float)
	    .038141,(float).019,(float)-7.5998e-5,(float).016972,(float)
	    -7.1916e-5,(float)13030.,(float)13040.,(float).0023802,(float)
	    .03944,(float).015907,(float)-6.3629e-5,(float).01388,(float)
	    -5.9546e-5,(float)13040.,(float)13050.,(float).0078692,(float)
	    .059793,(float).01191,(float)-4.6882e-5,(float).010043,(float)
	    -4.3832e-5,(float)13050.,(float)13060.,(float).014112,(float)
	    .0632,(float).0088039,(float)-3.4775e-5,(float).0068655,(float)
	    -3.1246e-5,(float)13060.,(float)13070.,(float).01506,(float)
	    .044198,(float).0062748,(float)-2.5099e-5,(float).0042475,(float)
	    -2.1017e-5,(float)13070.,(float)13080.,(float).019365,(float)
	    .045501,(float).0045529,(float)-1.8211e-5,(float).0025253,(float)
	    -1.4128e-5,(float)13080.,(float)13090.,(float).035332,(float)
	    .070599,(float).0026881,(float)-1.0606e-5,(float)6.7201e-4,(float)
	    -6.6426e-6,(float)13090.,(float)13100.,(float).032021,(float)
	    .071785,(float).0013302,(float)-5.2395e-6,(float)-7.0181e-4,(
	    float)-1.1794e-6,(float)13100.,(float)13110.,(float).020424,(
	    float).076507,(float)3.4001e-4,(float)-1.3505e-6,(float)-.0017047,
	    (float)2.7972e-6,(float)13110.,(float)13120.,(float).0083211,(
	    float).050488,(float)6.3467e-5,(float)-2.5366e-7,(float)-.0019594,
	    (float)3.8105e-6,(float)13120.,(float)13130.,(float).039472,(
	    float).095766,(float)7.6617e-4,(float)-3.019e-6,(float)-.0012832,(
	    float)1.1287e-6,(float)13130.,(float)13140.,(float).057473,(float)
	    .11569,(float).0028314,(float)-1.0907e-5,(float)8.5516e-4,(float)
	    -7.1029e-6,(float)13140.,(float)13150.,(float).046463,(float)
	    .16239,(float).0072656,(float)-2.6424e-5,(float).0059616,(float)
	    -2.4138e-5,(float)13150.,(float)13160.,(float).0070486,(float)
	    .18365,(float).016807,(float)-5.8784e-5,(float).017894,(float)
	    -6.4531e-5,(float)13160.,(float)13170.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)13170.,(float)13180.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13180.,(float)13190.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13190.,(float)13200.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13200.,(float)
	    13210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13210.,(float)13220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13220.,(float)13230.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13230.,(float)13240.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13240.,(float)13250.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13250.,(float)
	    13260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13260.,(float)13270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13270.,(float)13280.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13280.,(float)13290.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13290.,(float)13300.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13300.,(float)
	    13310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13310.,(float)13320.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13320.,(float)13330.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13330.,(float)13340.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13340.,(float)13350.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13350.,(float)
	    13360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13360.,(float)13370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13370.,(float)13380.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13380.,(float)13390.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13390.,(float)13400.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13400.,(float)
	    13410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13410.,(float)13420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13420.,(float)13430.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13430.,(float)13440.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13440.,(float)13450.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13450.,(float)
	    13460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13460.,(float)13470.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13470.,(float)13480.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13480.,(float)13490.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13490.,(float)13500.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13500.,(float)
	    13510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13510.,(float)13520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13520.,(float)13530.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13530.,(float)13540.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13540.,(float)13550.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13550.,(float)
	    13560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13560.,(float)13570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13570.,(float)13580.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13580.,(float)13590.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13590.,(float)13600.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13600.,(float)
	    13610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13610.,(float)13620.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13620.,(float)13630.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13630.,(float)13640.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13640.,(float)13650.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13650.,(float)
	    13660.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13660.,(float)13670.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13670.,(float)13680.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13680.,(float)13690.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13690.,(float)13700.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13700.,(float)
	    13710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13710.,(float)13720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13720.,(float)13730.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13730.,(float)13740.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13740.,(float)13750.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13750.,(float)
	    13760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13760.,(float)13770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13770.,(float)13780.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13780.,(float)13790.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13790.,(float)13800.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13800.,(float)
	    13810.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13810.,(float)13820.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13820.,(float)13830.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13830.,(float)13840.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13840.,(float)13850.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13850.,(float)
	    13860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13860.,(float)13870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13870.,(float)13880.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13880.,(float)13890.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13890.,(float)13900.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13900.,(float)
	    13910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13910.,(float)13920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13920.,(float)13930.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13930.,(float)13940.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13940.,(float)13950.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13950.,(float)
	    13960.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13960.,(float)13970.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13970.,(float)13980.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13980.,(float)13990.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13990.,(float)1.4e4,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)1.4e4,(float)
	    14010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14010.,(float)14020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14020.,(float)14030.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14030.,(float)14040.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14040.,(float)14050.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14050.,(float)
	    14060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14060.,(float)14070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14070.,(float)14080.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14080.,(float)14090.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14090.,(float)14100.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14100.,(float)
	    14110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14110.,(float)14120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14120.,(float)14130.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14130.,(float)14140.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14140.,(float)14150.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14150.,(float)
	    14160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14160.,(float)14170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14170.,(float)14180.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14180.,(float)14190.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14190.,(float)14200.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14200.,(float)
	    14210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14210.,(float)14220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14220.,(float)14230.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14230.,(float)14240.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14240.,(float)14250.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14250.,(float)
	    14260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14260.,(float)14270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14270.,(float)14280.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14280.,(float)14290.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14290.,(float)14300.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14300.,(float)
	    14310.,(float)3.2848e-8,(float).036386,(float).053505,(float)
	    -2.1402e-4,(float).051477,(float)-2.0994e-4,(float)14310.,(float)
	    14320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14320.,(float)14330.,(float)1.1021e-7,(float).036386,(
	    float).048248,(float)-1.9299e-4,(float).046221,(float)-1.8891e-4,(
	    float)14330.,(float)14340.,(float)3.4571e-7,(float).036386,(float)
	    .04326,(float)-1.7304e-4,(float).041233,(float)-1.6896e-4,(float)
	    14340.,(float)14350.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14350.,(float)14360.,(float)1.0131e-6,(
	    float).036386,(float).03854,(float)-1.5416e-4,(float).036513,(
	    float)-1.5008e-4,(float)14360.,(float)14370.,(float)2.7746e-6,(
	    float).036442,(float).034089,(float)-1.3635e-4,(float).032054,(
	    float)-1.3222e-4,(float)14370.,(float)14380.,(float)3.6059e-6,(
	    float).01824,(float).02993,(float)-1.1971e-4,(float).027892,(
	    float)-1.1557e-4,(float)14380.,(float)14390.,(float)3.4852e-6,(
	    float).018279,(float).029884,(float)-1.1951e-4,(float).027832,(
	    float)-1.1527e-4,(float)14390.,(float)14400.,(float)1.6898e-5,(
	    float).036461,(float).026,(float)-1.0399e-4,(float).023962,(float)
	    -9.9845e-5,(float)14400.,(float)14410.,(float)3.7525e-5,(float)
	    .037306,(float).022363,(float)-8.9448e-5,(float).020327,(float)
	    -8.5313e-5,(float)14410.,(float)14420.,(float)7.7568e-5,(float)
	    .038179,(float).018999,(float)-7.5991e-5,(float).016964,(float)
	    -7.1868e-5,(float)14420.,(float)14430.,(float)7.644e-5,(float)
	    .019568,(float).015927,(float)-6.3702e-5,(float).013885,(float)
	    -5.9534e-5,(float)14430.,(float)14440.,(float)2.0947e-4,(float)
	    .038957,(float).014055,(float)-5.5384e-5,(float).012236,(float)
	    -5.2506e-5,(float)14440.,(float)14450.,(float)3.5608e-4,(float)
	    .040125,(float).011458,(float)-4.5137e-5,(float).0095851,(float)
	    -4.1989e-5,(float)14450.,(float)14460.,(float)5.6079e-4,(float)
	    .041622,(float).0091259,(float)-3.5944e-5,(float).0072131,(float)
	    -3.2578e-5,(float)14460.,(float)14470.,(float)8.1523e-4,(float)
	    .043384,(float).0070608,(float)-2.7807e-5,(float).0051286,(float)
	    -2.4309e-5,(float)14470.,(float)14480.,(float).0016639,(float)
	    .067344,(float).0050076,(float)-1.9758e-5,(float).0030273,(float)
	    -1.599e-5,(float)14480.,(float)14490.,(float).0014146,(float)
	    .046358,(float).003106,(float)-1.2424e-5,(float).0010767,(float)
	    -8.3329e-6,(float)14490.,(float)14500.,(float).0022165,(float)
	    .071957,(float).0016398,(float)-6.477e-6,(float)-3.8835e-4,(float)
	    -2.4326e-6,(float)14500.,(float)14510.,(float).0015542,(float)
	    .07478,(float)6.4291e-4,(float)-2.5306e-6,(float)-.0013922,(float)
	    1.5886e-6,(float)14510.,(float)14520.,(float)3.1245e-4,(float)
	    .026878,(float)9.5018e-5,(float)-3.8003e-7,(float)-.0019316,(
	    float)3.6996e-6,(float)14520.,(float)14530.,(float).0015805,(
	    float).097644,(float)3.0344e-4,(float)-1.0773e-6,(float)-.0013616,
	    (float)5.954e-6,(float)14530.,(float)14540.,(float).0044256,(
	    float).14544,(float).0020504,(float)-7.4913e-6,(float)6.2397e-4,(
	    float)-9.5059e-7,(float)14540.,(float)14550.,(float).0039335,(
	    float).29035,(float).0078502,(float)-2.3749e-5,(float).0080984,(
	    float)-2.7655e-5,(float)14550.,(float)14560.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)14560.,(float)
	    14570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14570.,(float)14580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14580.,(float)14590.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14590.,(float)14600.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14600.,(float)14610.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14610.,(float)
	    14620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14620.,(float)14630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14630.,(float)14640.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14640.,(float)14650.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14650.,(float)14660.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14660.,(float)
	    14670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14670.,(float)14680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14680.,(float)14690.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14690.,(float)14700.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14700.,(float)14710.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14710.,(float)
	    14720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14720.,(float)14730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14730.,(float)14740.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14740.,(float)14750.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14750.,(float)14760.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14760.,(float)
	    14770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14770.,(float)14780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14780.,(float)14790.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14790.,(float)14800.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14800.,(float)14810.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14810.,(float)
	    14820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14820.,(float)14830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14830.,(float)14840.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14840.,(float)14850.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14850.,(float)14860.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14860.,(float)
	    14870.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14870.,(float)14880.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14880.,(float)14890.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14890.,(float)14900.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14900.,(float)14910.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14910.,(float)
	    14920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14920.,(float)14930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14930.,(float)14940.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14940.,(float)14950.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14950.,(float)14960.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14960.,(float)
	    14970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14970.,(float)14980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14980.,(float)14990.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14990.,(float)1.5e4,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)1.5e4,(float)15010.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)15010.,(float)
	    15020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15020.,(float)15030.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15030.,(float)15040.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15040.,(float)15050.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15050.,(float)15060.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15060.,(float)
	    15070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15070.,(float)15080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15080.,(float)15090.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15090.,(float)15100.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15100.,(float)15110.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15110.,(float)
	    15120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15120.,(float)15130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15130.,(float)15140.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15140.,(float)15150.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15150.,(float)15160.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15160.,(float)
	    15170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15170.,(float)15180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15180.,(float)15190.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15190.,(float)15200.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15200.,(float)15210.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15210.,(float)
	    15220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15220.,(float)15230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15230.,(float)15240.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15240.,(float)15250.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15250.,(float)15260.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15260.,(float)
	    15270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15270.,(float)15280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15280.,(float)15290.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15290.,(float)15300. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     oxygen (12740 - 15290 cm-1) */

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
} /* oxyg5_ */

#ifdef __cplusplus
	}
#endif
