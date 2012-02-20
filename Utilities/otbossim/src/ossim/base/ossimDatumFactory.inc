//*************************************************************************
// LGPL
// 
// Author:  Garrett Potts
//
//**************************************************************************
// $Id: ossimDatumFactory.inc 16414 2010-01-26 18:10:18Z gpotts $

struct ossimSevenParamDatumType
{
   const char* theCode;
   const char* theName;
   const char* theEllipsoidCode;
   ossim_float64 theSigmaX;
   ossim_float64 theSigmaY;
   ossim_float64 theSigmaZ;
   ossim_float64 theWestLongitude;
   ossim_float64 theEastLongitude;
   ossim_float64 theSouthLatitude;
   ossim_float64 theNorthLatitude;
   ossim_float64 theParam1;
   ossim_float64 theParam2;
   ossim_float64 theParam3;
   ossim_float64 theParam4;
   ossim_float64 theParam5;
   ossim_float64 theParam6;
   ossim_float64 theParam7;
};

struct ossimThreeParamDatumType
{
public:
   const char* theCode;
   const char* theName;
   const char* theEllipsoidCode;
   ossim_float64 theSigmaX;
   ossim_float64 theSigmaY;
   ossim_float64 theSigmaZ;
   ossim_float64 theWestLongitude;
   ossim_float64 theEastLongitude;
   ossim_float64 theSouthLatitude;
   ossim_float64 theNorthLatitude;
   ossim_float64 theParam1;
   ossim_float64 theParam2;
   ossim_float64 theParam3;
};

#define NUMBER_OF_SEVEN_PARAM_DATUMS 2
#define NUMBER_OF_THREE_PARAM_DATUMS 226

static ossimThreeParamDatumType threeParamDatum[] = {
{"ADI-A", "ADINDAN, Ethiopia", "CD", 3, 3, 3, 26, 50, -3, 25, -165, -11, 206},
{"ADI-B", "ADINDAN, Sudan", "CD", 3, 5, 3, 15, 45, -3, 31, -161, -14, 205},
{"ADI-C", "ADINDAN, Mali", "CD", 25, 25, 25, -20, 11, 3, 31, -123, -20, 220},
{"ADI-D", "ADINDAN, Senegal", "CD", 25, 25, 25, -24, -5, 5, 23, -128, -18, 224},
{"ADI-E", "ADINDAN, Burkina Faso", "CD", 25, 25, 25, -5, 8, 4, 22, -118, -14, 218},
{"ADI-F", "ADINDAN, Cameroon", "CD", 25, 25, 25, 3, 23, -4, 19, -134, -2, 210},
{"ADI-M", "ADINDAN, Mean", "CD", 5, 5, 3, 15, 55, -5, 31, -166, -15, 204},
{"AFG", "AFGOOYE, Somalia", "KA", 25, 25, 25, 35, 60, -8, 19, -43, -163, 45},
{"AIA", "ANTIGUA ISLAND ASTRO 1943", "CD", 25, 25, 25, -65, -61, 16, 20, -270, 13, 62},
{"AIN-A", "AIN EL ABD 1970, Bahrain", "IN", 25, 25, 25, 49, 53, 24, 28, -150, -250, -1},
{"AIN-B", "AIN EL ABD 1970, Saudi Arabia", "IN", 10, 10, 10, 28, 62, 8, 38, -143, -236, 7},
{"AMA", "AMERICAN SAMOA 1962", "CC", 25, 25, 25, -174, -165, -19, -9, -115, 118, 426},
{"ANO", "ANNA 1 ASTRO 1965, Cocos Is.", "AN", 25, 25, 25, 94, 99, -14, -10, -491, -22, 435},
{"ARF-A", "ARC 1950, Botswana", "CD", 3, 5, 3, 13, 36, -33, -13, -138, -105, -289},
{"ARF-B", "ARC 1950, Lesotho", "CD", 3, 3, 8, 21, 35, -36, -23, -125, -108, -295},
{"ARF-C", "ARC 1950, Malawi", "CD", 9, 24, 8, 26, 42, -21, -3, -161, -73, -317},
{"ARF-D", "ARC 1950, Swaziland", "CD", 15, 15, 15, 25, 40, -33, -20, -134, -105, -295},
{"ARF-E", "ARC 1950, Zaire", "CD", 25, 25, 25, 4, 38, -21, 10, -169, -19, -278},
{"ARF-F", "ARC 1950, Zambia", "CD", 21, 21, 27, 15, 40, -24, -1, -147, -74, -283},
{"ARF-G", "ARC 1950, Zimbabwe", "CD", 5, 8, 11, 19, 39, -29, -9, -142, -96, -293},
{"ARF-H", "ARC 1950, Burundi", "CD", 20, 20, 20, 21, 37, -11, 4, -153, -5, -292},
{"ARF-M", "ARC 1950, Mean", "CD", 20, 33, 20, 4, 42, -36, 10, -143, -90, -294},
{"ARS-A", "ARC 1960, Kenya", "CD", 4, 3, 3, 28, 47, -11, 8, -157, -2, -299},
{"ARS-B", "ARC 1960, Tanzania", "CD", 6, 9, 10, 23, 47, -18, 5, -175, -23, -303},
{"ARS-M", "ARC 1960, Kenya & Tanzania", "CD", 20, 20, 20, 23, 47, -18, 8, -160, -6, -302},
{"ASC", "ASCENSION ISLAND 1958", "IN", 25, 25, 25, -16, -13, -9, -6, -205, 107, 53},
{"ASM", "MONTSERRAT ISLAND ASTRO 1958", "CD", 25, 25, 25, -64, -61, 15, 18, 174, 359, 365},
{"ASQ", "ASTRO STATION 1952, Marcus Is.", "IN", 25, 25, 25, 152, 156, 22, 26, 124, -234, -25},
{"ATF", "ASTRO BEACON E 1845, Iwo Jima", "IN", 25, 25, 25, 140, 144, 22, 26, 145, 75, -272},
{"AUA", "AUSTRALIAN GEODETIC 1966", "AN", 3, 3, 3, 109, 161, -46, -4, -133, -48, 148},
{"AUG", "AUSTRALIAN GEODETIC 1984", "AN", 2, 2, 2, 109, 161, -46, -4, -134, -48, 149},
{"BAT", "DJAKARTA, INDONESIA", "BR", 3, 3, 3, 89, 146, -16, 11, -377, 681, -50},
{"BER", "BERMUDA 1957, Bermuda Islands", "CC", 20, 20, 20, -66, -63, 31, 34, -73, 213, 296},
{"BID", "BISSAU, Guinea-Bissau", "IN", 25, 25, 25, -23, -7, 5, 19, -173, 253, 27},
{"BOO", "BOGOTA OBSERVATORY, Columbia", "IN", 6, 5, 6, -85, -61, -10, 16, 307, 304, -318},
{"BUR", "BUKIT RIMPAH, Banka & Belitung", "BR", -1, -1, -1, 103, 110, -6, 0, -384, 664, -48},
{"CAC", "CAPE CANAVERAL, Fla & Bahamas", "CC", 3, 3, 3, -94, -12, 15, 38, -2, 151, 181},
{"CAI", "CAMPO INCHAUSPE 1969, Arg.", "IN", 5, 5, 5, -72, -51, -58, -27, -148, 136, 90},
{"CAO", "CANTON ASTRO 1966, Phoenix Is.", "IN", 15, 15, 15, -180, -165, -13, 3, 298, -304, -375},
{"CAP", "CAPE, South Africa", "CD", 3, 6, 6, 10, 40, -43, -15, -136, -108, -292},
{"CAZ", "CAMP AREA ASTRO, Camp McMurdo", "IN", -1, -1, -1, 135, 180, -85, -70, -104, -129, 239},
{"CCD", "S-JTSK, Czech Republic", "BR", 4, 2, 3, 6, 28, 43, 56, 589, 76, 480},
{"CGE", "CARTHAGE, Tunisia", "CD", 6, 9, 8, 2, 18, 24, 43, -263, 6, 431},
{"CHI", "CHATHAM ISLAND ASTRO 1971, NZ", "IN", 15, 15, 15, -180, -174, -46, -42, 175, -38, 113},
{"CHU", "CHUA ASTRO, Paraguay", "IN", 6, 9, 5, -69, -49, -33, -14, -134, 229, -29},
{"COA", "CORREGO ALEGRE, Brazil", "IN", 5, 3, 5, -80, -29, -39, -2, -206, 172, -6},
{"DAL", "DABOLA, Guinea", "CD", 15, 15, 15, 12, 11, 1, 19, -83, 37, 124},
{"DID", "DECEPTION ISLAND", "CD", 20, 20, 20, 58, 62, -65, -62, 260, 12, -147},
{"DOB", "GUX 1 ASTRO, Guadalcanal Is.", "IN", 25, 25, 25, 158, 163, -12, -8, 252, -209, -751},
{"EAS", "EASTER ISLAND 1967", "IN", 25, 25, 25, -111, -108, -29, -26, 211, 147, 111},
{"ENW", "WAKE-ENIWETOK 1960", "HO", 3, 3, 3, 159, 175, 1, 16, 102, 52, -38},
{"EST", "ESTONIA, 1937", "BR", 2, 3, 3, 16, 34, 52, 65, 374, 150, 588},
{"EUR-A", "EUROPEAN 1950, Western Europe", "IN", 3, 3, 3, -15, 25, 30, 78, -87, -96, -120},
{"EUR-B", "EUROPEAN 1950, Greece", "IN", 25, 25, 25, 14, 34, 30, 48, -84, -95, -130},
{"EUR-C", "EUROPEAN 1950, Norway & Finland", "IN", 3, 5, 3, -2, 38, 52, 80, -87, -95, -120},
{"EUR-D", "EUROPEAN 1950, Portugal & Spain", "IN", 5, 6, 3, -15, 10, 30, 49, -84, -107, -120},
{"EUR-E", "EUROPEAN 1950, Cyprus", "IN", 15, 15, 15, 31, 36, 33, 37, -104, -101, -140},
{"EUR-F", "EUROPEAN 1950, Egypt", "IN", 6, 8, 8, 19, 42, 16, 38, -130, -117, -151},
{"EUR-G", "EUROPEAN 1950, England, Channel", "IN", 3, 3, 3, -10, 3, 48, 62, -86, -96, -120},
{"EUR-H", "EUROPEAN 1950, Iran", "IN", 9, 12, 11, 37, 69, 19, 47, -117, -132, -164},
{"EUR-I", "EUROPEAN 1950, Sardinia(Italy)", "IN", 25, 25, 25, 6, 12, 37, 43, -97, -103, -120},
{"EUR-J", "EUROPEAN 1950, Sicily(Italy)", "IN", 20, 20, 20, 10, 17, 35, 40, -97, -88, -135},
{"EUR-K", "EUROPEAN 1950, England, Ireland", "IN", 3, 3, 3, -12, 3, 48, 62, -86, -96, -120},
{"EUR-L", "EUROPEAN 1950, Malta", "IN", 25, 25, 25, 12, 16, 34, 38, -107, -88, -149},
{"EUR-M", "EUROPEAN 1950, Mean (3 Param)", "IN", 3, 8, 5, 5, 33, 30, 80, -87, -98, -121},
{"EUR-S", "EUROPEAN 1950, Iraq, Israel", "IN", -1, -1, -1, 36, 57, -38, -4, -103, -106, -141},
{"EUR-T", "EUROPEAN 1950, Tunisia", "IN", 25, 25, 25, 2, 18, 24, 43, -112, -77, -145},
{"EUS", "EUROPEAN 1979", "IN", 3, 3, 3, -15, 24, 30, 80, -86, -98, -119},
{"FAH", "OMAN", "CD", 3, 3, 9, 46, 65, 10, 32, -346, -1, 224},
{"FLO", "OBSERVATORIO MET. 1939, Flores", "IN", 20, 20, 20, -33, -30, 38, 41, -425, -169, 81},
{"FOT", "FORT THOMAS 1955, Leeward Is.", "CD", 25, 25, 25, -64, -61, 16, 19, -7, 215, 225},
{"GAA", "GAN 1970, Rep. of Maldives", "IN", 25, 25, 25, 71, 75, -2, 9, -133, -321, 50},
{"GEO", "GEODETIC DATUM 1949, NZ", "IN", 5, 3, 5, 165, 180, -48, -33, 84, -22, 209},
{"GIZ", "DOS 1968, Gizo Island", "IN", 25, 25, 25, 155, 158, -10, -7, 230, -199, -752},
{"GRA", "GRACIOSA BASE SW 1948, Azores", "IN", 3, 3, 3, -30, -26, 37, 41, -104, 167, -38},
{"GSE", "GUNUNG SEGARA, Indonesia", "BR", -1, -1, -1, 106, 121, -6, 9, -403, 684, 41},
{"GUA", "GUAM 1963", "CC", 3, 3, 3, 143, 146, 12, 15, -100, -248, 259},
{"HEN", "HERAT NORTH, Afghanistan", "IN", -1, -1, -1, 55, 81, 23, 44, -333, -222, 114},
{"HER", "HERMANNSKOGEL, old Yugoslavia", "BR", -1, -1, -1, 7, 29, 35, 52, 682, -203, 480},
{"HIT", "PROVISIONAL SOUTH CHILEAN 1963", "IN", 25, 25, 25, -83, -60, -64, -25, 16, 196, 93},
{"HJO", "HJORSEY 1955, Iceland", "IN", 3, 3, 6, -24, -11, 61, 69, -73, 46, -86},
{"HKD", "HONG KONG 1963", "IN", 25, 25, 25, 112, 116, 21, 24, -156, -271, -189},
{"HTN", "HU-TZU-SHAN, Taiwan", "IN", 15, 15, 15, 117, 124, 20, 28, -637, -549, -203},
{"IBE", "BELLEVUE (IGN), Efate Is.", "IN", 20, 20, 20, 167, 171, -20, -16, -127, -769, 472},
{"IDN", "INDONESIAN 1974", "ID", 25, 25, 25, 89, 146, -16, 11, -24, -15, 5},
{"IND-B", "INDIAN, Bangladesh", "EA", 10, 8, 12, 80, 100, 15, 33, 282, 726, 254},
{"IND-I", "INDIAN, India & Nepal", "EC", 12, 10, 15, 62, 105, 2, 44, 295, 736, 257},
{"IND-P", "INDIAN, Pakistan", "EA", -1, -1, -1, 55, 81, 17, 44, 283, 682, 231},
{"INF-A", "INDIAN 1954, Thailand", "EA", 15, 6, 12, 91, 111, 0, 27, 217, 823, 299},
{"ING-A", "INDIAN 1960, Vietnam 16N", "EA", 25, 25, 25, 101, 115, 11, 23, 198, 881, 317},
{"ING-B", "INDIAN 1960, Con Son Island", "EA", 25, 25, 25, 104, 109, 6, 11, 182, 915, 344},
{"INH-A", "INDIAN 1975, Thailand", "EA", 12, 10, 12, 91, 111, 0, 27, 209, 818, 290},
{"INH-A1", "INDIAN 1975, Thailand", "EA", 3, 2, 3, 91, 111, 0, 27, 210, 814, 289},
{"IRL", "IRELAND 1965", "AM", 3, 3, 3, -12, -4, 50, 57, 506, -122, 611},
{"ISG", "ISTS 061 ASTRO 1968, S Georgia", "IN", 25, 25, 25, -38, -34, -56, -52, -794, 119, -298},
{"IST", "ISTS 073 ASTRO 1969, Diego Garc", "IN", 25, 25, 25, 69, 75, -10, -4, 208, -435, -229},
{"JOH", "JOHNSTON ISLAND 1961", "IN", 25, 25, 25, -76, -73, -46, -43, 189, -79, -202},
{"KAN", "KANDAWALA, Sri Lanka", "EA", 20, 20, 20, 77, 85, 4, 12, -97, 787, 86},
{"KEA", "KERTAU 1948, W Malaysia & Sing.", "EE", 10, 8, 6, 94, 112, -5, 12, -11, 851, 5},
{"KEG", "KERGUELEN ISLAND 1949", "IN", 25, 25, 25, 139, 180, -81, -74, 145, -187, 103},
{"KUS", "KUSAIE ASTRO 1951, Caroline Is.", "IN", 25, 25, 25, 134, 167, -1, 12, 647, 1777, -1124},
{"LCF", "L.C. 5 ASTRO 1961, Cayman Brac", "CC", 25, 25, 25, -81, -78, 18, 21, 42, 124, 147},
{"LEH", "LEIGON, Ghana", "CD", 2, 3, 2, -9, 7, -1, 17, -130, 29, 364},
{"LIB", "LIBERIA 1964", "CD", 15, 15, 15, -17, -1, -1, 14, -90, 40, 88},
{"LUZ-A", "LUZON, Phillipines", "CC", 8, 11, 9, 115, 128, 3, 23, -133, -77, -51},
{"LUZ-B", "LUZON, Mindanao Island", "CC", 25, 25, 25, 120, 128, 4, 12, -133, -79, -72},
{"MAS", "MASSAWA, Ethiopia", "BR", 25, 25, 25, 37, 53, 7, 25, 639, 405, 60},
{"MER", "MERCHICH, Morocco", "CD", 5, 3, 3, -19, 5, 22, 42, 31, 146, 47},
{"MID", "MIDWAY ASTRO 1961, Midway Is.", "IN", 25, 25, 25, -180, -169, 25, 30, 912, -58, 1227},
{"MIK", "MAHE 1971, Mahe Is.", "CD", 25, 25, 25, 54, 57, -6, -3, 41, -220, -134},
{"MIN-A", "MINNA, Cameroon", "CD", 25, 25, 25, 3, 23, -4, 19, -81, -84, 115},
{"MIN-B", "MINNA, Nigeria", "CD", 3, 6, 5, -4, 20, -1, 21, -92, -93, 122},
{"MOD", "ROME 1940, Sardinia", "IN", 25, 25, 25, 6, 12, 37, 43, -225, -65, 9},
{"MPO", "M'PORALOKO, Gabon", "CD", 25, 25, 25, 3, 20, -10, 8, -74, -130, 42},
{"MVS", "VITI LEVU 1916, Viti Levu Is.", "CD", 25, 25, 25, 176, 180, -20, -16, 51, 391, -36},
{"NAH-A", "NAHRWAN, Masirah Island (Oman)", "CD", 25, 25, 25, 57, 60, 19, 22, -247, -148, 369},
{"NAH-B", "NAHRWAN, United Arab Emirates", "CD", 25, 25, 25, 45, 62, 17, 32, -249, -156, 381},
{"NAH-C", "NAHRWAN, Saudi Arabia", "CD", 25, 25, 20, 28, 62, 8, 38, -243, -192, 477},
{"NAP", "NAPARIMA, Trinidad & Tobago", "IN", 15, 15, 15, -64, -59, 8, 13, -10, 375, 165},
{"NAR-A", "NORTH AMERICAN 1983, Alaska", "RF", 2, 2, 2, -175, -135, 48, 78, 0, 0, 0},
{"NAR-B", "NORTH AMERICAN 1983, Canada", "RF", 2, 2, 2, -150, -50, 36, 90, 0, 0, 0},
{"NAR-C", "NORTH AMERICAN 1983, CONUS", "RF", 2, 2, 2, -135, -60, 15, 60, 0, 0, 0},
{"NAR-D", "NORTH AMERICAN 1983, Mexico", "RF", 2, 2, 2, -122, -72, 11, 35, 0, 0, 0},
{"NAR-E", "NORTH AMERICAN 1983, Aleutian", "RF", 5, 2, 5, -180, 180, 51, 74, -2, 0, 4},
{"NAR-H", "NORTH AMERICAN 1983, Hawai'i", "RF", 2, 2, 2, -164, -153, 17, 24, 1, 1, -1},
{"NAS-A", "NORTH AMERICAN 1927, Eastern US", "CC", 5, 5, 8, -102, -60, 18, 55, -9, 161, 179},
{"NAS-B", "NORTH AMERICAN 1927, Western US", "CC", 5, 3, 3, -132, -87, 19, 55, -8, 159, 175},
{"NAS-C", "NORTH AMERICAN 1927, CONUS", "CC", 5, 5, 6, -135, -60, 15, 60, -8, 160, 176},
{"NAS-D", "NORTH AMERICAN 1927, Alaska", "CC", 5, 9, 5, -175, -130, 47, 78, -5, 135, 172},
{"NAS-E", "NORTH AMERICAN 1927, Canada", "CC", 15, 11, 6, -150, -50, 36, 90, -10, 158, 187},
{"NAS-F", "NORTH AMERICAN 1927, Alberta/BC", "CC", 8, 8, 6, -145, -105, 43, 65, -7, 162, 188},
{"NAS-G", "NORTH AMERICAN 1927, E. Canada", "CC", 6, 6, 3, -85, -45, 38, 68, -22, 160, 190},
{"NAS-H", "NORTH AMERICAN 1927, Man/Ont", "CC", 9, 5, 5, -108, -69, 36, 63, -9, 157, 184},
{"NAS-I", "NORTH AMERICAN 1927, NW Terr.", "CC", 5, 5, 3, -144, -55, 43, 90, 4, 159, 188},
{"NAS-J", "NORTH AMERICAN 1927, Yukon", "CC", 5, 8, 3, -147, -117, 53, 75, -7, 139, 181},
{"NAS-L", "NORTH AMERICAN 1927, Mexico", "CC", 8, 6, 6, -122, -80, 10, 38, -12, 130, 190},
{"NAS-N", "NORTH AMERICAN 1927, C. America", "CC", 8, 3, 5, -98, -77, 3, 25, 0, 125, 194},
{"NAS-O", "NORTH AMERICAN 1927, Canal Zone", "CC", 20, 20, 20, -86, -74, 3, 15, 0, 125, 201},
{"NAS-P", "NORTH AMERICAN 1927, Caribbean", "CC", 3, 9, 12, -87, -58, 8, 29, -3, 142, 183},
{"NAS-Q", "NORTH AMERICAN 1927, Bahamas", "CC", 5, 3, 5, -83, -71, 19, 29, -4, 154, 178},
{"NAS-R", "NORTH AMERICAN 1927, San Salv.", "CC", 25, 25, 25, -75, -74, 23, 26, 1, 140, 165},
{"NAS-T", "NORTH AMERICAN 1927, Cuba", "CC", 25, 25, 25, -87, -72, 18, 25, -9, 152, 178},
{"NAS-U", "NORTH AMERICAN 1927, Greenland", "CC", 25, 25, 25, 74, 56, 74, 81, 11, 114, 195},
{"NAS-V", "NORTH AMERICAN 1927, Aleutian E", "CC", 6, 8, 10, -180, -161, 50, 58, -2, 152, 149},
{"NAS-W", "NORTH AMERICAN 1927, Aleutian W", "CC", 10, 10, 10, 169, 180, 50, 58, 2, 204, 105},
{"NSD", "NORTH SAHARA 1959, Algeria", "CD", 25, 25, 25, -15, 11, 13, 43, -186, -93, 310},
{"NTF", "NOUVELLE TRIANGULATION DE LA FRANCE, FRANCE", "CE", 0, 0, 0, -6, 12, 41, 51, -168, -60, 320},
{"OEG", "OLD EGYPTIAN 1907", "HE", 3, 6, 8, 19, 42, 16, 38, -130, 110, -13},
{"OGB-A", "ORDNANCE GB 1936, England", "AA", 5, 5, 6, -12, 7, 44, 61, 371, -112, 434},
{"OGB-B", "ORDNANCE GB 1936, Eng., Wales", "AA", 10, 10, 15, -12, 7, 44, 61, 371, -111, 434},
{"OGB-C", "ORDNANCE GB 1936, Scotland", "AA", 10, 10, 10, -14, 4, 49, 66, 384, -111, 425},
{"OGB-D", "ORDNANCE GB 1936, Wales", "AA", 20, 20, 20, -11, 3, 46, 59, 370, -108, 434},
{"OGB-M", "ORDNANCE GB 1936, Mean (3 Para)", "AA", 10, 10, 15, -14, 7, 44, 66, 375, -111, 431},
{"OHA-A", "OLD HAWAI'IAN (CC), Hawai'i", "CC", 25, 25, 25, -158, -153, 17, 22, 89, -279, -183},
{"OHA-B", "OLD HAWAI'IAN (CC), Kauai", "CC", 20, 20, 20, -161, -158, 20, 24, 45, -290, -172},
{"OHA-C", "OLD HAWAI'IAN (CC), Maui", "CC", 25, 25, 25, -158, -154, 19, 23, 65, -290, -190},
{"OHA-D", "OLD HAWAI'IAN (CC), Oahu", "CC", 10, 6, 6, -160, -156, 20, 23, 58, -283, -182},
{"OHA-M", "OLD HAWAI'IAN (CC), Mean", "CC", 25, 20, 20, -164, -153, 17, 24, 61, -285, -181},
{"OHI-A", "OLD HAWAI'IAN (IN), Hawai'i", "IN", 25, 25, 25, -158, -153, 17, 22, 229, -222, -348},
{"OHI-B", "OLD HAWAI'IAN (IN), Kauai", "IN", 20, 20, 20, -161, -158, 20, 24, 185, -233, -337},
{"OHI-C", "OLD HAWAI'IAN (IN), Maui", "IN", 25, 25, 25, -158, -154, 19, 23, 205, -233, -355},
{"OHI-D", "OLD HAWAI'IAN (IN), Oahu", "IN", 10, 6, 6, -160, -156, 20, 23, 198, -226, -347},
{"OHI-M", "OLD HAWAI'IAN (IN), Mean", "IN", 25, 20, 20, -164, -153, 17, 24, 201, -228, -346},
{"PHA", "AYABELLA LIGHTHOUSE, Bjibouti", "CD", 25, 25, 25, 36, 49, 5, 20, -79, -129, 145},
{"PIT", "PITCAIRN ASTRO 1967", "IN", 25, 25, 25, -134, -119, -27, -21, 185, 165, 42},
{"PLN", "PICO DE LAS NIEVES, Canary Is.", "IN", 25, 25, 25, -20, -12, 26, 31, -307, -92, 127},
{"POS", "PORTO SANTO 1936, Madeira Is.", "IN", 25, 25, 25, -18, -15, 31, 35, -499, -249, 314},
{"PRP-A", "PROV. S AMERICAN 1956, Bolivia", "IN", 5, 11, 14, -75, -51, -28, -4, -270, 188, -388},
{"PRP-B", "PROV. S AMERICAN 1956, N Chile", "IN", 25, 25, 25, -83, -60, -45, -12, -270, 183, -390},
{"PRP-C", "PROV. S AMERICAN 1956, S Chile", "IN", 20, 20, 20, -83, -60, -64, -20, -305, 243, -442},
{"PRP-D", "PROV. S AMERICAN 1956, Colombia", "IN", 15, 15, 15, -85, -61, -10, 16, -282, 169, -371},
{"PRP-E", "PROV. S AMERICAN 1956, Ecuador", "IN", 3, 5, 3, -85, -70, -11, 7, -278, 171, -367},
{"PRP-F", "PROV. S AMERICAN 1956, Guyana", "IN", 6, 14, 5, -67, -51, -4, 14, -298, 159, -369},
{"PRP-G", "PROV. S AMERICAN 1956, Peru", "IN", 6, 8, 12, -87, -63, -24, 5, -279, 175, -379},
{"PRP-H", "PROV. S AMERICAN 1956, Venez", "IN", 9, 14, 15, -79, -54, -5, 18, -295, 173, -371},
{"PRP-M", "PROV. S AMERICAN 1956, Mean", "IN", 17, 27, 27, -87, -51, -64, 18, -288, 175, -376},
{"PTB", "POINT 58, Burkina Faso & Niger", "CD", 25, 25, 25, -15, 25, 0, 10, -106, -129, 165},
{"PTN", "POINT NOIRE 1948", "CD", 25, 25, 25, 5, 25, -11, 10, -148, 51, -291},
{"PUK", "PULKOVO 1942, Russia", "KA", -1, -1, -1, -180, 180, 36, 89, 28, -130, -95},
{"PUR", "PUERTO RICO & Virgin Is.", "CC", 3, 3, 3, -69, -63, 16, 20, 11, 72, -101},
{"QAT", "QATAR NATIONAL", "IN", 20, 20, 20, 45, 57, 19, 32, -128, -283, 22},
{"QUO", "QORNOQ, South Greenland", "IN", 25, 25, 32, -77, -7, 57, 85, 164, 138, -189},
{"REU", "REUNION, Mascarene Is.", "IN", 25, 25, 25, 47, 65, -27, -12, 94, -948, -1262},
{"SAE", "SANTO (DOS) 1965", "IN", 22, 25, 25, 160, 169, -17, -13, 170, 42, 84},
{"SAN-A", "SOUTH AMERICAN 1969, Argentina", "SA", 5, 5, 5, -76, -47, -62, -23, -62, -1, -37},
{"SAN-B", "SOUTH AMERICAN 1969, Bolivia", "SA", 15, 15, 15, -75, -51, -28, -4, -61, 2, -48},
{"SAN-C", "SOUTH AMERICAN 1969, Brazil", "SA", 3, 5, 5, -80, -29, -39, -2, -60, -2, -41},
{"SAN-D", "SOUTH AMERICAN 1969, Chile", "SA", 15, 8, 11, -83, -60, -64, -12, -75, -1, -44},
{"SAN-E", "SOUTH AMERICAN 1969, Colombia", "SA", 6, 6, 5, -85, -61, -10, 16, -44, 6, -36},
{"SAN-F", "SOUTH AMERICAN 1969, Ecuador", "SA", 3, 3, 3, -85, -70, -11, 7, -48, 3, -44},
{"SAN-G", "SOUTH AMERICAN 1969, Guyana", "SA", 9, 5, 5, -67, -51, -4, 14, -53, 3, -47},
{"SAN-H", "SOUTH AMERICAN 1969, Paraguay", "SA", 15, 15, 15, -69, -49, -33, -14, -61, 2, -33},
{"SAN-I", "SOUTH AMERICAN 1969, Peru", "SA", 5, 5, 5, -87, -63, -24, 5, -58, 0, -44},
{"SAN-J", "SOUTH AMERICAN 1969, Baltra", "SA", 25, 25, 25, -92, -89, -2, 1, -47, 26, -42},
{"SAN-K", "SOUTH AMERICAN 1969, Trinidad", "SA", 25, 25, 25, -68, -55, 4, 17, -45, 12, -33},
{"SAN-L", "SOUTH AMERICAN 1969, Venezuela", "SA", 3, 6, 3, -79, -54, -5, 18, -45, 8, -33},
{"SAN-M", "SOUTH AMERICAN 1969, Mean", "SA", 15, 6, 9, -90, -25, -65, -50, -57, 1, -41},
{"SAO", "SAO BRAZ, Santa Maria Is.", "IN", 25, 25, 25, -27, -23, 35, 39, -203, 141, 53},
{"SAP", "SAPPER HILL 1943, E Falkland Is", "IN", 1, 1, 1, -61, -56, -54, -50, -355, 21, 72},
{"SCK", "SCHWARZECK, Namibia", "BN", 20, 20, 20, 5, 31, -35, -11, 616, 97, -251},
{"SGM", "SELVAGEM GRADE 1938, Salvage Is", "IN", 25, 25, 25, -18, -14, 28, 32, -289, -124, 60},
{"SHB", "ASTRO DOS 71/4, St. Helena Is.", "IN", 25, 25, 25, -7, -4, -18, -14, -320, 550, -494},
{"SOA", "SOUTH ASIA, Singapore", "FA", 25, 25, 25, 102, 106, 0, 3, 7, -10, -26},
{"SPK-A", "S-42 (PULKOVO 1942), Hungary", "KA", 2, 2, 2, 11, 29, 40, 54, 28, -121, -77},
{"SPK-B", "S-42 (PULKOVO 1942), Poland", "KA", 4, 2, 4, 8, 30, 43, 60, 23, -124, -82},
{"SPK-C", "S-42 (PK42) Former Czechoslov.", "KA", 3, 3, 2, 6, 28, 42, 57, 26, -121, -78},
{"SPK-D", "S-42 (PULKOVO 1942), Latvia", "KA", 2, 2, 2, 15, 34, 50, 64, 24, -124, -82},
{"SPK-E", "S-42 (PK 1942), Kazakhstan", "KA", 25, 25, 25, 41, 93, 35, 62, 15, -130, -84},
{"SPK-F", "S-42 (PULKOVO 1942), Albania", "KA", 3, 3, 3, 14, 26, 34, 48, 24, -130, -92},
{"SPK-G", "S-42 (PULKOVO 1942), Romania", "KA", 3, 5, 3, 15, 35, 38, 54, 28, -121, -77},
{"SRL", "SIERRA LEONE 1960", "CD", 15, 15, 15, -19, -4, 1, 16, -88, 4, 101},
{"TAN", "TANANARIVE OBSERVATORY 1925", "IN", -1, -1, -1, 40, 53, -34, -8, -189, -242, -91},
{"TDC", "TRISTAN ASTRO 1968", "IN", 25, 25, 25, -14, -11, -39, -36, -632, 438, -609},
{"TIL", "TIMBALAI 1948, Brunei & E Malay", "EA", 10, 10, 12, 101, 125, -5, 15, -679, 669, -48},
{"TOY-A", "TOKYO, Japan", "BR", 8, 5, 8, 119, 156, 19, 51, -148, 507, 685},
{"TOY-B", "TOKYO, South Korea", "BR", 8, 5, 8, 120, 139, 27, 45, -146, 507, 687},
{"TOY-B1", "TOKYO, South Korea", "BR", 2, 2, 2, 120, 139, 27, 45, -147, 506, 687},
{"TOY-C", "TOKYO, Okinawa", "BR", 20, 5, 20, 119, 134, 19, 31, -158, 507, 676},
{"TOY-M", "TOKYO, Mean", "BR", 20, 5, 20, 120, 155, 23, 53, -148, 507, 685},
{"TRN", "ASTRO TERN ISLAND (FRIG) 1961", "IN", 25, 25, 25, -166, -166, 22, 26, 114, -116, -333},
{"VOI", "VOIROL 1874, Algeria", "CD", -1, -1, -1, -15, 11, 13, 43, -73, -247, 227},
{"VOR", "VOIROL 1960, Algeria", "CD", 25, 25, 25, -15, 11, 13, 43, -123, -206, 219},
{"WAK", "WAKE ISLAND ASTRO 1952", "IN", 25, 25, 25, -176, -171, 17, 21, 276, -57, 149},
{"WGD", "World Geodetic System 1972", "WD", 0, 0, 0, -180.0, 180.0, -90, 90, 0, 0, 0},
{"WGE", "World Geodetic System 1984", "WE", 0, 0, 0, -180.0, 180.0, -90, 90, 0, 0, 0},
{"YAC", "YACARE, Uruguay", "IN", -1, -1, -1, -65, -47, -40, -25, -155, 171, 37},
{"ZAN", "ZANDERIJ, Suriname", "IN", 5, 5, 8, -76, -47, -10, 20, -265, 120, -358},
{"6055", "Popular Visualisation", "7059", 0, 0, 0, -180.0, 180.0, -85, 85, 0, 0, 0},
{"", "", "", 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0}
};

static ossimSevenParamDatumType sevenParamDatum[] = {
{"EUR-7", "EUROPEAN 1950, Mean (7 Param)", "IN", 0, 0, 0, -180, 180, -90, 90, -102, -102, -129, 2.00228e-06, -8.92057e-07, 1.86653e-06, 2.4664e-06},
{"OGB-7", "ORDNANCE GB 1936, Mean (7 Para)", "AA", 0, 0, 0, -180, 180, -90, 90, 446, -99, 544, -4.58149e-06, -1.26536e-06, -2.10894e-06, -2.08927e-05},
{"", "", "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
