/* BDM.f -- translated by f2c (version 19970805).
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

/*<       subroutine   bdm >*/
/* Subroutine */ int bdm_()
{
    /* Initialized data */

    static real asy_m__[20] = { (float).701,(float).69,(float).687,(float)
	    .681,(float).677,(float).675,(float).671,(float).665,(float).66,(
	    float).655,(float).651,(float).648,(float).639,(float).626,(float)
	    .583,(float).587,(float).583,(float).595,(float).605,(float).547 }
	    ;
    static real ex_m__[20] = { (float).09978367,(float).0939049,(float)
	    .09241579,(float).08848752,(float).08496436,(float).08261336,(
	    float).07913338,(float).07475012,(float).06989562,(float)
	    .06496391,(float).06097984,(float).05852314,(float).05225345,(
	    float).04403058,(float).02414531,(float).0107873,(float)
	    .008476357,(float).003624489,(float).001813242,(float).001101573 }
	    ;
    static real sc_m__[20] = { (float).08870113,(float).08606911,(float)
	    .08536333,(float).08329929,(float).08053116,(float).07846993,(
	    float).07561909,(float).07217911,(float).06785794,(float)
	    .06316575,(float).05937214,(float).05706264,(float).05114494,(
	    float).04369425,(float).02392341,(float).01061324,(float)
	    .00830047,(float).003460222,(float).001624963,(float)6.737047e-4 }
	    ;
    static real phr[1660]	/* was [20][83] */ = { (float).3494,(float)
	    .3524,(float).3543,(float).3623,(float).3549,(float).3501,(float)
	    .3444,(float).3445,(float).3352,(float).3251,(float).3184,(float)
	    .3186,(float).3108,(float).3172,(float).3355,(float).2986,(float)
	    .304,(float).302,(float).302,(float).3991,(float).3432,(float)
	    .3465,(float).3485,(float).3565,(float).3494,(float).3448,(float)
	    .3394,(float).3396,(float).3308,(float).3211,(float).3146,(float)
	    .3148,(float).3076,(float).314,(float).3327,(float).297,(float)
	    .3027,(float).3011,(float).3015,(float).3988,(float).3196,(float)
	    .3245,(float).3269,(float).3352,(float).3296,(float).3255,(float)
	    .3213,(float).3223,(float).3154,(float).3071,(float).3017,(float)
	    .3016,(float).2963,(float).3035,(float).3233,(float).2925,(float)
	    .2989,(float).2986,(float).3003,(float).3978,(float).2868,(float)
	    .2939,(float).2968,(float).3054,(float).3019,(float).2985,(float)
	    .2961,(float).2982,(float).2937,(float).2874,(float).2836,(float)
	    .2834,(float).2807,(float).2893,(float).3114,(float).2886,(float)
	    .2955,(float).2971,(float).2996,(float).3964,(float).2541,(float)
	    .2629,(float).2661,(float).2749,(float).2732,(float).2707,(float)
	    .2698,(float).2729,(float).2706,(float).2664,(float).2641,(float)
	    .2641,(float).2639,(float).2739,(float).2995,(float).2868,(float)
	    .2941,(float).2968,(float).299,(float).3948,(float).2281,(float)
	    .2374,(float).2406,(float).2493,(float).2488,(float).2469,(float)
	    .247,(float).251,(float).2501,(float).2475,(float).2464,(float)
	    .2469,(float).2484,(float).2593,(float).2889,(float).2865,(float)
	    .2941,(float).2964,(float).2979,(float).3929,(float).2113,(float)
	    .2201,(float).2232,(float).2313,(float).2314,(float).2298,(float)
	    .2303,(float).2345,(float).2344,(float).2326,(float).2325,(float)
	    .2333,(float).2357,(float).2471,(float).2799,(float).2862,(float)
	    .294,(float).295,(float).2962,(float).3906,(float).2032,(float)
	    .2108,(float).2135,(float).2208,(float).2207,(float).2193,(float)
	    .2198,(float).2236,(float).2238,(float).2224,(float).2226,(float)
	    .2235,(float).2264,(float).2379,(float).2725,(float).2849,(float)
	    .2929,(float).2928,(float).2942,(float).3879,(float).2008,(float)
	    .2069,(float).2092,(float).2155,(float).2151,(float).2136,(float)
	    .2138,(float).217,(float).217,(float).2157,(float).2159,(float)
	    .2168,(float).2198,(float).231,(float).2664,(float).2824,(float)
	    .2906,(float).2901,(float).2919,(float).3847,(float).2006,(float)
	    .2052,(float).2072,(float).2125,(float).2118,(float).2101,(float)
	    .21,(float).2127,(float).2124,(float).2109,(float).2111,(float)
	    .2118,(float).2147,(float).2253,(float).2611,(float).2789,(float)
	    .287,(float).287,(float).2895,(float).3812,(float).1999,(float)
	    .2036,(float).2053,(float).2098,(float).2089,(float).2071,(float)
	    .2067,(float).2091,(float).2085,(float).2069,(float).207,(float)
	    .2076,(float).2103,(float).2203,(float).256,(float).2746,(float)
	    .2828,(float).2837,(float).2869,(float).3773,(float).1976,(float)
	    .2008,(float).2023,(float).2063,(float).2053,(float).2036,(float)
	    .2031,(float).2051,(float).2045,(float).203,(float).203,(float)
	    .2036,(float).2061,(float).2156,(float).2509,(float).2696,(float)
	    .2783,(float).2804,(float).2842,(float).3732,(float).1935,(float)
	    .1966,(float).1979,(float).2016,(float).2006,(float).1992,(float)
	    .1988,(float).2005,(float).2001,(float).1989,(float).1989,(float)
	    .1995,(float).202,(float).211,(float).2459,(float).2644,(float)
	    .2735,(float).2771,(float).2814,(float).3689,(float).1877,(float)
	    .191,(float).1923,(float).1959,(float).1952,(float).1941,(float)
	    .1938,(float).1955,(float).1952,(float).1944,(float).1945,(float)
	    .1952,(float).1978,(float).2065,(float).2412,(float).2592,(float)
	    .2686,(float).2737,(float).2785,(float).3645,(float).1811,(float)
	    .1847,(float).186,(float).1895,(float).1892,(float).1883,(float)
	    .1883,(float).1901,(float).1902,(float).1897,(float).1901,(float)
	    .1909,(float).1936,(float).2022,(float).2367,(float).2541,(float)
	    .2637,(float).2704,(float).2757,(float).3599,(float).174,(float)
	    .178,(float).1794,(float).1829,(float).1829,(float).1823,(float)
	    .1826,(float).1846,(float).1851,(float).185,(float).1856,(float)
	    .1865,(float).1895,(float).1981,(float).2326,(float).2492,(float)
	    .2588,(float).2671,(float).2728,(float).3552,(float).167,(float)
	    .1714,(float).1728,(float).1763,(float).1767,(float).1764,(float)
	    .177,(float).1791,(float).18,(float).1804,(float).1813,(float)
	    .1823,(float).1856,(float).1942,(float).2287,(float).2446,(float)
	    .2543,(float).2638,(float).2699,(float).3504,(float).1603,(float)
	    .1651,(float).1665,(float).1701,(float).1709,(float).1708,(float)
	    .1717,(float).174,(float).1752,(float).176,(float).1772,(float)
	    .1783,(float).182,(float).1907,(float).2253,(float).2403,(float)
	    .2501,(float).2605,(float).267,(float).3457,(float).1543,(float)
	    .1594,(float).1608,(float).1645,(float).1655,(float).1657,(float)
	    .1668,(float).1692,(float).1708,(float).172,(float).1735,(float)
	    .1747,(float).1786,(float).1875,(float).2223,(float).2364,(float)
	    .2461,(float).2574,(float).2643,(float).3409,(float).149,(float)
	    .1543,(float).1558,(float).1595,(float).1607,(float).1611,(float)
	    .1625,(float).165,(float).1669,(float).1685,(float).1701,(float)
	    .1715,(float).1757,(float).1847,(float).2197,(float).2329,(float)
	    .2426,(float).2543,(float).2615,(float).3363,(float).1445,(float)
	    .1499,(float).1514,(float).1552,(float).1566,(float).1572,(float)
	    .1587,(float).1613,(float).1636,(float).1655,(float).1673,(float)
	    .1688,(float).1733,(float).1824,(float).2176,(float).2299,(float)
	    .2394,(float).2515,(float).259,(float).3317,(float).1407,(float)
	    .1463,(float).1478,(float).1516,(float).1531,(float).1539,(float)
	    .1557,(float).1584,(float).1608,(float).163,(float).1651,(float)
	    .1666,(float).1714,(float).1806,(float).216,(float).2274,(float)
	    .2367,(float).2488,(float).2565,(float).3274,(float).1378,(float)
	    .1435,(float).145,(float).1488,(float).1504,(float).1513,(float)
	    .1532,(float).156,(float).1587,(float).1611,(float).1633,(float)
	    .165,(float).17,(float).1793,(float).215,(float).2253,(float)
	    .2344,(float).2464,(float).2542,(float).3232,(float).1357,(float)
	    .1414,(float).1428,(float).1466,(float).1484,(float).1495,(float)
	    .1515,(float).1544,(float).1572,(float).1599,(float).1622,(float)
	    .164,(float).1692,(float).1786,(float).2145,(float).2238,(float)
	    .2326,(float).2444,(float).2522,(float).3193,(float).1343,(float)
	    .14,(float).1414,(float).1452,(float).1472,(float).1483,(float)
	    .1504,(float).1535,(float).1564,(float).1592,(float).1618,(float)
	    .1637,(float).169,(float).1785,(float).2147,(float).2229,(float)
	    .2315,(float).2427,(float).2504,(float).3157,(float).1336,(float)
	    .1393,(float).1408,(float).1445,(float).1467,(float).1478,(float)
	    .15,(float).1533,(float).1563,(float).1592,(float).162,(float)
	    .1639,(float).1694,(float).1791,(float).2156,(float).2226,(float)
	    .2308,(float).2414,(float).249,(float).3125,(float).1336,(float)
	    .1394,(float).1409,(float).1446,(float).1469,(float).1481,(float)
	    .1503,(float).1537,(float).1569,(float).16,(float).1629,(float)
	    .1649,(float).1705,(float).1804,(float).2171,(float).2229,(float)
	    .2309,(float).2407,(float).2479,(float).3098,(float).1344,(float)
	    .1402,(float).1417,(float).1455,(float).1478,(float).149,(float)
	    .1513,(float).1549,(float).1582,(float).1614,(float).1645,(float)
	    .1665,(float).1724,(float).1824,(float).2193,(float).2239,(float)
	    .2316,(float).2405,(float).2473,(float).3076,(float).1358,(float)
	    .1417,(float).1432,(float).147,(float).1495,(float).1507,(float)
	    .1531,(float).1567,(float).1602,(float).1636,(float).1668,(float)
	    .1688,(float).1749,(float).1851,(float).2224,(float).2258,(float)
	    .2331,(float).2409,(float).2472,(float).3059,(float).138,(float)
	    .144,(float).1455,(float).1494,(float).1519,(float).1532,(float)
	    .1557,(float).1594,(float).163,(float).1665,(float).1698,(float)
	    .172,(float).1783,(float).1886,(float).2264,(float).2284,(float)
	    .2354,(float).242,(float).2477,(float).3049,(float).141,(float)
	    .147,(float).1485,(float).1524,(float).155,(float).1565,(float)
	    .1591,(float).1628,(float).1665,(float).1703,(float).1737,(float)
	    .176,(float).1824,(float).193,(float).2313,(float).232,(float)
	    .2386,(float).2439,(float).2488,(float).3047,(float).1446,(float)
	    .1507,(float).1523,(float).1563,(float).159,(float).1606,(float)
	    .1632,(float).1671,(float).1709,(float).1749,(float).1784,(float)
	    .1808,(float).1874,(float).1982,(float).2373,(float).2365,(float)
	    .2427,(float).2466,(float).2507,(float).3053,(float).1491,(float)
	    .1554,(float).1569,(float).1611,(float).1638,(float).1655,(float)
	    .1683,(float).1722,(float).1762,(float).1804,(float).184,(float)
	    .1866,(float).1934,(float).2045,(float).2444,(float).2421,(float)
	    .2479,(float).2503,(float).2534,(float).3067,(float).1543,(float)
	    .1608,(float).1624,(float).1667,(float).1695,(float).1713,(float)
	    .1742,(float).1783,(float).1825,(float).1868,(float).1907,(float)
	    .1934,(float).2005,(float).2119,(float).2526,(float).2488,(float)
	    .2543,(float).2549,(float).257,(float).3092,(float).1605,(float)
	    .1672,(float).1688,(float).1732,(float).1762,(float).1781,(float)
	    .1811,(float).1854,(float).1898,(float).1944,(float).1984,(float)
	    .2012,(float).2086,(float).2205,(float).2622,(float).2568,(float)
	    .2619,(float).2607,(float).2616,(float).3128,(float).1676,(float)
	    .1745,(float).1762,(float).1808,(float).1839,(float).1859,(float)
	    .1891,(float).1937,(float).1982,(float).203,(float).2073,(float)
	    .2102,(float).218,(float).2303,(float).2733,(float).2662,(float)
	    .2709,(float).2678,(float).2673,(float).3175,(float).1757,(float)
	    .1829,(float).1847,(float).1895,(float).1928,(float).1949,(float)
	    .1983,(float).203,(float).2078,(float).2129,(float).2174,(float)
	    .2205,(float).2286,(float).2414,(float).2859,(float).2772,(float)
	    .2814,(float).2762,(float).2743,(float).3236,(float).1849,(float)
	    .1925,(float).1944,(float).1994,(float).2028,(float).2052,(float)
	    .2087,(float).2137,(float).2187,(float).2242,(float).2289,(float)
	    .2323,(float).2407,(float).2542,(float).3002,(float).2898,(float)
	    .2936,(float).2861,(float).2826,(float).3312,(float).1953,(float)
	    .2034,(float).2053,(float).2106,(float).2143,(float).2168,(float)
	    .2205,(float).2258,(float).2312,(float).237,(float).242,(float)
	    .2455,(float).2545,(float).2686,(float).3164,(float).3042,(float)
	    .3077,(float).2977,(float).2925,(float).3403,(float).207,(float)
	    .2156,(float).2177,(float).2233,(float).2273,(float).2299,(float)
	    .2339,(float).2394,(float).2453,(float).2515,(float).2568,(float)
	    .2605,(float).2701,(float).2849,(float).3346,(float).3207,(float)
	    .3237,(float).3111,(float).304,(float).3511,(float).2202,(float)
	    .2294,(float).2317,(float).2376,(float).2419,(float).2446,(float)
	    .2489,(float).2549,(float).2612,(float).2678,(float).2735,(float)
	    .2773,(float).2876,(float).3032,(float).3552,(float).3395,(float)
	    .3419,(float).3266,(float).3173,(float).3638,(float).2274,(float)
	    .237,(float).2393,(float).2454,(float).2499,(float).2527,(float)
	    .2572,(float).2633,(float).2699,(float).2767,(float).2826,(float)
	    .2865,(float).2972,(float).3133,(float).3664,(float).3497,(float)
	    .3519,(float).3351,(float).3247,(float).3709,(float).2351,(float)
	    .245,(float).2474,(float).2537,(float).2583,(float).2612,(float)
	    .2659,(float).2723,(float).2791,(float).2861,(float).2923,(float)
	    .2963,(float).3073,(float).3239,(float).3782,(float).3606,(float)
	    .3626,(float).3442,(float).3327,(float).3786,(float).2518,(float)
	    .2625,(float).2651,(float).2719,(float).2768,(float).28,(float)
	    .2851,(float).2918,(float).2991,(float).3068,(float).3133,(float)
	    .3177,(float).3293,(float).3469,(float).404,(float).3844,(float)
	    .386,(float).3643,(float).3502,(float).3955,(float).2706,(float)
	    .2822,(float).285,(float).2923,(float).2975,(float).3012,(float)
	    .3066,(float).3139,(float).3216,(float).3299,(float).3369,(float)
	    .3417,(float).3541,(float).3728,(float).4329,(float).4113,(float)
	    .4123,(float).3871,(float).3702,(float).4149,(float).2918,(float)
	    .3043,(float).3073,(float).3152,(float).3209,(float).3249,(float)
	    .3307,(float).3386,(float).3469,(float).3559,(float).3635,(float)
	    .3687,(float).3819,(float).4017,(float).465,(float).4415,(float)
	    .4418,(float).4128,(float).3928,(float).4369,(float).3155,(float)
	    .3291,(float).3324,(float).341,(float).3473,(float).3515,(float)
	    .3579,(float).3665,(float).3755,(float).3852,(float).3934,(float)
	    .3989,(float).4131,(float).4343,(float).5009,(float).4752,(float)
	    .475,(float).4418,(float).4183,(float).4618,(float).3422,(float)
	    .3571,(float).3607,(float).37,(float).3769,(float).3815,(float)
	    .3884,(float).3978,(float).4076,(float).418,(float).4269,(float)
	    .4328,(float).4481,(float).4707,(float).5408,(float).5129,(float)
	    .512,(float).4743,(float).4471,(float).4899,(float).3722,(float)
	    .3886,(float).3925,(float).4027,(float).4103,(float).4152,(float)
	    .4228,(float).433,(float).4437,(float).455,(float).4646,(float)
	    .4709,(float).4874,(float).5115,(float).5851,(float).5551,(float)
	    .5534,(float).5109,(float).4794,(float).5214,(float).406,(float)
	    .4241,(float).4285,(float).4396,(float).4479,(float).4533,(float)
	    .4616,(float).4725,(float).4843,(float).4965,(float).5069,(float)
	    .5136,(float).5314,(float).557,(float).6343,(float).6021,(float)
	    .5996,(float).5517,(float).5156,(float).5566,(float).4442,(float)
	    .4642,(float).469,(float).4812,(float).4902,(float).4962,(float)
	    .5053,(float).5171,(float).5299,(float).5433,(float).5544,(float)
	    .5616,(float).5807,(float).608,(float).689,(float).6544,(float)
	    .6511,(float).5974,(float).5562,(float).596,(float).4873,(float)
	    .5095,(float).5148,(float).5281,(float).5381,(float).5446,(float)
	    .5546,(float).5675,(float).5814,(float).5959,(float).6078,(float)
	    .6156,(float).6361,(float).6651,(float).7496,(float).7126,(float)
	    .7084,(float).6483,(float).6014,(float).64,(float).5361,(float)
	    .5606,(float).5664,(float).5811,(float).5922,(float).5993,(float)
	    .6102,(float).6243,(float).6394,(float).655,(float).668,(float)
	    .6763,(float).6982,(float).7289,(float).817,(float).7774,(float)
	    .7719,(float).7051,(float).652,(float).689,(float).5913,(float)
	    .6185,(float).625,(float).641,(float).6533,(float).6612,(float)
	    .673,(float).6885,(float).7048,(float).7216,(float).7357,(float)
	    .7446,(float).7679,(float).8003,(float).8915,(float).8494,(float)
	    .8426,(float).7684,(float).7083,(float).7437,(float).6539,(float)
	    .6842,(float).6913,(float).709,(float).7226,(float).7312,(float)
	    .7442,(float).7609,(float).7786,(float).7967,(float).8118,(float)
	    .8214,(float).8462,(float).8802,(float).9741,(float).9292,(float)
	    .9211,(float).8389,(float).7711,(float).8045,(float).7251,(float)
	    .7589,(float).7667,(float).7862,(float).801,(float).8106,(float)
	    .8247,(float).8428,(float).862,(float).8815,(float).8976,(float)
	    .9078,(float).934,(float).9697,(float)1.065,(float)1.018,(float)
	    1.008,(float).9173,(float).8412,(float).8724,(float).8061,(float)
	    .8437,(float).8523,(float).8738,(float).8901,(float).9005,(float)
	    .916,(float).9356,(float).9562,(float).9772,(float).9943,(float)
	    1.005,(float)1.033,(float)1.07,(float)1.166,(float)1.116,(float)
	    1.105,(float)1.004,(float).9192,(float).948,(float).8983,(float)
	    .9403,(float).9499,(float).9734,(float).9913,(float)1.003,(float)
	    1.02,(float)1.041,(float)1.063,(float)1.085,(float)1.103,(float)
	    1.115,(float)1.143,(float)1.182,(float)1.277,(float)1.225,(float)
	    1.211,(float)1.102,(float)1.006,(float)1.033,(float)1.004,(float)
	    1.05,(float)1.061,(float)1.087,(float)1.106,(float)1.119,(float)
	    1.137,(float)1.159,(float)1.183,(float)1.207,(float)1.226,(float)
	    1.238,(float)1.268,(float)1.307,(float)1.4,(float)1.345,(float)
	    1.329,(float)1.209,(float)1.104,(float)1.127,(float)1.124,(float)
	    1.176,(float)1.188,(float)1.216,(float)1.237,(float)1.251,(float)
	    1.27,(float)1.294,(float)1.32,(float)1.345,(float)1.365,(float)
	    1.376,(float)1.407,(float)1.447,(float)1.536,(float)1.478,(float)
	    1.46,(float)1.33,(float)1.212,(float)1.233,(float)1.262,(float)
	    1.32,(float)1.333,(float)1.363,(float)1.387,(float)1.401,(float)
	    1.422,(float)1.447,(float)1.474,(float)1.5,(float)1.521,(float)
	    1.533,(float)1.564,(float)1.603,(float)1.685,(float)1.625,(float)
	    1.604,(float)1.464,(float)1.334,(float)1.352,(float)1.42,(float)
	    1.484,(float)1.498,(float)1.531,(float)1.556,(float)1.572,(float)
	    1.594,(float)1.621,(float)1.649,(float)1.675,(float)1.696,(float)
	    1.708,(float)1.74,(float)1.777,(float)1.848,(float)1.788,(float)
	    1.764,(float)1.613,(float)1.471,(float)1.485,(float)1.602,(float)
	    1.672,(float)1.687,(float)1.723,(float)1.75,(float)1.766,(float)
	    1.79,(float)1.818,(float)1.846,(float)1.873,(float)1.894,(float)
	    1.906,(float)1.936,(float)1.97,(float)2.028,(float)1.967,(float)
	    1.94,(float)1.78,(float)1.625,(float)1.636,(float)1.812,(float)
	    1.888,(float)1.904,(float)1.942,(float)1.971,(float)1.988,(float)
	    2.013,(float)2.041,(float)2.07,(float)2.096,(float)2.117,(float)
	    2.128,(float)2.156,(float)2.186,(float)2.224,(float)2.164,(float)
	    2.134,(float)1.966,(float)1.799,(float)1.807,(float)2.054,(float)
	    2.136,(float)2.153,(float)2.193,(float)2.223,(float)2.241,(float)
	    2.266,(float)2.294,(float)2.322,(float)2.348,(float)2.367,(float)
	    2.377,(float)2.402,(float)2.426,(float)2.439,(float)2.38,(float)
	    2.348,(float)2.174,(float)1.996,(float)2.001,(float)2.333,(float)
	    2.42,(float)2.438,(float)2.479,(float)2.51,(float)2.528,(float)
	    2.553,(float)2.58,(float)2.607,(float)2.631,(float)2.647,(float)
	    2.656,(float)2.676,(float)2.691,(float)2.673,(float)2.618,(float)
	    2.583,(float)2.408,(float)2.221,(float)2.222,(float)2.655,(float)
	    2.748,(float)2.766,(float)2.807,(float)2.838,(float)2.855,(float)
	    2.879,(float)2.904,(float)2.928,(float)2.948,(float)2.962,(float)
	    2.968,(float)2.981,(float)2.984,(float)2.927,(float)2.879,(float)
	    2.842,(float)2.671,(float)2.479,(float)2.475,(float)3.03,(float)
	    3.124,(float)3.142,(float)3.182,(float)3.211,(float)3.227,(float)
	    3.249,(float)3.27,(float)3.289,(float)3.304,(float)3.313,(float)
	    3.315,(float)3.319,(float)3.308,(float)3.201,(float)3.164,(float)
	    3.125,(float)2.967,(float)2.776,(float)2.764,(float)3.465,(float)
	    3.558,(float)3.575,(float)3.61,(float)3.636,(float)3.651,(float)
	    3.668,(float)3.682,(float)3.695,(float)3.702,(float)3.704,(float)
	    3.702,(float)3.693,(float)3.662,(float)3.497,(float)3.475,(float)
	    3.436,(float)3.301,(float)3.12,(float)3.097,(float)3.971,(float)
	    4.057,(float)4.071,(float)4.099,(float)4.12,(float)4.13,(float)
	    4.141,(float)4.147,(float)4.149,(float)4.146,(float)4.138,(float)
	    4.13,(float)4.104,(float)4.05,(float)3.815,(float)3.814,(float)
	    3.776,(float)3.679,(float)3.521,(float)3.479,(float)4.562,(float)
	    4.632,(float)4.642,(float)4.658,(float)4.669,(float)4.674,(float)
	    4.675,(float)4.667,(float)4.656,(float)4.638,(float)4.618,(float)
	    4.602,(float)4.556,(float)4.472,(float)4.154,(float)4.181,(float)
	    4.146,(float)4.109,(float)3.991,(float)3.919,(float)5.253,(float)
	    5.295,(float)5.297,(float)5.295,(float)5.292,(float)5.288,(float)
	    5.276,(float)5.249,(float)5.219,(float)5.182,(float)5.146,(float)
	    5.12,(float)5.048,(float)4.927,(float)4.515,(float)4.578,(float)
	    4.55,(float)4.597,(float)4.547,(float)4.426,(float)6.062,(float)
	    6.059,(float)6.05,(float)6.019,(float)5.996,(float)5.98,(float)
	    5.948,(float)5.897,(float)5.841,(float)5.78,(float)5.724,(float)
	    5.686,(float)5.581,(float)5.417,(float)4.896,(float)5.006,(float)
	    4.989,(float)5.153,(float)5.206,(float)5.008,(float)7.012,(float)
	    6.938,(float)6.913,(float)6.842,(float)6.79,(float)6.756,(float)
	    6.698,(float)6.614,(float)6.526,(float)6.433,(float)6.352,(float)
	    6.298,(float)6.154,(float)5.939,(float)5.296,(float)5.466,(float)
	    5.464,(float)5.786,(float)5.99,(float)5.672,(float)8.127,(float)
	    7.948,(float)7.9,(float)7.773,(float)7.681,(float)7.623,(float)
	    7.53,(float)7.403,(float)7.273,(float)7.141,(float)7.029,(float)
	    6.956,(float)6.766,(float)6.49,(float)5.714,(float)5.96,(float)
	    5.978,(float)6.507,(float)6.924,(float)6.425,(float)9.44,(float)
	    9.106,(float)9.025,(float)8.821,(float)8.675,(float)8.585,(float)
	    8.446,(float)8.264,(float)8.082,(float)7.901,(float)7.752,(float)
	    7.656,(float)7.412,(float)7.069,(float)6.15,(float)6.49,(float)
	    6.535,(float)7.327,(float)8.032,(float)7.267,(float)10.98,(float)
	    10.43,(float)10.3,(float)9.994,(float)9.777,(float)9.645,(float)
	    9.447,(float)9.196,(float)8.949,(float)8.711,(float)8.518,(float)
	    8.395,(float)8.09,(float)7.672,(float)6.604,(float)7.058,(float)
	    7.138,(float)8.254,(float)9.334,(float)8.194,(float)12.78,(float)
	    11.93,(float)11.74,(float)11.3,(float)10.99,(float)10.8,(float)
	    10.53,(float)10.2,(float)9.871,(float)9.564,(float)9.322,(float)
	    9.171,(float)8.797,(float)8.297,(float)7.079,(float)7.669,(float)
	    7.793,(float)9.293,(float)10.84,(float)9.184,(float)14.87,(float)
	    13.61,(float)13.35,(float)12.73,(float)12.3,(float)12.05,(float)
	    11.69,(float)11.26,(float)10.85,(float)10.46,(float)10.17,(float)
	    9.985,(float)9.536,(float)8.95,(float)7.583,(float)8.329,(float)
	    8.502,(float)10.44,(float)12.53,(float)10.2,(float)17.27,(float)
	    15.49,(float)15.13,(float)14.3,(float)13.73,(float)13.41,(float)
	    12.95,(float)12.41,(float)11.89,(float)11.42,(float)11.06,(float)
	    10.85,(float)10.32,(float)9.647,(float)8.128,(float)9.041,(float)
	    9.258,(float)11.66,(float)14.34,(float)11.18,(float)20.06,(float)
	    17.63,(float)17.15,(float)16.07,(float)15.35,(float)14.95,(float)
	    14.36,(float)13.7,(float)13.06,(float)12.49,(float)12.07,(float)
	    11.83,(float)11.2,(float)10.42,(float)8.71,(float)9.78,(float)
	    10.03,(float)12.87,(float)16.09,(float)12.03,(float)23.72,(float)
	    20.43,(float)19.78,(float)18.36,(float)17.42,(float)16.89,(float)
	    16.13,(float)15.29,(float)14.48,(float)13.78,(float)13.26,(float)
	    12.96,(float)12.2,(float)11.26,(float)9.26,(float)10.43,(float)
	    10.69,(float)13.83,(float)17.44,(float)12.61,(float)26.98,(float)
	    22.69,(float)21.87,(float)20.08,(float)18.91,(float)18.25,(float)
	    17.32,(float)16.32,(float)15.35,(float)14.52,(float)13.92,(float)
	    13.58,(float)12.71,(float)11.65,(float)9.472,(float)10.66,(float)
	    10.91,(float)14.15,(float)17.85,(float)12.77 };
    static real qhr[1660]	/* was [20][83] */ = { (float)3.388e-23,(
	    float)4.524e-22,(float)1.156e-21,(float)-4.206e-22,(float)
	    -4.642e-23,(float)1.625e-23,(float)5.125e-22,(float)-3.431e-23,(
	    float)5.428e-22,(float)-3.477e-22,(float)8.851e-22,(float)
	    -1.67e-22,(float)5.713e-22,(float)-3.391e-22,(float)-1.315e-22,(
	    float)2.669e-22,(float)3.473e-22,(float)2.287e-23,(float)
	    -1.316e-22,(float)1.171e-22,(float).002705,(float).002675,(float)
	    .002647,(float).002749,(float).002556,(float).002573,(float)
	    .002459,(float).002397,(float).00216,(float).002034,(float)
	    .001859,(float).001944,(float).00164,(float).001558,(float)
	    .001259,(float)2.49e-4,(float)9.441e-5,(float)-7.661e-5,(float)
	    1.287e-5,(float)-1.182e-4,(float).01324,(float).01299,(float)
	    .01288,(float).01315,(float).01228,(float).0122,(float).01162,(
	    float).01118,(float).01018,(float).009499,(float).00872,(float)
	    .008894,(float).007646,(float).00715,(float).005558,(float)
	    .001557,(float)6.895e-4,(float)-1.82e-4,(float)-7.324e-5,(float)
	    -6.476e-4,(float).02911,(float).02843,(float).02824,(float).0285,(
	    float).02679,(float).02625,(float).02501,(float).02394,(float)
	    .02199,(float).02036,(float).01878,(float).01865,(float).01635,(
	    float).01507,(float).01106,(float).00347,(float).001715,(float)
	    -3.819e-4,(float)-7.375e-4,(float)-.001734,(float).04717,(float)
	    .046,(float).04582,(float).04587,(float).04345,(float).04214,(
	    float).04023,(float).03859,(float).03564,(float).03281,(float)
	    .03043,(float).02968,(float).02638,(float).02396,(float).01676,(
	    float).00499,(float).002543,(float)-.00141,(float)-.002397,(float)
	    -.003588,(float).06492,(float).06326,(float).06312,(float).06291,(
	    float).06001,(float).05787,(float).05535,(float).05328,(float)
	    .04949,(float).04547,(float).04237,(float).04087,(float).03671,(
	    float).03316,(float).02214,(float).005817,(float).002424,(float)
	    -.003696,(float)-.004972,(float)-.006341,(float).08049,(float)
	    .0784,(float).07832,(float).07786,(float).07472,(float).07184,(
	    float).06887,(float).0665,(float).06216,(float).05712,(float)
	    .05343,(float).05121,(float).0464,(float).04196,(float).02691,(
	    float).00565,(float).001184,(float)-.007124,(float)-.00819,(float)
	    -.009982,(float).09251,(float).09019,(float).09019,(float).08953,(
	    float).08634,(float).08289,(float).07967,(float).07715,(float)
	    .07251,(float).06669,(float).06265,(float).05986,(float).05457,(
	    float).04932,(float).03081,(float).004433,(float)-.001028,(float)
	    -.0113,(float)-.01191,(float)-.01441,(float).1002,(float).09788,(
	    float).09795,(float).09722,(float).09402,(float).09034,(float)
	    .08708,(float).08439,(float).07971,(float).07353,(float).06924,(
	    float).06614,(float).0605,(float).0546,(float).03335,(float)
	    .00241,(float)-.003976,(float)-.0159,(float)-.01606,(float)
	    -.01953,(float).1033,(float).1013,(float).1013,(float).1007,(
	    float).09753,(float).09399,(float).09086,(float).08803,(float)
	    .08344,(float).07731,(float).07287,(float).06972,(float).06385,(
	    float).05752,(float).03421,(float)-3.422e-4,(float)-.007404,(
	    float)-.02082,(float)-.02061,(float)-.02524,(float).1023,(float)
	    .1007,(float).1008,(float).1003,(float).0973,(float).09414,(float)
	    .09122,(float).08844,(float).08402,(float).07815,(float).07377,(
	    float).07074,(float).06477,(float).05818,(float).03352,(float)
	    -.003735,(float)-.01123,(float)-.02596,(float)-.02556,(float)
	    -.03147,(float).0982,(float).09713,(float).09717,(float).09695,(
	    float).09412,(float).09147,(float).08882,(float).0862,(float)
	    .08198,(float).07657,(float).07232,(float).06963,(float).06359,(
	    float).05695,(float).03156,(float)-.007634,(float)-.01541,(float)
	    -.03131,(float)-.03088,(float)-.03817,(float).09197,(float).09141,
	    (float).09146,(float).09147,(float).08888,(float).08675,(float)
	    .0844,(float).08197,(float).07805,(float).07319,(float).0691,(
	    float).0668,(float).06079,(float).05431,(float).02855,(float)
	    -.01195,(float)-.01992,(float)-.03684,(float)-.03658,(float)
	    -.0453,(float).08458,(float).08445,(float).08452,(float).0847,(
	    float).08243,(float).08068,(float).07862,(float).07644,(float)
	    .0729,(float).06854,(float).0647,(float).06265,(float).05688,(
	    float).05058,(float).02461,(float)-.01662,(float)-.02481,(float)
	    -.04254,(float)-.04264,(float)-.05285,(float).07677,(float).07692,
	    (float).07705,(float).07729,(float).0754,(float).07386,(float)
	    .07208,(float).07019,(float).06704,(float).06305,(float).05955,(
	    float).05758,(float).05218,(float).04604,(float).01986,(float)
	    -.0217,(float)-.03006,(float)-.04843,(float)-.04903,(float)
	    -.06078,(float).06906,(float).06936,(float).06952,(float).06979,(
	    float).06823,(float).0668,(float).06526,(float).06359,(float)
	    .06082,(float).05711,(float).05392,(float).05196,(float).04696,(
	    float).04093,(float).01447,(float)-.02725,(float)-.03562,(float)
	    -.05451,(float)-.05574,(float)-.06908,(float).06178,(float).0621,(
	    float).06226,(float).06249,(float).06118,(float).05986,(float)
	    .05848,(float).05697,(float).05446,(float).05099,(float).04805,(
	    float).04612,(float).0414,(float).03542,(float).008665,(float)
	    -.0332,(float)-.04158,(float)-.0608,(float)-.06275,(float)-.07774,
	    (float).0551,(float).0553,(float).05544,(float).05559,(float)
	    .05445,(float).05322,(float).05192,(float).05056,(float).04816,(
	    float).04485,(float).04211,(float).04027,(float).03567,(float)
	    .02967,(float).002619,(float)-.03944,(float)-.04796,(float)
	    -.06736,(float)-.07004,(float)-.08673,(float).04908,(float).04908,
	    (float).04917,(float).04919,(float).04815,(float).047,(float)
	    .0457,(float).04446,(float).04206,(float).03885,(float).03624,(
	    float).0345,(float).0299,(float).0238,(float)-.003626,(float)
	    -.04594,(float)-.05468,(float)-.07417,(float)-.07761,(float)
	    -.09606,(float).04371,(float).04347,(float).0435,(float).04336,(
	    float).04235,(float).04123,(float).03991,(float).03871,(float)
	    .03627,(float).03311,(float).03055,(float).02886,(float).02421,(
	    float).01795,(float)-.0101,(float)-.05273,(float)-.06169,(float)
	    -.08127,(float)-.08543,(float)-.1057,(float).03894,(float).03845,(
	    float).03843,(float).03811,(float).03707,(float).03592,(float)
	    .03457,(float).03329,(float).03084,(float).02767,(float).0251,(
	    float).02337,(float).01867,(float).0122,(float)-.01679,(float)
	    -.05979,(float)-.06896,(float)-.08867,(float)-.09352,(float)
	    -.1156,(float).03476,(float).03399,(float).0339,(float).0334,(
	    float).03226,(float).03107,(float).02967,(float).02823,(float)
	    .02575,(float).02255,(float).01989,(float).01809,(float).01328,(
	    float).006539,(float)-.02363,(float)-.0671,(float)-.07648,(float)
	    -.09636,(float)-.1018,(float)-.1258,(float).03108,(float).03003,(
	    float).02987,(float).02917,(float).0279,(float).02665,(float)
	    .02519,(float).02353,(float).02099,(float).01772,(float).01493,(
	    float).01304,(float).008065,(float)9.936e-4,(float)-.03055,(float)
	    -.07461,(float)-.08426,(float)-.1044,(float)-.1104,(float)-.1363,(
	    float).02789,(float).0265,(float).02624,(float).02535,(float)
	    .02393,(float).02265,(float).02107,(float).01923,(float).01652,(
	    float).01313,(float).0102,(float).008231,(float).003014,(float)
	    -.004447,(float)-.03751,(float)-.08231,(float)-.09226,(float)
	    -.1127,(float)-.1192,(float)-.147,(float).02512,(float).02336,(
	    float).02298,(float).0219,(float).02031,(float).01904,(float)
	    .01728,(float).01528,(float).01233,(float).008757,(float).005681,(
	    float).003693,(float)-.001891,(float)-.009836,(float)-.04444,(
	    float)-.09016,(float)-.1004,(float)-.1213,(float)-.1283,(float)
	    -.1579,(float).0227,(float).02058,(float).02005,(float).01881,(
	    float).01701,(float).01576,(float).01382,(float).01161,(float)
	    .008374,(float).004622,(float).001362,(float)-6.171e-4,(float)
	    -.006651,(float)-.01513,(float)-.05139,(float)-.09814,(float)
	    -.1088,(float)-.1301,(float)-.1376,(float)-.169,(float).02055,(
	    float).0181,(float).01747,(float).01603,(float).01404,(float)
	    .01275,(float).01064,(float).008164,(float).004708,(float)
	    7.392e-4,(float)-.002723,(float)-.004788,(float)-.01122,(float)
	    -.02026,(float)-.05843,(float)-.1063,(float)-.1174,(float)-.1392,(
	    float)-.1471,(float)-.1802,(float).01865,(float).01588,(float)
	    .01518,(float).01352,(float).01136,(float).009934,(float).007682,(
	    float).004941,(float).001307,(float)-.002913,(float)-.006589,(
	    float)-.008846,(float)-.0156,(float)-.02521,(float)-.06553,(float)
	    -.1145,(float)-.1261,(float)-.1486,(float)-.1569,(float)-.1915,(
	    float).01696,(float).01389,(float).01315,(float).01125,(float)
	    .00893,(float).007307,(float).004928,(float).001932,(float)
	    -.00189,(float)-.006355,(float)-.01029,(float)-.01278,(float)
	    -.01985,(float)-.03004,(float)-.07263,(float)-.123,(float)-.135,(
	    float)-.1581,(float)-.1668,(float)-.2029,(float).01549,(float)
	    .0121,(float).01131,(float).009161,(float).006697,(float).004879,(
	    float).002351,(float)-8.723e-4,(float)-.004921,(float)-.00966,(
	    float)-.01385,(float)-.01657,(float)-.024,(float)-.03479,(float)
	    -.07968,(float)-.1314,(float)-.144,(float)-.1679,(float)-.1769,(
	    float)-.2144,(float).01422,(float).01049,(float).009615,(float)
	    .00724,(float).004618,(float).002661,(float)-6.637e-5,(float)
	    -.003485,(float)-.007821,(float)-.01289,(float)-.01729,(float)
	    -.02018,(float)-.02805,(float)-.03953,(float)-.08668,(float)-.14,(
	    float)-.1531,(float)-.1778,(float)-.1872,(float)-.2258,(float)
	    .01313,(float).00904,(float).008054,(float).005479,(float).002658,
	    (float)6.367e-4,(float)-.002319,(float)-.005947,(float)-.01063,(
	    float)-.01603,(float)-.02066,(float)-.02364,(float)-.03205,(float)
	    -.04426,(float)-.09364,(float)-.1487,(float)-.1622,(float)-.1878,(
	    float)-.1976,(float)-.2372,(float).01218,(float).007753,(float)
	    .006657,(float).003894,(float)8.311e-4,(float)-.001255,(float)
	    -.00441,(float)-.008331,(float)-.01333,(float)-.01901,(float)
	    -.02394,(float)-.02705,(float)-.03599,(float)-.04888,(float)
	    -.1006,(float)-.1574,(float)-.1714,(float)-.198,(float)-.2082,(
	    float)-.2485,(float).01132,(float).006579,(float).00542,(float)
	    .002435,(float)-8.244e-4,(float)-.003091,(float)-.006406,(float)
	    -.01062,(float)-.01588,(float)-.02186,(float)-.02709,(float)
	    -.03045,(float)-.0398,(float)-.05337,(float)-.1076,(float)-.1662,(
	    float)-.1807,(float)-.2082,(float)-.2188,(float)-.2597,(float)
	    .01057,(float).005498,(float).0043,(float).001059,(float)-.002341,
	    (float)-.00485,(float)-.00834,(float)-.01279,(float)-.0183,(float)
	    -.02463,(float)-.03014,(float)-.03378,(float)-.04353,(float)
	    -.05779,(float)-.1146,(float)-.1749,(float)-.1901,(float)-.2185,(
	    float)-.2294,(float)-.2708,(float).009939,(float).004519,(float)
	    .003277,(float)-2.257e-4,(float)-.003768,(float)-.006497,(float)
	    -.01019,(float)-.01487,(float)-.02066,(float)-.02736,(float)
	    -.03314,(float)-.03701,(float)-.04721,(float)-.06219,(float)
	    -.1215,(float)-.1837,(float)-.1994,(float)-.2289,(float)-.2401,(
	    float)-.2816,(float).009418,(float).003644,(float).002335,(float)
	    -.001414,(float)-.005133,(float)-.008032,(float)-.01195,(float)
	    -.01687,(float)-.02297,(float)-.03004,(float)-.0361,(float)
	    -.04015,(float)-.05087,(float)-.06658,(float)-.1283,(float)-.1925,
	    (float)-.2087,(float)-.2392,(float)-.2508,(float)-.2922,(float)
	    .009008,(float).002878,(float).001467,(float)-.0025,(float)
	    -.006448,(float)-.009461,(float)-.01361,(float)-.0188,(float)
	    -.02525,(float)-.03268,(float)-.03903,(float)-.04324,(float)
	    -.05451,(float)-.07095,(float)-.135,(float)-.2012,(float)-.218,(
	    float)-.2496,(float)-.2614,(float)-.3025,(float).008688,(float)
	    .002215,(float)6.896e-4,(float)-.003482,(float)-.007699,(float)
	    -.01081,(float)-.01518,(float)-.02068,(float)-.02748,(float)
	    -.03525,(float)-.04192,(float)-.04629,(float)-.05812,(float)
	    -.07529,(float)-.1416,(float)-.2099,(float)-.2271,(float)-.2598,(
	    float)-.2719,(float)-.3125,(float).008454,(float).001623,(float)
	    8.981e-8,(float)-.004388,(float)-.00886,(float)-.01211,(float)
	    -.01668,(float)-.0225,(float)-.02965,(float)-.03774,(float)
	    -.04476,(float)-.04933,(float)-.06167,(float)-.07954,(float)
	    -.1482,(float)-.2185,(float)-.2362,(float)-.2699,(float)-.2823,(
	    float)-.322,(float).008315,(float).001075,(float)-6.248e-4,(float)
	    -.005269,(float)-.009916,(float)-.01337,(float)-.01818,(float)
	    -.02423,(float)-.03172,(float)-.0402,(float)-.04752,(float)
	    -.05233,(float)-.06515,(float)-.0837,(float)-.1547,(float)-.227,(
	    float)-.2451,(float)-.2799,(float)-.2925,(float)-.3312,(float)
	    .008279,(float)8.19e-4,(float)-9.151e-4,(float)-.005701,(float)
	    -.01041,(float)-.01398,(float)-.01892,(float)-.02505,(float)
	    -.03272,(float)-.04143,(float)-.04886,(float)-.0538,(float)
	    -.06685,(float)-.08577,(float)-.1578,(float)-.2311,(float)-.2495,(
	    float)-.2849,(float)-.2975,(float)-.3356,(float).008274,(float)
	    5.775e-4,(float)-.001198,(float)-.006128,(float)-.01088,(float)
	    -.01457,(float)-.01966,(float)-.02585,(float)-.03372,(float)
	    -.04268,(float)-.0502,(float)-.05523,(float)-.06855,(float)
	    -.08785,(float)-.1609,(float)-.2352,(float)-.2538,(float)-.2897,(
	    float)-.3025,(float)-.3398,(float).008317,(float)1.801e-4,(float)
	    -.001684,(float)-.006891,(float)-.01179,(float)-.01566,(float)
	    -.02106,(float)-.02742,(float)-.03568,(float)-.04512,(float)
	    -.05284,(float)-.05803,(float)-.07192,(float)-.09198,(float)-.167,
	    (float)-.2433,(float)-.2623,(float)-.2993,(float)-.3122,(float)
	    -.3479,(float).008427,(float)-7.529e-5,(float)-.002032,(float)
	    -.007481,(float)-.01266,(float)-.01668,(float)-.0223,(float)
	    -.02903,(float)-.03761,(float)-.04739,(float)-.05546,(float)
	    -.06083,(float)-.07525,(float)-.09597,(float)-.1729,(float)-.2512,
	    (float)-.2706,(float)-.3086,(float)-.3216,(float)-.3555,(float)
	    .008611,(float)-2.034e-4,(float)-.002242,(float)-.007917,(float)
	    -.01345,(float)-.01763,(float)-.02341,(float)-.03062,(float)
	    -.03945,(float)-.04951,(float)-.05801,(float)-.06364,(float)
	    -.07848,(float)-.09978,(float)-.1787,(float)-.2588,(float)-.2785,(
	    float)-.3175,(float)-.3305,(float)-.3624,(float).008898,(float)
	    -2.525e-4,(float)-.002364,(float)-.008283,(float)-.01412,(float)
	    -.01853,(float)-.02447,(float)-.0321,(float)-.04119,(float)
	    -.05156,(float)-.06046,(float)-.06638,(float)-.0816,(float)-.1035,
	    (float)-.1842,(float)-.266,(float)-.286,(float)-.326,(float)
	    -.3391,(float)-.3687,(float).009317,(float)-2.5e-4,(float)
	    -.002436,(float)-.008634,(float)-.01469,(float)-.01934,(float)
	    -.02552,(float)-.0334,(float)-.04283,(float)-.0536,(float)-.06278,
	    (float)-.06896,(float)-.0846,(float)-.107,(float)-.1893,(float)
	    -.2728,(float)-.2931,(float)-.334,(float)-.3472,(float)-.3742,(
	    float).009885,(float)-1.807e-4,(float)-.002463,(float)-.008953,(
	    float)-.01514,(float)-.02002,(float)-.02651,(float)-.03454,(float)
	    -.04435,(float)-.05561,(float)-.06498,(float)-.07134,(float)
	    -.08744,(float)-.1105,(float)-.194,(float)-.2792,(float)-.2998,(
	    float)-.3415,(float)-.3546,(float)-.379,(float).0106,(float)
	    -4.815e-6,(float)-.002424,(float)-.009183,(float)-.0155,(float)
	    -.02054,(float)-.0274,(float)-.03552,(float)-.04579,(float)
	    -.05757,(float)-.06706,(float)-.07353,(float)-.09016,(float)
	    -.1138,(float)-.1983,(float)-.2851,(float)-.3058,(float)-.3483,(
	    float)-.3615,(float)-.383,(float).01144,(float)3.298e-4,(float)
	    -.002243,(float)-.009235,(float)-.01578,(float)-.02091,(float)
	    -.0281,(float)-.03636,(float)-.04714,(float)-.05935,(float)
	    -.06903,(float)-.07556,(float)-.09273,(float)-.117,(float)-.2021,(
	    float)-.2903,(float)-.3113,(float)-.3545,(float)-.3677,(float)
	    -.3861,(float).01236,(float)8.693e-4,(float)-.001825,(float)
	    -.009017,(float)-.01594,(float)-.02115,(float)-.02856,(float)
	    -.03719,(float)-.04834,(float)-.0608,(float)-.07085,(float)
	    -.07749,(float)-.09511,(float)-.1197,(float)-.2055,(float)-.295,(
	    float)-.316,(float)-.36,(float)-.373,(float)-.3884,(float).01335,(
	    float).001601,(float)-.00116,(float)-.00855,(float)-.01592,(float)
	    -.02127,(float)-.02879,(float)-.03801,(float)-.04932,(float)
	    -.06189,(float)-.07245,(float)-.07926,(float)-.09719,(float)-.122,
	    (float)-.2084,(float)-.2989,(float)-.3199,(float)-.3645,(float)
	    -.3776,(float)-.3897,(float).01448,(float).002449,(float)
	    -3.305e-4,(float)-.007968,(float)-.01563,(float)-.02128,(float)
	    -.0289,(float)-.03861,(float)-.04999,(float)-.06279,(float)
	    -.07371,(float)-.08082,(float)-.09889,(float)-.1239,(float)-.2109,
	    (float)-.3021,(float)-.3229,(float)-.3682,(float)-.3812,(float)
	    -.39,(float).01579,(float).00338,(float)5.932e-4,(float)-.007338,(
	    float)-.0151,(float)-.02112,(float)-.02894,(float)-.03884,(float)
	    -.05036,(float)-.06358,(float)-.07462,(float)-.08205,(float)
	    -.1002,(float)-.1254,(float)-.2125,(float)-.3043,(float)-.325,(
	    float)-.3708,(float)-.3838,(float)-.3893,(float).0173,(float)
	    .004453,(float).001609,(float)-.006589,(float)-.0144,(float)
	    -.02072,(float)-.0288,(float)-.03868,(float)-.05054,(float)
	    -.06419,(float)-.07526,(float)-.08287,(float)-.1012,(float)-.1267,
	    (float)-.2132,(float)-.3055,(float)-.3261,(float)-.3723,(float)
	    -.3854,(float)-.3875,(float).01901,(float).005747,(float).002774,(
	    float)-.005593,(float)-.0136,(float)-.01999,(float)-.02833,(float)
	    -.03827,(float)-.05054,(float)-.0645,(float)-.07566,(float)
	    -.08326,(float)-.1019,(float)-.1275,(float)-.213,(float)-.3056,(
	    float)-.3261,(float)-.3725,(float)-.3857,(float)-.3846,(float)
	    .02089,(float).007287,(float).004151,(float)-.004297,(float)
	    -.01263,(float)-.01894,(float)-.02751,(float)-.03766,(float)
	    -.05029,(float)-.06438,(float)-.07574,(float)-.08319,(float)
	    -.1022,(float)-.1278,(float)-.212,(float)-.3046,(float)-.3247,(
	    float)-.3714,(float)-.3849,(float)-.3806,(float).02289,(float)
	    .009066,(float).005802,(float)-.002697,(float)-.01141,(float)
	    -.01763,(float)-.02636,(float)-.03684,(float)-.04966,(float)
	    -.06376,(float)-.07537,(float)-.08271,(float)-.1019,(float)-.1273,
	    (float)-.2102,(float)-.3023,(float)-.322,(float)-.3689,(float)
	    -.3826,(float)-.3755,(float).025,(float).01106,(float).00771,(
	    float)-8.211e-4,(float)-.009913,(float)-.01604,(float)-.02487,(
	    float)-.03575,(float)-.04864,(float)-.06267,(float)-.07453,(float)
	    -.08176,(float)-.101,(float)-.1261,(float)-.2075,(float)-.2987,(
	    float)-.3179,(float)-.3649,(float)-.379,(float)-.3691,(float)
	    .02724,(float).0132,(float).00982,(float).001227,(float)-.008059,(
	    float)-.01424,(float)-.02313,(float)-.03421,(float)-.04712,(float)
	    -.06117,(float)-.07312,(float)-.08032,(float)-.0995,(float)-.1242,
	    (float)-.2036,(float)-.2936,(float)-.3123,(float)-.3593,(float)
	    -.3739,(float)-.3615,(float).02958,(float).01545,(float).01213,(
	    float).003444,(float)-.005823,(float)-.01226,(float)-.02114,(
	    float)-.03221,(float)-.04506,(float)-.05918,(float)-.07107,(float)
	    -.07839,(float)-.09723,(float)-.1215,(float)-.1986,(float)-.287,(
	    float)-.3052,(float)-.3519,(float)-.3673,(float)-.3526,(float)
	    .03201,(float).01787,(float).01464,(float).005887,(float)-.003285,
	    (float)-.01,(float)-.01883,(float)-.02979,(float)-.04254,(float)
	    -.05666,(float)-.06842,(float)-.0759,(float)-.09427,(float)-.118,(
	    float)-.1925,(float)-.279,(float)-.2963,(float)-.3428,(float)
	    -.3589,(float)-.3424,(float).03457,(float).0205,(float).01728,(
	    float).008595,(float)-5.974e-4,(float)-.007301,(float)-.01609,(
	    float)-.027,(float)-.03967,(float)-.05367,(float)-.06529,(float)
	    -.07263,(float)-.09069,(float)-.1138,(float)-.1852,(float)-.2692,(
	    float)-.2858,(float)-.3318,(float)-.3489,(float)-.3308,(float)
	    .03722,(float).02332,(float).02005,(float).01154,(float).002233,(
	    float)-.00421,(float)-.01296,(float)-.02382,(float)-.03646,(float)
	    -.05018,(float)-.06165,(float)-.06863,(float)-.08647,(float)
	    -.1089,(float)-.1768,(float)-.2577,(float)-.2735,(float)-.3188,(
	    float)-.3371,(float)-.3178,(float).03991,(float).02633,(float)
	    .02297,(float).01478,(float).00515,(float)-7.418e-4,(float)
	    -.009384,(float)-.02038,(float)-.03293,(float)-.04604,(float)
	    -.05757,(float)-.06393,(float)-.08165,(float)-.103,(float)-.1672,(
	    float)-.2446,(float)-.2595,(float)-.3039,(float)-.3234,(float)
	    -.3034,(float).04262,(float).02944,(float).02595,(float).01818,(
	    float).008199,(float).003032,(float)-.00549,(float)-.01664,(float)
	    -.02902,(float)-.04141,(float)-.05295,(float)-.05859,(float)
	    -.07613,(float)-.09632,(float)-.1564,(float)-.23,(float)-.2438,(
	    float)-.287,(float)-.3078,(float)-.2875,(float).04526,(float)
	    .03244,(float).02892,(float).0215,(float).01154,(float).006862,(
	    float)-.001521,(float)-.01243,(float)-.02457,(float)-.03642,(
	    float)-.04765,(float)-.05273,(float)-.06981,(float)-.08884,(float)
	    -.1447,(float)-.2139,(float)-.2265,(float)-.2682,(float)-.2903,(
	    float)-.2702,(float).04769,(float).03529,(float).03194,(float)
	    .02474,(float).01528,(float).01054,(float).002468,(float)-.00794,(
	    float)-.01945,(float)-.03102,(float)-.04154,(float)-.04664,(float)
	    -.06258,(float)-.08055,(float)-.1322,(float)-.1965,(float)-.2078,(
	    float)-.2474,(float)-.2709,(float)-.2514,(float).0499,(float)
	    .03801,(float).03488,(float).02793,(float).01913,(float).01419,(
	    float).006546,(float)-.003224,(float)-.01405,(float)-.02535,(
	    float)-.03501,(float)-.04024,(float)-.05483,(float)-.07182,(float)
	    -.1193,(float)-.1777,(float)-.1878,(float)-.2249,(float)-.2495,(
	    float)-.231,(float).0519,(float).04048,(float).03759,(float)
	    .03089,(float).02285,(float).01776,(float).01053,(float).001704,(
	    float)-.008622,(float)-.01955,(float)-.0283,(float)-.03349,(float)
	    -.04683,(float)-.06271,(float)-.1056,(float)-.1578,(float)-.1669,(
	    float)-.2008,(float)-.2262,(float)-.2093,(float).05358,(float)
	    .04266,(float).04001,(float).03357,(float).02626,(float).02117,(
	    float).01434,(float).006477,(float)-.003265,(float)-.01368,(float)
	    -.02156,(float)-.02651,(float)-.03863,(float)-.0532,(float)
	    -.09127,(float)-.1373,(float)-.145,(float)-.1755,(float)-.2013,(
	    float)-.1864,(float).0549,(float).04448,(float).04182,(float)
	    .03595,(float).02879,(float).02459,(float).01803,(float).01076,(
	    float).001392,(float)-.007927,(float)-.01537,(float)-.01924,(
	    float)-.03081,(float)-.04376,(float)-.07654,(float)-.1168,(float)
	    -.123,(float)-.1495,(float)-.1747,(float)-.1624,(float).05561,(
	    float).04553,(float).04259,(float).0375,(float).03028,(float)
	    .02764,(float).02113,(float).01462,(float).005168,(float)-.00287,(
	    float)-.009855,(float)-.01195,(float)-.0236,(float)-.03485,(float)
	    -.06181,(float)-.0964,(float)-.1012,(float)-.123,(float)-.147,(
	    float)-.1377,(float).05517,(float).04543,(float).04208,(float)
	    .03791,(float).03065,(float).02974,(float).02331,(float).0176,(
	    float).008102,(float).001242,(float)-.004989,(float)-.005381,(
	    float)-.01695,(float)-.02659,(float)-.04789,(float)-.07634,(float)
	    -.08006,(float)-.09693,(float)-.1187,(float)-.1128,(float).0531,(
	    float).044,(float).04009,(float).03714,(float).02939,(float)
	    .03066,(float).02455,(float).01876,(float).009738,(float).004428,(
	    float)-.001263,(float)1.154e-4,(float)-.01129,(float)-.01935,(
	    float)-.03535,(float)-.05701,(float)-.06003,(float)-.07212,(float)
	    -.09077,(float)-.08848,(float).04897,(float).04066,(float).03629,(
	    float).03449,(float).02662,(float).0297,(float).02408,(float)
	    .0184,(float).01022,(float).006122,(float).001438,(float).004222,(
	    float)-.006603,(float)-.01351,(float)-.02416,(float)-.03961,(
	    float)-.04183,(float)-.04973,(float)-.06426,(float)-.0655,(float)
	    .04243,(float).03529,(float).03098,(float).03004,(float).02267,(
	    float).02647,(float).0218,(float).01628,(float).009905,(float)
	    .006821,(float).003321,(float).006402,(float)-.002679,(float)
	    -.008481,(float)-.01481,(float)-.02516,(float)-.0263,(float)
	    -.03077,(float)-.04063,(float)-.04484,(float).03358,(float).02826,
	    (float).02458,(float).02432,(float).01759,(float).02148,(float)
	    .01828,(float).01222,(float).008604,(float).00716,(float).004031,(
	    float).006872,(float)6.384e-5,(float)-.003919,(float)-.007809,(
	    float)-.01338,(float)-.0146,(float)-.01595,(float)-.02163,(float)
	    -.02749,(float).02348,(float).01954,(float).01666,(float).01679,(
	    float).01192,(float).01532,(float).01307,(float).00902,(float)
	    .006227,(float).00552,(float).00363,(float).006538,(float).001208,
	    (float)-.002001,(float)-.002425,(float)-.005261,(float)-.006345,(
	    float)-.006038,(float)-.008394,(float)-.01423,(float).01324,(
	    float).01043,(float).008573,(float).00871,(float).006591,(float)
	    .008717,(float).007038,(float).006669,(float).003512,(float)
	    .002549,(float).00253,(float).005064,(float).001158,(float)
	    -.001903,(float)7.463e-4,(float)-.001734,(float)-.001145,(float)
	    -.001019,(float)-.001355,(float)-.005446,(float).004093,(float)
	    .003119,(float).002603,(float).002571,(float).002086,(float)
	    .00257,(float).002041,(float).002401,(float).001108,(float)
	    8.291e-4,(float)8.873e-4,(float).001805,(float)4.679e-4,(float)
	    -4.805e-4,(float)6.63e-4,(float)-3.817e-4,(float)1.771e-4,(float)
	    1.302e-4,(float)2.122e-4,(float)-9.621e-4,(float)2.63e-20,(float)
	    8.522e-21,(float)2.928e-20,(float)2.363e-20,(float)2.318e-20,(
	    float)2.508e-20,(float)1.995e-20,(float)-4.543e-21,(float)
	    1.924e-20,(float)-1.758e-20,(float)1.537e-20,(float)9.72e-21,(
	    float)3.459e-21,(float)-1.251e-21,(float)-2.429e-20,(float)
	    -7.847e-21,(float)4.417e-21,(float)-4.145e-22,(float)3.172e-22,(
	    float)1.176e-20 };
    static real uhr[1660]	/* was [20][83] */ = { (float)-.3494,(float)
	    -.3524,(float)-.3543,(float)-.3623,(float)-.3549,(float)-.3501,(
	    float)-.3444,(float)-.3445,(float)-.3352,(float)-.3251,(float)
	    -.3184,(float)-.3186,(float)-.3108,(float)-.3172,(float)-.3355,(
	    float)-.2986,(float)-.304,(float)-.302,(float)-.302,(float)-.3991,
	    (float)-.3431,(float)-.3463,(float)-.3483,(float)-.3563,(float)
	    -.3492,(float)-.3445,(float)-.3391,(float)-.3392,(float)-.3305,(
	    float)-.3208,(float)-.3143,(float)-.3144,(float)-.3072,(float)
	    -.3136,(float)-.3325,(float)-.2969,(float)-.3026,(float)-.301,(
	    float)-.3015,(float)-.3988,(float)-.3173,(float)-.322,(float)
	    -.3242,(float)-.3319,(float)-.3263,(float)-.3222,(float)-.318,(
	    float)-.3184,(float)-.3116,(float)-.3035,(float)-.2983,(float)
	    -.2981,(float)-.293,(float)-.2996,(float)-.3208,(float)-.2903,(
	    float)-.297,(float)-.2972,(float)-.2993,(float)-.3975,(float)
	    -.2772,(float)-.2842,(float)-.2868,(float)-.2945,(float)-.2913,(
	    float)-.2883,(float)-.286,(float)-.2873,(float)-.2833,(float)
	    -.2777,(float)-.2743,(float)-.2741,(float)-.272,(float)-.2795,(
	    float)-.3041,(float)-.2811,(float)-.289,(float)-.2919,(float)
	    -.2963,(float)-.3952,(float)-.2303,(float)-.2399,(float)-.243,(
	    float)-.2511,(float)-.2506,(float)-.249,(float)-.2488,(float)
	    -.2518,(float)-.2507,(float)-.248,(float)-.2467,(float)-.247,(
	    float)-.248,(float)-.257,(float)-.2862,(float)-.2717,(float)
	    -.2804,(float)-.2865,(float)-.2934,(float)-.3922,(float)-.1834,(
	    float)-.1954,(float)-.1988,(float)-.2074,(float)-.2094,(float)
	    -.2093,(float)-.2111,(float)-.2159,(float)-.2175,(float)-.2176,(
	    float)-.2186,(float)-.2197,(float)-.2234,(float)-.2344,(float)
	    -.269,(float)-.2631,(float)-.2723,(float)-.2817,(float)-.2907,(
	    float)-.3886,(float)-.1408,(float)-.1544,(float)-.158,(float)
	    -.167,(float)-.1712,(float)-.1723,(float)-.1758,(float)-.1822,(
	    float)-.1862,(float)-.1886,(float)-.1917,(float)-.1936,(float)-.2,
	    (float)-.2128,(float)-.2532,(float)-.2554,(float)-.265,(float)
	    -.2774,(float)-.2882,(float)-.3846,(float)-.1042,(float)-.1187,(
	    float)-.1225,(float)-.1316,(float)-.1373,(float)-.1395,(float)
	    -.1442,(float)-.1519,(float)-.1577,(float)-.1621,(float)-.1669,(
	    float)-.1696,(float)-.1781,(float)-.1927,(float)-.2386,(float)
	    -.2484,(float)-.2584,(float)-.2735,(float)-.2857,(float)-.3804,(
	    float)-.07359,(float)-.08839,(float)-.09214,(float)-.1013,(float)
	    -.1079,(float)-.111,(float)-.1165,(float)-.1249,(float)-.1321,(
	    float)-.1382,(float)-.1442,(float)-.1476,(float)-.1578,(float)
	    -.1739,(float)-.2248,(float)-.2417,(float)-.2524,(float)-.2698,(
	    float)-.283,(float)-.3758,(float)-.04855,(float)-.0631,(float)
	    -.06671,(float)-.07564,(float)-.08269,(float)-.08637,(float)
	    -.09248,(float)-.1012,(float)-.1093,(float)-.1167,(float)-.1236,(
	    float)-.1275,(float)-.139,(float)-.1563,(float)-.2116,(float)
	    -.2351,(float)-.2467,(float)-.266,(float)-.2802,(float)-.371,(
	    float)-.02835,(float)-.04229,(float)-.04569,(float)-.05424,(float)
	    -.06136,(float)-.06546,(float)-.07187,(float)-.08062,(float)
	    -.08922,(float)-.09761,(float)-.105,(float)-.1095,(float)-.1218,(
	    float)-.1401,(float)-.1991,(float)-.2286,(float)-.241,(float)
	    -.2621,(float)-.277,(float)-.3658,(float)-.01232,(float)-.02541,(
	    float)-.02853,(float)-.0366,(float)-.0435,(float)-.04792,(float)
	    -.05442,(float)-.06291,(float)-.07179,(float)-.08091,(float)
	    -.08855,(float)-.09341,(float)-.1062,(float)-.1252,(float)-.1873,(
	    float)-.222,(float)-.2351,(float)-.2579,(float)-.2736,(float)
	    -.3604,(float)1.89e-4,(float)-.01182,(float)-.01465,(float)
	    -.02215,(float)-.02875,(float)-.03327,(float)-.03969,(float)
	    -.04783,(float)-.05683,(float)-.0663,(float)-.07408,(float)
	    -.07916,(float)-.09233,(float)-.1117,(float)-.1761,(float)-.2154,(
	    float)-.2291,(float)-.2534,(float)-.2699,(float)-.3545,(float)
	    .009855,(float)-9.428e-4,(float)-.003506,(float)-.01035,(float)
	    -.01668,(float)-.02107,(float)-.02726,(float)-.03512,(float)
	    -.04406,(float)-.05355,(float)-.06144,(float)-.06654,(float)
	    -.07991,(float)-.09936,(float)-.1653,(float)-.2086,(float)-.2229,(
	    float)-.2488,(float)-.2659,(float)-.3484,(float).01727,(float)
	    .007715,(float).005396,(float)-7.512e-4,(float)-.006828,(float)
	    -.01092,(float)-.01682,(float)-.02442,(float)-.03314,(float)
	    -.04243,(float)-.05037,(float)-.05536,(float)-.06879,(float)
	    -.08813,(float)-.1551,(float)-.2016,(float)-.2165,(float)-.2438,(
	    float)-.2616,(float)-.3418,(float).02297,(float).0146,(float)
	    .01252,(float).007037,(float).001283,(float)-.002495,(float)
	    -.00806,(float)-.01535,(float)-.02373,(float)-.03274,(float)
	    -.0406,(float)-.04542,(float)-.05878,(float)-.07783,(float)-.1452,
	    (float)-.1945,(float)-.2098,(float)-.2386,(float)-.257,(float)
	    -.3349,(float).02739,(float).0201,(float).01826,(float).01339,(
	    float).008046,(float).004536,(float)-6.755e-4,(float)-.00755,(
	    float)-.01554,(float)-.02422,(float)-.0319,(float)-.03658,(float)
	    -.04968,(float)-.06833,(float)-.1356,(float)-.1874,(float)-.2029,(
	    float)-.2331,(float)-.252,(float)-.3275,(float).0309,(float)
	    .02458,(float).02297,(float).01864,(float).01379,(float).01048,(
	    float).005616,(float)-7.2e-4,(float)-.008318,(float)-.01669,(
	    float)-.02406,(float)-.02863,(float)-.04133,(float)-.05951,(float)
	    -.1263,(float)-.1801,(float)-.1959,(float)-.2273,(float)-.2466,(
	    float)-.3197,(float).0338,(float).02833,(float).02693,(float)
	    .02309,(float).01875,(float).01563,(float).01109,(float).005353,(
	    float)-.001873,(float)-.009947,(float)-.01693,(float)-.02137,(
	    float)-.03361,(float)-.0513,(float)-.1172,(float)-.1726,(float)
	    -.1887,(float)-.2212,(float)-.2409,(float)-.3115,(float).0363,(
	    float).0316,(float).03039,(float).02701,(float).02312,(float)
	    .02023,(float).016,(float).0108,(float).003948,(float)-.003766,(
	    float)-.01038,(float)-.01463,(float)-.02642,(float)-.04354,(float)
	    -.1082,(float)-.165,(float)-.1813,(float)-.2148,(float)-.2349,(
	    float)-.3028,(float).03857,(float).03459,(float).03353,(float)
	    .0306,(float).02708,(float).02445,(float).02054,(float).01576,(
	    float).009322,(float).002012,(float)-.004264,(float)-.008288,(
	    float)-.01962,(float)-.03608,(float)-.09927,(float)-.1572,(float)
	    -.1736,(float)-.2081,(float)-.2284,(float)-.2937,(float).04074,(
	    float).03743,(float).03652,(float).03398,(float).0308,(float)
	    .02841,(float).02481,(float).02041,(float).0144,(float).007505,(
	    float).001565,(float)-.002231,(float)-.01307,(float)-.02884,(
	    float)-.09033,(float)-.1491,(float)-.1656,(float)-.201,(float)
	    -.2216,(float)-.284,(float).0429,(float).04021,(float).03945,(
	    float).03728,(float).0344,(float).03223,(float).02894,(float)
	    .02486,(float).01931,(float).01284,(float).007208,(float).00363,(
	    float)-.006688,(float)-.02171,(float)-.08133,(float)-.1408,(float)
	    -.1573,(float)-.1934,(float)-.2143,(float)-.2737,(float).04513,(
	    float).04304,(float).04244,(float).0406,(float).03798,(float)
	    .03598,(float).03302,(float).02924,(float).02414,(float).01812,(
	    float).01278,(float).009366,(float)-3.697e-4,(float)-.01462,(
	    float)-.07222,(float)-.1322,(float)-.1486,(float)-.1855,(float)
	    -.2066,(float)-.2629,(float).04753,(float).04599,(float).04552,(
	    float).04399,(float).04162,(float).03978,(float).03715,(float)
	    .03364,(float).02899,(float).02341,(float).01836,(float).0151,(
	    float).005976,(float)-.00747,(float)-.0629,(float)-.1231,(float)
	    -.1396,(float)-.177,(float)-.1983,(float)-.2515,(float).05015,(
	    float).04911,(float).04876,(float).04752,(float).04539,(float)
	    .04373,(float).04138,(float).03816,(float).03392,(float).02877,(
	    float).02402,(float).02095,(float).01241,(float)-2.342e-4,(float)
	    -.05327,(float)-.1137,(float)-.13,(float)-.1681,(float)-.1896,(
	    float)-.2395,(float).05307,(float).05246,(float).05219,(float)
	    .05126,(float).04934,(float).04791,(float).0458,(float).04288,(
	    float).039,(float).03426,(float).02983,(float).02701,(float)
	    .01899,(float).007158,(float)-.04328,(float)-.1038,(float)-.1199,(
	    float)-.1586,(float)-.1802,(float)-.2268,(float).05632,(float)
	    .05609,(float).05589,(float).05525,(float).05356,(float).05235,(
	    float).05046,(float).04787,(float).04432,(float).03997,(float)
	    .03589,(float).03334,(float).02584,(float).01482,(float)-.03285,(
	    float)-.09336,(float)-.1093,(float)-.1484,(float)-.1702,(float)
	    -.2133,(float).05992,(float).06006,(float).05993,(float).05956,(
	    float).05811,(float).05711,(float).05543,(float).0532,(float)
	    .04995,(float).04599,(float).04227,(float).04,(float).03303,(
	    float).02285,(float)-.02188,(float)-.08228,(float)-.098,(float)
	    -.1376,(float)-.1596,(float)-.1991,(float).06392,(float).06444,(
	    float).06441,(float).06429,(float).06307,(float).06224,(float)
	    .0608,(float).05889,(float).05599,(float).05244,(float).04906,(
	    float).04704,(float).04065,(float).03135,(float)-.01031,(float)
	    -.0705,(float)-.08599,(float)-.126,(float)-.1482,(float)-.1841,(
	    float).06836,(float).06928,(float).06937,(float).0695,(float)
	    .06851,(float).0678,(float).06664,(float).06502,(float).06253,(
	    float).0594,(float).05635,(float).05452,(float).04879,(float)
	    .04045,(float).001921,(float)-.05794,(float)-.07316,(float)-.1137,
	    (float)-.136,(float)-.1682,(float).0733,(float).07463,(float)
	    .07486,(float).07522,(float).07449,(float).07387,(float).07299,(
	    float).07168,(float).06964,(float).06693,(float).06423,(float)
	    .06255,(float).05756,(float).0502,(float).01493,(float)-.04453,(
	    float)-.05941,(float)-.1004,(float)-.1229,(float)-.1513,(float)
	    .07882,(float).08056,(float).08092,(float).08152,(float).08109,(
	    float).08054,(float).07993,(float).07899,(float).07738,(float)
	    .07507,(float).07277,(float).07126,(float).06701,(float).06063,(
	    float).02884,(float)-.03018,(float)-.04468,(float)-.08612,(float)
	    -.1089,(float)-.1335,(float).08499,(float).08714,(float).08762,(
	    float).08848,(float).08833,(float).08792,(float).08757,(float)
	    .08699,(float).08581,(float).08392,(float).08204,(float).08076,(
	    float).07723,(float).07189,(float).04377,(float)-.01472,(float)
	    -.02888,(float)-.07079,(float)-.09387,(float)-.1146,(float).09188,
	    (float).09447,(float).09504,(float).09621,(float).09631,(float)
	    .0961,(float).09601,(float).09579,(float).09503,(float).0936,(
	    float).09214,(float).09113,(float).08834,(float).08408,(float)
	    .05984,(float).001974,(float)-.01189,(float)-.0543,(float)-.07772,
	    (float)-.09463,(float).09958,(float).1026,(float).1033,(float)
	    .1048,(float).1051,(float).1052,(float).1053,(float).1055,(float)
	    .1051,(float).1042,(float).1032,(float).1025,(float).1004,(float)
	    .09732,(float).07724,(float).01998,(float).006459,(float)-.03653,(
	    float)-.06037,(float)-.07341,(float).1082,(float).1117,(float)
	    .1124,(float).1143,(float).1149,(float).1152,(float).1157,(float)
	    .1163,(float).1163,(float).1159,(float).1153,(float).115,(float)
	    .1137,(float).1118,(float).0961,(float).03942,(float).0263,(float)
	    -.01736,(float)-.0417,(float)-.05089,(float).1177,(float).1218,(
	    float).1227,(float).1249,(float).1258,(float).1263,(float).1271,(
	    float).1281,(float).1287,(float).1289,(float).1287,(float).1288,(
	    float).1283,(float).1277,(float).1165,(float).06046,(float).04776,
	    (float).00335,(float)-.02161,(float)-.02699,(float).1283,(float)
	    .133,(float).1341,(float).1367,(float).138,(float).1386,(float)
	    .1398,(float).1413,(float).1424,(float).1432,(float).1436,(float)
	    .1439,(float).1444,(float).1452,(float).1386,(float).08329,(float)
	    .07101,(float).02573,(float)3.854e-5,(float)-.001594,(float).1402,
	    (float).1456,(float).1468,(float).1498,(float).1515,(float).1524,(
	    float).1539,(float).1559,(float).1577,(float).1591,(float).16,(
	    float).1606,(float).1622,(float).1643,(float).1627,(float).1081,(
	    float).09619,(float).04993,(float).02337,(float).0254,(float)
	    .1535,(float).1596,(float).161,(float).1645,(float).1665,(float)
	    .1677,(float).1697,(float).1722,(float).1746,(float).1766,(float)
	    .1782,(float).1792,(float).1817,(float).1854,(float).189,(float)
	    .1351,(float).1235,(float).07616,(float).04855,(float).05413,(
	    float).1607,(float).1672,(float).1687,(float).1725,(float).1747,(
	    float).176,(float).1782,(float).181,(float).1837,(float).1861,(
	    float).188,(float).1892,(float).1923,(float).1968,(float).203,(
	    float).1495,(float).1381,(float).09008,(float).06188,(float)
	    .06918,(float).1683,(float).1752,(float).1768,(float).1809,(float)
	    .1833,(float).1848,(float).1872,(float).1904,(float).1933,(float)
	    .1961,(float).1983,(float).1998,(float).2034,(float).2087,(float)
	    .2177,(float).1645,(float).1532,(float).1046,(float).07573,(float)
	    .08472,(float).185,(float).1927,(float).1946,(float).1991,(float)
	    .2021,(float).2039,(float).2068,(float).2106,(float).2143,(float)
	    .2179,(float).2207,(float).2227,(float).2274,(float).2344,(float)
	    .2491,(float).1965,(float).1856,(float).1354,(float).1051,(float)
	    .1173,(float).2036,(float).2123,(float).2144,(float).2196,(float)
	    .2232,(float).2253,(float).2287,(float).2333,(float).2377,(float)
	    .2421,(float).2457,(float).2481,(float).2541,(float).2629,(float)
	    .2835,(float).2314,(float).2209,(float).1689,(float).1368,(float)
	    .1521,(float).2244,(float).2343,(float).2367,(float).2426,(float)
	    .2467,(float).2493,(float).2532,(float).2586,(float).2638,(float)
	    .2691,(float).2735,(float).2765,(float).2838,(float).2945,(float)
	    .3211,(float).2696,(float).2593,(float).2052,(float).1712,(float)
	    .1893,(float).2478,(float).259,(float).2617,(float).2684,(float)
	    .2731,(float).2761,(float).2807,(float).2869,(float).2931,(float)
	    .2994,(float).3046,(float).3081,(float).3168,(float).3295,(float)
	    .3623,(float).3113,(float).3012,(float).2448,(float).2084,(float)
	    .229,(float).2741,(float).2868,(float).2898,(float).2974,(float)
	    .3028,(float).3063,(float).3116,(float).3186,(float).3258,(float)
	    .3332,(float).3393,(float).3433,(float).3536,(float).3684,(float)
	    .4076,(float).3569,(float).347,(float).2878,(float).2487,(float)
	    .2716,(float).3037,(float).318,(float).3214,(float).33,(float)
	    .3362,(float).3402,(float).3463,(float).3542,(float).3626,(float)
	    .3711,(float).3781,(float).3827,(float).3946,(float).4116,(float)
	    .4572,(float).4069,(float).3969,(float).3347,(float).2924,(float)
	    .3172,(float).3371,(float).3533,(float).3571,(float).3668,(float)
	    .3737,(float).3784,(float).3853,(float).3943,(float).4038,(float)
	    .4136,(float).4216,(float).4268,(float).4403,(float).4596,(float)
	    .5118,(float).4615,(float).4516,(float).3858,(float).3399,(float)
	    .3663,(float).3747,(float).393,(float).3973,(float).4082,(float)
	    .4161,(float).4213,(float).4292,(float).4393,(float).4501,(float)
	    .4612,(float).4703,(float).4762,(float).4914,(float).5131,(float)
	    .5717,(float).5214,(float).5115,(float).4416,(float).3915,(float)
	    .4191,(float).4173,(float).4378,(float).4427,(float).4549,(float)
	    .464,(float).4698,(float).4786,(float).4902,(float).5023,(float)
	    .5147,(float).525,(float).5315,(float).5487,(float).5728,(float)
	    .6376,(float).5872,(float).577,(float).5025,(float).4477,(float)
	    .476,(float).4654,(float).4886,(float).494,(float).5077,(float)
	    .518,(float).5245,(float).5344,(float).5475,(float).561,(float)
	    .5748,(float).5864,(float).5936,(float).6127,(float).6391,(float)
	    .7102,(float).6594,(float).6487,(float).569,(float).5089,(float)
	    .5375,(float).52,(float).5461,(float).5522,(float).5675,(float)
	    .5792,(float).5865,(float).5976,(float).6121,(float).6271,(float)
	    .6424,(float).6552,(float).6634,(float).6842,(float).713,(float)
	    .7899,(float).7386,(float).7273,(float).6418,(float).5756,(float)
	    .6042,(float).582,(float).6114,(float).6183,(float).6353,(float)
	    .6484,(float).6567,(float).669,(float).6851,(float).7017,(float)
	    .7186,(float).7327,(float).7417,(float).7644,(float).7955,(float)
	    .8775,(float).8256,(float).8135,(float).7216,(float).6486,(float)
	    .6765,(float).6525,(float).6857,(float).6933,(float).7124,(float)
	    .7269,(float).7362,(float).75,(float).7675,(float).7859,(float)
	    .8045,(float).8199,(float).8296,(float).8543,(float).8877,(float)
	    .9738,(float).9211,(float).9082,(float).809,(float).7285,(float)
	    .7554,(float).7328,(float).7702,(float).7787,(float).8,(float)
	    .816,(float).8264,(float).8417,(float).8608,(float).881,(float)
	    .9014,(float).918,(float).9285,(float).955,(float).9905,(float)
	    1.08,(float)1.026,(float)1.012,(float).905,(float).816,(float)
	    .8415,(float).8244,(float).8665,(float).876,(float).8996,(float)
	    .9173,(float).9289,(float).9457,(float).9665,(float).9885,(float)
	    1.011,(float)1.028,(float)1.04,(float)1.068,(float)1.105,(float)
	    1.196,(float)1.141,(float)1.126,(float)1.01,(float).9123,(float)
	    .9359,(float).9291,(float).9763,(float).9869,(float)1.013,(float)
	    1.033,(float)1.045,(float)1.064,(float)1.086,(float)1.11,(float)
	    1.134,(float)1.153,(float)1.165,(float)1.195,(float)1.233,(float)
	    1.324,(float)1.268,(float)1.251,(float)1.127,(float)1.018,(float)
	    1.04,(float)1.049,(float)1.102,(float)1.114,(float)1.142,(float)
	    1.164,(float)1.178,(float)1.198,(float)1.222,(float)1.248,(float)
	    1.273,(float)1.293,(float)1.305,(float)1.337,(float)1.376,(float)
	    1.464,(float)1.407,(float)1.388,(float)1.255,(float)1.135,(float)
	    1.154,(float)1.186,(float)1.245,(float)1.258,(float)1.29,(float)
	    1.314,(float)1.328,(float)1.35,(float)1.376,(float)1.404,(float)
	    1.43,(float)1.451,(float)1.464,(float)1.496,(float)1.535,(float)
	    1.618,(float)1.561,(float)1.539,(float)1.396,(float)1.265,(float)
	    1.281,(float)1.344,(float)1.41,(float)1.424,(float)1.458,(float)
	    1.484,(float)1.5,(float)1.523,(float)1.551,(float)1.58,(float)
	    1.607,(float)1.629,(float)1.642,(float)1.674,(float)1.712,(float)
	    1.786,(float)1.729,(float)1.705,(float)1.552,(float)1.409,(float)
	    1.423,(float)1.526,(float)1.598,(float)1.614,(float)1.65,(float)
	    1.678,(float)1.696,(float)1.72,(float)1.749,(float)1.779,(float)
	    1.807,(float)1.829,(float)1.842,(float)1.873,(float)1.909,(float)
	    1.971,(float)1.913,(float)1.887,(float)1.725,(float)1.57,(float)
	    1.581,(float)1.736,(float)1.814,(float)1.831,(float)1.87,(float)
	    1.9,(float)1.919,(float)1.944,(float)1.974,(float)2.004,(float)
	    2.033,(float)2.054,(float)2.067,(float)2.096,(float)2.128,(float)
	    2.172,(float)2.115,(float)2.086,(float)1.918,(float)1.751,(float)
	    1.758,(float)1.977,(float)2.063,(float)2.081,(float)2.122,(float)
	    2.154,(float)2.173,(float)2.199,(float)2.229,(float)2.259,(float)
	    2.287,(float)2.307,(float)2.318,(float)2.345,(float)2.371,(float)
	    2.391,(float)2.337,(float)2.305,(float)2.132,(float)1.954,(float)
	    1.958,(float)2.257,(float)2.348,(float)2.367,(float)2.41,(float)
	    2.442,(float)2.462,(float)2.488,(float)2.517,(float)2.546,(float)
	    2.572,(float)2.59,(float)2.6,(float)2.622,(float)2.64,(float)
	    2.629,(float)2.58,(float)2.545,(float)2.371,(float)2.185,(float)
	    2.185,(float)2.58,(float)2.677,(float)2.696,(float)2.74,(float)
	    2.772,(float)2.791,(float)2.817,(float)2.843,(float)2.87,(float)
	    2.893,(float)2.908,(float)2.915,(float)2.931,(float)2.938,(float)
	    2.887,(float)2.845,(float)2.808,(float)2.639,(float)2.447,(float)
	    2.443,(float)2.956,(float)3.055,(float)3.074,(float)3.116,(float)
	    3.147,(float)3.166,(float)3.189,(float)3.212,(float)3.234,(float)
	    3.252,(float)3.262,(float)3.266,(float)3.272,(float)3.264,(float)
	    3.165,(float)3.134,(float)3.096,(float)2.939,(float)2.749,(float)
	    2.737,(float)3.392,(float)3.491,(float)3.509,(float)3.547,(float)
	    3.575,(float)3.591,(float)3.611,(float)3.627,(float)3.643,(float)
	    3.653,(float)3.656,(float)3.656,(float)3.649,(float)3.623,(float)
	    3.465,(float)3.449,(float)3.411,(float)3.277,(float)3.097,(float)
	    3.073,(float)3.901,(float)3.992,(float)4.008,(float)4.039,(float)
	    4.061,(float)4.074,(float)4.087,(float)4.095,(float)4.1,(float)
	    4.1,(float)4.094,(float)4.088,(float)4.065,(float)4.014,(float)
	    3.787,(float)3.791,(float)3.754,(float)3.659,(float)3.501,(float)
	    3.459,(float)4.494,(float)4.57,(float)4.581,(float)4.6,(float)
	    4.614,(float)4.621,(float)4.624,(float)4.619,(float)4.61,(float)
	    4.596,(float)4.578,(float)4.563,(float)4.52,(float)4.439,(float)
	    4.129,(float)4.161,(float)4.127,(float)4.091,(float)3.975,(float)
	    3.903,(float)5.188,(float)5.236,(float)5.24,(float)5.24,(float)
	    5.24,(float)5.238,(float)5.228,(float)5.204,(float)5.177,(float)
	    5.143,(float)5.109,(float)5.085,(float)5.015,(float)4.898,(float)
	    4.493,(float)4.561,(float)4.534,(float)4.582,(float)4.533,(float)
	    4.412,(float)6.001,(float)6.004,(float)5.997,(float)5.969,(float)
	    5.948,(float)5.934,(float)5.905,(float)5.856,(float)5.803,(float)
	    5.745,(float)5.691,(float)5.654,(float)5.552,(float)5.391,(float)
	    4.876,(float)4.991,(float)4.975,(float)5.141,(float)5.195,(float)
	    4.996,(float)6.955,(float)6.888,(float)6.864,(float)6.796,(float)
	    6.747,(float)6.715,(float)6.66,(float)6.577,(float)6.492,(float)
	    6.403,(float)6.323,(float)6.27,(float)6.129,(float)5.916,(float)
	    5.278,(float)5.453,(float)5.452,(float)5.776,(float)5.981,(float)
	    5.663,(float)8.078,(float)7.905,(float)7.858,(float)7.733,(float)
	    7.644,(float)7.588,(float)7.497,(float)7.371,(float)7.244,(float)
	    7.115,(float)7.004,(float)6.932,(float)6.744,(float)6.471,(float)
	    5.699,(float)5.949,(float)5.968,(float)6.499,(float)6.917,(float)
	    6.418,(float)9.399,(float)9.071,(float)8.991,(float)8.788,(float)
	    8.645,(float)8.556,(float)8.42,(float)8.238,(float)8.058,(float)
	    7.881,(float)7.732,(float)7.637,(float)7.395,(float)7.054,(float)
	    6.137,(float)6.48,(float)6.526,(float)7.321,(float)8.026,(float)
	    7.263,(float)10.95,(float)10.4,(float)10.28,(float)9.97,(float)
	    9.754,(float)9.623,(float)9.427,(float)9.177,(float)8.932,(float)
	    8.695,(float)8.503,(float)8.381,(float)8.078,(float)7.66,(float)
	    6.593,(float)7.049,(float)7.131,(float)8.249,(float)9.33,(float)
	    8.19,(float)12.76,(float)11.91,(float)11.72,(float)11.28,(float)
	    10.97,(float)10.79,(float)10.52,(float)10.18,(float)9.86,(float)
	    9.554,(float)9.312,(float)9.161,(float)8.789,(float)8.289,(float)
	    7.071,(float)7.661,(float)7.787,(float)9.29,(float)10.84,(float)
	    9.182,(float)14.86,(float)13.6,(float)13.34,(float)12.72,(float)
	    12.29,(float)12.05,(float)11.69,(float)11.25,(float)10.84,(float)
	    10.46,(float)10.16,(float)9.979,(float)9.531,(float)8.945,(float)
	    7.579,(float)8.324,(float)8.497,(float)10.44,(float)12.53,(float)
	    10.2,(float)17.27,(float)15.49,(float)15.12,(float)14.29,(float)
	    13.73,(float)13.41,(float)12.94,(float)12.4,(float)11.88,(float)
	    11.42,(float)11.06,(float)10.85,(float)10.32,(float)9.644,(float)
	    8.125,(float)9.038,(float)9.256,(float)11.66,(float)14.34,(float)
	    11.18,(float)20.06,(float)17.63,(float)17.15,(float)16.07,(float)
	    15.35,(float)14.95,(float)14.36,(float)13.7,(float)13.06,(float)
	    12.49,(float)12.07,(float)11.83,(float)11.2,(float)10.42,(float)
	    8.71,(float)9.78,(float)10.03,(float)12.86,(float)16.09,(float)
	    12.03,(float)23.72,(float)20.43,(float)19.78,(float)18.36,(float)
	    17.42,(float)16.89,(float)16.13,(float)15.29,(float)14.48,(float)
	    13.78,(float)13.26,(float)12.96,(float)12.2,(float)11.26,(float)
	    9.26,(float)10.43,(float)10.69,(float)13.83,(float)17.44,(float)
	    12.61,(float)26.98,(float)22.69,(float)21.87,(float)20.08,(float)
	    18.91,(float)18.25,(float)17.32,(float)16.32,(float)15.35,(float)
	    14.52,(float)13.92,(float)13.58,(float)12.71,(float)11.65,(float)
	    9.472,(float)10.66,(float)10.91,(float)14.15,(float)17.85,(float)
	    12.77 };

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
/* Background desert model */
/* Parameters and refractive indices - G.A. d'Almeida, 'Atmospheric */
/* aerosols. Global climatology and radiative characteristics', 1991, */
/* pp.48,80,102. */
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
/*<        ph(i,j)=phr(i,j) >*/
	    sixs_aerbas__1.ph[i__ + j * 20 - 21] = phr[i__ + j * 20 - 21];
/*<        qh(i,j)=qhr(i,j) >*/
	    sixs_aerbas__1.qh[i__ + j * 20 - 21] = qhr[i__ + j * 20 - 21];
/*<        uh(i,j)=uhr(i,j) >*/
	    sixs_aerbas__1.uh[i__ + j * 20 - 21] = uhr[i__ + j * 20 - 21];
/*<     1 continue     >*/
/* L1: */
	}
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* bdm_ */

#ifdef __cplusplus
	}
#endif
