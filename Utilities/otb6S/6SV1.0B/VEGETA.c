/* VEGETA.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine   vegeta (r) >*/
/* Subroutine */ int vegeta_(real *r__)
{
    /* Initialized data */

    static real sr[1501] = { (float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float).03,(float).06,(float).06,(float)
	    .06,(float).061,(float).062,(float).063,(float).064,(float).065,(
	    float).066,(float).0675,(float).069,(float).0695,(float).07,(
	    float).071,(float).072,(float).073,(float).074,(float).076,(float)
	    .078,(float).079,(float).08,(float).0815,(float).083,(float).0835,
	    (float).084,(float).0865,(float).089,(float).091,(float).093,(
	    float).0955,(float).098,(float).1,(float).102,(float).103,(float)
	    .104,(float).105,(float).106,(float).108,(float).11,(float).1125,(
	    float).115,(float).117,(float).119,(float).12,(float).121,(float)
	    .123,(float).125,(float).126,(float).127,(float).1285,(float).13,(
	    float).1315,(float).133,(float).133,(float).133,(float).1335,(
	    float).134,(float).1335,(float).133,(float).132,(float).131,(
	    float).129,(float).127,(float).124,(float).121,(float).118,(float)
	    .115,(float).1125,(float).11,(float).1075,(float).105,(float).103,
	    (float).101,(float).0995,(float).098,(float).096,(float).094,(
	    float).092,(float).09,(float).0885,(float).087,(float).085,(float)
	    .083,(float).082,(float).081,(float).0805,(float).08,(float).079,(
	    float).078,(float).077,(float).076,(float).0755,(float).075,(
	    float).0745,(float).074,(float).0735,(float).073,(float).073,(
	    float).073,(float).073,(float).073,(float).0735,(float).074,(
	    float).0765,(float).079,(float).0895,(float).1,(float).119,(float)
	    .138,(float).1535,(float).169,(float).184,(float).199,(float)
	    .2135,(float).228,(float).2435,(float).259,(float).2745,(float)
	    .29,(float).303,(float).316,(float).333,(float).35,(float).364,(
	    float).378,(float).3905,(float).403,(float).4195,(float).436,(
	    float).449,(float).462,(float).4745,(float).487,(float).498,(
	    float).509,(float).51,(float).511,(float).5125,(float).514,(float)
	    .5165,(float).519,(float).5195,(float).52,(float).52,(float).52,(
	    float).521,(float).522,(float).522,(float).522,(float).522,(float)
	    .522,(float).5225,(float).523,(float).5235,(float).524,(float)
	    .524,(float).524,(float).524,(float).524,(float).524,(float).524,(
	    float).525,(float).526,(float).526,(float).526,(float).526,(float)
	    .526,(float).5265,(float).527,(float).527,(float).527,(float).527,
	    (float).527,(float).5275,(float).528,(float).528,(float).528,(
	    float).528,(float).528,(float).5285,(float).529,(float).529,(
	    float).529,(float).529,(float).529,(float).529,(float).529,(float)
	    .53,(float).531,(float).531,(float).531,(float).531,(float).531,(
	    float).531,(float).531,(float).531,(float).531,(float).5315,(
	    float).532,(float).532,(float).532,(float).532,(float).532,(float)
	    .532,(float).532,(float).532,(float).532,(float).5325,(float).533,
	    (float).533,(float).533,(float).533,(float).533,(float).5335,(
	    float).534,(float).534,(float).534,(float).534,(float).534,(float)
	    .534,(float).534,(float).5345,(float).535,(float).535,(float).535,
	    (float).5355,(float).536,(float).536,(float).536,(float).5365,(
	    float).537,(float).537,(float).537,(float).5365,(float).536,(
	    float).536,(float).536,(float).5355,(float).535,(float).535,(
	    float).535,(float).5345,(float).534,(float).533,(float).532,(
	    float).5315,(float).531,(float).5305,(float).53,(float).529,(
	    float).528,(float).528,(float).528,(float).5275,(float).527,(
	    float).527,(float).527,(float).5265,(float).526,(float).5255,(
	    float).525,(float).5245,(float).524,(float).523,(float).522,(
	    float).5215,(float).521,(float).52,(float).519,(float).5185,(
	    float).518,(float).5165,(float).515,(float).514,(float).513,(
	    float).5125,(float).512,(float).511,(float).51,(float).509,(float)
	    .508,(float).5075,(float).507,(float).5065,(float).506,(float)
	    .5055,(float).505,(float).5035,(float).502,(float).501,(float).5,(
	    float).499,(float).498,(float).497,(float).496,(float).4955,(
	    float).495,(float).494,(float).493,(float).4925,(float).492,(
	    float).492,(float).492,(float).492,(float).492,(float).492,(float)
	    .492,(float).492,(float).492,(float).4925,(float).493,(float).494,
	    (float).495,(float).495,(float).495,(float).4955,(float).496,(
	    float).496,(float).496,(float).496,(float).496,(float).4965,(
	    float).497,(float).497,(float).497,(float).497,(float).497,(float)
	    .4975,(float).498,(float).498,(float).498,(float).4975,(float)
	    .497,(float).497,(float).497,(float).497,(float).497,(float).496,(
	    float).495,(float).494,(float).493,(float).4925,(float).492,(
	    float).4915,(float).491,(float).4895,(float).488,(float).487,(
	    float).486,(float).484,(float).482,(float).48,(float).478,(float)
	    .477,(float).476,(float).474,(float).472,(float).4695,(float).467,
	    (float).4645,(float).462,(float).4565,(float).451,(float).446,(
	    float).441,(float).435,(float).429,(float).425,(float).421,(float)
	    .4145,(float).408,(float).4035,(float).399,(float).392,(float)
	    .385,(float).378,(float).371,(float).368,(float).365,(float).357,(
	    float).349,(float).344,(float).339,(float).3345,(float).33,(float)
	    .3255,(float).321,(float).315,(float).309,(float).3035,(float)
	    .298,(float).2935,(float).289,(float).284,(float).279,(float)
	    .2755,(float).272,(float).2695,(float).267,(float).263,(float)
	    .259,(float).255,(float).251,(float).247,(float).243,(float).238,(
	    float).233,(float).231,(float).229,(float).2265,(float).224,(
	    float).221,(float).218,(float).2165,(float).215,(float).215,(
	    float).215,(float).215,(float).215,(float).215,(float).215,(float)
	    .217,(float).219,(float).221,(float).223,(float).226,(float).229,(
	    float).2315,(float).234,(float).237,(float).24,(float).2445,(
	    float).249,(float).2525,(float).256,(float).258,(float).26,(float)
	    .2635,(float).267,(float).27,(float).273,(float).276,(float).279,(
	    float).2825,(float).286,(float).2895,(float).293,(float).2965,(
	    float).3,(float).303,(float).306,(float).309,(float).312,(float)
	    .3155,(float).319,(float).322,(float).325,(float).328,(float).331,
	    (float).334,(float).337,(float).339,(float).341,(float).343,(
	    float).345,(float).348,(float).351,(float).353,(float).355,(float)
	    .3575,(float).36,(float).361,(float).362,(float).3645,(float).367,
	    (float).368,(float).369,(float).3705,(float).372,(float).374,(
	    float).376,(float).377,(float).378,(float).3785,(float).379,(
	    float).38,(float).381,(float).3815,(float).382,(float).383,(float)
	    .384,(float).385,(float).386,(float).3865,(float).387,(float).388,
	    (float).389,(float).3885,(float).388,(float).388,(float).388,(
	    float).388,(float).388,(float).388,(float).388,(float).388,(float)
	    .388,(float).388,(float).388,(float).386,(float).384,(float).3835,
	    (float).383,(float).382,(float).381,(float).3805,(float).38,(
	    float).379,(float).378,(float).377,(float).376,(float).375,(float)
	    .374,(float).3735,(float).373,(float).372,(float).371,(float)
	    .3705,(float).37,(float).369,(float).368,(float).3675,(float).367,
	    (float).3665,(float).366,(float).3655,(float).365,(float).365,(
	    float).365,(float).364,(float).363,(float).3625,(float).362,(
	    float).3615,(float).361,(float).36,(float).359,(float).3585,(
	    float).358,(float).3575,(float).357,(float).356,(float).355,(
	    float).354,(float).353,(float).3515,(float).35,(float).3485,(
	    float).347,(float).3465,(float).346,(float).3455,(float).345,(
	    float).344,(float).343,(float).3415,(float).34,(float).3385,(
	    float).337,(float).336,(float).335,(float).333,(float).331,(float)
	    .3305,(float).33,(float).3255,(float).321,(float).3165,(float)
	    .312,(float).304,(float).296,(float).2845,(float).273,(float).247,
	    (float).221,(float).2035,(float).186,(float).172,(float).158,(
	    float).148,(float).138,(float).1335,(float).129,(float).125,(
	    float).121,(float).1155,(float).11,(float).106,(float).102,(float)
	    .0985,(float).095,(float).093,(float).091,(float).09,(float).089,(
	    float).0875,(float).086,(float).086,(float).086,(float).085,(
	    float).084,(float).084,(float).084,(float).084,(float).084,(float)
	    .085,(float).086,(float).0865,(float).087,(float).09,(float).093,(
	    float).0955,(float).098,(float).1015,(float).105,(float).1095,(
	    float).114,(float).115,(float).116,(float).12,(float).124,(float)
	    .1285,(float).133,(float).1335,(float).134,(float).1375,(float)
	    .141,(float).144,(float).147,(float).149,(float).151,(float).1535,
	    (float).156,(float).159,(float).162,(float).164,(float).166,(
	    float).168,(float).17,(float).172,(float).174,(float).1745,(float)
	    .175,(float).1765,(float).178,(float).1795,(float).181,(float)
	    .183,(float).185,(float).186,(float).187,(float).1875,(float).188,
	    (float).19,(float).192,(float).194,(float).196,(float).1975,(
	    float).199,(float).2,(float).201,(float).203,(float).205,(float)
	    .2065,(float).208,(float).21,(float).212,(float).2125,(float).213,
	    (float).2135,(float).214,(float).2155,(float).217,(float).218,(
	    float).219,(float).2195,(float).22,(float).2205,(float).221,(
	    float).2225,(float).224,(float).2255,(float).227,(float).228,(
	    float).229,(float).23,(float).231,(float).232,(float).233,(float)
	    .235,(float).237,(float).2375,(float).238,(float).2385,(float)
	    .239,(float).24,(float).241,(float).2415,(float).242,(float).2425,
	    (float).243,(float).244,(float).245,(float).245,(float).245,(
	    float).2455,(float).246,(float).247,(float).248,(float).248,(
	    float).248,(float).249,(float).25,(float).248,(float).246,(float)
	    .244,(float).242,(float).24,(float).238,(float).236,(float).234,(
	    float).232,(float).23,(float).228,(float).226,(float).224,(float)
	    .222,(float).22,(float).218,(float).216,(float).214,(float).212,(
	    float).21,(float).208,(float).206,(float).204,(float).202,(float)
	    .2,(float).198,(float).196,(float).194,(float).192,(float).19,(
	    float).188,(float).186,(float).184,(float).182,(float).18,(float)
	    .178,(float).176,(float).174,(float).172,(float).17,(float).168,(
	    float).166,(float).164,(float).162,(float).16,(float).158,(float)
	    .156,(float).154,(float).152,(float).15,(float).148,(float).146,(
	    float).144,(float).142,(float).14,(float).138,(float).136,(float)
	    .134,(float).132,(float).13,(float).128,(float).126,(float).124,(
	    float).122,(float).12,(float).118,(float).116,(float).114,(float)
	    .112,(float).11,(float).108,(float).106,(float).104,(float).102,(
	    float).1,(float).098,(float).096,(float).094,(float).092,(float)
	    .09,(float).088,(float).086,(float).084,(float).082,(float).08,(
	    float).078,(float).076,(float).074,(float).072,(float).07,(float)
	    .068,(float).066,(float).064,(float).062,(float).06,(float).058,(
	    float).056,(float).054,(float).052,(float).05,(float).048,(float)
	    .046,(float).044,(float).042,(float).04,(float).038,(float).036,(
	    float).034,(float).032,(float).03,(float).028,(float).026,(float)
	    .024,(float).022,(float).02,(float).018,(float).016,(float).014,(
	    float).012,(float).01,(float).01,(float).01,(float).01,(float).01,
	    (float).01,(float).01,(float).01,(float).01,(float).01,(float).01,
	    (float).01,(float).01,(float).01,(float).01,(float).01,(float).01,
	    (float).01,(float).01,(float).01,(float).01,(float).01,(float).01,
	    (float).01,(float).01,(float).01,(float).01,(float).01,(float).01,
	    (float).01,(float).01,(float).01,(float).01,(float).01,(float).01,
	    (float).01,(float).01,(float).01,(float).01,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0. };

    integer i__;

/*<       real sr(1501),r(1501) >*/
/*<       integer l,i >*/
/*     vegetation average reflectance */
/*     warning : values of dry sand ground reflectance are given */
/*     between 0.4 and 2.2 microns. outside this interval the */
/*     values are set to 0. */
/*<    >*/
    /* Parameter adjustments */
    --r__;

    /* Function Body */
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       do 1 i=1,1501 >*/
    for (i__ = 1; i__ <= 1501; ++i__) {
/*<       r(i)=sr(i) >*/
	r__[i__] = sr[i__ - 1];
/*<     1 continue >*/
/* L1: */
    }
/*<       return >*/
    return 0;
/*<       end >*/
} /* vegeta_ */

#ifdef __cplusplus
	}
#endif
