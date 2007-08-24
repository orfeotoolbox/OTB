/* BBM.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

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

/*<       subroutine  bbm >*/
/* Subroutine */ int bbm_()
{
    /* Initialized data */

    static real asy_m__[20] = { (float).709,(float).689,(float).684,(float)
	    .671,(float).659,(float).651,(float).639,(float).623,(float).605,(
	    float).586,(float).57,(float).56,(float).534,(float).5,(float)
	    .455,(float).493,(float).515,(float).573,(float).619,(float).698 }
	    ;
    static real ex_m__[20] = { (float).08254323,(float).06873854,(float)
	    .06574472,(float).05858313,(float).05299549,(float).04958472,(
	    float).04491529,(float).03959285,(float).03440313,(float).029725,(
	    float).02632282,(float).02437306,(float).01990872,(float)
	    .01504618,(float).006794238,(float).004662405,(float).00420307,(
	    float).003449567,(float).003032236,(float).002203834 };
    static real sc_m__[20] = { (float).078055,(float).06486657,(float)
	    .06199898,(float).05513438,(float).04977275,(float).04650205,(
	    float).04202277,(float).03691657,(float).03194462,(float)
	    .02746937,(float).02421588,(float).02235581,(float).01810272,(
	    float).01349245,(float).005778257,(float).003859948,(float)
	    .003459487,(float).002823499,(float).002492706,(float).001881847 }
	    ;
    static real phr[1660]	/* was [20][83] */ = { (float).1683,(float)
	    .1692,(float).1705,(float).1751,(float).1805,(float).1851,(float)
	    .1939,(float).2078,(float).2255,(float).2466,(float).2671,(float)
	    .2816,(float).3231,(float).387,(float).5767,(float).6058,(float)
	    .5984,(float).5521,(float).4886,(float).3196,(float).1674,(float)
	    .1683,(float).1696,(float).1741,(float).1795,(float).1841,(float)
	    .1928,(float).2067,(float).2242,(float).2449,(float).2653,(float)
	    .2796,(float).3207,(float).3837,(float).5696,(float).5965,(float)
	    .5884,(float).5412,(float).4784,(float).3131,(float).1655,(float)
	    .167,(float).1684,(float).1731,(float).1788,(float).1834,(float)
	    .192,(float).2061,(float).2234,(float).2437,(float).2641,(float)
	    .278,(float).3184,(float).3798,(float).5558,(float).5759,(float)
	    .5652,(float).5133,(float).4506,(float).2913,(float).1613,(float)
	    .1642,(float).1659,(float).171,(float).1771,(float).1818,(float)
	    .1907,(float).2049,(float).2225,(float).243,(float).2633,(float)
	    .2772,(float).3176,(float).3785,(float).5478,(float).5625,(float)
	    .549,(float).4918,(float).4279,(float).2692,(float).1557,(float)
	    .16,(float).1619,(float).1676,(float).1739,(float).1789,(float)
	    .1882,(float).2025,(float).2205,(float).2414,(float).2617,(float)
	    .2756,(float).3163,(float).3776,(float).5453,(float).5581,(float)
	    .5435,(float).4834,(float).4175,(float).2566,(float).1499,(float)
	    .1553,(float).1573,(float).1636,(float).1703,(float).1755,(float)
	    .185,(float).1997,(float).218,(float).2392,(float).26,(float)
	    .2739,(float).3151,(float).3769,(float).5476,(float).5613,(float)
	    .5469,(float).4875,(float).42,(float).2541,(float).1445,(float)
	    .1506,(float).1529,(float).1595,(float).1666,(float).172,(float)
	    .1818,(float).197,(float).2155,(float).2371,(float).2582,(float)
	    .2725,(float).3141,(float).3768,(float).5524,(float).5691,(float)
	    .5558,(float).4981,(float).4304,(float).2587,(float).14,(float)
	    .1464,(float).1487,(float).1556,(float).1628,(float).1686,(float)
	    .1786,(float).1939,(float).2126,(float).2347,(float).2559,(float)
	    .2707,(float).3123,(float).3758,(float).5549,(float).5752,(float)
	    .5633,(float).5066,(float).4406,(float).2657,(float).1361,(float)
	    .1423,(float).1446,(float).1516,(float).1588,(float).1648,(float)
	    .1749,(float).1901,(float).2089,(float).2312,(float).2523,(float)
	    .2672,(float).3087,(float).3723,(float).5511,(float).5735,(float)
	    .5622,(float).5068,(float).4431,(float).2696,(float).1327,(float)
	    .1384,(float).1406,(float).1474,(float).1548,(float).1604,(float)
	    .1705,(float).1858,(float).2047,(float).2266,(float).2478,(float)
	    .2621,(float).3036,(float).3663,(float).5412,(float).563,(float)
	    .5514,(float).4979,(float).4362,(float).2682,(float).1299,(float)
	    .1349,(float).137,(float).1435,(float).1508,(float).1561,(float)
	    .1661,(float).1812,(float).2,(float).2215,(float).2426,(float)
	    .2563,(float).2975,(float).3589,(float).5276,(float).5468,(float)
	    .5345,(float).4819,(float).4217,(float).2616,(float).1276,(float)
	    .1318,(float).1338,(float).1399,(float).1468,(float).1523,(float)
	    .162,(float).1766,(float).195,(float).2165,(float).2368,(float)
	    .2507,(float).2906,(float).3507,(float).5126,(float).528,(float)
	    .5153,(float).4619,(float).4032,(float).2514,(float).1257,(float)
	    .1291,(float).1308,(float).1367,(float).1432,(float).1487,(float)
	    .1582,(float).1724,(float).1903,(float).2115,(float).2314,(float)
	    .2453,(float).2841,(float).3427,(float).4978,(float).5088,(float)
	    .4955,(float).4408,(float).3832,(float).2391,(float).1239,(float)
	    .1267,(float).1283,(float).1338,(float).1402,(float).1455,(float)
	    .1547,(float).1687,(float).1863,(float).207,(float).2267,(float)
	    .2402,(float).2783,(float).3354,(float).4835,(float).4906,(float)
	    .4763,(float).4205,(float).3637,(float).2264,(float).1222,(float)
	    .1247,(float).1263,(float).1315,(float).1377,(float).1427,(float)
	    .1518,(float).1653,(float).1827,(float).2032,(float).2225,(float)
	    .2356,(float).273,(float).329,(float).4703,(float).4739,(float)
	    .4585,(float).4017,(float).3456,(float).2142,(float).1207,(float)
	    .123,(float).1245,(float).1295,(float).1355,(float).1405,(float)
	    .1493,(float).1625,(float).1796,(float).1998,(float).2187,(float)
	    .2316,(float).2683,(float).323,(float).4585,(float).4587,(float)
	    .4423,(float).385,(float).3292,(float).2028,(float).1193,(float)
	    .1216,(float).123,(float).1278,(float).1338,(float).1386,(float)
	    .1472,(float).1603,(float).177,(float).1968,(float).2155,(float)
	    .2281,(float).2642,(float).3177,(float).4479,(float).445,(float)
	    .4279,(float).37,(float).3145,(float).1924,(float).118,(float)
	    .1204,(float).1218,(float).1265,(float).1324,(float).1372,(float)
	    .1456,(float).1584,(float).1748,(float).1944,(float).2127,(float)
	    .2252,(float).2605,(float).3129,(float).4384,(float).4328,(float)
	    .4153,(float).3565,(float).3014,(float).183,(float).1168,(float)
	    .1195,(float).1208,(float).1256,(float).1314,(float).1362,(float)
	    .1445,(float).157,(float).1731,(float).1925,(float).2105,(float)
	    .2228,(float).2573,(float).3088,(float).4298,(float).4217,(float)
	    .4039,(float).3446,(float).2899,(float).1748,(float).1158,(float)
	    .1188,(float).1202,(float).125,(float).1308,(float).1355,(float)
	    .1437,(float).156,(float).1719,(float).191,(float).2088,(float)
	    .2209,(float).2548,(float).3052,(float).4219,(float).4118,(float)
	    .3936,(float).334,(float).2798,(float).1677,(float).1149,(float)
	    .1183,(float).1198,(float).1247,(float).1306,(float).1353,(float)
	    .1433,(float).1555,(float).1713,(float).1901,(float).2076,(float)
	    .2195,(float).2528,(float).3023,(float).4148,(float).4028,(float)
	    .3843,(float).3247,(float).271,(float).1617,(float).1143,(float)
	    .1182,(float).1197,(float).1248,(float).1307,(float).1354,(float)
	    .1435,(float).1555,(float).1711,(float).1897,(float).207,(float)
	    .2186,(float).2514,(float).2998,(float).4084,(float).3947,(float)
	    .376,(float).3165,(float).2633,(float).1567,(float).114,(float)
	    .1184,(float).12,(float).1253,(float).1313,(float).136,(float)
	    .144,(float).1559,(float).1714,(float).1899,(float).2069,(float)
	    .2184,(float).2506,(float).298,(float).4027,(float).3874,(float)
	    .3685,(float).3092,(float).2566,(float).1525,(float).114,(float)
	    .1189,(float).1206,(float).1262,(float).1323,(float).1371,(float)
	    .1451,(float).1569,(float).1723,(float).1906,(float).2074,(float)
	    .2187,(float).2503,(float).2968,(float).3977,(float).3809,(float)
	    .3618,(float).3028,(float).2508,(float).1493,(float).1143,(float)
	    .1198,(float).1216,(float).1275,(float).1338,(float).1386,(float)
	    .1466,(float).1585,(float).1738,(float).192,(float).2085,(float)
	    .2197,(float).2508,(float).2963,(float).3934,(float).3751,(float)
	    .3559,(float).2972,(float).2458,(float).1468,(float).115,(float)
	    .1212,(float).1231,(float).1292,(float).1357,(float).1406,(float)
	    .1487,(float).1606,(float).176,(float).194,(float).2104,(float)
	    .2214,(float).252,(float).2965,(float).3898,(float).3701,(float)
	    .3507,(float).2924,(float).2417,(float).1451,(float).1161,(float)
	    .123,(float).1251,(float).1315,(float).1382,(float).1432,(float)
	    .1514,(float).1634,(float).1788,(float).1968,(float).213,(float)
	    .2238,(float).254,(float).2976,(float).3869,(float).3659,(float)
	    .3464,(float).2882,(float).2384,(float).144,(float).1177,(float)
	    .1253,(float).1276,(float).1344,(float).1413,(float).1464,(float)
	    .1548,(float).1669,(float).1824,(float).2004,(float).2164,(float)
	    .2271,(float).2568,(float).2996,(float).3847,(float).3626,(float)
	    .3428,(float).2848,(float).2359,(float).1437,(float).1197,(float)
	    .1281,(float).1306,(float).1378,(float).145,(float).1503,(float)
	    .1588,(float).1711,(float).1868,(float).2047,(float).2207,(float)
	    .2313,(float).2606,(float).3025,(float).3836,(float).36,(float)
	    .34,(float).2823,(float).2341,(float).144,(float).1224,(float)
	    .1315,(float).1342,(float).1419,(float).1494,(float).1548,(float)
	    .1636,(float).1762,(float).192,(float).21,(float).2259,(float)
	    .2365,(float).2654,(float).3065,(float).3836,(float).3581,(float)
	    .338,(float).2808,(float).2328,(float).145,(float).1256,(float)
	    .1355,(float).1384,(float).1466,(float).1545,(float).1601,(float)
	    .1692,(float).1822,(float).1982,(float).2162,(float).2323,(float)
	    .2427,(float).2714,(float).3115,(float).3847,(float).3571,(float)
	    .3369,(float).2802,(float).2323,(float).1467,(float).1295,(float)
	    .1403,(float).1434,(float).1521,(float).1605,(float).1663,(float)
	    .1758,(float).1891,(float).2054,(float).2236,(float).2397,(float)
	    .2501,(float).2786,(float).3178,(float).3868,(float).3573,(float)
	    .3368,(float).2802,(float).2328,(float).1491,(float).134,(float)
	    .1458,(float).1492,(float).1585,(float).1673,(float).1735,(float)
	    .1833,(float).1971,(float).2138,(float).2323,(float).2484,(float)
	    .2588,(float).287,(float).3256,(float).3901,(float).3588,(float)
	    .338,(float).281,(float).2342,(float).1523,(float).1392,(float)
	    .1523,(float).1558,(float).1658,(float).1751,(float).1817,(float)
	    .192,(float).2063,(float).2234,(float).2422,(float).2584,(float)
	    .2689,(float).2969,(float).3348,(float).3948,(float).3613,(float)
	    .3401,(float).2829,(float).2365,(float).1563,(float).1453,(float)
	    .1596,(float).1635,(float).1741,(float).184,(float).191,(float)
	    .2019,(float).2168,(float).2343,(float).2535,(float).2699,(float)
	    .2806,(float).3084,(float).3455,(float).401,(float).365,(float)
	    .3434,(float).2859,(float).2397,(float).1612,(float).1523,(float)
	    .168,(float).1722,(float).1836,(float).1942,(float).2016,(float)
	    .2131,(float).2287,(float).2468,(float).2665,(float).2832,(float)
	    .2939,(float).3217,(float).3581,(float).4088,(float).3699,(float)
	    .3479,(float).29,(float).2439,(float).167,(float).1604,(float)
	    .1776,(float).1821,(float).1944,(float).2058,(float).2137,(float)
	    .2258,(float).2421,(float).261,(float).2812,(float).2982,(float)
	    .309,(float).3369,(float).3727,(float).4182,(float).3764,(float)
	    .3538,(float).2952,(float).2491,(float).1736,(float).1696,(float)
	    .1884,(float).1933,(float).2067,(float).2188,(float).2273,(float)
	    .2403,(float).2574,(float).277,(float).2979,(float).3152,(float)
	    .3262,(float).354,(float).3892,(float).4293,(float).3844,(float)
	    .3613,(float).3016,(float).2555,(float).1813,(float).1801,(float)
	    .2007,(float).2061,(float).2206,(float).2336,(float).2428,(float)
	    .2565,(float).2745,(float).2951,(float).3167,(float).3343,(float)
	    .3456,(float).3734,(float).408,(float).4424,(float).3941,(float)
	    .3703,(float).3094,(float).2632,(float).1901,(float).1921,(float)
	    .2146,(float).2205,(float).2363,(float).2504,(float).2601,(float)
	    .2748,(float).2939,(float).3154,(float).3378,(float).3559,(float)
	    .3673,(float).3955,(float).4294,(float).4574,(float).4056,(float)
	    .3805,(float).3188,(float).2724,(float).2004,(float).2057,(float)
	    .2303,(float).2368,(float).254,(float).2694,(float).2797,(float)
	    .2954,(float).3157,(float).3384,(float).3615,(float).3803,(float)
	    .3916,(float).4202,(float).4534,(float).4746,(float).4188,(float)
	    .3924,(float).3297,(float).283,(float).2122,(float).2132,(float)
	    .239,(float).2457,(float).2637,(float).2797,(float).2905,(float)
	    .3066,(float).3275,(float).3508,(float).3745,(float).3934,(float)
	    .4049,(float).4335,(float).4664,(float).4841,(float).4262,(float)
	    .3992,(float).3357,(float).2889,(float).2185,(float).2212,(float)
	    .2482,(float).2552,(float).2741,(float).2907,(float).3018,(float)
	    .3186,(float).3401,(float).364,(float).3881,(float).4074,(float)
	    .4189,(float).4476,(float).4801,(float).4941,(float).434,(float)
	    .4066,(float).3422,(float).295,(float).2253,(float).2388,(float)
	    .2684,(float).2761,(float).2967,(float).3148,(float).3269,(float)
	    .3448,(float).3675,(float).3927,(float).4178,(float).4375,(float)
	    .4495,(float).4782,(float).5098,(float).5163,(float).4511,(float)
	    .4229,(float).3563,(float).3086,(float).24,(float).2588,(float)
	    .2914,(float).2998,(float).3223,(float).3419,(float).3549,(float)
	    .3741,(float).3984,(float).4248,(float).451,(float).4713,(float)
	    .4835,(float).5122,(float).5428,(float).541,(float).4703,(float)
	    .441,(float).3726,(float).3241,(float).2568,(float).2814,(float)
	    .3175,(float).3268,(float).3512,(float).3726,(float).3865,(float)
	    .4071,(float).4329,(float).4608,(float).4879,(float).5089,(float)
	    .5212,(float).5501,(float).5794,(float).5683,(float).4919,(float)
	    .461,(float).3911,(float).3419,(float).276,(float).3072,(float)
	    .3472,(float).3573,(float).3839,(float).4071,(float).4221,(float)
	    .4442,(float).4716,(float).5008,(float).5291,(float).5506,(float)
	    .563,(float).5919,(float).6197,(float).5983,(float).5161,(float)
	    .4836,(float).4116,(float).362,(float).2977,(float).3366,(float)
	    .3809,(float).392,(float).4208,(float).446,(float).4622,(float)
	    .4858,(float).5149,(float).5455,(float).5747,(float).5968,(float)
	    .6093,(float).6379,(float).664,(float).6314,(float).5429,(float)
	    .5087,(float).4347,(float).3846,(float).3221,(float).3701,(float)
	    .4193,(float).4314,(float).4626,(float).4898,(float).5073,(float)
	    .5325,(float).5634,(float).5954,(float).6254,(float).648,(float)
	    .6605,(float).6887,(float).7126,(float).6677,(float).5723,(float)
	    .5365,(float).4604,(float).4097,(float).3497,(float).4085,(float)
	    .4629,(float).4761,(float).51,(float).5392,(float).558,(float)
	    .585,(float).6175,(float).6507,(float).6816,(float).7044,(float)
	    .717,(float).7443,(float).7656,(float).7074,(float).6046,(float)
	    .5675,(float).4889,(float).4378,(float).3807,(float).4524,(float)
	    .5126,(float).5269,(float).5636,(float).5949,(float).6151,(float)
	    .6437,(float).6779,(float).7122,(float).7438,(float).7665,(float)
	    .7792,(float).8051,(float).8233,(float).7504,(float).6402,(float)
	    .6019,(float).5204,(float).4692,(float).4157,(float).5027,(float)
	    .569,(float).5847,(float).6243,(float).6577,(float).6792,(float)
	    .7095,(float).7452,(float).7806,(float).8125,(float).8351,(float)
	    .8474,(float).8718,(float).8862,(float).7968,(float).6793,(float)
	    .6393,(float).5558,(float).5047,(float).4556,(float).5606,(float)
	    .6333,(float).6503,(float).693,(float).7286,(float).7512,(float)
	    .783,(float).8202,(float).8564,(float).8882,(float).9104,(float)
	    .9221,(float).9446,(float).9545,(float).847,(float).7218,(float)
	    .6796,(float).5954,(float).5445,(float).5011,(float).627,(float)
	    .7064,(float).7249,(float).7707,(float).8084,(float).832,(float)
	    .8652,(float).9037,(float).9403,(float).9715,(float).993,(float)
	    1.004,(float)1.024,(float)1.028,(float).9011,(float).7679,(float)
	    .7239,(float).6394,(float).5889,(float).5527,(float).7033,(float)
	    .7897,(float).8096,(float).8585,(float).8981,(float).9227,(float)
	    .957,(float).9962,(float)1.033,(float)1.063,(float)1.083,(float)
	    1.093,(float)1.109,(float)1.108,(float).9592,(float).8182,(float)
	    .7732,(float).6878,(float).6385,(float).6111,(float).7911,(float)
	    .8845,(float).9058,(float).9578,(float).999,(float)1.024,(float)
	    1.059,(float)1.099,(float)1.135,(float)1.164,(float)1.182,(float)
	    1.19,(float)1.202,(float)1.193,(float)1.022,(float).8729,(float)
	    .8273,(float).7415,(float).6944,(float).6777,(float).8919,(float)
	    .9924,(float)1.015,(float)1.07,(float)1.112,(float)1.138,(float)
	    1.173,(float)1.212,(float)1.247,(float)1.273,(float)1.289,(float)
	    1.296,(float)1.302,(float)1.284,(float)1.088,(float).9325,(float)
	    .8861,(float).8014,(float).7574,(float).7537,(float)1.008,(float)
	    1.115,(float)1.139,(float)1.196,(float)1.239,(float)1.265,(float)
	    1.3,(float)1.337,(float)1.37,(float)1.393,(float)1.405,(float)
	    1.41,(float)1.41,(float)1.382,(float)1.159,(float).9972,(float)
	    .9507,(float).8682,(float).8286,(float).8407,(float)1.141,(float)
	    1.255,(float)1.279,(float)1.338,(float)1.381,(float)1.406,(float)
	    1.44,(float)1.475,(float)1.505,(float)1.523,(float)1.531,(float)
	    1.533,(float)1.525,(float)1.486,(float)1.235,(float)1.068,(float)
	    1.022,(float).9427,(float).909,(float).9406,(float)1.295,(float)
	    1.413,(float)1.438,(float)1.497,(float)1.539,(float)1.564,(float)
	    1.595,(float)1.627,(float)1.651,(float)1.665,(float)1.667,(float)
	    1.665,(float)1.648,(float)1.597,(float)1.315,(float)1.145,(float)
	    1.099,(float)1.026,(float)1.,(float)1.055,(float)1.471,(float)
	    1.592,(float)1.617,(float)1.675,(float)1.716,(float)1.738,(float)
	    1.766,(float)1.793,(float)1.811,(float)1.817,(float)1.813,(float)
	    1.807,(float)1.78,(float)1.714,(float)1.401,(float)1.229,(float)
	    1.185,(float)1.12,(float)1.104,(float)1.187,(float)1.672,(float)
	    1.794,(float)1.819,(float)1.874,(float)1.911,(float)1.931,(float)
	    1.954,(float)1.974,(float)1.984,(float)1.981,(float)1.97,(float)
	    1.959,(float)1.919,(float)1.837,(float)1.493,(float)1.321,(float)
	    1.28,(float)1.226,(float)1.221,(float)1.338,(float)1.903,(float)
	    2.022,(float)2.045,(float)2.095,(float)2.127,(float)2.143,(float)
	    2.159,(float)2.17,(float)2.171,(float)2.157,(float)2.137,(float)
	    2.121,(float)2.066,(float)1.966,(float)1.591,(float)1.421,(float)
	    1.385,(float)1.346,(float)1.354,(float)1.513,(float)2.166,(float)
	    2.278,(float)2.298,(float)2.341,(float)2.365,(float)2.376,(float)
	    2.384,(float)2.383,(float)2.371,(float)2.345,(float)2.314,(float)
	    2.292,(float)2.221,(float)2.101,(float)1.695,(float)1.532,(float)
	    1.503,(float)1.481,(float)1.507,(float)1.715,(float)2.465,(float)
	    2.564,(float)2.58,(float)2.612,(float)2.626,(float)2.629,(float)
	    2.627,(float)2.613,(float)2.585,(float)2.545,(float)2.502,(float)
	    2.472,(float)2.383,(float)2.242,(float)1.805,(float)1.655,(float)
	    1.634,(float)1.635,(float)1.685,(float)1.949,(float)2.803,(float)
	    2.882,(float)2.893,(float)2.909,(float)2.91,(float)2.905,(float)
	    2.889,(float)2.858,(float)2.813,(float)2.756,(float)2.699,(float)
	    2.661,(float)2.552,(float)2.389,(float)1.924,(float)1.793,(float)
	    1.781,(float)1.812,(float)1.89,(float)2.22,(float)3.184,(float)
	    3.234,(float)3.238,(float)3.234,(float)3.218,(float)3.202,(float)
	    3.17,(float)3.12,(float)3.054,(float)2.977,(float)2.905,(float)
	    2.858,(float)2.728,(float)2.541,(float)2.052,(float)1.946,(float)
	    1.948,(float)2.017,(float)2.127,(float)2.536,(float)3.61,(float)
	    3.621,(float)3.615,(float)3.587,(float)3.55,(float)3.521,(float)
	    3.47,(float)3.397,(float)3.307,(float)3.208,(float)3.119,(float)
	    3.063,(float)2.909,(float)2.697,(float)2.193,(float)2.12,(float)
	    2.141,(float)2.254,(float)2.403,(float)2.902,(float)4.084,(float)
	    4.043,(float)4.025,(float)3.966,(float)3.904,(float)3.86,(float)
	    3.787,(float)3.687,(float)3.571,(float)3.447,(float)3.34,(float)
	    3.274,(float)3.095,(float)2.858,(float)2.347,(float)2.319,(float)
	    2.363,(float)2.532,(float)2.728,(float)3.33,(float)4.606,(float)
	    4.5,(float)4.466,(float)4.371,(float)4.28,(float)4.217,(float)
	    4.119,(float)3.99,(float)3.844,(float)3.694,(float)3.568,(float)
	    3.49,(float)3.286,(float)3.026,(float)2.517,(float)2.551,(float)
	    2.622,(float)2.862,(float)3.114,(float)3.834,(float)5.175,(float)
	    4.988,(float)4.937,(float)4.798,(float)4.674,(float)4.59,(float)
	    4.464,(float)4.302,(float)4.125,(float)3.946,(float)3.8,(float)
	    3.71,(float)3.482,(float)3.198,(float)2.711,(float)2.826,(float)
	    2.93,(float)3.259,(float)3.576,(float)4.426,(float)5.789,(float)
	    5.505,(float)5.433,(float)5.244,(float)5.082,(float)4.976,(float)
	    4.819,(float)4.622,(float)4.41,(float)4.201,(float)4.036,(float)
	    3.935,(float)3.682,(float)3.378,(float)2.939,(float)3.157,(float)
	    3.306,(float)3.74,(float)4.137,(float)5.122,(float)6.442,(float)
	    6.045,(float)5.949,(float)5.705,(float)5.501,(float)5.37,(float)
	    5.18,(float)4.946,(float)4.699,(float)4.461,(float)4.274,(float)
	    4.163,(float)3.888,(float)3.57,(float)3.213,(float)3.57,(float)
	    3.776,(float)4.336,(float)4.828,(float)5.95,(float)7.125,(float)
	    6.599,(float)6.477,(float)6.174,(float)5.924,(float)5.768,(float)
	    5.543,(float)5.271,(float)4.989,(float)4.723,(float)4.517,(float)
	    4.397,(float)4.104,(float)3.781,(float)3.558,(float)4.099,(float)
	    4.376,(float)5.092,(float)5.695,(float)6.939,(float)7.826,(float)
	    7.16,(float)7.01,(float)6.644,(float)6.349,(float)6.165,(float)
	    5.906,(float)5.597,(float)5.283,(float)4.99,(float)4.771,(float)
	    4.642,(float)4.341,(float)4.027,(float)4.011,(float)4.801,(float)
	    5.166,(float)6.077,(float)6.806,(float)8.129,(float)8.531,(float)
	    7.718,(float)7.54,(float)7.109,(float)6.769,(float)6.559,(float)
	    6.268,(float)5.926,(float)5.584,(float)5.273,(float)5.044,(float)
	    4.911,(float)4.615,(float)4.334,(float)4.642,(float)5.77,(float)
	    6.249,(float)7.396,(float)8.262,(float)9.575,(float)9.226,(float)
	    8.265,(float)8.059,(float)7.568,(float)7.187,(float)6.955,(float)
	    6.637,(float)6.268,(float)5.909,(float)5.59,(float)5.363,(float)
	    5.236,(float)4.964,(float)4.757,(float)5.578,(float)7.168,(float)
	    7.796,(float)9.222,(float)10.22,(float)11.34,(float)9.899,(float)
	    8.803,(float)8.573,(float)8.033,(float)7.62,(float)7.373,(float)
	    7.039,(float)6.66,(float)6.301,(float)5.997,(float)5.792,(float)
	    5.684,(float)5.479,(float)5.416,(float)7.057,(float)9.29,(float)
	    10.11,(float)11.85,(float)12.94,(float)13.53,(float)10.57,(float)
	    9.368,(float)9.124,(float)8.557,(float)8.133,(float)7.886,(float)
	    7.558,(float)7.201,(float)6.881,(float)6.635,(float)6.492,(float)
	    6.43,(float)6.374,(float)6.585,(float)9.572,(float)12.69,(float)
	    13.75,(float)15.8,(float)16.87,(float)16.28,(float)11.34,(float)
	    10.12,(float)9.883,(float)9.343,(float)8.96,(float)8.743,(float)
	    8.471,(float)8.211,(float)8.013,(float)7.913,(float)7.926,(float)
	    7.968,(float)8.228,(float)8.967,(float)14.2,(float)18.49,(float)
	    19.79,(float)22.,(float)22.68,(float)19.69,(float)12.87,(float)
	    11.85,(float)11.67,(float)11.29,(float)11.08,(float)10.97,(float)
	    10.89,(float)10.9,(float)11.03,(float)11.3,(float)11.67,(float)
	    11.93,(float)12.88,(float)14.66,(float)23.51,(float)29.01,(float)
	    30.37,(float)32.06,(float)31.53,(float)23.41,(float)20.31,(float)
	    20.01,(float)20.02,(float)20.14,(float)20.37,(float)20.56,(float)
	    20.95,(float)21.58,(float)22.44,(float)23.5,(float)24.55,(float)
	    25.24,(float)27.42,(float)30.92,(float)43.19,(float)47.46,(float)
	    47.65,(float)45.85,(float)41.98,(float)26.26,(float)53.92,(float)
	    49.4,(float)48.63,(float)47.03,(float)46.04,(float)45.52,(float)
	    45.01,(float)44.63,(float)44.53,(float)44.72,(float)45.08,(float)
	    45.37,(float)46.54,(float)48.62,(float)55.84,(float)56.54,(float)
	    55.52,(float)51.19,(float)45.6,(float)27.04 };
    static real qhr[1660]	/* was [20][83] */ = { (float)-8.147e-23,(
	    float)-3.118e-22,(float)1.009e-23,(float)5.571e-23,(float)
	    2.773e-22,(float)2.431e-22,(float)3.84e-23,(float)-7.752e-23,(
	    float)-4.595e-22,(float)5.904e-23,(float)-1.272e-22,(float)
	    5.169e-22,(float)-6.041e-23,(float)8.122e-22,(float)-5.817e-23,(
	    float)-8.368e-22,(float)3.184e-22,(float)-2.087e-22,(float)
	    -6.358e-22,(float)-3.486e-22,(float)5.946e-4,(float)4.345e-4,(
	    float)3.044e-4,(float)3.22e-4,(float)7.046e-5,(float)2.465e-4,(
	    float)2.406e-4,(float)1.308e-4,(float)-5.562e-5,(float)1.192e-4,(
	    float)-5.941e-5,(float)2.354e-4,(float)-6.454e-6,(float)2.142e-4,(
	    float).00159,(float).002451,(float).003036,(float).003408,(float)
	    .003636,(float).003063,(float).003607,(float).002544,(float)
	    .002098,(float).001983,(float).001388,(float).001908,(float)
	    .001633,(float).001361,(float)8.731e-4,(float).001141,(float)
	    9.194e-4,(float).001988,(float).001224,(float).001986,(float)
	    .008542,(float).01217,(float).01478,(float).01699,(float).01758,(
	    float).015,(float).007821,(float).005349,(float).004729,(float)
	    .00402,(float).003189,(float).003314,(float).002793,(float)
	    .002501,(float).001769,(float).001779,(float).001652,(float)
	    .002646,(float).002081,(float).003241,(float).01513,(float).02345,
	    (float).02822,(float).03431,(float).03632,(float).03242,(float)
	    .0128,(float).008687,(float).008006,(float).006318,(float).005208,
	    (float).00443,(float).003662,(float).002873,(float).002273,(float)
	    .001776,(float).001544,(float).001517,(float).001735,(float)
	    .002991,(float).01713,(float).0316,(float).03771,(float).04812,(
	    float).05343,(float).05059,(float).01851,(float).0125,(float)
	    .01143,(float).00888,(float).007015,(float).005859,(float).004611,
	    (float).003129,(float).002001,(float).001026,(float)3.092e-4,(
	    float)-3.247e-5,(float)-2.1e-4,(float)6.496e-4,(float).01588,(
	    float).03488,(float).04233,(float).05671,(float).06552,(float)
	    .0661,(float).02445,(float).01638,(float).01476,(float).01148,(
	    float).008798,(float).007582,(float).00561,(float).003561,(float)
	    .001454,(float)-1.717e-4,(float)-.001378,(float)-.001483,(float)
	    -.002898,(float)-.002916,(float).01285,(float).03414,(float).0429,
	    (float).06062,(float).0721,(float).07719,(float).02975,(float)
	    .02011,(float).01823,(float).01409,(float).01093,(float).009083,(
	    float).006591,(float).00408,(float).001377,(float)-.001086,(float)
	    -.002623,(float)-.003376,(float)-.005197,(float)-.00628,(float)
	    .007918,(float).03068,(float).03975,(float).06023,(float).07368,(
	    float).08298,(float).03412,(float).02351,(float).02153,(float)
	    .01661,(float).01294,(float).01045,(float).007589,(float).004391,(
	    float).001316,(float)-.00185,(float)-.003932,(float)-.00551,(
	    float)-.00765,(float)-.009845,(float).001257,(float).0245,(float)
	    .03343,(float).05559,(float).07029,(float).08364,(float).0376,(
	    float).02636,(float).0241,(float).01878,(float).01446,(float)
	    .01186,(float).008546,(float).004637,(float)8.342e-4,(float)
	    -.002917,(float)-.005724,(float)-.007429,(float)-.01075,(float)
	    -.01423,(float)-.006339,(float).01602,(float).02506,(float).04753,
	    (float).06307,(float).08023,(float).03991,(float).02824,(float)
	    .02588,(float).0202,(float).01564,(float).01278,(float).008953,(
	    float).004841,(float)2.139e-4,(float)-.004419,(float)-.007617,(
	    float)-.009547,(float)-.01402,(float)-.01907,(float)-.0146,(float)
	    .006106,(float).01513,(float).03769,(float).0537,(float).07407,(
	    float).04081,(float).02929,(float).02696,(float).02102,(float)
	    .01628,(float).01308,(float).008937,(float).004425,(float)
	    -7.326e-4,(float)-.006148,(float)-.009848,(float)-.01226,(float)
	    -.01765,(float)-.02422,(float)-.02374,(float)-.004311,(float)
	    .004611,(float).02687,(float).04334,(float).06634,(float).04049,(
	    float).02957,(float).02721,(float).02125,(float).01602,(float)
	    .01288,(float).008554,(float).003274,(float)-.002437,(float)
	    -.008229,(float)-.01289,(float)-.01545,(float)-.02212,(float)
	    -.03002,(float)-.0336,(float)-.01489,(float)-.005905,(float)
	    .01572,(float).0329,(float).05814,(float).03916,(float).02883,(
	    float).0265,(float).02055,(float).01516,(float).01194,(float)
	    .007367,(float).001732,(float)-.004614,(float)-.01107,(float)
	    -.01635,(float)-.01921,(float)-.02705,(float)-.03654,(float)
	    -.04386,(float)-.02598,(float)-.01695,(float).005064,(float)
	    .02278,(float).05015,(float).03692,(float).02726,(float).02509,(
	    float).01909,(float).01378,(float).01026,(float).005483,(float)
	    -3.23e-4,(float)-.007176,(float)-.01448,(float)-.02016,(float)
	    -.02365,(float)-.03236,(float)-.04356,(float)-.05467,(float)
	    -.03733,(float)-.02829,(float)-.005211,(float).0131,(float).04263,
	    (float).03406,(float).02517,(float).02308,(float).01715,(float)
	    .01173,(float).008159,(float).003214,(float)-.002992,(float)
	    -.01038,(float)-.01832,(float)-.02464,(float)-.02852,(float)
	    -.03837,(float)-.0512,(float)-.06603,(float)-.0487,(float)-.03936,
	    (float)-.01539,(float).003942,(float).0357,(float).03081,(float)
	    .02259,(float).02055,(float).0147,(float).009185,(float).005607,(
	    float)4.783e-4,(float)-.006093,(float)-.01407,(float)-.02267,(
	    float)-.02962,(float)-.03383,(float)-.04492,(float)-.05944,(float)
	    -.07775,(float)-.06023,(float)-.05042,(float)-.02524,(float)
	    -.004736,(float).02941,(float).02727,(float).01966,(float).01768,(
	    float).01186,(float).006271,(float).002638,(float)-.002668,(float)
	    -.009624,(float)-.01815,(float)-.02742,(float)-.03499,(float)
	    -.03961,(float)-.05189,(float)-.06812,(float)-.08984,(float)
	    -.07193,(float)-.06161,(float)-.03472,(float)-.01301,(float)
	    .02374,(float).02361,(float).01649,(float).01456,(float).008715,(
	    float).003047,(float)-6.83e-4,(float)-.006167,(float)-.01351,(
	    float)-.02259,(float)-.03257,(float)-.04076,(float)-.04585,(float)
	    -.05929,(float)-.07724,(float)-.1023,(float)-.08376,(float)
	    -.07293,(float)-.04388,(float)-.02087,(float).01866,(float).01993,
	    (float).01317,(float).01126,(float).005332,(float)-4.254e-4,(
	    float)-.004284,(float)-.009972,(float)-.01771,(float)-.02738,(
	    float)-.03809,(float)-.04689,(float)-.05249,(float)-.0671,(float)
	    -.08682,(float)-.115,(float)-.0957,(float)-.08427,(float)-.05283,(
	    float)-.02836,(float).01405,(float).01636,(float).009745,(float)
	    .007818,(float).001756,(float)-.004089,(float)-.008098,(float)
	    -.01405,(float)-.02218,(float)-.0325,(float)-.04399,(float)
	    -.05339,(float)-.05949,(float)-.07532,(float)-.09686,(float)
	    -.1281,(float)-.1077,(float)-.09552,(float)-.06171,(float)-.03556,
	    (float).009811,(float).01291,(float).006321,(float).00434,(float)
	    -.001907,(float)-.00792,(float)-.01207,(float)-.01831,(float)
	    -.02695,(float)-.03794,(float)-.05018,(float)-.06028,(float)
	    -.06683,(float)-.08398,(float)-.1073,(float)-.1415,(float)-.1197,(
	    float)-.1066,(float)-.07056,(float)-.0425,(float).005898,(float)
	    .009661,(float).002919,(float)8.244e-4,(float)-.005641,(float)
	    -.01189,(float)-.01616,(float)-.02274,(float)-.03194,(float)
	    -.04368,(float)-.05669,(float)-.06753,(float)-.07448,(float)
	    -.09306,(float)-.1182,(float)-.1551,(float)-.1318,(float)-.1176,(
	    float)-.07935,(float)-.04924,(float).002301,(float).006585,(float)
	    -4.226e-4,(float)-.002641,(float)-.009394,(float)-.01592,(float)
	    -.02037,(float)-.02733,(float)-.0372,(float)-.04966,(float)
	    -.06344,(float)-.07508,(float)-.08249,(float)-.1025,(float)-.1293,
	    (float)-.1689,(float)-.1438,(float)-.1286,(float)-.08798,(float)
	    -.05578,(float)-9.949e-4,(float).00373,(float)-.00371,(float)
	    -.006093,(float)-.01318,(float)-.02003,(float)-.02466,(float)
	    -.03207,(float)-.04261,(float)-.05588,(float)-.07049,(float)
	    -.08294,(float)-.09079,(float)-.1122,(float)-.1409,(float)-.1828,(
	    float)-.156,(float)-.1396,(float)-.09634,(float)-.06216,(float)
	    -.004003,(float).001045,(float)-.006925,(float)-.009479,(float)
	    -.01697,(float)-.02415,(float)-.02907,(float)-.03696,(float)
	    -.04822,(float)-.06229,(float)-.0778,(float)-.09104,(float)
	    -.09942,(float)-.1223,(float)-.1527,(float)-.1969,(float)-.1683,(
	    float)-.1506,(float)-.1044,(float)-.06839,(float)-.006763,(float)
	    -.001424,(float)-.01008,(float)-.01283,(float)-.02077,(float)
	    -.02832,(float)-.03354,(float)-.04198,(float)-.05397,(float)
	    -.06892,(float)-.08537,(float)-.09942,(float)-.1083,(float)-.1326,
	    (float)-.1649,(float)-.211,(float)-.1804,(float)-.1615,(float)
	    -.1124,(float)-.0743,(float)-.009293,(float)-.003753,(float)
	    -.01315,(float)-.01609,(float)-.02453,(float)-.03251,(float)
	    -.03811,(float)-.04713,(float)-.05992,(float)-.07572,(float)
	    -.09313,(float)-.108,(float)-.1176,(float)-.1432,(float)-.1772,(
	    float)-.2253,(float)-.1922,(float)-.1723,(float)-.1202,(float)
	    -.07982,(float)-.0116,(float)-.005895,(float)-.01618,(float)
	    -.01933,(float)-.0283,(float)-.03674,(float)-.04275,(float)
	    -.05241,(float)-.066,(float)-.08272,(float)-.1012,(float)-.1169,(
	    float)-.127,(float)-.154,(float)-.1898,(float)-.2397,(float)
	    -.2038,(float)-.1828,(float)-.1279,(float)-.08505,(float)-.01372,(
	    float)-.007905,(float)-.01911,(float)-.0225,(float)-.03202,(float)
	    -.04105,(float)-.04745,(float)-.05777,(float)-.0723,(float)
	    -.08994,(float)-.1094,(float)-.1261,(float)-.1368,(float)-.1651,(
	    float)-.2026,(float)-.254,(float)-.2152,(float)-.193,(float)
	    -.1354,(float)-.09008,(float)-.01571,(float)-.009782,(float)
	    -.02205,(float)-.02566,(float)-.03577,(float)-.0454,(float)
	    -.05227,(float)-.0633,(float)-.07871,(float)-.09734,(float)-.1178,
	    (float)-.1354,(float)-.1467,(float)-.1765,(float)-.2156,(float)
	    -.2681,(float)-.2265,(float)-.203,(float)-.1425,(float)-.09496,(
	    float)-.01757,(float)-.01153,(float)-.02493,(float)-.0288,(float)
	    -.03952,(float)-.04985,(float)-.05715,(float)-.06893,(float)
	    -.08529,(float)-.105,(float)-.1265,(float)-.1451,(float)-.1568,(
	    float)-.1881,(float)-.2288,(float)-.2819,(float)-.2375,(float)
	    -.2127,(float)-.1491,(float)-.09971,(float)-.01933,(float)-.01324,
	    (float)-.02782,(float)-.03192,(float)-.04332,(float)-.05435,(
	    float)-.06219,(float)-.07474,(float)-.09204,(float)-.1127,(float)
	    -.1354,(float)-.1549,(float)-.1672,(float)-.1998,(float)-.242,(
	    float)-.2954,(float)-.2482,(float)-.222,(float)-.1554,(float)
	    -.1042,(float)-.02098,(float)-.01486,(float)-.03071,(float)
	    -.03508,(float)-.0472,(float)-.05896,(float)-.06734,(float)
	    -.08069,(float)-.09895,(float)-.1207,(float)-.1445,(float)-.1649,(
	    float)-.1778,(float)-.2116,(float)-.2553,(float)-.3087,(float)
	    -.2584,(float)-.2308,(float)-.1613,(float)-.1083,(float)-.02245,(
	    float)-.01649,(float)-.03358,(float)-.03822,(float)-.05113,(float)
	    -.06369,(float)-.07263,(float)-.08678,(float)-.1061,(float)-.1289,
	    (float)-.1538,(float)-.175,(float)-.1885,(float)-.2236,(float)
	    -.2685,(float)-.3217,(float)-.2679,(float)-.2393,(float)-.1669,(
	    float)-.1119,(float)-.02371,(float)-.01812,(float)-.0365,(float)
	    -.04142,(float)-.05521,(float)-.06853,(float)-.07811,(float)
	    -.09307,(float)-.1133,(float)-.1372,(float)-.1632,(float)-.1853,(
	    float)-.1994,(float)-.2356,(float)-.2817,(float)-.3341,(float)
	    -.277,(float)-.2473,(float)-.1719,(float)-.1151,(float)-.02484,(
	    float)-.01973,(float)-.03945,(float)-.04473,(float)-.05943,(float)
	    -.07357,(float)-.0837,(float)-.09951,(float)-.1207,(float)-.1458,(
	    float)-.1729,(float)-.1958,(float)-.2104,(float)-.2477,(float)
	    -.2949,(float)-.3459,(float)-.2856,(float)-.2547,(float)-.1764,(
	    float)-.118,(float)-.02586,(float)-.02142,(float)-.04242,(float)
	    -.04806,(float)-.06375,(float)-.07879,(float)-.08947,(float)
	    -.1061,(float)-.1284,(float)-.1545,(float)-.1827,(float)-.2065,(
	    float)-.2215,(float)-.2598,(float)-.3079,(float)-.3572,(float)
	    -.2934,(float)-.2613,(float)-.1805,(float)-.1205,(float)-.02679,(
	    float)-.02317,(float)-.04549,(float)-.05149,(float)-.06827,(float)
	    -.08415,(float)-.09546,(float)-.1129,(float)-.1362,(float)-.1634,(
	    float)-.1927,(float)-.2171,(float)-.2327,(float)-.2718,(float)
	    -.3206,(float)-.3677,(float)-.3005,(float)-.2671,(float)-.1841,(
	    float)-.1226,(float)-.02761,(float)-.02491,(float)-.04867,(float)
	    -.05509,(float)-.07298,(float)-.08976,(float)-.1016,(float)-.1199,
	    (float)-.1442,(float)-.1725,(float)-.2028,(float)-.228,(float)
	    -.2438,(float)-.2839,(float)-.3331,(float)-.3774,(float)-.3068,(
	    float)-.2721,(float)-.1871,(float)-.1242,(float)-.02828,(float)
	    -.02672,(float)-.05193,(float)-.05882,(float)-.07782,(float)
	    -.0956,(float)-.1079,(float)-.127,(float)-.1523,(float)-.1818,(
	    float)-.213,(float)-.2388,(float)-.2548,(float)-.2958,(float)
	    -.3453,(float)-.3862,(float)-.3124,(float)-.2764,(float)-.1893,(
	    float)-.1254,(float)-.02875,(float)-.02768,(float)-.05365,(float)
	    -.06073,(float)-.08034,(float)-.09853,(float)-.1112,(float)-.1307,
	    (float)-.1565,(float)-.1865,(float)-.2181,(float)-.2442,(float)
	    -.2604,(float)-.3016,(float)-.3512,(float)-.3903,(float)-.3148,(
	    float)-.2783,(float)-.1901,(float)-.1258,(float)-.02888,(float)
	    -.02862,(float)-.05536,(float)-.06269,(float)-.08286,(float)
	    -.1016,(float)-.1144,(float)-.1344,(float)-.1607,(float)-.1912,(
	    float)-.2232,(float)-.2496,(float)-.2659,(float)-.3073,(float)
	    -.357,(float)-.3941,(float)-.3171,(float)-.2801,(float)-.1907,(
	    float)-.1261,(float)-.02893,(float)-.0306,(float)-.059,(float)
	    -.06675,(float)-.08811,(float)-.1077,(float)-.1212,(float)-.1419,(
	    float)-.1692,(float)-.2006,(float)-.2335,(float)-.2602,(float)
	    -.2769,(float)-.3186,(float)-.368,(float)-.401,(float)-.3206,(
	    float)-.283,(float)-.1915,(float)-.1261,(float)-.02891,(float)
	    -.03261,(float)-.06284,(float)-.07103,(float)-.09354,(float)
	    -.1141,(float)-.1282,(float)-.1497,(float)-.1778,(float)-.2101,(
	    float)-.2436,(float)-.2708,(float)-.2877,(float)-.3295,(float)
	    -.3785,(float)-.4069,(float)-.323,(float)-.2845,(float)-.1917,(
	    float)-.1256,(float)-.02886,(float)-.03468,(float)-.06686,(float)
	    -.07555,(float)-.09913,(float)-.1207,(float)-.1353,(float)-.1575,(
	    float)-.1866,(float)-.2196,(float)-.2537,(float)-.2813,(float)
	    -.2981,(float)-.3401,(float)-.3884,(float)-.4116,(float)-.3243,(
	    float)-.2848,(float)-.1915,(float)-.1246,(float)-.02874,(float)
	    -.03687,(float)-.07111,(float)-.08027,(float)-.1049,(float)-.1274,
	    (float)-.1425,(float)-.1656,(float)-.1955,(float)-.2291,(float)
	    -.2637,(float)-.2915,(float)-.3083,(float)-.3501,(float)-.3974,(
	    float)-.4151,(float)-.3246,(float)-.2842,(float)-.1903,(float)
	    -.123,(float)-.02838,(float)-.03921,(float)-.07559,(float)-.08518,
	    (float)-.1109,(float)-.1343,(float)-.15,(float)-.1737,(float)
	    -.2043,(float)-.2385,(float)-.2733,(float)-.3013,(float)-.3181,(
	    float)-.3594,(float)-.4054,(float)-.4173,(float)-.3237,(float)
	    -.2829,(float)-.1882,(float)-.1208,(float)-.02775,(float)-.04172,(
	    float)-.0803,(float)-.09029,(float)-.117,(float)-.1413,(float)
	    -.1576,(float)-.182,(float)-.2132,(float)-.2477,(float)-.2827,(
	    float)-.3106,(float)-.3274,(float)-.368,(float)-.4125,(float)
	    -.4181,(float)-.3218,(float)-.2807,(float)-.1851,(float)-.1182,(
	    float)-.02689,(float)-.04438,(float)-.08522,(float)-.09565,(float)
	    -.1234,(float)-.1485,(float)-.1653,(float)-.1902,(float)-.222,(
	    float)-.2567,(float)-.2917,(float)-.3194,(float)-.336,(float)
	    -.3758,(float)-.4185,(float)-.4173,(float)-.3189,(float)-.2775,(
	    float)-.1812,(float)-.115,(float)-.02574,(float)-.04723,(float)
	    -.09036,(float)-.1012,(float)-.1299,(float)-.1557,(float)-.173,(
	    float)-.1985,(float)-.2305,(float)-.2654,(float)-.3003,(float)
	    -.3276,(float)-.344,(float)-.3826,(float)-.4233,(float)-.4152,(
	    float)-.3147,(float)-.2732,(float)-.1766,(float)-.1112,(float)
	    -.02438,(float)-.05028,(float)-.09562,(float)-.1069,(float)-.1366,
	    (float)-.1629,(float)-.1807,(float)-.2066,(float)-.2389,(float)
	    -.2736,(float)-.3082,(float)-.335,(float)-.3512,(float)-.3883,(
	    float)-.4267,(float)-.4119,(float)-.3092,(float)-.2676,(float)
	    -.1716,(float)-.1068,(float)-.02294,(float)-.05349,(float)-.101,(
	    float)-.1127,(float)-.1432,(float)-.1702,(float)-.1882,(float)
	    -.2144,(float)-.247,(float)-.2814,(float)-.3153,(float)-.3416,(
	    float)-.3573,(float)-.3931,(float)-.4288,(float)-.407,(float)
	    -.3024,(float)-.2606,(float)-.1661,(float)-.1017,(float)-.02136,(
	    float)-.05685,(float)-.1065,(float)-.1186,(float)-.15,(float)
	    -.1774,(float)-.1955,(float)-.222,(float)-.2545,(float)-.2886,(
	    float)-.3218,(float)-.3473,(float)-.3624,(float)-.3966,(float)
	    -.4294,(float)-.4004,(float)-.2946,(float)-.2527,(float)-.1597,(
	    float)-.09625,(float)-.01946,(float)-.06038,(float)-.1121,(float)
	    -.1245,(float)-.1566,(float)-.1844,(float)-.2027,(float)-.2292,(
	    float)-.2615,(float)-.295,(float)-.3272,(float)-.3518,(float)
	    -.3663,(float)-.3985,(float)-.4284,(float)-.3924,(float)-.2857,(
	    float)-.2444,(float)-.1524,(float)-.09017,(float)-.01716,(float)
	    -.06401,(float)-.1176,(float)-.1304,(float)-.1632,(float)-.191,(
	    float)-.2094,(float)-.2358,(float)-.2678,(float)-.3005,(float)
	    -.3315,(float)-.355,(float)-.3689,(float)-.3989,(float)-.4257,(
	    float)-.3831,(float)-.2758,(float)-.2351,(float)-.1444,(float)
	    -.08368,(float)-.01456,(float)-.0677,(float)-.1231,(float)-.1362,(
	    float)-.1694,(float)-.1973,(float)-.2156,(float)-.2417,(float)
	    -.2732,(float)-.305,(float)-.3347,(float)-.3569,(float)-.37,(
	    float)-.3978,(float)-.4213,(float)-.3721,(float)-.2651,(float)
	    -.2249,(float)-.1358,(float)-.07706,(float)-.01179,(float)-.07138,
	    (float)-.1285,(float)-.1418,(float)-.1753,(float)-.2031,(float)
	    -.2211,(float)-.2469,(float)-.2776,(float)-.3083,(float)-.3365,(
	    float)-.3573,(float)-.3694,(float)-.3951,(float)-.4153,(float)
	    -.3595,(float)-.2534,(float)-.2138,(float)-.1266,(float)-.07029,(
	    float)-.008944,(float)-.07509,(float)-.1336,(float)-.1471,(float)
	    -.1807,(float)-.2082,(float)-.2259,(float)-.251,(float)-.2808,(
	    float)-.3102,(float)-.3367,(float)-.356,(float)-.3672,(float)
	    -.3905,(float)-.4072,(float)-.346,(float)-.2406,(float)-.2017,(
	    float)-.1175,(float)-.06295,(float)-.00603,(float)-.07871,(float)
	    -.1382,(float)-.1518,(float)-.1853,(float)-.2125,(float)-.2296,(
	    float)-.254,(float)-.2828,(float)-.3107,(float)-.3352,(float)
	    -.3531,(float)-.3632,(float)-.3841,(float)-.3973,(float)-.3313,(
	    float)-.2269,(float)-.1889,(float)-.1082,(float)-.05528,(float)
	    -.00285,(float)-.08211,(float)-.1423,(float)-.1559,(float)-.1891,(
	    float)-.2157,(float)-.2323,(float)-.2557,(float)-.2831,(float)
	    -.3095,(float)-.3321,(float)-.3483,(float)-.3573,(float)-.3757,(
	    float)-.3857,(float)-.3152,(float)-.213,(float)-.1762,(float)
	    -.09801,(float)-.04748,(float)8.254e-4,(float)-.08523,(float)
	    -.1458,(float)-.1593,(float)-.1919,(float)-.2177,(float)-.2337,(
	    float)-.256,(float)-.2819,(float)-.3064,(float)-.3271,(float)
	    -.3414,(float)-.3496,(float)-.3652,(float)-.3721,(float)-.2982,(
	    float)-.1985,(float)-.1633,(float)-.08756,(float)-.03938,(float)
	    .004851,(float)-.08791,(float)-.1484,(float)-.1617,(float)-.1936,(
	    float)-.2184,(float)-.2336,(float)-.2546,(float)-.2789,(float)
	    -.3015,(float)-.3201,(float)-.3327,(float)-.3397,(float)-.3529,(
	    float)-.3568,(float)-.2804,(float)-.1835,(float)-.1496,(float)
	    -.07742,(float)-.03115,(float).00891,(float)-.09009,(float)-.15,(
	    float)-.163,(float)-.1938,(float)-.2176,(float)-.2319,(float)
	    -.2514,(float)-.274,(float)-.2946,(float)-.311,(float)-.322,(
	    float)-.3278,(float)-.3387,(float)-.3399,(float)-.2616,(float)
	    -.1682,(float)-.1354,(float)-.06735,(float)-.02323,(float).01301,(
	    float)-.09154,(float)-.1505,(float)-.163,(float)-.1926,(float)
	    -.215,(float)-.2283,(float)-.2464,(float)-.267,(float)-.2856,(
	    float)-.3,(float)-.3092,(float)-.314,(float)-.3226,(float)-.3214,(
	    float)-.2422,(float)-.153,(float)-.1215,(float)-.05708,(float)
	    -.01557,(float).01724,(float)-.09222,(float)-.1496,(float)-.1616,(
	    float)-.1896,(float)-.2107,(float)-.2229,(float)-.2394,(float)
	    -.2581,(float)-.2746,(float)-.2867,(float)-.2945,(float)-.2983,(
	    float)-.3048,(float)-.3013,(float)-.2225,(float)-.1376,(float)
	    -.1076,(float)-.04707,(float)-.008146,(float).02144,(float)
	    -.09185,(float)-.1473,(float)-.1587,(float)-.1849,(float)-.2044,(
	    float)-.2155,(float)-.2304,(float)-.2469,(float)-.2616,(float)
	    -.2718,(float)-.278,(float)-.2804,(float)-.2856,(float)-.2805,(
	    float)-.202,(float)-.123,(float)-.094,(float)-.03691,(float)
	    -.001446,(float).0257,(float)-.09049,(float)-.1433,(float)-.154,(
	    float)-.1783,(float)-.1961,(float)-.2061,(float)-.2193,(float)
	    -.2337,(float)-.2464,(float)-.2549,(float)-.2595,(float)-.2613,(
	    float)-.2646,(float)-.2583,(float)-.1821,(float)-.1083,(float)
	    -.08107,(float)-.02738,(float).00553,(float).03033,(float)-.08802,
	    (float)-.1377,(float)-.1475,(float)-.1697,(float)-.1858,(float)
	    -.1948,(float)-.2063,(float)-.2188,(float)-.2294,(float)-.2361,(
	    float)-.2395,(float)-.2409,(float)-.2424,(float)-.2349,(float)
	    -.1631,(float)-.09318,(float)-.06851,(float)-.01892,(float).01275,
	    (float).03518,(float)-.08418,(float)-.1303,(float)-.1393,(float)
	    -.1593,(float)-.1737,(float)-.1813,(float)-.1913,(float)-.202,(
	    float)-.2109,(float)-.216,(float)-.2185,(float)-.219,(float)
	    -.2195,(float)-.2114,(float)-.144,(float)-.07926,(float)-.05633,(
	    float)-.01086,(float).01925,(float).03978,(float)-.07929,(float)
	    -.1215,(float)-.1294,(float)-.1473,(float)-.1595,(float)-.1665,(
	    float)-.175,(float)-.1835,(float)-.1905,(float)-.1948,(float)
	    -.1958,(float)-.1967,(float)-.1954,(float)-.1874,(float)-.1252,(
	    float)-.06647,(float)-.04594,(float)-.002282,(float).0251,(float)
	    .04412,(float)-.07309,(float)-.111,(float)-.1179,(float)-.1334,(
	    float)-.1442,(float)-.15,(float)-.1569,(float)-.1641,(float)
	    -.1696,(float)-.1725,(float)-.1732,(float)-.1735,(float)-.1718,(
	    float)-.1638,(float)-.1069,(float)-.05371,(float)-.03472,(float)
	    .005347,(float).03026,(float).04825,(float)-.0658,(float)-.09908,(
	    float)-.1053,(float)-.1183,(float)-.1277,(float)-.1321,(float)
	    -.1378,(float)-.1438,(float)-.1483,(float)-.1501,(float)-.1505,(
	    float)-.1498,(float)-.1486,(float)-.1411,(float)-.08852,(float)
	    -.04195,(float)-.02335,(float).01293,(float).03444,(float).05177,(
	    float)-.0578,(float)-.08654,(float)-.09168,(float)-.1027,(float)
	    -.11,(float)-.114,(float)-.1185,(float)-.1228,(float)-.1261,(
	    float)-.1277,(float)-.1272,(float)-.1269,(float)-.125,(float)
	    -.1184,(float)-.07094,(float)-.03037,(float)-.01346,(float).02113,
	    (float).03918,(float).0546,(float)-.04918,(float)-.07293,(float)
	    -.07731,(float)-.08599,(float)-.09253,(float)-.09509,(float)
	    -.09848,(float)-.1025,(float)-.1049,(float)-.1051,(float)-.1053,(
	    float)-.1043,(float)-.103,(float)-.09632,(float)-.05451,(float)
	    -.01726,(float)-.001714,(float).02784,(float).04471,(float).05699,
	    (float)-.04018,(float)-.05959,(float)-.06325,(float)-.06991,(
	    float)-.07509,(float)-.07652,(float)-.07936,(float)-.0821,(float)
	    -.08429,(float)-.08435,(float)-.08424,(float)-.08209,(float)
	    -.08201,(float)-.07641,(float)-.03645,(float)-.006639,(float)
	    .008967,(float).03647,(float).04838,(float).05823,(float)-.03185,(
	    float)-.04695,(float)-.04939,(float)-.05469,(float)-.05763,(float)
	    -.05964,(float)-.06174,(float)-.06267,(float)-.06387,(float)
	    -.06467,(float)-.06322,(float)-.06269,(float)-.06086,(float)
	    -.05653,(float)-.02061,(float).004547,(float).01804,(float).04476,
	    (float).05222,(float).05715,(float)-.02399,(float)-.0343,(float)
	    -.0362,(float)-.03963,(float)-.04234,(float)-.0432,(float)-.04421,
	    (float)-.04585,(float)-.04619,(float)-.04561,(float)-.04538,(
	    float)-.04434,(float)-.04285,(float)-.03796,(float)-.006627,(
	    float).01789,(float).03055,(float).04847,(float).05531,(float)
	    .05342,(float)-.01621,(float)-.02357,(float)-.02499,(float)
	    -.02695,(float)-.02868,(float)-.02818,(float)-.02929,(float)
	    -.02919,(float)-.03053,(float)-.02987,(float)-.02927,(float)
	    -.02571,(float)-.02673,(float)-.02227,(float).01133,(float).027,(
	    float).0396,(float).05261,(float).05415,(float).04631,(float)
	    -.01136,(float)-.01358,(float)-.01245,(float)-.0146,(float)
	    -.01415,(float)-.01719,(float)-.01589,(float)-.01669,(float)
	    -.01253,(float)-.01121,(float)-.0103,(float)-.01453,(float)
	    -.006035,(float)9.91e-4,(float).01676,(float).0422,(float).04239,(
	    float).04889,(float).05576,(float).03401,(float)-.007363,(float)
	    -.002723,(float)1.652e-4,(float)-.001422,(float)-.001035,(float)
	    -.006913,(float)-.001659,(float)-.007899,(float).0038,(float)
	    .008263,(float).006477,(float)-.00685,(float).01241,(float).02337,
	    (float).009972,(float).05411,(float).04046,(float).03218,(float)
	    .05041,(float).01763,(float).002718,(float).003655,(float).006597,
	    (float).004893,(float).005729,(float).003746,(float).005216,(
	    float).002972,(float).009025,(float).01151,(float).01014,(float)
	    .006172,(float).01335,(float).01932,(float).009494,(float).02476,(
	    float).01701,(float).009915,(float).01669,(float).003855,(float)
	    1.537e-20,(float)3.156e-20,(float)3.584e-20,(float)2.603e-20,(
	    float)1.868e-21,(float)1.868e-20,(float)3.065e-20,(float)
	    2.956e-20,(float)4.461e-21,(float)-7.992e-23,(float)6.269e-20,(
	    float)-1.118e-20,(float)-1.961e-20,(float)-4.703e-20,(float)
	    -9.754e-21,(float)-1.62e-19,(float)-8.898e-20,(float)-4.202e-20,(
	    float)3.465e-21,(float)-5.923e-20 };
    static real uhr[1660]	/* was [20][83] */ = { (float)-.1683,(float)
	    -.1692,(float)-.1705,(float)-.1751,(float)-.1805,(float)-.1851,(
	    float)-.1939,(float)-.2078,(float)-.2255,(float)-.2466,(float)
	    -.2671,(float)-.2816,(float)-.3231,(float)-.387,(float)-.5767,(
	    float)-.6058,(float)-.5984,(float)-.5521,(float)-.4886,(float)
	    -.3196,(float)-.1666,(float)-.1675,(float)-.1687,(float)-.1732,(
	    float)-.1785,(float)-.1831,(float)-.1918,(float)-.2055,(float)
	    -.2229,(float)-.2436,(float)-.2639,(float)-.2782,(float)-.3191,(
	    float)-.382,(float)-.5675,(float)-.5946,(float)-.5867,(float)
	    -.5399,(float)-.4774,(float)-.3128,(float)-.1609,(float)-.1617,(
	    float)-.1628,(float)-.167,(float)-.172,(float)-.1763,(float)
	    -.1844,(float)-.1975,(float)-.2138,(float)-.2333,(float)-.2524,(
	    float)-.2661,(float)-.3044,(float)-.3635,(float)-.5318,(float)
	    -.5508,(float)-.5407,(float)-.491,(float)-.4325,(float)-.2855,(
	    float)-.1545,(float)-.1561,(float)-.1573,(float)-.1615,(float)
	    -.1662,(float)-.1704,(float)-.1782,(float)-.1903,(float)-.2058,(
	    float)-.2244,(float)-.242,(float)-.2548,(float)-.2904,(float)
	    -.345,(float)-.49,(float)-.4965,(float)-.4824,(float)-.4261,(
	    float)-.3705,(float)-.2438,(float)-.1475,(float)-.1509,(float)
	    -.1524,(float)-.157,(float)-.1618,(float)-.1661,(float)-.174,(
	    float)-.1858,(float)-.2011,(float)-.2192,(float)-.236,(float)
	    -.2481,(float)-.2822,(float)-.3336,(float)-.4589,(float)-.452,(
	    float)-.4333,(float)-.3694,(float)-.3132,(float)-.1991,(float)
	    -.1394,(float)-.1451,(float)-.1469,(float)-.1524,(float)-.1579,(
	    float)-.1625,(float)-.1707,(float)-.1828,(float)-.1982,(float)
	    -.2163,(float)-.233,(float)-.245,(float)-.2784,(float)-.328,(
	    float)-.4414,(float)-.4228,(float)-.4002,(float)-.3292,(float)
	    -.2692,(float)-.1599,(float)-.1299,(float)-.1383,(float)-.1406,(
	    float)-.1472,(float)-.1535,(float)-.1585,(float)-.1672,(float)
	    -.18,(float)-.1958,(float)-.2141,(float)-.2312,(float)-.2432,(
	    float)-.2767,(float)-.3257,(float)-.4325,(float)-.4053,(float)
	    -.3797,(float)-.303,(float)-.2385,(float)-.1282,(float)-.1192,(
	    float)-.1303,(float)-.1332,(float)-.1409,(float)-.1481,(float)
	    -.1536,(float)-.1628,(float)-.1764,(float)-.1927,(float)-.2115,(
	    float)-.229,(float)-.2411,(float)-.2749,(float)-.3238,(float)
	    -.4271,(float)-.3942,(float)-.3663,(float)-.2853,(float)-.2168,(
	    float)-.1026,(float)-.1076,(float)-.1215,(float)-.1248,(float)
	    -.1338,(float)-.1417,(float)-.1479,(float)-.1577,(float)-.172,(
	    float)-.1888,(float)-.2082,(float)-.2261,(float)-.2386,(float)
	    -.2725,(float)-.3216,(float)-.423,(float)-.3865,(float)-.3571,(
	    float)-.2727,(float)-.2012,(float)-.08219,(float)-.0954,(float)
	    -.1119,(float)-.1159,(float)-.1258,(float)-.1349,(float)-.1413,(
	    float)-.1518,(float)-.1669,(float)-.1846,(float)-.2043,(float)
	    -.2228,(float)-.2351,(float)-.2699,(float)-.3193,(float)-.4193,(
	    float)-.3809,(float)-.3504,(float)-.2638,(float)-.1902,(float)
	    -.06633,(float)-.08296,(float)-.1019,(float)-.1064,(float)-.1174,(
	    float)-.1274,(float)-.1341,(float)-.1453,(float)-.161,(float)
	    -.1795,(float)-.1998,(float)-.2188,(float)-.231,(float)-.2664,(
	    float)-.3162,(float)-.4158,(float)-.3762,(float)-.3452,(float)
	    -.2573,(float)-.1821,(float)-.05402,(float)-.07076,(float)-.09168,
	    (float)-.09649,(float)-.1086,(float)-.1191,(float)-.1265,(float)
	    -.1384,(float)-.1545,(float)-.1735,(float)-.1946,(float)-.2136,(
	    float)-.2265,(float)-.2619,(float)-.3121,(float)-.4119,(float)
	    -.3717,(float)-.341,(float)-.252,(float)-.1759,(float)-.04447,(
	    float)-.05893,(float)-.08136,(float)-.08643,(float)-.09941,(float)
	    -.1106,(float)-.1186,(float)-.1309,(float)-.1477,(float)-.1671,(
	    float)-.1887,(float)-.2081,(float)-.2214,(float)-.2568,(float)
	    -.3073,(float)-.4072,(float)-.3671,(float)-.3365,(float)-.2473,(
	    float)-.1711,(float)-.03721,(float)-.04759,(float)-.07113,(float)
	    -.07656,(float)-.09014,(float)-.1021,(float)-.1102,(float)-.123,(
	    float)-.1405,(float)-.1605,(float)-.1825,(float)-.2023,(float)
	    -.2155,(float)-.2515,(float)-.3021,(float)-.4016,(float)-.362,(
	    float)-.3314,(float)-.2428,(float)-.1671,(float)-.03163,(float)
	    -.03696,(float)-.06131,(float)-.06697,(float)-.08106,(float)
	    -.09344,(float)-.1019,(float)-.1152,(float)-.1329,(float)-.1535,(
	    float)-.176,(float)-.196,(float)-.2092,(float)-.2455,(float)
	    -.2965,(float)-.3952,(float)-.3564,(float)-.3259,(float)-.238,(
	    float)-.1633,(float)-.02718,(float)-.02719,(float)-.05187,(float)
	    -.05761,(float)-.07211,(float)-.08482,(float)-.09357,(float)
	    -.1072,(float)-.1252,(float)-.1462,(float)-.1691,(float)-.1893,(
	    float)-.2026,(float)-.2391,(float)-.29,(float)-.3882,(float)-.35,(
	    float)-.3197,(float)-.2332,(float)-.1593,(float)-.02343,(float)
	    -.01822,(float)-.04284,(float)-.04865,(float)-.06334,(float)
	    -.0764,(float)-.08525,(float)-.09906,(float)-.1175,(float)-.1387,(
	    float)-.1618,(float)-.1823,(float)-.1956,(float)-.2323,(float)
	    -.2831,(float)-.3806,(float)-.3428,(float)-.313,(float)-.228,(
	    float)-.155,(float)-.02005,(float)-.01004,(float)-.03436,(float)
	    -.04014,(float)-.05492,(float)-.06808,(float)-.07711,(float)
	    -.09106,(float)-.1096,(float)-.131,(float)-.1544,(float)-.175,(
	    float)-.1884,(float)-.2249,(float)-.2756,(float)-.3723,(float)
	    -.335,(float)-.3059,(float)-.2221,(float)-.1504,(float)-.01687,(
	    float)-.002599,(float)-.02636,(float)-.03205,(float)-.04679,(
	    float)-.05996,(float)-.06908,(float)-.08309,(float)-.1017,(float)
	    -.1232,(float)-.1467,(float)-.1673,(float)-.1808,(float)-.2171,(
	    float)-.2676,(float)-.3632,(float)-.3266,(float)-.2981,(float)
	    -.2158,(float)-.1454,(float)-.01383,(float).004191,(float)-.01881,
	    (float)-.02439,(float)-.03893,(float)-.05207,(float)-.06115,(
	    float)-.07515,(float)-.09372,(float)-.1153,(float)-.1388,(float)
	    -.1594,(float)-.1728,(float)-.209,(float)-.2591,(float)-.3534,(
	    float)-.3174,(float)-.2895,(float)-.209,(float)-.1402,(float)
	    -.01089,(float).01039,(float)-.01167,(float)-.0171,(float)-.03135,
	    (float)-.04438,(float)-.05335,(float)-.06723,(float)-.08572,(
	    float)-.1072,(float)-.1307,(float)-.1512,(float)-.1644,(float)
	    -.2004,(float)-.25,(float)-.3428,(float)-.3075,(float)-.2802,(
	    float)-.2017,(float)-.1346,(float)-.007955,(float).01609,(float)
	    -.004909,(float)-.01016,(float)-.02403,(float)-.03685,(float)
	    -.04566,(float)-.05937,(float)-.07766,(float)-.09901,(float)
	    -.1223,(float)-.1426,(float)-.1557,(float)-.1913,(float)-.2403,(
	    float)-.3313,(float)-.2969,(float)-.2702,(float)-.1938,(float)
	    -.1285,(float)-.004932,(float).02137,(float).001533,(float)
	    -.00348,(float)-.01693,(float)-.02944,(float)-.0381,(float)
	    -.05153,(float)-.06952,(float)-.09058,(float)-.1136,(float)-.1337,
	    (float)-.1466,(float)-.1816,(float)-.2298,(float)-.319,(float)
	    -.2855,(float)-.2596,(float)-.1853,(float)-.1219,(float)-.001737,(
	    float).02633,(float).007717,(float).002951,(float)-.009997,(float)
	    -.02211,(float)-.03058,(float)-.04368,(float)-.06124,(float)
	    -.08192,(float)-.1046,(float)-.1243,(float)-.1371,(float)-.1714,(
	    float)-.2187,(float)-.3058,(float)-.2733,(float)-.2483,(float)
	    -.1761,(float)-.1149,(float).001654,(float).03106,(float).01373,(
	    float).009236,(float)-.003159,(float)-.01483,(float)-.02305,(
	    float)-.03574,(float)-.05284,(float)-.07302,(float)-.09524,(float)
	    -.1145,(float)-.127,(float)-.1606,(float)-.2068,(float)-.2917,(
	    float)-.2602,(float)-.2361,(float)-.1665,(float)-.1073,(float)
	    .005222,(float).03567,(float).01963,(float).0154,(float).003634,(
	    float)-.007576,(float)-.01545,(float)-.02767,(float)-.04421,(
	    float)-.06387,(float)-.08549,(float)-.1043,(float)-.1164,(float)
	    -.1491,(float)-.1941,(float)-.2766,(float)-.2463,(float)-.223,(
	    float)-.1563,(float)-.09929,(float).009004,(float).04023,(float)
	    .0255,(float).02154,(float).01045,(float)-2.282e-4,(float)
	    -.007721,(float)-.01942,(float)-.03533,(float)-.05434,(float)
	    -.07529,(float)-.09346,(float)-.1052,(float)-.137,(float)-.1806,(
	    float)-.2605,(float)-.2315,(float)-.2091,(float)-.1453,(float)
	    -.09078,(float).01308,(float).04484,(float).03138,(float).02771,(
	    float).01733,(float).007266,(float)1.95e-4,(float)-.01095,(float)
	    -.02609,(float)-.04439,(float)-.06458,(float)-.08205,(float)
	    -.0933,(float)-.124,(float)-.1662,(float)-.2431,(float)-.2159,(
	    float)-.1945,(float)-.1336,(float)-.08176,(float).01754,(float)
	    .04953,(float).03742,(float).03404,(float).02443,(float).015,(
	    float).008363,(float)-.002114,(float)-.01648,(float)-.03392,(
	    float)-.0532,(float)-.06991,(float)-.08071,(float)-.1102,(float)
	    -.1507,(float)-.2248,(float)-.1993,(float)-.179,(float)-.1211,(
	    float)-.07214,(float).0224,(float).05441,(float).04363,(float)
	    .04057,(float).03174,(float).02306,(float).01688,(float).00711,(
	    float)-.006367,(float)-.02284,(float)-.04114,(float)-.05697,(
	    float)-.06727,(float)-.0953,(float)-.134,(float)-.2052,(float)
	    -.1816,(float)-.1626,(float)-.1079,(float)-.0619,(float).02764,(
	    float).05956,(float).05018,(float).04741,(float).03944,(float)
	    .03148,(float).02589,(float).01689,(float).004304,(float)-.01112,(
	    float)-.02824,(float)-.04322,(float)-.05284,(float)-.07947,(float)
	    -.1162,(float)-.1844,(float)-.1629,(float)-.1451,(float)-.09412,(
	    float)-.051,(float).03327,(float).06502,(float).0571,(float)
	    .05465,(float).04756,(float).04044,(float).03542,(float).02727,(
	    float).01569,(float).001434,(float)-.01442,(float)-.02843,(float)
	    -.0374,(float)-.06242,(float)-.09703,(float)-.1624,(float)-.143,(
	    float)-.1265,(float)-.07967,(float)-.03936,(float).03941,(float)
	    .07091,(float).06449,(float).06238,(float).05622,(float).05002,(
	    float).04561,(float).03836,(float).02791,(float).01493,(float)
	    3.993e-4,(float)-.01248,(float)-.02079,(float)-.04403,(float)
	    -.07642,(float)-.139,(float)-.122,(float)-.1071,(float)-.06432,(
	    float)-.02691,(float).04619,(float).07728,(float).07247,(float)
	    .07075,(float).06557,(float).06036,(float).05655,(float).0503,(
	    float).04108,(float).02953,(float).01642,(float).004781,(float)
	    -.002916,(float)-.02415,(float)-.05423,(float)-.1141,(float)
	    -.09988,(float)-.08671,(float)-.04798,(float)-.01377,(float)
	    .05358,(float).08429,(float).08112,(float).07981,(float).0757,(
	    float).07158,(float).06843,(float).06322,(float).05538,(float)
	    .04533,(float).03373,(float).02342,(float).01647,(float)-.002749,(
	    float)-.03043,(float)-.08766,(float)-.07664,(float)-.06516,(float)
	    -.03074,(float)6.039e-5,(float).06158,(float).09207,(float).09063,
	    (float).08974,(float).08684,(float).08377,(float).08143,(float)
	    .07734,(float).07092,(float).06244,(float).05256,(float).04355,(
	    float).03753,(float).02029,(float)-.004862,(float)-.05953,(float)
	    -.05214,(float)-.04238,(float)-.01255,(float).01462,(float).07025,
	    (float).1007,(float).1011,(float).1007,(float).09909,(float)
	    .09721,(float).09566,(float).09277,(float).08791,(float).08117,(
	    float).07304,(float).06549,(float).06036,(float).04528,(float)
	    .02265,(float)-.02973,(float)-.02643,(float)-.0184,(float).006715,
	    (float).02999,(float).07974,(float).1103,(float).1126,(float)
	    .1127,(float).1126,(float).1121,(float).1113,(float).1097,(float)
	    .1066,(float).1017,(float).09537,(float).0894,(float).08519,(
	    float).07237,(float).05229,(float).001805,(float)7.463e-4,(float)
	    .00681,(float).02704,(float).04641,(float).09017,(float).1212,(
	    float).1254,(float).1261,(float).1277,(float).1285,(float).1287,(
	    float).1284,(float).1271,(float).1241,(float).1199,(float).1154,(
	    float).1122,(float).1017,(float).08431,(float).03501,(float)
	    .02965,(float).03332,(float).04837,(float).06409,(float).1017,(
	    float).1333,(float).1398,(float).1412,(float).1445,(float).1467,(
	    float).1479,(float).1491,(float).1496,(float).1489,(float).1468,(
	    float).1439,(float).1416,(float).1336,(float).1189,(float).07009,(
	    float).06005,(float).06119,(float).0709,(float).08289,(float)
	    .1144,(float).147,(float).1558,(float).1579,(float).1632,(float)
	    .1671,(float).1693,(float).172,(float).1746,(float).1761,(float)
	    .1762,(float).175,(float).1737,(float).1682,(float).1559,(float)
	    .1074,(float).09169,(float).09053,(float).09491,(float).1026,(
	    float).1283,(float).1545,(float).1646,(float).1671,(float).1733,(
	    float).1781,(float).1809,(float).1845,(float).1881,(float).1907,(
	    float).1919,(float).1916,(float).1909,(float).1864,(float).1754,(
	    float).127,(float).1081,(float).1058,(float).1074,(float).1129,(
	    float).1357,(float).1624,(float).1739,(float).1768,(float).1841,(
	    float).1898,(float).1932,(float).1975,(float).2023,(float).2061,(
	    float).2084,(float).2091,(float).2089,(float).2056,(float).1957,(
	    float).147,(float).1249,(float).1215,(float).1203,(float).1235,(
	    float).1436,(float).1798,(float).1944,(float).198,(float).2076,(
	    float).2152,(float).2198,(float).226,(float).233,(float).2392,(
	    float).2439,(float).2463,(float).2472,(float).2462,(float).2387,(
	    float).1886,(float).16,(float).1541,(float).147,(float).1461,(
	    float).1605,(float).1994,(float).2176,(float).2221,(float).234,(
	    float).2437,(float).2496,(float).2578,(float).267,(float).2758,(
	    float).2829,(float).2871,(float).2889,(float).2904,(float).2851,(
	    float).2322,(float).1971,(float).1884,(float).1751,(float).1705,(
	    float).1793,(float).2216,(float).2438,(float).2494,(float).2637,(
	    float).2756,(float).283,(float).2932,(float).3049,(float).3163,(
	    float).3258,(float).3318,(float).3346,(float).3383,(float).3348,(
	    float).2785,(float).2359,(float).2243,(float).2051,(float).1965,(
	    float).2003,(float).247,(float).2737,(float).2802,(float).2972,(
	    float).3115,(float).3203,(float).3328,(float).3471,(float).361,(
	    float).3729,(float).3807,(float).3844,(float).3902,(float).3882,(
	    float).3272,(float).2766,(float).2621,(float).2371,(float).2244,(
	    float).2236,(float).2758,(float).3075,(float).3152,(float).335,(
	    float).3518,(float).3622,(float).3769,(float).3939,(float).4104,(
	    float).4247,(float).4342,(float).4388,(float).4465,(float).4456,(
	    float).3784,(float).3194,(float).302,(float).271,(float).2545,(
	    float).2496,(float).3088,(float).3461,(float).355,(float).3777,(
	    float).3972,(float).4093,(float).4263,(float).4461,(float).4651,(
	    float).4815,(float).4928,(float).4981,(float).5074,(float).5071,(
	    float).4322,(float).3645,(float).3441,(float).307,(float).2869,(
	    float).2786,(float).3467,(float).3899,(float).4001,(float).4261,(
	    float).4482,(float).4621,(float).4816,(float).504,(float).5254,(
	    float).544,(float).5566,(float).5628,(float).5731,(float).5729,(
	    float).489,(float).4119,(float).3888,(float).3454,(float).3219,(
	    float).311,(float).3902,(float).4399,(float).4514,(float).4808,(
	    float).5057,(float).5214,(float).5433,(float).5683,(float).592,(
	    float).6125,(float).6263,(float).6333,(float).644,(float).6432,(
	    float).5486,(float).4617,(float).4361,(float).3865,(float).3598,(
	    float).3472,(float).4401,(float).4967,(float).5098,(float).5428,(
	    float).5705,(float).5879,(float).6121,(float).6398,(float).6657,(
	    float).6877,(float).7024,(float).7096,(float).7208,(float).7186,(
	    float).6108,(float).5146,(float).4857,(float).4307,(float).4014,(
	    float).3883,(float).4976,(float).5615,(float).5763,(float).613,(
	    float).6434,(float).6625,(float).689,(float).719,(float).7469,(
	    float).7701,(float).7854,(float).7926,(float).8036,(float).799,(
	    float).6761,(float).5701,(float).5378,(float).4786,(float).4468,(
	    float).4348,(float).5638,(float).6352,(float).6517,(float).6924,(
	    float).7255,(float).7461,(float).7746,(float).8069,(float).8362,(
	    float).8601,(float).8755,(float).8827,(float).8925,(float).8845,(
	    float).7449,(float).6283,(float).5931,(float).5304,(float).4963,(
	    float).4871,(float).6399,(float).7194,(float).7376,(float).7821,(
	    float).8178,(float).8398,(float).8701,(float).904,(float).9344,(
	    float).9584,(float).9734,(float).9802,(float).9879,(float).9754,(
	    float).8168,(float).6899,(float).6526,(float).5861,(float).5505,(
	    float).5462,(float).7277,(float).8153,(float).8352,(float).8834,(
	    float).9214,(float).9446,(float).9763,(float)1.011,(float)1.042,(
	    float)1.066,(float)1.08,(float)1.085,(float)1.09,(float)1.072,(
	    float).8919,(float).7556,(float).7162,(float).6462,(float).6109,(
	    float).6134,(float).8287,(float).9245,(float).9461,(float).9977,(
	    float)1.038,(float)1.062,(float)1.094,(float)1.13,(float)1.16,(
	    float)1.182,(float)1.194,(float)1.199,(float)1.2,(float)1.174,(
	    float).9705,(float).8252,(float).7841,(float).7119,(float).6778,(
	    float).6898,(float).9452,(float)1.049,(float)1.072,(float)1.126,(
	    float)1.168,(float)1.192,(float)1.225,(float)1.26,(float)1.289,(
	    float)1.309,(float)1.318,(float)1.321,(float)1.316,(float)1.282,(
	    float)1.053,(float).8988,(float).8567,(float).7842,(float).7523,(
	    float).7771,(float)1.079,(float)1.19,(float)1.214,(float)1.271,(
	    float)1.313,(float)1.338,(float)1.37,(float)1.403,(float)1.43,(
	    float)1.446,(float)1.452,(float)1.452,(float)1.44,(float)1.396,(
	    float)1.14,(float).9776,(float).9346,(float).8637,(float).8358,(
	    float).8773,(float)1.234,(float)1.351,(float)1.376,(float)1.433,(
	    float)1.475,(float)1.499,(float)1.529,(float)1.56,(float)1.583,(
	    float)1.594,(float)1.595,(float)1.592,(float)1.572,(float)1.516,(
	    float)1.23,(float)1.063,(float)1.019,(float).951,(float).9299,(
	    float).9922,(float)1.411,(float)1.532,(float)1.557,(float)1.615,(
	    float)1.655,(float)1.678,(float)1.705,(float)1.731,(float)1.748,(
	    float)1.753,(float)1.748,(float)1.741,(float)1.711,(float)1.642,(
	    float)1.324,(float)1.153,(float)1.111,(float)1.049,(float)1.035,(
	    float)1.124,(float)1.614,(float)1.737,(float)1.762,(float)1.817,(
	    float)1.855,(float)1.875,(float)1.898,(float)1.917,(float)1.927,(
	    float)1.924,(float)1.912,(float)1.9,(float)1.858,(float)1.773,(
	    float)1.424,(float)1.251,(float)1.211,(float)1.158,(float)1.154,(
	    float)1.275,(float)1.847,(float)1.968,(float)1.992,(float)2.043,(
	    float)2.075,(float)2.091,(float)2.108,(float)2.12,(float)2.119,(
	    float)2.106,(float)2.085,(float)2.068,(float)2.012,(float)1.91,(
	    float)1.529,(float)1.357,(float)1.32,(float)1.28,(float)1.289,(
	    float)1.45,(float)2.113,(float)2.228,(float)2.249,(float)2.292,(
	    float)2.318,(float)2.329,(float)2.338,(float)2.338,(float)2.325,(
	    float)2.3,(float)2.268,(float)2.246,(float)2.174,(float)2.052,(
	    float)1.639,(float)1.472,(float)1.442,(float)1.418,(float)1.443,(
	    float)1.652,(float)2.414,(float)2.518,(float)2.535,(float)2.568,(
	    float)2.583,(float)2.587,(float)2.586,(float)2.572,(float)2.545,(
	    float)2.505,(float)2.462,(float)2.432,(float)2.342,(float)2.2,(
	    float)1.755,(float)1.599,(float)1.576,(float)1.574,(float)1.621,(
	    float)1.886,(float)2.756,(float)2.84,(float)2.852,(float)2.87,(
	    float)2.872,(float)2.867,(float)2.853,(float)2.823,(float)2.779,(
	    float)2.721,(float)2.665,(float)2.626,(float)2.517,(float)2.352,(
	    float)1.878,(float)1.74,(float)1.725,(float)1.752,(float)1.827,(
	    float)2.157,(float)3.141,(float)3.196,(float)3.2,(float)3.199,(
	    float)3.185,(float)3.169,(float)3.139,(float)3.089,(float)3.024,(
	    float)2.947,(float)2.875,(float)2.828,(float)2.698,(float)2.509,(
	    float)2.01,(float)1.895,(float)1.895,(float)1.957,(float)2.064,(
	    float)2.473,(float)3.571,(float)3.587,(float)3.582,(float)3.556,(
	    float)3.521,(float)3.493,(float)3.443,(float)3.37,(float)3.281,(
	    float)3.182,(float)3.094,(float)3.038,(float)2.883,(float)2.669,(
	    float)2.154,(float)2.071,(float)2.089,(float)2.194,(float)2.34,(
	    float)2.839,(float)4.049,(float)4.014,(float)3.996,(float)3.939,(
	    float)3.879,(float)3.836,(float)3.764,(float)3.665,(float)3.549,(
	    float)3.426,(float)3.319,(float)3.252,(float)3.073,(float)2.835,(
	    float)2.31,(float)2.271,(float)2.311,(float)2.473,(float)2.665,(
	    float)3.268,(float)4.575,(float)4.475,(float)4.442,(float)4.348,(
	    float)4.259,(float)4.197,(float)4.1,(float)3.971,(float)3.826,(
	    float)3.676,(float)3.55,(float)3.472,(float)3.267,(float)3.005,(
	    float)2.482,(float)2.504,(float)2.571,(float)2.803,(float)3.05,(
	    float)3.773,(float)5.149,(float)4.967,(float)4.916,(float)4.78,(
	    float)4.657,(float)4.574,(float)4.448,(float)4.287,(float)4.11,(
	    float)3.931,(float)3.785,(float)3.695,(float)3.465,(float)3.179,(
	    float)2.677,(float)2.778,(float)2.878,(float)3.198,(float)3.512,(
	    float)4.366,(float)5.768,(float)5.488,(float)5.416,(float)5.23,(
	    float)5.068,(float)4.962,(float)4.806,(float)4.61,(float)4.398,(
	    float)4.189,(float)4.023,(float)3.921,(float)3.667,(float)3.361,(
	    float)2.904,(float)3.109,(float)3.253,(float)3.678,(float)4.072,(
	    float)5.063,(float)6.424,(float)6.031,(float)5.936,(float)5.693,(
	    float)5.49,(float)5.359,(float)5.169,(float)4.935,(float)4.689,(
	    float)4.45,(float)4.263,(float)4.151,(float)3.874,(float)3.554,(
	    float)3.178,(float)3.521,(float)3.722,(float)4.272,(float)4.761,(
	    float)5.892,(float)7.111,(float)6.589,(float)6.467,(float)6.165,(
	    float)5.916,(float)5.759,(float)5.535,(float)5.263,(float)4.981,(
	    float)4.713,(float)4.507,(float)4.386,(float)4.091,(float)3.765,(
	    float)3.521,(float)4.046,(float)4.319,(float)5.025,(float)5.626,(
	    float)6.882,(float)7.816,(float)7.152,(float)7.003,(float)6.636,(
	    float)6.342,(float)6.157,(float)5.898,(float)5.59,(float)5.275,(
	    float)4.982,(float)4.761,(float)4.631,(float)4.328,(float)4.01,(
	    float)3.972,(float)4.745,(float)5.105,(float)6.008,(float)6.735,(
	    float)8.075,(float)8.524,(float)7.712,(float)7.534,(float)7.103,(
	    float)6.763,(float)6.553,(float)6.261,(float)5.919,(float)5.577,(
	    float)5.264,(float)5.034,(float)4.9,(float)4.601,(float)4.316,(
	    float)4.601,(float)5.712,(float)6.187,(float)7.326,(float)8.192,(
	    float)9.525,(float)9.22,(float)8.26,(float)8.054,(float)7.563,(
	    float)7.181,(float)6.949,(float)6.631,(float)6.262,(float)5.901,(
	    float)5.581,(float)5.354,(float)5.225,(float)4.951,(float)4.739,(
	    float)5.536,(float)7.112,(float)7.737,(float)9.156,(float)10.16,(
	    float)11.3,(float)9.895,(float)8.799,(float)8.569,(float)8.029,(
	    float)7.615,(float)7.368,(float)7.033,(float)6.653,(float)6.294,(
	    float)5.989,(float)5.782,(float)5.673,(float)5.466,(float)5.399,(
	    float)7.019,(float)9.242,(float)10.06,(float)11.79,(float)12.89,(
	    float)13.5,(float)10.56,(float)9.365,(float)9.12,(float)8.553,(
	    float)8.129,(float)7.881,(float)7.553,(float)7.195,(float)6.874,(
	    float)6.627,(float)6.483,(float)6.42,(float)6.362,(float)6.569,(
	    float)9.541,(float)12.66,(float)13.71,(float)15.76,(float)16.84,(
	    float)16.26,(float)11.34,(float)10.12,(float)9.879,(float)9.339,(
	    float)8.956,(float)8.738,(float)8.466,(float)8.206,(float)8.007,(
	    float)7.906,(float)7.919,(float)7.96,(float)8.219,(float)8.955,(
	    float)14.18,(float)18.47,(float)19.76,(float)21.98,(float)22.66,(
	    float)19.69,(float)12.87,(float)11.85,(float)11.66,(float)11.29,(
	    float)11.07,(float)10.97,(float)10.88,(float)10.89,(float)11.03,(
	    float)11.3,(float)11.66,(float)11.93,(float)12.87,(float)14.66,(
	    float)23.5,(float)29.,(float)30.36,(float)32.05,(float)31.52,(
	    float)23.4,(float)20.31,(float)20.01,(float)20.02,(float)20.13,(
	    float)20.36,(float)20.56,(float)20.95,(float)21.57,(float)22.44,(
	    float)23.5,(float)24.55,(float)25.24,(float)27.42,(float)30.92,(
	    float)43.19,(float)47.46,(float)47.65,(float)45.85,(float)41.98,(
	    float)26.26,(float)53.92,(float)49.4,(float)48.63,(float)47.03,(
	    float)46.04,(float)45.52,(float)45.01,(float)44.63,(float)44.53,(
	    float)44.72,(float)45.08,(float)45.37,(float)46.54,(float)48.62,(
	    float)55.84,(float)56.54,(float)55.52,(float)51.19,(float)45.6,(
	    float)27.04 };

    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer i__, j;

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
/*<       real ex_m(20),sc_m(20),asy_m(20) >*/
/*<       integer i,j >*/
/* Biomass burning model */
/* Parameters and reflractive indices - AERONET measurements */
/* (see file BBM-params.dat) */
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
/*<       do 1 i=1,20 >*/
    for (i__ = 1; i__ <= 20; ++i__) {
/*<        asy(1,i)=asy_m(i) >*/
	sixs_coef__1.asy[(i__ << 2) - 4] = asy_m__[i__ - 1];
/*<        ex(1,i)=ex_m(i) >*/
	sixs_coef__1.ex[(i__ << 2) - 4] = ex_m__[i__ - 1];
/*<        sc(1,i)=sc_m(i) >*/
	sixs_coef__1.sc[(i__ << 2) - 4] = sc_m__[i__ - 1];
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
} /* bbm_ */

#ifdef __cplusplus
	}
#endif
