/* WAVA5.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       SUBROUTINE    WAVA5(a,inu) >*/
/* Subroutine */ int wava5_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)4.6416e-4,(
	    float).04653,(float).011484,(float)-5.0228e-5,(float).0057564,(
	    float)-2.8823e-5,(float)12740.,(float)12750.,(float)2.6026e-5,(
	    float).069686,(float).0050381,(float)-3.0969e-5,(float).0023565,(
	    float)-2.6498e-5,(float)12750.,(float)12760.,(float)2.1016e-4,(
	    float).078469,(float)-.0024738,(float)-2.0423e-6,(float)-.0056905,
	    (float)4.9529e-6,(float)12760.,(float)12770.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)12770.,(float)
	    12780.,(float)6.0427e-5,(float).037767,(float)5.4219e-4,(float)
	    -1.4563e-5,(float)-.002215,(float)-9.0107e-6,(float)12780.,(float)
	    12790.,(float)2.496e-5,(float).041123,(float)-.0026523,(float)
	    -1.7845e-6,(float)-.0054095,(float)3.7673e-6,(float)12790.,(float)
	    12800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)12800.,(float)12810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)12810.,(float)12820.,(float)
	    6.3228e-5,(float).037767,(float)-6.7259e-4,(float)-9.7034e-6,(
	    float)-.0034298,(float)-4.1516e-6,(float)12820.,(float)12830.,(
	    float)7.0635e-5,(float).034814,(float).0017376,(float)-1.9344e-5,(
	    float)-.0010196,(float)-1.3792e-5,(float)12830.,(float)12840.,(
	    float)3.5776e-5,(float).068419,(float).0046121,(float)-2.7372e-5,(
	    float).0015491,(float)-2.2291e-5,(float)12840.,(float)12850.,(
	    float)8.2613e-5,(float).072444,(float).0029409,(float)-2.2687e-5,(
	    float)1.2304e-4,(float)-1.7626e-5,(float)12850.,(float)12860.,(
	    float)1.3149e-5,(float).034814,(float).0017682,(float)-1.9467e-5,(
	    float)-9.8895e-4,(float)-1.3915e-5,(float)12860.,(float)12870.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12870.,(float)12880.,(float)1.0952e-5,(float).034814,(float)
	    .0039164,(float)-2.8059e-5,(float).0011592,(float)-2.2508e-5,(
	    float)12880.,(float)12890.,(float)1.1287e-5,(float).037767,(float)
	    .0031085,(float)-2.4828e-5,(float)3.5131e-4,(float)-1.9276e-5,(
	    float)12890.,(float)12900.,(float)1.2534e-5,(float).034814,(float)
	    .006137,(float)-3.6942e-5,(float).0033798,(float)-3.139e-5,(float)
	    12900.,(float)12910.,(float)1.9577e-5,(float).031771,(float)
	    .0046608,(float)-3.1037e-5,(float).0019036,(float)-2.5485e-5,(
	    float)12910.,(float)12920.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)12920.,(float)12930.,(float)
	    9.153e-6,(float).028818,(float).0079937,(float)-4.4369e-5,(float)
	    .0052366,(float)-3.8817e-5,(float)12930.,(float)12940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12940.,(
	    float)12950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)12950.,(float)12960.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)12960.,(float)12970.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    12970.,(float)12980.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)12980.,(float)12990.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)12990.,(float)
	    1.3e4,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)1.3e4,(float)13010.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)13010.,(float)13020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13020.,(
	    float)13030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)13030.,(float)13040.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)13040.,(float)13050.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13050.,(float)13060.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13060.,(float)13070.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13070.,(float)
	    13080.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13080.,(float)13090.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13090.,(float)13100.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13100.,(float)13110.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13110.,(float)13120.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13120.,(float)
	    13130.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13130.,(float)13140.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13140.,(float)13150.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13150.,(float)13160.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13160.,(float)13170.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13170.,(float)
	    13180.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13180.,(float)13190.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13190.,(float)13200.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13200.,(float)13210.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13210.,(float)13220.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13220.,(float)
	    13230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13230.,(float)13240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13240.,(float)13250.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13250.,(float)13260.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13260.,(float)13270.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13270.,(float)
	    13280.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13280.,(float)13290.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13290.,(float)13300.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    13300.,(float)13310.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)13310.,(float)13320.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)13320.,(float)
	    13330.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)13330.,(float)13340.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)13340.,(float)13350.,(float)
	    3.4847e-5,(float).04882,(float).025041,(float)-1.1142e-4,(float)
	    .022291,(float)-1.0647e-4,(float)13350.,(float)13360.,(float)
	    6.4041e-5,(float).10448,(float).01454,(float)-4.7733e-5,(float)
	    .013707,(float)-6.0524e-5,(float)13360.,(float)13370.,(float)
	    4.8972e-5,(float).061588,(float).015816,(float)-3.1234e-5,(float)
	    .012279,(float)-4.4662e-5,(float)13370.,(float)13380.,(float)
	    3.4101e-4,(float).20633,(float).0080284,(float)-9.6908e-6,(float)
	    .0076822,(float)-2.8886e-5,(float)13380.,(float)13390.,(float)
	    1.6731e-5,(float).03468,(float).029361,(float)-1.2984e-4,(float)
	    .026604,(float)-1.2428e-4,(float)13390.,(float)13400.,(float)
	    3.9582e-4,(float).10507,(float).018729,(float)-6.1046e-5,(float)
	    .015312,(float)-6.0499e-5,(float)13400.,(float)13410.,(float)
	    5.7395e-4,(float).20422,(float).012292,(float)-3.0378e-5,(float)
	    .010954,(float)-4.4352e-5,(float)13410.,(float)13420.,(float)
	    1.6304e-4,(float).075327,(float).024345,(float)-1.0453e-4,(float)
	    .022312,(float)-1.0356e-4,(float)13420.,(float)13430.,(float)
	    .0014276,(float).27047,(float).017891,(float)-7.3505e-5,(float)
	    .016458,(float)-7.1692e-5,(float)13430.,(float)13440.,(float)
	    3.2739e-4,(float).19765,(float).021521,(float)-5.2027e-5,(float)
	    .020205,(float)-6.8454e-5,(float)13440.,(float)13450.,(float)
	    8.4176e-4,(float).22651,(float).014704,(float)-5.7138e-5,(float)
	    .012449,(float)-5.5586e-5,(float)13450.,(float)13460.,(float)
	    .003321,(float).26989,(float).014889,(float)-6.1022e-5,(float)
	    .014319,(float)-6.4594e-5,(float)13460.,(float)13470.,(float)
	    .0020184,(float).15068,(float).014242,(float)-5.0382e-5,(float)
	    .014352,(float)-5.2958e-5,(float)13470.,(float)13480.,(float)
	    .0034847,(float).23656,(float).01296,(float)-5.4713e-5,(float)
	    .011654,(float)-5.3532e-5,(float)13480.,(float)13490.,(float)
	    .0051848,(float).22152,(float).01005,(float)-4.214e-5,(float)
	    .010802,(float)-4.9706e-5,(float)13490.,(float)13500.,(float)
	    .002246,(float).20257,(float).011509,(float)-1.145e-6,(float)
	    .012567,(float)-2.9901e-5,(float)13500.,(float)13510.,(float)
	    .01207,(float).22916,(float).0070442,(float)-3.1499e-5,(float)
	    .0063368,(float)-3.3814e-5,(float)13510.,(float)13520.,(float)
	    .011903,(float).21411,(float).006383,(float)-2.2698e-5,(float)
	    .0062241,(float)-2.6759e-5,(float)13520.,(float)13530.,(float)
	    .009548,(float).21575,(float).0044827,(float)-1.1921e-5,(float)
	    .0055699,(float)-2.5927e-5,(float)13530.,(float)13540.,(float)
	    .011523,(float).40682,(float).0091253,(float)-1.9397e-5,(float)
	    .0079449,(float)-3.2998e-5,(float)13540.,(float)13550.,(float)
	    .0084497,(float).29293,(float).0047841,(float)-1.1629e-5,(float)
	    .0057988,(float)-2.5164e-5,(float)13550.,(float)13560.,(float)
	    .026919,(float).27673,(float).0019766,(float)-3.7751e-6,(float)
	    .0016743,(float)-8.638e-6,(float)13560.,(float)13570.,(float)
	    .021945,(float).3387,(float).005114,(float)3.7089e-6,(float)
	    .0063004,(float)-2.3091e-5,(float)13570.,(float)13580.,(float)
	    .021982,(float).18246,(float)3.6494e-4,(float)1.1207e-6,(float)
	    -3.7751e-4,(float)-1.4267e-6,(float)13580.,(float)13590.,(float)
	    .010404,(float).21217,(float).0039463,(float)5.5125e-6,(float)
	    .0030886,(float)-1.2313e-5,(float)13590.,(float)13600.,(float)
	    .022785,(float).35675,(float).0080322,(float)-4.828e-6,(float)
	    .0057084,(float)-1.9313e-5,(float)13600.,(float)13610.,(float)
	    .021944,(float).17661,(float).0028308,(float)8.2581e-6,(float)
	    .0014574,(float)-4.9721e-6,(float)13610.,(float)13620.,(float)
	    .015009,(float).16917,(float).001106,(float)1.4866e-5,(float)
	    .001036,(float)-3.5775e-6,(float)13620.,(float)13630.,(float)
	    .028056,(float).27826,(float).010985,(float)-5.284e-5,(float)
	    .0083121,(float)-4.5055e-5,(float)13630.,(float)13640.,(float)
	    .017905,(float).21652,(float).0091202,(float)-4.3033e-5,(float)
	    .0064523,(float)-3.6725e-5,(float)13640.,(float)13650.,(float)
	    .040842,(float).16141,(float).002459,(float)-4.3246e-6,(float)
	    2.4001e-4,(float)-1.8088e-6,(float)13650.,(float)13660.,(float)
	    .064344,(float).36355,(float).002624,(float)-5.0938e-6,(float)
	    8.4629e-4,(float)-7.2912e-6,(float)13660.,(float)13670.,(float)
	    .098009,(float).46873,(float).003736,(float)-2.2472e-5,(float)
	    .0012376,(float)-1.8409e-5,(float)13670.,(float)13680.,(float)
	    .10223,(float).26441,(float).0043146,(float)-2.6517e-5,(float)
	    .003082,(float)-2.1843e-5,(float)13680.,(float)13690.,(float)
	    .051383,(float).36953,(float).0029461,(float)-4.1201e-6,(float)
	    .001609,(float)-1.0556e-5,(float)13690.,(float)13700.,(float)
	    .079035,(float).41629,(float).0039863,(float)-1.0096e-5,(float)
	    .0020593,(float)-1.5846e-5,(float)13700.,(float)13710.,(float)
	    .17862,(float).23727,(float)7.0664e-4,(float)-1.3429e-5,(float)
	    -.0024735,(float)-6.33e-6,(float)13710.,(float)13720.,(float)
	    .023457,(float).12905,(float)-.0022038,(float)-1.2856e-6,(float)
	    -.0036094,(float)1.0306e-6,(float)13720.,(float)13730.,(float)
	    .1498,(float).27847,(float)-7.0337e-4,(float)-8.8658e-6,(float)
	    -.0037774,(float)-2.5192e-6,(float)13730.,(float)13740.,(float)
	    .15951,(float).19705,(float)-.0012269,(float)-5.7441e-6,(float)
	    -.0026138,(float)-3.9795e-6,(float)13740.,(float)13750.,(float)
	    .030444,(float).2508,(float).001339,(float)-3.667e-6,(float)
	    .0013056,(float)-7.9865e-6,(float)13750.,(float)13760.,(float)
	    .091463,(float).42355,(float).0015252,(float)-5.082e-6,(float)
	    1.0296e-4,(float)-1.0731e-5,(float)13760.,(float)13770.,(float)
	    .055724,(float).23315,(float)5.0264e-4,(float)-5.1006e-7,(float)
	    -.0018097,(float)-3.8866e-6,(float)13770.,(float)13780.,(float)
	    .20577,(float).30719,(float)-.0013969,(float)1.9333e-6,(float)
	    -.0029895,(float)-5.1114e-7,(float)13780.,(float)13790.,(float)
	    .076501,(float).36547,(float).0034715,(float)-2.4093e-5,(float)
	    .0010916,(float)-1.9569e-5,(float)13790.,(float)13800.,(float)
	    .26798,(float).22416,(float)-.0016316,(float)-1.9581e-6,(float)
	    -.0041508,(float)1.1385e-6,(float)13800.,(float)13810.,(float)
	    .20338,(float).16881,(float)-.0022177,(float)-1.1382e-6,(float)
	    -.0032452,(float)-1.5814e-7,(float)13810.,(float)13820.,(float)
	    .12951,(float).12548,(float)-.0038707,(float)6.337e-6,(float)
	    -.0045229,(float)6.1639e-6,(float)13820.,(float)13830.,(float)
	    .088731,(float).17991,(float)-.0038169,(float)7.3459e-6,(float)
	    -.0042642,(float)6.6757e-6,(float)13830.,(float)13840.,(float)
	    .0067788,(float).12625,(float).0098132,(float)-4.8875e-5,(float)
	    .007723,(float)-4.7347e-5,(float)13840.,(float)13850.,(float)
	    .010502,(float).21694,(float)8.6711e-4,(float)4.6135e-6,(float)
	    .0011488,(float)-8.9674e-6,(float)13850.,(float)13860.,(float)
	    .04906,(float).16058,(float)-.0030216,(float)1.0044e-5,(float)
	    -.0024328,(float)6.7471e-6,(float)13860.,(float)13870.,(float)
	    .24278,(float).15261,(float)-.0051224,(float)8.9719e-6,(float)
	    -.0065887,(float)1.2603e-5,(float)13870.,(float)13880.,(float)
	    .27304,(float).29617,(float)-.003541,(float)3.4588e-6,(float)
	    -.0050128,(float)8.099e-6,(float)13880.,(float)13890.,(float)
	    .13252,(float).25864,(float)-.0015695,(float)-2.898e-6,(float)
	    -.002959,(float)-3.2727e-6,(float)13890.,(float)13900.,(float)
	    .27275,(float).14969,(float)-.0015578,(float)-4.3912e-6,(float)
	    -.0023477,(float)-2.7675e-6,(float)13900.,(float)13910.,(float)
	    .28731,(float).24848,(float)-2.6722e-4,(float)-9.0595e-6,(float)
	    -.002097,(float)-6.6308e-6,(float)13910.,(float)13920.,(float)
	    .20952,(float).31628,(float).0028828,(float)-2.1023e-5,(float)
	    9.3184e-4,(float)-1.7842e-5,(float)13920.,(float)13930.,(float)
	    .12621,(float).34509,(float).0047187,(float)-2.6132e-5,(float)
	    .0021269,(float)-2.2662e-5,(float)13930.,(float)13940.,(float)
	    .082581,(float).35712,(float).0065235,(float)-2.9651e-5,(float)
	    .0046608,(float)-2.9306e-5,(float)13940.,(float)13950.,(float)
	    .02215,(float).56251,(float).011273,(float)-2.023e-5,(float)
	    .0098905,(float)-3.5866e-5,(float)13950.,(float)13960.,(float)
	    .013395,(float).23595,(float).0075697,(float)-1.1677e-5,(float)
	    .0047404,(float)-1.3519e-5,(float)13960.,(float)13970.,(float)
	    .0097251,(float).389,(float).0045853,(float)-4.4223e-6,(float)
	    .0013332,(float)-8.8812e-6,(float)13970.,(float)13980.,(float)
	    .01416,(float).3192,(float).0014413,(float)7.7474e-7,(float)
	    .0010611,(float)-6.6038e-6,(float)13980.,(float)13990.,(float)
	    .0064431,(float).38168,(float).0021106,(float)-5.5268e-6,(float)
	    6.2478e-4,(float)-4.2781e-6,(float)13990.,(float)1.4e4,(float)
	    .0074763,(float).21449,(float)8.6863e-6,(float)-4.7583e-6,(float)
	    -.0024648,(float)-3.1646e-6,(float)1.4e4,(float)14010.,(float)
	    .0044339,(float).31044,(float).0062951,(float)-8.282e-6,(float)
	    .00239,(float)-1.0886e-5,(float)14010.,(float)14020.,(float)
	    .0063698,(float).37192,(float).0035714,(float)-7.0127e-6,(float)
	    .0022531,(float)-1.3753e-5,(float)14020.,(float)14030.,(float)
	    .0030618,(float).32199,(float).0067837,(float)-4.6145e-6,(float)
	    .0056415,(float)-1.9199e-5,(float)14030.,(float)14040.,(float)
	    .0045836,(float).19592,(float).0034269,(float)-6.7013e-6,(float)
	    .0015603,(float)-2.0576e-6,(float)14040.,(float)14050.,(float)
	    .0028921,(float).26752,(float).0054727,(float)-8.232e-6,(float)
	    .0034172,(float)-1.6235e-5,(float)14050.,(float)14060.,(float)
	    .0083604,(float).491,(float).0036871,(float)-2.6932e-6,(float)
	    .0022258,(float)-1.2432e-5,(float)14060.,(float)14070.,(float)
	    .0048958,(float).40049,(float).0028518,(float)-8.9707e-6,(float)
	    7.3805e-4,(float)-1.0552e-5,(float)14070.,(float)14080.,(float)
	    .0060175,(float).66182,(float).0036922,(float)-1.2082e-5,(float)
	    .002131,(float)-1.7251e-5,(float)14080.,(float)14090.,(float)
	    .0051267,(float).30376,(float).012373,(float)-5.2388e-5,(float)
	    .0092416,(float)-4.9701e-5,(float)14090.,(float)14100.,(float)
	    .0051874,(float).37923,(float).0085604,(float)-3.2476e-5,(float)
	    .0048279,(float)-2.831e-5,(float)14100.,(float)14110.,(float)
	    .0037549,(float).24752,(float).0026234,(float)1.353e-6,(float)
	    1.7089e-4,(float)-5.9579e-6,(float)14110.,(float)14120.,(float)
	    .008547,(float).48393,(float).0055118,(float)-1.2616e-5,(float)
	    .0021682,(float)-1.5595e-5,(float)14120.,(float)14130.,(float)
	    .007167,(float).42356,(float).0073835,(float)-3.8638e-5,(float)
	    .0043338,(float)-3.2395e-5,(float)14130.,(float)14140.,(float)
	    .0084611,(float).31558,(float).0047305,(float)-1.7484e-5,(float)
	    .0013223,(float)-1.1411e-5,(float)14140.,(float)14150.,(float)
	    .0086532,(float).27748,(float).005894,(float)-3.133e-5,(float)
	    .0032608,(float)-2.6952e-5,(float)14150.,(float)14160.,(float)
	    .012236,(float).32519,(float).0020656,(float)-1.1881e-5,(float)
	    -4.5247e-4,(float)-1.1093e-5,(float)14160.,(float)14170.,(float)
	    .019531,(float).25874,(float).0039764,(float)-2.6715e-5,(float)
	    .0012538,(float)-1.9528e-5,(float)14170.,(float)14180.,(float)
	    .0057931,(float).18836,(float).0039509,(float)-2.0877e-5,(float)
	    .001896,(float)-1.8677e-5,(float)14180.,(float)14190.,(float)
	    .0065709,(float).31403,(float).0030661,(float)-6.3795e-6,(float)
	    .0013225,(float)-1.1966e-5,(float)14190.,(float)14200.,(float)
	    .026818,(float).21236,(float).0016226,(float)-1.6759e-5,(float)
	    -4.3884e-4,(float)-1.1258e-5,(float)14200.,(float)14210.,(float)
	    .0015917,(float).17083,(float).0098825,(float)-2.9767e-5,(float)
	    .0059285,(float)-2.8608e-5,(float)14210.,(float)14220.,(float)
	    .036373,(float).30478,(float)9.7187e-4,(float)-8.1756e-6,(float)
	    8.1092e-5,(float)-1.0434e-5,(float)14220.,(float)14230.,(float)
	    .018351,(float).085618,(float)-.0015427,(float)-3.9022e-6,(float)
	    -.0026215,(float)7.5823e-7,(float)14230.,(float)14240.,(float)
	    .027421,(float).1672,(float)-.0016048,(float)1.04e-6,(float)
	    -.0021918,(float)-7.3937e-7,(float)14240.,(float)14250.,(float)
	    .019137,(float).34394,(float).0054701,(float)-3.1082e-5,(float)
	    .0021305,(float)-2.3894e-5,(float)14250.,(float)14260.,(float)
	    .011742,(float).24754,(float)-.0015398,(float)1.4005e-6,(float)
	    -.0032941,(float)1.7021e-7,(float)14260.,(float)14270.,(float)
	    .030406,(float).25107,(float)-.0021777,(float)1.1738e-6,(float)
	    -.0042308,(float)1.983e-6,(float)14270.,(float)14280.,(float)
	    .02572,(float).17272,(float).0014929,(float)-1.7124e-5,(float)
	    -2.0245e-4,(float)-1.5475e-5,(float)14280.,(float)14290.,(float)
	    .023411,(float).14155,(float)-.0036159,(float)5.4534e-6,(float)
	    -.0056547,(float)8.3323e-6,(float)14290.,(float)14300.,(float)
	    .050436,(float).17597,(float)-.0035358,(float)2.7001e-6,(float)
	    -.005998,(float)7.9894e-6,(float)14300.,(float)14310.,(float)
	    .010835,(float).16074,(float)-.0023328,(float)3.6258e-6,(float)
	    -.0036191,(float)6.6611e-7,(float)14310.,(float)14320.,(float)
	    .0097229,(float).1372,(float)-.0026043,(float)3.3682e-6,(float)
	    -.0039301,(float)2.3934e-6,(float)14320.,(float)14330.,(float)
	    .0041727,(float).26898,(float)8.9506e-4,(float)-3.3799e-6,(float)
	    -8.0987e-4,(float)-7.4366e-6,(float)14330.,(float)14340.,(float)
	    .0028313,(float).11399,(float)-.0023514,(float)1.9057e-8,(float)
	    -.0043191,(float)3.2086e-6,(float)14340.,(float)14350.,(float)
	    .0094087,(float).13847,(float)-.0033995,(float)5.3082e-6,(float)
	    -.0048841,(float)7.2565e-6,(float)14350.,(float)14360.,(float)
	    .045453,(float).1618,(float)-.0049821,(float)9.5517e-6,(float)
	    -.0062627,(float)1.3183e-5,(float)14360.,(float)14370.,(float)
	    .05478,(float).246,(float)-.003323,(float)3.3083e-6,(float)
	    -.0044595,(float)4.3249e-6,(float)14370.,(float)14380.,(float)
	    .057794,(float).29926,(float)-.0018524,(float)-2.6891e-6,(float)
	    -.0034713,(float)-1.4711e-6,(float)14380.,(float)14390.,(float)
	    .017789,(float).040974,(float)-5.5041e-4,(float)-9.0402e-6,(float)
	    -.0019035,(float)-4.1538e-6,(float)14390.,(float)14400.,(float)
	    .066509,(float).31802,(float)1.7874e-4,(float)-1.0469e-5,(float)
	    -.0014428,(float)-9.3839e-6,(float)14400.,(float)14410.,(float)
	    .033243,(float).21287,(float).0023294,(float)-1.8667e-5,(float)
	    -4.0745e-4,(float)-1.2383e-5,(float)14410.,(float)14420.,(float)
	    .030315,(float).32935,(float).005459,(float)-3.1244e-5,(float)
	    .0033095,(float)-2.8644e-5,(float)14420.,(float)14430.,(float)
	    .011921,(float).16637,(float).0064852,(float)-3.2883e-5,(float)
	    .0040894,(float)-2.982e-5,(float)14430.,(float)14440.,(float)
	    .0035817,(float).24062,(float).010713,(float)-2.2991e-5,(float)
	    .0076719,(float)-3.0879e-5,(float)14440.,(float)14450.,(float)
	    .0020434,(float).09533,(float).0074555,(float)-4.0758e-5,(float)
	    .003952,(float)-3.2281e-5,(float)14450.,(float)14460.,(float)
	    .0019531,(float).057626,(float).013393,(float)-6.572e-5,(float)
	    .010648,(float)-6.0338e-5,(float)14460.,(float)14470.,(float)
	    .0015972,(float).063531,(float)-.0028566,(float)-9.3552e-7,(float)
	    -.0057668,(float)5.2381e-6,(float)14470.,(float)14480.,(float)
	    6.208e-4,(float).14581,(float)-.0038143,(float)3.5516e-6,(float)
	    -.006417,(float)7.6586e-6,(float)14480.,(float)14490.,(float)
	    2.573e-4,(float).034903,(float).0017376,(float)-1.9344e-5,(float)
	    -.0010196,(float)-1.3792e-5,(float)14490.,(float)14500.,(float)
	    5.8326e-4,(float).07101,(float).0037987,(float)-2.7471e-5,(float)
	    .0011761,(float)-2.2502e-5,(float)14500.,(float)14510.,(float)
	    1.981e-4,(float).037767,(float)5.4219e-4,(float)-1.4563e-5,(float)
	    -.002215,(float)-9.0107e-6,(float)14510.,(float)14520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14520.,(
	    float)14530.,(float)6.1723e-4,(float).05996,(float).0029608,(
	    float)-2.3205e-5,(float)-5.41e-4,(float)-1.4715e-5,(float)14530.,(
	    float)14540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)14540.,(float)14550.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)14550.,(float)14560.,(
	    float)8.7384e-4,(float).20577,(float).001467,(float)-1.2035e-5,(
	    float)-.0013037,(float)-8.5219e-6,(float)14560.,(float)14570.,(
	    float)3.6981e-4,(float).15319,(float).0026086,(float)5.1176e-7,(
	    float)-9.4964e-4,(float)-4.1621e-6,(float)14570.,(float)14580.,(
	    float)5.3062e-4,(float).2039,(float)-9.2668e-4,(float)-1.7265e-6,(
	    float)-.0025209,(float)-3.9176e-6,(float)14580.,(float)14590.,(
	    float)2.3625e-4,(float).10952,(float)3.608e-5,(float)-6.6963e-7,(
	    float)-.0029777,(float)-5.5782e-7,(float)14590.,(float)14600.,(
	    float)2.5529e-4,(float).085175,(float).0086357,(float)-4.5158e-5,(
	    float).0055824,(float)-3.9246e-5,(float)14600.,(float)14610.,(
	    float)3.5469e-4,(float).17005,(float)5.5905e-4,(float)3.4517e-6,(
	    float)-.0012201,(float)-2.0532e-6,(float)14610.,(float)14620.,(
	    float)1.4962e-4,(float).070668,(float)3.6006e-4,(float)-1.3091e-5,
	    (float)-.002318,(float)-8.0334e-6,(float)14620.,(float)14630.,(
	    float)1.6283e-4,(float).043851,(float)-.0019069,(float)-4.7662e-6,
	    (float)-.0047857,(float)1.0305e-6,(float)14630.,(float)14640.,(
	    float)3.4849e-4,(float).091227,(float)1.957e-4,(float)-6.7027e-6,(
	    float)-.0019067,(float)-6.8831e-6,(float)14640.,(float)14650.,(
	    float)2.4098e-4,(float).073915,(float)5.8149e-4,(float)2.017e-6,(
	    float)-.0021878,(float)-6.9336e-7,(float)14650.,(float)14660.,(
	    float)1.8929e-4,(float).040095,(float)-9.767e-4,(float)-8.487e-6,(
	    float)-.003815,(float)-2.7719e-6,(float)14660.,(float)14670.,(
	    float)2.8551e-4,(float).089939,(float)9.0134e-4,(float)-1.5963e-5,
	    (float)-.0017529,(float)-1.0799e-5,(float)14670.,(float)14680.,(
	    float)1.3624e-4,(float).063104,(float).0051307,(float)-3.2797e-5,(
	    float).002189,(float)-2.6775e-5,(float)14680.,(float)14690.,(
	    float)9.1275e-5,(float).031681,(float).0031085,(float)-2.4828e-5,(
	    float)3.5131e-4,(float)-1.9276e-5,(float)14690.,(float)14700.,(
	    float)8.8122e-5,(float).040141,(float).0011384,(float)-1.6948e-5,(
	    float)-.0017809,(float)-1.1069e-5,(float)14700.,(float)14710.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14710.,(float)14720.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14720.,(float)14730.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14730.,(float)
	    14740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14740.,(float)14750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14750.,(float)14760.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14760.,(float)14770.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14770.,(float)14780.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14780.,(float)
	    14790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14790.,(float)14800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14800.,(float)14810.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14810.,(float)14820.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14820.,(float)14830.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14830.,(float)
	    14840.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14840.,(float)14850.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14850.,(float)14860.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14860.,(float)14870.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14870.,(float)14880.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14880.,(float)
	    14890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14890.,(float)14900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14900.,(float)14910.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14910.,(float)14920.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14920.,(float)14930.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14930.,(float)
	    14940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14940.,(float)14950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)14950.,(float)14960.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    14960.,(float)14970.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)14970.,(float)14980.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)14980.,(float)
	    14990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)14990.,(float)1.5e4,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)1.5e4,(float)15010.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15010.,(
	    float)15020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)15020.,(float)15030.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)15030.,(float)15040.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15040.,(float)15050.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15050.,(float)15060.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15060.,(float)
	    15070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15070.,(float)15080.,(float)7.1934e-4,(float).12682,(
	    float).0061012,(float)1.5559e-5,(float).0027848,(float)-4.5145e-6,
	    (float)15080.,(float)15090.,(float)4.9571e-4,(float).13321,(float)
	    .0050157,(float)1.7658e-5,(float).0024032,(float)-4.0605e-6,(
	    float)15090.,(float)15100.,(float)2.2621e-4,(float).12674,(float)
	    .016133,(float)-6.1229e-5,(float).013281,(float)-6.2746e-5,(float)
	    15100.,(float)15110.,(float)8.5668e-4,(float).12425,(float)
	    .016195,(float)-7.6668e-5,(float).013811,(float)-7.25e-5,(float)
	    15110.,(float)15120.,(float)5.5721e-4,(float).11061,(float)
	    .0048501,(float)3.2762e-5,(float).0041846,(float)4.2515e-7,(float)
	    15120.,(float)15130.,(float).0010214,(float).16075,(float)
	    .0077437,(float)-9.7938e-6,(float).0033244,(float)-1.335e-5,(
	    float)15130.,(float)15140.,(float).0037351,(float).23141,(float)
	    .0075468,(float)-1.8073e-5,(float).0038208,(float)-1.7101e-5,(
	    float)15140.,(float)15150.,(float)6.9279e-4,(float).16527,(float)
	    .0092564,(float)-2.4217e-5,(float).0059447,(float)-2.8136e-5,(
	    float)15150.,(float)15160.,(float).0020924,(float).19937,(float)
	    .0067646,(float)-1.9228e-5,(float).0021429,(float)-1.4011e-5,(
	    float)15160.,(float)15170.,(float).0029213,(float).11019,(float)
	    .0078105,(float)-4.068e-5,(float).0061601,(float)-3.7164e-5,(
	    float)15170.,(float)15180.,(float).0040031,(float).2888,(float)
	    .0050758,(float)-1.9158e-5,(float).0019869,(float)-1.728e-5,(
	    float)15180.,(float)15190.,(float).0024252,(float).15747,(float)
	    .0044234,(float)-2.0027e-5,(float)3.4479e-4,(float)-1.2338e-5,(
	    float)15190.,(float)15200.,(float).0087795,(float).14543,(float)
	    .0042621,(float)-2.7308e-5,(float)7.3031e-4,(float)-1.681e-5,(
	    float)15200.,(float)15210.,(float).0095997,(float).374,(float)
	    .0046013,(float)-2.8568e-5,(float).002061,(float)-2.4098e-5,(
	    float)15210.,(float)15220.,(float)9.5938e-4,(float).22877,(float)
	    .0032518,(float)-1.2364e-5,(float).00118,(float)-1.6396e-5,(float)
	    15220.,(float)15230.,(float).0089911,(float).27602,(float)
	    .0015486,(float)-1.7622e-5,(float)-7.0027e-4,(float)-1.3639e-5,(
	    float)15230.,(float)15240.,(float).0070384,(float).19525,(float)
	    .0017332,(float)-1.8418e-5,(float)-.0010753,(float)-1.2514e-5,(
	    float)15240.,(float)15250.,(float).012889,(float).20893,(float)
	    5.2643e-4,(float)-8.5726e-6,(float)-8.0955e-4,(float)-8.4315e-6,(
	    float)15250.,(float)15260.,(float).01071,(float).10552,(float)
	    -.0015016,(float)-6.0962e-6,(float)-.0038322,(float)-1.9428e-6,(
	    float)15260.,(float)15270.,(float).011725,(float).11142,(float)
	    -.0024386,(float)-2.089e-6,(float)-.0045783,(float)1.5311e-6,(
	    float)15270.,(float)15280.,(float).0036695,(float).13405,(float)
	    -.0019794,(float)1.4362e-7,(float)-.0035859,(float)2.0003e-6,(
	    float)15280.,(float)15290.,(float).0038912,(float).14992,(float)
	    -3.0601e-5,(float)8.2096e-6,(float)-1.4088e-4,(float)-1.393e-6,(
	    float)15290.,(float)15300. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/

/*     WATER VAPOR (12740 - 15280 CM-1) */

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
} /* wava5_ */

#ifdef __cplusplus
	}
#endif
