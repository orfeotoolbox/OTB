/* STM.f -- translated by f2c (version 19970805).
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

/*<       subroutine stm >*/
/* Subroutine */ int stm_()
{
    /* Initialized data */

    static real asy_m__[20] = { (float).787,(float).795,(float).796,(float).8,
	    (float).802,(float).804,(float).806,(float).808,(float).809,(
	    float).81,(float).81,(float).809,(float).807,(float).802,(float)
	    .77,(float).736,(float).723,(float).685,(float).647,(float).469 };
    static real ex_m__[20] = { (float)1.323426,(float)1.367378,(float)
	    1.375664,(float)1.389866,(float)1.397082,(float)1.397282,(float)
	    1.394358,(float)1.384032,(float)1.363314,(float)1.332585,(float)
	    1.301248,(float)1.278673,(float)1.211386,(float)1.101411,(float)
	    .720679,(float).5124845,(float).4508164,(float).3256089,(float)
	    .2397076,(float).06640586 };
    static real sc_m__[20] = { (float)1.323426,(float)1.367378,(float)
	    1.375664,(float)1.389866,(float)1.397082,(float)1.397282,(float)
	    1.394358,(float)1.384032,(float)1.363314,(float)1.332585,(float)
	    1.301248,(float)1.278673,(float)1.211386,(float)1.101411,(float)
	    .720679,(float).5124845,(float).4508164,(float).3256089,(float)
	    .2397076,(float).06640586 };
    static real phr[1660]	/* was [20][83] */ = { (float).2975,(float)
	    .2458,(float).2347,(float).2072,(float).1879,(float).1755,(float)
	    .1638,(float).1489,(float).1415,(float).1337,(float).1284,(float)
	    .1253,(float).1225,(float).12,(float).128,(float).142,(float)
	    .1485,(float).1709,(float).1989,(float).4039,(float).2696,(float)
	    .2255,(float).2165,(float).1924,(float).1756,(float).165,(float)
	    .1551,(float).1422,(float).1359,(float).1293,(float).1248,(float)
	    .1223,(float).1201,(float).1185,(float).1274,(float).1416,(float)
	    .1482,(float).1707,(float).1987,(float).4037,(float).2051,(float)
	    .1731,(float).1684,(float).1526,(float).1427,(float).1366,(float)
	    .1306,(float).1236,(float).1198,(float).1163,(float).114,(float)
	    .113,(float).1128,(float).1132,(float).1252,(float).14,(float)
	    .1468,(float).1696,(float).1978,(float).4031,(float).1882,(float)
	    .1522,(float).1476,(float).1348,(float).1286,(float).1235,(float)
	    .1178,(float).1141,(float).1103,(float).1075,(float).1066,(float)
	    .1062,(float).1067,(float).1081,(float).122,(float).1375,(float)
	    .1445,(float).1677,(float).1962,(float).402,(float).2239,(float)
	    .1759,(float).1687,(float).1519,(float).1445,(float).1363,(float)
	    .1276,(float).1226,(float).1156,(float).1102,(float).1085,(float)
	    .1073,(float).1062,(float).1062,(float).1188,(float).1346,(float)
	    .1417,(float).1653,(float).1942,(float).4004,(float).2685,(float)
	    .2136,(float).2047,(float).1832,(float).1724,(float).1615,(float)
	    .1496,(float).141,(float).1302,(float).1214,(float).1173,(float)
	    .1148,(float).1107,(float).1076,(float).1161,(float).1315,(float)
	    .1387,(float).1626,(float).1917,(float).3985,(float).2867,(float)
	    .2382,(float).2294,(float).2073,(float).1934,(float).1828,(float)
	    .1696,(float).1576,(float).1448,(float).1339,(float).1275,(float)
	    .1239,(float).1169,(float).1106,(float).114,(float).1285,(float)
	    .1356,(float).1596,(float).1889,(float).3961,(float).2805,(float)
	    .2436,(float).2353,(float).2165,(float).2016,(float).193,(float)
	    .1804,(float).167,(float).154,(float).1427,(float).1349,(float)
	    .1308,(float).122,(float).1135,(float).1121,(float).1255,(float)
	    .1325,(float).1564,(float).1859,(float).3934,(float).2628,(float)
	    .2355,(float).2287,(float).2133,(float).2005,(float).1932,(float)
	    .182,(float).1696,(float).1574,(float).1464,(float).1385,(float)
	    .1342,(float).1249,(float).1151,(float).1102,(float).1226,(float)
	    .1294,(float).1532,(float).1828,(float).3903,(float).2453,(float)
	    .222,(float).2174,(float).2042,(float).1948,(float).1873,(float)
	    .1777,(float).1674,(float).1565,(float).1459,(float).1387,(float)
	    .1344,(float).1252,(float).1151,(float).1082,(float).1197,(float)
	    .1264,(float).15,(float).1796,(float).387,(float).2311,(float)
	    .2096,(float).2058,(float).1942,(float).1868,(float).1796,(float)
	    .1711,(float).1624,(float).1524,(float).1427,(float).1362,(float)
	    .1322,(float).1236,(float).1137,(float).1059,(float).1168,(float)
	    .1234,(float).1468,(float).1764,(float).3834,(float).2201,(float)
	    .2001,(float).1961,(float).1853,(float).1778,(float).1718,(float)
	    .1641,(float).1559,(float).1467,(float).138,(float).132,(float)
	    .1284,(float).1204,(float).1112,(float).1034,(float).114,(float)
	    .1204,(float).1437,(float).1732,(float).3797,(float).2119,(float)
	    .1918,(float).1878,(float).1771,(float).1693,(float).1638,(float)
	    .1567,(float).1489,(float).1404,(float).1325,(float).127,(float)
	    .1238,(float).1164,(float).108,(float).1008,(float).1111,(float)
	    .1175,(float).1406,(float).1701,(float).3758,(float).2031,(float)
	    .1835,(float).1798,(float).169,(float).1614,(float).1558,(float)
	    .1491,(float).1417,(float).1341,(float).1267,(float).1216,(float)
	    .1187,(float).1121,(float).1044,(float).09812,(float).1084,(float)
	    .1147,(float).1377,(float).1671,(float).3718,(float).1933,(float)
	    .1749,(float).1713,(float).1608,(float).1539,(float).148,(float)
	    .1416,(float).1347,(float).1279,(float).1208,(float).1162,(float)
	    .1133,(float).1076,(float).1006,(float).09541,(float).1057,(float)
	    .112,(float).1349,(float).1643,(float).3678,(float).1849,(float)
	    .1656,(float).1619,(float).1523,(float).1462,(float).1402,(float)
	    .1342,(float).1278,(float).1215,(float).115,(float).1109,(float)
	    .1082,(float).1029,(float).09679,(float).09278,(float).1032,(
	    float).1095,(float).1323,(float).1616,(float).3638,(float).1764,(
	    float).1561,(float).1522,(float).1436,(float).1377,(float).133,(
	    float).1271,(float).1213,(float).115,(float).1093,(float).1057,(
	    float).1035,(float).09837,(float).09304,(float).09029,(float)
	    .1009,(float).1072,(float).13,(float).1592,(float).3598,(float)
	    .1653,(float).1466,(float).1431,(float).135,(float).1296,(float)
	    .1258,(float).1203,(float).1149,(float).109,(float).1039,(float)
	    .1007,(float).0988,(float).09419,(float).08947,(float).08797,(
	    float).09875,(float).1051,(float).1278,(float).1569,(float).3559,(
	    float).152,(float).138,(float).1349,(float).1273,(float).1221,(
	    float).1183,(float).1139,(float).1088,(float).1037,(float).09893,(
	    float).09599,(float).09428,(float).09039,(float).08618,(float)
	    .08585,(float).09687,(float).1033,(float).126,(float).155,(float)
	    .3522,(float).1401,(float).1298,(float).1269,(float).1204,(float)
	    .1149,(float).1115,(float).1078,(float).103,(float).09871,(float)
	    .09453,(float).09168,(float).0901,(float).08685,(float).08326,(
	    float).08397,(float).09528,(float).1018,(float).1244,(float).1534,
	    (float).3487,(float).1305,(float).1211,(float).1188,(float).1131,(
	    float).1084,(float).1055,(float).102,(float).09774,(float).0941,(
	    float).0905,(float).08792,(float).08647,(float).08359,(float)
	    .0807,(float).08238,(float).09398,(float).1005,(float).1231,(
	    float).152,(float).3454,(float).1216,(float).1129,(float).1111,(
	    float).1063,(float).1026,(float).1002,(float).09688,(float).0932,(
	    float).0899,(float).08683,(float).08467,(float).08337,(float)
	    .08068,(float).07847,(float).0811,(float).09298,(float).09963,(
	    float).1222,(float).1511,(float).3425,(float).1134,(float).1061,(
	    float).1045,(float).1006,(float).0975,(float).09529,(float).09251,
	    (float).08922,(float).08621,(float).08361,(float).08184,(float)
	    .08071,(float).07822,(float).07653,(float).08013,(float).09231,(
	    float).09906,(float).1217,(float).1506,(float).34,(float).1062,(
	    float).1004,(float).0992,(float).09577,(float).09308,(float)
	    .09099,(float).08873,(float).08576,(float).08313,(float).08086,(
	    float).07938,(float).07843,(float).07626,(float).0749,(float)
	    .07948,(float).09197,(float).09884,(float).1215,(float).1504,(
	    float).3379,(float).1001,(float).09533,(float).09465,(float)
	    .09133,(float).08933,(float).08735,(float).08542,(float).08295,(
	    float).08063,(float).07857,(float).07732,(float).07649,(float)
	    .07474,(float).07362,(float).07912,(float).09199,(float).09899,(
	    float).1218,(float).1508,(float).3364,(float).09488,(float).09133,
	    (float).09069,(float).08774,(float).08627,(float).08439,(float)
	    .0827,(float).08069,(float).07866,(float).07673,(float).07569,(
	    float).07493,(float).07361,(float).07273,(float).07908,(float)
	    .0924,(float).09953,(float).1226,(float).1516,(float).3355,(float)
	    .0912,(float).08829,(float).08751,(float).08502,(float).08377,(
	    float).08211,(float).08061,(float).07886,(float).07713,(float)
	    .07539,(float).07448,(float).0738,(float).0728,(float).07225,(
	    float).07939,(float).09323,(float).1005,(float).1238,(float).153,(
	    float).3352,(float).0889,(float).08568,(float).08517,(float)
	    .08293,(float).08174,(float).08043,(float).079,(float).07756,(
	    float).07601,(float).07453,(float).07368,(float).07311,(float)
	    .07231,(float).07216,(float).08008,(float).09449,(float).1019,(
	    float).1256,(float).1549,(float).3358,(float).08686,(float).0837,(
	    float).08347,(float).08154,(float).08043,(float).07921,(float)
	    .07789,(float).07675,(float).07535,(float).07411,(float).07333,(
	    float).07282,(float).0722,(float).07243,(float).08118,(float)
	    .09622,(float).1038,(float).1279,(float).1576,(float).3372,(float)
	    .08502,(float).08269,(float).08239,(float).08091,(float).07987,(
	    float).07857,(float).07737,(float).07632,(float).07517,(float)
	    .07409,(float).07341,(float).07295,(float).07251,(float).07305,(
	    float).08271,(float).09843,(float).1062,(float).1309,(float).1609,
	    (float).3396,(float).08471,(float).08238,(float).08178,(float)
	    .08072,(float).07963,(float).07864,(float).07742,(float).07636,(
	    float).07538,(float).07445,(float).07386,(float).07353,(float)
	    .07322,(float).07399,(float).08468,(float).1011,(float).1092,(
	    float).1345,(float).165,(float).343,(float).08559,(float).08235,(
	    float).08146,(float).08075,(float).07973,(float).07928,(float)
	    .07789,(float).07698,(float).07595,(float).07518,(float).07469,(
	    float).07455,(float).07434,(float).0753,(float).0871,(float).1044,
	    (float).1129,(float).1389,(float).1699,(float).3476,(float).08624,
	    (float).08286,(float).08207,(float).08128,(float).08058,(float)
	    .08015,(float).07881,(float).07807,(float).07705,(float).07637,(
	    float).076,(float).07594,(float).07591,(float).07704,(float).09,(
	    float).1083,(float).1171,(float).1441,(float).1758,(float).3536,(
	    float).0863,(float).08428,(float).08392,(float).08266,(float)
	    .08203,(float).08126,(float).08032,(float).07955,(float).07869,(
	    float).07808,(float).07779,(float).07772,(float).07795,(float)
	    .07925,(float).09343,(float).1129,(float).1221,(float).1502,(
	    float).1828,(float).361,(float).0877,(float).0865,(float).08605,(
	    float).08473,(float).08377,(float).08312,(float).08242,(float)
	    .08152,(float).08075,(float).08021,(float).08003,(float).08,(
	    float).08044,(float).08194,(float).09745,(float).1182,(float)
	    .1279,(float).1573,(float).1909,(float).37,(float).09053,(float)
	    .08908,(float).08827,(float).08716,(float).08592,(float).08568,(
	    float).08499,(float).08411,(float).08326,(float).08279,(float)
	    .08273,(float).08283,(float).08339,(float).08516,(float).1022,(
	    float).1245,(float).1347,(float).1656,(float).2003,(float).3808,(
	    float).0936,(float).09184,(float).09132,(float).09004,(float)
	    .0889,(float).08859,(float).08796,(float).08729,(float).08638,(
	    float).08594,(float).08597,(float).0862,(float).08688,(float)
	    .08896,(float).1076,(float).1316,(float).1425,(float).175,(float)
	    .2111,(float).3934,(float).09716,(float).09539,(float).09505,(
	    float).09369,(float).09265,(float).09215,(float).09154,(float)
	    .091,(float).09012,(float).08972,(float).08983,(float).09014,(
	    float).09097,(float).0934,(float).114,(float).1399,(float).1515,(
	    float).1859,(float).2235,(float).4082,(float).1019,(float).0999,(
	    float).09934,(float).09813,(float).09692,(float).0966,(float)
	    .09587,(float).0953,(float).0945,(float).09418,(float).09435,(
	    float).09471,(float).09573,(float).09855,(float).1213,(float)
	    .1493,(float).1618,(float).1983,(float).2377,(float).4253,(float)
	    .1071,(float).1049,(float).1045,(float).1032,(float).1021,(float)
	    .1016,(float).1008,(float).1003,(float).09966,(float).09941,(
	    float).09965,(float).09998,(float).1012,(float).1045,(float).1297,
	    (float).1601,(float).1736,(float).2125,(float).2539,(float).4449,(
	    float).1127,(float).1105,(float).1105,(float).1089,(float).1086,(
	    float).1073,(float).1066,(float).1062,(float).1057,(float).1055,(
	    float).1058,(float).1061,(float).1076,(float).1113,(float).1393,(
	    float).1725,(float).1871,(float).2286,(float).2724,(float).4672,(
	    float).1161,(float).1138,(float).1138,(float).1122,(float).112,(
	    float).1107,(float).1098,(float).1094,(float).1091,(float).1088,(
	    float).1092,(float).1095,(float).1111,(float).1151,(float).1447,(
	    float).1793,(float).1945,(float).2375,(float).2826,(float).4795,(
	    float).1202,(float).1174,(float).1174,(float).1158,(float).1155,(
	    float).1143,(float).1134,(float).1128,(float).1126,(float).1124,(
	    float).1128,(float).1131,(float).1149,(float).1192,(float).1504,(
	    float).1866,(float).2025,(float).247,(float).2934,(float).4925,(
	    float).1294,(float).126,(float).1255,(float).124,(float).1233,(
	    float).1226,(float).1214,(float).1205,(float).1203,(float).1205,(
	    float).1208,(float).1213,(float).1232,(float).1282,(float).1631,(
	    float).2028,(float).2201,(float).2679,(float).3172,(float).5209,(
	    float).1389,(float).1356,(float).1348,(float).1334,(float).1325,(
	    float).1318,(float).1305,(float).1295,(float).1293,(float).1297,(
	    float).1301,(float).1306,(float).1328,(float).1386,(float).1776,(
	    float).2213,(float).2402,(float).2917,(float).3441,(float).5529,(
	    float).1504,(float).1461,(float).1454,(float).1439,(float).1431,(
	    float).1421,(float).1408,(float).1401,(float).1398,(float).1401,(
	    float).1407,(float).1413,(float).1439,(float).1505,(float).1942,(
	    float).2424,(float).2631,(float).3187,(float).3745,(float).5886,(
	    float).1641,(float).1585,(float).1579,(float).156,(float).1552,(
	    float).1542,(float).1527,(float).1522,(float).1518,(float).1521,(
	    float).1529,(float).1536,(float).1566,(float).1642,(float).2133,(
	    float).2666,(float).2892,(float).3495,(float).4089,(float).6283,(
	    float).1786,(float).1734,(float).1725,(float).1704,(float).1693,(
	    float).1679,(float).1665,(float).166,(float).1657,(float).1659,(
	    float).167,(float).1678,(float).1714,(float).18,(float).2352,(
	    float).2944,(float).319,(float).3844,(float).4478,(float).6724,(
	    float).1954,(float).1899,(float).1893,(float).1866,(float).1854,(
	    float).1836,(float).1824,(float).182,(float).1816,(float).1818,(
	    float).1832,(float).1842,(float).1884,(float).1981,(float).2605,(
	    float).3262,(float).3531,(float).4241,(float).4916,(float).7211,(
	    float).2161,(float).2092,(float).2083,(float).2052,(float).2036,(
	    float).2022,(float).2009,(float).2005,(float).1998,(float).2002,(
	    float).2018,(float).2032,(float).208,(float).2191,(float).2898,(
	    float).3627,(float).3922,(float).4692,(float).541,(float).7748,(
	    float).2396,(float).2325,(float).2303,(float).2273,(float).2246,(
	    float).224,(float).2225,(float).2215,(float).2207,(float).2215,(
	    float).2233,(float).2251,(float).2308,(float).2435,(float).3237,(
	    float).4046,(float).4369,(float).5204,(float).5967,(float).8339,(
	    float).2653,(float).2584,(float).256,(float).2523,(float).2495,(
	    float).2486,(float).2471,(float).2459,(float).2453,(float).2463,(
	    float).2485,(float).2506,(float).2573,(float).2718,(float).363,(
	    float).4529,(float).4882,(float).5785,(float).6595,(float).8986,(
	    float).2963,(float).2868,(float).2851,(float).2805,(float).2789,(
	    float).2769,(float).2752,(float).2745,(float).2739,(float).275,(
	    float).2779,(float).2802,(float).2882,(float).305,(float).4088,(
	    float).5084,(float).5471,(float).6445,(float).7301,(float).9693,(
	    float).3324,(float).3206,(float).3183,(float).314,(float).3123,(
	    float).3103,(float).3081,(float).3075,(float).307,(float).3086,(
	    float).3121,(float).3148,(float).3243,(float).3439,(float).4621,(
	    float).5725,(float).6147,(float).7194,(float).8095,(float)1.046,(
	    float).3735,(float).3611,(float).3579,(float).3537,(float).3508,(
	    float).3492,(float).3467,(float).3457,(float).3457,(float).3481,(
	    float).3521,(float).3554,(float).3665,(float).3898,(float).5243,(
	    float).6464,(float).6923,(float).8044,(float).8987,(float)1.13,(
	    float).4204,(float).407,(float).4044,(float).3995,(float).3963,(
	    float).3941,(float).3916,(float).3904,(float).3913,(float).3945,(
	    float).3993,(float).4031,(float).4163,(float).4439,(float).5969,(
	    float).7316,(float).7814,(float).9008,(float).9988,(float)1.221,(
	    float).475,(float).4599,(float).4576,(float).4524,(float).4492,(
	    float).4465,(float).4438,(float).4433,(float).4449,(float).4491,(
	    float).4548,(float).4594,(float).4752,(float).508,(float).6818,(
	    float).8301,(float).8837,(float)1.01,(float)1.111,(float)1.319,(
	    float).5394,(float).5231,(float).5197,(float).5147,(float).5101,(
	    float).5083,(float).5057,(float).5058,(float).508,(float).5136,(
	    float).5204,(float).5261,(float).5453,(float).584,(float).7814,(
	    float).9437,(float)1.001,(float)1.134,(float)1.237,(float)1.424,(
	    float).6129,(float).5974,(float).5936,(float).5876,(float).5824,(
	    float).5808,(float).5788,(float).5794,(float).5831,(float).5902,(
	    float).5986,(float).6054,(float).6291,(float).6745,(float).8982,(
	    float)1.075,(float)1.136,(float)1.274,(float)1.377,(float)1.537,(
	    float).6964,(float).683,(float).6804,(float).6726,(float).6689,(
	    float).6659,(float).6651,(float).667,(float).6728,(float).6816,(
	    float).6923,(float).7005,(float).7296,(float).7827,(float)1.036,(
	    float)1.227,(float)1.291,(float)1.433,(float)1.534,(float)1.659,(
	    float).7964,(float).7828,(float).7808,(float).7726,(float).7705,(
	    float).7676,(float).7677,(float).7718,(float).7794,(float).7908,(
	    float).8047,(float).8151,(float).8502,(float).9124,(float)1.197,(
	    float)1.402,(float)1.469,(float)1.611,(float)1.708,(float)1.788,(
	    float).9181,(float).9016,(float).8981,(float).892,(float).89,(
	    float).8897,(float).8912,(float).8971,(float).9069,(float).9226,(
	    float).9404,(float).9538,(float).9954,(float)1.069,(float)1.388,(
	    float)1.604,(float)1.672,(float)1.813,(float)1.902,(float)1.925,(
	    float)1.057,(float)1.043,(float)1.039,(float)1.035,(float)1.034,(
	    float)1.036,(float)1.04,(float)1.047,(float)1.061,(float)1.083,(
	    float)1.105,(float)1.122,(float)1.171,(float)1.258,(float)1.612,(
	    float)1.837,(float)1.905,(float)2.039,(float)2.117,(float)2.069,(
	    float)1.217,(float)1.209,(float)1.207,(float)1.206,(float)1.207,(
	    float)1.211,(float)1.217,(float)1.229,(float)1.249,(float)1.278,(
	    float)1.306,(float)1.326,(float)1.385,(float)1.487,(float)1.875,(
	    float)2.105,(float)2.171,(float)2.293,(float)2.354,(float)2.222,(
	    float)1.407,(float)1.404,(float)1.407,(float)1.409,(float)1.417,(
	    float)1.421,(float)1.432,(float)1.451,(float)1.479,(float)1.516,(
	    float)1.551,(float)1.574,(float)1.646,(float)1.764,(float)2.185,(
	    float)2.412,(float)2.474,(float)2.577,(float)2.615,(float)2.382,(
	    float)1.633,(float)1.639,(float)1.645,(float)1.655,(float)1.669,(
	    float)1.677,(float)1.694,(float)1.722,(float)1.761,(float)1.807,(
	    float)1.851,(float)1.88,(float)1.965,(float)2.101,(float)2.549,(
	    float)2.765,(float)2.818,(float)2.894,(float)2.901,(float)2.548,(
	    float)1.9,(float)1.927,(float)1.934,(float)1.955,(float)1.975,(
	    float)1.991,(float)2.017,(float)2.056,(float)2.108,(float)2.167,(
	    float)2.22,(float)2.255,(float)2.357,(float)2.511,(float)2.974,(
	    float)3.167,(float)3.207,(float)3.244,(float)3.212,(float)2.721,(
	    float)2.216,(float)2.274,(float)2.287,(float)2.322,(float)2.351,(
	    float)2.376,(float)2.416,(float)2.47,(float)2.537,(float)2.611,(
	    float)2.676,(float)2.72,(float)2.838,(float)3.008,(float)3.472,(
	    float)3.624,(float)3.645,(float)3.631,(float)3.55,(float)2.898,(
	    float)2.599,(float)2.692,(float)2.716,(float)2.769,(float)2.817,(
	    float)2.854,(float)2.911,(float)2.984,(float)3.07,(float)3.163,(
	    float)3.242,(float)3.294,(float)3.428,(float)3.613,(float)4.05,(
	    float)4.14,(float)4.136,(float)4.054,(float)3.914,(float)3.079,(
	    float)3.062,(float)3.206,(float)3.242,(float)3.324,(float)3.4,(
	    float)3.452,(float)3.53,(float)3.627,(float)3.736,(float)3.85,(
	    float)3.945,(float)4.004,(float)4.153,(float)4.345,(float)4.717,(
	    float)4.72,(float)4.681,(float)4.515,(float)4.303,(float)3.263,(
	    float)3.634,(float)3.85,(float)3.899,(float)4.025,(float)4.133,(
	    float)4.205,(float)4.308,(float)4.433,(float)4.569,(float)4.707,(
	    float)4.815,(float)4.881,(float)5.039,(float)5.229,(float)5.484,(
	    float)5.365,(float)5.282,(float)5.012,(float)4.714,(float)3.447,(
	    float)4.352,(float)4.669,(float)4.737,(float)4.921,(float)5.062,(
	    float)5.16,(float)5.293,(float)5.448,(float)5.615,(float)5.776,(
	    float)5.892,(float)5.962,(float)6.121,(float)6.29,(float)6.355,(
	    float)6.076,(float)5.937,(float)5.542,(float)5.146,(float)3.631,(
	    float)5.27,(float)5.724,(float)5.823,(float)6.07,(float)6.251,(
	    float)6.379,(float)6.547,(float)6.735,(float)6.928,(float)7.103,(
	    float)7.221,(float)7.29,(float)7.433,(float)7.55,(float)7.333,(
	    float)6.85,(float)6.644,(float)6.1,(float)5.591,(float)3.81,(
	    float)6.481,(float)7.113,(float)7.245,(float)7.562,(float)7.793,(
	    float)7.949,(float)8.151,(float)8.367,(float)8.573,(float)8.746,(
	    float)8.853,(float)8.91,(float)9.008,(float)9.033,(float)8.417,(
	    float)7.679,(float)7.393,(float)6.678,(float)6.045,(float)3.984,(
	    float)8.163,(float)8.98,(float)9.145,(float)9.53,(float)9.812,(
	    float)9.985,(float)10.21,(float)10.43,(float)10.62,(float)10.77,(
	    float)10.84,(float)10.87,(float)10.88,(float)10.75,(float)9.594,(
	    float)8.551,(float)8.172,(float)7.267,(float)6.499,(float)4.15,(
	    float)10.59,(float)11.55,(float)11.73,(float)12.16,(float)12.46,(
	    float)12.64,(float)12.84,(float)13.03,(float)13.16,(float)13.22,(
	    float)13.22,(float)13.19,(float)13.05,(float)12.7,(float)10.84,(
	    float)9.446,(float)8.964,(float)7.852,(float)6.942,(float)4.304,(
	    float)14.17,(float)15.15,(float)15.32,(float)15.71,(float)15.94,(
	    float)16.07,(float)16.19,(float)16.26,(float)16.24,(float)16.15,(
	    float)16.01,(float)15.9,(float)15.52,(float)14.85,(float)12.13,(
	    float)10.34,(float)9.745,(float)8.416,(float)7.364,(float)4.444,(
	    float)19.56,(float)20.22,(float)20.29,(float)20.46,(float)20.46,(
	    float)20.47,(float)20.38,(float)20.19,(float)19.9,(float)19.54,(
	    float)19.18,(float)18.94,(float)18.24,(float)17.15,(float)13.41,(
	    float)11.19,(float)10.49,(float)8.942,(float)7.749,(float)4.567,(
	    float)27.63,(float)27.22,(float)27.05,(float)26.63,(float)26.18,(
	    float)25.9,(float)25.44,(float)24.8,(float)24.07,(float)23.3,(
	    float)22.63,(float)22.21,(float)21.07,(float)19.45,(float)14.6,(
	    float)11.97,(float)11.15,(float)9.406,(float)8.086,(float)4.671,(
	    float)39.22,(float)36.37,(float)35.76,(float)34.19,(float)32.98,(
	    float)32.2,(float)31.12,(float)29.84,(float)28.48,(float)27.15,(
	    float)26.09,(float)25.44,(float)23.79,(float)21.6,(float)15.63,(
	    float)12.62,(float)11.71,(float)9.788,(float)8.359,(float)4.753,(
	    float)54.07,(float)46.93,(float)45.59,(float)42.35,(float)40.09,(
	    float)38.6,(float)36.73,(float)34.64,(float)32.56,(float)30.59,(
	    float)29.13,(float)28.24,(float)26.09,(float)23.35,(float)16.42,(
	    float)13.11,(float)12.13,(float)10.07,(float)8.557,(float)4.811,(
	    float)68.07,(float)55.89,(float)53.77,(float)48.85,(float)45.6,(
	    float)43.45,(float)40.85,(float)38.07,(float)35.4,(float)32.92,(
	    float)31.15,(float)30.07,(float)27.56,(float)24.44,(float)16.89,(
	    float)13.4,(float)12.37,(float)10.23,(float)8.671,(float)4.844,(
	    float)72.5,(float)58.54,(float)56.16,(float)50.7,(float)47.14,(
	    float)44.78,(float)41.96,(float)38.99,(float)36.14,(float)33.52,(
	    float)31.67,(float)30.54,(float)27.93,(float)24.71,(float)17.,(
	    float)13.46,(float)12.42,(float)10.27,(float)8.698,(float)4.852 };
    static real qhr[1660]	/* was [20][83] */ = { (float)-5.308e-22,(
	    float)6.787e-22,(float)5.459e-22,(float)-2.16e-22,(float)
	    -2.354e-22,(float)-1.055e-22,(float)1.958e-23,(float)-1.308e-22,(
	    float)-8.726e-24,(float)-4.912e-23,(float)9.602e-23,(float)
	    2.191e-23,(float)-1.487e-22,(float)-1.464e-22,(float)-3.694e-22,(
	    float)-3.902e-22,(float)-5.161e-23,(float)1.407e-22,(float)
	    2.306e-22,(float)-1.974e-22,(float)-.003847,(float)-.001662,(
	    float)-8.407e-4,(float)-5.478e-4,(float)7.911e-5,(float)-4.079e-4,
	    (float)4.175e-5,(float)5.053e-4,(float)6.411e-4,(float)6.492e-4,(
	    float)7.231e-4,(float)6.181e-4,(float)7.015e-4,(float)6.29e-4,(
	    float)3.515e-4,(float)2.073e-4,(float)1.652e-4,(float)7.84e-5,(
	    float)1.529e-5,(float)-1.772e-4,(float)-.007671,(float)-.003169,(
	    float)-4.804e-4,(float)-3.328e-4,(float).001869,(float)2.357e-4,(
	    float).001454,(float).002848,(float).003394,(float).003305,(float)
	    .003548,(float).00319,(float).003409,(float).003055,(float).00174,
	    (float).001033,(float)8.25e-4,(float)3.879e-4,(float)6.618e-5,(
	    float)-9.349e-4,(float)-.001357,(float)9.185e-4,(float).003452,(
	    float).002842,(float).005575,(float).004274,(float).004916,(float)
	    .006212,(float).007113,(float).006903,(float).007227,(float)
	    .006894,(float).007016,(float).0064,(float).003816,(float).002299,
	    (float).001839,(float)8.488e-4,(float)1.025e-4,(float)-.002303,(
	    float).004828,(float).003963,(float).003932,(float).00443,(float)
	    .005783,(float).006725,(float).006743,(float).007468,(float)
	    .008528,(float).008964,(float).009379,(float).009547,(float)
	    .009635,(float).009252,(float).006001,(float).003696,(float)
	    .002962,(float).001322,(float)4.286e-5,(float)-.004288,(float)
	    -.00222,(float)-.003095,(float)-.00457,(float)-.001803,(float)
	    -.001751,(float).001356,(float).002535,(float).003675,(float)
	    .00534,(float).007371,(float).008188,(float).009106,(float)
	    .009834,(float).01043,(float).007727,(float).004902,(float)
	    .003931,(float).001656,(float)-2.063e-4,(float)-.006899,(float)
	    -.02526,(float)-.02183,(float)-.0215,(float)-.01715,(float)
	    -.01525,(float)-.01153,(float)-.008462,(float)-.005366,(float)
	    -.00199,(float).001685,(float).003559,(float).005059,(float)
	    .007324,(float).009451,(float).008609,(float).005663,(float)
	    .00453,(float).001711,(float)-7.331e-4,(float)-.01014,(float)
	    -.05228,(float)-.04467,(float)-.04196,(float)-.03619,(float)
	    -.03073,(float)-.0271,(float)-.02263,(float)-.01737,(float)
	    -.01168,(float)-.006733,(float)-.003404,(float)-.001485,(float)
	    .002669,(float).006562,(float).008516,(float).00583,(float)
	    .004625,(float).001388,(float)-.001609,(float)-.01403,(float)
	    -.07486,(float)-.06291,(float)-.06035,(float)-.05231,(float)
	    -.04557,(float)-.04117,(float)-.03546,(float)-.02903,(float)
	    -.0219,(float)-.01568,(float)-.01127,(float)-.008772,(float)
	    -.003157,(float).002449,(float).007521,(float).005367,(float)
	    .004163,(float)6.31e-4,(float)-.00288,(float)-.01856,(float)
	    -.0878,(float)-.07432,(float)-.07299,(float)-.06302,(float)
	    -.05729,(float)-.05124,(float)-.04487,(float)-.03802,(float)
	    -.0307,(float)-.02337,(float)-.01855,(float)-.01539,(float)
	    -.009005,(float)-.002067,(float).005816,(float).004316,(float)
	    .003161,(float)-5.766e-4,(float)-.00457,(float)-.02375,(float)
	    -.09331,(float)-.07984,(float)-.07823,(float)-.06842,(float)
	    -.06318,(float)-.05667,(float)-.05053,(float)-.04379,(float)
	    -.03643,(float)-.02887,(float)-.02402,(float)-.02067,(float)
	    -.01392,(float)-.006299,(float).003628,(float).00277,(float)
	    .001677,(float)-.00222,(float)-.006683,(float)-.02957,(float)
	    -.09431,(float)-.08006,(float)-.07741,(float)-.06906,(float)
	    -.06339,(float)-.05834,(float)-.05253,(float)-.04637,(float)
	    -.03897,(float)-.03203,(float)-.02736,(float)-.02437,(float)
	    -.01754,(float)-.00987,(float).001173,(float)8.345e-4,(float)
	    -2.09e-4,(float)-.004263,(float)-.009205,(float)-.03605,(float)
	    -.08953,(float)-.07549,(float)-.07284,(float)-.06578,(float)
	    -.06055,(float)-.05661,(float)-.05135,(float)-.04593,(float)
	    -.03928,(float)-.03319,(float)-.02894,(float)-.02639,(float)
	    -.01995,(float)-.01269,(float)-.001373,(float)-.001384,(float)
	    -.002412,(float)-.006662,(float)-.01212,(float)-.04316,(float)
	    -.07919,(float)-.06806,(float)-.06617,(float)-.06007,(float)
	    -.05588,(float)-.05225,(float)-.04799,(float)-.04323,(float)
	    -.03808,(float)-.03286,(float)-.02921,(float)-.0269,(float)
	    -.02134,(float)-.0148,(float)-.003888,(float)-.003793,(float)
	    -.004854,(float)-.009368,(float)-.01538,(float)-.05089,(float)
	    -.06584,(float)-.05945,(float)-.05821,(float)-.05313,(float)
	    -.0499,(float)-.04638,(float)-.04344,(float)-.03939,(float)
	    -.03571,(float)-.03147,(float)-.02851,(float)-.02638,(float)
	    -.02185,(float)-.01624,(float)-.006304,(float)-.006318,(float)
	    -.007469,(float)-.01233,(float)-.01898,(float)-.05925,(float)
	    -.05399,(float)-.04954,(float)-.04899,(float)-.04509,(float)
	    -.04325,(float)-.03989,(float)-.03806,(float)-.03532,(float)
	    -.03252,(float)-.02924,(float)-.02708,(float)-.02531,(float)
	    -.02166,(float)-.0171,(float)-.008584,(float)-.008901,(float)
	    -.0102,(float)-.01552,(float)-.02288,(float)-.0682,(float)-.04485,
	    (float)-.03957,(float)-.03932,(float)-.03694,(float)-.03617,(
	    float)-.03397,(float)-.03257,(float)-.03117,(float)-.02891,(float)
	    -.02652,(float)-.02515,(float)-.02393,(float)-.02101,(float)
	    -.01748,(float)-.0107,(float)-.0115,(float)-.01301,(float)-.01889,
	    (float)-.02706,(float)-.07773,(float)-.03392,(float)-.03153,(
	    float)-.03126,(float)-.03008,(float)-.02967,(float)-.02861,(float)
	    -.02774,(float)-.02684,(float)-.02539,(float)-.02388,(float)
	    -.02303,(float)-.02224,(float)-.0201,(float)-.01758,(float)
	    -.01265,(float)-.01409,(float)-.01587,(float)-.02241,(float)
	    -.03148,(float)-.08783,(float)-.02352,(float)-.02509,(float)
	    -.02512,(float)-.02451,(float)-.0241,(float)-.02347,(float)
	    -.02352,(float)-.02267,(float)-.02219,(float)-.0215,(float)
	    -.02094,(float)-.02042,(float)-.01907,(float)-.01751,(float)
	    -.01444,(float)-.01667,(float)-.01875,(float)-.02606,(float)
	    -.03613,(float)-.09849,(float)-.01671,(float)-.01935,(float)
	    -.01961,(float)-.01964,(float)-.01923,(float)-.01903,(float)
	    -.01961,(float)-.01905,(float)-.01929,(float)-.01926,(float)-.019,
	    (float)-.0187,(float)-.01799,(float)-.01733,(float)-.01609,(float)
	    -.01921,(float)-.02165,(float)-.02983,(float)-.04099,(float)
	    -.1097,(float)-.01125,(float)-.01391,(float)-.01464,(float)
	    -.01505,(float)-.01552,(float)-.01524,(float)-.016,(float)-.01609,
	    (float)-.0168,(float)-.01712,(float)-.01729,(float)-.01712,(float)
	    -.01694,(float)-.01705,(float)-.01762,(float)-.02172,(float)
	    -.02455,(float)-.0337,(float)-.04606,(float)-.1214,(float)
	    -.006411,(float)-.00939,(float)-.01062,(float)-.01111,(float)
	    -.01253,(float)-.01208,(float)-.01298,(float)-.01366,(float)
	    -.0146,(float)-.01519,(float)-.01575,(float)-.01569,(float)
	    -.01593,(float)-.0167,(float)-.01906,(float)-.0242,(float)-.02745,
	    (float)-.03767,(float)-.0513,(float)-.1335,(float)-.003637,(float)
	    -.006723,(float)-.007453,(float)-.008535,(float)-.009657,(float)
	    -.009792,(float)-.01077,(float)-.01151,(float)-.01258,(float)
	    -.01354,(float)-.01428,(float)-.01445,(float)-.01499,(float)
	    -.01632,(float)-.02041,(float)-.02666,(float)-.03036,(float)
	    -.04174,(float)-.05673,(float)-.1462,(float)-.002257,(float)
	    -.004789,(float)-.005056,(float)-.006538,(float)-.007314,(float)
	    -.007959,(float)-.008974,(float)-.009679,(float)-.01088,(float)
	    -.01214,(float)-.01298,(float)-.01335,(float)-.01418,(float)
	    -.01596,(float)-.02168,(float)-.0291,(float)-.03327,(float)
	    -.04592,(float)-.06234,(float)-.1592,(float)-4.036e-4,(float)
	    -.002548,(float)-.003402,(float)-.004446,(float)-.005819,(float)
	    -.006207,(float)-.007248,(float)-.008249,(float)-.009605,(float)
	    -.01093,(float)-.01191,(float)-.01231,(float)-.0135,(float)
	    -.01563,(float)-.02288,(float)-.03154,(float)-.03619,(float)
	    -.0502,(float)-.06812,(float)-.1727,(float).001497,(float)
	    -.001083,(float)-.002244,(float)-.003014,(float)-.004774,(float)
	    -.004666,(float)-.005826,(float)-.007079,(float)-.008601,(float)
	    -.009904,(float)-.011,(float)-.01135,(float)-.01293,(float)
	    -.01535,(float)-.02404,(float)-.03399,(float)-.03913,(float)
	    -.05459,(float)-.07408,(float)-.1866,(float).002087,(float)
	    -4.758e-4,(float)-.001182,(float)-.002147,(float)-.003636,(float)
	    -.00367,(float)-.004808,(float)-.006049,(float)-.007678,(float)
	    -.009012,(float)-.01015,(float)-.01056,(float)-.0124,(float)
	    -.0151,(float)-.02518,(float)-.03647,(float)-.04211,(float)-.0591,
	    (float)-.08021,(float)-.2009,(float).001657,(float)4.041e-4,(
	    float)-2.04e-4,(float)-.001212,(float)-.002503,(float)-.003007,(
	    float)-.00389,(float)-.005228,(float)-.006826,(float)-.008237,(
	    float)-.009345,(float)-.009909,(float)-.01191,(float)-.01489,(
	    float)-.02632,(float)-.03897,(float)-.04513,(float)-.06375,(float)
	    -.08653,(float)-.2154,(float).002143,(float).00128,(float)
	    4.527e-4,(float)-5.118e-4,(float)-.001788,(float)-.002176,(float)
	    -.003038,(float)-.004549,(float)-.006152,(float)-.007603,(float)
	    -.008677,(float)-.009308,(float)-.0115,(float)-.01471,(float)
	    -.02746,(float)-.04152,(float)-.04821,(float)-.06852,(float)
	    -.09304,(float)-.2303,(float).003304,(float).001548,(float)
	    8.72e-4,(float)-2.022e-4,(float)-.00133,(float)-.001423,(float)
	    -.00243,(float)-.003857,(float)-.005612,(float)-.007065,(float)
	    -.008121,(float)-.008767,(float)-.01115,(float)-.01456,(float)
	    -.02863,(float)-.04411,(float)-.05136,(float)-.07344,(float)
	    -.09976,(float)-.2455,(float).003609,(float).001781,(float)
	    .001457,(float)2.086e-4,(float)-7.149e-4,(float)-.001069,(float)
	    -.001931,(float)-.003234,(float)-.005069,(float)-.006523,(float)
	    -.007609,(float)-.00835,(float)-.01085,(float)-.01442,(float)
	    -.02982,(float)-.04676,(float)-.0546,(float)-.07852,(float)-.1067,
	    (float)-.2608,(float).003288,(float).002567,(float).002287,(float)
	    9.618e-4,(float)-6.828e-5,(float)-5.962e-4,(float)-.00132,(float)
	    -.002838,(float)-.00453,(float)-.005954,(float)-.007149,(float)
	    -.008019,(float)-.01058,(float)-.0143,(float)-.03103,(float)
	    -.04947,(float)-.05792,(float)-.08376,(float)-.1138,(float)-.2764,
	    (float).003797,(float).003456,(float).002885,(float).001733,(
	    float)4.573e-4,(float)9.352e-5,(float)-7.169e-4,(float)-.002501,(
	    float)-.004033,(float)-.005442,(float)-.006763,(float)-.007705,(
	    float)-.01035,(float)-.0142,(float)-.03228,(float)-.05226,(float)
	    -.06136,(float)-.08918,(float)-.1212,(float)-.2921,(float).004872,
	    (float).003663,(float).003056,(float).002072,(float).001058,(
	    float)6.808e-4,(float)-3.549e-4,(float)-.001975,(float)-.003557,(
	    float)-.005021,(float)-.00642,(float)-.007394,(float)-.01016,(
	    float)-.01416,(float)-.03357,(float)-.05515,(float)-.06492,(float)
	    -.0948,(float)-.1288,(float)-.3079,(float).006129,(float).00386,(
	    float).003389,(float).002448,(float).001663,(float).001268,(float)
	    -1.914e-5,(float)-.001434,(float)-.003079,(float)-.004591,(float)
	    -.006104,(float)-.007096,(float)-.009979,(float)-.01414,(float)
	    -.03492,(float)-.05815,(float)-.06864,(float)-.1006,(float)-.1366,
	    (float)-.3238,(float).006769,(float).004835,(float).004199,(float)
	    .003363,(float).002294,(float).001913,(float)5.437e-4,(float)
	    -.001045,(float)-.002562,(float)-.004111,(float)-.005782,(float)
	    -.006827,(float)-.009791,(float)-.01415,(float)-.03635,(float)
	    -.06129,(float)-.07252,(float)-.1066,(float)-.1448,(float)-.3397,(
	    float).007335,(float).005663,(float).004951,(float).004169,(float)
	    .003129,(float).002525,(float).001106,(float)-5.853e-4,(float)
	    -.001973,(float)-.003656,(float)-.005421,(float)-.006575,(float)
	    -.009579,(float)-.01417,(float)-.03789,(float)-.06459,(float)
	    -.07659,(float)-.1129,(float)-.1531,(float)-.3555,(float).008643,(
	    float).006322,(float).005611,(float).004819,(float).003931,(float)
	    .003139,(float).001661,(float)1.863e-5,(float)-.001383,(float)
	    -.003222,(float)-.005057,(float)-.006302,(float)-.009358,(float)
	    -.01422,(float)-.03954,(float)-.06805,(float)-.08087,(float)
	    -.1194,(float)-.1617,(float)-.3713,(float).009557,(float).007667,(
	    float).006817,(float).005963,(float).004575,(float).003855,(float)
	    .002412,(float)6.278e-4,(float)-8.448e-4,(float)-.002738,(float)
	    -.004709,(float)-.005988,(float)-.009143,(float)-.01429,(float)
	    -.04131,(float)-.07171,(float)-.08539,(float)-.1261,(float)-.1706,
	    (float)-.3868,(float).01068,(float).008904,(float).008115,(float)
	    .007034,(float).005319,(float).004686,(float).003195,(float)
	    .001332,(float)-2.873e-4,(float)-.00228,(float)-.004337,(float)
	    -.005596,(float)-.008919,(float)-.01439,(float)-.04323,(float)
	    -.07558,(float)-.09015,(float)-.133,(float)-.1797,(float)-.4021,(
	    float).01298,(float).009905,(float).008826,(float).0078,(float)
	    .006089,(float).005611,(float).004025,(float).002225,(float)
	    2.886e-4,(float)-.001886,(float)-.003936,(float)-.005117,(float)
	    -.008682,(float)-.01452,(float)-.04528,(float)-.07966,(float)
	    -.09515,(float)-.1402,(float)-.1891,(float)-.417,(float).01395,(
	    float).01067,(float).009268,(float).008361,(float).006411,(float)
	    .006124,(float).004532,(float).002699,(float)5.708e-4,(float)
	    -.001658,(float)-.003722,(float)-.004872,(float)-.008559,(float)
	    -.01459,(float)-.04635,(float)-.08179,(float)-.09775,(float)
	    -.1439,(float)-.1939,(float)-.4243,(float).01447,(float).01162,(
	    float).009985,(float).009085,(float).006748,(float).006635,(float)
	    .005086,(float).003165,(float)8.697e-4,(float)-.001385,(float)
	    -.003488,(float)-.004635,(float)-.008428,(float)-.01467,(float)
	    -.04747,(float)-.08398,(float)-.1004,(float)-.1477,(float)-.1987,(
	    float)-.4315,(float).01504,(float).01323,(float).01195,(float)
	    .01044,(float).007799,(float).007467,(float).006131,(float)
	    .004089,(float).001622,(float)-7.561e-4,(float)-.002916,(float)
	    -.004161,(float)-.008117,(float)-.01481,(float)-.0498,(float)
	    -.08854,(float)-.1059,(float)-.1555,(float)-.2086,(float)-.4456,(
	    float).01661,(float).01408,(float).01341,(float).01124,(float)
	    .009324,(float).008339,(float).007058,(float).005098,(float)
	    .002563,(float)-1.249e-4,(float)-.002227,(float)-.003613,(float)
	    -.007749,(float)-.01495,(float)-.05226,(float)-.09335,(float)
	    -.1117,(float)-.1635,(float)-.2186,(float)-.459,(float).01898,(
	    float).01571,(float).01459,(float).01252,(float).01057,(float)
	    .009799,(float).008325,(float).006137,(float).003436,(float)
	    6.209e-4,(float)-.001541,(float)-.002982,(float)-.007388,(float)
	    -.01511,(float)-.05486,(float)-.09842,(float)-.1177,(float)-.1718,
	    (float)-.2288,(float)-.4717,(float).02061,(float).01827,(float)
	    .01651,(float).01454,(float).01163,(float).01147,(float).009937,(
	    float).00714,(float).004277,(float).001598,(float)-8.513e-4,(
	    float)-.002308,(float)-.007034,(float)-.01524,(float)-.0576,(
	    float)-.1037,(float)-.1239,(float)-.1803,(float)-.2391,(float)
	    -.4835,(float).02232,(float).01991,(float).01849,(float).01614,(
	    float).01332,(float).01303,(float).01133,(float).008324,(float)
	    .005371,(float).002669,(float)-2.911e-5,(float)-.001572,(float)
	    -.006626,(float)-.01535,(float)-.0605,(float)-.1093,(float)-.1304,
	    (float)-.1891,(float)-.2494,(float)-.4945,(float).02514,(float)
	    .02125,(float).01987,(float).0176,(float).01535,(float).01479,(
	    float).01269,(float).009743,(float).006689,(float).003765,(float)
	    9.395e-4,(float)-7.213e-4,(float)-.006153,(float)-.01543,(float)
	    -.06359,(float)-.1151,(float)-.1372,(float)-.1981,(float)-.2598,(
	    float)-.5044,(float).02842,(float).02362,(float).02171,(float)
	    .01983,(float).01722,(float).01684,(float).01443,(float).01118,(
	    float).008146,(float).005048,(float).00203,(float)2.08e-4,(float)
	    -.005615,(float)-.01545,(float)-.06688,(float)-.1212,(float)
	    -.1442,(float)-.2072,(float)-.2702,(float)-.5131,(float).03015,(
	    float).02592,(float).0246,(float).02229,(float).01963,(float)
	    .01866,(float).01622,(float).01284,(float).009911,(float).006578,(
	    float).003312,(float).001176,(float)-.004972,(float)-.01541,(
	    float)-.0704,(float)-.1275,(float)-.1514,(float)-.2164,(float)
	    -.2805,(float)-.5205,(float).03158,(float).02787,(float).0274,(
	    float).02464,(float).02237,(float).02041,(float).01805,(float)
	    .01499,(float).01188,(float).008207,(float).004719,(float).002305,
	    (float)-.004239,(float)-.01534,(float)-.07414,(float)-.1341,(
	    float)-.1589,(float)-.2257,(float)-.2906,(float)-.5264,(float)
	    .03669,(float).03089,(float).02975,(float).02746,(float).0245,(
	    float).02324,(float).02049,(float).01729,(float).01382,(float)
	    .009949,(float).006144,(float).003727,(float)-.003435,(float)
	    -.01528,(float)-.07812,(float)-.1408,(float)-.1666,(float)-.235,(
	    float)-.3004,(float)-.5308,(float).04103,(float).03509,(float)
	    .03303,(float).03088,(float).02689,(float).02671,(float).0236,(
	    float).0196,(float).01599,(float).01199,(float).00777,(float)
	    .005314,(float)-.002463,(float)-.01518,(float)-.0823,(float)
	    -.1478,(float)-.1743,(float)-.2441,(float)-.3097,(float)-.5335,(
	    float).04226,(float).03916,(float).03747,(float).03451,(float)
	    .03038,(float).02937,(float).02675,(float).02248,(float).01869,(
	    float).01435,(float).009783,(float).00698,(float)-.001231,(float)
	    -.01506,(float)-.08667,(float)-.1549,(float)-.1822,(float)-.2531,(
	    float)-.3186,(float)-.5344,(float).04606,(float).04221,(float)
	    .04136,(float).03771,(float).03432,(float).03233,(float).02983,(
	    float).02611,(float).02173,(float).01684,(float).0121,(float)
	    .008974,(float)2.256e-4,(float)-.01491,(float)-.09116,(float)
	    -.1621,(float)-.19,(float)-.2617,(float)-.3268,(float)-.5333,(
	    float).05303,(float).04604,(float).04479,(float).04113,(float)
	    .0383,(float).03682,(float).03365,(float).0299,(float).02496,(
	    float).01957,(float).01466,(float).01142,(float).00185,(float)
	    -.01471,(float)-.09575,(float)-.1693,(float)-.1977,(float)-.2698,(
	    float)-.3341,(float)-.5302,(float).05746,(float).05197,(float)
	    .04979,(float).04632,(float).04288,(float).04179,(float).03846,(
	    float).0337,(float).02862,(float).02287,(float).01759,(float)
	    .01417,(float).003656,(float)-.01436,(float)-.1004,(float)-.1763,(
	    float)-.2052,(float)-.2774,(float)-.3404,(float)-.5249,(float)
	    .061,(float).05751,(float).05591,(float).05222,(float).04833,(
	    float).04665,(float).04341,(float).03818,(float).0328,(float)
	    .02673,(float).02099,(float).01723,(float).005654,(float)-.01383,(
	    float)-.105,(float)-.1831,(float)-.2123,(float)-.2841,(float)
	    -.3456,(float)-.5173,(float).06848,(float).06183,(float).06082,(
	    float).05728,(float).05407,(float).05236,(float).04857,(float)
	    .04368,(float).03737,(float).03088,(float).02479,(float).02076,(
	    float).00785,(float)-.01309,(float)-.1095,(float)-.1895,(float)
	    -.2188,(float)-.2898,(float)-.3493,(float)-.5073,(float).07725,(
	    float).06828,(float).06577,(float).06361,(float).06,(float).0591,(
	    float).05476,(float).0496,(float).04247,(float).03556,(float)
	    .02896,(float).02469,(float).01038,(float)-.01211,(float)-.1139,(
	    float)-.1954,(float)-.2247,(float)-.2944,(float)-.3514,(float)
	    -.4949,(float).08308,(float).07651,(float).07395,(float).07184,(
	    float).06706,(float).06618,(float).06171,(float).05564,(float)
	    .0486,(float).04115,(float).03368,(float).02885,(float).01347,(
	    float)-.01083,(float)-.118,(float)-.2005,(float)-.2296,(float)
	    -.2975,(float)-.3517,(float)-.4801,(float).08673,(float).08355,(
	    float).08361,(float).07958,(float).07582,(float).07311,(float)
	    .06882,(float).06262,(float).05585,(float).04731,(float).03906,(
	    float).03336,(float).01719,(float)-.009279,(float)-.1217,(float)
	    -.2047,(float)-.2334,(float)-.2989,(float)-.35,(float)-.4629,(
	    float).09333,(float).09061,(float).0912,(float).08707,(float)
	    .08512,(float).08086,(float).0767,(float).07113,(float).06356,(
	    float).05366,(float).04492,(float).03862,(float).02135,(float)
	    -.007482,(float)-.1247,(float)-.2078,(float)-.2358,(float)-.2984,(
	    float)-.346,(float)-.4432,(float).1045,(float).1001,(float).09799,
	    (float).09636,(float).09317,(float).09079,(float).08611,(float)
	    .0806,(float).07117,(float).06057,(float).0511,(float).04488,(
	    float).0258,(float)-.005331,(float)-.1269,(float)-.2094,(float)
	    -.2365,(float)-.2958,(float)-.3395,(float)-.4213,(float).1155,(
	    float).1107,(float).1079,(float).1069,(float).1017,(float).102,(
	    float).09663,(float).09015,(float).07944,(float).06847,(float)
	    .058,(float).05183,(float).03074,(float)-.002583,(float)-.128,(
	    float)-.2093,(float)-.2353,(float)-.2909,(float)-.3305,(float)
	    -.3971,(float).1211,(float).1202,(float).1194,(float).1174,(float)
	    .1138,(float).1128,(float).1074,(float).1,(float).08932,(float)
	    .07729,(float).06612,(float).05905,(float).03639,(float)9.063e-4,(
	    float)-.1279,(float)-.2072,(float)-.2319,(float)-.2833,(float)
	    -.3187,(float)-.3709,(float).1233,(float).1292,(float).1303,(
	    float).128,(float).1278,(float).123,(float).1184,(float).1111,(
	    float).1002,(float).08679,(float).07506,(float).06651,(float)
	    .04269,(float).005089,(float)-.1263,(float)-.2029,(float)-.2261,(
	    float)-.2731,(float)-.3041,(float)-.3429,(float).1321,(float)
	    .1388,(float).1403,(float).1395,(float).1402,(float).1346,(float)
	    .1299,(float).1231,(float).1111,(float).09672,(float).08401,(
	    float).07458,(float).04938,(float).009833,(float)-.1231,(float)
	    -.1963,(float)-.2178,(float)-.26,(float)-.2867,(float)-.3134,(
	    float).1445,(float).1497,(float).1506,(float).1521,(float).1511,(
	    float).148,(float).1421,(float).1351,(float).1219,(float).1067,(
	    float).09276,(float).08323,(float).05638,(float).01506,(float)
	    -.1179,(float)-.1871,(float)-.2068,(float)-.2442,(float)-.2666,(
	    float)-.2827,(float).1527,(float).1612,(float).1618,(float).1646,(
	    float).1629,(float).1612,(float).1546,(float).1468,(float).1328,(
	    float).1166,(float).1016,(float).09185,(float).06358,(float).0207,
	    (float)-.1108,(float)-.1753,(float)-.1931,(float)-.2256,(float)
	    -.244,(float)-.2512,(float).1531,(float).1729,(float).1742,(float)
	    .1762,(float).1762,(float).172,(float).1665,(float).1578,(float)
	    .1436,(float).1258,(float).1104,(float).09967,(float).07066,(
	    float).02665,(float)-.1018,(float)-.161,(float)-.1768,(float)
	    -.2046,(float)-.2192,(float)-.2193,(float).1504,(float).1808,(
	    float).1867,(float).1857,(float).1899,(float).1799,(float).176,(
	    float).1674,(float).1534,(float).1338,(float).1183,(float).1061,(
	    float).077,(float).03257,(float)-.09103,(float)-.1444,(float)
	    -.1582,(float)-.1814,(float)-.1928,(float)-.1877,(float).1585,(
	    float).1857,(float).1955,(float).1931,(float).2005,(float).1877,(
	    float).1827,(float).1744,(float).1602,(float).1394,(float).1237,(
	    float).1108,(float).08167,(float).03777,(float)-.07876,(float)
	    -.126,(float)-.1377,(float)-.1566,(float)-.1651,(float)-.1568,(
	    float).1756,(float).1927,(float).1989,(float).1996,(float).2039,(
	    float).1954,(float).1872,(float).1776,(float).1618,(float).1413,(
	    float).1249,(float).1131,(float).08353,(float).04143,(float)
	    -.0655,(float)-.1062,(float)-.1159,(float)-.131,(float)-.1371,(
	    float)-.1271,(float).1892,(float).2037,(float).2008,(float).205,(
	    float).2001,(float).1984,(float).1885,(float).1755,(float).1574,(
	    float).1384,(float).1214,(float).1112,(float).08191,(float).04292,
	    (float)-.0519,(float)-.0858,(float)-.09356,(float)-.1052,(float)
	    -.1094,(float)-.09931,(float).1897,(float).2113,(float).2088,(
	    float).2055,(float).1944,(float).1912,(float).1819,(float).1647,(
	    float).1485,(float).1301,(float).1133,(float).1032,(float).07671,(
	    float).04191,(float)-.03872,(float)-.06571,(float)-.07167,(float)
	    -.08025,(float)-.08302,(float)-.07396,(float).1712,(float).2034,(
	    float).2211,(float).1947,(float).188,(float).168,(float).1625,(
	    float).1434,(float).1349,(float).1157,(float).1004,(float).08785,(
	    float).06772,(float).03801,(float)-.02677,(float)-.04688,(float)
	    -.05119,(float)-.05716,(float)-.05887,(float)-.05161,(float).1322,
	    (float).1811,(float).2187,(float).1714,(float).1713,(float).1281,(
	    float).1309,(float).1119,(float).1131,(float).09404,(float).0814,(
	    float).06592,(float).05425,(float).03086,(float)-.0167,(float)
	    -.03025,(float)-.0331,(float)-.03691,(float)-.03788,(float)
	    -.03277,(float).08504,(float).1435,(float).1758,(float).1326,(
	    float).133,(float).08054,(float).09003,(float).07395,(float)
	    .07987,(float).06447,(float).05582,(float).04075,(float).0365,(
	    float).02082,(float)-.008919,(float)-.0167,(float)-.01832,(float)
	    -.02041,(float)-.0209,(float)-.01789,(float).03936,(float).08311,(
	    float).09568,(float).07461,(float).0731,(float).03755,(float)
	    .04509,(float).03589,(float).04026,(float).03161,(float).02757,(
	    float).01806,(float).01764,(float).01004,(float)-.003585,(float)
	    -.006904,(float)-.007594,(float)-.00846,(float)-.008646,(float)
	    -.007351,(float).008367,(float).01985,(float).02146,(float).01751,
	    (float).01672,(float).00796,(float).009761,(float).007629,(float)
	    .008715,(float).006695,(float).005893,(float).003567,(float)
	    .003711,(float).002105,(float)-6.734e-4,(float)-.001321,(float)
	    -.001456,(float)-.001622,(float)-.001656,(float)-.001403,(float)
	    1.901e-20,(float)-6.852e-20,(float)-2.92e-22,(float)6.497e-20,(
	    float)4.95e-20,(float)9.892e-20,(float)1.818e-20,(float)
	    -9.169e-22,(float)-3.362e-21,(float)6.922e-20,(float)2.684e-20,(
	    float)2.626e-20,(float)-3.55e-20,(float)2.511e-20,(float)
	    -5.888e-21,(float)4.135e-20,(float)-1.341e-21,(float)-1.894e-20,(
	    float)1.482e-20,(float)-2.286e-21 };
    static real uhr[1660]	/* was [20][83] */ = { (float)-.2975,(float)
	    -.2458,(float)-.2347,(float)-.2072,(float)-.1879,(float)-.1755,(
	    float)-.1638,(float)-.1489,(float)-.1415,(float)-.1337,(float)
	    -.1284,(float)-.1253,(float)-.1225,(float)-.12,(float)-.128,(
	    float)-.142,(float)-.1485,(float)-.1709,(float)-.1989,(float)
	    -.4039,(float)-.2668,(float)-.2241,(float)-.2152,(float)-.1914,(
	    float)-.1748,(float)-.1643,(float)-.1546,(float)-.1418,(float)
	    -.1356,(float)-.1291,(float)-.1246,(float)-.1221,(float)-.12,(
	    float)-.1184,(float)-.1274,(float)-.1416,(float)-.1482,(float)
	    -.1707,(float)-.1987,(float)-.4037,(float)-.1549,(float)-.1447,(
	    float)-.1427,(float)-.1329,(float)-.1262,(float)-.1222,(float)
	    -.1193,(float)-.1143,(float)-.1128,(float)-.111,(float)-.1098,(
	    float)-.1093,(float)-.1102,(float)-.1117,(float)-.1248,(float)
	    -.1399,(float)-.1467,(float)-.1695,(float)-.1977,(float)-.4031,(
	    float)-.01893,(float)-.04556,(float)-.04866,(float)-.05698,(float)
	    -.06112,(float)-.06482,(float)-.07024,(float)-.07374,(float)
	    -.0794,(float)-.08412,(float)-.08708,(float)-.08905,(float)
	    -.09449,(float)-.1007,(float)-.1202,(float)-.1368,(float)-.144,(
	    float)-.1675,(float)-.1961,(float)-.402,(float).08761,(float)
	    .03753,(float).03153,(float).01086,(float)-6.59e-5,(float)
	    -.009411,(float)-.02173,(float)-.03142,(float)-.04385,(float)
	    -.05464,(float)-.06128,(float)-.06548,(float)-.07561,(float)
	    -.0867,(float)-.1139,(float)-.1325,(float)-.1402,(float)-.1645,(
	    float)-.1937,(float)-.4004,(float).149,(float).09297,(float)
	    .08391,(float).05998,(float).0454,(float).03384,(float).01788,(
	    float).00398,(float)-.01264,(float)-.02736,(float)-.03663,(float)
	    -.04236,(float)-.05627,(float)-.07146,(float)-.1061,(float)-.127,(
	    float)-.1353,(float)-.1608,(float)-.1907,(float)-.3983,(float)
	    .169,(float).1209,(float).1109,(float).08847,(float).07199,(float)
	    .06082,(float).0448,(float).02856,(float).01078,(float)-.005128,(
	    float)-.01602,(float)-.02251,(float)-.03862,(float)-.05647,(float)
	    -.09731,(float)-.1207,(float)-.1296,(float)-.1563,(float)-.187,(
	    float)-.3957,(float).1621,(float).1259,(float).1176,(float).09828,
	    (float).08237,(float).07282,(float).05866,(float).04264,(float)
	    .02581,(float).01067,(float)-6.693e-4,(float)-.007273,(float)
	    -.02391,(float)-.04287,(float)-.08809,(float)-.1137,(float)-.1232,
	    (float)-.1512,(float)-.1828,(float)-.3926,(float).1425,(float)
	    .1161,(float).1105,(float).09476,(float).08222,(float).074,(float)
	    .0622,(float).04865,(float).03391,(float).0202,(float).009705,(
	    float).003367,(float)-.01248,(float)-.03132,(float)-.07884,(float)
	    -.1062,(float)-.1164,(float)-.1457,(float)-.1781,(float)-.3891,(
	    float).1202,(float).09999,(float).0965,(float).08454,(float)
	    .07657,(float).06903,(float).05955,(float).04937,(float).03702,(
	    float).02497,(float).01606,(float).01027,(float)-.004072,(float)
	    -.02191,(float)-.06988,(float)-.09867,(float)-.1093,(float)-.1397,
	    (float)-.173,(float)-.3851,(float).1006,(float).08441,(float)
	    .08184,(float).07332,(float).06805,(float).06219,(float).05464,(
	    float).04702,(float).03695,(float).02688,(float).01949,(float)
	    .01454,(float).001893,(float)-.01438,(float)-.06141,(float)
	    -.09113,(float)-.1022,(float)-.1336,(float)-.1676,(float)-.3806,(
	    float).08386,(float).07229,(float).06955,(float).06387,(float)
	    .05898,(float).05552,(float).04967,(float).04327,(float).03532,(
	    float).0274,(float).02112,(float).01709,(float).00611,(float)
	    -.008345,(float)-.05355,(float)-.08376,(float)-.09513,(float)
	    -.1273,(float)-.162,(float)-.3757,(float).0708,(float).06217,(
	    float).05952,(float).05578,(float).0514,(float).04945,(float)
	    .04499,(float).03953,(float).03338,(float).02725,(float).02193,(
	    float).01863,(float).009222,(float)-.003461,(float)-.04633,(float)
	    -.07664,(float)-.08821,(float)-.121,(float)-.1563,(float)-.3703,(
	    float).05982,(float).05317,(float).05163,(float).04867,(float)
	    .04579,(float).04406,(float).04066,(float).03665,(float).03176,(
	    float).02679,(float).02246,(float).01961,(float).01167,(float)
	    5.524e-4,(float)-.03973,(float)-.06985,(float)-.08152,(float)
	    -.1147,(float)-.1504,(float)-.3645,(float).05096,(float).04587,(
	    float).04572,(float).04299,(float).04188,(float).03959,(float)
	    .03709,(float).03458,(float).03059,(float).02633,(float).0229,(
	    float).02037,(float).01368,(float).003912,(float)-.0337,(float)
	    -.06339,(float)-.07508,(float)-.1084,(float)-.1444,(float)-.3582,(
	    float).04534,(float).04108,(float).04123,(float).0392,(float)
	    .03887,(float).03676,(float).03475,(float).03295,(float).0297,(
	    float).02608,(float).02328,(float).02115,(float).01539,(float)
	    .006786,(float)-.02821,(float)-.05728,(float)-.06891,(float)
	    -.1023,(float)-.1385,(float)-.3514,(float).04095,(float).03883,(
	    float).03829,(float).03724,(float).03623,(float).03534,(float)
	    .0336,(float).03169,(float).02902,(float).02615,(float).02365,(
	    float).02194,(float).0169,(float).009315,(float)-.02319,(float)
	    -.05149,(float)-.06302,(float)-.09627,(float)-.1324,(float)-.3442,
	    (float).03683,(float).03724,(float).03675,(float).03592,(float)
	    .03462,(float).03409,(float).03288,(float).03099,(float).02881,(
	    float).02647,(float).02419,(float).02271,(float).01836,(float)
	    .01161,(float)-.01858,(float)-.04602,(float)-.05738,(float)
	    -.09034,(float)-.1264,(float)-.3364,(float).03467,(float).03524,(
	    float).03552,(float).03453,(float).03409,(float).03301,(float)
	    .03214,(float).03081,(float).02907,(float).02689,(float).02493,(
	    float).02351,(float).0198,(float).01374,(float)-.01432,(float)
	    -.04082,(float)-.05197,(float)-.08453,(float)-.1204,(float)-.3282,
	    (float).03363,(float).03361,(float).03436,(float).03352,(float)
	    .03395,(float).03255,(float).03166,(float).03096,(float).02951,(
	    float).02739,(float).02577,(float).02441,(float).02119,(float)
	    .01573,(float)-.01035,(float)-.03587,(float)-.04678,(float)-.0788,
	    (float)-.1143,(float)-.3194,(float).03287,(float).03266,(float)
	    .03347,(float).03301,(float).03388,(float).0325,(float).0316,(
	    float).03132,(float).02992,(float).02801,(float).02666,(float)
	    .02544,(float).02249,(float).01761,(float)-.00661,(float)-.03112,(
	    float)-.04175,(float)-.07314,(float)-.1081,(float)-.31,(float)
	    .03248,(float).03251,(float).03299,(float).03301,(float).03375,(
	    float).03281,(float).032,(float).03176,(float).03035,(float)
	    .02876,(float).02756,(float).02654,(float).0238,(float).0194,(
	    float)-.00305,(float)-.02653,(float)-.03685,(float)-.06751,(float)
	    -.1019,(float)-.3,(float).03227,(float).03315,(float).03325,(
	    float).03357,(float).03376,(float).03336,(float).03272,(float)
	    .03214,(float).03096,(float).02966,(float).02852,(float).02766,(
	    float).02518,(float).02116,(float)3.643e-4,(float)-.02207,(float)
	    -.03205,(float)-.06189,(float)-.09558,(float)-.2894,(float).03202,
	    (float).03373,(float).03383,(float).03408,(float).03412,(float)
	    .03393,(float).03344,(float).03268,(float).03179,(float).03063,(
	    float).02956,(float).0288,(float).02665,(float).0229,(float)
	    .003671,(float)-.01769,(float)-.02731,(float)-.05623,(float)
	    -.0891,(float)-.278,(float).03221,(float).03388,(float).03404,(
	    float).03426,(float).03465,(float).03451,(float).03408,(float)
	    .03362,(float).03274,(float).03161,(float).03068,(float).03001,(
	    float).02817,(float).02468,(float).006915,(float)-.01333,(float)
	    -.02258,(float)-.05048,(float)-.08243,(float)-.2659,(float).03315,
	    (float).03429,(float).03451,(float).03476,(float).03528,(float)
	    .0352,(float).03483,(float).03472,(float).0338,(float).03272,(
	    float).03189,(float).03133,(float).02972,(float).02653,(float)
	    .01015,(float)-.008958,(float)-.01781,(float)-.04461,(float)
	    -.07552,(float)-.253,(float).03341,(float).03542,(float).03568,(
	    float).03596,(float).03607,(float).03611,(float).03588,(float)
	    .03576,(float).03498,(float).03407,(float).03322,(float).03275,(
	    float).03133,(float).0285,(float).01342,(float)-.004508,(float)
	    -.01296,(float)-.03855,(float)-.06831,(float)-.2391,(float).03416,
	    (float).0368,(float).03694,(float).03742,(float).03709,(float)
	    .03734,(float).03718,(float).03689,(float).03632,(float).03558,(
	    float).03473,(float).03432,(float).03304,(float).03059,(float)
	    .0168,(float)5.975e-5,(float)-.007946,(float)-.03226,(float)
	    -.06073,(float)-.2243,(float).03596,(float).03789,(float).03806,(
	    float).03875,(float).03849,(float).03875,(float).03852,(float)
	    .03828,(float).03788,(float).03721,(float).03646,(float).03607,(
	    float).03493,(float).0328,(float).02031,(float).00479,(float)
	    -.00273,(float)-.02567,(float)-.05272,(float)-.2084,(float).0375,(
	    float).03897,(float).03948,(float).04012,(float).04035,(float)
	    .04016,(float).03995,(float).03996,(float).03968,(float).03899,(
	    float).0384,(float).03801,(float).03701,(float).03514,(float)
	    .02401,(float).009731,(float).002748,(float)-.01872,(float)
	    -.04419,(float)-.1914,(float).03891,(float).04054,(float).04132,(
	    float).04176,(float).04245,(float).04187,(float).04174,(float)
	    .04184,(float).04165,(float).04098,(float).04053,(float).04016,(
	    float).0393,(float).03764,(float).02791,(float).01494,(float)
	    .008549,(float)-.01132,(float)-.03507,(float)-.1732,(float).04129,
	    (float).04285,(float).04339,(float).04385,(float).04439,(float)
	    .04421,(float).044,(float).04396,(float).04372,(float).04322,(
	    float).04283,(float).04258,(float).0418,(float).04035,(float)
	    .03206,(float).02049,(float).01474,(float)-.003382,(float)-.02525,
	    (float)-.1536,(float).04417,(float).04571,(float).0458,(float)
	    .04635,(float).04649,(float).04679,(float).04662,(float).04639,(
	    float).04606,(float).04572,(float).04539,(float).04526,(float)
	    .04458,(float).04334,(float).03651,(float).02646,(float).02139,(
	    float).005188,(float)-.01464,(float)-.1325,(float).04684,(float)
	    .04849,(float).04869,(float).04896,(float).04928,(float).04939,(
	    float).04938,(float).04919,(float).04885,(float).0485,(float)
	    .04833,(float).04821,(float).04769,(float).04666,(float).04133,(
	    float).03294,(float).02859,(float).0145,(float)-.003121,(float)
	    -.1098,(float).05013,(float).05137,(float).05183,(float).05179,(
	    float).05249,(float).05231,(float).05239,(float).05242,(float)
	    .05201,(float).05159,(float).05162,(float).0515,(float).05114,(
	    float).05035,(float).0466,(float).04003,(float).03646,(float)
	    .02466,(float).009448,(float)-.08535,(float).05405,(float).05498,(
	    float).05537,(float).05527,(float).05587,(float).05573,(float)
	    .05583,(float).05607,(float).0555,(float).05508,(float).05523,(
	    float).05519,(float).05494,(float).05445,(float).05241,(float)
	    .04783,(float).0451,(float).03581,(float).02321,(float)-.05904,(
	    float).05841,(float).05913,(float).05952,(float).05936,(float)
	    .05974,(float).05966,(float).05974,(float).06009,(float).05942,(
	    float).05907,(float).05925,(float).05931,(float).05917,(float)
	    .05902,(float).05887,(float).05646,(float).05467,(float).04809,(
	    float).03833,(float)-.03069,(float).06347,(float).06371,(float)
	    .064,(float).06393,(float).06419,(float).06418,(float).06417,(
	    float).06449,(float).06385,(float).0636,(float).06377,(float)
	    .06391,(float).06391,(float).06414,(float).06608,(float).06605,(
	    float).0653,(float).06166,(float).05501,(float)-1.341e-4,(float)
	    .06904,(float).06905,(float).06899,(float).06915,(float).06921,(
	    float).06938,(float).06925,(float).06936,(float).06886,(float)
	    .06873,(float).06888,(float).06905,(float).06922,(float).06987,(
	    float).07416,(float).07674,(float).07716,(float).07669,(float)
	    .07343,(float).03282,(float).07469,(float).07498,(float).07493,(
	    float).07503,(float).07508,(float).07512,(float).0749,(float)
	    .07485,(float).07458,(float).07452,(float).07468,(float).07481,(
	    float).07522,(float).07631,(float).08324,(float).0887,(float)
	    .09044,(float).09342,(float).09383,(float).06835,(float).08139,(
	    float).08126,(float).08148,(float).0814,(float).0818,(float)
	    .08147,(float).08112,(float).08116,(float).08105,(float).08102,(
	    float).08123,(float).08135,(float).08197,(float).08355,(float)
	    .09346,(float).1021,(float).1053,(float).1121,(float).1165,(float)
	    .1067,(float).08556,(float).08467,(float).08488,(float).08483,(
	    float).08536,(float).08505,(float).08453,(float).08465,(float)
	    .08454,(float).08456,(float).0848,(float).08494,(float).08565,(
	    float).08752,(float).09906,(float).1095,(float).1135,(float).1222,
	    (float).1287,(float).1269,(float).09018,(float).08842,(float)
	    .08848,(float).08854,(float).08906,(float).08894,(float).08822,(
	    float).08836,(float).08823,(float).08833,(float).08857,(float)
	    .08878,(float).08956,(float).09173,(float).105,(float).1173,(
	    float).1221,(float).1329,(float).1416,(float).148,(float).09939,(
	    float).09722,(float).09678,(float).09708,(float).09724,(float)
	    .09748,(float).09652,(float).09644,(float).09634,(float).09665,(
	    float).09687,(float).09717,(float).09817,(float).101,(float).1181,
	    (float).1344,(float).1409,(float).1563,(float).1696,(float).1925,(
	    float).1083,(float).1073,(float).1068,(float).1069,(float).1069,(
	    float).1067,(float).1059,(float).1056,(float).1057,(float).1061,(
	    float).1063,(float).1067,(float).108,(float).1116,(float).1329,(
	    float).1537,(float).1622,(float).1826,(float).2008,(float).2405,(
	    float).1199,(float).1181,(float).118,(float).1177,(float).1176,(
	    float).1172,(float).1164,(float).1162,(float).1163,(float).1168,(
	    float).1171,(float).1175,(float).1192,(float).1236,(float).1497,(
	    float).1757,(float).1863,(float).2122,(float).2355,(float).2923,(
	    float).134,(float).1307,(float).1305,(float).13,(float).1296,(
	    float).1294,(float).1285,(float).1284,(float).1284,(float).1288,(
	    float).1294,(float).1299,(float).132,(float).1373,(float).1689,(
	    float).2007,(float).2136,(float).2455,(float).2742,(float).348,(
	    float).1482,(float).1455,(float).145,(float).1444,(float).1436,(
	    float).1432,(float).1424,(float).1423,(float).1422,(float).1426,(
	    float).1435,(float).1441,(float).1467,(float).153,(float).191,(
	    float).2293,(float).2446,(float).2831,(float).3175,(float).408,(
	    float).1645,(float).162,(float).1618,(float).1605,(float).1597,(
	    float).1588,(float).1582,(float).1583,(float).1581,(float).1584,(
	    float).1596,(float).1605,(float).1637,(float).171,(float).2164,(
	    float).2619,(float).2799,(float).3255,(float).3658,(float).4725,(
	    float).1852,(float).1811,(float).1805,(float).1788,(float).1778,(
	    float).1774,(float).1766,(float).1767,(float).1761,(float).1766,(
	    float).1781,(float).1793,(float).1831,(float).1918,(float).2457,(
	    float).2992,(float).3202,(float).3733,(float).4199,(float).5419,(
	    float).2084,(float).2039,(float).2023,(float).2005,(float).1987,(
	    float).1988,(float).198,(float).1976,(float).1969,(float).1977,(
	    float).1994,(float).201,(float).2057,(float).2159,(float).2796,(
	    float).342,(float).3663,(float).4273,(float).4803,(float).6164,(
	    float).2329,(float).2296,(float).2281,(float).2254,(float).2236,(
	    float).2231,(float).2224,(float).2216,(float).2213,(float).2223,(
	    float).2243,(float).2261,(float).2319,(float).2439,(float).319,(
	    float).3911,(float).419,(float).4884,(float).5479,(float).6963,(
	    float).2629,(float).2581,(float).2569,(float).2536,(float).2526,(
	    float).2511,(float).2503,(float).2497,(float).2496,(float).2507,(
	    float).2533,(float).2554,(float).2624,(float).2767,(float).3648,(
	    float).4476,(float).4793,(float).5574,(float).6235,(float).782,(
	    float).2995,(float).2912,(float).2894,(float).2864,(float).2855,(
	    float).2842,(float).2827,(float).2824,(float).2822,(float).2838,(
	    float).2871,(float).2896,(float).298,(float).3151,(float).4182,(
	    float).5127,(float).5485,(float).6355,(float).7079,(float).8737,(
	    float).3403,(float).3305,(float).3283,(float).3254,(float).3236,(
	    float).3227,(float).3207,(float).3202,(float).3204,(float).3228,(
	    float).3266,(float).3297,(float).3396,(float).3604,(float).4804,(
	    float).5877,(float).6277,(float).7238,(float).8023,(float).9717,(
	    float).3861,(float).3762,(float).3744,(float).3709,(float).3687,(
	    float).367,(float).3649,(float).3643,(float).3655,(float).3686,(
	    float).3731,(float).3767,(float).3887,(float).414,(float).553,(
	    float).6742,(float).7186,(float).8237,(float).9077,(float)1.076,(
	    float).4398,(float).4288,(float).4271,(float).4233,(float).421,(
	    float).4185,(float).4166,(float).4165,(float).4184,(float).4225,(
	    float).4279,(float).4321,(float).4469,(float).4774,(float).6381,(
	    float).774,(float).8229,(float).9366,(float)1.025,(float)1.188,(
	    float).5032,(float).4909,(float).4887,(float).4847,(float).4812,(
	    float).4795,(float).4776,(float).4782,(float).4808,(float).4862,(
	    float).4927,(float).4979,(float).5162,(float).5526,(float).7378,(
	    float).8892,(float).9424,(float)1.064,(float)1.156,(float)1.306,(
	    float).5757,(float).5646,(float).5619,(float).5569,(float).5526,(
	    float).5512,(float).5499,(float).5508,(float).555,(float).5619,(
	    float).5699,(float).5763,(float).5991,(float).6423,(float).8549,(
	    float)1.022,(float)1.08,(float)1.208,(float)1.302,(float)1.432,(
	    float).6589,(float).6497,(float).6477,(float).641,(float).638,(
	    float).6354,(float).6353,(float).6374,(float).6435,(float).6521,(
	    float).6625,(float).6704,(float).6986,(float).7496,(float).9926,(
	    float)1.175,(float)1.237,(float)1.371,(float)1.465,(float)1.565,(
	    float).758,(float).748,(float).7467,(float).7396,(float).7385,(
	    float).7359,(float).7367,(float).7413,(float).7489,(float).7601,(
	    float).7738,(float).7839,(float).8181,(float).8784,(float)1.155,(
	    float)1.352,(float)1.417,(float)1.553,(float)1.645,(float)1.705,(
	    float).8782,(float).8653,(float).8629,(float).8577,(float).8571,(
	    float).8567,(float).8588,(float).8654,(float).8752,(float).8905,(
	    float).9083,(float).9213,(float).9622,(float)1.034,(float)1.346,(
	    float)1.557,(float)1.623,(float)1.759,(float)1.844,(float)1.852,(
	    float)1.017,(float)1.006,(float)1.002,(float).9997,(float).9993,(
	    float)1.002,(float)1.006,(float)1.014,(float)1.028,(float)1.049,(
	    float)1.072,(float)1.088,(float)1.137,(float)1.222,(float)1.571,(
	    float)1.792,(float)1.859,(float)1.99,(float)2.065,(float)2.007,(
	    float)1.175,(float)1.17,(float)1.169,(float)1.169,(float)1.171,(
	    float)1.175,(float)1.182,(float)1.194,(float)1.214,(float)1.243,(
	    float)1.271,(float)1.291,(float)1.349,(float)1.45,(float)1.835,(
	    float)2.063,(float)2.128,(float)2.248,(float)2.307,(float)2.169,(
	    float)1.362,(float)1.364,(float)1.368,(float)1.371,(float)1.379,(
	    float)1.383,(float)1.395,(float)1.414,(float)1.443,(float)1.479,(
	    float)1.515,(float)1.538,(float)1.609,(float)1.727,(float)2.147,(
	    float)2.373,(float)2.435,(float)2.537,(float)2.574,(float)2.337,(
	    float)1.588,(float)1.597,(float)1.604,(float)1.614,(float)1.63,(
	    float)1.637,(float)1.655,(float)1.684,(float)1.723,(float)1.77,(
	    float)1.813,(float)1.842,(float)1.928,(float)2.064,(float)2.512,(
	    float)2.729,(float)2.782,(float)2.857,(float)2.865,(float)2.511,(
	    float)1.853,(float)1.882,(float)1.89,(float)1.912,(float)1.933,(
	    float)1.949,(float)1.976,(float)2.016,(float)2.068,(float)2.127,(
	    float)2.181,(float)2.217,(float)2.319,(float)2.473,(float)2.94,(
	    float)3.134,(float)3.175,(float)3.212,(float)3.181,(float)2.69,(
	    float)2.167,(float)2.227,(float)2.24,(float)2.276,(float)2.306,(
	    float)2.332,(float)2.373,(float)2.428,(float)2.496,(float)2.571,(
	    float)2.636,(float)2.68,(float)2.799,(float)2.971,(float)3.439,(
	    float)3.594,(float)3.616,(float)3.603,(float)3.524,(float)2.873,(
	    float)2.547,(float)2.643,(float)2.667,(float)2.722,(float)2.771,(
	    float)2.808,(float)2.866,(float)2.94,(float)3.027,(float)3.121,(
	    float)3.202,(float)3.253,(float)3.39,(float)3.576,(float)4.02,(
	    float)4.114,(float)4.11,(float)4.031,(float)3.892,(float)3.06,(
	    float)3.006,(float)3.153,(float)3.191,(float)3.274,(float)3.351,(
	    float)3.403,(float)3.483,(float)3.581,(float)3.692,(float)3.808,(
	    float)3.904,(float)3.963,(float)4.114,(float)4.31,(float)4.691,(
	    float)4.697,(float)4.659,(float)4.496,(float)4.285,(float)3.248,(
	    float)3.574,(float)3.794,(float)3.844,(float)3.972,(float)4.081,(
	    float)4.155,(float)4.26,(float)4.386,(float)4.524,(float)4.664,(
	    float)4.774,(float)4.84,(float)5.002,(float)5.196,(float)5.46,(
	    float)5.346,(float)5.264,(float)4.996,(float)4.7,(float)3.436,(
	    float)4.289,(float)4.609,(float)4.679,(float)4.864,(float)5.008,(
	    float)5.107,(float)5.243,(float)5.401,(float)5.57,(float)5.733,(
	    float)5.852,(float)5.923,(float)6.085,(float)6.258,(float)6.334,(
	    float)6.06,(float)5.923,(float)5.53,(float)5.135,(float)3.622,(
	    float)5.202,(float)5.661,(float)5.76,(float)6.012,(float)6.196,(
	    float)6.326,(float)6.496,(float)6.688,(float)6.884,(float)7.062,(
	    float)7.183,(float)7.253,(float)7.399,(float)7.522,(float)7.316,(
	    float)6.837,(float)6.632,(float)6.091,(float)5.583,(float)3.805,(
	    float)6.408,(float)7.047,(float)7.181,(float)7.502,(float)7.737,(
	    float)7.895,(float)8.1,(float)8.32,(float)8.53,(float)8.707,(
	    float)8.817,(float)8.876,(float)8.978,(float)9.008,(float)8.403,(
	    float)7.669,(float)7.384,(float)6.672,(float)6.04,(float)3.98,(
	    float)8.085,(float)8.911,(float)9.079,(float)9.47,(float)9.756,(
	    float)9.932,(float)10.16,(float)10.39,(float)10.58,(float)10.73,(
	    float)10.81,(float)10.84,(float)10.85,(float)10.73,(float)9.584,(
	    float)8.544,(float)8.166,(float)7.262,(float)6.495,(float)4.147,(
	    float)10.5,(float)11.48,(float)11.67,(float)12.1,(float)12.41,(
	    float)12.58,(float)12.79,(float)12.99,(float)13.12,(float)13.19,(
	    float)13.19,(float)13.17,(float)13.03,(float)12.68,(float)10.84,(
	    float)9.442,(float)8.96,(float)7.849,(float)6.94,(float)4.303,(
	    float)14.08,(float)15.08,(float)15.25,(float)15.65,(float)15.89,(
	    float)16.03,(float)16.15,(float)16.22,(float)16.21,(float)16.12,(
	    float)15.98,(float)15.88,(float)15.51,(float)14.84,(float)12.13,(
	    float)10.34,(float)9.743,(float)8.415,(float)7.362,(float)4.443,(
	    float)19.47,(float)20.15,(float)20.22,(float)20.4,(float)20.41,(
	    float)20.43,(float)20.34,(float)20.16,(float)19.88,(float)19.52,(
	    float)19.16,(float)18.93,(float)18.23,(float)17.14,(float)13.4,(
	    float)11.19,(float)10.49,(float)8.941,(float)7.749,(float)4.567,(
	    float)27.55,(float)27.16,(float)27.,(float)26.59,(float)26.14,(
	    float)25.87,(float)25.41,(float)24.78,(float)24.05,(float)23.28,(
	    float)22.62,(float)22.2,(float)21.06,(float)19.45,(float)14.6,(
	    float)11.97,(float)11.15,(float)9.406,(float)8.085,(float)4.671,(
	    float)39.18,(float)36.33,(float)35.73,(float)34.17,(float)32.96,(
	    float)32.18,(float)31.11,(float)29.83,(float)28.47,(float)27.14,(
	    float)26.09,(float)25.43,(float)23.79,(float)21.6,(float)15.63,(
	    float)12.62,(float)11.71,(float)9.788,(float)8.359,(float)4.753,(
	    float)54.06,(float)46.92,(float)45.58,(float)42.34,(float)40.09,(
	    float)38.6,(float)36.73,(float)34.64,(float)32.56,(float)30.59,(
	    float)29.13,(float)28.24,(float)26.09,(float)23.35,(float)16.42,(
	    float)13.11,(float)12.13,(float)10.07,(float)8.557,(float)4.811,(
	    float)68.07,(float)55.89,(float)53.77,(float)48.85,(float)45.6,(
	    float)43.45,(float)40.85,(float)38.07,(float)35.4,(float)32.92,(
	    float)31.15,(float)30.07,(float)27.56,(float)24.44,(float)16.89,(
	    float)13.4,(float)12.37,(float)10.23,(float)8.671,(float)4.844,(
	    float)72.5,(float)58.54,(float)56.16,(float)50.7,(float)47.14,(
	    float)44.78,(float)41.96,(float)38.99,(float)36.14,(float)33.52,(
	    float)31.67,(float)30.54,(float)27.93,(float)24.71,(float)17.,(
	    float)13.46,(float)12.42,(float)10.27,(float)8.698,(float)4.852 };

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
/* Aerosol stratospheric model */
/* Parameters (STM-params.dat) are calculated with PINATUBO.f */
/* for one-year-old aerosol. PINATUBO.f is written based on */
/* Russel et al., 'Global to microscale evolution of the Pinatubo */
/* volcanic aerosol derived from diverse measurements and analyses', */
/* Journal of Geophysical Research, 101(D13), pp. 18745-18763, 1996. */
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
} /* stm_ */

#ifdef __cplusplus
	}
#endif
