/* otb_main6S_program_call_function.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"


/* Table of constant values */

static integer c__9 = 9;
static integer c__1 = 1;
static integer c__4 = 4;
static integer c__3 = 3;

/*<       program call6Sv1_0b >*/
/* Main program */ OTB_6S_MAIN__()
{
    /* Format strings */
    static char fmt_100[] = "(10x,\002 ----->  atmospheric reflectance :    \
  \002,f6.5)";
    static char fmt_200[] = "(10x,\002 ----->  atmospheric spherical albedo \
: \002,f6.5)";
    static char fmt_300[] = "(10x,\002 ----->  total gaseous transmission : \
  \002,f6.5)";
    static char fmt_400[] = "(10x,\002 ----->  downward transmittance :     \
  \002,f6.5)";
    static char fmt_500[] = "(10x,\002 ----->  upward transmittance :       \
  \002,f6.5)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(), s_rsle(cilist *), e_rsle(), s_wsfe(cilist *), do_fio(
	    integer *, char *, ftnlen), e_wsfe();

    /* Local variables */
    real otb_ratm__, pressure;
    integer i__, l;
    real s[1501];
    integer ik;
    real uw, uo3, phi0;
    integer iaer, iinf, jday;
    real asol, avis, phiv, sast;
    integer isup, iread;
    real taer55, tgasm, wlinf;
    integer month;
    real wlsup;
    extern /* Subroutine */ int otb_6s__(real *, real *, real *, real *, 
	    integer *, integer *, real *, real *, real *, integer *, real *, 
	    real *, real *, real *, real *, real *, real *, real *, real *);
    real sdtott, sutott;

    /* Fortran I/O blocks */
    static cilist io___4 = { 0, 6, 0, 0, 0 };
    static cilist io___5 = { 0, 0, 0, 0, 0 };
    static cilist io___12 = { 0, 6, 0, 0, 0 };
    static cilist io___13 = { 0, 6, 0, 0, 0 };
    static cilist io___14 = { 0, 6, 0, 0, 0 };
    static cilist io___15 = { 0, 0, 0, 0, 0 };
    static cilist io___19 = { 0, 6, 0, 0, 0 };
    static cilist io___20 = { 0, 6, 0, 0, 0 };
    static cilist io___21 = { 0, 6, 0, 0, 0 };
    static cilist io___22 = { 0, 0, 0, 0, 0 };
    static cilist io___24 = { 0, 6, 0, 0, 0 };
    static cilist io___25 = { 0, 6, 0, 0, 0 };
    static cilist io___26 = { 0, 6, 0, 0, 0 };
    static cilist io___27 = { 0, 6, 0, 0, 0 };
    static cilist io___28 = { 0, 0, 0, 0, 0 };
    static cilist io___30 = { 0, 6, 0, 0, 0 };
    static cilist io___31 = { 0, 6, 0, 0, 0 };
    static cilist io___34 = { 0, 0, 0, 0, 0 };
    static cilist io___37 = { 0, 6, 0, 0, 0 };
    static cilist io___38 = { 0, 6, 0, 0, 0 };
    static cilist io___40 = { 0, 0, 0, 0, 0 };
    static cilist io___47 = { 0, 6, 0, 0, 0 };
    static cilist io___48 = { 0, 6, 0, fmt_100, 0 };
    static cilist io___49 = { 0, 6, 0, fmt_200, 0 };
    static cilist io___50 = { 0, 6, 0, fmt_300, 0 };
    static cilist io___51 = { 0, 6, 0, fmt_400, 0 };
    static cilist io___52 = { 0, 6, 0, fmt_500, 0 };


/*<       IMPLICIT NONE >*/
/*<       real wlinf,wlsup,s(1501) >*/
/*<       real asol,phi0,avis,phiv >*/
/*<       integer month,jday >*/
/*<       real uw,uo3,taer55 >*/
/*<       real tgasm,sdtott,sutott,sast >*/
/*<       integer iwave,iinf,isup,inhomo,idirec,iaer >*/
/*<       integer iread,i,ik,l >*/
/* _otb	Atmospheric reflectance */
/*<       real otb_ratm	!Atmospheric reflectance >*/
/*<       real pressure     !Atmospheric pressure >*/
/* ***********************************************************************
 */
/*   Parameters  initialization */
/* ***********************************************************************
 */
/*<       iread=5 >*/
    iread = 5;
/*<       iinf=1 >*/
    iinf = 1;
/*<       isup=1501 >*/
    isup = 1501;
/*<       write(6,*) "------> Execution de l'interface d'appel de 6S " >*/
    s_wsle(&io___4);
    do_lio(&c__9, &c__1, "------> Execution de l'interface d'appel de 6S ", 
	    47L);
    e_wsle();
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                                                *     sun             c 
*/
/*                                              \ * /                   c 
*/
/*                                            * * * * *                 c 
*/
/*                                   z          / * \                   c 
*/
/*                                   +           /+                     c 
*/
/*            satellite    /         +          /                       c 
*/
/*                       o/          +         /                        c 
*/
/*                      /.\          +        /.                        c 
*/
/*                     / . \  _avis-_+_-asol_/ .                        c 
*/
/*                       .  \-      -+      /  .    north               c 
*/
/*                       .   \       +     /   .  +                     c 
*/
/*                       .    \      +    /    .+                       c 
*/
/*                       .     \     +   /    +.                        c 
*/
/*                       .      \    +  /   +  .                        c 
*/
/*                       .       \   + /  +    .                        c 
*/
/*                       .        \  +/ +      .                        c 
*/
/*    west + + + + + + + . + + + + +\+ + + + + . + + + + + + + + east   c 
*/
/*                       .          +..        .                        c 
*/
/*                       .        + .   .      .                        c 
*/
/*                       .      +  .      .    .                        c 
*/
/*                       .    +   .       .'.  .                        c 
*/
/*                       .  +    .. . , '     ..                        c 
*/
/*                       .+     .       \       .                       c 
*/
/*                      +.     .         \        .                     c 
*/
/*                    +  .    .           \         .                   c 
*/
/*             south     .   .       (phiv-phi0)                        c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* **********************************************************************c
 */
/*       igeom               geometrical conditions                     c 
*/
/*               --------------------------------------                 c 
*/
/*                                                                      c 
*/
/*   you choose your own conditions; igeom=0                            c 
*/
/*         0     enter solar zenith angle   (in degrees )               c 
*/
/*                     solar azimuth angle        "                     c 
*/
/*                     satellite zenith angle     "                     c 
*/
/*                     satellite azimuth angle    "                     c 
*/
/*                     month                                            c 
*/
/*                     day of the month                                 c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       read(iread,*) asol,phi0,avis,phiv,month,jday >*/
    io___5.ciunit = iread;
    s_rsle(&io___5);
    do_lio(&c__4, &c__1, (char *)&asol, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&phi0, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&avis, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&phiv, (ftnlen)sizeof(real));
    do_lio(&c__3, &c__1, (char *)&month, (ftnlen)sizeof(integer));
    do_lio(&c__3, &c__1, (char *)&jday, (ftnlen)sizeof(integer));
    e_rsle();
/*<       write(6,*) "asol,phi0,avis,phiv,month,jday : " >*/
    s_wsle(&io___12);
    do_lio(&c__9, &c__1, "asol,phi0,avis,phiv,month,jday : ", 33L);
    e_wsle();
/*<       write(6,*)  asol,phi0,avis,phiv,month,jday >*/
    s_wsle(&io___13);
    do_lio(&c__4, &c__1, (char *)&asol, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&phi0, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&avis, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&phiv, (ftnlen)sizeof(real));
    do_lio(&c__3, &c__1, (char *)&month, (ftnlen)sizeof(integer));
    do_lio(&c__3, &c__1, (char *)&jday, (ftnlen)sizeof(integer));
    e_wsle();
/*<       write(6,*) >*/
    s_wsle(&io___14);
    e_wsle();
/* **********************************************************************c
 */
/*       idatm      atmospheric model                                   c 
*/
/*                 --------------------                                 c 
*/
/*                 pressure (  in mb )                                  c 
*/
/*                 uw  (in  g/cm2 )                                     c 
*/
/*                 uo3 (in  cm-atm)                                     c 
*/
/*                 profil is taken from us62                            c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       read(iread,*) pressure, uw, uo3    >*/
    io___15.ciunit = iread;
    s_rsle(&io___15);
    do_lio(&c__4, &c__1, (char *)&pressure, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&uw, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&uo3, (ftnlen)sizeof(real));
    e_rsle();
/*<       write(6,*) "pressure, uw, uo3" >*/
    s_wsle(&io___19);
    do_lio(&c__9, &c__1, "pressure, uw, uo3", 17L);
    e_wsle();
/*<       write(6,*)  pressure, uw, uo3  >*/
    s_wsle(&io___20);
    do_lio(&c__4, &c__1, (char *)&pressure, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&uw, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&uo3, (ftnlen)sizeof(real));
    e_wsle();
/*<       write(6,*)   >*/
    s_wsle(&io___21);
    e_wsle();
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*       iaer       aerosol model(type)                                 c 
*/
/*                  --------------                                      c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*  you select one of the following standard aerosol models:            c 
*/
/*         0  no aerosols                                               c 
*/
/*         1  continental model  )                                      c 
*/
/*         2  maritime model     )  according to sra models             c 
*/
/*         3  urban model        )                                      c 
*/
/*         5  shettle model for background desert aerosol               c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       read(iread,*) iaer >*/
    io___22.ciunit = iread;
    s_rsle(&io___22);
    do_lio(&c__3, &c__1, (char *)&iaer, (ftnlen)sizeof(integer));
    e_rsle();
/*<       write(6,*) "iaer = ", iaer >*/
    s_wsle(&io___24);
    do_lio(&c__9, &c__1, "iaer = ", 7L);
    do_lio(&c__3, &c__1, (char *)&iaer, (ftnlen)sizeof(integer));
    e_wsle();
/*<       write(6,*) >*/
    s_wsle(&io___25);
    e_wsle();
/*<    >*/
    if (iaer != 0 && iaer != 1 && iaer != 2 && iaer != 3 && iaer != 5) {
/*<           write(6,*) "Choix iaer non compatible : ", iaer >*/
	s_wsle(&io___26);
	do_lio(&c__9, &c__1, "Choix iaer non compatible : ", 28L);
	do_lio(&c__3, &c__1, (char *)&iaer, (ftnlen)sizeof(integer));
	e_wsle();
/*< 	  write(6,*) "Choisir 0, 1, 2, 3 ou 5 uniquement" >*/
	s_wsle(&io___27);
	do_lio(&c__9, &c__1, "Choisir 0, 1, 2, 3 ou 5 uniquement", 34L);
	e_wsle();
/*<           goto 999 >*/
	goto L999;
/*<       endif >*/
    }
/*<       read(iread,*) taer55 >*/
    io___28.ciunit = iread;
    s_rsle(&io___28);
    do_lio(&c__4, &c__1, (char *)&taer55, (ftnlen)sizeof(real));
    e_rsle();
/*<       write(6,*)  "taer55 = ", taer55 >*/
    s_wsle(&io___30);
    do_lio(&c__9, &c__1, "taer55 = ", 9L);
    do_lio(&c__4, &c__1, (char *)&taer55, (ftnlen)sizeof(real));
    e_wsle();
/*<       write(6,*) >*/
    s_wsle(&io___31);
    e_wsle();
/* **********************************************************************c
 */
/*      iwave input of the spectral conditions                          c 
*/
/*            --------------------------------                          c 
*/
/*                                                                      c 
*/
/*         1  enter wlinf, wlsup and user's filter function s(lambda)   c 
*/
/*                          ( by step of 0.0025 micrometer).            c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       do 38 l=iinf,isup >*/
    i__1 = isup;
    for (l = iinf; l <= i__1; ++l) {
/*<        s(l)=0. >*/
	s[l - 1] = (float)0.;
/*<    38 continue >*/
/* L38: */
    }
/*<   110 read(iread,*) wlinf,wlsup >*/
/* L110: */
    io___34.ciunit = iread;
    s_rsle(&io___34);
    do_lio(&c__4, &c__1, (char *)&wlinf, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&wlsup, (ftnlen)sizeof(real));
    e_rsle();
/*<       write(6,*) " wlinf,wlsup : ", wlinf,wlsup >*/
    s_wsle(&io___37);
    do_lio(&c__9, &c__1, " wlinf,wlsup : ", 15L);
    do_lio(&c__4, &c__1, (char *)&wlinf, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&wlsup, (ftnlen)sizeof(real));
    e_wsle();
/*<       write(6,*) >*/
    s_wsle(&io___38);
    e_wsle();
/*<       iinf=(wlinf-.25)/0.0025+1.5 >*/
    iinf = (wlinf - (float).25) / (float).0025 + (float)1.5;
/*<       isup=(wlsup-.25)/0.0025+1.5 >*/
    isup = (wlsup - (float).25) / (float).0025 + (float)1.5;
/*<       do 1113 ik=iinf,isup >*/
    i__1 = isup;
    for (ik = iinf; ik <= i__1; ++ik) {
/*<        s(ik)=0. >*/
	s[ik - 1] = (float)0.;
/*<  1113 continue >*/
/* L1113: */
    }
/*<       read(iread,*) (s(i),i=iinf,isup) >*/
    io___40.ciunit = iread;
    s_rsle(&io___40);
    i__1 = isup;
    for (i__ = iinf; i__ <= i__1; ++i__) {
	do_lio(&c__4, &c__1, (char *)&s[i__ - 1], (ftnlen)sizeof(real));
    }
    e_rsle();
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                     start of computations                            c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*      goto 800 */
/*<    >*/
    otb_6s_ssssss_otb_main_function(&asol, &phi0, &avis, &phiv, &month, &jday, &pressure, &uw, &uo3, 
	    &iaer, &taer55, &wlinf, &wlsup, s, &otb_ratm__, &sast, &tgasm, &
	    sdtott, &sutott);
/*<       write(6,*) >*/
    s_wsle(&io___47);
    e_wsle();
/*< 800   write(6,100) otb_ratm >*/
/* L800: */
    s_wsfe(&io___48);
    do_fio(&c__1, (char *)&otb_ratm__, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(6,200) sast >*/
    s_wsfe(&io___49);
    do_fio(&c__1, (char *)&sast, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(6,300) tgasm >*/
    s_wsfe(&io___50);
    do_fio(&c__1, (char *)&tgasm, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(6,400) sdtott >*/
    s_wsfe(&io___51);
    do_fio(&c__1, (char *)&sdtott, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(6,500) sutott >*/
    s_wsfe(&io___52);
    do_fio(&c__1, (char *)&sutott, (ftnlen)sizeof(real));
    e_wsfe();
/*< 100   format(10x,40h ----->  atmospheric reflectance :      , f6.5)  >*/
/*< 200   format(10x,40h ----->  atmospheric spherical albedo : , f6.5)    >*/
/*< 300   format(10x,40h ----->  total gaseous transmission :   , f6.5)    >*/
/*< 400   format(10x,40h ----->  downward transmittance :       , f6.5)    >*/
/*< 500   format(10x,40h ----->  upward transmittance :         , f6.5)   >*/
/*< 999   continue >*/
L999:
/*<       end >*/
    return 0;
} /* OTB_6S_MAIN__ */

/* Main program alias */ int call6sv1_0b__ () { OTB_6S_MAIN__ (); return 0; }
#ifdef __cplusplus
	}
#endif
