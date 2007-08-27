/* WAVA4.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       SUBROUTINE    WAVA4(a,inu) >*/
/* Subroutine */ int wava4_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float).037011,(float)
	    .34865,(float).0071795,(float)-2.429e-5,(float).0061217,(float)
	    -2.5788e-5,(float)10180.,(float)10190.,(float).096531,(float)
	    .1963,(float).0044353,(float)-2.7769e-5,(float).0020496,(float)
	    -1.902e-5,(float)10190.,(float)10200.,(float).11553,(float).22356,
	    (float).0057418,(float)-2.861e-5,(float).005252,(float)-3.0274e-5,
	    (float)10200.,(float)10210.,(float).17877,(float).25357,(float)
	    .0036991,(float)-1.9865e-5,(float).0035644,(float)-1.5681e-5,(
	    float)10210.,(float)10220.,(float).094449,(float).35387,(float)
	    .0021016,(float)-1.5037e-5,(float)5.8316e-4,(float)-1.1084e-5,(
	    float)10220.,(float)10230.,(float).079869,(float).2603,(float)
	    .0051322,(float)-2.0923e-5,(float).0039204,(float)-2.2157e-5,(
	    float)10230.,(float)10240.,(float).30687,(float).32758,(float)
	    6.3705e-4,(float)-9.1396e-6,(float)2.5647e-4,(float)-3.6374e-6,(
	    float)10240.,(float)10250.,(float).15358,(float).13982,(float)
	    -8.3666e-4,(float)3.3048e-6,(float).0027185,(float)1.2493e-5,(
	    float)10250.,(float)10260.,(float).38232,(float).35543,(float)
	    -.0010313,(float)-1.8768e-6,(float)-.0016001,(float)3.8356e-7,(
	    float)10260.,(float)10270.,(float).0562,(float).34487,(float)
	    .0036934,(float)7.3751e-6,(float).0070064,(float)-1.2975e-5,(
	    float)10270.,(float)10280.,(float).11137,(float).21188,(float)
	    -.0024327,(float)1.6553e-5,(float)-3.0535e-4,(float)1.9603e-5,(
	    float)10280.,(float)10290.,(float).16704,(float).162,(float)
	    -.0028871,(float)9.8108e-6,(float)-4.298e-4,(float)1.7309e-5,(
	    float)10290.,(float)10300.,(float).15496,(float).19365,(float)
	    -.0025906,(float)1.7434e-5,(float)-3.5463e-4,(float)8.1537e-6,(
	    float)10300.,(float)10310.,(float).050239,(float).44504,(float)
	    .0082014,(float)4.7362e-5,(float).0097304,(float)-1.0209e-5,(
	    float)10310.,(float)10320.,(float).21509,(float).29651,(float)
	    -.0021764,(float)2.0847e-5,(float)-2.3124e-4,(float)1.0958e-5,(
	    float)10320.,(float)10330.,(float).15838,(float).17695,(float)
	    -.0019477,(float)2.8641e-6,(float)-.0031011,(float)6.2742e-6,(
	    float)10330.,(float)10340.,(float).45807,(float).42831,(float)
	    -4.7039e-5,(float)1.6616e-5,(float).001452,(float)6.4036e-6,(
	    float)10340.,(float)10350.,(float).14196,(float).4315,(float)
	    .0032845,(float)1.8573e-5,(float).0044632,(float)-3.8921e-6,(
	    float)10350.,(float)10360.,(float).46768,(float).4897,(float)
	    .0030587,(float)-9.9296e-6,(float).0017377,(float)-1.2227e-5,(
	    float)10360.,(float)10370.,(float).49107,(float).45881,(float)
	    .0017676,(float)2.3059e-5,(float).0037946,(float)-7.9699e-6,(
	    float)10370.,(float)10380.,(float).40709,(float).39513,(float)
	    .0024501,(float)3.4328e-6,(float).0033911,(float)-1.6475e-5,(
	    float)10380.,(float)10390.,(float).53368,(float).30449,(float)
	    .002764,(float)1.6491e-5,(float).0031852,(float)-7.9335e-6,(float)
	    10390.,(float)10400.,(float).50598,(float).38596,(float).0011335,(
	    float)7.9179e-6,(float).0016436,(float)-4.2442e-6,(float)10400.,(
	    float)10410.,(float).36712,(float).58535,(float).0035486,(float)
	    -2.5563e-6,(float).0025513,(float)-1.2028e-5,(float)10410.,(float)
	    10420.,(float).98775,(float).50104,(float).0056199,(float)
	    -1.6902e-5,(float).0043563,(float)-2.6045e-5,(float)10420.,(float)
	    10430.,(float).76493,(float).41145,(float).0027287,(float)
	    -1.2684e-5,(float)9.6642e-4,(float)-1.2013e-5,(float)10430.,(
	    float)10440.,(float).4518,(float).45681,(float).0055394,(float)
	    -1.8929e-5,(float).0049043,(float)-2.5553e-5,(float)10440.,(float)
	    10450.,(float).93845,(float).25977,(float).0063165,(float)
	    -3.3633e-5,(float).0030572,(float)-2.3591e-5,(float)10450.,(float)
	    10460.,(float)1.0799,(float).52983,(float).0049753,(float)
	    -2.2413e-5,(float).0037285,(float)-1.9018e-5,(float)10460.,(float)
	    10470.,(float)1.9954,(float).24666,(float).0043881,(float)
	    -2.9035e-5,(float).0020409,(float)-2.1664e-5,(float)10470.,(float)
	    10480.,(float).53954,(float).3608,(float).0038367,(float)
	    -2.492e-5,(float).0016345,(float)-2.0659e-5,(float)10480.,(float)
	    10490.,(float)1.9042,(float).28357,(float).0020331,(float)
	    -1.9809e-5,(float)1.178e-4,(float)-1.5063e-5,(float)10490.,(float)
	    10500.,(float)1.3639,(float).41799,(float).0017903,(float)
	    -1.4939e-5,(float)7.7921e-4,(float)-1.3895e-5,(float)10500.,(
	    float)10510.,(float).27245,(float).53566,(float).0028306,(float)
	    -2.1067e-6,(float).0041171,(float)-1.2108e-5,(float)10510.,(float)
	    10520.,(float)4.225,(float).34405,(float)-9.2601e-4,(float)
	    -5.8016e-6,(float)-.0020236,(float)-9.379e-7,(float)10520.,(float)
	    10530.,(float).41591,(float).38497,(float).004158,(float)
	    5.6585e-6,(float).0031671,(float)-1.1552e-5,(float)10530.,(float)
	    10540.,(float)2.4868,(float).21796,(float)-.0020342,(float)
	    2.1116e-6,(float)-.0020977,(float)3.2759e-6,(float)10540.,(float)
	    10550.,(float).9442,(float).51349,(float).0018825,(float)
	    2.2079e-6,(float).0015856,(float)-8.8233e-6,(float)10550.,(float)
	    10560.,(float)2.139,(float).25647,(float)-.0019634,(float)
	    7.9808e-6,(float)-.0024854,(float)4.6652e-6,(float)10560.,(float)
	    10570.,(float)1.3917,(float).33676,(float).0035449,(float)
	    -2.0344e-5,(float)3.2271e-4,(float)-1.2565e-5,(float)10570.,(
	    float)10580.,(float)5.9303,(float).30084,(float)1.3471e-4,(float)
	    -5.1658e-6,(float)-.0018823,(float)-4.8751e-6,(float)10580.,(
	    float)10590.,(float)1.0087,(float).13259,(float)-.0014095,(float)
	    -4.7735e-6,(float)-.0024541,(float)2.6832e-6,(float)10590.,(float)
	    10600.,(float)4.3983,(float).19046,(float)-.0034725,(float)
	    2.4224e-6,(float)-.0053127,(float)7.1676e-6,(float)10600.,(float)
	    10610.,(float).96572,(float).30982,(float)-.0027372,(float)
	    6.8946e-6,(float)-.0023672,(float)8.3925e-6,(float)10610.,(float)
	    10620.,(float).91323,(float).32206,(float)-.0024872,(float)
	    3.2962e-6,(float)-.0026005,(float)2.0821e-6,(float)10620.,(float)
	    10630.,(float).64383,(float).32142,(float)-.0040036,(float)
	    1.1751e-5,(float)-.0046913,(float)1.096e-5,(float)10630.,(float)
	    10640.,(float).20419,(float).32226,(float)-.0010057,(float)
	    3.7593e-7,(float)-.0012761,(float)-1.9796e-6,(float)10640.,(float)
	    10650.,(float)3.2582,(float).41089,(float)-.0042525,(float)
	    1.1056e-5,(float)-.003553,(float)1.043e-5,(float)10650.,(float)
	    10660.,(float)4.0011,(float).26987,(float)-.0041265,(float)
	    6.8604e-6,(float)-.0047731,(float)8.9525e-6,(float)10660.,(float)
	    10670.,(float)2.3087,(float).2861,(float)-.0029291,(float)
	    1.5494e-6,(float)-.0036647,(float)3.1219e-6,(float)10670.,(float)
	    10680.,(float)5.0379,(float).43636,(float)-4.8143e-4,(float)
	    6.8393e-6,(float)-.0011602,(float)8.769e-7,(float)10680.,(float)
	    10690.,(float)3.4572,(float).24029,(float)-3.8723e-4,(float)
	    -8.7391e-6,(float)-4.4231e-4,(float)-7.617e-6,(float)10690.,(
	    float)10700.,(float)3.4164,(float).31894,(float)5.1822e-4,(float)
	    -9.7127e-6,(float)-8.7127e-5,(float)-1.1526e-5,(float)10700.,(
	    float)10710.,(float)1.9975,(float).24091,(float).0020391,(float)
	    -1.8108e-5,(float)8.1249e-4,(float)-1.3281e-5,(float)10710.,(
	    float)10720.,(float)1.8993,(float).38637,(float).0046303,(float)
	    -2.5016e-5,(float).0035053,(float)-2.2372e-5,(float)10720.,(float)
	    10730.,(float)1.958,(float).33984,(float).0057686,(float)
	    -3.2398e-5,(float).0041169,(float)-2.863e-5,(float)10730.,(float)
	    10740.,(float).56316,(float).41883,(float).011363,(float)
	    -4.522e-5,(float).0082366,(float)-3.7842e-5,(float)10740.,(float)
	    10750.,(float).35401,(float).69851,(float).010496,(float)
	    -2.1851e-5,(float).0066866,(float)-2.4626e-5,(float)10750.,(float)
	    10760.,(float).19707,(float).69982,(float).010773,(float)
	    -9.278e-6,(float).0083529,(float)-1.8664e-5,(float)10760.,(float)
	    10770.,(float).2189,(float).61722,(float).0026481,(float)
	    1.4368e-5,(float).0037728,(float)-1.6702e-7,(float)10770.,(float)
	    10780.,(float).055267,(float).6593,(float).007075,(float)
	    3.4583e-6,(float).0056327,(float)-1.8713e-5,(float)10780.,(float)
	    10790.,(float).082626,(float).56771,(float).0014488,(float)
	    9.0784e-6,(float).0015016,(float)-4.5502e-6,(float)10790.,(float)
	    10800.,(float).070929,(float).31037,(float)3.4662e-6,(float)
	    4.3968e-6,(float)-6.1123e-4,(float)-1.1496e-7,(float)10800.,(
	    float)10810.,(float).097418,(float).44409,(float)-3.1642e-4,(
	    float)5.1491e-6,(float)-.0010289,(float)-1.8238e-6,(float)10810.,(
	    float)10820.,(float).084947,(float).43514,(float).0039381,(float)
	    -1.2895e-6,(float).0026186,(float)-1.3069e-5,(float)10820.,(float)
	    10830.,(float).05826,(float).18252,(float).0019735,(float)
	    -6.3696e-6,(float)-5.8857e-4,(float)-5.1048e-6,(float)10830.,(
	    float)10840.,(float).11036,(float).40791,(float).0027504,(float)
	    5.4669e-6,(float).0010814,(float)-7.3999e-6,(float)10840.,(float)
	    10850.,(float).063822,(float).3839,(float).0050367,(float)
	    -2.0231e-5,(float).0028937,(float)-1.8685e-5,(float)10850.,(float)
	    10860.,(float).010028,(float).29145,(float).0059963,(float)
	    -2.3315e-5,(float).0034252,(float)-2.032e-5,(float)10860.,(float)
	    10870.,(float).12414,(float).24481,(float).0065529,(float)
	    -3.7402e-5,(float).0035139,(float)-3.0249e-5,(float)10870.,(float)
	    10880.,(float).14508,(float).36269,(float)-3.4622e-4,(float)
	    7.2138e-6,(float)-3.3853e-4,(float)7.4773e-7,(float)10880.,(float)
	    10890.,(float).32952,(float).51235,(float).001156,(float)
	    -9.9464e-6,(float)-.0017097,(float)-6.9684e-6,(float)10890.,(
	    float)10900.,(float).081689,(float).43746,(float).002472,(float)
	    -8.5593e-6,(float)8.107e-4,(float)-1.0007e-5,(float)10900.,(float)
	    10910.,(float).36297,(float).47635,(float)-7.551e-4,(float)
	    3.2155e-6,(float)-.001416,(float)1.3385e-6,(float)10910.,(float)
	    10920.,(float).21833,(float).56037,(float).0015815,(float)
	    -1.3282e-5,(float)2.7446e-4,(float)-1.2751e-5,(float)10920.,(
	    float)10930.,(float).062283,(float).46899,(float).003716,(float)
	    9.7049e-7,(float).0030288,(float)-1.3634e-5,(float)10930.,(float)
	    10940.,(float).49012,(float).36842,(float)-8.7423e-4,(float)
	    -6.7163e-6,(float)-.0022164,(float)-2.6838e-6,(float)10940.,(
	    float)10950.,(float).14742,(float).42276,(float).0021422,(float)
	    5.937e-6,(float)8.8387e-4,(float)-6.8671e-6,(float)10950.,(float)
	    10960.,(float).281,(float).22881,(float)-.0022496,(float)
	    -9.4548e-7,(float)-.0031883,(float)4.4808e-7,(float)10960.,(float)
	    10970.,(float).22513,(float).41493,(float).0036269,(float)
	    -1.6185e-5,(float).0017037,(float)-1.7793e-5,(float)10970.,(float)
	    10980.,(float).21994,(float).21688,(float)-.0036261,(float)
	    5.7974e-6,(float)-.0041527,(float)8.0739e-6,(float)10980.,(float)
	    10990.,(float).14084,(float).33322,(float)1.3752e-4,(float)
	    -6.2038e-6,(float)-.0011146,(float)-6.0724e-6,(float)10990.,(
	    float)1.1e4,(float).323,(float).17636,(float)-.0013536,(float)
	    -5.8665e-7,(float)-.0024854,(float)-3.0662e-7,(float)1.1e4,(float)
	    11010.,(float).4018,(float).22575,(float)-.0022932,(float)
	    -2.3319e-6,(float)-.0044015,(float)2.451e-6,(float)11010.,(float)
	    11020.,(float).40187,(float).17827,(float)-.0028707,(float)
	    1.5665e-6,(float)-.0044357,(float)3.1485e-6,(float)11020.,(float)
	    11030.,(float).096411,(float).21466,(float)-.0022778,(float)
	    4.124e-6,(float)-.0027585,(float)2.0224e-6,(float)11030.,(float)
	    11040.,(float).096343,(float).15703,(float)-.0022356,(float)
	    1.5369e-6,(float)-.0030654,(float)-3.2505e-7,(float)11040.,(float)
	    11050.,(float).079848,(float).17965,(float)-.0020125,(float)
	    9.6504e-6,(float)-.00326,(float)5.3659e-6,(float)11050.,(float)
	    11060.,(float).062139,(float).10557,(float)-.0040683,(float)
	    9.3909e-6,(float)-.0047053,(float)1.0347e-5,(float)11060.,(float)
	    11070.,(float).25071,(float).1119,(float)-.0048932,(float)
	    9.8071e-6,(float)-.0052555,(float)9.7232e-6,(float)11070.,(float)
	    11080.,(float).50363,(float).18561,(float)-.0040852,(float)
	    4.7194e-6,(float)-.0061085,(float)9.9471e-6,(float)11080.,(float)
	    11090.,(float).23621,(float).35575,(float)-6.9998e-4,(float)
	    -3.8983e-7,(float)-7.4838e-4,(float)-6.3021e-6,(float)11090.,(
	    float)11100.,(float).41715,(float).20786,(float)-.0026671,(float)
	    -8.5957e-7,(float)-.0044855,(float)3.6346e-6,(float)11100.,(float)
	    11110.,(float).35497,(float).15568,(float)-2.0655e-4,(float)
	    -1.0414e-5,(float)-.0019241,(float)-7.491e-6,(float)11110.,(float)
	    11120.,(float).36391,(float).22115,(float)8.0888e-5,(float)
	    -8.0079e-6,(float)-7.2043e-4,(float)-8.8371e-6,(float)11120.,(
	    float)11130.,(float).239,(float).19893,(float).0017314,(float)
	    -1.745e-5,(float)-7.2547e-4,(float)-1.0781e-5,(float)11130.,(
	    float)11140.,(float).18535,(float).23329,(float).0044356,(float)
	    -2.4159e-5,(float).002562,(float)-2.1329e-5,(float)11140.,(float)
	    11150.,(float).18058,(float).20287,(float).0053731,(float)
	    -2.948e-5,(float).0026609,(float)-2.2876e-5,(float)11150.,(float)
	    11160.,(float).086563,(float).18472,(float).0071342,(float)
	    -3.4497e-5,(float).0062094,(float)-3.7103e-5,(float)11160.,(float)
	    11170.,(float).040545,(float).27173,(float).012595,(float)
	    -5.3501e-5,(float).0098569,(float)-5.1164e-5,(float)11170.,(float)
	    11180.,(float).015934,(float).21414,(float).021094,(float)
	    -9.1779e-5,(float).018771,(float)-8.8619e-5,(float)11180.,(float)
	    11190.,(float).032149,(float).20819,(float).0053736,(float)
	    4.7418e-6,(float).0028308,(float)-3.9739e-6,(float)11190.,(float)
	    11200.,(float).0033026,(float).1125,(float)-.0049337,(float)
	    8.3861e-6,(float)-.0074414,(float)1.2308e-5,(float)11200.,(float)
	    11210.,(float).0069621,(float).11936,(float).002482,(float)
	    1.4188e-5,(float).0041457,(float)-7.6324e-6,(float)11210.,(float)
	    11220.,(float).0018624,(float).073437,(float).0037334,(float)
	    6.4996e-5,(float).0021277,(float)2.507e-5,(float)11220.,(float)
	    11230.,(float).0056133,(float).14671,(float)5.7744e-4,(float)
	    -1.0633e-5,(float)-.0020996,(float)-6.969e-6,(float)11230.,(float)
	    11240.,(float)5.9301e-4,(float).13188,(float).0043109,(float)
	    -2.7217e-5,(float).0014102,(float)-2.1872e-5,(float)11240.,(float)
	    11250.,(float).0062071,(float).08021,(float).0031276,(float)
	    -2.2059e-5,(float)6.1977e-4,(float)-1.6464e-5,(float)11250.,(
	    float)11260.,(float)7.0445e-4,(float).090176,(float).006354,(
	    float)-1.7728e-5,(float).0035488,(float)-2.1566e-5,(float)11260.,(
	    float)11270.,(float).0024343,(float).082442,(float)-1.2318e-4,(
	    float)-1.1355e-5,(float)-.0023203,(float)-8.0507e-6,(float)11270.,
	    (float)11280.,(float).0053124,(float).10493,(float).002512,(float)
	    -2.1412e-5,(float)-2.5472e-4,(float)-1.6312e-5,(float)11280.,(
	    float)11290.,(float).0010156,(float).075674,(float).0067257,(
	    float)-3.904e-5,(float).0037539,(float)-3.2659e-5,(float)11290.,(
	    float)11300.,(float)4.5845e-4,(float).031771,(float).0083901,(
	    float)-4.5954e-5,(float).005633,(float)-4.0402e-5,(float)11300.,(
	    float)11310.,(float).0013128,(float).1362,(float).0032287,(float)
	    -2.4445e-5,(float)6.7354e-4,(float)-2.006e-5,(float)11310.,(float)
	    11320.,(float)9.1527e-4,(float).043387,(float).013028,(float)
	    -6.4031e-5,(float).010684,(float)-6.015e-5,(float)11320.,(float)
	    11330.,(float).0016381,(float).11364,(float).0082323,(float)
	    -4.1843e-5,(float).0046964,(float)-3.5054e-5,(float)11330.,(float)
	    11340.,(float)8.2138e-4,(float).051673,(float).0074756,(float)
	    -4.1981e-5,(float).0051092,(float)-3.7995e-5,(float)11340.,(float)
	    11350.,(float)8.2766e-4,(float).072646,(float).0053277,(float)
	    -3.266e-5,(float).0034473,(float)-3.0783e-5,(float)11350.,(float)
	    11360.,(float)2.9086e-4,(float).031771,(float).0094981,(float)
	    -5.0386e-5,(float).0067409,(float)-4.4834e-5,(float)11360.,(float)
	    11370.,(float)2.1807e-4,(float).058942,(float).015709,(float)
	    -5.9258e-5,(float).013592,(float)-6.3956e-5,(float)11370.,(float)
	    11380.,(float)6.2704e-4,(float).12257,(float).010699,(float)
	    -4.9284e-5,(float).0095849,(float)-5.3234e-5,(float)11380.,(float)
	    11390.,(float)1.0702e-4,(float).028818,(float).0079994,(float)
	    -4.4391e-5,(float).0052422,(float)-3.8839e-5,(float)11390.,(float)
	    11400.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11400.,(float)11410.,(float)2.2042e-4,(float).065689,(
	    float).016094,(float)-7.5895e-5,(float).013589,(float)-7.1741e-5,(
	    float)11410.,(float)11420.,(float)8.1809e-5,(float).030563,(float)
	    .019812,(float)-9.1641e-5,(float).017055,(float)-8.6089e-5,(float)
	    11420.,(float)11430.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11430.,(float)11440.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11440.,(float)
	    11450.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11450.,(float)11460.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11460.,(float)11470.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11470.,(float)11480.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)11480.,(float)11490.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11490.,(float)
	    11500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)11500.,(float)11510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)11510.,(float)11520.,(float)
	    6.8748e-6,(float).028818,(float).014135,(float)-6.8935e-5,(float)
	    .011378,(float)-6.3383e-5,(float)11520.,(float)11530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11530.,(
	    float)11540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)11540.,(float)11550.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)11550.,(float)11560.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    11560.,(float)11570.,(float)1.1592e-5,(float).037767,(float)
	    .0031085,(float)-2.4828e-5,(float)3.5131e-4,(float)-1.9276e-5,(
	    float)11570.,(float)11580.,(float)1.2377e-4,(float).091349,(float)
	    .012838,(float)-3.6325e-5,(float).0084626,(float)-3.8447e-5,(
	    float)11580.,(float)11590.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)11590.,(float)11600.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)11600.,(
	    float)11610.,(float)2.7776e-4,(float).099768,(float).016197,(
	    float)-7.1636e-5,(float).012877,(float)-6.2598e-5,(float)11610.,(
	    float)11620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)11620.,(float)11630.,(float)6.0387e-4,(float)
	    .079303,(float).011427,(float)-5.6399e-5,(float).0076314,(float)
	    -4.5247e-5,(float)11630.,(float)11640.,(float)8.3575e-5,(float)
	    .082334,(float).015996,(float)-7.3351e-5,(float).013978,(float)
	    -7.131e-5,(float)11640.,(float)11650.,(float)2.7734e-4,(float)
	    .028818,(float).012698,(float)-6.3185e-5,(float).0099405,(float)
	    -5.7633e-5,(float)11650.,(float)11660.,(float).0013776,(float)
	    .1275,(float).0089606,(float)-4.0931e-5,(float).006208,(float)
	    -3.6077e-5,(float)11660.,(float)11670.,(float)4.5458e-4,(float)
	    .05909,(float).0073062,(float)-4.1015e-5,(float).0053622,(float)
	    -3.8549e-5,(float)11670.,(float)11680.,(float).0019815,(float)
	    .15477,(float).006225,(float)-3.2087e-5,(float).0049876,(float)
	    -3.2313e-5,(float)11680.,(float)11690.,(float)9.9572e-4,(float)
	    .08831,(float).0056347,(float)-2.7093e-5,(float).0037058,(float)
	    -2.8108e-5,(float)11690.,(float)11700.,(float).0025341,(float)
	    .10399,(float).0032192,(float)-2.1942e-5,(float).0010784,(float)
	    -1.8674e-5,(float)11700.,(float)11710.,(float).001691,(float)
	    .18929,(float).0042421,(float)-2.4597e-5,(float).0031654,(float)
	    -2.3807e-5,(float)11710.,(float)11720.,(float).001706,(float)
	    .12598,(float)6.5013e-4,(float)-1.3447e-5,(float)-.0021977,(float)
	    -8.4803e-6,(float)11720.,(float)11730.,(float).0032703,(float)
	    .1478,(float).0014549,(float)-1.0754e-5,(float)3.5403e-4,(float)
	    -1.3234e-5,(float)11730.,(float)11740.,(float).0055601,(float)
	    .15726,(float)-.0020459,(float)-2.8933e-6,(float)-.0040613,(float)
	    2.3816e-6,(float)11740.,(float)11750.,(float).0023988,(float)
	    .10521,(float)-8.9604e-5,(float)-8.9256e-6,(float)-.0019341,(
	    float)-4.1252e-6,(float)11750.,(float)11760.,(float).0021014,(
	    float).20346,(float).001557,(float)-3.876e-7,(float)4.2674e-4,(
	    float)-7.4882e-6,(float)11760.,(float)11770.,(float).0044559,(
	    float).17649,(float)-.0026665,(float)2.6782e-6,(float)-.0043678,(
	    float)1.1412e-5,(float)11770.,(float)11780.,(float).0024355,(
	    float).1438,(float)-.0030347,(float)1.3498e-5,(float)-.0029099,(
	    float)8.3348e-6,(float)11780.,(float)11790.,(float).0012088,(
	    float).14652,(float).0043364,(float)-2.3499e-5,(float).004136,(
	    float)-2.5136e-5,(float)11790.,(float)11800.,(float)3.5615e-4,(
	    float).076935,(float)-4.1654e-4,(float)1.6868e-5,(float)
	    -3.6975e-4,(float)6.6392e-6,(float)11800.,(float)11810.,(float)
	    .0033205,(float).14642,(float)-.0022883,(float)1.5563e-5,(float)
	    -.0011424,(float)6.601e-6,(float)11810.,(float)11820.,(float)
	    9.6482e-4,(float).11674,(float)-.0020293,(float)3.3475e-5,(float)
	    .0018153,(float)1.8034e-5,(float)11820.,(float)11830.,(float)
	    .0025468,(float).23891,(float)-3.7386e-4,(float)2.5112e-5,(float)
	    -7.037e-4,(float)1.2657e-5,(float)11830.,(float)11840.,(float)
	    .003978,(float).19525,(float).0017761,(float)1.7009e-5,(float)
	    .0028654,(float)9.7695e-7,(float)11840.,(float)11850.,(float)
	    .009308,(float).22287,(float)-.0028141,(float)6.1915e-6,(float)
	    -.0035846,(float)9.0001e-6,(float)11850.,(float)11860.,(float)
	    .0011913,(float).14817,(float).0011043,(float)4.7279e-5,(float)
	    .0021704,(float)1.6632e-5,(float)11860.,(float)11870.,(float)
	    .010095,(float).3319,(float)-3.025e-4,(float)2.7919e-5,(float)
	    4.6706e-4,(float)1.3481e-5,(float)11870.,(float)11880.,(float)
	    .0094837,(float).29542,(float).0041573,(float)-4.5489e-6,(float)
	    .0046629,(float)-1.3608e-5,(float)11880.,(float)11890.,(float)
	    .0057807,(float).16128,(float).0022948,(float)3.093e-5,(float)
	    .0032856,(float)7.3247e-6,(float)11890.,(float)11900.,(float)
	    .0090317,(float).33992,(float).0064901,(float)1.3104e-5,(float)
	    .0058385,(float)-1.237e-5,(float)11900.,(float)11910.,(float)
	    .0034946,(float).17472,(float)9.0274e-4,(float)9.569e-6,(float)
	    7.5517e-4,(float)7.6716e-7,(float)11910.,(float)11920.,(float)
	    .010712,(float).34337,(float).0057845,(float)3.273e-6,(float)
	    .0051567,(float)-1.5291e-5,(float)11920.,(float)11930.,(float)
	    .018599,(float).48317,(float).010589,(float)-3.4336e-5,(float)
	    .0073964,(float)-3.6504e-5,(float)11930.,(float)11940.,(float)
	    .011137,(float).31418,(float).0088066,(float)-1.9115e-5,(float)
	    .0066643,(float)-2.5755e-5,(float)11940.,(float)11950.,(float)
	    .014277,(float).15769,(float).010665,(float)-4.2217e-5,(float)
	    .0066305,(float)-3.2258e-5,(float)11950.,(float)11960.,(float)
	    .027875,(float).26865,(float).010692,(float)-4.8825e-5,(float)
	    .007738,(float)-4.3664e-5,(float)11960.,(float)11970.,(float)
	    .012682,(float).20777,(float).010603,(float)-4.8734e-5,(float)
	    .0081358,(float)-4.1884e-5,(float)11970.,(float)11980.,(float)
	    .036127,(float).20496,(float).0077739,(float)-4.0475e-5,(float)
	    .0055098,(float)-3.2025e-5,(float)11980.,(float)11990.,(float)
	    .026197,(float).26047,(float).0093353,(float)-4.7349e-5,(float)
	    .0069758,(float)-4.2653e-5,(float)11990.,(float)1.2e4,(float)
	    .031642,(float).28308,(float).0072441,(float)-3.3775e-5,(float)
	    .0041744,(float)-2.7701e-5,(float)1.2e4,(float)12010.,(float)
	    .13538,(float).163,(float).0042895,(float)-2.9341e-5,(float)
	    .001443,(float)-2.3228e-5,(float)12010.,(float)12020.,(float)
	    .04616,(float).26257,(float).0059122,(float)-3.4545e-5,(float)
	    .0033253,(float)-2.959e-5,(float)12020.,(float)12030.,(float)
	    .098778,(float).16239,(float).0019096,(float)-1.947e-5,(float)
	    1.9375e-4,(float)-1.6507e-5,(float)12030.,(float)12040.,(float)
	    .04218,(float).15687,(float).0011607,(float)-1.5189e-5,(float)
	    3.26e-4,(float)-1.0808e-5,(float)12040.,(float)12050.,(float)
	    .081749,(float).1447,(float).0019087,(float)-1.8894e-5,(float)
	    4.6347e-5,(float)-1.5861e-5,(float)12050.,(float)12060.,(float)
	    .14244,(float).15544,(float)-4.8204e-4,(float)-1.0055e-5,(float)
	    -.0030791,(float)-4.5136e-6,(float)12060.,(float)12070.,(float)
	    .15967,(float).14014,(float)-.0015241,(float)-6.1008e-6,(float)
	    -.0038422,(float)-1.2202e-6,(float)12070.,(float)12080.,(float)
	    .14263,(float).16924,(float)-.0025815,(float)-1.3799e-6,(float)
	    -.0046065,(float)2.7252e-6,(float)12080.,(float)12090.,(float)
	    .028755,(float).09297,(float)-.0034559,(float)3.6475e-6,(float)
	    -.0038455,(float)8.6258e-6,(float)12090.,(float)12100.,(float)
	    .13867,(float).13244,(float)-.0035649,(float)5.23e-6,(float)
	    -.0053551,(float)8.6133e-6,(float)12100.,(float)12110.,(float)
	    .022667,(float).083805,(float)-.00172,(float)-3.4708e-6,(float)
	    -.0030806,(float)3.0367e-6,(float)12110.,(float)12120.,(float)
	    .092398,(float).11619,(float)-.0042643,(float)1.2112e-5,(float)
	    -.0033307,(float)1.1625e-5,(float)12120.,(float)12130.,(float)
	    .036116,(float).2779,(float).0029762,(float)1.3854e-6,(float)
	    .0042021,(float)-1.2139e-5,(float)12130.,(float)12140.,(float)
	    .44373,(float).46666,(float)3.1523e-4,(float)-3.0509e-6,(float)
	    -2.0293e-4,(float)-7.5095e-6,(float)12140.,(float)12150.,(float)
	    .27187,(float).27772,(float)-.0011411,(float)1.4254e-6,(float)
	    -.001681,(float)-8.1908e-7,(float)12150.,(float)12160.,(float)
	    .051449,(float).10126,(float)-.0037317,(float)6.5728e-6,(float)
	    -.0032342,(float)1.2751e-5,(float)12160.,(float)12170.,(float)
	    .038984,(float).17694,(float)-.0039312,(float)1.3072e-5,(float)
	    -.0033739,(float)1.401e-5,(float)12170.,(float)12180.,(float)
	    .0044227,(float).11724,(float).0062039,(float)-3.2339e-5,(float)
	    .0022912,(float)-2.0596e-5,(float)12180.,(float)12190.,(float)
	    .18992,(float).14819,(float)-.0050473,(float)8.811e-6,(float)
	    -.0068829,(float)1.2852e-5,(float)12190.,(float)12200.,(float)
	    .25583,(float).13027,(float)-.0044286,(float)5.7917e-6,(float)
	    -.0063447,(float)9.414e-6,(float)12200.,(float)12210.,(float)
	    .13625,(float).18565,(float)-.0034046,(float)2.2857e-6,(float)
	    -.0050965,(float)6.4148e-6,(float)12210.,(float)12220.,(float)
	    .29053,(float).22511,(float)-.0027471,(float)-8.8244e-7,(float)
	    -.0045852,(float)2.3044e-6,(float)12220.,(float)12230.,(float)
	    .19038,(float).14946,(float)-6.8588e-4,(float)-9.3784e-6,(float)
	    -.0027488,(float)-5.0351e-6,(float)12230.,(float)12240.,(float)
	    .33873,(float).28292,(float)-1.0104e-4,(float)-1.0239e-5,(float)
	    -.0013958,(float)-9.0346e-6,(float)12240.,(float)12250.,(float)
	    .1325,(float).17121,(float).0024653,(float)-1.9723e-5,(float)
	    1.8597e-4,(float)-1.4634e-5,(float)12250.,(float)12260.,(float)
	    .12157,(float).37106,(float).0037908,(float)-2.2351e-5,(float)
	    .0010325,(float)-1.6657e-5,(float)12260.,(float)12270.,(float)
	    .07651,(float).16258,(float).0034104,(float)-2.3774e-5,(float)
	    .0018529,(float)-2.1863e-5,(float)12270.,(float)12280.,(float)
	    .07529,(float).33562,(float).0076059,(float)-3.4182e-5,(float)
	    .0052641,(float)-3.0402e-5,(float)12280.,(float)12290.,(float)
	    .060409,(float).25407,(float).0068386,(float)-2.9386e-5,(float)
	    .0061709,(float)-2.8132e-5,(float)12290.,(float)12300.,(float)
	    .027161,(float).42647,(float).0097512,(float)-3.3363e-5,(float)
	    .0063871,(float)-3.1407e-5,(float)12300.,(float)12310.,(float)
	    .009111,(float).28999,(float).010916,(float)-3.5531e-5,(float)
	    .0085325,(float)-3.6715e-5,(float)12310.,(float)12320.,(float)
	    .019913,(float).33483,(float).0088198,(float)-1.1414e-5,(float)
	    .0072926,(float)-2.8077e-5,(float)12320.,(float)12330.,(float)
	    .0081899,(float).31312,(float).014481,(float)-5.2817e-5,(float)
	    .010281,(float)-4.4189e-5,(float)12330.,(float)12340.,(float)
	    .0047198,(float).32687,(float).0079614,(float)6.2153e-6,(float)
	    .0072159,(float)-1.9264e-5,(float)12340.,(float)12350.,(float)
	    .011347,(float).34063,(float).002382,(float)2.8934e-5,(float)
	    .0025122,(float)2.6528e-6,(float)12350.,(float)12360.,(float)
	    .0035162,(float).2474,(float).0064215,(float)-4.522e-6,(float)
	    .0068198,(float)-2.2326e-5,(float)12360.,(float)12370.,(float)
	    .0045582,(float).33334,(float).0040843,(float)-6.8827e-6,(float)
	    .0026935,(float)-1.6936e-5,(float)12370.,(float)12380.,(float)
	    .0025789,(float).33203,(float).004947,(float)-1.6065e-5,(float)
	    .003438,(float)-2.2968e-5,(float)12380.,(float)12390.,(float)
	    .0045336,(float).2043,(float).0047629,(float)-2.7839e-5,(float)
	    .0025633,(float)-2.4703e-5,(float)12390.,(float)12400.,(float)
	    .0030244,(float).20516,(float).0081732,(float)-4.1804e-5,(float)
	    .0046359,(float)-2.9989e-5,(float)12400.,(float)12410.,(float)
	    .0041242,(float).23106,(float).0057501,(float)-3.1437e-5,(float)
	    .0034257,(float)-2.8835e-5,(float)12410.,(float)12420.,(float)
	    .010287,(float).33673,(float).0041222,(float)-2.574e-5,(float)
	    .0014121,(float)-2.0905e-5,(float)12420.,(float)12430.,(float)
	    .0052103,(float).19644,(float).0037193,(float)-2.6341e-5,(float)
	    9.394e-4,(float)-2.0581e-5,(float)12430.,(float)12440.,(float)
	    2.2877e-4,(float).12174,(float).0092724,(float)-4.5126e-5,(float)
	    .0056853,(float)-3.7747e-5,(float)12440.,(float)12450.,(float)
	    .015209,(float).45993,(float).0011296,(float)-1.4042e-5,(float)
	    -.0011241,(float)-1.1493e-5,(float)12450.,(float)12460.,(float)
	    .005545,(float).1911,(float).0015443,(float)-1.3043e-5,(float)
	    -3.2861e-4,(float)-1.1764e-5,(float)12460.,(float)12470.,(float)
	    .011275,(float).1922,(float)7.547e-5,(float)-1.0276e-5,(float)
	    -.0020095,(float)-7.6051e-6,(float)12470.,(float)12480.,(float)
	    .014331,(float).19322,(float)-.0010929,(float)-7.1557e-6,(float)
	    -.0030976,(float)-4.5871e-6,(float)12480.,(float)12490.,(float)
	    .013106,(float).20676,(float)-.0021472,(float)-1.689e-6,(float)
	    -.00333,(float)-4.7675e-7,(float)12490.,(float)12500.,(float)
	    .0031859,(float).26518,(float).0018883,(float)-3.7481e-6,(float)
	    4.5064e-5,(float)-1.1501e-5,(float)12500.,(float)12510.,(float)
	    .0066833,(float).21228,(float)-.0021409,(float)4.8505e-6,(float)
	    -.0028895,(float)4.0738e-6,(float)12510.,(float)12520.,(float)
	    .011966,(float).28263,(float)-.0013922,(float)6.3802e-6,(float)
	    -7.3809e-4,(float)-1.1743e-6,(float)12520.,(float)12530.,(float)
	    .0040664,(float).24183,(float).0035636,(float)-2.8875e-6,(float)
	    .0022372,(float)-1.6411e-5,(float)12530.,(float)12540.,(float)
	    .014346,(float).3851,(float)2.0035e-4,(float)3.7739e-6,(float)
	    -6.1012e-5,(float)-7.988e-6,(float)12540.,(float)12550.,(float)
	    .031841,(float).37389,(float)-.0014467,(float)-2.5515e-6,(float)
	    -.0036219,(float)-9.456e-7,(float)12550.,(float)12560.,(float)
	    .014117,(float).15375,(float)-.0027582,(float)1.5847e-6,(float)
	    -.0042341,(float)5.2506e-6,(float)12560.,(float)12570.,(float)
	    .0069458,(float).17261,(float)-.0018858,(float)-1.3771e-6,(float)
	    -.0038705,(float)1.8317e-6,(float)12570.,(float)12580.,(float)
	    .0034163,(float).16476,(float)-.0038874,(float)8.9594e-6,(float)
	    -.004751,(float)6.5193e-6,(float)12580.,(float)12590.,(float)
	    .0010085,(float).13402,(float)8.9094e-4,(float)-6.018e-6,(float)
	    -6.4581e-6,(float)-9.4409e-6,(float)12590.,(float)12600.,(float)
	    .017942,(float).20327,(float)-.0039862,(float)7.5053e-6,(float)
	    -.0052785,(float)7.3576e-6,(float)12600.,(float)12610.,(float)
	    .0080749,(float).061138,(float)-.0049341,(float)7.5781e-6,(float)
	    -.0072374,(float)1.1499e-5,(float)12610.,(float)12620.,(float)
	    .022511,(float).21226,(float)-.0034799,(float)3.196e-6,(float)
	    -.0050773,(float)6.2716e-6,(float)12620.,(float)12630.,(float)
	    .0091364,(float).16111,(float)-.0029375,(float)2.6306e-7,(float)
	    -.0049473,(float)4.3965e-6,(float)12630.,(float)12640.,(float)
	    .014688,(float).1553,(float)-.0020029,(float)-2.3192e-6,(float)
	    -.0029945,(float)-2.1575e-6,(float)12640.,(float)12650.,(float)
	    .016309,(float).20204,(float)1.6988e-6,(float)-1.109e-5,(float)
	    -.0014702,(float)-9.9629e-6,(float)12650.,(float)12660.,(float)
	    .028046,(float).24162,(float)2.1643e-4,(float)-1.1209e-5,(float)
	    -.0017491,(float)-8.904e-6,(float)12660.,(float)12670.,(float)
	    .011289,(float).29319,(float).0034128,(float)-2.3021e-5,(float)
	    .0010846,(float)-1.8419e-5,(float)12670.,(float)12680.,(float)
	    .0049531,(float).14498,(float).0053073,(float)-2.8791e-5,(float)
	    .0032927,(float)-2.5157e-5,(float)12680.,(float)12690.,(float)
	    .0072485,(float).17855,(float).0056992,(float)-3.2465e-5,(float)
	    .0041153,(float)-3.2129e-5,(float)12690.,(float)12700.,(float)
	    .0061604,(float).2478,(float).0073217,(float)-3.1129e-5,(float)
	    .0058029,(float)-3.2007e-5,(float)12700.,(float)12710.,(float)
	    .0013393,(float).12405,(float).012672,(float)-4.8102e-5,(float)
	    .0088276,(float)-3.9342e-5,(float)12710.,(float)12720.,(float)
	    .0010615,(float).14687,(float).012203,(float)-5.0558e-5,(float)
	    .01058,(float)-5.4423e-5,(float)12720.,(float)12730.,(float)
	    4.7236e-4,(float).080989,(float).0036212,(float)3.4855e-5,(float)
	    .0023529,(float)7.9526e-6,(float)12730.,(float)12740. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/

/*     WATER VAPOR (10180 - 12720 CM-1) */

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
} /* wava4_ */

#ifdef __cplusplus
	}
#endif
