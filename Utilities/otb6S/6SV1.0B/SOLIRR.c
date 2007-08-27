/* SOLIRR.f -- translated by f2c (version 19970805).
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
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

/*<       subroutine solirr (wl,swl) >*/
/* Subroutine */ int solirr_(real *wl, real *swl)
{
    /* Initialized data */

    static real si[1501] = { (float)69.3,(float)77.65,(float)86.,(float)
	    100.06,(float)114.12,(float)137.06,(float)160.,(float)169.52,(
	    float)179.04,(float)178.02,(float)177.,(float)193.69,(float)
	    210.38,(float)241.69,(float)273.,(float)318.42,(float)363.84,(
	    float)434.42,(float)505.,(float)531.5,(float)558.,(float)547.5,(
	    float)537.,(float)559.02,(float)581.03,(float)619.52,(float)658.,(
	    float)694.39,(float)730.78,(float)774.39,(float)817.99,(float)
	    871.99,(float)925.99,(float)912.04,(float)898.09,(float)920.69,(
	    float)943.29,(float)925.99,(float)908.69,(float)936.09,(float)
	    963.49,(float)994.94,(float)1026.39,(float)980.74,(float)935.09,(
	    float)1036.29,(float)1137.49,(float)1163.74,(float)1189.99,(float)
	    1109.34,(float)1028.69,(float)1088.99,(float)1149.29,(float)
	    1033.69,(float)918.09,(float)1031.89,(float)1145.69,(float)
	    1035.09,(float)924.49,(float)1269.29,(float)1614.09,(float)
	    1631.09,(float)1648.09,(float)1677.19,(float)1706.29,(float)
	    1744.89,(float)1783.49,(float)1750.19,(float)1716.89,(float)
	    1705.19,(float)1693.49,(float)1597.69,(float)1501.89,(float)
	    1630.99,(float)1760.09,(float)1775.24,(float)1790.39,(float)
	    1859.94,(float)1929.49,(float)1993.44,(float)2057.39,(float)
	    2039.23,(float)2021.08,(float)2030.73,(float)2040.38,(float)
	    2026.53,(float)2012.68,(float)1999.53,(float)1986.38,(float)
	    2002.88,(float)2019.38,(float)2038.09,(float)2056.79,(float)
	    1967.74,(float)1878.68,(float)1905.83,(float)1932.98,(float)
	    1953.58,(float)1974.18,(float)1935.68,(float)1897.19,(float)
	    1916.78,(float)1936.38,(float)1937.23,(float)1938.09,(float)
	    1881.44,(float)1824.79,(float)1814.09,(float)1803.39,(float)
	    1832.24,(float)1861.09,(float)1885.93,(float)1910.78,(float)
	    1904.68,(float)1898.58,(float)1875.73,(float)1852.88,(float)
	    1865.64,(float)1878.39,(float)1874.74,(float)1871.09,(float)
	    1872.44,(float)1873.79,(float)1850.39,(float)1826.99,(float)
	    1837.04,(float)1847.09,(float)1841.18,(float)1835.28,(float)
	    1849.48,(float)1863.69,(float)1851.03,(float)1838.38,(float)
	    1840.73,(float)1843.08,(float)1802.83,(float)1762.58,(float)
	    1778.78,(float)1794.99,(float)1777.48,(float)1759.98,(float)
	    1764.73,(float)1769.49,(float)1753.48,(float)1737.48,(float)
	    1713.14,(float)1688.8,(float)1702.88,(float)1716.97,(float)
	    1696.07,(float)1675.17,(float)1672.03,(float)1668.89,(float)
	    1663.56,(float)1658.23,(float)1647.75,(float)1637.27,(float)
	    1630.02,(float)1622.77,(float)1606.06,(float)1589.36,(float)
	    1552.29,(float)1515.22,(float)1528.91,(float)1542.6,(float)1548.9,
	    (float)1555.21,(float)1544.41,(float)1533.62,(float)1525.24,(
	    float)1516.86,(float)1507.92,(float)1498.98,(float)1484.07,(float)
	    1469.17,(float)1464.28,(float)1459.39,(float)1448.73,(float)
	    1438.08,(float)1423.16,(float)1408.24,(float)1407.53,(float)
	    1406.82,(float)1397.82,(float)1388.82,(float)1378.51,(float)
	    1368.21,(float)1352.13,(float)1336.05,(float)1343.88,(float)
	    1351.71,(float)1339.6,(float)1327.5,(float)1320.72,(float)1313.94,
	    (float)1294.94,(float)1275.94,(float)1280.92,(float)1285.9,(float)
	    1278.04,(float)1270.19,(float)1263.68,(float)1257.18,(float)
	    1249.8,(float)1242.41,(float)1231.3,(float)1220.19,(float)1212.14,
	    (float)1204.1,(float)1201.69,(float)1199.29,(float)1194.78,(float)
	    1190.27,(float)1185.47,(float)1180.68,(float)1174.38,(float)
	    1168.09,(float)1156.17,(float)1144.26,(float)1143.46,(float)
	    1142.67,(float)1132.95,(float)1123.23,(float)1116.71,(float)
	    1110.19,(float)1110.89,(float)1111.59,(float)1094.8,(float)
	    1078.01,(float)1077.75,(float)1077.49,(float)1073.89,(float)
	    1070.29,(float)1058.71,(float)1047.13,(float)1045.66,(float)
	    1044.2,(float)1037.03,(float)1029.86,(float)1010.4,(float)990.94,(
	    float)966.91,(float)942.89,(float)972.87,(float)1002.86,(float)
	    978.93,(float)955.,(float)960.95,(float)966.91,(float)983.31,(
	    float)999.71,(float)991.91,(float)984.11,(float)979.05,(float)
	    973.99,(float)968.79,(float)963.6,(float)958.23,(float)952.87,(
	    float)947.93,(float)942.99,(float)937.99,(float)933.,(float)928.,(
	    float)923.,(float)918.18,(float)913.37,(float)908.74,(float)
	    904.11,(float)899.05,(float)893.99,(float)889.18,(float)884.37,(
	    float)879.74,(float)875.12,(float)870.24,(float)865.36,(float)
	    860.94,(float)856.53,(float)852.02,(float)847.5,(float)843.,(
	    float)838.5,(float)833.99,(float)829.49,(float)824.98,(float)
	    820.48,(float)815.99,(float)811.5,(float)806.99,(float)802.49,(
	    float)798.17,(float)793.86,(float)789.74,(float)785.63,(float)
	    781.25,(float)776.87,(float)772.92,(float)768.98,(float)764.8,(
	    float)760.63,(float)756.06,(float)751.49,(float)746.99,(float)
	    742.49,(float)738.18,(float)733.88,(float)729.76,(float)725.63,(
	    float)721.24,(float)716.86,(float)712.92,(float)708.99,(float)
	    704.81,(float)700.63,(float)696.25,(float)691.87,(float)687.94,(
	    float)684.01,(float)680.01,(float)676.,(float)671.8,(float)667.61,
	    (float)663.23,(float)658.86,(float)655.32,(float)651.77,(float)
	    649.07,(float)646.37,(float)643.74,(float)641.11,(float)638.05,(
	    float)634.99,(float)632.18,(float)629.37,(float)626.74,(float)
	    624.12,(float)621.06,(float)618.,(float)615.18,(float)612.37,(
	    float)609.92,(float)607.48,(float)604.79,(float)602.11,(float)
	    599.24,(float)596.38,(float)593.93,(float)591.48,(float)588.79,(
	    float)586.11,(float)583.25,(float)580.4,(float)577.94,(float)
	    575.48,(float)572.99,(float)570.51,(float)568.,(float)565.49,(
	    float)562.98,(float)560.47,(float)557.98,(float)555.5,(float)
	    553.01,(float)550.51,(float)548.,(float)545.49,(float)542.98,(
	    float)540.48,(float)537.98,(float)535.49,(float)533.19,(float)
	    530.9,(float)528.94,(float)526.99,(float)524.8,(float)522.62,(
	    float)520.24,(float)517.87,(float)515.44,(float)513.01,(float)
	    509.59,(float)506.17,(float)502.89,(float)499.62,(float)496.35,(
	    float)493.09,(float)489.81,(float)486.54,(float)483.27,(float)
	    480.01,(float)476.73,(float)473.46,(float)470.19,(float)466.92,(
	    float)463.64,(float)460.37,(float)457.1,(float)453.84,(float)
	    450.57,(float)447.3,(float)444.03,(float)440.76,(float)437.48,(
	    float)434.21,(float)430.94,(float)427.67,(float)424.4,(float)
	    421.13,(float)417.86,(float)414.59,(float)411.32,(float)408.05,(
	    float)404.78,(float)401.51,(float)398.24,(float)394.97,(float)
	    391.7,(float)388.43,(float)392.57,(float)396.71,(float)401.92,(
	    float)407.14,(float)405.32,(float)403.5,(float)401.67,(float)
	    399.84,(float)398.02,(float)396.21,(float)394.37,(float)392.54,(
	    float)390.72,(float)388.9,(float)387.06,(float)385.23,(float)
	    383.42,(float)381.6,(float)379.77,(float)377.95,(float)376.12,(
	    float)374.3,(float)372.48,(float)370.66,(float)368.82,(float)
	    366.99,(float)365.17,(float)363.35,(float)361.52,(float)359.69,(
	    float)357.87,(float)356.05,(float)354.22,(float)352.39,(float)
	    350.57,(float)348.75,(float)346.92,(float)345.1,(float)343.27,(
	    float)341.45,(float)341.84,(float)342.24,(float)342.95,(float)
	    343.66,(float)342.27,(float)340.89,(float)339.49,(float)338.09,(
	    float)336.69,(float)335.3,(float)333.91,(float)332.53,(float)
	    331.13,(float)329.73,(float)328.34,(float)326.96,(float)325.56,(
	    float)324.16,(float)322.77,(float)321.39,(float)319.99,(float)
	    318.59,(float)317.2,(float)315.82,(float)314.42,(float)313.03,(
	    float)311.63,(float)310.24,(float)308.85,(float)307.46,(float)
	    306.06,(float)304.66,(float)303.28,(float)301.9,(float)300.5,(
	    float)299.1,(float)297.71,(float)296.32,(float)294.93,(float)
	    293.54,(float)293.41,(float)293.28,(float)293.35,(float)293.42,(
	    float)292.26,(float)291.1,(float)289.97,(float)288.84,(float)
	    287.69,(float)286.54,(float)285.39,(float)284.25,(float)283.1,(
	    float)281.96,(float)280.81,(float)279.67,(float)278.52,(float)
	    277.38,(float)276.23,(float)275.08,(float)273.94,(float)272.8,(
	    float)271.65,(float)270.51,(float)269.36,(float)268.22,(float)
	    267.07,(float)265.93,(float)264.78,(float)263.64,(float)262.49,(
	    float)261.34,(float)260.2,(float)259.06,(float)257.91,(float)
	    256.77,(float)255.62,(float)254.47,(float)253.33,(float)252.2,(
	    float)251.16,(float)250.13,(float)249.11,(float)248.09,(float)
	    246.97,(float)245.86,(float)244.74,(float)243.61,(float)242.49,(
	    float)241.37,(float)240.24,(float)239.12,(float)238.,(float)
	    236.89,(float)235.76,(float)234.64,(float)233.51,(float)232.38,(
	    float)231.26,(float)230.13,(float)229.01,(float)227.9,(float)
	    226.77,(float)225.65,(float)224.53,(float)223.42,(float)222.29,(
	    float)221.16,(float)220.04,(float)218.92,(float)217.8,(float)
	    216.68,(float)215.55,(float)214.43,(float)213.3,(float)212.18,(
	    float)211.06,(float)209.94,(float)208.82,(float)207.69,(float)
	    206.99,(float)206.29,(float)205.65,(float)205.02,(float)203.98,(
	    float)202.95,(float)201.9,(float)200.85,(float)199.81,(float)
	    198.78,(float)197.74,(float)196.7,(float)195.65,(float)194.61,(
	    float)193.57,(float)192.54,(float)191.5,(float)190.47,(float)
	    189.42,(float)188.37,(float)187.33,(float)186.3,(float)185.26,(
	    float)184.22,(float)183.18,(float)182.14,(float)181.1,(float)
	    180.06,(float)179.02,(float)177.98,(float)176.93,(float)175.89,(
	    float)174.86,(float)173.83,(float)172.78,(float)171.73,(float)
	    170.7,(float)169.67,(float)168.62,(float)167.57,(float)167.59,(
	    float)167.6,(float)167.76,(float)167.93,(float)167.09,(float)
	    166.26,(float)165.42,(float)164.58,(float)163.75,(float)162.92,(
	    float)162.08,(float)161.25,(float)160.41,(float)159.58,(float)
	    158.74,(float)157.91,(float)157.07,(float)156.24,(float)155.4,(
	    float)154.57,(float)153.73,(float)152.9,(float)152.06,(float)
	    151.23,(float)150.39,(float)149.56,(float)148.72,(float)147.89,(
	    float)147.06,(float)146.23,(float)145.39,(float)144.55,(float)
	    143.71,(float)142.88,(float)142.05,(float)141.22,(float)140.38,(
	    float)139.54,(float)138.7,(float)137.86,(float)137.99,(float)
	    138.11,(float)138.36,(float)138.6,(float)137.94,(float)137.29,(
	    float)136.64,(float)136.,(float)135.35,(float)134.71,(float)
	    134.05,(float)133.39,(float)132.74,(float)132.09,(float)131.45,(
	    float)130.81,(float)130.15,(float)129.49,(float)128.84,(float)
	    128.2,(float)127.55,(float)126.9,(float)126.25,(float)125.6,(
	    float)124.94,(float)124.29,(float)123.64,(float)123.,(float)
	    122.35,(float)121.7,(float)121.05,(float)120.4,(float)119.74,(
	    float)119.09,(float)118.45,(float)117.81,(float)117.15,(float)
	    116.5,(float)115.85,(float)115.19,(float)115.25,(float)115.31,(
	    float)115.46,(float)115.62,(float)115.11,(float)114.6,(float)
	    114.09,(float)113.58,(float)113.06,(float)112.54,(float)112.03,(
	    float)111.53,(float)111.01,(float)110.5,(float)109.99,(float)
	    109.47,(float)108.95,(float)108.44,(float)107.93,(float)107.42,(
	    float)106.92,(float)106.42,(float)105.89,(float)105.37,(float)
	    104.85,(float)104.34,(float)103.83,(float)103.33,(float)102.81,(
	    float)102.29,(float)101.79,(float)101.29,(float)100.77,(float)
	    100.25,(float)99.74,(float)99.22,(float)98.71,(float)98.2,(float)
	    97.69,(float)97.18,(float)97.12,(float)97.07,(float)97.09,(float)
	    97.11,(float)96.68,(float)96.26,(float)95.84,(float)95.42,(float)
	    94.99,(float)94.56,(float)94.14,(float)93.72,(float)93.31,(float)
	    92.89,(float)92.46,(float)92.03,(float)91.61,(float)91.19,(float)
	    90.76,(float)90.34,(float)89.92,(float)89.49,(float)89.07,(float)
	    88.66,(float)88.24,(float)87.81,(float)87.39,(float)86.97,(float)
	    86.55,(float)86.12,(float)85.69,(float)85.26,(float)84.85,(float)
	    84.43,(float)84.01,(float)83.59,(float)83.17,(float)82.75,(float)
	    82.32,(float)81.89,(float)81.89,(float)81.89,(float)81.95,(float)
	    82.02,(float)81.68,(float)81.35,(float)81.,(float)80.65,(float)
	    80.32,(float)79.99,(float)79.64,(float)79.3,(float)78.96,(float)
	    78.61,(float)78.27,(float)77.94,(float)77.6,(float)77.26,(float)
	    76.91,(float)76.57,(float)76.24,(float)75.9,(float)75.56,(float)
	    75.22,(float)74.88,(float)74.54,(float)74.2,(float)73.86,(float)
	    73.52,(float)73.18,(float)72.84,(float)72.5,(float)72.16,(float)
	    71.82,(float)71.48,(float)71.14,(float)70.8,(float)70.47,(float)
	    70.13,(float)69.79,(float)69.76,(float)69.73,(float)69.76,(float)
	    69.8,(float)69.52,(float)69.24,(float)68.96,(float)68.68,(float)
	    68.41,(float)68.14,(float)67.85,(float)67.57,(float)67.29,(float)
	    67.02,(float)66.75,(float)66.48,(float)66.19,(float)65.9,(float)
	    65.63,(float)65.36,(float)65.08,(float)64.8,(float)64.53,(float)
	    64.25,(float)63.97,(float)63.69,(float)63.41,(float)63.14,(float)
	    62.85,(float)62.57,(float)62.3,(float)62.03,(float)61.75,(float)
	    61.47,(float)61.19,(float)60.92,(float)60.64,(float)60.36,(float)
	    60.08,(float)59.81,(float)59.8,(float)59.8,(float)59.82,(float)
	    59.85,(float)59.63,(float)59.4,(float)59.17,(float)58.95,(float)
	    58.73,(float)58.5,(float)58.28,(float)58.06,(float)57.83,(float)
	    57.6,(float)57.37,(float)57.15,(float)56.93,(float)56.7,(float)
	    56.48,(float)56.26,(float)56.03,(float)55.79,(float)55.57,(float)
	    55.36,(float)55.13,(float)54.9,(float)54.66,(float)54.43,(float)
	    54.22,(float)54.,(float)53.77,(float)53.55,(float)53.32,(float)
	    53.09,(float)52.87,(float)52.65,(float)52.43,(float)52.2,(float)
	    51.97,(float)51.75,(float)51.72,(float)51.68,(float)51.67,(float)
	    51.67,(float)51.48,(float)51.3,(float)51.11,(float)50.92,(float)
	    50.73,(float)50.55,(float)50.37,(float)50.18,(float)49.98,(float)
	    49.79,(float)49.61,(float)49.43,(float)49.23,(float)49.04,(float)
	    48.85,(float)48.67,(float)48.48,(float)48.3,(float)48.12,(float)
	    47.93,(float)47.73,(float)47.54,(float)47.36,(float)47.18,(float)
	    46.98,(float)46.79,(float)46.6,(float)46.42,(float)46.24,(float)
	    46.06,(float)45.87,(float)45.67,(float)45.48,(float)45.3,(float)
	    45.12,(float)44.93,(float)44.87,(float)44.82,(float)44.8,(float)
	    44.79,(float)44.62,(float)44.45,(float)44.29,(float)44.14,(float)
	    43.98,(float)43.83,(float)43.66,(float)43.49,(float)43.34,(float)
	    43.18,(float)43.02,(float)42.86,(float)42.7,(float)42.55,(float)
	    42.38,(float)42.21,(float)42.06,(float)41.9,(float)41.74,(float)
	    41.58,(float)41.42,(float)41.26,(float)41.1,(float)40.94,(float)
	    40.78,(float)40.62,(float)40.46,(float)40.31,(float)40.14,(float)
	    39.97,(float)39.81,(float)39.66,(float)39.5,(float)39.34,(float)
	    39.18,(float)39.03,(float)38.99,(float)38.96,(float)38.94,(float)
	    38.92,(float)38.79,(float)38.66,(float)38.52,(float)38.38,(float)
	    38.25,(float)38.12,(float)37.99,(float)37.86,(float)37.72,(float)
	    37.58,(float)37.44,(float)37.3,(float)37.17,(float)37.05,(float)
	    36.91,(float)36.77,(float)36.64,(float)36.5,(float)36.36,(float)
	    36.23,(float)36.09,(float)35.96,(float)35.82,(float)35.69,(float)
	    35.55,(float)35.42,(float)35.28,(float)35.15,(float)35.01,(float)
	    34.88,(float)34.75,(float)34.61,(float)34.47,(float)34.34,(float)
	    34.2,(float)34.07,(float)34.05,(float)34.03,(float)34.03,(float)
	    34.03,(float)33.91,(float)33.79,(float)33.68,(float)33.57,(float)
	    33.46,(float)33.35,(float)33.23,(float)33.12,(float)33.01,(float)
	    32.9,(float)32.78,(float)32.67,(float)32.55,(float)32.44,(float)
	    32.33,(float)32.23,(float)32.11,(float)32.,(float)31.89,(float)
	    31.77,(float)31.66,(float)31.55,(float)31.43,(float)31.31,(float)
	    31.2,(float)31.1,(float)30.99,(float)30.87,(float)30.76,(float)
	    30.66,(float)30.54,(float)30.42,(float)30.31,(float)30.2,(float)
	    30.08,(float)29.97,(float)29.93,(float)29.9,(float)29.88,(float)
	    29.87,(float)29.76,(float)29.66,(float)29.56,(float)29.46,(float)
	    29.36,(float)29.27,(float)29.17,(float)29.08,(float)28.98,(float)
	    28.88,(float)28.77,(float)28.67,(float)28.58,(float)28.49,(float)
	    28.39,(float)28.3,(float)28.2,(float)28.1,(float)28.,(float)27.91,
	    (float)27.81,(float)27.71,(float)27.61,(float)27.52,(float)27.41,(
	    float)27.31,(float)27.21,(float)27.12,(float)27.03,(float)26.93,(
	    float)26.83,(float)26.74,(float)26.64,(float)26.54,(float)26.44,(
	    float)26.35,(float)26.33,(float)26.31,(float)26.29,(float)26.28,(
	    float)26.2,(float)26.12,(float)26.04,(float)25.95,(float)25.87,(
	    float)25.79,(float)25.71,(float)25.64,(float)25.54,(float)25.45,(
	    float)25.37,(float)25.3,(float)25.21,(float)25.12,(float)25.05,(
	    float)24.98,(float)24.89,(float)24.8,(float)24.71,(float)24.63,(
	    float)24.55,(float)24.47,(float)24.39,(float)24.31,(float)24.22,(
	    float)24.14,(float)24.05,(float)23.97,(float)23.89,(float)23.81,(
	    float)23.73,(float)23.66,(float)23.56,(float)23.47,(float)23.39,(
	    float)23.31,(float)23.28,(float)23.26,(float)23.23,(float)23.21,(
	    float)23.13,(float)23.06,(float)22.99,(float)22.92,(float)22.84,(
	    float)22.76,(float)22.69,(float)22.63,(float)22.55,(float)22.47,(
	    float)22.41,(float)22.35,(float)22.27,(float)22.19,(float)22.11,(
	    float)22.04,(float)21.97,(float)21.9,(float)21.83,(float)21.76,(
	    float)21.68,(float)21.6,(float)21.53,(float)21.47,(float)21.39,(
	    float)21.31,(float)21.24,(float)21.18,(float)21.11,(float)21.03,(
	    float)20.96,(float)20.89,(float)20.81,(float)20.73,(float)20.66,(
	    float)20.6,(float)20.57,(float)20.55,(float)20.54,(float)20.53,(
	    float)20.46,(float)20.4,(float)20.34,(float)20.28,(float)20.21,(
	    float)20.14,(float)20.08,(float)20.03,(float)19.96,(float)19.9,(
	    float)19.83,(float)19.77,(float)19.71,(float)19.65,(float)19.59,(
	    float)19.53,(float)19.46,(float)19.39,(float)19.33,(float)19.27,(
	    float)19.21,(float)19.15,(float)19.08,(float)19.02,(float)18.96,(
	    float)18.9,(float)18.84,(float)18.78,(float)18.71,(float)18.64,(
	    float)18.58,(float)18.53,(float)18.46,(float)18.4,(float)18.33,(
	    float)18.27,(float)18.26,(float)18.25,(float)18.24,(float)18.24,(
	    float)18.19,(float)18.14,(float)18.08,(float)18.03,(float)17.98,(
	    float)17.93,(float)17.88,(float)17.83,(float)17.77,(float)17.71,(
	    float)17.66,(float)17.62,(float)17.56,(float)17.5,(float)17.45,(
	    float)17.41,(float)17.35,(float)17.29,(float)17.25,(float)17.21,(
	    float)17.14,(float)17.08,(float)17.04,(float)17.,(float)16.93,(
	    float)16.87,(float)16.83,(float)16.79,(float)16.72,(float)16.66,(
	    float)16.61,(float)16.57,(float)16.51,(float)16.46,(float)16.41,(
	    float)16.36,(float)16.34,(float)16.33,(float)16.31,(float)16.3,(
	    float)16.26,(float)16.22,(float)16.17,(float)16.13,(float)16.08,(
	    float)16.04,(float)16.,(float)15.96,(float)15.9,(float)15.84,(
	    float)15.81,(float)15.78,(float)15.73,(float)15.68,(float)15.63,(
	    float)15.59,(float)15.55,(float)15.5,(float)15.45,(float)15.4,(
	    float)15.36,(float)15.32,(float)15.28,(float)15.24,(float)15.18,(
	    float)15.13,(float)15.09,(float)15.05,(float)15.01,(float)14.96,(
	    float)14.91,(float)14.87,(float)14.82,(float)14.78,(float)14.73,(
	    float)14.69,(float)14.66,(float)14.64,(float)14.64,(float)14.63,(
	    float)14.59,(float)14.55,(float)14.5,(float)14.45,(float)14.41,(
	    float)14.38,(float)14.35,(float)14.32,(float)14.26,(float)14.21,(
	    float)14.18,(float)14.15,(float)14.1,(float)14.05,(float)14.01,(
	    float)13.98,(float)13.94,(float)13.91,(float)13.86,(float)13.82,(
	    float)13.78,(float)13.74,(float)13.7,(float)13.67,(float)13.62,(
	    float)13.58,(float)13.54,(float)13.5,(float)13.46,(float)13.43,(
	    float)13.39,(float)13.35,(float)13.3,(float)13.25,(float)13.22,(
	    float)13.18,(float)13.17,(float)13.16,(float)13.14,(float)13.12,(
	    float)13.09,(float)13.06,(float)13.03,(float)13.,(float)12.96,(
	    float)12.92,(float)12.89,(float)12.85,(float)12.81,(float)12.78,(
	    float)12.74,(float)12.7,(float)12.67,(float)12.65,(float)12.61,(
	    float)12.57,(float)12.53,(float)12.5,(float)12.46,(float)12.43,(
	    float)12.39,(float)12.36,(float)12.32,(float)12.28,(float)12.25,(
	    float)12.22,(float)12.18,(float)12.15,(float)12.11,(float)12.07,(
	    float)12.04,(float)12.01,(float)11.97,(float)11.94,(float)11.9,(
	    float)11.86,(float)11.85,(float)11.85,(float)11.85,(float)11.84,(
	    float)11.81,(float)11.78,(float)11.75,(float)11.72,(float)11.69,(
	    float)11.66,(float)11.63,(float)11.6,(float)11.58,(float)11.55,(
	    float)11.51,(float)11.47,(float)11.45,(float)11.42,(float)11.39,(
	    float)11.36,(float)11.33,(float)11.3,(float)11.27,(float)11.24,(
	    float)11.21,(float)11.18,(float)11.15,(float)11.12,(float)11.09,(
	    float)11.06,(float)11.03,(float)11.,(float)10.97,(float)10.94,(
	    float)10.91,(float)10.89,(float)10.85,(float)10.82,(float)10.78,(
	    float)10.75,(float)10.73,(float)10.72,(float)10.71,(float)10.7,(
	    float)10.67,(float)10.64,(float)10.62,(float)10.59,(float)10.55,(
	    float)10.52,(float)10.5,(float)10.47,(float)10.44,(float)10.42,(
	    float)10.39,(float)10.37,(float)10.34,(float)10.31,(float)10.28,(
	    float)10.25,(float)10.22,(float)10.2,(float)10.17,(float)10.15,(
	    float)10.12,(float)10.1,(float)10.06,(float)10.03,(float)10.,(
	    float)9.98,(float)9.95,(float)9.92,(float)9.89,(float)9.86,(float)
	    9.84,(float)9.82,(float)9.79,(float)9.75,(float)9.73,(float)9.71,(
	    float)9.7,(float)9.7,(float)9.7,(float)9.7,(float)9.67,(float)
	    9.63,(float)9.61,(float)9.59,(float)9.58,(float)9.56,(float)9.53,(
	    float)9.5,(float)9.48,(float)9.45,(float)9.43,(float)9.41,(float)
	    9.39,(float)9.36,(float)9.34,(float)9.32,(float)9.3,(float)9.27,(
	    float)9.24,(float)9.22,(float)9.2,(float)9.18,(float)9.15,(float)
	    9.13,(float)9.11,(float)9.08,(float)9.06,(float)9.05,(float)9.02,(
	    float)8.99,(float)8.96,(float)8.94,(float)8.92,(float)8.9,(float)
	    8.87,(float)8.85,(float)8.83,(float)8.81 };

    /* Format strings */
    static char fmt_20[] = "(\002      wavelength  less  than  0.25  micron \
 :\002,/,\002let's take s(l)=s(0.25)\002)";

    /* Builtin functions */
    integer s_wsfe(cilist *), e_wsfe();

    /* Local variables */
    real pas;
    integer iwl;

    /* Fortran I/O blocks */
    static cilist io___4 = { 0, 0, 0, fmt_20, 0 };


/*     si (in w/m2/micron) contains the values of the solar */
/*     irradiance between 0.25 and 4.0 microns, by step of 0.0025 m. */
/*     the value of the total irradiance is 1372 w/m2 on the whole */
/*     spectrum (1358 w/m2 between 0.25 and 4.0 microns) */

/*<       dimension si(1501) >*/
/*<       logical ier >*/
/*<       real wl,swl,si,pas >*/
/*<       integer iwr,i,iwl >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       pas=0.0025 >*/
    pas = (float).0025;
/*<       iwl=(wl-0.250)/pas+1.5 >*/
    iwl = (*wl - (float).25) / pas + (float)1.5;
/*<       if(iwl.lt.0) goto 10 >*/
    if (iwl < 0) {
	goto L10;
    }
/*<       swl=si(iwl) >*/
    *swl = si[iwl - 1];
/*<       return >*/
    return 0;
/*< 10    write(iwr, 20) >*/
L10:
    io___4.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___4);
    e_wsfe();
/*<       swl=si(1) >*/
    *swl = si[0];
/*< 20 >*/
/*<       return >*/
    return 0;
/*<       end >*/
} /* solirr_ */

#ifdef __cplusplus
	}
#endif
