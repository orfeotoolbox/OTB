/* SOOT.f -- translated by f2c (version 19970805).
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

/*<        subroutine   soot >*/
/* Subroutine */ int soot_()
{
    /* Initialized data */

    static real asy_m__[20] = { (float).423,(float).397,(float).391,(float)
	    .377,(float).365,(float).358,(float).348,(float).337,(float).324,(
	    float).31,(float).299,(float).293,(float).276,(float).253,(float)
	    .188,(float).155,(float).144,(float).121,(float).103,(float).055 }
	    ;
    static real ex_m__[20] = { (float).001017724,(float)8.614733e-4,(float)
	    8.291564e-4,(float)7.534558e-4,(float)6.955619e-4,(float)
	    6.605808e-4,(float)6.115299e-4,(float)5.541574e-4,(float)
	    5.00754e-4,(float)4.542466e-4,(float)4.189021e-4,(float)
	    3.975466e-4,(float)3.507939e-4,(float)2.984524e-4,(float)
	    1.898267e-4,(float)1.479603e-4,(float)1.378559e-4,(float)
	    1.161122e-4,(float)1.005532e-4,(float)6.124496e-5 };
    static real sc_m__[20] = { (float)2.956808e-4,(float)2.301183e-4,(float)
	    2.170554e-4,(float)1.871983e-4,(float)1.651221e-4,(float)
	    1.521293e-4,(float)1.348011e-4,(float)1.156599e-4,(float)
	    9.774679e-5,(float)8.21856e-5,(float)7.11141e-5,(float)
	    6.486094e-5,(float)5.093826e-5,(float)3.621259e-5,(float)
	    1.251277e-5,(float)6.450522e-6,(float)5.127908e-6,(float)
	    2.946072e-6,(float)1.794028e-6,(float)3.05519e-7 };
    static real phr[1660]	/* was [20][83] */ = { (float).4446,(float)
	    .4907,(float).5012,(float).5274,(float).5491,(float).5632,(float)
	    .5829,(float).6066,(float).6338,(float).6626,(float).6857,(float)
	    .6997,(float).7383,(float).7925,(float).9497,(float)1.036,(float)
	    1.065,(float)1.129,(float)1.18,(float)1.325,(float).4445,(float)
	    .4905,(float).501,(float).5272,(float).549,(float).563,(float)
	    .5827,(float).6064,(float).6336,(float).6624,(float).6855,(float)
	    .6995,(float).738,(float).7922,(float).9494,(float)1.036,(float)
	    1.065,(float)1.129,(float)1.18,(float)1.324,(float).444,(float)
	    .49,(float).5005,(float).5266,(float).5483,(float).5623,(float)
	    .582,(float).6057,(float).6328,(float).6616,(float).6846,(float)
	    .6986,(float).737,(float).7911,(float).9479,(float)1.034,(float)
	    1.063,(float)1.127,(float)1.178,(float)1.322,(float).4431,(float)
	    .489,(float).4995,(float).5255,(float).5472,(float).5611,(float)
	    .5807,(float).6043,(float).6314,(float).6601,(float).683,(float)
	    .6969,(float).7352,(float).7892,(float).9454,(float)1.031,(float)
	    1.06,(float)1.124,(float)1.174,(float)1.318,(float).4419,(float)
	    .4876,(float).498,(float).524,(float).5455,(float).5594,(float)
	    .5789,(float).6024,(float).6293,(float).6579,(float).6808,(float)
	    .6946,(float).7327,(float).7863,(float).9417,(float)1.027,(float)
	    1.056,(float)1.119,(float)1.169,(float)1.311,(float).4403,(float)
	    .4858,(float).4961,(float).522,(float).5434,(float).5572,(float)
	    .5766,(float).6,(float).6267,(float).6551,(float).6778,(float)
	    .6915,(float).7294,(float).7827,(float).9369,(float)1.022,(float)
	    1.05,(float)1.113,(float)1.163,(float)1.304,(float).4384,(float)
	    .4836,(float).4939,(float).5195,(float).5408,(float).5545,(float)
	    .5738,(float).597,(float).6235,(float).6517,(float).6742,(float)
	    .6878,(float).7254,(float).7782,(float).9311,(float)1.015,(float)
	    1.043,(float)1.105,(float)1.155,(float)1.294,(float).4363,(float)
	    .4811,(float).4913,(float).5167,(float).5378,(float).5514,(float)
	    .5705,(float).5935,(float).6198,(float).6476,(float).67,(float)
	    .6835,(float).7207,(float).773,(float).9242,(float)1.007,(float)
	    1.035,(float)1.096,(float)1.145,(float)1.283,(float).4339,(float)
	    .4783,(float).4884,(float).5135,(float).5345,(float).5479,(float)
	    .5668,(float).5895,(float).6155,(float).6431,(float).6652,(float)
	    .6785,(float).7153,(float).767,(float).9164,(float).9983,(float)
	    1.026,(float)1.086,(float)1.134,(float)1.27,(float).4312,(float)
	    .4752,(float).4852,(float).5101,(float).5307,(float).544,(float)
	    .5627,(float).5851,(float).6108,(float).6381,(float).6599,(float)
	    .673,(float).7093,(float).7604,(float).9076,(float).9883,(float)
	    1.015,(float)1.075,(float)1.122,(float)1.255,(float).4284,(float)
	    .4719,(float).4817,(float).5063,(float).5267,(float).5398,(float)
	    .5582,(float).5803,(float).6057,(float).6326,(float).654,(float)
	    .667,(float).7028,(float).7531,(float).898,(float).9773,(float)
	    1.004,(float)1.062,(float)1.109,(float)1.239,(float).4255,(float)
	    .4684,(float).4781,(float).5023,(float).5224,(float).5353,(float)
	    .5535,(float).5753,(float).6002,(float).6267,(float).6478,(float)
	    .6606,(float).6958,(float).7452,(float).8876,(float).9654,(float)
	    .9916,(float)1.049,(float)1.094,(float)1.222,(float).4225,(float)
	    .4647,(float).4743,(float).4981,(float).5179,(float).5306,(float)
	    .5485,(float).5699,(float).5944,(float).6204,(float).6412,(float)
	    .6537,(float).6883,(float).7369,(float).8765,(float).9527,(float)
	    .9784,(float)1.034,(float)1.079,(float)1.203,(float).4195,(float)
	    .461,(float).4704,(float).4938,(float).5133,(float).5258,(float)
	    .5433,(float).5643,(float).5883,(float).6139,(float).6342,(float)
	    .6465,(float).6804,(float).728,(float).8647,(float).9392,(float)
	    .9643,(float)1.019,(float)1.062,(float)1.184,(float).4165,(float)
	    .4572,(float).4665,(float).4895,(float).5085,(float).5208,(float)
	    .5379,(float).5585,(float).5821,(float).6071,(float).627,(float)
	    .6391,(float).6723,(float).7188,(float).8524,(float).9251,(float)
	    .9495,(float)1.003,(float)1.045,(float)1.163,(float).4135,(float)
	    .4535,(float).4625,(float).4851,(float).5037,(float).5157,(float)
	    .5325,(float).5527,(float).5757,(float).6002,(float).6197,(float)
	    .6314,(float).6638,(float).7093,(float).8396,(float).9103,(float)
	    .9341,(float).9858,(float)1.027,(float)1.141,(float).4106,(float)
	    .4498,(float).4586,(float).4807,(float).499,(float).5107,(float)
	    .5271,(float).5467,(float).5693,(float).5931,(float).6122,(float)
	    .6236,(float).6553,(float).6996,(float).8264,(float).8952,(float)
	    .9183,(float).9684,(float)1.008,(float)1.119,(float).4079,(float)
	    .4462,(float).4548,(float).4764,(float).4942,(float).5057,(float)
	    .5217,(float).5409,(float).5628,(float).5861,(float).6046,(float)
	    .6158,(float).6466,(float).6898,(float).813,(float).8796,(float)
	    .902,(float).9505,(float).9891,(float)1.096,(float).4053,(float)
	    .4428,(float).4512,(float).4723,(float).4897,(float).5008,(float)
	    .5164,(float).5351,(float).5564,(float).5791,(float).5971,(float)
	    .608,(float).6379,(float).6798,(float).7994,(float).8639,(float)
	    .8855,(float).9324,(float).9696,(float)1.073,(float).4031,(float)
	    .4396,(float).4478,(float).4683,(float).4853,(float).4961,(float)
	    .5113,(float).5294,(float).5502,(float).5722,(float).5897,(float)
	    .6002,(float).6293,(float).67,(float).7857,(float).848,(float)
	    .8689,(float).9141,(float).9499,(float)1.049,(float).4011,(float)
	    .4367,(float).4447,(float).4647,(float).4811,(float).4917,(float)
	    .5064,(float).524,(float).5442,(float).5655,(float).5825,(float)
	    .5927,(float).6209,(float).6602,(float).7721,(float).8321,(float)
	    .8522,(float).8957,(float).9301,(float)1.025,(float).3995,(float)
	    .4341,(float).4419,(float).4613,(float).4773,(float).4876,(float)
	    .5019,(float).5189,(float).5385,(float).5592,(float).5756,(float)
	    .5855,(float).6127,(float).6507,(float).7586,(float).8163,(float)
	    .8357,(float).8774,(float).9104,(float)1.001,(float).3983,(float)
	    .432,(float).4396,(float).4584,(float).474,(float).4839,(float)
	    .4977,(float).5143,(float).5332,(float).5532,(float).569,(float)
	    .5786,(float).6049,(float).6416,(float).7454,(float).8008,(float)
	    .8193,(float).8593,(float).8908,(float).9774,(float).3975,(float)
	    .4303,(float).4377,(float).456,(float).471,(float).4807,(float)
	    .4941,(float).51,(float).5283,(float).5476,(float).563,(float)
	    .5721,(float).5975,(float).6329,(float).7326,(float).7856,(float)
	    .8034,(float).8416,(float).8716,(float).9539,(float).3974,(float)
	    .4292,(float).4364,(float).4541,(float).4687,(float).478,(float)
	    .491,(float).5064,(float).524,(float).5427,(float).5574,(float)
	    .5663,(float).5907,(float).6247,(float).7203,(float).771,(float)
	    .788,(float).8243,(float).8529,(float).931,(float).3978,(float)
	    .4288,(float).4357,(float).4529,(float).467,(float).476,(float)
	    .4885,(float).5034,(float).5204,(float).5383,(float).5526,(float)
	    .5611,(float).5845,(float).6171,(float).7087,(float).757,(float)
	    .7731,(float).8077,(float).8349,(float).9086,(float).3989,(float)
	    .429,(float).4357,(float).4523,(float).466,(float).4747,(float)
	    .4868,(float).5011,(float).5175,(float).5348,(float).5484,(float)
	    .5566,(float).5791,(float).6104,(float).6978,(float).7438,(float)
	    .7591,(float).7919,(float).8175,(float).8871,(float).4008,(float)
	    .43,(float).4365,(float).4526,(float).4658,(float).4742,(float)
	    .4859,(float).4997,(float).5155,(float).5321,(float).5452,(float)
	    .553,(float).5746,(float).6045,(float).6878,(float).7315,(float)
	    .746,(float).7769,(float).8012,(float).8664,(float).4035,(float)
	    .4319,(float).4382,(float).4538,(float).4666,(float).4747,(float)
	    .4859,(float).4992,(float).5144,(float).5303,(float).5429,(float)
	    .5504,(float).571,(float).5996,(float).6789,(float).7202,(float)
	    .7339,(float).7631,(float).7858,(float).8469,(float).4071,(float)
	    .4347,(float).4408,(float).456,(float).4683,(float).4761,(float)
	    .487,(float).4998,(float).5143,(float).5296,(float).5417,(float)
	    .5489,(float).5686,(float).5958,(float).6711,(float).7101,(float)
	    .723,(float).7504,(float).7717,(float).8286,(float).4117,(float)
	    .4386,(float).4446,(float).4592,(float).4711,(float).4787,(float)
	    .4891,(float).5015,(float).5155,(float).5301,(float).5417,(float)
	    .5485,(float).5673,(float).5933,(float).6646,(float).7013,(float)
	    .7134,(float).739,(float).7589,(float).8117,(float).4174,(float)
	    .4436,(float).4494,(float).4636,(float).4752,(float).4825,(float)
	    .4926,(float).5044,(float).5179,(float).5319,(float).5429,(float)
	    .5495,(float).5674,(float).5921,(float).6595,(float).6939,(float)
	    .7052,(float).7291,(float).7475,(float).7962,(float).4244,(float)
	    .45,(float).4556,(float).4694,(float).4805,(float).4876,(float)
	    .4973,(float).5088,(float).5217,(float).5351,(float).5457,(float)
	    .5519,(float).569,(float).5924,(float).6559,(float).6881,(float)
	    .6986,(float).7207,(float).7378,(float).7824,(float).4327,(float)
	    .4576,(float).4631,(float).4765,(float).4873,(float).4942,(float)
	    .5036,(float).5146,(float).527,(float).5398,(float).5499,(float)
	    .5559,(float).5721,(float).5942,(float).654,(float).6839,(float)
	    .6937,(float).7141,(float).7297,(float).7703,(float).4424,(float)
	    .4668,(float).4722,(float).4852,(float).4957,(float).5023,(float)
	    .5114,(float).522,(float).5339,(float).5462,(float).5558,(float)
	    .5615,(float).5769,(float).5978,(float).6538,(float).6816,(float)
	    .6905,(float).7092,(float).7234,(float).7601,(float).4537,(float)
	    .4776,(float).4829,(float).4955,(float).5057,(float).5121,(float)
	    .5209,(float).5311,(float).5426,(float).5543,(float).5635,(float)
	    .5689,(float).5835,(float).6032,(float).6555,(float).6811,(float)
	    .6893,(float).7063,(float).7191,(float).7518,(float).4666,(float)
	    .4902,(float).4953,(float).5077,(float).5176,(float).5238,(float)
	    .5323,(float).5422,(float).5531,(float).5644,(float).5731,(float)
	    .5782,(float).592,(float).6106,(float).6592,(float).6826,(float)
	    .69,(float).7054,(float).7168,(float).7456,(float).4814,(float)
	    .5046,(float).5096,(float).5217,(float).5314,(float).5374,(float)
	    .5456,(float).5552,(float).5657,(float).5764,(float).5847,(float)
	    .5896,(float).6026,(float).62,(float).6649,(float).6862,(float)
	    .6929,(float).7065,(float).7166,(float).7415,(float).4982,(float)
	    .5211,(float).526,(float).5378,(float).5472,(float).5531,(float)
	    .5611,(float).5703,(float).5804,(float).5907,(float).5985,(float)
	    .6032,(float).6154,(float).6316,(float).6729,(float).6921,(float)
	    .6979,(float).7099,(float).7186,(float).7395,(float).5172,(float)
	    .5398,(float).5446,(float).5562,(float).5654,(float).571,(float)
	    .5788,(float).5877,(float).5974,(float).6072,(float).6146,(float)
	    .619,(float).6305,(float).6455,(float).6832,(float).7001,(float)
	    .7053,(float).7155,(float).7228,(float).7398,(float).5384,(float)
	    .5608,(float).5656,(float).5769,(float).5858,(float).5914,(float)
	    .5989,(float).6075,(float).6168,(float).6261,(float).6331,(float)
	    .6372,(float).648,(float).6618,(float).6958,(float).7106,(float)
	    .7149,(float).7234,(float).7293,(float).7424,(float).55,(float)
	    .5723,(float).577,(float).5882,(float).597,(float).6025,(float)
	    .6099,(float).6184,(float).6275,(float).6365,(float).6433,(float)
	    .6473,(float).6577,(float).6709,(float).703,(float).7167,(float)
	    .7206,(float).7283,(float).7335,(float).7446,(float).5622,(float)
	    .5843,(float).589,(float).6002,(float).6089,(float).6143,(float)
	    .6215,(float).6299,(float).6388,(float).6475,(float).6542,(float)
	    .658,(float).668,(float).6807,(float).7109,(float).7234,(float)
	    .727,(float).7337,(float).7382,(float).7473,(float).5886,(float)
	    .6106,(float).6152,(float).6261,(float).6346,(float).6398,(float)
	    .6469,(float).655,(float).6634,(float).6717,(float).6779,(float)
	    .6815,(float).6907,(float).7021,(float).7284,(float).7387,(float)
	    .7414,(float).7464,(float).7495,(float).7545,(float).6179,(float)
	    .6397,(float).6443,(float).655,(float).6632,(float).6683,(float)
	    .6751,(float).6829,(float).6909,(float).6986,(float).7044,(float)
	    .7077,(float).7161,(float).7263,(float).7486,(float).7565,(float)
	    .7584,(float).7616,(float).7631,(float).764,(float).6503,(float)
	    .6719,(float).6764,(float).6869,(float).6949,(float).6998,(float)
	    .7063,(float).7138,(float).7214,(float).7285,(float).7338,(float)
	    .7369,(float).7444,(float).7532,(float).7714,(float).7768,(float)
	    .7779,(float).7792,(float).7792,(float).7758,(float).6861,(float)
	    .7074,(float).7118,(float).722,(float).7298,(float).7344,(float)
	    .7407,(float).7478,(float).7549,(float).7615,(float).7663,(float)
	    .769,(float).7756,(float).783,(float).7968,(float).7997,(float)
	    .7999,(float).7992,(float).7977,(float).79,(float).7254,(float)
	    .7464,(float).7506,(float).7605,(float).768,(float).7725,(float)
	    .7784,(float).7851,(float).7917,(float).7976,(float).8019,(float)
	    .8043,(float).8099,(float).8158,(float).8249,(float).8252,(float)
	    .8244,(float).8217,(float).8185,(float).8063,(float).7685,(float)
	    .789,(float).7932,(float).8027,(float).8098,(float).814,(float)
	    .8196,(float).8259,(float).8318,(float).837,(float).8407,(float)
	    .8428,(float).8473,(float).8516,(float).8557,(float).8532,(float)
	    .8514,(float).8466,(float).8417,(float).8249,(float).8156,(float)
	    .8356,(float).8396,(float).8487,(float).8554,(float).8593,(float)
	    .8645,(float).8702,(float).8754,(float).8799,(float).8829,(float)
	    .8845,(float).8879,(float).8904,(float).8892,(float).8836,(float)
	    .8809,(float).8738,(float).8672,(float).8455,(float).867,(float)
	    .8864,(float).8901,(float).8987,(float).9048,(float).9084,(float)
	    .9131,(float).9182,(float).9227,(float).9262,(float).9285,(float)
	    .9297,(float).9317,(float).9324,(float).9254,(float).9166,(float)
	    .9127,(float).9034,(float).8949,(float).8682,(float).9231,(float)
	    .9415,(float).945,(float).9528,(float).9584,(float).9616,(float)
	    .9657,(float).9701,(float).9736,(float).9761,(float).9776,(float)
	    .9783,(float).9788,(float).9774,(float).9642,(float).952,(float)
	    .947,(float).9352,(float).9248,(float).8928,(float).984,(float)
	    1.001,(float)1.004,(float)1.011,(float)1.016,(float)1.019,(float)
	    1.022,(float)1.026,(float)1.028,(float)1.03,(float)1.03,(float)
	    1.03,(float)1.029,(float)1.026,(float)1.006,(float).9897,(float)
	    .9835,(float).9691,(float).9567,(float).9193,(float)1.05,(float)
	    1.066,(float)1.069,(float)1.075,(float)1.078,(float)1.081,(float)
	    1.083,(float)1.086,(float)1.087,(float)1.087,(float)1.087,(float)
	    1.086,(float)1.083,(float)1.077,(float)1.049,(float)1.03,(float)
	    1.022,(float)1.005,(float).9905,(float).9475,(float)1.122,(float)
	    1.135,(float)1.138,(float)1.142,(float)1.145,(float)1.147,(float)
	    1.148,(float)1.15,(float)1.15,(float)1.148,(float)1.146,(float)
	    1.145,(float)1.14,(float)1.131,(float)1.096,(float)1.072,(float)
	    1.063,(float)1.043,(float)1.026,(float).9772,(float)1.199,(float)
	    1.21,(float)1.212,(float)1.215,(float)1.217,(float)1.217,(float)
	    1.218,(float)1.218,(float)1.216,(float)1.213,(float)1.21,(float)
	    1.208,(float)1.201,(float)1.188,(float)1.144,(float)1.116,(float)
	    1.106,(float)1.082,(float)1.063,(float)1.008,(float)1.282,(float)
	    1.29,(float)1.291,(float)1.293,(float)1.293,(float)1.293,(float)
	    1.292,(float)1.29,(float)1.287,(float)1.282,(float)1.277,(float)
	    1.274,(float)1.265,(float)1.249,(float)1.195,(float)1.162,(float)
	    1.15,(float)1.124,(float)1.102,(float)1.041,(float)1.372,(float)
	    1.376,(float)1.377,(float)1.376,(float)1.375,(float)1.373,(float)
	    1.371,(float)1.367,(float)1.362,(float)1.354,(float)1.348,(float)
	    1.344,(float)1.331,(float)1.312,(float)1.248,(float)1.21,(float)
	    1.196,(float)1.166,(float)1.142,(float)1.074,(float)1.468,(float)
	    1.468,(float)1.467,(float)1.464,(float)1.461,(float)1.458,(float)
	    1.454,(float)1.448,(float)1.44,(float)1.43,(float)1.422,(float)
	    1.417,(float)1.401,(float)1.378,(float)1.303,(float)1.259,(float)
	    1.244,(float)1.21,(float)1.184,(float)1.109,(float)1.571,(float)
	    1.566,(float)1.564,(float)1.558,(float)1.552,(float)1.548,(float)
	    1.542,(float)1.534,(float)1.523,(float)1.51,(float)1.5,(float)
	    1.493,(float)1.474,(float)1.446,(float)1.359,(float)1.309,(float)
	    1.292,(float)1.255,(float)1.226,(float)1.144,(float)1.681,(float)
	    1.67,(float)1.667,(float)1.657,(float)1.649,(float)1.643,(float)
	    1.634,(float)1.623,(float)1.609,(float)1.594,(float)1.581,(float)
	    1.573,(float)1.55,(float)1.517,(float)1.417,(float)1.361,(float)
	    1.342,(float)1.301,(float)1.269,(float)1.18,(float)1.799,(float)
	    1.78,(float)1.775,(float)1.762,(float)1.75,(float)1.742,(float)
	    1.731,(float)1.717,(float)1.699,(float)1.68,(float)1.665,(float)
	    1.655,(float)1.628,(float)1.59,(float)1.476,(float)1.413,(float)
	    1.393,(float)1.348,(float)1.312,(float)1.216,(float)1.923,(float)
	    1.896,(float)1.889,(float)1.871,(float)1.856,(float)1.846,(float)
	    1.832,(float)1.814,(float)1.793,(float)1.77,(float)1.751,(float)
	    1.74,(float)1.709,(float)1.664,(float)1.535,(float)1.466,(float)
	    1.444,(float)1.394,(float)1.356,(float)1.252,(float)2.055,(float)
	    2.018,(float)2.009,(float)1.986,(float)1.967,(float)1.954,(float)
	    1.937,(float)1.915,(float)1.89,(float)1.862,(float)1.841,(float)
	    1.828,(float)1.791,(float)1.74,(float)1.596,(float)1.52,(float)
	    1.495,(float)1.441,(float)1.4,(float)1.288,(float)2.195,(float)
	    2.146,(float)2.135,(float)2.106,(float)2.082,(float)2.067,(float)
	    2.045,(float)2.019,(float)1.989,(float)1.957,(float)1.932,(float)
	    1.917,(float)1.875,(float)1.818,(float)1.657,(float)1.573,(float)
	    1.546,(float)1.488,(float)1.443,(float)1.324,(float)2.341,(float)
	    2.28,(float)2.266,(float)2.231,(float)2.202,(float)2.183,(float)
	    2.157,(float)2.126,(float)2.091,(float)2.055,(float)2.025,(float)
	    2.008,(float)1.961,(float)1.896,(float)1.717,(float)1.626,(float)
	    1.597,(float)1.535,(float)1.487,(float)1.359,(float)2.495,(float)
	    2.419,(float)2.402,(float)2.36,(float)2.325,(float)2.303,(float)
	    2.272,(float)2.236,(float)2.195,(float)2.153,(float)2.12,(float)
	    2.1,(float)2.047,(float)1.974,(float)1.778,(float)1.679,(float)
	    1.647,(float)1.58,(float)1.529,(float)1.394,(float)2.655,(float)
	    2.563,(float)2.542,(float)2.492,(float)2.451,(float)2.426,(float)
	    2.39,(float)2.348,(float)2.301,(float)2.253,(float)2.215,(float)
	    2.193,(float)2.133,(float)2.053,(float)1.837,(float)1.731,(float)
	    1.696,(float)1.625,(float)1.57,(float)1.428,(float)2.822,(float)
	    2.711,(float)2.687,(float)2.627,(float)2.58,(float)2.55,(float)
	    2.509,(float)2.46,(float)2.407,(float)2.353,(float)2.311,(float)
	    2.286,(float)2.219,(float)2.13,(float)1.896,(float)1.781,(float)
	    1.745,(float)1.668,(float)1.611,(float)1.46,(float)2.993,(float)
	    2.862,(float)2.834,(float)2.765,(float)2.71,(float)2.676,(float)
	    2.629,(float)2.574,(float)2.513,(float)2.453,(float)2.406,(float)
	    2.378,(float)2.304,(float)2.207,(float)1.953,(float)1.83,(float)
	    1.791,(float)1.71,(float)1.65,(float)1.491,(float)3.168,(float)
	    3.015,(float)2.983,(float)2.904,(float)2.841,(float)2.802,(float)
	    2.748,(float)2.686,(float)2.619,(float)2.551,(float)2.499,(float)
	    2.469,(float)2.388,(float)2.281,(float)2.008,(float)1.877,(float)
	    1.836,(float)1.751,(float)1.687,(float)1.521,(float)3.346,(float)
	    3.17,(float)3.132,(float)3.042,(float)2.971,(float)2.927,(float)
	    2.867,(float)2.797,(float)2.723,(float)2.648,(float)2.591,(float)
	    2.557,(float)2.469,(float)2.354,(float)2.06,(float)1.921,(float)
	    1.878,(float)1.789,(float)1.722,(float)1.549,(float)3.525,(float)
	    3.323,(float)3.28,(float)3.179,(float)3.099,(float)3.05,(float)
	    2.983,(float)2.906,(float)2.823,(float)2.742,(float)2.679,(float)
	    2.642,(float)2.547,(float)2.423,(float)2.11,(float)1.963,(float)
	    1.918,(float)1.824,(float)1.754,(float)1.576,(float)3.703,(float)
	    3.474,(float)3.426,(float)3.313,(float)3.224,(float)3.17,(float)
	    3.096,(float)3.01,(float)2.92,(float)2.831,(float)2.764,(float)
	    2.724,(float)2.621,(float)2.488,(float)2.156,(float)2.002,(float)
	    1.955,(float)1.857,(float)1.785,(float)1.6,(float)3.877,(float)
	    3.62,(float)3.567,(float)3.441,(float)3.344,(float)3.284,(float)
	    3.203,(float)3.11,(float)3.012,(float)2.916,(float)2.843,(float)
	    2.8,(float)2.69,(float)2.549,(float)2.199,(float)2.038,(float)
	    1.989,(float)1.888,(float)1.813,(float)1.622,(float)4.044,(float)
	    3.759,(float)3.701,(float)3.563,(float)3.456,(float)3.391,(float)
	    3.303,(float)3.202,(float)3.097,(float)2.994,(float)2.916,(float)
	    2.87,(float)2.753,(float)2.604,(float)2.237,(float)2.071,(float)
	    2.019,(float)1.915,(float)1.837,(float)1.642,(float)4.201,(float)
	    3.889,(float)3.824,(float)3.674,(float)3.559,(float)3.489,(float)
	    3.394,(float)3.286,(float)3.174,(float)3.064,(float)2.982,(float)
	    2.934,(float)2.81,(float)2.654,(float)2.271,(float)2.099,(float)
	    2.046,(float)1.939,(float)1.859,(float)1.659,(float)4.343,(float)
	    4.005,(float)3.936,(float)3.774,(float)3.651,(float)3.576,(float)
	    3.475,(float)3.36,(float)3.241,(float)3.126,(float)3.039,(float)
	    2.989,(float)2.86,(float)2.696,(float)2.301,(float)2.123,(float)
	    2.069,(float)1.959,(float)1.878,(float)1.673,(float)4.468,(float)
	    4.105,(float)4.031,(float)3.86,(float)3.729,(float)3.65,(float)
	    3.544,(float)3.423,(float)3.298,(float)3.178,(float)3.088,(float)
	    3.035,(float)2.901,(float)2.732,(float)2.325,(float)2.143,(float)
	    2.088,(float)1.976,(float)1.893,(float)1.685,(float)4.569,(float)
	    4.185,(float)4.108,(float)3.928,(float)3.792,(float)3.709,(float)
	    3.598,(float)3.473,(float)3.343,(float)3.219,(float)3.126,(float)
	    3.071,(float)2.934,(float)2.76,(float)2.344,(float)2.159,(float)
	    2.103,(float)1.989,(float)1.905,(float)1.695,(float)4.642,(float)
	    4.244,(float)4.163,(float)3.977,(float)3.836,(float)3.751,(float)
	    3.637,(float)3.508,(float)3.375,(float)3.248,(float)3.153,(float)
	    3.097,(float)2.956,(float)2.78,(float)2.357,(float)2.17,(float)
	    2.113,(float)1.998,(float)1.913,(float)1.701,(float)4.685,(float)
	    4.277,(float)4.195,(float)4.005,(float)3.862,(float)3.775,(float)
	    3.659,(float)3.528,(float)3.393,(float)3.264,(float)3.168,(float)
	    3.111,(float)2.969,(float)2.791,(float)2.364,(float)2.176,(float)
	    2.119,(float)2.003,(float)1.918,(float)1.705,(float)4.695,(float)
	    4.285,(float)4.203,(float)4.012,(float)3.868,(float)3.781,(float)
	    3.664,(float)3.533,(float)3.398,(float)3.268,(float)3.171,(float)
	    3.115,(float)2.972,(float)2.793,(float)2.366,(float)2.177,(float)
	    2.12,(float)2.004,(float)1.919,(float)1.706 };
    static real qhr[1660]	/* was [20][83] */ = { (float)-6.313e-22,(
	    float)-1.599e-21,(float)-1.77e-23,(float)5.94e-23,(float)
	    1.282e-21,(float)1.097e-21,(float)-6.691e-22,(float)1.88e-21,(
	    float)7.396e-22,(float)-6.344e-22,(float)-1.154e-21,(float)
	    2.554e-22,(float)4.116e-23,(float)2.315e-21,(float)1.785e-21,(
	    float)-1.552e-22,(float)5.632e-22,(float)7.582e-22,(float)
	    -3.864e-22,(float)-3.946e-22,(float)-2.429e-4,(float)-2.648e-4,(
	    float)-2.698e-4,(float)-2.822e-4,(float)-2.925e-4,(float)-2.99e-4,
	    (float)-3.084e-4,(float)-3.196e-4,(float)-3.322e-4,(float)
	    -3.452e-4,(float)-3.557e-4,(float)-3.62e-4,(float)-3.79e-4,(float)
	    -4.024e-4,(float)-4.69e-4,(float)-5.045e-4,(float)-5.159e-4,(
	    float)-5.405e-4,(float)-5.597e-4,(float)-6.127e-4,(float)-.001278,
	    (float)-.001394,(float)-.00142,(float)-.001485,(float)-.001539,(
	    float)-.001574,(float)-.001623,(float)-.001682,(float)-.001749,(
	    float)-.001817,(float)-.001872,(float)-.001906,(float)-.001995,(
	    float)-.002118,(float)-.002469,(float)-.002655,(float)-.002715,(
	    float)-.002845,(float)-.002946,(float)-.003225,(float)-.003133,(
	    float)-.003418,(float)-.003482,(float)-.003643,(float)-.003775,(
	    float)-.003861,(float)-.003981,(float)-.004127,(float)-.00429,(
	    float)-.004459,(float)-.004593,(float)-.004675,(float)-.004895,(
	    float)-.005198,(float)-.006057,(float)-.006514,(float)-.006661,(
	    float)-.006979,(float)-.007226,(float)-.007908,(float)-.005796,(
	    float)-.006325,(float)-.006445,(float)-.006744,(float)-.00699,(
	    float)-.007148,(float)-.007372,(float)-.007643,(float)-.007945,(
	    float)-.008258,(float)-.008508,(float)-.00866,(float)-.009068,(
	    float)-.009628,(float)-.01122,(float)-.01206,(float)-.01234,(
	    float)-.01292,(float)-.01338,(float)-.01464,(float)-.00925,(float)
	    -.0101,(float)-.01029,(float)-.01077,(float)-.01117,(float)
	    -.01142,(float)-.01178,(float)-.01221,(float)-.0127,(float)-.0132,
	    (float)-.0136,(float)-.01384,(float)-.01449,(float)-.01539,(float)
	    -.01793,(float)-.01928,(float)-.01971,(float)-.02065,(float)
	    -.02138,(float)-.02338,(float)-.01347,(float)-.01472,(float)
	    -.01501,(float)-.01571,(float)-.01629,(float)-.01666,(float)
	    -.01718,(float)-.01782,(float)-.01853,(float)-.01926,(float)
	    -.01984,(float)-.0202,(float)-.02115,(float)-.02246,(float)
	    -.02616,(float)-.02813,(float)-.02876,(float)-.03012,(float)
	    -.03118,(float)-.03409,(float)-.01844,(float)-.02017,(float)
	    -.02056,(float)-.02153,(float)-.02232,(float)-.02284,(float)
	    -.02356,(float)-.02443,(float)-.02541,(float)-.02641,(float)
	    -.02722,(float)-.02771,(float)-.02901,(float)-.03081,(float)
	    -.03588,(float)-.03856,(float)-.03943,(float)-.04128,(float)
	    -.04273,(float)-.04669,(float)-.02413,(float)-.02641,(float)
	    -.02692,(float)-.0282,(float)-.02925,(float)-.02993,(float)
	    -.03088,(float)-.03203,(float)-.03331,(float)-.03463,(float)
	    -.03569,(float)-.03633,(float)-.03804,(float)-.0404,(float)
	    -.04703,(float)-.05054,(float)-.05167,(float)-.05409,(float)
	    -.05597,(float)-.06114,(float)-.03051,(float)-.03341,(float)
	    -.03407,(float)-.0357,(float)-.03704,(float)-.0379,(float)-.03911,
	    (float)-.04057,(float)-.0422,(float)-.04388,(float)-.04522,(float)
	    -.04603,(float)-.04821,(float)-.05119,(float)-.05958,(float)-.064,
	    (float)-.06542,(float)-.06848,(float)-.07085,(float)-.07733,(
	    float)-.03754,(float)-.04116,(float)-.04197,(float)-.04399,(float)
	    -.04565,(float)-.04672,(float)-.04822,(float)-.05003,(float)
	    -.05204,(float)-.05412,(float)-.05577,(float)-.05678,(float)
	    -.05946,(float)-.06314,(float)-.07346,(float)-.07889,(float)
	    -.08063,(float)-.08438,(float)-.08727,(float)-.0952,(float)
	    -.04521,(float)-.0496,(float)-.05059,(float)-.05304,(float)
	    -.05506,(float)-.05635,(float)-.05817,(float)-.06036,(float)
	    -.06279,(float)-.0653,(float)-.06731,(float)-.06852,(float)
	    -.07176,(float)-.0762,(float)-.08861,(float)-.09513,(float)
	    -.09722,(float)-.1017,(float)-.1052,(float)-.1146,(float)-.05347,(
	    float)-.05871,(float)-.05989,(float)-.06281,(float)-.06522,(float)
	    -.06675,(float)-.06892,(float)-.07153,(float)-.07442,(float)
	    -.0774,(float)-.07977,(float)-.08121,(float)-.08506,(float)
	    -.09031,(float)-.105,(float)-.1126,(float)-.1151,(float)-.1204,(
	    float)-.1244,(float)-.1355,(float)-.06229,(float)-.06846,(float)
	    -.06984,(float)-.07327,(float)-.07609,(float)-.07789,(float)
	    -.08043,(float)-.08348,(float)-.08687,(float)-.09035,(float)
	    -.09313,(float)-.09481,(float)-.09929,(float)-.1054,(float)-.1225,
	    (float)-.1313,(float)-.1342,(float)-.1403,(float)-.145,(float)
	    -.1577,(float)-.07165,(float)-.0788,(float)-.08041,(float)-.08438,
	    (float)-.08765,(float)-.08973,(float)-.09266,(float)-.09619,(
	    float)-.1001,(float)-.1041,(float)-.1073,(float)-.1093,(float)
	    -.1144,(float)-.1215,(float)-.141,(float)-.1512,(float)-.1544,(
	    float)-.1614,(float)-.1667,(float)-.1812,(float)-.08151,(float)
	    -.08972,(float)-.09156,(float)-.0961,(float)-.09984,(float)-.1022,
	    (float)-.1056,(float)-.1096,(float)-.1141,(float)-.1186,(float)
	    -.1223,(float)-.1245,(float)-.1304,(float)-.1384,(float)-.1605,(
	    float)-.172,(float)-.1756,(float)-.1835,(float)-.1895,(float)
	    -.2057,(float)-.09185,(float)-.1012,(float)-.1032,(float)-.1084,(
	    float)-.1126,(float)-.1153,(float)-.1191,(float)-.1237,(float)
	    -.1287,(float)-.1339,(float)-.138,(float)-.1405,(float)-.1471,(
	    float)-.1561,(float)-.1809,(float)-.1937,(float)-.1978,(float)
	    -.2065,(float)-.2132,(float)-.2312,(float)-.1026,(float)-.1131,(
	    float)-.1154,(float)-.1212,(float)-.126,(float)-.129,(float)
	    -.1332,(float)-.1383,(float)-.144,(float)-.1497,(float)-.1543,(
	    float)-.1571,(float)-.1645,(float)-.1745,(float)-.2021,(float)
	    -.2162,(float)-.2208,(float)-.2304,(float)-.2378,(float)-.2575,(
	    float)-.1138,(float)-.1255,(float)-.1281,(float)-.1345,(float)
	    -.1398,(float)-.1432,(float)-.1479,(float)-.1535,(float)-.1598,(
	    float)-.1662,(float)-.1713,(float)-.1744,(float)-.1825,(float)
	    -.1936,(float)-.2239,(float)-.2395,(float)-.2444,(float)-.255,(
	    float)-.263,(float)-.2845,(float)-.1254,(float)-.1383,(float)
	    -.1412,(float)-.1483,(float)-.1541,(float)-.1578,(float)-.163,(
	    float)-.1693,(float)-.1761,(float)-.1832,(float)-.1888,(float)
	    -.1922,(float)-.2011,(float)-.2132,(float)-.2464,(float)-.2633,(
	    float)-.2687,(float)-.2801,(float)-.2888,(float)-.312,(float)
	    -.1374,(float)-.1515,(float)-.1547,(float)-.1625,(float)-.1689,(
	    float)-.1729,(float)-.1786,(float)-.1854,(float)-.193,(float)
	    -.2007,(float)-.2068,(float)-.2104,(float)-.2202,(float)-.2334,(
	    float)-.2693,(float)-.2876,(float)-.2934,(float)-.3057,(float)
	    -.315,(float)-.3398,(float)-.1497,(float)-.1651,(float)-.1686,(
	    float)-.1771,(float)-.184,(float)-.1884,(float)-.1946,(float)
	    -.202,(float)-.2102,(float)-.2185,(float)-.2251,(float)-.2291,(
	    float)-.2397,(float)-.254,(float)-.2926,(float)-.3122,(float)
	    -.3184,(float)-.3316,(float)-.3416,(float)-.3678,(float)-.1623,(
	    float)-.179,(float)-.1828,(float)-.192,(float)-.1995,(float)
	    -.2042,(float)-.2109,(float)-.2189,(float)-.2277,(float)-.2367,(
	    float)-.2439,(float)-.2481,(float)-.2595,(float)-.2748,(float)
	    -.3162,(float)-.3371,(float)-.3437,(float)-.3576,(float)-.3682,(
	    float)-.3959,(float)-.1751,(float)-.1932,(float)-.1972,(float)
	    -.2071,(float)-.2152,(float)-.2203,(float)-.2275,(float)-.2361,(
	    float)-.2456,(float)-.2552,(float)-.2628,(float)-.2674,(float)
	    -.2796,(float)-.296,(float)-.34,(float)-.3621,(float)-.369,(float)
	    -.3837,(float)-.3949,(float)-.4238,(float)-.1882,(float)-.2076,(
	    float)-.2119,(float)-.2225,(float)-.2312,(float)-.2367,(float)
	    -.2444,(float)-.2535,(float)-.2636,(float)-.2739,(float)-.282,(
	    float)-.2869,(float)-.2998,(float)-.3172,(float)-.3638,(float)
	    -.387,(float)-.3943,(float)-.4097,(float)-.4214,(float)-.4514,(
	    float)-.2016,(float)-.2223,(float)-.2269,(float)-.2382,(float)
	    -.2474,(float)-.2532,(float)-.2614,(float)-.2711,(float)-.2818,(
	    float)-.2927,(float)-.3013,(float)-.3065,(float)-.3202,(float)
	    -.3386,(float)-.3875,(float)-.4118,(float)-.4194,(float)-.4355,(
	    float)-.4476,(float)-.4786,(float)-.2151,(float)-.2371,(float)
	    -.2419,(float)-.2539,(float)-.2637,(float)-.2698,(float)-.2785,(
	    float)-.2888,(float)-.3001,(float)-.3116,(float)-.3207,(float)
	    -.3261,(float)-.3405,(float)-.3598,(float)-.411,(float)-.4363,(
	    float)-.4442,(float)-.4609,(float)-.4734,(float)-.5052,(float)
	    -.2288,(float)-.252,(float)-.2571,(float)-.2698,(float)-.2801,(
	    float)-.2866,(float)-.2957,(float)-.3065,(float)-.3184,(float)
	    -.3305,(float)-.34,(float)-.3457,(float)-.3608,(float)-.381,(
	    float)-.4342,(float)-.4604,(float)-.4686,(float)-.4857,(float)
	    -.4986,(float)-.5311,(float)-.2426,(float)-.267,(float)-.2724,(
	    float)-.2857,(float)-.2965,(float)-.3033,(float)-.3129,(float)
	    -.3242,(float)-.3366,(float)-.3493,(float)-.3592,(float)-.3652,(
	    float)-.3809,(float)-.4019,(float)-.4571,(float)-.484,(float)
	    -.4924,(float)-.5099,(float)-.523,(float)-.556,(float)-.2565,(
	    float)-.2821,(float)-.2877,(float)-.3016,(float)-.3129,(float)
	    -.32,(float)-.33,(float)-.3418,(float)-.3547,(float)-.3679,(float)
	    -.3782,(float)-.3844,(float)-.4007,(float)-.4225,(float)-.4793,(
	    float)-.5069,(float)-.5155,(float)-.5333,(float)-.5466,(float)
	    -.5798,(float)-.2704,(float)-.2971,(float)-.303,(float)-.3175,(
	    float)-.3292,(float)-.3366,(float)-.347,(float)-.3592,(float)
	    -.3727,(float)-.3863,(float)-.397,(float)-.4034,(float)-.4202,(
	    float)-.4427,(float)-.5009,(float)-.529,(float)-.5377,(float)
	    -.5558,(float)-.5693,(float)-.6025,(float)-.2844,(float)-.3121,(
	    float)-.3183,(float)-.3333,(float)-.3454,(float)-.3531,(float)
	    -.3638,(float)-.3765,(float)-.3903,(float)-.4044,(float)-.4154,(
	    float)-.422,(float)-.4393,(float)-.4623,(float)-.5217,(float)
	    -.5502,(float)-.559,(float)-.5773,(float)-.5907,(float)-.6238,(
	    float)-.2983,(float)-.327,(float)-.3334,(float)-.3489,(float)
	    -.3614,(float)-.3693,(float)-.3803,(float)-.3934,(float)-.4076,(
	    float)-.422,(float)-.4333,(float)-.4401,(float)-.4578,(float)
	    -.4813,(float)-.5417,(float)-.5703,(float)-.5792,(float)-.5975,(
	    float)-.611,(float)-.6436,(float)-.3122,(float)-.3418,(float)
	    -.3483,(float)-.3643,(float)-.3772,(float)-.3853,(float)-.3966,(
	    float)-.4099,(float)-.4245,(float)-.4393,(float)-.4508,(float)
	    -.4576,(float)-.4757,(float)-.4996,(float)-.5606,(float)-.5893,(
	    float)-.5982,(float)-.6165,(float)-.6298,(float)-.6619,(float)
	    -.3259,(float)-.3564,(float)-.3631,(float)-.3794,(float)-.3926,(
	    float)-.4009,(float)-.4124,(float)-.426,(float)-.4409,(float)
	    -.4559,(float)-.4676,(float)-.4746,(float)-.4929,(float)-.5171,(
	    float)-.5784,(float)-.6071,(float)-.6159,(float)-.634,(float)
	    -.6472,(float)-.6784,(float)-.3395,(float)-.3707,(float)-.3775,(
	    float)-.3942,(float)-.4076,(float)-.4161,(float)-.4278,(float)
	    -.4416,(float)-.4567,(float)-.4719,(float)-.4837,(float)-.4908,(
	    float)-.5093,(float)-.5337,(float)-.595,(float)-.6234,(float)
	    -.6322,(float)-.65,(float)-.6629,(float)-.6932,(float)-.353,(
	    float)-.3847,(float)-.3917,(float)-.4086,(float)-.4222,(float)
	    -.4308,(float)-.4427,(float)-.4566,(float)-.4718,(float)-.4872,(
	    float)-.4991,(float)-.5062,(float)-.5248,(float)-.5493,(float)
	    -.6103,(float)-.6383,(float)-.6469,(float)-.6644,(float)-.677,(
	    float)-.706,(float)-.3661,(float)-.3984,(float)-.4054,(float)
	    -.4225,(float)-.4363,(float)-.4449,(float)-.4569,(float)-.4709,(
	    float)-.4862,(float)-.5017,(float)-.5136,(float)-.5207,(float)
	    -.5393,(float)-.5638,(float)-.6242,(float)-.6516,(float)-.66,(
	    float)-.677,(float)-.6892,(float)-.7168,(float)-.379,(float)
	    -.4116,(float)-.4187,(float)-.4359,(float)-.4498,(float)-.4585,(
	    float)-.4705,(float)-.4845,(float)-.4998,(float)-.5152,(float)
	    -.5272,(float)-.5342,(float)-.5528,(float)-.577,(float)-.6365,(
	    float)-.6633,(float)-.6714,(float)-.6878,(float)-.6995,(float)
	    -.7256,(float)-.3915,(float)-.4243,(float)-.4314,(float)-.4487,(
	    float)-.4626,(float)-.4713,(float)-.4833,(float)-.4973,(float)
	    -.5125,(float)-.5278,(float)-.5397,(float)-.5467,(float)-.5651,(
	    float)-.589,(float)-.6473,(float)-.6732,(float)-.681,(float)
	    -.6968,(float)-.7079,(float)-.7322,(float)-.4036,(float)-.4365,(
	    float)-.4436,(float)-.4609,(float)-.4747,(float)-.4834,(float)
	    -.4952,(float)-.5091,(float)-.5242,(float)-.5394,(float)-.5511,(
	    float)-.558,(float)-.5761,(float)-.5997,(float)-.6564,(float)
	    -.6812,(float)-.6888,(float)-.7037,(float)-.7143,(float)-.7367,(
	    float)-.4095,(float)-.4423,(float)-.4494,(float)-.4667,(float)
	    -.4805,(float)-.4891,(float)-.5009,(float)-.5147,(float)-.5297,(
	    float)-.5448,(float)-.5564,(float)-.5632,(float)-.5811,(float)
	    -.6045,(float)-.6602,(float)-.6845,(float)-.6919,(float)-.7064,(
	    float)-.7167,(float)-.7381,(float)-.4152,(float)-.448,(float)
	    -.4551,(float)-.4723,(float)-.486,(float)-.4945,(float)-.5063,(
	    float)-.52,(float)-.5348,(float)-.5498,(float)-.5613,(float)
	    -.5681,(float)-.5858,(float)-.6089,(float)-.6637,(float)-.6874,(
	    float)-.6946,(float)-.7087,(float)-.7185,(float)-.739,(float)
	    -.4263,(float)-.4588,(float)-.4659,(float)-.4829,(float)-.4964,(
	    float)-.5048,(float)-.5164,(float)-.5298,(float)-.5444,(float)
	    -.559,(float)-.5703,(float)-.5768,(float)-.5941,(float)-.6165,(
	    float)-.6692,(float)-.6916,(float)-.6983,(float)-.7115,(float)
	    -.7207,(float)-.739,(float)-.4368,(float)-.4689,(float)-.4758,(
	    float)-.4925,(float)-.5058,(float)-.5141,(float)-.5254,(float)
	    -.5384,(float)-.5526,(float)-.5669,(float)-.5778,(float)-.5842,(
	    float)-.601,(float)-.6226,(float)-.6728,(float)-.6938,(float)
	    -.7001,(float)-.7123,(float)-.7207,(float)-.7368,(float)-.4466,(
	    float)-.4781,(float)-.4849,(float)-.5012,(float)-.5142,(float)
	    -.5223,(float)-.5332,(float)-.5459,(float)-.5596,(float)-.5735,(
	    float)-.584,(float)-.5901,(float)-.6062,(float)-.627,(float)
	    -.6744,(float)-.6939,(float)-.6998,(float)-.7109,(float)-.7185,(
	    float)-.7324,(float)-.4556,(float)-.4864,(float)-.493,(float)
	    -.5089,(float)-.5215,(float)-.5293,(float)-.5398,(float)-.552,(
	    float)-.5652,(float)-.5785,(float)-.5886,(float)-.5945,(float)
	    -.6099,(float)-.6296,(float)-.6741,(float)-.692,(float)-.6973,(
	    float)-.7074,(float)-.7141,(float)-.7257,(float)-.4638,(float)
	    -.4937,(float)-.5001,(float)-.5154,(float)-.5275,(float)-.535,(
	    float)-.5452,(float)-.5568,(float)-.5694,(float)-.5821,(float)
	    -.5917,(float)-.5972,(float)-.6118,(float)-.6305,(float)-.6718,(
	    float)-.688,(float)-.6928,(float)-.7017,(float)-.7076,(float)
	    -.7168,(float)-.471,(float)-.4999,(float)-.506,(float)-.5207,(
	    float)-.5323,(float)-.5395,(float)-.5491,(float)-.5601,(float)
	    -.5721,(float)-.5841,(float)-.5931,(float)-.5983,(float)-.612,(
	    float)-.6295,(float)-.6674,(float)-.6819,(float)-.6861,(float)
	    -.6939,(float)-.6989,(float)-.7058,(float)-.4773,(float)-.5049,(
	    float)-.5108,(float)-.5248,(float)-.5357,(float)-.5425,(float)
	    -.5516,(float)-.5619,(float)-.5731,(float)-.5844,(float)-.5929,(
	    float)-.5977,(float)-.6105,(float)-.6267,(float)-.661,(float)
	    -.6737,(float)-.6774,(float)-.684,(float)-.6881,(float)-.6927,(
	    float)-.4824,(float)-.5087,(float)-.5142,(float)-.5274,(float)
	    -.5377,(float)-.5441,(float)-.5526,(float)-.5621,(float)-.5725,(
	    float)-.583,(float)-.5908,(float)-.5953,(float)-.6071,(float)
	    -.6219,(float)-.6526,(float)-.6634,(float)-.6665,(float)-.672,(
	    float)-.6752,(float)-.6776,(float)-.4864,(float)-.5111,(float)
	    -.5163,(float)-.5286,(float)-.5382,(float)-.5441,(float)-.5519,(
	    float)-.5607,(float)-.5703,(float)-.5799,(float)-.587,(float)
	    -.5911,(float)-.6018,(float)-.6152,(float)-.6421,(float)-.6511,(
	    float)-.6537,(float)-.658,(float)-.6603,(float)-.6605,(float)
	    -.4892,(float)-.5121,(float)-.5169,(float)-.5283,(float)-.5371,(
	    float)-.5425,(float)-.5496,(float)-.5575,(float)-.5662,(float)
	    -.575,(float)-.5814,(float)-.585,(float)-.5946,(float)-.6066,(
	    float)-.6297,(float)-.6368,(float)-.6388,(float)-.642,(float)
	    -.6435,(float)-.6416,(float)-.4905,(float)-.5116,(float)-.516,(
	    float)-.5264,(float)-.5344,(float)-.5393,(float)-.5456,(float)
	    -.5526,(float)-.5604,(float)-.5682,(float)-.5739,(float)-.5771,(
	    float)-.5855,(float)-.5961,(float)-.6153,(float)-.6206,(float)
	    -.6221,(float)-.6241,(float)-.6248,(float)-.621,(float)-.4904,(
	    float)-.5096,(float)-.5135,(float)-.5228,(float)-.53,(float)
	    -.5343,(float)-.5399,(float)-.5459,(float)-.5527,(float)-.5596,(
	    float)-.5645,(float)-.5672,(float)-.5746,(float)-.5836,(float)
	    -.599,(float)-.6025,(float)-.6035,(float)-.6044,(float)-.6044,(
	    float)-.5988,(float)-.4888,(float)-.5058,(float)-.5093,(float)
	    -.5175,(float)-.5237,(float)-.5275,(float)-.5323,(float)-.5374,(
	    float)-.5432,(float)-.5491,(float)-.5533,(float)-.5555,(float)
	    -.5617,(float)-.5692,(float)-.5808,(float)-.5827,(float)-.5831,(
	    float)-.5831,(float)-.5823,(float)-.5751,(float)-.4855,(float)
	    -.5004,(float)-.5034,(float)-.5104,(float)-.5157,(float)-.5189,(
	    float)-.5229,(float)-.5271,(float)-.5318,(float)-.5367,(float)
	    -.5402,(float)-.5419,(float)-.5469,(float)-.553,(float)-.5609,(
	    float)-.5612,(float)-.5612,(float)-.5602,(float)-.5588,(float)
	    -.55,(float)-.4806,(float)-.4932,(float)-.4957,(float)-.5015,(
	    float)-.5059,(float)-.5085,(float)-.5116,(float)-.5148,(float)
	    -.5186,(float)-.5225,(float)-.5252,(float)-.5265,(float)-.5304,(
	    float)-.5349,(float)-.5394,(float)-.5381,(float)-.5377,(float)
	    -.5358,(float)-.5338,(float)-.5238,(float)-.4738,(float)-.4841,(
	    float)-.4861,(float)-.4908,(float)-.4942,(float)-.4962,(float)
	    -.4985,(float)-.5007,(float)-.5035,(float)-.5065,(float)-.5084,(
	    float)-.5093,(float)-.512,(float)-.5152,(float)-.5163,(float)
	    -.5137,(float)-.5128,(float)-.5102,(float)-.5077,(float)-.4966,(
	    float)-.4652,(float)-.4732,(float)-.4747,(float)-.4781,(float)
	    -.4806,(float)-.482,(float)-.4835,(float)-.4848,(float)-.4866,(
	    float)-.4886,(float)-.4898,(float)-.4903,(float)-.492,(float)
	    -.4938,(float)-.4919,(float)-.488,(float)-.4868,(float)-.4834,(
	    float)-.4804,(float)-.4685,(float)-.4547,(float)-.4603,(float)
	    -.4613,(float)-.4636,(float)-.4651,(float)-.4659,(float)-.4667,(
	    float)-.4671,(float)-.468,(float)-.4691,(float)-.4696,(float)
	    -.4697,(float)-.4703,(float)-.4709,(float)-.4662,(float)-.4611,(
	    float)-.4596,(float)-.4557,(float)-.4523,(float)-.4397,(float)
	    -.4423,(float)-.4455,(float)-.4461,(float)-.4471,(float)-.4478,(
	    float)-.4481,(float)-.4481,(float)-.4476,(float)-.4476,(float)
	    -.4479,(float)-.4477,(float)-.4474,(float)-.4472,(float)-.4466,(
	    float)-.4393,(float)-.4334,(float)-.4316,(float)-.4272,(float)
	    -.4235,(float)-.4105,(float)-.4278,(float)-.4288,(float)-.4289,(
	    float)-.4289,(float)-.4287,(float)-.4284,(float)-.4277,(float)
	    -.4265,(float)-.4257,(float)-.4251,(float)-.4244,(float)-.4238,(
	    float)-.4226,(float)-.421,(float)-.4116,(float)-.4049,(float)
	    -.4029,(float)-.3981,(float)-.3942,(float)-.3809,(float)-.4114,(
	    float)-.4103,(float)-.4099,(float)-.4088,(float)-.4078,(float)
	    -.4071,(float)-.4058,(float)-.4038,(float)-.4022,(float)-.401,(
	    float)-.3997,(float)-.3988,(float)-.3969,(float)-.3943,(float)
	    -.3831,(float)-.3758,(float)-.3737,(float)-.3686,(float)-.3646,(
	    float)-.3512,(float)-.393,(float)-.3898,(float)-.3891,(float)
	    -.3871,(float)-.3853,(float)-.3842,(float)-.3823,(float)-.3796,(
	    float)-.3774,(float)-.3755,(float)-.3738,(float)-.3726,(float)
	    -.3701,(float)-.3668,(float)-.3541,(float)-.3463,(float)-.3441,(
	    float)-.3389,(float)-.3348,(float)-.3216,(float)-.3727,(float)
	    -.3677,(float)-.3665,(float)-.3637,(float)-.3613,(float)-.3598,(
	    float)-.3574,(float)-.3542,(float)-.3514,(float)-.349,(float)
	    -.3469,(float)-.3455,(float)-.3424,(float)-.3385,(float)-.3247,(
	    float)-.3168,(float)-.3145,(float)-.3092,(float)-.3051,(float)
	    -.2923,(float)-.3505,(float)-.3439,(float)-.3424,(float)-.3388,(
	    float)-.3359,(float)-.3341,(float)-.3313,(float)-.3277,(float)
	    -.3244,(float)-.3216,(float)-.3192,(float)-.3176,(float)-.3141,(
	    float)-.3097,(float)-.2952,(float)-.2872,(float)-.285,(float)
	    -.2798,(float)-.2758,(float)-.2635,(float)-.3266,(float)-.3186,(
	    float)-.3169,(float)-.3127,(float)-.3094,(float)-.3073,(float)
	    -.3041,(float)-.3002,(float)-.2966,(float)-.2935,(float)-.2908,(
	    float)-.2892,(float)-.2854,(float)-.2806,(float)-.2659,(float)
	    -.258,(float)-.2558,(float)-.2507,(float)-.2469,(float)-.2353,(
	    float)-.3011,(float)-.2921,(float)-.2902,(float)-.2855,(float)
	    -.2819,(float)-.2796,(float)-.2762,(float)-.2721,(float)-.2683,(
	    float)-.2649,(float)-.2622,(float)-.2604,(float)-.2565,(float)
	    -.2516,(float)-.237,(float)-.2293,(float)-.2272,(float)-.2224,(
	    float)-.2187,(float)-.2079,(float)-.2743,(float)-.2645,(float)
	    -.2624,(float)-.2575,(float)-.2537,(float)-.2513,(float)-.2478,(
	    float)-.2436,(float)-.2397,(float)-.2362,(float)-.2335,(float)
	    -.2317,(float)-.2277,(float)-.2228,(float)-.2086,(float)-.2014,(
	    float)-.1993,(float)-.1949,(float)-.1915,(float)-.1816,(float)
	    -.2464,(float)-.2362,(float)-.234,(float)-.229,(float)-.2251,(
	    float)-.2227,(float)-.2192,(float)-.215,(float)-.2111,(float)
	    -.2077,(float)-.2049,(float)-.2032,(float)-.1993,(float)-.1945,(
	    float)-.1811,(float)-.1744,(float)-.1725,(float)-.1684,(float)
	    -.1654,(float)-.1565,(float)-.2177,(float)-.2074,(float)-.2053,(
	    float)-.2003,(float)-.1964,(float)-.1941,(float)-.1907,(float)
	    -.1867,(float)-.1829,(float)-.1796,(float)-.177,(float)-.1754,(
	    float)-.1717,(float)-.1671,(float)-.1547,(float)-.1487,(float)
	    -.147,(float)-.1433,(float)-.1405,(float)-.1327,(float)-.1886,(
	    float)-.1786,(float)-.1766,(float)-.1718,(float)-.1681,(float)
	    -.1659,(float)-.1627,(float)-.1589,(float)-.1554,(float)-.1523,(
	    float)-.1499,(float)-.1484,(float)-.145,(float)-.1408,(float)
	    -.1297,(float)-.1244,(float)-.1229,(float)-.1196,(float)-.1172,(
	    float)-.1105,(float)-.1596,(float)-.1502,(float)-.1483,(float)
	    -.1439,(float)-.1405,(float)-.1385,(float)-.1356,(float)-.1322,(
	    float)-.129,(float)-.1262,(float)-.124,(float)-.1227,(float)
	    -.1197,(float)-.116,(float)-.1063,(float)-.1017,(float)-.1004,(
	    float)-.09766,(float)-.09564,(float)-.08997,(float)-.1312,(float)
	    -.1227,(float)-.121,(float)-.1171,(float)-.1141,(float)-.1123,(
	    float)-.1098,(float)-.1068,(float)-.1041,(float)-.1016,(float)
	    -.09976,(float)-.09861,(float)-.09599,(float)-.09283,(float)
	    -.08467,(float)-.08086,(float)-.07981,(float)-.07755,(float)
	    -.07589,(float)-.07127,(float)-.1039,(float)-.09665,(float)-.0952,
	    (float)-.09186,(float)-.08934,(float)-.08783,(float)-.08571,(
	    float)-.08324,(float)-.08096,(float)-.07894,(float)-.07738,(float)
	    -.07644,(float)-.07429,(float)-.07171,(float)-.06512,(float)
	    -.06209,(float)-.06125,(float)-.05946,(float)-.05815,(float)
	    -.05453,(float)-.07844,(float)-.07256,(float)-.07139,(float)
	    -.06871,(float)-.0667,(float)-.06549,(float)-.06382,(float)
	    -.06187,(float)-.06008,(float)-.05849,(float)-.05728,(float)
	    -.05654,(float)-.05487,(float)-.05287,(float)-.04783,(float)
	    -.04553,(float)-.0449,(float)-.04355,(float)-.04257,(float)
	    -.03987,(float)-.05546,(float)-.05103,(float)-.05016,(float)
	    -.04816,(float)-.04667,(float)-.04578,(float)-.04454,(float)
	    -.04312,(float)-.04181,(float)-.04065,(float)-.03977,(float)
	    -.03923,(float)-.03802,(float)-.03658,(float)-.03298,(float)
	    -.03136,(float)-.03091,(float)-.02996,(float)-.02927,(float)
	    -.02739,(float)-.03565,(float)-.03264,(float)-.03205,(float)
	    -.03071,(float)-.02971,(float)-.02912,(float)-.0283,(float)
	    -.02736,(float)-.02649,(float)-.02573,(float)-.02515,(float)
	    -.0248,(float)-.02401,(float)-.02307,(float)-.02074,(float)-.0197,
	    (float)-.01941,(float)-.0188,(float)-.01836,(float)-.01717,(float)
	    -.01967,(float)-.01794,(float)-.0176,(float)-.01684,(float)
	    -.01627,(float)-.01593,(float)-.01547,(float)-.01494,(float)
	    -.01445,(float)-.01402,(float)-.01369,(float)-.0135,(float)
	    -.01305,(float)-.01253,(float)-.01124,(float)-.01067,(float)
	    -.01051,(float)-.01018,(float)-.009935,(float)-.009281,(float)
	    -.008147,(float)-.007407,(float)-.007263,(float)-.006937,(float)
	    -.006697,(float)-.006555,(float)-.006359,(float)-.006136,(float)
	    -.00593,(float)-.005751,(float)-.005614,(float)-.005533,(float)
	    -.005347,(float)-.005129,(float)-.004594,(float)-.004357,(float)
	    -.004292,(float)-.004155,(float)-.004055,(float)-.003786,(float)
	    -.001562,(float)-.001417,(float)-.00139,(float)-.001326,(float)
	    -.001279,(float)-.001252,(float)-.001214,(float)-.001171,(float)
	    -.001131,(float)-.001096,(float)-.00107,(float)-.001054,(float)
	    -.001019,(float)-9.767e-4,(float)-8.741e-4,(float)-8.287e-4,(
	    float)-8.162e-4,(float)-7.9e-4,(float)-7.71e-4,(float)-7.197e-4,(
	    float)-1.106e-21,(float)2.048e-21,(float)8.561e-21,(float)
	    1.225e-20,(float)-7.24e-21,(float)-3.342e-21,(float)3.529e-21,(
	    float)4.275e-21,(float)-1.693e-21,(float)-4.113e-21,(float)
	    -6.524e-21,(float)1.626e-21,(float)8.105e-22,(float)1.071e-21,(
	    float)-2.349e-21,(float)2.536e-21,(float)1.507e-22,(float)
	    -5.265e-21,(float)2.937e-21,(float)-1.517e-21 };
    static real uhr[1660]	/* was [20][83] */ = { (float)-.4446,(float)
	    -.4907,(float)-.5012,(float)-.5274,(float)-.5491,(float)-.5632,(
	    float)-.5829,(float)-.6066,(float)-.6338,(float)-.6626,(float)
	    -.6857,(float)-.6997,(float)-.7383,(float)-.7925,(float)-.9497,(
	    float)-1.036,(float)-1.065,(float)-1.129,(float)-1.18,(float)
	    -1.325,(float)-.4445,(float)-.4905,(float)-.501,(float)-.5272,(
	    float)-.549,(float)-.563,(float)-.5827,(float)-.6064,(float)
	    -.6336,(float)-.6624,(float)-.6855,(float)-.6995,(float)-.738,(
	    float)-.7922,(float)-.9494,(float)-1.036,(float)-1.065,(float)
	    -1.129,(float)-1.18,(float)-1.324,(float)-.444,(float)-.49,(float)
	    -.5005,(float)-.5266,(float)-.5483,(float)-.5623,(float)-.582,(
	    float)-.6057,(float)-.6328,(float)-.6616,(float)-.6846,(float)
	    -.6986,(float)-.737,(float)-.7911,(float)-.9479,(float)-1.034,(
	    float)-1.063,(float)-1.127,(float)-1.178,(float)-1.322,(float)
	    -.4431,(float)-.489,(float)-.4994,(float)-.5255,(float)-.5472,(
	    float)-.5611,(float)-.5807,(float)-.6043,(float)-.6314,(float)
	    -.66,(float)-.683,(float)-.6969,(float)-.7352,(float)-.7891,(
	    float)-.9453,(float)-1.031,(float)-1.06,(float)-1.124,(float)
	    -1.174,(float)-1.317,(float)-.4418,(float)-.4875,(float)-.4979,(
	    float)-.5239,(float)-.5455,(float)-.5593,(float)-.5789,(float)
	    -.6024,(float)-.6293,(float)-.6578,(float)-.6807,(float)-.6945,(
	    float)-.7326,(float)-.7863,(float)-.9416,(float)-1.027,(float)
	    -1.056,(float)-1.119,(float)-1.169,(float)-1.311,(float)-.4401,(
	    float)-.4856,(float)-.496,(float)-.5218,(float)-.5432,(float)
	    -.557,(float)-.5764,(float)-.5998,(float)-.6265,(float)-.6549,(
	    float)-.6776,(float)-.6913,(float)-.7292,(float)-.7825,(float)
	    -.9367,(float)-1.021,(float)-1.05,(float)-1.112,(float)-1.162,(
	    float)-1.303,(float)-.438,(float)-.4832,(float)-.4935,(float)
	    -.5191,(float)-.5404,(float)-.5541,(float)-.5734,(float)-.5966,(
	    float)-.6231,(float)-.6513,(float)-.6738,(float)-.6874,(float)
	    -.725,(float)-.7778,(float)-.9307,(float)-1.015,(float)-1.043,(
	    float)-1.105,(float)-1.154,(float)-1.294,(float)-.4355,(float)
	    -.4803,(float)-.4905,(float)-.516,(float)-.5371,(float)-.5507,(
	    float)-.5697,(float)-.5927,(float)-.619,(float)-.6469,(float)
	    -.6692,(float)-.6827,(float)-.7199,(float)-.7722,(float)-.9234,(
	    float)-1.006,(float)-1.034,(float)-1.095,(float)-1.144,(float)
	    -1.282,(float)-.4325,(float)-.477,(float)-.4871,(float)-.5123,(
	    float)-.5332,(float)-.5466,(float)-.5655,(float)-.5882,(float)
	    -.6142,(float)-.6418,(float)-.6639,(float)-.6772,(float)-.714,(
	    float)-.7657,(float)-.915,(float)-.9969,(float)-1.025,(float)
	    -1.085,(float)-1.133,(float)-1.268,(float)-.4291,(float)-.4731,(
	    float)-.4831,(float)-.508,(float)-.5287,(float)-.5419,(float)
	    -.5606,(float)-.583,(float)-.6087,(float)-.636,(float)-.6578,(
	    float)-.6709,(float)-.7072,(float)-.7583,(float)-.9055,(float)
	    -.9861,(float)-1.013,(float)-1.072,(float)-1.12,(float)-1.253,(
	    float)-.4253,(float)-.4687,(float)-.4786,(float)-.5031,(float)
	    -.5235,(float)-.5366,(float)-.555,(float)-.5772,(float)-.6025,(
	    float)-.6294,(float)-.6508,(float)-.6638,(float)-.6996,(float)
	    -.7498,(float)-.8947,(float)-.9739,(float)-1.001,(float)-1.059,(
	    float)-1.105,(float)-1.236,(float)-.4209,(float)-.4637,(float)
	    -.4735,(float)-.4977,(float)-.5178,(float)-.5307,(float)-.5488,(
	    float)-.5706,(float)-.5955,(float)-.622,(float)-.6431,(float)
	    -.6559,(float)-.691,(float)-.7405,(float)-.8827,(float)-.9604,(
	    float)-.9865,(float)-1.043,(float)-1.089,(float)-1.217,(float)
	    -.416,(float)-.4582,(float)-.4678,(float)-.4916,(float)-.5114,(
	    float)-.5241,(float)-.5419,(float)-.5633,(float)-.5878,(float)
	    -.6138,(float)-.6345,(float)-.647,(float)-.6816,(float)-.7301,(
	    float)-.8695,(float)-.9455,(float)-.9711,(float)-1.027,(float)
	    -1.071,(float)-1.196,(float)-.4106,(float)-.4521,(float)-.4615,(
	    float)-.4849,(float)-.5043,(float)-.5168,(float)-.5343,(float)
	    -.5552,(float)-.5792,(float)-.6047,(float)-.6251,(float)-.6373,(
	    float)-.6712,(float)-.7187,(float)-.855,(float)-.9292,(float)
	    -.9542,(float)-1.009,(float)-1.052,(float)-1.173,(float)-.4047,(
	    float)-.4454,(float)-.4546,(float)-.4776,(float)-.4966,(float)
	    -.5088,(float)-.5259,(float)-.5464,(float)-.5699,(float)-.5948,(
	    float)-.6147,(float)-.6267,(float)-.6598,(float)-.7063,(float)
	    -.8393,(float)-.9116,(float)-.936,(float)-.9889,(float)-1.031,(
	    float)-1.149,(float)-.3982,(float)-.438,(float)-.447,(float)
	    -.4695,(float)-.4881,(float)-.5,(float)-.5167,(float)-.5367,(
	    float)-.5597,(float)-.5841,(float)-.6035,(float)-.6152,(float)
	    -.6475,(float)-.6928,(float)-.8223,(float)-.8926,(float)-.9163,(
	    float)-.9676,(float)-1.009,(float)-1.122,(float)-.391,(float)-.43,
	    (float)-.4388,(float)-.4607,(float)-.4789,(float)-.4905,(float)
	    -.5068,(float)-.5263,(float)-.5486,(float)-.5724,(float)-.5913,(
	    float)-.6027,(float)-.6341,(float)-.6782,(float)-.8041,(float)
	    -.8722,(float)-.8952,(float)-.9449,(float)-.9845,(float)-1.095,(
	    float)-.3833,(float)-.4212,(float)-.4298,(float)-.4512,(float)
	    -.4688,(float)-.4802,(float)-.496,(float)-.5149,(float)-.5367,(
	    float)-.5597,(float)-.5781,(float)-.5892,(float)-.6197,(float)
	    -.6625,(float)-.7845,(float)-.8505,(float)-.8727,(float)-.9207,(
	    float)-.9589,(float)-1.065,(float)-.3749,(float)-.4118,(float)
	    -.4201,(float)-.4409,(float)-.458,(float)-.469,(float)-.4844,(
	    float)-.5027,(float)-.5237,(float)-.5461,(float)-.5639,(float)
	    -.5746,(float)-.6042,(float)-.6457,(float)-.7637,(float)-.8273,(
	    float)-.8487,(float)-.895,(float)-.9318,(float)-1.034,(float)
	    -.3658,(float)-.4016,(float)-.4097,(float)-.4297,(float)-.4463,(
	    float)-.457,(float)-.4718,(float)-.4895,(float)-.5099,(float)
	    -.5315,(float)-.5487,(float)-.5591,(float)-.5877,(float)-.6278,(
	    float)-.7415,(float)-.8027,(float)-.8233,(float)-.8678,(float)
	    -.9032,(float)-1.001,(float)-.356,(float)-.3906,(float)-.3984,(
	    float)-.4178,(float)-.4338,(float)-.444,(float)-.4583,(float)
	    -.4754,(float)-.495,(float)-.5159,(float)-.5324,(float)-.5424,(
	    float)-.57,(float)-.6086,(float)-.7179,(float)-.7766,(float)
	    -.7964,(float)-.8391,(float)-.873,(float)-.9662,(float)-.3455,(
	    float)-.3787,(float)-.3863,(float)-.4049,(float)-.4203,(float)
	    -.4302,(float)-.4439,(float)-.4602,(float)-.4791,(float)-.4991,(
	    float)-.5151,(float)-.5246,(float)-.5511,(float)-.5882,(float)
	    -.693,(float)-.7492,(float)-.7681,(float)-.8089,(float)-.8413,(
	    float)-.9301,(float)-.3341,(float)-.366,(float)-.3732,(float)
	    -.3911,(float)-.4058,(float)-.4153,(float)-.4284,(float)-.444,(
	    float)-.462,(float)-.4813,(float)-.4965,(float)-.5056,(float)
	    -.531,(float)-.5666,(float)-.6667,(float)-.7202,(float)-.7383,(
	    float)-.7772,(float)-.808,(float)-.8924,(float)-.3219,(float)
	    -.3524,(float)-.3593,(float)-.3763,(float)-.3904,(float)-.3994,(
	    float)-.4119,(float)-.4267,(float)-.4439,(float)-.4623,(float)
	    -.4768,(float)-.4855,(float)-.5097,(float)-.5436,(float)-.639,(
	    float)-.6899,(float)-.7071,(float)-.744,(float)-.7733,(float)
	    -.8531,(float)-.3088,(float)-.3378,(float)-.3443,(float)-.3605,(
	    float)-.3739,(float)-.3824,(float)-.3943,(float)-.4083,(float)
	    -.4246,(float)-.4421,(float)-.4559,(float)-.4641,(float)-.4871,(
	    float)-.5194,(float)-.6099,(float)-.658,(float)-.6743,(float)
	    -.7093,(float)-.737,(float)-.8124,(float)-.2948,(float)-.3222,(
	    float)-.3283,(float)-.3436,(float)-.3562,(float)-.3643,(float)
	    -.3755,(float)-.3887,(float)-.4041,(float)-.4206,(float)-.4336,(
	    float)-.4414,(float)-.4632,(float)-.4938,(float)-.5793,(float)
	    -.6247,(float)-.6401,(float)-.6732,(float)-.6993,(float)-.7702,(
	    float)-.2798,(float)-.3054,(float)-.3112,(float)-.3256,(float)
	    -.3374,(float)-.345,(float)-.3555,(float)-.3678,(float)-.3823,(
	    float)-.3978,(float)-.4101,(float)-.4174,(float)-.4379,(float)
	    -.4668,(float)-.5473,(float)-.59,(float)-.6045,(float)-.6356,(
	    float)-.6601,(float)-.7266,(float)-.2637,(float)-.2876,(float)
	    -.293,(float)-.3064,(float)-.3174,(float)-.3245,(float)-.3342,(
	    float)-.3457,(float)-.3591,(float)-.3737,(float)-.3852,(float)
	    -.392,(float)-.4112,(float)-.4383,(float)-.5138,(float)-.5537,(
	    float)-.5673,(float)-.5965,(float)-.6195,(float)-.6817,(float)
	    -.2465,(float)-.2686,(float)-.2735,(float)-.2859,(float)-.2961,(
	    float)-.3027,(float)-.3116,(float)-.3222,(float)-.3346,(float)
	    -.3482,(float)-.3589,(float)-.3652,(float)-.3831,(float)-.4084,(
	    float)-.4788,(float)-.516,(float)-.5287,(float)-.5559,(float)
	    -.5775,(float)-.6354,(float)-.2281,(float)-.2482,(float)-.2528,(
	    float)-.2641,(float)-.2735,(float)-.2795,(float)-.2876,(float)
	    -.2972,(float)-.3087,(float)-.3212,(float)-.3311,(float)-.3369,(
	    float)-.3535,(float)-.377,(float)-.4423,(float)-.4768,(float)
	    -.4886,(float)-.514,(float)-.534,(float)-.5878,(float)-.2084,(
	    float)-.2266,(float)-.2307,(float)-.2409,(float)-.2494,(float)
	    -.2548,(float)-.2622,(float)-.2708,(float)-.2813,(float)-.2927,(
	    float)-.3018,(float)-.3071,(float)-.3223,(float)-.344,(float)
	    -.4042,(float)-.4361,(float)-.4471,(float)-.4706,(float)-.4892,(
	    float)-.539,(float)-.1873,(float)-.2035,(float)-.2071,(float)
	    -.2163,(float)-.2238,(float)-.2287,(float)-.2353,(float)-.2429,(
	    float)-.2523,(float)-.2627,(float)-.2709,(float)-.2756,(float)
	    -.2896,(float)-.3095,(float)-.3647,(float)-.3939,(float)-.4041,(
	    float)-.4258,(float)-.443,(float)-.489,(float)-.1648,(float)
	    -.1789,(float)-.1821,(float)-.1901,(float)-.1967,(float)-.2009,(
	    float)-.2067,(float)-.2134,(float)-.2217,(float)-.231,(float)
	    -.2383,(float)-.2426,(float)-.2552,(float)-.2733,(float)-.3235,(
	    float)-.3503,(float)-.3596,(float)-.3796,(float)-.3955,(float)
	    -.438,(float)-.1408,(float)-.1527,(float)-.1554,(float)-.1622,(
	    float)-.1679,(float)-.1715,(float)-.1765,(float)-.1821,(float)
	    -.1893,(float)-.1976,(float)-.2041,(float)-.2078,(float)-.2191,(
	    float)-.2354,(float)-.2809,(float)-.3051,(float)-.3137,(float)
	    -.3321,(float)-.3468,(float)-.3858,(float)-.1151,(float)-.1247,(
	    float)-.127,(float)-.1326,(float)-.1373,(float)-.1404,(float)
	    -.1445,(float)-.1491,(float)-.1553,(float)-.1624,(float)-.1681,(
	    float)-.1713,(float)-.1812,(float)-.1958,(float)-.2366,(float)
	    -.2585,(float)-.2664,(float)-.2832,(float)-.2967,(float)-.3327,(
	    float)-.08759,(float)-.09501,(float)-.09675,(float)-.1012,(float)
	    -.1049,(float)-.1074,(float)-.1106,(float)-.1142,(float)-.1193,(
	    float)-.1254,(float)-.1302,(float)-.133,(float)-.1416,(float)
	    -.1545,(float)-.1908,(float)-.2105,(float)-.2177,(float)-.2331,(
	    float)-.2455,(float)-.2786,(float)-.05824,(float)-.06336,(float)
	    -.0646,(float)-.06781,(float)-.07061,(float)-.07246,(float)
	    -.07482,(float)-.07744,(float)-.08147,(float)-.08653,(float)
	    -.09049,(float)-.09274,(float)-.1001,(float)-.1113,(float)-.1433,(
	    float)-.161,(float)-.1676,(float)-.1816,(float)-.1931,(float)
	    -.2236,(float)-.02689,(float)-.02968,(float)-.03042,(float)
	    -.03241,(float)-.03424,(float)-.03549,(float)-.037,(float)-.03862,
	    (float)-.0416,(float)-.04564,(float)-.0488,(float)-.05057,(float)
	    -.05672,(float)-.06637,(float)-.09431,(float)-.1101,(float)-.1161,
	    (float)-.129,(float)-.1395,(float)-.1679,(float).006618,(float)
	    .006164,(float).005929,(float).005143,(float).004279,(float)
	    .003626,(float).002946,(float).002302,(float)3.445e-4,(float)
	    -.002696,(float)-.00508,(float)-.006389,(float)-.01137,(float)
	    -.01952,(float)-.04372,(float)-.05777,(float)-.06327,(float)
	    -.07509,(float)-.08491,(float)-.1114,(float).04241,(float).0443,(
	    float).04456,(float).04498,(float).04506,(float).04499,(float)
	    .04512,(float).04543,(float).04446,(float).04238,(float).04074,(
	    float).03987,(float).03599,(float).02924,(float).008459,(float)
	    -.00408,(float)-.009138,(float)-.02007,(float)-.02925,(float)
	    -.05422,(float).08066,(float).08486,(float).08562,(float).08721,(
	    float).08821,(float).08871,(float).08964,(float).09086,(float)
	    .09084,(float).08967,(float).08872,(float).08826,(float).08541,(
	    float).07994,(float).06221,(float).05096,(float).04626,(float)
	    .03605,(float).02737,(float).003521,(float).1007,(float).1061,(
	    float).1071,(float).1093,(float).1107,(float).1115,(float).1128,(
	    float).1145,(float).1149,(float).1142,(float).1135,(float).1133,(
	    float).1109,(float).106,(float).08967,(float).07897,(float).07443,
	    (float).06451,(float).05603,(float).03259,(float).1215,(float)
	    .128,(float).1292,(float).132,(float).1339,(float).1349,(float)
	    .1366,(float).1387,(float).1396,(float).1392,(float).1389,(float)
	    .1389,(float).1369,(float).1326,(float).1175,(float).1073,(float)
	    .1029,(float).09323,(float).08491,(float).06177,(float).1652,(
	    float).1739,(float).1756,(float).1794,(float).1821,(float).1837,(
	    float).1861,(float).189,(float).1907,(float).1912,(float).1914,(
	    float).1917,(float).1906,(float).1873,(float).1744,(float).1649,(
	    float).1607,(float).1514,(float).1433,(float).1204,(float).2118,(
	    float).2226,(float).2248,(float).2296,(float).2332,(float).2352,(
	    float).2383,(float).2419,(float).2444,(float).2455,(float).2463,(
	    float).2469,(float).2465,(float).244,(float).2327,(float).2238,(
	    float).2197,(float).2105,(float).2025,(float).1794,(float).2615,(
	    float).2744,(float).277,(float).2828,(float).2871,(float).2896,(
	    float).2932,(float).2976,(float).3007,(float).3024,(float).3036,(
	    float).3044,(float).3047,(float).3028,(float).2926,(float).2838,(
	    float).2797,(float).2705,(float).2623,(float).2387,(float).3147,(
	    float).3294,(float).3324,(float).3391,(float).344,(float).3469,(
	    float).3511,(float).356,(float).3596,(float).3619,(float).3634,(
	    float).3644,(float).3651,(float).3637,(float).3539,(float).345,(
	    float).3408,(float).3313,(float).3228,(float).2981,(float).3714,(
	    float).3879,(float).3912,(float).3986,(float).4041,(float).4073,(
	    float).4119,(float).4173,(float).4214,(float).424,(float).4258,(
	    float).4269,(float).4278,(float).4266,(float).4166,(float).4072,(
	    float).4028,(float).3928,(float).3839,(float).3577,(float).4319,(
	    float).4499,(float).4535,(float).4616,(float).4675,(float).471,(
	    float).4759,(float).4816,(float).486,(float).4888,(float).4907,(
	    float).4919,(float).4929,(float).4916,(float).4807,(float).4705,(
	    float).4658,(float).455,(float).4454,(float).4171,(float).4964,(
	    float).5157,(float).5195,(float).5281,(float).5343,(float).538,(
	    float).5431,(float).549,(float).5535,(float).5563,(float).5583,(
	    float).5594,(float).5603,(float).5586,(float).5461,(float).5346,(
	    float).5295,(float).5177,(float).5073,(float).4765,(float).5652,(
	    float).5854,(float).5894,(float).5983,(float).6047,(float).6084,(
	    float).6136,(float).6196,(float).624,(float).6267,(float).6285,(
	    float).6296,(float).63,(float).6278,(float).6127,(float).5996,(
	    float).5939,(float).5808,(float).5694,(float).5357,(float).6386,(
	    float).6593,(float).6633,(float).6723,(float).6788,(float).6825,(
	    float).6876,(float).6934,(float).6976,(float).7,(float).7015,(
	    float).7023,(float).7022,(float).6989,(float).6805,(float).6654,(
	    float).6589,(float).6444,(float).6317,(float).5945,(float).7166,(
	    float).7375,(float).7415,(float).7504,(float).7567,(float).7602,(
	    float).7651,(float).7706,(float).7743,(float).7761,(float).7771,(
	    float).7777,(float).7767,(float).7721,(float).7494,(float).7318,(
	    float).7245,(float).7082,(float).6941,(float).653,(float).7998,(
	    float).8203,(float).8242,(float).8327,(float).8385,(float).8418,(
	    float).8462,(float).8512,(float).8542,(float).8552,(float).8555,(
	    float).8557,(float).8534,(float).8472,(float).8192,(float).7987,(
	    float).7905,(float).7721,(float).7564,(float).7109,(float).8881,(
	    float).9078,(float).9114,(float).9192,(float).9244,(float).9272,(
	    float).9311,(float).9352,(float).9372,(float).9372,(float).9366,(
	    float).9362,(float).9325,(float).9241,(float).8899,(float).866,(
	    float).8568,(float).836,(float).8186,(float).7682,(float).9821,(
	    float)1.,(float)1.003,(float)1.01,(float)1.014,(float)1.017,(
	    float)1.02,(float)1.023,(float)1.023,(float)1.022,(float)1.02,(
	    float)1.019,(float)1.014,(float)1.003,(float).9614,(float).9337,(
	    float).9232,(float).8999,(float).8804,(float).8248,(float)1.082,(
	    float)1.098,(float)1.1,(float)1.106,(float)1.109,(float)1.11,(
	    float)1.112,(float)1.114,(float)1.113,(float)1.11,(float)1.107,(
	    float)1.105,(float)1.097,(float)1.083,(float)1.033,(float)1.001,(
	    float).9896,(float).9635,(float).9419,(float).8805,(float)1.187,(
	    float)1.2,(float)1.202,(float)1.206,(float)1.207,(float)1.208,(
	    float)1.208,(float)1.208,(float)1.205,(float)1.2,(float)1.196,(
	    float)1.193,(float)1.183,(float)1.165,(float)1.106,(float)1.069,(
	    float)1.056,(float)1.027,(float)1.003,(float).9353,(float)1.299,(
	    float)1.308,(float)1.309,(float)1.31,(float)1.31,(float)1.309,(
	    float)1.308,(float)1.306,(float)1.301,(float)1.293,(float)1.287,(
	    float)1.283,(float)1.27,(float)1.249,(float)1.179,(float)1.137,(
	    float)1.122,(float)1.089,(float)1.063,(float).989,(float)1.418,(
	    float)1.422,(float)1.421,(float)1.42,(float)1.417,(float)1.415,(
	    float)1.412,(float)1.407,(float)1.399,(float)1.389,(float)1.381,(
	    float)1.376,(float)1.359,(float)1.333,(float)1.252,(float)1.204,(
	    float)1.188,(float)1.151,(float)1.122,(float)1.042,(float)1.543,(
	    float)1.54,(float)1.539,(float)1.534,(float)1.529,(float)1.525,(
	    float)1.519,(float)1.511,(float)1.501,(float)1.488,(float)1.477,(
	    float)1.47,(float)1.45,(float)1.419,(float)1.325,(float)1.271,(
	    float)1.253,(float)1.213,(float)1.181,(float)1.093,(float)1.674,(
	    float)1.665,(float)1.662,(float)1.653,(float)1.644,(float)1.638,(
	    float)1.63,(float)1.619,(float)1.605,(float)1.588,(float)1.574,(
	    float)1.566,(float)1.542,(float)1.506,(float)1.397,(float)1.337,(
	    float)1.317,(float)1.273,(float)1.238,(float)1.143,(float)1.813,(
	    float)1.794,(float)1.789,(float)1.776,(float)1.764,(float)1.756,(
	    float)1.744,(float)1.729,(float)1.711,(float)1.691,(float)1.674,(
	    float)1.664,(float)1.635,(float)1.593,(float)1.47,(float)1.403,(
	    float)1.38,(float)1.332,(float)1.294,(float)1.191,(float)1.958,(
	    float)1.929,(float)1.922,(float)1.904,(float)1.887,(float)1.876,(
	    float)1.861,(float)1.842,(float)1.82,(float)1.795,(float)1.775,(
	    float)1.763,(float)1.729,(float)1.68,(float)1.541,(float)1.467,(
	    float)1.442,(float)1.39,(float)1.349,(float)1.238,(float)2.11,(
	    float)2.07,(float)2.06,(float)2.035,(float)2.014,(float)2.,(float)
	    1.981,(float)1.958,(float)1.93,(float)1.901,(float)1.877,(float)
	    1.863,(float)1.823,(float)1.768,(float)1.612,(float)1.53,(float)
	    1.503,(float)1.446,(float)1.402,(float)1.283,(float)2.268,(float)
	    2.215,(float)2.202,(float)2.171,(float)2.144,(float)2.127,(float)
	    2.103,(float)2.075,(float)2.042,(float)2.007,(float)1.98,(float)
	    1.963,(float)1.918,(float)1.855,(float)1.681,(float)1.591,(float)
	    1.562,(float)1.501,(float)1.453,(float)1.327,(float)2.433,(float)
	    2.364,(float)2.348,(float)2.309,(float)2.277,(float)2.256,(float)
	    2.228,(float)2.194,(float)2.155,(float)2.114,(float)2.083,(float)
	    2.064,(float)2.012,(float)1.941,(float)1.748,(float)1.651,(float)
	    1.62,(float)1.553,(float)1.502,(float)1.368,(float)2.603,(float)
	    2.517,(float)2.498,(float)2.451,(float)2.412,(float)2.387,(float)
	    2.353,(float)2.313,(float)2.268,(float)2.222,(float)2.185,(float)
	    2.164,(float)2.105,(float)2.026,(float)1.814,(float)1.709,(float)
	    1.675,(float)1.604,(float)1.55,(float)1.408,(float)2.779,(float)
	    2.674,(float)2.65,(float)2.594,(float)2.548,(float)2.519,(float)
	    2.48,(float)2.433,(float)2.381,(float)2.328,(float)2.287,(float)
	    2.263,(float)2.197,(float)2.11,(float)1.878,(float)1.764,(float)
	    1.728,(float)1.652,(float)1.595,(float)1.445,(float)2.958,(float)
	    2.832,(float)2.805,(float)2.738,(float)2.685,(float)2.652,(float)
	    2.606,(float)2.552,(float)2.493,(float)2.434,(float)2.387,(float)
	    2.36,(float)2.287,(float)2.191,(float)1.939,(float)1.817,(float)
	    1.779,(float)1.698,(float)1.638,(float)1.48,(float)3.141,(float)
	    2.992,(float)2.96,(float)2.883,(float)2.822,(float)2.783,(float)
	    2.731,(float)2.67,(float)2.604,(float)2.537,(float)2.486,(float)
	    2.455,(float)2.375,(float)2.27,(float)1.998,(float)1.867,(float)
	    1.827,(float)1.742,(float)1.678,(float)1.513,(float)3.326,(float)
	    3.152,(float)3.115,(float)3.027,(float)2.957,(float)2.913,(float)
	    2.854,(float)2.785,(float)2.711,(float)2.637,(float)2.581,(float)
	    2.547,(float)2.46,(float)2.345,(float)2.053,(float)1.915,(float)
	    1.872,(float)1.782,(float)1.716,(float)1.544,(float)3.51,(float)
	    3.31,(float)3.268,(float)3.168,(float)3.089,(float)3.04,(float)
	    2.974,(float)2.897,(float)2.815,(float)2.734,(float)2.672,(float)
	    2.636,(float)2.54,(float)2.417,(float)2.105,(float)1.959,(float)
	    1.914,(float)1.82,(float)1.75,(float)1.572,(float)3.692,(float)
	    3.465,(float)3.418,(float)3.305,(float)3.217,(float)3.163,(float)
	    3.089,(float)3.005,(float)2.915,(float)2.826,(float)2.759,(float)
	    2.719,(float)2.617,(float)2.484,(float)2.153,(float)1.999,(float)
	    1.952,(float)1.855,(float)1.782,(float)1.597,(float)3.87,(float)
	    3.615,(float)3.561,(float)3.436,(float)3.339,(float)3.279,(float)
	    3.199,(float)3.106,(float)3.008,(float)2.913,(float)2.84,(float)
	    2.797,(float)2.687,(float)2.546,(float)2.197,(float)2.036,(float)
	    1.987,(float)1.886,(float)1.811,(float)1.62,(float)4.039,(float)
	    3.756,(float)3.697,(float)3.559,(float)3.453,(float)3.388,(float)
	    3.3,(float)3.2,(float)3.095,(float)2.992,(float)2.914,(float)
	    2.869,(float)2.752,(float)2.603,(float)2.236,(float)2.069,(float)
	    2.018,(float)1.914,(float)1.836,(float)1.641,(float)4.198,(float)
	    3.887,(float)3.823,(float)3.673,(float)3.558,(float)3.488,(float)
	    3.393,(float)3.285,(float)3.173,(float)3.063,(float)2.981,(float)
	    2.933,(float)2.81,(float)2.653,(float)2.271,(float)2.098,(float)
	    2.046,(float)1.938,(float)1.859,(float)1.658,(float)4.342,(float)
	    4.004,(float)3.935,(float)3.773,(float)3.65,(float)3.575,(float)
	    3.474,(float)3.36,(float)3.241,(float)3.126,(float)3.039,(float)
	    2.988,(float)2.859,(float)2.696,(float)2.301,(float)2.123,(float)
	    2.069,(float)1.959,(float)1.878,(float)1.673,(float)4.467,(float)
	    4.104,(float)4.031,(float)3.859,(float)3.729,(float)3.65,(float)
	    3.543,(float)3.423,(float)3.298,(float)3.178,(float)3.088,(float)
	    3.035,(float)2.901,(float)2.732,(float)2.325,(float)2.143,(float)
	    2.088,(float)1.976,(float)1.893,(float)1.685,(float)4.568,(float)
	    4.185,(float)4.108,(float)3.928,(float)3.791,(float)3.709,(float)
	    3.598,(float)3.473,(float)3.343,(float)3.219,(float)3.126,(float)
	    3.071,(float)2.933,(float)2.76,(float)2.344,(float)2.159,(float)
	    2.103,(float)1.989,(float)1.905,(float)1.695,(float)4.642,(float)
	    4.244,(float)4.163,(float)3.977,(float)3.836,(float)3.751,(float)
	    3.637,(float)3.508,(float)3.375,(float)3.248,(float)3.152,(float)
	    3.097,(float)2.956,(float)2.78,(float)2.357,(float)2.17,(float)
	    2.113,(float)1.998,(float)1.913,(float)1.701,(float)4.685,(float)
	    4.277,(float)4.195,(float)4.005,(float)3.862,(float)3.775,(float)
	    3.659,(float)3.528,(float)3.393,(float)3.264,(float)3.168,(float)
	    3.111,(float)2.969,(float)2.791,(float)2.364,(float)2.176,(float)
	    2.119,(float)2.003,(float)1.918,(float)1.705,(float)4.695,(float)
	    4.285,(float)4.203,(float)4.012,(float)3.868,(float)3.781,(float)
	    3.664,(float)3.533,(float)3.398,(float)3.268,(float)3.171,(float)
	    3.115,(float)2.972,(float)2.793,(float)2.366,(float)2.177,(float)
	    2.12,(float)2.004,(float)1.919,(float)1.706 };

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
/* Soot model */
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
/*<       vi_m=6.05630412E-05 >*/
    vi_m__ = (float)6.05630412e-5;
/*<       vi(4)=vi_m >*/
    sixs_coef__1.vi[3] = vi_m__;
/*<       do 1 i=1,20 >*/
    for (i__ = 1; i__ <= 20; ++i__) {
/*<        asy(4,i)=asy_m(i) >*/
	sixs_coef__1.asy[(i__ << 2) - 1] = asy_m__[i__ - 1];
/*<        ex(4,i)=ex_m(i) >*/
	sixs_coef__1.ex[(i__ << 2) - 1] = ex_m__[i__ - 1];
/*<        sc(4,i)=sc_m(i) >*/
	sixs_coef__1.sc[(i__ << 2) - 1] = sc_m__[i__ - 1];
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
} /* soot_ */

#ifdef __cplusplus
	}
#endif
