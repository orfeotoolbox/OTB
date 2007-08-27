/* AATSR.f -- translated by f2c (version 19970805).
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
    real s[1501], wlinf, wlsup;
} sixs_ffu__;

#define sixs_ffu__1 sixs_ffu__

/*<       subroutine aatsr(iwa) >*/
/* Subroutine */ int aatsr_(integer *iwa)
{
    /* Initialized data */

    static struct {
	real e_1[4];
	integer fill_2[4];
	real e_3[4];
	integer fill_4[4];
	real e_5[4];
	integer fill_6[4];
	real e_7[4];
	integer fill_8[4];
	real e_9[4];
	integer fill_10[4];
	real e_11[4];
	integer fill_12[4];
	real e_13[4];
	integer fill_14[4];
	real e_15[4];
	integer fill_16[4];
	real e_17[4];
	integer fill_18[4];
	real e_19[4];
	integer fill_20[4];
	real e_21[4];
	integer fill_22[4];
	real e_23[4];
	integer fill_24[4];
	real e_25[4];
	integer fill_26[4];
	real e_27[4];
	integer fill_28[4];
	real e_29[4];
	integer fill_30[4];
	real e_31[4];
	integer fill_32[4];
	real e_33[4];
	integer fill_34[4];
	real e_35[4];
	integer fill_36[4];
	real e_37[4];
	integer fill_38[4];
	real e_39[4];
	integer fill_40[4];
	real e_41[4];
	integer fill_42[4];
	real e_43[4];
	integer fill_44[4];
	real e_45[4];
	integer fill_46[4];
	real e_47[4];
	integer fill_48[4];
	real e_49[4];
	integer fill_50[4];
	real e_51[4];
	integer fill_52[4];
	real e_53[4];
	integer fill_54[4];
	real e_55[4];
	integer fill_56[4];
	real e_57[4];
	integer fill_58[4];
	real e_59[4];
	integer fill_60[4];
	real e_61[4];
	integer fill_62[4];
	real e_63[4];
	integer fill_64[4];
	real e_65[4];
	integer fill_66[4];
	real e_67[4];
	integer fill_68[4];
	real e_69[4];
	integer fill_70[4];
	real e_71[4];
	integer fill_72[4];
	real e_73[4];
	integer fill_74[4];
	real e_75[4];
	integer fill_76[4];
	real e_77[4];
	integer fill_78[4];
	real e_79[4];
	integer fill_80[4];
	real e_81[4];
	integer fill_82[4];
	real e_83[4];
	integer fill_84[4];
	real e_85[4];
	integer fill_86[4];
	real e_87[4];
	integer fill_88[4];
	real e_89[4];
	integer fill_90[4];
	real e_91[4];
	integer fill_92[4];
	real e_93[4];
	integer fill_94[4];
	real e_95[4];
	integer fill_96[4];
	real e_97[4];
	integer fill_98[4];
	real e_99[4];
	integer fill_100[4];
	real e_101[4];
	integer fill_102[4];
	real e_103[4];
	integer fill_104[4];
	real e_105[4];
	integer fill_106[4];
	real e_107[4];
	integer fill_108[4];
	real e_109[4];
	integer fill_110[4];
	real e_111[4];
	integer fill_112[4];
	real e_113[4];
	integer fill_114[4];
	real e_115[4];
	integer fill_116[4];
	real e_117[4];
	integer fill_118[4];
	real e_119[4];
	integer fill_120[4];
	real e_121[4];
	integer fill_122[4];
	real e_123[4];
	integer fill_124[4];
	real e_125[4];
	integer fill_126[4];
	real e_127[4];
	integer fill_128[4];
	real e_129[4];
	integer fill_130[4];
	real e_131[4];
	integer fill_132[4];
	real e_133[4];
	integer fill_134[4];
	real e_135[4];
	integer fill_136[4];
	real e_137[4];
	integer fill_138[4];
	real e_139[4];
	integer fill_140[4];
	real e_141[4];
	integer fill_142[4];
	real e_143[4];
	integer fill_144[4];
	real e_145[4];
	integer fill_146[4];
	real e_147[4];
	integer fill_148[4];
	real e_149[4];
	integer fill_150[4];
	real e_151[4];
	integer fill_152[4];
	real e_153[4];
	integer fill_154[4];
	real e_155[4];
	integer fill_156[4];
	real e_157[4];
	integer fill_158[4];
	real e_159[4];
	integer fill_160[4];
	real e_161[4];
	integer fill_162[4];
	real e_163[4];
	integer fill_164[4];
	real e_165[4];
	integer fill_166[4];
	real e_167[4];
	integer fill_168[4];
	real e_169[4];
	integer fill_170[4];
	real e_171[4];
	integer fill_172[4];
	real e_173[4];
	integer fill_174[4];
	real e_175[4];
	integer fill_176[4];
	real e_177[4];
	integer fill_178[4];
	real e_179[4];
	integer fill_180[4];
	real e_181[4];
	integer fill_182[4];
	real e_183[4];
	integer fill_184[4];
	real e_185[4];
	integer fill_186[4];
	real e_187[4];
	integer fill_188[4];
	real e_189[4];
	integer fill_190[4];
	real e_191[4];
	integer fill_192[4];
	real e_193[4];
	integer fill_194[4];
	real e_195[4];
	integer fill_196[4];
	real e_197[4];
	integer fill_198[4];
	real e_199[4];
	integer fill_200[4];
	real e_201[4];
	integer fill_202[4];
	real e_203[4];
	integer fill_204[4];
	real e_205[4];
	integer fill_206[4];
	real e_207[4];
	integer fill_208[4];
	real e_209[4];
	integer fill_210[4];
	real e_211[4];
	integer fill_212[4];
	real e_213[4];
	integer fill_214[4];
	real e_215[4];
	integer fill_216[4];
	real e_217[4];
	integer fill_218[4];
	real e_219[4];
	integer fill_220[4];
	real e_221[4];
	integer fill_222[4];
	real e_223[4];
	integer fill_224[4];
	real e_225[4];
	integer fill_226[4];
	real e_227[4];
	integer fill_228[4];
	real e_229[4];
	integer fill_230[4];
	real e_231[4];
	integer fill_232[4];
	real e_233[4];
	integer fill_234[4];
	real e_235[4];
	integer fill_236[4];
	real e_237[4];
	integer fill_238[4];
	real e_239[4];
	integer fill_240[4];
	real e_241[4];
	integer fill_242[4];
	real e_243[4];
	integer fill_244[4];
	real e_245[4];
	integer fill_246[4];
	real e_247[4];
	integer fill_248[4];
	real e_249[4];
	integer fill_250[4];
	real e_251[4];
	integer fill_252[4];
	real e_253[4];
	integer fill_254[4];
	real e_255[4];
	integer fill_256[4];
	real e_257[4];
	integer fill_258[4];
	real e_259[4];
	integer fill_260[4];
	real e_261[4];
	integer fill_262[4];
	real e_263[4];
	integer fill_264[4];
	real e_265[4];
	integer fill_266[4];
	real e_267[4];
	integer fill_268[4];
	real e_269[4];
	integer fill_270[4];
	real e_271[4];
	integer fill_272[4];
	real e_273[4];
	integer fill_274[4];
	real e_275[4];
	integer fill_276[4];
	real e_277[4];
	integer fill_278[4];
	real e_279[4];
	integer fill_280[4];
	real e_281[4];
	integer fill_282[4];
	real e_283[4];
	integer fill_284[4];
	real e_285[4];
	integer fill_286[4];
	real e_287[4];
	integer fill_288[4];
	real e_289[4];
	integer fill_290[4];
	real e_291[4];
	integer fill_292[4];
	real e_293[4];
	integer fill_294[4];
	real e_295[4];
	integer fill_296[4];
	real e_297[4];
	integer fill_298[4];
	real e_299[4];
	integer fill_300[4];
	real e_301[4];
	integer fill_302[4];
	real e_303[4];
	integer fill_304[4];
	real e_305[4];
	integer fill_306[4];
	real e_307[4];
	integer fill_308[4];
	real e_309[4];
	integer fill_310[4];
	real e_311[4];
	integer fill_312[4];
	real e_313[4];
	integer fill_314[4];
	real e_315[4];
	integer fill_316[4];
	real e_317[4];
	integer fill_318[4];
	real e_319[4];
	integer fill_320[4];
	real e_321[4];
	integer fill_322[4];
	real e_323[4];
	integer fill_324[4];
	real e_325[4];
	integer fill_326[4];
	real e_327[4];
	integer fill_328[4];
	real e_329[4];
	integer fill_330[4];
	real e_331[4];
	integer fill_332[4];
	real e_333[4];
	integer fill_334[4];
	real e_335[4];
	integer fill_336[4];
	real e_337[4];
	integer fill_338[4];
	real e_339[4];
	integer fill_340[4];
	real e_341[4];
	integer fill_342[4];
	real e_343[4];
	integer fill_344[4];
	real e_345[4];
	integer fill_346[4];
	real e_347[4];
	integer fill_348[4];
	real e_349[4];
	integer fill_350[4];
	real e_351[4];
	integer fill_352[4];
	real e_353[4];
	integer fill_354[4];
	real e_355[4];
	integer fill_356[4];
	real e_357[4];
	integer fill_358[4];
	real e_359[4];
	integer fill_360[4];
	real e_361[4];
	integer fill_362[4];
	real e_363[4];
	integer fill_364[4];
	real e_365[4];
	integer fill_366[4];
	real e_367[4];
	integer fill_368[4];
	real e_369[4];
	integer fill_370[4];
	real e_371[4];
	integer fill_372[4];
	real e_373[4];
	integer fill_374[4];
	real e_375[4];
	integer fill_376[4];
	real e_377[4];
	integer fill_378[4];
	real e_379[4];
	integer fill_380[4];
	real e_381[4];
	integer fill_382[4];
	real e_383[4];
	integer fill_384[4];
	real e_385[4];
	integer fill_386[4];
	real e_387[4];
	integer fill_388[4];
	real e_389[4];
	integer fill_390[4];
	real e_391[4];
	integer fill_392[4];
	real e_393[4];
	integer fill_394[4];
	real e_395[4];
	integer fill_396[4];
	real e_397[4];
	integer fill_398[4];
	real e_399[4];
	integer fill_400[4];
	real e_401[4];
	integer fill_402[4];
	real e_403[4];
	integer fill_404[4];
	real e_405[4];
	integer fill_406[4];
	real e_407[4];
	integer fill_408[4];
	real e_409[4];
	integer fill_410[4];
	real e_411[4];
	integer fill_412[4];
	real e_413[4];
	integer fill_414[4];
	real e_415[4];
	integer fill_416[4];
	real e_417[4];
	integer fill_418[4];
	real e_419[4];
	integer fill_420[4];
	real e_421[4];
	integer fill_422[4];
	real e_423[4];
	integer fill_424[4];
	real e_425[4];
	integer fill_426[4];
	real e_427[4];
	integer fill_428[4];
	real e_429[4];
	integer fill_430[4];
	real e_431[4];
	integer fill_432[4];
	real e_433[4];
	integer fill_434[4];
	real e_435[4];
	integer fill_436[4];
	real e_437[4];
	integer fill_438[4];
	real e_439[4];
	integer fill_440[4];
	real e_441[4];
	integer fill_442[4];
	real e_443[4];
	integer fill_444[4];
	real e_445[4];
	integer fill_446[4];
	real e_447[4];
	integer fill_448[4];
	real e_449[4];
	integer fill_450[4];
	real e_451[4];
	integer fill_452[4];
	real e_453[4];
	integer fill_454[4];
	real e_455[4];
	integer fill_456[4];
	real e_457[4];
	integer fill_458[4];
	real e_459[4];
	integer fill_460[4];
	real e_461[4];
	integer fill_462[4];
	real e_463[4];
	integer fill_464[4];
	real e_465[4];
	integer fill_466[4];
	real e_467[4];
	integer fill_468[4];
	real e_469[4];
	integer fill_470[4];
	real e_471[4];
	integer fill_472[4];
	real e_473[4];
	integer fill_474[4];
	real e_475[4];
	integer fill_476[4];
	real e_477[4];
	integer fill_478[4];
	real e_479[4];
	integer fill_480[4];
	real e_481[4];
	integer fill_482[4];
	real e_483[4];
	integer fill_484[4];
	real e_485[4];
	integer fill_486[4];
	real e_487[4];
	integer fill_488[4];
	real e_489[4];
	integer fill_490[4];
	real e_491[4];
	integer fill_492[4];
	real e_493[4];
	integer fill_494[4];
	real e_495[4];
	integer fill_496[4];
	real e_497[4];
	integer fill_498[4];
	real e_499[4];
	integer fill_500[4];
	real e_501[4];
	integer fill_502[4];
	real e_503[4];
	integer fill_504[4];
	real e_505[4];
	integer fill_506[4];
	real e_507[4];
	integer fill_508[4];
	real e_509[4];
	integer fill_510[4];
	real e_511[4];
	integer fill_512[4];
	real e_513[4];
	integer fill_514[4];
	real e_515[4];
	integer fill_516[4];
	real e_517[4];
	integer fill_518[4];
	real e_519[4];
	integer fill_520[4];
	real e_521[4];
	integer fill_522[4];
	real e_523[4];
	integer fill_524[4];
	real e_525[4];
	integer fill_526[4];
	real e_527[4];
	integer fill_528[4];
	real e_529[4];
	integer fill_530[4];
	real e_531[4];
	integer fill_532[4];
	real e_533[4];
	integer fill_534[4];
	real e_535[4];
	integer fill_536[4];
	real e_537[4];
	integer fill_538[4];
	real e_539[4];
	integer fill_540[4];
	real e_541[4];
	integer fill_542[4];
	real e_543[4];
	integer fill_544[4];
	real e_545[4];
	integer fill_546[4];
	real e_547[4];
	integer fill_548[4];
	real e_549[4];
	integer fill_550[4];
	real e_551[4];
	integer fill_552[4];
	real e_553[4];
	integer fill_554[4];
	real e_555[4];
	integer fill_556[4];
	real e_557[4];
	integer fill_558[4];
	real e_559[4];
	integer fill_560[4];
	real e_561[4];
	integer fill_562[4];
	real e_563[4];
	integer fill_564[4];
	real e_565[4];
	integer fill_566[4];
	real e_567[4];
	integer fill_568[4];
	real e_569[4];
	integer fill_570[4];
	real e_571[4];
	integer fill_572[4];
	real e_573[4];
	integer fill_574[4];
	real e_575[4];
	integer fill_576[4];
	real e_577[4];
	integer fill_578[4];
	real e_579[4];
	integer fill_580[4];
	real e_581[4];
	integer fill_582[4];
	real e_583[4];
	integer fill_584[4];
	real e_585[4];
	integer fill_586[4];
	real e_587[4];
	integer fill_588[4];
	real e_589[4];
	integer fill_590[4];
	real e_591[4];
	integer fill_592[4];
	real e_593[4];
	integer fill_594[4];
	real e_595[4];
	integer fill_596[4];
	real e_597[4];
	integer fill_598[4];
	real e_599[4];
	integer fill_600[4];
	real e_601[4];
	integer fill_602[4];
	real e_603[4];
	integer fill_604[4];
	real e_605[4];
	integer fill_606[4];
	real e_607[4];
	integer fill_608[4];
	real e_609[4];
	integer fill_610[4];
	real e_611[4];
	integer fill_612[4];
	real e_613[4];
	integer fill_614[4];
	real e_615[4];
	integer fill_616[4];
	real e_617[4];
	integer fill_618[4];
	real e_619[4];
	integer fill_620[4];
	real e_621[4];
	integer fill_622[4];
	real e_623[4];
	integer fill_624[4];
	real e_625[4];
	integer fill_626[4];
	real e_627[4];
	integer fill_628[4];
	real e_629[4];
	integer fill_630[4];
	real e_631[4];
	integer fill_632[4];
	real e_633[4];
	integer fill_634[4];
	real e_635[4];
	integer fill_636[4];
	real e_637[4];
	integer fill_638[4];
	real e_639[4];
	integer fill_640[4];
	real e_641[4];
	integer fill_642[4];
	real e_643[4];
	integer fill_644[4];
	real e_645[4];
	integer fill_646[4];
	real e_647[4];
	integer fill_648[4];
	real e_649[4];
	integer fill_650[4];
	real e_651[4];
	integer fill_652[4];
	real e_653[4];
	integer fill_654[4];
	real e_655[4];
	integer fill_656[4];
	real e_657[4];
	integer fill_658[4];
	real e_659[4];
	integer fill_660[4];
	real e_661[4];
	integer fill_662[4];
	real e_663[4];
	integer fill_664[4];
	real e_665[4];
	integer fill_666[4];
	real e_667[4];
	integer fill_668[4];
	real e_669[4];
	integer fill_670[4];
	real e_671[4];
	integer fill_672[4];
	real e_673[4];
	integer fill_674[4];
	real e_675[4];
	integer fill_676[4];
	real e_677[4];
	integer fill_678[4];
	real e_679[4];
	integer fill_680[4];
	real e_681[4];
	integer fill_682[4];
	real e_683[4];
	integer fill_684[4];
	real e_685[4];
	integer fill_686[4];
	real e_687[4];
	integer fill_688[4];
	real e_689[4];
	integer fill_690[4];
	real e_691[4];
	integer fill_692[4];
	real e_693[4];
	integer fill_694[4];
	real e_695[4];
	integer fill_696[4];
	real e_697[4];
	integer fill_698[4];
	real e_699[4];
	integer fill_700[4];
	real e_701[4];
	integer fill_702[4];
	real e_703[4];
	integer fill_704[4];
	real e_705[4];
	integer fill_706[4];
	real e_707[4];
	integer fill_708[4];
	real e_709[4];
	integer fill_710[4];
	real e_711[4];
	integer fill_712[4];
	real e_713[4];
	integer fill_714[4];
	real e_715[4];
	integer fill_716[4];
	real e_717[4];
	integer fill_718[4];
	real e_719[4];
	integer fill_720[4];
	real e_721[4];
	integer fill_722[4];
	real e_723[4];
	integer fill_724[4];
	real e_725[4];
	integer fill_726[4];
	real e_727[4];
	integer fill_728[4];
	real e_729[4];
	integer fill_730[4];
	real e_731[4];
	integer fill_732[4];
	real e_733[4];
	integer fill_734[4];
	real e_735[4];
	integer fill_736[4];
	real e_737[4];
	integer fill_738[4];
	real e_739[4];
	integer fill_740[4];
	real e_741[4];
	integer fill_742[4];
	real e_743[4];
	integer fill_744[4];
	real e_745[4];
	integer fill_746[4];
	real e_747[4];
	integer fill_748[4];
	real e_749[4];
	integer fill_750[4];
	real e_751[4];
	integer fill_752[4];
	real e_753[4];
	integer fill_754[4];
	real e_755[4];
	integer fill_756[4];
	real e_757[4];
	integer fill_758[4];
	real e_759[4];
	integer fill_760[4];
	real e_761[4];
	integer fill_762[4];
	real e_763[4];
	integer fill_764[4];
	real e_765[4];
	integer fill_766[4];
	real e_767[4];
	integer fill_768[4];
	real e_769[4];
	integer fill_770[4];
	real e_771[4];
	integer fill_772[4];
	real e_773[4];
	integer fill_774[4];
	real e_775[4];
	integer fill_776[4];
	real e_777[4];
	integer fill_778[4];
	real e_779[4];
	integer fill_780[4];
	real e_781[4];
	integer fill_782[4];
	real e_783[4];
	integer fill_784[4];
	real e_785[4];
	integer fill_786[4];
	real e_787[4];
	integer fill_788[4];
	real e_789[4];
	integer fill_790[4];
	real e_791[4];
	integer fill_792[4];
	real e_793[4];
	integer fill_794[4];
	real e_795[4];
	integer fill_796[4];
	real e_797[4];
	integer fill_798[4];
	real e_799[4];
	integer fill_800[4];
	real e_801[4];
	integer fill_802[4];
	real e_803[4];
	integer fill_804[4];
	real e_805[4];
	integer fill_806[4];
	real e_807[4];
	integer fill_808[4];
	real e_809[4];
	integer fill_810[4];
	real e_811[4];
	integer fill_812[4];
	real e_813[4];
	integer fill_814[4];
	real e_815[4];
	integer fill_816[4];
	real e_817[4];
	integer fill_818[4];
	real e_819[4];
	integer fill_820[4];
	real e_821[4];
	integer fill_822[4];
	real e_823[4];
	integer fill_824[4];
	real e_825[4];
	integer fill_826[4];
	real e_827[4];
	integer fill_828[4];
	real e_829[4];
	integer fill_830[4];
	real e_831[4];
	integer fill_832[4];
	real e_833[4];
	integer fill_834[4];
	real e_835[4];
	integer fill_836[4];
	real e_837[4];
	integer fill_838[4];
	real e_839[4];
	integer fill_840[4];
	real e_841[4];
	integer fill_842[4];
	real e_843[4];
	integer fill_844[4];
	real e_845[4];
	integer fill_846[4];
	real e_847[4];
	integer fill_848[4];
	real e_849[4];
	integer fill_850[4];
	real e_851[4];
	integer fill_852[4];
	real e_853[4];
	integer fill_854[4];
	real e_855[4];
	integer fill_856[4];
	real e_857[4];
	integer fill_858[4];
	real e_859[4];
	integer fill_860[4];
	real e_861[4];
	integer fill_862[4];
	real e_863[4];
	integer fill_864[4];
	real e_865[4];
	integer fill_866[4];
	real e_867[4];
	integer fill_868[4];
	real e_869[4];
	integer fill_870[4];
	real e_871[4];
	integer fill_872[4];
	real e_873[4];
	integer fill_874[4];
	real e_875[4];
	integer fill_876[4];
	real e_877[4];
	integer fill_878[4];
	real e_879[4];
	integer fill_880[4];
	real e_881[4];
	integer fill_882[4];
	real e_883[4];
	integer fill_884[4];
	real e_885[4];
	integer fill_886[4];
	real e_887[4];
	integer fill_888[4];
	real e_889[4];
	integer fill_890[4];
	real e_891[4];
	integer fill_892[4];
	real e_893[4];
	integer fill_894[4];
	real e_895[4];
	integer fill_896[4];
	real e_897[4];
	integer fill_898[4];
	real e_899[4];
	integer fill_900[4];
	real e_901[4];
	integer fill_902[4];
	real e_903[4];
	integer fill_904[4];
	real e_905[4];
	integer fill_906[4];
	real e_907[4];
	integer fill_908[4];
	real e_909[4];
	integer fill_910[4];
	real e_911[4];
	integer fill_912[4];
	real e_913[4];
	integer fill_914[4];
	real e_915[4];
	integer fill_916[4];
	real e_917[4];
	integer fill_918[4];
	real e_919[4];
	integer fill_920[4];
	real e_921[4];
	integer fill_922[4];
	real e_923[4];
	integer fill_924[4];
	real e_925[4];
	integer fill_926[4];
	real e_927[4];
	integer fill_928[4];
	real e_929[4];
	integer fill_930[4];
	real e_931[4];
	integer fill_932[4];
	real e_933[4];
	integer fill_934[4];
	real e_935[4];
	integer fill_936[4];
	real e_937[4];
	integer fill_938[4];
	real e_939[4];
	integer fill_940[4];
	real e_941[4];
	integer fill_942[4];
	real e_943[4];
	integer fill_944[4];
	real e_945[4];
	integer fill_946[4];
	real e_947[4];
	integer fill_948[4];
	real e_949[4];
	integer fill_950[4];
	real e_951[4];
	integer fill_952[4];
	real e_953[4];
	integer fill_954[4];
	real e_955[4];
	integer fill_956[4];
	real e_957[4];
	integer fill_958[4];
	real e_959[4];
	integer fill_960[4];
	real e_961[4];
	integer fill_962[4];
	real e_963[4];
	integer fill_964[4];
	real e_965[4];
	integer fill_966[4];
	real e_967[4];
	integer fill_968[4];
	real e_969[4];
	integer fill_970[4];
	real e_971[4];
	integer fill_972[4];
	real e_973[4];
	integer fill_974[4];
	real e_975[4];
	integer fill_976[4];
	real e_977[4];
	integer fill_978[4];
	real e_979[4];
	integer fill_980[4];
	real e_981[4];
	integer fill_982[4];
	real e_983[4];
	integer fill_984[4];
	real e_985[4];
	integer fill_986[4];
	real e_987[4];
	integer fill_988[4];
	real e_989[4];
	integer fill_990[4];
	real e_991[4];
	integer fill_992[4];
	real e_993[4];
	integer fill_994[4];
	real e_995[4];
	integer fill_996[4];
	real e_997[4];
	integer fill_998[4];
	real e_999[4];
	integer fill_1000[4];
	real e_1001[4];
	integer fill_1002[4];
	real e_1003[4];
	integer fill_1004[4];
	real e_1005[4];
	integer fill_1006[4];
	real e_1007[4];
	integer fill_1008[4];
	real e_1009[4];
	integer fill_1010[4];
	real e_1011[4];
	integer fill_1012[4];
	real e_1013[4];
	integer fill_1014[4];
	real e_1015[4];
	integer fill_1016[4];
	real e_1017[4];
	integer fill_1018[4];
	real e_1019[4];
	integer fill_1020[4];
	real e_1021[4];
	integer fill_1022[4];
	real e_1023[4];
	integer fill_1024[4];
	real e_1025[4];
	integer fill_1026[4];
	real e_1027[4];
	integer fill_1028[4];
	real e_1029[4];
	integer fill_1030[4];
	real e_1031[4];
	integer fill_1032[4];
	real e_1033[4];
	integer fill_1034[4];
	real e_1035[4];
	integer fill_1036[4];
	real e_1037[4];
	integer fill_1038[4];
	real e_1039[4];
	integer fill_1040[4];
	real e_1041[4];
	integer fill_1042[4];
	real e_1043[4];
	integer fill_1044[4];
	real e_1045[4];
	integer fill_1046[4];
	real e_1047[4];
	integer fill_1048[4];
	real e_1049[4];
	integer fill_1050[4];
	real e_1051[4];
	integer fill_1052[4];
	real e_1053[4];
	integer fill_1054[4];
	real e_1055[4];
	integer fill_1056[4];
	real e_1057[4];
	integer fill_1058[4];
	real e_1059[4];
	integer fill_1060[4];
	real e_1061[4];
	integer fill_1062[4];
	real e_1063[4];
	integer fill_1064[4];
	real e_1065[4];
	integer fill_1066[4];
	real e_1067[4];
	integer fill_1068[4];
	real e_1069[4];
	integer fill_1070[4];
	real e_1071[4];
	integer fill_1072[4];
	real e_1073[4];
	integer fill_1074[4];
	real e_1075[4];
	integer fill_1076[4];
	real e_1077[4];
	integer fill_1078[4];
	real e_1079[4];
	integer fill_1080[4];
	real e_1081[4];
	integer fill_1082[4];
	real e_1083[4];
	integer fill_1084[4];
	real e_1085[4];
	integer fill_1086[4];
	real e_1087[4];
	integer fill_1088[4];
	real e_1089[4];
	integer fill_1090[4];
	real e_1091[4];
	integer fill_1092[4];
	real e_1093[4];
	integer fill_1094[4];
	real e_1095[4];
	integer fill_1096[4];
	real e_1097[4];
	integer fill_1098[4];
	real e_1099[4];
	integer fill_1100[4];
	real e_1101[4];
	integer fill_1102[4];
	real e_1103[4];
	integer fill_1104[4];
	real e_1105[4];
	integer fill_1106[4];
	real e_1107[4];
	integer fill_1108[4];
	real e_1109[4];
	integer fill_1110[4];
	real e_1111[4];
	integer fill_1112[4];
	real e_1113[4];
	integer fill_1114[4];
	real e_1115[4];
	integer fill_1116[4];
	real e_1117[4];
	integer fill_1118[4];
	real e_1119[4];
	integer fill_1120[4];
	real e_1121[4];
	integer fill_1122[4];
	real e_1123[4];
	integer fill_1124[4];
	real e_1125[4];
	integer fill_1126[4];
	real e_1127[4];
	integer fill_1128[4];
	real e_1129[4];
	integer fill_1130[4];
	real e_1131[4];
	integer fill_1132[4];
	real e_1133[4];
	integer fill_1134[4];
	real e_1135[4];
	integer fill_1136[4];
	real e_1137[4];
	integer fill_1138[4];
	real e_1139[4];
	integer fill_1140[4];
	real e_1141[4];
	integer fill_1142[4];
	real e_1143[4];
	integer fill_1144[4];
	real e_1145[4];
	integer fill_1146[4];
	real e_1147[4];
	integer fill_1148[4];
	real e_1149[4];
	integer fill_1150[4];
	real e_1151[4];
	integer fill_1152[4];
	real e_1153[4];
	integer fill_1154[4];
	real e_1155[4];
	integer fill_1156[4];
	real e_1157[4];
	integer fill_1158[4];
	real e_1159[4];
	integer fill_1160[4];
	real e_1161[4];
	integer fill_1162[4];
	real e_1163[4];
	integer fill_1164[4];
	real e_1165[4];
	integer fill_1166[4];
	real e_1167[4];
	integer fill_1168[4];
	real e_1169[4];
	integer fill_1170[4];
	real e_1171[4];
	integer fill_1172[4];
	real e_1173[4];
	integer fill_1174[4];
	real e_1175[4];
	integer fill_1176[4];
	real e_1177[4];
	integer fill_1178[4];
	real e_1179[4];
	integer fill_1180[4];
	real e_1181[4];
	integer fill_1182[4];
	real e_1183[4];
	integer fill_1184[4];
	real e_1185[4];
	integer fill_1186[4];
	real e_1187[4];
	integer fill_1188[4];
	real e_1189[4];
	integer fill_1190[4];
	real e_1191[4];
	integer fill_1192[4];
	real e_1193[4];
	integer fill_1194[4];
	real e_1195[4];
	integer fill_1196[4];
	real e_1197[4];
	integer fill_1198[4];
	real e_1199[4];
	integer fill_1200[4];
	real e_1201[4];
	integer fill_1202[4];
	real e_1203[4];
	integer fill_1204[4];
	real e_1205[4];
	integer fill_1206[4];
	real e_1207[4];
	integer fill_1208[4];
	real e_1209[4];
	integer fill_1210[4];
	real e_1211[4];
	integer fill_1212[4];
	real e_1213[4];
	integer fill_1214[4];
	real e_1215[4];
	integer fill_1216[4];
	real e_1217[4];
	integer fill_1218[4];
	real e_1219[4];
	integer fill_1220[4];
	real e_1221[4];
	integer fill_1222[4];
	real e_1223[4];
	integer fill_1224[4];
	real e_1225[4];
	integer fill_1226[4];
	real e_1227[4];
	integer fill_1228[4];
	real e_1229[4];
	integer fill_1230[4];
	real e_1231[4];
	integer fill_1232[4];
	real e_1233[4];
	integer fill_1234[4];
	real e_1235[4];
	integer fill_1236[4];
	real e_1237[4];
	integer fill_1238[4];
	real e_1239[4];
	integer fill_1240[4];
	real e_1241[4];
	integer fill_1242[4];
	real e_1243[4];
	integer fill_1244[4];
	real e_1245[4];
	integer fill_1246[4];
	real e_1247[4];
	integer fill_1248[4];
	real e_1249[4];
	integer fill_1250[4];
	real e_1251[4];
	integer fill_1252[4];
	real e_1253[4];
	integer fill_1254[4];
	real e_1255[4];
	integer fill_1256[4];
	real e_1257[4];
	integer fill_1258[4];
	real e_1259[4];
	integer fill_1260[4];
	real e_1261[4];
	integer fill_1262[4];
	real e_1263[4];
	integer fill_1264[4];
	real e_1265[4];
	integer fill_1266[4];
	real e_1267[4];
	integer fill_1268[4];
	real e_1269[4];
	integer fill_1270[4];
	real e_1271[4];
	integer fill_1272[4];
	real e_1273[4];
	integer fill_1274[4];
	real e_1275[4];
	integer fill_1276[4];
	real e_1277[4];
	integer fill_1278[4];
	real e_1279[4];
	integer fill_1280[4];
	real e_1281[4];
	integer fill_1282[4];
	real e_1283[4];
	integer fill_1284[4];
	real e_1285[4];
	integer fill_1286[4];
	real e_1287[4];
	integer fill_1288[4];
	real e_1289[4];
	integer fill_1290[4];
	real e_1291[4];
	integer fill_1292[4];
	real e_1293[4];
	integer fill_1294[4];
	real e_1295[4];
	integer fill_1296[4];
	real e_1297[4];
	integer fill_1298[4];
	real e_1299[4];
	integer fill_1300[4];
	real e_1301[4];
	integer fill_1302[4];
	real e_1303[4];
	integer fill_1304[4];
	real e_1305[4];
	integer fill_1306[4];
	real e_1307[4];
	integer fill_1308[4];
	real e_1309[4];
	integer fill_1310[4];
	real e_1311[4];
	integer fill_1312[4];
	real e_1313[4];
	integer fill_1314[4];
	real e_1315[4];
	integer fill_1316[4];
	real e_1317[4];
	integer fill_1318[4];
	real e_1319[4];
	integer fill_1320[4];
	real e_1321[4];
	integer fill_1322[4];
	real e_1323[4];
	integer fill_1324[4];
	real e_1325[4];
	integer fill_1326[4];
	real e_1327[4];
	integer fill_1328[4];
	real e_1329[4];
	integer fill_1330[4];
	real e_1331[4];
	integer fill_1332[4];
	real e_1333[4];
	integer fill_1334[4];
	real e_1335[4];
	integer fill_1336[4];
	real e_1337[4];
	integer fill_1338[4];
	real e_1339[4];
	integer fill_1340[4];
	real e_1341[4];
	integer fill_1342[4];
	real e_1343[4];
	integer fill_1344[4];
	real e_1345[4];
	integer fill_1346[4];
	real e_1347[4];
	integer fill_1348[4];
	real e_1349[4];
	integer fill_1350[4];
	real e_1351[4];
	integer fill_1352[4];
	real e_1353[4];
	integer fill_1354[4];
	real e_1355[4];
	integer fill_1356[4];
	real e_1357[4];
	integer fill_1358[4];
	real e_1359[4];
	integer fill_1360[4];
	real e_1361[4];
	integer fill_1362[4];
	real e_1363[4];
	integer fill_1364[4];
	real e_1365[4];
	integer fill_1366[4];
	real e_1367[4];
	integer fill_1368[4];
	real e_1369[4];
	integer fill_1370[4];
	real e_1371[4];
	integer fill_1372[4];
	real e_1373[4];
	integer fill_1374[4];
	real e_1375[4];
	integer fill_1376[4];
	real e_1377[4];
	integer fill_1378[4];
	real e_1379[4];
	integer fill_1380[4];
	real e_1381[4];
	integer fill_1382[4];
	real e_1383[4];
	integer fill_1384[4];
	real e_1385[4];
	integer fill_1386[4];
	real e_1387[4];
	integer fill_1388[4];
	real e_1389[4];
	integer fill_1390[4];
	real e_1391[4];
	integer fill_1392[4];
	real e_1393[4];
	integer fill_1394[4];
	real e_1395[4];
	integer fill_1396[4];
	real e_1397[4];
	integer fill_1398[4];
	real e_1399[4];
	integer fill_1400[4];
	real e_1401[4];
	integer fill_1402[4];
	real e_1403[4];
	integer fill_1404[4];
	real e_1405[4];
	integer fill_1406[4];
	real e_1407[4];
	integer fill_1408[4];
	real e_1409[4];
	integer fill_1410[4];
	real e_1411[4];
	integer fill_1412[4];
	real e_1413[4];
	integer fill_1414[4];
	real e_1415[4];
	integer fill_1416[4];
	real e_1417[4];
	integer fill_1418[4];
	real e_1419[4];
	integer fill_1420[4];
	real e_1421[4];
	integer fill_1422[4];
	real e_1423[4];
	integer fill_1424[4];
	real e_1425[4];
	integer fill_1426[4];
	real e_1427[4];
	integer fill_1428[4];
	real e_1429[4];
	integer fill_1430[4];
	real e_1431[4];
	integer fill_1432[4];
	real e_1433[4];
	integer fill_1434[4];
	real e_1435[4];
	integer fill_1436[4];
	real e_1437[4];
	integer fill_1438[4];
	real e_1439[4];
	integer fill_1440[4];
	real e_1441[4];
	integer fill_1442[4];
	real e_1443[4];
	integer fill_1444[4];
	real e_1445[4];
	integer fill_1446[4];
	real e_1447[4];
	integer fill_1448[4];
	real e_1449[4];
	integer fill_1450[4];
	real e_1451[4];
	integer fill_1452[4];
	real e_1453[4];
	integer fill_1454[4];
	real e_1455[4];
	integer fill_1456[4];
	real e_1457[4];
	integer fill_1458[4];
	real e_1459[4];
	integer fill_1460[4];
	real e_1461[4];
	integer fill_1462[4];
	real e_1463[4];
	integer fill_1464[4];
	real e_1465[4];
	integer fill_1466[4];
	real e_1467[4];
	integer fill_1468[4];
	real e_1469[4];
	integer fill_1470[4];
	real e_1471[4];
	integer fill_1472[4];
	real e_1473[4];
	integer fill_1474[4];
	real e_1475[4];
	integer fill_1476[4];
	real e_1477[4];
	integer fill_1478[4];
	real e_1479[4];
	integer fill_1480[4];
	real e_1481[4];
	integer fill_1482[4];
	real e_1483[4];
	integer fill_1484[4];
	real e_1485[4];
	integer fill_1486[4];
	real e_1487[4];
	integer fill_1488[4];
	real e_1489[4];
	integer fill_1490[4];
	real e_1491[4];
	integer fill_1492[4];
	real e_1493[4];
	integer fill_1494[4];
	real e_1495[4];
	integer fill_1496[4];
	real e_1497[4];
	integer fill_1498[4];
	real e_1499[4];
	integer fill_1500[4];
	real e_1501[4];
	integer fill_1502[4];
	real e_1503[4];
	integer fill_1504[4];
	real e_1505[4];
	integer fill_1506[4];
	real e_1507[4];
	integer fill_1508[4];
	real e_1509[4];
	integer fill_1510[4];
	real e_1511[4];
	integer fill_1512[4];
	real e_1513[4];
	integer fill_1514[4];
	real e_1515[4];
	integer fill_1516[4];
	real e_1517[4];
	integer fill_1518[4];
	real e_1519[4];
	integer fill_1520[4];
	real e_1521[4];
	integer fill_1522[4];
	real e_1523[4];
	integer fill_1524[4];
	real e_1525[4];
	integer fill_1526[4];
	real e_1527[4];
	integer fill_1528[4];
	real e_1529[4];
	integer fill_1530[4];
	real e_1531[4];
	integer fill_1532[4];
	real e_1533[4];
	integer fill_1534[4];
	real e_1535[4];
	integer fill_1536[4];
	real e_1537[4];
	integer fill_1538[4];
	real e_1539[4];
	integer fill_1540[4];
	real e_1541[4];
	integer fill_1542[4];
	real e_1543[4];
	integer fill_1544[4];
	real e_1545[4];
	integer fill_1546[4];
	real e_1547[4];
	integer fill_1548[4];
	real e_1549[4];
	integer fill_1550[4];
	real e_1551[4];
	integer fill_1552[4];
	real e_1553[4];
	integer fill_1554[4];
	real e_1555[4];
	integer fill_1556[4];
	real e_1557[4];
	integer fill_1558[4];
	real e_1559[4];
	integer fill_1560[4];
	real e_1561[4];
	integer fill_1562[4];
	real e_1563[4];
	integer fill_1564[4];
	real e_1565[4];
	integer fill_1566[4];
	real e_1567[4];
	integer fill_1568[4];
	real e_1569[4];
	integer fill_1570[4];
	real e_1571[4];
	integer fill_1572[4];
	real e_1573[4];
	integer fill_1574[4];
	real e_1575[4];
	integer fill_1576[4];
	real e_1577[4];
	integer fill_1578[4];
	real e_1579[4];
	integer fill_1580[4];
	real e_1581[4];
	integer fill_1582[4];
	real e_1583[4];
	integer fill_1584[4];
	real e_1585[4];
	integer fill_1586[4];
	real e_1587[4];
	integer fill_1588[4];
	real e_1589[4];
	integer fill_1590[4];
	real e_1591[4];
	integer fill_1592[4];
	real e_1593[4];
	integer fill_1594[4];
	real e_1595[4];
	integer fill_1596[4];
	real e_1597[4];
	integer fill_1598[4];
	real e_1599[4];
	integer fill_1600[4];
	real e_1601[4];
	integer fill_1602[4];
	real e_1603[4];
	integer fill_1604[4];
	real e_1605[4];
	integer fill_1606[4];
	real e_1607[4];
	integer fill_1608[4];
	real e_1609[4];
	integer fill_1610[4];
	real e_1611[4];
	integer fill_1612[4];
	real e_1613[4];
	integer fill_1614[4];
	real e_1615[4];
	integer fill_1616[4];
	real e_1617[4];
	integer fill_1618[4];
	real e_1619[4];
	integer fill_1620[4];
	real e_1621[4];
	integer fill_1622[4];
	real e_1623[4];
	integer fill_1624[4];
	real e_1625[4];
	integer fill_1626[4];
	real e_1627[4];
	integer fill_1628[4];
	real e_1629[4];
	integer fill_1630[4];
	real e_1631[4];
	integer fill_1632[4];
	real e_1633[4];
	integer fill_1634[4];
	real e_1635[4];
	integer fill_1636[4];
	real e_1637[4];
	integer fill_1638[4];
	real e_1639[4];
	integer fill_1640[4];
	real e_1641[4];
	integer fill_1642[4];
	real e_1643[4];
	integer fill_1644[4];
	real e_1645[4];
	integer fill_1646[4];
	real e_1647[4];
	integer fill_1648[4];
	real e_1649[4];
	integer fill_1650[4];
	real e_1651[4];
	integer fill_1652[4];
	real e_1653[4];
	integer fill_1654[4];
	real e_1655[4];
	integer fill_1656[4];
	real e_1657[4];
	integer fill_1658[4];
	real e_1659[4];
	integer fill_1660[4];
	real e_1661[4];
	integer fill_1662[4];
	real e_1663[4];
	integer fill_1664[4];
	real e_1665[4];
	integer fill_1666[4];
	real e_1667[4];
	integer fill_1668[4];
	real e_1669[4];
	integer fill_1670[4];
	real e_1671[4];
	integer fill_1672[4];
	real e_1673[4];
	integer fill_1674[4];
	real e_1675[4];
	integer fill_1676[4];
	real e_1677[4];
	integer fill_1678[4];
	real e_1679[4];
	integer fill_1680[4];
	real e_1681[4];
	integer fill_1682[4];
	real e_1683[4];
	integer fill_1684[4];
	real e_1685[4];
	integer fill_1686[4];
	real e_1687[4];
	integer fill_1688[4];
	real e_1689[4];
	integer fill_1690[4];
	real e_1691[4];
	integer fill_1692[4];
	real e_1693[4];
	integer fill_1694[4];
	real e_1695[4];
	integer fill_1696[4];
	real e_1697[4];
	integer fill_1698[4];
	real e_1699[4];
	integer fill_1700[4];
	real e_1701[4];
	integer fill_1702[4];
	real e_1703[4];
	integer fill_1704[4];
	real e_1705[4];
	integer fill_1706[4];
	real e_1707[4];
	integer fill_1708[4];
	real e_1709[4];
	integer fill_1710[4];
	real e_1711[4];
	integer fill_1712[4];
	real e_1713[4];
	integer fill_1714[4];
	real e_1715[4];
	integer fill_1716[4];
	real e_1717[4];
	integer fill_1718[4];
	real e_1719[4];
	integer fill_1720[4];
	real e_1721[4];
	integer fill_1722[4];
	real e_1723[4];
	integer fill_1724[4];
	real e_1725[4];
	integer fill_1726[4];
	real e_1727[4];
	integer fill_1728[4];
	real e_1729[4];
	integer fill_1730[4];
	real e_1731[4];
	integer fill_1732[4];
	real e_1733[4];
	integer fill_1734[4];
	real e_1735[4];
	integer fill_1736[4];
	real e_1737[4];
	integer fill_1738[4];
	real e_1739[4];
	integer fill_1740[4];
	real e_1741[4];
	integer fill_1742[4];
	real e_1743[4];
	integer fill_1744[4];
	real e_1745[4];
	integer fill_1746[4];
	real e_1747[4];
	integer fill_1748[4];
	real e_1749[4];
	integer fill_1750[4];
	real e_1751[4];
	integer fill_1752[4];
	real e_1753[4];
	integer fill_1754[4];
	real e_1755[4];
	integer fill_1756[4];
	real e_1757[4];
	integer fill_1758[4];
	real e_1759[4];
	integer fill_1760[4];
	real e_1761[4];
	integer fill_1762[4];
	real e_1763[4];
	integer fill_1764[4];
	real e_1765[4];
	integer fill_1766[4];
	real e_1767[4];
	integer fill_1768[4];
	real e_1769[4];
	integer fill_1770[4];
	real e_1771[4];
	integer fill_1772[4];
	real e_1773[4];
	integer fill_1774[4];
	real e_1775[4];
	integer fill_1776[4];
	real e_1777[4];
	integer fill_1778[4];
	real e_1779[4];
	integer fill_1780[4];
	real e_1781[4];
	integer fill_1782[4];
	real e_1783[4];
	integer fill_1784[4];
	real e_1785[4];
	integer fill_1786[4];
	real e_1787[4];
	integer fill_1788[4];
	real e_1789[4];
	integer fill_1790[4];
	real e_1791[4];
	integer fill_1792[4];
	real e_1793[4];
	integer fill_1794[4];
	real e_1795[4];
	integer fill_1796[4];
	real e_1797[4];
	integer fill_1798[4];
	real e_1799[4];
	integer fill_1800[4];
	real e_1801[4];
	integer fill_1802[4];
	real e_1803[4];
	integer fill_1804[4];
	real e_1805[4];
	integer fill_1806[4];
	real e_1807[4];
	integer fill_1808[4];
	real e_1809[4];
	integer fill_1810[4];
	real e_1811[4];
	integer fill_1812[4];
	real e_1813[4];
	integer fill_1814[4];
	real e_1815[4];
	integer fill_1816[4];
	real e_1817[4];
	integer fill_1818[4];
	real e_1819[4];
	integer fill_1820[4];
	real e_1821[4];
	integer fill_1822[4];
	real e_1823[4];
	integer fill_1824[4];
	real e_1825[4];
	integer fill_1826[4];
	real e_1827[4];
	integer fill_1828[4];
	real e_1829[4];
	integer fill_1830[4];
	real e_1831[4];
	integer fill_1832[4];
	real e_1833[4];
	integer fill_1834[4];
	real e_1835[4];
	integer fill_1836[4];
	real e_1837[4];
	integer fill_1838[4];
	real e_1839[4];
	integer fill_1840[4];
	real e_1841[4];
	integer fill_1842[4];
	real e_1843[4];
	integer fill_1844[4];
	real e_1845[4];
	integer fill_1846[4];
	real e_1847[4];
	integer fill_1848[4];
	real e_1849[4];
	integer fill_1850[4];
	real e_1851[4];
	integer fill_1852[4];
	real e_1853[4];
	integer fill_1854[4];
	real e_1855[4];
	integer fill_1856[4];
	real e_1857[4];
	integer fill_1858[4];
	real e_1859[4];
	integer fill_1860[4];
	real e_1861[4];
	integer fill_1862[4];
	real e_1863[4];
	integer fill_1864[4];
	real e_1865[4];
	integer fill_1866[4];
	real e_1867[4];
	integer fill_1868[4];
	real e_1869[4];
	integer fill_1870[4];
	real e_1871[4];
	integer fill_1872[4];
	real e_1873[4];
	integer fill_1874[4];
	real e_1875[4];
	integer fill_1876[4];
	real e_1877[4];
	integer fill_1878[4];
	real e_1879[4];
	integer fill_1880[4];
	real e_1881[4];
	integer fill_1882[4];
	real e_1883[4];
	integer fill_1884[4];
	real e_1885[4];
	integer fill_1886[4];
	real e_1887[4];
	integer fill_1888[4];
	real e_1889[4];
	integer fill_1890[4];
	real e_1891[4];
	integer fill_1892[4];
	real e_1893[4];
	integer fill_1894[4];
	real e_1895[4];
	integer fill_1896[4];
	real e_1897[4];
	integer fill_1898[4];
	real e_1899[4];
	integer fill_1900[4];
	real e_1901[4];
	integer fill_1902[4];
	real e_1903[4];
	integer fill_1904[4];
	real e_1905[4];
	integer fill_1906[4];
	real e_1907[4];
	integer fill_1908[4];
	real e_1909[4];
	integer fill_1910[4];
	real e_1911[4];
	integer fill_1912[4];
	real e_1913[4];
	integer fill_1914[4];
	real e_1915[4];
	integer fill_1916[4];
	real e_1917[4];
	integer fill_1918[4];
	real e_1919[4];
	integer fill_1920[4];
	real e_1921[4];
	integer fill_1922[4];
	real e_1923[4];
	integer fill_1924[4];
	real e_1925[4];
	integer fill_1926[4];
	real e_1927[4];
	integer fill_1928[4];
	real e_1929[4];
	integer fill_1930[4];
	real e_1931[4];
	integer fill_1932[4];
	real e_1933[4];
	integer fill_1934[4];
	real e_1935[4];
	integer fill_1936[4];
	real e_1937[4];
	integer fill_1938[4];
	real e_1939[4];
	integer fill_1940[4];
	real e_1941[4];
	integer fill_1942[4];
	real e_1943[4];
	integer fill_1944[4];
	real e_1945[4];
	integer fill_1946[4];
	real e_1947[4];
	integer fill_1948[4];
	real e_1949[4];
	integer fill_1950[4];
	real e_1951[4];
	integer fill_1952[4];
	real e_1953[4];
	integer fill_1954[4];
	real e_1955[4];
	integer fill_1956[4];
	real e_1957[4];
	integer fill_1958[4];
	real e_1959[4];
	integer fill_1960[4];
	real e_1961[4];
	integer fill_1962[4];
	real e_1963[4];
	integer fill_1964[4];
	real e_1965[4];
	integer fill_1966[4];
	real e_1967[4];
	integer fill_1968[4];
	real e_1969[4];
	integer fill_1970[4];
	real e_1971[4];
	integer fill_1972[4];
	real e_1973[4];
	integer fill_1974[4];
	real e_1975[4];
	integer fill_1976[4];
	real e_1977[4];
	integer fill_1978[4];
	real e_1979[4];
	integer fill_1980[4];
	real e_1981[4];
	integer fill_1982[4];
	real e_1983[4];
	integer fill_1984[4];
	real e_1985[4];
	integer fill_1986[4];
	real e_1987[4];
	integer fill_1988[4];
	real e_1989[4];
	integer fill_1990[4];
	real e_1991[4];
	integer fill_1992[4];
	real e_1993[4];
	integer fill_1994[4];
	real e_1995[4];
	integer fill_1996[4];
	real e_1997[4];
	integer fill_1998[4];
	real e_1999[4];
	integer fill_2000[4];
	real e_2001[4];
	integer fill_2002[4];
	real e_2003[4];
	integer fill_2004[4];
	real e_2005[4];
	integer fill_2006[4];
	real e_2007[4];
	integer fill_2008[4];
	real e_2009[4];
	integer fill_2010[4];
	real e_2011[4];
	integer fill_2012[4];
	real e_2013[4];
	integer fill_2014[4];
	real e_2015[4];
	integer fill_2016[4];
	real e_2017[4];
	integer fill_2018[4];
	real e_2019[4];
	integer fill_2020[4];
	real e_2021[4];
	integer fill_2022[4];
	real e_2023[4];
	integer fill_2024[4];
	real e_2025[4];
	integer fill_2026[4];
	real e_2027[4];
	integer fill_2028[4];
	real e_2029[4];
	integer fill_2030[4];
	real e_2031[4];
	integer fill_2032[4];
	real e_2033[4];
	integer fill_2034[4];
	real e_2035[4];
	integer fill_2036[4];
	real e_2037[4];
	integer fill_2038[4];
	real e_2039[4];
	integer fill_2040[4];
	real e_2041[4];
	integer fill_2042[4];
	real e_2043[4];
	integer fill_2044[4];
	real e_2045[4];
	integer fill_2046[4];
	real e_2047[4];
	integer fill_2048[4];
	real e_2049[4];
	integer fill_2050[4];
	real e_2051[4];
	integer fill_2052[4];
	real e_2053[4];
	integer fill_2054[4];
	real e_2055[4];
	integer fill_2056[4];
	real e_2057[4];
	integer fill_2058[4];
	real e_2059[4];
	integer fill_2060[4];
	real e_2061[4];
	integer fill_2062[4];
	real e_2063[4];
	integer fill_2064[4];
	real e_2065[4];
	integer fill_2066[4];
	real e_2067[4];
	integer fill_2068[4];
	real e_2069[4];
	integer fill_2070[4];
	real e_2071[4];
	integer fill_2072[4];
	real e_2073[4];
	integer fill_2074[4];
	real e_2075[4];
	integer fill_2076[4];
	real e_2077[4];
	integer fill_2078[4];
	real e_2079[4];
	integer fill_2080[4];
	real e_2081[4];
	integer fill_2082[4];
	real e_2083[4];
	integer fill_2084[4];
	real e_2085[4];
	integer fill_2086[4];
	real e_2087[4];
	integer fill_2088[4];
	real e_2089[4];
	integer fill_2090[4];
	real e_2091[4];
	integer fill_2092[4];
	real e_2093[4];
	integer fill_2094[4];
	real e_2095[4];
	integer fill_2096[4];
	real e_2097[4];
	integer fill_2098[4];
	real e_2099[4];
	integer fill_2100[4];
	real e_2101[4];
	integer fill_2102[4];
	real e_2103[4];
	integer fill_2104[4];
	real e_2105[4];
	integer fill_2106[4];
	real e_2107[4];
	integer fill_2108[4];
	real e_2109[4];
	integer fill_2110[4];
	real e_2111[4];
	integer fill_2112[4];
	real e_2113[4];
	integer fill_2114[4];
	real e_2115[4];
	integer fill_2116[4];
	real e_2117[4];
	integer fill_2118[4];
	real e_2119[4];
	integer fill_2120[4];
	real e_2121[4];
	integer fill_2122[4];
	real e_2123[4];
	integer fill_2124[4];
	real e_2125[4];
	integer fill_2126[4];
	real e_2127[4];
	integer fill_2128[4];
	real e_2129[4];
	integer fill_2130[4];
	real e_2131[4];
	integer fill_2132[4];
	real e_2133[4];
	integer fill_2134[4];
	real e_2135[4];
	integer fill_2136[4];
	real e_2137[4];
	integer fill_2138[4];
	real e_2139[4];
	integer fill_2140[4];
	real e_2141[4];
	integer fill_2142[4];
	real e_2143[4];
	integer fill_2144[4];
	real e_2145[4];
	integer fill_2146[4];
	real e_2147[4];
	integer fill_2148[4];
	real e_2149[4];
	integer fill_2150[4];
	real e_2151[4];
	integer fill_2152[4];
	real e_2153[4];
	integer fill_2154[4];
	real e_2155[4];
	integer fill_2156[4];
	real e_2157[4];
	integer fill_2158[4];
	real e_2159[4];
	integer fill_2160[4];
	real e_2161[4];
	integer fill_2162[4];
	real e_2163[4];
	integer fill_2164[4];
	real e_2165[4];
	integer fill_2166[4];
	real e_2167[4];
	integer fill_2168[4];
	real e_2169[4];
	integer fill_2170[4];
	real e_2171[4];
	integer fill_2172[4];
	real e_2173[4];
	integer fill_2174[4];
	real e_2175[4];
	integer fill_2176[4];
	real e_2177[4];
	integer fill_2178[4];
	real e_2179[4];
	integer fill_2180[4];
	real e_2181[4];
	integer fill_2182[4];
	real e_2183[4];
	integer fill_2184[4];
	real e_2185[4];
	integer fill_2186[4];
	real e_2187[4];
	integer fill_2188[4];
	real e_2189[4];
	integer fill_2190[4];
	real e_2191[4];
	integer fill_2192[4];
	real e_2193[4];
	integer fill_2194[4];
	real e_2195[4];
	integer fill_2196[4];
	real e_2197[4];
	integer fill_2198[4];
	real e_2199[4];
	integer fill_2200[4];
	real e_2201[4];
	integer fill_2202[4];
	real e_2203[4];
	integer fill_2204[4];
	real e_2205[4];
	integer fill_2206[4];
	real e_2207[4];
	integer fill_2208[4];
	real e_2209[4];
	integer fill_2210[4];
	real e_2211[4];
	integer fill_2212[4];
	real e_2213[4];
	integer fill_2214[4];
	real e_2215[4];
	integer fill_2216[4];
	real e_2217[4];
	integer fill_2218[4];
	real e_2219[4];
	integer fill_2220[4];
	real e_2221[4];
	integer fill_2222[4];
	real e_2223[4];
	integer fill_2224[4];
	real e_2225[4];
	integer fill_2226[4];
	real e_2227[4];
	integer fill_2228[4];
	real e_2229[4];
	integer fill_2230[4];
	real e_2231[4];
	integer fill_2232[4];
	real e_2233[4];
	integer fill_2234[4];
	real e_2235[4];
	integer fill_2236[4];
	real e_2237[4];
	integer fill_2238[4];
	real e_2239[4];
	integer fill_2240[4];
	real e_2241[4];
	integer fill_2242[4];
	real e_2243[4];
	integer fill_2244[4];
	real e_2245[4];
	integer fill_2246[4];
	real e_2247[4];
	integer fill_2248[4];
	real e_2249[4];
	integer fill_2250[4];
	real e_2251[4];
	integer fill_2252[4];
	real e_2253[4];
	integer fill_2254[4];
	real e_2255[4];
	integer fill_2256[4];
	real e_2257[4];
	integer fill_2258[4];
	real e_2259[4];
	integer fill_2260[4];
	real e_2261[4];
	integer fill_2262[4];
	real e_2263[4];
	integer fill_2264[4];
	real e_2265[4];
	integer fill_2266[4];
	real e_2267[4];
	integer fill_2268[4];
	real e_2269[4];
	integer fill_2270[4];
	real e_2271[4];
	integer fill_2272[4];
	real e_2273[4];
	integer fill_2274[4];
	real e_2275[4];
	integer fill_2276[4];
	real e_2277[4];
	integer fill_2278[4];
	real e_2279[4];
	integer fill_2280[4];
	real e_2281[4];
	integer fill_2282[4];
	real e_2283[4];
	integer fill_2284[4];
	real e_2285[4];
	integer fill_2286[4];
	real e_2287[4];
	integer fill_2288[4];
	real e_2289[4];
	integer fill_2290[4];
	real e_2291[4];
	integer fill_2292[4];
	real e_2293[4];
	integer fill_2294[4];
	real e_2295[4];
	integer fill_2296[4];
	real e_2297[4];
	integer fill_2298[4];
	real e_2299[4];
	integer fill_2300[4];
	real e_2301[4];
	integer fill_2302[4];
	real e_2303[4];
	integer fill_2304[4];
	real e_2305[4];
	integer fill_2306[4];
	real e_2307[4];
	integer fill_2308[4];
	real e_2309[4];
	integer fill_2310[4];
	real e_2311[4];
	integer fill_2312[4];
	real e_2313[4];
	integer fill_2314[4];
	real e_2315[4];
	integer fill_2316[4];
	real e_2317[4];
	integer fill_2318[4];
	real e_2319[4];
	integer fill_2320[4];
	real e_2321[4];
	integer fill_2322[4];
	real e_2323[4];
	integer fill_2324[4];
	real e_2325[4];
	integer fill_2326[4];
	real e_2327[4];
	integer fill_2328[4];
	real e_2329[4];
	integer fill_2330[4];
	real e_2331[4];
	integer fill_2332[4];
	real e_2333[4];
	integer fill_2334[4];
	real e_2335[4];
	integer fill_2336[4];
	real e_2337[4];
	integer fill_2338[4];
	real e_2339[4];
	integer fill_2340[4];
	real e_2341[4];
	integer fill_2342[4];
	real e_2343[4];
	integer fill_2344[4];
	real e_2345[4];
	integer fill_2346[4];
	real e_2347[4];
	integer fill_2348[4];
	real e_2349[4];
	integer fill_2350[4];
	real e_2351[4];
	integer fill_2352[4];
	real e_2353[4];
	integer fill_2354[4];
	real e_2355[4];
	integer fill_2356[4];
	real e_2357[4];
	integer fill_2358[4];
	real e_2359[4];
	integer fill_2360[4];
	real e_2361[4];
	integer fill_2362[4];
	real e_2363[4];
	integer fill_2364[4];
	real e_2365[4];
	integer fill_2366[4];
	real e_2367[4];
	integer fill_2368[4];
	real e_2369[4];
	integer fill_2370[4];
	real e_2371[4];
	integer fill_2372[4];
	real e_2373[4];
	integer fill_2374[4];
	real e_2375[4];
	integer fill_2376[4];
	real e_2377[4];
	integer fill_2378[4];
	real e_2379[4];
	integer fill_2380[4];
	real e_2381[4];
	integer fill_2382[4];
	real e_2383[4];
	integer fill_2384[4];
	real e_2385[4];
	integer fill_2386[4];
	real e_2387[4];
	integer fill_2388[4];
	real e_2389[4];
	integer fill_2390[4];
	real e_2391[4];
	integer fill_2392[4];
	real e_2393[4];
	integer fill_2394[4];
	real e_2395[4];
	integer fill_2396[4];
	real e_2397[4];
	integer fill_2398[4];
	real e_2399[4];
	integer fill_2400[4];
	real e_2401[4];
	integer fill_2402[4];
	real e_2403[4];
	integer fill_2404[4];
	real e_2405[4];
	integer fill_2406[4];
	real e_2407[4];
	integer fill_2408[4];
	real e_2409[4];
	integer fill_2410[4];
	real e_2411[4];
	integer fill_2412[4];
	real e_2413[4];
	integer fill_2414[4];
	real e_2415[4];
	integer fill_2416[4];
	real e_2417[4];
	integer fill_2418[4];
	real e_2419[4];
	integer fill_2420[4];
	real e_2421[4];
	integer fill_2422[4];
	real e_2423[4];
	integer fill_2424[4];
	real e_2425[4];
	integer fill_2426[4];
	real e_2427[4];
	integer fill_2428[4];
	real e_2429[4];
	integer fill_2430[4];
	real e_2431[4];
	integer fill_2432[4];
	real e_2433[4];
	integer fill_2434[4];
	real e_2435[4];
	integer fill_2436[4];
	real e_2437[4];
	integer fill_2438[4];
	real e_2439[4];
	integer fill_2440[4];
	real e_2441[4];
	integer fill_2442[4];
	real e_2443[4];
	integer fill_2444[4];
	real e_2445[4];
	integer fill_2446[4];
	real e_2447[4];
	integer fill_2448[4];
	real e_2449[4];
	integer fill_2450[4];
	real e_2451[4];
	integer fill_2452[4];
	real e_2453[4];
	integer fill_2454[4];
	real e_2455[4];
	integer fill_2456[4];
	real e_2457[4];
	integer fill_2458[4];
	real e_2459[4];
	integer fill_2460[4];
	real e_2461[4];
	integer fill_2462[4];
	real e_2463[4];
	integer fill_2464[4];
	real e_2465[4];
	integer fill_2466[4];
	real e_2467[4];
	integer fill_2468[4];
	real e_2469[4];
	integer fill_2470[4];
	real e_2471[4];
	integer fill_2472[4];
	real e_2473[4];
	integer fill_2474[4];
	real e_2475[4];
	integer fill_2476[4];
	real e_2477[4];
	integer fill_2478[4];
	real e_2479[4];
	integer fill_2480[4];
	real e_2481[4];
	integer fill_2482[4];
	real e_2483[4];
	integer fill_2484[4];
	real e_2485[4];
	integer fill_2486[4];
	real e_2487[4];
	integer fill_2488[4];
	real e_2489[4];
	integer fill_2490[4];
	real e_2491[4];
	integer fill_2492[4];
	real e_2493[4];
	integer fill_2494[4];
	real e_2495[4];
	integer fill_2496[4];
	real e_2497[4];
	integer fill_2498[4];
	real e_2499[4];
	integer fill_2500[4];
	real e_2501[4];
	integer fill_2502[4];
	real e_2503[4];
	integer fill_2504[4];
	real e_2505[4];
	integer fill_2506[4];
	real e_2507[4];
	integer fill_2508[4];
	real e_2509[4];
	integer fill_2510[4];
	real e_2511[4];
	integer fill_2512[4];
	real e_2513[4];
	integer fill_2514[4];
	real e_2515[4];
	integer fill_2516[4];
	real e_2517[4];
	integer fill_2518[4];
	real e_2519[4];
	integer fill_2520[4];
	real e_2521[4];
	integer fill_2522[4];
	real e_2523[4];
	integer fill_2524[4];
	real e_2525[4];
	integer fill_2526[4];
	real e_2527[4];
	integer fill_2528[4];
	real e_2529[4];
	integer fill_2530[4];
	real e_2531[4];
	integer fill_2532[4];
	real e_2533[4];
	integer fill_2534[4];
	real e_2535[4];
	integer fill_2536[4];
	real e_2537[4];
	integer fill_2538[4];
	real e_2539[4];
	integer fill_2540[4];
	real e_2541[4];
	integer fill_2542[4];
	real e_2543[4];
	integer fill_2544[4];
	real e_2545[4];
	integer fill_2546[4];
	real e_2547[4];
	integer fill_2548[4];
	real e_2549[4];
	integer fill_2550[4];
	real e_2551[4];
	integer fill_2552[4];
	real e_2553[4];
	integer fill_2554[4];
	real e_2555[4];
	integer fill_2556[4];
	real e_2557[4];
	integer fill_2558[4];
	real e_2559[4];
	integer fill_2560[4];
	real e_2561[4];
	integer fill_2562[4];
	real e_2563[4];
	integer fill_2564[4];
	real e_2565[4];
	integer fill_2566[4];
	real e_2567[4];
	integer fill_2568[4];
	real e_2569[4];
	integer fill_2570[4];
	real e_2571[4];
	integer fill_2572[4];
	real e_2573[4];
	integer fill_2574[4];
	real e_2575[4];
	integer fill_2576[4];
	real e_2577[4];
	integer fill_2578[4];
	real e_2579[4];
	integer fill_2580[4];
	real e_2581[4];
	integer fill_2582[4];
	real e_2583[4];
	integer fill_2584[4];
	real e_2585[4];
	integer fill_2586[4];
	real e_2587[4];
	integer fill_2588[4];
	real e_2589[4];
	integer fill_2590[4];
	real e_2591[4];
	integer fill_2592[4];
	real e_2593[4];
	integer fill_2594[4];
	real e_2595[4];
	integer fill_2596[4];
	real e_2597[4];
	integer fill_2598[4];
	real e_2599[4];
	integer fill_2600[4];
	real e_2601[4];
	integer fill_2602[4];
	real e_2603[4];
	integer fill_2604[4];
	real e_2605[4];
	integer fill_2606[4];
	real e_2607[4];
	integer fill_2608[4];
	real e_2609[4];
	integer fill_2610[4];
	real e_2611[4];
	integer fill_2612[4];
	real e_2613[4];
	integer fill_2614[4];
	real e_2615[4];
	integer fill_2616[4];
	real e_2617[4];
	integer fill_2618[4];
	real e_2619[4];
	integer fill_2620[4];
	real e_2621[4];
	integer fill_2622[4];
	real e_2623[4];
	integer fill_2624[4];
	real e_2625[4];
	integer fill_2626[4];
	real e_2627[4];
	integer fill_2628[4];
	real e_2629[4];
	integer fill_2630[4];
	real e_2631[4];
	integer fill_2632[4];
	real e_2633[4];
	integer fill_2634[4];
	real e_2635[4];
	integer fill_2636[4];
	real e_2637[4];
	integer fill_2638[4];
	real e_2639[4];
	integer fill_2640[4];
	real e_2641[4];
	integer fill_2642[4];
	real e_2643[4];
	integer fill_2644[4];
	real e_2645[4];
	integer fill_2646[4];
	real e_2647[4];
	integer fill_2648[4];
	real e_2649[4];
	integer fill_2650[4];
	real e_2651[4];
	integer fill_2652[4];
	real e_2653[4];
	integer fill_2654[4];
	real e_2655[4];
	integer fill_2656[4];
	real e_2657[4];
	integer fill_2658[4];
	real e_2659[4];
	integer fill_2660[4];
	real e_2661[4];
	integer fill_2662[4];
	real e_2663[4];
	integer fill_2664[4];
	real e_2665[4];
	integer fill_2666[4];
	real e_2667[4];
	integer fill_2668[4];
	real e_2669[4];
	integer fill_2670[4];
	real e_2671[4];
	integer fill_2672[4];
	real e_2673[4];
	integer fill_2674[4];
	real e_2675[4];
	integer fill_2676[4];
	real e_2677[4];
	integer fill_2678[4];
	real e_2679[4];
	integer fill_2680[4];
	real e_2681[4];
	integer fill_2682[4];
	real e_2683[4];
	integer fill_2684[4];
	real e_2685[4];
	integer fill_2686[4];
	real e_2687[4];
	integer fill_2688[4];
	real e_2689[4];
	integer fill_2690[4];
	real e_2691[4];
	integer fill_2692[4];
	real e_2693[4];
	integer fill_2694[4];
	real e_2695[4];
	integer fill_2696[4];
	real e_2697[4];
	integer fill_2698[4];
	real e_2699[4];
	integer fill_2700[4];
	real e_2701[4];
	integer fill_2702[4];
	real e_2703[4];
	integer fill_2704[4];
	real e_2705[4];
	integer fill_2706[4];
	real e_2707[4];
	integer fill_2708[4];
	real e_2709[4];
	integer fill_2710[4];
	real e_2711[4];
	integer fill_2712[4];
	real e_2713[4];
	integer fill_2714[4];
	real e_2715[4];
	integer fill_2716[4];
	real e_2717[4];
	integer fill_2718[4];
	real e_2719[4];
	integer fill_2720[4];
	real e_2721[4];
	integer fill_2722[4];
	real e_2723[4];
	integer fill_2724[4];
	real e_2725[4];
	integer fill_2726[4];
	real e_2727[4];
	integer fill_2728[4];
	real e_2729[4];
	integer fill_2730[4];
	real e_2731[4];
	integer fill_2732[4];
	real e_2733[4];
	integer fill_2734[4];
	real e_2735[4];
	integer fill_2736[4];
	real e_2737[4];
	integer fill_2738[4];
	real e_2739[4];
	integer fill_2740[4];
	real e_2741[4];
	integer fill_2742[4];
	real e_2743[4];
	integer fill_2744[4];
	real e_2745[4];
	integer fill_2746[4];
	real e_2747[4];
	integer fill_2748[4];
	real e_2749[4];
	integer fill_2750[4];
	real e_2751[4];
	integer fill_2752[4];
	real e_2753[4];
	integer fill_2754[4];
	real e_2755[4];
	integer fill_2756[4];
	real e_2757[4];
	integer fill_2758[4];
	real e_2759[4];
	integer fill_2760[4];
	real e_2761[4];
	integer fill_2762[4];
	real e_2763[4];
	integer fill_2764[4];
	real e_2765[4];
	integer fill_2766[4];
	real e_2767[4];
	integer fill_2768[4];
	real e_2769[4];
	integer fill_2770[4];
	real e_2771[4];
	integer fill_2772[4];
	real e_2773[4];
	integer fill_2774[4];
	real e_2775[4];
	integer fill_2776[4];
	real e_2777[4];
	integer fill_2778[4];
	real e_2779[4];
	integer fill_2780[4];
	real e_2781[4];
	integer fill_2782[4];
	real e_2783[4];
	integer fill_2784[4];
	real e_2785[4];
	integer fill_2786[4];
	real e_2787[4];
	integer fill_2788[4];
	real e_2789[4];
	integer fill_2790[4];
	real e_2791[4];
	integer fill_2792[4];
	real e_2793[4];
	integer fill_2794[4];
	real e_2795[4];
	integer fill_2796[4];
	real e_2797[4];
	integer fill_2798[4];
	real e_2799[4];
	integer fill_2800[4];
	real e_2801[4];
	integer fill_2802[4];
	real e_2803[4];
	integer fill_2804[4];
	real e_2805[4];
	integer fill_2806[4];
	real e_2807[4];
	integer fill_2808[4];
	real e_2809[4];
	integer fill_2810[4];
	real e_2811[4];
	integer fill_2812[4];
	real e_2813[4];
	integer fill_2814[4];
	real e_2815[4];
	integer fill_2816[4];
	real e_2817[4];
	integer fill_2818[4];
	real e_2819[4];
	integer fill_2820[4];
	real e_2821[4];
	integer fill_2822[4];
	real e_2823[4];
	integer fill_2824[4];
	real e_2825[4];
	integer fill_2826[4];
	real e_2827[4];
	integer fill_2828[4];
	real e_2829[4];
	integer fill_2830[4];
	real e_2831[4];
	integer fill_2832[4];
	real e_2833[4];
	integer fill_2834[4];
	real e_2835[4];
	integer fill_2836[4];
	real e_2837[4];
	integer fill_2838[4];
	real e_2839[4];
	integer fill_2840[4];
	real e_2841[4];
	integer fill_2842[4];
	real e_2843[4];
	integer fill_2844[4];
	real e_2845[4];
	integer fill_2846[4];
	real e_2847[4];
	integer fill_2848[4];
	real e_2849[4];
	integer fill_2850[4];
	real e_2851[4];
	integer fill_2852[4];
	real e_2853[4];
	integer fill_2854[4];
	real e_2855[4];
	integer fill_2856[4];
	real e_2857[4];
	integer fill_2858[4];
	real e_2859[4];
	integer fill_2860[4];
	real e_2861[4];
	integer fill_2862[4];
	real e_2863[4];
	integer fill_2864[4];
	real e_2865[4];
	integer fill_2866[4];
	real e_2867[4];
	integer fill_2868[4];
	real e_2869[4];
	integer fill_2870[4];
	real e_2871[4];
	integer fill_2872[4];
	real e_2873[4];
	integer fill_2874[4];
	real e_2875[4];
	integer fill_2876[4];
	real e_2877[4];
	integer fill_2878[4];
	real e_2879[4];
	integer fill_2880[4];
	real e_2881[4];
	integer fill_2882[4];
	real e_2883[4];
	integer fill_2884[4];
	real e_2885[4];
	integer fill_2886[4];
	real e_2887[4];
	integer fill_2888[4];
	real e_2889[4];
	integer fill_2890[4];
	real e_2891[4];
	integer fill_2892[4];
	real e_2893[4];
	integer fill_2894[4];
	real e_2895[4];
	integer fill_2896[4];
	real e_2897[4];
	integer fill_2898[4];
	real e_2899[4];
	integer fill_2900[4];
	real e_2901[4];
	integer fill_2902[4];
	real e_2903[4];
	integer fill_2904[4];
	real e_2905[4];
	integer fill_2906[4];
	real e_2907[4];
	integer fill_2908[4];
	real e_2909[4];
	integer fill_2910[4];
	real e_2911[4];
	integer fill_2912[4];
	real e_2913[4];
	integer fill_2914[4];
	real e_2915[4];
	integer fill_2916[4];
	real e_2917[4];
	integer fill_2918[4];
	real e_2919[4];
	integer fill_2920[4];
	real e_2921[4];
	integer fill_2922[4];
	real e_2923[4];
	integer fill_2924[4];
	real e_2925[4];
	integer fill_2926[4];
	real e_2927[4];
	integer fill_2928[4];
	real e_2929[4];
	integer fill_2930[4];
	real e_2931[4];
	integer fill_2932[4];
	real e_2933[4];
	integer fill_2934[4];
	real e_2935[4];
	integer fill_2936[4];
	real e_2937[4];
	integer fill_2938[4];
	real e_2939[4];
	integer fill_2940[4];
	real e_2941[4];
	integer fill_2942[4];
	real e_2943[4];
	integer fill_2944[4];
	real e_2945[4];
	integer fill_2946[4];
	real e_2947[4];
	integer fill_2948[4];
	real e_2949[4];
	integer fill_2950[4];
	real e_2951[4];
	integer fill_2952[4];
	real e_2953[4];
	integer fill_2954[4];
	real e_2955[4];
	integer fill_2956[4];
	real e_2957[4];
	integer fill_2958[4];
	real e_2959[4];
	integer fill_2960[4];
	real e_2961[4];
	integer fill_2962[4];
	real e_2963[4];
	integer fill_2964[4];
	real e_2965[4];
	integer fill_2966[4];
	real e_2967[4];
	integer fill_2968[4];
	real e_2969[4];
	integer fill_2970[4];
	real e_2971[4];
	integer fill_2972[4];
	real e_2973[4];
	integer fill_2974[4];
	real e_2975[4];
	integer fill_2976[4];
	real e_2977[4];
	integer fill_2978[4];
	real e_2979[4];
	integer fill_2980[4];
	real e_2981[4];
	integer fill_2982[4];
	real e_2983[4];
	integer fill_2984[4];
	real e_2985[4];
	integer fill_2986[4];
	real e_2987[4];
	integer fill_2988[4];
	real e_2989[4];
	integer fill_2990[4];
	real e_2991[4];
	integer fill_2992[4];
	real e_2993[4];
	integer fill_2994[4];
	real e_2995[4];
	integer fill_2996[4];
	real e_2997[4];
	integer fill_2998[4];
	real e_2999[4];
	integer fill_3000[4];
	real e_3001[4];
	integer fill_3002[4];
	} equiv_5 = { (float)0., (float)0., (float)0., (float)0., {0}, (float)
		0., (float)0., (float)0., (float)0., {0}, (float)0., (float)
		0., (float)0., (float)0., {0}, (float)0., (float)0., (float)
		0., (float)0., {0}, (float)0., (float)0., (float)0., (float)
		0., {0}, (float)0., (float)0., (float)0., (float)0., {0}, (
		float)0., (float)0., (float)0., (float)0., {0}, (float)0., (
		float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float).00114, (float)0., (float)0., (float)0., {0}, (float)
		.0012, (float)0., (float)0., (float)0., {0}, (float).00109, (
		float)0., (float)0., (float)0., {0}, (float).00373, (float)0.,
		 (float)0., (float)0., {0}, (float).00495, (float)0., (float)
		0., (float)0., {0}, (float).01321, (float)0., (float)0., (
		float)0., {0}, (float).03203, (float)0., (float)0., (float)0.,
		 {0}, (float).0819, (float)0., (float)0., (float)0., {0}, (
		float).25129, (float)0., (float)0., (float)0., {0}, (float)
		.69749, (float)0., (float)0., (float)0., {0}, (float).97208, (
		float)0., (float)0., (float)0., {0}, (float)1., (float)0., (
		float)0., (float)0., {0}, (float).98655, (float)0., (float)0.,
		 (float)0., {0}, (float).92234, (float)0., (float)0., (float)
		0., {0}, (float).78359, (float)0., (float)0., (float)0., {0}, 
		(float).64632, (float)0., (float)0., (float)0., {0}, (float)
		.55174, (float)0., (float)0., (float)0., {0}, (float).50241, (
		float)0., (float)0., (float)0., {0}, (float).41232, (float)0.,
		 (float)0., (float)0., {0}, (float).23254, (float)0., (float)
		0., (float)0., {0}, (float).10744, (float)0., (float)0., (
		float)0., {0}, (float).05272, (float)0., (float)0., (float)0.,
		 {0}, (float).02769, (float)0., (float)0., (float)0., {0}, (
		float).01528, (float)0., (float)0., (float)0., {0}, (float)
		.0097, (float)0., (float)0., (float)0., {0}, (float).0054, (
		float)0., (float)0., (float)0., {0}, (float).00114, (float)0.,
		 (float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)7e-5, 
		(float)0., (float)0., {0}, (float)0., (float).00106, (float)
		0., (float)0., {0}, (float)0., (float).00259, (float)0., (
		float)0., {0}, (float)0., (float).00588, (float)0., (float)0.,
		 {0}, (float)0., (float).01313, (float)0., (float)0., {0}, (
		float)0., (float).03452, (float)0., (float)0., {0}, (float)0.,
		 (float).09925, (float)0., (float)0., {0}, (float)0., (float)
		.25868, (float)0., (float)0., {0}, (float)0., (float).53662, (
		float)0., (float)0., {0}, (float)0., (float).75311, (float)0.,
		 (float)0., {0}, (float)0., (float).90226, (float)0., (float)
		0., {0}, (float)0., (float).99149, (float)0., (float)0., {0}, 
		(float)0., (float)1., (float)0., (float)0., {0}, (float)0., (
		float).96028, (float)0., (float)0., {0}, (float)0., (float)
		.9011, (float)0., (float)0., {0}, (float)0., (float).76907, (
		float)0., (float)0., {0}, (float)0., (float).47949, (float)0.,
		 (float)0., {0}, (float)0., (float).21557, (float)0., (float)
		0., {0}, (float)0., (float).08929, (float)0., (float)0., {0}, 
		(float)0., (float).04225, (float)0., (float)0., {0}, (float)
		0., (float).02272, (float)0., (float)0., {0}, (float)0., (
		float).01292, (float)0., (float)0., {0}, (float)0., (float)
		.00778, (float)0., (float)0., {0}, (float)0., (float).00473, (
		float)0., (float)0., {0}, (float)0., (float).00264, (float)0.,
		 (float)0., {0}, (float)0., (float).00144, (float)0., (float)
		0., {0}, (float)0., (float)6e-4, (float)0., (float)0., {0}, (
		float)0., (float)3.1e-4, (float)0., (float)0., {0}, (float)0.,
		 (float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)5.9e-4, (float)0.,
		 {0}, (float)0., (float)0., (float).00104, (float)0., {0}, (
		float)0., (float)0., (float).0021, (float)0., {0}, (float)0., 
		(float)0., (float).00389, (float)0., {0}, (float)0., (float)
		0., (float).00839, (float)0., {0}, (float)0., (float)0., (
		float).01991, (float)0., {0}, (float)0., (float)0., (float)
		.05815, (float)0., {0}, (float)0., (float)0., (float).20311, (
		float)0., {0}, (float)0., (float)0., (float).59432, (float)0.,
		 {0}, (float)0., (float)0., (float).953, (float)0., {0}, (
		float)0., (float)0., (float).92949, (float)0., {0}, (float)0.,
		 (float)0., (float).87362, (float)0., {0}, (float)0., (float)
		0., (float).91049, (float)0., {0}, (float)0., (float)0., (
		float)1., (float)0., {0}, (float)0., (float)0., (float).99161,
		 (float)0., {0}, (float)0., (float)0., (float).79466, (float)
		0., {0}, (float)0., (float)0., (float).46286, (float)0., {0}, 
		(float)0., (float)0., (float).20633, (float)0., {0}, (float)
		0., (float)0., (float).08993, (float)0., {0}, (float)0., (
		float)0., (float).04324, (float)0., {0}, (float)0., (float)0.,
		 (float).02196, (float)0., {0}, (float)0., (float)0., (float)
		.01186, (float)0., {0}, (float)0., (float)0., (float).00674, (
		float)0., {0}, (float)0., (float)0., (float).00372, (float)0.,
		 {0}, (float)0., (float)0., (float).00199, (float)0., {0}, (
		float)0., (float)0., (float)9.9e-4, (float)0., {0}, (float)0.,
		 (float)0., (float)3.5e-4, (float)0., {0}, (float)0., (float)
		0., (float)1e-5, (float)0., {0}, (float)0., (float)0., (float)
		1e-5, (float)0., {0}, (float)0., (float)0., (float)0., (float)
		0., {0}, (float)0., (float)0., (float)0., (float)0., {0}, (
		float)0., (float)0., (float)0., (float)0., {0}, (float)0., (
		float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)1e-5, {0}, (float)
		0., (float)0., (float)0., (float)1e-5, {0}, (float)0., (float)
		0., (float)0., (float)0., {0}, (float)0., (float)0., (float)
		0., (float)0., {0}, (float)0., (float)0., (float)0., (float)
		0., {0}, (float)0., (float)0., (float)0., (float)0., {0}, (
		float)0., (float)0., (float)0., (float)0., {0}, (float)0., (
		float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)1e-5, {0}, (float)
		0., (float)0., (float)0., (float)3e-5, {0}, (float)0., (float)
		0., (float)0., (float)7e-5, {0}, (float)0., (float)0., (float)
		0., (float)1e-4, {0}, (float)0., (float)0., (float)0., (float)
		1.6e-4, {0}, (float)0., (float)0., (float)0., (float)1.8e-4, {
		0}, (float)0., (float)0., (float)0., (float)1.9e-4, {0}, (
		float)0., (float)0., (float)0., (float)3.8e-4, {0}, (float)0.,
		 (float)0., (float)0., (float)7e-4, {0}, (float)0., (float)0.,
		 (float)0., (float)7.4e-4, {0}, (float)0., (float)0., (float)
		0., (float)8.7e-4, {0}, (float)0., (float)0., (float)0., (
		float)9.9e-4, {0}, (float)0., (float)0., (float)0., (float)
		.00112, {0}, (float)0., (float)0., (float)0., (float).00144, {
		0}, (float)0., (float)0., (float)0., (float).00178, {0}, (
		float)0., (float)0., (float)0., (float).0021, {0}, (float)0., 
		(float)0., (float)0., (float).00252, {0}, (float)0., (float)
		0., (float)0., (float).00314, {0}, (float)0., (float)0., (
		float)0., (float).00381, {0}, (float)0., (float)0., (float)0.,
		 (float).00482, {0}, (float)0., (float)0., (float)0., (float)
		.00617, {0}, (float)0., (float)0., (float)0., (float).00808, {
		0}, (float)0., (float)0., (float)0., (float).01087, {0}, (
		float)0., (float)0., (float)0., (float).01507, {0}, (float)0.,
		 (float)0., (float)0., (float).02115, {0}, (float)0., (float)
		0., (float)0., (float).03014, {0}, (float)0., (float)0., (
		float)0., (float).04352, {0}, (float)0., (float)0., (float)0.,
		 (float).06372, {0}, (float)0., (float)0., (float)0., (float)
		.09368, {0}, (float)0., (float)0., (float)0., (float).13303, {
		0}, (float)0., (float)0., (float)0., (float).18172, {0}, (
		float)0., (float)0., (float)0., (float).239, {0}, (float)0., (
		float)0., (float)0., (float).29735, {0}, (float)0., (float)0.,
		 (float)0., (float).36285, {0}, (float)0., (float)0., (float)
		0., (float).43193, {0}, (float)0., (float)0., (float)0., (
		float).50378, {0}, (float)0., (float)0., (float)0., (float)
		.57403, {0}, (float)0., (float)0., (float)0., (float).64865, {
		0}, (float)0., (float)0., (float)0., (float).72474, {0}, (
		float)0., (float)0., (float)0., (float).79732, {0}, (float)0.,
		 (float)0., (float)0., (float).86795, {0}, (float)0., (float)
		0., (float)0., (float).92477, {0}, (float)0., (float)0., (
		float)0., (float).96695, {0}, (float)0., (float)0., (float)0.,
		 (float).99173, {0}, (float)0., (float)0., (float)0., (float)
		1., {0}, (float)0., (float)0., (float)0., (float).99937, {0}, 
		(float)0., (float)0., (float)0., (float).99592, {0}, (float)
		0., (float)0., (float)0., (float).98624, {0}, (float)0., (
		float)0., (float)0., (float).97501, {0}, (float)0., (float)0.,
		 (float)0., (float).95695, {0}, (float)0., (float)0., (float)
		0., (float).93594, {0}, (float)0., (float)0., (float)0., (
		float).90325, {0}, (float)0., (float)0., (float)0., (float)
		.87127, {0}, (float)0., (float)0., (float)0., (float).83451, {
		0}, (float)0., (float)0., (float)0., (float).79304, {0}, (
		float)0., (float)0., (float)0., (float).7532, {0}, (float)0., 
		(float)0., (float)0., (float).70611, {0}, (float)0., (float)
		0., (float)0., (float).65958, {0}, (float)0., (float)0., (
		float)0., (float).60791, {0}, (float)0., (float)0., (float)0.,
		 (float).55348, {0}, (float)0., (float)0., (float)0., (float)
		.49819, {0}, (float)0., (float)0., (float)0., (float).43767, {
		0}, (float)0., (float)0., (float)0., (float).37291, {0}, (
		float)0., (float)0., (float)0., (float).30937, {0}, (float)0.,
		 (float)0., (float)0., (float).2488, {0}, (float)0., (float)
		0., (float)0., (float).19559, {0}, (float)0., (float)0., (
		float)0., (float).14848, {0}, (float)0., (float)0., (float)0.,
		 (float).10947, {0}, (float)0., (float)0., (float)0., (float)
		.0794, {0}, (float)0., (float)0., (float)0., (float).05786, {
		0}, (float)0., (float)0., (float)0., (float).04258, {0}, (
		float)0., (float)0., (float)0., (float).03172, {0}, (float)0.,
		 (float)0., (float)0., (float).02439, {0}, (float)0., (float)
		0., (float)0., (float).01898, {0}, (float)0., (float)0., (
		float)0., (float).015, {0}, (float)0., (float)0., (float)0., (
		float).01187, {0}, (float)0., (float)0., (float)0., (float)
		.00984, {0}, (float)0., (float)0., (float)0., (float).0081, {
		0}, (float)0., (float)0., (float)0., (float).00664, {0}, (
		float)0., (float)0., (float)0., (float).00558, {0}, (float)0.,
		 (float)0., (float)0., (float).00481, {0}, (float)0., (float)
		0., (float)0., (float).00407, {0}, (float)0., (float)0., (
		float)0., (float).00346, {0}, (float)0., (float)0., (float)0.,
		 (float).00293, {0}, (float)0., (float)0., (float)0., (float)
		.0026, {0}, (float)0., (float)0., (float)0., (float).00218, {
		0}, (float)0., (float)0., (float)0., (float).0018, {0}, (
		float)0., (float)0., (float)0., (float).00167, {0}, (float)0.,
		 (float)0., (float)0., (float).00149, {0}, (float)0., (float)
		0., (float)0., (float).00132, {0}, (float)0., (float)0., (
		float)0., (float).00127, {0}, (float)0., (float)0., (float)0.,
		 (float).00118, {0}, (float)0., (float)0., (float)0., (float)
		8.3e-4, {0}, (float)0., (float)0., (float)0., (float)5.6e-4, {
		0}, (float)0., (float)0., (float)0., (float)4.9e-4, {0}, (
		float)0., (float)0., (float)0., (float)4.8e-4, {0}, (float)0.,
		 (float)0., (float)0., (float)4.8e-4, {0}, (float)0., (float)
		0., (float)0., (float)4.6e-4, {0}, (float)0., (float)0., (
		float)0., (float)4.3e-4, {0}, (float)0., (float)0., (float)0.,
		 (float)3.8e-4, {0}, (float)0., (float)0., (float)0., (float)
		3.1e-4, {0}, (float)0., (float)0., (float)0., (float)2.7e-4, {
		0}, (float)0., (float)0., (float)0., (float)2.4e-4, {0}, (
		float)0., (float)0., (float)0., (float)2.4e-4, {0}, (float)0.,
		 (float)0., (float)0., (float)2.3e-4, {0}, (float)0., (float)
		0., (float)0., (float)2.4e-4, {0}, (float)0., (float)0., (
		float)0., (float)2.4e-4, {0}, (float)0., (float)0., (float)0.,
		 (float)2.4e-4, {0}, (float)0., (float)0., (float)0., (float)
		2.5e-4, {0}, (float)0., (float)0., (float)0., (float)2.5e-4, {
		0}, (float)0., (float)0., (float)0., (float)2.6e-4, {0}, (
		float)0., (float)0., (float)0., (float)2.6e-4, {0}, (float)0.,
		 (float)0., (float)0., (float)2.7e-4, {0}, (float)0., (float)
		0., (float)0., (float)2.7e-4, {0}, (float)0., (float)0., (
		float)0., (float)2.7e-4, {0}, (float)0., (float)0., (float)0.,
		 (float)2.8e-4, {0}, (float)0., (float)0., (float)0., (float)
		2.8e-4, {0}, (float)0., (float)0., (float)0., (float)2.8e-4, {
		0}, (float)0., (float)0., (float)0., (float)2.9e-4, {0}, (
		float)0., (float)0., (float)0., (float)2.9e-4, {0}, (float)0.,
		 (float)0., (float)0., (float)2.8e-4, {0}, (float)0., (float)
		0., (float)0., (float)3.2e-4, {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0., {0}, (float)0., 
		(float)0., (float)0., (float)0., {0}, (float)0., (float)0., (
		float)0., (float)0., {0}, (float)0., (float)0., (float)0., (
		float)0., {0}, (float)0., (float)0., (float)0., (float)0., {0}
		, (float)0., (float)0., (float)0., (float)0. };

#define sr ((real *)&equiv_5)


    integer i__;
    real wli[8], wls[8];


/* created: J. Nieke july/2003 */
/* aatsr response function values */
/* are based on version (ESA/Vega/H. Tait) */
/* v55_srf.txt, v659_srf.txt, v870_srf.txt, v16_srf.txt */
/* Values are interpolated to 2.5nm wavelenght intervals */


/*<       real s,wlinf,wlsup >*/
/*<       common /sixs_ffu/ s(1501),wlinf,wlsup >*/
/*<       real sr(8,1501),wli(8),wls(8) >*/
/*<       integer iwa,l,i >*/
/* band 1 of AATSR  (0.525000 => 0.592500um) */
/*<    >*/
/* band 2 of AATSR  (0.6275 => 0.6975um) */
/*<    >*/
/* band 3 of AATSR  (0.8325 => 0.9025um) */
/*<    >*/
/* band 4 of AATSR  (1.4475 => 1.7775um) */
/*<    >*/
/* channel 1 lower and upper wavelength */
/*<       wli(1)=0.525 >*/
    wli[0] = (float).525;
/*<       wls(1)=0.5925 >*/
    wls[0] = (float).5925;
/* channel 2 lower and upper wavelength */
/*<       wli(2)=0.6275 >*/
    wli[1] = (float).6275;
/*<       wls(2)=0.6975 >*/
    wls[1] = (float).6975;
/* channel 3 lower and upper wavelength */
/*<       wli(3)=0.8325 >*/
    wli[2] = (float).8325;
/*<       wls(3)=0.9025 >*/
    wls[2] = (float).9025;
/* channel 4 lower and upper wavelength */
/*<       wli(4)=1.4475 >*/
    wli[3] = (float)1.4475;
/*<       wls(4)=1.7775 >*/
    wls[3] = (float)1.7775;
/*<       do 1 i=1,1501 >*/
    for (i__ = 1; i__ <= 1501; ++i__) {
/*<       s(i)=sr(iwa,i) >*/
	sixs_ffu__1.s[i__ - 1] = sr[*iwa + (i__ << 3) - 9];
/*<     1 continue >*/
/* L1: */
    }
/*<       wlinf=wli(iwa) >*/
    sixs_ffu__1.wlinf = wli[*iwa - 1];
/*<       wlsup=wls(iwa) >*/
    sixs_ffu__1.wlsup = wls[*iwa - 1];
/*<       return >*/
    return 0;
/*<       end     >*/
} /* aatsr_ */

#undef sr

#ifdef __cplusplus
	}
#endif

