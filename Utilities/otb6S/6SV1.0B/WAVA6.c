/* WAVA6.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       SUBROUTINE    WAVA6(a,inu) >*/
/* Subroutine */ int wava6_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float).011482,(float)
	    .13183,(float)-.0038755,(float)3.4491e-6,(float)-.0069899,(float)
	    9.3146e-6,(float)15300.,(float)15310.,(float).0015124,(float)
	    .19547,(float).0028474,(float)-4.7616e-6,(float).0017802,(float)
	    -1.079e-5,(float)15310.,(float)15320.,(float).0092482,(float)
	    .16207,(float)-.0025675,(float)1.271e-5,(float)-.0027267,(float)
	    6.2076e-6,(float)15320.,(float)15330.,(float).0104,(float).25148,(
	    float).0028271,(float)-1.6657e-5,(float).0013667,(float)
	    -1.7862e-5,(float)15330.,(float)15340.,(float).036425,(float)
	    .35831,(float)-.0010013,(float)-1.0673e-6,(float)-.0025219,(float)
	    -1.7475e-6,(float)15340.,(float)15350.,(float).0050755,(float)
	    .12303,(float)-.0018115,(float)9.8037e-7,(float)-.0034793,(float)
	    8.572e-7,(float)15350.,(float)15360.,(float).0040405,(float)
	    .079669,(float)-.0035356,(float)4.2888e-6,(float)-.0049964,(float)
	    5.6042e-6,(float)15360.,(float)15370.,(float).0012748,(float)
	    .10104,(float).0012869,(float)-4.1165e-6,(float)-2.5366e-4,(float)
	    -8.5685e-6,(float)15370.,(float)15380.,(float).004332,(float)
	    .087049,(float)-.0037819,(float)4.8177e-6,(float)-.0066616,(float)
	    9.4206e-6,(float)15380.,(float)15390.,(float).018759,(float)
	    .14839,(float)-.0050183,(float)8.8733e-6,(float)-.0069647,(float)
	    1.2888e-5,(float)15390.,(float)15400.,(float).013161,(float)
	    .13948,(float)-.0039758,(float)3.962e-6,(float)-.006383,(float)
	    8.2226e-6,(float)15400.,(float)15410.,(float).014766,(float)
	    .30841,(float)-.0025568,(float)-1.0002e-6,(float)-.0049777,(float)
	    2.925e-6,(float)15410.,(float)15420.,(float).021082,(float).15162,
	    (float)-.0015321,(float)-5.6826e-6,(float)-.0038546,(float)
	    -1.6492e-6,(float)15420.,(float)15430.,(float).020195,(float)
	    .19485,(float)-2.7645e-4,(float)-9.8888e-6,(float)-.0024022,(
	    float)-6.5352e-6,(float)15430.,(float)15440.,(float).011166,(
	    float).18799,(float).0030297,(float)-2.1309e-5,(float)4.9231e-4,(
	    float)-1.6445e-5,(float)15440.,(float)15450.,(float).011405,(
	    float).30263,(float).0033723,(float)-1.9312e-5,(float).0012213,(
	    float)-1.8339e-5,(float)15450.,(float)15460.,(float).0081879,(
	    float).2948,(float).0056918,(float)-2.2518e-5,(float).0049723,(
	    float)-2.6308e-5,(float)15460.,(float)15470.,(float).0086587,(
	    float).31194,(float).0049388,(float)-2.4988e-5,(float).0025729,(
	    float)-2.3785e-5,(float)15470.,(float)15480.,(float).0029624,(
	    float).25004,(float).0021191,(float)1.2889e-5,(float).0021547,(
	    float)-5.0741e-6,(float)15480.,(float)15490.,(float).0010355,(
	    float).1518,(float).010772,(float)-5.3973e-5,(float).0081789,(
	    float)-4.9803e-5,(float)15490.,(float)15500.,(float).0019209,(
	    float).15869,(float).0035388,(float)-1.5675e-6,(float).0030429,(
	    float)-1.3275e-5,(float)15500.,(float)15510.,(float).0021168,(
	    float).22695,(float).0033055,(float)-7.1274e-7,(float).0018066,(
	    float)-1.051e-5,(float)15510.,(float)15520.,(float)3.63e-4,(float)
	    .041337,(float).004643,(float)-3.0966e-5,(float).0016831,(float)
	    -2.5006e-5,(float)15520.,(float)15530.,(float)6.422e-4,(float)
	    .040141,(float).0011384,(float)-1.6948e-5,(float)-.0017809,(float)
	    -1.1069e-5,(float)15530.,(float)15540.,(float).0017361,(float)
	    .12935,(float)7.0897e-6,(float)-1.1804e-5,(float)-.0029721,(float)
	    -5.606e-6,(float)15540.,(float)15550.,(float)3.6772e-4,(float)
	    .069843,(float).0015876,(float)-1.7017e-5,(float)-.0014188,(float)
	    -1.1298e-5,(float)15550.,(float)15560.,(float)8.1562e-4,(float)
	    .085732,(float).0028597,(float)-2.3409e-5,(float)-8.1651e-5,(
	    float)-1.7172e-5,(float)15560.,(float)15570.,(float)4.1832e-5,(
	    float).037286,(float).0071945,(float)-4.1172e-5,(float).0048022,(
	    float)-3.6355e-5,(float)15570.,(float)15580.,(float)2.89e-4,(
	    float).059772,(float).00431,(float)-2.7519e-5,(float).0025363,(
	    float)-2.6086e-5,(float)15580.,(float)15590.,(float)3.4153e-4,(
	    float).068927,(float).0058634,(float)-2.8708e-5,(float).002971,(
	    float)-2.6151e-5,(float)15590.,(float)15600.,(float)6.3462e-4,(
	    float).10803,(float).0062584,(float)-1.912e-5,(float).0049128,(
	    float)-2.4384e-5,(float)15600.,(float)15610.,(float)3.9706e-4,(
	    float).12303,(float).0073825,(float)-2.4245e-5,(float).0050938,(
	    float)-2.795e-5,(float)15610.,(float)15620.,(float)1.1711e-4,(
	    float).063154,(float).0049218,(float)-2.6583e-5,(float).0021634,(
	    float)-2.368e-5,(float)15620.,(float)15630.,(float)2.8022e-4,(
	    float).091171,(float).011895,(float)-5.9964e-5,(float).0091244,(
	    float)-5.4363e-5,(float)15630.,(float)15640.,(float)4.8561e-4,(
	    float).17953,(float).0048672,(float)-2.2506e-5,(float).0012405,(
	    float)-1.7834e-5,(float)15640.,(float)15650.,(float)3.295e-4,(
	    float).13103,(float).0079212,(float)-3.1361e-5,(float).0048579,(
	    float)-3.0501e-5,(float)15650.,(float)15660.,(float)6.2254e-4,(
	    float).13066,(float).0068683,(float)-3.4929e-5,(float).0042198,(
	    float)-3.0853e-5,(float)15660.,(float)15670.,(float)3.4175e-4,(
	    float).0686,(float).0071102,(float)-4.0828e-5,(float).0041954,(
	    float)-3.4941e-5,(float)15670.,(float)15680.,(float)6.7893e-4,(
	    float).25103,(float).002638,(float)-1.0784e-5,(float)-1.9402e-4,(
	    float)-1.0493e-5,(float)15680.,(float)15690.,(float).0022454,(
	    float).2126,(float).0045521,(float)-3.0054e-5,(float).0018943,(
	    float)-2.5356e-5,(float)15690.,(float)15700.,(float)2.2324e-4,(
	    float).16386,(float)9.9924e-5,(float)-4.6542e-6,(float)-.0032498,(
	    float)-4.4042e-7,(float)15700.,(float)15710.,(float).0011732,(
	    float).071121,(float).0017438,(float)-1.9369e-5,(float)-.0010378,(
	    float)-1.3774e-5,(float)15710.,(float)15720.,(float).0011939,(
	    float).22194,(float).0022505,(float)-2.0274e-5,(float)-3.0389e-4,(
	    float)-1.5915e-5,(float)15720.,(float)15730.,(float).0011335,(
	    float).1131,(float)6.5038e-4,(float)-1.4226e-5,(float)-.0023716,(
	    float)-8.4484e-6,(float)15730.,(float)15740.,(float).0013693,(
	    float).066759,(float)-4.9503e-4,(float)-1.0274e-5,(float)
	    -.0033442,(float)-4.9491e-6,(float)15740.,(float)15750.,(float)
	    .0022675,(float).11019,(float)-.0015659,(float)-6.0385e-6,(float)
	    -.0042073,(float)-9.9453e-7,(float)15750.,(float)15760.,(float)
	    .0021492,(float).11169,(float)-.0025591,(float)-1.868e-6,(float)
	    -.0052151,(float)2.9531e-6,(float)15760.,(float)15770.,(float)
	    5.2724e-4,(float).079449,(float)-.0015077,(float)3.7703e-7,(float)
	    -.0037765,(float)1.0136e-6,(float)15770.,(float)15780.,(float)
	    .0017852,(float).092282,(float)-.0042337,(float)4.5463e-6,(float)
	    -.0075615,(float)1.1296e-5,(float)15780.,(float)15790.,(float)
	    5.7289e-4,(float).18138,(float).001042,(float)1.1358e-7,(float)
	    -.0010771,(float)-3.8585e-6,(float)15790.,(float)15800.,(float)
	    .0024903,(float).21722,(float)6.1161e-4,(float)2.0815e-6,(float)
	    -6.5545e-4,(float)-6.5063e-6,(float)15800.,(float)15810.,(float)
	    .0033934,(float).23501,(float)3.4083e-4,(float)-1.2029e-5,(float)
	    -.0027676,(float)-5.6508e-6,(float)15810.,(float)15820.,(float)
	    .0043759,(float).14995,(float)-.003628,(float)2.8054e-6,(float)
	    -.0061589,(float)7.1308e-6,(float)15820.,(float)15830.,(float)
	    9.6328e-4,(float).12298,(float)-.0027284,(float)2.0822e-6,(float)
	    -.0051044,(float)4.1652e-6,(float)15830.,(float)15840.,(float)
	    6.1564e-4,(float).047547,(float)-.0041578,(float)4.2375e-6,(float)
	    -.007361,(float)1.0687e-5,(float)15840.,(float)15850.,(float)
	    1.445e-4,(float).047164,(float)-.0026523,(float)-1.7845e-6,(float)
	    -.0054095,(float)3.7673e-6,(float)15850.,(float)15860.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)15860.,(
	    float)15870.,(float).0027617,(float).080618,(float)-.0054341,(
	    float)9.35e-6,(float)-.0086841,(float)1.5797e-5,(float)15870.,(
	    float)15880.,(float).0038448,(float).0889,(float)-.0045065,(float)
	    5.7229e-6,(float)-.0074065,(float)1.161e-5,(float)15880.,(float)
	    15890.,(float).002669,(float).12359,(float)-.003403,(float)
	    1.5184e-6,(float)-.0062551,(float)6.94e-6,(float)15890.,(float)
	    15900.,(float).0035205,(float).15929,(float)-.0029195,(float)
	    -6.0033e-7,(float)-.0055137,(float)4.5063e-6,(float)15900.,(float)
	    15910.,(float).0033857,(float).15288,(float)-5.5305e-4,(float)
	    -1.0049e-5,(float)-.0030402,(float)-5.3251e-6,(float)15910.,(
	    float)15920.,(float).0055123,(float).20681,(float)6.3027e-5,(
	    float)-1.1203e-5,(float)-.0021942,(float)-7.6854e-6,(float)15920.,
	    (float)15930.,(float).0025555,(float).15612,(float).0036149,(
	    float)-2.367e-5,(float)8.1154e-4,(float)-1.8842e-5,(float)15930.,(
	    float)15940.,(float)6.0708e-4,(float).15267,(float).0069583,(
	    float)-2.3922e-5,(float).0059157,(float)-3.3379e-5,(float)15940.,(
	    float)15950.,(float).0013307,(float).12652,(float).005072,(float)
	    -3.1273e-5,(float).0029461,(float)-2.8552e-5,(float)15950.,(float)
	    15960.,(float)1.9184e-4,(float).031816,(float).010823,(float)
	    -5.5684e-5,(float).0080654,(float)-5.0132e-5,(float)15960.,(float)
	    15970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)15970.,(float)15980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)15980.,(float)15990.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    15990.,(float)1.6e4,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)1.6e4,(float)16010.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)16010.,(float)
	    16020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)16020.,(float)16030.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)16030.,(float)16040.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
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
	    16690.,(float)16700.,(float)9.3657e-4,(float).16814,(float)
	    .0071611,(float)-3.2337e-5,(float).0051118,(float)-3.429e-5,(
	    float)16700.,(float)16710.,(float).002716,(float).13632,(float)
	    .0024507,(float)-1.7614e-5,(float)6.4545e-4,(float)-1.7104e-5,(
	    float)16710.,(float)16720.,(float).01021,(float).33525,(float)
	    .0035136,(float)-2.0013e-5,(float).0012395,(float)-1.7557e-5,(
	    float)16720.,(float)16730.,(float).0056241,(float).14518,(float)
	    .0025514,(float)-6.0743e-6,(float)4.0412e-4,(float)-9.6804e-6,(
	    float)16730.,(float)16740.,(float).0090151,(float).16435,(float)
	    5.9525e-4,(float)-7.4453e-6,(float)-7.7433e-4,(float)-8.1348e-6,(
	    float)16740.,(float)16750.,(float).017084,(float).37569,(float)
	    .0017092,(float)-1.0104e-5,(float)5.4764e-5,(float)-1.1565e-5,(
	    float)16750.,(float)16760.,(float).004068,(float).16946,(float)
	    .0051318,(float)-1.2576e-5,(float).0021847,(float)-1.7187e-5,(
	    float)16760.,(float)16770.,(float).014025,(float).25075,(float)
	    -4.9903e-4,(float)-3.9474e-6,(float)-.0024356,(float)-2.4014e-6,(
	    float)16770.,(float)16780.,(float).0084391,(float).29482,(float)
	    .00143,(float)-1.4909e-5,(float)-5.8149e-4,(float)-1.2205e-5,(
	    float)16780.,(float)16790.,(float).0042603,(float).15728,(float)
	    -.0017076,(float)1.1819e-5,(float)-.0025115,(float)1.7886e-6,(
	    float)16790.,(float)16800.,(float).016111,(float).35565,(float)
	    9.8453e-4,(float)-1.0561e-5,(float)-.0015547,(float)-8.0917e-6,(
	    float)16800.,(float)16810.,(float).032806,(float).40538,(float)
	    3.2361e-4,(float)-7.1099e-6,(float)-.0017591,(float)-6.4585e-6,(
	    float)16810.,(float)16820.,(float).039593,(float).37366,(float)
	    -.0017738,(float)-3.6504e-6,(float)-.0044894,(float)1.5591e-6,(
	    float)16820.,(float)16830.,(float).0041709,(float).15463,(float)
	    -.001046,(float)-1.7983e-6,(float)-.0027043,(float)-2.9685e-6,(
	    float)16830.,(float)16840.,(float).0035117,(float).15272,(float)
	    -.0028931,(float)3.0829e-6,(float)-.0046348,(float)4.9372e-6,(
	    float)16840.,(float)16850.,(float).014133,(float).12212,(float)
	    -.0040965,(float)4.1489e-6,(float)-.0071928,(float)1.0033e-5,(
	    float)16850.,(float)16860.,(float).0077536,(float).19684,(float)
	    -.0044631,(float)8.2821e-6,(float)-.0062227,(float)1.0476e-5,(
	    float)16860.,(float)16870.,(float).025192,(float).3428,(float)
	    -.003503,(float)5.8569e-6,(float)-.0052023,(float)7.3334e-6,(
	    float)16870.,(float)16880.,(float).030248,(float).27295,(float)
	    -.0026774,(float)1.2042e-6,(float)-.0042996,(float)2.2385e-6,(
	    float)16880.,(float)16890.,(float).012936,(float).30454,(float)
	    -.0022564,(float)-1.1282e-6,(float)-.0042435,(float)1.4398e-6,(
	    float)16890.,(float)16900.,(float).011031,(float).24979,(float)
	    -.0032635,(float)5.324e-6,(float)-.0041759,(float)4.4122e-6,(
	    float)16900.,(float)16910.,(float).014293,(float).29998,(float)
	    -3.4569e-4,(float)-9.0328e-6,(float)-.0021933,(float)-6.7146e-6,(
	    float)16910.,(float)16920.,(float).0091809,(float).29615,(float)
	    .0023482,(float)-1.7235e-5,(float)-5.9956e-4,(float)-1.1968e-5,(
	    float)16920.,(float)16930.,(float).008126,(float).323,(float)
	    1.1339e-4,(float)-9.8017e-7,(float)-.0013639,(float)-4.0912e-6,(
	    float)16930.,(float)16940.,(float).040319,(float).36952,(float)
	    -.0034585,(float)7.4285e-6,(float)-.0043038,(float)5.2677e-6,(
	    float)16940.,(float)16950.,(float).013921,(float).36738,(float)
	    -7.7085e-4,(float)4.5894e-7,(float)-.0026272,(float)-1.4583e-6,(
	    float)16950.,(float)16960.,(float).014345,(float).25675,(float)
	    -.0022235,(float)1.8237e-6,(float)-.0031796,(float)3.5185e-6,(
	    float)16960.,(float)16970.,(float).016501,(float).22846,(float)
	    -7.3791e-4,(float)-8.1276e-6,(float)-.0030972,(float)-3.4017e-6,(
	    float)16970.,(float)16980.,(float).019984,(float).24072,(float)
	    4.6284e-4,(float)-1.157e-5,(float)-.0016535,(float)-8.0246e-6,(
	    float)16980.,(float)16990.,(float).014605,(float).42551,(float)
	    .0042003,(float)-2.4403e-5,(float).0012313,(float)-1.961e-5,(
	    float)16990.,(float)1.7e4,(float).0056604,(float).31925,(float)
	    .0037029,(float)-1.1447e-5,(float).0028937,(float)-1.9869e-5,(
	    float)1.7e4,(float)17010.,(float).0063352,(float).35092,(float)
	    .0026893,(float)-1.5677e-5,(float)5.5216e-4,(float)-1.3231e-5,(
	    float)17010.,(float)17020.,(float).003844,(float).22874,(float)
	    -1.2718e-4,(float)-9.3893e-8,(float)-.0018171,(float)-1.6527e-7,(
	    float)17020.,(float)17030.,(float).0013581,(float).13591,(float)
	    .0068077,(float)-3.5228e-5,(float).0039267,(float)-3.0049e-5,(
	    float)17030.,(float)17040.,(float).0018736,(float).25317,(float)
	    .004094,(float)-1.115e-5,(float).0020737,(float)-1.6908e-5,(float)
	    17040.,(float)17050.,(float).0012084,(float).1688,(float)
	    -.0010058,(float)-7.8902e-7,(float)-.0025863,(float)-1.1192e-6,(
	    float)17050.,(float)17060.,(float).0010487,(float).069135,(float)
	    .006375,(float)-3.3619e-5,(float).0036725,(float)-3.0416e-5,(
	    float)17060.,(float)17070.,(float)5.0984e-4,(float).11462,(float)
	    .0031391,(float)-2.153e-5,(float)4.5939e-4,(float)-1.7989e-5,(
	    float)17070.,(float)17080.,(float)8.4428e-4,(float).14457,(float)
	    -5.8124e-4,(float)-7.7648e-6,(float)-.0029191,(float)-4.3661e-6,(
	    float)17080.,(float)17090.,(float)3.0536e-4,(float).074835,(float)
	    1.5726e-4,(float)-1.2441e-5,(float)-.0024876,(float)-7.6201e-6,(
	    float)17090.,(float)17100.,(float)5.526e-4,(float).12225,(float)
	    .0040329,(float)-2.6244e-5,(float).0012152,(float)-2.1176e-5,(
	    float)17100.,(float)17110.,(float)3.7571e-4,(float).06606,(float)
	    .004893,(float)-1.7932e-5,(float).0027254,(float)-2.1243e-5,(
	    float)17110.,(float)17120.,(float)3.8817e-4,(float).096834,(float)
	    .0050965,(float)-2.8072e-5,(float).0025414,(float)-2.5439e-5,(
	    float)17120.,(float)17130.,(float)4.3455e-4,(float).10332,(float)
	    .0037899,(float)-2.5805e-5,(float).0011068,(float)-2.147e-5,(
	    float)17130.,(float)17140.,(float)2.797e-4,(float).13222,(float)
	    .0049706,(float)-1.1993e-5,(float).0022042,(float)-1.6597e-5,(
	    float)17140.,(float)17150.,(float)3.5542e-4,(float).094199,(float)
	    .0069032,(float)-3.8399e-5,(float).0042436,(float)-3.3972e-5,(
	    float)17150.,(float)17160.,(float)1.4332e-4,(float).10096,(float)
	    .0080342,(float)-4.4157e-5,(float).0052994,(float)-3.893e-5,(
	    float)17160.,(float)17170.,(float)1.131e-4,(float).07032,(float)
	    .0094132,(float)-1.9979e-5,(float).0084399,(float)-3.3269e-5,(
	    float)17170.,(float)17180.,(float)1.097e-4,(float).073653,(float)
	    .0070015,(float)-3.1921e-5,(float).0041785,(float)-3.033e-5,(
	    float)17180.,(float)17190.,(float)1.9539e-4,(float).11002,(float)
	    .0025848,(float)-2.1294e-5,(float)1.5132e-4,(float)-1.7461e-5,(
	    float)17190.,(float)17200.,(float)2.2464e-5,(float).040498,(float)
	    .0015081,(float)-1.8426e-5,(float)-8.0302e-4,(float)-1.3773e-5,(
	    float)17200.,(float)17210.,(float)1.1068e-4,(float).041949,(float)
	    .0011384,(float)-1.6948e-5,(float)-.001862,(float)-1.0906e-5,(
	    float)17210.,(float)17220.,(float)2.6372e-4,(float).1423,(float)
	    .0074076,(float)-9.6003e-7,(float).004947,(float)-1.6056e-5,(
	    float)17220.,(float)17230.,(float)1.6225e-4,(float).064754,(float)
	    9.1255e-4,(float)4.5791e-6,(float)2.8912e-5,(float)-2.743e-6,(
	    float)17230.,(float)17240.,(float)3.4492e-4,(float).13306,(float)
	    -.0022341,(float)-3.1568e-6,(float)-.0051367,(float)2.5216e-6,(
	    float)17240.,(float)17250.,(float)2.9083e-4,(float).1273,(float)
	    .014643,(float)-7.0324e-5,(float).011799,(float)-6.4717e-5,(float)
	    17250.,(float)17260.,(float)7.9159e-5,(float).084959,(float)
	    .0033618,(float)1.0957e-5,(float)-1.9083e-6,(float)1.5045e-6,(
	    float)17260.,(float)17270.,(float)3.991e-4,(float).1611,(float)
	    .0052228,(float)-6.6224e-6,(float).003315,(float)-1.7428e-5,(
	    float)17270.,(float)17280.,(float)3.6117e-4,(float).14552,(float)
	    .0053239,(float)-2.7921e-6,(float).0027764,(float)-1.3072e-5,(
	    float)17280.,(float)17290.,(float)5.6601e-4,(float).2023,(float)
	    .0081509,(float)-3.975e-5,(float).0051039,(float)-3.5401e-5,(
	    float)17290.,(float)17300.,(float)5.522e-4,(float).27444,(float)
	    .0022665,(float)-4.1297e-6,(float)6.9271e-4,(float)-1.2175e-5,(
	    float)17300.,(float)17310.,(float).0012177,(float).31594,(float)
	    .0026446,(float)-1.1117e-5,(float)-.0010442,(float)-7.3851e-6,(
	    float)17310.,(float)17320.,(float).0012068,(float).36919,(float)
	    .0046393,(float)-2.5102e-5,(float).001937,(float)-2.1181e-5,(
	    float)17320.,(float)17330.,(float)8.1484e-4,(float).12504,(float)
	    .0033991,(float)-2.4509e-5,(float)-1.7719e-4,(float)-1.657e-5,(
	    float)17330.,(float)17340.,(float).0010075,(float).10415,(float)
	    .0045176,(float)-3.0424e-5,(float).0016795,(float)-2.4622e-5,(
	    float)17340.,(float)17350.,(float).0014383,(float).23566,(float)
	    .0013476,(float)-1.0965e-5,(float)-.0023371,(float)-4.3633e-6,(
	    float)17350.,(float)17360.,(float)7.8276e-4,(float).2229,(float)
	    -.0015478,(float)-1.8316e-6,(float)-.0039408,(float)4.9016e-7,(
	    float)17360.,(float)17370.,(float).0023195,(float).1644,(float)
	    .0012573,(float)-1.668e-5,(float)-.0019183,(float)-9.5692e-6,(
	    float)17370.,(float)17380.,(float)7.0669e-4,(float).18348,(float)
	    -.001353,(float)-5.5696e-6,(float)-.0046409,(float)1.1901e-6,(
	    float)17380.,(float)17390.,(float).0026577,(float).24613,(float)
	    -3.4475e-4,(float)-9.149e-6,(float)-.0027647,(float)-4.4906e-6,(
	    float)17390.,(float)17400.,(float).0014266,(float).18058,(float)
	    -1.7564e-4,(float)-3.1091e-6,(float)-.0023868,(float)-2.4568e-6,(
	    float)17400.,(float)17410.,(float).0013368,(float).089183,(float)
	    -7.5731e-4,(float)-3.0495e-6,(float)-.0013138,(float)-5.5029e-6,(
	    float)17410.,(float)17420.,(float).0031555,(float).29805,(float)
	    -.0014978,(float)-6.4349e-7,(float)-.0036817,(float)1.3405e-6,(
	    float)17420.,(float)17430.,(float).0015803,(float).20706,(float)
	    .0027308,(float)-1.4728e-5,(float)2.1412e-4,(float)-1.4434e-5,(
	    float)17430.,(float)17440.,(float).0025444,(float).19283,(float)
	    -.0026123,(float)3.7928e-6,(float)-.0040723,(float)3.3236e-6,(
	    float)17440.,(float)17450.,(float).0028951,(float).14488,(float)
	    8.2222e-4,(float)-1.4617e-5,(float)-.0020986,(float)-8.6758e-6,(
	    float)17450.,(float)17460.,(float)9.9222e-4,(float).058296,(float)
	    -6.973e-4,(float)-8.8805e-6,(float)-.0030012,(float)-5.1319e-6,(
	    float)17460.,(float)17470.,(float).0049761,(float).23841,(float)
	    -.0034073,(float)2.1975e-6,(float)-.00609,(float)6.7461e-6,(float)
	    17470.,(float)17480.,(float).0024737,(float).16103,(float)
	    -.0033546,(float)5.2399e-6,(float)-.0050448,(float)4.9288e-6,(
	    float)17480.,(float)17490.,(float)6.5774e-4,(float).076908,(float)
	    -.0048253,(float)7.2725e-6,(float)-.0072344,(float)1.1372e-5,(
	    float)17490.,(float)17500.,(float)8.5946e-4,(float).1372,(float)
	    -.0034619,(float)3.8209e-6,(float)-.0055141,(float)6.4456e-6,(
	    float)17500.,(float)17510.,(float)1.3088e-4,(float).077214,(float)
	    1.0378e-4,(float)-6.7806e-6,(float)-.0024574,(float)-4.3444e-6,(
	    float)17510.,(float)17520.,(float)7.2177e-4,(float).12037,(float)
	    -.0032371,(float)3.8429e-6,(float)-.005706,(float)6.3102e-6,(
	    float)17520.,(float)17530.,(float).0025932,(float).057507,(float)
	    -.0053841,(float)9.3892e-6,(float)-.0080102,(float)1.4171e-5,(
	    float)17530.,(float)17540.,(float).0064329,(float).34361,(float)
	    -.0033999,(float)4.1519e-6,(float)-.0047285,(float)4.1554e-6,(
	    float)17540.,(float)17550.,(float).0020356,(float).18182,(float)
	    -.0031524,(float)9.2073e-7,(float)-.0056549,(float)4.9733e-6,(
	    float)17550.,(float)17560.,(float).0065331,(float).22623,(float)
	    -.0015636,(float)-4.1008e-6,(float)-.0032357,(float)-2.3034e-6,(
	    float)17560.,(float)17570.,(float).0059972,(float).29545,(float)
	    -6.0272e-4,(float)-7.9159e-6,(float)-.0032228,(float)-3.1603e-6,(
	    float)17570.,(float)17580.,(float).0040039,(float).3136,(float)
	    .0032065,(float)-2.0617e-5,(float)6.7568e-4,(float)-1.7932e-5,(
	    float)17580.,(float)17590.,(float).0016959,(float).28903,(float)
	    .005156,(float)-1.9347e-5,(float).0033983,(float)-2.1801e-5,(
	    float)17590.,(float)17600.,(float)7.0732e-4,(float).16014,(float)
	    .0010501,(float)-1.4628e-5,(float)-.0015478,(float)-9.7179e-6,(
	    float)17600.,(float)17610.,(float).0012558,(float).1798,(float)
	    .0044641,(float)-2.5933e-5,(float).0019766,(float)-2.2898e-5,(
	    float)17610.,(float)17620.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)17620.,(float)17630.,(float)
	    5.944e-5,(float).033158,(float).0060208,(float)-3.6477e-5,(float)
	    .0032636,(float)-3.0925e-5,(float)17630.,(float)17640.,(float)
	    1.6627e-4,(float).041123,(float)-.0027846,(float)-1.2553e-6,(
	    float)-.0055418,(float)4.2965e-6,(float)17640.,(float)17650.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17650.,(float)17660.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17660.,(float)17670.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17670.,(float)
	    17680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17680.,(float)17690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17690.,(float)17700.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17700.,(float)17710.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17710.,(float)17720.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17720.,(float)
	    17730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17730.,(float)17740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17740.,(float)17750.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17750.,(float)17760.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17760.,(float)17770.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17770.,(float)
	    17780.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17780.,(float)17790.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17790.,(float)17800.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17800.,(float)17810.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)17810.,(float)17820.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)17820.,(float)
	    17830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)17830.,(float)17840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)17840.,(float)17850.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    17850.,(float)17860. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/

/*     WATER VAPOR (15300 - 17860 CM-1) */

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
} /* wava6_ */

#ifdef __cplusplus
	}
#endif
