/* WATE.f -- translated by f2c (version 19970805).
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

/*<        subroutine   wate >*/
/* Subroutine */ int wate_()
{
    /* Initialized data */

    static real asy_m__[20] = { (float).663,(float).657,(float).655,(float)
	    .651,(float).648,(float).645,(float).642,(float).638,(float).633,(
	    float).628,(float).623,(float).62,(float).613,(float).605,(float)
	    .563,(float).549,(float).535,(float).493,(float).469,(float).369 }
	    ;
    static real ex_m__[20] = { (float).01668202,(float).01485427,(float)
	    .01444705,(float).01344278,(float).0126322,(float).01211881,(
	    float).01139759,(float).01053951,(float).009646374,(float)
	    .008791811,(float).008128974,(float).007729647,(float).006740132,(
	    float).005455605,(float).00283833,(float).001442047,(float)
	    .001220081,(float).001323107,(float)8.564052e-4,(float)
	    1.457627e-4 };
    static real sc_m__[20] = { (float).01611682,(float).01436933,(float)
	    .0139778,(float).01301239,(float).01223016,(float).0117355,(float)
	    .0110181,(float).01014493,(float).009284852,(float).008424427,(
	    float).007772448,(float).007389077,(float).006363583,(float)
	    .005064777,(float).002432695,(float).001098548,(float)9.381454e-4,
	    (float)5.656477e-4,(float)4.127454e-4,(float)1.24475e-4 };
    static real phr[1660]	/* was [20][83] */ = { (float).4078,(float)
	    .3768,(float).3723,(float).3587,(float).3523,(float).3444,(float)
	    .3349,(float).3249,(float).3184,(float).3086,(float).3051,(float)
	    .3033,(float).2928,(float).2827,(float).3014,(float).3262,(float)
	    .3448,(float).3923,(float).4345,(float).6107,(float).4008,(float)
	    .3712,(float).3669,(float).3539,(float).3477,(float).3402,(float)
	    .3311,(float).3214,(float).3153,(float).306,(float).3027,(float)
	    .3011,(float).291,(float).2814,(float).3009,(float).3259,(float)
	    .3446,(float).3921,(float).4343,(float).6105,(float).3754,(float)
	    .3505,(float).3467,(float).3359,(float).3304,(float).3244,(float)
	    .3167,(float).3083,(float).3034,(float).2958,(float).2933,(float)
	    .2923,(float).2838,(float).2762,(float).2986,(float).3249,(float)
	    .3435,(float).3915,(float).4335,(float).6093,(float).3409,(float)
	    .3219,(float).3189,(float).3107,(float).3062,(float).3021,(float)
	    .2962,(float).2894,(float).2863,(float).281,(float).2795,(float)
	    .2793,(float).2731,(float).2683,(float).295,(float).3232,(float)
	    .3418,(float).3903,(float).4322,(float).6073,(float).3069,(float)
	    .2929,(float).2907,(float).2848,(float).2815,(float).2789,(float)
	    .2748,(float).2697,(float).2683,(float).265,(float).2646,(float)
	    .2651,(float).2614,(float).2596,(float).2907,(float).321,(float)
	    .3396,(float).3888,(float).4304,(float).6045,(float).279,(float)
	    .2686,(float).2671,(float).2629,(float).2607,(float).259,(float)
	    .2562,(float).2525,(float).2524,(float).2507,(float).2512,(float)
	    .2523,(float).2506,(float).2514,(float).2863,(float).3186,(float)
	    .3371,(float).3868,(float).4281,(float).601,(float).2597,(float)
	    .2513,(float).2502,(float).2469,(float).2455,(float).2442,(float)
	    .2422,(float).2395,(float).2402,(float).2395,(float).2406,(float)
	    .242,(float).2418,(float).2446,(float).2822,(float).3161,(float)
	    .3344,(float).3846,(float).4254,(float).5968,(float).2483,(float)
	    .2406,(float).2396,(float).2368,(float).2356,(float).2346,(float)
	    .2329,(float).2309,(float).2319,(float).2316,(float).233,(float)
	    .2346,(float).2353,(float).2394,(float).2785,(float).3133,(float)
	    .3315,(float).382,(float).4224,(float).5921,(float).2419,(float)
	    .2345,(float).2335,(float).2308,(float).2296,(float).2286,(float)
	    .2272,(float).2253,(float).2264,(float).2263,(float).2279,(float)
	    .2294,(float).2306,(float).2354,(float).2752,(float).3105,(float)
	    .3285,(float).3791,(float).419,(float).5869,(float).2373,(float)
	    .2303,(float).2294,(float).2267,(float).2256,(float).2245,(float)
	    .2232,(float).2214,(float).2225,(float).2226,(float).2241,(float)
	    .2256,(float).2272,(float).2323,(float).2722,(float).3074,(float)
	    .3252,(float).376,(float).4154,(float).5811,(float).2327,(float)
	    .2263,(float).2254,(float).2229,(float).2219,(float).2209,(float)
	    .2197,(float).2181,(float).2193,(float).2194,(float).221,(float)
	    .2225,(float).2242,(float).2295,(float).2693,(float).3042,(float)
	    .3218,(float).3727,(float).4114,(float).575,(float).227,(float)
	    .2215,(float).2207,(float).2186,(float).2177,(float).217,(float)
	    .216,(float).2147,(float).2159,(float).2163,(float).2179,(float)
	    .2194,(float).2214,(float).2268,(float).2665,(float).3008,(float)
	    .3182,(float).3692,(float).4073,(float).5684,(float).2204,(float)
	    .2157,(float).2151,(float).2135,(float).2129,(float).2125,(float)
	    .2118,(float).2108,(float).2122,(float).2129,(float).2147,(float)
	    .2163,(float).2184,(float).224,(float).2637,(float).2973,(float)
	    .3144,(float).3655,(float).403,(float).5615,(float).2128,(float)
	    .2093,(float).2088,(float).2078,(float).2076,(float).2075,(float)
	    .2071,(float).2066,(float).2082,(float).2092,(float).2111,(float)
	    .2128,(float).2153,(float).2211,(float).2608,(float).2937,(float)
	    .3106,(float).3617,(float).3985,(float).5543,(float).2049,(float)
	    .2026,(float).2023,(float).2019,(float).202,(float).2021,(float)
	    .2021,(float).2021,(float).2039,(float).2053,(float).2075,(float)
	    .2093,(float).2121,(float).2182,(float).2579,(float).2901,(float)
	    .3067,(float).3579,(float).3939,(float).5469,(float).1969,(float)
	    .1958,(float).1958,(float).1958,(float).1963,(float).1967,(float)
	    .197,(float).1974,(float).1996,(float).2014,(float).2038,(float)
	    .2056,(float).2089,(float).2152,(float).2551,(float).2865,(float)
	    .3029,(float).354,(float).3893,(float).5393,(float).1894,(float)
	    .1893,(float).1895,(float).19,(float).1909,(float).1914,(float)
	    .1921,(float).1929,(float).1954,(float).1975,(float).2001,(float)
	    .2021,(float).2057,(float).2122,(float).2523,(float).2829,(float)
	    .2991,(float).3501,(float).3847,(float).5316,(float).1825,(float)
	    .1833,(float).1836,(float).1846,(float).1857,(float).1865,(float)
	    .1875,(float).1887,(float).1914,(float).1939,(float).1966,(float)
	    .1987,(float).2027,(float).2095,(float).2497,(float).2796,(float)
	    .2955,(float).3463,(float).3801,(float).5239,(float).1763,(float)
	    .1779,(float).1783,(float).1797,(float).181,(float).1821,(float)
	    .1833,(float).1848,(float).1878,(float).1906,(float).1935,(float)
	    .1956,(float).1999,(float).2069,(float).2473,(float).2764,(float)
	    .292,(float).3427,(float).3756,(float).5163,(float).171,(float)
	    .1731,(float).1736,(float).1754,(float).1769,(float).1781,(float)
	    .1796,(float).1814,(float).1846,(float).1876,(float).1907,(float)
	    .1929,(float).1974,(float).2047,(float).2451,(float).2734,(float)
	    .2888,(float).3392,(float).3713,(float).5087,(float).1664,(float)
	    .169,(float).1696,(float).1717,(float).1734,(float).1748,(float)
	    .1765,(float).1785,(float).1819,(float).1851,(float).1883,(float)
	    .1907,(float).1954,(float).2028,(float).2433,(float).2707,(float)
	    .2858,(float).3359,(float).3673,(float).5014,(float).1627,(float)
	    .1657,(float).1664,(float).1687,(float).1705,(float).1721,(float)
	    .1739,(float).1762,(float).1797,(float).1831,(float).1865,(float)
	    .1889,(float).1938,(float).2014,(float).2418,(float).2683,(float)
	    .2832,(float).3329,(float).3635,(float).4943,(float).1598,(float)
	    .1631,(float).1639,(float).1663,(float).1684,(float).17,(float)
	    .172,(float).1745,(float).1781,(float).1817,(float).1851,(float)
	    .1876,(float).1927,(float).2004,(float).2408,(float).2664,(float)
	    .2809,(float).3302,(float).36,(float).4876,(float).1576,(float)
	    .1613,(float).1622,(float).1647,(float).1669,(float).1686,(float)
	    .1707,(float).1734,(float).1771,(float).1809,(float).1844,(float)
	    .1869,(float).1922,(float).2,(float).2402,(float).2648,(float)
	    .2791,(float).328,(float).357,(float).4814,(float).1561,(float)
	    .1601,(float).1612,(float).1638,(float).1662,(float).1678,(float)
	    .1701,(float).1729,(float).1768,(float).1807,(float).1843,(float)
	    .1867,(float).1922,(float).2001,(float).2401,(float).2638,(float)
	    .2778,(float).3262,(float).3545,(float).4757,(float).1555,(float)
	    .1598,(float).1609,(float).1636,(float).1662,(float).1678,(float)
	    .1701,(float).1731,(float).1771,(float).1811,(float).1848,(float)
	    .1873,(float).1929,(float).2008,(float).2407,(float).2633,(float)
	    .2771,(float).325,(float).3526,(float).4706,(float).1556,(float)
	    .1602,(float).1613,(float).1642,(float).1668,(float).1685,(float)
	    .1709,(float).174,(float).1781,(float).1822,(float).1859,(float)
	    .1884,(float).1942,(float).2022,(float).2419,(float).2635,(float)
	    .277,(float).3244,(float).3513,(float).4663,(float).1565,(float)
	    .1614,(float).1625,(float).1655,(float).1681,(float).1699,(float)
	    .1725,(float).1756,(float).1798,(float).184,(float).1878,(float)
	    .1903,(float).1962,(float).2042,(float).2439,(float).2643,(float)
	    .2776,(float).3245,(float).3507,(float).4628,(float).1583,(float)
	    .1633,(float).1645,(float).1676,(float).1703,(float).1721,(float)
	    .1747,(float).178,(float).1823,(float).1866,(float).1904,(float)
	    .193,(float).199,(float).2071,(float).2466,(float).2659,(float)
	    .279,(float).3254,(float).351,(float).4603,(float).1608,(float)
	    .166,(float).1672,(float).1704,(float).1732,(float).175,(float)
	    .1778,(float).1811,(float).1855,(float).1899,(float).1939,(float)
	    .1965,(float).2026,(float).2107,(float).2502,(float).2683,(float)
	    .2812,(float).3272,(float).3522,(float).4587,(float).1641,(float)
	    .1694,(float).1707,(float).174,(float).1769,(float).1788,(float)
	    .1816,(float).1852,(float).1896,(float).1941,(float).1982,(float)
	    .2008,(float).207,(float).2152,(float).2547,(float).2717,(float)
	    .2844,(float).33,(float).3544,(float).4584,(float).1683,(float)
	    .1738,(float).1751,(float).1785,(float).1816,(float).1835,(float)
	    .1864,(float).1901,(float).1946,(float).1993,(float).2034,(float)
	    .2061,(float).2125,(float).2207,(float).2603,(float).276,(float)
	    .2886,(float).3338,(float).3577,(float).4593,(float).1734,(float)
	    .179,(float).1804,(float).1839,(float).1871,(float).1891,(float)
	    .1921,(float).196,(float).2006,(float).2054,(float).2096,(float)
	    .2124,(float).2189,(float).2273,(float).267,(float).2815,(float)
	    .294,(float).3388,(float).3623,(float).4617,(float).1794,(float)
	    .1853,(float).1867,(float).1904,(float).1937,(float).1957,(float)
	    .1989,(float).2028,(float).2076,(float).2125,(float).2169,(float)
	    .2197,(float).2264,(float).2349,(float).2749,(float).2882,(float)
	    .3007,(float).3451,(float).3682,(float).4655,(float).1864,(float)
	    .1926,(float).1941,(float).1979,(float).2013,(float).2034,(float)
	    .2067,(float).2108,(float).2158,(float).2209,(float).2254,(float)
	    .2283,(float).2351,(float).2438,(float).2842,(float).2963,(float)
	    .3087,(float).3529,(float).3756,(float).4711,(float).1945,(float)
	    .2009,(float).2026,(float).2065,(float).2101,(float).2123,(float)
	    .2157,(float).22,(float).2252,(float).2304,(float).2351,(float)
	    .2381,(float).2452,(float).254,(float).295,(float).3059,(float)
	    .3182,(float).3622,(float).3847,(float).4784,(float).2038,(float)
	    .2105,(float).2122,(float).2163,(float).2202,(float).2224,(float)
	    .226,(float).2306,(float).2359,(float).2414,(float).2462,(float)
	    .2493,(float).2566,(float).2657,(float).3073,(float).317,(float)
	    .3293,(float).3733,(float).3955,(float).4877,(float).2145,(float)
	    .2214,(float).2232,(float).2276,(float).2316,(float).234,(float)
	    .2377,(float).2425,(float).2481,(float).2538,(float).2589,(float)
	    .2621,(float).2697,(float).279,(float).3215,(float).3299,(float)
	    .3423,(float).3863,(float).4082,(float).499,(float).2265,(float)
	    .2338,(float).2357,(float).2403,(float).2446,(float).2471,(float)
	    .251,(float).2561,(float).2619,(float).2679,(float).2732,(float)
	    .2765,(float).2844,(float).2941,(float).3376,(float).3448,(float)
	    .3573,(float).4012,(float).4231,(float).5126,(float).24,(float)
	    .2479,(float).2499,(float).2548,(float).2592,(float).2619,(float)
	    .2661,(float).2714,(float).2775,(float).2838,(float).2893,(float)
	    .2928,(float).3011,(float).3112,(float).3558,(float).3618,(float)
	    .3744,(float).4185,(float).4402,(float).5286,(float).2553,(float)
	    .2638,(float).2659,(float).2711,(float).2757,(float).2786,(float)
	    .283,(float).2886,(float).2951,(float).3017,(float).3075,(float)
	    .3112,(float).3199,(float).3303,(float).3763,(float).3812,(float)
	    .3939,(float).4381,(float).4599,(float).5471,(float).2636,(float)
	    .2724,(float).2746,(float).28,(float).2847,(float).2877,(float)
	    .2923,(float).298,(float).3046,(float).3115,(float).3174,(float)
	    .3212,(float).3301,(float).3408,(float).3875,(float).3918,(float)
	    .4046,(float).449,(float).4707,(float).5574,(float).2725,(float)
	    .2816,(float).2838,(float).2894,(float).2942,(float).2974,(float)
	    .302,(float).3079,(float).3148,(float).3219,(float).3279,(float)
	    .3318,(float).3409,(float).3519,(float).3993,(float).4031,(float)
	    .416,(float).4605,(float).4822,(float).5684,(float).2919,(float)
	    .3015,(float).3038,(float).3098,(float).3151,(float).3185,(float)
	    .3234,(float).3297,(float).337,(float).3444,(float).3508,(float)
	    .355,(float).3646,(float).3761,(float).4252,(float).4278,(float)
	    .4409,(float).4856,(float).5074,(float).5926,(float).3137,(float)
	    .3239,(float).3264,(float).3328,(float).3385,(float).3421,(float)
	    .3473,(float).3542,(float).3618,(float).3697,(float).3766,(float)
	    .3809,(float).3911,(float).4032,(float).4541,(float).4556,(float)
	    .4689,(float).514,(float).5357,(float).6198,(float).338,(float)
	    .349,(float).3518,(float).3586,(float).3648,(float).3686,(float)
	    .3742,(float).3816,(float).3897,(float).3981,(float).4053,(float)
	    .4099,(float).4207,(float).4335,(float).4864,(float).4867,(float)
	    .5002,(float).5456,(float).5674,(float).6503,(float).3653,(float)
	    .3773,(float).3803,(float).3877,(float).3943,(float).3983,(float)
	    .4044,(float).4122,(float).4209,(float).4299,(float).4375,(float)
	    .4424,(float).4538,(float).4673,(float).5223,(float).5216,(float)
	    .5353,(float).581,(float).6028,(float).6842,(float).3959,(float)
	    .4091,(float).4124,(float).4203,(float).4273,(float).4317,(float)
	    .4382,(float).4465,(float).4559,(float).4654,(float).4736,(float)
	    .4787,(float).4908,(float).5051,(float).5623,(float).5604,(float)
	    .5744,(float).6204,(float).6421,(float).7218,(float).4304,(float)
	    .4448,(float).4482,(float).4568,(float).4643,(float).4691,(float)
	    .4761,(float).485,(float).495,(float).5052,(float).5138,(float)
	    .5193,(float).5321,(float).5472,(float).6067,(float).6037,(float)
	    .6179,(float).664,(float).6857,(float).7633,(float).4693,(float)
	    .4848,(float).4885,(float).4978,(float).5058,(float).5111,(float)
	    .5186,(float).5281,(float).5387,(float).5497,(float).5588,(float)
	    .5646,(float).5782,(float).5942,(float).656,(float).6519,(float)
	    .6662,(float).7123,(float).7338,(float).8087,(float).513,(float)
	    .5297,(float).5337,(float).5438,(float).5524,(float).5582,(float)
	    .5662,(float).5764,(float).5877,(float).5994,(float).6091,(float)
	    .6153,(float).6297,(float).6466,(float).7106,(float).7053,(float)
	    .7198,(float).7657,(float).7869,(float).8584,(float).562,(float)
	    .5803,(float).5845,(float).5955,(float).6048,(float).611,(float)
	    .6196,(float).6305,(float).6426,(float).6551,(float).6653,(float)
	    .6719,(float).6871,(float).7049,(float).7711,(float).7646,(float)
	    .7791,(float).8245,(float).8452,(float).9125,(float).617,(float)
	    .6371,(float).6418,(float).6537,(float).6637,(float).6702,(float)
	    .6795,(float).6912,(float).7042,(float).7173,(float).7282,(float)
	    .7351,(float).7512,(float).7699,(float).838,(float).8302,(float)
	    .8447,(float).8892,(float).9092,(float).9713,(float).679,(float)
	    .7011,(float).7063,(float).719,(float).7298,(float).7367,(float)
	    .7468,(float).7593,(float).7731,(float).787,(float).7985,(float)
	    .8057,(float).8227,(float).8422,(float).912,(float).9027,(float)
	    .9172,(float).9602,(float).9792,(float)1.035,(float).7492,(float)
	    .7732,(float).7788,(float).7925,(float).8042,(float).8115,(float)
	    .8223,(float).8356,(float).8503,(float).865,(float).8771,(float)
	    .8845,(float).9024,(float).9228,(float).9936,(float).9829,(float)
	    .997,(float)1.038,(float)1.056,(float)1.103,(float).8284,(float)
	    .8544,(float).8605,(float).8753,(float).8878,(float).8956,(float)
	    .9071,(float).9213,(float).9367,(float).9522,(float).9649,(float)
	    .9727,(float).9912,(float)1.012,(float)1.084,(float)1.071,(float)
	    1.085,(float)1.123,(float)1.139,(float)1.177,(float).9179,(float)
	    .9461,(float).9525,(float).9685,(float).9818,(float).9903,(float)
	    1.002,(float)1.017,(float)1.034,(float)1.05,(float)1.063,(float)
	    1.071,(float)1.09,(float)1.112,(float)1.183,(float)1.169,(float)
	    1.182,(float)1.216,(float)1.23,(float)1.256,(float)1.019,(float)
	    1.05,(float)1.056,(float)1.074,(float)1.088,(float)1.097,(float)
	    1.11,(float)1.125,(float)1.142,(float)1.159,(float)1.173,(float)
	    1.181,(float)1.201,(float)1.223,(float)1.293,(float)1.276,(float)
	    1.288,(float)1.318,(float)1.329,(float)1.341,(float)1.134,(float)
	    1.167,(float)1.174,(float)1.192,(float)1.207,(float)1.217,(float)
	    1.23,(float)1.246,(float)1.264,(float)1.281,(float)1.295,(float)
	    1.304,(float)1.324,(float)1.346,(float)1.413,(float)1.395,(float)
	    1.405,(float)1.429,(float)1.436,(float)1.432,(float)1.263,(float)
	    1.299,(float)1.307,(float)1.326,(float)1.341,(float)1.352,(float)
	    1.366,(float)1.382,(float)1.4,(float)1.418,(float)1.432,(float)
	    1.441,(float)1.461,(float)1.483,(float)1.545,(float)1.525,(float)
	    1.533,(float)1.549,(float)1.552,(float)1.528,(float)1.41,(float)
	    1.448,(float)1.456,(float)1.477,(float)1.493,(float)1.503,(float)
	    1.518,(float)1.535,(float)1.553,(float)1.571,(float)1.585,(float)
	    1.594,(float)1.613,(float)1.635,(float)1.691,(float)1.668,(float)
	    1.673,(float)1.68,(float)1.678,(float)1.63,(float)1.576,(float)
	    1.617,(float)1.626,(float)1.647,(float)1.663,(float)1.674,(float)
	    1.689,(float)1.707,(float)1.725,(float)1.743,(float)1.756,(float)
	    1.764,(float)1.783,(float)1.803,(float)1.85,(float)1.824,(float)
	    1.827,(float)1.822,(float)1.813,(float)1.738,(float)1.765,(float)
	    1.808,(float)1.817,(float)1.839,(float)1.856,(float)1.867,(float)
	    1.882,(float)1.899,(float)1.917,(float)1.934,(float)1.947,(float)
	    1.954,(float)1.972,(float)1.99,(float)2.024,(float)1.996,(float)
	    1.994,(float)1.975,(float)1.959,(float)1.852,(float)1.979,(float)
	    2.024,(float)2.033,(float)2.055,(float)2.072,(float)2.083,(float)
	    2.098,(float)2.115,(float)2.131,(float)2.147,(float)2.159,(float)
	    2.166,(float)2.181,(float)2.196,(float)2.215,(float)2.183,(float)
	    2.177,(float)2.14,(float)2.115,(float)1.972,(float)2.222,(float)
	    2.268,(float)2.277,(float)2.299,(float)2.316,(float)2.326,(float)
	    2.34,(float)2.356,(float)2.371,(float)2.385,(float)2.395,(float)
	    2.4,(float)2.413,(float)2.425,(float)2.423,(float)2.387,(float)
	    2.375,(float)2.318,(float)2.283,(float)2.098,(float)2.498,(float)
	    2.544,(float)2.553,(float)2.575,(float)2.59,(float)2.6,(float)
	    2.613,(float)2.626,(float)2.639,(float)2.65,(float)2.658,(float)
	    2.661,(float)2.67,(float)2.676,(float)2.649,(float)2.61,(float)
	    2.59,(float)2.51,(float)2.462,(float)2.229,(float)2.811,(float)
	    2.856,(float)2.865,(float)2.885,(float)2.898,(float)2.907,(float)
	    2.918,(float)2.928,(float)2.938,(float)2.945,(float)2.949,(float)
	    2.95,(float)2.953,(float)2.952,(float)2.893,(float)2.851,(float)
	    2.823,(float)2.714,(float)2.652,(float)2.365,(float)3.167,(float)
	    3.209,(float)3.216,(float)3.233,(float)3.244,(float)3.251,(float)
	    3.259,(float)3.266,(float)3.27,(float)3.272,(float)3.271,(float)
	    3.269,(float)3.265,(float)3.255,(float)3.158,(float)3.112,(float)
	    3.074,(float)2.932,(float)2.854,(float)2.506,(float)3.571,(float)
	    3.607,(float)3.612,(float)3.625,(float)3.631,(float)3.636,(float)
	    3.639,(float)3.641,(float)3.638,(float)3.633,(float)3.626,(float)
	    3.621,(float)3.607,(float)3.587,(float)3.443,(float)3.394,(float)
	    3.343,(float)3.164,(float)3.066,(float)2.651,(float)4.028,(float)
	    4.055,(float)4.058,(float)4.065,(float)4.065,(float)4.065,(float)
	    4.063,(float)4.057,(float)4.046,(float)4.032,(float)4.017,(float)
	    4.007,(float)3.982,(float)3.947,(float)3.747,(float)3.695,(float)
	    3.631,(float)3.409,(float)3.289,(float)2.798,(float)4.548,(float)
	    4.56,(float)4.56,(float)4.557,(float)4.549,(float)4.544,(float)
	    4.534,(float)4.518,(float)4.495,(float)4.47,(float)4.445,(float)
	    4.429,(float)4.389,(float)4.337,(float)4.071,(float)4.016,(float)
	    3.936,(float)3.666,(float)3.522,(float)2.948,(float)5.137,(float)
	    5.128,(float)5.123,(float)5.107,(float)5.088,(float)5.076,(float)
	    5.055,(float)5.027,(float)4.989,(float)4.949,(float)4.912,(float)
	    4.888,(float)4.83,(float)4.757,(float)4.413,(float)4.356,(float)
	    4.257,(float)3.933,(float)3.762,(float)3.099,(float)5.806,(float)
	    5.766,(float)5.753,(float)5.72,(float)5.686,(float)5.664,(float)
	    5.63,(float)5.585,(float)5.528,(float)5.47,(float)5.418,(float)
	    5.384,(float)5.305,(float)5.206,(float)4.772,(float)4.712,(float)
	    4.593,(float)4.209,(float)4.009,(float)3.248,(float)6.564,(float)
	    6.481,(float)6.458,(float)6.401,(float)6.347,(float)6.313,(float)
	    6.261,(float)6.194,(float)6.114,(float)6.033,(float)5.962,(float)
	    5.916,(float)5.811,(float)5.681,(float)5.143,(float)5.081,(float)
	    4.939,(float)4.491,(float)4.258,(float)3.395,(float)7.422,(float)
	    7.279,(float)7.244,(float)7.154,(float)7.074,(float)7.023,(float)
	    6.949,(float)6.855,(float)6.745,(float)6.636,(float)6.542,(float)
	    6.482,(float)6.345,(float)6.178,(float)5.523,(float)5.458,(float)
	    5.291,(float)4.775,(float)4.506,(float)3.538,(float)8.393,(float)
	    8.169,(float)8.115,(float)7.982,(float)7.868,(float)7.797,(float)
	    7.693,(float)7.565,(float)7.418,(float)7.275,(float)7.153,(float)
	    7.077,(float)6.902,(float)6.692,(float)5.905,(float)5.837,(float)
	    5.643,(float)5.056,(float)4.75,(float)3.672,(float)9.49,(float)
	    9.154,(float)9.076,(float)8.887,(float)8.728,(float)8.63,(float)
	    8.49,(float)8.319,(float)8.128,(float)7.942,(float)7.788,(float)
	    7.692,(float)7.473,(float)7.213,(float)6.281,(float)6.209,(float)
	    5.987,(float)5.328,(float)4.983,(float)3.797,(float)10.72,(float)
	    10.23,(float)10.13,(float)9.863,(float)9.649,(float)9.516,(float)
	    9.33,(float)9.107,(float)8.861,(float)8.626,(float)8.434,(float)
	    8.315,(float)8.045,(float)7.728,(float)6.641,(float)6.563,(float)
	    6.312,(float)5.582,(float)5.199,(float)3.909,(float)12.09,(float)
	    11.4,(float)11.25,(float)10.9,(float)10.61,(float)10.43,(float)
	    10.19,(float)9.909,(float)9.6,(float)9.306,(float)9.071,(float)
	    8.925,(float)8.599,(float)8.219,(float)6.971,(float)6.885,(float)
	    6.606,(float)5.809,(float)5.39,(float)4.005,(float)13.58,(float)
	    12.62,(float)12.42,(float)11.95,(float)11.59,(float)11.35,(float)
	    11.04,(float)10.69,(float)10.31,(float)9.948,(float)9.668,(float)
	    9.492,(float)9.107,(float)8.66,(float)7.255,(float)7.159,(float)
	    6.855,(float)6.,(float)5.548,(float)4.082,(float)15.08,(float)
	    13.8,(float)13.55,(float)12.93,(float)12.49,(float)12.19,(float)
	    11.81,(float)11.38,(float)10.93,(float)10.5,(float)10.17,(float)
	    9.968,(float)9.527,(float)9.018,(float)7.473,(float)7.368,(float)
	    7.043,(float)6.143,(float)5.666,(float)4.138,(float)16.32,(float)
	    14.72,(float)14.42,(float)13.68,(float)13.16,(float)12.8,(float)
	    12.36,(float)11.87,(float)11.35,(float)10.87,(float)10.51,(float)
	    10.28,(float)9.801,(float)9.246,(float)7.606,(float)7.493,(float)
	    7.155,(float)6.227,(float)5.734,(float)4.17,(float)16.73,(float)
	    15.01,(float)14.69,(float)13.9,(float)13.35,(float)12.98,(float)
	    12.51,(float)12.,(float)11.47,(float)10.97,(float)10.6,(float)
	    10.37,(float)9.873,(float)9.304,(float)7.638,(float)7.523,(float)
	    7.182,(float)6.248,(float)5.75,(float)4.177 };
    static real qhr[1660]	/* was [20][83] */ = { (float)3.27e-22,(float)
	    3.072e-22,(float)-5.665e-23,(float)-2.285e-22,(float)-1.177e-22,(
	    float)-3.107e-22,(float)-3.274e-22,(float)7.827e-22,(float)
	    1.904e-22,(float)7.601e-22,(float)-8.737e-23,(float)-1.387e-22,(
	    float)-5.055e-22,(float)-6.295e-22,(float)-3.916e-22,(float)
	    -8.098e-22,(float)-3.673e-23,(float)7.005e-22,(float)-7.083e-22,(
	    float)-7.188e-22,(float).003207,(float).002568,(float).002492,(
	    float).002245,(float).002156,(float).00201,(float).001817,(float)
	    .001678,(float).001502,(float).001298,(float).001206,(float)
	    .001144,(float)9.32e-4,(float)7.026e-4,(float)2.488e-4,(float)
	    3.08e-6,(float)-1.188e-5,(float)-1.645e-4,(float)-1.681e-4,(float)
	    -2.182e-4,(float).0153,(float).01248,(float).01212,(float).01099,(
	    float).01051,(float).00986,(float).00897,(float).008272,(float)
	    .007441,(float).00648,(float).006014,(float).005715,(float).00468,
	    (float).003544,(float).001259,(float)-6.454e-6,(float)-8.432e-5,(
	    float)-8.69e-4,(float)-8.896e-4,(float)-.001157,(float).03251,(
	    float).02717,(float).02642,(float).02411,(float).02298,(float)
	    .0217,(float).01991,(float).01831,(float).01659,(float).01459,(
	    float).01353,(float).01287,(float).01062,(float).008087,(float)
	    .002881,(float)-1.116e-4,(float)-2.988e-4,(float)-.002146,(float)
	    -.002204,(float)-.002874,(float).05092,(float).0436,(float).04246,
	    (float).03901,(float).03702,(float).03518,(float).03254,(float)
	    .02982,(float).02725,(float).0242,(float).02241,(float).02135,(
	    float).01774,(float).01358,(float).004825,(float)-4.447e-4,(float)
	    -7.816e-4,(float)-.004009,(float)-.004136,(float)-.005413,(float)
	    .06799,(float).05948,(float).05794,(float).05358,(float).05062,(
	    float).04841,(float).04511,(float).04118,(float).03791,(float)
	    .03396,(float).03142,(float).02998,(float).02507,(float).01926,(
	    float).006761,(float)-.001159,(float)-.001678,(float)-.006477,(
	    float)-.006718,(float)-.008822,(float).08225,(float).07318,(float)
	    .07122,(float).06628,(float).0624,(float).06001,(float).05626,(
	    float).05122,(float).04742,(float).0428,(float).03958,(float)
	    .03782,(float).03178,(float).02445,(float).00836,(float)-.002397,(
	    float)-.003123,(float)-.009571,(float)-.009979,(float)-.01315,(
	    float).0928,(float).08356,(float).08128,(float).07605,(float)
	    .0715,(float).06903,(float).06501,(float).05917,(float).055,(
	    float).04993,(float).04619,(float).0442,(float).03725,(float)
	    .02863,(float).009342,(float)-.00427,(float)-.005222,(float)
	    -.01331,(float)-.01395,(float)-.01842,(float).09921,(float).08997,
	    (float).08756,(float).08227,(float).0774,(float).07494,(float)
	    .07079,(float).06459,(float).06017,(float).05485,(float).05079,(
	    float).04867,(float).04108,(float).03144,(float).009497,(float)
	    -.006842,(float)-.008035,(float)-.01769,(float)-.01864,(float)
	    -.02467,(float).1015,(float).09239,(float).09004,(float).08487,(
	    float).08003,(float).07767,(float).0735,(float).06734,(float)
	    .0628,(float).05739,(float).05322,(float).05106,(float).04309,(
	    float).03272,(float).0087,(float)-.01014,(float)-.01158,(float)
	    -.02274,(float)-.02405,(float)-.03188,(float).1,(float).09131,(
	    float).08915,(float).08424,(float).07972,(float).07748,(float)
	    .0734,(float).06758,(float).06304,(float).05768,(float).05355,(
	    float).05141,(float).04329,(float).03248,(float).006911,(float)
	    -.01414,(float)-.01586,(float)-.02843,(float)-.03019,(float)
	    -.04007,(float).09567,(float).08754,(float).08559,(float).08108,(
	    float).07706,(float).07492,(float).07102,(float).06569,(float)
	    .06125,(float).05603,(float).05206,(float).04997,(float).04188,(
	    float).03084,(float).004156,(float)-.01881,(float)-.02082,(float)
	    -.03477,(float)-.03704,(float)-.04919,(float).08939,(float).08199,
	    (float).08022,(float).07617,(float).07265,(float).07063,(float)
	    .06697,(float).06212,(float).05787,(float).05284,(float).04908,(
	    float).04705,(float).03912,(float).028,(float)5.045e-4,(float)
	    -.02411,(float)-.02642,(float)-.04172,(float)-.04457,(float)
	    -.05922,(float).08197,(float).07541,(float).0738,(float).07019,(
	    float).06706,(float).06518,(float).0618,(float).05734,(float)
	    .05333,(float).04854,(float).04496,(float).04298,(float).03527,(
	    float).02418,(float)-.003947,(float)-.02999,(float)-.03263,(float)
	    -.04928,(float)-.05275,(float)-.07012,(float).0741,(float).06836,(
	    float).06691,(float).06366,(float).06076,(float).05905,(float)
	    .05594,(float).05176,(float).048,(float).04347,(float).04002,(
	    float).0381,(float).03061,(float).0196,(float)-.009095,(float)
	    -.03639,(float)-.03938,(float)-.0574,(float)-.06156,(float)
	    -.08184,(float).06626,(float).06119,(float).05987,(float).05689,(
	    float).05413,(float).05258,(float).04972,(float).04575,(float)
	    .04218,(float).0379,(float).03454,(float).03266,(float).02537,(
	    float).01443,(float)-.01484,(float)-.04326,(float)-.04662,(float)
	    -.06607,(float)-.07097,(float)-.09433,(float).0587,(float).0541,(
	    float).05292,(float).05012,(float).04748,(float).04602,(float)
	    .04334,(float).03956,(float).03614,(float).03202,(float).02873,(
	    float).02687,(float).01974,(float).008843,(float)-.02109,(float)
	    -.05057,(float)-.05433,(float)-.07525,(float)-.08093,(float)
	    -.1075,(float).05152,(float).04726,(float).04619,(float).04349,(
	    float).04101,(float).03953,(float).03698,(float).03337,(float)
	    .03004,(float).026,(float).02276,(float).02087,(float).01386,(
	    float).002958,(float)-.02777,(float)-.05828,(float)-.06245,(float)
	    -.08491,(float)-.09141,(float)-.1214,(float).04482,(float).04077,(
	    float).03981,(float).03712,(float).03482,(float).03321,(float)
	    .03075,(float).02726,(float).02399,(float).01995,(float).01674,(
	    float).01479,(float).00784,(float)-.003134,(float)-.03481,(float)
	    -.06634,(float)-.07095,(float)-.09503,(float)-.1024,(float)-.1359,
	    (float).03867,(float).03471,(float).0338,(float).0311,(float)
	    .02888,(float).02718,(float).02475,(float).0213,(float).01803,(
	    float).01396,(float).01072,(float).008703,(float).001739,(float)
	    -.009368,(float)-.04214,(float)-.07473,(float)-.07981,(float)
	    -.1056,(float)-.1138,(float)-.1509,(float).0331,(float).02911,(
	    float).02818,(float).02545,(float).02318,(float).02148,(float)
	    .01902,(float).01553,(float).01219,(float).008058,(float).004747,(
	    float).00267,(float)-.004392,(float)-.01569,(float)-.04973,(float)
	    -.08343,(float)-.08898,(float)-.1165,(float)-.1256,(float)-.1664,(
	    float).02806,(float).02394,(float).02292,(float).02016,(float)
	    .01774,(float).01608,(float).01355,(float).009959,(float).006484,(
	    float).002279,(float)-.001154,(float)-.003281,(float)-.01052,(
	    float)-.02207,(float)-.05752,(float)-.0924,(float)-.09844,(float)
	    -.1278,(float)-.1378,(float)-.1823,(float).02347,(float).01912,(
	    float).018,(float).01517,(float).0126,(float).01096,(float)
	    .008329,(float).004604,(float)9.587e-4,(float)-.003369,(float)
	    -.006948,(float)-.009142,(float)-.0166,(float)-.02848,(float)
	    -.0655,(float)-.1016,(float)-.1082,(float)-.1394,(float)-.1504,(
	    float)-.1986,(float).01923,(float).01461,(float).01342,(float)
	    .01046,(float).007783,(float).006063,(float).003308,(float)
	    -5.389e-4,(float)-.004373,(float)-.008884,(float)-.01262,(float)
	    -.01491,(float)-.02263,(float)-.03488,(float)-.07363,(float)
	    -.1111,(float)-.1181,(float)-.1514,(float)-.1632,(float)-.2152,(
	    float).01529,(float).01033,(float).009128,(float).005972,(float)
	    .003261,(float).001359,(float)-.001532,(float)-.005472,(float)
	    -.009519,(float)-.01427,(float)-.01815,(float)-.02059,(float)
	    -.02858,(float)-.04128,(float)-.08188,(float)-.1207,(float)-.1283,
	    (float)-.1636,(float)-.1764,(float)-.232,(float).01167,(float)
	    .006317,(float).005096,(float).001719,(float)-.001037,(float)
	    -.003135,(float)-.006182,(float)-.01022,(float)-.01451,(float)
	    -.01953,(float)-.02359,(float)-.02617,(float)-.03448,(float)
	    -.04766,(float)-.09023,(float)-.1306,(float)-.1387,(float)-.176,(
	    float)-.1897,(float)-.2491,(float).008357,(float).002599,(float)
	    .001309,(float)-.002273,(float)-.00518,(float)-.0074,(float)
	    -.01062,(float)-.01482,(float)-.01939,(float)-.02467,(float)
	    -.02893,(float)-.03165,(float)-.04033,(float)-.05404,(float)
	    -.09867,(float)-.1406,(float)-.1492,(float)-.1887,(float)-.2033,(
	    float)-.2662,(float).005291,(float)-8.265e-4,(float)-.002246,(
	    float)-.006013,(float)-.009181,(float)-.01144,(float)-.01485,(
	    float)-.0193,(float)-.02416,(float)-.02968,(float)-.03419,(float)
	    -.03704,(float)-.04612,(float)-.06039,(float)-.1072,(float)-.1507,
	    (float)-.1599,(float)-.2015,(float)-.217,(float)-.2833,(float)
	    .002424,(float)-.004024,(float)-.005601,(float)-.009552,(float)
	    -.01302,(float)-.01531,(float)-.01891,(float)-.02368,(float)
	    -.0288,(float)-.03458,(float)-.03936,(float)-.04234,(float)
	    -.05186,(float)-.06673,(float)-.1157,(float)-.161,(float)-.1707,(
	    float)-.2145,(float)-.2308,(float)-.3005,(float)-2.987e-4,(float)
	    -.007076,(float)-.00878,(float)-.01295,(float)-.01669,(float)
	    -.01906,(float)-.02286,(float)-.02795,(float)-.03333,(float)
	    -.03939,(float)-.04445,(float)-.04758,(float)-.05754,(float)
	    -.07304,(float)-.1243,(float)-.1713,(float)-.1816,(float)-.2275,(
	    float)-.2447,(float)-.3175,(float)-.002903,(float)-.01005,(float)
	    -.01181,(float)-.01626,(float)-.02018,(float)-.02274,(float)
	    -.02673,(float)-.03208,(float)-.03774,(float)-.04412,(float)
	    -.04944,(float)-.05276,(float)-.06315,(float)-.07931,(float)
	    -.1329,(float)-.1818,(float)-.1926,(float)-.2407,(float)-.2586,(
	    float)-.3344,(float)-.005337,(float)-.01294,(float)-.01475,(float)
	    -.01949,(float)-.02354,(float)-.02633,(float)-.03053,(float)
	    -.0361,(float)-.04206,(float)-.04879,(float)-.05435,(float)
	    -.05787,(float)-.06872,(float)-.08556,(float)-.1416,(float)-.1923,
	    (float)-.2036,(float)-.2539,(float)-.2725,(float)-.351,(float)
	    -.007573,(float)-.0157,(float)-.01759,(float)-.0226,(float)
	    -.02685,(float)-.02979,(float)-.03422,(float)-.04003,(float)
	    -.04632,(float)-.05339,(float)-.05922,(float)-.0629,(float)
	    -.07424,(float)-.09178,(float)-.1502,(float)-.2028,(float)-.2146,(
	    float)-.267,(float)-.2864,(float)-.3674,(float)-.009691,(float)
	    -.01828,(float)-.02031,(float)-.02556,(float)-.0301,(float)
	    -.03312,(float)-.03778,(float)-.04392,(float)-.05053,(float)
	    -.05791,(float)-.06404,(float)-.06788,(float)-.07972,(float)
	    -.09796,(float)-.1587,(float)-.2134,(float)-.2257,(float)-.2802,(
	    float)-.3001,(float)-.3833,(float)-.01178,(float)-.02072,(float)
	    -.0229,(float)-.0284,(float)-.03326,(float)-.03636,(float)-.04125,
	    (float)-.04776,(float)-.05467,(float)-.06236,(float)-.0688,(float)
	    -.0728,(float)-.08514,(float)-.1041,(float)-.1673,(float)-.2239,(
	    float)-.2366,(float)-.2932,(float)-.3137,(float)-.3988,(float)
	    -.01383,(float)-.02309,(float)-.0254,(float)-.03116,(float)
	    -.03632,(float)-.03955,(float)-.04465,(float)-.05153,(float)
	    -.05873,(float)-.06674,(float)-.07349,(float)-.07767,(float)
	    -.09051,(float)-.1102,(float)-.1757,(float)-.2344,(float)-.2475,(
	    float)-.3061,(float)-.3271,(float)-.4138,(float)-.01578,(float)
	    -.02543,(float)-.02785,(float)-.03389,(float)-.0393,(float)-.0427,
	    (float)-.04801,(float)-.05521,(float)-.06272,(float)-.07108,(
	    float)-.07812,(float)-.08248,(float)-.09582,(float)-.1162,(float)
	    -.1841,(float)-.2448,(float)-.2583,(float)-.3188,(float)-.3402,(
	    float)-.4282,(float)-.01759,(float)-.02774,(float)-.03026,(float)
	    -.03659,(float)-.04219,(float)-.04579,(float)-.05134,(float)
	    -.0588,(float)-.06665,(float)-.07536,(float)-.08267,(float)
	    -.08722,(float)-.1011,(float)-.1222,(float)-.1923,(float)-.255,(
	    float)-.269,(float)-.3313,(float)-.353,(float)-.4419,(float)
	    -.01927,(float)-.02999,(float)-.03261,(float)-.03923,(float)
	    -.04504,(float)-.04879,(float)-.0546,(float)-.06233,(float)
	    -.07052,(float)-.07959,(float)-.08717,(float)-.09188,(float)
	    -.1062,(float)-.1281,(float)-.2004,(float)-.2652,(float)-.2795,(
	    float)-.3436,(float)-.3655,(float)-.4548,(float)-.02086,(float)
	    -.03213,(float)-.03487,(float)-.04177,(float)-.04783,(float)
	    -.05172,(float)-.05777,(float)-.06581,(float)-.07431,(float)
	    -.08373,(float)-.09159,(float)-.09647,(float)-.1113,(float)-.1339,
	    (float)-.2084,(float)-.2751,(float)-.2897,(float)-.3555,(float)
	    -.3775,(float)-.4669,(float)-.02249,(float)-.03416,(float)-.03701,
	    (float)-.0442,(float)-.05051,(float)-.05459,(float)-.06086,(float)
	    -.06923,(float)-.07802,(float)-.08778,(float)-.09591,(float)-.101,
	    (float)-.1163,(float)-.1396,(float)-.2161,(float)-.2848,(float)
	    -.2997,(float)-.367,(float)-.389,(float)-.4782,(float)-.02333,(
	    float)-.03515,(float)-.03802,(float)-.04539,(float)-.05181,(float)
	    -.05602,(float)-.06238,(float)-.07092,(float)-.07983,(float)
	    -.08976,(float)-.09803,(float)-.1032,(float)-.1187,(float)-.1424,(
	    float)-.2199,(float)-.2896,(float)-.3046,(float)-.3726,(float)
	    -.3946,(float)-.4834,(float)-.02417,(float)-.03613,(float)-.03901,
	    (float)-.04656,(float)-.05308,(float)-.05744,(float)-.06388,(
	    float)-.07257,(float)-.08161,(float)-.09172,(float)-.1001,(float)
	    -.1054,(float)-.1212,(float)-.1452,(float)-.2236,(float)-.2943,(
	    float)-.3093,(float)-.378,(float)-.4,(float)-.4884,(float)-.02574,
	    (float)-.03805,(float)-.04096,(float)-.04886,(float)-.05557,(
	    float)-.0602,(float)-.06683,(float)-.0758,(float)-.08513,(float)
	    -.09557,(float)-.1042,(float)-.1097,(float)-.1259,(float)-.1506,(
	    float)-.2308,(float)-.3034,(float)-.3186,(float)-.3886,(float)
	    -.4103,(float)-.4975,(float)-.02702,(float)-.03986,(float)-.04291,
	    (float)-.05107,(float)-.05804,(float)-.06279,(float)-.06966,(
	    float)-.07891,(float)-.08857,(float)-.09932,(float)-.1082,(float)
	    -.1139,(float)-.1305,(float)-.1558,(float)-.2378,(float)-.3122,(
	    float)-.3275,(float)-.3986,(float)-.42,(float)-.5056,(float)
	    -.02805,(float)-.04152,(float)-.04481,(float)-.05316,(float)
	    -.06048,(float)-.06518,(float)-.07234,(float)-.08192,(float)
	    -.09192,(float)-.1029,(float)-.1121,(float)-.1178,(float)-.135,(
	    float)-.1609,(float)-.2444,(float)-.3205,(float)-.3359,(float)
	    -.4079,(float)-.4289,(float)-.5124,(float)-.029,(float)-.04304,(
	    float)-.04656,(float)-.0551,(float)-.06279,(float)-.06745,(float)
	    -.07487,(float)-.08482,(float)-.09512,(float)-.1064,(float)-.1158,
	    (float)-.1216,(float)-.1392,(float)-.1657,(float)-.2506,(float)
	    -.3284,(float)-.3438,(float)-.4165,(float)-.437,(float)-.518,(
	    float)-.03001,(float)-.04445,(float)-.0481,(float)-.05691,(float)
	    -.06488,(float)-.06966,(float)-.07728,(float)-.08757,(float)
	    -.0981,(float)-.1097,(float)-.1193,(float)-.1252,(float)-.1432,(
	    float)-.1703,(float)-.2563,(float)-.3357,(float)-.3511,(float)
	    -.4244,(float)-.4442,(float)-.5222,(float)-.03105,(float)-.04578,(
	    float)-.04943,(float)-.05859,(float)-.06672,(float)-.07182,(float)
	    -.07958,(float)-.09011,(float)-.1008,(float)-.1127,(float)-.1226,(
	    float)-.1287,(float)-.147,(float)-.1745,(float)-.2616,(float)
	    -.3425,(float)-.3577,(float)-.4314,(float)-.4504,(float)-.525,(
	    float)-.03198,(float)-.04704,(float)-.05059,(float)-.06017,(float)
	    -.06833,(float)-.07386,(float)-.08174,(float)-.09242,(float)
	    -.1033,(float)-.1155,(float)-.1255,(float)-.1318,(float)-.1504,(
	    float)-.1784,(float)-.2663,(float)-.3485,(float)-.3636,(float)
	    -.4375,(float)-.4556,(float)-.5263,(float)-.0326,(float)-.04816,(
	    float)-.05164,(float)-.0616,(float)-.06976,(float)-.07564,(float)
	    -.08369,(float)-.09447,(float)-.1056,(float)-.1181,(float)-.1282,(
	    float)-.1347,(float)-.1535,(float)-.1819,(float)-.2704,(float)
	    -.3538,(float)-.3687,(float)-.4425,(float)-.4596,(float)-.5261,(
	    float)-.03277,(float)-.049,(float)-.05263,(float)-.06279,(float)
	    -.07108,(float)-.07703,(float)-.08533,(float)-.09628,(float)
	    -.1077,(float)-.1204,(float)-.1306,(float)-.1371,(float)-.1562,(
	    float)-.185,(float)-.2739,(float)-.3584,(float)-.3729,(float)
	    -.4465,(float)-.4624,(float)-.5244,(float)-.03258,(float)-.04946,(
	    float)-.05344,(float)-.06363,(float)-.07229,(float)-.07801,(float)
	    -.08658,(float)-.09782,(float)-.1095,(float)-.1223,(float)-.1327,(
	    float)-.1392,(float)-.1586,(float)-.1876,(float)-.2766,(float)
	    -.362,(float)-.3762,(float)-.4493,(float)-.464,(float)-.521,(
	    float)-.03228,(float)-.04956,(float)-.05387,(float)-.0641,(float)
	    -.07324,(float)-.07871,(float)-.08748,(float)-.09906,(float)
	    -.1109,(float)-.1238,(float)-.1344,(float)-.1408,(float)-.1604,(
	    float)-.1897,(float)-.2785,(float)-.3646,(float)-.3784,(float)
	    -.4508,(float)-.4641,(float)-.5159,(float)-.03202,(float)-.04941,(
	    float)-.05384,(float)-.06425,(float)-.07376,(float)-.0792,(float)
	    -.08805,(float)-.09995,(float)-.1119,(float)-.1248,(float)-.1356,(
	    float)-.142,(float)-.1618,(float)-.1912,(float)-.2796,(float)
	    -.3662,(float)-.3795,(float)-.451,(float)-.4629,(float)-.5091,(
	    float)-.03162,(float)-.04911,(float)-.05345,(float)-.06416,(float)
	    -.07378,(float)-.07945,(float)-.08833,(float)-.1004,(float)-.1124,
	    (float)-.1254,(float)-.1362,(float)-.1427,(float)-.1625,(float)
	    -.192,(float)-.2797,(float)-.3667,(float)-.3794,(float)-.4497,(
	    float)-.4601,(float)-.5006,(float)-.03082,(float)-.04863,(float)
	    -.05284,(float)-.06383,(float)-.07333,(float)-.07934,(float)
	    -.08827,(float)-.1003,(float)-.1123,(float)-.1255,(float)-.1363,(
	    float)-.1428,(float)-.1626,(float)-.1921,(float)-.2789,(float)
	    -.3659,(float)-.378,(float)-.4469,(float)-.4557,(float)-.4904,(
	    float)-.02951,(float)-.04784,(float)-.05203,(float)-.06318,(float)
	    -.07251,(float)-.07872,(float)-.08776,(float)-.09973,(float)
	    -.1118,(float)-.125,(float)-.1357,(float)-.1423,(float)-.162,(
	    float)-.1914,(float)-.2769,(float)-.3638,(float)-.3753,(float)
	    -.4426,(float)-.4497,(float)-.4784,(float)-.02774,(float)-.0466,(
	    float)-.05091,(float)-.06207,(float)-.07135,(float)-.07753,(float)
	    -.08671,(float)-.09856,(float)-.1107,(float)-.124,(float)-.1345,(
	    float)-.141,(float)-.1607,(float)-.1899,(float)-.2739,(float)
	    -.3603,(float)-.3711,(float)-.4365,(float)-.442,(float)-.4647,(
	    float)-.0256,(float)-.04486,(float)-.04937,(float)-.06043,(float)
	    -.06979,(float)-.07578,(float)-.08505,(float)-.09684,(float)-.109,
	    (float)-.1222,(float)-.1327,(float)-.1391,(float)-.1586,(float)
	    -.1875,(float)-.2696,(float)-.3553,(float)-.3653,(float)-.4288,(
	    float)-.4325,(float)-.4493,(float)-.02325,(float)-.04262,(float)
	    -.04726,(float)-.05823,(float)-.0677,(float)-.07351,(float)
	    -.08278,(float)-.09456,(float)-.1067,(float)-.1197,(float)-.1301,(
	    float)-.1363,(float)-.1556,(float)-.1841,(float)-.2641,(float)
	    -.3487,(float)-.358,(float)-.4192,(float)-.4212,(float)-.4322,(
	    float)-.0207,(float)-.03988,(float)-.04454,(float)-.05545,(float)
	    -.06503,(float)-.0707,(float)-.07988,(float)-.09172,(float)-.1036,
	    (float)-.1165,(float)-.1267,(float)-.1328,(float)-.1518,(float)
	    -.1797,(float)-.2573,(float)-.3406,(float)-.349,(float)-.4078,(
	    float)-.4081,(float)-.4135,(float)-.01785,(float)-.03672,(float)
	    -.04131,(float)-.05217,(float)-.06179,(float)-.06734,(float)
	    -.07638,(float)-.08822,(float)-.09986,(float)-.1124,(float)-.1225,
	    (float)-.1284,(float)-.147,(float)-.1743,(float)-.2491,(float)
	    -.3307,(float)-.3383,(float)-.3945,(float)-.3932,(float)-.3933,(
	    float)-.01456,(float)-.03327,(float)-.03774,(float)-.0485,(float)
	    -.05797,(float)-.06342,(float)-.07232,(float)-.08396,(float)
	    -.09534,(float)-.1076,(float)-.1174,(float)-.1232,(float)-.1413,(
	    float)-.1679,(float)-.2396,(float)-.3191,(float)-.3259,(float)
	    -.3794,(float)-.3765,(float)-.3717,(float)-.01079,(float)-.02954,(
	    float)-.03391,(float)-.04447,(float)-.05359,(float)-.05892,(float)
	    -.0677,(float)-.07892,(float)-.09009,(float)-.102,(float)-.1115,(
	    float)-.117,(float)-.1347,(float)-.1603,(float)-.2288,(float)
	    -.3058,(float)-.3118,(float)-.3624,(float)-.3581,(float)-.3487,(
	    float)-.006552,(float)-.0255,(float)-.0299,(float)-.04006,(float)
	    -.04867,(float)-.05381,(float)-.06252,(float)-.07313,(float)
	    -.08412,(float)-.09576,(float)-.1048,(float)-.11,(float)-.1271,(
	    float)-.1517,(float)-.2166,(float)-.2907,(float)-.2959,(float)
	    -.3435,(float)-.3379,(float)-.3247,(float)-.002046,(float)-.0211,(
	    float)-.02558,(float)-.0352,(float)-.04324,(float)-.04817,(float)
	    -.05675,(float)-.06667,(float)-.07745,(float)-.08875,(float)
	    -.09726,(float)-.1021,(float)-.1185,(float)-.142,(float)-.2032,(
	    float)-.274,(float)-.2784,(float)-.3229,(float)-.3162,(float)
	    -.2996,(float).002427,(float)-.01625,(float)-.02068,(float)
	    -.02982,(float)-.03733,(float)-.04211,(float)-.05042,(float)
	    -.05971,(float)-.07012,(float)-.08098,(float)-.08897,(float)
	    -.09358,(float)-.1092,(float)-.1314,(float)-.1886,(float)-.2557,(
	    float)-.2593,(float)-.3006,(float)-.293,(float)-.2738,(float)
	    .006831,(float)-.01094,(float)-.01509,(float)-.02387,(float)
	    -.0311,(float)-.03569,(float)-.04355,(float)-.05239,(float)
	    -.06223,(float)-.07249,(float)-.08004,(float)-.08439,(float)
	    -.09905,(float)-.1199,(float)-.173,(float)-.2358,(float)-.2387,(
	    float)-.2769,(float)-.2685,(float)-.2475,(float).01125,(float)
	    -.005391,(float)-.009021,(float)-.01759,(float)-.02463,(float)
	    -.029,(float)-.0363,(float)-.04477,(float)-.05388,(float)-.06342,(
	    float)-.07058,(float)-.07467,(float)-.08833,(float)-.1077,(float)
	    -.1564,(float)-.2147,(float)-.217,(float)-.2518,(float)-.243,(
	    float)-.2209,(float).01556,(float)1.815e-4,(float)-.002838,(float)
	    -.01121,(float)-.0179,(float)-.02214,(float)-.02885,(float)
	    -.03691,(float)-.0452,(float)-.054,(float)-.06073,(float)-.06457,(
	    float)-.07715,(float)-.09501,(float)-.1393,(float)-.1925,(float)
	    -.1943,(float)-.2258,(float)-.2168,(float)-.1944,(float).01973,(
	    float).005608,(float).003017,(float)-.004967,(float)-.01103,(
	    float)-.01526,(float)-.02139,(float)-.02889,(float)-.03637,(float)
	    -.04452,(float)-.05069,(float)-.0543,(float)-.06573,(float)
	    -.08198,(float)-.1217,(float)-.1696,(float)-.1709,(float)-.199,(
	    float)-.1901,(float)-.1682,(float).02378,(float).01045,(float)
	    .008104,(float)7.145e-4,(float)-.00419,(float)-.008605,(float)
	    -.01428,(float)-.02082,(float)-.02761,(float)-.0353,(float)
	    -.04067,(float)-.04408,(float)-.05431,(float)-.06888,(float)
	    -.1041,(float)-.1463,(float)-.1472,(float)-.1719,(float)-.1634,(
	    float)-.1427,(float).02733,(float).01427,(float).01235,(float)
	    .005468,(float).002318,(float)-.002624,(float)-.00792,(float)
	    -.01297,(float)-.01923,(float)-.02664,(float)-.031,(float)-.03425,
	    (float)-.0432,(float)-.05606,(float)-.08671,(float)-.123,(float)
	    -.1236,(float)-.145,(float)-.1371,(float)-.1182,(float).03014,(
	    float).01684,(float).01563,(float).009077,(float).00788,(float)
	    .002332,(float)-.002621,(float)-.005784,(float)-.01167,(float)
	    -.01881,(float)-.02207,(float)-.02514,(float)-.03277,(float)
	    -.04388,(float)-.07,(float)-.1004,(float)-.1007,(float)-.1187,(
	    float)-.1116,(float)-.09509,(float).03208,(float).01791,(float)
	    .01744,(float).01128,(float).01185,(float).006005,(float).001348,(
	    float)2.719e-4,(float)-.005415,(float)-.01215,(float)-.01433,(
	    float)-.01706,(float)-.02345,(float)-.03273,(float)-.05435,(float)
	    -.07876,(float)-.07898,(float)-.09361,(float)-.08761,(float)
	    -.07378,(float).03229,(float).01747,(float).0176,(float).01205,(
	    float).01387,(float).008157,(float).003877,(float).004674,(float)
	    -8.093e-4,(float)-.006858,(float)-.008122,(float)-.01036,(float)
	    -.01555,(float)-.02298,(float)-.04014,(float)-.05881,(float)
	    -.05895,(float)-.07033,(float)-.06551,(float)-.0546,(float).03032,
	    (float).01587,(float).0161,(float).01158,(float).01371,(float)
	    .008858,(float).005104,(float).007019,(float).001951,(float)
	    -.003038,(float)-.003659,(float)-.005266,(float)-.009332,(float)
	    -.0149,(float)-.02772,(float)-.04108,(float)-.04116,(float)
	    -.04948,(float)-.04587,(float)-.03789,(float).02608,(float).01318,
	    (float).01301,(float).009914,(float).0115,(float).008231,(float)
	    .005129,(float).007265,(float).002941,(float)-6.839e-4,(float)
	    -9.475e-4,(float)-.001865,(float)-.004876,(float)-.008663,(float)
	    -.01739,(float)-.02607,(float)-.02611,(float)-.03164,(float)
	    -.02921,(float)-.02395,(float).01884,(float).009121,(float).0091,(
	    float).007016,(float).008198,(float).006104,(float).003927,(float)
	    .005707,(float).002675,(float)3.868e-4,(float)3.168e-4,(float)
	    -1.354e-4,(float)-.002042,(float)-.004266,(float)-.009372,(float)
	    -.01421,(float)-.01423,(float)-.01738,(float)-.01599,(float)
	    -.01303,(float).009652,(float).004451,(float).004953,(float)
	    .00353,(float).00449,(float).003074,(float).002011,(float).003083,
	    (float).001652,(float)5.016e-4,(float)5.233e-4,(float)2.818e-4,(
	    float)-5.68e-4,(float)-.001564,(float)-.003802,(float)-.005822,(
	    float)-.005834,(float)-.007169,(float)-.006579,(float)-.005337,(
	    float).002171,(float)9.663e-4,(float).00121,(float)7.827e-4,(
	    float).001092,(float)6.67e-4,(float)4.411e-4,(float)7.031e-4,(
	    float)4.204e-4,(float)1.542e-4,(float)1.689e-4,(float)1.009e-4,(
	    float)-6.441e-5,(float)-2.702e-4,(float)-7.186e-4,(float)-.001108,
	    (float)-.001111,(float)-.001371,(float)-.001257,(float)-.001017,(
	    float)5.166e-21,(float)1.243e-20,(float)1.736e-20,(float)
	    -3.524e-21,(float)-3.76e-20,(float)2.643e-20,(float)-1.732e-20,(
	    float)-1.986e-20,(float)1.092e-20,(float)-1.453e-20,(float)
	    -2.604e-20,(float)-1.08e-20,(float)-2.8e-20,(float)2.095e-20,(
	    float)2.245e-21,(float)-2.604e-21,(float)1.081e-21,(float)
	    -5.448e-21,(float)8.424e-21,(float)9.963e-21 };
    static real uhr[1660]	/* was [20][83] */ = { (float)-.4078,(float)
	    -.3768,(float)-.3723,(float)-.3587,(float)-.3523,(float)-.3444,(
	    float)-.3349,(float)-.3249,(float)-.3184,(float)-.3086,(float)
	    -.3051,(float)-.3033,(float)-.2928,(float)-.2827,(float)-.3014,(
	    float)-.3262,(float)-.3448,(float)-.3923,(float)-.4345,(float)
	    -.6107,(float)-.4006,(float)-.3711,(float)-.3668,(float)-.3538,(
	    float)-.3476,(float)-.3401,(float)-.331,(float)-.3214,(float)
	    -.3152,(float)-.306,(float)-.3027,(float)-.301,(float)-.291,(
	    float)-.2814,(float)-.3009,(float)-.3259,(float)-.3446,(float)
	    -.3921,(float)-.4343,(float)-.6105,(float)-.372,(float)-.3481,(
	    float)-.3445,(float)-.334,(float)-.3287,(float)-.3229,(float)
	    -.3155,(float)-.3072,(float)-.3025,(float)-.2951,(float)-.2927,(
	    float)-.2917,(float)-.2834,(float)-.2759,(float)-.2985,(float)
	    -.3248,(float)-.3435,(float)-.3914,(float)-.4335,(float)-.6093,(
	    float)-.328,(float)-.3122,(float)-.3097,(float)-.3029,(float)
	    -.2992,(float)-.2957,(float)-.2907,(float)-.2847,(float)-.2823,(
	    float)-.2776,(float)-.2765,(float)-.2765,(float)-.271,(float)
	    -.2668,(float)-.2944,(float)-.3228,(float)-.3415,(float)-.3903,(
	    float)-.4321,(float)-.6072,(float)-.2777,(float)-.2704,(float)
	    -.2691,(float)-.2662,(float)-.2646,(float)-.2633,(float)-.261,(
	    float)-.2577,(float)-.2578,(float)-.2562,(float)-.2567,(float)
	    -.2578,(float)-.2556,(float)-.2553,(float)-.289,(float)-.32,(
	    float)-.3386,(float)-.3886,(float)-.4301,(float)-.6042,(float)
	    -.2287,(float)-.2286,(float)-.2287,(float)-.229,(float)-.2297,(
	    float)-.23,(float)-.2302,(float)-.2297,(float)-.2322,(float)
	    -.2333,(float)-.2355,(float)-.2375,(float)-.2387,(float)-.2423,(
	    float)-.2825,(float)-.3164,(float)-.335,(float)-.3863,(float)
	    -.4275,(float)-.6003,(float)-.1851,(float)-.1905,(float)-.1917,(
	    float)-.1946,(float)-.1972,(float)-.1987,(float)-.201,(float)
	    -.2029,(float)-.2074,(float)-.2109,(float)-.2146,(float)-.2173,(
	    float)-.2216,(float)-.2289,(float)-.2751,(float)-.3121,(float)
	    -.3307,(float)-.3835,(float)-.4242,(float)-.5955,(float)-.1482,(
	    float)-.1574,(float)-.1596,(float)-.1643,(float)-.1684,(float)
	    -.1707,(float)-.1745,(float)-.1784,(float)-.1845,(float)-.1898,(
	    float)-.1948,(float)-.1981,(float)-.205,(float)-.2154,(float)
	    -.2673,(float)-.3072,(float)-.3258,(float)-.3802,(float)-.4203,(
	    float)-.5899,(float)-.1175,(float)-.1292,(float)-.132,(float)
	    -.1379,(float)-.1431,(float)-.1461,(float)-.151,(float)-.1564,(
	    float)-.1637,(float)-.1705,(float)-.1764,(float)-.1803,(float)
	    -.1892,(float)-.2023,(float)-.259,(float)-.3018,(float)-.3203,(
	    float)-.3764,(float)-.4159,(float)-.5835,(float)-.09206,(float)
	    -.1051,(float)-.1083,(float)-.1151,(float)-.1211,(float)-.1246,(
	    float)-.1303,(float)-.1368,(float)-.145,(float)-.1528,(float)
	    -.1595,(float)-.1638,(float)-.1744,(float)-.1897,(float)-.2505,(
	    float)-.2959,(float)-.3144,(float)-.372,(float)-.4109,(float)
	    -.5763,(float)-.07104,(float)-.08476,(float)-.08809,(float)
	    -.09555,(float)-.1021,(float)-.106,(float)-.1121,(float)-.1195,(
	    float)-.1282,(float)-.1368,(float)-.1441,(float)-.1488,(float)
	    -.1606,(float)-.1776,(float)-.2419,(float)-.2897,(float)-.3081,(
	    float)-.3671,(float)-.4053,(float)-.5683,(float)-.05377,(float)
	    -.06764,(float)-.07094,(float)-.07883,(float)-.0857,(float)
	    -.08992,(float)-.0963,(float)-.1043,(float)-.1133,(float)-.1224,(
	    float)-.1301,(float)-.135,(float)-.1478,(float)-.1661,(float)
	    -.2331,(float)-.2832,(float)-.3014,(float)-.3618,(float)-.3992,(
	    float)-.5596,(float)-.03958,(float)-.05336,(float)-.05657,(float)
	    -.06466,(float)-.07161,(float)-.07601,(float)-.08253,(float)
	    -.09085,(float)-.09996,(float)-.1094,(float)-.1174,(float)-.1225,(
	    float)-.1359,(float)-.1553,(float)-.2243,(float)-.2764,(float)
	    -.2945,(float)-.356,(float)-.3926,(float)-.5501,(float)-.02786,(
	    float)-.04147,(float)-.04462,(float)-.05267,(float)-.05955,(float)
	    -.06397,(float)-.07056,(float)-.07899,(float)-.08816,(float)
	    -.09782,(float)-.1059,(float)-.111,(float)-.1249,(float)-.145,(
	    float)-.2156,(float)-.2693,(float)-.2872,(float)-.3497,(float)
	    -.3854,(float)-.5399,(float)-.01818,(float)-.03151,(float)-.03464,
	    (float)-.04247,(float)-.04921,(float)-.0535,(float)-.0601,(float)
	    -.0685,(float)-.07766,(float)-.08738,(float)-.09549,(float)-.1006,
	    (float)-.1148,(float)-.1353,(float)-.2068,(float)-.2621,(float)
	    -.2797,(float)-.3429,(float)-.3778,(float)-.529,(float)-.01018,(
	    float)-.02308,(float)-.02617,(float)-.03369,(float)-.04025,(float)
	    -.04437,(float)-.05089,(float)-.05916,(float)-.06825,(float)
	    -.07791,(float)-.08596,(float)-.09095,(float)-.1053,(float)-.1261,
	    (float)-.198,(float)-.2545,(float)-.272,(float)-.3357,(float)
	    -.3696,(float)-.5173,(float)-.003538,(float)-.01583,(float)
	    -.01883,(float)-.02603,(float)-.03236,(float)-.03632,(float)
	    -.04269,(float)-.0508,(float)-.0597,(float)-.06922,(float)-.07715,
	    (float)-.08206,(float)-.0964,(float)-.1173,(float)-.1893,(float)
	    -.2468,(float)-.2639,(float)-.328,(float)-.361,(float)-.5049,(
	    float).002058,(float)-.009485,(float)-.0123,(float)-.01922,(float)
	    -.02527,(float)-.02916,(float)-.0353,(float)-.04321,(float)
	    -.05182,(float)-.06116,(float)-.06893,(float)-.07377,(float)
	    -.08796,(float)-.1089,(float)-.1805,(float)-.2388,(float)-.2556,(
	    float)-.3198,(float)-.3518,(float)-.4917,(float).006933,(float)
	    -.003835,(float)-.006407,(float)-.01308,(float)-.0188,(float)
	    -.02265,(float)-.02853,(float)-.0362,(float)-.04446,(float)-.0536,
	    (float)-.06116,(float)-.06594,(float)-.07989,(float)-.1006,(float)
	    -.1716,(float)-.2306,(float)-.247,(float)-.3112,(float)-.342,(
	    float)-.4778,(float).01138,(float).001322,(float)-.001026,(float)
	    -.007422,(float)-.01281,(float)-.0166,(float)-.02222,(float)
	    -.0296,(float)-.03752,(float)-.04641,(float)-.05374,(float)
	    -.05843,(float)-.07208,(float)-.09258,(float)-.1626,(float)-.222,(
	    float)-.2381,(float)-.3019,(float)-.3317,(float)-.463,(float)
	    .01558,(float).006165,(float).00398,(float)-.002095,(float)
	    -.00718,(float)-.01085,(float)-.01621,(float)-.02328,(float)
	    -.03089,(float)-.03949,(float)-.04658,(float)-.05113,(float)
	    -.06446,(float)-.08462,(float)-.1535,(float)-.2131,(float)-.2288,(
	    float)-.2922,(float)-.3208,(float)-.4475,(float).01965,(float)
	    .01083,(float).008757,(float).003038,(float)-.001787,(float)
	    -.005263,(float)-.01038,(float)-.01714,(float)-.02446,(float)
	    -.03274,(float)-.03957,(float)-.04396,(float)-.05694,(float)
	    -.07669,(float)-.1442,(float)-.2039,(float)-.2191,(float)-.2818,(
	    float)-.3092,(float)-.4311,(float).02367,(float).01543,(float)
	    .01343,(float).008084,(float).003489,(float)2.49e-4,(float)
	    -.004628,(float)-.0111,(float)-.01811,(float)-.02605,(float)
	    -.03262,(float)-.03681,(float)-.04942,(float)-.06871,(float)
	    -.1346,(float)-.1942,(float)-.2089,(float)-.2708,(float)-.297,(
	    float)-.4138,(float).02775,(float).02004,(float).01811,(float)
	    .01314,(float).008745,(float).005774,(float).001127,(float)
	    -.005068,(float)-.01176,(float)-.01934,(float)-.02564,(float)
	    -.02962,(float)-.04184,(float)-.0606,(float)-.1247,(float)-.1841,(
	    float)-.1983,(float)-.2592,(float)-.2841,(float)-.3956,(float)
	    .03192,(float).02475,(float).02289,(float).01827,(float).01408,(
	    float).01138,(float).006964,(float).001041,(float)-.005318,(float)
	    -.01252,(float)-.01855,(float)-.02232,(float)-.03412,(float)
	    -.05231,(float)-.1144,(float)-.1735,(float)-.1871,(float)-.2469,(
	    float)-.2704,(float)-.3764,(float).03625,(float).02959,(float)
	    .02782,(float).02355,(float).01958,(float).01711,(float).01294,(
	    float).00732,(float).001301,(float)-.005525,(float)-.01127,(float)
	    -.01482,(float)-.02616,(float)-.04375,(float)-.1037,(float)-.1624,
	    (float)-.1754,(float)-.2338,(float)-.2559,(float)-.3562,(float)
	    .0408,(float).03462,(float).03298,(float).02903,(float).02534,(
	    float).02304,(float).01912,(float).01385,(float).008176,(float)
	    .001721,(float)-.003707,(float)-.007059,(float)-.01791,(float)
	    -.03485,(float)-.09251,(float)-.1506,(float)-.163,(float)-.2198,(
	    float)-.2406,(float)-.335,(float).04562,(float).03992,(float)
	    .03844,(float).03478,(float).03142,(float).02924,(float).02559,(
	    float).02071,(float).01538,(float).009294,(float).004205,(float)
	    .001045,(float)-.009273,(float)-.02554,(float)-.08074,(float)
	    -.1382,(float)-.1499,(float)-.205,(float)-.2243,(float)-.3126,(
	    float).05078,(float).04557,(float).04427,(float).04088,(float)
	    .03787,(float).03581,(float).03242,(float).02794,(float).02299,(
	    float).01727,(float).01254,(float).009572,(float)-1.876e-4,(float)
	    -.01574,(float)-.06832,(float)-.125,(float)-.1361,(float)-.1893,(
	    float)-.2071,(float)-.2891,(float).0564,(float).05169,(float)
	    .05053,(float).04745,(float).04476,(float).04285,(float).03973,(
	    float).03562,(float).03106,(float).02574,(float).02136,(float)
	    .01861,(float).009424,(float)-.005368,(float)-.05518,(float)-.111,
	    (float)-.1213,(float)-.1725,(float)-.1888,(float)-.2643,(float)
	    .06252,(float).05837,(float).05729,(float).05458,(float).05212,(
	    float).05044,(float).0476,(float).04381,(float).03966,(float)
	    .03478,(float).03075,(float).02824,(float).01964,(float).005651,(
	    float)-.04121,(float)-.09608,(float)-.1057,(float)-.1547,(float)
	    -.1693,(float)-.2383,(float).06919,(float).06566,(float).06466,(
	    float).06232,(float).06006,(float).05865,(float).05611,(float)
	    .0526,(float).04889,(float).04449,(float).04081,(float).03854,(
	    float).03057,(float).01741,(float)-.02635,(float)-.08017,(float)
	    -.08898,(float)-.1357,(float)-.1487,(float)-.2109,(float).07647,(
	    float).07359,(float).07271,(float).07072,(float).06869,(float)
	    .06753,(float).0653,(float).06211,(float).05886,(float).05495,(
	    float).05163,(float).04962,(float).04229,(float).03002,(float)
	    -.01047,(float)-.06318,(float)-.07117,(float)-.1154,(float)-.1267,
	    (float)-.182,(float).08447,(float).08221,(float).08148,(float)
	    .07985,(float).0781,(float).07715,(float).07525,(float).07245,(
	    float).06966,(float).06625,(float).06332,(float).06158,(float)
	    .05494,(float).04359,(float).006534,(float)-.04499,(float)-.05212,
	    (float)-.09369,(float)-.1033,(float)-.1516,(float).0933,(float)
	    .09162,(float).09107,(float).08979,(float).0884,(float).08765,(
	    float).08607,(float).08374,(float).08141,(float).07849,(float)
	    .076,(float).07452,(float).06861,(float).05823,(float).02477,(
	    float)-.02548,(float)-.03173,(float)-.07052,(float)-.07835,(float)
	    -.1196,(float).1031,(float).1019,(float).1016,(float).1007,(float)
	    .09971,(float).09912,(float).09789,(float).09606,(float).09421,(
	    float).09181,(float).08979,(float).08859,(float).08344,(float)
	    .07408,(float).04438,(float)-.004541,(float)-.009874,(float)
	    -.04576,(float)-.0518,(float)-.08596,(float).1138,(float).1133,(
	    float).1131,(float).1126,(float).1121,(float).1117,(float).1108,(
	    float).1095,(float).1082,(float).1063,(float).1048,(float).1039,(
	    float).09955,(float).09126,(float).06551,(float).01797,(float)
	    .01359,(float)-.01925,(float)-.02348,(float)-.0505,(float).1257,(
	    float).126,(float).1259,(float).1259,(float).1257,(float).1256,(
	    float).1251,(float).1243,(float).1235,(float).1222,(float).1212,(
	    float).1206,(float).1171,(float).1099,(float).0883,(float).04221,(
	    float).03881,(float).00912,(float).006725,(float)-.01316,(float)
	    .1389,(float).14,(float).1401,(float).1406,(float).1407,(float)
	    .1409,(float).1408,(float).1405,(float).1403,(float).1397,(float)
	    .1392,(float).1389,(float).1363,(float).1303,(float).1129,(float)
	    .06834,(float).06594,(float).03952,(float).03897,(float).02617,(
	    float).1535,(float).1555,(float).1558,(float).1567,(float).1573,(
	    float).1578,(float).1582,(float).1583,(float).1588,(float).1588,(
	    float).1588,(float).1589,(float).1572,(float).1524,(float).1396,(
	    float).09654,(float).09517,(float).07211,(float).0734,(float)
	    .0676,(float).1698,(float).1726,(float).1731,(float).1746,(float)
	    .1756,(float).1765,(float).1773,(float).178,(float).1791,(float)
	    .1799,(float).1805,(float).1809,(float).1802,(float).1767,(float)
	    .1685,(float).127,(float).1267,(float).1071,(float).1102,(float)
	    .1112,(float).1786,(float).1818,(float).1825,(float).1842,(float)
	    .1855,(float).1865,(float).1875,(float).1886,(float).1901,(float)
	    .1912,(float).1921,(float).1927,(float).1925,(float).1896,(float)
	    .1838,(float).1431,(float).1434,(float).1255,(float).1295,(float)
	    .1339,(float).1879,(float).1915,(float).1923,(float).1944,(float)
	    .196,(float).1971,(float).1983,(float).1998,(float).2016,(float)
	    .203,(float).2042,(float).2051,(float).2053,(float).2032,(float)
	    .1998,(float).1599,(float).1607,(float).1446,(float).1495,(float)
	    .1572,(float).208,(float).2127,(float).2137,(float).2163,(float)
	    .2185,(float).2199,(float).2217,(float).2238,(float).2264,(float)
	    .2286,(float).2305,(float).2317,(float).233,(float).2323,(float)
	    .2338,(float).1956,(float).1974,(float).1849,(float).1916,(float)
	    .2057,(float).2304,(float).2362,(float).2376,(float).2408,(float)
	    .2436,(float).2453,(float).2476,(float).2505,(float).2538,(float)
	    .2568,(float).2594,(float).261,(float).2635,(float).2642,(float)
	    .2708,(float).2342,(float).2371,(float).2281,(float).2366,(float)
	    .2567,(float).2554,(float).2624,(float).264,(float).268,(float)
	    .2714,(float).2735,(float).2764,(float).28,(float).2842,(float)
	    .288,(float).2913,(float).2934,(float).297,(float).2992,(float)
	    .3111,(float).2761,(float).2801,(float).2746,(float).2848,(float)
	    .3104,(float).2834,(float).2916,(float).2936,(float).2983,(float)
	    .3024,(float).3048,(float).3084,(float).3128,(float).3178,(float)
	    .3225,(float).3266,(float).3291,(float).334,(float).3378,(float)
	    .3549,(float).3216,(float).3266,(float).3246,(float).3364,(float)
	    .3671,(float).3148,(float).3243,(float).3266,(float).3321,(float)
	    .3369,(float).3398,(float).344,(float).3493,(float).3551,(float)
	    .3608,(float).3656,(float).3687,(float).3748,(float).3802,(float)
	    .4027,(float).3709,(float).377,(float).3784,(float).3916,(float)
	    .4268,(float).3499,(float).3609,(float).3634,(float).3699,(float)
	    .3753,(float).3788,(float).3837,(float).3898,(float).3966,(float)
	    .4033,(float).4089,(float).4125,(float).4198,(float).4269,(float)
	    .4548,(float).4245,(float).4316,(float).4363,(float).4508,(float)
	    .4896,(float).3893,(float).4018,(float).4047,(float).412,(float)
	    .4183,(float).4223,(float).4279,(float).435,(float).4427,(float)
	    .4505,(float).4569,(float).461,(float).4697,(float).4785,(float)
	    .5117,(float).4829,(float).4909,(float).4986,(float).5142,(float)
	    .5558,(float).4334,(float).4476,(float).4509,(float).4593,(float)
	    .4663,(float).4709,(float).4773,(float).4853,(float).4941,(float)
	    .5029,(float).5102,(float).5148,(float).5248,(float).5354,(float)
	    .5738,(float).5463,(float).5552,(float).5657,(float).5822,(float)
	    .6255,(float).4829,(float).4991,(float).5028,(float).5122,(float)
	    .5201,(float).5252,(float).5325,(float).5414,(float).5514,(float)
	    .5612,(float).5694,(float).5746,(float).586,(float).5982,(float)
	    .6417,(float).6154,(float).6251,(float).6379,(float).6551,(float)
	    .6989,(float).5384,(float).5568,(float).5611,(float).5716,(float)
	    .5805,(float).586,(float).5941,(float).6041,(float).6152,(float)
	    .6262,(float).6353,(float).6409,(float).6537,(float).6676,(float)
	    .7158,(float).6907,(float).7009,(float).7157,(float).7333,(float)
	    .7761,(float).6011,(float).6216,(float).6265,(float).6381,(float)
	    .6481,(float).6541,(float).6631,(float).6743,(float).6865,(float)
	    .6985,(float).7085,(float).7146,(float).7288,(float).7444,(float)
	    .7969,(float).7726,(float).7834,(float).7995,(float).817,(float)
	    .8573,(float).6719,(float).6946,(float).7,(float).7128,(float)
	    .7238,(float).7305,(float).7404,(float).7527,(float).766,(float)
	    .7792,(float).79,(float).7967,(float).8122,(float).8293,(float)
	    .8856,(float).862,(float).8731,(float).8897,(float).9069,(float)
	    .9428,(float).7518,(float).7768,(float).7827,(float).7969,(float)
	    .8089,(float).8163,(float).8271,(float).8405,(float).8549,(float)
	    .8691,(float).8808,(float).888,(float).9047,(float).9233,(float)
	    .9826,(float).9595,(float).9705,(float).987,(float)1.003,(float)
	    1.033,(float).8419,(float).8695,(float).8758,(float).8914,(float)
	    .9044,(float).9126,(float).9243,(float).9388,(float).9542,(float)
	    .9695,(float).9819,(float).9896,(float)1.007,(float)1.027,(float)
	    1.089,(float)1.066,(float)1.077,(float)1.092,(float)1.106,(float)
	    1.127,(float).9438,(float).9741,(float).9809,(float).9979,(float)
	    1.012,(float)1.021,(float)1.033,(float)1.049,(float)1.065,(float)
	    1.082,(float)1.095,(float)1.103,(float)1.121,(float)1.142,(float)
	    1.205,(float)1.182,(float)1.192,(float)1.205,(float)1.217,(float)
	    1.226,(float)1.059,(float)1.092,(float)1.099,(float)1.118,(float)
	    1.133,(float)1.142,(float)1.156,(float)1.172,(float)1.19,(float)
	    1.207,(float)1.22,(float)1.229,(float)1.248,(float)1.27,(float)
	    1.332,(float)1.308,(float)1.317,(float)1.326,(float)1.335,(float)
	    1.33,(float)1.189,(float)1.226,(float)1.233,(float)1.253,(float)
	    1.269,(float)1.279,(float)1.293,(float)1.311,(float)1.329,(float)
	    1.347,(float)1.36,(float)1.369,(float)1.389,(float)1.411,(float)
	    1.47,(float)1.446,(float)1.454,(float)1.457,(float)1.462,(float)
	    1.438,(float)1.337,(float)1.376,(float)1.385,(float)1.406,(float)
	    1.422,(float)1.433,(float)1.448,(float)1.466,(float)1.484,(float)
	    1.503,(float)1.517,(float)1.525,(float)1.545,(float)1.567,(float)
	    1.622,(float)1.596,(float)1.602,(float)1.597,(float)1.598,(float)
	    1.552,(float)1.504,(float)1.546,(float)1.555,(float)1.577,(float)
	    1.595,(float)1.606,(float)1.621,(float)1.64,(float)1.659,(float)
	    1.677,(float)1.691,(float)1.699,(float)1.719,(float)1.74,(float)
	    1.787,(float)1.76,(float)1.762,(float)1.748,(float)1.742,(float)
	    1.67,(float)1.694,(float)1.739,(float)1.748,(float)1.771,(float)
	    1.789,(float)1.8,(float)1.816,(float)1.835,(float)1.853,(float)
	    1.871,(float)1.885,(float)1.892,(float)1.911,(float)1.931,(float)
	    1.967,(float)1.938,(float)1.937,(float)1.91,(float)1.897,(float)
	    1.794,(float)1.909,(float)1.956,(float)1.966,(float)1.99,(float)
	    2.008,(float)2.019,(float)2.035,(float)2.053,(float)2.071,(float)
	    2.088,(float)2.1,(float)2.107,(float)2.124,(float)2.142,(float)
	    2.163,(float)2.132,(float)2.126,(float)2.083,(float)2.062,(float)
	    1.923,(float)2.154,(float)2.202,(float)2.212,(float)2.236,(float)
	    2.254,(float)2.265,(float)2.28,(float)2.297,(float)2.314,(float)
	    2.329,(float)2.34,(float)2.346,(float)2.36,(float)2.374,(float)
	    2.376,(float)2.343,(float)2.331,(float)2.269,(float)2.237,(float)
	    2.056,(float)2.432,(float)2.481,(float)2.49,(float)2.514,(float)
	    2.53,(float)2.541,(float)2.555,(float)2.571,(float)2.585,(float)
	    2.598,(float)2.606,(float)2.61,(float)2.621,(float)2.63,(float)
	    2.607,(float)2.571,(float)2.552,(float)2.467,(float)2.423,(float)
	    2.195,(float)2.747,(float)2.795,(float)2.804,(float)2.826,(float)
	    2.841,(float)2.851,(float)2.863,(float)2.876,(float)2.887,(float)
	    2.896,(float)2.901,(float)2.903,(float)2.908,(float)2.91,(float)
	    2.857,(float)2.818,(float)2.79,(float)2.678,(float)2.619,(float)
	    2.337,(float)3.104,(float)3.15,(float)3.158,(float)3.177,(float)
	    3.19,(float)3.198,(float)3.207,(float)3.216,(float)3.222,(float)
	    3.226,(float)3.227,(float)3.226,(float)3.224,(float)3.218,(float)
	    3.126,(float)3.084,(float)3.046,(float)2.903,(float)2.826,(float)
	    2.483,(float)3.51,(float)3.55,(float)3.556,(float)3.572,(float)
	    3.58,(float)3.586,(float)3.591,(float)3.595,(float)3.594,(float)
	    3.591,(float)3.586,(float)3.581,(float)3.57,(float)3.553,(float)
	    3.415,(float)3.37,(float)3.32,(float)3.14,(float)3.044,(float)
	    2.633,(float)3.969,(float)4.001,(float)4.005,(float)4.014,(float)
	    4.017,(float)4.018,(float)4.018,(float)4.015,(float)4.005,(float)
	    3.994,(float)3.98,(float)3.971,(float)3.949,(float)3.917,(float)
	    3.724,(float)3.675,(float)3.612,(float)3.389,(float)3.272,(float)
	    2.784,(float)4.491,(float)4.509,(float)4.51,(float)4.509,(float)
	    4.504,(float)4.5,(float)4.492,(float)4.479,(float)4.458,(float)
	    4.435,(float)4.412,(float)4.397,(float)4.36,(float)4.311,(float)
	    4.052,(float)4.,(float)3.921,(float)3.65,(float)3.508,(float)
	    2.938,(float)5.083,(float)5.08,(float)5.076,(float)5.063,(float)
	    5.046,(float)5.035,(float)5.017,(float)4.991,(float)4.956,(float)
	    4.918,(float)4.883,(float)4.859,(float)4.805,(float)4.735,(float)
	    4.398,(float)4.343,(float)4.246,(float)3.921,(float)3.752,(float)
	    3.091,(float)5.755,(float)5.721,(float)5.709,(float)5.679,(float)
	    5.647,(float)5.627,(float)5.595,(float)5.553,(float)5.498,(float)
	    5.443,(float)5.392,(float)5.359,(float)5.283,(float)5.187,(float)
	    4.76,(float)4.702,(float)4.584,(float)4.201,(float)4.001,(float)
	    3.243,(float)6.516,(float)6.44,(float)6.418,(float)6.364,(float)
	    6.312,(float)6.28,(float)6.23,(float)6.166,(float)6.088,(float)
	    6.009,(float)5.94,(float)5.895,(float)5.792,(float)5.665,(float)
	    5.134,(float)5.074,(float)4.932,(float)4.485,(float)4.252,(float)
	    3.392,(float)7.38,(float)7.243,(float)7.208,(float)7.122,(float)
	    7.043,(float)6.995,(float)6.923,(float)6.831,(float)6.723,(float)
	    6.616,(float)6.524,(float)6.465,(float)6.33,(float)6.166,(float)
	    5.516,(float)5.453,(float)5.287,(float)4.771,(float)4.503,(float)
	    3.535,(float)8.358,(float)8.139,(float)8.086,(float)7.956,(float)
	    7.843,(float)7.774,(float)7.672,(float)7.546,(float)7.401,(float)
	    7.26,(float)7.139,(float)7.064,(float)6.891,(float)6.683,(float)
	    5.9,(float)5.834,(float)5.64,(float)5.054,(float)4.748,(float)
	    3.671,(float)9.463,(float)9.131,(float)9.054,(float)8.868,(float)
	    8.71,(float)8.614,(float)8.475,(float)8.305,(float)8.115,(float)
	    7.932,(float)7.779,(float)7.683,(float)7.465,(float)7.207,(float)
	    6.278,(float)6.207,(float)5.985,(float)5.326,(float)4.981,(float)
	    3.796,(float)10.71,(float)10.22,(float)10.11,(float)9.851,(float)
	    9.637,(float)9.505,(float)9.32,(float)9.098,(float)8.854,(float)
	    8.62,(float)8.428,(float)8.309,(float)8.041,(float)7.724,(float)
	    6.639,(float)6.562,(float)6.311,(float)5.581,(float)5.198,(float)
	    3.909,(float)12.08,(float)11.39,(float)11.25,(float)10.89,(float)
	    10.61,(float)10.43,(float)10.19,(float)9.904,(float)9.596,(float)
	    9.303,(float)9.068,(float)8.922,(float)8.597,(float)8.217,(float)
	    6.97,(float)6.884,(float)6.605,(float)5.809,(float)5.389,(float)
	    4.005,(float)13.57,(float)12.61,(float)12.42,(float)11.94,(float)
	    11.58,(float)11.35,(float)11.04,(float)10.69,(float)10.31,(float)
	    9.947,(float)9.667,(float)9.49,(float)9.106,(float)8.66,(float)
	    7.254,(float)7.159,(float)6.855,(float)6.,(float)5.548,(float)
	    4.082,(float)15.08,(float)13.79,(float)13.55,(float)12.93,(float)
	    12.49,(float)12.19,(float)11.81,(float)11.38,(float)10.93,(float)
	    10.5,(float)10.17,(float)9.968,(float)9.527,(float)9.018,(float)
	    7.473,(float)7.368,(float)7.043,(float)6.143,(float)5.666,(float)
	    4.138,(float)16.32,(float)14.72,(float)14.42,(float)13.68,(float)
	    13.16,(float)12.8,(float)12.36,(float)11.87,(float)11.35,(float)
	    10.87,(float)10.51,(float)10.28,(float)9.801,(float)9.246,(float)
	    7.606,(float)7.493,(float)7.155,(float)6.227,(float)5.734,(float)
	    4.17,(float)16.73,(float)15.01,(float)14.69,(float)13.9,(float)
	    13.35,(float)12.98,(float)12.51,(float)12.,(float)11.47,(float)
	    10.97,(float)10.6,(float)10.37,(float)9.873,(float)9.304,(float)
	    7.638,(float)7.523,(float)7.182,(float)6.248,(float)5.75,(float)
	    4.177 };

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
/*<       integer i,j >*/
/* Water-soluble model */
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
/*<       vi_m=0.00182737305 >*/
    vi_m__ = (float).00182737305;
/*<       vi(2)=vi_m >*/
    sixs_coef__1.vi[1] = vi_m__;
/*<       do 1 i=1,20 >*/
    for (i__ = 1; i__ <= 20; ++i__) {
/*<        asy(2,i)=asy_m(i) >*/
	sixs_coef__1.asy[(i__ << 2) - 3] = asy_m__[i__ - 1];
/*<        ex(2,i)=ex_m(i) >*/
	sixs_coef__1.ex[(i__ << 2) - 3] = ex_m__[i__ - 1];
/*<        sc(2,i)=sc_m(i) >*/
	sixs_coef__1.sc[(i__ << 2) - 3] = sc_m__[i__ - 1];
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
} /* wate_ */

#ifdef __cplusplus
	}
#endif
