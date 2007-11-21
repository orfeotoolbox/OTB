/* otb_main6S_function.f -- translated by f2c (version 19970805).
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

Extern struct {
    real rmax, rmin;
    integer icp;
    real rn[40]	/* was [10][4] */, ri[40]	/* was [10][4] */, x1[4], x2[
	    4], x3[4], cij[4];
    integer irsunph;
    real rsunph[50], nrsunph[50];
} mie_in__;

#define mie_in__1 mie_in__

Extern struct {
    real zpl[34], ppl[34], tpl[34], whpl[34], wopl[34];
} sixs_planesim__;

#define sixs_planesim__1 sixs_planesim__

Extern struct {
    real xacc;
} sixs_test__;

#define sixs_test__1 sixs_test__

Extern struct {
    real phasel[830]	/* was [10][83] */, cgaus[83], pdgs[83];
} sixs_sos__;

#define sixs_sos__1 sixs_sos__

Extern struct {
    real pha[83], betal[81];
} sixs_trunc__;

#define sixs_trunc__1 sixs_trunc__

Extern struct {
    real s[1501], wlinf, wlsup;
} sixs_ffu__;

#define sixs_ffu__1 sixs_ffu__

Extern struct {
    real delta, sigma;
} sixs_del__;

#define sixs_del__1 sixs_del__

Extern struct {
    real z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

Extern struct {
    real ext[10], ome[10], gasym[10], phase[10];
} sixs_aer__;

#define sixs_aer__1 sixs_aer__

Extern struct {
    real roatm[30]	/* was [3][10] */, dtdir[30]	/* was [3][10] */, 
	    dtdif[30]	/* was [3][10] */, utdir[30]	/* was [3][10] */, 
	    utdif[30]	/* was [3][10] */, sphal[30]	/* was [3][10] */, 
	    wldis[10], trayl[10], traypl[10];
} sixs_disc__;

#define sixs_disc__1 sixs_disc__

/* Table of constant values */

static real c_b4 = (float)-1.;
static real c_b5 = (float)1.;
static real c_b6 = (float)0.;
static integer c__4 = 4;
static integer c__1 = 1;
static integer c__3 = 3;
static doublereal c_b76 = 4.;
static integer c__2 = 2;

/*<    >*/
/* Subroutine */ int otb_6s_ssssss_otb_main_function(real *otb_asol__, real *otb_phi0__, real *
	otb_avis__, real *otb_phiv__, integer *otb_month__, integer *
	otb_jday__, real *otb_pressure__, real *otb_uw__, real *otb_uo3__, 
	integer *otb_iaer__, real *otb_taer55__, real *otb_wlinf__, real *
	otb_wlsup__, real *otb_s__, real *otb_ratm__, real *otb_sast__, real *
	otb_tgasm__, real *otb_sdtott__, real *otb_sutott__)
{
    /* Initialized data */

    static real angmu[10] = { (float)85.,(float)80.,(float)70.,(float)60.,(
	    float)50.,(float)40.,(float)30.,(float)20.,(float)10.,(float)0. };
    static real angphi[13] = { (float)0.,(float)30.,(float)60.,(float)90.,(
	    float)120.,(float)150.,(float)180.,(float)210.,(float)240.,(float)
	    270.,(float)300.,(float)330.,(float)360. };
    static real wldisc[10] = { (float).4,(float).488,(float).515,(float).55,(
	    float).633,(float).694,(float).86,(float)1.536,(float)2.25,(float)
	    3.75 };
    static char etiq1[60*8+1] = "(1h*,22x,34h user defined conditions       \
   ,t79,1h*)     (1h*,22x,24h meteosat observation   ,t79,1h*)              \
 (1h*,22x,25h goes east observation   ,t79,1h*)              (1h*,22x,25h go\
es west observation   ,t79,1h*)              (1h*,22x,30h avhrr (AM noaa) ob\
servation  ,t79,1h*)         (1h*,22x,30h avhrr (PM noaa) observation  ,t79,\
1h*)         (1h*,22x,24h h.r.v.   observation   ,t79,1h*)               (1h\
*,22x,24h t.m.     observation   ,t79,1h*)               ";
    static char nsat[17*61+1] = " constant         user s           meteosat\
         goes east        goes west        avhrr 1 (noaa6)  avhrr 2 (noaa6) \
 avhrr 1 (noaa7)  avhrr 2 (noaa7)  avhrr 1 (noaa8)  avhrr 2 (noaa8)  avhrr 1\
 (noaa9)  avhrr 2 (noaa9)  avhrr 1 (noaa10) avhrr 2 (noaa10) avhrr 1 (noaa11\
) avhrr 2 (noaa11) hrv1 1           hrv1 2           hrv1 3           hrv1 p\
an         hrv2 1           hrv2 2           hrv2 3           hrv2 pan      \
    tm  1            tm  2            tm  3            tm  4            tm  \
5            tm  7            mss 4            mss 5            mss 6       \
     mss 7            mas 1            mas 2            mas 3            mas\
 4            mas 5            mas 6            mas 7            modis 1    \
      modis 2          modis 3          modis 4          modis 5          mo\
dis 6          modis 7         avhrr 1 (noaa12) avhrr 2 (noaa12) avhrr 1 (no\
aa14) avhrr 2 (noaa14) polder 1         polder 2         polder 3         po\
lder 4         polder 6         polder 6         polder 7         polder 8  \
      ";
    static char atmid[51*7+1] = "no absorption computed                     \
        tropical            (uh2o=4.12g/cm2,uo3=.247cm-atm)midlatitude summe\
r  (uh2o=2.93g/cm2,uo3=.319cm-atm)midlatitude winter  (uh2o=.853g/cm2,uo3=.3\
95cm-atm)subarctic  summer   (uh2o=2.10g/cm2,uo3=.480cm-atm)subarctic  winte\
r   (uh2o=.419g/cm2,uo3=.480cm-atm)us  standard 1962   (uh2o=1.42g/cm2,uo3=.\
344cm-atm)";
    static char reflec[71*8+1] = "(1h*,12x,39h user defined spectral reflect\
ance     ,f6.3,t79     ,1h*) (1h*,12x,27h monochromatic reflectance ,f6.3,t7\
9,1h*)                  (1h*,12x,39h constant reflectance over the spectra ,\
f6.3,t79     ,1h*) (1h*,12x,39h spectral vegetation ground reflectance,f6.3,\
t79     ,1h*) (1h*,12x,39h spectral clear water reflectance      ,f6.3,t79  \
   ,1h*) (1h*,12x,39h spectral dry sand ground reflectance  ,f6.3,t79     ,1\
h*) (1h*,12x,39h spectral lake water reflectance       ,f6.3,t79     ,1h*) (\
1h*,12x,39h spectral volcanic debris reflectance  ,f6.3,t79     ,1h*) ";

    /* Format strings */
    static char fmt_98[] = "(/////,\002*\002,30(\002*\002),\002 6s version 4\
.1 \002,30(\002*\002),t79,\002*\002,/,\002*\002,t79,\002*\002,/,\002*\002,22\
x,\002 geometrical conditions identity  \002,t79,\002*\002,/,\002*\002,22x\
,\002 -------------------------------  \002,t79,\002*\002)";
    static char fmt_1401[] = "(\002*\002,t79,\002*\002)";
    static char fmt_103[] = "(\002*\002,2x,\002 month:\002,i3,\002 day : \
\002,i3,t79,\002*\002)";
    static char fmt_101[] = "(\002*\002,15x,\002 month:\002,i3,\002 day :\
 \002,i3,\002 universal time:\002,f6.2,\002 (hh.dd)  \002,t79,\002*\002,/\
,\002*\002,15x,\002latitude: \002,f7.2,\002 deg \002,6x,\002 longitude: \002\
,f7.2,\002 deg \002,t79,\002*\002)";
    static char fmt_102[] = "(\002*\002,2x,\002 solar zenith angle:  \002,f6\
.2,\002 deg \002,\002 solar azimuthal angle:      \002,f6.2,\002 deg \002,t7\
9,\002*\002)";
    static char fmt_1110[] = "(\002*\002,2x,\002 view zenith angle:   \002,f\
6.2,\002 deg \002,\002 view azimuthal angle:       \002,f6.2,\002 deg \002,t\
79,\002*\002,/,\002*\002,2x,\002 scattering angle:    \002,f6.2,\002 deg \
\002,\002 azimuthal angle difference: \002,f6.2,\002 deg \002,t79,\002*\002)";
    static char fmt_1119[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002 \
atmospheric model description \002,t79,\002*\002,/,\002*\002,22x,\002 ------\
----------------------- \002,t79,\002*\002)";
    static char fmt_1281[] = "(\002*\002,10x,\002 atmospheric model identity\
 :  \002,t79,\002*\002,/,\002*\002,12x,\002 user defined water content : uh2\
o=\002,f6.3,\002 g/cm2 \002,t79,\002*\002,/,\002*\002,12x,\002 user defined \
ozone content : uo3 =\002,f6.3,\002 cm-atm\002,t79,\002*\002)";
    static char fmt_1272[] = "(\002*\002,\002 atmospheric model identity :\
 \002,t79,\002*\002,/,\002*\002,12x,\002 user defined atmospheric model  \
\002,t79,\002*\002,/,\002*\002,12x,\002*altitude  \002,\002*pressure  \002\
,\002*temp.     \002,\002*h2o dens. \002,\002*o3 dens.  \002,t79,\002*\002)";
    static char fmt_1271[] = "(\002*\002,12x,5e11.4,t79,\002*\002)";
    static char fmt_1261[] = "(\002*\002,10x,\002 atmospheric model identity\
 : \002,t79,\002*\002,/,\002*\002,15x,a51,t79,\002*\002)";
    static char fmt_133[] = "(\002*\002,10x,\002 aerosols type identity :\
\002,t79,\002*\002,/,\002*\002,15x,\002 user defined aerosols model \002,t79,\
\002*\002,/,\002*\002,26x,f6.3,\002 % of dust-like\002,t79,\002*\002,/,\002\
*\002,26x,f6.3,\002 % of water-soluble\002,t79,\002*\002,/,\002*\002,26x,f6.\
3,\002 % of oceanic\002,t79,\002*\002,/,\002*\002,26x,f6.3,\002 % of soot\
\002,t79,\002*\002)";
    static char fmt_134[] = "(\002*\002,10x,\002 aerosols type identity :\
\002,t79,\002*\002,/,\002*\002,15x,\002 user defined aerosols model \002,t79,\
\002*\002,/,\002*\002,15x,\002using \002,i1,\002 Log-normal size-distributio\
n(s)\002,t79,\002*\002,/,\002*\002,15x,\002Mean radius  Stand. Dev.  Percent\
. dencity\002,t79,\002*\002)";
    static char fmt_135[] = "(\002*\002,t41,f6.4,t55,f5.3,t69,e8.3,t79,\002\
*\002)";
    static char fmt_136[] = "(\002*\002,10x,\002 aerosols type identity :\
\002,t79,\002*\002,/,\002*\002,15x,\002 user defined aerosols model \002,t79,\
\002*\002,/,\002*\002,15x,\002using a Modified Gamma size-distribution\002,t\
79,\002*\002,/,\002*\002,19x,\002Alpha         b             Gamma\002,t79\
,\002*\002,/,\002*\002,t20,f6.3,t31,f6.3,t47,f6.3,t79,\002*\002)";
    static char fmt_137[] = "(\002*\002,10x,\002 aerosols type identity :\
\002,t79,\002*\002,/,\002*\002,15x,\002 user defined aerosols model \002,t79,\
\002*\002,/,\002*\002,15x,\002using a Power law size-distribution with alpha=\
\002,f3.1,t79,\002*\002)";
    static char fmt_132[] = "(\002*\002,10x,\002 aerosols type identity :\
\002,t79,\002*\002,/,\002*\002,15x,a15,\002 aerosols model\002,t79,\002*\002)"
	    ;
    static char fmt_138[] = "(\002*\002,10x,\002 aerosols type identity :\
\002,t79,\002*\002,/,\002*\002,15x,\002 user defined aerosols model \002,t79,\
\002*\002,/,\002*\002,15x,\002using data from the file:\002,t79,\002*\002,/\
,\002*\002,t25,a30,t79,\002*\002)";
    static char fmt_139[] = "(\002*\002,15x,\002 results saved into the fi\
le:\002,t79,\002*\002,/,\002*\002,t25,a30,t79,\002*\002)";
    static char fmt_1301[] = "(\002*\002,10x,\002 aerosols type identity \
:\002,t79,\002*\002,/,\002*\002,15x,\002 no aerosols computed   \002,t79,\
\002*\002)";
    static char fmt_140[] = "(\002*\002,10x,\002 optical condition identit\
y :\002,t79,\002*\002,/,\002*\002,15x,\002 user def. opt. thick. at 550nm\
\002:,f7.4,t79,\002*\002,/,\002*\002,t79,\002*\002)";
    static char fmt_141[] = "(\002*\002,10x,\002 optical condition identit\
y :\002,t79,\002*\002,/,\002*\002,15x,\002 visibility :\002,f6.2,\002 km \
\002,\002 opt. thick. 550nm :\002,f7.4,t79,\002*\002,/,\002*\002,t79,\002\
*\002)";
    static char fmt_148[] = "(\002*\002,22x,\002 spectral condition  \002,t7\
9,\002*\002,/,\002*\002,22x,\002 ------------------  \002,t79,\002*\002)";
    static char fmt_1510[] = "(\002*\002,10x,a17,t79,\002*\002,/,\002*\002,1\
5x,\002value of filter function :\002,t79,\002*\002,/,\002*\002,15x,\002 wl \
inf=\002,f6.3,\002 mic\002,2x,\002 wl sup=\002,f6.3,\002 mic\002,t79,\002\
*\002)";
    static char fmt_149[] = "(\002*\002,12x,\002 monochromatic calculation a\
t wl :\002,f6.3,\002 micron \002,t79,\002*\002)";
    static char fmt_169[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002 t\
arget type  \002,t79,\002*\002,/,\002*\002,22x,\002 -----------  \002,t79\
,\002*\002,/,\002*\002,10x,\002 inhomogeneous ground , radius of target \002\
,f6.3,\002 km  \002,t79,\002*\002)";
    static char fmt_170[] = "(\002*\002,15x,\002 target reflectance : \002,t\
79,\002*\002)";
    static char fmt_171[] = "(\002*\002,15x,\002 environmental reflectance\
 : \002,t79,\002*\002)";
    static char fmt_168[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002 t\
arget type  \002,t79,\002*\002,/,\002*\002,22x,\002 -----------  \002,t79\
,\002*\002,/,\002*\002,10x,\002 homogeneous ground \002,t79,\002*\002)";
    static char fmt_190[] = "(\002*\002,15x,\002 brdf from in-situ measureme\
nts\002,t79,\002*\002)";
    static char fmt_187[] = "(\002*\002,t79,\002*\002,/,\002*\002,15x,\002 b\
rdf selected                    \002,t79,\002*\002,/,\002*\002,15x,\002     \
rodir    robar    ropbar    albedo \002,t79,\002*\002,/,\002*\002,15x,4(f9.4\
,1x),t79,\002*\002)";
    static char fmt_191[] = "(\002*\002,15x,\002 Hapke's model selected\002,\
t79,\002*\002/,\002*\002,16x,\002om:\002,f5.3,1x,\002af:\002,f5.3,1x,\002s0\
:\002,f5.3,1x,\002h:\002,f5.3,t79,\002*\002)";
    static char fmt_192[] = "(\002*\002,15x,\002 Pinty and Verstraete's mode\
l selected\002,t79,\002*\002/,\002*\002,16x,\002om:\002,f5.3,1x,\002rad :\
\002,f5.3,1x,\002lad  :\002,f5.3,1x,t79,\002*\002)";
    static char fmt_200[] = "(\002*\002,15x,\002 single scattering only     \
         :  \002,t79,\002*\002)";
    static char fmt_201[] = "(\002*\002,15x,\002 multiple scattering (Dickin\
son et al)  \002,t79,\002*\002)";
    static char fmt_197[] = "(\002*\002,15x,\002 given kappa1 and kappa2:   \
             \002,t79,\002*\002,/,\002*\002,20x,\002kpa1:\002,f5.3,1x,\002kp\
a2:\002,f5.3,t79,\002*\002)";
    static char fmt_198[] = "(\002*\002,15x,\002 Goudrian's parametrization \
of kappa :   \002,t79,\002*\002,/,\002*\002,20x,\002 ksil:\002,f5.3,1x,t79\
,\002*\002)";
    static char fmt_199[] = "(\002*\002,15x,\002 modified Goudrian's paramet\
rization :   \002,t79,\002*\002,/,\002*\002,20x,\002 ksil:\002,f5.3,1x,t79\
,\002*\002)";
    static char fmt_202[] = "(\002*\002,15x,\002 isotropic phase function   \
         :  \002,t79,\002*\002)";
    static char fmt_203[] = "(\002*\002,15x,\002 Heyney-Greenstein's phase f\
unction  :  \002,t79,\002*\002,/,\002*\002,20x,\002assym:\002,f5.3,1x,t79\
,\002*\002)";
    static char fmt_204[] = "(\002*\002,15x,\002 Legendre polynomial phase f\
unction  :  \002,t79,\002*\002,/,\002*\002,20x,\002beta1:\002,f5.3,1x,\002be\
ta2:\002,f5.3,t79,\002*\002)";
    static char fmt_193[] = "(\002*\002,15x,\002 Roujean et al.'s model sele\
cted\002,t79,\002*\002/,\002*\002,16x,\002k0:\002,f5.3,1x,\002k1:\002,f5.3,1\
x,\002k2:\002,f5.3,t79,\002*\002)";
    static char fmt_194[] = "(\002*\002,15x,\002 Walthall et al.'s model sel\
ected\002,t79,\002*\002/,\002*\002,16x,\002a:\002,f5.3,1x,\002ap:\002,f5.3,1\
x,\002b:\002,f5.3,1x,\002om:\002,f5.3,t79,\002*\002)";
    static char fmt_195[] = "(\002*\002,15x,\002 Minnaert's model selecte\
d\002,t79,\002*\002/,\002*\002,16x,\002par1:\002,f5.3,1x,\002par2:\002,f5.3,\
t79,\002*\002)";
    static char fmt_196[] = "(\002*\002,15x,\002 ocean model selected\002,t7\
9,\002*\002/,\002*\002,16x,\002wind speed [m/s] :\002,f5.1,2x,\002azimuth of\
 the wind [deg] :\002,f8.2,t79,\002*\002/,\002*\002,16x,\002salinity [ppt] \
:\002,f5.1,4x,\002pigment conc. [mg/m3] :\002,f6.2,t79,\002*\002)";
    static char fmt_205[] = "(\002*\002,15x,\002 Iaquinta and Pinty BRDF mod\
el selected \002,t79,\002*\002,/,\002*\002,16x,\002Rl:\002,f5.3,1x,\002Tl\
:\002,f5.3,1x,\002Rs:\002,f5.3,1x,1x,\002LAl:\002,f5.3,t79,\002*\002)";
    static char fmt_207[] = "(\002*\002,15x,a19,t79,\002*\002)";
    static char fmt_208[] = "(\002*\002,15x,a19,1x,f5.2,t79,\002*\002)";
    static char fmt_209[] = "(\002*\002,15x,a31,t79,\002*\002)";
    static char fmt_206[] = "(\002*\002,15x,\002 Rahman et al. model selecte\
d \002,t79,\002*\002,/,\002*\002,16x,\002Rho0\002:,f6.3,1x,\002af\002:,f6.3,\
1x,\002xk:\002,f6.3,1x,t79,\002*\002)";
    static char fmt_210[] = "(\002*\002,2x,\002 Kuusk BRDF model,           \
           \002,t79,\002*\002,/,\002*\002,12x,\002LAI:\002,f5.3,2x,\002eps\
:\002,f6.4,2x,\002thm:\002,f4.1,1x,\002sl:\002,f4.2,t79,\002*\002,/,\002*\
\002,12x,\002cAB:\002,f6.2,1x,\002cW:\002,f5.3,1x,\002N:\002,f5.3,1x,\002cn\
:\002,f4.2,1x,\002rsl1:\002,f5.3,t79,\002*\002)";
    static char fmt_173[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002 t\
arget elevation description \002,t79,\002*\002,/,\002*\002,22x,\002 --------\
-------------------- \002,t79,\002*\002)";
    static char fmt_174[] = "(\002*\002,10x,\002 ground pressure  [mb]\002,1\
x,f7.2,1x,t79,\002*\002)";
    static char fmt_175[] = "(\002*\002,10x,\002 ground altitude  [km]\002,f\
6.3,1x,t79,\002*\002)";
    static char fmt_176[] = "(\002*\002,15x,\002 gaseous content at target l\
evel: \002,t79,\002*\002,/,\002*\002,15x,\002 uh2o=\002,f6.3,\002 g/cm2 \002\
,5x,\002  uo3=\002,f6.3,\002 cm-atm\002,t79,\002*\002)";
    static char fmt_178[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002 p\
lane simulation description \002,t79,\002*\002,/,\002*\002,22x,\002 --------\
-------------------- \002,t79,\002*\002)";
    static char fmt_179[] = "(\002*\002,10x,\002 plane  pressure          [m\
b] \002,f7.2,1x,t79,\002*\002)";
    static char fmt_180[] = "(\002*\002,10x,\002 plane  altitude absolute [k\
m] \002,f6.3,1x,t79,\002*\002)";
    static char fmt_181[] = "(\002*\002,15x,\002 atmosphere under plane desc\
ription: \002,t79,\002*\002)";
    static char fmt_182[] = "(\002*\002,15x,\002 ozone content           \
 \002,f6.3,1x,t79,\002*\002)";
    static char fmt_183[] = "(\002*\002,15x,\002 h2o   content           \
 \002,f6.3,1x,t79,\002*\002)";
    static char fmt_184[] = "(\002*\002,15x,\002aerosol opt. thick. 550nm\
 \002,f6.3,1x,t79,\002*\002)";
    static char fmt_177[] = "(\002*\002,t79,\002*\002,/,\002*\002,23x,\002 a\
tmospheric correction activated \002,t79,\002*\002,/,\002*\002,23x,\002 ----\
---------------------------- \002,t79,\002*\002)";
    static char fmt_185[] = "(\002*\002,10x,\002 input apparent reflectance \
: \002,f6.3,t79,\002*\002)";
    static char fmt_186[] = "(\002*\002,10x,\002 input measured radiance [w/\
m2/sr/mic] \002,f7.3,t79,\002*\002)";
    static char fmt_172[] = "(\002*\002,t79,\002*\002,/,79(\002*\002),///)";
    static char fmt_1500[] = "(\002*\002,1x,\002wave   total  total  total  \
total  atm.   \002,\002swl    step   sbor   dsol   toar \002,t79,\002*\002,/,\
\002*\002,1x,\002       gas    scat   scat   spheri intr   \002,t79,\002*\
\002,/,\002*\002,1x,\002       trans  down   up     albedo refl   \002,t79\
,\002*\002)";
    static char fmt_1501[] = "(\002*\002,6(f6.4,1x),f6.1,1x,4(f6.4,1x),t79\
,\002*\002)";
    static char fmt_430[] = "(79(\002*\002),/,\002*\002,t79,\002*\002,/\
,\002*\002,24x,\002 integrated values of  :   \002,t79,\002*\002,/,\002*\002\
,24x,\002 --------------------      \002,t79,\002*\002,/,\002*\002,6x,\002 a\
pparent reflectance \002,f7.4,3x,\002 appar. rad.(w/m2/sr/mic) \002,f10.3,1x\
,t79,\002*\002,/,\002*\002,18x,\002 total gaseous transmittance  \002,f5.3,t\
79,\002*\002,/,\002*\002,t79,\002*\002,/,79(\002*\002))";
    static char fmt_431[] = "(\002*\002,t79,\002*\002,/,\002*\002,24x,\002 c\
oupling aerosol -wv  :   \002,t79,\002*\002,/,\002*\002,24x,\002 -----------\
---------      \002,t79,\002*\002,/,\002*\002,10x,\002 wv above aerosol :\
 \002,f7.3,4x,\002 wv mixed with aerosol : \002,f7.3,1x,t79,\002*\002,/,\002*\
\002,22x,\002 wv under aerosol : \002,f7.3,t79,\002*\002)";
    static char fmt_432[] = "(\002*\002,t79,\002*\002,/,\002*\002,24x,\002 i\
nt. normalized  values  of  : \002,t79,\002*\002,/,\002*\002,24x,\002 ------\
---------------------    \002,t79,\002*\002,/,\002*\002,22x,\002% of irradia\
nce at ground level\002,t79,\002*\002,/,\002*\002,5x,\002% of direct  irr\
.\002,4x,\002% of diffuse irr.\002,4x,\002% of enviro. irr \002,t79,\002*\
\002,/,\002*\002,3(10x,f10.3),t79,\002*\002,/,\002*\002,22x,\002 reflectance\
 at satellite level\002,t79,\002*\002,/,\002*\002,5x,\002atm. intrin. ref\
.\002,3x,a11,\002 ref.\002,2x,a6,\002 reflectance\002,t79,\002*\002,/,\002\
*\002,3(10x,f10.3),t79,\002*\002,/,\002*\002,t79,\002*\002)";
    static char fmt_434[] = "(\002*\002,24x,\002 int. absolute values of\002\
,t79,\002*\002,/,\002*\002,24x,\002 -----------------------\002,t79,\002*\
\002,/,\002*\002,22x,\002irr. at ground level (w/m2/mic)  \002,t79,\002*\002\
,/,\002*\002,5x,\002direct solar irr.\002,4x,\002atm. diffuse irr.\002,4x\
,\002environment  irr \002,t79,\002*\002,/,\002*\002,3(10x,f10.3),t79,\002\
*\002,/,\002*\002,22x,\002rad at satel. level (w/m2/sr/mic)\002,t79,\002*\
\002,/,\002*\002,5x,\002atm. intrin. rad.\002,4x,a11,\002 rad.\002,4x,a6,\
\002 radiance\002,t79,\002*\002,/,\002*\002,3(10x,f10.3),t79,\002*\002,/,\
\002*\002,t79,\002*\002)";
    static char fmt_436[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002so\
l. spect (in w/m2/mic)\002,t79,\002*\002,/,\002*\002,30x,f10.3,t79,\002*\002\
,/,\002*\002,t79,\002*\002,/,79(\002*\002))";
    static char fmt_437[] = "(\002*\002,t79,\002*\002,/,\002*\002,10x,\002in\
t. funct filter (in mic)   \002,10x,\002 int. sol. spect (in w/m2)\002,t79\
,\002*\002,/,\002*\002,10x,f12.7,30x,f10.3,t79,\002*\002,/,\002*\002,t79,\
\002*\002,/,79(\002*\002))";
    static char fmt_929[] = "(\002 \002,////)";
    static char fmt_930[] = "(79(\002*\002),/,\002*\002,t79,\002*\002,/\
,\002*\002,t27,\002 integrated values of  :   \002,t79,\002*\002,/,\002*\002\
,t27,\002 --------------------      \002,t79,\002*\002,/,\002*\002,t79,\002\
*\002,/,\002*\002,t30,\002 downward \002,t45,\002  upward  \002,t60,\002   t\
otal  \002,t79,\002*\002)";
    static char fmt_931[] = "(\002*\002,6x,a20,t32,f7.5,t47,f7.5,t62,f7.5,t7\
9,\002*\002)";
    static char fmt_939[] = "(\002*\002,t79,\002*\002,/,\002*\002,t30,\002 r\
ayleigh \002,t45,\002 aerosols \002,t60,\002   total  \002,t79,\002*\002,/\
,\002*\002,t79,\002*\002)";
    static char fmt_1402[] = "(\002*\002,t79,\002*\002,/,79(\002*\002))";
    static char fmt_940[] = "(79(\002*\002),/,/,/,/,79(\002*\002),/\002*\002\
,23x,\002 atmospheric correction result \002,t79,\002*\002,/,\002*\002,23x\
,\002 ----------------------------- \002,t79,\002*\002)";
    static char fmt_941[] = "(\002*\002,6x,\002 input apparent reflectance  \
          :\002,1x,f8.3,t79,\002*\002)";
    static char fmt_942[] = "(\002*\002,6x,\002 measured radiance [w/m2/sr/m\
ic]       :\002,1x,f8.3,t79,\002*\002)";
    static char fmt_943[] = "(\002*\002,6x,\002 atmospherically corrected re\
flectance :\002,1x,f8.3,t79,\002*\002)";
    static char fmt_944[] = "(\002*\002,6x,\002 coefficients xa xb xc       \
          :\002,1x,3(f8.5,1x),t79,\002*\002,/,\002*\002,6x,\002 y=xa*(measur\
ed radiance)-xb;  acr=y/(1.+xc*y)\002,t79,\002*\002,/,79(\002*\002))";

    /* System generated locals */
    address a__1[2];
    integer i__1, i__2[2];
    real r__1, r__2;
    doublereal d__1;
    cilist ci__1;

    /* Builtin functions */
    /* Subroutine */ /*int s_copy(char *, char *, ftnlen, ftnlen);*/
    double acos(doublereal), cos(doublereal);
    /* Subroutine */ int s_stop(char *, ftnlen);
    double sqrt(doublereal);
    integer s_rsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_rsle();
    double pow_dd(doublereal *, doublereal *), log(doublereal);
    integer s_rsfe(cilist *), do_fio(integer *, char *, ftnlen), e_rsfe(), 
	    i_indx(char *, char *, ftnlen, ftnlen);
    /* Subroutine */ /*int s_cat(char *, char **, integer *, integer *, ftnlen);*/
    double exp(doublereal);
    integer s_wsfe(cilist *), e_wsfe();

    /* Local variables */
    real phi_wind__, brdfints[2499]	/* was [51][49] */;
    extern /* Subroutine */ int pressure_(real *, real *, real *);
    real c__[4];
    integer i__, j, k, l, n;
    real v;
    integer i1, i2;
    extern /* Subroutine */ int presplane_(real *, real *, real *, real *);
    real gb[51], pc;
    integer ik;
    real sb, gp[49], fr, pi, es, xa, xb, xc;
    integer np;
    real rm[51], ro, rp[49], wl;
    integer nt, mu;
    real tu, uw, ea0, ee0, pi2;
    integer mu2;
    real uo3;
    extern /* Subroutine */ int specinterp_(real *, real *, real *, real *, 
	    real *, real *, real *);
    real fae, eei, ani[6]	/* was [2][3] */, rad, seb, fra, sha, pcl, 
	    anr[6]	/* was [2][3] */, phi, roc, roe, etn, rog, avr, esn, 
	    xle, prl, uli, pps, ptl, prs, swl, xpp, xlt, pws, xps, puw, sli, 
	    cwi;
    extern /* Subroutine */ int us62_();
    real ea0n, ee0n, phi0, par1, par2, par3, par4, xla0, tgp1, tgp2;
    integer mum1;
    real xlm1[2499]	/* was [51][49] */, xlm2[2499]	/* was [51][49] */, 
	    puo3, cabi, adif, phaa, coef;
    char file[80];
    real aini[6]	/* was [2][3] */;
    integer iaer, iinf;
    real tdif, sasa, sham, ainr[6]	/* was [2][3] */, phar, taer, asol, 
	    rocl[1501], roel[1501], avis, dsol, phiv, rapp, xlat, sbor, step, 
	    sasr, xmud, sast, tsca, xlon, tray, tdir, xlen, xltn, xrad;
    integer jday;
    real xmup;
    integer isup;
    real xmus, accu2, accu3, xmuv;
    char file2[80];
    real uwus, palt;
    integer pild, pihs;
    real pxlt, xsal, thmi, vaii, rnci, puoz, xla0n, ratm1, ratm2, ratm3, 
	    rsl1i, edifa, uo3us;
    integer iread, ibrdf;
    real dgasm, asaer, edifr, tdifd, tmdif;
    integer igeom;
    real robar[1501], taer55, refet, tgasm, ugasm, taerp, asray, dtota, tamoy,
	     astot, utota, romix, trayp, dtott, dtotr, dgtot, tgtot, ugtot, 
	    tdird, tdiru, wlmoy, tdifu, trmoy, utotr, rsurf, utott, tmdir;
    integer month, idatm;
    real puwus;
    integer iaerp, iwave;
    real robar1, robar2, refet1, refet2, refet3;
    integer isort;
    real ftray;
    extern /* Subroutine */ int gauss_(real *, real *, real *, real *, 
	    integer *);
    integer igrou1, igrou2;
    real xnorm1, xnorm2, sddica, dtdica, puo3us;
    integer idirec;
    real anglem[48], phirad, sdmoca, robard[1501], sudica, stdica, sodaer, 
	    dtmoca, rocave, fophsa, roeave, ssdaer, utdica, sdmeth, robarp[
	    1501], taer55p, alumet, sdwava, sumoca, stmoca, sroaer, dtmeth, 
	    sdtota, sodray, fophsr, utmoca, sdniox, sumeth, suwava, stwava, 
	    stmeth, tamoyp, dtniox, sdozon, sdoxyg, sodtot, sroray, sdtotr, 
	    sdtott, sutota, dtozon, stniox, suniox, dtoxyg, pizmoy, utniox, 
	    utmeth, trmoyp, srotot, stozon, suozon, suoxyg, stoxyg, sutotr, 
	    utozon, sutott, utoxyg, ttozon, ttdica, ttoxyg, ttniox, ttmeth, 
	    ttmoca, dtwava, utwava, ttwava, rorayl, roaero, romeas1, romeas2, 
	    romeas3, pizera, fophst, pizerr, pizert;
    integer idatmp, inhomo, igroun;
    real optics[3], albbrdf, struct__[4];
    extern /* Subroutine */ int varsol_(integer *, integer *, real *), 
	    aeroso_(integer *, real *, real *, real *, char *, ftnlen), 
	    discom_(integer *, integer *, real *, real *, real *, real *, 
	    real *, real *, real *, integer *, integer *, integer *, real *, 
	    real *, real *, real *, real *, real *), odrayl_(real *, real *), 
	    solirr_(real *, real *), abstra_(integer *, real *, real *, real *
	    , real *, real *, real *, real *, integer *, real *, real *, real 
	    *, real *, real *, real *, real *, real *, real *, real *, real *,
	     real *, real *, real *, real *, real *, real *, real *, real *, 
	    real *, real *, real *, real *, real *, real *), interp_(integer *
	    , integer *, real *, real *, real *, real *, real *, real *, real 
	    *, real *, real *, real *, real *, real *, real *, real *, real *,
	     real *, real *, real *, real *, real *, real *, real *, real *), 
	    enviro_(real *, real *, real *, real *, real *, real *, real *, 
	    real *);
    real alumeas, sodaerp, weightm[48], attwava, sodrayp;
    integer options[5];
    real sodtotp;
    extern /* Subroutine */ int equivwl_(integer *, integer *, real *, real *)
	    ;
    real pizmoyp;

    /* Fortran I/O blocks */
    static cilist io___52 = { 0, 0, 0, 0, 0 };
    static cilist io___55 = { 0, 0, 0, 0, 0 };
    static cilist io___56 = { 0, 5, 0, 0, 0 };
    static cilist io___57 = { 0, 5, 0, 0, 0 };
    static cilist io___58 = { 0, 5, 0, 0, 0 };
    static cilist io___59 = { 0, 0, 0, 0, 0 };
    static cilist io___60 = { 0, 0, 0, 0, 0 };
    static cilist io___61 = { 0, 5, 0, 0, 0 };
    static cilist io___62 = { 0, 5, 0, 0, 0 };
    static cilist io___63 = { 0, 0, 0, 0, 0 };
    static cilist io___64 = { 0, 0, 0, 0, 0 };
    static cilist io___65 = { 0, 5, 0, 0, 0 };
    static cilist io___66 = { 0, 5, 0, 0, 0 };
    static cilist io___67 = { 0, 5, 0, 0, 0 };
    static cilist io___68 = { 0, 5, 0, 0, 0 };
    static cilist io___69 = { 0, 5, 0, 0, 0 };
    static cilist io___70 = { 0, 5, 0, 0, 0 };
    static cilist io___72 = { 0, 5, 0, 0, 0 };
    static cilist io___89 = { 0, 0, 0, 0, 0 };
    static cilist io___93 = { 0, 0, 0, 0, 0 };
    static cilist io___115 = { 0, 0, 0, fmt_98, 0 };
    static cilist io___116 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___117 = { 0, 0, 0, fmt_103, 0 };
    static cilist io___118 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___122 = { 0, 0, 0, fmt_102, 0 };
    static cilist io___123 = { 0, 0, 0, fmt_1110, 0 };
    static cilist io___124 = { 0, 0, 0, fmt_1119, 0 };
    static cilist io___125 = { 0, 0, 0, fmt_1281, 0 };
    static cilist io___126 = { 0, 0, 0, fmt_1272, 0 };
    static cilist io___127 = { 0, 0, 0, fmt_1271, 0 };
    static cilist io___128 = { 0, 0, 0, fmt_1261, 0 };
    static cilist io___129 = { 0, 0, 0, fmt_133, 0 };
    static cilist io___130 = { 0, 0, 0, fmt_134, 0 };
    static cilist io___131 = { 0, 0, 0, fmt_135, 0 };
    static cilist io___132 = { 0, 0, 0, fmt_136, 0 };
    static cilist io___133 = { 0, 0, 0, fmt_137, 0 };
    static cilist io___134 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___135 = { 0, 0, 0, fmt_138, 0 };
    static cilist io___136 = { 0, 0, 0, fmt_139, 0 };
    static cilist io___137 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___138 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___139 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___140 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___141 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___142 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___143 = { 0, 0, 0, fmt_1301, 0 };
    static cilist io___144 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___145 = { 0, 0, 0, fmt_140, 0 };
    static cilist io___146 = { 0, 0, 0, fmt_141, 0 };
    static cilist io___147 = { 0, 0, 0, fmt_148, 0 };
    static cilist io___148 = { 0, 0, 0, fmt_1510, 0 };
    static cilist io___149 = { 0, 0, 0, fmt_149, 0 };
    static cilist io___150 = { 0, 0, 0, fmt_1510, 0 };
    static cilist io___158 = { 0, 0, 0, fmt_169, 0 };
    static cilist io___161 = { 0, 0, 0, fmt_170, 0 };
    static cilist io___163 = { 0, 0, 0, fmt_171, 0 };
    static cilist io___164 = { 0, 0, 0, fmt_168, 0 };
    static cilist io___165 = { 0, 0, 0, fmt_168, 0 };
    static cilist io___167 = { 0, 0, 0, fmt_190, 0 };
    static cilist io___168 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___175 = { 0, 0, 0, fmt_191, 0 };
    static cilist io___180 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___181 = { 0, 0, 0, fmt_192, 0 };
    static cilist io___185 = { 0, 0, 0, fmt_200, 0 };
    static cilist io___186 = { 0, 0, 0, fmt_201, 0 };
    static cilist io___187 = { 0, 0, 0, fmt_197, 0 };
    static cilist io___188 = { 0, 0, 0, fmt_198, 0 };
    static cilist io___189 = { 0, 0, 0, fmt_199, 0 };
    static cilist io___190 = { 0, 0, 0, fmt_202, 0 };
    static cilist io___191 = { 0, 0, 0, fmt_203, 0 };
    static cilist io___192 = { 0, 0, 0, fmt_204, 0 };
    static cilist io___193 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___194 = { 0, 0, 0, fmt_193, 0 };
    static cilist io___195 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___196 = { 0, 0, 0, fmt_194, 0 };
    static cilist io___197 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___198 = { 0, 0, 0, fmt_195, 0 };
    static cilist io___199 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___200 = { 0, 0, 0, fmt_196, 0 };
    static cilist io___205 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___206 = { 0, 0, 0, fmt_205, 0 };
    static cilist io___212 = { 0, 0, 0, fmt_207, 0 };
    static cilist io___213 = { 0, 0, 0, fmt_208, 0 };
    static cilist io___216 = { 0, 0, 0, fmt_209, 0 };
    static cilist io___217 = { 0, 0, 0, fmt_209, 0 };
    static cilist io___218 = { 0, 0, 0, fmt_209, 0 };
    static cilist io___219 = { 0, 0, 0, fmt_209, 0 };
    static cilist io___220 = { 0, 0, 0, fmt_209, 0 };
    static cilist io___221 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___222 = { 0, 0, 0, fmt_206, 0 };
    static cilist io___223 = { 0, 0, 0, fmt_210, 0 };
    static cilist io___233 = { 0, 0, 0, fmt_187, 0 };
    static cilist io___234 = { 0, 0, 0, fmt_173, 0 };
    static cilist io___235 = { 0, 0, 0, fmt_174, 0 };
    static cilist io___236 = { 0, 0, 0, fmt_175, 0 };
    static cilist io___237 = { 0, 0, 0, fmt_176, 0 };
    static cilist io___238 = { 0, 0, 0, fmt_178, 0 };
    static cilist io___239 = { 0, 0, 0, fmt_179, 0 };
    static cilist io___240 = { 0, 0, 0, fmt_180, 0 };
    static cilist io___241 = { 0, 0, 0, fmt_181, 0 };
    static cilist io___242 = { 0, 0, 0, fmt_182, 0 };
    static cilist io___243 = { 0, 0, 0, fmt_183, 0 };
    static cilist io___244 = { 0, 0, 0, fmt_184, 0 };
    static cilist io___245 = { 0, 0, 0, fmt_177, 0 };
    static cilist io___246 = { 0, 0, 0, fmt_185, 0 };
    static cilist io___247 = { 0, 0, 0, fmt_186, 0 };
    static cilist io___248 = { 0, 0, 0, fmt_172, 0 };
    static cilist io___305 = { 0, 0, 0, fmt_1500, 0 };
    static cilist io___373 = { 0, 0, 0, fmt_1501, 0 };
    static cilist io___393 = { 0, 0, 0, fmt_430, 0 };
    static cilist io___394 = { 0, 0, 0, fmt_431, 0 };
    static cilist io___395 = { 0, 0, 0, fmt_432, 0 };
    static cilist io___396 = { 0, 0, 0, fmt_434, 0 };
    static cilist io___397 = { 0, 0, 0, fmt_432, 0 };
    static cilist io___398 = { 0, 0, 0, fmt_434, 0 };
    static cilist io___399 = { 0, 0, 0, fmt_436, 0 };
    static cilist io___400 = { 0, 0, 0, fmt_437, 0 };
    static cilist io___401 = { 0, 0, 0, fmt_929, 0 };
    static cilist io___402 = { 0, 0, 0, fmt_930, 0 };
    static cilist io___403 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___404 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___405 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___406 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___407 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___408 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___409 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___410 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___411 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___412 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___413 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___414 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___415 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___416 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___417 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___418 = { 0, 0, 0, fmt_939, 0 };
    static cilist io___419 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___420 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___421 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___422 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___424 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___427 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___428 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___429 = { 0, 0, 0, fmt_1402, 0 };
    static cilist io___434 = { 0, 0, 0, fmt_940, 0 };
    static cilist io___435 = { 0, 0, 0, fmt_941, 0 };
    static cilist io___436 = { 0, 0, 0, fmt_942, 0 };
    static cilist io___437 = { 0, 0, 0, fmt_943, 0 };
    static cilist io___438 = { 0, 0, 0, fmt_944, 0 };


/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*       ********************************************************       c 
*/
/*       *           second simulation of satellite signal      *       c 
*/
/*       *                 in the solar spectrum                *       c 
*/
/*       *           ... (6s) ....... (6s) ...... (6s) ...      *       c 
*/
/*       *                 .... (6s) ...... (6s)...             *       c 
*/
/*       *                     ...... (6s) ......               *       c 
*/
/*       *                        version  4.1                  *       c 
*/
/*       *                                                      *       c 
*/
/*       *  this code predicts the satellite signal from 0.25   *       c 
*/
/*       *  to 4.0 microns assuming cloudless atmosphere.       *       c 
*/
/*       *  the main atmospheric effects (gaseous absorption    *       c 
*/
/*       *  by water vapor,carbon dioxyde,oxygen and ozone;     *       c 
*/
/*       *  scattering by molecules and aerosols) are taken     *       c 
*/
/*       *  into account. non-uniform surfaces may be           *       c 
*/
/*       *  considered,as well as bidirectional reflectances    *       c 
*/
/*       *            as boundary conditions                    *       c 
*/
/*       *                                                      *       c 
*/
/*       *   the following input parameters are needed          *       c 
*/
/*       *         geometrical conditions                       *       c 
*/
/*       *         atmospheric model for gaseous components     *       c 
*/
/*       *         aerosol model (type and concentration)       *       c 
*/
/*       *         spectral condition                           *       c 
*/
/*       *         ground reflectance (type and spectral var.)  *       c 
*/
/*       *   at each step, you can either select some proposed  *       c 
*/
/*       *  standard conditions (for example,spectral bands of  *       c 
*/
/*       *  satellite for spectral conditions) or define your   *       c 
*/
/*       *  own conditions(in the example,you have to define    *       c 
*/
/*       *  the assumed spectral response).                     *       c 
*/
/*       *                                                      *       c 
*/
/*       *   more details are given at each data input step     *       c 
*/
/*       *                                                      *       c 
*/
/*       ********************************************************       c 
*/
/*                                                                      c 
*/
/*                                                                      c 
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
/*                                                                      c 
*/
/*                                                                      c 
*/
/*       ********************************************************       c 
*/
/*       *             authors of this code are                 *       c 
*/
/*       *                                                      *       c 
*/
/*       *  (1) Vermote E.; (2) Tanre D.;(2) Deuze J.L.         *       c 
*/
/*       *           (2) Herman M.,(3) MOrcrette J.J..          *       c 
*/
/*       *                                                      *       c 
*/
/*       *                       from                           *       c 
*/
/*       *                                                      *       c 
*/
/*       *     (1) Affiliation: Department of Geography         *       c 
*/
/*       *         University of Maryland                       *       c 
*/
/*       *         address: Goddard Space Flight Center	       *       c 
*/
/*       *         Code 923 		      		       *       c */
/*       *         greenbelt, md 20771                          *       c 
*/
/*       *         USA                                          *       c 
*/
/*       *                                                      *       c 
*/
/*       *     (2) laboratoire d' optique atmospherique         *       c 
*/
/*       *         universite des sciences et techniques        *       c 
*/
/*       *         de lille                                     *       c 
*/
/*       *         u.e.r. de physique fondamentale              *       c 
*/
/*       *         59655 villeneuve d' ascq cedex               *       c 
*/
/*       *         france                                       *       c 
*/
/*       *                                                      *       c 
*/
/*       *     (3) e.c.m.w.f.                                   *       c 
*/
/*       *                                                      *       c 
*/
/*       *                                                      *       c 
*/
/*       *                                                      *       c 
*/
/*       *                                                      *       c 
*/
/*       ********************************************************       c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* **********************************************************************c
 */
/*       ********************************************************       c 
*/
/*       *                limits of validity                    *       c 
*/
/*       *                                                      *       c 
*/
/*       *   geometrical parameters    no limitations           *       c 
*/
/*       *                                                      *       c 
*/
/*       *   atmospheric model         no limitations           *       c 
*/
/*       *                                                      *       c 
*/
/*       *   aerosol model             the visibility must be   *       c 
*/
/*       *                             better than 5.0km        *       c 
*/
/*       *                             for smaller values       *       c 
*/
/*       *                             calculations might be    *       c 
*/
/*       *                             no more valid.           *       c 
*/
/*       *                                                      *       c 
*/
/*       *   spectral conditions       the gaseous transmittance*       c 
*/
/*       *                             and the scattering func  *       c 
*/
/*       *                             tions are valid from 0.25*       c 
*/
/*       *                             to 4.0 micron. but the   *       c 
*/
/*       *                             treatment of interaction *       c 
*/
/*       *                             between absorption and   *       c 
*/
/*       *                             scattering is correct for*       c 
*/
/*       *                             not too large absorption *       c 
*/
/*       *                             if you want to compute   *       c 
*/
/*       *                             signal within absorption *       c 
*/
/*       *                             bands,this interaction   *       c 
*/
/*       *                             ought to be reconsidered *       c 
*/
/*       *                                                      *       c 
*/
/*       *   ground reflectance (type) you can consider a patchy*       c 
*/
/*       *                             structure:that is a circu*       c 
*/
/*       *                             lar target of radius rad *       c 
*/
/*       *                             and of reflectance roc,  *       c 
*/
/*       *                             within an environnement  *       c 
*/
/*       *                             of reflectance roe.      *       c 
*/
/*       *                                                      *       c 
*/
/*       *   ground reflectance (type continued): for uniform   *       c 
*/
/*       *                             surface conditions only, *       c 
*/
/*       *                             you may consider directio*       c 
*/
/*       *                             nal reflectance as bounda*       c 
*/
/*       *                             ry conditions.           *       c 
*/
/*       *                             some analytical model are*       c 
*/
/*       *                             proposed, the user can   *       c 
*/
/*       *                             specify his own values.  *       c 
*/
/*       *                             the code assumes that the*       c 
*/
/*       *                             brdf is spectrally inde- *       c 
*/
/*       *                             pendent                  *       c 
*/
/*       *                                                      *       c 
*/
/*       *   ground reflectance (spectral variation) four typi  *       c 
*/
/*       *                             cal reflectances are pro *       c 
*/
/*       *                             posed, defined within    *       c 
*/
/*       *                             given spectral range.    *       c 
*/
/*       *                             this range differs accor *       c 
*/
/*       *                             ding to the selected case*       c 
*/
/*       *                             the reflectance is set to*       c 
*/
/*       *                             0 outside this range,due *       c 
*/
/*       *                             to the deficiency of data*       c 
*/
/*       *                             user must verify these   *       c 
*/
/*       *                             limits. that is obviously*       c 
*/
/*       *                             irrelevant for brdf      *       c 
*/
/*       *                                                      *       c 
*/
/*       ********************************************************       c 
*/
/* **********************************************************************c
 */
/************************************************************************
*****c*/
/* for considering brdf< we have to compute the downward radiance in the  
   c*/
/* whole hemisphere. to perform such computions, we selected the successiv
e  c*/
/* orders of scattering method. that method requires numerical integration
   c*/
/* over angles and optical depth. the integration method is the gauss meth
od,c*/
/* mu is the number of angles nmu+1, nmu is settled to 24. the accuracy of
   c*/
/* the computations is obviously depending on the nmu value. this value   
   c*/
/* can be easily changed as a parameter as well as the nt value which     
   c*/
/* is the number of layers for performing the vertical integration. the   
   c*/
/* downward radiance is computed for nmu values of the zenith angle and np
   c*/
/* values of the azimuth angle. the integration of the product of the     
   c*/
/* radiance by the brdf is so performed over the nmu*np values. np is sett
ledc*/
/* to 13, that value can be also changed. mu2 is equal to 2 times nmu.    
   c*/
/* xlmus is the downward radiance, xf the downward irradiance, rm and gb  
   c*/
/* the angles and the weights for the gauss integration over the zenith, r
p  c*/
/* and gp respectively for the azimuth integration.                       
   c*/
/************************************************************************
*****c*/
/*     parameter(nt_p=13,mu_p=13,mu2_p=24,np_p=13) */
/*<       parameter(nt_p=26,mu_p=25,mu2_p=48,np_p=49) >*/
/*<    >*/
/*<       dimension  xlmus(-mu_p:mu_p,np_p),xlmuv(-mu_p:mu_p,np_p) >*/
/*<    >*/
/*<         real anglem,weightm,rm,gb,accu2,accu3 >*/
/*<         real rp,gp,xlmus,xlmuv,angmu,angphi,brdfints,brdfdats >*/
/*<         real brdfintv,brdfdatv,robar,robarp,robard,xlm1,xlm2 >*/
/*<         real c,wldisc,ani,anr,aini,ainr,rocl,roel,zpl,ppl,tpl,whpl >*/
/*<         real wopl,xacc,phasel,pdgs,cgaus,pha,betal,s,wlinf,wlsup,delta >*/
/*<         real sigma,z,p,t,wh,wo,ext,ome,gasym,phase,roatm,dtdir >*/
/*<         real dtdif,utdir,utdif,sphal,wldis,trayl,traypl,pi,pi2,step >*/
/*<         real asol,phi0,avis,phiv,tu,xlon,xlat,xlonan,hna,dsol,campm >*/
/*<         real phi,phirad,xmus,xmuv,xmup,xmud,adif,uw,uo3,taer55 >*/
/*<         real taer,v,xps,uwus,uo3us,xpp,taer55p,puw,puo3,puwus >*/
/*<         real puo3us,wl,wlmoy,tamoy,tamoyp,pizmoy,pizmoyp,trmoy >*/
/*<         real trmoyp,fr,rad,spalt >*/
/*<         real albbrdf,par1,par2,par3,par4,robar1,xnorm1,rob,xnor,rodir >*/
/*<         real rdown,rdir,robar2,xnorm2,ro,roc,roe,rapp,rocave,roeave >*/
/*<         real seb,sbor,swl,sb,refet,refet1,refet2,refet3,alumet >*/
/*<         real tgasm,rog,dgasm,ugasm,sdwava,sdozon,sddica,sdoxyg >*/
/*<         real sdniox,sdmoca,sdmeth,suwava,suozon,sudica,suoxyg >*/
/*<         real suniox,sumoca,sumeth,stwava,stozon,stdica,stoxyg,stniox >*/
/*<         real stmoca,stmeth,sodray,sodaer,sodtot,fophsr,fophsa,sroray >*/
/*<         real sroaer,srotot,ssdaer,sdtotr,sdtota,sdtott,sutotr,sutota >*/
/*<         real sutott,sasr,sasa,sast,dtozon,dtdica,dtoxyg >*/
/*<         real dtniox,dtmeth,dtmoca,utozon,utdica,utoxyg,utniox >*/
/*<         real utmeth,utmoca,attwava,ttozon,ttdica,ttoxyg,ttniox >*/
/*<         real ttmeth,ttmoca,dtwava,utwava,ttwava,coef,romix,rorayl >*/
/*<         real roaero,phaa,phar,tsca,tray,trayp,taerp,dtott,utott >*/
/*<         real astot,asray,asaer,utotr,utota,dtotr,dtota,dgtot,tgtot >*/
/*<         real tgp1,tgp2 >*/
/*<         real ugtot,edifr,edifa,tdird,tdiru,tdifd,tdifu,fra >*/
/*<         real fae,avr,romeas1,romeas2,romeas3,alumeas,sodrayp >*/
/*<         real ratm1,ratm2,ratm3,rsurf >*/
/*<         real sodaerp,sodtotp,tdir,tdif,etn,esn,es,ea0n,ea0,ee0n >*/
/*<         real ee0,tmdir,tmdif,xla0n,xla0,xltn,xlt,xlen,xle,pizera >*/
/*<         real fophst,pizerr,pizert,xrad,xa,xb,xc >*/
/*<         real sha,sham >*/
/*<         integer nt,mu,mu2,np,k,iwr,mum1,idatmp >*/
/*<         integer j,iread,l,igeom,month,jday,nc,nl,idatm,iaer,iaerp,n >*/
/*<         integer iwave,iinf,isup,ik,i,inhomo,idirec,ibrdf,igroun >*/
/*<         integer igrou1,igrou2,isort >*/
/* _otb_adaptation Beginning: otb variables declaration */
/*<         real otb_asol,otb_phi0		!solar zenithal and azimutal angles (inp >*/
/*< 	real otb_avis,otb_phiv		!viewing zenithal and azimutal angles (input)  >*/
/*< 	integer otb_month,otb_jday 	!date (input) >*/
/*< 	real otb_pressure		!real pressure (input) >*/
/*< 	real otb_uw, otb_uo3		!real water vapor and ozone amounts (input) >*/
/*< 	integer otb_iaer		!number of aerosol model (input) >*/
/*< 	real otb_taer55			!aerosol optical thickness at 550-nm (input) >*/
/*< 	real otb_wlinf, otb_wlsup       !wavelength min and max for the spectral band (input) >*/
/*< 	real otb_s(1501)		!spectral band definition (input) >*/
/*< 	real otb_ratm			!Atmospheric reflectance (output) >*/
/*< 	real otb_sast			!Atmospheric spherical albedo (output) >*/
/*< 	real otb_tgasm			!Total gaseous transmission (output) >*/
/*< 	real otb_sdtott			!Downward transmittance (output) >*/
/*< 	real otb_sutott			!Upward transmittance (output) >*/
/* _otb_adaptation End : otb variables declaration */
/* ***********************************************************************
 */
/*                             return to 6s */
/* ***********************************************************************
 */
/*<       dimension c(4),wldisc(10),ani(2,3),anr(2,3),aini(2,3),ainr(2,3) >*/
/*<       dimension rocl(1501),roel(1501) >*/
/*<       real rn,ri,x1,x2,x3,cij,rsunph,nrsunph,rmax,rmin >*/
/*<       integer icp,irsunph,i1,i2 >*/
/*<       character etiq1(8)*60,nsat(61)*17,atmid(7)*51,reflec(8)*71 >*/
/*<       character FILE*80,FILE2*80 >*/
/*<       logical ier >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<    >*/
/* ***********************************************************************
 */
/*     for considering pixel and sensor  altitude */
/* ***********************************************************************
 */
/*<       real pps,palt,ftray >*/
/*<       common /sixs_planesim/zpl(34),ppl(34),tpl(34),whpl(34),wopl(34) >*/
/*<       common /sixs_test/xacc >*/
/* ***********************************************************************
 */
/*     for considering brdf */
/* ***********************************************************************
 */
/*<       common /sixs_sos/phasel(10,83),cgaus(83),pdgs(83) >*/
/*<       common /sixs_trunc/pha(83),betal(0:80) >*/
/*<       real optics(3),struct(4) >*/
/*<       integer options(5) >*/
/*<       integer pild,pihs >*/
/*<       real pxLt,pc,pRl,pTl,pRs >*/
/*<       real pws,phi_wind,xsal,pcl,paw >*/
/*<       real uli,eei,thmi,sli,cabi,cwi,vaii,rnci,rsl1i >*/
/* ***********************************************************************
 */
/*                             return to 6s */
/* ***********************************************************************
 */
/*<       common /sixs_ffu/s(1501),wlinf,wlsup >*/
/*<       common /sixs_del/ delta,sigma >*/
/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       common /sixs_aer/ext(10),ome(10),gasym(10),phase(10) >*/
/*<    >*/
/************************************************************************
*****c*/
/*  angmu and angphi are the angles were the brdf is measured. these value
s  c*/
/*  can be changed as soon as they are well distributed over the whole spa
ce c*/
/*  before the gauss integration, these values are interpolated to the gau
ss c*/
/*  angles                                                                
   c*/
/************************************************************************
*****c*/
/*<       data angmu /85.0,80.0,70.0,60.0,50.0,40.0,30.0,20.0,10.0,0.00/ >*/
    /* Parameter adjustments */
    --otb_s__;

    /* Function Body */
/*     data angmu /0.0,10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0,85.0/ */
/*<    >*/
/* ***********************************************************************
 */
/*                             return to 6s */
/* ***********************************************************************
 */
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<       FILE='  ' >*/
    s_copy(file, "  ", 80L, 2L);
/*<       FILE2='  ' >*/
    s_copy(file2, "  ", 80L, 2L);
/* ***********************************************************************
 */
/*   Parameters  initialization */
/* ***********************************************************************
 */
/*<       nt=nt_p >*/
    nt = 26;
/*<       mu=mu_p >*/
    mu = 25;
/*<       mu2=mu2_p >*/
    mu2 = 48;
/*<       np=np_p >*/
    np = 49;
/*<       iwr=6 >*/
    sixs_ier__1.iwr = 6;
/*<       ier=.FALSE. >*/
    sixs_ier__1.ier = FALSE_;
/*<       iinf=1 >*/
    iinf = 1;
/*<       isup=1501 >*/
    isup = 1501;
/* ***********************************************************************
 */
/*  preliminary computations for gauss integration */
/* ***********************************************************************
 */
/*<       pi=acos(-1.) >*/
    pi = acos((float)-1.);
/*<       pi2=2*pi >*/
    pi2 = pi * 2;
/*<       accu2=1.E-03 >*/
    accu2 = (float).001;
/*<       accu3=1.E-07 >*/
    accu3 = (float)1e-7;
/*<       do k=1,13 >*/
    for (k = 1; k <= 13; ++k) {
/*<        angphi(k)=angphi(k)*pi/180. >*/
	angphi[k - 1] = angphi[k - 1] * pi / (float)180.;
/*<       enddo >*/
    }
/*<       do k=1,10 >*/
    for (k = 1; k <= 10; ++k) {
/*<        angmu(k)=cos(angmu(k)*pi/180.) >*/
	angmu[k - 1] = cos(angmu[k - 1] * pi / (float)180.);
/*<       enddo >*/
    }
/*<       call gauss(-1.,1.,anglem,weightm,mu2) >*/
    gauss_(&c_b4, &c_b5, anglem, weightm, &mu2);
/*<       call gauss(0.,pi2,rp,gp,np) >*/
    gauss_(&c_b6, &pi2, rp, gp, &np);
/*<       mum1=mu-1 >*/
    mum1 = mu - 1;
/*<       do 581 j=-mum1,-1 >*/
    for (j = -mum1; j <= -1; ++j) {
/*<        k=mu+j >*/
	k = mu + j;
/*<        rm(-j-mu)=anglem(k) >*/
	rm[-j - mu + 25] = anglem[k - 1];
/*<        gb(-j-mu)=weightm(k) >*/
	gb[-j - mu + 25] = weightm[k - 1];
/*<   581 continue >*/
/* L581: */
    }
/*<       do 582 j=1,mum1 >*/
    i__1 = mum1;
    for (j = 1; j <= i__1; ++j) {
/*<        k=mum1+j >*/
	k = mum1 + j;
/*<        rm(mu-j)=anglem(k) >*/
	rm[mu - j + 25] = anglem[k - 1];
/*<        gb(mu-j)=weightm(k) >*/
	gb[mu - j + 25] = weightm[k - 1];
/*<   582 continue >*/
/* L582: */
    }
/*<       gb(-mu)=0. >*/
    gb[-mu + 25] = (float)0.;
/*<       gb(0)=0. >*/
    gb[25] = (float)0.;
/*<       gb(mu)=0. >*/
    gb[mu + 25] = (float)0.;
/* ***********************************************************************
 */
/*                             return to 6s */
/* ***********************************************************************
 */
/* constantes values */
/*<       sigma=0.056032 >*/
    sixs_del__1.sigma = (float).056032;
/*<       delta=0.0279 >*/
    sixs_del__1.delta = (float).0279;
/*<       xacc=1.e-06 >*/
    sixs_test__1.xacc = (float)1e-6;
/*<       iread=5 >*/
    iread = 5;
/*<       step=0.0025 >*/
    step = (float).0025;
/*<       do 1111 l=1,10 >*/
    for (l = 1; l <= 10; ++l) {
/*<        wldis(l)=wldisc(l) >*/
	sixs_disc__1.wldis[l - 1] = wldisc[l - 1];
/*<  1111 continue >*/
/* L1111: */
    }
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                                                *     sun             c 
*/
/*                      n. of column,n. of line.(full scale 17000*12000)c 
*/
/*                                                                      c 
*/
/*         4       avhrr ( PM noaa )                                    c 
*/
/*                 enter month,day,decimal hour (universal time-hh.ddd) c 
*/
/*                       n. of column(1-2048),xlonan,hna                c 
*/
/*                       give long.(xlonan) and overpass hour (hna) at  c 
*/
/*                       the ascendant node at equator                  c 
*/
/*                                                                      c 
*/
/*         5       avhrr ( AM noaa )                                    c 
*/
/*                 enter month,day,decimal hour (universal time-hh.ddd) c 
*/
/*                       n. of column(1-2048),xlonan,hna                c 
*/
/*                       give long.(xlonan) and overpass hour (hna) at  c 
*/
/*                       the ascendant node at equator                  c 
*/
/*                                                                      c 
*/
/*         6       hrv   ( spot )    * enter month,day,hh.ddd,long.,lat.c 
*/
/*                                                                      c 
*/
/*         7       tm    ( landsat ) * enter month,day,hh.ddd,long.,lat.c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*     note:       for hrv and tm experiments long. and lat. are the    c 
*/
/*                 coordinates of the scene center.                     c 
*/
/*                 lat. must be > 0 for north lat., < 0 for south lat.  c 
*/
/*                 long. must be > 0 for east long., <0 for west long.  c 
*/
/*                                                                      c 
*/
/*                 solar and viewing positions are computed             c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* _otb_adaptation Beginning: igeom = 0 by default */
/* _otb      read(iread,*) igeom */
/*<       igeom = 0 >*/
    igeom = 0;
/*<       asol  = otb_asol >*/
    asol = *otb_asol__;
/*<       phi0  = otb_phi0 >*/
    phi0 = *otb_phi0__;
/*<       avis  = otb_avis >*/
    avis = *otb_avis__;
/*<       phiv  = otb_phiv >*/
    phiv = *otb_phiv__;
/*<       month = otb_month >*/
    month = *otb_month__;
/*<       jday  = otb_jday >*/
    jday = *otb_jday__;
/* _otb      goto(1001,1002,1003,1004,1005,1006,1007),igeom */
/*   igeom=0..... */
/* _otb      read(iread,*) asol,phi0,avis,phiv,month,jday */
/* _otb      goto 22 */

/* _otb 1001 read(iread,*) month,jday,tu,nc,nl */
/* _otb      call posmto(month,jday,tu,nc,nl, */
/* _otb     1            asol,phi0,avis,phiv,xlon,xlat) */
/* _otb      goto 22 */
/* _otb 1002 read(iread,*) month,jday,tu,nc,nl */
/* _otb      call posge(month,jday,tu,nc,nl, */
/* _otb     1           asol,phi0,avis,phiv,xlon,xlat) */
/* _otb      goto 22 */
/* _otb 1003 read(iread,*) month,jday,tu,nc,nl */
/* _otb      call posgw(month,jday,tu,nc,nl, */
/* _otb     1           asol,phi0,avis,phiv,xlon,xlat) */
/* _otb      goto 22 */
/* _otb 1004 read(iread,*) month,jday,tu,nc,xlonan,hna */
/* _otb      campm=1.0 */
/* _otb      call posnoa(month,jday,tu,nc,xlonan,hna,campm, */
/* _otb     1            asol,phi0,avis,phiv,xlon,xlat) */
/* _otb      goto 22 */
/* _otb 1005 read(iread,*) month,jday,tu,nc,xlonan,hna */
/* _otb      campm=-1.0 */
/* _otb      call posnoa(month,jday,tu,nc,xlonan,hna,campm, */
/* _otb     1            asol,phi0,avis,phiv,xlon,xlat) */
/* _otb      goto 22 */
/* _otb 1006 read(iread,*) month,jday,tu,xlon,xlat */
/* _otb      call posspo(month,jday,tu,xlon,xlat, */
/* _otb     a            asol,phi0,avis,phiv) */
/* _otb      goto 22 */
/* _otb 1007 read(iread,*) month,jday,tu,xlon,xlat */
/* _otb      call poslan(month,jday,tu,xlon,xlat, */
/* _otb     s            asol,phi0,avis,phiv) */
/* _otb   22 continue */
/* _otb_adaptation End : igeom = 0 by default */
/*<       if(ier) stop >*/
    if (sixs_ier__1.ier) {
	s_stop("", 0L);
    }
/*<       dsol=1. >*/
    dsol = (float)1.;
/*<    >*/
    varsol_(&jday, &month, &dsol);
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                                 / scattered direction                c 
*/
/*                               /                                      c 
*/
/*                             /                                        c 
*/
/*                           / adif                                     c 
*/
/*    incident   + + + + + + + + + + + + + + +                          c 
*/
/*    direction                                                         c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       phi=abs(phiv-phi0) >*/
    phi = (r__1 = phiv - phi0, dabs(r__1));
/*<       phirad=(phi0-phiv)*pi/180. >*/
    phirad = (phi0 - phiv) * pi / (float)180.;
/*<       if (phirad.lt.0.) phirad=phirad+2.*pi >*/
    if (phirad < (float)0.) {
	phirad += pi * (float)2.;
    }
/*<       if (phirad.gt.(2.*pi)) phirad=phirad-2.*pi >*/
    if (phirad > pi * (float)2.) {
	phirad -= pi * (float)2.;
    }
/*<       xmus=cos(asol*pi/180.) >*/
    xmus = cos(asol * pi / (float)180.);
/*<       xmuv=cos(avis*pi/180.) >*/
    xmuv = cos(avis * pi / (float)180.);
/*<       xmup=cos(phirad) >*/
    xmup = cos(phirad);
/*<       xmud=-xmus*xmuv-sqrt(1.-xmus*xmus)*sqrt(1.-xmuv*xmuv)*xmup >*/
    xmud = -xmus * xmuv - sqrt((float)1. - xmus * xmus) * sqrt((float)1. - 
	    xmuv * xmuv) * xmup;
/* test vermote bug */
/*<       if (xmud.gt.1.) xmud=1. >*/
    if (xmud > (float)1.) {
	xmud = (float)1.;
    }
/*<       if (xmud.lt.-1.) xmud=-1. >*/
    if (xmud < (float)-1.) {
	xmud = (float)-1.;
    }
/*<       adif=acos(xmud)*180./pi >*/
    adif = acos(xmud) * (float)180. / pi;
/* **********************************************************************c
 */
/*       idatm      atmospheric model                                   c 
*/
/*                 --------------------                                 c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*  you select one of the following standard atmosphere: idatm=0 to 6   c 
*/
/*         0    no gaseous absorption                                   c 
*/
/*         1    tropical                )                               c 
*/
/*         2    midlatitude summer      )                               c 
*/
/*         3    midlatitude winter      )                               c 
*/
/*         4    subarctic summer        )      from lowtran             c 
*/
/*         5    subarctic winter        )                               c 
*/
/*         6    us standard 62          )                               c 
*/
/*                                                                      c 
*/
/*  or you define your own atmospheric model idatm=7 or 8               c 
*/
/*         7    user profile  (radiosonde data on 34 levels)            c 
*/
/*              enter altitude       (  in km )                         c 
*/
/*                    pressure       (  in mb )                         c 
*/
/*                    temperature    (  in k  )                         c 
*/
/*                    h2o density    (in  g/m3)                         c 
*/
/*                    o3  density    (in  g/m3)                         c 
*/
/*                                                                      c 
*/
/*           for example, altitudes are  from  0 to 25km step of 1km    c 
*/
/*                        from 25 to 50km step of 5km                   c 
*/
/*                        and two values at 70km and 100km              c 
*/
/*                        so you have 34*5 values to input.             c 
*/
/*         8    enter water vapor and ozone contents                    c 
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
/* _otb_adaptation Beginning: idatm=7, use of atmospheric model US62, */
/*_otb_adaptation            + normalization of vertical profiles from rea
l pressure*/
/* _otb_adaptation              and real ozone and vater vaper amounts */
/* _otb      uw=0. */
/* _otb      uo3=0. */
/* _otb      read(iread,*) idatm */
/*<       idatm=7 >*/
    idatm = 7;
/*<       call us62 >*/
    us62_();
/* _otb * Water vapor and ozone amount for the standard US62 profile. */
/*<       uwus  = 1.424 >*/
    uwus = (float)1.424;
/*<       uo3us = 0.344 >*/
    uo3us = (float).344;
/* _otb * Normalization from real values : uw, uo3, pressure (input data) 
*/
/*<       do 7 k=1,34 >*/
    for (k = 1; k <= 34; ++k) {
/*<          p(k)=p(k)*otb_pressure/p(1) >*/
	sixs_atm__1.p[k - 1] = sixs_atm__1.p[k - 1] * *otb_pressure__ / 
		sixs_atm__1.p[0];
/*<          wh(k)=wh(k)*otb_uw/uwus >*/
	sixs_atm__1.wh[k - 1] = sixs_atm__1.wh[k - 1] * *otb_uw__ / uwus;
/*<          wo(k)=wo(k)*otb_uo3/uo3us   >*/
	sixs_atm__1.wo[k - 1] = sixs_atm__1.wo[k - 1] * *otb_uo3__ / uo3us;
/*<     7 continue >*/
/* L7: */
    }
/* _otb      if(idatm.eq.0) go to 5 */
/* _otb      if(idatm.eq.8) read(iread,*) uw,uo3 */
/*    FILE.mie (Extinction and scattering coefficients, single          c 
*/
/*    scattering albedo, Asymmetry parameter, phase function at         c 
*/
/*    predefined wavelengths) and then can be re-used with the          c 
*/
/*    option iaer=12 where FILE is an identification name you           c 
*/
/*    have to enter.                                                    c 
*/
/*                                                                      c 
*/
/*    So, if you select iaer=8,9,10,or 11, next line following the      c 
*/
/*    requested inputs by the options 8,9,10, or 11 you have to enter   c 
*/
/*    iaerp                                                             c 
*/
/*                                                                      c 
*/
/*        iaerp=0    results will not be saved                          c 
*/
/*        iaerp=1    results will be saved into the file FILE.mie       c 
*/
/*                    next line enter FILE                              c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*   example for iaer and iaerp                                         c 
*/
/* 8                      Multimodal Log-Normale distribution selected  c 
*/
/* 0.0001 100.0 3         Rmin, Rmax, 3 components                      c 
*/
/* 0.5000 2.99 1.66E-7    Rmean, Sigma, percentage density-1st componentc 
*/
/* 1.53 1.53 1.53 1.53 1.53 1.53 1.52 1.40 1.22 1.27  nr-10 wavelengths c 
*/
/* .008 .008 .008 .008 .008 .008 .008 .008 .009 .011  ni-10 wavelengths c 
*/
/* 0.0050 2.99 0.5945     Rmean, Sigma, percentage density-2nd componentc 
*/
/* 1.53 1.53 1.53 1.53 1.53 1.53 1.52 1.51 1.42 1.452 nr-10 wavelengths c 
*/
/* .005 .005 .005 .005 .006 .007 .012 .023 .010 .004  ni-10 wavelengths c 
*/
/* 0.0118 2.00 0.4055     Rmean, Sigma, percentage density-3rd componentc 
*/
/* 1.75 1.75 1.75 1.75 1.75 1.75 1.75 1.77 1.81 1.90  nr-10 wavelengths c 
*/
/* .46  .45  .45  .44  .43  .43  .43  .46  .50  .57   ni-10 wavelengths c 
*/
/* 1                      Results will be saved into FILE.mie           c 
*/
/* URBAN-WCP112           Identification of the output file called FILE c 
*/
/*                    -> results will be saved into URBAN-WCP112.mie    c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       rmin=0. >*/
    mie_in__1.rmin = (float)0.;
/*<       rmax=0. >*/
    mie_in__1.rmax = (float)0.;
/*<       icp=1 >*/
    mie_in__1.icp = 1;
/*<       do i=1,4 >*/
    for (i__ = 1; i__ <= 4; ++i__) {
/*<        x1(i)=0.0 >*/
	mie_in__1.x1[i__ - 1] = (float)0.;
/*<        x2(i)=0.0 >*/
	mie_in__1.x2[i__ - 1] = (float)0.;
/*<        x3(i)=0.0 >*/
	mie_in__1.x3[i__ - 1] = (float)0.;
/*<        do l=1,10 >*/
	for (l = 1; l <= 10; ++l) {
/*<         rn(l,i)=0.0 >*/
	    mie_in__1.rn[l + i__ * 10 - 11] = (float)0.;
/*<         ri(l,i)=0.0 >*/
	    mie_in__1.ri[l + i__ * 10 - 11] = (float)0.;
/*<        enddo >*/
	}
/*<       enddo >*/
    }
/*<       do i=1,50 >*/
    for (i__ = 1; i__ <= 50; ++i__) {
/*<        rsunph(i)=0. >*/
	mie_in__1.rsunph[i__ - 1] = (float)0.;
/*<        nrsunph(i)=0. >*/
	mie_in__1.nrsunph[i__ - 1] = (float)0.;
/*<       enddo >*/
    }
/*<       cij(1)=1.00 >*/
    mie_in__1.cij[0] = (float)1.;
/* _otb_adaptation Beginning: iaer becomes input argument */
/* _otb      read(iread,*) read(iread,*) iaer */
/*<        iaer = otb_iaer >*/
    iaer = *otb_iaer__;
/* _obt   --> expected values are 0,1,2,3 or 5 */
/* _otb_adaptation End :  iaer becomes input argument */
/*<       if(iaer.eq.4) read(iread,*) (c(n),n=1,4) >*/
    if (iaer == 4) {
	io___52.ciunit = iread;
	s_rsle(&io___52);
	for (n = 1; n <= 4; ++n) {
	    do_lio(&c__4, &c__1, (char *)&c__[n - 1], (ftnlen)sizeof(real));
	}
	e_rsle();
    }
/*<       goto(49,40,41,42,49,49,49,49,43,44,45,46,47),iaer+1 >*/
    switch (iaer + 1) {
	case 1:  goto L49;
	case 2:  goto L40;
	case 3:  goto L41;
	case 4:  goto L42;
	case 5:  goto L49;
	case 6:  goto L49;
	case 7:  goto L49;
	case 8:  goto L49;
	case 9:  goto L43;
	case 10:  goto L44;
	case 11:  goto L45;
	case 12:  goto L46;
	case 13:  goto L47;
    }
/*<    40 c(1)=0.70 >*/
L40:
    c__[0] = (float).7;
/*<       c(2)=0.29 >*/
    c__[1] = (float).29;
/*<       c(3)=0.00 >*/
    c__[2] = (float)0.;
/*<       c(4)=0.01 >*/
    c__[3] = (float).01;
/*<       go to 49 >*/
    goto L49;
/*<    41 c(1)=0.00 >*/
L41:
    c__[0] = (float)0.;
/*<       c(2)=0.05 >*/
    c__[1] = (float).05;
/*<       c(3)=0.95 >*/
    c__[2] = (float).95;
/*<       c(4)=0.00 >*/
    c__[3] = (float)0.;
/*<       go to 49 >*/
    goto L49;
/*<    42 c(1)=0.17 >*/
L42:
    c__[0] = (float).17;
/*<       c(2)=0.61 >*/
    c__[1] = (float).61;
/*<       c(3)=0.00 >*/
    c__[2] = (float)0.;
/*<       c(4)=0.22 >*/
    c__[3] = (float).22;
/*<       go to 49 >*/
    goto L49;
/*<    43 read(iread,*) rmin,rmax,icp >*/
L43:
    io___55.ciunit = iread;
    s_rsle(&io___55);
    do_lio(&c__4, &c__1, (char *)&mie_in__1.rmin, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&mie_in__1.rmax, (ftnlen)sizeof(real));
    do_lio(&c__3, &c__1, (char *)&mie_in__1.icp, (ftnlen)sizeof(integer));
    e_rsle();
/*<       do i=1,icp >*/
    i__1 = mie_in__1.icp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<        read(5,*)x1(i),x2(i),cij(i) >*/
	s_rsle(&io___56);
	do_lio(&c__4, &c__1, (char *)&mie_in__1.x1[i__ - 1], (ftnlen)sizeof(
		real));
	do_lio(&c__4, &c__1, (char *)&mie_in__1.x2[i__ - 1], (ftnlen)sizeof(
		real));
	do_lio(&c__4, &c__1, (char *)&mie_in__1.cij[i__ - 1], (ftnlen)sizeof(
		real));
	e_rsle();
/*<        read(5,*)(rn(l,i),l=1,10) >*/
	s_rsle(&io___57);
	for (l = 1; l <= 10; ++l) {
	    do_lio(&c__4, &c__1, (char *)&mie_in__1.rn[l + i__ * 10 - 11], (
		    ftnlen)sizeof(real));
	}
	e_rsle();
/*<        read(5,*)(ri(l,i),l=1,10) >*/
	s_rsle(&io___58);
	for (l = 1; l <= 10; ++l) {
	    do_lio(&c__4, &c__1, (char *)&mie_in__1.ri[l + i__ * 10 - 11], (
		    ftnlen)sizeof(real));
	}
	e_rsle();
/*<       enddo >*/
    }
/*<       go to 49 >*/
    goto L49;
/*<    44 read(iread,*) rmin,rmax >*/
L44:
    io___59.ciunit = iread;
    s_rsle(&io___59);
    do_lio(&c__4, &c__1, (char *)&mie_in__1.rmin, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&mie_in__1.rmax, (ftnlen)sizeof(real));
    e_rsle();
/*<       read(iread,*) x1(1),x2(1),x3(1) >*/
    io___60.ciunit = iread;
    s_rsle(&io___60);
    do_lio(&c__4, &c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&mie_in__1.x2[0], (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&mie_in__1.x3[0], (ftnlen)sizeof(real));
    e_rsle();
/*<       read(5,*)(rn(l,1),l=1,10) >*/
    s_rsle(&io___61);
    for (l = 1; l <= 10; ++l) {
	do_lio(&c__4, &c__1, (char *)&mie_in__1.rn[l - 1], (ftnlen)sizeof(
		real));
    }
    e_rsle();
/*<       read(5,*)(ri(l,1),l=1,10) >*/
    s_rsle(&io___62);
    for (l = 1; l <= 10; ++l) {
	do_lio(&c__4, &c__1, (char *)&mie_in__1.ri[l - 1], (ftnlen)sizeof(
		real));
    }
    e_rsle();
/*<       go to 49 >*/
    goto L49;
/*<    45 read(iread,*) rmin,rmax >*/
L45:
    io___63.ciunit = iread;
    s_rsle(&io___63);
    do_lio(&c__4, &c__1, (char *)&mie_in__1.rmin, (ftnlen)sizeof(real));
    do_lio(&c__4, &c__1, (char *)&mie_in__1.rmax, (ftnlen)sizeof(real));
    e_rsle();
/*<       read(iread,*) x1(1) >*/
    io___64.ciunit = iread;
    s_rsle(&io___64);
    do_lio(&c__4, &c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(real));
    e_rsle();
/*<       read(5,*)(rn(l,1),l=1,10) >*/
    s_rsle(&io___65);
    for (l = 1; l <= 10; ++l) {
	do_lio(&c__4, &c__1, (char *)&mie_in__1.rn[l - 1], (ftnlen)sizeof(
		real));
    }
    e_rsle();
/*<       read(5,*)(ri(l,1),l=1,10) >*/
    s_rsle(&io___66);
    for (l = 1; l <= 10; ++l) {
	do_lio(&c__4, &c__1, (char *)&mie_in__1.ri[l - 1], (ftnlen)sizeof(
		real));
    }
    e_rsle();
/*<       go to 49 >*/
    goto L49;
/*<    46 read(5,*)irsunph >*/
L46:
    s_rsle(&io___67);
    do_lio(&c__3, &c__1, (char *)&mie_in__1.irsunph, (ftnlen)sizeof(integer));
    e_rsle();
/*<       do i=1,irsunph >*/
    i__1 = mie_in__1.irsunph;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<        read(5,*)rsunph(i),nrsunph(i) >*/
	s_rsle(&io___68);
	do_lio(&c__4, &c__1, (char *)&mie_in__1.rsunph[i__ - 1], (ftnlen)
		sizeof(real));
	do_lio(&c__4, &c__1, (char *)&mie_in__1.nrsunph[i__ - 1], (ftnlen)
		sizeof(real));
	e_rsle();
/*<        nrsunph(i)=nrsunph(i)/(rsunph(i)**4.)/alog(10.0) >*/
	d__1 = (doublereal) mie_in__1.rsunph[i__ - 1];
	mie_in__1.nrsunph[i__ - 1] = mie_in__1.nrsunph[i__ - 1] / pow_dd(&
		d__1, &c_b76) / log((float)10.);
/*<       enddo >*/
    }
/*<       rmin=rsunph(1) >*/
    mie_in__1.rmin = mie_in__1.rsunph[0];
/*<       rmax=rsunph(irsunph)+1e-07 >*/
    mie_in__1.rmax = mie_in__1.rsunph[mie_in__1.irsunph - 1] + (float)1e-7;
/*<       read(5,*)(rn(l,1),l=1,10) >*/
    s_rsle(&io___69);
    for (l = 1; l <= 10; ++l) {
	do_lio(&c__4, &c__1, (char *)&mie_in__1.rn[l - 1], (ftnlen)sizeof(
		real));
    }
    e_rsle();
/*<       read(5,*)(ri(l,1),l=1,10) >*/
    s_rsle(&io___70);
    for (l = 1; l <= 10; ++l) {
	do_lio(&c__4, &c__1, (char *)&mie_in__1.ri[l - 1], (ftnlen)sizeof(
		real));
    }
    e_rsle();
/*<       go to 49 >*/
    goto L49;
/*<    47 read(5,'(A80)')FILE2 >*/
L47:
    ci__1.cierr = 0;
    ci__1.ciend = 0;
    ci__1.ciunit = 5;
    ci__1.cifmt = "(A80)";
    s_rsfe(&ci__1);
    do_fio(&c__1, file2, 80L);
    e_rsfe();
/*<       i2=index(FILE2,' ')-1 >*/
    i2 = i_indx(file2, " ", 80L, 1L) - 1;
/*<       go to 49 >*/
    goto L49;
/*<    49 continue >*/
L49:
/*<       if (iaer.ge.8.and.iaer.le.11)then >*/
    if (iaer >= 8 && iaer <= 11) {
/*<        read(5,*)iaerp >*/
	s_rsle(&io___72);
	do_lio(&c__3, &c__1, (char *)&iaerp, (ftnlen)sizeof(integer));
	e_rsle();
/*<        if (iaerp.eq.1)read(5,'(A80)')FILE >*/
	if (iaerp == 1) {
	    ci__1.cierr = 0;
	    ci__1.ciend = 0;
	    ci__1.ciunit = 5;
	    ci__1.cifmt = "(A80)";
	    s_rsfe(&ci__1);
	    do_fio(&c__1, file, 80L);
	    e_rsfe();
	}
/*<        i1=index(FILE,' ')-1 >*/
	i1 = i_indx(file, " ", 80L, 1L) - 1;
/*<        FILE2=FILE(1:I1)//'.mie' >*/
/* Writing concatenation */
	i__2[0] = i1, a__1[0] = file;
	i__2[1] = 4, a__1[1] = ".mie";
	s_cat(file2, a__1, i__2, &c__2, 80L);
/*<        i2=index(FILE2,' ')-1 >*/
	i2 = i_indx(file2, " ", 80L, 1L) - 1;
/*<       endif >*/
    }
/*<       call aeroso(iaer,c,xmud,wldis,FILE2) >*/
    aeroso_(&iaer, c__, &xmud, sixs_disc__1.wldis, file2, 80L);
/* **********************************************************************c
 */
/*              aerosol model (concentration)                           c 
*/
/*              ----------------------------                            c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*  v             if you have an estimate of the meteorological         c 
*/
/*                parameter: the visibility v, enter directly the       c 
*/
/*                value of v in km (the aerosol optical depth will      c 
*/
/*                be computed from a standard aerosol profile)          c 
*/
/*                                                                      c 
*/
/*  v=0, taer55   if you have an estimate of aerosol optical depth ,    c 
*/
/*                enter v=0 for the visibility and enter the aerosol    c 
*/
/*                optical depth at 550                                  c 
*/
/*                                                                      c 
*/
/*  v=-1          warning:  if iaer=0, enter v=-1                       c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* _otb_adaptation Beginning: v=0 and taer55 becomes argument value */
/* _otb      taer55=0. */
/*<            v=0. >*/
    v = (float)0.;
/*< 	   taer=0.	!kept for further use >*/
    taer = (float)0.;
/* _otb      read(iread,*) v */
/* _otb      if(v) 71,10,11 */
/* _otb   10 read(iread,*) taer55 */
/* _otb      v=exp(-log(taer55/2.7628)/0.79902) */
/* _otb      goto 71 */
/* _otb   11 call oda550(iaer,v,taer55) */
/* _otb   71 continue */
/*<         taer55 = otb_taer55 >*/
    taer55 = *otb_taer55__;
/* _otb_adaptation End :  v=0 and taer55 becomes argument value */
/* **********************************************************************c
 */
/* xps is the parameter to express the  altitude of target              c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                  xps >=0. means the target is at the sea level       c 
*/
/*                                                                      c 
*/
/*                  xps <0. means you know the altitude of the target   c 
*/
/*                        expressed in km and you put that value as xps c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* _otb_adaptation Beginning: xps=0 Target at sea level */
/* _otb        read(iread,*) xps */
/*< 	xps=0.	 >*/
    xps = (float)0.;
/* _otb_adaptation End : xps=0 Target at sea level */
/*<        if (xps.ge.0.) then >*/
    if (xps >= (float)0.) {
/*<         xps=0. >*/
	xps = (float)0.;
/*<         uwus=1.424 >*/
	uwus = (float)1.424;
/*<         uo3us=0.344 >*/
	uo3us = (float).344;
/*<        else >*/
    } else {
/*<         if (idatm.ne.8) then >*/
	if (idatm != 8) {
/*<          call pressure(uw,uo3,xps) >*/
	    pressure_(&uw, &uo3, &xps);
/*<         else >*/
	} else {
/*<          call pressure(uwus,uo3us,xps) >*/
	    pressure_(&uwus, &uo3us, &xps);
/*<         endif >*/
	}
/*<        endif >*/
    }
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*  xpp is the parameter to express the sensor altitude                 c 
*/
/*                                                                      c 
*/
/*         xpp= -1000  means that the sensor is a board a satellite     c 
*/
/*         xpp=     0  means that the sensor is at the ground level     c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*     for aircraft simulations                                         c 
*/
/*    -100< xpp <0  means you know the altitude of the sensor expressed c 
*/
/*                  in kilometers units                                 c 
*/
/*     this altitude is relative to the target altitude                 c 
*/
/*                                                                      c 
*/
/*     for aircraft simulations only, you have to give                  c 
*/
/* 	puw,po3   (water vapor content,ozone content between the       c */
/*                  aircraft and the surface)                           c 
*/
/* 	taerp     (the aerosol optical thickness at 550nm between the  c */
/*                  aircraft and the surface)                           c 
*/
/*    if these data are not available, enter negative values for all    c 
*/
/*    of them, puw,po3 will then be interpolated from the us62 standard c 
*/
/*    profile according to the values at ground level. Taerp will be    c 
*/
/*    computed according to a 2km exponential profile for aerosol.      c 
*/
/* **********************************************************************c
 */
/* _otb_adaptation Beginning: xpp=-1000 sensor aboard a satellite */
/* _otb        read(iread,*) xpp */
/*< 	xpp=-1000.	 >*/
    xpp = (float)-1e3;
/* _otb_adaptation End : xpp=-1000 sensor aboard a satellite */
/*<         xpp=-xpp >*/
    xpp = -xpp;
/*<         if (xpp.le.0.0) then >*/
    if (xpp <= (float)0.) {
/*          ground measurement option */
/*<            palt=0. >*/
	palt = (float)0.;
/*<            pps=p(1) >*/
	pps = sixs_atm__1.p[0];
/*< 	   idatmp=0 >*/
	idatmp = 0;
/*< 	   taer55p=0. >*/
	taer55p = (float)0.;
/*< 	   puw=0. >*/
	puw = (float)0.;
/*< 	   puoz=0. >*/
	puoz = (float)0.;
/*<            else >*/
    } else {
/*< 	   if (xpp.ge.100.) then >*/
	if (xpp >= (float)100.) {
/* 	       satellite case of equivalent */
/*< 	      palt=1000. >*/
	    palt = (float)1e3;
/*< 	      pps=0. >*/
	    pps = (float)0.;
/*< 	      taer55p=taer55 >*/
	    taer55p = taer55;
/*< 	      ftray=1. >*/
	    ftray = (float)1.;
/*< 	      idatmp=4 >*/
	    idatmp = 4;
/*< 	      else >*/
	} else {
/* 	      "real" plane case */
/*<               read(iread,*) puw,puo3 >*/
	    io___89.ciunit = iread;
	    s_rsle(&io___89);
	    do_lio(&c__4, &c__1, (char *)&puw, (ftnlen)sizeof(real));
	    do_lio(&c__4, &c__1, (char *)&puo3, (ftnlen)sizeof(real));
	    e_rsle();
/*< 	      if (puw.lt.0.) then >*/
	    if (puw < (float)0.) {
/*<                  call presplane(puw,puo3,xpp,ftray) >*/
		presplane_(&puw, &puo3, &xpp, &ftray);
/*< 	         idatmp=2 >*/
		idatmp = 2;
/*< 	         if (idatm.eq.8) then >*/
		if (idatm == 8) {
/*< 	            puwus=puw >*/
		    puwus = puw;
/*< 	            puo3us=puo3 >*/
		    puo3us = puo3;
/*< 	            puw=puw*uw/uwus >*/
		    puw = puw * uw / uwus;
/*< 	            puo3=puo3*uo3/uo3us >*/
		    puo3 = puo3 * uo3 / uo3us;
/*< 	            idatmp=8 >*/
		    idatmp = 8;
/*< 	         endif >*/
		}
/*<               else >*/
	    } else {
/*< 	         call presplane(puwus,puo3us,xpp,ftray) >*/
		presplane_(&puwus, &puo3us, &xpp, &ftray);
/*< 	         idatmp=8 >*/
		idatmp = 8;
/*<               endif >*/
	    }
/*<               if(ier) stop >*/
	    if (sixs_ier__1.ier) {
		s_stop("", 0L);
	    }
/*<               palt=zpl(34)-z(1) >*/
	    palt = sixs_planesim__1.zpl[33] - sixs_atm__1.z__[0];
/*< 	      pps=ppl(34) >*/
	    pps = sixs_planesim__1.ppl[33];
/*<               read(iread,*) taer55p >*/
	    io___93.ciunit = iread;
	    s_rsle(&io___93);
	    do_lio(&c__4, &c__1, (char *)&taer55p, (ftnlen)sizeof(real));
	    e_rsle();
/*< 	    if ((taer55p.lt.0.).or.((taer55-taer55p).lt.accu2)) then >*/
	    if (taer55p < (float)0. || taer55 - taer55p < accu2) {
/* a scale heigh of 2km is assumed in case no value is given f
or taer55p */
/*<                taer55p=taer55*(1.-exp(-palt/2.)) >*/
		taer55p = taer55 * ((float)1. - exp(-palt / (float)2.));
/*<             else >*/
	    } else {
/* compute effective scale heigh */
/*<                sham=exp(-palt/4.) >*/
		sham = exp(-palt / (float)4.);
/*<                sha=1.-(taer55p/taer55) >*/
		sha = (float)1. - taer55p / taer55;
/*<                if (sha.ge.sham) then >*/
		if (sha >= sham) {
/*<                   taer55p=taer55*(1.-exp(-palt/4.)) >*/
		    taer55p = taer55 * ((float)1. - exp(-palt / (float)4.));
/*<                else >*/
		} else {
/*<                   sha=-palt/log(sha) >*/
		    sha = -palt / log(sha);
/*<                   taer55p=taer55*(1.-exp(-palt/sha)) >*/
		    taer55p = taer55 * ((float)1. - exp(-palt / sha));
/*<                endif >*/
		}
/*<             endif >*/
	    }
/*<          endif >*/
	}
/*<       endif >*/
    }
/* **********************************************************************c
 */
/*        37  3rd      "               ( 0.8300-0.9125)                 c 
*/
/*        38  4th      "               ( 0.9000-0.9975)                 c 
*/
/*        39  5th      "               ( 1.8200-1.9575)                 c 
*/
/*        40  6th      "               ( 2.0950-2.1925)                 c 
*/
/*        41  7th      "               ( 3.5800-3.8700)                 c 
*/
/*        42  MODIS   band 1           ( 0.6100-0.6850)                 c 
*/
/*        43  MODIS   band 2           ( 0.8200-0.9025)                 c 
*/
/*        44  MODIS   band 3           ( 0.4500-0.4825)                 c 
*/
/*        45  MODIS   band 4           ( 0.5400-0.5700)                 c 
*/
/*        46  MODIS   band 5           ( 1.2150-1.2700)                 c 
*/
/*        47  MODIS   band 6           ( 1.6000-1.6650)                 c 
*/
/*        48  MODIS   band 7           ( 2.0575-2.1825)                 c 
*/
/*        49  1st band of avhrr(noaa12 ( 0.500-1.000 )                  c 
*/
/*        50  2nd      "               ( 0.650-1.120 )                  c 
*/
/*        51  1st band of avhrr(noaa14 ( 0.500-1.110 )                  c 
*/
/*        52  2nd      "               ( 0.680-1.100 )                  c 
*/
/*        53  POLDER  band 1           ( 0.4125-0.4775)                 c 
*/
/*        54  POLDER  band 2 (non polar( 0.4100-0.5225)                 c 
*/
/*        55  POLDER  band 3 (non polar( 0.5325-0.5950)                 c 
*/
/*        56  POLDER  band 4   P1      ( 0.6300-0.7025)                 c 
*/
/*        57  POLDER  band 5 (non polar( 0.7450-0.7800)                 c 
*/
/*        58  POLDER  band 6 (non polar( 0.7000-0.8300)                 c 
*/
/*        59  POLDER  band 7   P1      ( 0.8100-0.9200)                 c 
*/
/*        60  POLDER  band 8 (non polar( 0.8650-0.9400)                 c 
*/
/*  note: wl has to be in micrometer                                    c 
*/
/* **********************************************************************c
 */
/*_otb_adaptation Beginning: iwave=1 and wlinf, wlsup, s(l) become input a
rguments*/
/* _otb        do 38 l=iinf,isup */
/* _otb        s(l)=1. */
/* _otb    38 continue */
/* _otb       read(iread,*) iwave */
/* _otb           if (iwave.eq.-2) goto 1600 */
/* _otb      if (iwave) 16,17,18 */
/* _otb   16 read(iread,*) wl */
/* _otb      wlinf=wl */
/* _otb      wlsup=wl */
/* _otb      go to 19 */
/* _otb   17 read(iread,*) wlinf,wlsup */
/* _otb      go to 19 */
/* _otb 1600 read(iread,*) wlinf,wlsup */
/* _otb      go to 19 */
/* _otb   18 goto (110,111,112,112,114,114,114,114,114,114,114,114 */
/* _otb     s      ,114,114,114,114,118,118,118,118,118,118,118,118 */
/* _otb     s     ,121,121,121,121,121,121,127,127,127,127 */
/* _otb     s     ,128,128,128,128,128,128,128,129,129,129,129,129 */
/* _otb     s     ,129,129,130,130,130,130 */
/* _otb     s     ,131,131,131,131,131,131,131,131),iwave */
/* _otb  110 read(iread,*) wlinf,wlsup */
/* _otb      iinf=(wlinf-.25)/0.0025+1.5 */
/* _otb      isup=(wlsup-.25)/0.0025+1.5 */
/* _otb      do 1113 ik=iinf,isup */
/* _otb       s(ik)=0. */
/* _otb 1113 continue */
/* _otb      read(iread,*) (s(i),i=iinf,isup) */
/* _otb      goto 20 */
/* _otb  111 call meteo */
/* _otb      go to 19 */
/* _otb  112 call goes(iwave-2) */
/* _otb      go to 19 */
/* _otb  114 call avhrr(iwave-4) */
/* _otb      go to 19 */
/* _otb  118 call hrv(iwave-16) */
/* _otb      go to 19 */
/* _otb  121 call tm(iwave-24) */
/* _otb      go to 19 */
/* _otb  127 call mss(iwave-30) */
/* _otb      goto 19 */
/* _otb  128 call mas(iwave-34) */
/* _otb      goto 19 */
/* _otb  129 call modis(iwave-41) */
/* _otb      goto 19 */
/* _otb  130 call avhrr(iwave-36) */
/* _otb      goto 19 */
/* _otb  131 call polder(iwave-52) */
/*<       iwave=1 >*/
    iwave = 1;
/*<       wlinf = otb_wlinf >*/
    sixs_ffu__1.wlinf = *otb_wlinf__;
/*<       wlsup = otb_wlsup >*/
    sixs_ffu__1.wlsup = *otb_wlsup__;
/*<       do k=1,1501 >*/
    for (k = 1; k <= 1501; ++k) {
/*<          s(k) = otb_s(k) >*/
	sixs_ffu__1.s[k - 1] = otb_s__[k];
/*<       enddo >*/
    }
/* _otb_adaptation End : iwave=1 and s(l) becomes an input argument */
/*<    19 iinf=(wlinf-.25)/0.0025+1.5 >*/
/* L19: */
    iinf = (sixs_ffu__1.wlinf - (float).25) / (float).0025 + (float)1.5;
/*<       isup=(wlsup-.25)/0.0025+1.5 >*/
    isup = (sixs_ffu__1.wlsup - (float).25) / (float).0025 + (float)1.5;
/*<    20 continue >*/
/* L20: */
/* **********************************************************************c
 */
/* here, we first compute an equivalent wavelenght which is the input   c 
*/
/* value for monochromatic conditions or the integrated value for a     c 
*/
/* filter functionr (call equivwl) then, the atmospheric properties are c 
*/
/* computed for that wavelength (call discom then call specinterp)      c 
*/
/* molecular optical thickness is computed too (call odrayl). lastly    c 
*/
/* the successive order of scattering code is called three times.       c 
*/
/* first for a sun at thetas with the scattering properties of aerosols c 
*/
/* and molecules, second with a pure molecular atmosphere, then with thec 
*/
/* actual atmosphere for a sun at thetav. the iso code allows us to     c 
*/
/* compute the scattering transmissions and the spherical albedo. all   c 
*/
/* these computations are performed for checking the accuracy of the    c 
*/
/* analytical expressions and in addition for computing the averaged    c 
*/
/* directional reflectances                                             c 
*/
/* **********************************************************************c
 */
/*<       if(iwave.ne.-1) then >*/
    if (iwave != -1) {
/*<    >*/
	equivwl_(&iinf, &isup, &step, &wlmoy);
/*<       else >*/
    } else {
/*<         wlmoy=wl >*/
	wlmoy = wl;
/*<       endif >*/
    }
/*<    >*/
    discom_(&idatmp, &iaer, &xmus, &xmuv, &phi, &taer55, &taer55p, &palt, &
	    phirad, &nt, &mu, &np, rm, gb, rp, &ftray, xlm1, xlm2);
/*<       if(iaer.ne.0) then >*/
    if (iaer != 0) {
/*<    >*/
	specinterp_(&wlmoy, &taer55, &taer55p, &tamoy, &tamoyp, &pizmoy, &
		pizmoyp);
/*<       endif >*/
    }
/*<    >*/
    odrayl_(&wlmoy, &trmoy);
/*<       trmoyp=trmoy*ftray >*/
    trmoyp = trmoy * ftray;
/*<       if (idatmp.eq.4) then >*/
    if (idatmp == 4) {
/*<           trmoyp=trmoy >*/
	trmoyp = trmoy;
/*<           tamoyp=tamoy >*/
	tamoyp = tamoy;
/*<       endif >*/
    }
/*<       if (idatmp.eq.0) then >*/
    if (idatmp == 0) {
/*<          trmoyp=0. >*/
	trmoyp = (float)0.;
/*<          tamoyp=0. >*/
	tamoyp = (float)0.;
/*<       endif >*/
    }
/* *********************************************************************c 
*/
/*     inhomo        ground reflectance (type)                          c 
*/
/*                   ------------------                                 c 
*/
/*                                                                      c 
*/
/*  you consider an homogeneous surface:                                c 
*/
/*     enter - inhomo=0                                                 c 
*/
/*                you may consider directional surface  effects         c 
*/
/*                  idirec=0 (no directional effect)                    c 
*/
/*                          you have to specify the surface reflectance:c 
*/
/*                          igroun  (see note1) which is uniform and    c 
*/
/*                          lambertian                                  c 
*/
/*                  idirec=1 ( directional effect)                      c 
*/
/*                          you have to specify the brdf of the surface c 
*/
/*                           for the actual solar illumination you  are c 
*/
/*                           considering as well as the brdf for a sun  c 
*/
/*                           which would be at an angle thetav, in      c 
*/
/*                   ---------------------------------------            c 
*/
/* note1: values of the reflectance selected by igroun,igrou1 or igrou2 c 
*/
/*        may correspond to the following cases,                        c 
*/
/*         0  constant value of ro (or roc,or roe) whatever the wavelen c 
*/
/*            gth. you enter this constant value of ro (or roc or roe). c 
*/
/*        -1  you have to enter the value of ro (or roc,or roe) by step c 
*/
/*            of 0.0025 micron from wlinf to wlsup (if you have used thec 
*/
/*            satellite bands,see implicit values for these limits).    c 
*/
/*         1  mean spectral value of green vegetation                   c 
*/
/*         2  mean spectral value of clear water                        c 
*/
/*         3  mean spectral value of sand                               c 
*/
/*         4  mean spectral value of lake water                         c 
*/
/*                                                                      c 
*/
/*                       ground reflectance (brdf)                      c 
*/
/*                       -------------------------                      c 
*/
/* note2: values of the directional reflectance is assumed spectrally   c 
*/
/*        independent, so you have to specify, the brdf at the          c 
*/
/*        wavelength for monochromatic condition of the mean value      c 
*/
/*        over the spectral band                                        c 
*/
/*         0  you have to enter the value of ro for sun at thetas by    c 
*/
/*            step of 10 degrees for zenith view  angles (from 0 to 80  c 
*/
/*            and the value for 85) and by step of 30 degrees for       c 
*/
/*            azimuth view angles from 0 to 360 degrees, you have to do c 
*/
/*            same for a sun which would be at thetav. in addition, the c 
*/
/*            spherical albedo of the surface has to be specified ,as   c 
*/
/*            well as the observed reflectance in the selected geometry c 
*/
/*           rodir(sun zenith,view zenith, relative azimuth).	       c */
/* 		 						       c */
/*        you also may select one of the following models               c 
*/
/*         1  hapke model                                               c 
*/
/*             the parameters are: om,af,s0,h                           c 
*/
/*                    om= albedo                                        c 
*/
/*                    af=assymetry parameter for the phase function     c 
*/
/*                    s0=amplitude of hot spot                          c 
*/
/*                    h=width of the hot spot                           c 
*/
/*                                                                      c 
*/
/*         2  verstraete et al. model                                   c 
*/
/*             the parameters are:                                      c 
*/
/*                there is three lines of parameters:                   c 
*/
/*                              line 1 (choice of options)              c 
*/
/*                              line 2 (structural parameters)          c 
*/
/*                              line 3 (optical parameters)             c 
*/
/*                line 1:  opt3 opt4 opt5                               c 
*/
/*                    opt1=1 parametrized model (see verstraete et al., c 
*/
/*                           JGR, 95, 11755-11765, 1990)                c 
*/
/*                    opt2=1 reflectance factor (see pinty et al., JGR, c 
*/
/*                           95, 11767-11775, 1990)                     c 
*/
/*                    opt3=0 for given values of kappa (see struc below)c 
*/
/*                    opt5=0 for single scattering only                 c 
*/
/*                         1 for dickinson et al. parameterization of   c 
*/
/*                           multiple scattering                        c 
*/
/*                line 2:  str1 str2 str3 str4                          c 
*/
/*                    str1='leaf area density', in m2 m-3               c 
*/
/*                    str2=radius of the sun flecks on the scatterer (m)c 
*/
/*                    str3=leaf orientation parameter:                  c 
*/
/*                         if opt3=0 then str3=kappa1                   c 
*/
/*                         if opt3=1 or 2  then str3=chil               c 
*/
/*                    str4=leaf orientation parameter (continued):      c 
*/
/*                         if opt3=0 then str4=kappa2                   c 
*/
/*                         if opt3=1 or 2 then str4 is not used         c 
*/
/*                line 3:  optics1 optics2 optics3                      c 
*/
/*                    optics1=single scattering albedo, n/d value       c 
*/
/*                            between 0.0 and 1.0                       c 
*/
/*                    optics2= phase function parameter:                c 
*/
/*                         if opt4=0 then this input is not used        c 
*/
/*                         if opt4=1 then asymmetry factor, n/d value   c 
*/
/*                                   between -1.0and 1.0                c 
*/
/*                         if opt4=2 then first coefficient of legendre c 
*/
/*                                   polynomial                         c 
*/
/*                    optics3=second coefficient of legendre polynomial c 
*/
/*                            (if opt4=2)                               c 
*/
/*                                                                      c 
*/
/*         3  Roujean et al. model                                      c 
*/
/*             the parameters are: k0,k1,k2                             c 
*/
/*                 k0=albedo.                                           c 
*/
/*                 k1=geometric parameter for hot spot effect           c 
*/
/*                 k2=geometric parameter for hot spot effect           c 
*/
/*                                                                      c 
*/
/*         4  walthall et al. model                                     c 
*/
/*             the parameters are: a,ap,b,c                             c 
*/
/*                 a=term in square ts*tv                               c 
*/
/*                 ap=term in square ts*ts+tv*tv                        c 
*/
/*                 b=term in ts*tv*cos(phi) (limacon de pascal)         c 
*/
/*                 c=albedo                                             c 
*/
/*                                                                      c 
*/
/*         5  minnaert model                                            c 
*/
/*             the parameters are: par1,par2                            c 
*/
/*                                                                      c 
*/
/*         6  Ocean                                                     c 
*/
/*             the parameter are: pws,phi_wind,xsal,pcl                 c 
*/
/*                 pws=wind speed (in m/s)                              c 
*/
/*                 phi_wind=azim. of the wind (in degres)               c 
*/
/*                 xsal=salinity (in ppt) xsal=34.3ppt if xsal<0        c 
*/
/*                 pcl=pigment concentration (in mg/m3)                 c 
*/
/*                                                                      c 
*/
/*         7  Iaquinta and Pinty model                                  c 
*/
/*             the parameters are:                                      c 
*/
/*                there is 3 lines of parameters:                       c 
*/
/*                          line 1: choice of option (pild,pihs)        c 
*/
/*                          line 2: structural parameters (pxLt,pc)     c 
*/
/*                          line 3: optical parameters (pRl,pTl,pRs)    c 
*/
/*                Line 1: pild,pihs                                     c 
*/
/*                    pild=1  planophile leaf distribution              c 
*/
/*                    pild=2  erectophile leaf distribution             c 
*/
/*                    pild=3  plagiophile leaf distribution             c 
*/
/*                    pild=4  extremophile leaf distribution            c 
*/
/*                    pild=5  uniform leaf distribution                 c 
*/
/*                                                                      c 
*/
/*                    pihs=0  no hot spot                               c 
*/
/*                    pihs=1  hot spot                                  c 
*/
/*                Line 2: pxLt,pc                                       c 
*/
/*                    pxLt=Leaf area index [1.,15.]                     c 
*/
/*                    pc=Hot spot parameter: 2*r*Lambda [0.,2.]         c 
*/
/*                Line 3: pRl,pTl,pRs                                   c 
*/
/*                    pRl=Leaf reflectance  [0.,0.99]                   c 
*/
/*                    pTl=Leaf transmitance [0.,0.99]                   c 
*/
/*                    pRs=Soil albedo       [0.,0.99]                   c 
*/
/*                         NB: pRl+PTl <0.99                            c 
*/
/*                                                                      c 
*/
/*         8  Rahman et al. model                                       c 
*/
/*             the parameters are: rho0,af,xk                           c 
*/
/*                 rho0=Intensity of the reflectance of the surface     c 
*/
/*                      cover, N/D value greater or equal to 0          c 
*/
/*                 af=Asymmetry factor, N/D value between -1.0 and 1.0  c 
*/
/*                 xk=Structural parameter of the medium                c 
*/
/*         9   Kuusk's multispectral CR model                           c 
*/
/*             Reference:                                               c 
*/
/*             Kuusk A. A multispectral canopy reflectance model.       c 
*/
/*             Remote Sens. Environ., 1994, 50:75-82                    c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*             the parameters are:                                      c 
*/
/*                                                                      c 
*/
/*     line 1: structural parameters (ul,eps,thm,sl)                    c 
*/
/*     line 2: optical parameters (cAB,cW,N,cn,s1)                      c 
*/
/*                                                                      c 
*/
/*             ul=LAI                                                   c 
*/
/*             eps,thm - LAD parameters                                 c 
*/
/*             sl      - relative leaf size                             c 
*/
/*             cAB     - chlorophyll content, ug/cm^2                   c 
*/
/*             cW      - leaf water equivalent thickness                c 
*/
/*             N       - the effective number of elementary layers      c 
*/
/*                       inside a leaf                                  c 
*/
/*             cn      - the ratio of refractive indices of the leaf    c 
*/
/*                       surface wax and internal material              c 
*/
/*             s1      - the weight of the 1st Price function for the   c 
*/
/*                       soil reflectance                               c 
*/
/* **********************************************************************c
 */
/*<       fr=0. >*/
    fr = (float)0.;
/*<       rad=0. >*/
    rad = (float)0.;
/*<       do 1116 ik=iinf,isup >*/
    i__1 = isup;
    for (ik = iinf; ik <= i__1; ++ik) {
/*<         rocl(ik)=0. >*/
	rocl[ik - 1] = (float)0.;
/*<         roel(ik)=0. >*/
	roel[ik - 1] = (float)0.;
/*<  1116 continue >*/
/* L1116: */
    }
/* **********************************************************************c
 */
/*     uniform or non-uniform surface conditions                        c 
*/
/* **********************************************************************c
 */
/* _otb_adaptation Beginning: inhomo=0 idirec=0 igroun=0 ro=0 */
/*<       inhomo=0 >*/
    inhomo = 0;
/*<       idirec=0 >*/
    idirec = 0;
/* _otb      read(iread,*) inhomo */
/* _otb      if(inhomo) 30,30,31 */
/* _otb  30  read(iread,*) idirec */
/* _otb      if(idirec)21,21,25 */
/* **********************************************************************c
 */
/*     uniform conditions with brdf conditions                          c 
*/
/* **********************************************************************c
 */
/* _otb        call roujbrdf(par1,par2,par3,mu,np,rm,rp, */
/* _otb     s           brdfintv) */
/* _otb        call roujalbe(par1,par2,par3, */
/* _otb     s       albbrdf) */
/* _otb        go to 69 */
/* _otb      endif */
/* **********************************************************************c
 */
/*     brdf from walthall et al's model */
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.4) then */
/* _otb        read(iread,*) par1,par2,par3,par4 */
/* _otb        rm(-mu)=phirad */
/* _otb        rm(mu)=xmuv */
/* _otb        rm(0)=xmus */
/* _otb        call waltbrdf(par1,par2,par3,par4,mu,np,rm,rp, */
/* _otb     s           brdfints) */
/* _otb        rm(-mu)=2.*pi-phirad */
/* _otb        rm(mu)=xmus */
/* _otb        rm(0)=xmuv */
/* _otb        call waltbrdf(par1,par2,par3,par4,mu,np,rm,rp, */
/* _otb     s           brdfintv) */
/* _otb        call waltalbe(par1,par2,par3,par4, */
/* _otb     s       albbrdf) */
/* _otb        go to 69 */
/* _otb      endif */
/* **********************************************************************c
 */
/*     brdf from minnaert's model                                       c 
*/
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.5) then */
/* _otb        read(iread,*) par1,par2 */
/* _otb        rm(-mu)=phirad */
/* _otb        rm(mu)=xmuv */
/* _otb        rm(0)=xmus */
/* _otb        call minnbrdf(par1,par2,mu,np,rm, */
/* _otb     s           brdfints) */
/* _otb        rm(-mu)=2.*pi-phirad */
/* _otb        rm(mu)=xmus */
/* _otb        rm(0)=xmuv */
/* _otb        call minnbrdf(par1,par2,mu,np,rm, */
/* _otb     s           brdfintv) */
/* _otb        call minnalbe(par1,par2, */
/* _otb     s       albbrdf) */
/* _otb        go to 69 */
/* _otb      endif */
/* **********************************************************************c
 */
/*     brdf from ocean condition */
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.6) then */
/* _otb        read(iread,*) pws,phi_wind,xsal,pcl */
/* _otb        if (xsal.lt.0.001)xsal=34.3 */
/* _otb        paw=phi0-phi_wind */
/* _otb        rm(-mu)=phirad */
/* _otb        rm(mu)=xmuv */
/* _otb        rm(0)=xmus */
/* _otb        call oceabrdf(pws,paw,xsal,pcl,wlmoy,mu,np,rm,rp, */
/* _otb     s           brdfints) */
/* _otb        rm(-mu)=2.*pi-phirad */
/* _otb        rm(mu)=xmus */
/* _otb        rm(0)=xmuv */
/* _otb        call oceabrdf(pws,paw,xsal,pcl,wlmoy,mu,np,rm,rp, */
/* _otb     s           brdfintv) */
/* _otb        call oceaalbe(pws,paw,xsal,pcl,wlmoy, */
/* _otb     s       albbrdf) */
/* _otb        go to 69 */
/* _otb      endif */

/* **********************************************************************c
 */
/*     brdf from Iaquinta and Pinty model */
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.7) then */
/* _otb        read(iread,*) pild,pihs */
/* _otb        read(iread,*) pxLt,pc */
/* _otb        read(iread,*) pRl,pTl,pRs */
/* _otb        rm(-mu)=phirad */
/* _otb        rm(mu)=xmuv */
/* _otb        rm(0)=xmus */
/* _otb        call iapibrdf(pild,pxlt,prl,ptl,prs,pihs,pc,mu,np,rm,rp, */
/* _otb     s           brdfints) */
/* _otb        rm(-mu)=2.*pi-phirad */
/* _otb        rm(mu)=xmus */
/* _otb        rm(0)=xmuv */
/* _otb        call iapibrdf(pild,pxlt,prl,ptl,prs,pihs,pc,mu,np,rm,rp, */
/* _otb     s           brdfintv) */
/* _otb        call iapialbe(pild,pxlt,prl,ptl,prs,pihs,pc, */
/* _otb     s       albbrdf) */
/* _otb        go to 69 */
/* _otb      endif */

/* **********************************************************************c
 */
/*     brdf from Rahman model */
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.8) then */
/* _otb      if(ibrdf.eq.9) then */
/* _otb         read(iread,*) uli,eei,thmi,sli */
/* _otb         read(iread,*) cabi,cwi,vaii,rnci,rsl1i */
/* _otb         rm(-mu)=phirad */
/* _otb        rm(mu)=xmuv */
/* _otb         rm(0)=xmus */
/*_otb         call akbrdf(eei,thmi,uli,sli,rsl1i,wlmoy,rnci,cabi,cwi,vaii
*/
/* _otb     &            ,mu,np,rm,rp,brdfints) */
/* _otb         rm(-mu)=2.*pi-phirad */
/* _otb         rm(mu)=xmus */
/* _otb         rm(0)=xmuv */
/*_otb         call akbrdf(eei,thmi,uli,sli,rsl1i,wlmoy,rnci,cabi,cwi,vaii
*/
/* _otb     &            ,mu,np,rm,rp,brdfintv) */

/* _otb         call akalbe */
/* _otb*    & (eei,thmi,uli,sli,rsl1i,wlmoy,rnci,cabi,cwi,vaii,albbrdf) */
/* _otb     & (albbrdf) */
/* _otb         go to 69 */
/* _otb      endif */

/* _otb   69 continue */
/* **********************************************************************c
 */
/* the downward irradiance was computed for a sun at thetas and         c 
*/
/* several viewing directions (mu zenith times np azimuth). then, the   c 
*/
/* code computes the product of ldown*brdf integrated over the total    c 
*/
/* hemisphere and gives the averaged directional reflectance after the  c 
*/
/* normalization. the resulting reflectance is named robar              c 
*/
/* **********************************************************************c
 */
/* _otb      robar1=0. */
/* _otb      xnorm1=0. */
/* _otb      do 83 j=1,np */
/* _otb        rob=0. */
/* _otb        xnor=0. */
/* _otb        do 84 k=1,mu-1 */
/* _otb          rdown=xlmus(-k,j) */
/* _otb          rdir=brdfintv(k,j) */
/* _otb          rob=rob+rdown*rdir*rm(k)*gb(k) */
/* _otb          xnor=xnor+rdown*rm(k)*gb(k) */
/* _otb   84   continue */
/* _otb        robar1=robar1+rob*gp(j) */
/* _otb        xnorm1=xnorm1+xnor*gp(j) */
/* _otb   83 continue */
/* **********************************************************************c
 */
/* the downward irradiance was computed for a sun at thetav and         c 
*/
/* several viewing directions (mu zenith times np azimuth). then, the   c 
*/
/* code computes the product of ldown*brdf integrated over the total    c 
*/
/* hemisphere and gives the averaged directional reflectance after the  c 
*/
/* normalization. the resulting reflectance is named robarp             c 
*/
/* **********************************************************************c
 */
/* _otb      robar2=0. */
/* _otb      xnorm2=0. */
/* _otb      do 85 j=1,np */
/* _otb        rob=0. */
/* _otb        xnor=0. */
/* _otb        do 86 k=1,mu-1 */
/* _otb          rdown=xlmuv(-k,j) */
/* _otb          rdir=brdfints(k,j) */
/* _otb          rob=rob+rdown*rdir*rm(k)*gb(k) */
/* _otb          xnor=xnor+rdown*rm(k)*gb(k) */
/* _otb   86   continue */
/* _otb        robar2=robar2+rob*gp(j) */
/* _otb        xnorm2=xnorm2+xnor*gp(j) */
/* _otb   85 continue */
/*  robard is assumed equal to albbrdf */
/*       print 301,brdfints(mu,1),robar1,xnorm1, */
/*    s       robar2,xnorm2,albbrdf */
/*       print 301,robar1/xnorm1,robar2/xnorm2 */
/*       print 301,betal(0)/3,pizmoy */
/* 301  format(6(f10.4,2x)) */
/* 501  format(5(i10,2x)) */
/* _otb      do 335 l=iinf,isup */
/* _otb        rocl(l)=brdfints(mu,1) */
/* _otb        roel(l)=brdfints(mu,1) */
/* _otb        robar(l)=robar1/xnorm1 */
/* _otb        if (idatmp.ne.0) then */
/* _otb          robarp(l)=robar2/xnorm2 */
/* _otb        else */
/* _otb          robarp(l)=0. */
/* _otb          xnorm2=1. */
/* _otb          robar2=0. */
/* _otb        endif */
/* _otb        robard(l)=albbrdf */
/* _otb  335 continue */
/* _otb      go to 34 */
/* **********************************************************************c
 */
/*     uniform surface with lambertian conditions                       c 
*/
/* **********************************************************************c
 */
/* _otb  21  read(iread,*) igroun */
/* _otb      if(igroun) 29,32,33 */
/* _otb  29  read(iread,*) (rocl(i),i=iinf,isup) */
/* _otb      goto 36 */
/* _otb  32  read(iread,*) ro */
/* _otb      do 35 l=iinf,isup */
/* _otb        rocl(l)=ro */
/* _otb   35 continue */
/* _otb      goto 36 */
/* _otb  33  if(igroun.eq.1) call vegeta(rocl) */
/* _otb      if(igroun.eq.2) call clearw(rocl) */
/* _otb      if(igroun.eq.3) call sand  (rocl) */
/* _otb      if(igroun.eq.4) call lakew (rocl) */
/* _otb   36 do 39 l=iinf,isup */
/* _otb        roel(l)=rocl(l) */
/* _otb   39 continue */
/* _otb      go to 34 */
/* **********************************************************************c
 */
/*     non-uniform conditions with lambertian conditions                c 
*/
/* **********************************************************************c
 */
/* _otb 31   read(iread,*) igrou1,igrou2,rad */
/* _otb      if(igrou1) 59,60,63 */
/* _otb  59  read(iread,*) (rocl(i),i=iinf,isup) */
/* _otb      goto 61 */
/* _otb  60  read(iread,*) roc */
/* _otb      do 64 l=iinf,isup */
/* _otb        rocl(l)=roc */
/* _otb   64 continue */
/* _otb      go to 61 */
/* _otb  63  if(igrou1.eq.1) call vegeta(rocl) */
/* _otb      if(igrou1.eq.2) call clearw(rocl) */
/* _otb      if(igrou1.eq.3) call sand  (rocl) */
/* _otb      if(igrou1.eq.4) call lakew (rocl) */
/* _otb   61 if(igrou2) 66,62,65 */
/* _otb  66  read(iread,*) (roel(i),i=iinf,isup) */
/* _otb      goto 34 */
/* _otb  62  read(iread,*) roe */
/* _otb      do 67 l=iinf,isup */
/* _otb        roel(l)=roe */
/* _otb   67 continue */
/* _otb      go to 34 */
/* _otb  65  if(igrou2.eq.1) call vegeta(roel) */
/* _otb      if(igrou2.eq.2) call clearw(roel) */
/* _otb      if(igrou2.eq.3) call sand  (roel) */
/* _otb      if(igrou2.eq.4) call lakew (roel) */
/* _otb   34 continue */
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*       rapp    that input parameter allows to activate atmospheric    c 
*/
/*               correction mode                                        c 
*/
/*                                                                      c 
*/
/*               if rapp <-1. no atmospheric correction is performed    c 
*/
/*                                                                      c 
*/
/*               if rapp >0. :  the code retrieve the value of the      c 
*/
/*               surface reflectance (rog) that will produce a radiance c 
*/
/*               equal to rapp [w/m2/str/mic] in the atmospheric        c 
*/
/*               conditions described by user before                    c 
*/
/*                                                                      c 
*/
/*               if -1.<rapp<0. : the code retrieve the value of the    c 
*/
/*               surface reflectance (rog) value that will produce a    c 
*/
/*               'reflectance' (radiance*pi/(mus*es)) equal to -rapp    c 
*/
/*               where mus is the cosine of solar zenith angle,         c 
*/
/*               es is the solar constant integrated upon the           c 
*/
/*               filter response and taking account for earth-solar     c 
*/
/*               distance, es is in [w/m2/sr/mic].                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* _otb_adaptation Beginning: rapp=-10. no atmospheric correction */
/* _otb        read(iread,*) rapp */
/*< 	rapp=-10. >*/
    rapp = (float)-10.;
/* _otb_adaptation End : rapp=-10. no atmospheric correction */
/* **********************************************************************c
 */
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                     example of input cards                           c 
*/
/*                                                                      c 
*/
/* 4                            (avhrr observation)                     c 
*/
/* 7 6 10.1  600  0.0  10.0     (month,day,htu,cn,longan,han)           c 
*/
/* 8                            (user's   model)                        c 
*/
/* 3.0   0.35                   ( uh2o(g/cm2) ,uo3(cm-atm) )            c 
*/
/* 4                            (aerosols model)                        c 
*/
/* 0.25  0.25  0.25  0.25       ( % of:dust-like,water-sol,oceanic,soot)c 
*/
/* 23.0                         (visibility (km) )                      c 
*/
/* -0.5                         (target at 0.5km high)                  c 
*/
/* -1000                        (sensor aboard a satellite)             c 
*/
/* 6                            (avhrr 2 (noaa 8) band)                 c 
*/
/* 1                            (ground type,i.e. non homogeneous)      c 
*/
/* 2    1    0.50               (target,env.,radius(km) )               c 
*/
/* -0.10                        (atmospheric correction mode for a TOA  c 
*/
/*                                   reflectance equal to 0.10)         c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* **********************************************************************c
 */
/*                     print of initial conditions                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* ---- geometrical conditions ---- */
/*<       write(iwr, 98) >*/
    io___115.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___115);
    e_wsfe();
/*<       write(iwr, etiq1(igeom+1)) >*/
    ci__1.cierr = 0;
    ci__1.ciunit = sixs_ier__1.iwr;
    ci__1.cifmt = etiq1 + igeom * 60;
    s_wsfe(&ci__1);
    e_wsfe();
/*<       if(igeom.eq.0) then >*/
    if (igeom == 0) {
/*< 	 write(iwr, 1401) >*/
	io___116.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___116);
	e_wsfe();
/*< 	 write(iwr, 103)month,jday >*/
	io___117.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___117);
	do_fio(&c__1, (char *)&month, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&jday, (ftnlen)sizeof(integer));
	e_wsfe();
/*<       endif >*/
    }
/*<       if(igeom.ne.0) write(iwr, 101)month,jday,tu,xlat,xlon >*/
    if (igeom != 0) {
	io___118.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___118);
	do_fio(&c__1, (char *)&month, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&jday, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&tu, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&xlat, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&xlon, (ftnlen)sizeof(real));
	e_wsfe();
    }
/*<       write(iwr, 102)asol,phi0 >*/
    io___122.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___122);
    do_fio(&c__1, (char *)&asol, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&phi0, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 1110)avis,phiv,adif,phi >*/
    io___123.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___123);
    do_fio(&c__1, (char *)&avis, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&phiv, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&adif, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&phi, (ftnlen)sizeof(real));
    e_wsfe();
/* --- atmospheric model ---- */
/*<       write(iwr, 1119) >*/
    io___124.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___124);
    e_wsfe();
/*<       if(idatm-7)226,227,228 >*/
    if ((i__1 = idatm - 7) < 0) {
	goto L226;
    } else if (i__1 == 0) {
	goto L227;
    } else {
	goto L228;
    }
/*<   228 write(iwr, 1281)uw,uo3 >*/
L228:
    io___125.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___125);
    do_fio(&c__1, (char *)&uw, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&uo3, (ftnlen)sizeof(real));
    e_wsfe();
/*<       goto 219 >*/
    goto L219;
/*<   227 write(iwr, 1272) >*/
L227:
    io___126.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___126);
    e_wsfe();
/*<       do 229 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<         write(iwr, 1271)z(i),p(i),t(i),wh(i),wo(i) >*/
	io___127.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___127);
	do_fio(&c__1, (char *)&sixs_atm__1.z__[i__ - 1], (ftnlen)sizeof(real))
		;
	do_fio(&c__1, (char *)&sixs_atm__1.p[i__ - 1], (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&sixs_atm__1.t[i__ - 1], (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&sixs_atm__1.wh[i__ - 1], (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&sixs_atm__1.wo[i__ - 1], (ftnlen)sizeof(real));
	e_wsfe();
/*<   229 continue >*/
/* L229: */
    }
/*<       goto 219 >*/
    goto L219;
/*<   226 write(iwr, 1261)atmid(idatm+1) >*/
L226:
    io___128.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___128);
    do_fio(&c__1, atmid + idatm * 51, 51L);
    e_wsfe();
/* --- aerosols model (type) ---- */
/*<  219  if (iaer.lt.4) then >*/
L219:
    if (iaer < 4) {
/*<         goto(230,231,232,233),iaer+1 >*/
	switch (iaer + 1) {
	    case 1:  goto L230;
	    case 2:  goto L231;
	    case 3:  goto L232;
	    case 4:  goto L233;
	}
/*<       else >*/
    } else {
/*<         if (iaer.ge.5.and.iaer.le.7) goto(234,235,236),iaer-4 >*/
	if (iaer >= 5 && iaer <= 7) {
	    switch (iaer - 4) {
		case 1:  goto L234;
		case 2:  goto L235;
		case 3:  goto L236;
	    }
	}
/*<         if (iaer.eq.4)write(iwr,133)(c(i),i=1,4) >*/
	if (iaer == 4) {
	    io___129.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___129);
	    for (i__ = 1; i__ <= 4; ++i__) {
		do_fio(&c__1, (char *)&c__[i__ - 1], (ftnlen)sizeof(real));
	    }
	    e_wsfe();
	}
/*<         if (iaer.eq.8)then >*/
	if (iaer == 8) {
/*<           write(iwr,134)icp >*/
	    io___130.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___130);
	    do_fio(&c__1, (char *)&mie_in__1.icp, (ftnlen)sizeof(integer));
	    e_wsfe();
/*<           do i=1,icp >*/
	    i__1 = mie_in__1.icp;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/*<             write(iwr,135)x1(i),x2(i),cij(i) >*/
		io___131.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___131);
		do_fio(&c__1, (char *)&mie_in__1.x1[i__ - 1], (ftnlen)sizeof(
			real));
		do_fio(&c__1, (char *)&mie_in__1.x2[i__ - 1], (ftnlen)sizeof(
			real));
		do_fio(&c__1, (char *)&mie_in__1.cij[i__ - 1], (ftnlen)sizeof(
			real));
		e_wsfe();
/*<           enddo >*/
	    }
/*<         endif >*/
	}
/*<         if (iaer.eq.9)write(iwr,136)x1(1),x2(1),x3(1) >*/
	if (iaer == 9) {
	    io___132.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___132);
	    do_fio(&c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&mie_in__1.x2[0], (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&mie_in__1.x3[0], (ftnlen)sizeof(real));
	    e_wsfe();
	}
/*<         if (iaer.eq.10)write(iwr,137)x1(1) >*/
	if (iaer == 10) {
	    io___133.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___133);
	    do_fio(&c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(real));
	    e_wsfe();
	}
/*<         if (iaer.eq.11)write(iwr, 132)' Sun Photometer' >*/
	if (iaer == 11) {
	    io___134.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___134);
	    do_fio(&c__1, " Sun Photometer", 15L);
	    e_wsfe();
	}
/*<         if (iaer.eq.12)write(iwr,138)FILE2(1:i2) >*/
	if (iaer == 12) {
	    io___135.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___135);
	    do_fio(&c__1, file2, i2);
	    e_wsfe();
	}
/*< 	if (iaerp.eq.1)write(iwr,139)FILE2(1:i2) >*/
	if (iaerp == 1) {
	    io___136.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___136);
	    do_fio(&c__1, file2, i2);
	    e_wsfe();
	}
/*<         goto 249 >*/
	goto L249;
/*<       endif >*/
    }
/*<   234 write(iwr, 132)'       Desertic' >*/
L234:
    io___137.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___137);
    do_fio(&c__1, "       Desertic", 15L);
    e_wsfe();
/*<       goto 249 >*/
    goto L249;
/*<   235 write(iwr, 132)'          Smoke' >*/
L235:
    io___138.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___138);
    do_fio(&c__1, "          Smoke", 15L);
    e_wsfe();
/*<       goto 249 >*/
    goto L249;
/*<   236 write(iwr, 132)'  Stratospheric' >*/
L236:
    io___139.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___139);
    do_fio(&c__1, "  Stratospheric", 15L);
    e_wsfe();
/*<       goto 249 >*/
    goto L249;
/*<   233 write(iwr, 132)'          Urban' >*/
L233:
    io___140.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___140);
    do_fio(&c__1, "          Urban", 15L);
    e_wsfe();
/*<       go to 249 >*/
    goto L249;
/*<   232 write(iwr, 132)'       Maritime' >*/
L232:
    io___141.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___141);
    do_fio(&c__1, "       Maritime", 15L);
    e_wsfe();
/*<       goto 249 >*/
    goto L249;
/*<   231 write(iwr, 132)'    Continental' >*/
L231:
    io___142.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___142);
    do_fio(&c__1, "    Continental", 15L);
    e_wsfe();
/*<       goto 249 >*/
    goto L249;
/*<   230 write(iwr, 1301) >*/
L230:
    io___143.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___143);
    e_wsfe();
/*<   249 continue >*/
L249:
/* --- aerosol model (concentration) ---- */
/*<       if(iaer.eq.0) write(iwr, 1401) >*/
    if (iaer == 0) {
	io___144.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___144);
	e_wsfe();
    }
/*<       if(iaer.eq.0) goto 1112 >*/
    if (iaer == 0) {
	goto L1112;
    }
/*<       if(abs(v).le.xacc) write(iwr, 140)taer55 >*/
    if (dabs(v) <= sixs_test__1.xacc) {
	io___145.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___145);
	do_fio(&c__1, (char *)&taer55, (ftnlen)sizeof(real));
	e_wsfe();
    }
/*<       if(abs(v).gt.xacc) write(iwr, 141)v,taer55 >*/
    if (dabs(v) > sixs_test__1.xacc) {
	io___146.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___146);
	do_fio(&c__1, (char *)&v, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&taer55, (ftnlen)sizeof(real));
	e_wsfe();
    }
/* --- spectral condition ---- */
/*<  1112 write(iwr, 148) >*/
L1112:
    io___147.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___147);
    e_wsfe();
/*<       if(iwave.eq.-2) write(iwr, 1510) nsat(1),wlinf,wlsup >*/
    if (iwave == -2) {
	io___148.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___148);
	do_fio(&c__1, nsat, 17L);
	do_fio(&c__1, (char *)&sixs_ffu__1.wlinf, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&sixs_ffu__1.wlsup, (ftnlen)sizeof(real));
	e_wsfe();
    }
/*<       if(iwave.eq.-1) write(iwr, 149) wl >*/
    if (iwave == -1) {
	io___149.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___149);
	do_fio(&c__1, (char *)&wl, (ftnlen)sizeof(real));
	e_wsfe();
    }
/*<       if(iwave.ge.0) write(iwr, 1510) nsat(iwave+1), wlinf,wlsup >*/
    if (iwave >= 0) {
	io___150.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___150);
	do_fio(&c__1, nsat + iwave * 17, 17L);
	do_fio(&c__1, (char *)&sixs_ffu__1.wlinf, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&sixs_ffu__1.wlsup, (ftnlen)sizeof(real));
	e_wsfe();
    }
/* --- ground reflectance (type and spectral variation) ---- */
/*<       if(idirec.eq.0) then >*/
    if (idirec == 0) {
/*<         rocave=0. >*/
	rocave = (float)0.;
/*<         roeave=0. >*/
	roeave = (float)0.;
/*<         seb=0. >*/
	seb = (float)0.;
/*<         do 264 i=iinf,isup >*/
	i__1 = isup;
	for (i__ = iinf; i__ <= i__1; ++i__) {
/*<           sbor=s(i) >*/
	    sbor = sixs_ffu__1.s[i__ - 1];
/*<           if(i.eq.iinf.or.i.eq.isup) sbor=sbor*0.5 >*/
	    if (i__ == iinf || i__ == isup) {
		sbor *= (float).5;
	    }
/*<           wl=.25+(i-1)*step >*/
	    wl = (i__ - 1) * step + (float).25;
/*<    >*/
	    solirr_(&wl, &swl);
/*<           swl=swl*dsol >*/
	    swl *= dsol;
/*<           rocave=rocave+rocl(i)*sbor*swl*step >*/
	    rocave += rocl[i__ - 1] * sbor * swl * step;
/*<           roeave=roeave+roel(i)*sbor*swl*step >*/
	    roeave += roel[i__ - 1] * sbor * swl * step;
/*<           seb=seb+sbor*swl*step >*/
	    seb += sbor * swl * step;
/*<   264   continue >*/
/* L264: */
	}
/*<         rocave=rocave/seb >*/
	rocave /= seb;
/*<         roeave=roeave/seb >*/
	roeave /= seb;
/*<         isort=0 >*/
	isort = 0;
/*<         ro=rocave >*/
	ro = rocave;
/*<         if(inhomo.eq.0) goto 260 >*/
	if (inhomo == 0) {
	    goto L260;
	}
/*<         write(iwr, 169)rad >*/
	io___158.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___158);
	do_fio(&c__1, (char *)&rad, (ftnlen)sizeof(real));
	e_wsfe();
/*<         igroun=igrou1 >*/
	igroun = igrou1;
/*<         ro=rocave >*/
	ro = rocave;
/*<         write(iwr, 170) >*/
	io___161.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___161);
	e_wsfe();
/*<         goto 261 >*/
	goto L261;
/*<   262   igroun=igrou2 >*/
L262:
	igroun = igrou2;
/*<         ro=roeave >*/
	ro = roeave;
/*<         write(iwr, 171) >*/
	io___163.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___163);
	e_wsfe();
/*<         goto 261 >*/
	goto L261;
/*<   260   write(iwr, 168) >*/
L260:
	io___164.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___164);
	e_wsfe();
/*<   261   if (igroun.gt.0)write(iwr, reflec(igroun+3))ro >*/
L261:
	if (igroun > 0) {
	    ci__1.cierr = 0;
	    ci__1.ciunit = sixs_ier__1.iwr;
	    ci__1.cifmt = reflec + (igroun + 2) * 71;
	    s_wsfe(&ci__1);
	    do_fio(&c__1, (char *)&ro, (ftnlen)sizeof(real));
	    e_wsfe();
	}
/*<         if (igroun.gt.0)goto 158 >*/
	if (igroun > 0) {
	    goto L158;
	}
/*<         if(igroun.eq.-1) write(iwr, reflec(1))ro >*/
	if (igroun == -1) {
	    ci__1.cierr = 0;
	    ci__1.ciunit = sixs_ier__1.iwr;
	    ci__1.cifmt = reflec;
	    s_wsfe(&ci__1);
	    do_fio(&c__1, (char *)&ro, (ftnlen)sizeof(real));
	    e_wsfe();
	}
/*<         if(igroun.eq.-1) goto 158 >*/
	if (igroun == -1) {
	    goto L158;
	}
/*<         if(iwave.eq.-1)  write(iwr, reflec(2))ro >*/
	if (iwave == -1) {
	    ci__1.cierr = 0;
	    ci__1.ciunit = sixs_ier__1.iwr;
	    ci__1.cifmt = reflec + 71;
	    s_wsfe(&ci__1);
	    do_fio(&c__1, (char *)&ro, (ftnlen)sizeof(real));
	    e_wsfe();
	}
/*<         if(iwave.ne.-1)  write(iwr, reflec(3))ro >*/
	if (iwave != -1) {
	    ci__1.cierr = 0;
	    ci__1.ciunit = sixs_ier__1.iwr;
	    ci__1.cifmt = reflec + 142;
	    s_wsfe(&ci__1);
	    do_fio(&c__1, (char *)&ro, (ftnlen)sizeof(real));
	    e_wsfe();
	}
/*<  158    isort=isort+1 >*/
L158:
	++isort;
/*<         if(inhomo.eq.0) goto 999 >*/
	if (inhomo == 0) {
	    goto L999;
	}
/*<         if(isort.eq.2) goto 999 >*/
	if (isort == 2) {
	    goto L999;
	}
/*<         goto 262 >*/
	goto L262;
/*<       else >*/
    } else {
/*<         write(iwr, 168) >*/
	io___165.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___165);
	e_wsfe();
/*<         if(idirec.eq.1) then >*/
	if (idirec == 1) {
/*<    >*/
	    switch (ibrdf + 1) {
		case 1:  goto L2000;
		case 2:  goto L2001;
		case 3:  goto L2002;
		case 4:  goto L2003;
		case 5:  goto L2004;
		case 6:  goto L2005;
		case 7:  goto L2006;
		case 8:  goto L2007;
		case 9:  goto L2008;
		case 10:  goto L2010;
	    }
/*<  2000    write(iwr, 190) >*/
L2000:
	    io___167.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___167);
	    e_wsfe();
/*<    >*/
	    io___168.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___168);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2001    write(iwr, 191)par1,par2,par3,par4 >*/
L2001:
	    io___175.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___175);
	    do_fio(&c__1, (char *)&par1, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par3, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par4, (ftnlen)sizeof(real));
	    e_wsfe();
/*<    >*/
	    io___180.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___180);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2002    write(iwr, 192)optics(1),struct(1),struct(2) >*/
L2002:
	    io___181.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___181);
	    do_fio(&c__1, (char *)&optics[0], (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&struct__[0], (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&struct__[1], (ftnlen)sizeof(real));
	    e_wsfe();
/*<          if (options(5).eq.0) write(iwr, 200) >*/
	    if (options[4] == 0) {
		io___185.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___185);
		e_wsfe();
	    }
/*<          if (options(5).eq.1) write(iwr, 201) >*/
	    if (options[4] == 1) {
		io___186.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___186);
		e_wsfe();
	    }
/*<          if (options(3).eq.0) write(iwr, 197)struct(3),struct(4) >*/
	    if (options[2] == 0) {
		io___187.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___187);
		do_fio(&c__1, (char *)&struct__[2], (ftnlen)sizeof(real));
		do_fio(&c__1, (char *)&struct__[3], (ftnlen)sizeof(real));
		e_wsfe();
	    }
/*<          if (options(3).eq.1) write(iwr, 198)struct(3) >*/
	    if (options[2] == 1) {
		io___188.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___188);
		do_fio(&c__1, (char *)&struct__[2], (ftnlen)sizeof(real));
		e_wsfe();
	    }
/*<          if (options(3).eq.2) write(iwr, 199)struct(3) >*/
	    if (options[2] == 2) {
		io___189.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___189);
		do_fio(&c__1, (char *)&struct__[2], (ftnlen)sizeof(real));
		e_wsfe();
	    }
/*<          if (options(4).eq.0) write(iwr, 202) >*/
	    if (options[3] == 0) {
		io___190.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___190);
		e_wsfe();
	    }
/*<          if (options(4).eq.1) write(iwr, 203)optics(2) >*/
	    if (options[3] == 1) {
		io___191.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___191);
		do_fio(&c__1, (char *)&optics[1], (ftnlen)sizeof(real));
		e_wsfe();
	    }
/*<          if (options(4).eq.2) write(iwr, 204)optics(2),optics(3) >*/
	    if (options[3] == 2) {
		io___192.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___192);
		do_fio(&c__1, (char *)&optics[1], (ftnlen)sizeof(real));
		do_fio(&c__1, (char *)&optics[2], (ftnlen)sizeof(real));
		e_wsfe();
	    }
/*<    >*/
	    io___193.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___193);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2003    write(iwr, 193)par1,par2,par3 >*/
L2003:
	    io___194.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___194);
	    do_fio(&c__1, (char *)&par1, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par3, (ftnlen)sizeof(real));
	    e_wsfe();
/*<    >*/
	    io___195.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___195);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2004    write(iwr, 194)par1,par2,par3,par4 >*/
L2004:
	    io___196.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___196);
	    do_fio(&c__1, (char *)&par1, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par3, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par4, (ftnlen)sizeof(real));
	    e_wsfe();
/*<    >*/
	    io___197.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___197);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2005    write(iwr, 195)par1,par2 >*/
L2005:
	    io___198.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___198);
	    do_fio(&c__1, (char *)&par1, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par2, (ftnlen)sizeof(real));
	    e_wsfe();
/*<    >*/
	    io___199.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___199);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2006    write(iwr, 196)pws,phi_wind,xsal,pcl >*/
L2006:
	    io___200.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___200);
	    do_fio(&c__1, (char *)&pws, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&phi_wind__, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&xsal, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&pcl, (ftnlen)sizeof(real));
	    e_wsfe();
/*<    >*/
	    io___205.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___205);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2007    write(iwr, 205) pRl,pTl,pRs,PxLt >*/
L2007:
	    io___206.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___206);
	    do_fio(&c__1, (char *)&prl, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&ptl, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&prs, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&pxlt, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          if (pihs.eq.0) then >*/
	    if (pihs == 0) {
/*<            write(iwr,207)' no hot spot       ' >*/
		io___212.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___212);
		do_fio(&c__1, " no hot spot       ", 19L);
		e_wsfe();
/*<          else >*/
	    } else {
/*<            write(iwr,208)' hot spot parameter',pc >*/
		io___213.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___213);
		do_fio(&c__1, " hot spot parameter", 19L);
		do_fio(&c__1, (char *)&pc, (ftnlen)sizeof(real));
		e_wsfe();
/*<          endif >*/
	    }
/*<          if (pild.eq.1) write(iwr,209) ' planophile   leaf distribution' >*/
	    if (pild == 1) {
		io___216.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___216);
		do_fio(&c__1, " planophile   leaf distribution", 31L);
		e_wsfe();
	    }
/*<          if (pild.eq.2) write(iwr,209) ' erectophile  leaf distribution' >*/
	    if (pild == 2) {
		io___217.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___217);
		do_fio(&c__1, " erectophile  leaf distribution", 31L);
		e_wsfe();
	    }
/*<          if (pild.eq.3) write(iwr,209) ' plagiophile  leaf distribution' >*/
	    if (pild == 3) {
		io___218.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___218);
		do_fio(&c__1, " plagiophile  leaf distribution", 31L);
		e_wsfe();
	    }
/*<          if (pild.eq.4) write(iwr,209) ' extremophile leaf distribution' >*/
	    if (pild == 4) {
		io___219.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___219);
		do_fio(&c__1, " extremophile leaf distribution", 31L);
		e_wsfe();
	    }
/*<          if (pild.eq.5) write(iwr,209) ' uniform      leaf distribution' >*/
	    if (pild == 5) {
		io___220.ciunit = sixs_ier__1.iwr;
		s_wsfe(&io___220);
		do_fio(&c__1, " uniform      leaf distribution", 31L);
		e_wsfe();
	    }
/*<    >*/
	    io___221.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___221);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2008    write(iwr, 206) par1,par2,par3 >*/
L2008:
	    io___222.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___222);
	    do_fio(&c__1, (char *)&par1, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&par3, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2010    write(iwr, 210)uli,eei,thmi,sli,cabi,cwi,vaii,rnci,rsl1i >*/
L2010:
	    io___223.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___223);
	    do_fio(&c__1, (char *)&uli, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&eei, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&thmi, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&sli, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&cabi, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&cwi, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&vaii, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&rnci, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&rsl1i, (ftnlen)sizeof(real));
	    e_wsfe();
/*<    >*/
	    io___233.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___233);
	    do_fio(&c__1, (char *)&brdfints[mu + 25], (ftnlen)sizeof(real));
	    r__1 = robar1 / xnorm1;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    r__2 = robar2 / xnorm2;
	    do_fio(&c__1, (char *)&r__2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&albbrdf, (ftnlen)sizeof(real));
	    e_wsfe();
/*<          goto 2009 >*/
	    goto L2009;
/*<  2009   endif >*/
L2009:
	    ;
	}
/*<       endif >*/
    }
/* --- pressure at ground level (174) and altitude (175) ---- */
/*<   999 write(iwr, 173) >*/
L999:
    io___234.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___234);
    e_wsfe();
/*<       write(iwr, 174)p(1) >*/
    io___235.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___235);
    do_fio(&c__1, (char *)&sixs_atm__1.p[0], (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 175)xps >*/
    io___236.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___236);
    do_fio(&c__1, (char *)&xps, (ftnlen)sizeof(real));
    e_wsfe();
/*<       if (xps.gt.0.) write(iwr, 176)uw,uo3 >*/
    if (xps > (float)0.) {
	io___237.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___237);
	do_fio(&c__1, (char *)&uw, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&uo3, (ftnlen)sizeof(real));
	e_wsfe();
    }
/* --- plane simulation output if selected ---- */
/*<       if (palt.lt.1000.) then >*/
    if (palt < (float)1e3) {
/*<        write(iwr, 178) >*/
	io___238.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___238);
	e_wsfe();
/*<        write(iwr, 179)pps >*/
	io___239.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___239);
	do_fio(&c__1, (char *)&pps, (ftnlen)sizeof(real));
	e_wsfe();
/*<        write(iwr, 180)zpl(34) >*/
	io___240.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___240);
	do_fio(&c__1, (char *)&sixs_planesim__1.zpl[33], (ftnlen)sizeof(real))
		;
	e_wsfe();
/*<        write(iwr, 181) >*/
	io___241.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___241);
	e_wsfe();
/*<        write(iwr, 182)puo3 >*/
	io___242.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___242);
	do_fio(&c__1, (char *)&puo3, (ftnlen)sizeof(real));
	e_wsfe();
/*<        write(iwr, 183)puw >*/
	io___243.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___243);
	do_fio(&c__1, (char *)&puw, (ftnlen)sizeof(real));
	e_wsfe();
/*<        write(iwr, 184)taer55p >*/
	io___244.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___244);
	do_fio(&c__1, (char *)&taer55p, (ftnlen)sizeof(real));
	e_wsfe();
/*<       endif >*/
    }
/* ---- atmospheric correction  ---- */
/*<       if (rapp.gt.-1.) then >*/
    if (rapp > (float)-1.) {
/*<        write(iwr, 177) >*/
	io___245.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___245);
	e_wsfe();
/*<        if (rapp.lt.0.) then >*/
	if (rapp < (float)0.) {
/*<         write(iwr, 185)-rapp >*/
	    io___246.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___246);
	    r__1 = -rapp;
	    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
	    e_wsfe();
/*<        else >*/
	} else {
/*<         write(iwr, 186)rapp >*/
	    io___247.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___247);
	    do_fio(&c__1, (char *)&rapp, (ftnlen)sizeof(real));
	    e_wsfe();
/*<        endif >*/
	}
/*<       endif >*/
    }
/*<       write(iwr, 172) >*/
    io___248.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___248);
    e_wsfe();
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
/* ---- initilialization */
/*<       sb=0. >*/
    sb = (float)0.;
/*<       seb=0. >*/
    seb = (float)0.;
/*<       refet=0. >*/
    refet = (float)0.;
/*<       refet1=0. >*/
    refet1 = (float)0.;
/*<       refet2=0. >*/
    refet2 = (float)0.;
/*<       refet3=0. >*/
    refet3 = (float)0.;
/*<       alumet=0. >*/
    alumet = (float)0.;
/*<       tgasm=0. >*/
    tgasm = (float)0.;
/*<       rog=0. >*/
    rog = (float)0.;
/*<       dgasm=0. >*/
    dgasm = (float)0.;
/*<       ugasm=0. >*/
    ugasm = (float)0.;
/*<       sdwava=0. >*/
    sdwava = (float)0.;
/*<       sdozon=0. >*/
    sdozon = (float)0.;
/*<       sddica=0. >*/
    sddica = (float)0.;
/*<       sdoxyg=0. >*/
    sdoxyg = (float)0.;
/*<       sdniox=0. >*/
    sdniox = (float)0.;
/*<       sdmoca=0. >*/
    sdmoca = (float)0.;
/*<       sdmeth=0. >*/
    sdmeth = (float)0.;
/*<       suwava=0. >*/
    suwava = (float)0.;
/*<       suozon=0. >*/
    suozon = (float)0.;
/*<       sudica=0. >*/
    sudica = (float)0.;
/*<       suoxyg=0. >*/
    suoxyg = (float)0.;
/*<       suniox=0. >*/
    suniox = (float)0.;
/*<       sumoca=0. >*/
    sumoca = (float)0.;
/*<       sumeth=0. >*/
    sumeth = (float)0.;
/*<       stwava=0. >*/
    stwava = (float)0.;
/*<       stozon=0. >*/
    stozon = (float)0.;
/*<       stdica=0. >*/
    stdica = (float)0.;
/*<       stoxyg=0. >*/
    stoxyg = (float)0.;
/*<       stniox=0. >*/
    stniox = (float)0.;
/*<       stmoca=0. >*/
    stmoca = (float)0.;
/*<       stmeth=0. >*/
    stmeth = (float)0.;
/*<       sodray=0. >*/
    sodray = (float)0.;
/*<       sodrayp=0. >*/
    sodrayp = (float)0.;
/*<       sodaer=0. >*/
    sodaer = (float)0.;
/*<       sodaerp=0. >*/
    sodaerp = (float)0.;
/*<       sodtot=0. >*/
    sodtot = (float)0.;
/*<       sodtotp=0. >*/
    sodtotp = (float)0.;
/*<       fophsr=0. >*/
    fophsr = (float)0.;
/*<       fophsa=0. >*/
    fophsa = (float)0.;
/*<       sroray=0. >*/
    sroray = (float)0.;
/*<       sroaer=0. >*/
    sroaer = (float)0.;
/*<       srotot=0. >*/
    srotot = (float)0.;
/*<       ssdaer=0. >*/
    ssdaer = (float)0.;
/*<       sdtotr=0. >*/
    sdtotr = (float)0.;
/*<       sdtota=0. >*/
    sdtota = (float)0.;
/*<       sdtott=0. >*/
    sdtott = (float)0.;
/*<       sutotr=0. >*/
    sutotr = (float)0.;
/*<       sutota=0. >*/
    sutota = (float)0.;
/*<       sutott=0. >*/
    sutott = (float)0.;
/*<       sasr=0. >*/
    sasr = (float)0.;
/*<       sasa=0. >*/
    sasa = (float)0.;
/*<       sast=0. >*/
    sast = (float)0.;
/*<       do 52 i=1,2 >*/
    for (i__ = 1; i__ <= 2; ++i__) {
/*<         do 53 j=1,3 >*/
	for (j = 1; j <= 3; ++j) {
/*<           ani(i,j)=0. >*/
	    ani[i__ + (j << 1) - 3] = (float)0.;
/*<           aini(i,j)=0. >*/
	    aini[i__ + (j << 1) - 3] = (float)0.;
/*<           anr(i,j)=0. >*/
	    anr[i__ + (j << 1) - 3] = (float)0.;
/*<           ainr(i,j)=0. >*/
	    ainr[i__ + (j << 1) - 3] = (float)0.;
/*<    53   continue >*/
/* L53: */
	}
/*<    52 continue >*/
/* L52: */
    }
/* ---- spectral loop ---- */
/*<       if (iwave.eq.-2) write(iwr,1500) >*/
    if (iwave == -2) {
	io___305.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___305);
	e_wsfe();
    }
/*<       do 51 l=iinf,isup >*/
    i__1 = isup;
    for (l = iinf; l <= i__1; ++l) {
/*<         sbor=s(l) >*/
	sbor = sixs_ffu__1.s[l - 1];
/*<         if(l.eq.iinf.or.l.eq.isup) sbor=sbor*0.5 >*/
	if (l == iinf || l == isup) {
	    sbor *= (float).5;
	}
/*<         if(iwave.eq.-1) sbor=1.0/step >*/
	if (iwave == -1) {
	    sbor = (float)1. / step;
	}
/*<         roc=rocl(l) >*/
	roc = rocl[l - 1];
/*<         roe=roel(l) >*/
	roe = roel[l - 1];
/*<         wl=.25+(l-1)*step >*/
	wl = (l - 1) * step + (float).25;

/*<    >*/
	r__1 = uw / (float)2.;
	r__2 = puw / (float)2.;
	abstra_(&idatm, &wl, &xmus, &xmuv, &r__1, &uo3, &uwus, &uo3us, &
		idatmp, &r__2, &puo3, &puwus, &puo3us, &dtwava, &dtozon, &
		dtdica, &dtoxyg, &dtniox, &dtmeth, &dtmoca, &utwava, &utozon, 
		&utdica, &utoxyg, &utniox, &utmeth, &utmoca, &attwava, &
		ttozon, &ttdica, &ttoxyg, &ttniox, &ttmeth, &ttmoca);
/*<    >*/
	abstra_(&idatm, &wl, &xmus, &xmuv, &uw, &uo3, &uwus, &uo3us, &idatmp, 
		&puw, &puo3, &puwus, &puo3us, &dtwava, &dtozon, &dtdica, &
		dtoxyg, &dtniox, &dtmeth, &dtmoca, &utwava, &utozon, &utdica, 
		&utoxyg, &utniox, &utmeth, &utmoca, &ttwava, &ttozon, &ttdica,
		 &ttoxyg, &ttniox, &ttmeth, &ttmoca);
/*<         if (dtwava.lt.accu3) dtwava=0. >*/
	if (dtwava < accu3) {
	    dtwava = (float)0.;
	}
/*<         if (dtozon.lt.accu3) dtozon=0. >*/
	if (dtozon < accu3) {
	    dtozon = (float)0.;
	}
/*<         if (dtdica.lt.accu3) dtdica=0. >*/
	if (dtdica < accu3) {
	    dtdica = (float)0.;
	}
/*<         if (dtniox.lt.accu3) dtniox=0. >*/
	if (dtniox < accu3) {
	    dtniox = (float)0.;
	}
/*<         if (dtmeth.lt.accu3) dtmeth=0. >*/
	if (dtmeth < accu3) {
	    dtmeth = (float)0.;
	}
/*<         if (dtmoca.lt.accu3) dtmeth=0. >*/
	if (dtmoca < accu3) {
	    dtmeth = (float)0.;
	}
/*<         if (utwava.lt.accu3) utwava=0. >*/
	if (utwava < accu3) {
	    utwava = (float)0.;
	}
/*<         if (utozon.lt.accu3) utozon=0. >*/
	if (utozon < accu3) {
	    utozon = (float)0.;
	}
/*<         if (utdica.lt.accu3) utdica=0. >*/
	if (utdica < accu3) {
	    utdica = (float)0.;
	}
/*<         if (utniox.lt.accu3) utniox=0. >*/
	if (utniox < accu3) {
	    utniox = (float)0.;
	}
/*<         if (utmeth.lt.accu3) utmeth=0. >*/
	if (utmeth < accu3) {
	    utmeth = (float)0.;
	}
/*<         if (utmoca.lt.accu3) utmeth=0. >*/
	if (utmoca < accu3) {
	    utmeth = (float)0.;
	}
/*<         if (ttwava.lt.accu3) ttwava=0. >*/
	if (ttwava < accu3) {
	    ttwava = (float)0.;
	}
/*<         if (ttozon.lt.accu3) ttozon=0. >*/
	if (ttozon < accu3) {
	    ttozon = (float)0.;
	}
/*<         if (ttdica.lt.accu3) ttdica=0. >*/
	if (ttdica < accu3) {
	    ttdica = (float)0.;
	}
/*<         if (ttniox.lt.accu3) ttniox=0. >*/
	if (ttniox < accu3) {
	    ttniox = (float)0.;
	}
/*<         if (ttmeth.lt.accu3) ttmeth=0. >*/
	if (ttmeth < accu3) {
	    ttmeth = (float)0.;
	}
/*<         if (ttmoca.lt.accu3) ttmeth=0. >*/
	if (ttmoca < accu3) {
	    ttmeth = (float)0.;
	}

/*<    >*/
	solirr_(&wl, &swl);
/*<         swl=swl*dsol >*/
	swl *= dsol;
/*<         coef=sbor*step*swl >*/
	coef = sbor * step * swl;
/*<    >*/
	interp_(&iaer, &idatmp, &wl, &taer55, &taer55p, &xmud, &romix, &
		rorayl, &roaero, &phaa, &phar, &tsca, &tray, &trayp, &taer, &
		taerp, &dtott, &utott, &astot, &asray, &asaer, &utotr, &utota,
		 &dtotr, &dtota);
/*<         dgtot=dtwava*dtozon*dtdica*dtoxyg*dtniox*dtmeth*dtmoca >*/
	dgtot = dtwava * dtozon * dtdica * dtoxyg * dtniox * dtmeth * dtmoca;
/*<         tgtot=ttwava*ttozon*ttdica*ttoxyg*ttniox*ttmeth*ttmoca >*/
	tgtot = ttwava * ttozon * ttdica * ttoxyg * ttniox * ttmeth * ttmoca;
/*<         ugtot=utwava*utozon*utdica*utoxyg*utniox*utmeth*utmoca >*/
	ugtot = utwava * utozon * utdica * utoxyg * utniox * utmeth * utmoca;
/*<         tgp1=ttozon*ttdica*ttoxyg*ttniox*ttmeth*ttmoca >*/
	tgp1 = ttozon * ttdica * ttoxyg * ttniox * ttmeth * ttmoca;
/*<         tgp2=attwava*ttozon*ttdica*ttoxyg*ttniox*ttmeth*ttmoca >*/
	tgp2 = attwava * ttozon * ttdica * ttoxyg * ttniox * ttmeth * ttmoca;
/*<         edifr=utotr-exp(-trayp/xmuv) >*/
	edifr = utotr - exp(-trayp / xmuv);
/*<         edifa=utota-exp(-taerp/xmuv) >*/
	edifa = utota - exp(-taerp / xmuv);
/*<         if (idirec.eq.1) then >*/
	if (idirec == 1) {
/*<           tdird=exp(-(trayp+taerp)/xmus) >*/
	    tdird = exp(-(trayp + taerp) / xmus);
/*<           tdiru=exp(-(trayp+taerp)/xmuv) >*/
	    tdiru = exp(-(trayp + taerp) / xmuv);
/*<           tdifd=dtott-tdird >*/
	    tdifd = dtott - tdird;
/*<           tdifu=utott-tdiru >*/
	    tdifu = utott - tdiru;
/*< 	  >*/
	    rsurf = roc * tdird * tdiru + robar[l - 1] * tdifd * tdiru + 
		    robarp[l - 1] * tdifu * tdird + robard[l - 1] * tdifd * 
		    tdifu + (tdifd + tdird) * (tdifu + tdiru) * astot * 
		    robard[l - 1] * robard[l - 1] / ((float)1. - astot * 
		    robard[l - 1]);
/*<         avr=robard(l) >*/
	    avr = robard[l - 1];
/*<         else >*/
	} else {
/*<    >*/
	    enviro_(&edifr, &edifa, &rad, &palt, &xmuv, &fra, &fae, &fr);
/*<           avr=roc*fr+(1.-fr)*roe >*/
	    avr = roc * fr + ((float)1. - fr) * roe;
/*<    >*/
	    rsurf = roc * dtott * exp(-(trayp + taerp) / xmuv) / ((float)1. - 
		    avr * astot) + avr * dtott * (utott - exp(-(trayp + taerp)
		     / xmuv)) / ((float)1. - avr * astot);
/*<         endif >*/
	}
/*<         ratm1=(romix-rorayl)*tgtot+rorayl*tgp1 >*/
	ratm1 = (romix - rorayl) * tgtot + rorayl * tgp1;
/*<         ratm3=romix*tgp1 >*/
	ratm3 = romix * tgp1;
/*<         ratm2=(romix-rorayl)*tgp2+rorayl*tgp1 >*/
	ratm2 = (romix - rorayl) * tgp2 + rorayl * tgp1;
/*<         romeas1=ratm1+rsurf*tgtot >*/
	romeas1 = ratm1 + rsurf * tgtot;
/*<         romeas2=ratm2+rsurf*tgtot >*/
	romeas2 = ratm2 + rsurf * tgtot;
/*<         romeas3=ratm3+rsurf*tgtot >*/
	romeas3 = ratm3 + rsurf * tgtot;
/*    computing integrated values over the spectral band */
/*<         if (iwave.eq.-2) then >*/
	if (iwave == -2) {
/*<    >*/
	    io___373.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___373);
	    do_fio(&c__1, (char *)&wl, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&tgtot, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&dtott, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&utott, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&astot, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&ratm2, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&swl, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&step, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&sbor, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&dsol, (ftnlen)sizeof(real));
	    do_fio(&c__1, (char *)&romeas2, (ftnlen)sizeof(real));
	    e_wsfe();
/*<         endif >*/
	}
/*<         alumeas=xmus*swl*romeas2/pi >*/
	alumeas = xmus * swl * romeas2 / pi;
/*<         fophsa=fophsa+phaa*coef >*/
	fophsa += phaa * coef;
/*<         fophsr=fophsr+phar*coef >*/
	fophsr += phar * coef;
/*<         sasr=sasr+asray*coef >*/
	sasr += asray * coef;
/*<         sasa=sasa+asaer*coef >*/
	sasa += asaer * coef;
/*<         sast=sast+astot*coef >*/
	sast += astot * coef;
/*<         sroray=sroray+rorayl*coef >*/
	sroray += rorayl * coef;
/*<         sroaer=sroaer+roaero*coef >*/
	sroaer += roaero * coef;
/*<         sodray=sodray+tray*coef >*/
	sodray += tray * coef;
/*<         sodaer=sodaer+taer*coef >*/
	sodaer += taer * coef;
/*<         sodrayp=sodrayp+trayp*coef >*/
	sodrayp += trayp * coef;
/*<         sodaerp=sodaerp+taerp*coef >*/
	sodaerp += taerp * coef;
/*<         ssdaer=ssdaer+tsca*coef >*/
	ssdaer += tsca * coef;
/*<         sodtot=sodtot+(taer+tray)*coef >*/
	sodtot += (taer + tray) * coef;
/*<         sodtotp=sodtotp+(taerp+trayp)*coef >*/
	sodtotp += (taerp + trayp) * coef;
/*<         srotot=srotot+(romix)*coef >*/
	srotot += romix * coef;
/*<         rog=rog+roc*coef >*/
	rog += roc * coef;
/*<         refet=refet+romeas2*coef >*/
	refet += romeas2 * coef;
/*<         refet1=refet1+romeas1*coef >*/
	refet1 += romeas1 * coef;
/*<         refet2=refet2+romeas2*coef >*/
	refet2 += romeas2 * coef;
/*<         refet3=refet3+romeas3*coef >*/
	refet3 += romeas3 * coef;
/*<         alumet=alumet+alumeas*sbor*step >*/
	alumet += alumeas * sbor * step;
/*<         tgasm=tgasm+tgtot*coef >*/
	tgasm += tgtot * coef;
/*<         dgasm=dgasm+dgtot*coef >*/
	dgasm += dgtot * coef;
/*<         ugasm=ugasm+ugtot*coef >*/
	ugasm += ugtot * coef;
/*<         sdwava=sdwava+dtwava*coef >*/
	sdwava += dtwava * coef;
/*<         sdozon=sdozon+dtozon*coef >*/
	sdozon += dtozon * coef;
/*<         sddica=sddica+dtdica*coef >*/
	sddica += dtdica * coef;
/*<         sdoxyg=sdoxyg+dtoxyg*coef >*/
	sdoxyg += dtoxyg * coef;
/*<         sdniox=sdniox+dtniox*coef >*/
	sdniox += dtniox * coef;
/*<         sdmeth=sdmeth+dtmeth*coef >*/
	sdmeth += dtmeth * coef;
/*<         sdmoca=sdmoca+dtmoca*coef >*/
	sdmoca += dtmoca * coef;
/*<         suwava=suwava+utwava*coef >*/
	suwava += utwava * coef;
/*<         suozon=suozon+utozon*coef >*/
	suozon += utozon * coef;
/*<         sudica=sudica+utdica*coef >*/
	sudica += utdica * coef;
/*<         suoxyg=suoxyg+utoxyg*coef >*/
	suoxyg += utoxyg * coef;
/*<         suniox=suniox+utniox*coef >*/
	suniox += utniox * coef;
/*<         sumeth=sumeth+utmeth*coef >*/
	sumeth += utmeth * coef;
/*<         sumoca=sumoca+utmoca*coef >*/
	sumoca += utmoca * coef;
/*<         stwava=stwava+ttwava*coef >*/
	stwava += ttwava * coef;
/*<         stozon=stozon+ttozon*coef >*/
	stozon += ttozon * coef;
/*<         stdica=stdica+ttdica*coef >*/
	stdica += ttdica * coef;
/*<         stoxyg=stoxyg+ttoxyg*coef >*/
	stoxyg += ttoxyg * coef;
/*<         stniox=stniox+ttniox*coef >*/
	stniox += ttniox * coef;
/*<         stmeth=stmeth+ttmeth*coef >*/
	stmeth += ttmeth * coef;
/*<         stmoca=stmoca+ttmoca*coef >*/
	stmoca += ttmoca * coef;
/*<         sdtotr=sdtotr+dtotr*coef >*/
	sdtotr += dtotr * coef;
/*<         sdtota=sdtota+dtota*coef >*/
	sdtota += dtota * coef;
/*<         sdtott=sdtott+dtott*coef >*/
	sdtott += dtott * coef;
/*<         sutotr=sutotr+utotr*coef >*/
	sutotr += utotr * coef;
/*<         sutota=sutota+utota*coef >*/
	sutota += utota * coef;
/*<         sutott=sutott+utott*coef >*/
	sutott += utott * coef;
/*<         sb=sb+sbor*step >*/
	sb += sbor * step;
/*<         seb=seb+coef >*/
	seb += coef;
/*    output at the ground level. */
/*<         tdir=exp(-(tray+taer)/xmus) >*/
	tdir = exp(-(tray + taer) / xmus);
/*<         tdif=dtott-tdir >*/
	tdif = dtott - tdir;
/*<         etn=dtott*dgtot/(1.-avr*astot) >*/
	etn = dtott * dgtot / ((float)1. - avr * astot);
/*<         esn=tdir*dgtot >*/
	esn = tdir * dgtot;
/*<         es=tdir*dgtot*xmus*swl >*/
	es = tdir * dgtot * xmus * swl;
/*<         ea0n=tdif*dgtot >*/
	ea0n = tdif * dgtot;
/*<         ea0=tdif*dgtot*xmus*swl >*/
	ea0 = tdif * dgtot * xmus * swl;
/*<         ee0n=dgtot*avr*astot*dtott/(1.-avr*astot) >*/
	ee0n = dgtot * avr * astot * dtott / ((float)1. - avr * astot);
/*<         ee0=xmus*swl*dgtot*avr*astot*dtott/(1.-avr*astot) >*/
	ee0 = xmus * swl * dgtot * avr * astot * dtott / ((float)1. - avr * 
		astot);
/*<         if (etn.gt.accu3) then >*/
	if (etn > accu3) {
/*<            ani(1,1)=esn/etn >*/
	    ani[0] = esn / etn;
/*<            ani(1,2)=ea0n/etn >*/
	    ani[2] = ea0n / etn;
/*<            ani(1,3)=ee0n/etn >*/
	    ani[4] = ee0n / etn;
/*<         else >*/
	} else {
/*<            ani(1,1)=0. >*/
	    ani[0] = (float)0.;
/*<            ani(1,2)=0. >*/
	    ani[2] = (float)0.;
/*<            ani(1,3)=0. >*/
	    ani[4] = (float)0.;
/*<         endif >*/
	}
/*<         ani(2,1)=es >*/
	ani[1] = es;
/*<         ani(2,2)=ea0 >*/
	ani[3] = ea0;
/*<         ani(2,3)=ee0 >*/
	ani[5] = ee0;
/*<         do 955 j=1,3 >*/
	for (j = 1; j <= 3; ++j) {
/*<           aini(1,j)=aini(1,j)+ani(1,j)*coef >*/
	    aini[(j << 1) - 2] += ani[(j << 1) - 2] * coef;
/*<           aini(2,j)=aini(2,j)+ani(2,j)*sbor*step >*/
	    aini[(j << 1) - 1] += ani[(j << 1) - 1] * sbor * step;
/*<   955   continue >*/
/* L955: */
	}
/*    output at satellite level */
/*<         tmdir=exp(-(tray+taer)/xmuv) >*/
	tmdir = exp(-(tray + taer) / xmuv);
/*<         tmdif=utott-tmdir >*/
	tmdif = utott - tmdir;
/*<         xla0n=ratm2 >*/
	xla0n = ratm2;
/*<         xla0=xla0n*xmus*swl/pi >*/
	xla0 = xla0n * xmus * swl / pi;
/*<         xltn=roc*dtott*tmdir*tgtot/(1.-avr*astot) >*/
	xltn = roc * dtott * tmdir * tgtot / ((float)1. - avr * astot);
/*<         xlt=xltn*xmus*swl/pi >*/
	xlt = xltn * xmus * swl / pi;
/*<         xlen=avr*dtott*tmdif*tgtot/(1.-avr*astot) >*/
	xlen = avr * dtott * tmdif * tgtot / ((float)1. - avr * astot);
/*<         xle=xlen*xmus*swl/pi >*/
	xle = xlen * xmus * swl / pi;
/*<         anr(1,1)=xla0n >*/
	anr[0] = xla0n;
/*<         anr(1,2)=xlen >*/
	anr[2] = xlen;
/*<         anr(1,3)=xltn >*/
	anr[4] = xltn;
/*<         anr(2,1)=xla0 >*/
	anr[1] = xla0;
/*<         anr(2,2)=xle >*/
	anr[3] = xle;
/*<         anr(2,3)=xlt >*/
	anr[5] = xlt;
/*<         do 56 j=1,3 >*/
	for (j = 1; j <= 3; ++j) {
/*<           ainr(1,j)=ainr(1,j)+anr(1,j)*coef >*/
	    ainr[(j << 1) - 2] += anr[(j << 1) - 2] * coef;
/*<           ainr(2,j)=ainr(2,j)+anr(2,j)*sbor*step >*/
	    ainr[(j << 1) - 1] += anr[(j << 1) - 1] * sbor * step;
/*<    56   continue >*/
/* L56: */
	}
/*<    51 continue >*/
/* L51: */
    }
/* ---- integrated values of apparent reflectance, radiance          ---- 
*/
/* ---- and gaseous transmittances (total,downward,separately gases) ---- 
*/
/*<       refet=refet/seb >*/
    refet /= seb;
/*<       refet1=refet1/seb >*/
    refet1 /= seb;
/*<       refet2=refet2/seb >*/
    refet2 /= seb;
/*<       refet3=refet3/seb >*/
    refet3 /= seb;
/*<       tgasm=tgasm/seb >*/
    tgasm /= seb;
/*<       dgasm=dgasm/seb >*/
    dgasm /= seb;
/*<       ugasm=ugasm/seb >*/
    ugasm /= seb;
/*<       sasa=sasa/seb >*/
    sasa /= seb;
/*<       sasr=sasr/seb >*/
    sasr /= seb;
/*<       sast=sast/seb >*/
    sast /= seb;
/*<       sdniox=sdniox/seb >*/
    sdniox /= seb;
/*<       sdmoca=sdmoca/seb >*/
    sdmoca /= seb;
/*<       sdmeth=sdmeth/seb >*/
    sdmeth /= seb;
/*<       sdwava=sdwava/seb >*/
    sdwava /= seb;
/*<       sdozon=sdozon/seb >*/
    sdozon /= seb;
/*<       sddica=sddica/seb >*/
    sddica /= seb;
/*<       suniox=suniox/seb >*/
    suniox /= seb;
/*<       sumoca=sumoca/seb >*/
    sumoca /= seb;
/*<       sumeth=sumeth/seb >*/
    sumeth /= seb;
/*<       suwava=suwava/seb >*/
    suwava /= seb;
/*<       suozon=suozon/seb >*/
    suozon /= seb;
/*<       sudica=sudica/seb >*/
    sudica /= seb;
/*<       suoxyg=suoxyg/seb >*/
    suoxyg /= seb;
/*<       sdoxyg=sdoxyg/seb >*/
    sdoxyg /= seb;
/*<       stniox=stniox/seb >*/
    stniox /= seb;
/*<       stmoca=stmoca/seb >*/
    stmoca /= seb;
/*<       stmeth=stmeth/seb >*/
    stmeth /= seb;
/*<       stwava=stwava/seb >*/
    stwava /= seb;
/*<       stozon=stozon/seb >*/
    stozon /= seb;
/*<       stdica=stdica/seb >*/
    stdica /= seb;
/*<       stoxyg=stoxyg/seb >*/
    stoxyg /= seb;
/*<       sdtotr=sdtotr/seb >*/
    sdtotr /= seb;
/*<       sdtota=sdtota/seb >*/
    sdtota /= seb;
/*<       sdtott=sdtott/seb >*/
    sdtott /= seb;
/*<       sutotr=sutotr/seb >*/
    sutotr /= seb;
/*<       sutota=sutota/seb >*/
    sutota /= seb;
/*<       sutott=sutott/seb >*/
    sutott /= seb;
/*<       rog=rog/seb >*/
    rog /= seb;
/*<       sroray=sroray/seb >*/
    sroray /= seb;
/*<       sroaer=sroaer/seb >*/
    sroaer /= seb;
/*<       srotot=srotot/seb >*/
    srotot /= seb;
/*<       alumet=alumet/sb >*/
    alumet /= sb;
/*<       pizera=0.0 >*/
    pizera = (float)0.;
/*<       if(iaer.ne.0) pizera=ssdaer/sodaer >*/
    if (iaer != 0) {
	pizera = ssdaer / sodaer;
    }
/*<       sodray=sodray/seb >*/
    sodray /= seb;
/*<       sodaer=sodaer/seb >*/
    sodaer /= seb;
/*<       sodtot=sodtot/seb >*/
    sodtot /= seb;
/*<       sodrayp=sodrayp/seb >*/
    sodrayp /= seb;
/*<       sodaerp=sodaerp/seb >*/
    sodaerp /= seb;
/*<       sodtotp=sodtotp/seb >*/
    sodtotp /= seb;
/*<       fophsa=fophsa/seb >*/
    fophsa /= seb;
/*<       fophsr=fophsr/seb >*/
    fophsr /= seb;
/*<       do 57 j=1,3 >*/
    for (j = 1; j <= 3; ++j) {
/*<         aini(1,j)=aini(1,j)/seb >*/
	aini[(j << 1) - 2] /= seb;
/*<         ainr(1,j)=ainr(1,j)/seb >*/
	ainr[(j << 1) - 2] /= seb;
/*<         aini(2,j)=aini(2,j)/sb >*/
	aini[(j << 1) - 1] /= sb;
/*<         ainr(2,j)=ainr(2,j)/sb >*/
	ainr[(j << 1) - 1] /= sb;
/*<    57 continue >*/
/* L57: */
    }
/* _otb_adaptation Beginning: Atmospheric reflectance storage */
/*<       otb_ratm   = ainr(1,1) >*/
    *otb_ratm__ = ainr[0];
/*<       otb_sast   = sast >*/
    *otb_sast__ = sast;
/*<       otb_tgasm  = tgasm >*/
    *otb_tgasm__ = tgasm;
/*<       otb_sdtott = sdtott >*/
    *otb_sdtott__ = sdtott;
/*<       otb_sutott = sutott >*/
    *otb_sutott__ = sutott;
/* _otb_adaptation End : Atmospheric reflectance storage */
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                       print of final results                         c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<  160  write(iwr, 430 )refet,alumet,tgasm >*/
/* L160: */
    io___393.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___393);
    do_fio(&c__1, (char *)&refet, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&alumet, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&tgasm, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 431 )refet1,refet2,refet3 >*/
    io___394.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___394);
    do_fio(&c__1, (char *)&refet1, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&refet2, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&refet3, (ftnlen)sizeof(real));
    e_wsfe();
/*<       if(inhomo.ne.0) then >*/
    if (inhomo != 0) {
/*<    >*/
	io___395.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___395);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 2], (ftnlen)sizeof(real));
	}
	do_fio(&c__1, "environment", 11L);
	do_fio(&c__1, "target", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 2], (ftnlen)sizeof(real));
	}
	e_wsfe();
/*<    >*/
	io___396.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___396);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 1], (ftnlen)sizeof(real));
	}
	do_fio(&c__1, "environment", 11L);
	do_fio(&c__1, "target", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
/*<       endif >*/
    }
/*<       if(inhomo.eq.0) then >*/
    if (inhomo == 0) {
/*<    >*/
	io___397.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___397);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 2], (ftnlen)sizeof(real));
	}
	do_fio(&c__1, "background ", 11L);
	do_fio(&c__1, "pixel ", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 2], (ftnlen)sizeof(real));
	}
	e_wsfe();
/*<    >*/
	io___398.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___398);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 1], (ftnlen)sizeof(real));
	}
	do_fio(&c__1, "background ", 11L);
	do_fio(&c__1, "pixel ", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
/*<       endif >*/
    }
/*<       if (iwave.eq.-1)then >*/
    if (iwave == -1) {
/*<         write(iwr, 436)seb >*/
	io___399.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___399);
	do_fio(&c__1, (char *)&seb, (ftnlen)sizeof(real));
	e_wsfe();
/*<       else >*/
    } else {
/*<         write(iwr, 437)sb,seb >*/
	io___400.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___400);
	do_fio(&c__1, (char *)&sb, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&seb, (ftnlen)sizeof(real));
	e_wsfe();
/*<       endif >*/
    }
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                    print of complementary results                    c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       write(iwr, 929) >*/
    io___401.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___401);
    e_wsfe();
/*<       write(iwr, 930) >*/
    io___402.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___402);
    e_wsfe();
/*<       write(iwr, 931)'global gas. trans. :',dgasm,ugasm,tgasm >*/
    io___403.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___403);
    do_fio(&c__1, "global gas. trans. :", 20L);
    do_fio(&c__1, (char *)&dgasm, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&ugasm, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&tgasm, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'water   "     "    :',sdwava,suwava,stwava >*/
    io___404.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___404);
    do_fio(&c__1, "water   \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdwava, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&suwava, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&stwava, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'ozone   "     "    :',sdozon,suozon,stozon >*/
    io___405.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___405);
    do_fio(&c__1, "ozone   \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdozon, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&suozon, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&stozon, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'co2     "     "    :',sddica,sudica,stdica >*/
    io___406.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___406);
    do_fio(&c__1, "co2     \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sddica, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sudica, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&stdica, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'oxyg    "     "    :',sdoxyg,suoxyg,stoxyg >*/
    io___407.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___407);
    do_fio(&c__1, "oxyg    \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdoxyg, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&suoxyg, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&stoxyg, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'no2     "     "    :',sdniox,suniox,stniox >*/
    io___408.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___408);
    do_fio(&c__1, "no2     \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdniox, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&suniox, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&stniox, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'ch4     "     "    :',sdmeth,sumeth,stmeth >*/
    io___409.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___409);
    do_fio(&c__1, "ch4     \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdmeth, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sumeth, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&stmeth, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'co      "     "    :',sdmoca,sumoca,stmoca >*/
    io___410.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___410);
    do_fio(&c__1, "co      \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdmoca, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sumoca, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&stmoca, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___411.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___411);
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___412.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___412);
    e_wsfe();
/*<       write(iwr, 931)'rayl.  sca. trans. :',sdtotr,sutotr,sutotr*sdtotr >*/
    io___413.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___413);
    do_fio(&c__1, "rayl.  sca. trans. :", 20L);
    do_fio(&c__1, (char *)&sdtotr, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sutotr, (ftnlen)sizeof(real));
    r__1 = sutotr * sdtotr;
    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'aeros. sca.   "    :',sdtota,sutota,sutota*sdtota >*/
    io___414.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___414);
    do_fio(&c__1, "aeros. sca.   \"    :", 20L);
    do_fio(&c__1, (char *)&sdtota, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sutota, (ftnlen)sizeof(real));
    r__1 = sutota * sdtota;
    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'total  sca.   "    :',sdtott,sutott,sutott*sdtott >*/
    io___415.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___415);
    do_fio(&c__1, "total  sca.   \"    :", 20L);
    do_fio(&c__1, (char *)&sdtott, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sutott, (ftnlen)sizeof(real));
    r__1 = sutott * sdtott;
    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___416.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___416);
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___417.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___417);
    e_wsfe();
/*<       write(iwr, 939) >*/
    io___418.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___418);
    e_wsfe();
/*<       write(iwr, 931)'spherical albedo   :',sasr,sasa,sast >*/
    io___419.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___419);
    do_fio(&c__1, "spherical albedo   :", 20L);
    do_fio(&c__1, (char *)&sasr, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sasa, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sast, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'optical depth total:',sodray,sodaer,sodtot >*/
    io___420.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___420);
    do_fio(&c__1, "optical depth total:", 20L);
    do_fio(&c__1, (char *)&sodray, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sodaer, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sodtot, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'optical depth plane:',sodrayp,sodaerp,sodtotp >*/
    io___421.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___421);
    do_fio(&c__1, "optical depth plane:", 20L);
    do_fio(&c__1, (char *)&sodrayp, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sodaerp, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sodtotp, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 931)'reflectance        :',sroray,sroaer,srotot >*/
    io___422.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___422);
    do_fio(&c__1, "reflectance        :", 20L);
    do_fio(&c__1, (char *)&sroray, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&sroaer, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&srotot, (ftnlen)sizeof(real));
    e_wsfe();
/*<       fophst=(sodray*fophsr+sodaer*fophsa)/(sodray+sodaer) >*/
    fophst = (sodray * fophsr + sodaer * fophsa) / (sodray + sodaer);
/*<       write(iwr, 931)'phase function     :',fophsr,fophsa,fophst >*/
    io___424.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___424);
    do_fio(&c__1, "phase function     :", 20L);
    do_fio(&c__1, (char *)&fophsr, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&fophsa, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&fophst, (ftnlen)sizeof(real));
    e_wsfe();
/*<       pizerr=1. >*/
    pizerr = (float)1.;
/*<       pizert=(pizerr*sodray+pizera*sodaer)/(sodray+sodaer) >*/
    pizert = (pizerr * sodray + pizera * sodaer) / (sodray + sodaer);
/*<       write(iwr, 931)'sing. scat. albedo :',pizerr,pizera,pizert >*/
    io___427.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___427);
    do_fio(&c__1, "sing. scat. albedo :", 20L);
    do_fio(&c__1, (char *)&pizerr, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&pizera, (ftnlen)sizeof(real));
    do_fio(&c__1, (char *)&pizert, (ftnlen)sizeof(real));
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___428.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___428);
    e_wsfe();
/*<       write(iwr, 1402) >*/
    io___429.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___429);
    e_wsfe();
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                                                                      c 
*/
/*                    atmospheric correction                            c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       if (rapp.ge.-1.) then >*/
    if (rapp >= (float)-1.) {
/*< 	 if (rapp.ge.0.) then >*/
	if (rapp >= (float)0.) {
/*< 	    xrad=rapp >*/
	    xrad = rapp;
/*< 	    rapp=pi*xrad*sb/xmus/seb >*/
	    rapp = pi * xrad * sb / xmus / seb;
/*< 	 else >*/
	} else {
/*< 	    rapp=-rapp >*/
	    rapp = -rapp;
/*< 	    xrad=xmus*seb*(rapp)/pi/sb >*/
	    xrad = xmus * seb * rapp / pi / sb;
/*< 	 endif >*/
	}
/*<          rog=rapp/tgasm >*/
	rog = rapp / tgasm;
/*<          rog=(rog-ainr(1,1)/tgasm)/sutott/sdtott >*/
	rog = (rog - ainr[0] / tgasm) / sutott / sdtott;
/*<          rog=rog/(1.+rog*sast) >*/
	rog /= rog * sast + (float)1.;
/*< 	 xa=pi*sb/xmus/seb/tgasm/sutott/sdtott >*/
	xa = pi * sb / xmus / seb / tgasm / sutott / sdtott;
/*< 	 xb=srotot/sutott/sdtott/tgasm >*/
	xb = srotot / sutott / sdtott / tgasm;
/*< 	 xc=sast >*/
	xc = sast;
/*<          write(iwr, 940) >*/
	io___434.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___434);
	e_wsfe();
/*<          write(iwr, 941)rapp >*/
	io___435.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___435);
	do_fio(&c__1, (char *)&rapp, (ftnlen)sizeof(real));
	e_wsfe();
/*<          write(iwr, 942)xrad >*/
	io___436.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___436);
	do_fio(&c__1, (char *)&xrad, (ftnlen)sizeof(real));
	e_wsfe();
/*<          write(iwr, 943)rog >*/
	io___437.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___437);
	do_fio(&c__1, (char *)&rog, (ftnlen)sizeof(real));
	e_wsfe();
/*<          write(iwr, 944)xa,xb,xc >*/
	io___438.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___438);
	do_fio(&c__1, (char *)&xa, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&xb, (ftnlen)sizeof(real));
	do_fio(&c__1, (char *)&xc, (ftnlen)sizeof(real));
	e_wsfe();
/*<       endif >*/
    }
/* _otb_adaptation Beginning: */
/*<       return	 >*/
    return 0;
/* _otb_adaptation End : */
/*<       stop >*/
    s_stop("", 0L);
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                   output editing formats                             c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<    >*/
/*<    >*/
/*<    >*/
/*<   103 format(1h*,2x,7h month:,i3,7h day : ,i3,t79,1h*) >*/
/*<  1 >*/
/*<  1 >*/
/*<  1 >*/
/*<  1 >*/
/*<  1271 format(1h*,12x,5e11.4,t79,1h*) >*/
/*<  1 >*/
/*<  1 >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<   135 format(1h*,T41,f6.4,T55,f5.3,T69,e8.3,T79,1h*) >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<  1 >*/
/*<    >*/
/*<    >*/
/*<   170 format(1h*,15x,22h target reflectance : ,t79,1h*) >*/
/*<   171 format(1h*,15x,29h environmental reflectance : ,t79,1h*) >*/
/*<   172 format(1h*,t79,1h*,/,79(1h*),///) >*/
/*<    >*/
/*<   174 format(1h*,10x,22h ground pressure  [mb]    ,1x,f7.2,1x,t79,1h*) >*/
/*<   175 format(1h*,10x,22h ground altitude  [km]    ,f6.3,1x,t79,1h*) >*/
/*<    >*/
/*<    >*/
/*<   185 format(1h*,10x,30h input apparent reflectance : , f6.3,t79,1h*) >*/
/*<    >*/
/*<    >*/
/*<   190 format(1h*,15x,31h brdf from in-situ measurements,t79,1h*) >*/
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
/*<   207 format(1h*,15x,A19,t79,1h*) >*/
/*<   208 format(1h*,15x,A19,1x,f5.2,t79,1h*) >*/
/*<   209 format(1h*,15x,A31,t79,1h*) >*/
/*<    >*/
/* pressure at ground level (174) and altitude (175) */
/*<    >*/
/*<   179 format(1h*,10x,31h plane  pressure          [mb] ,f7.2,1x,t79,1h*) >*/
/*<   180 format(1h*,10x,31h plane  altitude absolute [km] ,f6.3,1x,t79,1h*) >*/
/*<   181 format(1h*,15x,37h atmosphere under plane description: ,t79,1h*) >*/
/*<   182 format(1h*,15x,26h ozone content            ,f6.3,1x,t79,1h*) >*/
/*<   183 format(1h*,15x,26h h2o   content            ,f6.3,1x,t79,1h*) >*/
/*<   184 format(1h*,15x,26haerosol opt. thick. 550nm ,f6.3,1x,t79,1h*) >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<   929 format(1h ,////) >*/
/*<    >*/
/*<   931 format(1h*,6x,a20,t32,f7.5,t47,f7.5,t62,f7.5,t79,1h*) >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<  1401 format(1h*,t79,1h*) >*/
/*<  1402 format(1h*,t79,1h*,/,79(1h*)) >*/
/*<  1 >*/
/*<  1501 format(1h*,6(F6.4,1X),F6.1,1X,4(F6.4,1X),t79,1h*) >*/
/*<       end >*/
    return 0;
} /* otb_6s_ssssss_otb_main_function */

#ifdef __cplusplus
	}
#endif
