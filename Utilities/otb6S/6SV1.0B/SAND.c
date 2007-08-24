/* SAND.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine sand(r) >*/
/* Subroutine */ int sand_(real *r__)
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
	    0.,(float)0.,(float)0.,(float).0455,(float).091,(float).091,(
	    float).091,(float).091,(float).091,(float).091,(float).091,(float)
	    .091,(float).091,(float).091,(float).091,(float).093,(float).095,(
	    float).095,(float).095,(float).095,(float).095,(float).095,(float)
	    .095,(float).095,(float).095,(float).095,(float).095,(float).095,(
	    float).095,(float).096,(float).097,(float).097,(float).097,(float)
	    .0985,(float).1,(float).1015,(float).103,(float).103,(float).103,(
	    float).105,(float).107,(float).107,(float).107,(float).107,(float)
	    .107,(float).107,(float).107,(float).1085,(float).11,(float).112,(
	    float).114,(float).1155,(float).117,(float).119,(float).121,(
	    float).121,(float).121,(float).123,(float).125,(float).125,(float)
	    .125,(float).125,(float).125,(float).1265,(float).128,(float).127,
	    (float).126,(float).1285,(float).131,(float).1325,(float).134,(
	    float).134,(float).134,(float).134,(float).134,(float).134,(float)
	    .134,(float).134,(float).134,(float).134,(float).134,(float).1375,
	    (float).141,(float).143,(float).145,(float).147,(float).149,(
	    float).149,(float).149,(float).1515,(float).154,(float).1555,(
	    float).157,(float).1585,(float).16,(float).1615,(float).163,(
	    float).163,(float).163,(float).1645,(float).166,(float).1675,(
	    float).169,(float).171,(float).173,(float).175,(float).177,(float)
	    .179,(float).181,(float).183,(float).185,(float).187,(float).189,(
	    float).192,(float).195,(float).197,(float).199,(float).2015,(
	    float).204,(float).204,(float).204,(float).206,(float).208,(float)
	    .211,(float).214,(float).217,(float).22,(float).222,(float).224,(
	    float).226,(float).228,(float).2305,(float).233,(float).234,(
	    float).235,(float).237,(float).239,(float).2405,(float).242,(
	    float).244,(float).246,(float).246,(float).246,(float).247,(float)
	    .248,(float).2495,(float).251,(float).2525,(float).254,(float)
	    .255,(float).256,(float).258,(float).26,(float).2615,(float).263,(
	    float).263,(float).263,(float).263,(float).263,(float).2645,(
	    float).266,(float).268,(float).27,(float).2715,(float).273,(float)
	    .2745,(float).276,(float).2775,(float).279,(float).279,(float)
	    .279,(float).2805,(float).282,(float).283,(float).284,(float).284,
	    (float).284,(float).284,(float).284,(float).285,(float).286,(
	    float).286,(float).286,(float).286,(float).286,(float).288,(float)
	    .29,(float).291,(float).292,(float).292,(float).292,(float).292,(
	    float).292,(float).292,(float).292,(float).292,(float).292,(float)
	    .292,(float).292,(float).292,(float).292,(float).2935,(float).295,
	    (float).295,(float).295,(float).2965,(float).298,(float).298,(
	    float).298,(float).2995,(float).301,(float).301,(float).301,(
	    float).3035,(float).306,(float).306,(float).306,(float).308,(
	    float).31,(float).31,(float).31,(float).312,(float).314,(float)
	    .314,(float).314,(float).314,(float).314,(float).315,(float).316,(
	    float).316,(float).316,(float).316,(float).316,(float).316,(float)
	    .316,(float).316,(float).316,(float).3175,(float).319,(float).32,(
	    float).321,(float).321,(float).321,(float).321,(float).321,(float)
	    .321,(float).321,(float).3225,(float).324,(float).324,(float).324,
	    (float).324,(float).324,(float).324,(float).324,(float).323,(
	    float).322,(float).322,(float).322,(float).322,(float).322,(float)
	    .321,(float).32,(float).32,(float).32,(float).3185,(float).317,(
	    float).317,(float).317,(float).317,(float).317,(float).3185,(
	    float).32,(float).32,(float).32,(float).32,(float).32,(float)
	    .3215,(float).323,(float).323,(float).323,(float).325,(float).327,
	    (float).327,(float).327,(float).327,(float).327,(float).328,(
	    float).329,(float).329,(float).329,(float).3305,(float).332,(
	    float).332,(float).332,(float).332,(float).332,(float).332,(float)
	    .332,(float).3325,(float).333,(float).334,(float).335,(float)
	    .3365,(float).338,(float).3395,(float).341,(float).343,(float)
	    .345,(float).3465,(float).348,(float).3505,(float).353,(float)
	    .353,(float).353,(float).353,(float).353,(float).353,(float).353,(
	    float).354,(float).355,(float).355,(float).355,(float).355,(float)
	    .355,(float).355,(float).355,(float).354,(float).353,(float).353,(
	    float).353,(float).353,(float).353,(float).353,(float).353,(float)
	    .353,(float).353,(float).353,(float).353,(float).3545,(float).356,
	    (float).356,(float).356,(float).356,(float).356,(float).356,(
	    float).356,(float).358,(float).36,(float).36,(float).36,(float)
	    .36,(float).36,(float).3615,(float).363,(float).363,(float).363,(
	    float).363,(float).363,(float).365,(float).367,(float).367,(float)
	    .367,(float).367,(float).367,(float).3685,(float).37,(float).37,(
	    float).37,(float).37,(float).37,(float).37,(float).37,(float).37,(
	    float).37,(float).37,(float).37,(float).3685,(float).367,(float)
	    .367,(float).367,(float).3655,(float).364,(float).363,(float).362,
	    (float).362,(float).362,(float).362,(float).362,(float).3605,(
	    float).359,(float).359,(float).359,(float).359,(float).359,(float)
	    .359,(float).359,(float).359,(float).359,(float).361,(float).363,(
	    float).363,(float).363,(float).363,(float).363,(float).363,(float)
	    .363,(float).363,(float).363,(float).364,(float).365,(float).367,(
	    float).369,(float).369,(float).369,(float).369,(float).369,(float)
	    .369,(float).369,(float).369,(float).369,(float).3705,(float).372,
	    (float).372,(float).372,(float).372,(float).372,(float).372,(
	    float).372,(float).3735,(float).375,(float).375,(float).375,(
	    float).375,(float).375,(float).375,(float).375,(float).375,(float)
	    .375,(float).375,(float).375,(float).375,(float).375,(float).375,(
	    float).375,(float).375,(float).375,(float).375,(float).375,(float)
	    .375,(float).375,(float).375,(float).375,(float).375,(float).375,(
	    float).375,(float).375,(float).375,(float).375,(float).376,(float)
	    .377,(float).377,(float).377,(float).377,(float).377,(float).377,(
	    float).377,(float).377,(float).377,(float).377,(float).377,(float)
	    .377,(float).377,(float).377,(float).377,(float).377,(float).377,(
	    float).377,(float).377,(float).377,(float).377,(float).377,(float)
	    .377,(float).378,(float).379,(float).379,(float).379,(float).379,(
	    float).379,(float).379,(float).379,(float).3805,(float).382,(
	    float).382,(float).382,(float).382,(float).382,(float).382,(float)
	    .382,(float).3835,(float).385,(float).385,(float).385,(float).385,
	    (float).385,(float).385,(float).385,(float).385,(float).385,(
	    float).385,(float).385,(float).3865,(float).388,(float).388,(
	    float).388,(float).388,(float).388,(float).388,(float).388,(float)
	    .388,(float).388,(float).388,(float).388,(float).388,(float).388,(
	    float).3895,(float).391,(float).391,(float).391,(float).391,(
	    float).391,(float).391,(float).391,(float).391,(float).391,(float)
	    .391,(float).391,(float).391,(float).391,(float).391,(float).391,(
	    float).391,(float).391,(float).391,(float).391,(float).391,(float)
	    .391,(float).3925,(float).394,(float).394,(float).394,(float).394,
	    (float).394,(float).394,(float).394,(float).394,(float).394,(
	    float).394,(float).394,(float).394,(float).394,(float).395,(float)
	    .396,(float).396,(float).396,(float).396,(float).396,(float).396,(
	    float).396,(float).396,(float).396,(float).396,(float).396,(float)
	    .3945,(float).393,(float).393,(float).393,(float).393,(float).393,
	    (float).393,(float).393,(float).391,(float).389,(float).3875,(
	    float).386,(float).386,(float).386,(float).384,(float).382,(float)
	    .382,(float).382,(float).38,(float).378,(float).378,(float).378,(
	    float).376,(float).374,(float).3715,(float).369,(float).369,(
	    float).369,(float).369,(float).369,(float).37,(float).371,(float)
	    .371,(float).371,(float).371,(float).371,(float).371,(float).371,(
	    float).371,(float).371,(float).371,(float).371,(float).371,(float)
	    .371,(float).371,(float).371,(float).371,(float).371,(float).371,(
	    float).371,(float).3725,(float).374,(float).374,(float).374,(
	    float).3755,(float).377,(float).377,(float).377,(float).377,(
	    float).377,(float).377,(float).377,(float).378,(float).379,(float)
	    .379,(float).379,(float).379,(float).379,(float).379,(float).379,(
	    float).381,(float).383,(float).384,(float).385,(float).3865,(
	    float).388,(float).388,(float).388,(float).388,(float).388,(float)
	    .39,(float).392,(float).392,(float).392,(float).392,(float).392,(
	    float).3935,(float).395,(float).395,(float).395,(float).395,(
	    float).395,(float).395,(float).395,(float).395,(float).395,(float)
	    .394,(float).393,(float).393,(float).393,(float).393,(float).393,(
	    float).3905,(float).388,(float).388,(float).388,(float).388,(
	    float).388,(float).388,(float).388,(float).388,(float).388,(float)
	    .3865,(float).385,(float).385,(float).385,(float).385,(float).385,
	    (float).383,(float).381,(float).381,(float).381,(float).381,(
	    float).381,(float).381,(float).381,(float).381,(float).381,(float)
	    .381,(float).381,(float).381,(float).381,(float).381,(float).381,(
	    float).3775,(float).374,(float).374,(float).374,(float).374,(
	    float).374,(float).374,(float).374,(float).374,(float).374,(float)
	    .374,(float).374,(float).374,(float).374,(float).374,(float).374,(
	    float).373,(float).372,(float).3705,(float).369,(float).369,(
	    float).369,(float).369,(float).369,(float).369,(float).369,(float)
	    .369,(float).369,(float).369,(float).369,(float).369,(float).369,(
	    float).369,(float).369,(float).369,(float).369,(float).369,(float)
	    .369,(float).369,(float).369,(float).369,(float).369,(float).369,(
	    float).369,(float).369,(float).369,(float).369,(float).369,(float)
	    .369,(float).369,(float).369,(float).369,(float).369,(float).369,(
	    float).369,(float).369,(float).369,(float).369,(float)0.,(float)
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
	    0.,(float)0.,(float)0. };

    integer i__;

/*<       real r,sr >*/
/*<       integer l,i >*/
/*<       dimension sr(1501),r(1501) >*/

/*     sand average reflectance */
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
} /* sand_ */

#ifdef __cplusplus
	}
#endif
