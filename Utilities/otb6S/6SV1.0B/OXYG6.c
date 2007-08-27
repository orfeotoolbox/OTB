/* OXYG6.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    oxyg6(a,inu) >*/
/* Subroutine */ int oxyg6_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)15300.,(float)
	    15310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15310.,(float)15320.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15320.,(float)15330.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15330.,(float)15340.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15340.,(float)15350.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15350.,(float)
	    15360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15360.,(float)15370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15370.,(float)15380.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15380.,(float)15390.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15390.,(float)15400.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15400.,(float)
	    15410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15410.,(float)15420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15420.,(float)15430.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15430.,(float)15440.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15440.,(float)15450.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15450.,(float)
	    15460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15460.,(float)15470.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15470.,(float)15480.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15480.,(float)15490.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15490.,(float)15500.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15500.,(float)
	    15510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15510.,(float)15520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15520.,(float)15530.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15530.,(float)15540.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15540.,(float)15550.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15550.,(float)
	    15560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15560.,(float)15570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15570.,(float)15580.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15580.,(float)15590.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15590.,(float)15600.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15600.,(float)
	    15610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15610.,(float)15620.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15620.,(float)15630.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15630.,(float)15640.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15640.,(float)15650.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15650.,(float)
	    15660.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15660.,(float)15670.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15670.,(float)15680.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15680.,(float)15690.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15690.,(float)15700.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15700.,(float)
	    15710.,(float)1.596e-8,(float).018194,(float).038561,(float)
	    -1.5424e-4,(float).036533,(float)-1.5016e-4,(float)15710.,(float)
	    15720.,(float)1.5504e-8,(float).018194,(float).03852,(float)
	    -1.5408e-4,(float).036492,(float)-1.5e-4,(float)15720.,(float)
	    15730.,(float)8.6149e-8,(float).036385,(float).03409,(float)
	    -1.3636e-4,(float).032063,(float)-1.3228e-4,(float)15730.,(float)
	    15740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15740.,(float)15750.,(float)2.2012e-7,(float).036385,(
	    float).029911,(float)-1.1964e-4,(float).027883,(float)-1.1556e-4,(
	    float)15750.,(float)15760.,(float)5.2469e-7,(float).036384,(float)
	    .026002,(float)-1.0401e-4,(float).023974,(float)-9.9924e-5,(float)
	    15760.,(float)15770.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15770.,(float)15780.,(float)1.1652e-6,(
	    float).037236,(float).022365,(float)-8.9458e-5,(float).020337,(
	    float)-8.5374e-5,(float)15780.,(float)15790.,(float)2.4086e-6,(
	    float).038115,(float).019,(float)-7.5998e-5,(float).016972,(float)
	    -7.1915e-5,(float)15790.,(float)15800.,(float)4.63e-6,(float)
	    .039407,(float).015907,(float)-6.3629e-5,(float).01388,(float)
	    -5.9545e-5,(float)15800.,(float)15810.,(float)4.2478e-6,(float)
	    .019927,(float).01311,(float)-5.2439e-5,(float).011082,(float)
	    -4.8357e-5,(float)15810.,(float)15820.,(float)4.0121e-6,(float)
	    .019927,(float).013066,(float)-5.2262e-5,(float).011038,(float)
	    -4.818e-5,(float)15820.,(float)15830.,(float)1.3653e-5,(float)
	    .041148,(float).010543,(float)-4.2171e-5,(float).0085153,(float)
	    -3.8088e-5,(float)15830.,(float)15840.,(float)2.0849e-5,(float)
	    .04334,(float).0082714,(float)-3.3085e-5,(float).0062412,(float)
	    -2.8988e-5,(float)15840.,(float)15850.,(float)2.9295e-5,(float)
	    .044164,(float).0062748,(float)-2.5099e-5,(float).004247,(float)
	    -2.1015e-5,(float)15850.,(float)15860.,(float)3.768e-5,(float)
	    .045464,(float).0045528,(float)-1.8211e-5,(float).002525,(float)
	    -1.4127e-5,(float)15860.,(float)15870.,(float)6.8735e-5,(float)
	    .070552,(float).0026882,(float)-1.0606e-5,(float)6.7182e-4,(float)
	    -6.6419e-6,(float)15870.,(float)15880.,(float)6.2284e-5,(float)
	    .071735,(float).0013301,(float)-5.2392e-6,(float)-7.0196e-4,(
	    float)-1.1788e-6,(float)15880.,(float)15890.,(float)3.9741e-5,(
	    float).0779,(float)3.5779e-4,(float)-1.1367e-6,(float)-.0013542,(
	    float)4.5537e-6,(float)15890.,(float)15900.,(float)1.6287e-5,(
	    float).058682,(float)3.1178e-4,(float)2.4589e-6,(float)5.313e-4,(
	    float)1.3293e-5,(float)15900.,(float)15910.,(float)9.9855e-5,(
	    float).14228,(float).0013456,(float)-1.6726e-6,(float).0010356,(
	    float)3.4749e-6,(float)15910.,(float)15920.,(float)1.9246e-4,(
	    float).31716,(float).005754,(float)-1.6318e-5,(float).0052332,(
	    float)-1.9378e-5,(float)15920.,(float)15930.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)15930.,(float)
	    15940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15940.,(float)15950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15950.,(float)15960.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15960.,(float)15970.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)15970.,(float)15980.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15980.,(float)
	    15990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15990.,(float)1.6e4,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)1.6e4,(float)16010.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16010.,(
	    float)16020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)16020.,(float)16030.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)16030.,(float)16040.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16040.,(float)16050.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16050.,(float)16060.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16060.,(float)
	    16070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16070.,(float)16080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16080.,(float)16090.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16090.,(float)16100.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16100.,(float)16110.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16110.,(float)
	    16120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16120.,(float)16130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16130.,(float)16140.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16140.,(float)16150.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16150.,(float)16160.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16160.,(float)
	    16170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16170.,(float)16180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16180.,(float)16190.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16190.,(float)16200.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16200.,(float)16210.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16210.,(float)
	    16220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16220.,(float)16230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16230.,(float)16240.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16240.,(float)16250.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16250.,(float)16260.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16260.,(float)
	    16270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16270.,(float)16280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16280.,(float)16290.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16290.,(float)16300.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16300.,(float)16310.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16310.,(float)
	    16320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16320.,(float)16330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16330.,(float)16340.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16340.,(float)16350.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16350.,(float)16360.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16360.,(float)
	    16370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16370.,(float)16380.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16380.,(float)16390.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16390.,(float)16400.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16400.,(float)16410.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16410.,(float)
	    16420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16420.,(float)16430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16430.,(float)16440.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16440.,(float)16450.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16450.,(float)16460.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16460.,(float)
	    16470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16470.,(float)16480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16480.,(float)16490.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16490.,(float)16500.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16500.,(float)16510.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16510.,(float)
	    16520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16520.,(float)16530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16530.,(float)16540.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16540.,(float)16550.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16550.,(float)16560.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16560.,(float)
	    16570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16570.,(float)16580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16580.,(float)16590.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16590.,(float)16600.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16600.,(float)16610.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16610.,(float)
	    16620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16620.,(float)16630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16630.,(float)16640.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16640.,(float)16650.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16650.,(float)16660.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16660.,(float)
	    16670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16670.,(float)16680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16680.,(float)16690.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16690.,(float)16700.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16700.,(float)16710.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16710.,(float)
	    16720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16720.,(float)16730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16730.,(float)16740.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16740.,(float)16750.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16750.,(float)16760.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16760.,(float)
	    16770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16770.,(float)16780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16780.,(float)16790.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16790.,(float)16800.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16800.,(float)16810.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16810.,(float)
	    16820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16820.,(float)16830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16830.,(float)16840.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16840.,(float)16850.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16850.,(float)16860.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16860.,(float)
	    16870.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16870.,(float)16880.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16880.,(float)16890.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16890.,(float)16900.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16900.,(float)16910.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16910.,(float)
	    16920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16920.,(float)16930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16930.,(float)16940.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16940.,(float)16950.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)16950.,(float)16960.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)16960.,(float)
	    16970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16970.,(float)16980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16980.,(float)16990.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    16990.,(float)1.7e4,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)1.7e4,(float)17010.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)17010.,(float)
	    17020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17020.,(float)17030.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17030.,(float)17040.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17040.,(float)17050.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17050.,(float)17060.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17060.,(float)
	    17070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17070.,(float)17080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17080.,(float)17090.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17090.,(float)17100.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17100.,(float)17110.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17110.,(float)
	    17120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17120.,(float)17130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17130.,(float)17140.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17140.,(float)17150.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17150.,(float)17160.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17160.,(float)
	    17170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17170.,(float)17180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17180.,(float)17190.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17190.,(float)17200.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17200.,(float)17210.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17210.,(float)
	    17220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17220.,(float)17230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17230.,(float)17240.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17240.,(float)17250.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17250.,(float)17260.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17260.,(float)
	    17270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17270.,(float)17280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17280.,(float)17290.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17290.,(float)17300.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17300.,(float)17310.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17310.,(float)
	    17320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17320.,(float)17330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17330.,(float)17340.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17340.,(float)17350.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17350.,(float)17360.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17360.,(float)
	    17370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17370.,(float)17380.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17380.,(float)17390.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17390.,(float)17400.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17400.,(float)17410.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17410.,(float)
	    17420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17420.,(float)17430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17430.,(float)17440.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17440.,(float)17450.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17450.,(float)17460.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17460.,(float)
	    17470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17470.,(float)17480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17480.,(float)17490.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17490.,(float)17500.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17500.,(float)17510.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17510.,(float)
	    17520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17520.,(float)17530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17530.,(float)17540.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17540.,(float)17550.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17550.,(float)17560.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17560.,(float)
	    17570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17570.,(float)17580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17580.,(float)17590.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17590.,(float)17600.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17600.,(float)17610.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17610.,(float)
	    17620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17620.,(float)17630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17630.,(float)17640.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17640.,(float)17650.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17650.,(float)17660.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17660.,(float)
	    17670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17670.,(float)17680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17680.,(float)17690.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17690.,(float)17700.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17700.,(float)17710.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17710.,(float)
	    17720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17720.,(float)17730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17730.,(float)17740.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17740.,(float)17750.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17750.,(float)17760.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17760.,(float)
	    17770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17770.,(float)17780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17780.,(float)17790.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17790.,(float)17800.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17800.,(float)17810.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17810.,(float)
	    17820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17820.,(float)17830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17830.,(float)17840.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17840.,(float)17850.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17850.,(float)17860. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     oxygen (15300 - 17870 cm-1) */

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
} /* oxyg6_ */

#ifdef __cplusplus
	}
#endif
