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
    doublereal s[1501], wlinf, wlsup;
} sixs_ffu__;

#define sixs_ffu__1 sixs_ffu__

/*<       subroutine aatsr(iwa) >*/
/* Subroutine */ int aatsr_(integer *iwa)
{
    /* Initialized data */

    static struct {
	doublereal e_1[4];
	doublereal fill_2[4];
	doublereal e_3[4];
	doublereal fill_4[4];
	doublereal e_5[4];
	doublereal fill_6[4];
	doublereal e_7[4];
	doublereal fill_8[4];
	doublereal e_9[4];
	doublereal fill_10[4];
	doublereal e_11[4];
	doublereal fill_12[4];
	doublereal e_13[4];
	doublereal fill_14[4];
	doublereal e_15[4];
	doublereal fill_16[4];
	doublereal e_17[4];
	doublereal fill_18[4];
	doublereal e_19[4];
	doublereal fill_20[4];
	doublereal e_21[4];
	doublereal fill_22[4];
	doublereal e_23[4];
	doublereal fill_24[4];
	doublereal e_25[4];
	doublereal fill_26[4];
	doublereal e_27[4];
	doublereal fill_28[4];
	doublereal e_29[4];
	doublereal fill_30[4];
	doublereal e_31[4];
	doublereal fill_32[4];
	doublereal e_33[4];
	doublereal fill_34[4];
	doublereal e_35[4];
	doublereal fill_36[4];
	doublereal e_37[4];
	doublereal fill_38[4];
	doublereal e_39[4];
	doublereal fill_40[4];
	doublereal e_41[4];
	doublereal fill_42[4];
	doublereal e_43[4];
	doublereal fill_44[4];
	doublereal e_45[4];
	doublereal fill_46[4];
	doublereal e_47[4];
	doublereal fill_48[4];
	doublereal e_49[4];
	doublereal fill_50[4];
	doublereal e_51[4];
	doublereal fill_52[4];
	doublereal e_53[4];
	doublereal fill_54[4];
	doublereal e_55[4];
	doublereal fill_56[4];
	doublereal e_57[4];
	doublereal fill_58[4];
	doublereal e_59[4];
	doublereal fill_60[4];
	doublereal e_61[4];
	doublereal fill_62[4];
	doublereal e_63[4];
	doublereal fill_64[4];
	doublereal e_65[4];
	doublereal fill_66[4];
	doublereal e_67[4];
	doublereal fill_68[4];
	doublereal e_69[4];
	doublereal fill_70[4];
	doublereal e_71[4];
	doublereal fill_72[4];
	doublereal e_73[4];
	doublereal fill_74[4];
	doublereal e_75[4];
	doublereal fill_76[4];
	doublereal e_77[4];
	doublereal fill_78[4];
	doublereal e_79[4];
	doublereal fill_80[4];
	doublereal e_81[4];
	doublereal fill_82[4];
	doublereal e_83[4];
	doublereal fill_84[4];
	doublereal e_85[4];
	doublereal fill_86[4];
	doublereal e_87[4];
	doublereal fill_88[4];
	doublereal e_89[4];
	doublereal fill_90[4];
	doublereal e_91[4];
	doublereal fill_92[4];
	doublereal e_93[4];
	doublereal fill_94[4];
	doublereal e_95[4];
	doublereal fill_96[4];
	doublereal e_97[4];
	doublereal fill_98[4];
	doublereal e_99[4];
	doublereal fill_100[4];
	doublereal e_101[4];
	doublereal fill_102[4];
	doublereal e_103[4];
	doublereal fill_104[4];
	doublereal e_105[4];
	doublereal fill_106[4];
	doublereal e_107[4];
	doublereal fill_108[4];
	doublereal e_109[4];
	doublereal fill_110[4];
	doublereal e_111[4];
	doublereal fill_112[4];
	doublereal e_113[4];
	doublereal fill_114[4];
	doublereal e_115[4];
	doublereal fill_116[4];
	doublereal e_117[4];
	doublereal fill_118[4];
	doublereal e_119[4];
	doublereal fill_120[4];
	doublereal e_121[4];
	doublereal fill_122[4];
	doublereal e_123[4];
	doublereal fill_124[4];
	doublereal e_125[4];
	doublereal fill_126[4];
	doublereal e_127[4];
	doublereal fill_128[4];
	doublereal e_129[4];
	doublereal fill_130[4];
	doublereal e_131[4];
	doublereal fill_132[4];
	doublereal e_133[4];
	doublereal fill_134[4];
	doublereal e_135[4];
	doublereal fill_136[4];
	doublereal e_137[4];
	doublereal fill_138[4];
	doublereal e_139[4];
	doublereal fill_140[4];
	doublereal e_141[4];
	doublereal fill_142[4];
	doublereal e_143[4];
	doublereal fill_144[4];
	doublereal e_145[4];
	doublereal fill_146[4];
	doublereal e_147[4];
	doublereal fill_148[4];
	doublereal e_149[4];
	doublereal fill_150[4];
	doublereal e_151[4];
	doublereal fill_152[4];
	doublereal e_153[4];
	doublereal fill_154[4];
	doublereal e_155[4];
	doublereal fill_156[4];
	doublereal e_157[4];
	doublereal fill_158[4];
	doublereal e_159[4];
	doublereal fill_160[4];
	doublereal e_161[4];
	doublereal fill_162[4];
	doublereal e_163[4];
	doublereal fill_164[4];
	doublereal e_165[4];
	doublereal fill_166[4];
	doublereal e_167[4];
	doublereal fill_168[4];
	doublereal e_169[4];
	doublereal fill_170[4];
	doublereal e_171[4];
	doublereal fill_172[4];
	doublereal e_173[4];
	doublereal fill_174[4];
	doublereal e_175[4];
	doublereal fill_176[4];
	doublereal e_177[4];
	doublereal fill_178[4];
	doublereal e_179[4];
	doublereal fill_180[4];
	doublereal e_181[4];
	doublereal fill_182[4];
	doublereal e_183[4];
	doublereal fill_184[4];
	doublereal e_185[4];
	doublereal fill_186[4];
	doublereal e_187[4];
	doublereal fill_188[4];
	doublereal e_189[4];
	doublereal fill_190[4];
	doublereal e_191[4];
	doublereal fill_192[4];
	doublereal e_193[4];
	doublereal fill_194[4];
	doublereal e_195[4];
	doublereal fill_196[4];
	doublereal e_197[4];
	doublereal fill_198[4];
	doublereal e_199[4];
	doublereal fill_200[4];
	doublereal e_201[4];
	doublereal fill_202[4];
	doublereal e_203[4];
	doublereal fill_204[4];
	doublereal e_205[4];
	doublereal fill_206[4];
	doublereal e_207[4];
	doublereal fill_208[4];
	doublereal e_209[4];
	doublereal fill_210[4];
	doublereal e_211[4];
	doublereal fill_212[4];
	doublereal e_213[4];
	doublereal fill_214[4];
	doublereal e_215[4];
	doublereal fill_216[4];
	doublereal e_217[4];
	doublereal fill_218[4];
	doublereal e_219[4];
	doublereal fill_220[4];
	doublereal e_221[4];
	doublereal fill_222[4];
	doublereal e_223[4];
	doublereal fill_224[4];
	doublereal e_225[4];
	doublereal fill_226[4];
	doublereal e_227[4];
	doublereal fill_228[4];
	doublereal e_229[4];
	doublereal fill_230[4];
	doublereal e_231[4];
	doublereal fill_232[4];
	doublereal e_233[4];
	doublereal fill_234[4];
	doublereal e_235[4];
	doublereal fill_236[4];
	doublereal e_237[4];
	doublereal fill_238[4];
	doublereal e_239[4];
	doublereal fill_240[4];
	doublereal e_241[4];
	doublereal fill_242[4];
	doublereal e_243[4];
	doublereal fill_244[4];
	doublereal e_245[4];
	doublereal fill_246[4];
	doublereal e_247[4];
	doublereal fill_248[4];
	doublereal e_249[4];
	doublereal fill_250[4];
	doublereal e_251[4];
	doublereal fill_252[4];
	doublereal e_253[4];
	doublereal fill_254[4];
	doublereal e_255[4];
	doublereal fill_256[4];
	doublereal e_257[4];
	doublereal fill_258[4];
	doublereal e_259[4];
	doublereal fill_260[4];
	doublereal e_261[4];
	doublereal fill_262[4];
	doublereal e_263[4];
	doublereal fill_264[4];
	doublereal e_265[4];
	doublereal fill_266[4];
	doublereal e_267[4];
	doublereal fill_268[4];
	doublereal e_269[4];
	doublereal fill_270[4];
	doublereal e_271[4];
	doublereal fill_272[4];
	doublereal e_273[4];
	doublereal fill_274[4];
	doublereal e_275[4];
	doublereal fill_276[4];
	doublereal e_277[4];
	doublereal fill_278[4];
	doublereal e_279[4];
	doublereal fill_280[4];
	doublereal e_281[4];
	doublereal fill_282[4];
	doublereal e_283[4];
	doublereal fill_284[4];
	doublereal e_285[4];
	doublereal fill_286[4];
	doublereal e_287[4];
	doublereal fill_288[4];
	doublereal e_289[4];
	doublereal fill_290[4];
	doublereal e_291[4];
	doublereal fill_292[4];
	doublereal e_293[4];
	doublereal fill_294[4];
	doublereal e_295[4];
	doublereal fill_296[4];
	doublereal e_297[4];
	doublereal fill_298[4];
	doublereal e_299[4];
	doublereal fill_300[4];
	doublereal e_301[4];
	doublereal fill_302[4];
	doublereal e_303[4];
	doublereal fill_304[4];
	doublereal e_305[4];
	doublereal fill_306[4];
	doublereal e_307[4];
	doublereal fill_308[4];
	doublereal e_309[4];
	doublereal fill_310[4];
	doublereal e_311[4];
	doublereal fill_312[4];
	doublereal e_313[4];
	doublereal fill_314[4];
	doublereal e_315[4];
	doublereal fill_316[4];
	doublereal e_317[4];
	doublereal fill_318[4];
	doublereal e_319[4];
	doublereal fill_320[4];
	doublereal e_321[4];
	doublereal fill_322[4];
	doublereal e_323[4];
	doublereal fill_324[4];
	doublereal e_325[4];
	doublereal fill_326[4];
	doublereal e_327[4];
	doublereal fill_328[4];
	doublereal e_329[4];
	doublereal fill_330[4];
	doublereal e_331[4];
	doublereal fill_332[4];
	doublereal e_333[4];
	doublereal fill_334[4];
	doublereal e_335[4];
	doublereal fill_336[4];
	doublereal e_337[4];
	doublereal fill_338[4];
	doublereal e_339[4];
	doublereal fill_340[4];
	doublereal e_341[4];
	doublereal fill_342[4];
	doublereal e_343[4];
	doublereal fill_344[4];
	doublereal e_345[4];
	doublereal fill_346[4];
	doublereal e_347[4];
	doublereal fill_348[4];
	doublereal e_349[4];
	doublereal fill_350[4];
	doublereal e_351[4];
	doublereal fill_352[4];
	doublereal e_353[4];
	doublereal fill_354[4];
	doublereal e_355[4];
	doublereal fill_356[4];
	doublereal e_357[4];
	doublereal fill_358[4];
	doublereal e_359[4];
	doublereal fill_360[4];
	doublereal e_361[4];
	doublereal fill_362[4];
	doublereal e_363[4];
	doublereal fill_364[4];
	doublereal e_365[4];
	doublereal fill_366[4];
	doublereal e_367[4];
	doublereal fill_368[4];
	doublereal e_369[4];
	doublereal fill_370[4];
	doublereal e_371[4];
	doublereal fill_372[4];
	doublereal e_373[4];
	doublereal fill_374[4];
	doublereal e_375[4];
	doublereal fill_376[4];
	doublereal e_377[4];
	doublereal fill_378[4];
	doublereal e_379[4];
	doublereal fill_380[4];
	doublereal e_381[4];
	doublereal fill_382[4];
	doublereal e_383[4];
	doublereal fill_384[4];
	doublereal e_385[4];
	doublereal fill_386[4];
	doublereal e_387[4];
	doublereal fill_388[4];
	doublereal e_389[4];
	doublereal fill_390[4];
	doublereal e_391[4];
	doublereal fill_392[4];
	doublereal e_393[4];
	doublereal fill_394[4];
	doublereal e_395[4];
	doublereal fill_396[4];
	doublereal e_397[4];
	doublereal fill_398[4];
	doublereal e_399[4];
	doublereal fill_400[4];
	doublereal e_401[4];
	doublereal fill_402[4];
	doublereal e_403[4];
	doublereal fill_404[4];
	doublereal e_405[4];
	doublereal fill_406[4];
	doublereal e_407[4];
	doublereal fill_408[4];
	doublereal e_409[4];
	doublereal fill_410[4];
	doublereal e_411[4];
	doublereal fill_412[4];
	doublereal e_413[4];
	doublereal fill_414[4];
	doublereal e_415[4];
	doublereal fill_416[4];
	doublereal e_417[4];
	doublereal fill_418[4];
	doublereal e_419[4];
	doublereal fill_420[4];
	doublereal e_421[4];
	doublereal fill_422[4];
	doublereal e_423[4];
	doublereal fill_424[4];
	doublereal e_425[4];
	doublereal fill_426[4];
	doublereal e_427[4];
	doublereal fill_428[4];
	doublereal e_429[4];
	doublereal fill_430[4];
	doublereal e_431[4];
	doublereal fill_432[4];
	doublereal e_433[4];
	doublereal fill_434[4];
	doublereal e_435[4];
	doublereal fill_436[4];
	doublereal e_437[4];
	doublereal fill_438[4];
	doublereal e_439[4];
	doublereal fill_440[4];
	doublereal e_441[4];
	doublereal fill_442[4];
	doublereal e_443[4];
	doublereal fill_444[4];
	doublereal e_445[4];
	doublereal fill_446[4];
	doublereal e_447[4];
	doublereal fill_448[4];
	doublereal e_449[4];
	doublereal fill_450[4];
	doublereal e_451[4];
	doublereal fill_452[4];
	doublereal e_453[4];
	doublereal fill_454[4];
	doublereal e_455[4];
	doublereal fill_456[4];
	doublereal e_457[4];
	doublereal fill_458[4];
	doublereal e_459[4];
	doublereal fill_460[4];
	doublereal e_461[4];
	doublereal fill_462[4];
	doublereal e_463[4];
	doublereal fill_464[4];
	doublereal e_465[4];
	doublereal fill_466[4];
	doublereal e_467[4];
	doublereal fill_468[4];
	doublereal e_469[4];
	doublereal fill_470[4];
	doublereal e_471[4];
	doublereal fill_472[4];
	doublereal e_473[4];
	doublereal fill_474[4];
	doublereal e_475[4];
	doublereal fill_476[4];
	doublereal e_477[4];
	doublereal fill_478[4];
	doublereal e_479[4];
	doublereal fill_480[4];
	doublereal e_481[4];
	doublereal fill_482[4];
	doublereal e_483[4];
	doublereal fill_484[4];
	doublereal e_485[4];
	doublereal fill_486[4];
	doublereal e_487[4];
	doublereal fill_488[4];
	doublereal e_489[4];
	doublereal fill_490[4];
	doublereal e_491[4];
	doublereal fill_492[4];
	doublereal e_493[4];
	doublereal fill_494[4];
	doublereal e_495[4];
	doublereal fill_496[4];
	doublereal e_497[4];
	doublereal fill_498[4];
	doublereal e_499[4];
	doublereal fill_500[4];
	doublereal e_501[4];
	doublereal fill_502[4];
	doublereal e_503[4];
	doublereal fill_504[4];
	doublereal e_505[4];
	doublereal fill_506[4];
	doublereal e_507[4];
	doublereal fill_508[4];
	doublereal e_509[4];
	doublereal fill_510[4];
	doublereal e_511[4];
	doublereal fill_512[4];
	doublereal e_513[4];
	doublereal fill_514[4];
	doublereal e_515[4];
	doublereal fill_516[4];
	doublereal e_517[4];
	doublereal fill_518[4];
	doublereal e_519[4];
	doublereal fill_520[4];
	doublereal e_521[4];
	doublereal fill_522[4];
	doublereal e_523[4];
	doublereal fill_524[4];
	doublereal e_525[4];
	doublereal fill_526[4];
	doublereal e_527[4];
	doublereal fill_528[4];
	doublereal e_529[4];
	doublereal fill_530[4];
	doublereal e_531[4];
	doublereal fill_532[4];
	doublereal e_533[4];
	doublereal fill_534[4];
	doublereal e_535[4];
	doublereal fill_536[4];
	doublereal e_537[4];
	doublereal fill_538[4];
	doublereal e_539[4];
	doublereal fill_540[4];
	doublereal e_541[4];
	doublereal fill_542[4];
	doublereal e_543[4];
	doublereal fill_544[4];
	doublereal e_545[4];
	doublereal fill_546[4];
	doublereal e_547[4];
	doublereal fill_548[4];
	doublereal e_549[4];
	doublereal fill_550[4];
	doublereal e_551[4];
	doublereal fill_552[4];
	doublereal e_553[4];
	doublereal fill_554[4];
	doublereal e_555[4];
	doublereal fill_556[4];
	doublereal e_557[4];
	doublereal fill_558[4];
	doublereal e_559[4];
	doublereal fill_560[4];
	doublereal e_561[4];
	doublereal fill_562[4];
	doublereal e_563[4];
	doublereal fill_564[4];
	doublereal e_565[4];
	doublereal fill_566[4];
	doublereal e_567[4];
	doublereal fill_568[4];
	doublereal e_569[4];
	doublereal fill_570[4];
	doublereal e_571[4];
	doublereal fill_572[4];
	doublereal e_573[4];
	doublereal fill_574[4];
	doublereal e_575[4];
	doublereal fill_576[4];
	doublereal e_577[4];
	doublereal fill_578[4];
	doublereal e_579[4];
	doublereal fill_580[4];
	doublereal e_581[4];
	doublereal fill_582[4];
	doublereal e_583[4];
	doublereal fill_584[4];
	doublereal e_585[4];
	doublereal fill_586[4];
	doublereal e_587[4];
	doublereal fill_588[4];
	doublereal e_589[4];
	doublereal fill_590[4];
	doublereal e_591[4];
	doublereal fill_592[4];
	doublereal e_593[4];
	doublereal fill_594[4];
	doublereal e_595[4];
	doublereal fill_596[4];
	doublereal e_597[4];
	doublereal fill_598[4];
	doublereal e_599[4];
	doublereal fill_600[4];
	doublereal e_601[4];
	doublereal fill_602[4];
	doublereal e_603[4];
	doublereal fill_604[4];
	doublereal e_605[4];
	doublereal fill_606[4];
	doublereal e_607[4];
	doublereal fill_608[4];
	doublereal e_609[4];
	doublereal fill_610[4];
	doublereal e_611[4];
	doublereal fill_612[4];
	doublereal e_613[4];
	doublereal fill_614[4];
	doublereal e_615[4];
	doublereal fill_616[4];
	doublereal e_617[4];
	doublereal fill_618[4];
	doublereal e_619[4];
	doublereal fill_620[4];
	doublereal e_621[4];
	doublereal fill_622[4];
	doublereal e_623[4];
	doublereal fill_624[4];
	doublereal e_625[4];
	doublereal fill_626[4];
	doublereal e_627[4];
	doublereal fill_628[4];
	doublereal e_629[4];
	doublereal fill_630[4];
	doublereal e_631[4];
	doublereal fill_632[4];
	doublereal e_633[4];
	doublereal fill_634[4];
	doublereal e_635[4];
	doublereal fill_636[4];
	doublereal e_637[4];
	doublereal fill_638[4];
	doublereal e_639[4];
	doublereal fill_640[4];
	doublereal e_641[4];
	doublereal fill_642[4];
	doublereal e_643[4];
	doublereal fill_644[4];
	doublereal e_645[4];
	doublereal fill_646[4];
	doublereal e_647[4];
	doublereal fill_648[4];
	doublereal e_649[4];
	doublereal fill_650[4];
	doublereal e_651[4];
	doublereal fill_652[4];
	doublereal e_653[4];
	doublereal fill_654[4];
	doublereal e_655[4];
	doublereal fill_656[4];
	doublereal e_657[4];
	doublereal fill_658[4];
	doublereal e_659[4];
	doublereal fill_660[4];
	doublereal e_661[4];
	doublereal fill_662[4];
	doublereal e_663[4];
	doublereal fill_664[4];
	doublereal e_665[4];
	doublereal fill_666[4];
	doublereal e_667[4];
	doublereal fill_668[4];
	doublereal e_669[4];
	doublereal fill_670[4];
	doublereal e_671[4];
	doublereal fill_672[4];
	doublereal e_673[4];
	doublereal fill_674[4];
	doublereal e_675[4];
	doublereal fill_676[4];
	doublereal e_677[4];
	doublereal fill_678[4];
	doublereal e_679[4];
	doublereal fill_680[4];
	doublereal e_681[4];
	doublereal fill_682[4];
	doublereal e_683[4];
	doublereal fill_684[4];
	doublereal e_685[4];
	doublereal fill_686[4];
	doublereal e_687[4];
	doublereal fill_688[4];
	doublereal e_689[4];
	doublereal fill_690[4];
	doublereal e_691[4];
	doublereal fill_692[4];
	doublereal e_693[4];
	doublereal fill_694[4];
	doublereal e_695[4];
	doublereal fill_696[4];
	doublereal e_697[4];
	doublereal fill_698[4];
	doublereal e_699[4];
	doublereal fill_700[4];
	doublereal e_701[4];
	doublereal fill_702[4];
	doublereal e_703[4];
	doublereal fill_704[4];
	doublereal e_705[4];
	doublereal fill_706[4];
	doublereal e_707[4];
	doublereal fill_708[4];
	doublereal e_709[4];
	doublereal fill_710[4];
	doublereal e_711[4];
	doublereal fill_712[4];
	doublereal e_713[4];
	doublereal fill_714[4];
	doublereal e_715[4];
	doublereal fill_716[4];
	doublereal e_717[4];
	doublereal fill_718[4];
	doublereal e_719[4];
	doublereal fill_720[4];
	doublereal e_721[4];
	doublereal fill_722[4];
	doublereal e_723[4];
	doublereal fill_724[4];
	doublereal e_725[4];
	doublereal fill_726[4];
	doublereal e_727[4];
	doublereal fill_728[4];
	doublereal e_729[4];
	doublereal fill_730[4];
	doublereal e_731[4];
	doublereal fill_732[4];
	doublereal e_733[4];
	doublereal fill_734[4];
	doublereal e_735[4];
	doublereal fill_736[4];
	doublereal e_737[4];
	doublereal fill_738[4];
	doublereal e_739[4];
	doublereal fill_740[4];
	doublereal e_741[4];
	doublereal fill_742[4];
	doublereal e_743[4];
	doublereal fill_744[4];
	doublereal e_745[4];
	doublereal fill_746[4];
	doublereal e_747[4];
	doublereal fill_748[4];
	doublereal e_749[4];
	doublereal fill_750[4];
	doublereal e_751[4];
	doublereal fill_752[4];
	doublereal e_753[4];
	doublereal fill_754[4];
	doublereal e_755[4];
	doublereal fill_756[4];
	doublereal e_757[4];
	doublereal fill_758[4];
	doublereal e_759[4];
	doublereal fill_760[4];
	doublereal e_761[4];
	doublereal fill_762[4];
	doublereal e_763[4];
	doublereal fill_764[4];
	doublereal e_765[4];
	doublereal fill_766[4];
	doublereal e_767[4];
	doublereal fill_768[4];
	doublereal e_769[4];
	doublereal fill_770[4];
	doublereal e_771[4];
	doublereal fill_772[4];
	doublereal e_773[4];
	doublereal fill_774[4];
	doublereal e_775[4];
	doublereal fill_776[4];
	doublereal e_777[4];
	doublereal fill_778[4];
	doublereal e_779[4];
	doublereal fill_780[4];
	doublereal e_781[4];
	doublereal fill_782[4];
	doublereal e_783[4];
	doublereal fill_784[4];
	doublereal e_785[4];
	doublereal fill_786[4];
	doublereal e_787[4];
	doublereal fill_788[4];
	doublereal e_789[4];
	doublereal fill_790[4];
	doublereal e_791[4];
	doublereal fill_792[4];
	doublereal e_793[4];
	doublereal fill_794[4];
	doublereal e_795[4];
	doublereal fill_796[4];
	doublereal e_797[4];
	doublereal fill_798[4];
	doublereal e_799[4];
	doublereal fill_800[4];
	doublereal e_801[4];
	doublereal fill_802[4];
	doublereal e_803[4];
	doublereal fill_804[4];
	doublereal e_805[4];
	doublereal fill_806[4];
	doublereal e_807[4];
	doublereal fill_808[4];
	doublereal e_809[4];
	doublereal fill_810[4];
	doublereal e_811[4];
	doublereal fill_812[4];
	doublereal e_813[4];
	doublereal fill_814[4];
	doublereal e_815[4];
	doublereal fill_816[4];
	doublereal e_817[4];
	doublereal fill_818[4];
	doublereal e_819[4];
	doublereal fill_820[4];
	doublereal e_821[4];
	doublereal fill_822[4];
	doublereal e_823[4];
	doublereal fill_824[4];
	doublereal e_825[4];
	doublereal fill_826[4];
	doublereal e_827[4];
	doublereal fill_828[4];
	doublereal e_829[4];
	doublereal fill_830[4];
	doublereal e_831[4];
	doublereal fill_832[4];
	doublereal e_833[4];
	doublereal fill_834[4];
	doublereal e_835[4];
	doublereal fill_836[4];
	doublereal e_837[4];
	doublereal fill_838[4];
	doublereal e_839[4];
	doublereal fill_840[4];
	doublereal e_841[4];
	doublereal fill_842[4];
	doublereal e_843[4];
	doublereal fill_844[4];
	doublereal e_845[4];
	doublereal fill_846[4];
	doublereal e_847[4];
	doublereal fill_848[4];
	doublereal e_849[4];
	doublereal fill_850[4];
	doublereal e_851[4];
	doublereal fill_852[4];
	doublereal e_853[4];
	doublereal fill_854[4];
	doublereal e_855[4];
	doublereal fill_856[4];
	doublereal e_857[4];
	doublereal fill_858[4];
	doublereal e_859[4];
	doublereal fill_860[4];
	doublereal e_861[4];
	doublereal fill_862[4];
	doublereal e_863[4];
	doublereal fill_864[4];
	doublereal e_865[4];
	doublereal fill_866[4];
	doublereal e_867[4];
	doublereal fill_868[4];
	doublereal e_869[4];
	doublereal fill_870[4];
	doublereal e_871[4];
	doublereal fill_872[4];
	doublereal e_873[4];
	doublereal fill_874[4];
	doublereal e_875[4];
	doublereal fill_876[4];
	doublereal e_877[4];
	doublereal fill_878[4];
	doublereal e_879[4];
	doublereal fill_880[4];
	doublereal e_881[4];
	doublereal fill_882[4];
	doublereal e_883[4];
	doublereal fill_884[4];
	doublereal e_885[4];
	doublereal fill_886[4];
	doublereal e_887[4];
	doublereal fill_888[4];
	doublereal e_889[4];
	doublereal fill_890[4];
	doublereal e_891[4];
	doublereal fill_892[4];
	doublereal e_893[4];
	doublereal fill_894[4];
	doublereal e_895[4];
	doublereal fill_896[4];
	doublereal e_897[4];
	doublereal fill_898[4];
	doublereal e_899[4];
	doublereal fill_900[4];
	doublereal e_901[4];
	doublereal fill_902[4];
	doublereal e_903[4];
	doublereal fill_904[4];
	doublereal e_905[4];
	doublereal fill_906[4];
	doublereal e_907[4];
	doublereal fill_908[4];
	doublereal e_909[4];
	doublereal fill_910[4];
	doublereal e_911[4];
	doublereal fill_912[4];
	doublereal e_913[4];
	doublereal fill_914[4];
	doublereal e_915[4];
	doublereal fill_916[4];
	doublereal e_917[4];
	doublereal fill_918[4];
	doublereal e_919[4];
	doublereal fill_920[4];
	doublereal e_921[4];
	doublereal fill_922[4];
	doublereal e_923[4];
	doublereal fill_924[4];
	doublereal e_925[4];
	doublereal fill_926[4];
	doublereal e_927[4];
	doublereal fill_928[4];
	doublereal e_929[4];
	doublereal fill_930[4];
	doublereal e_931[4];
	doublereal fill_932[4];
	doublereal e_933[4];
	doublereal fill_934[4];
	doublereal e_935[4];
	doublereal fill_936[4];
	doublereal e_937[4];
	doublereal fill_938[4];
	doublereal e_939[4];
	doublereal fill_940[4];
	doublereal e_941[4];
	doublereal fill_942[4];
	doublereal e_943[4];
	doublereal fill_944[4];
	doublereal e_945[4];
	doublereal fill_946[4];
	doublereal e_947[4];
	doublereal fill_948[4];
	doublereal e_949[4];
	doublereal fill_950[4];
	doublereal e_951[4];
	doublereal fill_952[4];
	doublereal e_953[4];
	doublereal fill_954[4];
	doublereal e_955[4];
	doublereal fill_956[4];
	doublereal e_957[4];
	doublereal fill_958[4];
	doublereal e_959[4];
	doublereal fill_960[4];
	doublereal e_961[4];
	doublereal fill_962[4];
	doublereal e_963[4];
	doublereal fill_964[4];
	doublereal e_965[4];
	doublereal fill_966[4];
	doublereal e_967[4];
	doublereal fill_968[4];
	doublereal e_969[4];
	doublereal fill_970[4];
	doublereal e_971[4];
	doublereal fill_972[4];
	doublereal e_973[4];
	doublereal fill_974[4];
	doublereal e_975[4];
	doublereal fill_976[4];
	doublereal e_977[4];
	doublereal fill_978[4];
	doublereal e_979[4];
	doublereal fill_980[4];
	doublereal e_981[4];
	doublereal fill_982[4];
	doublereal e_983[4];
	doublereal fill_984[4];
	doublereal e_985[4];
	doublereal fill_986[4];
	doublereal e_987[4];
	doublereal fill_988[4];
	doublereal e_989[4];
	doublereal fill_990[4];
	doublereal e_991[4];
	doublereal fill_992[4];
	doublereal e_993[4];
	doublereal fill_994[4];
	doublereal e_995[4];
	doublereal fill_996[4];
	doublereal e_997[4];
	doublereal fill_998[4];
	doublereal e_999[4];
	doublereal fill_1000[4];
	doublereal e_1001[4];
	doublereal fill_1002[4];
	doublereal e_1003[4];
	doublereal fill_1004[4];
	doublereal e_1005[4];
	doublereal fill_1006[4];
	doublereal e_1007[4];
	doublereal fill_1008[4];
	doublereal e_1009[4];
	doublereal fill_1010[4];
	doublereal e_1011[4];
	doublereal fill_1012[4];
	doublereal e_1013[4];
	doublereal fill_1014[4];
	doublereal e_1015[4];
	doublereal fill_1016[4];
	doublereal e_1017[4];
	doublereal fill_1018[4];
	doublereal e_1019[4];
	doublereal fill_1020[4];
	doublereal e_1021[4];
	doublereal fill_1022[4];
	doublereal e_1023[4];
	doublereal fill_1024[4];
	doublereal e_1025[4];
	doublereal fill_1026[4];
	doublereal e_1027[4];
	doublereal fill_1028[4];
	doublereal e_1029[4];
	doublereal fill_1030[4];
	doublereal e_1031[4];
	doublereal fill_1032[4];
	doublereal e_1033[4];
	doublereal fill_1034[4];
	doublereal e_1035[4];
	doublereal fill_1036[4];
	doublereal e_1037[4];
	doublereal fill_1038[4];
	doublereal e_1039[4];
	doublereal fill_1040[4];
	doublereal e_1041[4];
	doublereal fill_1042[4];
	doublereal e_1043[4];
	doublereal fill_1044[4];
	doublereal e_1045[4];
	doublereal fill_1046[4];
	doublereal e_1047[4];
	doublereal fill_1048[4];
	doublereal e_1049[4];
	doublereal fill_1050[4];
	doublereal e_1051[4];
	doublereal fill_1052[4];
	doublereal e_1053[4];
	doublereal fill_1054[4];
	doublereal e_1055[4];
	doublereal fill_1056[4];
	doublereal e_1057[4];
	doublereal fill_1058[4];
	doublereal e_1059[4];
	doublereal fill_1060[4];
	doublereal e_1061[4];
	doublereal fill_1062[4];
	doublereal e_1063[4];
	doublereal fill_1064[4];
	doublereal e_1065[4];
	doublereal fill_1066[4];
	doublereal e_1067[4];
	doublereal fill_1068[4];
	doublereal e_1069[4];
	doublereal fill_1070[4];
	doublereal e_1071[4];
	doublereal fill_1072[4];
	doublereal e_1073[4];
	doublereal fill_1074[4];
	doublereal e_1075[4];
	doublereal fill_1076[4];
	doublereal e_1077[4];
	doublereal fill_1078[4];
	doublereal e_1079[4];
	doublereal fill_1080[4];
	doublereal e_1081[4];
	doublereal fill_1082[4];
	doublereal e_1083[4];
	doublereal fill_1084[4];
	doublereal e_1085[4];
	doublereal fill_1086[4];
	doublereal e_1087[4];
	doublereal fill_1088[4];
	doublereal e_1089[4];
	doublereal fill_1090[4];
	doublereal e_1091[4];
	doublereal fill_1092[4];
	doublereal e_1093[4];
	doublereal fill_1094[4];
	doublereal e_1095[4];
	doublereal fill_1096[4];
	doublereal e_1097[4];
	doublereal fill_1098[4];
	doublereal e_1099[4];
	doublereal fill_1100[4];
	doublereal e_1101[4];
	doublereal fill_1102[4];
	doublereal e_1103[4];
	doublereal fill_1104[4];
	doublereal e_1105[4];
	doublereal fill_1106[4];
	doublereal e_1107[4];
	doublereal fill_1108[4];
	doublereal e_1109[4];
	doublereal fill_1110[4];
	doublereal e_1111[4];
	doublereal fill_1112[4];
	doublereal e_1113[4];
	doublereal fill_1114[4];
	doublereal e_1115[4];
	doublereal fill_1116[4];
	doublereal e_1117[4];
	doublereal fill_1118[4];
	doublereal e_1119[4];
	doublereal fill_1120[4];
	doublereal e_1121[4];
	doublereal fill_1122[4];
	doublereal e_1123[4];
	doublereal fill_1124[4];
	doublereal e_1125[4];
	doublereal fill_1126[4];
	doublereal e_1127[4];
	doublereal fill_1128[4];
	doublereal e_1129[4];
	doublereal fill_1130[4];
	doublereal e_1131[4];
	doublereal fill_1132[4];
	doublereal e_1133[4];
	doublereal fill_1134[4];
	doublereal e_1135[4];
	doublereal fill_1136[4];
	doublereal e_1137[4];
	doublereal fill_1138[4];
	doublereal e_1139[4];
	doublereal fill_1140[4];
	doublereal e_1141[4];
	doublereal fill_1142[4];
	doublereal e_1143[4];
	doublereal fill_1144[4];
	doublereal e_1145[4];
	doublereal fill_1146[4];
	doublereal e_1147[4];
	doublereal fill_1148[4];
	doublereal e_1149[4];
	doublereal fill_1150[4];
	doublereal e_1151[4];
	doublereal fill_1152[4];
	doublereal e_1153[4];
	doublereal fill_1154[4];
	doublereal e_1155[4];
	doublereal fill_1156[4];
	doublereal e_1157[4];
	doublereal fill_1158[4];
	doublereal e_1159[4];
	doublereal fill_1160[4];
	doublereal e_1161[4];
	doublereal fill_1162[4];
	doublereal e_1163[4];
	doublereal fill_1164[4];
	doublereal e_1165[4];
	doublereal fill_1166[4];
	doublereal e_1167[4];
	doublereal fill_1168[4];
	doublereal e_1169[4];
	doublereal fill_1170[4];
	doublereal e_1171[4];
	doublereal fill_1172[4];
	doublereal e_1173[4];
	doublereal fill_1174[4];
	doublereal e_1175[4];
	doublereal fill_1176[4];
	doublereal e_1177[4];
	doublereal fill_1178[4];
	doublereal e_1179[4];
	doublereal fill_1180[4];
	doublereal e_1181[4];
	doublereal fill_1182[4];
	doublereal e_1183[4];
	doublereal fill_1184[4];
	doublereal e_1185[4];
	doublereal fill_1186[4];
	doublereal e_1187[4];
	doublereal fill_1188[4];
	doublereal e_1189[4];
	doublereal fill_1190[4];
	doublereal e_1191[4];
	doublereal fill_1192[4];
	doublereal e_1193[4];
	doublereal fill_1194[4];
	doublereal e_1195[4];
	doublereal fill_1196[4];
	doublereal e_1197[4];
	doublereal fill_1198[4];
	doublereal e_1199[4];
	doublereal fill_1200[4];
	doublereal e_1201[4];
	doublereal fill_1202[4];
	doublereal e_1203[4];
	doublereal fill_1204[4];
	doublereal e_1205[4];
	doublereal fill_1206[4];
	doublereal e_1207[4];
	doublereal fill_1208[4];
	doublereal e_1209[4];
	doublereal fill_1210[4];
	doublereal e_1211[4];
	doublereal fill_1212[4];
	doublereal e_1213[4];
	doublereal fill_1214[4];
	doublereal e_1215[4];
	doublereal fill_1216[4];
	doublereal e_1217[4];
	doublereal fill_1218[4];
	doublereal e_1219[4];
	doublereal fill_1220[4];
	doublereal e_1221[4];
	doublereal fill_1222[4];
	doublereal e_1223[4];
	doublereal fill_1224[4];
	doublereal e_1225[4];
	doublereal fill_1226[4];
	doublereal e_1227[4];
	doublereal fill_1228[4];
	doublereal e_1229[4];
	doublereal fill_1230[4];
	doublereal e_1231[4];
	doublereal fill_1232[4];
	doublereal e_1233[4];
	doublereal fill_1234[4];
	doublereal e_1235[4];
	doublereal fill_1236[4];
	doublereal e_1237[4];
	doublereal fill_1238[4];
	doublereal e_1239[4];
	doublereal fill_1240[4];
	doublereal e_1241[4];
	doublereal fill_1242[4];
	doublereal e_1243[4];
	doublereal fill_1244[4];
	doublereal e_1245[4];
	doublereal fill_1246[4];
	doublereal e_1247[4];
	doublereal fill_1248[4];
	doublereal e_1249[4];
	doublereal fill_1250[4];
	doublereal e_1251[4];
	doublereal fill_1252[4];
	doublereal e_1253[4];
	doublereal fill_1254[4];
	doublereal e_1255[4];
	doublereal fill_1256[4];
	doublereal e_1257[4];
	doublereal fill_1258[4];
	doublereal e_1259[4];
	doublereal fill_1260[4];
	doublereal e_1261[4];
	doublereal fill_1262[4];
	doublereal e_1263[4];
	doublereal fill_1264[4];
	doublereal e_1265[4];
	doublereal fill_1266[4];
	doublereal e_1267[4];
	doublereal fill_1268[4];
	doublereal e_1269[4];
	doublereal fill_1270[4];
	doublereal e_1271[4];
	doublereal fill_1272[4];
	doublereal e_1273[4];
	doublereal fill_1274[4];
	doublereal e_1275[4];
	doublereal fill_1276[4];
	doublereal e_1277[4];
	doublereal fill_1278[4];
	doublereal e_1279[4];
	doublereal fill_1280[4];
	doublereal e_1281[4];
	doublereal fill_1282[4];
	doublereal e_1283[4];
	doublereal fill_1284[4];
	doublereal e_1285[4];
	doublereal fill_1286[4];
	doublereal e_1287[4];
	doublereal fill_1288[4];
	doublereal e_1289[4];
	doublereal fill_1290[4];
	doublereal e_1291[4];
	doublereal fill_1292[4];
	doublereal e_1293[4];
	doublereal fill_1294[4];
	doublereal e_1295[4];
	doublereal fill_1296[4];
	doublereal e_1297[4];
	doublereal fill_1298[4];
	doublereal e_1299[4];
	doublereal fill_1300[4];
	doublereal e_1301[4];
	doublereal fill_1302[4];
	doublereal e_1303[4];
	doublereal fill_1304[4];
	doublereal e_1305[4];
	doublereal fill_1306[4];
	doublereal e_1307[4];
	doublereal fill_1308[4];
	doublereal e_1309[4];
	doublereal fill_1310[4];
	doublereal e_1311[4];
	doublereal fill_1312[4];
	doublereal e_1313[4];
	doublereal fill_1314[4];
	doublereal e_1315[4];
	doublereal fill_1316[4];
	doublereal e_1317[4];
	doublereal fill_1318[4];
	doublereal e_1319[4];
	doublereal fill_1320[4];
	doublereal e_1321[4];
	doublereal fill_1322[4];
	doublereal e_1323[4];
	doublereal fill_1324[4];
	doublereal e_1325[4];
	doublereal fill_1326[4];
	doublereal e_1327[4];
	doublereal fill_1328[4];
	doublereal e_1329[4];
	doublereal fill_1330[4];
	doublereal e_1331[4];
	doublereal fill_1332[4];
	doublereal e_1333[4];
	doublereal fill_1334[4];
	doublereal e_1335[4];
	doublereal fill_1336[4];
	doublereal e_1337[4];
	doublereal fill_1338[4];
	doublereal e_1339[4];
	doublereal fill_1340[4];
	doublereal e_1341[4];
	doublereal fill_1342[4];
	doublereal e_1343[4];
	doublereal fill_1344[4];
	doublereal e_1345[4];
	doublereal fill_1346[4];
	doublereal e_1347[4];
	doublereal fill_1348[4];
	doublereal e_1349[4];
	doublereal fill_1350[4];
	doublereal e_1351[4];
	doublereal fill_1352[4];
	doublereal e_1353[4];
	doublereal fill_1354[4];
	doublereal e_1355[4];
	doublereal fill_1356[4];
	doublereal e_1357[4];
	doublereal fill_1358[4];
	doublereal e_1359[4];
	doublereal fill_1360[4];
	doublereal e_1361[4];
	doublereal fill_1362[4];
	doublereal e_1363[4];
	doublereal fill_1364[4];
	doublereal e_1365[4];
	doublereal fill_1366[4];
	doublereal e_1367[4];
	doublereal fill_1368[4];
	doublereal e_1369[4];
	doublereal fill_1370[4];
	doublereal e_1371[4];
	doublereal fill_1372[4];
	doublereal e_1373[4];
	doublereal fill_1374[4];
	doublereal e_1375[4];
	doublereal fill_1376[4];
	doublereal e_1377[4];
	doublereal fill_1378[4];
	doublereal e_1379[4];
	doublereal fill_1380[4];
	doublereal e_1381[4];
	doublereal fill_1382[4];
	doublereal e_1383[4];
	doublereal fill_1384[4];
	doublereal e_1385[4];
	doublereal fill_1386[4];
	doublereal e_1387[4];
	doublereal fill_1388[4];
	doublereal e_1389[4];
	doublereal fill_1390[4];
	doublereal e_1391[4];
	doublereal fill_1392[4];
	doublereal e_1393[4];
	doublereal fill_1394[4];
	doublereal e_1395[4];
	doublereal fill_1396[4];
	doublereal e_1397[4];
	doublereal fill_1398[4];
	doublereal e_1399[4];
	doublereal fill_1400[4];
	doublereal e_1401[4];
	doublereal fill_1402[4];
	doublereal e_1403[4];
	doublereal fill_1404[4];
	doublereal e_1405[4];
	doublereal fill_1406[4];
	doublereal e_1407[4];
	doublereal fill_1408[4];
	doublereal e_1409[4];
	doublereal fill_1410[4];
	doublereal e_1411[4];
	doublereal fill_1412[4];
	doublereal e_1413[4];
	doublereal fill_1414[4];
	doublereal e_1415[4];
	doublereal fill_1416[4];
	doublereal e_1417[4];
	doublereal fill_1418[4];
	doublereal e_1419[4];
	doublereal fill_1420[4];
	doublereal e_1421[4];
	doublereal fill_1422[4];
	doublereal e_1423[4];
	doublereal fill_1424[4];
	doublereal e_1425[4];
	doublereal fill_1426[4];
	doublereal e_1427[4];
	doublereal fill_1428[4];
	doublereal e_1429[4];
	doublereal fill_1430[4];
	doublereal e_1431[4];
	doublereal fill_1432[4];
	doublereal e_1433[4];
	doublereal fill_1434[4];
	doublereal e_1435[4];
	doublereal fill_1436[4];
	doublereal e_1437[4];
	doublereal fill_1438[4];
	doublereal e_1439[4];
	doublereal fill_1440[4];
	doublereal e_1441[4];
	doublereal fill_1442[4];
	doublereal e_1443[4];
	doublereal fill_1444[4];
	doublereal e_1445[4];
	doublereal fill_1446[4];
	doublereal e_1447[4];
	doublereal fill_1448[4];
	doublereal e_1449[4];
	doublereal fill_1450[4];
	doublereal e_1451[4];
	doublereal fill_1452[4];
	doublereal e_1453[4];
	doublereal fill_1454[4];
	doublereal e_1455[4];
	doublereal fill_1456[4];
	doublereal e_1457[4];
	doublereal fill_1458[4];
	doublereal e_1459[4];
	doublereal fill_1460[4];
	doublereal e_1461[4];
	doublereal fill_1462[4];
	doublereal e_1463[4];
	doublereal fill_1464[4];
	doublereal e_1465[4];
	doublereal fill_1466[4];
	doublereal e_1467[4];
	doublereal fill_1468[4];
	doublereal e_1469[4];
	doublereal fill_1470[4];
	doublereal e_1471[4];
	doublereal fill_1472[4];
	doublereal e_1473[4];
	doublereal fill_1474[4];
	doublereal e_1475[4];
	doublereal fill_1476[4];
	doublereal e_1477[4];
	doublereal fill_1478[4];
	doublereal e_1479[4];
	doublereal fill_1480[4];
	doublereal e_1481[4];
	doublereal fill_1482[4];
	doublereal e_1483[4];
	doublereal fill_1484[4];
	doublereal e_1485[4];
	doublereal fill_1486[4];
	doublereal e_1487[4];
	doublereal fill_1488[4];
	doublereal e_1489[4];
	doublereal fill_1490[4];
	doublereal e_1491[4];
	doublereal fill_1492[4];
	doublereal e_1493[4];
	doublereal fill_1494[4];
	doublereal e_1495[4];
	doublereal fill_1496[4];
	doublereal e_1497[4];
	doublereal fill_1498[4];
	doublereal e_1499[4];
	doublereal fill_1500[4];
	doublereal e_1501[4];
	doublereal fill_1502[4];
	doublereal e_1503[4];
	doublereal fill_1504[4];
	doublereal e_1505[4];
	doublereal fill_1506[4];
	doublereal e_1507[4];
	doublereal fill_1508[4];
	doublereal e_1509[4];
	doublereal fill_1510[4];
	doublereal e_1511[4];
	doublereal fill_1512[4];
	doublereal e_1513[4];
	doublereal fill_1514[4];
	doublereal e_1515[4];
	doublereal fill_1516[4];
	doublereal e_1517[4];
	doublereal fill_1518[4];
	doublereal e_1519[4];
	doublereal fill_1520[4];
	doublereal e_1521[4];
	doublereal fill_1522[4];
	doublereal e_1523[4];
	doublereal fill_1524[4];
	doublereal e_1525[4];
	doublereal fill_1526[4];
	doublereal e_1527[4];
	doublereal fill_1528[4];
	doublereal e_1529[4];
	doublereal fill_1530[4];
	doublereal e_1531[4];
	doublereal fill_1532[4];
	doublereal e_1533[4];
	doublereal fill_1534[4];
	doublereal e_1535[4];
	doublereal fill_1536[4];
	doublereal e_1537[4];
	doublereal fill_1538[4];
	doublereal e_1539[4];
	doublereal fill_1540[4];
	doublereal e_1541[4];
	doublereal fill_1542[4];
	doublereal e_1543[4];
	doublereal fill_1544[4];
	doublereal e_1545[4];
	doublereal fill_1546[4];
	doublereal e_1547[4];
	doublereal fill_1548[4];
	doublereal e_1549[4];
	doublereal fill_1550[4];
	doublereal e_1551[4];
	doublereal fill_1552[4];
	doublereal e_1553[4];
	doublereal fill_1554[4];
	doublereal e_1555[4];
	doublereal fill_1556[4];
	doublereal e_1557[4];
	doublereal fill_1558[4];
	doublereal e_1559[4];
	doublereal fill_1560[4];
	doublereal e_1561[4];
	doublereal fill_1562[4];
	doublereal e_1563[4];
	doublereal fill_1564[4];
	doublereal e_1565[4];
	doublereal fill_1566[4];
	doublereal e_1567[4];
	doublereal fill_1568[4];
	doublereal e_1569[4];
	doublereal fill_1570[4];
	doublereal e_1571[4];
	doublereal fill_1572[4];
	doublereal e_1573[4];
	doublereal fill_1574[4];
	doublereal e_1575[4];
	doublereal fill_1576[4];
	doublereal e_1577[4];
	doublereal fill_1578[4];
	doublereal e_1579[4];
	doublereal fill_1580[4];
	doublereal e_1581[4];
	doublereal fill_1582[4];
	doublereal e_1583[4];
	doublereal fill_1584[4];
	doublereal e_1585[4];
	doublereal fill_1586[4];
	doublereal e_1587[4];
	doublereal fill_1588[4];
	doublereal e_1589[4];
	doublereal fill_1590[4];
	doublereal e_1591[4];
	doublereal fill_1592[4];
	doublereal e_1593[4];
	doublereal fill_1594[4];
	doublereal e_1595[4];
	doublereal fill_1596[4];
	doublereal e_1597[4];
	doublereal fill_1598[4];
	doublereal e_1599[4];
	doublereal fill_1600[4];
	doublereal e_1601[4];
	doublereal fill_1602[4];
	doublereal e_1603[4];
	doublereal fill_1604[4];
	doublereal e_1605[4];
	doublereal fill_1606[4];
	doublereal e_1607[4];
	doublereal fill_1608[4];
	doublereal e_1609[4];
	doublereal fill_1610[4];
	doublereal e_1611[4];
	doublereal fill_1612[4];
	doublereal e_1613[4];
	doublereal fill_1614[4];
	doublereal e_1615[4];
	doublereal fill_1616[4];
	doublereal e_1617[4];
	doublereal fill_1618[4];
	doublereal e_1619[4];
	doublereal fill_1620[4];
	doublereal e_1621[4];
	doublereal fill_1622[4];
	doublereal e_1623[4];
	doublereal fill_1624[4];
	doublereal e_1625[4];
	doublereal fill_1626[4];
	doublereal e_1627[4];
	doublereal fill_1628[4];
	doublereal e_1629[4];
	doublereal fill_1630[4];
	doublereal e_1631[4];
	doublereal fill_1632[4];
	doublereal e_1633[4];
	doublereal fill_1634[4];
	doublereal e_1635[4];
	doublereal fill_1636[4];
	doublereal e_1637[4];
	doublereal fill_1638[4];
	doublereal e_1639[4];
	doublereal fill_1640[4];
	doublereal e_1641[4];
	doublereal fill_1642[4];
	doublereal e_1643[4];
	doublereal fill_1644[4];
	doublereal e_1645[4];
	doublereal fill_1646[4];
	doublereal e_1647[4];
	doublereal fill_1648[4];
	doublereal e_1649[4];
	doublereal fill_1650[4];
	doublereal e_1651[4];
	doublereal fill_1652[4];
	doublereal e_1653[4];
	doublereal fill_1654[4];
	doublereal e_1655[4];
	doublereal fill_1656[4];
	doublereal e_1657[4];
	doublereal fill_1658[4];
	doublereal e_1659[4];
	doublereal fill_1660[4];
	doublereal e_1661[4];
	doublereal fill_1662[4];
	doublereal e_1663[4];
	doublereal fill_1664[4];
	doublereal e_1665[4];
	doublereal fill_1666[4];
	doublereal e_1667[4];
	doublereal fill_1668[4];
	doublereal e_1669[4];
	doublereal fill_1670[4];
	doublereal e_1671[4];
	doublereal fill_1672[4];
	doublereal e_1673[4];
	doublereal fill_1674[4];
	doublereal e_1675[4];
	doublereal fill_1676[4];
	doublereal e_1677[4];
	doublereal fill_1678[4];
	doublereal e_1679[4];
	doublereal fill_1680[4];
	doublereal e_1681[4];
	doublereal fill_1682[4];
	doublereal e_1683[4];
	doublereal fill_1684[4];
	doublereal e_1685[4];
	doublereal fill_1686[4];
	doublereal e_1687[4];
	doublereal fill_1688[4];
	doublereal e_1689[4];
	doublereal fill_1690[4];
	doublereal e_1691[4];
	doublereal fill_1692[4];
	doublereal e_1693[4];
	doublereal fill_1694[4];
	doublereal e_1695[4];
	doublereal fill_1696[4];
	doublereal e_1697[4];
	doublereal fill_1698[4];
	doublereal e_1699[4];
	doublereal fill_1700[4];
	doublereal e_1701[4];
	doublereal fill_1702[4];
	doublereal e_1703[4];
	doublereal fill_1704[4];
	doublereal e_1705[4];
	doublereal fill_1706[4];
	doublereal e_1707[4];
	doublereal fill_1708[4];
	doublereal e_1709[4];
	doublereal fill_1710[4];
	doublereal e_1711[4];
	doublereal fill_1712[4];
	doublereal e_1713[4];
	doublereal fill_1714[4];
	doublereal e_1715[4];
	doublereal fill_1716[4];
	doublereal e_1717[4];
	doublereal fill_1718[4];
	doublereal e_1719[4];
	doublereal fill_1720[4];
	doublereal e_1721[4];
	doublereal fill_1722[4];
	doublereal e_1723[4];
	doublereal fill_1724[4];
	doublereal e_1725[4];
	doublereal fill_1726[4];
	doublereal e_1727[4];
	doublereal fill_1728[4];
	doublereal e_1729[4];
	doublereal fill_1730[4];
	doublereal e_1731[4];
	doublereal fill_1732[4];
	doublereal e_1733[4];
	doublereal fill_1734[4];
	doublereal e_1735[4];
	doublereal fill_1736[4];
	doublereal e_1737[4];
	doublereal fill_1738[4];
	doublereal e_1739[4];
	doublereal fill_1740[4];
	doublereal e_1741[4];
	doublereal fill_1742[4];
	doublereal e_1743[4];
	doublereal fill_1744[4];
	doublereal e_1745[4];
	doublereal fill_1746[4];
	doublereal e_1747[4];
	doublereal fill_1748[4];
	doublereal e_1749[4];
	doublereal fill_1750[4];
	doublereal e_1751[4];
	doublereal fill_1752[4];
	doublereal e_1753[4];
	doublereal fill_1754[4];
	doublereal e_1755[4];
	doublereal fill_1756[4];
	doublereal e_1757[4];
	doublereal fill_1758[4];
	doublereal e_1759[4];
	doublereal fill_1760[4];
	doublereal e_1761[4];
	doublereal fill_1762[4];
	doublereal e_1763[4];
	doublereal fill_1764[4];
	doublereal e_1765[4];
	doublereal fill_1766[4];
	doublereal e_1767[4];
	doublereal fill_1768[4];
	doublereal e_1769[4];
	doublereal fill_1770[4];
	doublereal e_1771[4];
	doublereal fill_1772[4];
	doublereal e_1773[4];
	doublereal fill_1774[4];
	doublereal e_1775[4];
	doublereal fill_1776[4];
	doublereal e_1777[4];
	doublereal fill_1778[4];
	doublereal e_1779[4];
	doublereal fill_1780[4];
	doublereal e_1781[4];
	doublereal fill_1782[4];
	doublereal e_1783[4];
	doublereal fill_1784[4];
	doublereal e_1785[4];
	doublereal fill_1786[4];
	doublereal e_1787[4];
	doublereal fill_1788[4];
	doublereal e_1789[4];
	doublereal fill_1790[4];
	doublereal e_1791[4];
	doublereal fill_1792[4];
	doublereal e_1793[4];
	doublereal fill_1794[4];
	doublereal e_1795[4];
	doublereal fill_1796[4];
	doublereal e_1797[4];
	doublereal fill_1798[4];
	doublereal e_1799[4];
	doublereal fill_1800[4];
	doublereal e_1801[4];
	doublereal fill_1802[4];
	doublereal e_1803[4];
	doublereal fill_1804[4];
	doublereal e_1805[4];
	doublereal fill_1806[4];
	doublereal e_1807[4];
	doublereal fill_1808[4];
	doublereal e_1809[4];
	doublereal fill_1810[4];
	doublereal e_1811[4];
	doublereal fill_1812[4];
	doublereal e_1813[4];
	doublereal fill_1814[4];
	doublereal e_1815[4];
	doublereal fill_1816[4];
	doublereal e_1817[4];
	doublereal fill_1818[4];
	doublereal e_1819[4];
	doublereal fill_1820[4];
	doublereal e_1821[4];
	doublereal fill_1822[4];
	doublereal e_1823[4];
	doublereal fill_1824[4];
	doublereal e_1825[4];
	doublereal fill_1826[4];
	doublereal e_1827[4];
	doublereal fill_1828[4];
	doublereal e_1829[4];
	doublereal fill_1830[4];
	doublereal e_1831[4];
	doublereal fill_1832[4];
	doublereal e_1833[4];
	doublereal fill_1834[4];
	doublereal e_1835[4];
	doublereal fill_1836[4];
	doublereal e_1837[4];
	doublereal fill_1838[4];
	doublereal e_1839[4];
	doublereal fill_1840[4];
	doublereal e_1841[4];
	doublereal fill_1842[4];
	doublereal e_1843[4];
	doublereal fill_1844[4];
	doublereal e_1845[4];
	doublereal fill_1846[4];
	doublereal e_1847[4];
	doublereal fill_1848[4];
	doublereal e_1849[4];
	doublereal fill_1850[4];
	doublereal e_1851[4];
	doublereal fill_1852[4];
	doublereal e_1853[4];
	doublereal fill_1854[4];
	doublereal e_1855[4];
	doublereal fill_1856[4];
	doublereal e_1857[4];
	doublereal fill_1858[4];
	doublereal e_1859[4];
	doublereal fill_1860[4];
	doublereal e_1861[4];
	doublereal fill_1862[4];
	doublereal e_1863[4];
	doublereal fill_1864[4];
	doublereal e_1865[4];
	doublereal fill_1866[4];
	doublereal e_1867[4];
	doublereal fill_1868[4];
	doublereal e_1869[4];
	doublereal fill_1870[4];
	doublereal e_1871[4];
	doublereal fill_1872[4];
	doublereal e_1873[4];
	doublereal fill_1874[4];
	doublereal e_1875[4];
	doublereal fill_1876[4];
	doublereal e_1877[4];
	doublereal fill_1878[4];
	doublereal e_1879[4];
	doublereal fill_1880[4];
	doublereal e_1881[4];
	doublereal fill_1882[4];
	doublereal e_1883[4];
	doublereal fill_1884[4];
	doublereal e_1885[4];
	doublereal fill_1886[4];
	doublereal e_1887[4];
	doublereal fill_1888[4];
	doublereal e_1889[4];
	doublereal fill_1890[4];
	doublereal e_1891[4];
	doublereal fill_1892[4];
	doublereal e_1893[4];
	doublereal fill_1894[4];
	doublereal e_1895[4];
	doublereal fill_1896[4];
	doublereal e_1897[4];
	doublereal fill_1898[4];
	doublereal e_1899[4];
	doublereal fill_1900[4];
	doublereal e_1901[4];
	doublereal fill_1902[4];
	doublereal e_1903[4];
	doublereal fill_1904[4];
	doublereal e_1905[4];
	doublereal fill_1906[4];
	doublereal e_1907[4];
	doublereal fill_1908[4];
	doublereal e_1909[4];
	doublereal fill_1910[4];
	doublereal e_1911[4];
	doublereal fill_1912[4];
	doublereal e_1913[4];
	doublereal fill_1914[4];
	doublereal e_1915[4];
	doublereal fill_1916[4];
	doublereal e_1917[4];
	doublereal fill_1918[4];
	doublereal e_1919[4];
	doublereal fill_1920[4];
	doublereal e_1921[4];
	doublereal fill_1922[4];
	doublereal e_1923[4];
	doublereal fill_1924[4];
	doublereal e_1925[4];
	doublereal fill_1926[4];
	doublereal e_1927[4];
	doublereal fill_1928[4];
	doublereal e_1929[4];
	doublereal fill_1930[4];
	doublereal e_1931[4];
	doublereal fill_1932[4];
	doublereal e_1933[4];
	doublereal fill_1934[4];
	doublereal e_1935[4];
	doublereal fill_1936[4];
	doublereal e_1937[4];
	doublereal fill_1938[4];
	doublereal e_1939[4];
	doublereal fill_1940[4];
	doublereal e_1941[4];
	doublereal fill_1942[4];
	doublereal e_1943[4];
	doublereal fill_1944[4];
	doublereal e_1945[4];
	doublereal fill_1946[4];
	doublereal e_1947[4];
	doublereal fill_1948[4];
	doublereal e_1949[4];
	doublereal fill_1950[4];
	doublereal e_1951[4];
	doublereal fill_1952[4];
	doublereal e_1953[4];
	doublereal fill_1954[4];
	doublereal e_1955[4];
	doublereal fill_1956[4];
	doublereal e_1957[4];
	doublereal fill_1958[4];
	doublereal e_1959[4];
	doublereal fill_1960[4];
	doublereal e_1961[4];
	doublereal fill_1962[4];
	doublereal e_1963[4];
	doublereal fill_1964[4];
	doublereal e_1965[4];
	doublereal fill_1966[4];
	doublereal e_1967[4];
	doublereal fill_1968[4];
	doublereal e_1969[4];
	doublereal fill_1970[4];
	doublereal e_1971[4];
	doublereal fill_1972[4];
	doublereal e_1973[4];
	doublereal fill_1974[4];
	doublereal e_1975[4];
	doublereal fill_1976[4];
	doublereal e_1977[4];
	doublereal fill_1978[4];
	doublereal e_1979[4];
	doublereal fill_1980[4];
	doublereal e_1981[4];
	doublereal fill_1982[4];
	doublereal e_1983[4];
	doublereal fill_1984[4];
	doublereal e_1985[4];
	doublereal fill_1986[4];
	doublereal e_1987[4];
	doublereal fill_1988[4];
	doublereal e_1989[4];
	doublereal fill_1990[4];
	doublereal e_1991[4];
	doublereal fill_1992[4];
	doublereal e_1993[4];
	doublereal fill_1994[4];
	doublereal e_1995[4];
	doublereal fill_1996[4];
	doublereal e_1997[4];
	doublereal fill_1998[4];
	doublereal e_1999[4];
	doublereal fill_2000[4];
	doublereal e_2001[4];
	doublereal fill_2002[4];
	doublereal e_2003[4];
	doublereal fill_2004[4];
	doublereal e_2005[4];
	doublereal fill_2006[4];
	doublereal e_2007[4];
	doublereal fill_2008[4];
	doublereal e_2009[4];
	doublereal fill_2010[4];
	doublereal e_2011[4];
	doublereal fill_2012[4];
	doublereal e_2013[4];
	doublereal fill_2014[4];
	doublereal e_2015[4];
	doublereal fill_2016[4];
	doublereal e_2017[4];
	doublereal fill_2018[4];
	doublereal e_2019[4];
	doublereal fill_2020[4];
	doublereal e_2021[4];
	doublereal fill_2022[4];
	doublereal e_2023[4];
	doublereal fill_2024[4];
	doublereal e_2025[4];
	doublereal fill_2026[4];
	doublereal e_2027[4];
	doublereal fill_2028[4];
	doublereal e_2029[4];
	doublereal fill_2030[4];
	doublereal e_2031[4];
	doublereal fill_2032[4];
	doublereal e_2033[4];
	doublereal fill_2034[4];
	doublereal e_2035[4];
	doublereal fill_2036[4];
	doublereal e_2037[4];
	doublereal fill_2038[4];
	doublereal e_2039[4];
	doublereal fill_2040[4];
	doublereal e_2041[4];
	doublereal fill_2042[4];
	doublereal e_2043[4];
	doublereal fill_2044[4];
	doublereal e_2045[4];
	doublereal fill_2046[4];
	doublereal e_2047[4];
	doublereal fill_2048[4];
	doublereal e_2049[4];
	doublereal fill_2050[4];
	doublereal e_2051[4];
	doublereal fill_2052[4];
	doublereal e_2053[4];
	doublereal fill_2054[4];
	doublereal e_2055[4];
	doublereal fill_2056[4];
	doublereal e_2057[4];
	doublereal fill_2058[4];
	doublereal e_2059[4];
	doublereal fill_2060[4];
	doublereal e_2061[4];
	doublereal fill_2062[4];
	doublereal e_2063[4];
	doublereal fill_2064[4];
	doublereal e_2065[4];
	doublereal fill_2066[4];
	doublereal e_2067[4];
	doublereal fill_2068[4];
	doublereal e_2069[4];
	doublereal fill_2070[4];
	doublereal e_2071[4];
	doublereal fill_2072[4];
	doublereal e_2073[4];
	doublereal fill_2074[4];
	doublereal e_2075[4];
	doublereal fill_2076[4];
	doublereal e_2077[4];
	doublereal fill_2078[4];
	doublereal e_2079[4];
	doublereal fill_2080[4];
	doublereal e_2081[4];
	doublereal fill_2082[4];
	doublereal e_2083[4];
	doublereal fill_2084[4];
	doublereal e_2085[4];
	doublereal fill_2086[4];
	doublereal e_2087[4];
	doublereal fill_2088[4];
	doublereal e_2089[4];
	doublereal fill_2090[4];
	doublereal e_2091[4];
	doublereal fill_2092[4];
	doublereal e_2093[4];
	doublereal fill_2094[4];
	doublereal e_2095[4];
	doublereal fill_2096[4];
	doublereal e_2097[4];
	doublereal fill_2098[4];
	doublereal e_2099[4];
	doublereal fill_2100[4];
	doublereal e_2101[4];
	doublereal fill_2102[4];
	doublereal e_2103[4];
	doublereal fill_2104[4];
	doublereal e_2105[4];
	doublereal fill_2106[4];
	doublereal e_2107[4];
	doublereal fill_2108[4];
	doublereal e_2109[4];
	doublereal fill_2110[4];
	doublereal e_2111[4];
	doublereal fill_2112[4];
	doublereal e_2113[4];
	doublereal fill_2114[4];
	doublereal e_2115[4];
	doublereal fill_2116[4];
	doublereal e_2117[4];
	doublereal fill_2118[4];
	doublereal e_2119[4];
	doublereal fill_2120[4];
	doublereal e_2121[4];
	doublereal fill_2122[4];
	doublereal e_2123[4];
	doublereal fill_2124[4];
	doublereal e_2125[4];
	doublereal fill_2126[4];
	doublereal e_2127[4];
	doublereal fill_2128[4];
	doublereal e_2129[4];
	doublereal fill_2130[4];
	doublereal e_2131[4];
	doublereal fill_2132[4];
	doublereal e_2133[4];
	doublereal fill_2134[4];
	doublereal e_2135[4];
	doublereal fill_2136[4];
	doublereal e_2137[4];
	doublereal fill_2138[4];
	doublereal e_2139[4];
	doublereal fill_2140[4];
	doublereal e_2141[4];
	doublereal fill_2142[4];
	doublereal e_2143[4];
	doublereal fill_2144[4];
	doublereal e_2145[4];
	doublereal fill_2146[4];
	doublereal e_2147[4];
	doublereal fill_2148[4];
	doublereal e_2149[4];
	doublereal fill_2150[4];
	doublereal e_2151[4];
	doublereal fill_2152[4];
	doublereal e_2153[4];
	doublereal fill_2154[4];
	doublereal e_2155[4];
	doublereal fill_2156[4];
	doublereal e_2157[4];
	doublereal fill_2158[4];
	doublereal e_2159[4];
	doublereal fill_2160[4];
	doublereal e_2161[4];
	doublereal fill_2162[4];
	doublereal e_2163[4];
	doublereal fill_2164[4];
	doublereal e_2165[4];
	doublereal fill_2166[4];
	doublereal e_2167[4];
	doublereal fill_2168[4];
	doublereal e_2169[4];
	doublereal fill_2170[4];
	doublereal e_2171[4];
	doublereal fill_2172[4];
	doublereal e_2173[4];
	doublereal fill_2174[4];
	doublereal e_2175[4];
	doublereal fill_2176[4];
	doublereal e_2177[4];
	doublereal fill_2178[4];
	doublereal e_2179[4];
	doublereal fill_2180[4];
	doublereal e_2181[4];
	doublereal fill_2182[4];
	doublereal e_2183[4];
	doublereal fill_2184[4];
	doublereal e_2185[4];
	doublereal fill_2186[4];
	doublereal e_2187[4];
	doublereal fill_2188[4];
	doublereal e_2189[4];
	doublereal fill_2190[4];
	doublereal e_2191[4];
	doublereal fill_2192[4];
	doublereal e_2193[4];
	doublereal fill_2194[4];
	doublereal e_2195[4];
	doublereal fill_2196[4];
	doublereal e_2197[4];
	doublereal fill_2198[4];
	doublereal e_2199[4];
	doublereal fill_2200[4];
	doublereal e_2201[4];
	doublereal fill_2202[4];
	doublereal e_2203[4];
	doublereal fill_2204[4];
	doublereal e_2205[4];
	doublereal fill_2206[4];
	doublereal e_2207[4];
	doublereal fill_2208[4];
	doublereal e_2209[4];
	doublereal fill_2210[4];
	doublereal e_2211[4];
	doublereal fill_2212[4];
	doublereal e_2213[4];
	doublereal fill_2214[4];
	doublereal e_2215[4];
	doublereal fill_2216[4];
	doublereal e_2217[4];
	doublereal fill_2218[4];
	doublereal e_2219[4];
	doublereal fill_2220[4];
	doublereal e_2221[4];
	doublereal fill_2222[4];
	doublereal e_2223[4];
	doublereal fill_2224[4];
	doublereal e_2225[4];
	doublereal fill_2226[4];
	doublereal e_2227[4];
	doublereal fill_2228[4];
	doublereal e_2229[4];
	doublereal fill_2230[4];
	doublereal e_2231[4];
	doublereal fill_2232[4];
	doublereal e_2233[4];
	doublereal fill_2234[4];
	doublereal e_2235[4];
	doublereal fill_2236[4];
	doublereal e_2237[4];
	doublereal fill_2238[4];
	doublereal e_2239[4];
	doublereal fill_2240[4];
	doublereal e_2241[4];
	doublereal fill_2242[4];
	doublereal e_2243[4];
	doublereal fill_2244[4];
	doublereal e_2245[4];
	doublereal fill_2246[4];
	doublereal e_2247[4];
	doublereal fill_2248[4];
	doublereal e_2249[4];
	doublereal fill_2250[4];
	doublereal e_2251[4];
	doublereal fill_2252[4];
	doublereal e_2253[4];
	doublereal fill_2254[4];
	doublereal e_2255[4];
	doublereal fill_2256[4];
	doublereal e_2257[4];
	doublereal fill_2258[4];
	doublereal e_2259[4];
	doublereal fill_2260[4];
	doublereal e_2261[4];
	doublereal fill_2262[4];
	doublereal e_2263[4];
	doublereal fill_2264[4];
	doublereal e_2265[4];
	doublereal fill_2266[4];
	doublereal e_2267[4];
	doublereal fill_2268[4];
	doublereal e_2269[4];
	doublereal fill_2270[4];
	doublereal e_2271[4];
	doublereal fill_2272[4];
	doublereal e_2273[4];
	doublereal fill_2274[4];
	doublereal e_2275[4];
	doublereal fill_2276[4];
	doublereal e_2277[4];
	doublereal fill_2278[4];
	doublereal e_2279[4];
	doublereal fill_2280[4];
	doublereal e_2281[4];
	doublereal fill_2282[4];
	doublereal e_2283[4];
	doublereal fill_2284[4];
	doublereal e_2285[4];
	doublereal fill_2286[4];
	doublereal e_2287[4];
	doublereal fill_2288[4];
	doublereal e_2289[4];
	doublereal fill_2290[4];
	doublereal e_2291[4];
	doublereal fill_2292[4];
	doublereal e_2293[4];
	doublereal fill_2294[4];
	doublereal e_2295[4];
	doublereal fill_2296[4];
	doublereal e_2297[4];
	doublereal fill_2298[4];
	doublereal e_2299[4];
	doublereal fill_2300[4];
	doublereal e_2301[4];
	doublereal fill_2302[4];
	doublereal e_2303[4];
	doublereal fill_2304[4];
	doublereal e_2305[4];
	doublereal fill_2306[4];
	doublereal e_2307[4];
	doublereal fill_2308[4];
	doublereal e_2309[4];
	doublereal fill_2310[4];
	doublereal e_2311[4];
	doublereal fill_2312[4];
	doublereal e_2313[4];
	doublereal fill_2314[4];
	doublereal e_2315[4];
	doublereal fill_2316[4];
	doublereal e_2317[4];
	doublereal fill_2318[4];
	doublereal e_2319[4];
	doublereal fill_2320[4];
	doublereal e_2321[4];
	doublereal fill_2322[4];
	doublereal e_2323[4];
	doublereal fill_2324[4];
	doublereal e_2325[4];
	doublereal fill_2326[4];
	doublereal e_2327[4];
	doublereal fill_2328[4];
	doublereal e_2329[4];
	doublereal fill_2330[4];
	doublereal e_2331[4];
	doublereal fill_2332[4];
	doublereal e_2333[4];
	doublereal fill_2334[4];
	doublereal e_2335[4];
	doublereal fill_2336[4];
	doublereal e_2337[4];
	doublereal fill_2338[4];
	doublereal e_2339[4];
	doublereal fill_2340[4];
	doublereal e_2341[4];
	doublereal fill_2342[4];
	doublereal e_2343[4];
	doublereal fill_2344[4];
	doublereal e_2345[4];
	doublereal fill_2346[4];
	doublereal e_2347[4];
	doublereal fill_2348[4];
	doublereal e_2349[4];
	doublereal fill_2350[4];
	doublereal e_2351[4];
	doublereal fill_2352[4];
	doublereal e_2353[4];
	doublereal fill_2354[4];
	doublereal e_2355[4];
	doublereal fill_2356[4];
	doublereal e_2357[4];
	doublereal fill_2358[4];
	doublereal e_2359[4];
	doublereal fill_2360[4];
	doublereal e_2361[4];
	doublereal fill_2362[4];
	doublereal e_2363[4];
	doublereal fill_2364[4];
	doublereal e_2365[4];
	doublereal fill_2366[4];
	doublereal e_2367[4];
	doublereal fill_2368[4];
	doublereal e_2369[4];
	doublereal fill_2370[4];
	doublereal e_2371[4];
	doublereal fill_2372[4];
	doublereal e_2373[4];
	doublereal fill_2374[4];
	doublereal e_2375[4];
	doublereal fill_2376[4];
	doublereal e_2377[4];
	doublereal fill_2378[4];
	doublereal e_2379[4];
	doublereal fill_2380[4];
	doublereal e_2381[4];
	doublereal fill_2382[4];
	doublereal e_2383[4];
	doublereal fill_2384[4];
	doublereal e_2385[4];
	doublereal fill_2386[4];
	doublereal e_2387[4];
	doublereal fill_2388[4];
	doublereal e_2389[4];
	doublereal fill_2390[4];
	doublereal e_2391[4];
	doublereal fill_2392[4];
	doublereal e_2393[4];
	doublereal fill_2394[4];
	doublereal e_2395[4];
	doublereal fill_2396[4];
	doublereal e_2397[4];
	doublereal fill_2398[4];
	doublereal e_2399[4];
	doublereal fill_2400[4];
	doublereal e_2401[4];
	doublereal fill_2402[4];
	doublereal e_2403[4];
	doublereal fill_2404[4];
	doublereal e_2405[4];
	doublereal fill_2406[4];
	doublereal e_2407[4];
	doublereal fill_2408[4];
	doublereal e_2409[4];
	doublereal fill_2410[4];
	doublereal e_2411[4];
	doublereal fill_2412[4];
	doublereal e_2413[4];
	doublereal fill_2414[4];
	doublereal e_2415[4];
	doublereal fill_2416[4];
	doublereal e_2417[4];
	doublereal fill_2418[4];
	doublereal e_2419[4];
	doublereal fill_2420[4];
	doublereal e_2421[4];
	doublereal fill_2422[4];
	doublereal e_2423[4];
	doublereal fill_2424[4];
	doublereal e_2425[4];
	doublereal fill_2426[4];
	doublereal e_2427[4];
	doublereal fill_2428[4];
	doublereal e_2429[4];
	doublereal fill_2430[4];
	doublereal e_2431[4];
	doublereal fill_2432[4];
	doublereal e_2433[4];
	doublereal fill_2434[4];
	doublereal e_2435[4];
	doublereal fill_2436[4];
	doublereal e_2437[4];
	doublereal fill_2438[4];
	doublereal e_2439[4];
	doublereal fill_2440[4];
	doublereal e_2441[4];
	doublereal fill_2442[4];
	doublereal e_2443[4];
	doublereal fill_2444[4];
	doublereal e_2445[4];
	doublereal fill_2446[4];
	doublereal e_2447[4];
	doublereal fill_2448[4];
	doublereal e_2449[4];
	doublereal fill_2450[4];
	doublereal e_2451[4];
	doublereal fill_2452[4];
	doublereal e_2453[4];
	doublereal fill_2454[4];
	doublereal e_2455[4];
	doublereal fill_2456[4];
	doublereal e_2457[4];
	doublereal fill_2458[4];
	doublereal e_2459[4];
	doublereal fill_2460[4];
	doublereal e_2461[4];
	doublereal fill_2462[4];
	doublereal e_2463[4];
	doublereal fill_2464[4];
	doublereal e_2465[4];
	doublereal fill_2466[4];
	doublereal e_2467[4];
	doublereal fill_2468[4];
	doublereal e_2469[4];
	doublereal fill_2470[4];
	doublereal e_2471[4];
	doublereal fill_2472[4];
	doublereal e_2473[4];
	doublereal fill_2474[4];
	doublereal e_2475[4];
	doublereal fill_2476[4];
	doublereal e_2477[4];
	doublereal fill_2478[4];
	doublereal e_2479[4];
	doublereal fill_2480[4];
	doublereal e_2481[4];
	doublereal fill_2482[4];
	doublereal e_2483[4];
	doublereal fill_2484[4];
	doublereal e_2485[4];
	doublereal fill_2486[4];
	doublereal e_2487[4];
	doublereal fill_2488[4];
	doublereal e_2489[4];
	doublereal fill_2490[4];
	doublereal e_2491[4];
	doublereal fill_2492[4];
	doublereal e_2493[4];
	doublereal fill_2494[4];
	doublereal e_2495[4];
	doublereal fill_2496[4];
	doublereal e_2497[4];
	doublereal fill_2498[4];
	doublereal e_2499[4];
	doublereal fill_2500[4];
	doublereal e_2501[4];
	doublereal fill_2502[4];
	doublereal e_2503[4];
	doublereal fill_2504[4];
	doublereal e_2505[4];
	doublereal fill_2506[4];
	doublereal e_2507[4];
	doublereal fill_2508[4];
	doublereal e_2509[4];
	doublereal fill_2510[4];
	doublereal e_2511[4];
	doublereal fill_2512[4];
	doublereal e_2513[4];
	doublereal fill_2514[4];
	doublereal e_2515[4];
	doublereal fill_2516[4];
	doublereal e_2517[4];
	doublereal fill_2518[4];
	doublereal e_2519[4];
	doublereal fill_2520[4];
	doublereal e_2521[4];
	doublereal fill_2522[4];
	doublereal e_2523[4];
	doublereal fill_2524[4];
	doublereal e_2525[4];
	doublereal fill_2526[4];
	doublereal e_2527[4];
	doublereal fill_2528[4];
	doublereal e_2529[4];
	doublereal fill_2530[4];
	doublereal e_2531[4];
	doublereal fill_2532[4];
	doublereal e_2533[4];
	doublereal fill_2534[4];
	doublereal e_2535[4];
	doublereal fill_2536[4];
	doublereal e_2537[4];
	doublereal fill_2538[4];
	doublereal e_2539[4];
	doublereal fill_2540[4];
	doublereal e_2541[4];
	doublereal fill_2542[4];
	doublereal e_2543[4];
	doublereal fill_2544[4];
	doublereal e_2545[4];
	doublereal fill_2546[4];
	doublereal e_2547[4];
	doublereal fill_2548[4];
	doublereal e_2549[4];
	doublereal fill_2550[4];
	doublereal e_2551[4];
	doublereal fill_2552[4];
	doublereal e_2553[4];
	doublereal fill_2554[4];
	doublereal e_2555[4];
	doublereal fill_2556[4];
	doublereal e_2557[4];
	doublereal fill_2558[4];
	doublereal e_2559[4];
	doublereal fill_2560[4];
	doublereal e_2561[4];
	doublereal fill_2562[4];
	doublereal e_2563[4];
	doublereal fill_2564[4];
	doublereal e_2565[4];
	doublereal fill_2566[4];
	doublereal e_2567[4];
	doublereal fill_2568[4];
	doublereal e_2569[4];
	doublereal fill_2570[4];
	doublereal e_2571[4];
	doublereal fill_2572[4];
	doublereal e_2573[4];
	doublereal fill_2574[4];
	doublereal e_2575[4];
	doublereal fill_2576[4];
	doublereal e_2577[4];
	doublereal fill_2578[4];
	doublereal e_2579[4];
	doublereal fill_2580[4];
	doublereal e_2581[4];
	doublereal fill_2582[4];
	doublereal e_2583[4];
	doublereal fill_2584[4];
	doublereal e_2585[4];
	doublereal fill_2586[4];
	doublereal e_2587[4];
	doublereal fill_2588[4];
	doublereal e_2589[4];
	doublereal fill_2590[4];
	doublereal e_2591[4];
	doublereal fill_2592[4];
	doublereal e_2593[4];
	doublereal fill_2594[4];
	doublereal e_2595[4];
	doublereal fill_2596[4];
	doublereal e_2597[4];
	doublereal fill_2598[4];
	doublereal e_2599[4];
	doublereal fill_2600[4];
	doublereal e_2601[4];
	doublereal fill_2602[4];
	doublereal e_2603[4];
	doublereal fill_2604[4];
	doublereal e_2605[4];
	doublereal fill_2606[4];
	doublereal e_2607[4];
	doublereal fill_2608[4];
	doublereal e_2609[4];
	doublereal fill_2610[4];
	doublereal e_2611[4];
	doublereal fill_2612[4];
	doublereal e_2613[4];
	doublereal fill_2614[4];
	doublereal e_2615[4];
	doublereal fill_2616[4];
	doublereal e_2617[4];
	doublereal fill_2618[4];
	doublereal e_2619[4];
	doublereal fill_2620[4];
	doublereal e_2621[4];
	doublereal fill_2622[4];
	doublereal e_2623[4];
	doublereal fill_2624[4];
	doublereal e_2625[4];
	doublereal fill_2626[4];
	doublereal e_2627[4];
	doublereal fill_2628[4];
	doublereal e_2629[4];
	doublereal fill_2630[4];
	doublereal e_2631[4];
	doublereal fill_2632[4];
	doublereal e_2633[4];
	doublereal fill_2634[4];
	doublereal e_2635[4];
	doublereal fill_2636[4];
	doublereal e_2637[4];
	doublereal fill_2638[4];
	doublereal e_2639[4];
	doublereal fill_2640[4];
	doublereal e_2641[4];
	doublereal fill_2642[4];
	doublereal e_2643[4];
	doublereal fill_2644[4];
	doublereal e_2645[4];
	doublereal fill_2646[4];
	doublereal e_2647[4];
	doublereal fill_2648[4];
	doublereal e_2649[4];
	doublereal fill_2650[4];
	doublereal e_2651[4];
	doublereal fill_2652[4];
	doublereal e_2653[4];
	doublereal fill_2654[4];
	doublereal e_2655[4];
	doublereal fill_2656[4];
	doublereal e_2657[4];
	doublereal fill_2658[4];
	doublereal e_2659[4];
	doublereal fill_2660[4];
	doublereal e_2661[4];
	doublereal fill_2662[4];
	doublereal e_2663[4];
	doublereal fill_2664[4];
	doublereal e_2665[4];
	doublereal fill_2666[4];
	doublereal e_2667[4];
	doublereal fill_2668[4];
	doublereal e_2669[4];
	doublereal fill_2670[4];
	doublereal e_2671[4];
	doublereal fill_2672[4];
	doublereal e_2673[4];
	doublereal fill_2674[4];
	doublereal e_2675[4];
	doublereal fill_2676[4];
	doublereal e_2677[4];
	doublereal fill_2678[4];
	doublereal e_2679[4];
	doublereal fill_2680[4];
	doublereal e_2681[4];
	doublereal fill_2682[4];
	doublereal e_2683[4];
	doublereal fill_2684[4];
	doublereal e_2685[4];
	doublereal fill_2686[4];
	doublereal e_2687[4];
	doublereal fill_2688[4];
	doublereal e_2689[4];
	doublereal fill_2690[4];
	doublereal e_2691[4];
	doublereal fill_2692[4];
	doublereal e_2693[4];
	doublereal fill_2694[4];
	doublereal e_2695[4];
	doublereal fill_2696[4];
	doublereal e_2697[4];
	doublereal fill_2698[4];
	doublereal e_2699[4];
	doublereal fill_2700[4];
	doublereal e_2701[4];
	doublereal fill_2702[4];
	doublereal e_2703[4];
	doublereal fill_2704[4];
	doublereal e_2705[4];
	doublereal fill_2706[4];
	doublereal e_2707[4];
	doublereal fill_2708[4];
	doublereal e_2709[4];
	doublereal fill_2710[4];
	doublereal e_2711[4];
	doublereal fill_2712[4];
	doublereal e_2713[4];
	doublereal fill_2714[4];
	doublereal e_2715[4];
	doublereal fill_2716[4];
	doublereal e_2717[4];
	doublereal fill_2718[4];
	doublereal e_2719[4];
	doublereal fill_2720[4];
	doublereal e_2721[4];
	doublereal fill_2722[4];
	doublereal e_2723[4];
	doublereal fill_2724[4];
	doublereal e_2725[4];
	doublereal fill_2726[4];
	doublereal e_2727[4];
	doublereal fill_2728[4];
	doublereal e_2729[4];
	doublereal fill_2730[4];
	doublereal e_2731[4];
	doublereal fill_2732[4];
	doublereal e_2733[4];
	doublereal fill_2734[4];
	doublereal e_2735[4];
	doublereal fill_2736[4];
	doublereal e_2737[4];
	doublereal fill_2738[4];
	doublereal e_2739[4];
	doublereal fill_2740[4];
	doublereal e_2741[4];
	doublereal fill_2742[4];
	doublereal e_2743[4];
	doublereal fill_2744[4];
	doublereal e_2745[4];
	doublereal fill_2746[4];
	doublereal e_2747[4];
	doublereal fill_2748[4];
	doublereal e_2749[4];
	doublereal fill_2750[4];
	doublereal e_2751[4];
	doublereal fill_2752[4];
	doublereal e_2753[4];
	doublereal fill_2754[4];
	doublereal e_2755[4];
	doublereal fill_2756[4];
	doublereal e_2757[4];
	doublereal fill_2758[4];
	doublereal e_2759[4];
	doublereal fill_2760[4];
	doublereal e_2761[4];
	doublereal fill_2762[4];
	doublereal e_2763[4];
	doublereal fill_2764[4];
	doublereal e_2765[4];
	doublereal fill_2766[4];
	doublereal e_2767[4];
	doublereal fill_2768[4];
	doublereal e_2769[4];
	doublereal fill_2770[4];
	doublereal e_2771[4];
	doublereal fill_2772[4];
	doublereal e_2773[4];
	doublereal fill_2774[4];
	doublereal e_2775[4];
	doublereal fill_2776[4];
	doublereal e_2777[4];
	doublereal fill_2778[4];
	doublereal e_2779[4];
	doublereal fill_2780[4];
	doublereal e_2781[4];
	doublereal fill_2782[4];
	doublereal e_2783[4];
	doublereal fill_2784[4];
	doublereal e_2785[4];
	doublereal fill_2786[4];
	doublereal e_2787[4];
	doublereal fill_2788[4];
	doublereal e_2789[4];
	doublereal fill_2790[4];
	doublereal e_2791[4];
	doublereal fill_2792[4];
	doublereal e_2793[4];
	doublereal fill_2794[4];
	doublereal e_2795[4];
	doublereal fill_2796[4];
	doublereal e_2797[4];
	doublereal fill_2798[4];
	doublereal e_2799[4];
	doublereal fill_2800[4];
	doublereal e_2801[4];
	doublereal fill_2802[4];
	doublereal e_2803[4];
	doublereal fill_2804[4];
	doublereal e_2805[4];
	doublereal fill_2806[4];
	doublereal e_2807[4];
	doublereal fill_2808[4];
	doublereal e_2809[4];
	doublereal fill_2810[4];
	doublereal e_2811[4];
	doublereal fill_2812[4];
	doublereal e_2813[4];
	doublereal fill_2814[4];
	doublereal e_2815[4];
	doublereal fill_2816[4];
	doublereal e_2817[4];
	doublereal fill_2818[4];
	doublereal e_2819[4];
	doublereal fill_2820[4];
	doublereal e_2821[4];
	doublereal fill_2822[4];
	doublereal e_2823[4];
	doublereal fill_2824[4];
	doublereal e_2825[4];
	doublereal fill_2826[4];
	doublereal e_2827[4];
	doublereal fill_2828[4];
	doublereal e_2829[4];
	doublereal fill_2830[4];
	doublereal e_2831[4];
	doublereal fill_2832[4];
	doublereal e_2833[4];
	doublereal fill_2834[4];
	doublereal e_2835[4];
	doublereal fill_2836[4];
	doublereal e_2837[4];
	doublereal fill_2838[4];
	doublereal e_2839[4];
	doublereal fill_2840[4];
	doublereal e_2841[4];
	doublereal fill_2842[4];
	doublereal e_2843[4];
	doublereal fill_2844[4];
	doublereal e_2845[4];
	doublereal fill_2846[4];
	doublereal e_2847[4];
	doublereal fill_2848[4];
	doublereal e_2849[4];
	doublereal fill_2850[4];
	doublereal e_2851[4];
	doublereal fill_2852[4];
	doublereal e_2853[4];
	doublereal fill_2854[4];
	doublereal e_2855[4];
	doublereal fill_2856[4];
	doublereal e_2857[4];
	doublereal fill_2858[4];
	doublereal e_2859[4];
	doublereal fill_2860[4];
	doublereal e_2861[4];
	doublereal fill_2862[4];
	doublereal e_2863[4];
	doublereal fill_2864[4];
	doublereal e_2865[4];
	doublereal fill_2866[4];
	doublereal e_2867[4];
	doublereal fill_2868[4];
	doublereal e_2869[4];
	doublereal fill_2870[4];
	doublereal e_2871[4];
	doublereal fill_2872[4];
	doublereal e_2873[4];
	doublereal fill_2874[4];
	doublereal e_2875[4];
	doublereal fill_2876[4];
	doublereal e_2877[4];
	doublereal fill_2878[4];
	doublereal e_2879[4];
	doublereal fill_2880[4];
	doublereal e_2881[4];
	doublereal fill_2882[4];
	doublereal e_2883[4];
	doublereal fill_2884[4];
	doublereal e_2885[4];
	doublereal fill_2886[4];
	doublereal e_2887[4];
	doublereal fill_2888[4];
	doublereal e_2889[4];
	doublereal fill_2890[4];
	doublereal e_2891[4];
	doublereal fill_2892[4];
	doublereal e_2893[4];
	doublereal fill_2894[4];
	doublereal e_2895[4];
	doublereal fill_2896[4];
	doublereal e_2897[4];
	doublereal fill_2898[4];
	doublereal e_2899[4];
	doublereal fill_2900[4];
	doublereal e_2901[4];
	doublereal fill_2902[4];
	doublereal e_2903[4];
	doublereal fill_2904[4];
	doublereal e_2905[4];
	doublereal fill_2906[4];
	doublereal e_2907[4];
	doublereal fill_2908[4];
	doublereal e_2909[4];
	doublereal fill_2910[4];
	doublereal e_2911[4];
	doublereal fill_2912[4];
	doublereal e_2913[4];
	doublereal fill_2914[4];
	doublereal e_2915[4];
	doublereal fill_2916[4];
	doublereal e_2917[4];
	doublereal fill_2918[4];
	doublereal e_2919[4];
	doublereal fill_2920[4];
	doublereal e_2921[4];
	doublereal fill_2922[4];
	doublereal e_2923[4];
	doublereal fill_2924[4];
	doublereal e_2925[4];
	doublereal fill_2926[4];
	doublereal e_2927[4];
	doublereal fill_2928[4];
	doublereal e_2929[4];
	doublereal fill_2930[4];
	doublereal e_2931[4];
	doublereal fill_2932[4];
	doublereal e_2933[4];
	doublereal fill_2934[4];
	doublereal e_2935[4];
	doublereal fill_2936[4];
	doublereal e_2937[4];
	doublereal fill_2938[4];
	doublereal e_2939[4];
	doublereal fill_2940[4];
	doublereal e_2941[4];
	doublereal fill_2942[4];
	doublereal e_2943[4];
	doublereal fill_2944[4];
	doublereal e_2945[4];
	doublereal fill_2946[4];
	doublereal e_2947[4];
	doublereal fill_2948[4];
	doublereal e_2949[4];
	doublereal fill_2950[4];
	doublereal e_2951[4];
	doublereal fill_2952[4];
	doublereal e_2953[4];
	doublereal fill_2954[4];
	doublereal e_2955[4];
	doublereal fill_2956[4];
	doublereal e_2957[4];
	doublereal fill_2958[4];
	doublereal e_2959[4];
	doublereal fill_2960[4];
	doublereal e_2961[4];
	doublereal fill_2962[4];
	doublereal e_2963[4];
	doublereal fill_2964[4];
	doublereal e_2965[4];
	doublereal fill_2966[4];
	doublereal e_2967[4];
	doublereal fill_2968[4];
	doublereal e_2969[4];
	doublereal fill_2970[4];
	doublereal e_2971[4];
	doublereal fill_2972[4];
	doublereal e_2973[4];
	doublereal fill_2974[4];
	doublereal e_2975[4];
	doublereal fill_2976[4];
	doublereal e_2977[4];
	doublereal fill_2978[4];
	doublereal e_2979[4];
	doublereal fill_2980[4];
	doublereal e_2981[4];
	doublereal fill_2982[4];
	doublereal e_2983[4];
	doublereal fill_2984[4];
	doublereal e_2985[4];
	doublereal fill_2986[4];
	doublereal e_2987[4];
	doublereal fill_2988[4];
	doublereal e_2989[4];
	doublereal fill_2990[4];
	doublereal e_2991[4];
	doublereal fill_2992[4];
	doublereal e_2993[4];
	doublereal fill_2994[4];
	doublereal e_2995[4];
	doublereal fill_2996[4];
	doublereal e_2997[4];
	doublereal fill_2998[4];
	doublereal e_2999[4];
	doublereal fill_3000[4];
	doublereal e_3001[4];
	doublereal fill_3002[4];
	} equiv_5 = { 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, .00114, 0., 0., 0., {0}, .0012, 0., 0., 0., {0}, 
		.00109, 0., 0., 0., {0}, .00373, 0., 0., 0., {0}, .00495, 0., 
		0., 0., {0}, .01321, 0., 0., 0., {0}, .03203, 0., 0., 0., {0},
		 .0819, 0., 0., 0., {0}, .25129, 0., 0., 0., {0}, .69749, 0., 
		0., 0., {0}, .97208, 0., 0., 0., {0}, 1., 0., 0., 0., {0}, 
		.98655, 0., 0., 0., {0}, .92234, 0., 0., 0., {0}, .78359, 0., 
		0., 0., {0}, .64632, 0., 0., 0., {0}, .55174, 0., 0., 0., {0},
		 .50241, 0., 0., 0., {0}, .41232, 0., 0., 0., {0}, .23254, 0.,
		 0., 0., {0}, .10744, 0., 0., 0., {0}, .05272, 0., 0., 0., {0}
		, .02769, 0., 0., 0., {0}, .01528, 0., 0., 0., {0}, .0097, 0.,
		 0., 0., {0}, .0054, 0., 0., 0., {0}, .00114, 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 7e-5, 0., 
		0., {0}, 0., .00106, 0., 0., {0}, 0., .00259, 0., 0., {0}, 0.,
		 .00588, 0., 0., {0}, 0., .01313, 0., 0., {0}, 0., .03452, 0.,
		 0., {0}, 0., .09925, 0., 0., {0}, 0., .25868, 0., 0., {0}, 
		0., .53662, 0., 0., {0}, 0., .75311, 0., 0., {0}, 0., .90226, 
		0., 0., {0}, 0., .99149, 0., 0., {0}, 0., 1., 0., 0., {0}, 0.,
		 .96028, 0., 0., {0}, 0., .9011, 0., 0., {0}, 0., .76907, 0., 
		0., {0}, 0., .47949, 0., 0., {0}, 0., .21557, 0., 0., {0}, 0.,
		 .08929, 0., 0., {0}, 0., .04225, 0., 0., {0}, 0., .02272, 0.,
		 0., {0}, 0., .01292, 0., 0., {0}, 0., .00778, 0., 0., {0}, 
		0., .00473, 0., 0., {0}, 0., .00264, 0., 0., {0}, 0., .00144, 
		0., 0., {0}, 0., 6e-4, 0., 0., {0}, 0., 3.1e-4, 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 5.9e-4, 0., {0}, 0., 0., .00104, 0., {0}, 0.,
		 0., .0021, 0., {0}, 0., 0., .00389, 0., {0}, 0., 0., .00839, 
		0., {0}, 0., 0., .01991, 0., {0}, 0., 0., .05815, 0., {0}, 0.,
		 0., .20311, 0., {0}, 0., 0., .59432, 0., {0}, 0., 0., .953, 
		0., {0}, 0., 0., .92949, 0., {0}, 0., 0., .87362, 0., {0}, 0.,
		 0., .91049, 0., {0}, 0., 0., 1., 0., {0}, 0., 0., .99161, 0.,
		 {0}, 0., 0., .79466, 0., {0}, 0., 0., .46286, 0., {0}, 0., 
		0., .20633, 0., {0}, 0., 0., .08993, 0., {0}, 0., 0., .04324, 
		0., {0}, 0., 0., .02196, 0., {0}, 0., 0., .01186, 0., {0}, 0.,
		 0., .00674, 0., {0}, 0., 0., .00372, 0., {0}, 0., 0., .00199,
		 0., {0}, 0., 0., 9.9e-4, 0., {0}, 0., 0., 3.5e-4, 0., {0}, 
		0., 0., 1e-5, 0., {0}, 0., 0., 1e-5, 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 1e-5, {0}, 0., 0., 0., 1e-5, {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 1e-5, {0}, 0., 0., 0., 3e-5, {0}, 0., 0., 0., 
		7e-5, {0}, 0., 0., 0., 1e-4, {0}, 0., 0., 0., 1.6e-4, {0}, 0.,
		 0., 0., 1.8e-4, {0}, 0., 0., 0., 1.9e-4, {0}, 0., 0., 0., 
		3.8e-4, {0}, 0., 0., 0., 7e-4, {0}, 0., 0., 0., 7.4e-4, {0}, 
		0., 0., 0., 8.7e-4, {0}, 0., 0., 0., 9.9e-4, {0}, 0., 0., 0., 
		.00112, {0}, 0., 0., 0., .00144, {0}, 0., 0., 0., .00178, {0},
		 0., 0., 0., .0021, {0}, 0., 0., 0., .00252, {0}, 0., 0., 0., 
		.00314, {0}, 0., 0., 0., .00381, {0}, 0., 0., 0., .00482, {0},
		 0., 0., 0., .00617, {0}, 0., 0., 0., .00808, {0}, 0., 0., 0.,
		 .01087, {0}, 0., 0., 0., .01507, {0}, 0., 0., 0., .02115, {0}
		, 0., 0., 0., .03014, {0}, 0., 0., 0., .04352, {0}, 0., 0., 
		0., .06372, {0}, 0., 0., 0., .09368, {0}, 0., 0., 0., .13303, 
		{0}, 0., 0., 0., .18172, {0}, 0., 0., 0., .239, {0}, 0., 0., 
		0., .29735, {0}, 0., 0., 0., .36285, {0}, 0., 0., 0., .43193, 
		{0}, 0., 0., 0., .50378, {0}, 0., 0., 0., .57403, {0}, 0., 0.,
		 0., .64865, {0}, 0., 0., 0., .72474, {0}, 0., 0., 0., .79732,
		 {0}, 0., 0., 0., .86795, {0}, 0., 0., 0., .92477, {0}, 0., 
		0., 0., .96695, {0}, 0., 0., 0., .99173, {0}, 0., 0., 0., 1., 
		{0}, 0., 0., 0., .99937, {0}, 0., 0., 0., .99592, {0}, 0., 0.,
		 0., .98624, {0}, 0., 0., 0., .97501, {0}, 0., 0., 0., .95695,
		 {0}, 0., 0., 0., .93594, {0}, 0., 0., 0., .90325, {0}, 0., 
		0., 0., .87127, {0}, 0., 0., 0., .83451, {0}, 0., 0., 0., 
		.79304, {0}, 0., 0., 0., .7532, {0}, 0., 0., 0., .70611, {0}, 
		0., 0., 0., .65958, {0}, 0., 0., 0., .60791, {0}, 0., 0., 0., 
		.55348, {0}, 0., 0., 0., .49819, {0}, 0., 0., 0., .43767, {0},
		 0., 0., 0., .37291, {0}, 0., 0., 0., .30937, {0}, 0., 0., 0.,
		 .2488, {0}, 0., 0., 0., .19559, {0}, 0., 0., 0., .14848, {0},
		 0., 0., 0., .10947, {0}, 0., 0., 0., .0794, {0}, 0., 0., 0., 
		.05786, {0}, 0., 0., 0., .04258, {0}, 0., 0., 0., .03172, {0},
		 0., 0., 0., .02439, {0}, 0., 0., 0., .01898, {0}, 0., 0., 0.,
		 .015, {0}, 0., 0., 0., .01187, {0}, 0., 0., 0., .00984, {0}, 
		0., 0., 0., .0081, {0}, 0., 0., 0., .00664, {0}, 0., 0., 0., 
		.00558, {0}, 0., 0., 0., .00481, {0}, 0., 0., 0., .00407, {0},
		 0., 0., 0., .00346, {0}, 0., 0., 0., .00293, {0}, 0., 0., 0.,
		 .0026, {0}, 0., 0., 0., .00218, {0}, 0., 0., 0., .0018, {0}, 
		0., 0., 0., .00167, {0}, 0., 0., 0., .00149, {0}, 0., 0., 0., 
		.00132, {0}, 0., 0., 0., .00127, {0}, 0., 0., 0., .00118, {0},
		 0., 0., 0., 8.3e-4, {0}, 0., 0., 0., 5.6e-4, {0}, 0., 0., 0.,
		 4.9e-4, {0}, 0., 0., 0., 4.8e-4, {0}, 0., 0., 0., 4.8e-4, {0}
		, 0., 0., 0., 4.6e-4, {0}, 0., 0., 0., 4.3e-4, {0}, 0., 0., 
		0., 3.8e-4, {0}, 0., 0., 0., 3.1e-4, {0}, 0., 0., 0., 2.7e-4, 
		{0}, 0., 0., 0., 2.4e-4, {0}, 0., 0., 0., 2.4e-4, {0}, 0., 0.,
		 0., 2.3e-4, {0}, 0., 0., 0., 2.4e-4, {0}, 0., 0., 0., 2.4e-4,
		 {0}, 0., 0., 0., 2.4e-4, {0}, 0., 0., 0., 2.5e-4, {0}, 0., 
		0., 0., 2.5e-4, {0}, 0., 0., 0., 2.6e-4, {0}, 0., 0., 0., 
		2.6e-4, {0}, 0., 0., 0., 2.7e-4, {0}, 0., 0., 0., 2.7e-4, {0},
		 0., 0., 0., 2.7e-4, {0}, 0., 0., 0., 2.8e-4, {0}, 0., 0., 0.,
		 2.8e-4, {0}, 0., 0., 0., 2.8e-4, {0}, 0., 0., 0., 2.9e-4, {0}
		, 0., 0., 0., 2.9e-4, {0}, 0., 0., 0., 2.8e-4, {0}, 0., 0., 
		0., 3.2e-4, {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {
		0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., 
		{0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0.,
		 {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 
		0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0.,
		 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 
		0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0.,
		 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 
		0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0.,
		 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 
		0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0},
		 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}
		, 0., 0., 0., 0., {0}, 0., 0., 0., 0., {0}, 0., 0., 0., 0. };

#define sr ((doublereal *)&equiv_5)


    integer i__;
    doublereal wli[8], wls[8];


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
    wli[0] = .525;
/*<       wls(1)=0.5925 >*/
    wls[0] = .5925;
/* channel 2 lower and upper wavelength */
/*<       wli(2)=0.6275 >*/
    wli[1] = .6275;
/*<       wls(2)=0.6975 >*/
    wls[1] = .6975;
/* channel 3 lower and upper wavelength */
/*<       wli(3)=0.8325 >*/
    wli[2] = .8325;
/*<       wls(3)=0.9025 >*/
    wls[2] = .9025;
/* channel 4 lower and upper wavelength */
/*<       wli(4)=1.4475 >*/
    wli[3] = 1.4475;
/*<       wls(4)=1.7775 >*/
    wls[3] = 1.7775;
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
