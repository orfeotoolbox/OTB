/* DUST.f -- translated by f2c (version 19970805).
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

/*<       subroutine   dust >*/
/* Subroutine */ int dust_()
{
    /* Initialized data */

    static real asy_m__[20] = { (float).846,(float).838,(float).836,(float)
	    .832,(float).827,(float).826,(float).822,(float).817,(float).812,(
	    float).807,(float).802,(float).8,(float).792,(float).785,(float)
	    .778,(float).795,(float).808,(float).859,(float).892,(float).853 }
	    ;
    static real ex_m__[20] = { (float)8.230388,(float)8.305723,(float)
	    8.328883,(float)8.368396,(float)8.415968,(float)8.42865,(float)
	    8.467864,(float)8.519962,(float)8.583242,(float)8.632867,(float)
	    8.689027,(float)8.704309,(float)8.803438,(float)8.924636,(float)
	    9.300344,(float)9.404915,(float)9.368683,(float)8.811917,(float)
	    7.876924,(float)7.002555 };
    static real sc_m__[20] = { (float)5.5657,(float)5.744132,(float)5.786949,(
	    float)5.888061,(float)5.976042,(float)6.028808,(float)6.112733,(
	    float)6.215003,(float)6.333235,(float)6.450123,(float)6.547941,(
	    float)6.603794,(float)6.771705,(float)7.00242,(float)7.719834,(
	    float)8.043072,(float)8.08431,(float)7.738148,(float)6.921192,(
	    float)6.187184 };
    static real phr[1660]	/* was [20][83] */ = { (float).3243,(float)
	    .3504,(float).3625,(float).3853,(float).4323,(float).4322,(float)
	    .4427,(float).496,(float).5164,(float).5181,(float).5654,(float)
	    .5712,(float).5994,(float).5925,(float).3548,(float).236,(float)
	    .2183,(float).1136,(float).0644,(float).07657,(float).316,(float)
	    .341,(float).3534,(float).3726,(float).413,(float).4125,(float)
	    .4235,(float).4724,(float).4898,(float).4938,(float).5352,(float)
	    .5403,(float).5672,(float).5612,(float).3322,(float).2178,(float)
	    .1988,(float).1007,(float).05458,(float).07193,(float).3591,(
	    float).3912,(float).4034,(float).4175,(float).4481,(float).4415,(
	    float).4548,(float).4868,(float).5048,(float).5082,(float).5364,(
	    float).5328,(float).5564,(float).5433,(float).3156,(float).1964,(
	    float).17,(float).08035,(float).04514,(float).06322,(float).3476,(
	    float).3864,(float).3995,(float).4135,(float).4391,(float).4331,(
	    float).4487,(float).4722,(float).492,(float).4987,(float).5185,(
	    float).5126,(float).5333,(float).5175,(float).313,(float).194,(
	    float).166,(float).08849,(float).05591,(float).06634,(float).3226,
	    (float).3531,(float).3637,(float).3787,(float).403,(float).4048,(
	    float).4149,(float).4366,(float).4535,(float).4599,(float).4779,(
	    float).4783,(float).4909,(float).4768,(float).3146,(float).2077,(
	    float).1823,(float).1064,(float).05989,(float).07486,(float).2902,
	    (float).3218,(float).3363,(float).3464,(float).3689,(float).3649,(
	    float).3774,(float).3942,(float).4158,(float).4226,(float).4371,(
	    float).4312,(float).4489,(float).4387,(float).3242,(float).2326,(
	    float).204,(float).1102,(float).05586,(float).07879,(float).2596,(
	    float).2983,(float).3122,(float).3207,(float).3346,(float).3299,(
	    float).3463,(float).3518,(float).3775,(float).3885,(float).3962,(
	    float).3866,(float).4085,(float).4069,(float).3406,(float).2537,(
	    float).2133,(float).1035,(float).05049,(float).07773,(float).2419,
	    (float).2747,(float).2775,(float).2931,(float).2956,(float).3059,(
	    float).3185,(float).3219,(float).3353,(float).3506,(float).3542,(
	    float).3578,(float).3701,(float).3811,(float).3519,(float).2599,(
	    float).2101,(float).09485,(float).04611,(float).07451,(float)
	    .2232,(float).239,(float).2391,(float).255,(float).265,(float)
	    .2776,(float).2823,(float).2946,(float).3016,(float).3106,(float)
	    .3207,(float).3308,(float).3386,(float).3563,(float).3468,(float)
	    .2539,(float).201,(float).08717,(float).04284,(float).07098,(
	    float).1846,(float).1973,(float).2059,(float).2139,(float).2299,(
	    float).2307,(float).2372,(float).2565,(float).2648,(float).2717,(
	    float).2835,(float).287,(float).3018,(float).3207,(float).3281,(
	    float).2416,(float).1911,(float).08152,(float).04062,(float)
	    .06784,(float).1428,(float).1651,(float).1719,(float).1797,(float)
	    .1895,(float).1899,(float).2001,(float).2072,(float).2222,(float)
	    .2321,(float).2396,(float).2392,(float).2571,(float).2746,(float)
	    .2985,(float).2274,(float).1825,(float).07739,(float).03894,(
	    float).06531,(float).1168,(float).1363,(float).1389,(float).1483,(
	    float).1499,(float).1577,(float).1664,(float).1679,(float).1798,(
	    float).1928,(float).1962,(float).2008,(float).2129,(float).2299,(
	    float).2611,(float).2136,(float).1742,(float).07472,(float).03775,
	    (float).06342,(float).09709,(float).108,(float).1102,(float).1176,
	    (float).1248,(float).1289,(float).1339,(float).1415,(float).1499,(
	    float).1568,(float).1648,(float).1685,(float).18,(float).1957,(
	    float).2251,(float).1981,(float).1662,(float).07332,(float).0369,(
	    float).06207,(float).08103,(float).08892,(float).09256,(float)
	    .09732,(float).1054,(float).1059,(float).1105,(float).1205,(float)
	    .1266,(float).1316,(float).1396,(float).1417,(float).1531,(float)
	    .1668,(float).1957,(float).1817,(float).1576,(float).07256,(float)
	    .03647,(float).06114,(float).07004,(float).0784,(float).08045,(
	    float).0853,(float).08953,(float).09193,(float).09644,(float)
	    .1019,(float).1077,(float).1139,(float).119,(float).1217,(float)
	    .1307,(float).1423,(float).171,(float).1655,(float).148,(float)
	    .07272,(float).03634,(float).06053,(float).0632,(float).07077,(
	    float).0712,(float).07638,(float).07772,(float).08229,(float)
	    .08606,(float).08837,(float).09357,(float).1003,(float).1033,(
	    float).1072,(float).1135,(float).1235,(float).15,(float).1485,(
	    float).1385,(float).07329,(float).03638,(float).06011,(float)
	    .05814,(float).06358,(float).06493,(float).06854,(float).07041,(
	    float).07362,(float).07676,(float).07963,(float).08409,(float)
	    .08989,(float).09258,(float).09605,(float).1015,(float).1101,(
	    float).1331,(float).1334,(float).1282,(float).07381,(float).03668,
	    (float).05981,(float).05326,(float).05933,(float).06051,(float)
	    .06361,(float).06557,(float).06746,(float).07074,(float).073,(
	    float).07747,(float).08219,(float).08497,(float).08694,(float)
	    .09286,(float).1005,(float).1188,(float).1212,(float).1175,(float)
	    .07415,(float).03732,(float).0595,(float).05106,(float).0551,(
	    float).05595,(float).05873,(float).06245,(float).06315,(float)
	    .06528,(float).06925,(float).07286,(float).07531,(float).07979,(
	    float).0809,(float).08691,(float).09362,(float).1074,(float).1102,
	    (float).1079,(float).07411,(float).03816,(float).05911,(float)
	    .0494,(float).05225,(float).05397,(float).05571,(float).05991,(
	    float).05925,(float).0613,(float).06632,(float).06935,(float)
	    .07122,(float).07564,(float).07628,(float).08209,(float).08802,(
	    float).09941,(float).1012,(float).09907,(float).07328,(float)
	    .03919,(float).0585,(float).0487,(float).05194,(float).05247,(
	    float).05508,(float).05656,(float).05816,(float).06028,(float)
	    .06339,(float).06534,(float).06883,(float).07122,(float).07342,(
	    float).07726,(float).08284,(float).0932,(float).09373,(float)
	    .09203,(float).07137,(float).04024,(float).0577,(float).04778,(
	    float).05138,(float).0516,(float).05421,(float).05516,(float)
	    .05746,(float).05938,(float).06103,(float).06333,(float).06691,(
	    float).06867,(float).07136,(float).07418,(float).07917,(float)
	    .0884,(float).08759,(float).08621,(float).0694,(float).04118,(
	    float).05674,(float).0482,(float).05099,(float).05119,(float)
	    .05365,(float).05353,(float).05736,(float).05857,(float).05995,(
	    float).06142,(float).06558,(float).06649,(float).07074,(float)
	    .07177,(float).07662,(float).0847,(float).08329,(float).08117,(
	    float).06678,(float).04189,(float).05577,(float).04788,(float)
	    .05019,(float).05069,(float).05277,(float).05403,(float).05622,(
	    float).0575,(float).06034,(float).06153,(float).06441,(float)
	    .06643,(float).06956,(float).07148,(float).07603,(float).08216,(
	    float).08024,(float).07705,(float).06417,(float).04234,(float)
	    .05485,(float).04772,(float).05009,(float).05103,(float).05261,(
	    float).05539,(float).05591,(float).05712,(float).05995,(float)
	    .06279,(float).06427,(float).0676,(float).06862,(float).07242,(
	    float).07642,(float).08044,(float).0778,(float).07415,(float)
	    .06139,(float).04253,(float).05396,(float).04784,(float).04996,(
	    float).05174,(float).05262,(float).05675,(float).05538,(float)
	    .05693,(float).06075,(float).06407,(float).06474,(float).06877,(
	    float).06817,(float).0734,(float).07699,(float).07981,(float)
	    .07591,(float).07245,(float).0588,(float).04238,(float).05316,(
	    float).04869,(float).05225,(float).05266,(float).05491,(float)
	    .05634,(float).05713,(float).05924,(float).06106,(float).06367,(
	    float).06628,(float).06842,(float).06912,(float).07313,(float)
	    .07689,(float).07995,(float).07502,(float).07116,(float).05642,(
	    float).04189,(float).05249,(float).04996,(float).05356,(float)
	    .05382,(float).05623,(float).05705,(float).0592,(float).06095,(
	    float).06174,(float).06438,(float).06768,(float).06902,(float)
	    .07099,(float).07361,(float).07716,(float).0809,(float).075,(
	    float).07028,(float).05471,(float).04135,(float).05211,(float)
	    .05202,(float).05519,(float).05511,(float).05787,(float).05789,(
	    float).06115,(float).06269,(float).06349,(float).06546,(float)
	    .06929,(float).07022,(float).07334,(float).0749,(float).0785,(
	    float).08223,(float).07553,(float).06998,(float).05351,(float)
	    .04079,(float).052,(float).0528,(float).05641,(float).05686,(
	    float).05919,(float).06072,(float).06213,(float).06402,(float)
	    .06621,(float).06841,(float).07105,(float).07329,(float).07487,(
	    float).07806,(float).08175,(float).08385,(float).07636,(float)
	    .07034,(float).05247,(float).04021,(float).0521,(float).05469,(
	    float).05805,(float).05922,(float).06104,(float).06362,(float)
	    .06397,(float).06583,(float).06858,(float).07172,(float).07377,(
	    float).07671,(float).07713,(float).08153,(float).0851,(float)
	    .08613,(float).07738,(float).07157,(float).05169,(float).03982,(
	    float).05246,(float).05593,(float).06066,(float).06272,(float)
	    .06405,(float).06673,(float).06597,(float).06862,(float).07111,(
	    float).07521,(float).07767,(float).08028,(float).07946,(float)
	    .08518,(float).08877,(float).08903,(float).079,(float).07347,(
	    float).05156,(float).0396,(float).05313,(float).0591,(float)
	    .06323,(float).06472,(float).06664,(float).06976,(float).06955,(
	    float).07187,(float).07485,(float).07843,(float).08059,(float)
	    .08373,(float).08371,(float).08879,(float).09248,(float).09256,(
	    float).08176,(float).07534,(float).05198,(float).03966,(float)
	    .0542,(float).06304,(float).06633,(float).06745,(float).06981,(
	    float).07261,(float).07405,(float).0756,(float).07902,(float)
	    .08167,(float).08416,(float).08727,(float).08901,(float).09263,(
	    float).09669,(float).09657,(float).08511,(float).07742,(float)
	    .05276,(float).04003,(float).05562,(float).06599,(float).06987,(
	    float).07095,(float).07354,(float).07714,(float).07774,(float)
	    .07968,(float).084,(float).08663,(float).08859,(float).09258,(
	    float).09343,(float).09821,(float).1025,(float).1008,(float)
	    .08852,(float).08053,(float).05375,(float).04059,(float).05735,(
	    float).06985,(float).07371,(float).0754,(float).07771,(float)
	    .08242,(float).082,(float).08408,(float).08898,(float).09257,(
	    float).09403,(float).0988,(float).09861,(float).1046,(float).1088,
	    (float).1064,(float).09255,(float).08427,(float).0552,(float)
	    .0414,(float).05946,(float).07474,(float).07894,(float).08057,(
	    float).08325,(float).08687,(float).08777,(float).08997,(float)
	    .0945,(float).09782,(float).1004,(float).1043,(float).1053,(float)
	    .1104,(float).1148,(float).1129,(float).09769,(float).0881,(float)
	    .05741,(float).04262,(float).06211,(float).08012,(float).08533,(
	    float).08557,(float).08967,(float).09191,(float).09513,(float)
	    .09726,(float).1006,(float).1034,(float).1069,(float).1104,(float)
	    .1133,(float).1168,(float).1216,(float).1194,(float).1034,(float)
	    .0926,(float).0601,(float).04421,(float).06525,(float).08582,(
	    float).09124,(float).09202,(float).09601,(float).09852,(float)
	    .1018,(float).104,(float).1081,(float).1107,(float).1145,(float)
	    .118,(float).1214,(float).1248,(float).1299,(float).1269,(float)
	    .1099,(float).09819,(float).06304,(float).0461,(float).06887,(
	    float).09162,(float).09803,(float).09913,(float).1033,(float)
	    .1069,(float).1088,(float).1117,(float).1161,(float).1199,(float)
	    .1234,(float).1276,(float).1294,(float).1347,(float).1397,(float)
	    .1367,(float).1173,(float).1043,(float).06663,(float).04841,(
	    float).07309,(float).09846,(float).1067,(float).1079,(float).1124,
	    (float).1152,(float).1176,(float).1213,(float).1238,(float).1294,(
	    float).1341,(float).1375,(float).1387,(float).1448,(float).1498,(
	    float).148,(float).1256,(float).1111,(float).07119,(float).05129,(
	    float).07808,(float).1009,(float).1112,(float).1131,(float).1174,(
	    float).1203,(float).1215,(float).1263,(float).1283,(float).1349,(
	    float).14,(float).143,(float).1433,(float).1505,(float).1554,(
	    float).1532,(float).1302,(float).1151,(float).07373,(float).05293,
	    (float).08087,(float).105,(float).1168,(float).118,(float).1232,(
	    float).1243,(float).1269,(float).1323,(float).1329,(float).1395,(
	    float).146,(float).148,(float).1492,(float).1558,(float).161,(
	    float).1587,(float).1349,(float).1194,(float).07642,(float).05471,
	    (float).08386,(float).1138,(float).1253,(float).128,(float).1323,(
	    float).1357,(float).1371,(float).1421,(float).1444,(float).1518,(
	    float).1573,(float).1607,(float).1617,(float).1688,(float).1741,(
	    float).17,(float).1455,(float).1291,(float).0822,(float).05867,(
	    float).09045,(float).1246,(float).1344,(float).1375,(float).142,(
	    float).1478,(float).148,(float).1526,(float).1591,(float).1649,(
	    float).1691,(float).1747,(float).1753,(float).1836,(float).1896,(
	    float).1837,(float).1573,(float).1399,(float).08908,(float).06332,
	    (float).09808,(float).1362,(float).1455,(float).1498,(float).1538,
	    (float).161,(float).1606,(float).1652,(float).1731,(float).1796,(
	    float).1837,(float).19,(float).19,(float).1994,(float).2057,(
	    float).2002,(float).1705,(float).1522,(float).09734,(float).06885,
	    (float).107,(float).1473,(float).1616,(float).165,(float).1704,(
	    float).1739,(float).176,(float).1823,(float).186,(float).1944,(
	    float).2016,(float).2054,(float).206,(float).2154,(float).2219,(
	    float).2178,(float).1857,(float).1664,(float).1067,(float).07527,(
	    float).1172,(float).1611,(float).1759,(float).1798,(float).1854,(
	    float).1898,(float).1924,(float).1986,(float).2023,(float).2118,(
	    float).219,(float).2236,(float).2246,(float).2344,(float).2414,(
	    float).2371,(float).2032,(float).1824,(float).1172,(float).08266,(
	    float).129,(float).1732,(float).1898,(float).1974,(float).2008,(
	    float).2107,(float).2063,(float).2143,(float).2216,(float).2338,(
	    float).2387,(float).2462,(float).2423,(float).2573,(float).2644,(
	    float).2589,(float).2229,(float).2005,(float).1298,(float).09136,(
	    float).1427,(float).1897,(float).2075,(float).2154,(float).2193,(
	    float).2298,(float).2261,(float).2342,(float).2414,(float).2546,(
	    float).2603,(float).2678,(float).2649,(float).2798,(float).2876,(
	    float).2826,(float).2451,(float).2217,(float).1444,(float).1016,(
	    float).1586,(float).2101,(float).2249,(float).2317,(float).2374,(
	    float).2505,(float).2484,(float).2549,(float).2662,(float).2767,(
	    float).2813,(float).2913,(float).2913,(float).3044,(float).3134,(
	    float).3082,(float).2707,(float).2458,(float).1611,(float).1135,(
	    float).177,(float).2304,(float).2462,(float).2535,(float).2597,(
	    float).274,(float).2719,(float).2788,(float).2908,(float).3023,(
	    float).3073,(float).318,(float).3184,(float).3322,(float).342,(
	    float).3389,(float).3002,(float).273,(float).1805,(float).1275,(
	    float).1984,(float).254,(float).2702,(float).2763,(float).2847,(
	    float).2986,(float).2982,(float).3057,(float).319,(float).3296,(
	    float).336,(float).347,(float).3479,(float).3629,(float).3743,(
	    float).3742,(float).3331,(float).3042,(float).2037,(float).1442,(
	    float).2236,(float).2787,(float).2948,(float).3028,(float).3109,(
	    float).3278,(float).3259,(float).3338,(float).3495,(float).3616,(
	    float).3677,(float).3804,(float).3802,(float).3976,(float).4103,(
	    float).4136,(float).3703,(float).3406,(float).2305,(float).1638,(
	    float).2531,(float).306,(float).3259,(float).3322,(float).3431,(
	    float).357,(float).3598,(float).3684,(float).3812,(float).3939,(
	    float).4031,(float).4145,(float).4174,(float).4335,(float).4478,(
	    float).4562,(float).4136,(float).3818,(float).2616,(float).1869,(
	    float).2875,(float).3378,(float).3559,(float).3624,(float).3746,(
	    float).3904,(float).3945,(float).4027,(float).4204,(float).4303,(
	    float).4403,(float).4529,(float).459,(float).4739,(float).491,(
	    float).5038,(float).4634,(float).4287,(float).2986,(float).2147,(
	    float).3284,(float).3659,(float).3928,(float).4018,(float).4137,(
	    float).43,(float).4314,(float).4431,(float).4577,(float).4737,(
	    float).4856,(float).4978,(float).5009,(float).5205,(float).5389,(
	    float).5594,(float).5194,(float).4834,(float).3423,(float).2479,(
	    float).3765,(float).4035,(float).4306,(float).4391,(float).4531,(
	    float).4704,(float).4746,(float).4859,(float).5039,(float).5181,(
	    float).5309,(float).5447,(float).5516,(float).57,(float).5917,(
	    float).6209,(float).5833,(float).5473,(float).3937,(float).2875,(
	    float).4337,(float).4393,(float).4753,(float).4861,(float).5002,(
	    float).5168,(float).5205,(float).535,(float).55,(float).5693,(
	    float).5851,(float).5982,(float).6042,(float).6259,(float).65,(
	    float).6897,(float).6567,(float).6205,(float).4552,(float).3358,(
	    float).502,(float).4858,(float).5264,(float).5328,(float).5529,(
	    float).5611,(float).5758,(float).5914,(float).6028,(float).6201,(
	    float).6432,(float).6525,(float).6657,(float).6838,(float).712,(
	    float).7698,(float).7427,(float).7037,(float).528,(float).3941,(
	    float).5834,(float).536,(float).5803,(float).5849,(float).6089,(
	    float).6133,(float).6349,(float).6516,(float).6593,(float).6798,(
	    float).7082,(float).7156,(float).7324,(float).7504,(float).7826,(
	    float).8596,(float).8407,(float).8011,(float).6154,(float).4649,(
	    float).6815,(float).5962,(float).633,(float).633,(float).6637,(
	    float).6722,(float).6999,(float).7137,(float).7294,(float).745,(
	    float).7732,(float).7858,(float).8081,(float).8258,(float).8644,(
	    float).958,(float).9513,(float).9173,(float).7208,(float).5523,(
	    float).7996,(float).6485,(float).6854,(float).6967,(float).7219,(
	    float).7469,(float).7585,(float).7757,(float).8073,(float).8255,(
	    float).849,(float).8704,(float).8848,(float).9151,(float).9597,(
	    float)1.068,(float)1.08,(float)1.051,(float).8468,(float).6595,(
	    float).9427,(float).7072,(float).7571,(float).7683,(float).7969,(
	    float).819,(float).836,(float).8562,(float).8843,(float).906,(
	    float).9353,(float).9557,(float).9761,(float)1.005,(float)1.056,(
	    float)1.197,(float)1.232,(float)1.205,(float)1.,(float).7922,(
	    float)1.117,(float).7761,(float).8406,(float).8482,(float).8836,(
	    float).8945,(float).9231,(float).948,(float).9682,(float).994,(
	    float)1.034,(float)1.05,(float)1.076,(float)1.106,(float)1.166,(
	    float)1.345,(float)1.408,(float)1.387,(float)1.187,(float).9585,(
	    float)1.329,(float).8649,(float).9153,(float).9209,(float).9625,(
	    float).985,(float)1.018,(float)1.039,(float)1.073,(float)1.096,(
	    float)1.134,(float)1.16,(float)1.191,(float)1.225,(float)1.294,(
	    float)1.516,(float)1.61,(float)1.603,(float)1.415,(float)1.167,(
	    float)1.591,(float).9362,(float)1.007,(float)1.022,(float)1.062,(
	    float)1.095,(float)1.115,(float)1.145,(float)1.179,(float)1.217,(
	    float)1.258,(float)1.288,(float)1.311,(float)1.361,(float)1.439,(
	    float)1.708,(float)1.848,(float)1.858,(float)1.694,(float)1.43,(
	    float)1.913,(float)1.036,(float)1.116,(float)1.127,(float)1.176,(
	    float)1.201,(float)1.236,(float)1.269,(float)1.307,(float)1.341,(
	    float)1.392,(float)1.423,(float)1.459,(float)1.508,(float)1.603,(
	    float)1.928,(float)2.126,(float)2.161,(float)2.039,(float)1.764,(
	    float)2.312,(float)1.154,(float)1.225,(float)1.239,(float)1.292,(
	    float)1.334,(float)1.371,(float)1.401,(float)1.456,(float)1.493,(
	    float)1.543,(float)1.587,(float)1.627,(float)1.687,(float)1.799,(
	    float)2.19,(float)2.459,(float)2.522,(float)2.466,(float)2.193,(
	    float)2.81,(float)1.261,(float)1.362,(float)1.39,(float)1.441,(
	    float)1.497,(float)1.516,(float)1.56,(float)1.621,(float)1.679,(
	    float)1.733,(float)1.787,(float)1.813,(float)1.901,(float)2.028,(
	    float)2.507,(float)2.857,(float)2.956,(float)3.001,(float)2.746,(
	    float)3.431,(float)1.424,(float)1.534,(float)1.552,(float)1.623,(
	    float)1.665,(float)1.714,(float)1.762,(float)1.821,(float)1.881,(
	    float)1.957,(float)2.008,(float)2.055,(float)2.145,(float)2.3,(
	    float)2.887,(float)3.333,(float)3.489,(float)3.671,(float)3.466,(
	    float)4.213,(float)1.594,(float)1.73,(float)1.763,(float)1.836,(
	    float)1.902,(float)1.937,(float)1.999,(float)2.071,(float)2.155,(
	    float)2.236,(float)2.305,(float)2.346,(float)2.467,(float)2.649,(
	    float)3.357,(float)3.923,(float)4.144,(float)4.518,(float)4.407,(
	    float)5.199,(float)1.832,(float)1.992,(float)2.026,(float)2.118,(
	    float)2.191,(float)2.24,(float)2.313,(float)2.399,(float)2.498,(
	    float)2.599,(float)2.682,(float)2.734,(float)2.878,(float)3.099,(
	    float)3.955,(float)4.668,(float)4.967,(float)5.602,(float)5.65,(
	    float)6.449,(float)2.152,(float)2.347,(float)2.388,(float)2.502,(
	    float)2.582,(float)2.658,(float)2.745,(float)2.844,(float)2.962,(
	    float)3.098,(float)3.19,(float)3.27,(float)3.433,(float)3.705,(
	    float)4.749,(float)5.632,(float)6.022,(float)7.003,(float)7.306,(
	    float)8.048,(float)2.617,(float)2.856,(float)2.909,(float)3.052,(
	    float)3.169,(float)3.255,(float)3.363,(float)3.503,(float)3.656,(
	    float)3.816,(float)3.948,(float)4.039,(float)4.257,(float)4.596,(
	    float)5.869,(float)6.929,(float)7.419,(float)8.848,(float)9.537,(
	    float)10.1,(float)3.335,(float)3.662,(float)3.74,(float)3.928,(
	    float)4.088,(float)4.189,(float)4.342,(float)4.528,(float)4.736,(
	    float)4.95,(float)5.119,(float)5.228,(float)5.517,(float)5.941,(
	    float)7.494,(float)8.742,(float)9.362,(float)11.34,(float)12.59,(
	    float)12.75,(float)4.608,(float)5.08,(float)5.173,(float)5.455,(
	    float)5.645,(float)5.828,(float)6.041,(float)6.274,(float)6.551,(
	    float)6.868,(float)7.074,(float)7.25,(float)7.603,(float)8.148,(
	    float)10.,(float)11.44,(float)12.19,(float)14.81,(float)16.84,(
	    float)16.25,(float)7.076,(float)7.789,(float)7.947,(float)8.364,(
	    float)8.69,(float)8.933,(float)9.239,(float)9.62,(float)10.01,(
	    float)10.43,(float)10.74,(float)10.98,(float)11.45,(float)12.14,(
	    float)14.21,(float)15.76,(float)16.59,(float)19.9,(float)22.94,(
	    float)20.95,(float)12.68,(float)13.92,(float)14.19,(float)14.87,(
	    float)15.39,(float)15.74,(float)16.24,(float)16.8,(float)17.4,(
	    float)18.,(float)18.42,(float)18.69,(float)19.33,(float)20.12,(
	    float)21.96,(float)23.21,(float)24.03,(float)27.93,(float)32.14,(
	    float)27.4,(float)28.42,(float)30.64,(float)31.13,(float)32.27,(
	    float)33.19,(float)33.7,(float)34.42,(float)35.29,(float)36.08,(
	    float)36.73,(float)37.25,(float)37.49,(float)38.06,(float)38.51,(
	    float)38.05,(float)37.66,(float)38.01,(float)41.85,(float)47.11,(
	    float)36.86,(float)89.63,(float)93.19,(float)93.92,(float)95.21,(
	    float)96.27,(float)96.5,(float)96.88,(float)97.25,(float)97.21,(
	    float)96.55,(float)96.09,(float)95.41,(float)93.86,(float)90.71,(
	    float)77.68,(float)70.09,(float)68.38,(float)69.31,(float)74.26,(
	    float)50.85,(float)546.3,(float)522.3,(float)516.2,(float)499.2,(
	    float)484.4,(float)474.2,(float)459.7,(float)441.3,(float)421.1,(
	    float)399.6,(float)382.1,(float)370.6,(float)342.6,(float)304.7,(
	    float)205.8,(float)159.3,(float)147.5,(float)130.1,(float)125.1,(
	    float)65.26,(float)5621.,(float)4201.,(float)3938.,(float)3362.,(
	    float)2954.,(float)2722.,(float)2420.,(float)2096.,(float)1798.,(
	    float)1542.,(float)1362.,(float)1262.,(float)1035.,(float)791.2,(
	    float)361.8,(float)235.3,(float)207.6,(float)165.9,(float)149.9,(
	    float)69.8 };
    static real qhr[1660]	/* was [20][83] */ = { (float)1.268e-22,(
	    float)8.915e-22,(float)-6.146e-22,(float)-8.679e-22,(float)
	    5.549e-22,(float)3.355e-22,(float)2.263e-22,(float)-1.172e-21,(
	    float)-5.743e-22,(float)-4.904e-22,(float)-1.071e-22,(float)
	    1.195e-21,(float)-2.802e-22,(float)7.227e-22,(float)3.341e-22,(
	    float)4.569e-23,(float)3.013e-23,(float)-8.339e-23,(float)
	    -1.652e-22,(float)-4.718e-23,(float).02476,(float).01047,(float)
	    .01969,(float).01142,(float).02641,(float).01874,(float).0132,(
	    float).02621,(float).02658,(float).01744,(float).02586,(float)
	    .02337,(float).02409,(float).01962,(float)1.668e-4,(float)
	    -.003494,(float)-.006586,(float)-.003001,(float)-3.713e-5,(float)
	    -3.367e-4,(float).02255,(float).008794,(float).01817,(float)
	    .01278,(float).04434,(float).02124,(float).01774,(float).05035,(
	    float).04799,(float).02926,(float).05239,(float).04146,(float)
	    .05438,(float).05306,(float).01905,(float).004438,(float)-.006701,
	    (float)-.001501,(float).003969,(float)9.818e-4,(float)-.008459,(
	    float)-.01221,(float)-.00623,(float)-.007503,(float).01074,(float)
	    -2.99e-4,(float)-1.102e-4,(float).01893,(float).02133,(float)
	    .01488,(float).02991,(float).02593,(float).03805,(float).04545,(
	    float).03846,(float).01387,(float).003526,(float).009853,(float)
	    .00787,(float).005409,(float)-.037,(float)-.02791,(float)-.03044,(
	    float)-.02461,(float)-.03317,(float)-.02018,(float)-.01613,(float)
	    -.02101,(float)-.01628,(float)-.003591,(float)-.005289,(float)
	    .004162,(float).007287,(float).02058,(float).0506,(float).01729,(
	    float).01083,(float).01559,(float).004872,(float).007205,(float)
	    -.05391,(float)-.04559,(float)-.05209,(float)-.04284,(float)
	    -.05365,(float)-.03582,(float)-.03241,(float)-.03886,(float)
	    -.03471,(float)-.01959,(float)-.02169,(float)-.008725,(float)
	    -.006781,(float).009166,(float).04795,(float).0162,(float).008649,
	    (float).01043,(float)6.642e-4,(float).004406,(float)-.05697,(
	    float)-.05726,(float)-.05398,(float)-.05172,(float)-.04567,(float)
	    -.04506,(float)-.04148,(float)-.03113,(float)-.02654,(float)
	    -.01971,(float)-.01274,(float)-.008606,(float).002331,(float)
	    .01769,(float).03824,(float).01017,(float)-.001759,(float).002577,
	    (float)-.00274,(float)-4.729e-4,(float)-.04863,(float)-.04209,(
	    float)-.03391,(float)-.03515,(float)-.01946,(float)-.03142,(float)
	    -.02489,(float)-.0147,(float)-8.06e-4,(float)3.157e-4,(float)
	    .01198,(float).004025,(float).02494,(float).03513,(float).03434,(
	    float)-.00219,(float)-.01359,(float)-.003925,(float)-.004686,(
	    float)-.005499,(float)-.0216,(float)-.009986,(float)-.007471,(
	    float)-.003311,(float)-.00409,(float)-.001718,(float).006124,(
	    float).00629,(float).01513,(float).02603,(float).02723,(float)
	    .02814,(float).03896,(float).04622,(float).03286,(float)-.01421,(
	    float)-.02471,(float)-.01012,(float)-.006046,(float)-.01012,(
	    float).002527,(float).008919,(float).003891,(float).01311,(float)
	    .008491,(float).02246,(float).02474,(float).02117,(float).02537,(
	    float).03606,(float).03617,(float).04608,(float).04654,(float)
	    .05277,(float).02928,(float)-.02101,(float)-.03343,(float)-.01511,
	    (float)-.007327,(float)-.01419,(float).007879,(float).01561,(
	    float).01574,(float).02056,(float).01844,(float).02391,(float)
	    .02857,(float).02847,(float).03273,(float).0422,(float).04163,(
	    float).04622,(float).05034,(float).05581,(float).0295,(float)
	    -.02372,(float)-.0376,(float)-.02013,(float)-.008648,(float)
	    -.01783,(float).008199,(float).01519,(float).02093,(float).02022,(
	    float).02527,(float).02167,(float).02612,(float).02779,(float)
	    .03695,(float).0412,(float).04372,(float).04163,(float).05053,(
	    float).05464,(float).03271,(float)-.02141,(float)-.03836,(float)
	    -.02543,(float)-.009896,(float)-.02108,(float).01009,(float)
	    .01543,(float).01697,(float).01896,(float).02248,(float).0219,(
	    float).02471,(float).02789,(float).03215,(float).03473,(float)
	    .03843,(float).03901,(float).04468,(float).04927,(float).03308,(
	    float)-.01531,(float)-.0367,(float)-.02928,(float)-.01121,(float)
	    -.02385,(float).01381,(float).01286,(float).01185,(float).0154,(
	    float).01768,(float).02166,(float).0214,(float).02736,(float)
	    .02589,(float).02781,(float).03191,(float).03714,(float).03791,(
	    float).04313,(float).03264,(float)-.01001,(float)-.03092,(float)
	    -.03297,(float)-.01258,(float)-.02614,(float).0106,(float).01151,(
	    float).01116,(float).01383,(float).01647,(float).0187,(float)
	    .01898,(float).02288,(float).02356,(float).02484,(float).02863,(
	    float).03139,(float).03378,(float).03835,(float).03297,(float)
	    -.004689,(float)-.02429,(float)-.03565,(float)-.01432,(float)
	    -.02769,(float).007513,(float).01267,(float).01147,(float).01481,(
	    float).01332,(float).01663,(float).01882,(float).01786,(float)
	    .01992,(float).02373,(float).02415,(float).02669,(float).02862,(
	    float).03267,(float).03119,(float).001363,(float)-.01861,(float)
	    -.037,(float)-.01624,(float)-.02862,(float).006562,(float).009975,
	    (float).01049,(float).01219,(float).01159,(float).01396,(float)
	    .01571,(float).01574,(float).01756,(float).02128,(float).02113,(
	    float).02362,(float).02494,(float).02844,(float).02856,(float)
	    .005275,(float)-.01251,(float)-.03797,(float)-.01813,(float)
	    -.02888,(float).004469,(float).007821,(float).009795,(float)
	    .01017,(float).01144,(float).01111,(float).01304,(float).01373,(
	    float).01675,(float).01941,(float).01978,(float).0202,(float)
	    .02303,(float).026,(float).02664,(float).007626,(float)-.008017,(
	    float)-.03668,(float)-.01998,(float)-.02848,(float).004342,(float)
	    .006247,(float).007055,(float).008202,(float).009839,(float)
	    .009925,(float).01121,(float).01353,(float).01476,(float).0165,(
	    float).01789,(float).01838,(float).02103,(float).02379,(float)
	    .02484,(float).008819,(float)-.004221,(float)-.03487,(float)
	    -.02148,(float)-.02766,(float).00387,(float).005102,(float)
	    .005709,(float).006948,(float).008875,(float).008866,(float)
	    .009889,(float).0123,(float).01358,(float).01463,(float).01657,(
	    float).01688,(float).01951,(float).02197,(float).02333,(float)
	    .008749,(float)-.001206,(float)-.03197,(float)-.02289,(float)
	    -.02647,(float).003488,(float).005426,(float).005082,(float)
	    .006972,(float).006834,(float).009426,(float).01011,(float).01004,
	    (float).01168,(float).0138,(float).01451,(float).01596,(float)
	    .01736,(float).01969,(float).02166,(float).008743,(float)2.898e-4,
	    (float)-.02826,(float)-.0236,(float)-.02509,(float).001873,(float)
	    .004434,(float).004872,(float).00619,(float).006969,(float)
	    .007567,(float).008898,(float).009056,(float).01147,(float).01319,
	    (float).0141,(float).01429,(float).01671,(float).01876,(float)
	    .01996,(float).008694,(float).001564,(float)-.02552,(float)
	    -.02407,(float)-.02369,(float)8.242e-4,(float).0047,(float)
	    .004787,(float).006439,(float).005712,(float).007128,(float)
	    .008921,(float).007886,(float).01027,(float).01287,(float).0128,(
	    float).01335,(float).01536,(float).01747,(float).018,(float)
	    .008697,(float).001966,(float)-.02208,(float)-.02368,(float)
	    -.02232,(float)1.707e-4,(float).002735,(float).003997,(float)
	    .004648,(float).006149,(float).00571,(float).007182,(float)
	    .007899,(float).01043,(float).01178,(float).01285,(float).01243,(
	    float).01519,(float).01693,(float).01671,(float).008481,(float)
	    .001962,(float)-.01931,(float)-.0229,(float)-.02109,(float)
	    -5.39e-4,(float).002602,(float).003696,(float).004435,(float)
	    .005588,(float).005228,(float).006796,(float).007281,(float)
	    .01003,(float).01128,(float).01251,(float).01168,(float).01491,(
	    float).0167,(float).01536,(float).007835,(float).002216,(float)
	    -.01667,(float)-.02176,(float)-.02002,(float)-3.612e-4,(float)
	    1.81e-4,(float).001473,(float).001918,(float).00611,(float)
	    .004137,(float).004625,(float).008963,(float).01019,(float)
	    .009016,(float).0129,(float).01164,(float).01537,(float).01727,(
	    float).01453,(float).007277,(float).002135,(float)-.01454,(float)
	    -.02038,(float)-.01907,(float)-2.495e-4,(float).001072,(float)
	    .001427,(float).00266,(float).004177,(float).004865,(float)
	    .005367,(float).008128,(float).008618,(float).009107,(float)
	    .01137,(float).01192,(float).01399,(float).01623,(float).01374,(
	    float).006824,(float).001819,(float)-.01253,(float)-.01895,(float)
	    -.01834,(float)-.001048,(float)-1.217e-4,(float).001349,(float)
	    .001885,(float).004484,(float).003448,(float).004419,(float)
	    .008272,(float).008749,(float).009229,(float).01142,(float).01125,
	    (float).01388,(float).0159,(float).0138,(float).006472,(float)
	    .001428,(float)-.01133,(float)-.01772,(float)-.0178,(float)
	    -.001244,(float).001517,(float).001024,(float).00319,(float)
	    .002991,(float).004953,(float).006144,(float).006622,(float)
	    .00764,(float).00954,(float).01031,(float).01151,(float).01281,(
	    float).0147,(float).01355,(float).006108,(float).001478,(float)
	    -.01019,(float)-.01648,(float)-.01737,(float)-.002457,(float)
	    4.575e-4,(float).001414,(float).002531,(float).003738,(float)
	    .00401,(float).005411,(float).006194,(float).008194,(float)
	    .009797,(float).01065,(float).01126,(float).01291,(float).01446,(
	    float).01344,(float).005573,(float).001519,(float)-.009347,(float)
	    -.01539,(float)-.01708,(float)-.001549,(float).001173,(float)
	    4.724e-4,(float).003014,(float).002014,(float).005156,(float)
	    .006249,(float).006127,(float).006869,(float).009819,(float)
	    .009588,(float).01192,(float).01206,(float).01374,(float).01313,(
	    float).005435,(float).001486,(float)-.008573,(float)-.01445,(
	    float)-.0169,(float)-.004067,(float)3.59e-4,(float).001628,(float)
	    .002541,(float).004061,(float).003211,(float).005222,(float)
	    .006022,(float).008858,(float).01013,(float).01143,(float).01064,(
	    float).01379,(float).01538,(float).01261,(float).005348,(float)
	    .001357,(float)-.008007,(float)-.01367,(float)-.01683,(float)
	    -.001796,(float)-.001717,(float)-9.738e-4,(float)3.263e-4,(float)
	    .003613,(float).003659,(float).003798,(float).008311,(float)
	    .00832,(float).008269,(float).01127,(float).01222,(float).01385,(
	    float).01575,(float).01256,(float).005386,(float).001147,(float)
	    -.007655,(float)-.01312,(float)-.01692,(float)-.002753,(float)
	    8.142e-4,(float)7.231e-4,(float).002842,(float).002326,(float)
	    .004971,(float).006125,(float).005975,(float).00766,(float).01005,
	    (float).0105,(float).01246,(float).01311,(float).01494,(float)
	    .01244,(float).005309,(float).001426,(float)-.007264,(float)
	    -.01278,(float)-.01707,(float)-.004492,(float)-8.557e-4,(float)
	    .001648,(float).001856,(float).004274,(float).003029,(float)
	    .004896,(float).006272,(float).009486,(float).01096,(float).0121,(
	    float).01146,(float).01437,(float).01548,(float).01323,(float)
	    .005225,(float).001829,(float)-.006791,(float)-.01244,(float)
	    -.01725,(float)-.002752,(float)-1.025e-4,(float)-3.753e-4,(float)
	    .002172,(float).002807,(float).004759,(float).005991,(float)
	    .007114,(float).008386,(float).01057,(float).01144,(float).01251,(
	    float).01403,(float).01542,(float).01401,(float).005663,(float)
	    .001888,(float)-.006614,(float)-.01223,(float)-.01755,(float)
	    -.004822,(float).001248,(float).001837,(float).003738,(float)
	    .003406,(float).004766,(float).007145,(float).005577,(float)
	    .009288,(float).01216,(float).01216,(float).01208,(float).01477,(
	    float).01628,(float).01399,(float).006047,(float).002242,(float)
	    -.006553,(float)-.01218,(float)-.01795,(float)-.005504,(float)
	    -.001358,(float).001687,(float).001736,(float).005575,(float)
	    .002813,(float).005129,(float).00745,(float).01118,(float).01194,(
	    float).01408,(float).01183,(float).01656,(float).01779,(float)
	    .01446,(float).006261,(float).00305,(float)-.00638,(float)-.01215,
	    (float)-.01836,(float)-.003245,(float)-7.803e-4,(float)-4.635e-4,(
	    float).001778,(float).004299,(float).004695,(float).00589,(float)
	    .008602,(float).01032,(float).01127,(float).01369,(float).01354,(
	    float).01649,(float).01797,(float).01532,(float).007047,(float)
	    .003673,(float)-.00626,(float)-.01213,(float)-.01885,(float)
	    -.003857,(float)7.688e-4,(float).001536,(float).003486,(float)
	    .003577,(float).005471,(float).007235,(float).007773,(float)
	    .01004,(float).01264,(float).01331,(float).01443,(float).01624,(
	    float).01813,(float).01538,(float).008086,(float).004362,(float)
	    -.006197,(float)-.01218,(float)-.01937,(float)-.005823,(float)
	    -7.587e-4,(float).001999,(float).002549,(float).0059,(float)
	    .003873,(float).006232,(float).008895,(float).01187,(float).01296,
	    (float).015,(float).0143,(float).01775,(float).01948,(float)
	    .01533,(float).008532,(float).005665,(float)-.006101,(float)
	    -.01234,(float)-.01993,(float)-.002124,(float).001022,(float)
	    5.311e-4,(float).003672,(float).003044,(float).007237,(float)
	    .008208,(float).008417,(float).009991,(float).01291,(float).01354,
	    (float).01616,(float).01668,(float).01875,(float).01614,(float)
	    .008645,(float).006338,(float)-.006076,(float)-.01244,(float)
	    -.02026,(float)-.003315,(float)-.002572,(float)5.462e-5,(float)
	    7.203e-4,(float).006663,(float).004518,(float).005417,(float)
	    .01028,(float).01288,(float).01248,(float).01626,(float).01507,(
	    float).01908,(float).02064,(float).01667,(float).009205,(float)
	    .006978,(float)-.006109,(float)-.01256,(float)-.02061,(float)
	    -.00196,(float)-4.315e-4,(float)-1.677e-4,(float).002449,(float)
	    .004768,(float).006216,(float).007206,(float).01081,(float).01206,
	    (float).01347,(float).016,(float).01574,(float).01936,(float)
	    .02149,(float).01881,(float).01082,(float).007626,(float)-.005996,
	    (float)-.01278,(float)-.02125,(float)-.003327,(float).001019,(
	    float).001806,(float).003997,(float).00532,(float).007103,(float)
	    .008669,(float).009788,(float).01274,(float).01478,(float).01651,(
	    float).01641,(float).01989,(float).02214,(float).02027,(float)
	    .01264,(float).008406,(float)-.005687,(float)-.01301,(float)
	    -.02188,(float)-.005509,(float).001875,(float).002745,(float)
	    .005103,(float).006569,(float).007084,(float).00976,(float)
	    .009921,(float).01348,(float).01571,(float).01701,(float).01743,(
	    float).02018,(float).02241,(float).02104,(float).01413,(float)
	    .009918,(float)-.005619,(float)-.01334,(float)-.02266,(float)
	    -.003118,(float)-4.092e-4,(float).001526,(float).003269,(float)
	    .0072,(float).007509,(float).008739,(float).01222,(float).01401,(
	    float).01539,(float).01762,(float).0195,(float).02084,(float)
	    .02331,(float).02245,(float).01593,(float).0114,(float)-.005624,(
	    float)-.01371,(float)-.02344,(float)-.001082,(float)-6.854e-4,(
	    float)-3.013e-4,(float).002811,(float).007851,(float).00896,(
	    float).009252,(float).01378,(float).01469,(float).01537,(float)
	    .01858,(float).0209,(float).02196,(float).0245,(float).02493,(
	    float).01813,(float).01239,(float)-.005103,(float)-.01394,(float)
	    -.02414,(float)3.212e-4,(float).002913,(float)-2.097e-4,(float)
	    .005948,(float).005473,(float).01228,(float).01279,(float).0138,(
	    float).01326,(float).01705,(float).01768,(float).02259,(float)
	    .02164,(float).02494,(float).02767,(float).02008,(float).01399,(
	    float)-.0046,(float)-.01419,(float)-.02489,(float).001378,(float)
	    .005484,(float).001492,(float).008544,(float).004245,(float)
	    .01379,(float).01504,(float).014,(float).01307,(float).01936,(
	    float).01778,(float).02357,(float).02215,(float).02608,(float)
	    .03004,(float).0216,(float).01624,(float)-.004475,(float)-.01459,(
	    float)-.02573,(float)-4.333e-4,(float).00629,(float).005193,(
	    float).009975,(float).006273,(float).01267,(float).01547,(float)
	    .01373,(float).01588,(float).02237,(float).02046,(float).02261,(
	    float).02476,(float).02845,(float).03304,(float).0238,(float)
	    .01771,(float)-.003889,(float)-.01482,(float)-.02651,(float)
	    -.002347,(float).007964,(float).007723,(float).01191,(float)
	    .007705,(float).0131,(float).01708,(float).01269,(float).0178,(
	    float).02453,(float).02224,(float).02303,(float).02642,(float)
	    .02964,(float).03633,(float).02666,(float).01879,(float)-.002786,(
	    float)-.01493,(float)-.02712,(float)-.001399,(float).008576,(
	    float).006748,(float).0123,(float).006867,(float).01566,(float)
	    .01849,(float).0136,(float).01682,(float).02419,(float).02141,(
	    float).0264,(float).02579,(float).02964,(float).03829,(float)
	    .02888,(float).02146,(float)-.002153,(float)-.01521,(float)
	    -.02791,(float)-.002813,(float).0087,(float).007801,(float).01287,
	    (float).007293,(float).01553,(float).01906,(float).01369,(float)
	    .01716,(float).0255,(float).02159,(float).02738,(float).02594,(
	    float).03016,(float).03985,(float).03036,(float).02455,(float)
	    -.001486,(float)-.01542,(float)-.02868,(float)-.004009,(float)
	    .009681,(float).008641,(float).01418,(float).006801,(float).01539,
	    (float).0203,(float).01264,(float).01738,(float).02762,(float)
	    .02189,(float).0267,(float).02639,(float).03082,(float).0427,(
	    float).03324,(float).02607,(float)5.661e-6,(float)-.01534,(float)
	    -.02913,(float)-.006005,(float).007736,(float).008715,(float)
	    .01277,(float).008444,(float).01335,(float).01875,(float).01214,(
	    float).01952,(float).0283,(float).02413,(float).02505,(float)
	    .02864,(float).03255,(float).04578,(float).03643,(float).02811,(
	    float).001504,(float)-.0154,(float)-.02965,(float)-.005521,(float)
	    .006657,(float).006408,(float).01147,(float).006439,(float).01344,
	    (float).01797,(float).01224,(float).01837,(float).02694,(float)
	    .02357,(float).02563,(float).02866,(float).03317,(float).04765,(
	    float).03835,(float).03165,(float).002807,(float)-.0154,(float)
	    -.03011,(float)-.005109,(float).001938,(float).002116,(float)
	    .006827,(float).00586,(float).01152,(float).01404,(float).01369,(
	    float).01788,(float).0236,(float).02375,(float).02602,(float)
	    .02937,(float).0346,(float).04812,(float).04051,(float).0345,(
	    float).004649,(float)-.01501,(float)-.03031,(float)-.007876,(
	    float)5.279e-4,(float)-3.111e-4,(float).005626,(float).003223,(
	    float).01025,(float).01334,(float).01118,(float).01547,(float)
	    .02251,(float).02155,(float).02495,(float).02752,(float).03345,(
	    float).04968,(float).04391,(float).03637,(float).006591,(float)
	    -.0147,(float)-.03038,(float)-.008446,(float)-.005814,(float)
	    -.007032,(float)-7.231e-4,(float)3.031e-4,(float).008236,(float)
	    .008714,(float).01219,(float).01178,(float).01713,(float).01819,(
	    float).02565,(float).02461,(float).03161,(float).05035,(float)
	    .04708,(float).03955,(float).009116,(float)-.0142,(float)-.03013,(
	    float)-.01587,(float)-.01153,(float)-.009027,(float)-.005651,(
	    float)2.25e-4,(float).001398,(float).003267,(float).01015,(float)
	    .01121,(float).01383,(float).01756,(float).02134,(float).02407,(
	    float).0314,(float).05016,(float).04807,(float).0436,(float)
	    .01215,(float)-.01317,(float)-.02967,(float)-.02224,(float)
	    -.01495,(float)-.01203,(float)-.008878,(float)-.003539,(float)
	    -.003515,(float)-4.783e-4,(float).005442,(float).007955,(float)
	    .01067,(float).01458,(float).01729,(float).02125,(float).0285,(
	    float).05161,(float).05076,(float).04602,(float).01426,(float)
	    -.01214,(float)-.02892,(float)-.02776,(float)-.01527,(float)
	    -.01565,(float)-.009875,(float)-.01139,(float)-.005077,(float)
	    -.001286,(float)-.003573,(float).001397,(float).008326,(float)
	    .008183,(float).01336,(float).01523,(float).02306,(float).05113,(
	    float).05418,(float).04828,(float).01796,(float)-.01083,(float)
	    -.02755,(float)-.02752,(float)-.02602,(float)-.02859,(float)
	    -.02113,(float)-.0182,(float)-.01056,(float)-.01079,(float)
	    -.003473,(float)-.005306,(float)-.00201,(float).00276,(float)
	    .01032,(float).01101,(float).02084,(float).04874,(float).05633,(
	    float).05033,(float).0228,(float)-.008337,(float)-.02562,(float)
	    -.03971,(float)-.03755,(float)-.0326,(float)-.03108,(float)
	    -.01868,(float)-.02413,(float)-.02215,(float)-.01,(float)-.006195,
	    (float)-.007234,(float).001497,(float)-2.555e-4,(float).009503,(
	    float).01854,(float).04621,(float).0568,(float).05366,(float)
	    .02606,(float)-.006306,(float)-.02328,(float)-.05038,(float)
	    -.04496,(float)-.03945,(float)-.03878,(float)-.02639,(float)
	    -.03163,(float)-.03003,(float)-.01888,(float)-.01378,(float)
	    -.01587,(float)-.005853,(float)-.006704,(float).002281,(float)
	    .01152,(float).04301,(float).05788,(float).05725,(float).03027,(
	    float)-.003129,(float)-.02036,(float)-.05438,(float)-.05354,(
	    float)-.05263,(float)-.04865,(float)-.03951,(float)-.03768,(float)
	    -.03895,(float)-.02498,(float)-.02609,(float)-.02868,(float)
	    -.01693,(float)-.01122,(float)-.007148,(float).005739,(float)
	    .03629,(float).0568,(float).0602,(float).03632,(float).001217,(
	    float)-.01595,(float)-.06782,(float)-.07206,(float)-.06436,(float)
	    -.06617,(float)-.04185,(float)-.0544,(float)-.05641,(float)
	    -.03189,(float)-.02939,(float)-.04142,(float)-.02007,(float)
	    -.02425,(float)-.01019,(float).001794,(float).03227,(float).05861,
	    (float).05813,(float).04196,(float).005563,(float)-.01081,(float)
	    -.08576,(float)-.07231,(float)-.06736,(float)-.0658,(float)
	    -.05879,(float)-.06391,(float)-.05817,(float)-.05337,(float)
	    -.04444,(float)-.04308,(float)-.03546,(float)-.03827,(float)
	    -.02554,(float)-.01378,(float).02879,(float).05881,(float).05912,(
	    float).04816,(float).01181,(float)-.004895,(float)-.09301,(float)
	    -.08516,(float)-.08161,(float)-.07857,(float)-.07798,(float)
	    -.07361,(float)-.07001,(float)-.06653,(float)-.0624,(float)
	    -.05402,(float)-.05307,(float)-.04682,(float)-.04225,(float)
	    -.0281,(float).02171,(float).0561,(float).06185,(float).05429,(
	    float).01875,(float).002784,(float)-.1103,(float)-.103,(float)
	    -.09714,(float)-.09628,(float)-.08801,(float)-.09088,(float)
	    -.08741,(float)-.07995,(float)-.07218,(float)-.06868,(float)
	    -.06244,(float)-.06168,(float)-.05133,(float)-.03814,(float)
	    .02015,(float).05159,(float).06386,(float).06013,(float).02696,(
	    float).01122,(float)-.1294,(float)-.1099,(float)-.1062,(float)
	    -.1027,(float)-.1085,(float)-.1041,(float)-.09479,(float)-.1041,(
	    float)-.08915,(float)-.0747,(float)-.0791,(float)-.0788,(float)
	    -.06732,(float)-.05394,(float).01023,(float).04816,(float).06292,(
	    float).06981,(float).03742,(float).02192,(float)-.1358,(float)
	    -.1343,(float)-.1375,(float)-.1287,(float)-.1288,(float)-.1144,(
	    float)-.1152,(float)-.1105,(float)-.1097,(float)-.1022,(float)
	    -.0968,(float)-.0822,(float)-.08226,(float)-.06431,(float)
	    -.001314,(float).04755,(float).05653,(float).07773,(float).04885,(
	    float).0338,(float)-.1531,(float)-.1605,(float)-.1647,(float)
	    -.1558,(float)-.1413,(float)-.135,(float)-.1404,(float)-.1158,(
	    float)-.1203,(float)-.127,(float)-.1035,(float)-.09563,(float)
	    -.08529,(float)-.0636,(float)-.004303,(float).04797,(float).05427,
	    (float).08519,(float).06171,(float).04555,(float)-.1867,(float)
	    -.1845,(float)-.1723,(float)-.1757,(float)-.1492,(float)-.1664,(
	    float)-.1631,(float)-.1343,(float)-.124,(float)-.1337,(float)
	    -.1067,(float)-.1193,(float)-.08863,(float)-.06949,(float)-.01217,
	    (float).04573,(float).05557,(float).09469,(float).07738,(float)
	    .05906,(float)-.1947,(float)-.211,(float)-.212,(float)-.2041,(
	    float)-.1788,(float)-.1729,(float)-.1828,(float)-.1388,(float)
	    -.1513,(float)-.1619,(float)-.129,(float)-.115,(float)-.1055,(
	    float)-.07644,(float)-.01671,(float).04337,(float).06068,(float)
	    .1001,(float).09339,(float).0714,(float)-.2355,(float)-.2412,(
	    float)-.2262,(float)-.2289,(float)-.185,(float)-.2081,(float)
	    -.2092,(float)-.1523,(float)-.1526,(float)-.1699,(float)-.1278,(
	    float)-.1342,(float)-.1014,(float)-.0697,(float)-.004811,(float)
	    .04462,(float).06457,(float).1114,(float).1113,(float).08277,(
	    float)-.2765,(float)-.2572,(float)-.2233,(float)-.235,(float)
	    -.1963,(float)-.2327,(float)-.2178,(float)-.1769,(float)-.1536,(
	    float)-.1539,(float)-.1274,(float)-.1454,(float)-.1001,(float)
	    -.07127,(float)3.506e-4,(float).05837,(float).0675,(float).116,(
	    float).1272,(float).09052,(float)-.2621,(float)-.2445,(float)
	    -.2653,(float)-.2259,(float)-.2498,(float)-.1805,(float)-.1823,(
	    float)-.1698,(float)-.1977,(float)-.1567,(float)-.1616,(float)
	    -.08839,(float)-.124,(float)-.07447,(float).01281,(float).09104,(
	    float).0557,(float).1219,(float).1371,(float).09304,(float)-.2857,
	    (float)-.2903,(float)-.2455,(float)-.2563,(float)-.1505,(float)
	    -.207,(float)-.2122,(float)-.1072,(float)-.08945,(float)-.1272,(
	    float)-.05051,(float)-.07265,(float)-.01593,(float).01157,(float)
	    .09823,(float).1244,(float).07216,(float).1272,(float).1355,(
	    float).08736,(float)-.2604,(float)-.2884,(float)-.09803,(float)
	    -.2237,(float).06993,(float)-.2145,(float)-.2078,(float).01727,(
	    float).1337,(float)-.008717,(float).1614,(float)-.03975,(float)
	    .1754,(float).1504,(float).1908,(float).142,(float).1193,(float)
	    .0856,(float).07581,(float).05598,(float).08863,(float).07441,(
	    float).1603,(float).09307,(float).2651,(float).1464,(float).1096,(
	    float).1725,(float).2616,(float).1571,(float).2539,(float).1465,(
	    float).2386,(float).1925,(float).1472,(float).08351,(float).07938,
	    (float).03885,(float).02173,(float).01352,(float)1.341e-18,(float)
	    -6.003e-18,(float)-7.181e-18,(float)-3.242e-18,(float)-4.398e-18,(
	    float)1.28e-18,(float)2.688e-18,(float)5.777e-18,(float)7.57e-19,(
	    float)2.719e-18,(float)-2.801e-19,(float)-9.238e-19,(float)
	    -8.006e-19,(float)8.456e-19,(float)-4.975e-19,(float)2.65e-19,(
	    float)-1.204e-19,(float)-3.065e-19,(float)9.486e-20,(float)
	    -1.058e-19 };
    static real uhr[1660]	/* was [20][83] */ = { (float)-.3243,(float)
	    -.3504,(float)-.3625,(float)-.3853,(float)-.4323,(float)-.4322,(
	    float)-.4427,(float)-.496,(float)-.5164,(float)-.5181,(float)
	    -.5654,(float)-.5712,(float)-.5994,(float)-.5925,(float)-.3548,(
	    float)-.236,(float)-.2183,(float)-.1136,(float)-.0644,(float)
	    -.07657,(float)-.2275,(float)-.2583,(float)-.2725,(float)-.2956,(
	    float)-.3391,(float)-.3413,(float)-.3553,(float)-.4083,(float)
	    -.4297,(float)-.4384,(float)-.4828,(float)-.4907,(float)-.5223,(
	    float)-.5233,(float)-.3133,(float)-.2085,(float)-.1922,(float)
	    -.09834,(float)-.05294,(float)-.07138,(float)-.01795,(float)
	    -.04707,(float)-.05735,(float)-.07782,(float)-.1088,(float)-.1128,
	    (float)-.1299,(float)-.1697,(float)-.1933,(float)-.2108,(float)
	    -.2465,(float)-.2569,(float)-.2911,(float)-.3052,(float)-.1695,(
	    float)-.1093,(float)-.1014,(float)-.04735,(float)-.02004,(float)
	    -.0524,(float).04439,(float).02789,(float).02289,(float).01077,(
	    float)-.003171,(float)-.007923,(float)-.02048,(float)-.04294,(
	    float)-.05802,(float)-.07562,(float)-.09581,(float)-.1062,(float)
	    -.1285,(float)-.1376,(float)-.04147,(float)-.009279,(float)
	    -.008594,(float).00231,(float).00555,(float)-.02943,(float).03578,
	    (float).03269,(float).03027,(float).02519,(float).01441,(float)
	    .01379,(float).008951,(float)-.005037,(float)-.01449,(float)
	    -.02196,(float)-.03659,(float)-.04171,(float)-.05458,(float)
	    -.05303,(float).02811,(float).0544,(float).05487,(float).03021,(
	    float).01412,(float)-.01269,(float).01267,(float).0135,(float)
	    .007699,(float).01022,(float)2.126e-4,(float).007758,(float)
	    .004957,(float)-.004217,(float)-.01248,(float)-.01282,(float)
	    -.02283,(float)-.01919,(float)-.0296,(float)-.02092,(float).05365,
	    (float).08631,(float).08811,(float).038,(float).01288,(float)
	    -.004242,(float)-.01034,(float)-.01829,(float)-.02313,(float)
	    -.01947,(float)-.01919,(float)-.01261,(float)-.01808,(float)
	    -.01286,(float)-.02362,(float)-.0263,(float)-.0262,(float)-.01819,
	    (float)-.0265,(float)-.01634,(float).05498,(float).09693,(float)
	    .09656,(float).03552,(float).00989,(float)-.001328,(float)-.03552,
	    (float)-.04433,(float)-.04096,(float)-.04326,(float)-.0334,(float)
	    -.03633,(float)-.04058,(float)-.03018,(float)-.03299,(float)
	    -.03815,(float)-.03239,(float)-.02964,(float)-.03047,(float)
	    -.02247,(float).0491,(float).09204,(float).09036,(float).0308,(
	    float).007319,(float)-8.748e-4,(float)-.05009,(float)-.0421,(
	    float)-.03741,(float)-.03956,(float)-.03951,(float)-.04292,(float)
	    -.03879,(float)-.03998,(float)-.03497,(float)-.03195,(float)
	    -.03282,(float)-.03479,(float)-.02948,(float)-.02243,(float)
	    .04089,(float).07919,(float).07855,(float).02686,(float).005413,(
	    float)-.001035,(float)-.04099,(float)-.03186,(float)-.03494,(
	    float)-.03079,(float)-.03483,(float)-.02978,(float)-.02715,(float)
	    -.03348,(float)-.02895,(float)-.02503,(float)-.0255,(float)
	    -.02321,(float)-.02136,(float)-.01528,(float).02886,(float).06369,
	    (float).06551,(float).024,(float).003973,(float)-.001153,(float)
	    -.02721,(float)-.03047,(float)-.03292,(float)-.02897,(float)
	    -.02856,(float)-.02259,(float)-.024,(float)-.0187,(float)-.02253,(
	    float)-.02079,(float)-.01786,(float)-.01096,(float)-.01299,(float)
	    -.00669,(float).01992,(float).04884,(float).05402,(float).02242,(
	    float).003066,(float)-.001031,(float)-.02594,(float)-.02724,(
	    float)-.02654,(float)-.02497,(float)-.01972,(float)-.02031,(float)
	    -.02033,(float)-.01341,(float)-.01395,(float)-.01406,(float)
	    -.009588,(float)-.007853,(float)-.005201,(float)-6.13e-4,(float)
	    .01745,(float).03684,(float).04436,(float).02143,(float).002644,(
	    float)-6.918e-4,(float)-.02533,(float)-.02107,(float)-.01916,(
	    float)-.01821,(float)-.01776,(float)-.01749,(float)-.01474,(float)
	    -.01375,(float)-.01104,(float)-.007127,(float)-.007054,(float)
	    -.006274,(float)-.002967,(float)5.223e-4,(float).0156,(float)
	    .0287,(float).03628,(float).02109,(float).002585,(float)-9.753e-5,
	    (float)-.02248,(float)-.0183,(float)-.01894,(float)-.0161,(float)
	    -.01724,(float)-.01337,(float)-.01181,(float)-.01335,(float)
	    -.01042,(float)-.006479,(float)-.00642,(float)-.003348,(float)
	    -.002263,(float).001365,(float).01214,(float).02269,(float).03001,
	    (float).02134,(float).002752,(float)7.33e-4,(float)-.0215,(float)
	    -.01933,(float)-.0192,(float)-.01711,(float)-.01587,(float)
	    -.01375,(float)-.01288,(float)-.01116,(float)-.009425,(float)
	    -.007255,(float)-.005374,(float)-.002809,(float)-.001152,(float)
	    .002894,(float).01022,(float).0186,(float).02499,(float).02156,(
	    float).00326,(float).001688,(float)-.02233,(float)-.02083,(float)
	    -.01882,(float)-.01834,(float)-.01416,(float)-.01573,(float)
	    -.01461,(float)-.009871,(float)-.008193,(float)-.007829,(float)
	    -.004197,(float)-.004043,(float)-9.091e-5,(float).003881,(float)
	    .01022,(float).01696,(float).02102,(float).02184,(float).004136,(
	    float).002725,(float)-.02209,(float)-.02007,(float)-.01973,(float)
	    -.01796,(float)-.01489,(float)-.01481,(float)-.01396,(float)
	    -.01029,(float)-.009074,(float)-.008612,(float)-.005056,(float)
	    -.00411,(float)-9.361e-4,(float).003137,(float).009971,(float)
	    .01574,(float).01899,(float).02211,(float).005316,(float).003713,(
	    float)-.02106,(float)-.01973,(float)-.01992,(float)-.01765,(float)
	    -.01631,(float)-.01421,(float)-.01361,(float)-.011,(float)-.01039,
	    (float)-.008582,(float)-.006425,(float)-.003913,(float)-.002342,(
	    float).001673,(float).01037,(float).01479,(float).01769,(float)
	    .02217,(float).006561,(float).00464,(float)-.02203,(float)-.01893,
	    (float)-.01825,(float)-.01662,(float)-.01659,(float)-.01481,(
	    float)-.01308,(float)-.01255,(float)-.01045,(float)-.007332,(
	    float)-.006812,(float)-.005076,(float)-.002902,(float)9.269e-4,(
	    float).01086,(float).0149,(float).01649,(float).02205,(float)
	    .007916,(float).005533,(float)-.022,(float)-.01882,(float)-.0188,(
	    float)-.01674,(float)-.01658,(float)-.01419,(float)-.01296,(float)
	    -.0128,(float)-.01053,(float)-.007874,(float)-.006876,(float)
	    -.005094,(float)-.002954,(float)9.24e-4,(float).01077,(float)
	    .01502,(float).01642,(float).02163,(float).009371,(float).006384,(
	    float)-.02227,(float)-.01979,(float)-.01896,(float)-.01769,(float)
	    -.01541,(float)-.01492,(float)-.01394,(float)-.0121,(float)
	    -.009603,(float)-.008479,(float)-.006017,(float)-.005268,(float)
	    -.002203,(float).001605,(float).01081,(float).01533,(float).01624,
	    (float).02157,(float).01068,(float).007328,(float)-.02192,(float)
	    -.01974,(float)-.01868,(float)-.0175,(float)-.01529,(float)
	    -.01523,(float)-.01404,(float)-.01134,(float)-.009431,(float)
	    -.008158,(float)-.005717,(float)-.005347,(float)-.001818,(float)
	    .002073,(float).01098,(float).01562,(float).01643,(float).02118,(
	    float).01203,(float).008258,(float)-.02216,(float)-.02,(float)
	    -.01895,(float)-.01776,(float)-.01377,(float)-.01567,(float)
	    -.01414,(float)-.01036,(float)-.008129,(float)-.008214,(float)
	    -.004357,(float)-.005671,(float)-4.951e-4,(float).00327,(float)
	    .01189,(float).01582,(float).01715,(float).0207,(float).01312,(
	    float).009143,(float)-.02116,(float)-.01834,(float)-.01813,(float)
	    -.01617,(float)-.01455,(float)-.01377,(float)-.01242,(float)
	    -.01091,(float)-.00865,(float)-.006986,(float)-.004872,(float)
	    -.004336,(float)-9.19e-4,(float).00303,(float).01298,(float)
	    .01641,(float).01773,(float).02043,(float).01394,(float).01007,(
	    float)-.02043,(float)-.01736,(float)-.01723,(float)-.01504,(float)
	    -.01482,(float)-.01284,(float)-.01119,(float)-.01013,(float)
	    -.008813,(float)-.005735,(float)-.005091,(float)-.003083,(float)
	    -.001151,(float).002866,(float).01421,(float).01715,(float).01847,
	    (float).02035,(float).01455,(float).01101,(float)-.01964,(float)
	    -.01565,(float)-.01625,(float)-.0134,(float)-.01458,(float)-.011,(
	    float)-.009432,(float)-.009804,(float)-.008393,(float)-.00453,(
	    float)-.004568,(float)-.001539,(float)-4.745e-4,(float).003714,(
	    float).01496,(float).01824,(float).01943,(float).02048,(float)
	    .01502,(float).0121,(float)-.01853,(float)-.01667,(float)-.01593,(
	    float)-.01434,(float)-.01217,(float)-.01088,(float)-.01012,(float)
	    -.007304,(float)-.006066,(float)-.004489,(float)-.0022,(float)
	    4.284e-5,(float).001872,(float).006027,(float).01611,(float)
	    .01966,(float).02052,(float).02086,(float).0155,(float).0133,(
	    float)-.01754,(float)-.01492,(float)-.01448,(float)-.01256,(float)
	    -.01104,(float)-.009663,(float)-.008492,(float)-.005993,(float)
	    -.004523,(float)-.002666,(float)-4.823e-4,(float).001385,(float)
	    .003777,(float).008167,(float).01744,(float).02102,(float).02214,(
	    float).02118,(float).01597,(float).01452,(float)-.01771,(float)
	    -.0147,(float)-.01257,(float)-.01194,(float)-.007639,(float)
	    -.009634,(float)-.00797,(float)-.004045,(float)-.001066,(float)
	    -6.568e-4,(float).002991,(float).001904,(float).007191,(float)
	    .01134,(float).01963,(float).0226,(float).02411,(float).02152,(
	    float).01638,(float).01583,(float)-.01472,(float)-.01251,(float)
	    -.01157,(float)-.009849,(float)-.00712,(float)-.006501,(float)
	    -.005433,(float)-.002252,(float)-7.197e-5,(float).001308,(float)
	    .004298,(float).005102,(float).008798,(float).0132,(float).0222,(
	    float).02504,(float).02589,(float).02245,(float).01691,(float)
	    .01733,(float)-.01352,(float)-.01047,(float)-.009595,(float)
	    -.007649,(float)-.005115,(float)-.004269,(float)-.002901,(float)
	    5.06e-5,(float).001953,(float).00363,(float).006428,(float)
	    .008036,(float).01108,(float).01583,(float).02459,(float).02775,(
	    float).0277,(float).0236,(float).0175,(float).01902,(float)
	    -.009567,(float)-.008317,(float)-.009181,(float)-.005715,(float)
	    -.003964,(float)-2.647e-4,(float)4.081e-6,(float).003228,(float)
	    .003462,(float).00545,(float).00834,(float).01239,(float).01324,(
	    float).01807,(float).02753,(float).03076,(float).03023,(float)
	    .02479,(float).01823,(float).02087,(float)-.009255,(float)
	    -.004739,(float)-.004405,(float)-.001671,(float)-9.363e-4,(float)
	    .001572,(float).003413,(float).00512,(float).007174,(float).01033,
	    (float).01206,(float).01458,(float).01705,(float).0218,(float)
	    .03145,(float).03411,(float).03331,(float).02609,(float).01908,(
	    float).02289,(float)-.006918,(float)-.003179,(float)-.002159,(
	    float)2.362e-4,(float).003409,(float).003747,(float).00573,(float)
	    .009411,(float).01161,(float).01366,(float).01669,(float).01793,(
	    float).02177,(float).02645,(float).03648,(float).03785,(float)
	    .03648,(float).02761,(float).02012,(float).02513,(float)-.002846,(
	    float).002167,(float).001466,(float).0055,(float).004963,(float)
	    .009521,(float).01153,(float).01177,(float).01407,(float).01868,(
	    float).01942,(float).02313,(float).02486,(float).02995,(float)
	    .04139,(float).04213,(float).04012,(float).02962,(float).02138,(
	    float).02771,(float)-.001439,(float).005453,(float).00707,(float)
	    .009494,(float).01041,(float).01195,(float).01514,(float).016,(
	    float).01987,(float).02418,(float).02527,(float).02711,(float)
	    .03082,(float).03603,(float).04626,(float).04695,(float).04457,(
	    float).03204,(float).02297,(float).03066,(float).004171,(float)
	    .006826,(float).008585,(float).01093,(float).01626,(float).0161,(
	    float).01751,(float).02377,(float).02597,(float).02711,(float)
	    .03205,(float).03347,(float).038,(float).0434,(float).05233,(
	    float).05213,(float).04944,(float).03453,(float).02479,(float)
	    .03391,(float).008762,(float).01329,(float).01377,(float).01738,(
	    float).02074,(float).02243,(float).02428,(float).02858,(float)
	    .03146,(float).03375,(float).03803,(float).03973,(float).0445,(
	    float).0504,(float).05903,(float).05795,(float).05497,(float)
	    .03741,(float).02682,(float).03753,(float).0109,(float).01912,(
	    float).02235,(float).02426,(float).02805,(float).02677,(float)
	    .03088,(float).03357,(float).03938,(float).04277,(float).04582,(
	    float).04503,(float).05224,(float).05794,(float).06685,(float)
	    .06486,(float).06108,(float).04108,(float).02919,(float).04172,(
	    float).0184,(float).0237,(float).02656,(float).02912,(float)
	    .03467,(float).03418,(float).03701,(float).04242,(float).04644,(
	    float).04937,(float).0534,(float).05426,(float).06014,(float)
	    .06595,(float).07655,(float).07288,(float).06756,(float).04526,(
	    float).03198,(float).04649,(float).02884,(float).0312,(float)
	    .03128,(float).03616,(float).03989,(float).045,(float).04562,(
	    float).05172,(float).05292,(float).05631,(float).0608,(float)
	    .06591,(float).06843,(float).0753,(float).08663,(float).08128,(
	    float).07513,(float).04974,(float).03513,(float).05182,(float)
	    .02866,(float).03354,(float).03672,(float).03928,(float).04751,(
	    float).04617,(float).0483,(float).05684,(float).06022,(float)
	    .06125,(float).06801,(float).06919,(float).07551,(float).08223,(
	    float).09116,(float).08575,(float).07953,(float).05223,(float)
	    .03685,(float).05475,(float).03381,(float).04061,(float).04147,(
	    float).04609,(float).04928,(float).05283,(float).05523,(float)
	    .06051,(float).06336,(float).0671,(float).07166,(float).07529,(
	    float).07977,(float).08715,(float).09689,(float).09042,(float)
	    .08416,(float).055,(float).03872,(float).05789,(float).0396,(
	    float).04866,(float).05297,(float).05536,(float).06204,(float)
	    .06081,(float).06469,(float).06992,(float).07667,(float).0797,(
	    float).08498,(float).08497,(float).09305,(float).1,(float).109,(
	    float).1016,(float).09372,(float).06123,(float).04291,(float)
	    .06486,(float).05157,(float).05869,(float).0606,(float).06555,(
	    float).0721,(float).07332,(float).07658,(float).08332,(float)
	    .0878,(float).09125,(float).09705,(float).09891,(float).1059,(
	    float).1134,(float).1238,(float).1145,(float).104,(float).06833,(
	    float).04782,(float).07282,(float).06634,(float).06984,(float)
	    .07029,(float).07693,(float).08275,(float).08783,(float).08945,(
	    float).09825,(float).1,(float).1041,(float).1104,(float).1154,(
	    float).1202,(float).1289,(float).1401,(float).128,(float).1166,(
	    float).07631,(float).05345,(float).08192,(float).07588,(float)
	    .08732,(float).0862,(float).09465,(float).09633,(float).1031,(
	    float).1072,(float).1103,(float).1155,(float).1221,(float).1264,(
	    float).1306,(float).1368,(float).1456,(float).1576,(float).1429,(
	    float).1312,(float).08562,(float).05995,(float).09229,(float)
	    .08726,(float).1039,(float).1049,(float).1124,(float).1117,(float)
	    .1186,(float).1249,(float).1248,(float).133,(float).1425,(float)
	    .1444,(float).1477,(float).1554,(float).1646,(float).1777,(float)
	    .1605,(float).1468,(float).09653,(float).06758,(float).1043,(
	    float).09928,(float).1181,(float).1238,(float).1284,(float).1331,(
	    float).1329,(float).1413,(float).1436,(float).1551,(float).1631,(
	    float).1667,(float).166,(float).1779,(float).187,(float).2002,(
	    float).1804,(float).1645,(float).109,(float).07645,(float).1182,(
	    float).1166,(float).1346,(float).1412,(float).1458,(float).1533,(
	    float).1523,(float).1601,(float).166,(float).1766,(float).1837,(
	    float).1891,(float).1895,(float).2012,(float).2113,(float).2237,(
	    float).2024,(float).1859,(float).1234,(float).08673,(float).1341,(
	    float).1371,(float).1507,(float).1578,(float).1627,(float).1769,(
	    float).1735,(float).1794,(float).1921,(float).2015,(float).2047,(
	    float).2154,(float).2151,(float).2285,(float).2396,(float).2495,(
	    float).2278,(float).2099,(float).1403,(float).09874,(float).1527,(
	    float).1568,(float).1733,(float).1821,(float).1869,(float).2014,(
	    float).1964,(float).2045,(float).216,(float).2285,(float).2335,(
	    float).2433,(float).2408,(float).2573,(float).2686,(float).2811,(
	    float).2574,(float).2362,(float).1598,(float).1129,(float).1743,(
	    float).182,(float).1985,(float).2055,(float).213,(float).226,(
	    float).2247,(float).2329,(float).2439,(float).2559,(float).2629,(
	    float).2723,(float).2716,(float).2878,(float).3003,(float).3166,(
	    float).2903,(float).2671,(float).1824,(float).1295,(float).1994,(
	    float).2087,(float).2222,(float).2309,(float).238,(float).2561,(
	    float).2529,(float).2599,(float).2774,(float).2885,(float).2932,(
	    float).3065,(float).3057,(float).3235,(float).3377,(float).355,(
	    float).3266,(float).3036,(float).2092,(float).149,(float).2288,(
	    float).2357,(float).2526,(float).2616,(float).2697,(float).2881,(
	    float).2857,(float).2936,(float).3101,(float).3234,(float).3292,(
	    float).343,(float).3426,(float).3616,(float).3771,(float).3976,(
	    float).3693,(float).3446,(float).2404,(float).1722,(float).2633,(
	    float).2673,(float).2853,(float).2932,(float).3039,(float).3213,(
	    float).3218,(float).3306,(float).3485,(float).3596,(float).3681,(
	    float).3811,(float).3843,(float).4016,(float).4195,(float).446,(
	    float).4191,(float).3907,(float).2769,(float).1999,(float).304,(
	    float).297,(float).3221,(float).3324,(float).3427,(float).3611,(
	    float).3602,(float).3712,(float).3871,(float).4029,(float).4129,(
	    float).426,(float).4279,(float).448,(float).4673,(float).5007,(
	    float).475,(float).4449,(float).3203,(float).2329,(float).352,(
	    float).3342,(float).3598,(float).3703,(float).3819,(float).4039,(
	    float).4026,(float).4133,(float).4347,(float).4494,(float).4581,(
	    float).475,(float).478,(float).4997,(float).5221,(float).5613,(
	    float).538,(float).5082,(float).3718,(float).2725,(float).4092,(
	    float).3698,(float).4059,(float).4198,(float).4307,(float).451,(
	    float).4483,(float).4634,(float).4801,(float).5017,(float).5142,(
	    float).5291,(float).5297,(float).5559,(float).5802,(float).6306,(
	    float).6111,(float).5807,(float).4326,(float).3206,(float).4772,(
	    float).418,(float).4596,(float).4665,(float).4853,(float).4938,(
	    float).5066,(float).5224,(float).5322,(float).5508,(float).5726,(
	    float).5819,(float).5929,(float).6121,(float).6406,(float).7101,(
	    float).6965,(float).6637,(float).5048,(float).3785,(float).5583,(
	    float).4708,(float).5129,(float).5173,(float).5405,(float).5458,(
	    float).5661,(float).5817,(float).5914,(float).61,(float).6361,(
	    float).6447,(float).661,(float).6787,(float).7117,(float).7984,(
	    float).7935,(float).7606,(float).5921,(float).4492,(float).6563,(
	    float).5307,(float).5654,(float).5667,(float).5953,(float).6068,(
	    float).6303,(float).6434,(float).6611,(float).6772,(float).7018,(
	    float).7165,(float).7352,(float).7554,(float).7941,(float).8966,(
	    float).9035,(float).8755,(float).697,(float).5363,(float).7741,(
	    float).584,(float).6197,(float).6307,(float).6551,(float).6804,(
	    float).6903,(float).7071,(float).7383,(float).7563,(float).778,(
	    float).7996,(float).8123,(float).8431,(float).8877,(float)1.006,(
	    float)1.032,(float)1.008,(float).8221,(float).643,(float).9167,(
	    float).6446,(float).6905,(float).7018,(float).7291,(float).7523,(
	    float).7684,(float).7865,(float).8166,(float).8359,(float).8627,(
	    float).884,(float).9047,(float).9325,(float).9832,(float)1.133,(
	    float)1.183,(float)1.162,(float).9752,(float).7755,(float)1.091,(
	    float).7125,(float).774,(float).7826,(float).8157,(float).8294,(
	    float).8541,(float).8777,(float).9,(float).9253,(float).9614,(
	    float).9791,(float)1.002,(float)1.034,(float)1.093,(float)1.28,(
	    float)1.358,(float)1.342,(float)1.161,(float).9413,(float)1.303,(
	    float).8015,(float).8514,(float).8565,(float).8973,(float).9173,(
	    float).9506,(float).9707,(float)1.002,(float)1.024,(float)1.063,(
	    float)1.086,(float)1.117,(float)1.149,(float)1.217,(float)1.451,(
	    float)1.558,(float)1.557,(float)1.388,(float)1.149,(float)1.564,(
	    float).876,(float).9416,(float).9551,(float).9948,(float)1.025,(
	    float)1.049,(float)1.076,(float)1.109,(float)1.142,(float)1.183,(
	    float)1.211,(float)1.239,(float)1.282,(float)1.36,(float)1.64,(
	    float)1.794,(float)1.812,(float)1.667,(float)1.411,(float)1.886,(
	    float).9746,(float)1.049,(float)1.059,(float)1.107,(float)1.132,(
	    float)1.168,(float)1.197,(float)1.238,(float)1.267,(float)1.315,(
	    float)1.346,(float)1.384,(float)1.43,(float)1.524,(float)1.857,(
	    float)2.071,(float)2.113,(float)2.011,(float)1.745,(float)2.284,(
	    float)1.09,(float)1.159,(float)1.172,(float)1.224,(float)1.265,(
	    float)1.3,(float)1.329,(float)1.383,(float)1.419,(float)1.466,(
	    float)1.51,(float)1.548,(float)1.606,(float)1.717,(float)2.117,(
	    float)2.403,(float)2.472,(float)2.437,(float)2.174,(float)2.782,(
	    float)1.199,(float)1.294,(float)1.32,(float)1.371,(float)1.425,(
	    float)1.446,(float)1.486,(float)1.546,(float)1.601,(float)1.652,(
	    float)1.705,(float)1.734,(float)1.816,(float)1.942,(float)2.431,(
	    float)2.797,(float)2.904,(float)2.972,(float)2.726,(float)3.402,(
	    float)1.36,(float)1.463,(float)1.479,(float)1.549,(float)1.592,(
	    float)1.639,(float)1.684,(float)1.745,(float)1.801,(float)1.872,(
	    float)1.925,(float)1.97,(float)2.058,(float)2.212,(float)2.808,(
	    float)3.271,(float)3.435,(float)3.639,(float)3.444,(float)4.184,(
	    float)1.526,(float)1.657,(float)1.691,(float)1.76,(float)1.825,(
	    float)1.858,(float)1.917,(float)1.989,(float)2.069,(float)2.149,(
	    float)2.215,(float)2.254,(float)2.372,(float)2.553,(float)3.274,(
	    float)3.859,(float)4.088,(float)4.486,(float)4.385,(float)5.17,(
	    float)1.764,(float)1.917,(float)1.946,(float)2.038,(float)2.104,(
	    float)2.16,(float)2.228,(float)2.31,(float)2.401,(float)2.502,(
	    float)2.579,(float)2.638,(float)2.772,(float)2.991,(float)3.864,(
	    float)4.599,(float)4.909,(float)5.569,(float)5.628,(float)6.419,(
	    float)2.077,(float)2.262,(float)2.3,(float)2.412,(float)2.491,(
	    float)2.567,(float)2.649,(float)2.749,(float)2.858,(float)2.988,(
	    float)3.08,(float)3.163,(float)3.318,(float)3.59,(float)4.649,(
	    float)5.559,(float)5.96,(float)6.969,(float)7.283,(float)8.02,(
	    float)2.534,(float)2.764,(float)2.815,(float)2.954,(float)3.067,(
	    float)3.154,(float)3.257,(float)3.395,(float)3.541,(float)3.698,(
	    float)3.826,(float)3.919,(float)4.13,(float)4.467,(float)5.758,(
	    float)6.851,(float)7.354,(float)8.813,(float)9.514,(float)10.07,(
	    float)3.247,(float)3.565,(float)3.638,(float)3.824,(float)3.976,(
	    float)4.083,(float)4.231,(float)4.409,(float)4.609,(float)4.824,(
	    float)4.986,(float)5.103,(float)5.378,(float)5.8,(float)7.369,(
	    float)8.653,(float)9.292,(float)11.3,(float)12.56,(float)12.73,(
	    float)4.514,(float)4.972,(float)5.059,(float)5.34,(float)5.526,(
	    float)5.714,(float)5.92,(float)6.153,(float)6.42,(float)6.732,(
	    float)6.937,(float)7.12,(float)7.463,(float)8.007,(float)9.871,(
	    float)11.35,(float)12.12,(float)14.77,(float)16.81,(float)16.22,(
	    float)6.968,(float)7.674,(float)7.829,(float)8.243,(float)8.562,(
	    float)8.808,(float)9.113,(float)9.488,(float)9.875,(float)10.3,(
	    float)10.6,(float)10.84,(float)11.31,(float)12.,(float)14.08,(
	    float)15.66,(float)16.51,(float)19.87,(float)22.92,(float)20.93,(
	    float)12.56,(float)13.79,(float)14.06,(float)14.74,(float)15.25,(
	    float)15.61,(float)16.11,(float)16.67,(float)17.26,(float)17.86,(
	    float)18.28,(float)18.55,(float)19.19,(float)19.99,(float)21.85,(
	    float)23.12,(float)23.96,(float)27.9,(float)32.12,(float)27.39,(
	    float)28.29,(float)30.5,(float)30.97,(float)32.12,(float)33.04,(
	    float)33.57,(float)34.27,(float)35.16,(float)35.93,(float)36.58,(
	    float)37.12,(float)37.37,(float)37.93,(float)38.39,(float)37.97,(
	    float)37.6,(float)37.95,(float)41.82,(float)47.1,(float)36.85,(
	    float)89.49,(float)93.05,(float)93.77,(float)95.07,(float)96.14,(
	    float)96.37,(float)96.75,(float)97.14,(float)97.09,(float)96.43,(
	    float)95.98,(float)95.31,(float)93.76,(float)90.63,(float)77.64,(
	    float)70.06,(float)68.35,(float)69.3,(float)74.26,(float)50.85,(
	    float)546.2,(float)522.2,(float)516.1,(float)499.1,(float)484.4,(
	    float)474.1,(float)459.6,(float)441.2,(float)421.,(float)399.6,(
	    float)382.1,(float)370.6,(float)342.5,(float)304.7,(float)205.7,(
	    float)159.3,(float)147.5,(float)130.1,(float)125.1,(float)65.26,(
	    float)5621.,(float)4201.,(float)3938.,(float)3362.,(float)2954.,(
	    float)2722.,(float)2420.,(float)2096.,(float)1798.,(float)1542.,(
	    float)1362.,(float)1262.,(float)1035.,(float)791.2,(float)361.8,(
	    float)235.3,(float)207.6,(float)165.9,(float)149.9,(float)69.8 };

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
/* Dust model */
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
/*<       vi_m=18.2859898 >*/
    vi_m__ = (float)18.2859898;
/*<       vi(1)=vi_m >*/
    sixs_coef__1.vi[0] = vi_m__;
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
} /* dust_ */

#ifdef __cplusplus
	}
#endif
