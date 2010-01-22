/* VERSALBE.f -- translated by f2c (version 19970805).
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

/* Table of constant values */

static integer c__24 = 24;
static integer c__48 = 48;
static integer c__1 = 1;
static integer c__5 = 5;

/*<    >*/
/* Subroutine */ int versalbe_(integer *option, doublereal *optics, 
	doublereal *struct__, doublereal *brdfalb)
{
    /* Initialized data */

    static char fmt[60*6+1] = "(i10)                                        \
               (e10.3)                                                     (\
1x, a10, 6 (i8, 2x))                                       (1x, a10, 6 (f8.3\
, 2x))                                     (1x, a, f9.6)                    \
                           (1x, a)                                          \
           ";
    static char errmsg[80*15+1] = "101: option (1) not an acceptable value, \
reset to 1                             102: option (2) not an acceptable val\
ue, reset to 1                             103: option (3) not an acceptable\
 value, reset to 1                             104: option (4) not an accept\
able value, reset to 1                             105: option (5) not an ac\
ceptable value, reset to 1                             201: illumination zen\
ith angle (angles (1)) not acceptable                      202: observation \
zenith angle (angles (2)) not acceptable                       203: relative\
 azimuth angle (angles (3)) not acceptable                         301: sing\
le scattering albedo (optics (1)) not acceptable                       302: \
phase function parameter (optics (2) not acceptable                        4\
01: leaf area density (struct (1)) not acceptable                           \
   402: radius of sun flecks (struct (2)) not acceptable                    \
       411: kappa1 (struct (3) with option (3) = 0) not acceptable          \
           412: kappa2 (struct (4) with option (3) = 0) not acceptable      \
               421: chil (struct (3) with option (3) = 1) not acceptable    \
                   ";

    /* System generated locals */
    cilist ci__1;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), sin(doublereal), acos(
	    doublereal);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(), 
	    s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();

    /* Local variables */
    integer j, k, l;
    doublereal y, fa[48], fi, ta[24], pi, si1, si2, mu1, mu2, wfa[48], wta[24]
	    , phi1, phi2, pond, summ, teta1, teta2;
    extern doublereal mvbp1_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *);
    integer error;
    doublereal angles[3];

    /* Fortran I/O blocks */
    static cilist io___24 = { 0, 0, 0, 0, 0 };
    static cilist io___25 = { 0, 0, 0, 0, 0 };



/* interface between the computer code of the model of verstraete and pin 
*/
/* the computer code is courtesy of michel verstraete. */
/* see subroutine mvbp1 for a complete description */


/*  variables angles, error, optics, option, struct and mvbp1 are fully */
/*  described in the file mvbp1.for. */
/*     angdeg       angles in degrees */
/*     brdf         variable to receive the bidirectional reflectance */
/*                  value returned by the function mvbp1. */
/*     fmt          formats. */
/*     errmsg       error messages. */
/*<       parameter (nta=24,nfa=48) >*/
/*<       real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa) >*/
/*<       real mu1,mu2,fi >*/
/*<       real pi,y >*/

/*<       integer  option(5), error >*/
/*<       real angles(3), optics(3), struct(4) >*/
/*<       real         mvbp1 >*/
/*<       character*60 fmt (6) >*/
/*<       character*80 errmsg (15) >*/
/*<       external     mvbp1 >*/
/*<       logical ier >*/
/*<       real brdfalb,summ,si2,si1,pond >*/
/*<       integer iwr,k,j,l >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<       data fmt (1) /'(i10)'/ >*/
    /* Parameter adjustments */
    --struct__;
    --optics;
    --option;

    /* Function Body */
/*<       data fmt (2) /'(e10.3)'/ >*/
/*<       data fmt (3) /'(1x, a10, 6 (i8, 2x))'/ >*/
/*<       data fmt (4) /'(1x, a10, 6 (f8.3, 2x))'/ >*/
/*<       data fmt (5) /'(1x, a, f9.6)'/ >*/
/*<       data fmt (6) /'(1x, a)'/ >*/
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

/*<       pi=atan(1.)*4. >*/
    pi = atan(1.) * 4.;
/*<       teta1=0. >*/
    teta1 = 0.;
/*<       teta2=pi/2. >*/
    teta2 = pi / 2.;
/*<       call gauss(teta1,teta2,ta,wta,nta) >*/
    gauss_(&teta1, &teta2, ta, wta, &c__24);
/*<       phi1=0. >*/
    phi1 = 0.;
/*<       phi2=2.*pi >*/
    phi2 = pi * 2.;
/*<       call gauss(phi1,phi2,fa,wfa,nfa) >*/
    gauss_(&phi1, &phi2, fa, wfa, &c__48);
/*<       brdfalb=0. >*/
    *brdfalb = 0.;
/*<       summ=0. >*/
    summ = 0.;
/*<       do 1 k=1,nfa >*/
    for (k = 1; k <= 48; ++k) {
/*<       do 2 j=1,nta >*/
	for (j = 1; j <= 24; ++j) {
/*<       do 3 l=1,nta >*/
	    for (l = 1; l <= 24; ++l) {
/*<       mu2=cos(ta(j)) >*/
		mu2 = cos(ta[j - 1]);
/*<       mu1=cos(ta(l)) >*/
		mu1 = cos(ta[l - 1]);
/*<       si2=sin(ta(j)) >*/
		si2 = sin(ta[j - 1]);
/*<       si1=sin(ta(l)) >*/
		si1 = sin(ta[l - 1]);
/*<       fi=fa(k) >*/
		fi = fa[k - 1];
/* compute geometrical parameter for mvbp1 */
/*<       angles(1)=acos(mu1) >*/
		angles[0] = acos(mu1);
/*<       angles(2)=acos(mu2) >*/
		angles[1] = acos(mu2);
/*<       angles(3)=fi >*/
		angles[2] = fi;
/*<       y=mvbp1(option,angles,optics,struct,error) >*/
		y = mvbp1_(&option[1], angles, &optics[1], &struct__[1], &
			error);
/*<       if (error .eq. 101) write (iwr, fmt (6)) errmsg (1) >*/
		if (error == 101) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 102) write (iwr, fmt (6)) errmsg (2) >*/
		if (error == 102) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 80, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 103) write (iwr, fmt (6)) errmsg (3) >*/
		if (error == 103) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 160, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 104) write (iwr, fmt (6)) errmsg (4) >*/
		if (error == 104) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 240, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 105) write (iwr, fmt (6)) errmsg (5) >*/
		if (error == 105) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 320, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 201) then >*/
		if (error == 201) {
/*<          write (iwr, fmt (6)) errmsg (6) >*/
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 400, 80L);
		    e_wsfe();
/*<          write(iwr,*) angles(1) >*/
		    io___24.ciunit = sixs_ier__1.iwr;
		    s_wsle(&io___24);
		    do_lio(&c__5, &c__1, (char *)&angles[0], (ftnlen)sizeof(
			    doublereal));
		    e_wsle();
/*<          endif >*/
		}
/*<       if (error .eq. 202) then >*/
		if (error == 202) {
/*<          write(iwr,fmt (6)) errmsg (7) >*/
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 480, 80L);
		    e_wsfe();
/*<          write(iwr,*) angles(2) >*/
		    io___25.ciunit = sixs_ier__1.iwr;
		    s_wsle(&io___25);
		    do_lio(&c__5, &c__1, (char *)&angles[1], (ftnlen)sizeof(
			    doublereal));
		    e_wsle();
/*<          endif >*/
		}
/*<       if (error .eq. 203) write (iwr, fmt (6)) errmsg (8) >*/
		if (error == 203) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 560, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 301) write (iwr, fmt (6)) errmsg (9) >*/
		if (error == 301) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 640, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 302) write (iwr, fmt (6)) errmsg (10) >*/
		if (error == 302) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 720, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 401) write (iwr, fmt (6)) errmsg (11) >*/
		if (error == 401) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 800, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 402) write (iwr, fmt (6)) errmsg (12) >*/
		if (error == 402) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 880, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 411) write (iwr, fmt (6)) errmsg (13) >*/
		if (error == 411) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 960, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 412) write (iwr, fmt (6)) errmsg (14) >*/
		if (error == 412) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 1040, 80L);
		    e_wsfe();
		}
/*<       if (error .eq. 421) write (iwr, fmt (6)) errmsg (15) >*/
		if (error == 421) {
		    ci__1.cierr = 0;
		    ci__1.ciunit = sixs_ier__1.iwr;
		    ci__1.cifmt = fmt + 300;
		    s_wsfe(&ci__1);
		    do_fio(&c__1, errmsg + 1120, 80L);
		    e_wsfe();
		}
/*<       pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l) >*/
		pond = mu1 * mu2 * si1 * si2 * wfa[k - 1] * wta[j - 1] * wta[
			l - 1];
/*<       brdfalb=brdfalb+pond*y >*/
		*brdfalb += pond * y;
/*<       summ=summ+pond >*/
		summ += pond;
/*<   3   continue >*/
/* L3: */
	    }
/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }
/*<       brdfalb=brdfalb/summ >*/
    *brdfalb /= summ;
/*<       return >*/
    return 0;
/*<       end >*/
} /* versalbe_ */

#ifdef __cplusplus
	}
#endif
