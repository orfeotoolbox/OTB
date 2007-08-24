/* WAVA2.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       SUBROUTINE wava2(a,inu) >*/
/* Subroutine */ int wava2_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float).32591,(float)
	    .48473,(float).010062,(float)1.8245e-5,(float).01189,(float)
	    -1.2621e-5,(float)5060.,(float)5070.,(float).73059,(float).13181,(
	    float).010626,(float)7.3795e-6,(float).011376,(float)-1.7764e-5,(
	    float)5070.,(float)5080.,(float).39211,(float).39522,(float)
	    .01459,(float)-6.8376e-6,(float).016326,(float)-3.165e-5,(float)
	    5080.,(float)5090.,(float)2.3255,(float).26901,(float).0092468,(
	    float)6.3821e-6,(float).0083396,(float)-1.2628e-5,(float)5090.,(
	    float)5100.,(float)1.9706,(float).34245,(float).0056073,(float)
	    -9.4265e-8,(float).0059986,(float)-1.0398e-5,(float)5100.,(float)
	    5110.,(float)2.8938,(float).22738,(float).022463,(float)
	    -9.7912e-5,(float).021063,(float)-8.8497e-5,(float)5110.,(float)
	    5120.,(float)1.1454,(float).40811,(float).0089642,(float)
	    -5.1753e-6,(float).0089863,(float)-1.9479e-5,(float)5120.,(float)
	    5130.,(float)2.92,(float).33384,(float).014298,(float)-3.2571e-5,(
	    float).012431,(float)-3.5993e-5,(float)5130.,(float)5140.,(float)
	    6.318,(float).28323,(float).0099085,(float)-1.94e-5,(float)
	    .0060737,(float)-1.671e-5,(float)5140.,(float)5150.,(float)14.918,
	    (float).41681,(float).012935,(float)-5.2738e-5,(float).010041,(
	    float)-4.0263e-5,(float)5150.,(float)5160.,(float)10.19,(float)
	    .13237,(float).011091,(float)-4.8728e-5,(float).0081335,(float)
	    -2.7045e-5,(float)5160.,(float)5170.,(float)14.437,(float).36156,(
	    float).0094685,(float)-3.4308e-5,(float).0067537,(float)
	    -2.4477e-5,(float)5170.,(float)5180.,(float)34.792,(float).30287,(
	    float).0073719,(float)-3.7632e-5,(float).0051901,(float)
	    -2.8247e-5,(float)5180.,(float)5190.,(float)21.869,(float).22075,(
	    float).0069763,(float)-3.1715e-5,(float).0041088,(float)
	    -1.7042e-5,(float)5190.,(float)5200.,(float)94.938,(float).25888,(
	    float).0046429,(float)-2.8823e-5,(float).0034889,(float)
	    -2.3494e-5,(float)5200.,(float)5210.,(float)18.298,(float).20311,(
	    float).0050431,(float)-2.4681e-5,(float).0028469,(float)-1.707e-5,
	    (float)5210.,(float)5220.,(float)69.538,(float).19126,(float)
	    .002196,(float)-2.0268e-5,(float)6.875e-4,(float)-1.7413e-5,(
	    float)5220.,(float)5230.,(float)40.772,(float).23521,(float)
	    .0018896,(float)-1.692e-5,(float)5.9126e-4,(float)-1.3609e-5,(
	    float)5230.,(float)5240.,(float)131.08,(float).17928,(float)
	    1.5657e-4,(float)-1.2114e-5,(float)-.0017405,(float)-8.3108e-6,(
	    float)5240.,(float)5250.,(float)69.597,(float).10896,(float)
	    -.0015337,(float)-4.7461e-6,(float)-.0027082,(float)-1.5598e-6,(
	    float)5250.,(float)5260.,(float)153.51,(float).20025,(float)
	    -.0022803,(float)-2.9253e-6,(float)-.004398,(float)2.8697e-6,(
	    float)5260.,(float)5270.,(float)2.6114,(float).24878,(float)
	    -8.2927e-4,(float)4.6226e-6,(float)-1.1922e-4,(float)1.0717e-5,(
	    float)5270.,(float)5280.,(float)48.681,(float).175,(float)
	    -.0037857,(float)4.6902e-6,(float)-.0036353,(float)1.1426e-5,(
	    float)5280.,(float)5290.,(float)77.56,(float).13557,(float)
	    -.0037228,(float)3.6212e-6,(float)-.0041326,(float)1.5278e-5,(
	    float)5290.,(float)5300.,(float)60.444,(float).1151,(float)
	    -.0047324,(float)1.0254e-5,(float)-.0036821,(float)1.4636e-5,(
	    float)5300.,(float)5310.,(float)15.328,(float).19998,(float)
	    -.0012681,(float)2.9898e-6,(float)-4.683e-4,(float)2.0999e-6,(
	    float)5310.,(float)5320.,(float)87.54,(float).16972,(float)
	    -.0041972,(float)7.1961e-6,(float)-.0037508,(float)7.0298e-6,(
	    float)5320.,(float)5330.,(float)195.61,(float).26284,(float)
	    -.0023874,(float)-2.1407e-7,(float)-.0037245,(float)4.6625e-6,(
	    float)5330.,(float)5340.,(float)168.41,(float).26546,(float)
	    2.4931e-4,(float)-9.4775e-6,(float)-.0016338,(float)-6.291e-6,(
	    float)5340.,(float)5350.,(float)93.921,(float).31129,(float)
	    .0028343,(float)-7.1173e-6,(float).0026325,(float)-1.1816e-5,(
	    float)5350.,(float)5360.,(float)25.579,(float).36877,(float)
	    .0072262,(float)-1.5821e-5,(float).0060257,(float)-2.1996e-5,(
	    float)5360.,(float)5370.,(float)137.79,(float).17089,(float)
	    -.004665,(float)1.3962e-5,(float)-.002641,(float)1.792e-5,(float)
	    5370.,(float)5380.,(float)84.199,(float).12293,(float)-.0039643,(
	    float)9.7938e-6,(float)-.0017023,(float)1.5901e-5,(float)5380.,(
	    float)5390.,(float)140.72,(float).10478,(float)-.0041237,(float)
	    5.0169e-6,(float)-.0043789,(float)1.7244e-5,(float)5390.,(float)
	    5400.,(float)56.622,(float).23084,(float)-.0025409,(float)
	    -2.9811e-7,(float)-.0027639,(float)7.6091e-6,(float)5400.,(float)
	    5410.,(float)185.34,(float).21363,(float)-.0028691,(float)
	    -4.4392e-7,(float)-.0042358,(float)6.6265e-6,(float)5410.,(float)
	    5420.,(float)96.955,(float).12223,(float)-5.2081e-4,(float)
	    -9.6057e-6,(float)-.0016418,(float)-1.9789e-6,(float)5420.,(float)
	    5430.,(float)117.89,(float).13571,(float)-.0014763,(float)
	    -5.7081e-6,(float)-.0024763,(float)-1.7817e-7,(float)5430.,(float)
	    5440.,(float)191.02,(float).24199,(float)6.976e-4,(float)
	    -1.4228e-5,(float)-.001445,(float)-7.5253e-6,(float)5440.,(float)
	    5450.,(float)22.138,(float).14268,(float).0012465,(float)
	    -1.3898e-5,(float).00202,(float)-8.418e-6,(float)5450.,(float)
	    5460.,(float)95.115,(float).24823,(float).0035551,(float)
	    -2.5604e-5,(float).0012041,(float)-1.8465e-5,(float)5460.,(float)
	    5470.,(float)62.976,(float).15811,(float).0050592,(float)
	    -2.9507e-5,(float).0029262,(float)-2.138e-5,(float)5470.,(float)
	    5480.,(float)29.147,(float).11805,(float).008422,(float)
	    -4.2987e-5,(float).0067418,(float)-3.6032e-5,(float)5480.,(float)
	    5490.,(float)51.614,(float).19818,(float).0058436,(float)-3.46e-5,
	    (float).003939,(float)-2.8598e-5,(float)5490.,(float)5500.,(float)
	    23.266,(float).1679,(float).011075,(float)-5.348e-5,(float)
	    .0084735,(float)-4.6037e-5,(float)5500.,(float)5510.,(float)
	    10.989,(float).20302,(float).0063972,(float)-1.4524e-5,(float)
	    .0042223,(float)-2.0806e-5,(float)5510.,(float)5520.,(float)
	    10.027,(float).16542,(float).013794,(float)-6.332e-5,(float)
	    .011093,(float)-5.5041e-5,(float)5520.,(float)5530.,(float)9.3954,
	    (float).21952,(float).013366,(float)-5.7474e-5,(float).0098104,(
	    float)-4.6605e-5,(float)5530.,(float)5540.,(float)4.2252,(float)
	    .19874,(float).016633,(float)-7.121e-5,(float).01335,(float)
	    -6.0705e-5,(float)5540.,(float)5550.,(float)6.2656,(float).26354,(
	    float).0098363,(float)-9.5486e-6,(float).0079542,(float)
	    -2.4062e-5,(float)5550.,(float)5560.,(float)1.6378,(float).16463,(
	    float).013976,(float)-8.3427e-6,(float).012806,(float)-3.571e-5,(
	    float)5560.,(float)5570.,(float)1.9767,(float).27227,(float)
	    .014721,(float)-2.6094e-5,(float).012774,(float)-4.5522e-5,(float)
	    5570.,(float)5580.,(float).586,(float).20958,(float).012874,(
	    float)3.9258e-5,(float).011964,(float)-1.5397e-5,(float)5580.,(
	    float)5590.,(float).73058,(float).24104,(float).02517,(float)
	    -9.8784e-5,(float).021222,(float)-8.5036e-5,(float)5590.,(float)
	    5600.,(float)1.3668,(float).20354,(float).00746,(float)-5.4501e-6,
	    (float).010036,(float)-2.7451e-5,(float)5600.,(float)5610.,(float)
	    .7017,(float).14013,(float).0054963,(float)1.2171e-5,(float)
	    .010386,(float)-5.7235e-6,(float)5610.,(float)5620.,(float).97806,
	    (float).19171,(float).0021812,(float)1.6431e-5,(float).0066588,(
	    float)9.5186e-6,(float)5620.,(float)5630.,(float).29391,(float)
	    .23855,(float).0090316,(float)2.3604e-5,(float).012635,(float)
	    -6.6897e-6,(float)5630.,(float)5640.,(float).15354,(float).15176,(
	    float).0057073,(float)2e-5,(float).014561,(float)-4.0698e-6,(
	    float)5640.,(float)5650.,(float).08073,(float).28405,(float)
	    .014753,(float)3.439e-5,(float).018447,(float)-1.775e-5,(float)
	    5650.,(float)5660.,(float).86381,(float).14813,(float).0029607,(
	    float)-2.0586e-5,(float).0028212,(float)-1.2594e-5,(float)5660.,(
	    float)5670.,(float).22599,(float).16383,(float).0084407,(float)
	    -3.2709e-5,(float).011049,(float)-1.6279e-5,(float)5670.,(float)
	    5680.,(float).0030594,(float).17069,(float).034284,(float)
	    -1.0113e-4,(float).035042,(float)-1.1247e-4,(float)5680.,(float)
	    5690.,(float).38291,(float).16459,(float).0096772,(float)
	    -4.3054e-5,(float).0087884,(float)-3.2862e-5,(float)5690.,(float)
	    5700.,(float).17817,(float).065371,(float).0044945,(float)
	    -2.6954e-5,(float).0044075,(float)7.1895e-7,(float)5700.,(float)
	    5710.,(float).0073272,(float).32673,(float).018605,(float)
	    -3.6631e-5,(float).021288,(float)-5.004e-5,(float)5710.,(float)
	    5720.,(float).33773,(float).13375,(float).0060665,(float)
	    -3.0734e-5,(float).0046922,(float)-2.5584e-5,(float)5720.,(float)
	    5730.,(float).083205,(float).058895,(float).0083229,(float)
	    -4.2023e-5,(float).0099661,(float)-2.959e-5,(float)5730.,(float)
	    5740.,(float).1796,(float).1205,(float).0065668,(float)-3.475e-5,(
	    float).004411,(float)-2.7489e-5,(float)5740.,(float)5750.,(float)
	    .074406,(float).16592,(float).012426,(float)-4.4049e-5,(float)
	    .012086,(float)-4.8157e-5,(float)5750.,(float)5760.,(float)
	    .029803,(float).065159,(float).011506,(float)-5.4589e-5,(float)
	    .012823,(float)-5.1753e-5,(float)5760.,(float)5770.,(float).134,(
	    float).079723,(float).011174,(float)-5.413e-5,(float).010639,(
	    float)-4.7462e-5,(float)5770.,(float)5780.,(float).027397,(float)
	    .11303,(float).0070869,(float)-8.5837e-6,(float).0087642,(float)
	    -2.4197e-5,(float)5780.,(float)5790.,(float).04813,(float).1528,(
	    float).017121,(float)-6.8541e-5,(float).016166,(float)-7.1739e-5,(
	    float)5790.,(float)5800.,(float).098878,(float).11702,(float)
	    .011647,(float)-5.4757e-5,(float).0095467,(float)-4.9145e-5,(
	    float)5800.,(float)5810.,(float).0011937,(float).11746,(float)
	    .029514,(float)-1.178e-4,(float).026592,(float)-1.0438e-4,(float)
	    5810.,(float)5820.,(float).021084,(float).041603,(float).018136,(
	    float)-8.3736e-5,(float).01662,(float)-7.5802e-5,(float)5820.,(
	    float)5830.,(float).041859,(float).10038,(float).011698,(float)
	    -5.1084e-5,(float).0091292,(float)-3.8447e-5,(float)5830.,(float)
	    5840.,(float).0042835,(float).054158,(float).020564,(float)
	    -7.9064e-5,(float).016849,(float)-6.1318e-5,(float)5840.,(float)
	    5850.,(float).039742,(float).14212,(float).015543,(float)
	    -6.5164e-5,(float).012937,(float)-6.0213e-5,(float)5850.,(float)
	    5860.,(float).0044465,(float).13575,(float).018218,(float)
	    -6.1185e-5,(float).017472,(float)-6.1811e-5,(float)5860.,(float)
	    5870.,(float).013183,(float).084879,(float).018786,(float)
	    -7.5902e-5,(float).01503,(float)-6.7738e-5,(float)5870.,(float)
	    5880.,(float).0036488,(float).086695,(float).023765,(float)
	    -7.6455e-5,(float).019459,(float)-6.0338e-5,(float)5880.,(float)
	    5890.,(float).018172,(float).13595,(float).016681,(float)
	    -5.4016e-5,(float).014894,(float)-6.0543e-5,(float)5890.,(float)
	    5900.,(float).0024118,(float).072933,(float).025664,(float)
	    -9.6848e-5,(float).026465,(float)-1.0476e-4,(float)5900.,(float)
	    5910.,(float).01494,(float).1207,(float).017582,(float)-6.8945e-5,
	    (float).016946,(float)-6.9945e-5,(float)5910.,(float)5920.,(float)
	    .0014531,(float).081478,(float).02565,(float)-5.6688e-5,(float)
	    .020849,(float)-5.2418e-5,(float)5920.,(float)5930.,(float)
	    .004057,(float).064725,(float).024344,(float)-1.0574e-4,(float)
	    .021209,(float)-9.2148e-5,(float)5930.,(float)5940.,(float)
	    5.9362e-4,(float).052682,(float).023362,(float)-8.193e-5,(float)
	    .01857,(float)-7.6269e-5,(float)5940.,(float)5950.,(float)
	    .0021418,(float).075485,(float).023733,(float)-8.0625e-5,(float)
	    .021119,(float)-8.5514e-5,(float)5950.,(float)5960.,(float)
	    .0033439,(float).12209,(float).022556,(float)-8.973e-5,(float)
	    .019031,(float)-8.5459e-5,(float)5960.,(float)5970.,(float)
	    .0014641,(float).13608,(float).015568,(float)3.7809e-6,(float)
	    .01603,(float)-2.9177e-5,(float)5970.,(float)5980.,(float)
	    6.3999e-4,(float).094011,(float).027455,(float)-3.9873e-5,(float)
	    .019599,(float)-3.6883e-5,(float)5980.,(float)5990.,(float)
	    .0013405,(float).090254,(float).029576,(float)-1.1767e-4,(float)
	    .026759,(float)-1.1158e-4,(float)5990.,(float)6e3,(float).0017604,
	    (float).17547,(float).017851,(float)-6.4948e-5,(float).016287,(
	    float)-5.6437e-5,(float)6e3,(float)6010.,(float).0011569,(float)
	    .13348,(float).026563,(float)-8.7926e-5,(float).023528,(float)
	    -9.1174e-5,(float)6010.,(float)6020.,(float)2.8575e-4,(float)
	    .078833,(float).019053,(float)-1.8306e-5,(float).018303,(float)
	    -5.1186e-5,(float)6020.,(float)6030.,(float)8.7532e-4,(float)
	    .16112,(float).022556,(float)-3.3852e-5,(float).021206,(float)
	    -6.4336e-5,(float)6030.,(float)6040.,(float)4.8531e-4,(float)
	    .054398,(float).022035,(float)-9.7698e-5,(float).019943,(float)
	    -8.9334e-5,(float)6040.,(float)6050.,(float)4.2083e-4,(float)
	    .13305,(float).011783,(float)6.0192e-5,(float).01191,(float)
	    7.0965e-6,(float)6050.,(float)6060.,(float)3.1283e-4,(float)
	    .10974,(float).024828,(float)-3.5695e-5,(float).021926,(float)
	    -6.102e-5,(float)6060.,(float)6070.,(float).0016917,(float).14682,
	    (float).0023913,(float)1.937e-5,(float).0028143,(float)1.0914e-5,(
	    float)6070.,(float)6080.,(float)2.4107e-4,(float).056225,(float)
	    .019103,(float)-5.8306e-6,(float).02305,(float)-4.6373e-5,(float)
	    6080.,(float)6090.,(float)4.4079e-4,(float).073062,(float)
	    .0078845,(float)-3.8279e-5,(float).0064841,(float)-2.5252e-5,(
	    float)6090.,(float)6100.,(float)1.7156e-4,(float).08447,(float)
	    .010336,(float)1.0341e-4,(float).0085994,(float)4.3979e-5,(float)
	    6100.,(float)6110.,(float)1.5736e-4,(float).047359,(float).020348,
	    (float)-3.159e-5,(float).024132,(float)-5.6992e-5,(float)6110.,(
	    float)6120.,(float).0010666,(float).13835,(float).0061165,(float)
	    -8.7157e-6,(float).0071462,(float)-1.5e-5,(float)6120.,(float)
	    6130.,(float)4.1402e-5,(float).071564,(float).014541,(float)
	    -3.0661e-5,(float).013703,(float)-3.2448e-5,(float)6130.,(float)
	    6140.,(float)1.6673e-5,(float).033602,(float).019118,(float)
	    -8.8866e-5,(float).016523,(float)-8.3641e-5,(float)6140.,(float)
	    6150.,(float)4.5173e-4,(float).19416,(float).0081406,(float)
	    -4.6697e-6,(float).0096975,(float)-2.7594e-5,(float)6150.,(float)
	    6160.,(float)8.4192e-5,(float).0848,(float).0068868,(float)
	    2.0465e-5,(float).0065557,(float)-3.9158e-6,(float)6160.,(float)
	    6170.,(float)1.9983e-4,(float).034955,(float).013243,(float)
	    -4.8483e-5,(float).014163,(float)-4.8663e-5,(float)6170.,(float)
	    6180.,(float)5.654e-4,(float).092508,(float).0032982,(float)
	    -2.8242e-7,(float).0023537,(float)-1.1797e-5,(float)6180.,(float)
	    6190.,(float)2.6602e-4,(float).14864,(float)-.0035881,(float)
	    8.4292e-6,(float)-.0064288,(float)1.1323e-5,(float)6190.,(float)
	    6200.,(float)6.6332e-4,(float).082351,(float)4.0475e-5,(float)
	    -9.1191e-6,(float)-.0013832,(float)-7.433e-7,(float)6200.,(float)
	    6210.,(float)5.089e-4,(float).13947,(float).003362,(float)
	    -3.1112e-6,(float)4.078e-4,(float)8.6176e-7,(float)6210.,(float)
	    6220.,(float)1.2815e-4,(float).087906,(float).012262,(float)
	    -4.0572e-5,(float).01202,(float)-4.9984e-5,(float)6220.,(float)
	    6230.,(float)1.2854e-4,(float).12035,(float).0025735,(float)
	    -1.5152e-5,(float)-2.5291e-4,(float)-1.3133e-5,(float)6230.,(
	    float)6240.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6240.,(float)6250.,(float)3.8748e-4,(float)
	    .088892,(float).0070553,(float)-2.9374e-5,(float).0061982,(float)
	    -3.1937e-5,(float)6250.,(float)6260.,(float)1.6711e-4,(float)
	    .096035,(float).013535,(float)-5.983e-5,(float).011956,(float)
	    -5.7292e-5,(float)6260.,(float)6270.,(float)5.1664e-4,(float)
	    .11783,(float).0086933,(float)-2.7481e-5,(float).0070865,(float)
	    -2.8728e-5,(float)6270.,(float)6280.,(float).001613,(float).21378,
	    (float).0034321,(float)-1.5373e-5,(float).0028098,(float)
	    -8.7169e-6,(float)6280.,(float)6290.,(float)1.104e-4,(float)
	    .05027,(float).011461,(float)-5.2791e-5,(float).010667,(float)
	    -5.0501e-5,(float)6290.,(float)6300.,(float).0022339,(float)
	    .16944,(float).0043566,(float)-1.8834e-5,(float).0025925,(float)
	    -1.7523e-5,(float)6300.,(float)6310.,(float)7.0293e-4,(float)
	    .10055,(float).0086229,(float)-3.5378e-5,(float).0057065,(float)
	    -3.2557e-5,(float)6310.,(float)6320.,(float)2.805e-4,(float)
	    .12764,(float).0056648,(float)7.4065e-5,(float).0085368,(float)
	    1.7469e-5,(float)6320.,(float)6330.,(float)6.2308e-4,(float)
	    .089292,(float).01011,(float)-2.8113e-5,(float).010994,(float)
	    -3.9797e-5,(float)6330.,(float)6340.,(float)4.9924e-4,(float)
	    .15704,(float).014233,(float)-5.3536e-5,(float).011904,(float)
	    -5.3159e-5,(float)6340.,(float)6350.,(float)4.5485e-4,(float)
	    .2206,(float).0045598,(float)-2.1706e-6,(float).0040803,(float)
	    -1.9834e-5,(float)6350.,(float)6360.,(float)4.4859e-4,(float)
	    .31977,(float).015688,(float)-3.0431e-5,(float).012918,(float)
	    -4.8166e-5,(float)6360.,(float)6370.,(float)2.7811e-4,(float)
	    .13882,(float).011781,(float)-2.0096e-5,(float).011797,(float)
	    -2.376e-5,(float)6370.,(float)6380.,(float)8.6844e-4,(float).2569,
	    (float).011839,(float)-6.454e-6,(float).0097827,(float)-2.9934e-5,
	    (float)6380.,(float)6390.,(float)8.4205e-4,(float).26741,(float)
	    .010125,(float)-9.2493e-6,(float).009507,(float)-2.6314e-5,(float)
	    6390.,(float)6400.,(float).0016976,(float).15422,(float).0095094,(
	    float)1.9053e-5,(float).010682,(float)-7.9536e-6,(float)6400.,(
	    float)6410.,(float)4.7975e-4,(float).09426,(float).011121,(float)
	    -4.7922e-5,(float).0093837,(float)-4.1295e-5,(float)6410.,(float)
	    6420.,(float)5.2022e-4,(float).16038,(float).017528,(float)
	    -3.7366e-5,(float).019737,(float)-6.1655e-5,(float)6420.,(float)
	    6430.,(float)8.2574e-4,(float).16899,(float).016228,(float)
	    -5.7574e-5,(float).014443,(float)-6.2197e-5,(float)6430.,(float)
	    6440.,(float).0021296,(float).24095,(float).018574,(float)
	    -5.7065e-5,(float).018731,(float)-6.6716e-5,(float)6440.,(float)
	    6450.,(float)2.6201e-4,(float).11418,(float).021012,(float)
	    -6.7706e-5,(float).019337,(float)-6.9411e-5,(float)6450.,(float)
	    6460.,(float).0026631,(float).29606,(float).0094001,(float)
	    3.2264e-5,(float).0092032,(float)-9.2228e-6,(float)6460.,(float)
	    6470.,(float).0025496,(float).1395,(float).02196,(float)
	    -9.1558e-5,(float).019725,(float)-7.8434e-5,(float)6470.,(float)
	    6480.,(float).0030217,(float).24286,(float).021221,(float)
	    -4.9207e-5,(float).017707,(float)-5.6279e-5,(float)6480.,(float)
	    6490.,(float).0025925,(float).22189,(float).011972,(float)
	    -3.5111e-5,(float).011532,(float)-3.4767e-5,(float)6490.,(float)
	    6500.,(float).0036908,(float).17271,(float).017079,(float)
	    -6.2898e-5,(float).018276,(float)-6.9468e-5,(float)6500.,(float)
	    6510.,(float).0058412,(float).27765,(float).021239,(float)
	    -8.2779e-5,(float).019328,(float)-7.9739e-5,(float)6510.,(float)
	    6520.,(float).0010097,(float).36424,(float).014201,(float)
	    1.4574e-5,(float).014137,(float)-2.5464e-5,(float)6520.,(float)
	    6530.,(float).015531,(float).36032,(float).015962,(float)
	    -5.5747e-5,(float).013807,(float)-5.4878e-5,(float)6530.,(float)
	    6540.,(float).021827,(float).16998,(float).012887,(float)
	    -5.3561e-5,(float).013686,(float)-4.8168e-5,(float)6540.,(float)
	    6550.,(float).025524,(float).44895,(float).014216,(float)
	    -3.1158e-5,(float).013867,(float)-4.6536e-5,(float)6550.,(float)
	    6560.,(float).0046252,(float).14687,(float).017426,(float)
	    -7.1751e-5,(float).018048,(float)-6.5015e-5,(float)6560.,(float)
	    6570.,(float).022064,(float).27,(float).015309,(float)-5.8062e-5,(
	    float).016735,(float)-6.1409e-5,(float)6570.,(float)6580.,(float)
	    .042593,(float).28026,(float).015131,(float)-6.0674e-5,(float)
	    .014017,(float)-5.2957e-5,(float)6580.,(float)6590.,(float)
	    .039072,(float).14924,(float).0094106,(float)-4.0098e-5,(float)
	    .0098574,(float)-2.9732e-5,(float)6590.,(float)6600.,(float)
	    .054808,(float).37702,(float).012248,(float)-2.1938e-5,(float)
	    .012749,(float)-3.4984e-5,(float)6600.,(float)6610.,(float)
	    .036908,(float).16694,(float).011325,(float)-4.1839e-5,(float)
	    .012369,(float)-2.3977e-5,(float)6610.,(float)6620.,(float)
	    .0049128,(float).31728,(float).016178,(float)-8.7947e-6,(float)
	    .019507,(float)-3.6007e-5,(float)6620.,(float)6630.,(float)
	    .027419,(float).23237,(float).022088,(float)-4.83e-5,(float)
	    .021397,(float)-5.8784e-5,(float)6630.,(float)6640.,(float).09883,
	    (float).17529,(float).0067054,(float)-2.8981e-5,(float).0098183,(
	    float)-2.1164e-5,(float)6640.,(float)6650.,(float).097673,(float)
	    .26249,(float).015514,(float)-2.0366e-5,(float).018041,(float)
	    -4.7825e-5,(float)6650.,(float)6660.,(float).029234,(float).39325,
	    (float).011043,(float)6.9627e-6,(float).014995,(float)-2.8824e-5,(
	    float)6660.,(float)6670.,(float).073358,(float).26517,(float)
	    .022641,(float)-9.0861e-5,(float).020163,(float)-8.1954e-5,(float)
	    6670.,(float)6680.,(float).28609,(float).27963,(float).012746,(
	    float)-3.1154e-5,(float).011122,(float)-2.9019e-5,(float)6680.,(
	    float)6690.,(float).15415,(float).42581,(float).0092965,(float)
	    -1.6598e-5,(float).010866,(float)-3.0004e-5,(float)6690.,(float)
	    6700.,(float).50854,(float).25928,(float).011806,(float)
	    -4.9706e-5,(float).01195,(float)-4.4564e-5,(float)6700.,(float)
	    6710.,(float).54248,(float).29219,(float).0086655,(float)
	    -1.078e-5,(float).0096072,(float)-2.0886e-5,(float)6710.,(float)
	    6720.,(float)1.1829,(float).23177,(float).0084264,(float)
	    -3.6835e-5,(float).0078692,(float)-3.165e-5,(float)6720.,(float)
	    6730.,(float).66413,(float).22652,(float).0096388,(float)
	    -4.5502e-5,(float).0094036,(float)-4.2748e-5,(float)6730.,(float)
	    6740.,(float)2.0327,(float).2021,(float).0051382,(float)
	    -2.4524e-5,(float).0066314,(float)-1.6094e-5,(float)6740.,(float)
	    6750.,(float)2.8639,(float).19875,(float).0048057,(float)
	    -2.8413e-5,(float).0044821,(float)-2.1857e-5,(float)6750.,(float)
	    6760.,(float)2.3946,(float).11681,(float).0025701,(float)
	    -2.0163e-5,(float).0026061,(float)-1.6227e-5,(float)6760.,(float)
	    6770.,(float)2.3817,(float).35427,(float).0039392,(float)
	    -1.7028e-5,(float).004929,(float)-1.8773e-5,(float)6770.,(float)
	    6780.,(float)1.6155,(float).23449,(float)7.3307e-4,(float)
	    -7.3637e-6,(float).0016677,(float)-2.5528e-6,(float)6780.,(float)
	    6790.,(float)8.3481,(float).29624,(float)2.1533e-4,(float)
	    -8.9091e-6,(float)-1.954e-4,(float)-6.9089e-6,(float)6790.,(float)
	    6800.,(float)3.9018,(float).13513,(float)-.002043,(float)
	    4.7043e-6,(float).0019385,(float)1.5453e-5,(float)6800.,(float)
	    6810.,(float)3.3911,(float).24098,(float)-2.0667e-4,(float)
	    -6.3633e-6,(float)-1.6731e-4,(float)-2.2699e-6,(float)6810.,(
	    float)6820.,(float)2.8798,(float).25576,(float)-.0021074,(float)
	    5.4951e-6,(float)-.0021041,(float)8.2116e-6,(float)6820.,(float)
	    6830.,(float)2.7426,(float).15514,(float)-.0029057,(float)
	    1.2463e-5,(float).0017031,(float)1.9513e-5,(float)6830.,(float)
	    6840.,(float)3.2308,(float).18429,(float)-.0036805,(float)
	    1.3879e-5,(float)3.8114e-4,(float)2.111e-5,(float)6840.,(float)
	    6850.,(float).82936,(float).18124,(float)4.1735e-4,(float)
	    1.4616e-6,(float).0025198,(float)-1.3464e-6,(float)6850.,(float)
	    6860.,(float).35131,(float).41646,(float).015485,(float)-4.647e-5,
	    (float).013388,(float)-3.6422e-5,(float)6860.,(float)6870.,(float)
	    4.6683,(float).15324,(float)-.0042182,(float)9.0437e-6,(float)
	    -.0013329,(float)1.8577e-5,(float)6870.,(float)6880.,(float)
	    2.8455,(float).17389,(float)-.0022094,(float)1.4962e-6,(float)
	    -.0015658,(float)9.7344e-6,(float)6880.,(float)6890.,(float)
	    8.8724,(float).42657,(float)-.0018169,(float)8.1105e-6,(float)
	    -6.5751e-4,(float)1.2321e-5,(float)6890.,(float)6900.,(float)
	    .81595,(float).32457,(float).0028256,(float)1.2078e-6,(float)
	    .0059842,(float)1.543e-6,(float)6900.,(float)6910.,(float)12.055,(
	    float).40831,(float)-.0011236,(float)2.2677e-6,(float)-8.159e-4,(
	    float)1.7078e-6,(float)6910.,(float)6920.,(float)1.4005,(float)
	    .42874,(float).0073135,(float)-1.6167e-5,(float).0082389,(float)
	    -1.8229e-5,(float)6920.,(float)6930.,(float)9.4914,(float).29494,(
	    float)-.0033677,(float)1.1156e-5,(float)-.0016627,(float)
	    1.6279e-5,(float)6930.,(float)6940.,(float)3.731,(float).44986,(
	    float).0084547,(float)-3.0585e-5,(float).010303,(float)-3.5351e-5,
	    (float)6940.,(float)6950.,(float)7.9581,(float).26975,(float)
	    -.0014256,(float)4.1782e-6,(float)1.3886e-4,(float)7.5184e-6,(
	    float)6950.,(float)6960.,(float)3.409,(float).33223,(float)
	    -8.355e-4,(float)9.9982e-6,(float).0032926,(float)7.9289e-6,(
	    float)6960.,(float)6970.,(float)6.9934,(float).49549,(float)
	    .0034299,(float)1.3868e-5,(float).0073246,(float)-1.1357e-5,(
	    float)6970.,(float)6980.,(float)8.4288,(float).29235,(float)
	    .0012515,(float)-6.6867e-6,(float).0022057,(float)-7.2817e-6,(
	    float)6980.,(float)6990.,(float)3.7771,(float).34377,(float)
	    .0066839,(float)1.4877e-5,(float).0099074,(float)-2.4726e-5,(
	    float)6990.,(float)7e3,(float)5.9358,(float).4452,(float).0047532,
	    (float)-1.1726e-5,(float).0057444,(float)-1.6773e-5,(float)7e3,(
	    float)7010.,(float)3.6653,(float).46656,(float).0075924,(float)
	    -1.4859e-5,(float).0085483,(float)-2.7681e-5,(float)7010.,(float)
	    7020.,(float)7.623,(float).59145,(float).010355,(float)-2.3877e-5,
	    (float).0084375,(float)-3.5191e-5,(float)7020.,(float)7030.,(
	    float)2.9785,(float).49616,(float).0095318,(float)-2.8442e-5,(
	    float).0094393,(float)-3.7258e-5,(float)7030.,(float)7040.,(float)
	    11.489,(float).4463,(float).012118,(float)-4.7464e-5,(float)
	    .0089557,(float)-4.3911e-5,(float)7040.,(float)7050.,(float)5.707,
	    (float).30579,(float).012792,(float)-5.848e-5,(float).011943,(
	    float)-4.9567e-5,(float)7050.,(float)7060.,(float)4.3341,(float)
	    .45254,(float).0087102,(float)-3.6977e-5,(float).0062554,(float)
	    -2.9678e-5,(float)7060.,(float)7070.,(float)18.415,(float).27325,(
	    float).011045,(float)-5.5565e-5,(float).0087811,(float)-4.6635e-5,
	    (float)7070.,(float)7080.,(float)9.2559,(float).37006,(float)
	    .0093565,(float)-4.6638e-5,(float).008623,(float)-4.1694e-5,(
	    float)7080.,(float)7090.,(float)34.399,(float).2696,(float)
	    .0073722,(float)-4.0297e-5,(float).005669,(float)-3.4996e-5,(
	    float)7090.,(float)7100.,(float)27.357,(float).28185,(float)
	    .0041186,(float)-2.4656e-5,(float).0015333,(float)-1.4846e-5,(
	    float)7100.,(float)7110.,(float)59.142,(float).21235,(float)
	    .004202,(float)-2.8298e-5,(float).0026623,(float)-2.2726e-5,(
	    float)7110.,(float)7120.,(float)8.729,(float).31743,(float)
	    .0050667,(float)-2.9057e-5,(float).0050497,(float)-2.4756e-5,(
	    float)7120.,(float)7130.,(float)66.372,(float).25989,(float)
	    .0021171,(float)-2.001e-5,(float)9.8789e-4,(float)-1.5045e-5,(
	    float)7130.,(float)7140.,(float)40.254,(float).15248,(float)
	    6.4299e-4,(float)-1.3315e-5,(float)-6.7491e-4,(float)-2.9781e-7,(
	    float)7140.,(float)7150.,(float)1.5991,(float).45391,(float)
	    .0083749,(float)-1.2893e-5,(float).011832,(float)-2.3466e-5,(
	    float)7150.,(float)7160.,(float)138.4,(float).26185,(float)
	    -9.2005e-4,(float)-7.5058e-6,(float)-.0022766,(float)1.2031e-6,(
	    float)7160.,(float)7170.,(float)13.476,(float).27958,(float)
	    1.7108e-4,(float)2.8602e-6,(float).0019003,(float)2.2534e-6,(
	    float)7170.,(float)7180.,(float)95.96,(float).20582,(float)
	    -.0022657,(float)1.3812e-6,(float)-.0015351,(float)8.8048e-6,(
	    float)7180.,(float)7190.,(float)14.69,(float).24608,(float)
	    7.8336e-4,(float)1.5252e-5,(float).003739,(float)-6.918e-6,(float)
	    7190.,(float)7200.,(float)55.288,(float).25862,(float)-.0013454,(
	    float)9.9116e-6,(float)-8.8298e-4,(float)1.7423e-6,(float)7200.,(
	    float)7210.,(float)48.147,(float).25568,(float).0049183,(float)
	    -2.3894e-5,(float).0037052,(float)-2.2724e-5,(float)7210.,(float)
	    7220.,(float)40.415,(float).20103,(float)-.0024259,(float)
	    7.5229e-6,(float)-.0017525,(float)2.6464e-6,(float)7220.,(float)
	    7230.,(float)109.84,(float).33075,(float)7.5622e-4,(float)
	    -1.2921e-5,(float)-.0011898,(float)-5.9125e-6,(float)7230.,(float)
	    7240.,(float)130.02,(float).18533,(float)-.0033239,(float)
	    1.8173e-6,(float)-.0046918,(float)8.1132e-6,(float)7240.,(float)
	    7250.,(float)24.491,(float).30951,(float)-.0026793,(float)
	    5.8588e-6,(float)-.0021237,(float)4.8e-6,(float)7250.,(float)
	    7260.,(float)19.609,(float).15703,(float)-.0025743,(float)
	    2.8622e-6,(float)-.0016255,(float)4.4422e-6,(float)7260.,(float)
	    7270.,(float)17.072,(float).26657,(float)-.0029675,(float)
	    1.2335e-5,(float)-6.9569e-4,(float)7.7744e-6,(float)7270.,(float)
	    7280.,(float)25.243,(float).38981,(float)-.0023499,(float)
	    7.2438e-6,(float)-.0015088,(float)4.0106e-6,(float)7280.,(float)
	    7290.,(float)139.62,(float).16193,(float)-.0050479,(float)
	    9.269e-6,(float)-.0053276,(float)1.5266e-5,(float)7290.,(float)
	    7300.,(float)86.705,(float).11761,(float)-.0037859,(float)
	    4.4125e-6,(float)-.0030774,(float)1.0295e-5,(float)7300.,(float)
	    7310.,(float)66.458,(float).22506,(float)-.0031197,(float)
	    2.1671e-6,(float)-.003942,(float)5.6842e-6,(float)7310.,(float)
	    7320.,(float)147.42,(float).24611,(float)-.0023854,(float)
	    -1.4496e-6,(float)-.0034285,(float)1.2805e-6,(float)7320.,(float)
	    7330.,(float)87.291,(float).22526,(float)-1.1541e-4,(float)
	    -9.6671e-6,(float)-9.0479e-4,(float)-7.8703e-6,(float)7330.,(
	    float)7340.,(float)155.36,(float).27754,(float)-2.3889e-4,(float)
	    -8.8494e-6,(float)-.001057,(float)-7.3024e-6,(float)7340.,(float)
	    7350.,(float)60.8,(float).21141,(float).0022877,(float)-1.825e-5,(
	    float).0021735,(float)-1.6084e-5,(float)7350.,(float)7360.,(float)
	    65.821,(float).31454,(float).0035102,(float)-2.3014e-5,(float)
	    .0021764,(float)-1.996e-5,(float)7360.,(float)7370.,(float)39.314,
	    (float).19452,(float).0045708,(float)-2.8104e-5,(float).0035157,(
	    float)-2.3151e-5,(float)7370.,(float)7380.,(float)33.606,(float)
	    .30823,(float).0071777,(float)-3.6203e-5,(float).0052193,(float)
	    -2.5338e-5,(float)7380.,(float)7390.,(float)28.667,(float).22825,(
	    float).0067636,(float)-3.404e-5,(float).0062429,(float)-3.5213e-5,
	    (float)7390.,(float)7400.,(float)9.5852,(float).3028,(float)
	    .012912,(float)-5.3899e-5,(float).010209,(float)-4.4318e-5,(float)
	    7400.,(float)7410.,(float)7.5752,(float).36859,(float).010673,(
	    float)-1.5633e-5,(float).0094631,(float)-3.3501e-5,(float)7410.,(
	    float)7420.,(float)3.3599,(float).31735,(float).015132,(float)
	    -5.0575e-5,(float).013714,(float)-4.8842e-5,(float)7420.,(float)
	    7430.,(float)1.421,(float).51877,(float).010564,(float)2.7798e-5,(
	    float).010716,(float)-1.6576e-5,(float)7430.,(float)7440.,(float)
	    1.3357,(float).34135,(float).0055009,(float)2.9075e-5,(float)
	    .010731,(float)-1.4716e-5,(float)7440.,(float)7450.,(float).90292,
	    (float).35257,(float).0064439,(float)4.3621e-6,(float).0093295,(
	    float)-7.9773e-6,(float)7450.,(float)7460.,(float).47321,(float)
	    .34413,(float).0075753,(float)3.1118e-5,(float).0088184,(float)
	    2.3143e-6,(float)7460.,(float)7470.,(float).14277,(float).63005,(
	    float).015765,(float)-3.876e-5,(float).014284,(float)-4.1344e-5,(
	    float)7470.,(float)7480.,(float).84751,(float).092347,(float)
	    .0044433,(float)-2.3373e-5,(float).0065281,(float)-2.0814e-5,(
	    float)7480.,(float)7490.,(float).34962,(float).49346,(float)
	    3.9951e-4,(float)2.823e-6,(float).0021242,(float)-6.058e-7,(float)
	    7490.,(float)7500.,(float).66686,(float).32642,(float).0015242,(
	    float)-8.7602e-6,(float).0033093,(float)-6.2194e-6,(float)7500.,(
	    float)7510.,(float).7038,(float).37824,(float).0040335,(float)
	    -1.7144e-5,(float).0058765,(float)-1.8563e-5,(float)7510.,(float)
	    7520.,(float).44176,(float).28314,(float).0041609,(float)
	    -2.1428e-5,(float).0052238,(float)-2.1647e-5,(float)7520.,(float)
	    7530.,(float).23859,(float).25536,(float).0046384,(float)
	    -3.1469e-6,(float).0056672,(float)-1.3268e-5,(float)7530.,(float)
	    7540.,(float).12451,(float).19246,(float).003173,(float)-1.645e-5,
	    (float).0046905,(float)-1.9919e-5,(float)7540.,(float)7550.,(
	    float).40142,(float).2566,(float).0068917,(float)-2.2016e-5,(
	    float).0042921,(float)-2.2533e-5,(float)7550.,(float)7560.,(float)
	    .24593,(float).28458,(float)3.567e-4,(float)-9.0627e-7,(float)
	    6.7869e-4,(float)-6.7459e-6,(float)7560.,(float)7570.,(float)
	    .41189,(float).23957,(float)-2.7548e-4,(float)3.566e-6,(float)
	    3.9542e-4,(float)-3.8485e-6,(float)7570.,(float)7580.,(float)
	    .20341,(float).14662,(float).0060408,(float)-3.5022e-5,(float)
	    .0043079,(float)-3.1225e-5,(float)7580.,(float)7590.,(float)
	    .18188,(float).14279,(float).0012602,(float)-3.0833e-6,(float)
	    .0033638,(float)-1.3417e-5,(float)7590.,(float)7600.,(float)
	    .17978,(float).35062,(float).005044,(float)-3.1112e-6,(float)
	    .0036165,(float)-1.3022e-5,(float)7600.,(float)7610.,(float)
	    .31504,(float).16627,(float).003265,(float)-1.3345e-5,(float)
	    .0040505,(float)-2.0124e-5,(float)7610.,(float)7620. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/

/*     WATER VAPOR (5060 - 7600 CM-1) */

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
} /* wava2_ */

#ifdef __cplusplus
	}
#endif
