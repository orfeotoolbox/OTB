/* OCEA.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/* Common Block Declarations */

Extern struct {
    integer nquad;
} num_quad__;

#define num_quad__1 num_quad__

Extern struct {
    real ph[20000]	/* was [20][1000] */, qh[20000]	/* was [20][1000] */, 
	    uh[20000]	/* was [20][1000] */;
} sixs_aerbas__;

#define sixs_aerbas__1 sixs_aerbas__

Extern struct {
    real ex[80]	/* was [4][20] */, sc[80]	/* was [4][20] */, asy[80]	
	    /* was [4][20] */, vi[4];
} sixs_coef__;

#define sixs_coef__1 sixs_coef__

/*<        subroutine   ocea >*/
/* Subroutine */ int ocea_()
{
    /* Initialized data */

    static real asy_m__[20] = { (float).79,(float).787,(float).785,(float)
	    .784,(float).782,(float).784,(float).781,(float).784,(float).781,(
	    float).78,(float).78,(float).779,(float).779,(float).778,(float)
	    .779,(float).785,(float).787,(float).792,(float).816,(float).749 }
	    ;
    static real ex_m__[20] = { (float)3.492134,(float)3.555835,(float)3.57744,
	    (float)3.601985,(float)3.625801,(float)3.628089,(float)3.658286,(
	    float)3.658747,(float)3.696527,(float)3.730609,(float)3.736655,(
	    float)3.76356,(float)3.776731,(float)3.813161,(float)3.830686,(
	    float)3.729777,(float)3.675602,(float)3.516177,(float)3.257454,(
	    float)2.827842 };
    static real sc_m__[20] = { (float)3.4921,(float)3.555833,(float)3.577438,(
	    float)3.601983,(float)3.625799,(float)3.628088,(float)3.658284,(
	    float)3.658746,(float)3.696526,(float)3.730607,(float)3.73665,(
	    float)3.763552,(float)3.774958,(float)3.809129,(float)3.805399,(
	    float)3.662509,(float)3.619132,(float)3.413872,(float)2.935607,(
	    float)2.749985 };
    static real phr[1660]	/* was [20][83] */ = { (float).8426,(float)
	    .7371,(float).8416,(float).7122,(float).8664,(float).6608,(float)
	    .6706,(float).5889,(float).6593,(float).6636,(float).5688,(float)
	    .5587,(float).5378,(float).5168,(float).3996,(float).3001,(float)
	    .2823,(float).2252,(float).131,(float).2124,(float).6434,(float)
	    .6009,(float).6946,(float).6039,(float).6659,(float).5459,(float)
	    .5495,(float).4993,(float).5301,(float).5313,(float).4824,(float)
	    .4718,(float).4466,(float).4308,(float).3448,(float).2702,(float)
	    .2552,(float).2063,(float).1235,(float).2038,(float).5956,(float)
	    .5322,(float).5479,(float).5204,(float).5552,(float).5048,(float)
	    .4882,(float).4727,(float).4588,(float).4331,(float).4236,(float)
	    .4102,(float).3985,(float).3776,(float).2744,(float).2249,(float)
	    .2123,(float).1757,(float).1075,(float).1826,(float).5002,(float)
	    .4548,(float).4787,(float).4514,(float).5141,(float).4428,(float)
	    .4283,(float).4128,(float).4268,(float).3833,(float).378,(float)
	    .3746,(float).3581,(float).3454,(float).2618,(float).2075,(float)
	    .1951,(float).1674,(float).1051,(float).1735,(float).4105,(float)
	    .4231,(float).4314,(float).4316,(float).4301,(float).4113,(float)
	    .4208,(float).3737,(float).3862,(float).3809,(float).3504,(float)
	    .3706,(float).3376,(float).3293,(float).2804,(float).2173,(float)
	    .2063,(float).1797,(float).1162,(float).1792,(float).3836,(float)
	    .4052,(float).4126,(float).4104,(float).4072,(float).3873,(float)
	    .4096,(float).36,(float).3848,(float).3787,(float).3637,(float)
	    .3749,(float).3474,(float).3456,(float).2976,(float).2344,(float)
	    .225,(float).1971,(float).1279,(float).1937,(float).3575,(float)
	    .368,(float).3866,(float).3784,(float).3892,(float).3655,(float)
	    .3788,(float).3534,(float).3614,(float).3648,(float).3511,(float)
	    .3569,(float).3401,(float).334,(float).2947,(float).2445,(float)
	    .233,(float).2027,(float).1325,(float).2072,(float).333,(float)
	    .3354,(float).3507,(float).3412,(float).3549,(float).3399,(float)
	    .3436,(float).3382,(float).3344,(float).3414,(float).326,(float)
	    .33,(float).3225,(float).3164,(float).2798,(float).2409,(float)
	    .2294,(float).1997,(float).1308,(float).2138,(float).3124,(float)
	    .3138,(float).3253,(float).3191,(float).3279,(float).3196,(float)
	    .3238,(float).3167,(float).3203,(float).3151,(float).3112,(float)
	    .3079,(float).3044,(float).3057,(float).2638,(float).2303,(float)
	    .2198,(float).1918,(float).1263,(float).215,(float).3045,(float)
	    .3088,(float).3055,(float).3072,(float).3039,(float).3124,(float)
	    .3103,(float).2988,(float).2993,(float).3003,(float).2946,(float)
	    .3001,(float).2916,(float).2831,(float).2525,(float).2189,(float)
	    .2091,(float).1829,(float).1212,(float).2119,(float).2944,(float)
	    .3007,(float).3074,(float).3039,(float).3054,(float).2971,(float)
	    .3053,(float).2874,(float).2957,(float).2909,(float).2833,(float)
	    .2901,(float).2768,(float).2749,(float).2468,(float).2091,(float)
	    .1996,(float).1761,(float).1166,(float).2053,(float).313,(float)
	    .2947,(float).3016,(float).2959,(float).2999,(float).2926,(float)
	    .2946,(float).2958,(float).2936,(float).2844,(float).2884,(float)
	    .2808,(float).2782,(float).2694,(float).2364,(float).2043,(float)
	    .1948,(float).17,(float).1127,(float).1954,(float).3192,(float)
	    .3047,(float).3081,(float).3044,(float).3095,(float).2948,(float)
	    .2988,(float).2978,(float).287,(float).2831,(float).2804,(float)
	    .2791,(float).2727,(float).2609,(float).2288,(float).1989,(float)
	    .1897,(float).1643,(float).1094,(float).1843,(float).3212,(float)
	    .3135,(float).3114,(float).3166,(float).3093,(float).2983,(float)
	    .3058,(float).2869,(float).2902,(float).2779,(float).2755,(float)
	    .2726,(float).2645,(float).2594,(float).2199,(float).1914,(float)
	    .1833,(float).1593,(float).1064,(float).1734,(float).3219,(float)
	    .3072,(float).2991,(float).2903,(float).2942,(float).2961,(float)
	    .2862,(float).2834,(float).2796,(float).2661,(float).2714,(float)
	    .2647,(float).2585,(float).2462,(float).2093,(float).183,(float)
	    .1761,(float).1528,(float).1035,(float).1629,(float).272,(float)
	    .2729,(float).2695,(float).2685,(float).2658,(float).2627,(float)
	    .2616,(float).2525,(float).2515,(float).245,(float).243,(float)
	    .2433,(float).2334,(float).2234,(float).1969,(float).1709,(float)
	    .1658,(float).1457,(float).1002,(float).1531,(float).1896,(float)
	    .2174,(float).2187,(float).2197,(float).2103,(float).2145,(float)
	    .2177,(float).203,(float).2033,(float).2168,(float).2001,(float)
	    .2095,(float).1961,(float).1933,(float).1803,(float).1572,(float)
	    .1536,(float).1381,(float).0966,(float).1436,(float).1406,(float)
	    .1588,(float).1602,(float).1659,(float).1651,(float).1668,(float)
	    .1697,(float).1663,(float).1712,(float).1755,(float).1722,(float)
	    .1725,(float).1692,(float).17,(float).1609,(float).145,(float)
	    .1419,(float).1296,(float).09285,(float).1348,(float).1112,(float)
	    .1248,(float).1267,(float).1316,(float).1318,(float).1355,(float)
	    .1372,(float).1363,(float).1405,(float).145,(float).1431,(float)
	    .1478,(float).1461,(float).1469,(float).1437,(float).1322,(float)
	    .1308,(float).1203,(float).08897,(float).1272,(float).08642,(
	    float).1018,(float).1009,(float).1077,(float).1062,(float).1118,(
	    float).1158,(float).1144,(float).1173,(float).121,(float).123,(
	    float).1257,(float).1251,(float).1276,(float).1274,(float).1217,(
	    float).1212,(float).1116,(float).0852,(float).1206,(float).07607,(
	    float).08348,(float).08076,(float).08814,(float).09173,(float)
	    .09367,(float).09579,(float).1005,(float).1038,(float).1048,(
	    float).1096,(float).108,(float).112,(float).1152,(float).1142,(
	    float).1126,(float).1122,(float).1047,(float).08176,(float).1151,(
	    float).06522,(float).0735,(float).0751,(float).08054,(float)
	    .08102,(float).08208,(float).08666,(float).08513,(float).09074,(
	    float).0946,(float).09526,(float).09734,(float).09955,(float)
	    .1024,(float).1066,(float).1031,(float).1029,(float).09838,(float)
	    .07844,(float).1107,(float).05889,(float).06763,(float).06945,(
	    float).07234,(float).0705,(float).07585,(float).0776,(float).078,(
	    float).08075,(float).08601,(float).08662,(float).08935,(float)
	    .09025,(float).09267,(float).09947,(float).0965,(float).09669,(
	    float).09271,(float).0755,(float).1071,(float).05396,(float)
	    .06191,(float).06487,(float).06789,(float).06683,(float).07008,(
	    float).07292,(float).07446,(float).07543,(float).07905,(float)
	    .07964,(float).08281,(float).08358,(float).08647,(float).09336,(
	    float).09209,(float).09196,(float).08816,(float).073,(float).1042,
	    (float).05403,(float).0585,(float).06009,(float).06153,(float)
	    .06425,(float).06669,(float).06827,(float).07029,(float).07246,(
	    float).07576,(float).07711,(float).07891,(float).08109,(float)
	    .08277,(float).08845,(float).08818,(float).08785,(float).08463,(
	    float).07107,(float).1022,(float).05099,(float).0571,(float).0602,
	    (float).06069,(float).06434,(float).06452,(float).06565,(float)
	    .06724,(float).071,(float).07431,(float).07407,(float).07586,(
	    float).0767,(float).08042,(float).08567,(float).08503,(float)
	    .08478,(float).0823,(float).06969,(float).1009,(float).05312,(
	    float).05775,(float).05861,(float).0623,(float).06605,(float)
	    .06434,(float).0673,(float).06657,(float).07184,(float).07193,(
	    float).07372,(float).07496,(float).07662,(float).07934,(float)
	    .08339,(float).08264,(float).08275,(float).08067,(float).06873,(
	    float).1002,(float).05451,(float).06092,(float).05718,(float)
	    .06139,(float).06611,(float).06681,(float).0675,(float).06844,(
	    float).07128,(float).07079,(float).07438,(float).0763,(float)
	    .07581,(float).07891,(float).0819,(float).0813,(float).08117,(
	    float).07946,(float).06809,(float).1001,(float).05772,(float)
	    .06167,(float).06273,(float).06629,(float).06616,(float).06584,(
	    float).06969,(float).06837,(float).07116,(float).07205,(float)
	    .07378,(float).07527,(float).07548,(float).07803,(float).08271,(
	    float).08088,(float).08028,(float).07876,(float).06787,(float)
	    .1007,(float).06023,(float).06361,(float).06565,(float).0695,(
	    float).06686,(float).06769,(float).07152,(float).07044,(float)
	    .07401,(float).07254,(float).07485,(float).07573,(float).07713,(
	    float).07916,(float).08283,(float).08114,(float).08069,(float)
	    .07854,(float).06822,(float).1019,(float).06418,(float).06903,(
	    float).06835,(float).07217,(float).06866,(float).07257,(float)
	    .07437,(float).07275,(float).0732,(float).07474,(float).07721,(
	    float).07931,(float).07839,(float).07961,(float).08322,(float)
	    .08147,(float).08124,(float).0792,(float).06902,(float).1037,(
	    float).06527,(float).07096,(float).07368,(float).07294,(float)
	    .07485,(float).07321,(float).07546,(float).07142,(float).07674,(
	    float).07882,(float).07781,(float).0788,(float).07919,(float)
	    .08182,(float).0855,(float).0817,(float).08157,(float).08085,(
	    float).07019,(float).1059,(float).06742,(float).07184,(float)
	    .07535,(float).07528,(float).07979,(float).07348,(float).07707,(
	    float).07433,(float).0799,(float).08157,(float).07875,(float)
	    .08026,(float).08084,(float).08377,(float).08712,(float).08302,(
	    float).08289,(float).08232,(float).07171,(float).1087,(float)
	    .07013,(float).07387,(float).07684,(float).07636,(float).07674,(
	    float).07569,(float).07993,(float).07682,(float).07918,(float)
	    .08212,(float).08057,(float).08262,(float).08268,(float).08415,(
	    float).08869,(float).08619,(float).0854,(float).08368,(float)
	    .07369,(float).1123,(float).07259,(float).07345,(float).07894,(
	    float).0796,(float).08148,(float).07601,(float).08173,(float)
	    .07981,(float).08244,(float).08465,(float).08385,(float).08354,(
	    float).08594,(float).0883,(float).09052,(float).08901,(float)
	    .08837,(float).0865,(float).07631,(float).1166,(float).07394,(
	    float).07282,(float).08157,(float).078,(float).0875,(float).07629,
	    (float).0801,(float).08221,(float).08828,(float).0886,(float)
	    .08722,(float).08508,(float).08982,(float).09301,(float).09368,(
	    float).09171,(float).09169,(float).0902,(float).07946,(float)
	    .1215,(float).07478,(float).07554,(float).08408,(float).08043,(
	    float).08774,(float).0811,(float).08452,(float).08415,(float)
	    .09053,(float).09026,(float).09066,(float).08891,(float).09255,(
	    float).09631,(float).09776,(float).09569,(float).09499,(float)
	    .09374,(float).08301,(float).1273,(float).07715,(float).08142,(
	    float).08347,(float).08519,(float).0899,(float).08746,(float)
	    .0886,(float).08919,(float).0922,(float).09384,(float).09366,(
	    float).09556,(float).09571,(float).09909,(float).1013,(float)
	    .1006,(float).09951,(float).09791,(float).08719,(float).134,(
	    float).08478,(float).08376,(float).08672,(float).08747,(float)
	    .09508,(float).08929,(float).09152,(float).09547,(float).1005,(
	    float).09784,(float).1009,(float).09839,(float).1033,(float).1061,
	    (float).1063,(float).106,(float).1053,(float).1036,(float).09225,(
	    float).1416,(float).08979,(float).08866,(float).09469,(float)
	    .09079,(float).101,(float).09444,(float).09693,(float).1007,(
	    float).1051,(float).104,(float).1071,(float).1037,(float).1086,(
	    float).1129,(float).1132,(float).1123,(float).1113,(float).1096,(
	    float).09801,(float).1506,(float).09464,(float).09809,(float)
	    .09861,(float).1023,(float).1022,(float).1054,(float).1069,(float)
	    .106,(float).1076,(float).1103,(float).1108,(float).1147,(float)
	    .114,(float).1159,(float).1211,(float).1191,(float).1177,(float)
	    .1161,(float).1045,(float).161,(float).09427,(float).1049,(float)
	    .1042,(float).1104,(float).1041,(float).1112,(float).1138,(float)
	    .1093,(float).1097,(float).1144,(float).1122,(float).1204,(float)
	    .1156,(float).1183,(float).1254,(float).123,(float).1215,(float)
	    .12,(float).1081,(float).1666,(float).1028,(float).1051,(float)
	    .1064,(float).1104,(float).1135,(float).1103,(float).1153,(float)
	    .115,(float).1169,(float).1176,(float).1187,(float).1221,(float)
	    .1217,(float).1249,(float).1297,(float).1274,(float).1259,(float)
	    .1244,(float).1121,(float).1724,(float).1102,(float).1109,(float)
	    .1161,(float).1146,(float).1254,(float).1186,(float).1207,(float)
	    .1228,(float).1289,(float).1296,(float).131,(float).1284,(float)
	    .1332,(float).1377,(float).1379,(float).1373,(float).1359,(float)
	    .1344,(float).121,(float).185,(float).1198,(float).1254,(float)
	    .1262,(float).1316,(float).1323,(float).1324,(float).1364,(float)
	    .1337,(float).1374,(float).1382,(float).1398,(float).1421,(float)
	    .1441,(float).1464,(float).1491,(float).148,(float).1469,(float)
	    .1451,(float).1311,(float).1997,(float).128,(float).1378,(float)
	    .142,(float).1436,(float).1444,(float).1464,(float).1482,(float)
	    .1433,(float).1468,(float).1531,(float).1494,(float).1557,(float)
	    .1531,(float).1574,(float).1643,(float).1604,(float).1586,(float)
	    .1573,(float).1426,(float).2168,(float).1445,(float).15,(float)
	    .1573,(float).1569,(float).1635,(float).1555,(float).1633,(float)
	    .159,(float).1666,(float).1667,(float).1671,(float).1677,(float)
	    .1697,(float).1748,(float).1789,(float).1744,(float).1731,(float)
	    .1723,(float).156,(float).2361,(float).1604,(float).1666,(float)
	    .1684,(float).172,(float).183,(float).1731,(float).1778,(float)
	    .1757,(float).1841,(float).185,(float).1843,(float).1858,(float)
	    .1881,(float).1925,(float).1936,(float).1915,(float).1903,(float)
	    .1886,(float).1714,(float).2579,(float).1768,(float).1895,(float)
	    .1934,(float).1958,(float).1944,(float).195,(float).2021,(float)
	    .1947,(float).1985,(float).2033,(float).2007,(float).2075,(float)
	    .2042,(float).2083,(float).2155,(float).2109,(float).2089,(float)
	    .2069,(float).1888,(float).2828,(float).1994,(float).2086,(float)
	    .2146,(float).2123,(float).2175,(float).2167,(float).2207,(float)
	    .215,(float).2243,(float).227,(float).2241,(float).2279,(float)
	    .2257,(float).2319,(float).238,(float).2322,(float).2302,(float)
	    .2293,(float).2091,(float).3109,(float).2309,(float).229,(float)
	    .2365,(float).2367,(float).2535,(float).2335,(float).2427,(float)
	    .2445,(float).2565,(float).2523,(float).254,(float).2495,(float)
	    .2573,(float).2631,(float).2616,(float).258,(float).257,(float)
	    .2551,(float).2327,(float).3428,(float).2565,(float).2618,(float)
	    .2647,(float).2655,(float).2743,(float).2678,(float).2733,(float)
	    .2736,(float).2809,(float).2788,(float).2828,(float).2798,(float)
	    .2852,(float).29,(float).2901,(float).2882,(float).2864,(float)
	    .2835,(float).2598,(float).3798,(float).2911,(float).3,(float)
	    .2991,(float).303,(float).3041,(float).3054,(float).3111,(float)
	    .3074,(float).3079,(float).3141,(float).3126,(float).3195,(float)
	    .3145,(float).3188,(float).3259,(float).3207,(float).3184,(float)
	    .3163,(float).2909,(float).4228,(float).318,(float).333,(float)
	    .3443,(float).3428,(float).3488,(float).3386,(float).3492,(float)
	    .3371,(float).3524,(float).3554,(float).3482,(float).3524,(float)
	    .3505,(float).3596,(float).3668,(float).3591,(float).3566,(float)
	    .3555,(float).3273,(float).4716,(float).3703,(float).3761,(float)
	    .3853,(float).3839,(float).3902,(float).3825,(float).3912,(float)
	    .3863,(float).3968,(float).3984,(float).4003,(float).3936,(float)
	    .4015,(float).4058,(float).4091,(float).4045,(float).403,(float)
	    .4005,(float).3697,(float).5269,(float).4197,(float).4294,(float)
	    .4286,(float).4325,(float).4369,(float).4416,(float).4417,(float)
	    .4432,(float).4453,(float).442,(float).4489,(float).4496,(float)
	    .45,(float).4559,(float).4574,(float).4575,(float).4556,(float)
	    .4519,(float).4189,(float).5912,(float).4777,(float).4934,(float)
	    .4908,(float).4958,(float).4909,(float).4964,(float).5052,(float)
	    .4945,(float).4967,(float).503,(float).5024,(float).5095,(float)
	    .5068,(float).5092,(float).5208,(float).5169,(float).514,(float)
	    .5121,(float).4765,(float).6656,(float).5351,(float).5513,(float)
	    .556,(float).5588,(float).5575,(float).5581,(float).5681,(float)
	    .5606,(float).57,(float).5699,(float).5708,(float).5705,(float)
	    .5702,(float).5805,(float).5911,(float).5861,(float).5843,(float)
	    .5835,(float).5443,(float).7513,(float).6305,(float).6135,(float)
	    .6274,(float).6227,(float).6392,(float).6265,(float).6322,(float)
	    .6452,(float).6483,(float).6515,(float).6544,(float).6412,(float)
	    .6587,(float).6598,(float).6665,(float).6675,(float).6677,(float)
	    .6661,(float).6239,(float).8503,(float).6948,(float).7221,(float)
	    .7126,(float).7247,(float).7165,(float).7284,(float).7343,(float)
	    .7256,(float).7255,(float).7292,(float).7288,(float).7449,(float)
	    .7363,(float).7409,(float).7591,(float).7643,(float).7627,(float)
	    .7614,(float).7174,(float).966,(float).8032,(float).8242,(float)
	    .806,(float).8165,(float).8007,(float).8331,(float).8314,(float)
	    .8269,(float).8248,(float).8238,(float).8358,(float).8497,(float)
	    .8402,(float).8458,(float).8703,(float).8749,(float).8738,(float)
	    .8745,(float).8283,(float)1.101,(float).9123,(float).9168,(float)
	    .9229,(float).9301,(float).9262,(float).9285,(float).9413,(float)
	    .9382,(float).9349,(float).9585,(float).9476,(float).9532,(float)
	    .957,(float).9639,(float).9971,(float)1.002,(float)1.005,(float)
	    1.01,(float).9602,(float)1.256,(float)1.045,(float)1.052,(float)
	    1.045,(float)1.054,(float)1.051,(float)1.064,(float)1.068,(float)
	    1.074,(float)1.071,(float)1.08,(float)1.088,(float)1.091,(float)
	    1.104,(float)1.108,(float)1.139,(float)1.157,(float)1.162,(float)
	    1.17,(float)1.117,(float)1.437,(float)1.202,(float)1.181,(float)
	    1.18,(float)1.183,(float)1.199,(float)1.209,(float)1.201,(float)
	    1.232,(float)1.223,(float)1.234,(float)1.246,(float)1.244,(float)
	    1.26,(float)1.271,(float)1.31,(float)1.339,(float)1.346,(float)
	    1.355,(float)1.304,(float)1.649,(float)1.35,(float)1.371,(float)
	    1.339,(float)1.363,(float)1.342,(float)1.394,(float)1.388,(float)
	    1.402,(float)1.387,(float)1.399,(float)1.417,(float)1.438,(float)
	    1.439,(float)1.452,(float)1.512,(float)1.553,(float)1.561,(float)
	    1.577,(float)1.529,(float)1.899,(float)1.547,(float)1.555,(float)
	    1.539,(float)1.559,(float)1.539,(float)1.575,(float)1.587,(float)
	    1.59,(float)1.579,(float)1.618,(float)1.612,(float)1.647,(float)
	    1.647,(float)1.661,(float)1.758,(float)1.801,(float)1.813,(float)
	    1.843,(float)1.798,(float)2.192,(float)1.746,(float)1.761,(float)
	    1.746,(float)1.758,(float)1.773,(float)1.804,(float)1.789,(float)
	    1.825,(float)1.822,(float)1.845,(float)1.864,(float)1.877,(float)
	    1.9,(float)1.93,(float)2.034,(float)2.102,(float)2.123,(float)
	    2.163,(float)2.125,(float)2.534,(float)1.991,(float)1.987,(float)
	    2.,(float)2.001,(float)2.018,(float)2.045,(float)2.039,(float)
	    2.099,(float)2.107,(float)2.119,(float)2.157,(float)2.148,(float)
	    2.198,(float)2.239,(float)2.363,(float)2.462,(float)2.493,(float)
	    2.544,(float)2.519,(float)2.935,(float)2.292,(float)2.256,(float)
	    2.238,(float)2.272,(float)2.313,(float)2.329,(float)2.325,(float)
	    2.415,(float)2.411,(float)2.428,(float)2.476,(float)2.474,(float)
	    2.542,(float)2.583,(float)2.752,(float)2.895,(float)2.932,(float)
	    3.001,(float)2.998,(float)3.41,(float)2.558,(float)2.614,(float)
	    2.586,(float)2.638,(float)2.634,(float)2.699,(float)2.691,(float)
	    2.751,(float)2.766,(float)2.781,(float)2.846,(float)2.871,(float)
	    2.92,(float)2.991,(float)3.24,(float)3.405,(float)3.452,(float)
	    3.554,(float)3.582,(float)3.972,(float)2.934,(float)2.954,(float)
	    2.99,(float)3.016,(float)3.033,(float)3.079,(float)3.082,(float)
	    3.163,(float)3.203,(float)3.241,(float)3.314,(float)3.302,(float)
	    3.408,(float)3.504,(float)3.818,(float)4.018,(float)4.09,(float)
	    4.229,(float)4.299,(float)4.636,(float)3.345,(float)3.396,(float)
	    3.41,(float)3.46,(float)3.513,(float)3.538,(float)3.564,(float)
	    3.664,(float)3.713,(float)3.765,(float)3.838,(float)3.85,(float)
	    3.98,(float)4.102,(float)4.502,(float)4.784,(float)4.877,(float)
	    5.049,(float)5.181,(float)5.421,(float)3.825,(float)3.88,(float)
	    3.942,(float)3.991,(float)4.088,(float)4.056,(float)4.128,(float)
	    4.266,(float)4.361,(float)4.413,(float)4.52,(float)4.476,(float)
	    4.681,(float)4.866,(float)5.355,(float)5.723,(float)5.84,(float)
	    6.064,(float)6.276,(float)6.351,(float)4.451,(float)4.503,(float)
	    4.567,(float)4.649,(float)4.782,(float)4.761,(float)4.829,(float)
	    5.023,(float)5.136,(float)5.196,(float)5.353,(float)5.312,(float)
	    5.572,(float)5.793,(float)6.435,(float)6.89,(float)7.037,(float)
	    7.318,(float)7.642,(float)7.469,(float)5.155,(float)5.402,(float)
	    5.412,(float)5.551,(float)5.625,(float)5.761,(float)5.822,(float)
	    5.956,(float)6.092,(float)6.228,(float)6.394,(float)6.488,(float)
	    6.685,(float)6.962,(float)7.85,(float)8.363,(float)8.542,(float)
	    8.895,(float)9.369,(float)8.819,(float)6.128,(float)6.516,(float)
	    6.598,(float)6.753,(float)6.865,(float)6.998,(float)7.147,(float)
	    7.27,(float)7.494,(float)7.736,(float)7.848,(float)8.012,(float)
	    8.232,(float)8.615,(float)9.727,(float)10.3,(float)10.52,(float)
	    10.94,(float)11.59,(float)10.46,(float)7.764,(float)8.2,(float)
	    8.306,(float)8.564,(float)8.698,(float)8.866,(float)9.083,(float)
	    9.373,(float)9.599,(float)9.943,(float)10.1,(float)10.24,(float)
	    10.6,(float)11.06,(float)12.32,(float)12.98,(float)13.2,(float)
	    13.65,(float)14.51,(float)12.46,(float)10.5,(float)11.11,(float)
	    11.17,(float)11.64,(float)11.93,(float)12.14,(float)12.39,(float)
	    12.85,(float)13.15,(float)13.46,(float)13.82,(float)13.9,(float)
	    14.42,(float)14.91,(float)16.17,(float)16.83,(float)17.02,(float)
	    17.39,(float)18.46,(float)14.97,(float)15.94,(float)16.88,(float)
	    17.22,(float)17.69,(float)18.17,(float)18.36,(float)18.71,(float)
	    19.24,(float)19.74,(float)20.17,(float)20.5,(float)20.57,(float)
	    21.12,(float)21.62,(float)22.4,(float)22.68,(float)22.71,(float)
	    22.8,(float)24.04,(float)18.18,(float)29.5,(float)31.12,(float)
	    31.3,(float)32.05,(float)32.33,(float)32.93,(float)33.25,(float)
	    33.83,(float)34.1,(float)34.42,(float)34.63,(float)34.7,(float)
	    34.85,(float)34.71,(float)33.47,(float)32.46,(float)32.01,(float)
	    31.15,(float)32.36,(float)22.41,(float)74.09,(float)75.67,(float)
	    75.72,(float)75.77,(float)75.34,(float)75.63,(float)75.14,(float)
	    74.27,(float)73.18,(float)72.17,(float)70.96,(float)70.86,(float)
	    68.31,(float)65.34,(float)56.06,(float)50.6,(float)48.71,(float)
	    45.22,(float)45.59,(float)27.9,(float)321.8,(float)299.9,(float)
	    295.1,(float)282.,(float)271.4,(float)262.8,(float)253.3,(float)
	    239.,(float)226.4,(float)214.,(float)202.1,(float)196.2,(float)
	    179.3,(float)159.2,(float)110.4,(float)88.08,(float)81.47,(float)
	    69.77,(float)65.89,(float)33.1,(float)1844.,(float)1405.,(float)
	    1323.,(float)1138.,(float)1019.,(float)941.7,(float)842.4,(float)
	    740.1,(float)643.6,(float)560.6,(float)500.2,(float)466.7,(float)
	    391.1,(float)309.2,(float)161.9,(float)114.7,(float)102.5,(float)
	    82.55,(float)74.9,(float)34.69 };
    static real qhr[1660]	/* was [20][83] */ = { (float)-5.652e-22,(
	    float)9.414e-22,(float)1.377e-21,(float)5.153e-22,(float)
	    6.702e-22,(float)-3.949e-22,(float)-7.177e-24,(float)-3.24e-22,(
	    float)1.72e-22,(float)-7.197e-22,(float)8.139e-22,(float)
	    -7.425e-22,(float)5.346e-22,(float)8.02e-22,(float)-5.546e-22,(
	    float)1.866e-22,(float)3.793e-22,(float)2.609e-22,(float)
	    -5.096e-23,(float)-2.177e-22,(float)-.004313,(float)-.02148,(
	    float)-.0324,(float)-.03973,(float)-.02668,(float)-.02874,(float)
	    -.03681,(float)-.009355,(float)-.02559,(float)-.0509,(float)
	    -.02558,(float)-.02536,(float)-.02282,(float)-.02738,(float)
	    -.01304,(float)-.007885,(float)-.007249,(float)-.005137,(float)
	    -.001452,(float)1.329e-4,(float).08133,(float).03455,(float)
	    .04286,(float).02373,(float)8.251e-4,(float).04286,(float).01664,(
	    float).01593,(float).0142,(float).02446,(float).01282,(float)
	    .0169,(float).01047,(float)-.001208,(float).003771,(float)
	    -.001345,(float)-.003766,(float)-.004366,(float)-.001341,(float)
	    .00422,(float).02057,(float).07867,(float).08798,(float).06997,(
	    float).04873,(float).06443,(float).06839,(float).01917,(float)
	    .01941,(float).04509,(float).01142,(float).05213,(float).001513,(
	    float).01797,(float).02171,(float).009548,(float).007035,(float)
	    .005665,(float).003994,(float).01304,(float).04002,(float).04848,(
	    float).039,(float).0409,(float).05822,(float).04238,(float).04673,
	    (float).02439,(float).05511,(float).04499,(float).03862,(float)
	    .03816,(float).02542,(float).0403,(float).01232,(float).01448,(
	    float).01472,(float).01406,(float).008694,(float).01961,(float)
	    .03516,(float).03081,(float).03007,(float).03377,(float).03429,(
	    float).02595,(float).02728,(float).01935,(float).02178,(float)
	    .01829,(float).02034,(float).01641,(float).02023,(float).01935,(
	    float).008975,(float).01025,(float).01122,(float).01053,(float)
	    .007307,(float).02278,(float).01535,(float).006826,(float).01692,(
	    float).006982,(float)-.001839,(float).008069,(float).005361,(
	    float).007732,(float)-.007668,(float).002361,(float)-.003045,(
	    float)-3.1e-4,(float)-.003904,(float)-.005775,(float)-.003002,(
	    float)-.001326,(float)-.002153,(float)-.001487,(float)1.631e-4,(
	    float).02239,(float)-.01468,(float)-.01285,(float)-.01615,(float)
	    -.01835,(float)-.002601,(float)-.01591,(float)-.01598,(float)
	    -.02749,(float)-.01665,(float)-.02006,(float)-.02027,(float)
	    -.02087,(float)-.02441,(float)-.0212,(float)-.0247,(float)-.0198,(
	    float)-.01956,(float)-.01449,(float)-.008888,(float).01925,(float)
	    -.0531,(float)-.02542,(float)-.0291,(float)-.0177,(float)-.04287,(
	    float)-.03601,(float)-.02734,(float)-.05613,(float)-.04416,(float)
	    -.03582,(float)-.04761,(float)-.0399,(float)-.04896,(float)
	    -.04041,(float)-.04071,(float)-.0361,(float)-.03404,(float)
	    -.02718,(float)-.0174,(float).01498,(float)-.06907,(float)-.06736,
	    (float)-.0505,(float)-.06745,(float)-.06043,(float)-.0714,(float)
	    -.06933,(float)-.06637,(float)-.06232,(float)-.05358,(float)
	    -.06124,(float)-.06941,(float)-.06321,(float)-.05868,(float)
	    -.05149,(float)-.04634,(float)-.04349,(float)-.03704,(float)
	    -.02431,(float).01017,(float)-.0904,(float)-.09198,(float)-.08576,
	    (float)-.08655,(float)-.08286,(float)-.09417,(float)-.0866,(float)
	    -.08221,(float)-.07647,(float)-.07447,(float)-.07568,(float)
	    -.08761,(float)-.07552,(float)-.06833,(float)-.06463,(float)
	    -.05262,(float)-.04988,(float)-.04262,(float)-.02893,(float)
	    .006718,(float)-.1349,(float)-.1066,(float)-.1091,(float)-.1051,(
	    float)-.1083,(float)-.1015,(float)-.1027,(float)-.112,(float)
	    -.1002,(float)-.08945,(float)-.09914,(float)-.0891,(float)-.0933,(
	    float)-.08267,(float)-.06949,(float)-.05715,(float)-.05493,(float)
	    -.04596,(float)-.03167,(float).004972,(float)-.1587,(float)-.1388,
	    (float)-.1339,(float)-.133,(float)-.1315,(float)-.1202,(float)
	    -.1238,(float)-.12,(float)-.113,(float)-.1068,(float)-.1048,(
	    float)-.1005,(float)-.09863,(float)-.08673,(float)-.06932,(float)
	    -.05584,(float)-.0546,(float)-.04705,(float)-.03279,(float)
	    .004223,(float)-.1714,(float)-.1596,(float)-.1502,(float)-.153,(
	    float)-.1377,(float)-.1395,(float)-.1361,(float)-.1182,(float)
	    -.1162,(float)-.1059,(float)-.1049,(float)-.1067,(float)-.09404,(
	    float)-.08466,(float)-.06602,(float)-.0521,(float)-.05109,(float)
	    -.04461,(float)-.03224,(float).004145,(float)-.1827,(float)-.1506,
	    (float)-.1358,(float)-.1223,(float)-.135,(float)-.1301,(float)
	    -.1154,(float)-.1248,(float)-.1149,(float)-.09378,(float)-.1087,(
	    float)-.09678,(float)-.09572,(float)-.08131,(float)-.05468,(float)
	    -.04839,(float)-.04825,(float)-.04,(float)-.03074,(float).004127,(
	    float)-.1288,(float)-.1155,(float)-.1073,(float)-.1081,(float)
	    -.09677,(float)-.09785,(float)-.09423,(float)-.09325,(float)
	    -.08304,(float)-.07825,(float)-.0779,(float)-.07441,(float)
	    -.07181,(float)-.05719,(float)-.0446,(float)-.03949,(float)
	    -.04026,(float)-.03403,(float)-.02843,(float).003634,(float)
	    -.05406,(float)-.06667,(float)-.06237,(float)-.06315,(float)
	    -.05612,(float)-.06059,(float)-.05514,(float)-.04818,(float)
	    -.04706,(float)-.05091,(float)-.0443,(float)-.04953,(float)
	    -.03974,(float)-.03646,(float)-.03401,(float)-.02864,(float)
	    -.03002,(float)-.0285,(float)-.02558,(float).002971,(float)
	    -.02625,(float)-.02285,(float)-.02447,(float)-.02324,(float)
	    -.02971,(float)-.02294,(float)-.02077,(float)-.02681,(float)
	    -.02803,(float)-.02584,(float)-.02972,(float)-.02205,(float)
	    -.02616,(float)-.02559,(float)-.02117,(float)-.02141,(float)
	    -.02356,(float)-.02351,(float)-.023,(float).002403,(float)-.01019,
	    (float)-.01157,(float)-.007987,(float)-.01108,(float)-.01186,(
	    float)-.009932,(float)-.01127,(float)-.01105,(float)-.01121,(
	    float)-.0102,(float)-.01305,(float)-.01164,(float)-.01341,(float)
	    -.01172,(float)-.0125,(float)-.01553,(float)-.01751,(float)
	    -.01813,(float)-.02036,(float).001743,(float)-.002488,(float)
	    -6.434e-4,(float).001037,(float)-.0012,(float)-.001925,(float)
	    -.003946,(float)-9.738e-4,(float)-.00434,(float)-.00309,(float)
	    -.002085,(float)-.005027,(float)-.002738,(float)-.004901,(float)
	    -.004864,(float)-.005944,(float)-.01121,(float)-.0131,(float)
	    -.01437,(float)-.01808,(float)9.126e-4,(float)-.001738,(float)
	    .004941,(float).003342,(float).004999,(float)6.147e-4,(float)
	    .001885,(float).005773,(float)-.001019,(float)-7.171e-4,(float)
	    .002355,(float)-.002336,(float).001631,(float)-.001544,(float)
	    -.002056,(float)-.002347,(float)-.007991,(float)-.009902,(float)
	    -.0118,(float)-.01626,(float)-2.284e-4,(float).001211,(float)
	    5.109e-4,(float).003115,(float).001866,(float).003895,(float)
	    .001371,(float).002083,(float).002498,(float).003461,(float)
	    .004949,(float).002746,(float)2.907e-4,(float).001408,(float)
	    .002111,(float)-.001709,(float)-.005756,(float)-.007011,(float)
	    -.00915,(float)-.01461,(float)-.001605,(float)4.563e-4,(float)
	    9.727e-4,(float).004347,(float).00334,(float).005068,(float)
	    9.16e-4,(float).003273,(float).003301,(float).002662,(float)
	    .005296,(float).001041,(float).001065,(float).001477,(float)
	    .001751,(float)-5.68e-4,(float)-.003976,(float)-.005689,(float)
	    -.00808,(float)-.01347,(float)-.00291,(float).003466,(float)
	    .002256,(float).00178,(float).001777,(float).002352,(float)
	    .003312,(float).002883,(float).002703,(float).002987,(float)
	    .002505,(float).003174,(float).003352,(float).003356,(float)
	    .002285,(float)-4.256e-4,(float)-.003076,(float)-.005154,(float)
	    -.007606,(float)-.01259,(float)-.004133,(float)-1.493e-4,(float)
	    1.119e-4,(float)7.169e-4,(float).001436,(float).002879,(float)
	    6.612e-4,(float)8.928e-4,(float).002312,(float).003538,(float)
	    .002509,(float).003479,(float).001473,(float).002461,(float)
	    .003585,(float)-7.978e-4,(float)-.002895,(float)-.004173,(float)
	    -.006912,(float)-.01191,(float)-.005437,(float)3.994e-4,(float)
	    -.00249,(float)-1.787e-4,(float)-.001121,(float).002913,(float)
	    -5.298e-4,(float)-6.255e-4,(float).001985,(float).004784,(float)
	    .001762,(float).003466,(float)-6.874e-4,(float).003178,(float)
	    .00408,(float)-.001225,(float)-.003062,(float)-.003917,(float)
	    -.006317,(float)-.01146,(float)-.006707,(float)-.002694,(float)
	    -2.937e-4,(float)-1.866e-4,(float)-.001198,(float)-.002223,(float)
	    -.001417,(float)-2.589e-4,(float)-4.751e-4,(float)-8.729e-4,(
	    float).001775,(float)2.804e-4,(float)3.727e-4,(float)6.163e-4,(
	    float)8.305e-4,(float)-8.54e-4,(float)-.002996,(float)-.004468,(
	    float)-.006653,(float)-.01129,(float)-.007812,(float)-.005007,(
	    float)-.003061,(float)-3.717e-4,(float)-.001074,(float)-.005321,(
	    float)-.001727,(float)-.001137,(float)-.001738,(float)-.003289,(
	    float)3.908e-4,(float)-.002467,(float)1.043e-4,(float)-.00131,(
	    float)-7.996e-4,(float)-4.459e-4,(float)-.00298,(float)-.004325,(
	    float)-.007137,(float)-.01118,(float)-.008877,(float)-.007406,(
	    float)-.004907,(float)-.00419,(float)-.003401,(float)-.004496,(
	    float)-.003372,(float)-.002237,(float)-.002378,(float)-.002055,(
	    float)-.001609,(float)-.002409,(float)-.001979,(float)-.001013,(
	    float)-2.206e-4,(float)-.001989,(float)-.003291,(float)-.004049,(
	    float)-.006806,(float)-.01106,(float)-.009991,(float)-.009019,(
	    float)-.00615,(float)-.007022,(float)-.008284,(float)-.002506,(
	    float)-.004507,(float)-.004908,(float)-.005705,(float)-.001804,(
	    float)-8.453e-4,(float)-.001813,(float)-.001952,(float)-.002031,(
	    float)4.236e-4,(float)-.002721,(float)-.004053,(float)-.004832,(
	    float)-.006484,(float)-.01105,(float)-.01106,(float)-.01274,(
	    float)-.009551,(float)-.004385,(float)-.006722,(float)-.006739,(
	    float)-.008046,(float)-.005277,(float)-.006433,(float)-.004225,(
	    float)-4.221e-4,(float)-.00508,(float)-.004301,(float)-.004379,(
	    float)-.001935,(float)-8.114e-4,(float)-.003831,(float)-.005011,(
	    float)-.006791,(float)-.01115,(float)-.01208,(float)-.01195,(
	    float)-.01002,(float)-.006997,(float)-.005177,(float)-.006839,(
	    float)-.00903,(float)-.004451,(float)-.004408,(float)-.004292,(
	    float)-.003227,(float)-.003482,(float)-.004007,(float)-.002608,(
	    float)-.001592,(float)-.001992,(float)-.003254,(float)-.00428,(
	    float)-.006897,(float)-.01122,(float)-.01311,(float)-.009552,(
	    float)-.01206,(float)-.01042,(float)-.009347,(float)-.005546,(
	    float)-.008927,(float)-.007563,(float)-.003969,(float)-.001369,(
	    float)-.003607,(float)-2.263e-5,(float)-.003971,(float)-6.183e-4,(
	    float).001039,(float)-.003635,(float)-.002734,(float)-.003671,(
	    float)-.006685,(float)-.01127,(float)-.01413,(float)-.008995,(
	    float)-.0091,(float)-.006191,(float)-.008328,(float)-.005542,(
	    float)-.005286,(float)-.006745,(float)-.003565,(float)-.001593,(
	    float)-.001441,(float)-9.982e-4,(float)-.002212,(float)-7.185e-4,(
	    float)4.55e-4,(float)-.001756,(float)-.002997,(float)-.003999,(
	    float)-.006533,(float)-.01145,(float)-.01507,(float)-.008926,(
	    float)-.005451,(float)-.004346,(float)-.005107,(float)-.004234,(
	    float)-.003281,(float)-.003568,(float)-.003522,(float)-.002009,(
	    float)8.404e-4,(float)-5.088e-4,(float)-6.598e-4,(float)-3.818e-4,
	    (float).001046,(float)-2.795e-4,(float)-.003025,(float)-.004196,(
	    float)-.006329,(float)-.01163,(float)-.01598,(float)-.006393,(
	    float)-.003768,(float)-.006999,(float)-.003385,(float)-8.077e-4,(
	    float)-6.92e-4,(float)-.0012,(float)-5.782e-5,(float)5.345e-4,(
	    float)8.303e-4,(float).001044,(float).001032,(float)8.694e-4,(
	    float).001521,(float)-.001043,(float)-.001965,(float)-.00315,(
	    float)-.006145,(float)-.01178,(float)-.01684,(float)-.002516,(
	    float).002641,(float)-.001642,(float).001059,(float)-.003385,(
	    float).002602,(float).002491,(float)4.956e-4,(float).001736,(
	    float).002055,(float).002645,(float).004533,(float).00264,(float)
	    .003464,(float)4.989e-4,(float)-.001457,(float)-.0027,(float)
	    -.0061,(float)-.01197,(float)-.01754,(float)-.001326,(float)
	    .005438,(float).003803,(float).00583,(float).002486,(float)
	    .004785,(float).007746,(float)-2.255e-4,(float).001974,(float)
	    .005549,(float).003514,(float).006206,(float).003864,(float)
	    .00435,(float).002797,(float)-.001773,(float)-.002816,(float)
	    -.005659,(float)-.01215,(float)-.01824,(float).001299,(float)
	    .005056,(float).005121,(float).006722,(float).00351,(float).00416,
	    (float).007179,(float).004909,(float).005268,(float).006134,(
	    float).005332,(float).0065,(float).005761,(float).00573,(float)
	    .003355,(float)-2.319e-4,(float)-.001362,(float)-.005194,(float)
	    -.01226,(float)-.01911,(float).003362,(float).005468,(float)
	    .006217,(float).007302,(float).006383,(float).00759,(float)
	    .007156,(float).007191,(float).007065,(float).008483,(float)
	    .007357,(float).007573,(float).007659,(float).008103,(float)
	    .003589,(float).001232,(float)-4.341e-4,(float)-.004903,(float)
	    -.01244,(float)-.01991,(float).004485,(float).009817,(float)
	    .009376,(float).01098,(float).01321,(float).009385,(float).01314,(
	    float).005593,(float).01055,(float).01277,(float).009363,(float)
	    .01072,(float).008838,(float).01006,(float).005501,(float)
	    6.366e-4,(float)-4.483e-4,(float)-.004035,(float)-.0126,(float)
	    -.02051,(float).00563,(float).01079,(float).01007,(float).009981,(
	    float).0158,(float).01059,(float).01313,(float).006115,(float)
	    .01333,(float).01429,(float).01166,(float).01178,(float).01052,(
	    float).01214,(float).006332,(float)8.594e-4,(float)5.659e-5,(
	    float)-.003516,(float)-.01262,(float)-.02079,(float).006771,(
	    float).01084,(float).01456,(float).01275,(float).01651,(float)
	    .008535,(float).01336,(float).007903,(float).01523,(float).01594,(
	    float).01333,(float).01061,(float).01259,(float).01383,(float)
	    .007456,(float).001673,(float).001032,(float)-.003219,(float)
	    -.01264,(float)-.02106,(float).01022,(float).009448,(float).01745,
	    (float).0125,(float).01558,(float).01049,(float).01225,(float)
	    .01328,(float).01631,(float).01703,(float).01562,(float).01079,(
	    float).01465,(float).01552,(float).008347,(float).004333,(float)
	    .002605,(float)-.003215,(float)-.01282,(float)-.02146,(float)
	    .01252,(float).0161,(float).01543,(float).01623,(float).01728,(
	    float).01694,(float).01854,(float).0156,(float).01758,(float)
	    .01644,(float).01571,(float).01752,(float).01486,(float).01547,(
	    float).009798,(float).004814,(float).002722,(float)-.002886,(
	    float)-.01311,(float)-.0217,(float).01659,(float).01855,(float)
	    .01825,(float).01743,(float).02283,(float).01781,(float).02003,(
	    float).01761,(float).02096,(float).02071,(float).0205,(float)
	    .01904,(float).01833,(float).01995,(float).01176,(float).005511,(
	    float).003645,(float)-.001855,(float)-.01314,(float)-.02211,(
	    float).02257,(float).01618,(float).01987,(float).0178,(float)
	    .02308,(float).01729,(float).01786,(float).02237,(float).02608,(
	    float).02338,(float).02396,(float).01741,(float).02314,(float)
	    .02294,(float).01182,(float).007957,(float).006001,(float)
	    -9.998e-4,(float)-.01309,(float)-.02266,(float).02038,(float)
	    .0227,(float).02443,(float).02224,(float).02508,(float).02452,(
	    float).02448,(float).0244,(float).02591,(float).02527,(float)
	    .02515,(float).0225,(float).02269,(float).02445,(float).014,(
	    float).009499,(float).006844,(float)-6.527e-4,(float)-.01327,(
	    float)-.02292,(float).02396,(float).02782,(float).03077,(float)
	    .03006,(float).02401,(float).02989,(float).02963,(float).02697,(
	    float).02342,(float).02967,(float).02609,(float).02662,(float)
	    .02445,(float).02457,(float).01758,(float).01017,(float).00744,(
	    float)1.118e-4,(float)-.01331,(float)-.02289,(float).02829,(float)
	    .02992,(float).02968,(float).02789,(float).03473,(float).0303,(
	    float).02795,(float).02854,(float).03227,(float).03417,(float)
	    .02929,(float).02836,(float).0286,(float).02931,(float).01764,(
	    float).01134,(float).00904,(float).001955,(float)-.01307,(float)
	    -.02282,(float).02622,(float).03257,(float).0336,(float).03505,(
	    float).03949,(float).03068,(float).03517,(float).03133,(float)
	    .03423,(float).03694,(float).03304,(float).03031,(float).02962,(
	    float).03258,(float).01944,(float).01385,(float).01074,(float)
	    .002583,(float)-.013,(float)-.02253,(float).03117,(float).04231,(
	    float).04489,(float).04417,(float).03137,(float).04281,(float)
	    .04348,(float).03471,(float).03271,(float).03621,(float).03278,(
	    float).0386,(float).03174,(float).03044,(float).02505,(float)
	    .01574,(float).01194,(float).002506,(float)-.013,(float)-.02219,(
	    float).03548,(float).03962,(float).04517,(float).04075,(float)
	    .03798,(float).04487,(float).03882,(float).04033,(float).03682,(
	    float).04565,(float).034,(float).03844,(float).03386,(float)
	    .03379,(float).02669,(float).01745,(float).01392,(float).004828,(
	    float)-.01248,(float)-.02208,(float).04045,(float).04831,(float)
	    .04316,(float).04337,(float).05508,(float).0437,(float).04532,(
	    float).03909,(float).04823,(float).05179,(float).0436,(float)
	    .04273,(float).04029,(float).04396,(float).02577,(float).01808,(
	    float).01539,(float).008041,(float)-.01171,(float)-.02176,(float)
	    .04483,(float).05199,(float).05245,(float).05498,(float).04682,(
	    float).04908,(float).05204,(float).04932,(float).04623,(float)
	    .04982,(float).04444,(float).04566,(float).04386,(float).04278,(
	    float).03125,(float).0223,(float).01904,(float).008175,(float)
	    -.01125,(float)-.02092,(float).04913,(float).05227,(float).05575,(
	    float).05322,(float).05096,(float).05736,(float).05212,(float)
	    .05292,(float).04942,(float).05453,(float).04499,(float).05043,(
	    float).04414,(float).04506,(float).036,(float).02555,(float)
	    .02107,(float).009348,(float)-.01065,(float)-.0198,(float).04837,(
	    float).06304,(float).06458,(float).06139,(float).05694,(float)
	    .05621,(float).05906,(float).0509,(float).04909,(float).06488,(
	    float).04726,(float).05705,(float).04571,(float).04588,(float)
	    .03937,(float).02615,(float).02186,(float).01291,(float)-.009443,(
	    float)-.01841,(float).05558,(float).06644,(float).0659,(float)
	    .06674,(float).0638,(float).0665,(float).06778,(float).05525,(
	    float).06249,(float).06501,(float).05435,(float).05887,(float)
	    .05412,(float).05532,(float).04114,(float).02827,(float).02566,(
	    float).01596,(float)-.008028,(float)-.01695,(float).05632,(float)
	    .06961,(float).07301,(float).07196,(float).06197,(float).06731,(
	    float).06769,(float).06602,(float).05941,(float).06474,(float)
	    .05612,(float).06274,(float).05507,(float).05771,(float).04509,(
	    float).03563,(float).03135,(float).01712,(float)-.006614,(float)
	    -.01556,(float).07013,(float).07646,(float).0653,(float).06613,(
	    float).06336,(float).07626,(float).06898,(float).06821,(float)
	    .06084,(float).06791,(float).06334,(float).0713,(float).06218,(
	    float).05807,(float).0486,(float).03828,(float).03269,(float)
	    .02057,(float)-.0048,(float)-.01355,(float).07452,(float).07575,(
	    float).0853,(float).08054,(float).07228,(float).0717,(float)
	    .08005,(float).07123,(float).0678,(float).07775,(float).06192,(
	    float).0689,(float).06413,(float).06175,(float).05804,(float)
	    .03732,(float).03479,(float).02485,(float)-.002568,(float)-.0108,(
	    float).07231,(float).08536,(float).081,(float).08335,(float)
	    .06858,(float).08259,(float).08349,(float).07677,(float).06821,(
	    float).07289,(float).06735,(float).076,(float).06922,(float)
	    .06692,(float).0577,(float).04515,(float).04155,(float).02842,(
	    float)2.316e-4,(float)-.007844,(float).0889,(float).07497,(float)
	    .07168,(float).07577,(float).08092,(float).07632,(float).07226,(
	    float).08395,(float).07542,(float).07601,(float).07705,(float)
	    .07541,(float).07416,(float).06815,(float).05971,(float).05192,(
	    float).0462,(float).03146,(float).003061,(float)-.004848,(float)
	    .08722,(float).0943,(float).07552,(float).08372,(float).07072,(
	    float).09607,(float).08684,(float).08011,(float).06948,(float)
	    .06822,(float).07289,(float).0908,(float).07933,(float).06942,(
	    float).06539,(float).05435,(float).04829,(float).03422,(float)
	    .00607,(float)-9.065e-4,(float).09534,(float).0863,(float).08379,(
	    float).09626,(float).0714,(float).07728,(float).09591,(float)
	    .08679,(float).07942,(float).07584,(float).08262,(float).08251,(
	    float).08333,(float).07462,(float).07594,(float).05621,(float)
	    .05326,(float).03999,(float).01038,(float).003866,(float).08891,(
	    float).08251,(float).07635,(float).08004,(float).0698,(float)
	    .07295,(float).07709,(float).09204,(float).06939,(float).07646,(
	    float).08428,(float).07776,(float).08379,(float).07353,(float)
	    .07405,(float).06525,(float).06045,(float).04683,(float).01547,(
	    float).007975,(float).08269,(float).06499,(float).07311,(float)
	    .07107,(float).09243,(float).0761,(float).07128,(float).0737,(
	    float).09442,(float).08409,(float).09148,(float).07403,(float)
	    .08689,(float).09213,(float).07652,(float).06667,(float).06287,(
	    float).05261,(float).02014,(float).01258,(float).1003,(float)
	    .07332,(float).05822,(float).06142,(float).06652,(float).08304,(
	    float).06704,(float).09114,(float).08481,(float).05685,(float)
	    .0967,(float).0857,(float).09666,(float).08559,(float).07664,(
	    float).07688,(float).07114,(float).05304,(float).02522,(float)
	    .01858,(float).06763,(float).08381,(float).0648,(float).07472,(
	    float).06055,(float).07244,(float).08266,(float).07572,(float)
	    .07315,(float).07211,(float).08878,(float).08986,(float).08051,(
	    float).08296,(float).08816,(float).07661,(float).07291,(float)
	    .05871,(float).03139,(float).02488,(float).08108,(float).03474,(
	    float).05886,(float).04178,(float).06636,(float).04201,(float)
	    .04439,(float).07807,(float).08639,(float).06113,(float).09254,(
	    float).06068,(float).08971,(float).09431,(float).09041,(float)
	    .08159,(float).07942,(float).06699,(float).03847,(float).03137,(
	    float).0153,(float).05205,(float).06843,(float).04422,(float)
	    .05477,(float).05154,(float).05628,(float).03782,(float).06086,(
	    float).07916,(float).04597,(float).07006,(float).04985,(float)
	    .07357,(float).09147,(float).08153,(float).07793,(float).07472,(
	    float).04606,(float).03738,(float).04417,(float).009879,(float)
	    .05198,(float).01839,(float).06935,(float).01437,(float).01977,(
	    float).04951,(float).105,(float).0597,(float).08947,(float).04909,
	    (float).0892,(float).1039,(float).08411,(float).08537,(float)
	    .08813,(float).0796,(float).05331,(float).04299,(float).04865,(
	    float).03438,(float).0243,(float).05264,(float).003807,(float)
	    .03095,(float).05664,(float).07015,(float).03096,(float).01954,(
	    float).05791,(float).0656,(float).07194,(float).06191,(float)
	    .09423,(float).09095,(float).09055,(float).07448,(float).05918,(
	    float).04889,(float)-.02253,(float).04582,(float).01679,(float)
	    .05821,(float)4.117e-4,(float).06875,(float).06722,(float).008199,
	    (float)-.00655,(float).02637,(float).006618,(float).07687,(float)
	    .03749,(float).0347,(float).09277,(float).08504,(float).08425,(
	    float).08043,(float).06658,(float).05418,(float)-.008601,(float)
	    .00784,(float)-.06881,(float)-.01082,(float).03388,(float).03148,(
	    float).02624,(float).0194,(float).03203,(float)-.003274,(float)
	    .02768,(float).05273,(float).04367,(float).04893,(float).07651,(
	    float).08838,(float).0867,(float).08777,(float).07396,(float)
	    .05791,(float).01806,(float)-.01749,(float)-.04788,(float).01844,(
	    float)-.06244,(float).007756,(float).02828,(float).03688,(float)
	    -.0172,(float)-.001087,(float).02528,(float).03299,(float).0463,(
	    float).03296,(float).08829,(float).08481,(float).08757,(float)
	    .09274,(float).08132,(float).06107,(float)-.07635,(float).0246,(
	    float)-.08695,(float)-.001667,(float)-.04231,(float)-.01868,(
	    float).004223,(float)5.608e-5,(float).01845,(float)-.005281,(
	    float).05614,(float).03491,(float).05292,(float).06785,(float)
	    .06671,(float).09359,(float).1005,(float).09622,(float).08653,(
	    float).06273,(float)-.005849,(float)-.1583,(float)-.1898,(float)
	    -.201,(float).106,(float)-.02055,(float)-.1563,(float).0439,(
	    float).1281,(float)-.009384,(float).1248,(float)-.001907,(float)
	    .1102,(float).1091,(float).02218,(float).1219,(float).1175,(float)
	    .08918,(float).08619,(float).05995,(float)-.009235,(float)-.05196,
	    (float)-.06728,(float)-.08919,(float)-.1044,(float).05688,(float)
	    -.0607,(float).1358,(float)-.03856,(float)-.09625,(float).00779,(
	    float).08271,(float).06302,(float).0215,(float).06686,(float)
	    .1652,(float).1312,(float).06334,(float).0814,(float).05742,(
	    float)-.02265,(float)-.07299,(float).1288,(float)-.03129,(float)
	    -.1264,(float)-.01536,(float)-.05531,(float).1169,(float)-.08897,(
	    float).07555,(float).03756,(float).04703,(float).06338,(float)
	    -.01466,(float).1021,(float).1679,(float).1295,(float).04469,(
	    float).07159,(float).04489,(float).3535,(float)-.3856,(float)
	    -.2377,(float)-.3706,(float).4247,(float)-.2547,(float)-.3453,(
	    float).1794,(float).244,(float).265,(float).3277,(float)-.1358,(
	    float).3112,(float).09967,(float).04647,(float).11,(float).1019,(
	    float).04983,(float).054,(float).01604,(float).3803,(float)
	    -.01326,(float)-.2115,(float)-.08864,(float).4541,(float).3047,(
	    float).08463,(float).4116,(float).45,(float)-.1381,(float).1548,(
	    float).009753,(float).2792,(float).1809,(float).0139,(float)
	    .01693,(float).02994,(float).03037,(float).01954,(float)7.223e-4,(
	    float)-2.302e-18,(float)9.563e-19,(float)7.805e-19,(float)
	    -3.064e-18,(float)8.861e-19,(float)4.664e-19,(float)2.078e-18,(
	    float)-1.607e-18,(float)1.261e-18,(float)-4.426e-19,(float)
	    -3.183e-19,(float)4.34e-19,(float)9.208e-19,(float)-6.464e-20,(
	    float)3.706e-19,(float)2.805e-19,(float)6.088e-20,(float)
	    -8.083e-20,(float)-3.907e-20,(float)-4.384e-20 };
    static real uhr[1660]	/* was [20][83] */ = { (float)-.8426,(float)
	    -.7371,(float)-.8416,(float)-.7122,(float)-.8664,(float)-.6608,(
	    float)-.6706,(float)-.5889,(float)-.6593,(float)-.6636,(float)
	    -.5688,(float)-.5587,(float)-.5378,(float)-.5168,(float)-.3996,(
	    float)-.3001,(float)-.2823,(float)-.2252,(float)-.131,(float)
	    -.2124,(float)-.1512,(float)-.2069,(float)-.3336,(float)-.3136,(
	    float)-.3516,(float)-.2564,(float)-.3198,(float)-.3041,(float)
	    -.3391,(float)-.3651,(float)-.3293,(float)-.3274,(float)-.3285,(
	    float)-.3438,(float)-.3168,(float)-.2572,(float)-.2452,(float)
	    -.2012,(float)-.1223,(float)-.203,(float).1679,(float).11,(float)
	    .09135,(float).08403,(float).09632,(float).05049,(float).04301,(
	    float).02199,(float).008364,(float)-.01746,(float)-.0216,(float)
	    -.04008,(float)-.04629,(float)-.06097,(float)-.1228,(float)-.132,(
	    float)-.1304,(float)-.1206,(float)-.08941,(float)-.1662,(float)
	    .2232,(float).209,(float).2252,(float).1993,(float).2176,(float)
	    .1855,(float).1719,(float).1577,(float).1549,(float).1192,(float)
	    .1168,(float).114,(float).09712,(float).08793,(float).01827,(
	    float)-.0201,(float)-.02797,(float)-.04027,(float)-.04865,(float)
	    -.1175,(float).1845,(float).2205,(float).2291,(float).2105,(float)
	    .2284,(float).2171,(float).2037,(float).1853,(float).1888,(float)
	    .1931,(float).1559,(float).1774,(float).1408,(float).1405,(float)
	    .09226,(float).04422,(float).0331,(float).01409,(float)-.01497,(
	    float)-.07631,(float).1837,(float).1991,(float).201,(float).2002,(
	    float).2014,(float).1947,(float).2043,(float).185,(float).1881,(
	    float).1981,(float).1794,(float).1834,(float).168,(float).1646,(
	    float).1194,(float).07563,(float).06589,(float).0458,(float)
	    .006717,(float)-.04685,(float).1682,(float).1714,(float).175,(
	    float).1792,(float).1735,(float).1732,(float).1804,(float).1764,(
	    float).1804,(float).173,(float).1709,(float).1683,(float).1685,(
	    float).1628,(float).1239,(float).08821,(float).08013,(float)
	    .05784,(float).0175,(float)-.02746,(float).1336,(float).1405,(
	    float).1408,(float).147,(float).1546,(float).151,(float).149,(
	    float).1473,(float).1559,(float).1437,(float).1498,(float).1485,(
	    float).1478,(float).1442,(float).114,(float).08727,(float).07979,(
	    float).05822,(float).02105,(float)-.01602,(float).115,(float)
	    .1101,(float).105,(float).1066,(float).1331,(float).1202,(float)
	    .1157,(float).1235,(float).1343,(float).1199,(float).1308,(float)
	    .122,(float).124,(float).1247,(float).09693,(float).07862,(float)
	    .07176,(float).05374,(float).02099,(float)-.009286,(float).09094,(
	    float).0938,(float).1006,(float).102,(float).09517,(float).09626,(
	    float).1037,(float).09957,(float).09504,(float).1009,(float)
	    .09775,(float).103,(float).09613,(float).09378,(float).08495,(
	    float).06639,(float).06056,(float).04651,(float).01948,(float)
	    -.005518,(float).06528,(float).08414,(float).08382,(float).08936,(
	    float).08239,(float).07806,(float).09017,(float).07441,(float)
	    .07598,(float).08032,(float).073,(float).08363,(float).07267,(
	    float).07476,(float).07222,(float).05388,(float).05007,(float)
	    .04075,(float).01779,(float)-.003717,(float).04627,(float).06728,(
	    float).0633,(float).06719,(float).06215,(float).06872,(float)
	    .06776,(float).05497,(float).06421,(float).06715,(float).05842,(
	    float).06368,(float).05809,(float).06281,(float).05605,(float)
	    .04542,(float).04322,(float).03561,(float).01642,(float)-.003145,(
	    float).0377,(float).05005,(float).04451,(float).04886,(float)
	    .0421,(float).05575,(float).04827,(float).04692,(float).04938,(
	    float).05098,(float).04893,(float).05153,(float).0478,(float)
	    .04919,(float).04485,(float).03776,(float).03693,(float).03126,(
	    float).01511,(float)-.002474,(float).03534,(float).03647,(float)
	    .03715,(float).03401,(float).03817,(float).03391,(float).03547,(
	    float).03867,(float).04081,(float).03847,(float).03945,(float)
	    .03881,(float).03904,(float).03882,(float).03834,(float).03177,(
	    float).03125,(float).02733,(float).01386,(float)-.001032,(float)
	    .01775,(float).02936,(float).02777,(float).03445,(float).01735,(
	    float).02664,(float).03437,(float).02592,(float).02308,(float)
	    .02976,(float).02725,(float).03151,(float).02642,(float).02669,(
	    float).03263,(float).02771,(float).02626,(float).02306,(float)
	    .01282,(float)7.928e-4,(float).003677,(float).01733,(float).01595,
	    (float).02153,(float).006287,(float).02403,(float).02049,(float)
	    .0161,(float).01172,(float).01945,(float).01045,(float).0256,(
	    float).01512,(float).01515,(float).02842,(float).02399,(float)
	    .02268,(float).02054,(float).01222,(float).002525,(float).008794,(
	    float).006293,(float)-3.422e-4,(float).00638,(float).008136,(
	    float).007792,(float).008467,(float).01415,(float).01688,(float)
	    .007791,(float).02091,(float).0166,(float).02154,(float).02085,(
	    float).02225,(float).02183,(float).02242,(float).01991,(float)
	    .01221,(float).004049,(float).004774,(float)3.976e-4,(float)
	    -.002512,(float)-2.794e-4,(float).003077,(float).002567,(float)
	    .00404,(float).009052,(float).0123,(float).006595,(float).01823,(
	    float).01176,(float).019,(float).01814,(float).01883,(float)
	    .02161,(float).02178,(float).01853,(float).01227,(float).005865,(
	    float)-.002936,(float).001455,(float)-.00353,(float).002265,(
	    float)-.003569,(float).005055,(float).005856,(float).006789,(
	    float).003472,(float).005837,(float).008151,(float).01176,(float)
	    .0123,(float).01036,(float).01916,(float).02033,(float).01981,(
	    float).01776,(float).01246,(float).008004,(float)-.001055,(float)
	    -.003213,(float)-.001489,(float)-4.472e-4,(float)-.002117,(float)
	    .001865,(float)4.377e-4,(float).005208,(float).004617,(float)
	    .00668,(float).009053,(float).01015,(float).01123,(float).01216,(
	    float).01937,(float).01909,(float).01903,(float).0182,(float)
	    .01295,(float).01014,(float)-.004216,(float)5.012e-4,(float)
	    -.00135,(float)5.364e-4,(float)-.001946,(float).003724,(float)
	    .004102,(float).002953,(float).002471,(float).005578,(float)
	    .006726,(float).01212,(float).009375,(float).009457,(float).01935,
	    (float).01917,(float).01882,(float).01831,(float).0135,(float)
	    .01221,(float)-.00393,(float)-3.795e-4,(float)-.002088,(float)
	    5.133e-4,(float)-.003192,(float).002628,(float).004671,(float)
	    .002873,(float).004665,(float).00531,(float).007144,(float).01034,
	    (float).009789,(float).01169,(float).01982,(float).01939,(float)
	    .01913,(float).01893,(float).01435,(float).01418,(float)-.002856,(
	    float)-.003606,(float)-.003299,(float)-.001371,(float)6.389e-4,(
	    float)-9.249e-4,(float).0017,(float).004717,(float).006909,(float)
	    .006381,(float).01054,(float).008066,(float).01182,(float).0146,(
	    float).01889,(float).02009,(float).02066,(float).02021,(float)
	    .01545,(float).01608,(float)-.003038,(float)-.002712,(float)
	    -.005737,(float)-.002577,(float)4.394e-4,(float).001178,(float)
	    .001434,(float).004994,(float).00761,(float).006208,(float).01033,
	    (float).009159,(float).01229,(float).01453,(float).01784,(float)
	    .02156,(float).02198,(float).02124,(float).0166,(float).01824,(
	    float)-.004482,(float)8.825e-4,(float)-.002474,(float).002268,(
	    float)-.00109,(float).002739,(float).0057,(float).003266,(float)
	    .00571,(float).006047,(float).008399,(float).01119,(float).01105,(
	    float).01394,(float).02071,(float).02164,(float).02199,(float)
	    .02228,(float).01776,(float).02061,(float)-.004071,(float)
	    3.683e-4,(float)-1.479e-4,(float).003891,(float)-8.206e-4,(float)
	    .001562,(float).006306,(float).003654,(float).006339,(float)
	    .007906,(float).009657,(float).01101,(float).01169,(float).01413,(
	    float).02268,(float).02229,(float).0228,(float).02362,(float)
	    .01899,(float).02291,(float)-.00159,(float)-.001942,(float)
	    -7.506e-5,(float).001116,(float)6.905e-5,(float).002546,(float)
	    .003382,(float).00761,(float).007626,(float).008396,(float).01143,
	    (float).01041,(float).01397,(float).01633,(float).02282,(float)
	    .02475,(float).02518,(float).02499,(float).02047,(float).02532,(
	    float)-.001264,(float)-.001362,(float).002579,(float).00281,(
	    float).001113,(float).002344,(float).005572,(float).007326,(float)
	    .009925,(float).01134,(float).01189,(float).01083,(float).01553,(
	    float).01773,(float).02415,(float).02657,(float).02712,(float)
	    .02684,(float).02222,(float).02811,(float)-.001784,(float)
	    8.682e-4,(float).0033,(float).002739,(float).00723,(float).004967,
	    (float).0057,(float).007803,(float).01292,(float).01415,(float)
	    .01542,(float).01326,(float).0173,(float).02035,(float).02597,(
	    float).02792,(float).02865,(float).02928,(float).02413,(float)
	    .03125,(float).001478,(float).003818,(float).004082,(float)
	    .004702,(float).00912,(float).00847,(float).007806,(float).01208,(
	    float).01354,(float).01615,(float).0161,(float).01673,(float)
	    .01899,(float).02221,(float).028,(float).03036,(float).03087,(
	    float).03124,(float).02606,(float).03468,(float).006338,(float)
	    .007277,(float).006788,(float).008594,(float).01113,(float).01109,
	    (float).0125,(float).01616,(float).01763,(float).01708,(float)
	    .01942,(float).02001,(float).02263,(float).02462,(float).03068,(
	    float).0332,(float).03344,(float).03319,(float).02818,(float)
	    .03836,(float).009648,(float).008885,(float).009169,(float).01042,
	    (float).01523,(float).01367,(float).01375,(float).0194,(float)
	    .021,(float).02139,(float).02356,(float).02191,(float).02562,(
	    float).02899,(float).03297,(float).03623,(float).03626,(float)
	    .0362,(float).03071,(float).0423,(float).01363,(float).01318,(
	    float).01302,(float).01405,(float).01948,(float).01831,(float)
	    .01729,(float).02363,(float).02619,(float).02413,(float).02721,(
	    float).02539,(float).0301,(float).03353,(float).03614,(float)
	    .03949,(float).03996,(float).03956,(float).03364,(float).04684,(
	    float).01701,(float).01908,(float).01588,(float).02009,(float)
	    .02122,(float).0234,(float).02386,(float).02678,(float).02812,(
	    float).02692,(float).03069,(float).03158,(float).03259,(float)
	    .03569,(float).0404,(float).04289,(float).04317,(float).04284,(
	    float).03675,(float).05219,(float).01945,(float).02488,(float)
	    .02172,(float).02662,(float).02494,(float).02935,(float).02992,(
	    float).02864,(float).03083,(float).03055,(float).03326,(float)
	    .037,(float).0354,(float).03918,(float).04572,(float).04617,(
	    float).04626,(float).04656,(float).04013,(float).05803,(float)
	    .02154,(float).02793,(float).02824,(float).02858,(float).02981,(
	    float).03149,(float).03261,(float).03244,(float).03404,(float)
	    .03817,(float).03684,(float).03955,(float).03981,(float).04318,(
	    float).0502,(float).05076,(float).05076,(float).05099,(float)
	    .04405,(float).06425,(float).02649,(float).03077,(float).03144,(
	    float).03317,(float).03346,(float).03429,(float).03714,(float)
	    .03814,(float).04007,(float).04198,(float).0427,(float).04345,(
	    float).04567,(float).04883,(float).05506,(float).05606,(float)
	    .05599,(float).05587,(float).04854,(float).07121,(float).02941,(
	    float).03552,(float).03614,(float).03922,(float).04134,(float)
	    .03978,(float).04285,(float).04209,(float).04421,(float).04731,(
	    float).04649,(float).04933,(float).04981,(float).05382,(float)
	    .06099,(float).06098,(float).06105,(float).06142,(float).0535,(
	    float).07906,(float).03563,(float).03997,(float).04318,(float)
	    .04341,(float).04615,(float).04564,(float).04749,(float).04724,(
	    float).05155,(float).05276,(float).05345,(float).05397,(float)
	    .05782,(float).06063,(float).06752,(float).067,(float).06718,(
	    float).06764,(float).05903,(float).08776,(float).04004,(float)
	    .04453,(float).04702,(float).04793,(float).05214,(float).05085,(
	    float).0529,(float).05458,(float).05915,(float).05833,(float)
	    .06171,(float).06058,(float).06443,(float).06961,(float).07365,(
	    float).07474,(float).07457,(float).07421,(float).06528,(float)
	    .09752,(float).04883,(float).05067,(float).0516,(float).05442,(
	    float).0589,(float).05827,(float).05921,(float).06271,(float)
	    .0647,(float).06646,(float).06764,(float).06889,(float).07186,(
	    float).0747,(float).08126,(float).08249,(float).08209,(float)
	    .08162,(float).07238,(float).1086,(float).05012,(float).05686,(
	    float).05825,(float).06102,(float).06197,(float).06404,(float)
	    .06534,(float).06451,(float).06747,(float).07209,(float).06934,(
	    float).07388,(float).07388,(float).07788,(float).08619,(float)
	    .08622,(float).08588,(float).08595,(float).07627,(float).1146,(
	    float).0558,(float).05724,(float).06118,(float).06086,(float)
	    .07025,(float).06469,(float).06603,(float).06961,(float).0748,(
	    float).07664,(float).07545,(float).07673,(float).07914,(float)
	    .08467,(float).09118,(float).09038,(float).09003,(float).0908,(
	    float).08041,(float).1208,(float).06611,(float).06685,(float)
	    .07179,(float).07101,(float).07636,(float).07351,(float).07655,(
	    float).0794,(float).08388,(float).08648,(float).08753,(float)
	    .08589,(float).09051,(float).09541,(float).101,(float).1005,(
	    float).1002,(float).1008,(float).08949,(float).1344,(float).07306,
	    (float).08308,(float).07945,(float).08426,(float).0827,(float)
	    .08817,(float).09017,(float).09111,(float).09402,(float).09355,(
	    float).0977,(float).09998,(float).1021,(float).1055,(float).1111,(
	    float).1125,(float).1122,(float).1112,(float).0998,(float).1497,(
	    float).08434,(float).09196,(float).09315,(float).09396,(float)
	    .0981,(float).09956,(float).1003,(float).1023,(float).1055,(float)
	    .1083,(float).1084,(float).1118,(float).1125,(float).118,(float)
	    .1247,(float).1253,(float).1243,(float).1239,(float).1116,(float)
	    .1671,(float).09886,(float).1046,(float).1078,(float).1087,(float)
	    .1174,(float).1115,(float).1156,(float).115,(float).1228,(float)
	    .1244,(float).1249,(float).1247,(float).1281,(float).1339,(float)
	    .1396,(float).1388,(float).1384,(float).1392,(float).1251,(float)
	    .1868,(float).1162,(float).1215,(float).1222,(float).1254,(float)
	    .1311,(float).1276,(float).1324,(float).1335,(float).1376,(float)
	    .1407,(float).1418,(float).1419,(float).1461,(float).1499,(float)
	    .1558,(float).1561,(float).1556,(float).1552,(float).1404,(float)
	    .2091,(float).132,(float).143,(float).1414,(float).1472,(float)
	    .148,(float).1492,(float).1545,(float).1527,(float).1558,(float)
	    .1572,(float).1593,(float).1632,(float).163,(float).1681,(float)
	    .1759,(float).1755,(float).1744,(float).1734,(float).158,(float)
	    .2344,(float).1552,(float).1602,(float).1642,(float).1664,(float)
	    .1729,(float).1686,(float).1743,(float).1724,(float).1806,(float)
	    .1803,(float).1824,(float).1817,(float).185,(float).1912,(float)
	    .1983,(float).1963,(float).1953,(float).196,(float).1783,(float)
	    .2628,(float).182,(float).1813,(float).1869,(float).189,(float)
	    .2052,(float).1886,(float).1961,(float).1998,(float).2092,(float)
	    .2079,(float).2086,(float).2045,(float).213,(float).2183,(float)
	    .2222,(float).2214,(float).2211,(float).2214,(float).2018,(float)
	    .295,(float).212,(float).2142,(float).216,(float).2186,(float)
	    .2238,(float).2221,(float).2268,(float).229,(float).2346,(float)
	    .2315,(float).2389,(float).2356,(float).2424,(float).2468,(float)
	    .2502,(float).2521,(float).2506,(float).2491,(float).2286,(float)
	    .3321,(float).2457,(float).249,(float).2484,(float).2529,(float)
	    .2569,(float).2572,(float).2609,(float).2647,(float).2644,(float)
	    .2659,(float).2686,(float).2719,(float).2725,(float).276,(float)
	    .2848,(float).2848,(float).2833,(float).2817,(float).2597,(float)
	    .3751,(float).2747,(float).284,(float).2934,(float).2941,(float)
	    .301,(float).2898,(float).2997,(float).2929,(float).3067,(float)
	    .3075,(float).3051,(float).3053,(float).3088,(float).3164,(float)
	    .3254,(float).3217,(float).3205,(float).321,(float).296,(float)
	    .424,(float).321,(float).3269,(float).3363,(float).3354,(float)
	    .3419,(float).3335,(float).3424,(float).3385,(float).3496,(float)
	    .3533,(float).3524,(float).3478,(float).3551,(float).3611,(float)
	    .3679,(float).3663,(float).3654,(float).3656,(float).3381,(float)
	    .4795,(float).3711,(float).3818,(float).3786,(float).3837,(float)
	    .3865,(float).3927,(float).3936,(float).3947,(float).3989,(float)
	    .3942,(float).4012,(float).4032,(float).4043,(float).4104,(float)
	    .4154,(float).4196,(float).4183,(float).4164,(float).3871,(float)
	    .5437,(float).4338,(float).4407,(float).4398,(float).4436,(float)
	    .4409,(float).4463,(float).4521,(float).4513,(float).4515,(float)
	    .4533,(float).4587,(float).46,(float).4636,(float).4662,(float)
	    .4781,(float).4784,(float).4771,(float).476,(float).4444,(float)
	    .6179,(float).4887,(float).5003,(float).5034,(float).5064,(float)
	    .507,(float).5085,(float).5162,(float).5129,(float).5236,(float)
	    .5211,(float).5242,(float).523,(float).5248,(float).5354,(float)
	    .5479,(float).5467,(float).5455,(float).5468,(float).5117,(float)
	    .7035,(float).5837,(float).5665,(float).5749,(float).5696,(float)
	    .5896,(float).58,(float).5815,(float).5945,(float).6013,(float)
	    .6011,(float).6075,(float).5951,(float).612,(float).6143,(float)
	    .6221,(float).628,(float).6285,(float).6288,(float).591,(float)
	    .8024,(float).6498,(float).6671,(float).6606,(float).6681,(float)
	    .667,(float).6761,(float).678,(float).6797,(float).6781,(float)
	    .6814,(float).6836,(float).6927,(float).6907,(float).6954,(float)
	    .7134,(float).7239,(float).7237,(float).7239,(float).6842,(float)
	    .9178,(float).7571,(float).7721,(float).7526,(float).763,(float)
	    .7513,(float).781,(float).7763,(float).7785,(float).7775,(float)
	    .774,(float).7891,(float).7986,(float).7937,(float).8001,(float)
	    .8247,(float).8331,(float).8331,(float).8362,(float).7944,(float)
	    1.052,(float).8658,(float).8667,(float).8687,(float).8767,(float)
	    .8728,(float).8779,(float).8903,(float).8884,(float).8857,(float)
	    .9063,(float).8994,(float).9034,(float).9086,(float).916,(float)
	    .9508,(float).9599,(float).9629,(float).9702,(float).9258,(float)
	    1.208,(float).9953,(float)1.003,(float).9887,(float).9972,(float)
	    .9981,(float)1.015,(float)1.014,(float)1.023,(float)1.02,(float)
	    1.027,(float)1.038,(float)1.042,(float)1.055,(float)1.059,(float)
	    1.09,(float)1.113,(float)1.119,(float)1.129,(float)1.082,(float)
	    1.388,(float)1.155,(float)1.125,(float)1.126,(float)1.128,(float)
	    1.146,(float)1.153,(float)1.144,(float)1.182,(float)1.174,(float)
	    1.181,(float)1.196,(float)1.189,(float)1.21,(float)1.222,(float)
	    1.261,(float)1.295,(float)1.303,(float)1.315,(float)1.269,(float)
	    1.6,(float)1.299,(float)1.317,(float)1.281,(float)1.305,(float)
	    1.286,(float)1.34,(float)1.331,(float)1.35,(float)1.333,(float)
	    1.344,(float)1.364,(float)1.383,(float)1.387,(float)1.399,(float)
	    1.461,(float)1.508,(float)1.517,(float)1.535,(float)1.492,(float)
	    1.85,(float)1.498,(float)1.502,(float)1.478,(float)1.499,(float)
	    1.481,(float)1.522,(float)1.529,(float)1.537,(float)1.525,(float)
	    1.559,(float)1.558,(float)1.592,(float)1.594,(float)1.608,(float)
	    1.705,(float)1.754,(float)1.768,(float)1.8,(float)1.762,(float)
	    2.143,(float)1.695,(float)1.707,(float)1.685,(float)1.699,(float)
	    1.713,(float)1.748,(float)1.73,(float)1.769,(float)1.764,(float)
	    1.783,(float)1.808,(float)1.821,(float)1.845,(float)1.873,(float)
	    1.98,(float)2.053,(float)2.075,(float)2.119,(float)2.087,(float)
	    2.485,(float)1.936,(float)1.929,(float)1.94,(float)1.942,(float)
	    1.953,(float)1.985,(float)1.979,(float)2.039,(float)2.044,(float)
	    2.059,(float)2.095,(float)2.087,(float)2.137,(float)2.176,(float)
	    2.309,(float)2.412,(float)2.443,(float)2.498,(float)2.481,(float)
	    2.886,(float)2.238,(float)2.195,(float)2.169,(float)2.207,(float)
	    2.246,(float)2.265,(float)2.261,(float)2.358,(float)2.347,(float)
	    2.359,(float)2.414,(float)2.409,(float)2.481,(float)2.521,(float)
	    2.693,(float)2.845,(float)2.882,(float)2.955,(float)2.959,(float)
	    3.361,(float)2.501,(float)2.552,(float)2.514,(float)2.569,(float)
	    2.564,(float)2.636,(float)2.622,(float)2.688,(float)2.701,(float)
	    2.709,(float)2.782,(float)2.807,(float)2.857,(float)2.926,(float)
	    3.179,(float)3.351,(float)3.4,(float)3.507,(float)3.543,(float)
	    3.923,(float)2.871,(float)2.887,(float)2.913,(float)2.944,(float)
	    2.951,(float)3.011,(float)3.01,(float)3.095,(float)3.13,(float)
	    3.165,(float)3.243,(float)3.232,(float)3.339,(float)3.434,(float)
	    3.754,(float)3.963,(float)4.037,(float)4.18,(float)4.259,(float)
	    4.587,(float)3.273,(float)3.324,(float)3.329,(float)3.385,(float)
	    3.429,(float)3.462,(float)3.489,(float)3.591,(float)3.63,(float)
	    3.684,(float)3.759,(float)3.774,(float)3.902,(float)4.024,(float)
	    4.436,(float)4.726,(float)4.821,(float)5.,(float)5.141,(float)
	    5.373,(float)3.754,(float)3.799,(float)3.85,(float)3.902,(float)
	    3.999,(float)3.973,(float)4.041,(float)4.189,(float)4.28,(float)
	    4.326,(float)4.442,(float)4.394,(float)4.603,(float)4.787,(float)
	    5.285,(float)5.662,(float)5.783,(float)6.014,(float)6.236,(float)
	    6.304,(float)4.364,(float)4.417,(float)4.472,(float)4.559,(float)
	    4.681,(float)4.675,(float)4.737,(float)4.935,(float)5.044,(float)
	    5.101,(float)5.265,(float)5.224,(float)5.486,(float)5.71,(float)
	    6.362,(float)6.828,(float)6.978,(float)7.266,(float)7.603,(float)
	    7.422,(float)5.061,(float)5.306,(float)5.298,(float)5.443,(float)
	    5.514,(float)5.664,(float)5.717,(float)5.862,(float)5.992,(float)
	    6.123,(float)6.297,(float)6.392,(float)6.591,(float)6.868,(float)
	    7.768,(float)8.299,(float)8.481,(float)8.844,(float)9.331,(float)
	    8.772,(float)6.005,(float)6.402,(float)6.475,(float)6.633,(float)
	    6.738,(float)6.881,(float)7.025,(float)7.155,(float)7.37,(float)
	    7.621,(float)7.73,(float)7.899,(float)8.117,(float)8.504,(float)
	    9.639,(float)10.23,(float)10.45,(float)10.89,(float)11.55,(float)
	    10.41,(float)7.627,(float)8.059,(float)8.152,(float)8.412,(float)
	    8.539,(float)8.72,(float)8.932,(float)9.233,(float)9.457,(float)
	    9.795,(float)9.962,(float)10.1,(float)10.46,(float)10.93,(float)
	    12.22,(float)12.89,(float)13.13,(float)13.59,(float)14.47,(float)
	    12.42,(float)10.32,(float)10.94,(float)10.98,(float)11.46,(float)
	    11.73,(float)11.96,(float)12.21,(float)12.68,(float)12.97,(float)
	    13.27,(float)13.66,(float)13.73,(float)14.26,(float)14.75,(float)
	    16.05,(float)16.74,(float)16.93,(float)17.33,(float)18.43,(float)
	    14.93,(float)15.74,(float)16.69,(float)17.01,(float)17.48,(float)
	    17.97,(float)18.17,(float)18.51,(float)19.05,(float)19.55,(float)
	    20.,(float)20.32,(float)20.39,(float)20.95,(float)21.46,(float)
	    22.28,(float)22.59,(float)22.63,(float)22.74,(float)24.01,(float)
	    18.15,(float)29.32,(float)30.95,(float)31.07,(float)31.86,(float)
	    32.11,(float)32.78,(float)33.08,(float)33.68,(float)33.93,(float)
	    34.26,(float)34.48,(float)34.57,(float)34.73,(float)34.59,(float)
	    33.38,(float)32.41,(float)31.96,(float)31.12,(float)32.34,(float)
	    22.39,(float)73.89,(float)75.54,(float)75.55,(float)75.63,(float)
	    75.17,(float)75.52,(float)75.02,(float)74.18,(float)73.06,(float)
	    72.04,(float)70.86,(float)70.78,(float)68.22,(float)65.27,(float)
	    56.02,(float)50.58,(float)48.69,(float)45.21,(float)45.58,(float)
	    27.9,(float)321.7,(float)299.8,(float)295.,(float)282.,(float)
	    271.3,(float)262.7,(float)253.3,(float)239.,(float)226.4,(float)
	    214.,(float)202.1,(float)196.2,(float)179.3,(float)159.2,(float)
	    110.4,(float)88.07,(float)81.46,(float)69.77,(float)65.89,(float)
	    33.1,(float)1844.,(float)1405.,(float)1323.,(float)1138.,(float)
	    1019.,(float)941.7,(float)842.4,(float)740.1,(float)643.6,(float)
	    560.6,(float)500.2,(float)466.7,(float)391.1,(float)309.2,(float)
	    161.9,(float)114.7,(float)102.5,(float)82.55,(float)74.9,(float)
	    34.69 };

    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer i__, j;
    real vi_m__;

/* - to vary the number of quadratures */
/*<       include "paramdef.inc" >*/
/*<       integer nquad >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change >*/
/* Attention */
/* mu2_p has to be equal to (mu_p-1)*2 */
/*<       common /num_quad/ nquad >*/
/*<       real ph,qh,uh >*/
/*<       common /sixs_aerbas/ ph(20,nqmax_p),qh(20,nqmax_p),uh(20,nqmax_p) >*/
/*<       real phr(20,nqdef_p),qhr(20,nqdef_p),uhr(20,nqdef_p) >*/
/* - to vary the number of quadratures */
/*<       real ex,sc,asy,vi >*/
/*<       common /sixs_coef/ ex(4,20),sc(4,20),asy(4,20),vi(4) >*/
/*<       real ex_m(20),sc_m(20),asy_m(20),vi_m >*/
/*<        integer i,j >*/
/* Oceanic model */
/* Parameters and refractive indices - G.A. d'Almeida, 'Atmospheric */
/* aerosols. Global climatology and radiative characteristics', 1991. */
/* Extinction coefficients are calculated in km-1 */
/* *************** MIE (asy) ****************** */
/*<    >*/
/* *************** MIE (ext&sca) ****************** */
/*<    >*/
/* ************** MIE (phase fun ph) *************** */
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
/* ************** MIE (phase fun qh) *************** */
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
/* ************** MIE (phase fun uh) *************** */
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
/*<       vi_m=4.99809027 >*/
    vi_m__ = (float)4.99809027;
/*<       vi(3)=vi_m >*/
    sixs_coef__1.vi[2] = vi_m__;
/*<       do 1 i=1,20 >*/
    for (i__ = 1; i__ <= 20; ++i__) {
/*<        asy(3,i)=asy_m(i) >*/
	sixs_coef__1.asy[(i__ << 2) - 2] = asy_m__[i__ - 1];
/*<        ex(3,i)=ex_m(i) >*/
	sixs_coef__1.ex[(i__ << 2) - 2] = ex_m__[i__ - 1];
/*<        sc(3,i)=sc_m(i) >*/
	sixs_coef__1.sc[(i__ << 2) - 2] = sc_m__[i__ - 1];
/*<       do 1 j=1,nquad >*/
	i__1 = num_quad__1.nquad;
	for (j = 1; j <= i__1; ++j) {
/*<       ph(i,j)=phr(i,j) >*/
	    sixs_aerbas__1.ph[i__ + j * 20 - 21] = phr[i__ + j * 20 - 21];
/*<       qh(i,j)=qhr(i,j) >*/
	    sixs_aerbas__1.qh[i__ + j * 20 - 21] = qhr[i__ + j * 20 - 21];
/*<       uh(i,j)=uhr(i,j) >*/
	    sixs_aerbas__1.uh[i__ + j * 20 - 21] = uhr[i__ + j * 20 - 21];
/*<     1 continue >*/
/* L1: */
	}
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* ocea_ */

#ifdef __cplusplus
	}
#endif
