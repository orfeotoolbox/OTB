/* IAPITOOLS.f -- translated by f2c (version 19970805).
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
    doublereal xgm[20], wgm[20];
    integer n;
} gauss_m__;

#define gauss_m__1 gauss_m__

Extern struct {
    doublereal xlt, rl, tl, rs, c__;
    integer ild;
} p_;

#define p_1 p_

Extern struct {
    doublereal ro_1_c__, ro_1_s__, ro_mult__;
} ro_;

#define ro_1 ro_

Extern struct {
    doublereal a_ld__, b_ld__, c_ld__, d_ld__;
} ld_;

#define ld_1 ld_

Extern struct {
    doublereal dl, xl;
} l_;

#define l_1 l_

/*<         real function Ro_1 (Theta_i,Phi_i,Theta_e,Phi_e) >*/
doublereal ro_1__(doublereal *theta_i__, doublereal *phi_i__, doublereal *
	theta_e__, doublereal *phi_e__)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val, d__1;

    /* Builtin functions */
    double cos(doublereal), exp(doublereal);

    /* Local variables */
    extern doublereal h_(doublereal *, doublereal *);
    integer j;
    doublereal ge, gi, ke, ki, xl;
    extern doublereal g_f__(doublereal *), geo_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal xdb, xli, xmm, xrm, xmu, xmui, xtmu;
    extern doublereal gamma_f__(doublereal *, doublereal *, doublereal *, 
	    doublereal *);

/*<         common /gauss_m/xgm (20),wgm (20),n >*/
/*<         real xgm,wgm >*/
/*<         integer n >*/
/*<         real G_f,Geo,h,gamma_f >*/
/*<         common /p/xLt,Rl,Tl,Rs,c,ild >*/
/*<         real xLt,Rl,Tl,Rs,c >*/
/*<         integer ild >*/
/*<         common /Ro/Ro_1_c,Ro_1_s,Ro_mult >*/
/*<         real Ro_1_c,Ro_1_s,Ro_mult >*/
/*<         real Theta_i,Phi_i,Theta_e,Phi_e,xmui,xmu,xtmu >*/
/*<         real Gi,Ge,Ki,Ke,xLi >*/
/*<         real xmm,xrm >*/
/*<         real xL >*/
/*<         double precision xdb >*/
/*<         integer j >*/

/*<         xtmu=1.E-05 >*/
    xtmu = 1e-5;
/*<         xmui=abs(cos(Theta_i)) >*/
    xmui = (d__1 = cos(*theta_i__), abs(d__1));
/*<         xmu=cos(Theta_e) >*/
    xmu = cos(*theta_e__);
/*<         if (abs(xmu).lt.xtmu) xmu=xtmu >*/
    if (abs(xmu) < xtmu) {
	xmu = xtmu;
    }

/*<         Gi=G_f (Theta_i) >*/
    gi = g_f__(theta_i__);
/*<         Ge=G_f (Theta_e) >*/
    ge = g_f__(theta_e__);

/*<         Ki=Gi/xmui >*/
    ki = gi / xmui;
/*<         Ke=Ge/xmu >*/
    ke = ge / xmu;

/*<         xmm=0.5*(xLt+0.) >*/
    xmm = (p_1.xlt + 0.) * .5;
/*<         xrm=0.5*(xLt-0.) >*/
    xrm = (p_1.xlt + 0.) * .5;
/*<         Ro_1_c=0. >*/
    ro_1.ro_1_c__ = 0.;
/*<         xLi=c/Geo (Theta_i,Phi_i,Theta_e,Phi_e)  >*/
    xli = p_1.c__ / geo_(theta_i__, phi_i__, theta_e__, phi_e__);
/*<         do j=1,n >*/
    i__1 = gauss_m__1.n;
    for (j = 1; j <= i__1; ++j) {
/*<           xL=xmm+xrm*xgm(j) >*/
	xl = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<           xdb=(Ki+Ke*h(xL,xLi))*dble(xL) >*/
	xdb = (ki + ke * h_(&xl, &xli)) * xl;
/*<           if (abs(xdb).lt.1.E-30) xdb=0. >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<           if (xdb.le.20) Ro_1_c=Ro_1_c+wgm(j)*xrm*dexp(-xdb) >*/
	if (xdb <= 20.) {
	    ro_1.ro_1_c__ += gauss_m__1.wgm[j - 1] * xrm * exp(-xdb);
	}
/*<         enddo >*/
    }

/*<    >*/
    ro_1.ro_1_c__ = ro_1.ro_1_c__ * gamma_f__(theta_i__, phi_i__, theta_e__, 
	    phi_e__) / xmui / xmu;

/*<         xdb=(Ki+Ke*h (xLt,xLi))*dble(xLt) >*/
    xdb = (ki + ke * h_(&p_1.xlt, &xli)) * p_1.xlt;
/*<         if (abs(xdb).lt.1.E-30) xdb=0. >*/
    if (abs(xdb) < 1e-30) {
	xdb = 0.;
    }
/*<         if (xdb.le.20) Ro_1_s=Rs*dexp(-xdb) >*/
    if (xdb <= 20.) {
	ro_1.ro_1_s__ = p_1.rs * exp(-xdb);
    }

/*<         Ro_1=Ro_1_c+Ro_1_s >*/
    ret_val = ro_1.ro_1_c__ + ro_1.ro_1_s__;

/*<         return >*/
    return ret_val;
/*<         end >*/
} /* ro_1__ */


/*<         real function Gamma_f (Theta_p,Phi_p,Theta,Phi) >*/
doublereal gamma_f__(doublereal *theta_p__, doublereal *phi_p__, doublereal *
	theta, doublereal *phi)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal ret_val;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    doublereal f;
    integer i__, j;
    extern doublereal gl_(doublereal *);
    doublereal dp, xt, yt, dpp, xmm, ymm, sum, xrm, yrm;

/*<         parameter (Pi=3.141592653589793) >*/
/*<         common /p/xLt,Rl,Tl,Rs,c,ild >*/
/*<         real xLt,Rl,Tl,Rs,c,gl >*/
/*<         integer ild >*/
/*<         common /gauss_m/xgm (20),wgm (20),n >*/
/*<         real xgm,wgm >*/
/*<         integer n >*/
/*<         real Theta_p,Phi_p,Theta,Phi >*/
/*<         real xmm,xrm,xt >*/
/*<         real ymm,yrm,yt >*/
/*<         real dp,dpp,f >*/
/*<         real sum >*/
/*<         integer i,j >*/

/*<         xmm=0.5*(Pi/2.+0.) >*/
    xmm = .78539816339744828;
/*<         xrm=0.5*(Pi/2.-0.) >*/
    xrm = .78539816339744828;
/*<         ymm=0.5*(2.*Pi+0.) >*/
    ymm = 3.1415926535897931;
/*<         yrm=0.5*(2.*Pi-0.) >*/
    yrm = 3.1415926535897931;
/*<         Gamma_f = 0. >*/
    ret_val = 0.;
/*<         do j=1,n >*/
    i__1 = gauss_m__1.n;
    for (j = 1; j <= i__1; ++j) {
/*<           xt=xmm+xrm*xgm (j) >*/
	xt = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<           sum=0. >*/
	sum = 0.;
/*<           do i=1,n >*/
	i__2 = gauss_m__1.n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/*<             yt=ymm+yrm*xgm (i) >*/
	    yt = ymm + yrm * gauss_m__1.xgm[i__ - 1];
/*<    >*/
	    dpp = cos(*theta_p__) * cos(xt) + sin(*theta_p__) * sin(xt) * cos(
		    *phi_p__ - yt);
/*<    >*/
	    dp = cos(*theta) * cos(xt) + sin(*theta) * sin(xt) * cos(*phi - 
		    yt);
/* correction when porting code to HP730 */
/*<             if (dp*dpp.lt.0.) then >*/
	    if (dp * dpp < 0.) {
/*<               f=Rl*abs (dp)/Pi >*/
		f = p_1.rl * abs(dp) / 3.141592653589793;
/*<               else >*/
	    } else {
/*<                   f=Tl*abs (dp)/Pi >*/
		f = p_1.tl * abs(dp) / 3.141592653589793;
/*<             endif >*/
	    }
/* end of correction */
/*<             sum=sum+wgm (i)*xrm*gl (xt)*f*abs (dpp) >*/
	    sum += gauss_m__1.wgm[i__ - 1] * xrm * gl_(&xt) * f * abs(dpp);
/*<           enddo >*/
	}
/*<           Gamma_f=Gamma_f+wgm (j)*yrm*sum >*/
	ret_val += gauss_m__1.wgm[j - 1] * yrm * sum;
/*<         enddo >*/
    }
/*<         Gamma_f=Gamma_f/2. >*/
    ret_val /= 2.;
/*<         return >*/
    return ret_val;
/*<         end >*/
} /* gamma_f__ */


/*<         real function Geo (Theta_i,Phi_i,Theta_e,Phi_e)  >*/
doublereal geo_(doublereal *theta_i__, doublereal *phi_i__, doublereal *
	theta_e__, doublereal *phi_e__)
{
    /* System generated locals */
    doublereal ret_val, d__1, d__2, d__3;

    /* Builtin functions */
    double tan(doublereal), cos(doublereal), sqrt(doublereal);

/*<         real Theta_i,Phi_i,Theta_e,Phi_e >*/
/*<    >*/
/* Computing 2nd power */
    d__2 = tan(*theta_i__);
/* Computing 2nd power */
    d__3 = tan(*theta_e__);
    ret_val = sqrt((d__1 = d__2 * d__2 + d__3 * d__3 - tan(*theta_i__) * 2. * 
	    tan(*theta_e__) * cos(*phi_i__ - *phi_e__), abs(d__1)));
/*<         if (Geo.lt.1.e-35) Geo=1.e-35 >*/
    if (ret_val < 1e-35) {
	ret_val = 1e-35;
    }
/*<         return >*/
    return ret_val;
/*<         end >*/
} /* geo_ */


/*<         real function h (xL,xLi) >*/
doublereal h_(doublereal *xl, doublereal *xli)
{
    /* System generated locals */
    doublereal ret_val;

/*<         parameter (Pi=3.141592653589793) >*/
/*<         real xL,xLi >*/
/*<         if (xL.lt.xLi) then >*/
    if (*xl < *xli) {
/*<           h=(1.-4./3./Pi)/xLi*xL >*/
	ret_val = .57558681842161241 / *xli * *xl;
/*<           else >*/
    } else {
/*<               h=1.-4./3./Pi*xLi/xL >*/
	ret_val = 1. - *xli * .42441318157838753 / *xl;
/*<         endif >*/
    }
/*<         return >*/
    return ret_val;
/*<         end >*/
} /* h_ */


/*<         real function G_f (Theta) >*/
doublereal g_f__(doublereal *theta)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Local variables */
    integer j;
    extern doublereal gl_(doublereal *);
    doublereal xt;
    extern doublereal psi_(doublereal *, doublereal *);
    doublereal xmm, xrm;

/*<         parameter (Pi=3.141592653589793) >*/
/*<         common /p/xLt,Rl,Tl,Rs,c,ild >*/
/*<         real xLt,Rl,Tl,Rs,c,psi,gl >*/
/*<         integer ild >*/
/*<         common /gauss_m/xgm (20),wgm (20),n >*/
/*<         real xgm,wgm >*/
/*<         integer n >*/
/*<         real Theta >*/
/*<         real xmm,xrm,xt >*/
/*<         integer j >*/

/*<         xmm=0.5*(Pi/2.+0.) >*/
    xmm = .78539816339744828;
/*<         xrm=0.5*(Pi/2.-0.) >*/
    xrm = .78539816339744828;
/*<         G_f = 0. >*/
    ret_val = 0.;
/*<         do j=1,n >*/
    i__1 = gauss_m__1.n;
    for (j = 1; j <= i__1; ++j) {
/*<           xt=xmm+xrm*xgm (j) >*/
	xt = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<           G_f=G_f+wgm (j)*xrm*Psi (Theta,xt)*gl (xt) >*/
	ret_val += gauss_m__1.wgm[j - 1] * xrm * psi_(theta, &xt) * gl_(&xt);
/*<         enddo >*/
    }
/*<         return >*/
    return ret_val;
/*<         end >*/
} /* g_f__ */


/*<         real function Psi (Theta,xt) >*/
doublereal psi_(doublereal *theta, doublereal *xt)
{
    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal), acos(doublereal);

    /* Local variables */
    doublereal pt, cpt, smu, xmu;

/*<         parameter (Pi=3.141592653589793) >*/
/*<         common /p/xLt,Rl,Tl,Rs,c,ild >*/
/*<         real xLt,Rl,Tl,Rs,c >*/
/*<         integer ild >*/
/*<         real Theta,xt >*/
/*<         real cpt,pt >*/
/*<         real xmu,smu >*/

/*<         xmu=cos (xt) >*/
    xmu = cos(*xt);
/*<         smu=sin (xt) >*/
    smu = sin(*xt);
/*<         if (xmu.eq.1.) then >*/
    if (xmu == 1.) {
/*<           Psi=cos (Theta) >*/
	ret_val = cos(*theta);
/*<           else >*/
    } else {
/*<               if (sin (Theta).eq.0.) then >*/
	if (sin(*theta) == 0.) {
/*<                 Psi=xmu >*/
	    ret_val = xmu;
/*<                 else >*/
	} else {
/*<                     if (smu.eq.0.) then >*/
	    if (smu == 0.) {
/*<                       cpt=0. >*/
		cpt = 0.;
/*<                       else >*/
	    } else {
/*<    >*/
		cpt = xmu * 1. / smu * cos(*theta) / sin(*theta);
/*<                     endif >*/
	    }
/*<                     if (abs (cpt).gt.1.) then >*/
	    if (abs(cpt) > 1.) {
/*<                       Psi=xmu*cos (Theta) >*/
		ret_val = xmu * cos(*theta);
/*<                       else >*/
	    } else {
/*<                           pt=acos (-cpt) >*/
		pt = acos(-cpt);
/*<    >*/
		ret_val = xmu * cos(*theta) * (pt * .63661977236758138 - 1.) 
			+ smu * .63661977236758138 * sin(*theta) * sin(pt);
/*<                     endif >*/
	    }
/*<               endif >*/
	}
/*<         endif >*/
    }
/*<         Psi=abs (Psi) >*/
    ret_val = abs(ret_val);
/*<         return >*/
    return ret_val;
/*<         end >*/
} /* psi_ */


/*<         real function gl (Theta) >*/
doublereal gl_(doublereal *theta)
{
    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

/*<         parameter (Pi=3.141592653589793) >*/
/*<         common /ld/a_ld,b_ld,c_ld,d_ld >*/
/*<         real a_ld,b_ld,c_ld,d_ld >*/
/*<         real Theta  >*/

/*<    >*/
    ret_val = ld_1.a_ld__ + ld_1.b_ld__ * cos(*theta * 2.) + ld_1.c_ld__ * 
	    cos(*theta * 4.) + ld_1.d_ld__ * sin(*theta);
/*<         return >*/
    return ret_val;
/*<         end >*/
} /* gl_ */


/*<         subroutine gauleg(x1,x2,x,w,n) >*/
/* Subroutine */ int gauleg_(doublereal *x1, doublereal *x2, doublereal *x, 
	doublereal *w, integer *n)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double cos(doublereal);

    /* Local variables */
    integer i__, j, m;
    doublereal z__, p1, p2, p3, z1, pp, xl, xm;

/*<         integer n >*/
/*<         real x1,x2,x (n),w (n) >*/
/*<         double precision eps >*/
/*<         parameter (eps=3.d-14) >*/
/*<         integer i,j,m >*/
/*<         double precision p1,p2,p3,pp,xl,xm,z,z1 >*/
/*<         m=(n+1)/2 >*/
    /* Parameter adjustments */
    --w;
    --x;

    /* Function Body */
    m = (*n + 1) / 2;
/*<         xm=0.5d00*(x2+x1) >*/
    xm = (*x2 + *x1) * .5;
/*<         xl=0.5d00*(x2-x1) >*/
    xl = (*x2 - *x1) * .5;
/*<         do i=1,m >*/
    i__1 = m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<           z=cos(3.141592654d00*(i-.25d00)/(n+.5d00)) >*/
	z__ = cos((i__ - .25) * 3.141592654 / (*n + .5));
/*< 1         continue >*/
L1:
/*<             p1=1.d00 >*/
	p1 = 1.;
/*<             p2=0.d00 >*/
	p2 = 0.;
/*<             do j=1,n >*/
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
/*<               p3=p2 >*/
	    p3 = p2;
/*<               p2=p1 >*/
	    p2 = p1;
/*<               p1=((2.d00*j-1.d00)*z*p2-(j-1.d00)*p3)/j >*/
	    p1 = ((j * 2. - 1.) * z__ * p2 - (j - 1.) * p3) / j;
/*<             enddo >*/
	}
/*<             pp=n*(z*p1-p2)/(z*z-1.d00) >*/
	pp = *n * (z__ * p1 - p2) / (z__ * z__ - 1.);
/*<             z1=z >*/
	z1 = z__;
/*<             z=z1-p1/pp >*/
	z__ = z1 - p1 / pp;
/*<           if(abs(z-z1).gt.eps)go to 1 >*/
	if ((d__1 = z__ - z1, abs(d__1)) > 3e-14) {
	    goto L1;
	}
/*<           x(i)=real (xm-xl*z) >*/
	x[i__] = xm - xl * z__;
/*<           x(n+1-i)=real (xm+xl*z) >*/
	x[*n + 1 - i__] = xm + xl * z__;
/*<           w(i)=real (2.d00*xl/((1.d00-z*z)*pp*pp)) >*/
	w[i__] = xl * 2. / ((1. - z__ * z__) * pp * pp);
/*<           w(n+1-i)=w(i) >*/
	w[*n + 1 - i__] = w[i__];
/*<         enddo >*/
    }
/*<         return >*/
    return 0;
/*<         end >*/
} /* gauleg_ */


/*<         subroutine solve (Theta_i) >*/
/* Subroutine */ int solve_(doublereal *theta_i__)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double cos(doublereal), acos(doublereal), exp(doublereal);

    /* Local variables */
    doublereal g[20];
    integer j, k, l;
    doublereal s[20], q1[20], gi;
    integer nc;
    doublereal xi[420]	/* was [21][20] */, q0d[20], q0u[20];
    extern doublereal g_f__(doublereal *);
    doublereal xdb, xif[420]	/* was [21][20] */, xmm, sum, xrm, xmu, xi0t, 
	    xi1t, ximt, xmui, epsilon;

/*<         parameter (Pi=3.141592653589793) >*/
/*<         parameter (m=20) >*/
/*<         common /gauss_m/xgm (20),wgm (20),n >*/
/*<         real xgm,wgm,g_f >*/
/*<         integer n >*/
/*<         common /p/xLt,Rl,Tl,Rs,c,ild >*/
/*<         real xLt,Rl,Tl,Rs,c >*/
/*<         integer ild >*/
/*<         common /Ro/Ro_1_c,Ro_1_s,Ro_mult >*/
/*<         real Ro_1_c,Ro_1_s,Ro_mult >*/
/*<         real Theta_i,xmui,Gi >*/
/*<         double precision xdb >*/
/*<         common /l/dL,xL >*/
/*<         real dL,xL >*/
/*<         real xI0t,xI1t,xImt >*/
/*<         real xI (m+1,20) >*/
/*<         real Q0d (m),Q0u (m),Q1 (m),S (m),xIf (m+1,20) >*/
/*<         real G (20) >*/
/*<         integer j,k,l >*/
/*<         real xmm,xrm,xmu >*/
/*<         real sum >*/
/*<         integer nc >*/
/*<         real Epsilon >*/

/*<         Epsilon=1.e-4 >*/
    epsilon = 1e-4;

/*<         xmui=abs (cos (Theta_i)) >*/
    xmui = (d__1 = cos(*theta_i__), abs(d__1));

/*<         dL=xLt/float (m) >*/
    l_1.dl = p_1.xlt / 20.;

/* - Computation of G-functions */

/*<         Gi=G_f (Theta_i) >*/
    gi = g_f__(theta_i__);
/*<         xmm=0.5*(1.+(-1.)) >*/
    xmm = 0.;
/*<         xrm=0.5*(1.-(-1.)) >*/
    xrm = 1.;
/*<         do j=1,n >*/
    i__1 = gauss_m__1.n;
    for (j = 1; j <= i__1; ++j) {
/*<           xmu=xmm+xrm*xgm (j) >*/
	xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<           G (j)=G_f (acos (xmu)) >*/
	d__1 = acos(xmu);
	g[j - 1] = g_f__(&d__1);
/*<         enddo >*/
    }

/* - Initialisation of S (k) & xIf (k,j) */

/*<         do k=1,m >*/
    for (k = 1; k <= 20; ++k) {
/*<           S (k)=0. >*/
	s[k - 1] = 0.;
/*<           do j=1,n >*/
	i__1 = gauss_m__1.n;
	for (j = 1; j <= i__1; ++j) {
/*<             xIf (k,j)=0. >*/
	    xif[k + j * 21 - 22] = 0.;
/*<           enddo >*/
	}
/*<         enddo >*/
    }

/* - Computation of Q0d (k) & Q0u (k) <- first collision source */

/* - (down) */

/*<         do k=1,m >*/
    for (k = 1; k <= 20; ++k) {
/*<           xL=(k-.5)*dL >*/
	l_1.xl = (k - .5) * l_1.dl;
/*<           xdb=Gi/xmui*dble(xL) >*/
	xdb = gi / xmui * l_1.xl;
/*<           if (abs(xdb).lt.1.E-30) xdb=0. >*/
	if (abs(xdb) < 1e-30) {
	    xdb = 0.;
	}
/*<           if (xdb.lt.20) Q0d(k)=(Rl+Tl)/2.*Gi*dexp(-xdb) >*/
	if (xdb < 20.) {
	    q0d[k - 1] = (p_1.rl + p_1.tl) / 2. * gi * exp(-xdb);
	}
/*<         enddo >*/
    }

/* - (up) */

/*<         xdb=Gi/xmui*dble(xLt) >*/
    xdb = gi / xmui * p_1.xlt;
/*<         if (abs(xdb).lt.1.E-30) xdb=0. >*/
    if (abs(xdb) < 1e-30) {
	xdb = 0.;
    }
/*<         if (xdb.lt.20) xI0t=2.*Rs*xmui*dexp(-xdb) >*/
    if (xdb < 20.) {
	xi0t = p_1.rs * 2. * xmui * exp(-xdb);
    }
/*<         do k=m,1,-1 >*/
    for (k = 20; k >= 1; --k) {
/*<           xL=(k-.5)*dL >*/
	l_1.xl = (k - .5) * l_1.dl;
/*<           sum=0. >*/
	sum = 0.;
/*<           do j=n/2+1,n >*/
	i__1 = gauss_m__1.n;
	for (j = gauss_m__1.n / 2 + 1; j <= i__1; ++j) {
/*<             xmu=xmm+xrm*xgm (j) >*/
	    xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<             xdb=dble(G(j))/dble(xmu)*(xLt-xL) >*/
	    xdb = g[j - 1] / xmu * (p_1.xlt - l_1.xl);
/*<             if (abs(xdb).lt.1.E-30) xdb=0. >*/
	    if (abs(xdb) < 1e-30) {
		xdb = 0.;
	    }
/*<    >*/
	    if (xdb < 20.) {
		sum += gauss_m__1.wgm[j - 1] * xrm * xi0t * (p_1.rl + p_1.tl) 
			/ 2. * g[j - 1] * exp(-xdb);
	    }
/*<           enddo >*/
	}
/*<           Q0u (k)=sum >*/
	q0u[k - 1] = sum;
/*<         enddo >*/
    }

/* - Computation of xI (k,j) <- single scattering */

/* - Initialisation of xI (k,j) */

/*<         do k=1,m+1 >*/
    for (k = 1; k <= 21; ++k) {
/*<           do j=1,n/2 >*/
	i__1 = gauss_m__1.n / 2;
	for (j = 1; j <= i__1; ++j) {
/*<             xI (k,j)=0. >*/
	    xi[k + j * 21 - 22] = 0.;
/*<           enddo >*/
	}
/*<         enddo >*/
    }

/* - (down) */

/*<         do k=1,m >*/
    for (k = 1; k <= 20; ++k) {
/*<           do j=1,n/2 >*/
	i__1 = gauss_m__1.n / 2;
	for (j = 1; j <= i__1; ++j) {
/*<             xmu=xmm+xrm*xgm (j) >*/
	    xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<    >*/
	    xi[k + 1 + j * 21 - 22] = (q0d[k - 1] - xi[k + j * 21 - 22] * (g[
		    j - 1] / 2. + xmu / l_1.dl)) / (g[j - 1] / 2. - xmu / 
		    l_1.dl);
/*<           enddo >*/
	}
/*<         enddo >*/
    }

/* - (boundary condition) */

/*<         xI1t=0. >*/
    xi1t = 0.;
/*<         do j=1,n/2 >*/
    i__1 = gauss_m__1.n / 2;
    for (j = 1; j <= i__1; ++j) {
/*<           xmu=xmm+xrm*xgm (j) >*/
	xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<           xI1t=xI1t+wgm (j)*xrm*2.*Rs*abs (xmu)*xI (m+1,j) >*/
	xi1t += gauss_m__1.wgm[j - 1] * xrm * 2. * p_1.rs * abs(xmu) * xi[j * 
		21 - 1];
/*<         enddo >*/
    }

/*<         do j=n/2+1,n >*/
    i__1 = gauss_m__1.n;
    for (j = gauss_m__1.n / 2 + 1; j <= i__1; ++j) {
/*<           xI (m+1,j)=0. >*/
	xi[j * 21 - 1] = 0.;
/*<         enddo >*/
    }

/* - (up) */

/*<         do k=m,1,-1 >*/
    for (k = 20; k >= 1; --k) {
/*<           do j=n/2+1,n >*/
	i__1 = gauss_m__1.n;
	for (j = gauss_m__1.n / 2 + 1; j <= i__1; ++j) {
/*<             xmu=xmm+xrm*xgm (j) >*/
	    xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<    >*/
	    xi[k + j * 21 - 22] = (q0d[k - 1] - xi[k + 1 + j * 21 - 22] * (g[
		    j - 1] / 2. - xmu / l_1.dl)) / (g[j - 1] / 2. + xmu / 
		    l_1.dl);
/*<           enddo >*/
	}
/*<         enddo >*/
    }

/* - Computation of Q1 (k) <- second collision source */

/*<         do k=1,m >*/
    for (k = 1; k <= 20; ++k) {
/*<           sum=0. >*/
	sum = 0.;
/*<           do j=1,n >*/
	i__1 = gauss_m__1.n;
	for (j = 1; j <= i__1; ++j) {
/*<    >*/
	    sum += gauss_m__1.wgm[j - 1] * xrm * (p_1.rl + p_1.tl) / 2. * g[j 
		    - 1] * (xi[k + 1 + j * 21 - 22] + xi[k + j * 21 - 22]) / 
		    2.;
/*<           enddo >*/
	}
/*<           Q1 (k)=sum >*/
	q1[k - 1] = sum;
/*<         enddo >*/
    }

/* - Computation of xI (k,j) <- multiple scattering */

/* - Initialisation of xI (k,j) */

/*<         do k=1,m+1 >*/
    for (k = 1; k <= 21; ++k) {
/*<           do j=1,n/2 >*/
	i__1 = gauss_m__1.n / 2;
	for (j = 1; j <= i__1; ++j) {
/*<             xI (k,j)=0. >*/
	    xi[k + j * 21 - 22] = 0.;
/*<           enddo >*/
	}
/*<         enddo >*/
    }
/*<         l=0 >*/
    l = 0;
/*<  1      l=l+1 >*/
L1:
    ++l;

/* - (down) */

/*<         do k=1,m >*/
    for (k = 1; k <= 20; ++k) {
/*<           do j=1,n/2 >*/
	i__1 = gauss_m__1.n / 2;
	for (j = 1; j <= i__1; ++j) {
/*<             xmu=xmm+xrm*xgm (j) >*/
	    xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<    >*/
	    xi[k + 1 + j * 21 - 22] = (s[k - 1] + q0u[k - 1] + q1[k - 1] - xi[
		    k + j * 21 - 22] * (g[j - 1] / 2. + xmu / l_1.dl)) / (g[j 
		    - 1] / 2. - xmu / l_1.dl);
/*<           enddo >*/
	}
/*<         enddo >*/
    }

/* - (boundary condition) */

/*<         xImt=0. >*/
    ximt = 0.;
/*<         do j=1,n/2 >*/
    i__1 = gauss_m__1.n / 2;
    for (j = 1; j <= i__1; ++j) {
/*<           xmu=xmm+xrm*xgm (j) >*/
	xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<           xImt=xImt+wgm (j)*xrm*2.*Rs*abs (xmu)*xI (m+1,j) >*/
	ximt += gauss_m__1.wgm[j - 1] * xrm * 2. * p_1.rs * abs(xmu) * xi[j * 
		21 - 1];
/*<         enddo >*/
    }
/*<         do j=n/2+1,n >*/
    i__1 = gauss_m__1.n;
    for (j = gauss_m__1.n / 2 + 1; j <= i__1; ++j) {
/*<           xI (m+1,j)=xImt+xI1t >*/
	xi[j * 21 - 1] = ximt + xi1t;
/*<         enddo >*/
    }

/* - (up) */

/*<         do k=m,1,-1 >*/
    for (k = 20; k >= 1; --k) {
/*<           do j=n/2+1,n >*/
	i__1 = gauss_m__1.n;
	for (j = gauss_m__1.n / 2 + 1; j <= i__1; ++j) {
/*<             xmu=xmm+xrm*xgm (j) >*/
	    xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<    >*/
	    xi[k + j * 21 - 22] = (s[k - 1] + q0u[k - 1] + q1[k - 1] - xi[k + 
		    1 + j * 21 - 22] * (g[j - 1] / 2. - xmu / l_1.dl)) / (g[j 
		    - 1] / 2. + xmu / l_1.dl);
/*<           enddo >*/
	}
/*<         enddo >*/
    }

/* - End test */

/*<         nc=0 >*/
    nc = 0;
/*<         do k=1,m+1 >*/
    for (k = 1; k <= 21; ++k) {
/*<           do j=1,n >*/
	i__1 = gauss_m__1.n;
	for (j = 1; j <= i__1; ++j) {
/*<             if (abs (xIf (k,j)-xI (k,j)).lt.Epsilon) nc=nc+1 >*/
	    if ((d__1 = xif[k + j * 21 - 22] - xi[k + j * 21 - 22], abs(d__1))
		     < epsilon) {
		++nc;
	    }
/*<             xIf (k,j)=xI (k,j) >*/
	    xif[k + j * 21 - 22] = xi[k + j * 21 - 22];
/*<           enddo >*/
	}
/*<         enddo >*/
    }
/*<         if ((l.lt.50).and.(nc.ne.(m+1)*n)) then >*/
    if (l < 50 && nc != gauss_m__1.n * 21) {

/* - Computation of S (k) <- distributed source */

/*<           do k=1,m >*/
	for (k = 1; k <= 20; ++k) {
/*<             sum=0. >*/
	    sum = 0.;
/*<             do j=1,n >*/
	    i__1 = gauss_m__1.n;
	    for (j = 1; j <= i__1; ++j) {
/*<    >*/
		sum += gauss_m__1.wgm[j - 1] * xrm * (p_1.rl + p_1.tl) / 2. * 
			g[j - 1] * (xi[k + 1 + j * 21 - 22] + xi[k + j * 21 - 
			22]) / 2.;
/*<             enddo >*/
	    }
/*<             S (k)=sum >*/
	    s[k - 1] = sum;
/*<           enddo >*/
	}
/*<           goto 1 >*/
	goto L1;
/*<         endif >*/
    }

/* - Computation of Ro_mult */

/*<         sum=0. >*/
    sum = 0.;
/*<         do j=n/2+1,n >*/
    i__1 = gauss_m__1.n;
    for (j = gauss_m__1.n / 2 + 1; j <= i__1; ++j) {
/*<           xmu=xmm+xrm*xgm (j) >*/
	xmu = xmm + xrm * gauss_m__1.xgm[j - 1];
/*<           sum=sum+wgm (j)*xrm*xI (1,j)*xmu/xmui >*/
	sum += gauss_m__1.wgm[j - 1] * xrm * xi[j * 21 - 21] * xmu / xmui;
/*<         enddo >*/
    }
/*<         Ro_mult=sum >*/
    ro_1.ro_mult__ = sum;
/*<         return >*/
    return 0;
/*<         end >*/
} /* solve_ */

/*<         subroutine lad >*/
/* Subroutine */ int lad_()
{
/*<         parameter (Pi=3.141592653589793) >*/
/*<         common /p/xLt,Rl,Tl,Rs,c,ild >*/
/*<         real xLt,Rl,Tl,Rs,c >*/
/*<         integer ild >*/
/*<         common /ld/a_ld,b_ld,c_ld,d_ld >*/
/*<         real a_ld,b_ld,c_ld,d_ld >*/

/*<         if (ild.eq.1) then >*/
    if (p_1.ild == 1) {
/*<           a_ld=2./Pi >*/
	ld_1.a_ld__ = .63661977236758138;
/*<           b_ld=2./Pi >*/
	ld_1.b_ld__ = .63661977236758138;
/*<           c_ld=0. >*/
	ld_1.c_ld__ = 0.;
/*<           d_ld=0. >*/
	ld_1.d_ld__ = 0.;
/*<           else >*/
    } else {
/*<               if (ild.eq.2) then >*/
	if (p_1.ild == 2) {
/*<                 a_ld=2./Pi >*/
	    ld_1.a_ld__ = .63661977236758138;
/*<                 b_ld=-2./Pi >*/
	    ld_1.b_ld__ = -.63661977236758138;
/*<                 c_ld=0. >*/
	    ld_1.c_ld__ = 0.;
/*<                 d_ld=0. >*/
	    ld_1.d_ld__ = 0.;
/*<                 else >*/
	} else {
/*<                     if (ild.eq.3) then >*/
	    if (p_1.ild == 3) {
/*<                       a_ld=2./Pi >*/
		ld_1.a_ld__ = .63661977236758138;
/*<                       b_ld=0. >*/
		ld_1.b_ld__ = 0.;
/*<                       c_ld=-2./Pi >*/
		ld_1.c_ld__ = -.63661977236758138;
/*<                       d_ld=0. >*/
		ld_1.d_ld__ = 0.;
/*<                       else >*/
	    } else {
/*<                           if (ild.eq.4) then >*/
		if (p_1.ild == 4) {
/*<                             a_ld=2./Pi >*/
		    ld_1.a_ld__ = .63661977236758138;
/*<                             b_ld=0. >*/
		    ld_1.b_ld__ = 0.;
/*<                             c_ld=2./Pi >*/
		    ld_1.c_ld__ = .63661977236758138;
/*<                             d_ld=0. >*/
		    ld_1.d_ld__ = 0.;
/*<                             else >*/
		} else {
/*<                                 a_ld=0. >*/
		    ld_1.a_ld__ = 0.;
/*<                                 b_ld=0. >*/
		    ld_1.b_ld__ = 0.;
/*<                                 c_ld=0. >*/
		    ld_1.c_ld__ = 0.;
/*<                                 d_ld=1. >*/
		    ld_1.d_ld__ = 1.;
/*<                           endif >*/
		}
/*<                     endif >*/
	    }
/*<               endif >*/
	}
/*<         endif >*/
    }
/*<         return >*/
    return 0;
/*<         end >*/
} /* lad_ */

#ifdef __cplusplus
	}
#endif
