/* WAVA3.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       SUBROUTINE    WAVA3(a,inu) >*/
/* Subroutine */ int wava3_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float).092641,(float)
	    .26739,(float).0074828,(float)-3.6295e-5,(float).0065918,(float)
	    -3.6255e-5,(float)7620.,(float)7630.,(float).24311,(float).19859,(
	    float).0029686,(float)-1.983e-5,(float).0023399,(float)-1.6807e-5,
	    (float)7630.,(float)7640.,(float).12025,(float).11463,(float)
	    .005982,(float)-3.2695e-5,(float).00555,(float)-2.817e-5,(float)
	    7640.,(float)7650.,(float).13658,(float).26836,(float).0072502,(
	    float)-2.1381e-5,(float).0073061,(float)-3.0405e-5,(float)7650.,(
	    float)7660.,(float).10472,(float).25523,(float).0079597,(float)
	    -3.2676e-5,(float).0058214,(float)-3.1308e-5,(float)7660.,(float)
	    7670.,(float).099714,(float).25003,(float).012338,(float)
	    -5.6896e-5,(float).010569,(float)-5.1734e-5,(float)7670.,(float)
	    7680.,(float).058366,(float).22207,(float).011694,(float)
	    -5.1427e-5,(float).011909,(float)-4.905e-5,(float)7680.,(float)
	    7690.,(float).035387,(float).17329,(float).016862,(float)
	    -7.5246e-5,(float).013502,(float)-6.3311e-5,(float)7690.,(float)
	    7700.,(float).036568,(float).21034,(float).0081374,(float)
	    -2.4226e-5,(float).0063861,(float)-2.5253e-5,(float)7700.,(float)
	    7710.,(float).027597,(float).33962,(float).017925,(float)
	    -6.8929e-5,(float).015283,(float)-6.6807e-5,(float)7710.,(float)
	    7720.,(float).010316,(float).41286,(float).019917,(float)
	    -4.8503e-5,(float).017007,(float)-5.8717e-5,(float)7720.,(float)
	    7730.,(float).0081553,(float).2556,(float).021997,(float)
	    -8.8437e-5,(float).020546,(float)-8.5162e-5,(float)7730.,(float)
	    7740.,(float).015975,(float).21315,(float).0071481,(float)
	    3.1142e-6,(float).010474,(float)-1.3461e-5,(float)7740.,(float)
	    7750.,(float).013868,(float).24329,(float).0094502,(float)
	    -1.8614e-5,(float).010702,(float)-3.4118e-5,(float)7750.,(float)
	    7760.,(float).0077141,(float).22964,(float).011399,(float)
	    -1.0108e-5,(float).012776,(float)-3.7304e-5,(float)7760.,(float)
	    7770.,(float).0021739,(float).34321,(float).019734,(float)
	    -5.1909e-5,(float).019414,(float)-7.3442e-5,(float)7770.,(float)
	    7780.,(float).004572,(float).18176,(float).010354,(float)
	    -1.7399e-5,(float).010948,(float)-3.5358e-5,(float)7780.,(float)
	    7790.,(float).0011295,(float).18695,(float).014646,(float)
	    4.036e-5,(float).015891,(float)-2.0174e-5,(float)7790.,(float)
	    7800.,(float).0033023,(float).15293,(float).013409,(float)
	    -5.3371e-5,(float).011569,(float)-4.7548e-5,(float)7800.,(float)
	    7810.,(float).0052016,(float).24404,(float).012628,(float)
	    -3.6541e-5,(float).01295,(float)-3.8949e-5,(float)7810.,(float)
	    7820.,(float)1.133e-4,(float).081552,(float).01837,(float)
	    -7.7312e-5,(float).014695,(float)-7.0503e-5,(float)7820.,(float)
	    7830.,(float).0054995,(float).14297,(float).0065566,(float)
	    -2.675e-5,(float).0045262,(float)-2.0113e-5,(float)7830.,(float)
	    7840.,(float)3.1204e-4,(float).10156,(float).0088239,(float)
	    -3.8012e-5,(float).0082963,(float)-3.4068e-5,(float)7840.,(float)
	    7850.,(float).0019523,(float).20697,(float).02048,(float)
	    -6.0781e-5,(float).018775,(float)-6.9708e-5,(float)7850.,(float)
	    7860.,(float).0024757,(float).09698,(float).011294,(float)
	    -4.651e-5,(float).0098378,(float)-4.2459e-5,(float)7860.,(float)
	    7870.,(float)2.7171e-4,(float).14226,(float).017805,(float)
	    -6.9496e-5,(float).015097,(float)-7.0161e-5,(float)7870.,(float)
	    7880.,(float).0012276,(float).10591,(float).014194,(float)
	    -5.4065e-5,(float).010612,(float)-5.2507e-5,(float)7880.,(float)
	    7890.,(float)4.1923e-4,(float).13358,(float).01696,(float)
	    -4.4025e-5,(float).015096,(float)-5.8421e-5,(float)7890.,(float)
	    7900.,(float)3.3444e-4,(float).13345,(float).011391,(float)
	    -1.184e-6,(float).012153,(float)-2.3277e-5,(float)7900.,(float)
	    7910.,(float).0013058,(float).21624,(float).016927,(float)
	    -6.9213e-5,(float).013941,(float)-6.3876e-5,(float)7910.,(float)
	    7920.,(float)1.1995e-4,(float).075585,(float).013632,(float)
	    -4.0157e-5,(float).013768,(float)-5.0661e-5,(float)7920.,(float)
	    7930.,(float).0011716,(float).18257,(float).013971,(float)
	    -5.1254e-5,(float).012846,(float)-5.6651e-5,(float)7930.,(float)
	    7940.,(float)5.052e-4,(float).076356,(float).012922,(float)
	    -2.0119e-5,(float).013988,(float)-4.2851e-5,(float)7940.,(float)
	    7950.,(float)2.4426e-4,(float).1253,(float).010339,(float)
	    -4.3784e-6,(float).010719,(float)-3.665e-5,(float)7950.,(float)
	    7960.,(float)5.414e-4,(float).072313,(float).019259,(float)
	    -8.565e-5,(float).01592,(float)-7.9109e-5,(float)7960.,(float)
	    7970.,(float)7.6374e-4,(float).068718,(float).018517,(float)
	    -6.7616e-5,(float).016394,(float)-7.3663e-5,(float)7970.,(float)
	    7980.,(float)2.5606e-4,(float).077067,(float).016494,(float)
	    -7.5101e-5,(float).01272,(float)-6.6656e-5,(float)7980.,(float)
	    7990.,(float)1.5654e-4,(float).11158,(float).010695,(float)
	    -4.0866e-5,(float).0081416,(float)-4.363e-5,(float)7990.,(float)
	    8e3,(float)4.1929e-5,(float).090036,(float).024363,(float)
	    -1.0641e-4,(float).021621,(float)-1.0231e-4,(float)8e3,(float)
	    8010.,(float)7.3175e-4,(float).10587,(float).016949,(float)
	    -6.039e-5,(float).015862,(float)-7.1742e-5,(float)8010.,(float)
	    8020.,(float)8.008e-5,(float).030563,(float).019843,(float)
	    -9.1766e-5,(float).017086,(float)-8.6214e-5,(float)8020.,(float)
	    8030.,(float)2.4279e-5,(float).056812,(float).022624,(float)
	    -9.9621e-5,(float).020301,(float)-9.7332e-5,(float)8030.,(float)
	    8040.,(float)1.8866e-5,(float).052834,(float).012394,(float)
	    -4.4016e-5,(float).0093994,(float)-4.6495e-5,(float)8040.,(float)
	    8050.,(float)9.7669e-4,(float).089736,(float).0062407,(float)
	    -2.5133e-5,(float).005852,(float)-2.556e-5,(float)8050.,(float)
	    8060.,(float).0011471,(float).029409,(float).02069,(float)
	    -9.511e-5,(float).018237,(float)-9.0575e-5,(float)8060.,(float)
	    8070.,(float)7.7566e-4,(float).12509,(float).012079,(float)
	    -5.088e-5,(float).010725,(float)-5.2092e-5,(float)8070.,(float)
	    8080.,(float).003103,(float).073291,(float).016462,(float)
	    -7.3991e-5,(float).015208,(float)-6.8516e-5,(float)8080.,(float)
	    8090.,(float).001251,(float).11933,(float).014229,(float)
	    -6.2251e-5,(float).011779,(float)-6.099e-5,(float)8090.,(float)
	    8100.,(float).001499,(float).13662,(float).0065894,(float)
	    -8.557e-6,(float).0083512,(float)-1.9086e-5,(float)8100.,(float)
	    8110.,(float).0020532,(float).10298,(float).015994,(float)
	    -6.5137e-5,(float).014131,(float)-6.2564e-5,(float)8110.,(float)
	    8120.,(float).0017448,(float).067598,(float).014749,(float)
	    -6.293e-5,(float).013541,(float)-5.9773e-5,(float)8120.,(float)
	    8130.,(float).0015039,(float).12946,(float).018816,(float)
	    -5.0022e-5,(float).017492,(float)-5.9638e-5,(float)8130.,(float)
	    8140.,(float).0046638,(float).1035,(float).0062829,(float)
	    -3.4218e-5,(float).0036814,(float)-2.5761e-5,(float)8140.,(float)
	    8150.,(float).0010319,(float).066367,(float).0071775,(float)
	    -2.8782e-5,(float).010631,(float)-2.023e-5,(float)8150.,(float)
	    8160.,(float).0046436,(float).096269,(float).015527,(float)
	    -6.1225e-5,(float).015071,(float)-6.3728e-5,(float)8160.,(float)
	    8170.,(float).0057048,(float).15352,(float).0032127,(float)
	    -1.6432e-5,(float).0035091,(float)-1.2277e-5,(float)8170.,(float)
	    8180.,(float).011525,(float).15916,(float).0083892,(float)
	    -1.0637e-5,(float).0067801,(float)-2.5386e-5,(float)8180.,(float)
	    8190.,(float).0074791,(float).18295,(float).0048041,(float)
	    -2.0748e-5,(float).0044988,(float)-1.7162e-5,(float)8190.,(float)
	    8200.,(float).011502,(float).15897,(float).0099626,(float)
	    -1.7036e-5,(float).010154,(float)-3.5251e-5,(float)8200.,(float)
	    8210.,(float).032694,(float).12981,(float).0016557,(float)
	    1.4661e-5,(float).0035153,(float)1.0696e-6,(float)8210.,(float)
	    8220.,(float).007492,(float).12913,(float).0071536,(float)
	    -2.9018e-5,(float).0069835,(float)-3.3541e-5,(float)8220.,(float)
	    8230.,(float).047072,(float).21294,(float).0094858,(float)
	    -4.2608e-5,(float).0075764,(float)-3.9014e-5,(float)8230.,(float)
	    8240.,(float).017525,(float).16065,(float).0024969,(float)
	    -1.0727e-6,(float).0020059,(float)-5.349e-6,(float)8240.,(float)
	    8250.,(float).057547,(float).16256,(float).0053651,(float)
	    -3.1796e-5,(float).0032968,(float)-2.8841e-5,(float)8250.,(float)
	    8260.,(float).043086,(float).11299,(float).0044958,(float)
	    -2.4017e-5,(float).0036046,(float)-2.0674e-5,(float)8260.,(float)
	    8270.,(float).10051,(float).15621,(float).0034824,(float)
	    -2.1289e-5,(float).0019497,(float)-2.0952e-5,(float)8270.,(float)
	    8280.,(float).073404,(float).16218,(float).0017977,(float)
	    -1.3976e-5,(float)1.5476e-4,(float)-6.5472e-6,(float)8280.,(float)
	    8290.,(float).08523,(float).11256,(float)2.9553e-4,(float)
	    -9.9833e-6,(float)-1.2813e-4,(float)-4.6912e-6,(float)8290.,(
	    float)8300.,(float).1818,(float).19506,(float)-.0018708,(float)
	    -2.1763e-6,(float)-.0027279,(float)2.3065e-6,(float)8300.,(float)
	    8310.,(float).030278,(float).1674,(float).0055443,(float)
	    -2.3707e-5,(float).0051615,(float)-2.3428e-5,(float)8310.,(float)
	    8320.,(float).11462,(float).20492,(float)-.0010659,(float)
	    -9.4245e-7,(float)-.0022118,(float)4.2668e-6,(float)8320.,(float)
	    8330.,(float).099231,(float).19833,(float)-3.5775e-4,(float)
	    -3.838e-6,(float)-.0025607,(float)-3.6304e-7,(float)8330.,(float)
	    8340.,(float).099199,(float).18538,(float)-.0023017,(float)
	    6.6528e-6,(float)-.0023339,(float)6.1821e-6,(float)8340.,(float)
	    8350.,(float).059379,(float).077001,(float)-.004912,(float)
	    1.4256e-5,(float)-.0025954,(float)3.0037e-5,(float)8350.,(float)
	    8360.,(float).044008,(float).20401,(float).0014099,(float)
	    -5.1535e-6,(float).0019118,(float)-1.6011e-6,(float)8360.,(float)
	    8370.,(float).090539,(float).08076,(float)-.0035383,(float)
	    1.3223e-5,(float)-.0028138,(float)1.3001e-5,(float)8370.,(float)
	    8380.,(float).014326,(float).1809,(float).0059988,(float)7.331e-7,
	    (float).0076689,(float)-1.7035e-5,(float)8380.,(float)8390.,(
	    float).046664,(float).1734,(float)-8.7294e-4,(float)1.0561e-5,(
	    float)3.1024e-4,(float)2.7678e-6,(float)8390.,(float)8400.,(float)
	    .10171,(float).16186,(float)-.0017894,(float)-2.8284e-6,(float)
	    -.0040067,(float)-1.4559e-7,(float)8400.,(float)8410.,(float)
	    .30821,(float).25107,(float)-.0032854,(float)5.8502e-6,(float)
	    -.0036961,(float)9.3802e-6,(float)8410.,(float)8420.,(float)
	    .063199,(float).24205,(float)-.0011158,(float)3.7207e-6,(float)
	    -.0013624,(float)5.0058e-6,(float)8420.,(float)8430.,(float)
	    .099053,(float).16171,(float)-.0031354,(float)1.4107e-5,(float)
	    -4.1612e-4,(float)9.0938e-6,(float)8430.,(float)8440.,(float)
	    .18961,(float).16575,(float)-.0036207,(float)4.9966e-6,(float)
	    -.0041916,(float)8.2034e-6,(float)8440.,(float)8450.,(float)
	    .17725,(float).21239,(float)-7.7569e-4,(float)-2.0023e-6,(float)
	    -4.9558e-4,(float)-5.1555e-6,(float)8450.,(float)8460.,(float)
	    .27753,(float).33354,(float)9.0668e-4,(float)-7.5987e-6,(float)
	    1.7804e-4,(float)-6.4173e-6,(float)8460.,(float)8470.,(float)
	    .14123,(float).13127,(float)-1.2234e-4,(float)-8.3078e-6,(float)
	    -4.417e-4,(float)-4.4651e-6,(float)8470.,(float)8480.,(float)
	    .093615,(float).12378,(float)-.0022473,(float)6.446e-6,(float)
	    -.0023144,(float)2.1685e-5,(float)8480.,(float)8490.,(float)
	    .20449,(float).23918,(float).0011176,(float)-3.6761e-6,(float)
	    .0018,(float)-5.0232e-6,(float)8490.,(float)8500.,(float).089489,(
	    float).16701,(float).0039531,(float)-6.6471e-6,(float).0051864,(
	    float)4.9471e-6,(float)8500.,(float)8510.,(float).23406,(float)
	    .40842,(float).0043192,(float)-1.5846e-5,(float).0044314,(float)
	    -2.3087e-5,(float)8510.,(float)8520.,(float).14482,(float).32325,(
	    float).0041937,(float)5.6608e-6,(float).0044945,(float)3.2348e-7,(
	    float)8520.,(float)8530.,(float).064432,(float).26897,(float)
	    .0065578,(float)-1.2568e-5,(float).007607,(float)-2.0405e-5,(
	    float)8530.,(float)8540.,(float).057924,(float).33321,(float)
	    .0091543,(float)9.8398e-6,(float).0086335,(float)-1.8156e-5,(
	    float)8540.,(float)8550.,(float).10794,(float).40047,(float)
	    .010231,(float)1.2947e-5,(float).010777,(float)-2.3286e-5,(float)
	    8550.,(float)8560.,(float).081479,(float).27006,(float).010378,(
	    float)-1.4458e-5,(float).0096133,(float)-3.1676e-5,(float)8560.,(
	    float)8570.,(float).21551,(float).37486,(float).015271,(float)
	    -4.3087e-5,(float).011958,(float)-4.878e-5,(float)8570.,(float)
	    8580.,(float).1197,(float).42176,(float).011436,(float)-2.7559e-5,
	    (float).012343,(float)-4.8358e-5,(float)8580.,(float)8590.,(float)
	    .1194,(float).26383,(float).017025,(float)-6.3908e-5,(float)
	    .012895,(float)-5.3752e-5,(float)8590.,(float)8600.,(float).35022,
	    (float).24839,(float).015312,(float)-6.7204e-5,(float).011349,(
	    float)-5.723e-5,(float)8600.,(float)8610.,(float).43429,(float)
	    .32749,(float).014111,(float)-6.1858e-5,(float).0111,(float)
	    -5.2145e-5,(float)8610.,(float)8620.,(float).649,(float).17775,(
	    float).012005,(float)-5.7105e-5,(float).0091759,(float)-4.5878e-5,
	    (float)8620.,(float)8630.,(float).62583,(float).2559,(float)
	    .011761,(float)-5.603e-5,(float).0090211,(float)-4.8934e-5,(float)
	    8630.,(float)8640.,(float).3575,(float).36475,(float).010308,(
	    float)-4.3411e-5,(float).0088328,(float)-3.9083e-5,(float)8640.,(
	    float)8650.,(float)1.1133,(float).13506,(float).0080358,(float)
	    -4.3334e-5,(float).0068137,(float)-3.9624e-5,(float)8650.,(float)
	    8660.,(float)1.3387,(float).31665,(float).0078192,(float)
	    -3.8805e-5,(float).0049558,(float)-3.3187e-5,(float)8660.,(float)
	    8670.,(float)2.7498,(float).16946,(float).0049939,(float)
	    -3.0487e-5,(float).0031261,(float)-2.5786e-5,(float)8670.,(float)
	    8680.,(float)2.4398,(float).19531,(float).0044402,(float)
	    -2.9193e-5,(float).0022819,(float)-2.5054e-5,(float)8680.,(float)
	    8690.,(float)3.0993,(float).19328,(float).0022645,(float)
	    -1.9507e-5,(float)5.743e-4,(float)-1.6236e-5,(float)8690.,(float)
	    8700.,(float)1.2713,(float).17117,(float).0010656,(float)
	    -1.5353e-5,(float)-6.2313e-4,(float)-8.8734e-6,(float)8700.,(
	    float)8710.,(float)3.1649,(float).17696,(float).0010942,(float)
	    -1.5108e-5,(float)-8.8648e-4,(float)-8.7401e-6,(float)8710.,(
	    float)8720.,(float)3.1075,(float).11851,(float)-9.4639e-5,(float)
	    -9.2083e-6,(float)-3.6777e-4,(float)-6.4753e-6,(float)8720.,(
	    float)8730.,(float)7.0407,(float).19201,(float)-.0021209,(float)
	    -3.6303e-6,(float)-.0046941,(float)1.0611e-6,(float)8730.,(float)
	    8740.,(float)1.2454,(float).17319,(float)-.0019955,(float)
	    -1.4314e-6,(float)-.0021532,(float)-5.8242e-7,(float)8740.,(float)
	    8750.,(float).83443,(float).1489,(float)-.0031702,(float)3.926e-6,
	    (float)-.0022958,(float)3.8306e-6,(float)8750.,(float)8760.,(
	    float)3.6918,(float).11215,(float)-.0041869,(float)4.6517e-6,(
	    float)-.0065413,(float)1.1815e-5,(float)8760.,(float)8770.,(float)
	    .93114,(float).16247,(float)8.6756e-4,(float)-4.9431e-6,(float)
	    6.7174e-4,(float)-4.722e-6,(float)8770.,(float)8780.,(float)
	    2.5247,(float).085808,(float)-.004835,(float)9.227e-6,(float)
	    -.0060959,(float)1.2095e-5,(float)8780.,(float)8790.,(float)
	    .53387,(float).20115,(float)-.0016551,(float)7.1592e-6,(float)
	    -5.3003e-4,(float)9.888e-7,(float)8790.,(float)8800.,(float)
	    6.6475,(float).28681,(float)-.0018744,(float)3.5629e-6,(float)
	    -.0015672,(float)2.949e-6,(float)8800.,(float)8810.,(float)13.449,
	    (float).46562,(float).0010817,(float)-8.1158e-6,(float)4.4146e-4,(
	    float)-9.6568e-6,(float)8810.,(float)8820.,(float)2.3072,(float)
	    .29228,(float).0018897,(float)4.5344e-6,(float).0023839,(float)
	    -8.7201e-6,(float)8820.,(float)8830.,(float).38221,(float).2037,(
	    float)4.9695e-4,(float)6.5162e-6,(float).0021768,(float)3.2622e-6,
	    (float)8830.,(float)8840.,(float)1.1249,(float).14056,(float)
	    -.0038223,(float)1.6534e-5,(float)2.8673e-4,(float)1.9579e-5,(
	    float)8840.,(float)8850.,(float)4.8272,(float).098788,(float)
	    -.005013,(float)9.0675e-6,(float)-.005665,(float)1.3142e-5,(float)
	    8850.,(float)8860.,(float)7.562,(float).14568,(float)-.0043638,(
	    float)5.3176e-6,(float)-.0063322,(float)1.074e-5,(float)8860.,(
	    float)8870.,(float)2.4448,(float).13155,(float)-.002417,(float)
	    -2.7983e-7,(float)-.0034547,(float)3.4138e-6,(float)8870.,(float)
	    8880.,(float)8.0582,(float).19292,(float)-.0028534,(float)
	    -8.0358e-7,(float)-.0051566,(float)3.9531e-6,(float)8880.,(float)
	    8890.,(float)5.7451,(float).18168,(float)-5.0148e-4,(float)
	    -9.5111e-6,(float)-.0019306,(float)-6.0172e-6,(float)8890.,(float)
	    8900.,(float)7.0547,(float).19464,(float)-.0010049,(float)
	    -7.4656e-6,(float)-.0025268,(float)-4.3122e-6,(float)8900.,(float)
	    8910.,(float)5.4039,(float).15719,(float).0016933,(float)
	    -1.8412e-5,(float)5.6219e-4,(float)-1.6145e-5,(float)8910.,(float)
	    8920.,(float)3.5653,(float).19909,(float).0033052,(float)
	    -2.2923e-5,(float)5.191e-4,(float)-1.4385e-5,(float)8920.,(float)
	    8930.,(float)2.9405,(float).30828,(float).0050674,(float)
	    -2.9116e-5,(float).0027789,(float)-2.3443e-5,(float)8930.,(float)
	    8940.,(float)1.9927,(float).15337,(float).0040525,(float)
	    -2.4922e-5,(float).0021726,(float)-1.8937e-5,(float)8940.,(float)
	    8950.,(float)3.1994,(float).39446,(float).0066249,(float)
	    -3.4407e-5,(float).0041704,(float)-2.98e-5,(float)8950.,(float)
	    8960.,(float)1.0124,(float).3051,(float).0095861,(float)-4.26e-5,(
	    float).007142,(float)-3.9564e-5,(float)8960.,(float)8970.,(float)
	    .55494,(float).31765,(float).012349,(float)-5.0981e-5,(float)
	    .0084323,(float)-3.9712e-5,(float)8970.,(float)8980.,(float)
	    .62962,(float).34745,(float).0068325,(float)-6.7823e-6,(float)
	    .006612,(float)-2.2655e-5,(float)8980.,(float)8990.,(float).39155,
	    (float).17506,(float).014525,(float)-6.4228e-5,(float).012836,(
	    float)-5.854e-5,(float)8990.,(float)9e3,(float).29338,(float)
	    .19531,(float).014921,(float)-5.7995e-5,(float).012214,(float)
	    -4.6413e-5,(float)9e3,(float)9010.,(float).117,(float).34769,(
	    float).013815,(float)-1.4315e-5,(float).011336,(float)-3.2553e-5,(
	    float)9010.,(float)9020.,(float).16223,(float).35064,(float)
	    .0089364,(float)2.1825e-5,(float).010979,(float)-2.503e-5,(float)
	    9020.,(float)9030.,(float).021742,(float).25837,(float).021279,(
	    float)-6.9818e-5,(float).017662,(float)-6.8568e-5,(float)9030.,(
	    float)9040.,(float).056078,(float).27822,(float).012493,(float)
	    1.8431e-5,(float).013824,(float)-2.7676e-5,(float)9040.,(float)
	    9050.,(float).050045,(float).24927,(float).0025877,(float)
	    4.5316e-5,(float).0058234,(float)9.7906e-6,(float)9050.,(float)
	    9060.,(float).074041,(float).1688,(float).0031016,(float)
	    -1.2901e-5,(float).0031503,(float)-9.5272e-6,(float)9060.,(float)
	    9070.,(float).035788,(float).25671,(float).0055742,(float)
	    -3.3952e-6,(float).0067343,(float)-9.7505e-6,(float)9070.,(float)
	    9080.,(float).057922,(float).23584,(float).0022587,(float)
	    -9.2537e-6,(float).0015239,(float)-7.0447e-6,(float)9080.,(float)
	    9090.,(float).033597,(float).22977,(float).0030824,(float)
	    -1.1293e-5,(float).0033924,(float)-5.6695e-6,(float)9090.,(float)
	    9100.,(float).070807,(float).38398,(float).0033167,(float)
	    -8.2422e-6,(float).002352,(float)-1.7904e-5,(float)9100.,(float)
	    9110.,(float).007025,(float).18707,(float).008814,(float)
	    -2.739e-5,(float).008404,(float)-2.9932e-5,(float)9110.,(float)
	    9120.,(float).069936,(float).25325,(float).0019174,(float)
	    -1.1175e-5,(float).001279,(float)-1.4168e-5,(float)9120.,(float)
	    9130.,(float).052387,(float).30346,(float)9.3076e-4,(float)
	    5.2452e-6,(float).0021985,(float)-1.2943e-5,(float)9130.,(float)
	    9140.,(float).015639,(float).086845,(float).0072697,(float)
	    -3.4455e-5,(float).0051914,(float)-3.2991e-5,(float)9140.,(float)
	    9150.,(float).091802,(float).17031,(float)-5.4142e-4,(float)
	    2.9713e-6,(float)-.0010743,(float)-2.3472e-6,(float)9150.,(float)
	    9160.,(float).011168,(float).099076,(float).005622,(float)
	    -3.0231e-5,(float).0047197,(float)-2.3663e-5,(float)9160.,(float)
	    9170.,(float).048371,(float).1417,(float)-5.9903e-4,(float)
	    -4.5058e-6,(float)-.0011328,(float)9.448e-7,(float)9170.,(float)
	    9180.,(float).018874,(float).15366,(float).0042614,(float)
	    -2.6279e-5,(float)8.9656e-4,(float)-2.0193e-5,(float)9180.,(float)
	    9190.,(float).016112,(float).12245,(float).0061155,(float)
	    -3.4764e-5,(float).0039418,(float)-3.163e-5,(float)9190.,(float)
	    9200.,(float).065149,(float).14651,(float).0013623,(float)
	    -1.4483e-5,(float)1.0964e-5,(float)-1.1796e-5,(float)9200.,(float)
	    9210.,(float).011149,(float).21185,(float).0041991,(float)
	    -1.3718e-5,(float).0030892,(float)-2.0781e-5,(float)9210.,(float)
	    9220.,(float).031171,(float).12753,(float).0045967,(float)
	    -2.5041e-5,(float).0038974,(float)-2.7102e-5,(float)9220.,(float)
	    9230.,(float)4.0823e-4,(float).030563,(float).0247,(float)
	    -1.1119e-4,(float).021942,(float)-1.0564e-4,(float)9230.,(float)
	    9240.,(float).012977,(float).16914,(float).0055548,(float)
	    -2.5826e-5,(float).0033619,(float)-2.2004e-5,(float)9240.,(float)
	    9250.,(float).020816,(float).10896,(float).0066122,(float)
	    -3.4731e-5,(float).0052212,(float)-3.4187e-5,(float)9250.,(float)
	    9260.,(float).0030262,(float).11279,(float).010901,(float)
	    -5.2842e-5,(float).0086449,(float)-4.5417e-5,(float)9260.,(float)
	    9270.,(float).0033673,(float).11423,(float).010899,(float)
	    -3.4157e-5,(float).0080866,(float)-3.8803e-5,(float)9270.,(float)
	    9280.,(float).017536,(float).16659,(float).0083759,(float)
	    -4.46e-5,(float).005112,(float)-3.6571e-5,(float)9280.,(float)
	    9290.,(float)8.1624e-4,(float).16081,(float).015916,(float)
	    -6.3848e-5,(float).013156,(float)-6.3084e-5,(float)9290.,(float)
	    9300.,(float).011313,(float).13057,(float).011852,(float)
	    -5.8953e-5,(float).0088407,(float)-5.1297e-5,(float)9300.,(float)
	    9310.,(float).0015081,(float).069261,(float).0041982,(float)
	    -7.5358e-6,(float).0057185,(float)-1.6686e-5,(float)9310.,(float)
	    9320.,(float).0045702,(float).1342,(float).013143,(float)
	    -5.4394e-5,(float).01017,(float)-5.2848e-5,(float)9320.,(float)
	    9330.,(float)7.5901e-4,(float).16485,(float).017789,(float)
	    -4.1657e-5,(float).014232,(float)-5.1704e-5,(float)9330.,(float)
	    9340.,(float).0023315,(float).14437,(float).015924,(float)
	    -7.3095e-5,(float).012997,(float)-6.7546e-5,(float)9340.,(float)
	    9350.,(float).0024761,(float).13342,(float).017367,(float)
	    -6.5679e-5,(float).014468,(float)-6.2866e-5,(float)9350.,(float)
	    9360.,(float).0011255,(float).11586,(float).016217,(float)
	    -5.9242e-5,(float).013009,(float)-5.7135e-5,(float)9360.,(float)
	    9370.,(float)4.6631e-4,(float).031553,(float).0231,(float)
	    -1.0479e-4,(float).020342,(float)-9.924e-5,(float)9370.,(float)
	    9380.,(float)5.8334e-4,(float).14333,(float).014779,(float)
	    -4.7773e-5,(float).011406,(float)-5.2169e-5,(float)9380.,(float)
	    9390.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)9390.,(float)9400.,(float)4.6492e-4,(float).064777,(float)
	    .012413,(float)-2.9282e-5,(float).013774,(float)-4.9601e-5,(float)
	    9400.,(float)9410.,(float)4.0085e-4,(float).030483,(float).010823,
	    (float)-5.5684e-5,(float).0080667,(float)-5.0135e-5,(float)9410.,(
	    float)9420.,(float)6.5363e-5,(float).053752,(float).0231,(float)
	    -1.0479e-4,(float).020342,(float)-9.924e-5,(float)9420.,(float)
	    9430.,(float)2.4119e-5,(float).027609,(float).03515,(float)
	    -1.5299e-4,(float).032393,(float)-1.4744e-4,(float)9430.,(float)
	    9440.,(float)1.3718e-4,(float).04682,(float).01614,(float)
	    -5.2767e-5,(float).016119,(float)-6.7234e-5,(float)9440.,(float)
	    9450.,(float)5.534e-5,(float).046161,(float).016226,(float)
	    -4.5595e-5,(float).015446,(float)-5.9474e-5,(float)9450.,(float)
	    9460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)9460.,(float)9470.,(float)5.7428e-5,(float).063455,(float)
	    .017377,(float)-4.729e-5,(float).016614,(float)-6.2421e-5,(float)
	    9470.,(float)9480.,(float)3.8406e-5,(float).031771,(float)
	    .0046608,(float)-3.1037e-5,(float).0019036,(float)-2.5485e-5,(
	    float)9480.,(float)9490.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)9490.,(float)9500.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)9500.,(float)9510.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    9510.,(float)9520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)9520.,(float)9530.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)9530.,(float)9540.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9540.,
	    (float)9550.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9550.,(float)9560.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9560.,(float)9570.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9570.,(
	    float)9580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9580.,(float)9590.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9590.,(float)9600.,(float)
	    2.4903e-5,(float).024969,(float).025759,(float)-1.1543e-4,(float)
	    .023002,(float)-1.0988e-4,(float)9600.,(float)9610.,(float)
	    4.8976e-5,(float).034814,(float).004643,(float)-3.0966e-5,(float)
	    .0018858,(float)-2.5414e-5,(float)9610.,(float)9620.,(float)
	    6.6257e-5,(float).052528,(float).020662,(float)-9.5042e-5,(float)
	    .017905,(float)-8.949e-5,(float)9620.,(float)9630.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9630.,(
	    float)9640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9640.,(float)9650.,(float)1.9405e-4,(float).0908,(
	    float).012893,(float)-4.7907e-5,(float).008999,(float)-4.3597e-5,(
	    float)9650.,(float)9660.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)9660.,(float)9670.,(float)3.5455e-4,(
	    float).062906,(float).01178,(float)-5.9514e-5,(float).0090225,(
	    float)-5.3961e-5,(float)9670.,(float)9680.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)9680.,(float)9690.,(
	    float)7.3346e-4,(float).095357,(float).0080776,(float)-3.5126e-5,(
	    float).0063761,(float)-3.2075e-5,(float)9690.,(float)9700.,(float)
	    2.6888e-4,(float).062321,(float).01172,(float)-5.6759e-5,(float)
	    .0098726,(float)-5.5238e-5,(float)9700.,(float)9710.,(float)
	    4.8999e-4,(float).10498,(float).0062757,(float)-3.2864e-5,(float)
	    .0049464,(float)-3.2626e-5,(float)9710.,(float)9720.,(float)
	    .0010507,(float).071841,(float).0058574,(float)-3.2015e-5,(float)
	    .0039666,(float)-3.1106e-5,(float)9720.,(float)9730.,(float)
	    .0025494,(float).10376,(float).0030444,(float)-2.0413e-5,(float)
	    .0012384,(float)-1.8532e-5,(float)9730.,(float)9740.,(float)
	    .0013019,(float).13387,(float).0019451,(float)-1.3612e-5,(float)
	    -8.9213e-6,(float)-1.3158e-5,(float)9740.,(float)9750.,(float)
	    6.2353e-4,(float).078233,(float)7.0756e-4,(float)-1.519e-5,(float)
	    -.0020739,(float)-9.6993e-6,(float)9750.,(float)9760.,(float)
	    .0056535,(float).1971,(float)1.6071e-4,(float)-5.7489e-6,(float)
	    -.0014878,(float)-4.8238e-6,(float)9760.,(float)9770.,(float)
	    .001909,(float).041123,(float)-.0019069,(float)-4.7662e-6,(float)
	    -.004664,(float)7.8558e-7,(float)9770.,(float)9780.,(float)
	    .0017619,(float).13162,(float)-.0022639,(float)7.7041e-7,(float)
	    -.0049756,(float)4.3744e-6,(float)9780.,(float)9790.,(float)
	    5.7144e-4,(float).099432,(float).0040501,(float)-1.5471e-5,(float)
	    .0018591,(float)-1.7658e-5,(float)9790.,(float)9800.,(float)
	    .0038209,(float).13329,(float)-.0046298,(float)6.3691e-6,(float)
	    -.0077028,(float)1.2343e-5,(float)9800.,(float)9810.,(float)
	    .0017147,(float).079407,(float)4.5502e-4,(float)-1.3444e-5,(float)
	    -.0030355,(float)-5.7728e-6,(float)9810.,(float)9820.,(float)
	    .0012145,(float).072418,(float)2.5064e-4,(float)-5.9535e-6,(float)
	    3.3766e-6,(float)-6.2695e-6,(float)9820.,(float)9830.,(float)
	    .0011057,(float).13614,(float).0018444,(float)-3.817e-6,(float)
	    -6.6263e-4,(float)-6.1358e-6,(float)9830.,(float)9840.,(float)
	    .0021971,(float).053016,(float)-.0048189,(float)9.7527e-6,(float)
	    -.0060279,(float)1.7406e-5,(float)9840.,(float)9850.,(float)
	    .0019933,(float).17956,(float)-.0010152,(float)7.3537e-6,(float)
	    -.0033437,(float)4.3246e-6,(float)9850.,(float)9860.,(float)
	    8.7206e-4,(float).047547,(float)-.0041578,(float)4.2375e-6,(float)
	    -.007361,(float)1.0687e-5,(float)9860.,(float)9870.,(float)
	    .0029909,(float).069285,(float)-.0047147,(float)1.0991e-5,(float)
	    -.0056075,(float)1.5292e-5,(float)9870.,(float)9880.,(float)
	    .001506,(float).17429,(float).003529,(float)-1.5241e-5,(float)
	    9.8555e-4,(float)-1.3485e-5,(float)9880.,(float)9890.,(float)
	    .0024655,(float).15187,(float)-9.6328e-4,(float)3.0116e-6,(float)
	    -.002379,(float)6.199e-6,(float)9890.,(float)9900.,(float)
	    .0081762,(float).23349,(float)-.0033911,(float)1.8786e-6,(float)
	    -.0060119,(float)6.563e-6,(float)9900.,(float)9910.,(float)
	    .0035952,(float).09571,(float)-.002565,(float)6.9268e-6,(float)
	    -.0025302,(float)8.7846e-6,(float)9910.,(float)9920.,(float)
	    .004102,(float).16571,(float)-8.2348e-4,(float)4.4729e-6,(float)
	    5.0555e-5,(float)-1.3431e-6,(float)9920.,(float)9930.,(float)
	    .0015616,(float).12559,(float)-.001712,(float)9.5737e-7,(float)
	    -.002329,(float)2.8914e-6,(float)9930.,(float)9940.,(float)
	    .0029147,(float).1397,(float).0017969,(float)-5.9773e-6,(float)
	    .0040034,(float)-1.6529e-5,(float)9940.,(float)9950.,(float)
	    .0021013,(float).0985,(float).0026194,(float)-1.629e-5,(float)
	    .0026829,(float)-1.8333e-5,(float)9950.,(float)9960.,(float)
	    .0025118,(float).078548,(float)-.0011652,(float)-3.1932e-6,(float)
	    -.0017647,(float)-3.6546e-6,(float)9960.,(float)9970.,(float)
	    .0054922,(float).27847,(float).0022288,(float)-6.2983e-6,(float)
	    .0011941,(float)-9.9545e-6,(float)9970.,(float)9980.,(float)
	    .0077074,(float).24403,(float).0023234,(float)-1.7376e-5,(float)
	    9.1247e-4,(float)-1.6357e-5,(float)9980.,(float)9990.,(float)
	    .0015919,(float).17498,(float).004876,(float)-1.537e-5,(float)
	    .0030207,(float)-2.2305e-5,(float)9990.,(float)1e4,(float)
	    .0023052,(float).12024,(float).0026434,(float)3.4883e-6,(float)
	    .0053038,(float)-7.4735e-6,(float)1e4,(float)10010.,(float)
	    9.9075e-4,(float).060379,(float).0048519,(float)-2.4596e-5,(float)
	    .0039231,(float)-2.671e-5,(float)10010.,(float)10020.,(float)
	    .0026106,(float).15994,(float).0057076,(float)-1.5791e-5,(float)
	    .0063216,(float)-2.8717e-5,(float)10020.,(float)10030.,(float)
	    9.0384e-4,(float).21421,(float).0073532,(float)-1.3113e-6,(float)
	    .009333,(float)-1.9232e-5,(float)10030.,(float)10040.,(float)
	    .0020888,(float).22211,(float).0093854,(float)-1.1006e-5,(float)
	    .0077851,(float)-1.8919e-5,(float)10040.,(float)10050.,(float)
	    .0010096,(float).093543,(float).0051826,(float)-2.8108e-5,(float)
	    .0041051,(float)-2.3759e-5,(float)10050.,(float)10060.,(float)
	    .0022561,(float).25885,(float).013532,(float)-1.4879e-5,(float)
	    .011628,(float)-3.4287e-5,(float)10060.,(float)10070.,(float)
	    .0077191,(float).28721,(float).0097888,(float)-1.1073e-5,(float)
	    .008539,(float)-2.4771e-5,(float)10070.,(float)10080.,(float)
	    .0034195,(float).20647,(float).0074823,(float)-2.1978e-5,(float)
	    .007444,(float)-2.4751e-5,(float)10080.,(float)10090.,(float)
	    .0063694,(float).19871,(float).011877,(float)-3.28e-5,(float)
	    .009567,(float)-3.9672e-5,(float)10090.,(float)10100.,(float)
	    .0063072,(float).3731,(float).0047744,(float)2.8648e-5,(float)
	    .006887,(float)-8.4982e-6,(float)10100.,(float)10110.,(float)
	    .0010617,(float).15783,(float).019027,(float)-3.7386e-5,(float)
	    .017346,(float)-6.0605e-5,(float)10110.,(float)10120.,(float)
	    .016376,(float).34661,(float).014148,(float)-4.8223e-5,(float)
	    .010187,(float)-4.3401e-5,(float)10120.,(float)10130.,(float)
	    .0077156,(float).39174,(float).0071212,(float)8.3584e-6,(float)
	    .0074196,(float)-2.0778e-5,(float)10130.,(float)10140.,(float)
	    .0081991,(float).34376,(float).010657,(float)-2.049e-5,(float)
	    .009186,(float)-3.4657e-5,(float)10140.,(float)10150.,(float)
	    .045748,(float).35631,(float).011031,(float)-4.1938e-5,(float)
	    .0079877,(float)-3.3077e-5,(float)10150.,(float)10160.,(float)
	    .026803,(float).2161,(float).011739,(float)-5.3624e-5,(float)
	    .010169,(float)-5.0362e-5,(float)10160.,(float)10170.,(float)
	    .061462,(float).24685,(float).008227,(float)-3.1656e-5,(float)
	    .0077778,(float)-3.1665e-5,(float)10170.,(float)10180. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/

/*     WATER VAPOR (7620 - 10160 CM-1) */

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
} /* wava3_ */

#ifdef __cplusplus
	}
#endif
