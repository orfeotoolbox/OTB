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

/*OTB */
/*Extern*/
struct {
    integer nquad;
} num_quad__;

#define num_quad__1 num_quad__

/*OTB */
/*Extern*/
struct {
    integer num_z__;
    doublereal alt_z__[101], taer_z__[101], taer55_z__[101];
} aeroprof_;

#define aeroprof_1 aeroprof_

/*OTB */
/*Extern*/
struct {
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

/*OTB */
/*Extern*/
struct {
    doublereal rmax, rmin;
    integer icp;
    doublereal rn[80]	/* was [20][4] */, ri[80]	/* was [20][4] */, x1[
	    4], x2[4], x3[4], cij[4];
    integer irsunph;
    doublereal rsunph[50], nrsunph[50];
} mie_in__;

#define mie_in__1 mie_in__

/*OTB */
/*Extern*/
struct {
    integer igmax;
} multorder_;

#define multorder_1 multorder_

/*OTB */
/*Extern*/
struct {
    doublereal zpl[34], ppl[34], tpl[34], whpl[34], wopl[34];
} sixs_planesim__;

#define sixs_planesim__1 sixs_planesim__

/*OTB */
/*Extern*/
struct {
    doublereal xacc;
} sixs_test__;

#define sixs_test__1 sixs_test__

/*OTB */
/*Extern*/
struct {
    doublereal s[1501], wlinf, wlsup;
} sixs_ffu__;

#define sixs_ffu__1 sixs_ffu__

/*OTB */
/*Extern*/
struct {
    doublereal delta, sigma;
} sixs_del__;

#define sixs_del__1 sixs_del__

/*OTB */
/*Extern*/
struct {
    doublereal z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

/*OTB */
/*Extern*/
struct {
    doublereal ext[20], ome[20], gasym[20], phase[20], qhase[20], uhase[20];
} sixs_aer__;

#define sixs_aer__1 sixs_aer__

/*OTB */
/*Extern*/
struct {
    doublereal roatm[60]	/* was [3][20] */, dtdir[60]	/* was [3][20]
	     */, dtdif[60]	/* was [3][20] */, utdir[60]	/* was [3][20]
	     */, utdif[60]	/* was [3][20] */, sphal[60]	/* was [3][20]
	     */, wldis[20], trayl[20], traypl[20], rqatm[60]	/* was [3][20]
	     */, ruatm[60]	/* was [3][20] */;
} sixs_disc__;

#define sixs_disc__1 sixs_disc__

/* Table of constant values */

static doublereal c_b4 = -1.;
static doublereal c_b5 = 1.;
static doublereal c_b6 = 0.;
static integer c__3 = 3;
static integer c__1 = 1;
static integer c__5 = 5;
static integer c__2 = 2;
static doublereal c_b308 = 2.;
static integer c__9 = 9;

/*<    >*/
/* Subroutine */ int otb_6s_ssssss_otb_main_function(doublereal *otb_asol__, doublereal *otb_phi0__, 
	doublereal *otb_avis__, doublereal *otb_phiv__, integer *otb_month__, 
	integer *otb_jday__, doublereal *otb_pressure__, doublereal *otb_uw__,
	 doublereal *otb_uo3__, integer *otb_iaer__, doublereal *otb_taer55__,
	 doublereal *otb_wlinf__, doublereal *otb_wlsup__, doublereal *
	otb_s__, doublereal *otb_ratm__, doublereal *otb_sast__, doublereal *
	otb_tgasm__, doublereal *otb_sdtott__, doublereal *otb_sutott__, 
	doublereal *otb_tdif_up__, doublereal *otb_tdir_up__, doublereal *
	otb_tdif_up_ray__, doublereal *otb_tdif_up_aer__)
{
    /* Initialized data */

    static doublereal angmu[10] = { 85.,80.,70.,60.,50.,40.,30.,20.,10.,0. };
    static doublereal angphi[13] = { 0.,30.,60.,90.,120.,150.,180.,210.,240.,
	    270.,300.,330.,360. };
    static doublereal wldisc[20] = { .35,.4,.412,.443,.47,.488,.515,.55,.59,
	    .633,.67,.694,.76,.86,1.24,1.536,1.65,1.95,2.25,3.75 };
    static char etiq1[60*8+1] = "(1h*,22x,34h user defined conditions       \
   ,t79,1h*)     (1h*,22x,24h meteosat observation   ,t79,1h*)              \
 (1h*,22x,25h goes east observation   ,t79,1h*)              (1h*,22x,25h go\
es west observation   ,t79,1h*)              (1h*,22x,30h avhrr (AM noaa) ob\
servation  ,t79,1h*)         (1h*,22x,30h avhrr (PM noaa) observation  ,t79,\
1h*)         (1h*,22x,24h h.r.v.   observation   ,t79,1h*)               (1h\
*,22x,24h t.m.     observation   ,t79,1h*)               ";
    static char nsat[17*119+1] = " constant         user s           meteosa\
t         goes east        goes west        avhrr 1 (noaa6)  avhrr 2 (noaa6)\
  avhrr 1 (noaa7)  avhrr 2 (noaa7)  avhrr 1 (noaa8)  avhrr 2 (noaa8)  avhrr \
1 (noaa9)  avhrr 2 (noaa9)  avhrr 1 (noaa10) avhrr 2 (noaa10) avhrr 1 (noaa1\
1) avhrr 2 (noaa11) hrv1 1           hrv1 2           hrv1 3           hrv1 \
pan         hrv2 1           hrv2 2           hrv2 3           hrv2 pan     \
     tm  1            tm  2            tm  3            tm  4            tm \
 5            tm  7            mss 4            mss 5            mss 6      \
      mss 7            mas 1            mas 2            mas 3            ma\
s 4            mas 5            mas 6            mas 7            modis 1   \
       modis 2          modis 3          modis 4          modis 5          m\
odis 6          modis 7          modis 8         avhrr 1 (noaa12) avhrr 2 (n\
oaa12) avhrr 1 (noaa14) avhrr 2 (noaa14) polder 1         polder 2         p\
older 3         polder 4         polder 5         polder 6         polder 7 \
        polder 8         seawifs 1        seawifs 2        seawifs 3        \
seawifs 4        seawifs 5        seawifs 6        seawifs 7        seawifs \
8        aatsr   1        aatsr   2        aatsr   3        aatsr   4       \
 meris   1        meris   2        meris   3        meris   4        meris  \
 5        meris   6        meris   7        meris   8        meris   9      \
  meris   10       meris   11       meris   12       meris   13       meris \
  14       meris   15       gli     1        gli     2        gli     3     \
   gli     4        gli     5        gli     6        gli     7        gli  \
   8        gli     9        gli     10       gli     11       gli     12   \
    gli     13       gli     14       gli     15       gli     16       gli \
    17       gli     18       gli     19       gli     20       gli     21  \
     gli     22       gli     23       gli     24       gli     25       gli\
     26       gli     27       gli     28       gli     29       gli     30 \
     ";
    static char atmid[51*7+1] = "no absorption computed                     \
        tropical            (uh2o=4.12g/cm2,uo3=.247cm-atm)midlatitude summe\
r  (uh2o=2.93g/cm2,uo3=.319cm-atm)midlatitude winter  (uh2o=.853g/cm2,uo3=.3\
95cm-atm)subarctic  summer   (uh2o=2.10g/cm2,uo3=.480cm-atm)subarctic  winte\
r   (uh2o=.419g/cm2,uo3=.480cm-atm)us  standard 1962   (uh2o=1.42g/cm2,uo3=.\
344cm-atm)";

    /* Format strings */
    static char fmt_98[] = "(/////,\002*\002,30(\002*\002),\002 6sV version \
1.0B \002,30(\002*\002),t79,\002*\002,/,\002*\002,t79,\002*\002,/,\002*\002,\
22x,\002 geometrical conditions identity  \002,t79,\002*\002,/,\002*\002,22x,\
\002 -------------------------------  \002,t79,\002*\002)";
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
    static char fmt_5550[] = "(\002*\002,10x,\002 aerosols type identity \
:\002,t79,\002*\002)";
    static char fmt_5554[] = "(\002*\002,15x,\002no aerosols computed\002,t7\
9,\002*\002)";
    static char fmt_5551[] = "(\002*\002,11x,\002  user-defined aerosol prof\
ile:\002,i2,\002 layers\002,t79,\002*\002)";
    static char fmt_5552[] = "(\002*\002,13x,\002 Layer   Height(km)   Opt. \
thick.(at 0.55 mkm)\002,3x,\002  Model\002,t79,\002*\002)";
    static char fmt_5553[] = "(\002*\002,15x,i2,1x,f10.1,13x,f5.3,15x,a15,t7\
9,\002*\002)";
    static char fmt_132[] = "(\002*\002,15x,a30,t79,\002*\002)";
    static char fmt_133[] = "(\002*\002,13x,\002user-defined aerosol model:\
 \002,t79,\002*\002,/,\002*\002,26x,f6.3,\002 % of dust-like\002,t79,\002\
*\002,/,\002*\002,26x,f6.3,\002 % of water-soluble\002,t79,\002*\002,/,\002\
*\002,26x,f6.3,\002 % of oceanic\002,t79,\002*\002,/,\002*\002,26x,f6.3,\002\
 % of soot\002,t79,\002*\002)";
    static char fmt_134[] = "(\002*\002,13x,\002user-defined aerosol model:\
 \002,i2,\002 Log-Normal size distribution(s)\002,t79,\002*\002,/,\002*\002,\
15x,\002Mean radius   Stand. Dev.  Percent. density\002,t79,\002*\002)";
    static char fmt_135[] = "(\002*\002,t19,f6.4,t33,f5.3,t47,e8.3,t79,\002\
*\002)";
    static char fmt_136[] = "(\002*\002,13x,\002user-defined aerosol model\
:\002,\002 modified Gamma size distribution\002,t79,\002*\002,/,\002*\002,19\
x,\002Alpha: \002,f6.3,\002   b: \002,f6.3,\002   Gamma: \002,f6.3,t79,\002\
*\002)";
    static char fmt_137[] = "(\002*\002,13x,\002user-defined aerosol model\
:\002,\002 Junge Power-Law size distribution\002,t79,\002*\002,/,\002*\002,1\
9x,\002Alpha: \002,f6.3,t79,\002*\002)";
    static char fmt_139[] = "(\002*\002,15x,\002 results saved into the fi\
le:\002,t79,\002*\002,/,\002*\002,20x,a30,t79,\002*\002)";
    static char fmt_138[] = "(\002*\002,13x,\002user-defined aerosol model u\
sing data from\002,\002 the file:\002,t79,\002*\002,/,\002*\002,20x,a30,t79\
,\002*\002)";
    static char fmt_140[] = "(\002*\002,10x,\002 optical condition identit\
y :\002,t79,\002*\002,/,\002*\002,15x,\002 user def. opt. thick. at 550 nm \
:\002,f7.4,t79,\002*\002,/,\002*\002,t79,\002*\002)";
    static char fmt_141[] = "(\002*\002,10x,\002 optical condition identit\
y :\002,t79,\002*\002,/,\002*\002,14x,\002 visibility :\002,f6.2,\002 km \
\002,\002 opt. thick. 550 nm : \002,f7.4,t79,\002*\002)";
    static char fmt_5555[] = "(\002*\002,t79,\002*\002)";
    static char fmt_148[] = "(\002*\002,22x,\002 spectral condition  \002,t7\
9,\002*\002,/,\002*\002,22x,\002 ------------------  \002,t79,\002*\002)";
    static char fmt_1510[] = "(\002*\002,10x,a17,t79,\002*\002,/,\002*\002,1\
5x,\002value of filter function :\002,t79,\002*\002,/,\002*\002,15x,\002 wl \
inf=\002,f6.3,\002 mic\002,2x,\002 wl sup=\002,f6.3,\002 mic\002,t79,\002\
*\002)";
    static char fmt_149[] = "(\002*\002,11x,\002 monochromatic calculation a\
t wl\002:,f6.3,\002 micron \002,t79,\002*\002)";
    static char fmt_142[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002 S\
urface polarization parameters    \002,t79,\002*\002,/,\002*\002,22x,\002 --\
-------------------------------- \002,t79,\002*\002,/,\002*\002,t79,\002*\
\002)";
    static char fmt_146[] = "(\002*\002,t79,\002*\002,/,\002*\002,\002  User\
's input roQ and roU          \002,2(f8.3,1x),t79,\002*\002,/,\002*\002)";
    static char fmt_144[] = "(\002*\002,t79,\002*\002,/,\002*\002,\002 Nadal\
 and Breon with %  vegetation \002,1(f8.2,1x),t79,\002*\002,/,\002*\002)";
    static char fmt_145[] = "(\002*\002,t79,\002*\002,/,\002*\002,\002  Sung\
lint Model  windspeed,azimuth \002,2(f8.3,1x),t79,\002*\002,/,\002*\002)";
    static char fmt_143[] = "(\002*\002,t79,\002*\002,/,\002*\002,\002 Surfa\
ce Polarization Q,U,Rop,Chi   \002,3(f8.5,1x),f8.2,1x,t79,\002*\002,/,\002\
*\002,t79,\002*\002)";
    static char fmt_169[] = "(\002*\002,t79,\002*\002,/,\002*\002,22x,\002 t\
arget type  \002,t79,\002*\002,/,\002*\002,22x,\002 -----------  \002,t79\
,\002*\002,/,\002*\002,10x,\002 inhomogeneous ground , radius of target \002\
,f6.3,\002 km  \002,t79,\002*\002)";
    static char fmt_170[] = "(\002*\002,15x,\002 target reflectance : \002,t\
79,\002*\002)";
    static char fmt_171[] = "(\002*\002,15x,\002 environmental reflectance\
 : \002,t79,\002*\002)";
    static char fmt_1500[] = "(\002*\002,1x,\002wave   total  total  total  \
total  atm.   \002,\002swl    step   sbor   dsol   toar \002,t79,\002*\002,/,\
\002*\002,1x,\002       gas    scat   scat   spheri intr   \002,t79,\002*\
\002,/,\002*\002,1x,\002       trans  down   up     albedo refl   \002,t79\
,\002*\002)";
    static char fmt_2222[] = "(a28,3(f10.7,1x))";
    static char fmt_2223[] = "(a24,1x,a80)";
    static char fmt_333[] = "(f10.5,1x,f10.5,1x,i3,f10.5,f10.5)";
    static char fmt_430[] = "(79(\002*\002),/,\002*\002,t79,\002*\002,/\
,\002*\002,24x,\002 integrated values of  :   \002,t79,\002*\002,/,\002*\002\
,24x,\002 --------------------      \002,t79,\002*\002,/,\002*\002,t79,\002\
*\002,/,\002*\002,6x,\002 apparent reflectance \002,f10.7,1x,\002 appar. rad\
.(w/m2/sr/mic) \002,f8.3,1x,t79,\002*\002,/,\002*\002,18x,\002 total gaseous\
 transmittance  \002,f5.3,t79,\002*\002,/,\002*\002,t79,\002*\002,/,79(\002\
*\002))";
    static char fmt_431[] = "(\002*\002,t79,\002*\002,/,\002*\002,24x,\002 c\
oupling aerosol -wv  :   \002,t79,\002*\002,/,\002*\002,24x,\002 -----------\
---------      \002,t79,\002*\002,/,\002*\002,10x,\002 wv above aerosol :\
 \002,f7.3,4x,\002 wv mixed with aerosol : \002,f7.3,1x,t79,\002*\002,/,\002*\
\002,22x,\002 wv under aerosol : \002,f7.3,t79,\002*\002)";
    static char fmt_429[] = "(79(\002*\002),/,\002*\002,t79,\002*\002,/\
,\002*\002,24x,\002 integrated values of  :   \002,t79,\002*\002,/,\002*\002\
,24x,\002 --------------------      \002,t79,\002*\002,/,\002*\002,t79,\002\
*\002,/,\002*\002,6x,\002 app. polarized refl. \002,f7.4,3x,\002 app. pol. r\
ad. (w/m2/sr/mic) \002,f8.3,1x,t79,\002*\002,/,\002*\002,12x,\002 direction \
of the plane of polarization\002,f6.2,t79,\002*\002,/,\002*\002,18x,\002 tot\
al polarization ratio     \002,f5.3,t79,\002*\002,/,\002*\002,t79,\002*\002,\
/,79(\002*\002))";
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
    static char fmt_931[] = "(\002*\002,6x,a20,t32,f8.5,t47,f8.5,t62,f8.5,t7\
9,\002*\002)";
    static char fmt_939[] = "(\002*\002,t79,\002*\002,/,\002*\002,t30,\002 r\
ayleigh \002,t45,\002 aerosols \002,t60,\002   total  \002,t79,\002*\002,/\
,\002*\002,t79,\002*\002)";
    static char fmt_932[] = "(\002*\002,6x,a20,t32,f8.2,t47,f8.2,t62,f8.2,t7\
9,\002*\002)";
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
    static char fmt_222[] = "(\002*\002,6x,\002 atmospherically corrected re\
flectance  \002,t79,\002*\002,/,\002*\002,6x,\002 Lambertian case :  \002,1x\
,f10.5,t79,\002*\002,/,\002*\002,6x,\002 BRDF       case :  \002,1x,f10.5,t7\
9,\002*\002)";

    /* System generated locals */
    address a__1[2];
    integer i__1, i__2[2], i__3;
    doublereal d__1, d__2, d__3;
    cilist ci__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    /* Subroutine */ /*int s_copy(char *, char *, ftnlen, ftnlen);*/
    double acos(doublereal), cos(doublereal);
    /* Subroutine */ int s_stop(char *, ftnlen);
    double sqrt(doublereal);
    integer s_rsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_rsle(), s_rsfe(cilist *), do_fio(integer *, char *, ftnlen), 
	    e_rsfe(), i_indx(char *, char *, ftnlen, ftnlen);
    /* Subroutine */ /*int s_cat(char *, char **, integer *, integer *, ftnlen);*/
    double exp(doublereal), log(doublereal);
    integer s_wsle(cilist *), e_wsle(), s_wsfe(cilist *), e_wsfe();
    double atan2(doublereal, doublereal), pow_dd(doublereal *, doublereal *);
    integer f_open(olist *), f_clos(cllist *);
    double atan(doublereal);

    /* Local variables */
    extern /* Subroutine */ int aeroso_(integer *, doublereal *, doublereal *,
	     doublereal *, char *, integer *, ftnlen), equivwl_(integer *, 
	    integer *, doublereal *, doublereal *), discom_(integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), odrayl_(doublereal *, doublereal *), polnad_(
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), polglit_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), solirr_(doublereal *, doublereal *), abstra_(
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, integer *, doublereal *
	    , doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), interp_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *, integer *, doublereal *, doublereal *, doublereal *
	    , doublereal *, doublereal *, doublereal *, integer *), enviro_(
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    doublereal rqatm2, ruatm2, tdirqu, rqmeas2, rumeas2, qlumeas, ulumeas, 
	    xtphi, refet_fi__[181], roatm_fi__[
	    10860]	/* was [3][20][181] */, height_z__[101],  
	    rfoamave, brdfints[2499]	/* was [51][49] */, romix_fi__[181], 
	    rglitave;
	doublereal qlumet = 0;
	doublereal phi_wind__ = 0;
	doublereal rqfet = 0;
    doublereal rufet = 0;
	doublereal ulumet = 0;
    extern /* Subroutine */ int dirpopol_(doublereal *, doublereal *, 
	    doublereal *), pressure_(doublereal *, doublereal *, doublereal *)
	    ;
    doublereal c__[4];
    integer i__, j, k, l, n;
    doublereal v, y;
    char aer_model__[50*15];
    integer iaer_prof__, i1, i2;
    doublereal rorayl_fi__[181];
    extern /* Subroutine */ int presplane_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    doublereal robarstar, gb[51];
    integer ik;
    doublereal sb, gp[49], fr, pi, es, xa, xb, xc;
    integer np;
    doublereal rm[51], ro, rp[49];
	doublereal wl=0;
    integer nt, mu;
    doublereal tu, uw, ea0, ee0, elsesdpaer, pi2;
	doublereal ul=0;
    integer mu2;
    doublereal uo3;
    extern /* Subroutine */ int specinterp_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    doublereal robarpstar, fae, cfi, ani[6]	/* was [2][3] */, rad, seb, 
	    fra, sha, tdd, anr[6]	/* was [2][3] */, phi, roc, roe, etn, 
	    rog, avr, dtr, esn, xle, tdu, tsd, its, swl, xpp, xlt, xps, puw, 
	    tsu, azw, pps, ea0n, ee0n;
    doublereal pws=0;
    integer nfi;
    extern /* Subroutine */ int us62_();
    doublereal phi0;
    integer ifi;
    doublereal xla0, xap, tgp1, tgp2;
    integer mum1;
    doublereal xlm1[2499]	/* was [51][49] */, xlm2[2499]	/* was [51][
	    49] */, puo3, adif, scaa, phaa, qhaa, coef, uhaa, aini[6]	/* 
	    was [2][3] */;
    integer iaer;
    char file[80];
    integer iinf;
    doublereal tdif, sasa, sham, ainr[6]	/* was [2][3] */, phar, taer, 
	    tsca, asol, rocl[1501], roel[1501], avis, dsol, phiv, rapp, xlat, 
	    sbor, step, sasr, xmud, sast, tray, xlon, qhar, uhar, tdir, xltn, 
	    xlen, xrad, xmup;
    integer ipol, jday;
    doublereal xmus, accu2, accu3, xmuv;
    integer isup, ilut, irop;
    doublereal uwus, ropq, ropu, pveg, wspd, razw, total_height__;
    char file2[80];
    doublereal palt, xla0n, ratm1, ratm2, ratm3, robarbarstar, puoz, xpol, 
	    edifa, cscaa, coefa, uo3us, coefb, coefc;
    integer iread, aerod;
	integer ibrdf=0;
    doublereal dgasm, asaer, robar[1501], taer55, refet, tgasm, filut[1025]	
	    /* was [25][41] */, rpfet, ugasm, tamoy, taerp, romix, trayp, 
	    dtott, rqmix, rumix, rolut[1025]	/* was [25][41] */, astot, 
	    wlmoy, asray, trmoy, utotr, luttv, utott, utota, dtotr, dtota, 
	    puwus, dgtot, tgtot, robar1, robar2, refet1, refet2, refet3, 
	    coefp, ugtot, edifr, rpfet1, rpfet2, rpfet3, tdird, tdiru, tdifd, 
	    tdifu, rsurf, tmdir, tmdif;
    integer igeom, month, idatm, iaerp, iwave;
	integer igrou1 = 0;
    doublereal xnorm1, xnorm2;
    integer igrou2 = 0;
    doublereal sddica, dtdica;
    integer isort, irapp;
    doublereal iscama, puo3us;
    integer nbisca, idirec;
    doublereal rwatl[1501], anglem[48], iscami, phirad, sdmoca, robard[1501], 
	    sudica, stdica, dtmoca, rocave, sodaer, roeave, fophsa, ssdaer, 
	    sdmeth, robarp[1501], taer55p, alumet, sdwava, sumoca, stmoca, 
	    sroaer, sdtota;
    integer nfilut[25];
    doublereal sodray, sdniox, sumeth, plumet, suwava, stwava, tamoyp, stmeth,
	     sdozon, sdoxyg, roluti[1025]	/* was [25][41] */, sodtot, 
	    fophsr, sroray, sdtotr, sdtott, stniox, suniox, rolutq[1025]	
	    /* was [25][41] */, pizmoy, roluts[20500]	/* was [20][25][41] */
	    , rolutu[1025]	/* was [25][41] */, trmoyp, lutmuv, suozon, 
	    suoxyg, stozon, stoxyg, srotot, sutotr, sutota, sutott, dtozon, 
	    dtoxyg, dtniox, dtmeth, utozon, utdica, utoxyg, utniox, utmeth, 
	    utmoca, ttozon, ttdica, ttoxyg, ttniox, ttmeth, ttmoca, dtwava, 
	    utwava, ttwava, rorayl, roaero, rqrayl, albbrdf, rqaero, foqhsr, 
	    foqhsa, foqhst, rurayl, ruaero, srpray, srpaer, srptot, srqray, 
	    srqaer, srqtot, sruray, sruaer, srutot, fouhst, fouhsr, fouhsa, 
	    romeas1, romeas2, romeas3, alumeas, sdpray, sdpaer, sdptot, 
	    sdppaer, spdpaer, sodaerp, pizera, fophst, weightm[48], pizerr, 
	    attwava, pizert;
    integer idatmp, inhomo, igroun;
    doublereal discri, rogbrdf, rfoaml[1501], sodrayp, sdppray, spdpray, 
	    rglitl[1501], cij_out__[4], sodtotp, sdpptot, spdptot, rolutiq[
	    1025]	/* was [25][41] */, ftray, ratm2_fi__[181], rolutiu[
	    1025]	/* was [25][41] */, pizmoyp, rolutsq[20500]	/* 
	    was [20][25][41] */, rolutsu[20500]	/* was [20][25][41] */, 
	    rwatave;
    extern /* Subroutine */ int gauss_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, integer *), varsol_(integer *, integer *, 
	    doublereal *);

    /* Fortran I/O blocks */
    static cilist io___60 = { 0, 5, 0, 0, 0 };
    static cilist io___61 = { 0, 5, 0, 0, 0 };
    static cilist io___62 = { 0, 0, 0, 0, 0 };
    static cilist io___65 = { 0, 0, 0, 0, 0 };
    static cilist io___66 = { 0, 5, 0, 0, 0 };
    static cilist io___67 = { 0, 5, 0, 0, 0 };
    static cilist io___68 = { 0, 5, 0, 0, 0 };
    static cilist io___70 = { 0, 0, 0, 0, 0 };
    static cilist io___71 = { 0, 0, 0, 0, 0 };
    static cilist io___72 = { 0, 5, 0, 0, 0 };
    static cilist io___73 = { 0, 5, 0, 0, 0 };
    static cilist io___74 = { 0, 0, 0, 0, 0 };
    static cilist io___75 = { 0, 0, 0, 0, 0 };
    static cilist io___76 = { 0, 5, 0, 0, 0 };
    static cilist io___77 = { 0, 5, 0, 0, 0 };
    static cilist io___78 = { 0, 5, 0, 0, 0 };
    static cilist io___79 = { 0, 5, 0, 0, 0 };
    static cilist io___80 = { 0, 5, 0, 0, 0 };
    static cilist io___81 = { 0, 5, 0, 0, 0 };
    static cilist io___83 = { 0, 5, 0, 0, 0 };
    static cilist io___98 = { 0, 0, 0, 0, 0 };
    static cilist io___102 = { 0, 0, 0, 0, 0 };
    static cilist io___123 = { 0, 6, 0, 0, 0 };
    static cilist io___150 = { 0, 0, 0, 0, 0 };
    static cilist io___153 = { 0, 0, 0, 0, 0 };
    static cilist io___155 = { 0, 0, 0, 0, 0 };
    static cilist io___163 = { 0, 0, 0, fmt_98, 0 };
    static cilist io___164 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___165 = { 0, 0, 0, fmt_103, 0 };
    static cilist io___166 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___170 = { 0, 0, 0, fmt_102, 0 };
    static cilist io___171 = { 0, 0, 0, fmt_1110, 0 };
    static cilist io___172 = { 0, 0, 0, fmt_1119, 0 };
    static cilist io___173 = { 0, 0, 0, fmt_1281, 0 };
    static cilist io___174 = { 0, 0, 0, fmt_1272, 0 };
    static cilist io___175 = { 0, 0, 0, fmt_1271, 0 };
    static cilist io___176 = { 0, 0, 0, fmt_1261, 0 };
    static cilist io___177 = { 0, 0, 0, fmt_5550, 0 };
    static cilist io___178 = { 0, 0, 0, fmt_5554, 0 };
    static cilist io___180 = { 0, 6, 0, fmt_5551, 0 };
    static cilist io___181 = { 0, 6, 0, fmt_5552, 0 };
    static cilist io___182 = { 0, 6, 0, fmt_5553, 0 };
    static cilist io___183 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___184 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___185 = { 0, 0, 0, fmt_132, 0 };
    static cilist io___186 = { 0, 0, 0, fmt_133, 0 };
    static cilist io___187 = { 0, 6, 0, fmt_134, 0 };
    static cilist io___188 = { 0, 0, 0, fmt_135, 0 };
    static cilist io___189 = { 0, 0, 0, fmt_136, 0 };
    static cilist io___190 = { 0, 0, 0, fmt_137, 0 };
    static cilist io___191 = { 0, 0, 0, fmt_139, 0 };
    static cilist io___192 = { 0, 0, 0, fmt_138, 0 };
    static cilist io___193 = { 0, 0, 0, fmt_140, 0 };
    static cilist io___194 = { 0, 0, 0, fmt_141, 0 };
    static cilist io___195 = { 0, 6, 0, fmt_5555, 0 };
    static cilist io___196 = { 0, 0, 0, fmt_148, 0 };
    static cilist io___197 = { 0, 0, 0, fmt_1510, 0 };
    static cilist io___198 = { 0, 0, 0, fmt_149, 0 };
    static cilist io___199 = { 0, 0, 0, fmt_1510, 0 };
    static cilist io___200 = { 0, 0, 0, fmt_142, 0 };
    static cilist io___201 = { 0, 0, 0, fmt_146, 0 };
    static cilist io___202 = { 0, 0, 0, fmt_144, 0 };
    static cilist io___203 = { 0, 0, 0, fmt_145, 0 };
    static cilist io___204 = { 0, 0, 0, fmt_143, 0 };
    static cilist io___211 = { 0, 0, 0, fmt_169, 0 };
    static cilist io___213 = { 0, 0, 0, fmt_170, 0 };
    static cilist io___215 = { 0, 0, 0, fmt_171, 0 };
    static cilist io___296 = { 0, 0, 0, fmt_1500, 0 };
    static cilist io___426 = { 0, 6, 0, 0, 0 };
    static cilist io___427 = { 0, 10, 0, fmt_2222, 0 };
    static cilist io___428 = { 0, 10, 0, fmt_2222, 0 };
    static cilist io___430 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___431 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___432 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___433 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___434 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___435 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___436 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___437 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___438 = { 0, 10, 0, fmt_333, 0 };
    static cilist io___439 = { 0, 10, 0, fmt_333, 0 };
    static cilist io___440 = { 0, 10, 0, fmt_2222, 0 };
    static cilist io___441 = { 0, 10, 0, fmt_2222, 0 };
    static cilist io___442 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___443 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___444 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___445 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___446 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___447 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___448 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___449 = { 0, 10, 0, fmt_2223, 0 };
    static cilist io___451 = { 0, 0, 0, fmt_430, 0 };
    static cilist io___452 = { 0, 0, 0, fmt_431, 0 };
    static cilist io___454 = { 0, 0, 0, fmt_429, 0 };
    static cilist io___455 = { 0, 0, 0, fmt_432, 0 };
    static cilist io___456 = { 0, 0, 0, fmt_434, 0 };
    static cilist io___457 = { 0, 0, 0, fmt_432, 0 };
    static cilist io___458 = { 0, 0, 0, fmt_434, 0 };
    static cilist io___459 = { 0, 0, 0, fmt_436, 0 };
    static cilist io___460 = { 0, 0, 0, fmt_437, 0 };
    static cilist io___461 = { 0, 0, 0, fmt_929, 0 };
    static cilist io___462 = { 0, 0, 0, fmt_930, 0 };
    static cilist io___463 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___464 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___465 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___466 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___467 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___468 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___469 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___470 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___471 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___472 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___473 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___474 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___475 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___476 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___477 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___478 = { 0, 0, 0, fmt_939, 0 };
    static cilist io___479 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___480 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___481 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___482 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___483 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___484 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___485 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___486 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___487 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___488 = { 0, 0, 0, fmt_932, 0 };
    static cilist io___489 = { 0, 0, 0, fmt_932, 0 };
    static cilist io___490 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___491 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___492 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___493 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___494 = { 0, 0, 0, fmt_931, 0 };
    static cilist io___495 = { 0, 0, 0, fmt_1401, 0 };
    static cilist io___496 = { 0, 0, 0, fmt_1402, 0 };
    static cilist io___520 = { 0, 0, 0, fmt_940, 0 };
    static cilist io___521 = { 0, 0, 0, fmt_941, 0 };
    static cilist io___522 = { 0, 0, 0, fmt_942, 0 };
    static cilist io___523 = { 0, 0, 0, fmt_943, 0 };
    static cilist io___524 = { 0, 0, 0, fmt_944, 0 };
    static cilist io___525 = { 0, 0, 0, fmt_222, 0 };
    static cilist io___526 = { 0, 0, 0, fmt_944, 0 };


/* _otb MOD V2 : New outputs : otb_tdif_up =  upward diffuse transmittance
 */
/* _otb MOD V2 : New outputs : otb_tdir_up =  upward direct transmittance 
*/
/*_otb MOD V2 : New outputs : otb_tdif_up_ray = upward dif. trans for rayl
eigh*/
/*_otb MOD V2 : New outputs : otb_tdif_up_aer = upward dif. transm. for ae
rosols*/
/* _otb MOD V2 : */
/* _otb MOD V2 : Normalization of US62 pressure profile */
/* _otb MOD V2 : by the user defined pressure */
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
/*       *           ... (6sV) ....... (6sV) ...... (6sV) ...   *       c 
*/
/*       *                        version  1.0B                 *       c 
*/
/*       *                                                      *       c 
*/
/*       *                        Vector Code                   *       c 
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
/*       *             The authors of this code are             *       c 
*/
/*       *                                                      *       c 
*/
/*       *            (1) Vermote E and Kotchenova.S.;          *       c 
*/
/*       *            (2) Roger J.C..;                          *       c 
*/
/*       *            (3) Tanre D.; Deuze J.L; Herman M.;       *       c 
*/
/*       *            (4) Morcrette J.J..                       *       c 
*/
/*       *                                                      *       c 
*/
/*       *                       from                           *       c 
*/
/*       *                                                      *       c 
*/
/*       *     (1) Affiliation: Department of Geography         *       c 
*/
/*       *         University of Maryland and                   *       c 
*/
/*       *         Goddard Space Flight Center	               *       c */
/*       *         Code 614.5    		      	       *       c */
/*       *         USA                                          *       c 
*/
/*       *                                                      *       c 
*/
/*       *     (2) ELICO/LOCL                                   *       c 
*/
/*       *         Universite du Littoral Cote d'Opale          *       c 
*/
/*       *         32 Ave Foch, 62930 Wimereux                  *       c 
*/
/*       *         France                                       *       c 
*/
/*       *                                                      *       c 
*/
/*       *     (3) laboratoire d' optique atmospherique         *       c 
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
/*       *     (4) e.c.m.w.f.                                   *       c 
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
/*<       parameter(nt_p=30,mu_p=25,mu2_p=48,np_p=49,nfi_p=181,nquad_p=83)	! >*/
/*<       parameter (nt_p_max=100,nqmax_p=1000,nqdef_p=83) ! do not change	! >*/
/* Attention				!Added for OTB */
/* mu2_p has to be equal to (mu_p-1)*2	!Added for OTB */
/* _otb      include "paramdef.inc" */
/*<    >*/
/*<       dimension  xlmus(-mu_p:mu_p,np_p),xlmuv(-mu_p:mu_p,np_p) >*/
/*<    >*/
/*<    >*/
/*<         real rolut(mu_p,41),roluts(20,mu_p,41),roluti(mu_p,41) >*/
/*<         real rolutq(mu_p,41),rolutsq(20,mu_p,41),rolutiq(mu_p,41) >*/
/*<         real rolutu(mu_p,41),rolutsu(20,mu_p,41),rolutiu(mu_p,41) >*/
/*< 	real filut(mu_p,41) >*/
/*< 	integer aerod >*/
/*< 	real its,lutmuv,luttv,iscama,iscami,scaa,cscaa,cfi >*/
/*< 	integer nfilut(mu_p),nbisca >*/
/*< 	real dtr  >*/
/*<         real anglem,weightm,rm,gb,accu2,accu3 >*/
/*<         real rp,gp,xlmus,xlmuv,angmu,angphi,brdfints,brdfdats >*/
/*<         real brdfintv,brdfdatv,robar,robarp,robard,xlm1,xlm2 >*/
/*<         real c,wldisc,ani,anr,aini,ainr,rocl,roel,zpl,ppl,tpl,whpl >*/
/*<         real wopl,xacc,s,wlinf,wlsup,delta >*/
/*< 	real nwlinf,nwlsup >*/
/*< 	integer niinf,nisup >*/
/*<         real sigma,z,p,t,wh,wo,ext,ome,gasym,phase,qhase,roatm,dtdir >*/
/*<         real dtdif,utdir,utdif,sphal,wldis,trayl,traypl,pi,pi2,step >*/
/*<         real asol,phi0,avis,phiv,tu,xlon,xlat,xlonan,hna,dsol,campm >*/
/*<         real phi,phirad,xmus,xmuv,xmup,xmud,adif,uw,uo3,taer55 >*/
/*<         real taer,v,xps,uwus,uo3us,xpp,taer55p,puw,puo3,puwus >*/
/*<         real puo3us,wl,wlmoy,tamoy,tamoyp,pizmoy,pizmoyp,trmoy >*/
/*<         real trmoyp,fr,rad,spalt,sha,sham,uhase >*/
/*<         real albbrdf,par1,par2,par3,par4,robar1,xnorm1,rob,xnor,rodir >*/
/*<         real rdown,rdir,robar2,xnorm2,ro,roc,roe,rapp,rocave,roeave >*/
/*<         real seb,sbor,swl,sb,refet,refet1,refet2,refet3,alumet >*/
/*< 	real refeti,pinst,ksiinst,ksirad >*/
/*<         real rpfet,rpfet1,rpfet2,rpfet3,plumet,plumeas >*/
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
/*< 	real rqmix,rqrayl,rqaero,qhaa,qhar,foqhsr,foqhsa,foqhst >*/
/*< 	real rumix,rurayl,ruaero,uhaa,uhar,rpmix,rpaero,rprayl >*/
/*< 	real srpray,srpaer,srptot,rpmeas1,rpmeas2,rpmeas3 >*/
/*< 	real srqray,srqaer,srqtot,sruray,sruaer,srutot >*/
/*<         real astot,asray,asaer,utotr,utota,dtotr,dtota,dgtot,tgtot >*/
/*<         real tgp1,tgp2,rqatm,ruatm,fouhst,fouhsr,fouhsa,coefp >*/
/*<         real ugtot,edifr,edifa,tdird,tdiru,tdifd,tdifu,fra >*/
/*<         real fae,avr,romeas1,romeas2,romeas3,alumeas,sodrayp >*/
/*<         real sdppray,sdppaer,sdpptot,rop,sdpray,sdpaer,sdptot >*/
/*< 	real spdpray,spdpaer,spdptot >*/
/*<         real ratm1,ratm2,ratm3,rsurf,rpatm1,rpatm2,rpatm3,rpsurf >*/
/*<         real sodaerp,sodtotp,tdir,tdif,etn,esn,es,ea0n,ea0,ee0n >*/
/*<         real ee0,tmdir,tmdif,xla0n,xla0,xltn,xlt,xlen,xle,pizera >*/
/*<         real fophst,pizerr,pizert,xrad,xa,xb,xc >*/
/*<         integer nt,mu,mu2,np,k,iwr,mum1,idatmp,ipol >*/
/*<         integer j,iread,l,igeom,month,jday,nc,nl,idatm,iaer,iaerp,n >*/
/*<         integer iwave,iinf,isup,ik,i,inhomo,idirec,ibrdf,igroun >*/
/*<         integer igrou1,igrou2,isort,irapp,ilut >*/
/* variables used in the BRDF coupling correction process */
/*< 	real robarstar,robarpstar,robarbarstar,tdd,tdu,tsd,tsu >*/
/*< 	real coefa,coefb,coefc,discri,rogbrdf	 >*/
/* variables related to surface polarization */
/*<         integer irop >*/
/*< 	real ropq,ropu,pveg,wspd,azw,razw >*/
/* ***********************************************************************
 */
/*                 to vary the number of quadratures */
/* ***********************************************************************
 */
/*<       integer nquad >*/
/*<       common /num_quad/ nquad  >*/
/* ***********************************************************************
 */
/*                     the aerosol profile */
/* ***********************************************************************
 */
/*<       integer iaer_prof,num_z >*/
/*<       real alt_z,taer_z,taer55_z,total_height,height_z(0:nt_p_max) >*/
/*<    >*/
/*<       character aer_model(15)*50 >*/
/* _otb_adaptation Beginning: otb variables declaration */
/* Added_for_OTB */
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
/*< 	real otb_sutott			!Upward transmittance (output)	 >*/
/*< 	real otb_tdif_up		!Upward diffuse transmittance (output) >*/
/*< 	real otb_tdir_up		!Upward direct transmittance (output) >*/
/*< 	real otb_tdif_up_ray 		!Upward diffuse transmittance for rayleigh (output) >*/
/*< 	real otb_tdif_up_aer 		!Upward diffuse transmittance for aerosols (output)	 >*/
/* _otb_adaptation End : otb variables declaration */
/* ***********************************************************************
 */
/*                             return to 6s */
/* ***********************************************************************
 */
/*<       dimension c(4),wldisc(20),ani(2,3),anr(2,3),aini(2,3),ainr(2,3) >*/
/*<       dimension rocl(1501),roel(1501) >*/
/*<       real rfoaml(1501),rglitl(1501),rwatl(1501) >*/
/*<       real rn,ri,x1,x2,x3,cij,rsunph,nrsunph,rmax,rmin,cij_out(4) >*/
/*<       integer icp,irsunph,i1,i2 >*/
/*<       character etiq1(8)*60,nsat(119)*17,atmid(7)*51,reflec(8)*71 >*/
/*<       character FILE*80,FILE2*80 >*/
/*<       logical ier >*/
/*<       integer igmax >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<    >*/
/*<       common /multorder/ igmax >*/
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
/*     for considering aerosol and brdf */
/* ***********************************************************************
 */
/*<       integer options(5) >*/
/*<       integer pild,pihs >*/
/*<       real optics(3),struct(4) >*/
/*<       real pxLt,pc,pRl,pTl,pRs >*/
/*<       real pws,phi_wind,xsal,pcl,paw,rfoam,rwat,rglit >*/
/*<       real rfoamave,rwatave,rglitave >*/
/*<       real uli,eei,thmi,sli,cabi,cwi,vaii,rnci,rsl1i >*/
/*<       real p1,p2,p3 >*/
/* ***********************************************************************
 */
/*                             return to 6s */
/* ***********************************************************************
 */
/*<       common /sixs_ffu/s(1501),wlinf,wlsup >*/
/*<       common /sixs_del/ delta,sigma >*/
/*<       common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34) >*/
/*<    >*/
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
    nt = 30;
/*<       mu=mu_p >*/
    mu = 25;
/*<       mu2=mu2_p >*/
    mu2 = 48;
/*<       np=np_p >*/
    np = 49;
/*<       nfi=nfi_p >*/
    nfi = 181;
/*<       iwr=6 >*/
    sixs_ier__1.iwr = 6;
/*<       ier=.FALSE. >*/
    sixs_ier__1.ier = FALSE_;
/*<       iinf=1 >*/
    iinf = 1;
/*<       isup=1501 >*/
    isup = 1501;
/*<       igmax=20 >*/
    multorder_1.igmax = 20;
/* ***********************************************************************
 */
/*  preliminary computations for gauss integration */
/* ***********************************************************************
 */
/*<       pi=acos(-1.) >*/
    pi = acos(-1.);
/*<       pi2=2*pi >*/
    pi2 = pi * 2;
/*<       accu2=1.E-03 >*/
    accu2 = .001;
/*<       accu3=1.E-07 >*/
    accu3 = 1e-7;
/*<       do k=1,13 >*/
    for (k = 1; k <= 13; ++k) {
/*<        angphi(k)=angphi(k)*pi/180. >*/
	angphi[k - 1] = angphi[k - 1] * pi / 180.;
/*<       enddo >*/
    }
/*<       do k=1,10 >*/
    for (k = 1; k <= 10; ++k) {
/*<        angmu(k)=cos(angmu(k)*pi/180.) >*/
	angmu[k - 1] = cos(angmu[k - 1] * pi / 180.);
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
    gb[-mu + 25] = 0.;
/*<       gb(0)=0. >*/
    gb[25] = 0.;
/*<       gb(mu)=0. >*/
    gb[mu + 25] = 0.;
/* ***********************************************************************
 */
/*                             return to 6s */
/* ***********************************************************************
 */
/* constantes values */
/*<       sigma=0.056032 >*/
    sixs_del__1.sigma = .056032;
/*<       delta=0.0279 >*/
    sixs_del__1.delta = .0279;
/* CC     pinst=0.02 */
/* CC     ksiinst=0. */
/*<       xacc=1.e-06 >*/
    sixs_test__1.xacc = 1e-6;
/*<       iread=5 >*/
    iread = 5;
/*<       step=0.0025 >*/
    step = .0025;
/*<       do 1111 l=1,20 >*/
    for (l = 1; l <= 20; ++l) {
/*<        wldis(l)=wldisc(l) >*/
	sixs_disc__1.wldis[l - 1] = wldisc[l - 1];
/*<  1111 continue >*/
/* L1111: */
    }
/* **********************************************************************c
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
/*<       igeom = 0			!Added_for_OTB >*/
    igeom = 0;
/*<       asol  = otb_asol		!Added_for_OTB >*/
    asol = *otb_asol__;
/*<       phi0  = otb_phi0		!Added_for_OTB >*/
    phi0 = *otb_phi0__;
/*<       avis  = otb_avis		!Added_for_OTB >*/
    avis = *otb_avis__;
/*<       phiv  = otb_phiv		!Added_for_OTB >*/
    phiv = *otb_phiv__;
/*<       month = otb_month		!Added_for_OTB >*/
    month = *otb_month__;
/*<       jday  = otb_jday		!Added_for_OTB >*/
    jday = *otb_jday__;
/*<       if (igeom.lt.0) then >*/
    if (igeom < 0) {
/*<           if (igeom.lt.-10) then >*/
	if (igeom < -10) {
/*< 	     igmax=int(abs(igeom/10)) >*/
	    multorder_1.igmax = (i__1 = igeom / 10, abs(i__1));
/*< 	     igeom=igeom+igmax*10 >*/
	    igeom += multorder_1.igmax * 10;
/*< 	  endif    >*/
	}
/*<           ilut=0 >*/
	ilut = 0;
/*< 	  igeom=0 >*/
	igeom = 0;
/*<       endif >*/
    }
/*<       ilut=0	   >*/
    ilut = 0;
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
    dsol = 1.;
/*<       call varsol(jday,month,dsol) >*/
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
    phi = (d__1 = phiv - phi0, abs(d__1));
/*<       phirad=(phi0-phiv)*pi/180. >*/
    phirad = (phi0 - phiv) * pi / 180.;
/*<       if (phirad.lt.0.) phirad=phirad+2.*pi >*/
    if (phirad < 0.) {
	phirad += pi * 2.;
    }
/*<       if (phirad.gt.(2.*pi)) phirad=phirad-2.*pi >*/
    if (phirad > pi * 2.) {
	phirad -= pi * 2.;
    }
/*<       xmus=cos(asol*pi/180.) >*/
    xmus = cos(asol * pi / 180.);
/*<       xmuv=cos(avis*pi/180.) >*/
    xmuv = cos(avis * pi / 180.);
/*<       xmup=cos(phirad) >*/
    xmup = cos(phirad);
/*<       xmud=-xmus*xmuv-sqrt(1.-xmus*xmus)*sqrt(1.-xmuv*xmuv)*xmup >*/
    xmud = -xmus * xmuv - sqrt(1. - xmus * xmus) * sqrt(1. - xmuv * xmuv) * 
	    xmup;
/* test vermote bug */
/*<       if (xmud.gt.1.) xmud=1. >*/
    if (xmud > 1.) {
	xmud = 1.;
    }
/*<       if (xmud.lt.-1.) xmud=-1. >*/
    if (xmud < -1.) {
	xmud = -1.;
    }
/*<       adif=acos(xmud)*180./pi >*/
    adif = acos(xmud) * 180. / pi;
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
/* _otb_adaptation Beginning: idatm=8, use of atmospheric model US62, */
/* _otb_adaptation              and real ozone and vater vaper amounts */
/* _otb      uw=0. */
/* _otb      uo3=0. */
/* _otb      read(iread,*) idatm */
/*<       idatm=8		!Added_for_OTB >*/
    idatm = 8;
/*<       call us62		!Added_for_OTB >*/
    us62_();
/*<       uw = otb_uw	!Added_for_OTB >*/
    uw = *otb_uw__;
/*<       uo3 = otb_uo3	!Added_for_OTB >*/
    uo3 = *otb_uo3__;
/* Normalization of the US62 pressure profile accouting for */
/* the ground pressure entered by user. */

/* Normalization of gas amounts is performed in subroutine ABSTRA. */
/*<       do 123 i=1,34        		!Added_for_OTB >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<          p(i)=p(i)*otb_pressure/p(1) 	!Added_for_OTB         >*/
	sixs_atm__1.p[i__ - 1] = sixs_atm__1.p[i__ - 1] * *otb_pressure__ / 
		sixs_atm__1.p[0];
/*< 123   continue				!Added_for_OTB >*/
/* L123: */
    }
/* _otb      if(idatm.eq.0) go to 5 */
/* _otb      if(idatm.eq.8) read(iread,*) uw,uo3 */
/* _otb      if(idatm.ne.7) go to 6 */
/* _otb      do 7 k=1,34 */
/* _otb       read(iread,*) z(k),p(k),t(k),wh(k),wo(k) */
/* _otb    7 continue */
/* _otb      go to 5 */
/* _otb    6 if(idatm.eq.1)  call tropic */
/* _otb      if(idatm.eq.2)  call midsum */
/* _otb      if(idatm.eq.3)  call midwin */
/* _otb      if(idatm.eq.4)  call subsum */
/* _otb      if(idatm.eq.5)  call subwin */
/* _otb      if(idatm.eq.6)  call us62 */
/*     we have to define an atmosphere to compute rayleigh optical depth 
*/
/* _otb    5 if(idatm.eq.0.or.idatm.eq.8)  call us62 */
/*_otb_adaptation End: 	idatm=8, use of atmospheric model US62,real amount
s*/
/* **********************************************************************c
 */
/*      THIS OPTION IS NOT AVAILABLE THE CODE RUNS WITH IPOL=1          c 
*/
/*       ipol       computation of the atmospheric polarization         c 
*/
/*                  -------------------------------------------         c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*      read(iread,*) ipol */
/*<        ipol=1 >*/
    ipol = 1;
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*       iaer       aerosol model(type) and profile                     c 
*/
/*                  --------------                                      c 
*/
/*      iaer = -1  The user-defined profile. You have to input the      c 
*/
/*                 number of layers first, then the height (km),        c 
*/
/*                 optical thickness (at 550 nm), and the type of       c 
*/
/*                 aerosol (see below) for each layer, starting from    c 
*/
/*                 the ground. The present version of the program       c 
*/
/*                 works only with the same type of aerosol for each    c 
*/
/*                 layer.                                               c 
*/
/*                                                                      c 
*/
/*                 Example for iaer = -1:                               c 
*/
/*                 4                                                    c 
*/
/*                 2.0 0.200 1                                          c 
*/
/*                 10.0 0.025 1                                         c 
*/
/*        12  Reading of data previously saved into FILE                c 
*/
/*             you have to enter the identification name FILE in the    c 
*/
/*             next line of inputs.                                     c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/*  iaerp and FILE  aerosol model(type)-Printing of results             c 
*/
/*                  ---------------------------------------             c 
*/
/*                                                                      c 
*/
/* For iaer=8,9,10,and 11:                                              c 
*/
/*    results from the MIE subroutine may be saved into the file        c 
*/
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
    mie_in__1.rmin = 0.;
/*<       rmax=0. >*/
    mie_in__1.rmax = 0.;
/*<       icp=1 >*/
    mie_in__1.icp = 1;
/*<       do i=1,4 >*/
    for (i__ = 1; i__ <= 4; ++i__) {
/*<        x1(i)=0.0 >*/
	mie_in__1.x1[i__ - 1] = 0.;
/*<        x2(i)=0.0 >*/
	mie_in__1.x2[i__ - 1] = 0.;
/*<        x3(i)=0.0 >*/
	mie_in__1.x3[i__ - 1] = 0.;
/*<        do l=1,20 >*/
	for (l = 1; l <= 20; ++l) {
/*<         rn(l,i)=0.0 >*/
	    mie_in__1.rn[l + i__ * 20 - 21] = 0.;
/*<         ri(l,i)=0.0 >*/
	    mie_in__1.ri[l + i__ * 20 - 21] = 0.;
/*<        enddo >*/
	}
/*<       enddo >*/
    }
/*<       do i=1,50 >*/
    for (i__ = 1; i__ <= 50; ++i__) {
/*<        rsunph(i)=0. >*/
	mie_in__1.rsunph[i__ - 1] = 0.;
/*<        nrsunph(i)=0. >*/
	mie_in__1.nrsunph[i__ - 1] = 0.;
/*<       enddo >*/
    }
/*<       cij(1)=1.00 >*/
    mie_in__1.cij[0] = 1.;
/*<        taer=0.		 >*/
    taer = 0.;
/*<        taer55=0.     >*/
    taer55 = 0.;
/*<        iaer_prof=0	 >*/
    iaer_prof__ = 0;
/* _otb_adaptation Beginning: iaer becomes input arguments */
/* _otb      read(iread,*) read(iread,*) iaer */
/*<        iaer = otb_iaer     >*/
    iaer = *otb_iaer__;
/* _obt   --> expected values are 0,1,2,3 or 5 */
/* _otb_adaptation End :  iaer becomes input argument */
/*  the user-defined aerosol profile */
/*<       if (iaer.lt.0) then >*/
    if (iaer < 0) {
/*<       total_height=0.0 >*/
	total_height__ = 0.;
/*<       iaer_prof=1 >*/
	iaer_prof__ = 1;
/*<       num_z=0 >*/
	aeroprof_1.num_z__ = 0;
/*<       do i=0,50 >*/
	for (i__ = 0; i__ <= 50; ++i__) {
/*<       alt_z(i)=0.0 >*/
	    aeroprof_1.alt_z__[i__] = 0.;
/*<       taer55_z(i)=0.0 >*/
	    aeroprof_1.taer55_z__[i__] = 0.;
/*<       taer_z(i)=0.0 >*/
	    aeroprof_1.taer_z__[i__] = 0.;
/*<       height_z(i)=0.0 >*/
	    height_z__[i__] = 0.;
/*<       enddo >*/
	}
/*<       read(5,*) num_z >*/
	s_rsle(&io___60);
	do_lio(&c__3, &c__1, (char *)&aeroprof_1.num_z__, (ftnlen)sizeof(
		integer));
	e_rsle();
/*<       do i=0,num_z-1 >*/
	i__1 = aeroprof_1.num_z__ - 1;
	for (i__ = 0; i__ <= i__1; ++i__) {
/*<        read(5,*) height_z(num_z-i),taer55_z(num_z-i),iaer >*/
	    s_rsle(&io___61);
	    do_lio(&c__5, &c__1, (char *)&height_z__[aeroprof_1.num_z__ - i__]
		    , (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&aeroprof_1.taer55_z__[
		    aeroprof_1.num_z__ - i__], (ftnlen)sizeof(doublereal));
	    do_lio(&c__3, &c__1, (char *)&iaer, (ftnlen)sizeof(integer));
	    e_rsle();
/*<        alt_z(num_z-1-i)=total_height+height_z(num_z-i) >*/
	    aeroprof_1.alt_z__[aeroprof_1.num_z__ - 1 - i__] = total_height__ 
		    + height_z__[aeroprof_1.num_z__ - i__];
/*<        total_height=total_height+height_z(num_z-i) >*/
	    total_height__ += height_z__[aeroprof_1.num_z__ - i__];
/*<        taer55=taer55+taer55_z(num_z-i) >*/
	    taer55 += aeroprof_1.taer55_z__[aeroprof_1.num_z__ - i__];
/*<       enddo >*/
	}
/*<       endif >*/
    }
/*  the user-defined aerosol profile */
/*<       if (iaer.ge.0.and.iaer.le.7) nquad=nqdef_p >*/
    if (iaer >= 0 && iaer <= 7) {
	num_quad__1.nquad = 83;
    }
/*<       if (iaer.ge.8.and.iaer.le.11) nquad=nquad_p >*/
    if (iaer >= 8 && iaer <= 11) {
	num_quad__1.nquad = 83;
    }
/*<       if(iaer.eq.4) read(iread,*) (c(n),n=1,4) >*/
    if (iaer == 4) {
	io___62.ciunit = iread;
	s_rsle(&io___62);
	for (n = 1; n <= 4; ++n) {
	    do_lio(&c__5, &c__1, (char *)&c__[n - 1], (ftnlen)sizeof(
		    doublereal));
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
    c__[0] = .7;
/*<       c(2)=0.29 >*/
    c__[1] = .29;
/*<       c(3)=0.00 >*/
    c__[2] = 0.;
/*<       c(4)=0.01  >*/
    c__[3] = .01;
/*<       go to 49 >*/
    goto L49;
/*<    41 c(1)=0.00 >*/
L41:
    c__[0] = 0.;
/*<       c(2)=0.05 >*/
    c__[1] = .05;
/*<       c(3)=0.95 >*/
    c__[2] = .95;
/*<       c(4)=0.00      >*/
    c__[3] = 0.;
/*<       go to 49 >*/
    goto L49;
/*<    42 c(1)=0.17 >*/
L42:
    c__[0] = .17;
/*<       c(2)=0.61 >*/
    c__[1] = .61;
/*<       c(3)=0.00 >*/
    c__[2] = 0.;
/*<       c(4)=0.22 >*/
    c__[3] = .22;
/*<       go to 49 >*/
    goto L49;
/*<    43 read(iread,*) rmin,rmax,icp >*/
L43:
    io___65.ciunit = iread;
    s_rsle(&io___65);
    do_lio(&c__5, &c__1, (char *)&mie_in__1.rmin, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&mie_in__1.rmax, (ftnlen)sizeof(doublereal));
    do_lio(&c__3, &c__1, (char *)&mie_in__1.icp, (ftnlen)sizeof(integer));
    e_rsle();
/*<       do i=1,icp >*/
    i__1 = mie_in__1.icp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<        read(5,*)x1(i),x2(i),cij(i) >*/
	s_rsle(&io___66);
	do_lio(&c__5, &c__1, (char *)&mie_in__1.x1[i__ - 1], (ftnlen)sizeof(
		doublereal));
	do_lio(&c__5, &c__1, (char *)&mie_in__1.x2[i__ - 1], (ftnlen)sizeof(
		doublereal));
	do_lio(&c__5, &c__1, (char *)&mie_in__1.cij[i__ - 1], (ftnlen)sizeof(
		doublereal));
	e_rsle();
/*<        read(5,*)(rn(l,i),l=1,20) >*/
	s_rsle(&io___67);
	for (l = 1; l <= 20; ++l) {
	    do_lio(&c__5, &c__1, (char *)&mie_in__1.rn[l + i__ * 20 - 21], (
		    ftnlen)sizeof(doublereal));
	}
	e_rsle();
/*<        read(5,*)(ri(l,i),l=1,20) >*/
	s_rsle(&io___68);
	for (l = 1; l <= 20; ++l) {
	    do_lio(&c__5, &c__1, (char *)&mie_in__1.ri[l + i__ * 20 - 21], (
		    ftnlen)sizeof(doublereal));
	}
	e_rsle();
/*<       enddo >*/
    }
/*<         do i=1,icp >*/
    i__1 = mie_in__1.icp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          cij_out(i)=cij(i) >*/
	cij_out__[i__ - 1] = mie_in__1.cij[i__ - 1];
/*<         enddo >*/
    }
/*<       go to 49 >*/
    goto L49;
/*<    44 read(iread,*) rmin,rmax >*/
L44:
    io___70.ciunit = iread;
    s_rsle(&io___70);
    do_lio(&c__5, &c__1, (char *)&mie_in__1.rmin, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&mie_in__1.rmax, (ftnlen)sizeof(doublereal));
    e_rsle();
/*<       read(iread,*) x1(1),x2(1),x3(1) >*/
    io___71.ciunit = iread;
    s_rsle(&io___71);
    do_lio(&c__5, &c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(doublereal))
	    ;
    do_lio(&c__5, &c__1, (char *)&mie_in__1.x2[0], (ftnlen)sizeof(doublereal))
	    ;
    do_lio(&c__5, &c__1, (char *)&mie_in__1.x3[0], (ftnlen)sizeof(doublereal))
	    ;
    e_rsle();
/*<       read(5,*)(rn(l,1),l=1,20) >*/
    s_rsle(&io___72);
    for (l = 1; l <= 20; ++l) {
	do_lio(&c__5, &c__1, (char *)&mie_in__1.rn[l - 1], (ftnlen)sizeof(
		doublereal));
    }
    e_rsle();
/*<       read(5,*)(ri(l,1),l=1,20) >*/
    s_rsle(&io___73);
    for (l = 1; l <= 20; ++l) {
	do_lio(&c__5, &c__1, (char *)&mie_in__1.ri[l - 1], (ftnlen)sizeof(
		doublereal));
    }
    e_rsle();
/*<       go to 49 >*/
    goto L49;
/*<    45 read(iread,*) rmin,rmax >*/
L45:
    io___74.ciunit = iread;
    s_rsle(&io___74);
    do_lio(&c__5, &c__1, (char *)&mie_in__1.rmin, (ftnlen)sizeof(doublereal));
    do_lio(&c__5, &c__1, (char *)&mie_in__1.rmax, (ftnlen)sizeof(doublereal));
    e_rsle();
/*<       read(iread,*) x1(1) >*/
    io___75.ciunit = iread;
    s_rsle(&io___75);
    do_lio(&c__5, &c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(doublereal))
	    ;
    e_rsle();
/*<       read(5,*)(rn(l,1),l=1,20) >*/
    s_rsle(&io___76);
    for (l = 1; l <= 20; ++l) {
	do_lio(&c__5, &c__1, (char *)&mie_in__1.rn[l - 1], (ftnlen)sizeof(
		doublereal));
    }
    e_rsle();
/*<       read(5,*)(ri(l,1),l=1,20) >*/
    s_rsle(&io___77);
    for (l = 1; l <= 20; ++l) {
	do_lio(&c__5, &c__1, (char *)&mie_in__1.ri[l - 1], (ftnlen)sizeof(
		doublereal));
    }
    e_rsle();
/*<       go to 49 >*/
    goto L49;
/*<    46 read(5,*)irsunph >*/
L46:
    s_rsle(&io___78);
    do_lio(&c__3, &c__1, (char *)&mie_in__1.irsunph, (ftnlen)sizeof(integer));
    e_rsle();
/*<       do i=1,irsunph >*/
    i__1 = mie_in__1.irsunph;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<        read(5,*)rsunph(i),nrsunph(i) >*/
	s_rsle(&io___79);
	do_lio(&c__5, &c__1, (char *)&mie_in__1.rsunph[i__ - 1], (ftnlen)
		sizeof(doublereal));
	do_lio(&c__5, &c__1, (char *)&mie_in__1.nrsunph[i__ - 1], (ftnlen)
		sizeof(doublereal));
	e_rsle();
/*       nrsunph(i)=nrsunph(i)/(rsunph(i)**4.)/(4*3.1415/3) */
/*<       enddo >*/
    }
/*<       rmin=rsunph(1) >*/
    mie_in__1.rmin = mie_in__1.rsunph[0];
/*<       rmax=rsunph(irsunph)+1e-07 >*/
    mie_in__1.rmax = mie_in__1.rsunph[mie_in__1.irsunph - 1] + 1e-7;
/*<       read(5,*)(rn(l,1),l=1,20) >*/
    s_rsle(&io___80);
    for (l = 1; l <= 20; ++l) {
	do_lio(&c__5, &c__1, (char *)&mie_in__1.rn[l - 1], (ftnlen)sizeof(
		doublereal));
    }
    e_rsle();
/*<       read(5,*)(ri(l,1),l=1,20) >*/
    s_rsle(&io___81);
    for (l = 1; l <= 20; ++l) {
	do_lio(&c__5, &c__1, (char *)&mie_in__1.ri[l - 1], (ftnlen)sizeof(
		doublereal));
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
	s_rsle(&io___83);
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
/*<       call aeroso(iaer,c,xmud,wldis,FILE2,ipol) >*/
    aeroso_(&iaer, c__, &xmud, sixs_disc__1.wldis, file2, &ipol, 80L);
/* **********************************************************************c
 */
/*                 aerosol model (concentration)                        c 
*/
/*                 ----------------------------                         c 
*/
/*             (only for the default exponential profile)               c 
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
/* _otb_adaptation Beginning: v=0 and taer55 become argument values */
/* _otb      if (iaer_prof.eq.0) then */
/* _otb      read(iread,*) v */
/* _otb      if(v) 71,10,11 */
/* _otb   10 read(iread,*) taer55 */
/* _otb      v=exp(-log(taer55/2.7628)/0.79902) */
/* _otb      goto 71 */
/* _otb   11 call oda550(iaer,v,taer55) */
/* _otb   71 continue */
/* _otb      endif */
/*<            v=0.				!Added_for_OTB >*/
    v = 0.;
/*<            taer55 = otb_taer55		!Added_for_OTB    >*/
    taer55 = *otb_taer55__;
/* _otb_adaptation End :  v=0 and taer55 become  argument values */
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
/* _otb 771   read(iread,*) xps */
/*< 	xps=0.		!Added_for_OTB	 >*/
    xps = 0.;
/* _otb_adaptation End : xps=0 Target at sea level */
/*<        if (xps.ge.0.) then >*/
    if (xps >= 0.) {
/*<         xps=0. >*/
	xps = 0.;
/*<         uwus=1.424 >*/
	uwus = 1.424;
/*<         uo3us=0.344 >*/
	uo3us = .344;
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
/*                  in kilometers units      			       c */
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
/*< 	xpp=-1000.		!Added_for_OTB	 >*/
    xpp = -1e3;
/* _otb_adaptation End : xpp=-1000 sensor aboard a satellite */
/*<         xpp=-xpp >*/
    xpp = -xpp;
/*<         if (xpp.le.0.0) then >*/
    if (xpp <= 0.) {
/*          ground measurement option */
/*<            palt=0. >*/
	palt = 0.;
/*<            pps=p(1) >*/
	pps = sixs_atm__1.p[0];
/*< 	   idatmp=0 >*/
	idatmp = 0;
/*< 	   taer55p=0. >*/
	taer55p = 0.;
/*< 	   puw=0. >*/
	puw = 0.;
/*< 	   puoz=0. >*/
	puoz = 0.;
/*<            else >*/
    } else {
/*< 	   if (xpp.ge.100.) then >*/
	if (xpp >= 100.) {
/* 	       satellite case of equivalent */
/*< 	      palt=1000. >*/
	    palt = 1e3;
/*< 	      pps=0. >*/
	    pps = 0.;
/*< 	      taer55p=taer55 >*/
	    taer55p = taer55;
/*< 	      ftray=1. >*/
	    ftray = 1.;
/*< 	      idatmp=4 >*/
	    idatmp = 4;
/*< 	      else >*/
	} else {
/* 	      "real" plane case */
/*<               read(iread,*) puw,puo3 >*/
	    io___98.ciunit = iread;
	    s_rsle(&io___98);
	    do_lio(&c__5, &c__1, (char *)&puw, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&puo3, (ftnlen)sizeof(doublereal));
	    e_rsle();
/*< 	      if (puw.lt.0.) then >*/
	    if (puw < 0.) {
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
/*< 	      else >*/
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
	    io___102.ciunit = iread;
	    s_rsle(&io___102);
	    do_lio(&c__5, &c__1, (char *)&taer55p, (ftnlen)sizeof(doublereal))
		    ;
	    e_rsle();
/*< 	    if ((taer55p.lt.0.).or.((taer55-taer55p).lt.accu2)) then >*/
	    if (taer55p < 0. || taer55 - taer55p < accu2) {
/* a scale heigh of 2km is assumed in case no value is given f
or taer55p */
/*<                taer55p=taer55*(1.-exp(-palt/2.)) >*/
		taer55p = taer55 * (1. - exp(-palt / 2.));
/*<             else >*/
	    } else {
/* compute effective scale heigh */
/*<                sham=exp(-palt/4.) >*/
		sham = exp(-palt / 4.);
/*<                sha=1.-(taer55p/taer55) >*/
		sha = 1. - taer55p / taer55;
/*<                if (sha.ge.sham) then >*/
		if (sha >= sham) {
/*<                   taer55p=taer55*(1.-exp(-palt/4.)) >*/
		    taer55p = taer55 * (1. - exp(-palt / 4.));
/*<                else >*/
		} else {
/*<                   sha=-palt/log(sha) >*/
		    sha = -palt / log(sha);
/*<                   taer55p=taer55*(1.-exp(-palt/sha)) >*/
		    taer55p = taer55 * (1. - exp(-palt / sha));
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
/*        49  MODIS   band 8           ( 0.4025-0.4225)                 c 
*/
/*        50  1st band of avhrr(noaa12 ( 0.500-1.000 )                  c 
*/
/*        51  2nd      "               ( 0.650-1.120 )                  c 
*/
/*        52  1st band of avhrr(noaa14 ( 0.500-1.110 )                  c 
*/
/*        53  2nd      "               ( 0.680-1.100 )                  c 
*/
/*        54  POLDER  band 1           ( 0.4125-0.4775)                 c 
*/
/*        55  POLDER  band 2 (non polar( 0.4100-0.5225)                 c 
*/
/*        56  POLDER  band 3 (non polar( 0.5325-0.5950)                 c 
*/
/*        57  POLDER  band 4   P1      ( 0.6300-0.7025)                 c 
*/
/*        58  POLDER  band 5 (non polar( 0.7450-0.7800)                 c 
*/
/*        59  POLDER  band 6 (non polar( 0.7000-0.8300)                 c 
*/
/*        60  POLDER  band 7   P1      ( 0.8100-0.9200)                 c 
*/
/*        61  POLDER  band 8 (non polar( 0.8650-0.9400)                 c 
*/
/*        62  SEAWIFS band 1           ( 0.3825-0.70)                   c 
*/
/*        63  SEAWIFS band 2           ( 0.3800-0.58)                   c 
*/
/*        64  SEAWIFS band 3           ( 0.3800-1.02)                   c 
*/
/*        65  SEAWIFS band 4           ( 0.3800-1.02)                   c 
*/
/*        66  SEAWIFS band 5           ( 0.3825-1.15)                   c 
*/
/*        67  SEAWIFS band 6           ( 0.3825-1.05)                   c 
*/
/*        68  SEAWIFS band 7           ( 0.3800-1.15)                   c 
*/
/*        69  SEAWIFS band 8           ( 0.3800-1.15)                   c 
*/
/*        70  AATSR   band 1           ( 0.5250-0.5925)                 c 
*/
/*        71  AATSR   band 2           ( 0.6275-0.6975)                 c 
*/
/*        72  AATSR   band 3           ( 0.8325-0.9025)                 c 
*/
/*        73  AATSR   band 4           ( 1.4475-1.7775)                 c 
*/
/*        74  MERIS   band 1           ( 0.412)                         c 
*/
/*        75  MERIS   band 2           ( 0.442)                         c 
*/
/*        76  MERIS   band 3           ( 0.489)                         c 
*/
/*        77  MERIS   band 4           ( 0.509)                         c 
*/
/*        78  MERIS   band 5           ( 0.559)                         c 
*/
/*        79  MERIS   band 6           ( 0.619)                         c 
*/
/*        93  GLI     band 5           (0.460-1km)                      c 
*/
/*        94  GLI     band 6           (0.490-1km)                      c 
*/
/*        95  GLI     band 7           (0.520-1km)                      c 
*/
/*        96  GLI     band 8           (0.545-1km)                      c 
*/
/*        97  GLI     band 9           (0.565-1km)                      c 
*/
/*        98  GLI     band 10          (0.625-1km)                      c 
*/
/*        99  GLI     band 11          (0.666-1km)                      c 
*/
/*       100  GLI     band 12          (0.680-1km)                      c 
*/
/*       101  GLI     band 13          (0.678-1km)                      c 
*/
/*       102  GLI     band 14          (0.710-1km)                      c 
*/
/*       103  GLI     band 15          (0.710-1km)       (bis?)         c 
*/
/*       104  GLI     band 16          (0.749-1km)                      c 
*/
/*       105  GLI     band 17          (0.763-1km)                      c 
*/
/*       106  GLI     band 18          (0.865-1km)                      c 
*/
/*       107  GLI     band 19          (0.865-1km)       (bis?)         c 
*/
/*       108  GLI     band 20          (0.460-0.25km)                   c 
*/
/*       109  GLI     band 21          (0.545-0.25km)                   c 
*/
/*       110  GLI     band 22          (0.660-0.25km)                   c 
*/
/*       111  GLI     band 23          (0.825-0.25km)                   c 
*/
/*       112  GLI     band 24          (1.050-1km)                      c 
*/
/*       113  GLI     band 25          (1.135-1km)                      c 
*/
/*       114  GLI     band 26          (1.240-1km)                      c 
*/
/*       115  GLI     band 27          (1.338-1km)                      c 
*/
/*       116  GLI     band 28          (1.640-1km)                      c 
*/
/*       117  GLI     band 29          (2.210-1km)                      c 
*/
/*       118  GLI     band 30          (3.715-1km)                      c 
*/
/*  note: wl has to be in micrometer                                    c 
*/
/* **********************************************************************c
 */
/*_otb_adaptation Beginning: iwave=1 and wlinf, wlsup, s(l) become input a
rguments*/
/* _otb      do 38 l=iinf,isup */
/* _otb       s(l)=1. */
/* _otb   38 continue */
/* _otb      read(iread,*) iwave */
/* _otb      if (iwave.eq.-2) goto 1600 */
/* _otb      if (iwave) 16,17,18 */
/* _otb   16 read(iread,*) wl */
/* _otb      wlinf=wl */
/* _otb      wlsup=wl */
/* _otb      go to 19 */
/* _otb   17 read(iread,*) wlinf,wlsup */
/* _otb      go to 19 */
/* _otb 1600 read(iread,*) wlinf,wlsup */
/* _otb      go to 19 */
/*       110 */
/*       111     band of meteosat        (2) */
/*       112     band of goes east       (3,4) */
/*       114     band of avhr            (5,16) */
/*       118     band of hrv1            (17,24) */
/*       121     band of tm              (25,30) */
/*       127     band of mss             (31,34) */
/*       128     band of MAS             (35,41) */
/*       129     MODIS   band            (42,49) */
/*       130     band of avhrr           (50,53) */
/*       131     POLDER  band            (54,61) */
/*       113     SEAWIFS band            (62,69) */
/*       150     AATSR   band            (70,73) */
/*       151     MERIS   band            (74,88) */
/*       152     GLI     band            (89,118) */
/* _otb   18 goto (110, */
/* _otb     s      111, */
/* _otb     s      112,112, */
/* _otb     s      114,114,114,114,114,114,114,114,114,114,114,114, */
/* _otb     s      118,118,118,118,118,118,118,118, */
/* _otb     s      121,121,121,121,121,121, */
/* _otb     s      127,127,127,127, */
/* _otb     s      128,128,128,128,128,128,128, */
/* _otb     s      129,129,129,129,129,129,129,129, */
/* _otb     s      130,130,130,130, */
/* _otb     s      131,131,131,131,131,131,131,131, */
/* _otb     s      113,113,113,113,113,113,113,113, */
/* _otb     s      150,150,150,150, */
/* _otb     s      151,151,151,151,151,151,151,151, */
/* _otb     s      151,151,151,151,151,151,151, */
/* _otb     s      152,152,152,152,152,152,152,152,152,152, */
/* _otb     s      152,152,152,152,152,152,152,152,152,152, */
/* _otb     s      152,152,152,152,152,152,152,152,152,152 */
/* _otb     s     ),iwave */
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
/* _otb  130 call avhrr(iwave-37) */
/* _otb      goto 19 */
/* _otb  131 call polder(iwave-53) */
/* _otb      goto 19 */
/* _otb  113 call seawifs(iwave-61) */
/* _otb      goto 19 */
/* _otb  150 call aatsr(iwave-69) */
/* _otb      goto 19 */
/* _otb  151 call meris(iwave-73) */
/* _otb      goto 19 */
/* _otb  152 call gli(iwave-88) */
/*<       iwave=1			!Added_for_OTB >*/
    iwave = 1;
/*<       wlinf = otb_wlinf		!Added_for_OTB >*/
    sixs_ffu__1.wlinf = *otb_wlinf__;
/*<       wlsup = otb_wlsup		!Added_for_OTB >*/
    sixs_ffu__1.wlsup = *otb_wlsup__;
/*<       do k=1,1501		!Added_for_OTB >*/
    for (k = 1; k <= 1501; ++k) {
/*<          s(k) = otb_s(k)	!Added_for_OTB >*/
	sixs_ffu__1.s[k - 1] = otb_s__[k];
/*<       enddo			!Added_for_OTB >*/
    }
/* _otb_adaptation End : iwave=1 and s(l) becomes an input argument */
/*<    19 iinf=(wlinf-.25)/0.0025+1.5 >*/
/* L19: */
    iinf = (integer) ((sixs_ffu__1.wlinf - .25) / .0025 + 1.5);
/*<       isup=(wlsup-.25)/0.0025+1.5 >*/
    isup = (integer) ((sixs_ffu__1.wlsup - .25) / .0025 + 1.5);
/*<    20 continue >*/
/* L20: */
/* ***********************************************************************
 */
/* LOOK UP TABLE INITIALIZATION */
/* ***********************************************************************
 */
/*  initialization of look up table variable */
/*     Write(6,*) "TOTO THE HERO" */
/*<       do i=1,mu >*/
    i__1 = mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<       nfilut(i)=0 >*/
	nfilut[i__ - 1] = 0;
/*<       do j=1,41 >*/
	for (j = 1; j <= 41; ++j) {
/*<       rolut(i,j)=0. >*/
	    rolut[i__ + j * 25 - 26] = 0.;
/*<       rolutq(i,j)=0. >*/
	    rolutq[i__ + j * 25 - 26] = 0.;
/*<       rolutu(i,j)=0. >*/
	    rolutu[i__ + j * 25 - 26] = 0.;
/*<       filut(i,j)=0. >*/
	    filut[i__ + j * 25 - 26] = 0.;
/*<       roluti(i,j)=0. >*/
	    roluti[i__ + j * 25 - 26] = 0.;
/*<       rolutiq(i,j)=0. >*/
	    rolutiq[i__ + j * 25 - 26] = 0.;
/*<       rolutiu(i,j)=0. >*/
	    rolutiu[i__ + j * 25 - 26] = 0.;
/*<       enddo >*/
	}
/*<       enddo >*/
    }
/*<       xmus=cos(asol*pi/180.) >*/
    xmus = cos(asol * pi / 180.);
/*<       its=acos(xmus)*180.0/pi >*/
    its = acos(xmus) * 180. / pi;
/* Case standart LUT */
/*<       if (ilut.eq.1) then >*/
    if (ilut == 1) {
/*<        do i=1,mu-1 >*/
	i__1 = mu - 1;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<          lutmuv=rm(i) >*/
	    lutmuv = rm[i__ + 25];
/*<          luttv=acos(lutmuv)*180./pi >*/
	    luttv = acos(lutmuv) * 180. / pi;
/*<          iscama=(180-abs(luttv-its)) >*/
	    iscama = 180 - (d__1 = luttv - its, abs(d__1));
/*<          iscami=(180-(luttv+its)) >*/
	    iscami = 180 - (luttv + its);
/*<          nbisca=int(0.01+(iscama-iscami)/4.0)+1 >*/
	    nbisca = (integer) ((iscama - iscami) / 4. + .01) + 1;
/*<          nfilut(i)=nbisca >*/
	    nfilut[i__ - 1] = nbisca;
/*<          filut(i,1)=0.0 >*/
	    filut[i__ - 1] = 0.;
/*<          filut(i,nbisca)=180.0 >*/
	    filut[i__ + nbisca * 25 - 26] = 180.;
/*< 	 scaa=iscama >*/
	    scaa = iscama;
/*<          do j=2,nfilut(i)-1 >*/
	    i__3 = nfilut[i__ - 1] - 1;
	    for (j = 2; j <= i__3; ++j) {
/*<           scaa=scaa-4.0 >*/
		scaa += -4.;
/*<           cscaa=cos(scaa*pi/180.) >*/
		cscaa = cos(scaa * pi / 180.);
/*<    >*/
		cfi = -(cscaa + xmus * lutmuv) / (sqrt(1 - xmus * xmus) * 
			sqrt(1. - lutmuv * lutmuv));
/*<           filut(i,j)=acos(cfi)*180.0/pi >*/
		filut[i__ + j * 25 - 26] = acos(cfi) * 180. / pi;
/*<          enddo >*/
	    }
/*<       enddo >*/
	}
/*<       i=mu >*/
	i__ = mu;
/*<          lutmuv=cos(avis*pi/180.) >*/
	lutmuv = cos(avis * pi / 180.);
/*<          luttv=acos(lutmuv)*180./pi >*/
	luttv = acos(lutmuv) * 180. / pi;
/*<          iscama=(180-abs(luttv-its)) >*/
	iscama = 180 - (d__1 = luttv - its, abs(d__1));
/*<          iscami=(180-(luttv+its)) >*/
	iscami = 180 - (luttv + its);
/*<          nbisca=int((iscama-iscami)/4)+1 >*/
	nbisca = (integer) ((iscama - iscami) / 4) + 1;
/*<          nfilut(i)=nbisca >*/
	nfilut[i__ - 1] = nbisca;
/*<          filut(i,1)=0.0 >*/
	filut[i__ - 1] = 0.;
/*<          filut(i,nbisca)=180.0 >*/
	filut[i__ + nbisca * 25 - 26] = 180.;
/*< 	 scaa=iscama >*/
	scaa = iscama;
/*<          do j=2,nfilut(i)-1 >*/
	i__1 = nfilut[i__ - 1] - 1;
	for (j = 2; j <= i__1; ++j) {
/*<           scaa=scaa-4.0 >*/
	    scaa += -4.;
/*<           cscaa=cos(scaa*pi/180.) >*/
	    cscaa = cos(scaa * pi / 180.);
/*<    >*/
	    cfi = -(cscaa + xmus * lutmuv) / (sqrt(1 - xmus * xmus) * sqrt(1. 
		    - lutmuv * lutmuv));
/*<           filut(i,j)=acos(cfi)*180.0/pi >*/
	    filut[i__ + j * 25 - 26] = acos(cfi) * 180. / pi;
/*<          enddo >*/
	}
/*<         endif >*/
    }
/* END Case standart LUT */
/* Case LUT for APS */
/*<       if (ilut.eq.3) then >*/
    if (ilut == 3) {
/*<        do i=1,mu-1 >*/
	i__1 = mu - 1;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<          nbisca=2 >*/
	    nbisca = 2;
/*<          nfilut(i)=nbisca >*/
	    nfilut[i__ - 1] = nbisca;
/*<          filut(i,1)=(phi0-phiv) >*/
	    filut[i__ - 1] = phi0 - phiv;
/*<          filut(i,nbisca)=(phi0-phiv)+180.0 >*/
	    filut[i__ + nbisca * 25 - 26] = phi0 - phiv + 180.;
/*<       enddo >*/
	}
/*<       i=mu >*/
	i__ = mu;
/*<          nbisca=1 >*/
	nbisca = 1;
/*<          nfilut(i)=nbisca >*/
	nfilut[i__ - 1] = nbisca;
/*<          filut(i,1)=(phi0-phiv) >*/
	filut[i__ - 1] = phi0 - phiv;
/*<          endif >*/
    }
/* END 	Case LUT for APS */
/* CCC Check initialization  (debug) */
/*<        do i=1,mu >*/
    i__1 = mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          lutmuv=rm(i) >*/
	lutmuv = rm[i__ + 25];
/*<          luttv=acos(lutmuv)*180./pi >*/
	luttv = acos(lutmuv) * 180. / pi;
/*<         do j=1,nfilut(i) >*/
	i__3 = nfilut[i__ - 1];
	for (j = 1; j <= i__3; ++j) {
/*<    >*/
	    cscaa = -xmus * lutmuv - cos(filut[i__ + j * 25 - 26] * pi / 180.)
		     * sqrt(1. - xmus * xmus) * sqrt(1. - lutmuv * lutmuv);
/*<        scaa=acos(cscaa)*180./pi >*/
	    scaa = acos(cscaa) * 180. / pi;
/*<       write(6,*) its,luttv,filut(i,j),scaa >*/
	    s_wsle(&io___123);
	    do_lio(&c__5, &c__1, (char *)&its, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&luttv, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&filut[i__ + j * 25 - 26], (ftnlen)
		    sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&scaa, (ftnlen)sizeof(doublereal));
	    e_wsle();
/*<       enddo >*/
	}
/*<       enddo >*/
    }
/* CCC Check initialization  (debug) */
/* ***********************************************************************
 */
/* END LOOK UP TABLE INITIALIZATION */
/* ***********************************************************************
 */
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
    discom_(&idatmp, &iaer, &iaer_prof__, &xmus, &xmuv, &phi, &taer55, &
	    taer55p, &palt, &phirad, &nt, &mu, &np, rm, gb, rp, &ftray, &ipol,
	     xlm1, xlm2, roatm_fi__, &nfi, nfilut, filut, roluts, rolutsq, 
	    rolutsu);
/*<       if(iaer.ne.0) then >*/
    if (iaer != 0) {
/*<    >*/
	specinterp_(&wlmoy, &taer55, &taer55p, &tamoy, &tamoyp, &pizmoy, &
		pizmoyp, &ipol);
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
	trmoyp = 0.;
/*<          tamoyp=0. >*/
	tamoyp = 0.;
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
/*                           addition you have to give the surface      c 
*/
/*                           albedo (spherical albedo). you can also    c 
*/
/*                           select one of the selected model from the  c 
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
/* do 1113 ik=iinf,isup */
/* _otb       s(ik)=0. */
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
/*             ul=LAI     [0.1...10]                                    c 
*/
/*             eps,thm - LAD parameters                                 c 
*/
/*             eps [0.0..0.9] thm [0.0..90.0]                           c 
*/
/*             sl      - relative leaf size  [0.01..1.0]                c 
*/
/*             cAB     - chlorophyll content, ug/cm^2    [30]           c 
*/
/*             cW      - leaf water equivalent thickness  [0.01..0.03]  c 
*/
/*             N       - the effective number of elementary layers      c 
*/
/*                       inside a leaf   [1.225]                        c 
*/
/*             cn      - the ratio of refractive indices of the leaf    c 
*/
/*                       surface wax and internal material  [1.0]       c 
*/
/*             s1      - the weight of the 1st Price function for the   c 
*/
/*                       soil reflectance     [0.1..0.8]                c 
*/
/*        10  MODIS operational BDRF                                     c
 */
/*             the parameters are: p1,p2,p3                             c 
*/
/*                 p1 weight for lambertian kernel                      c 
*/
/*                 p2 weight for Ross Thick kernel                      c 
*/
/*                 p3 weight for Li Sparse  kernel                      c 
*/
/* **********************************************************************c
 */
/*<       fr=0. >*/
    fr = 0.;
/*<       rad=0. >*/
    rad = 0.;
/*<       do 1116 ik=iinf,isup >*/
    i__1 = isup;
    for (ik = iinf; ik <= i__1; ++ik) {
/*<         rocl(ik)=0. >*/
	rocl[ik - 1] = 0.;
/*<         roel(ik)=0. >*/
	roel[ik - 1] = 0.;
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
/*<       inhomo=0		!Added_for_OTB >*/
    inhomo = 0;
/*<       idirec=0		!Added_for_OTB >*/
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
/* _otb        call versalbe(options,optics,struct, */
/* _otb     s       albbrdf) */
/* _otb        go to 69 */
/* _otb      endif */
/* **********************************************************************c
 */
/*     brdf from Roujean et al's model                                  c 
*/
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.3) then */
/* _otb        read(iread,*) par1,par2,par3 */
/* _otb */
/* _otb        srm(-1)=phirad */
/* _otb        srm(1)=xmuv */
/* _otb        srm(0)=xmus */
/* _otb        call roujbrdf(par1,par2,par3,1,1,srm,srp, */
/* _otb     s           sbrdftmp) */
/* _otb        do l=iinf,isup */
/* _otb           sbrdf(l)=sbrdftmp(1,1) */
/* _otb           enddo */
/* _otb */
/* _otb        rm(-mu)=phirad */
/* _otb        rm(mu)=xmuv */
/* _otb        rm(0)=xmus */
/* _otb        call roujbrdf(par1,par2,par3,mu,np,rm,rp, */
/* _otb     s           brdfints) */
/* _otb        rm(-mu)=2.*pi-phirad */
/* _otb        rm(mu)=xmus */
/* _otb        rm(0)=xmuv */
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
/* _otb        srm(-1)=phirad */
/* _otb        srm(1)=xmuv */
/* _otb        srm(0)=xmus */
/* _otb        call waltbrdf(par1,par2,par3,par4,1,1,srm,srp, */
/* _otb     s           sbrdftmp) */
/* _otb        do l=iinf,isup */
/* _otb           sbrdf(l)=sbrdftmp(1,1) */
/* _otb           enddo */
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
/* _otb        srm(-1)=phirad */
/* _otb        srm(1)=xmuv */
/* _otb        srm(0)=xmus */
/* _otb        call minnbrdf(par1,par2,1,1,srm, */
/* _otb     s           sbrdftmp) */
/* _otb        do l=iinf,isup */
/* _otb           sbrdf(l)=sbrdftmp(1,1) */
/* _otb           enddo */
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
/* _otb     s   	mu,np,rm,rp, */
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
/* _otb */
/* _otb        srm(-1)=phirad */
/* _otb        srm(1)=xmuv */
/* _otb        srm(0)=xmus */
/* _otb        call iapibrdf(pild,pxlt,prl,ptl,prs,pihs,pc,1,1,srm,srp, */
/* _otb     s           sbrdftmp) */
/* _otb        do l=iinf,isup */
/* _otb           sbrdf(l)=sbrdftmp(1,1) */
/* _otb           enddo */
/* _otb */
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
/* _otb        read(iread,*) par1,par2,par3 */
/* _otb        srm(-1)=phirad */
/* _otb        srm(1)=xmuv */
/* _otb        srm(0)=xmus */
/* _otb        call rahmbrdf(par1,par2,par3,1,1,srm,srp, */
/* _otb     s           sbrdftmp) */
/* _otb        do l=iinf,isup */
/* _otb           sbrdf(l)=sbrdftmp(1,1) */
/* _otb           enddo */
/* _otb        rm(-mu)=phirad */
/* _otb        rm(mu)=xmuv */
/* _otb        rm(0)=xmus */
/* _otb        call rahmbrdf(par1,par2,par3,mu,np,rm,rp, */
/* _otb     s           brdfints) */
/* _otb        rm(-mu)=2.*pi-phirad */
/* _otb        rm(mu)=xmus */
/* _otb        rm(0)=xmuv */
/* _otb        call rahmbrdf(par1,par2,par3,mu,np,rm,rp, */
/* _otb     s           brdfintv) */
/* _otb        call rahmalbe(par1,par2,par3, */
/* _otb     s       albbrdf) */
/* _otb        go to 69 */
/* _otb      endif */

/* **********************************************************************c
 */
/*     brdf from kuusk's msrm model                                     c 
*/
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.9) then */
/* _otb         read(iread,*) uli,eei,thmi,sli */
/* _otb         read(iread,*) cabi,cwi,vaii,rnci,rsl1i */
/* _otb */
/* _otb        do l=iinf,isup */
/* _otb           srm(-1)=phirad */
/* _otb           srm(1)=xmuv */
/* _otb           srm(0)=xmus */
/* _otb           wl=.25+(l-1)*step */
/* _otb           call akbrdf(eei,thmi,uli,sli,rsl1i,wl,rnci,cabi,cwi,vaii
 */
/* _otb     s      ,1,1,srm,srp,sbrdftmp) */
/* _otb           sbrdf(l)=sbrdftmp(1,1) */
/* _otb           enddo */
/* _otb */
/* _otb         rm(-mu)=phirad */
/* _otb         rm(mu)=xmuv */
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

/* **********************************************************************c
 */
/*     brdf from MODIS BRDF   model                                     c 
*/
/* **********************************************************************c
 */
/* _otb      if(ibrdf.eq.10) then */
/* _otb         read(iread,*)p1,p2,p3 */
/* _otb */
/* _otb           srm(-1)=phirad */
/* _otb           srm(1)=xmuv */
/* _otb           srm(0)=xmus */
/* _otb           call modisbrdf(p1,p2,p3 */
/* _otb     s      ,1,1,srm,srp,sbrdftmp) */
/* _otb        do l=iinf,isup */
/* _otb           sbrdf(l)=sbrdftmp(1,1) */
/* _otb           enddo */
/* _otb */
/* _otb         rm(-mu)=phirad */
/* _otb         rm(mu)=xmuv */
/* _otb         rm(0)=xmus */
/* _otb         call modisbrdf(p1,p2,p3 */
/* up */
/* _otb       s(ik)=0. */
/* _otb     &            ,mu,np,rm,rp,brdfints) */
/* _otb         rm(-mu)=2.*pi-phirad */
/* _otb         rm(mu)=xmus */
/* _otb         rm(0)=xmuv */
/* _otb         call modisbrdf(p1,p2,p3 */
/* _otb     &            ,mu,np,rm,rp,brdfintv) */

/* _otb         call modisalbe(p1,p2,p3 */
/* _otb     &                 ,albbrdf) */
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
/* _otb        rocl(l)=sbrdf(l) */
/* _otb        roel(l)=sbrdf(l) */
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
/*<        igroun=0		!Added_for_OTB >*/
    igroun = 0;
/* _otb */
/* _otb      if(igroun) 29,32,33 */
/* _otb */
/* _otb  29  read(iread,*) nwlinf,nwlsup */
/* _otb      niinf=(nwlinf-.25)/0.0025+1.5 */
/* _otb      nisup=(nwlsup-.25)/0.0025+1.5 */
/* _otb      read(iread,*) (rocl(i),i=niinf,nisup) */
/* _otb      goto 36 */
/* _otb */
/* _otb  32  read(iread,*) ro */
/*<        ro=0.0		!Added_for_OTB >*/
    ro = 0.;
/* _otb */
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
/* _otb_adaptation End: inhomo=0 idirec=0 igroun=0 ro=0 */
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*       irapp   that input parameter allows activating atmospheric     c 
*/
/*               correction mode                                        c 
*/
/*                                                                      c 
*/
/* 		-1: No atmospheric Correction is performed             c */
/* 	       0,1: Atmospheric Correction with Lambertian assumption  c */
/*                   and with the assumption that                       c 
*/
/* 		    target BRDF is proportional to the input BRDF (see c */
/* 		    case idirec=1)                                     c */
/*                                                                      c 
*/
/*        rapp   parameter that contains the reflectance/radiance       c 
*/
/*               to be corrected.                                       c 
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
/*_otb_adaptation Beginning: irapp=-1 & rapp=-10. no atmospheric correctio
n*/
/* _otb        read(iread,*) irapp */
/* _otb       if (irapp.ge.0) then */
/* _otb          irapp=1 */
/* _otb          read(iread,*) rapp */
/* _otb          endif */
/*< 	irapp=-1		!Added_for_OTB (No atm. corrections selected) >*/
    irapp = -1;
/*< 	rapp=-10.		!Added_for_OTB >*/
    rapp = -10.;
/* _otb_adaptation End : irapp=-1 & rapp=-10. no atmospheric correction */
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*      Some optional input for polarization                            c 
*/
/*                                                                      c 
*/
/*  you can input polarization definition through irop:                 c 
*/
/*         1  enter ropq and ropu (stokes parameter for polarized       c 
*/
/*            surface reflectance                                       c 
*/
/*         2   enter pveg (% vegetation) for use in Nadal,Breon model   c 
*/
/*         3   enter wspd for sunglint polarization  (sunglint)         c 
*/
/*         anything else will result in assuming than surface does not  c 
*/
/*         polarized.                                                   c 
*/
/*                                                                      c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*       ilut=0 */
/*       read(iread,*,end=37) ilut */
/*<        irop=0 >*/
    irop = 0;
/* _otb       read(iread,*,end=37) irop */
/*<        if (irop.eq.1) then >*/
    if (irop == 1) {
/*<        read(iread,*) ropq,ropu >*/
	io___150.ciunit = iread;
	s_rsle(&io___150);
	do_lio(&c__5, &c__1, (char *)&ropq, (ftnlen)sizeof(doublereal));
	do_lio(&c__5, &c__1, (char *)&ropu, (ftnlen)sizeof(doublereal));
	e_rsle();
/*<        endif >*/
    }
/*<        if (irop.eq.2) then >*/
    if (irop == 2) {
/*<        read(iread,*) pveg >*/
	io___153.ciunit = iread;
	s_rsle(&io___153);
	do_lio(&c__5, &c__1, (char *)&pveg, (ftnlen)sizeof(doublereal));
	e_rsle();
/*<        call polnad(asol,avis,phi,pveg,ropq,ropu) >*/
	polnad_(&asol, &avis, &phi, &pveg, &ropq, &ropu);
/*<        endif >*/
    }
/*<        if (irop.eq.3) then >*/
    if (irop == 3) {
/*<        read(iread,*) wspd,azw >*/
	io___155.ciunit = iread;
	s_rsle(&io___155);
	do_lio(&c__5, &c__1, (char *)&wspd, (ftnlen)sizeof(doublereal));
	do_lio(&c__5, &c__1, (char *)&azw, (ftnlen)sizeof(doublereal));
	e_rsle();
/*<        razw=phi0-azw >*/
	razw = phi0 - azw;
/*<        call polglit(asol,avis,phi,wspd,razw,ropq,ropu) >*/
	polglit_(&asol, &avis, &phi, &wspd, &razw, &ropq, &ropu);
/*<        endif >*/
    }
/*<  37    if ((irop.lt.1).or.(irop.gt.3)) then >*/
/* L37: */
    if (irop < 1 || irop > 3) {
/*<        if (idirec.eq.0) then >*/
	if (idirec == 0) {
/*<        ropq=0.000 >*/
	    ropq = 0.;
/*<        ropu=0.000 >*/
	    ropu = 0.;
/*<        else >*/
	} else {
/*<        if (ibrdf.eq.6) then >*/
	    if (ibrdf == 6) {
/*<           irop=3 >*/
		irop = 3;
/*< 	  wspd=pws >*/
		wspd = pws;
/*< 	  azw=phi_wind >*/
		azw = phi_wind__;
/*< 	  razw=phi0-azw >*/
		razw = phi0 - azw;
/*< 	  phi=phi0-phiv >*/
		phi = phi0 - phiv;
/*<           call polglit(asol,avis,phi,wspd,razw,ropq,ropu) >*/
		polglit_(&asol, &avis, &phi, &wspd, &razw, &ropq, &ropu);
/*< 	  endif >*/
	    }
/*<        if (ibrdf.eq.9) then >*/
	    if (ibrdf == 9) {
/*<           irop=2 >*/
		irop = 2;
/*<           pveg=ul >*/
		pveg = ul;
/*< 	  if (pveg.gt.1.) pveg=1 >*/
		if (pveg > 1.) {
		    pveg = 1.;
		}
/*< 	  call polnad(asol,avis,phi,pveg,ropq,ropu) >*/
		polnad_(&asol, &avis, &phi, &pveg, &ropq, &ropu);
/*< 	  endif >*/
	    }
/*<        endif   >*/
	}
/*<        endif >*/
    }
/*      write(6,*) "Surface polarization reflectance, Q,U,rop ", */
/*    s            ropq,ropu,sqrt(ropq*ropq+ropu*ropu) */
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
/* _otb_adaptation Beginning: jump writings */
/*<       goto 8888	!Added_for_OTB >*/
    goto L8888;
/* _otb_adaptation End : jump writings */
/* ---- geometrical conditions ---- */
/*<       write(iwr, 98) >*/
    io___163.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___163);
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
	io___164.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___164);
	e_wsfe();
/*< 	 write(iwr, 103)month,jday >*/
	io___165.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___165);
	do_fio(&c__1, (char *)&month, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&jday, (ftnlen)sizeof(integer));
	e_wsfe();
/*<       endif >*/
    }
/*<       if(igeom.ne.0) write(iwr, 101)month,jday,tu,xlat,xlon >*/
    if (igeom != 0) {
	io___166.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___166);
	do_fio(&c__1, (char *)&month, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&jday, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&tu, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xlat, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xlon, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
/*<       write(iwr, 102)asol,phi0 >*/
    io___170.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___170);
    do_fio(&c__1, (char *)&asol, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&phi0, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 1110)avis,phiv,adif,phi >*/
    io___171.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___171);
    do_fio(&c__1, (char *)&avis, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&phiv, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&adif, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&phi, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* --- atmospheric model ---- */
/*<       write(iwr, 1119) >*/
    io___172.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___172);
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
    io___173.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___173);
    do_fio(&c__1, (char *)&uw, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&uo3, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       goto 219 >*/
    goto L219;
/*<   227 write(iwr, 1272) >*/
L227:
    io___174.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___174);
    e_wsfe();
/*<       do 229 i=1,34 >*/
    for (i__ = 1; i__ <= 34; ++i__) {
/*<         write(iwr, 1271)z(i),p(i),t(i),wh(i),wo(i) >*/
	io___175.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___175);
	do_fio(&c__1, (char *)&sixs_atm__1.z__[i__ - 1], (ftnlen)sizeof(
		doublereal));
	do_fio(&c__1, (char *)&sixs_atm__1.p[i__ - 1], (ftnlen)sizeof(
		doublereal));
	do_fio(&c__1, (char *)&sixs_atm__1.t[i__ - 1], (ftnlen)sizeof(
		doublereal));
	do_fio(&c__1, (char *)&sixs_atm__1.wh[i__ - 1], (ftnlen)sizeof(
		doublereal));
	do_fio(&c__1, (char *)&sixs_atm__1.wo[i__ - 1], (ftnlen)sizeof(
		doublereal));
	e_wsfe();
/*<   229 continue >*/
/* L229: */
    }
/*<       goto 219 >*/
    goto L219;
/*<   226 write(iwr, 1261)atmid(idatm+1) >*/
L226:
    io___176.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___176);
    do_fio(&c__1, atmid + idatm * 51, 51L);
    e_wsfe();
/* --- aerosols model (type) ---- */
/*< 219    write(iwr,5550) >*/
L219:
    io___177.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___177);
    e_wsfe();
/*<        if(iaer.eq.0) then >*/
    if (iaer == 0) {
/*<         write(iwr, 5554) >*/
	io___178.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___178);
	e_wsfe();
/*<         goto 1112 >*/
	goto L1112;
/*<        endif >*/
    }
/*<        if (iaer_prof.eq.1) then                            >*/
    if (iaer_prof__ == 1) {
/*<        aer_model(1)="Continental" >*/
	s_copy(aer_model__, "Continental", 50L, 11L);
/*<        aer_model(2)=" Maritime" >*/
	s_copy(aer_model__ + 50, " Maritime", 50L, 9L);
/*<        aer_model(3)="   Urban" >*/
	s_copy(aer_model__ + 100, "   Urban", 50L, 8L);
/*<        aer_model(4)="user-defined" >*/
	s_copy(aer_model__ + 150, "user-defined", 50L, 12L);
/*<        aer_model(5)="  Desert" >*/
	s_copy(aer_model__ + 200, "  Desert", 50L, 8L);
/*<        aer_model(6)="Biomass Burning" >*/
	s_copy(aer_model__ + 250, "Biomass Burning", 50L, 15L);
/*<        aer_model(7)="Stratospheric" >*/
	s_copy(aer_model__ + 300, "Stratospheric", 50L, 13L);
/*<        aer_model(8)="user-defined" >*/
	s_copy(aer_model__ + 350, "user-defined", 50L, 12L);
/*<        aer_model(9)="user-defined" >*/
	s_copy(aer_model__ + 400, "user-defined", 50L, 12L);
/*<        aer_model(10)="user-defined" >*/
	s_copy(aer_model__ + 450, "user-defined", 50L, 12L);
/*<        aer_model(11)="Sun Photometer" >*/
	s_copy(aer_model__ + 500, "Sun Photometer", 50L, 14L);
/*<        aer_model(12)="user-defined"            >*/
	s_copy(aer_model__ + 550, "user-defined", 50L, 12L);
/*<        num_z=num_z-1 >*/
	--aeroprof_1.num_z__;
/*<        write(6,5551) num_z >*/
	s_wsfe(&io___180);
	do_fio(&c__1, (char *)&aeroprof_1.num_z__, (ftnlen)sizeof(integer));
	e_wsfe();
/*<        write(6,5552) >*/
	s_wsfe(&io___181);
	e_wsfe();
/*<        do i=1,num_z >*/
	i__1 = aeroprof_1.num_z__;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<    >*/
	    s_wsfe(&io___182);
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&height_z__[aeroprof_1.num_z__ + 1 - i__], (
		    ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&aeroprof_1.taer55_z__[aeroprof_1.num_z__ + 
		    1 - i__], (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, aer_model__ + (iaer - 1) * 50, 50L);
	    e_wsfe();
/*<        enddo >*/
	}
/*<        endif >*/
    }
/*<        if (iaer_prof.eq.0) then >*/
    if (iaer_prof__ == 0) {
/*<        aer_model(1)="Continental aerosol model" >*/
	s_copy(aer_model__, "Continental aerosol model", 50L, 25L);
/*<        aer_model(2)="Maritime aerosol model" >*/
	s_copy(aer_model__ + 50, "Maritime aerosol model", 50L, 22L);
/*<        aer_model(3)="Urban aerosol model" >*/
	s_copy(aer_model__ + 100, "Urban aerosol model", 50L, 19L);
/*<        aer_model(5)="Desert aerosol model" >*/
	s_copy(aer_model__ + 200, "Desert aerosol model", 50L, 20L);
/*<        aer_model(6)="Biomass Burning aerosol model" >*/
	s_copy(aer_model__ + 250, "Biomass Burning aerosol model", 50L, 29L);
/*<        aer_model(7)="Stratospheric aerosol model" >*/
	s_copy(aer_model__ + 300, "Stratospheric aerosol model", 50L, 27L);
/*<        aer_model(11)="Sun Photometer aerosol model" >*/
	s_copy(aer_model__ + 500, "Sun Photometer aerosol model", 50L, 28L);
/*<       if (iaer.ge.1.and.iaer.lt.4) write (iwr,132) aer_model(iaer) >*/
	if (iaer >= 1 && iaer < 4) {
	    io___183.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___183);
	    do_fio(&c__1, aer_model__ + (iaer - 1) * 50, 50L);
	    e_wsfe();
	}
/*<       if (iaer.ge.5.and.iaer.le.7) write (iwr,132) aer_model(iaer) >*/
	if (iaer >= 5 && iaer <= 7) {
	    io___184.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___184);
	    do_fio(&c__1, aer_model__ + (iaer - 1) * 50, 50L);
	    e_wsfe();
	}
/*<       if (iaer.eq.11) write(iwr,132) aer_model(iaer) >*/
	if (iaer == 11) {
	    io___185.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___185);
	    do_fio(&c__1, aer_model__ + (iaer - 1) * 50, 50L);
	    e_wsfe();
	}
/*<       endif >*/
    }
/*<        if (iaer.eq.4)write(iwr,133)(c(i),i=1,4) >*/
    if (iaer == 4) {
	io___186.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___186);
	for (i__ = 1; i__ <= 4; ++i__) {
	    do_fio(&c__1, (char *)&c__[i__ - 1], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
/*<        if (iaer.eq.8) then >*/
    if (iaer == 8) {
/*<         write(6,134) icp >*/
	s_wsfe(&io___187);
	do_fio(&c__1, (char *)&mie_in__1.icp, (ftnlen)sizeof(integer));
	e_wsfe();
/*<         do i=1,icp >*/
	i__1 = mie_in__1.icp;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<          write(iwr,135)x1(i),x2(i),cij_out(i) >*/
	    io___188.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___188);
	    do_fio(&c__1, (char *)&mie_in__1.x1[i__ - 1], (ftnlen)sizeof(
		    doublereal));
	    do_fio(&c__1, (char *)&mie_in__1.x2[i__ - 1], (ftnlen)sizeof(
		    doublereal));
	    do_fio(&c__1, (char *)&cij_out__[i__ - 1], (ftnlen)sizeof(
		    doublereal));
	    e_wsfe();
/*<         enddo >*/
	}
/*<        endif >*/
    }
/*<        if (iaer.eq.9) write(iwr,136)x1(1),x2(1),x3(1) >*/
    if (iaer == 9) {
	io___189.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___189);
	do_fio(&c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&mie_in__1.x2[0], (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&mie_in__1.x3[0], (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
/*<        if (iaer.eq.10) write(iwr,137)x1(1)  >*/
    if (iaer == 10) {
	io___190.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___190);
	do_fio(&c__1, (char *)&mie_in__1.x1[0], (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
/*<        if (iaerp.eq.1)write(iwr,139)FILE2(1:i2) >*/
    if (iaerp == 1) {
	io___191.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___191);
	do_fio(&c__1, file2, i2);
	e_wsfe();
    }
/*<        if (iaer.eq.12)write(iwr,138)FILE2(1:i2) >*/
    if (iaer == 12) {
	io___192.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___192);
	do_fio(&c__1, file2, i2);
	e_wsfe();
    }
/* --- aerosol model (concentration) ---- */
/* --- for the exponential profile ---- */
/*<       if (iaer_prof.eq.0) then >*/
    if (iaer_prof__ == 0) {
/*<       if(abs(v).le.xacc) write(iwr, 140)taer55 >*/
	if (abs(v) <= sixs_test__1.xacc) {
	    io___193.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___193);
	    do_fio(&c__1, (char *)&taer55, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
/*<       if(abs(v).gt.xacc) write(iwr, 141)v,taer55 >*/
	if (abs(v) > sixs_test__1.xacc) {
	    io___194.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___194);
	    do_fio(&c__1, (char *)&v, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&taer55, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
/*<       endif >*/
    }
/*< 1112  write(6,5555) >*/
L1112:
    s_wsfe(&io___195);
    e_wsfe();
/* --- spectral condition ---- */
/*<       write(iwr, 148) >*/
    io___196.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___196);
    e_wsfe();
/*<       if(iwave.eq.-2) write(iwr, 1510) nsat(1),wlinf,wlsup >*/
    if (iwave == -2) {
	io___197.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___197);
	do_fio(&c__1, nsat, 17L);
	do_fio(&c__1, (char *)&sixs_ffu__1.wlinf, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sixs_ffu__1.wlsup, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
/*<       if(iwave.eq.-1) write(iwr, 149) wl >*/
    if (iwave == -1) {
	io___198.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___198);
	do_fio(&c__1, (char *)&wl, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
/*<       if(iwave.ge.0) write(iwr, 1510) nsat(iwave+1), wlinf,wlsup >*/
    if (iwave >= 0) {
	io___199.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___199);
	do_fio(&c__1, nsat + iwave * 17, 17L);
	do_fio(&c__1, (char *)&sixs_ffu__1.wlinf, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sixs_ffu__1.wlsup, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
/* ---- atmospheric polarization requested */
/*<       if (ipol.ne.0)then >*/
    if (ipol != 0) {
/*< 	write(iwr, 142) >*/
	io___200.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___200);
	e_wsfe();
/*< 	if (irop.eq.1) write(iwr,146) ropq,ropq >*/
	if (irop == 1) {
	    io___201.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___201);
	    do_fio(&c__1, (char *)&ropq, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&ropq, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
/*< 	if (irop.eq.2) write(iwr,144) pveg*100.0 >*/
	if (irop == 2) {
	    io___202.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___202);
	    d__1 = pveg * 100.;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
/*< 	if (irop.eq.3) write(iwr,145) wspd,azw >*/
	if (irop == 3) {
	    io___203.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___203);
	    do_fio(&c__1, (char *)&wspd, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&azw, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
/*< 	w >*/
	io___204.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___204);
	do_fio(&c__1, (char *)&ropq, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&ropu, (ftnlen)sizeof(doublereal));
	d__1 = sqrt(ropq * ropq + ropu * ropu);
	do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	d__2 = atan2(ropu, ropq) * 180. / 3.1415927 / 2.;
	do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       endif >*/
    }
/* _otb_adaptation Beginning: jump writings */
/*< 8888  continue    	!Added_for_OTB >*/
L8888:
/* _otb_adaptation End : jump writings */
/* --- ground reflectance (type and spectral variation) ---- */
/*<       if(idirec.eq.0) then >*/
    if (idirec == 0) {
/*<         rocave=0. >*/
	rocave = 0.;
/*<         roeave=0. >*/
	roeave = 0.;
/*<         seb=0. >*/
	seb = 0.;
/*<         do 264 i=iinf,isup >*/
	i__1 = isup;
	for (i__ = iinf; i__ <= i__1; ++i__) {
/*<           sbor=s(i) >*/
	    sbor = sixs_ffu__1.s[i__ - 1];
/*<           if(i.eq.iinf.or.i.eq.isup) sbor=sbor*0.5 >*/
	    if (i__ == iinf || i__ == isup) {
		sbor *= .5;
	    }
/*<           wl=.25+(i-1)*step >*/
	    wl = (i__ - 1) * step + .25;
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
	io___211.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___211);
	do_fio(&c__1, (char *)&rad, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         igroun=igrou1 >*/
	igroun = igrou1;
/*<         ro=rocave >*/
	ro = rocave;
/*<         write(iwr, 170) >*/
	io___213.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___213);
	e_wsfe();
/*<         goto 261 >*/
	goto L261;
/*<   262   igroun=igrou2 >*/
L262:
	igroun = igrou2;
/*<         ro=roeave >*/
	ro = roeave;
/*<         write(iwr, 171) >*/
	io___215.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___215);
	e_wsfe();
/*<         goto 261 >*/
	goto L261;
/* _otb_adaptation Beginning: jump writings */
/* _otb  260   write(iwr, 168) */
/* _otb  261   if (igroun.gt.0)write(iwr, reflec(igroun+3))ro */
/*<   260  continue    	!Added_for_OTB >*/
L260:
/*<   261  continue    	!Added_for_OTB >*/
L261:
/* _otb_adaptation End : jump writings */
/* _otb       if (igroun.gt.0)goto 158 */
/* _otb        if(igroun.eq.-1) write(iwr, reflec(1))ro */
/* _otb        if(igroun.eq.-1) goto 158 */
/* _otb        if(iwave.eq.-1)  write(iwr, reflec(2))ro */
/* _otb        if(iwave.ne.-1)  write(iwr, reflec(3))ro */
/*<  158    isort=isort+1 >*/
/* L158: */
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
/* _otb        write(iwr, 168) */
/*<         if(idirec.eq.1) then >*/
	if (idirec == 1) {
/*<         rocave=0. >*/
	    rocave = 0.;
/*<         rfoamave=0. >*/
	    rfoamave = 0.;
/*<         rwatave=0. >*/
	    rwatave = 0.;
/*<         rglitave=0. >*/
	    rglitave = 0.;
/*<         seb=0. >*/
	    seb = 0.;
/*<         do  i=iinf,isup >*/
	    i__1 = isup;
	    for (i__ = iinf; i__ <= i__1; ++i__) {
/*<           sbor=s(i) >*/
		sbor = sixs_ffu__1.s[i__ - 1];
/*<           if(i.eq.iinf.or.i.eq.isup) sbor=sbor*0.5 >*/
		if (i__ == iinf || i__ == isup) {
		    sbor *= .5;
		}
/*<           wl=.25+(i-1)*step >*/
		wl = (i__ - 1) * step + .25;
/*<    >*/
		solirr_(&wl, &swl);
/*<           swl=swl*dsol >*/
		swl *= dsol;
/*<           rocave=rocave+rocl(i)*sbor*swl*step >*/
		rocave += rocl[i__ - 1] * sbor * swl * step;
/*<           rfoamave=rfoamave+rfoaml(i)*sbor*swl*step >*/
		rfoamave += rfoaml[i__ - 1] * sbor * swl * step;
/*<           rwatave=rwatave+rwatl(i)*sbor*swl*step >*/
		rwatave += rwatl[i__ - 1] * sbor * swl * step;
/*<           rglitave=rglitave+rglitl(i)*sbor*swl*step >*/
		rglitave += rglitl[i__ - 1] * sbor * swl * step;
/*<           seb=seb+sbor*swl*step >*/
		seb += sbor * swl * step;
/*<         enddo >*/
	    }
/*<         rocave=rocave/seb >*/
	    rocave /= seb;
/*< 	rfoamave=rfoamave/seb >*/
	    rfoamave /= seb;
/*< 	rwatave=rwatave/seb >*/
	    rwatave /= seb;
/*< 	rglitave=rglitave/seb >*/
	    rglitave /= seb;
/*_otb         goto(2000,2001,2002,2003,2004,2005,2006,2007,2008,2
010,2011)*/
/* _otb     *    ,(ibrdf+1) */
/* _otb 2000    write(iwr, 190) */
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2001    write(iwr, 191)par1,par2,par3,par4 */
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2002    write(iwr, 192)optics(1),struct(1),struct(2) */
/* _otb         if (options(5).eq.0) write(iwr, 200) */
/* _otb         if (options(5).eq.1) write(iwr, 201) */
/* _otb         if (options(3).eq.0) write(iwr, 197)struct(3),stru
ct(4) */
/* _otb         if (options(3).eq.1) write(iwr, 198)struct(3) */
/* _otb         if (options(3).eq.2) write(iwr, 199)struct(3) */
/* _otb         if (options(4).eq.0) write(iwr, 202) */
/* _otb         if (options(4).eq.1) write(iwr, 203)optics(2) */
/* _otb         if (options(4).eq.2) write(iwr, 204)optics(2),opti
cs(3) */
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2003    write(iwr, 193)par1,par2,par3 */
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2004    write(iwr, 194)par1,par2,par3,par4 */
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2005    write(iwr, 195)par1,par2 */
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2006    write(iwr, 196)pws,phi_wind,xsal,pcl */
/* _otb         write(iwr,500) rfoamave,rwatave,rglitave */
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2007    write(iwr, 205) pRl,pTl,pRs,PxLt */
/* _otb         if (pihs.eq.0) then */
/* _otb           write(iwr,207)' no hot spot       ' */
/* _otb         else */
/* _otb           write(iwr,208)' hot spot parameter',pc */
/* _otb         endif */
/*_otb         if (pild.eq.1) write(iwr,209) ' planophile   leaf d
istribution'*/
/*_otb         if (pild.eq.2) write(iwr,209) ' erectophile  leaf d
istribution'*/
/*_otb         if (pild.eq.3) write(iwr,209) ' plagiophile  leaf d
istribution'*/
/*_otb         if (pild.eq.4) write(iwr,209) ' extremophile leaf d
istribution'*/
/*_otb         if (pild.eq.5) write(iwr,209) ' uniform      leaf d
istribution'*/
/* _otb         write(iwr, 187) */
/* _otb     *rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2008    write(iwr, 206) par1,par2,par3 */
/* _otb         goto 2009 */
/* _otb 2010    write(iwr, 210)uli,eei,thmi,sli,cabi,cwi,vaii,rnci
,rsl1i */
/* _otb         write(iwr, 187) */
/* _otb     *   rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/* _otb 2011    write(iwr, 211)p1,p2,p3 */
/* _otb         write(iwr, 187) */
/* _otb     *   rocave,robar1/xnorm1,robar2/xnorm2,albbrdf */
/* _otb         goto 2009 */
/*<  2009   endif >*/
/* L2009: */
	}
/*<       endif >*/
    }
/*<   50  continue >*/
/* L50: */
/* --- pressure at ground level (174) and altitude (175) ---- */
/* _otb_adaptation Beginning: jump writings */
/* _otb  999 write(iwr, 173) */
/* _otb      write(iwr, 174)p(1) */
/* _otb      write(iwr, 175)xps */
/* _otb      if (xps.gt.0..and.idatm.ne.0) write(iwr, 176)uw,uo3 */
/*<   999  continue    	!Added_for_OTB >*/
L999:
/* _otb_adaptation End : jump writings */
/* --- plane simulation output if selected ---- */
/* _otb      if (palt.lt.1000.) then */
/* _otb       write(iwr, 178) */
/* _otb       write(iwr, 179)pps */
/* _otb       write(iwr, 180)zpl(34) */
/* _otb       write(iwr, 181) */
/* _otb       write(iwr, 182)puo3 */
/* _otb       write(iwr, 183)puw */
/* _otb       write(iwr, 184)taer55p */
/* _otb      endif */
/* ---- atmospheric correction  ---- */
/* _otb      if (irapp.ge.0) then */
/* _otb        write(iwr, 177) */
/* _otb          if (irapp.eq. 0) write(iwr, 220) */
/* _otb          if (irapp.eq. 1) write(iwr, 221) */
/* _otb       if (rapp.lt.0.) then */
/* _otb        write(iwr, 185)-rapp */
/* _otb       else */
/* _otb        write(iwr, 186)rapp */
/* _otb       endif */
/* _otb      endif */
/* _otb      write(iwr, 172) */
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
/* Start Update Look up table */
/*< 	do i=1,mu >*/
    i__1 = mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*< 	do j=1,41 >*/
	for (j = 1; j <= 41; ++j) {
/*< 	roluti(i,j)=0.0 >*/
	    roluti[i__ + j * 25 - 26] = 0.;
/*< 	rolutiq(i,j)=0.0 >*/
	    rolutiq[i__ + j * 25 - 26] = 0.;
/*< 	rolutiu(i,j)=0.0 >*/
	    rolutiu[i__ + j * 25 - 26] = 0.;
/*< 	enddo >*/
	}
/*< 	enddo >*/
    }
/* End Update Look up table */
/*<       sb=0. >*/
    sb = 0.;
/*<       seb=0. >*/
    seb = 0.;
/*<       refet=0. >*/
    refet = 0.;
/*<       refet1=0. >*/
    refet1 = 0.;
/*<       refet2=0. >*/
    refet2 = 0.;
/*<       refet3=0. >*/
    refet3 = 0.;
/*<       rpfet=0. >*/
    rpfet = 0.;
/*<       rpfet1=0. >*/
    rpfet1 = 0.;
/*<       rpfet2=0. >*/
    rpfet2 = 0.;
/*<       rpfet3=0. >*/
    rpfet3 = 0.;
/*<       alumet=0. >*/
    alumet = 0.;
/*<       plumet=0. >*/
    plumet = 0.;
/*<       tgasm=0. >*/
    tgasm = 0.;
/*<       rog=0. >*/
    rog = 0.;
/*<       dgasm=0. >*/
    dgasm = 0.;
/*<       ugasm=0. >*/
    ugasm = 0.;
/*<       sdwava=0. >*/
    sdwava = 0.;
/*<       sdozon=0. >*/
    sdozon = 0.;
/*<       sddica=0. >*/
    sddica = 0.;
/*<       sdoxyg=0. >*/
    sdoxyg = 0.;
/*<       sdniox=0. >*/
    sdniox = 0.;
/*<       sdmoca=0. >*/
    sdmoca = 0.;
/*<       sdmeth=0. >*/
    sdmeth = 0.;
/*<       suwava=0. >*/
    suwava = 0.;
/*<       suozon=0. >*/
    suozon = 0.;
/*<       sudica=0. >*/
    sudica = 0.;
/*<       suoxyg=0. >*/
    suoxyg = 0.;
/*<       suniox=0. >*/
    suniox = 0.;
/*<       sumoca=0. >*/
    sumoca = 0.;
/*<       sumeth=0. >*/
    sumeth = 0.;
/*<       stwava=0. >*/
    stwava = 0.;
/*<       stozon=0. >*/
    stozon = 0.;
/*<       stdica=0. >*/
    stdica = 0.;
/*<       stoxyg=0. >*/
    stoxyg = 0.;
/*<       stniox=0. >*/
    stniox = 0.;
/*<       stmoca=0. >*/
    stmoca = 0.;
/*<       stmeth=0. >*/
    stmeth = 0.;
/*<       sodray=0. >*/
    sodray = 0.;
/*<       sodrayp=0. >*/
    sodrayp = 0.;
/*<       sodaer=0. >*/
    sodaer = 0.;
/*<       sodaerp=0. >*/
    sodaerp = 0.;
/*<       sodtot=0. >*/
    sodtot = 0.;
/*<       sodtotp=0. >*/
    sodtotp = 0.;
/*<       fophsr=0. >*/
    fophsr = 0.;
/*<       fophsa=0. >*/
    fophsa = 0.;
/*<       foqhsr=0. >*/
    foqhsr = 0.;
/*<       foqhsa=0. >*/
    foqhsa = 0.;
/*<       fouhsr=0. >*/
    fouhsr = 0.;
/*<       fouhsa=0. >*/
    fouhsa = 0.;
/*<       sroray=0. >*/
    sroray = 0.;
/*<       sroaer=0. >*/
    sroaer = 0.;
/*<       srotot=0. >*/
    srotot = 0.;
/*<       srpray=0. >*/
    srpray = 0.;
/*<       srpaer=0. >*/
    srpaer = 0.;
/*<       srptot=0. >*/
    srptot = 0.;
/*<       srqray=0. >*/
    srqray = 0.;
/*<       srqaer=0. >*/
    srqaer = 0.;
/*<       srqtot=0. >*/
    srqtot = 0.;
/*<       sruray=0. >*/
    sruray = 0.;
/*<       sruaer=0. >*/
    sruaer = 0.;
/*<       srutot=0. >*/
    srutot = 0.;
/*<       ssdaer=0. >*/
    ssdaer = 0.;
/*<       sdtotr=0. >*/
    sdtotr = 0.;
/*<       sdtota=0. >*/
    sdtota = 0.;
/*<       sdtott=0. >*/
    sdtott = 0.;
/*<       sutotr=0. >*/
    sutotr = 0.;
/*<       sutota=0. >*/
    sutota = 0.;
/*<       sutott=0. >*/
    sutott = 0.;
/*<       sasr=0. >*/
    sasr = 0.;
/*<       sasa=0. >*/
    sasa = 0.;
/*<       sast=0. >*/
    sast = 0.;
/*<       do 52 i=1,2 >*/
    for (i__ = 1; i__ <= 2; ++i__) {
/*<         do 53 j=1,3 >*/
	for (j = 1; j <= 3; ++j) {
/*<           ani(i,j)=0. >*/
	    ani[i__ + (j << 1) - 3] = 0.;
/*<           aini(i,j)=0. >*/
	    aini[i__ + (j << 1) - 3] = 0.;
/*<           anr(i,j)=0. >*/
	    anr[i__ + (j << 1) - 3] = 0.;
/*<           ainr(i,j)=0. >*/
	    ainr[i__ + (j << 1) - 3] = 0.;
/*<    53   continue >*/
/* L53: */
	}
/*<    52 continue >*/
/* L52: */
    }
/* ---- spectral loop ---- */
/*<       if (iwave.eq.-2) write(iwr,1500) >*/
    if (iwave == -2) {
	io___296.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___296);
	e_wsfe();
    }
/*<         do 51 l=iinf,isup >*/
    i__1 = isup;
    for (l = iinf; l <= i__1; ++l) {
/*<         sbor=s(l) >*/
	sbor = sixs_ffu__1.s[l - 1];
/*<         if(l.eq.iinf.or.l.eq.isup) sbor=sbor*0.5 >*/
	if (l == iinf || l == isup) {
	    sbor *= .5;
	}
/*<         if(iwave.eq.-1) sbor=1.0/step >*/
	if (iwave == -1) {
	    sbor = 1. / step;
	}
/*<         roc=rocl(l) >*/
	roc = rocl[l - 1];
/*<         roe=roel(l) >*/
	roe = roel[l - 1];
/*<         wl=.25+(l-1)*step >*/
	wl = (l - 1) * step + .25;

/*<    >*/
	d__1 = uw / 2.;
	d__2 = puw / 2.;
	abstra_(&idatm, &wl, &xmus, &xmuv, &d__1, &uo3, &uwus, &uo3us, &
		idatmp, &d__2, &puo3, &puwus, &puo3us, &dtwava, &dtozon, &
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
	    dtwava = 0.;
	}
/*<         if (dtozon.lt.accu3) dtozon=0. >*/
	if (dtozon < accu3) {
	    dtozon = 0.;
	}
/*<         if (dtdica.lt.accu3) dtdica=0. >*/
	if (dtdica < accu3) {
	    dtdica = 0.;
	}
/*<         if (dtniox.lt.accu3) dtniox=0. >*/
	if (dtniox < accu3) {
	    dtniox = 0.;
	}
/*<         if (dtmeth.lt.accu3) dtmeth=0. >*/
	if (dtmeth < accu3) {
	    dtmeth = 0.;
	}
/*<         if (dtmoca.lt.accu3) dtmeth=0. >*/
	if (dtmoca < accu3) {
	    dtmeth = 0.;
	}
/*<         if (utwava.lt.accu3) utwava=0. >*/
	if (utwava < accu3) {
	    utwava = 0.;
	}
/*<         if (utozon.lt.accu3) utozon=0. >*/
	if (utozon < accu3) {
	    utozon = 0.;
	}
/*<         if (utdica.lt.accu3) utdica=0. >*/
	if (utdica < accu3) {
	    utdica = 0.;
	}
/*<         if (utniox.lt.accu3) utniox=0. >*/
	if (utniox < accu3) {
	    utniox = 0.;
	}
/*<         if (utmeth.lt.accu3) utmeth=0. >*/
	if (utmeth < accu3) {
	    utmeth = 0.;
	}
/*<         if (utmoca.lt.accu3) utmeth=0. >*/
	if (utmoca < accu3) {
	    utmeth = 0.;
	}
/*<         if (ttwava.lt.accu3) ttwava=0. >*/
	if (ttwava < accu3) {
	    ttwava = 0.;
	}
/*<         if (ttozon.lt.accu3) ttozon=0. >*/
	if (ttozon < accu3) {
	    ttozon = 0.;
	}
/*<         if (ttdica.lt.accu3) ttdica=0. >*/
	if (ttdica < accu3) {
	    ttdica = 0.;
	}
/*<         if (ttniox.lt.accu3) ttniox=0. >*/
	if (ttniox < accu3) {
	    ttniox = 0.;
	}
/*<         if (ttmeth.lt.accu3) ttmeth=0. >*/
	if (ttmeth < accu3) {
	    ttmeth = 0.;
	}
/*<         if (ttmoca.lt.accu3) ttmeth=0. >*/
	if (ttmoca < accu3) {
	    ttmeth = 0.;
	}

/*<    >*/
	solirr_(&wl, &swl);
/*<         swl=swl*dsol >*/
	swl *= dsol;
/*<         coef=sbor*step*swl >*/
	coef = sbor * step * swl;
/*<         coefp=sbor*step >*/
	coefp = sbor * step;
/*<    >*/
	interp_(&iaer, &idatmp, &wl, &taer55, &taer55p, &xmud, &romix, &
		rorayl, &roaero, &phaa, &phar, &rqmix, &rqrayl, &rqaero, &
		qhaa, &qhar, &rumix, &rurayl, &ruaero, &uhaa, &uhar, &tsca, &
		tray, &trayp, &taer, &taerp, &dtott, &utott, &astot, &asray, &
		asaer, &utotr, &utota, &dtotr, &dtota, &ipol, roatm_fi__, 
		romix_fi__, rorayl_fi__, &nfi, roluts, rolut, rolutsq, rolutq,
		 rolutsu, rolutu, nfilut);
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
/* C--- computing integrated values over the spectral band------ */
/*<         sb=sb+sbor*step >*/
	sb += sbor * step;
/*<         seb=seb+coef >*/
	seb += coef;
/*  ---unpolarized light */
/*<           edifr=utotr-exp(-trayp/xmuv) >*/
	edifr = utotr - exp(-trayp / xmuv);
/*<           edifa=utota-exp(-taerp/xmuv) >*/
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
		    robard[l - 1] * robard[l - 1] / (1. - astot * robard[l - 
		    1]);
/*<         avr=robard(l) >*/
	    avr = robard[l - 1];
/*<         else >*/
	} else {
/*<           call enviro(edifr,edifa,rad,palt,xmuv,fra,fae,fr) >*/
	    enviro_(&edifr, &edifa, &rad, &palt, &xmuv, &fra, &fae, &fr);
/*<           avr=roc*fr+(1.-fr)*roe >*/
	    avr = roc * fr + (1. - fr) * roe;
/*<    >*/
	    rsurf = roc * dtott * exp(-(trayp + taerp) / xmuv) / (1. - avr * 
		    astot) + avr * dtott * (utott - exp(-(trayp + taerp) / 
		    xmuv)) / (1. - avr * astot);
/*<         endif >*/
	}
/*<         ratm1=(romix-rorayl)*tgtot+rorayl*tgp1 >*/
	ratm1 = (romix - rorayl) * tgtot + rorayl * tgp1;
/*<         ratm3=romix*tgp1 >*/
	ratm3 = romix * tgp1;
/*<         ratm2=(romix-rorayl)*tgp2+rorayl*tgp1 >*/
	ratm2 = (romix - rorayl) * tgp2 + rorayl * tgp1;
/*< 	do i=1,nfi >*/
	i__3 = nfi;
	for (i__ = 1; i__ <= i__3; ++i__) {
/*< 	ratm2_fi(i)=(romix_fi(i)-rorayl_fi(i))*tgp2+rorayl_fi(i)*tgp1 >*/
	    ratm2_fi__[i__ - 1] = (romix_fi__[i__ - 1] - rorayl_fi__[i__ - 1])
		     * tgp2 + rorayl_fi__[i__ - 1] * tgp1;
/*< 	enddo >*/
	}
/*<         romeas1=ratm1+rsurf*tgtot >*/
	romeas1 = ratm1 + rsurf * tgtot;
/*<         romeas2=ratm2+rsurf*tgtot >*/
	romeas2 = ratm2 + rsurf * tgtot;
/*<         romeas3=ratm3+rsurf*tgtot >*/
	romeas3 = ratm3 + rsurf * tgtot;
/*    computing integrated values over the spectral band */
/*<         alumeas=xmus*swl*romeas2/pi >*/
	alumeas = xmus * swl * romeas2 / pi;
/*<         alumet=alumet+alumeas*sbor*step >*/
	alumet += alumeas * sbor * step;
/*< 	rfoamave=rfoamave+rfoaml(i)*sbor*swl*step >*/
	rfoamave += rfoaml[i__ - 1] * sbor * swl * step;
/*< 	rwatave=rwatave+rwatl(i)*sbor*swl*step >*/
	rwatave += rwatl[i__ - 1] * sbor * swl * step;
/*< 	rglitave=rglitave+rglitl(i)*sbor*swl*step >*/
	rglitave += rglitl[i__ - 1] * sbor * swl * step;
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
/*< 	do i=1,nfi >*/
	i__3 = nfi;
	for (i__ = 1; i__ <= i__3; ++i__) {
/*< 	refet_fi(i)=refet_fi(i)+ratm2_fi(i)*coef >*/
	    refet_fi__[i__ - 1] += ratm2_fi__[i__ - 1] * coef;
/*< 	enddo >*/
	}
/* Start Update Look up table */
/* 	do i=1,mu */
/* 	do j=1,41 */
/* 	roluti(i,j)=roluti(i,j)+rolut(i,j)*coef */
/* 	rolutiq(i,j)=rolutiq(i,j)+rolutq(i,j)*coef */
/* 	rolutiu(i,j)=rolutiu(i,j)+rolutu(i,j)*coef */
/* 	enddo */
/* 	enddo */
/* End Update Look up table */
/*<         if (iwave.eq.-2) then >*/
	if (iwave == -2) {
/* _otb          write(iwr,1501) wl,tgtot,dtott,utott,astot,ratm2,
swl,roc, */
/* _otb     s            sbor,dsol,romeas2 */
/*<         endif >*/
	}
/*  ---polarized light: */
/*       -the spectral integration without the solar irradiance */
/*           because the sun does not generate polarized light */
/*       -we assume a Lambertian ground, then no polarized */
/*           surface reflectance (rpsurf=0.0, avr=0.0, roc=0.0) */
/*< 	if (ipol.ne.0)then >*/
	if (ipol != 0) {
/*<           rqatm2=(rqmix-rqrayl)*tgp2+rqrayl*tgp1 >*/
	    rqatm2 = (rqmix - rqrayl) * tgp2 + rqrayl * tgp1;
/*<           ruatm2=(rumix-rurayl)*tgp2+rurayl*tgp1 >*/
	    ruatm2 = (rumix - rurayl) * tgp2 + rurayl * tgp1;
/*<           tdirqu=exp(-(trayp+taerp)*(1./xmuv+1./xmus)) >*/
	    tdirqu = exp(-(trayp + taerp) * (1. / xmuv + 1. / xmus));
/*< 	  rqmeas2=rqatm2+ropq*tgtot*tdirqu >*/
	    rqmeas2 = rqatm2 + ropq * tgtot * tdirqu;
/*< 	  rumeas2=ruatm2+ropu*tgtot*tdirqu >*/
	    rumeas2 = ruatm2 + ropu * tgtot * tdirqu;
/*<           qlumeas=xmus*swl*rqmeas2/pi >*/
	    qlumeas = xmus * swl * rqmeas2 / pi;
/*<           ulumeas=xmus*swl*rumeas2/pi >*/
	    ulumeas = xmus * swl * rumeas2 / pi;
/*< 	  qlumet=qlumet+qlumeas*coefp >*/
	    qlumet += qlumeas * coefp;
/*< 	  ulumet=ulumet+ulumeas*coefp >*/
	    ulumet += ulumeas * coefp;
/*<           foqhsa=foqhsa+qhaa*coef >*/
	    foqhsa += qhaa * coef;
/*<           foqhsr=foqhsr+qhar*coef >*/
	    foqhsr += qhar * coef;
/*<           fouhsa=fouhsa+uhaa*coef >*/
	    fouhsa += uhaa * coef;
/*<           fouhsr=fouhsr+uhar*coef >*/
	    fouhsr += uhar * coef;
/*<           srqray=srqray+rqrayl*coef >*/
	    srqray += rqrayl * coef;
/*<           srqaer=srqaer+rqaero*coef >*/
	    srqaer += rqaero * coef;
/*<           srqtot=srqtot+rqmix*coef >*/
	    srqtot += rqmix * coef;
/*<           sruray=sruray+rurayl*coef >*/
	    sruray += rurayl * coef;
/*<           sruaer=sruaer+ruaero*coef >*/
	    sruaer += ruaero * coef;
/*<           srutot=srutot+rumix*coef >*/
	    srutot += rumix * coef;
/*<           rqfet=rqfet+rqmeas2*coefp >*/
	    rqfet += rqmeas2 * coefp;
/*<           rufet=rufet+rumeas2*coefp >*/
	    rufet += rumeas2 * coefp;
/* Start Update Look up table */
/*< 	do i=1,mu >*/
	    i__3 = mu;
	    for (i__ = 1; i__ <= i__3; ++i__) {
/*< 	do j=1,41 >*/
		for (j = 1; j <= 41; ++j) {
/*< 	roluti(i,j)=roluti(i,j)+rolut(i,j)*coef >*/
		    roluti[i__ + j * 25 - 26] += rolut[i__ + j * 25 - 26] * 
			    coef;
/*< 	rolutiq(i,j)=rolutiq(i,j)+rolutq(i,j)*coef >*/
		    rolutiq[i__ + j * 25 - 26] += rolutq[i__ + j * 25 - 26] * 
			    coef;
/*< 	rolutiu(i,j)=rolutiu(i,j)+rolutu(i,j)*coef >*/
		    rolutiu[i__ + j * 25 - 26] += rolutu[i__ + j * 25 - 26] * 
			    coef;
/*< 	enddo >*/
		}
/*< 	enddo >*/
	    }
/* End Update Look up table */
/*<         endif >*/
	}
/*  ---gazes and other characteritics used in both light */
/*<         srotot=srotot+(romix)*coef >*/
	srotot += romix * coef;
/*<         fophsa=fophsa+phaa*coef >*/
	fophsa += phaa * coef;
/*<         fophsr=fophsr+phar*coef >*/
	fophsr += phar * coef;
/*<         sroray=sroray+rorayl*coef >*/
	sroray += rorayl * coef;
/*<         sroaer=sroaer+roaero*coef >*/
	sroaer += roaero * coef;
/*<         sasr=sasr+asray*coef >*/
	sasr += asray * coef;
/*<         sasa=sasa+asaer*coef >*/
	sasa += asaer * coef;
/*<         sast=sast+astot*coef >*/
	sast += astot * coef;
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
/*  ---output at the ground level. */
/*<         tdir=exp(-(tray+taer)/xmus) >*/
	tdir = exp(-(tray + taer) / xmus);
/*<         tdif=dtott-tdir >*/
	tdif = dtott - tdir;
/*<         etn=dtott*dgtot/(1.-avr*astot) >*/
	etn = dtott * dgtot / (1. - avr * astot);
/*<         esn=tdir*dgtot >*/
	esn = tdir * dgtot;
/*<         es=tdir*dgtot*xmus*swl >*/
	es = tdir * dgtot * xmus * swl;
/*<         ea0n=tdif*dgtot >*/
	ea0n = tdif * dgtot;
/*<         ea0=tdif*dgtot*xmus*swl >*/
	ea0 = tdif * dgtot * xmus * swl;
/*<         ee0n=dgtot*avr*astot*dtott/(1.-avr*astot) >*/
	ee0n = dgtot * avr * astot * dtott / (1. - avr * astot);
/*<         ee0=xmus*swl*dgtot*avr*astot*dtott/(1.-avr*astot) >*/
	ee0 = xmus * swl * dgtot * avr * astot * dtott / (1. - avr * astot);
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
	    ani[0] = 0.;
/*<            ani(1,2)=0. >*/
	    ani[2] = 0.;
/*<            ani(1,3)=0. >*/
	    ani[4] = 0.;
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
/*  ---output at satellite level */
/* old version is commented (new changes are immediately below */
/* Jan-15-2004 */
/*        tmdir=exp(-(tray+taerp)/xmuv) */
/*<         tmdir=exp(-(trayp+taerp)/xmuv) >*/
	tmdir = exp(-(trayp + taerp) / xmuv);
/*<         tmdif=utott-tmdir >*/
	tmdif = utott - tmdir;
/*<         xla0n=ratm2 >*/
	xla0n = ratm2;
/*<         xla0=xla0n*xmus*swl/pi >*/
	xla0 = xla0n * xmus * swl / pi;
/*<         xltn=roc*dtott*tmdir*tgtot/(1.-avr*astot) >*/
	xltn = roc * dtott * tmdir * tgtot / (1. - avr * astot);
/*<         xlt=xltn*xmus*swl/pi >*/
	xlt = xltn * xmus * swl / pi;
/*<         xlen=avr*dtott*tmdif*tgtot/(1.-avr*astot) >*/
	xlen = avr * dtott * tmdif * tgtot / (1. - avr * astot);
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
/*<    51   continue >*/
/* L51: */
    }
/* c---- integrated values of apparent reflectance, radiance          ----
 */
/* c---- and gaseous transmittances (total,downward,separately gases) ----
 */
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
/*<       pizera=0.0 >*/
    pizera = 0.;
/*<       pizerr=1. >*/
    pizerr = 1.;
/*<       if(iaer.ne.0) pizera=ssdaer/sodaer/seb >*/
    if (iaer != 0) {
	pizera = ssdaer / sodaer / seb;
    }
/*<       pizert=(pizerr*sodray+pizera*sodaer)/(sodray+sodaer) >*/
    pizert = (pizerr * sodray + pizera * sodaer) / (sodray + sodaer);
/*<       rfoamave=rfoamave/seb >*/
    rfoamave /= seb;
/*<       rwatave=rwatave/seb >*/
    rwatave /= seb;
/*<       rglitave=rglitave/seb >*/
    rglitave /= seb;
/*<       sroray=sroray/seb >*/
    sroray /= seb;
/*<       sroaer=sroaer/seb >*/
    sroaer /= seb;
/*<       srotot=srotot/seb >*/
    srotot /= seb;
/*<       fophsa=fophsa/seb >*/
    fophsa /= seb;
/*<       fophsr=fophsr/seb >*/
    fophsr /= seb;
/*<       fophst=(sodray*fophsr+sodaer*fophsa)/(sodray+sodaer) >*/
    fophst = (sodray * fophsr + sodaer * fophsa) / (sodray + sodaer);
/*  ---unpolarized light */
/*<         refet=refet/seb >*/
    refet /= seb;
/*<         refet1=refet1/seb >*/
    refet1 /= seb;
/*<         refet2=refet2/seb >*/
    refet2 /= seb;
/*<         refet3=refet3/seb >*/
    refet3 /= seb;
/*<         rog=rog/seb >*/
    rog /= seb;
/*<         alumet=alumet/sb >*/
    alumet /= sb;
/*  ---polarized light */
/*<       if (ipol.ne.0)then >*/
    if (ipol != 0) {
/*< 	rqfet=rqfet/sb >*/
	rqfet /= sb;
/*< 	rufet=rufet/sb >*/
	rufet /= sb;
/*<  	srqray=srqray/seb >*/
	srqray /= seb;
/*<  	srqaer=srqaer/seb >*/
	srqaer /= seb;
/*<  	srqtot=srqtot/seb >*/
	srqtot /= seb;
/*<  	sruray=sruray/seb >*/
	sruray /= seb;
/*<  	sruaer=sruaer/seb >*/
	sruaer /= seb;
/*<  	srutot=srutot/seb >*/
	srutot /= seb;
/*< 	plumet=plumet/sb >*/
	plumet /= sb;
/*<  	foqhsa=foqhsa/seb >*/
	foqhsa /= seb;
/*<  	foqhsr=foqhsr/seb >*/
	foqhsr /= seb;
/*<         foqhst=(sodray*foqhsr+sodaer*foqhsa)/(sodray+sodaer) >*/
	foqhst = (sodray * foqhsr + sodaer * foqhsa) / (sodray + sodaer);
/*<  	fouhsa=fouhsa/seb >*/
	fouhsa /= seb;
/*<  	fouhsr=fouhsr/seb >*/
	fouhsr /= seb;
/*<         fouhst=(sodray*fouhsr+sodaer*fouhsa)/(sodray+sodaer) >*/
	fouhst = (sodray * fouhsr + sodaer * fouhsa) / (sodray + sodaer);
/*      we define the polarized reflectances */
/*< 	srpray=sqrt(srqray**2.+sruray**2.) >*/
	srpray = sqrt(pow_dd(&srqray, &c_b308) + pow_dd(&sruray, &c_b308));
/*<  	srpaer=sqrt(srqaer**2.+sruaer**2.) >*/
	srpaer = sqrt(pow_dd(&srqaer, &c_b308) + pow_dd(&sruaer, &c_b308));
/*< 	srptot=sqrt(srqtot**2.+srutot**2.) >*/
	srptot = sqrt(pow_dd(&srqtot, &c_b308) + pow_dd(&srutot, &c_b308));
/*      we define the primary degrees of polarization */
/*< 	spdpray=foqhsr/fophsr >*/
	spdpray = foqhsr / fophsr;
/*< 	if (iaer.ne.0) then >*/
	if (iaer != 0) {
/*< 	 spdpaer=foqhsa/fophsa >*/
	    spdpaer = foqhsa / fophsa;
/*< 	else >*/
	} else {
/*< 	 spdpaer=0.0 >*/
	    spdpaer = 0.;
/*< 	endif >*/
	}
/*< 	spdptot=foqhst/fophst >*/
	spdptot = foqhst / fophst;
/*      we define the degrees of polarization */
/*< 	sdpray=100.*srpray/sroray >*/
	sdpray = srpray * 100. / sroray;
/*< 	if (sroaer.ne.0) then >*/
	if (sroaer != 0.) {
/*< 	 sdpaer=100.*srpaer/sroaer >*/
	    sdpaer = srpaer * 100. / sroaer;
/*< 	else sdpaer=0.0 >*/
	    elsesdpaer = 0.;
/*< 	endif  >*/
	}
/*< 	sdptot=100.*srptot/srotot >*/
	sdptot = srptot * 100. / srotot;
/*      and we compute the direction of the plane of polarization */
/*< 	call dirpopol(srqray*xmus,sruray*xmus,sdppray) >*/
	d__1 = srqray * xmus;
	d__2 = sruray * xmus;
	dirpopol_(&d__1, &d__2, &sdppray);
/*< 	call dirpopol(srqaer*xmus,sruaer*xmus,sdppaer) >*/
	d__1 = srqaer * xmus;
	d__2 = sruaer * xmus;
	dirpopol_(&d__1, &d__2, &sdppaer);
/*< 	call dirpopol(srqtot*xmus,srutot*xmus,sdpptot) >*/
	d__1 = srqtot * xmus;
	d__2 = srutot * xmus;
	dirpopol_(&d__1, &d__2, &sdpptot);
/* C	ksirad=sdpptot*3.1415927/180. */
/* C	refeti=refet+pinst*rpfet*cos(2*(ksiinst*3.1415925/180.+ksirad)) 
*/
/*<       endif >*/
    }
/*<       do 57 j=1,3 >*/
    for (j = 1; j <= 3; ++j) {
/*  ---output at the ground level. */
/*<         aini(1,j)=aini(1,j)/seb >*/
	aini[(j << 1) - 2] /= seb;
/*<         aini(2,j)=aini(2,j)/sb >*/
	aini[(j << 1) - 1] /= sb;
/*  ---output at satellite level */
/*<         ainr(1,j)=ainr(1,j)/seb >*/
	ainr[(j << 1) - 2] /= seb;
/*<         ainr(2,j)=ainr(2,j)/sb >*/
	ainr[(j << 1) - 1] /= sb;
/*<    57 continue >*/
/* L57: */
    }
/* _otb_adaptation Beginning: Atmospheric reflectance storage */
/*<       otb_ratm   = ainr(1,1)	!Added_for_OTB : atmospheric reflectance >*/
    *otb_ratm__ = ainr[0];
/*<       otb_sast   = sast		!Added_for_OTB : atmospheric spherical albedo >*/
    *otb_sast__ = sast;
/*<       otb_tgasm  = tgasm	!Added_for_OTB : total gaseous transmissio >*/
    *otb_tgasm__ = tgasm;
/*<       otb_sdtott = sdtott	!Added_for_OTB : downward transmittance >*/
    *otb_sdtott__ = sdtott;
/*<       otb_sutott = sutott	!Added_for_OTB : upward transmittance >*/
    *otb_sutott__ = sutott;
/* Added_for_OTB : upward directe transmittance */
/*<       otb_tdir_up = exp(-sodtot/xmuv)     >*/
    *otb_tdir_up__ = exp(-sodtot / xmuv);
/* Added_for_OTB : upward diffuse transmittance */
/*<       otb_tdif_up = otb_sutott - otb_tdir_up  >*/
    *otb_tdif_up__ = *otb_sutott__ - *otb_tdir_up__;
/* Added_for_OTB : upward diffuse transmittance for Rayleigh */
/*<       otb_tdif_up_ray = sutotr - exp(-sodray/xmuv) >*/
    *otb_tdif_up_ray__ = sutotr - exp(-sodray / xmuv);
/* Added_for_OTB : upward diffuse transmittance for aerosols */
/*<       otb_tdif_up_aer = sutota - exp(-sodaer/xmuv)    >*/
    *otb_tdif_up_aer__ = sutota - exp(-sodaer / xmuv);
/* _otb_adaptation End : Atmospheric reflectance storage */
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                       print of final results                         c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/* _otb_adaptation Beginning: go to end of code */
/*<       goto 9999	!Added_for_OTB >*/
    goto L9999;
/* _otb_adaptation End : go to end of code */
/* begining case for a lut output */
/* SIMPLE LUT in azimuth */
/*<       if (ilut.eq.2) then >*/
    if (ilut == 2) {
/*<           do ifi=1,nfi >*/
	i__1 = nfi;
	for (ifi = 1; ifi <= i__1; ++ifi) {
/*< 	  xtphi=(ifi-1)*180.0/(nfi-1) >*/
	    xtphi = (ifi - 1) * 180. / (nfi - 1);
/*< 	  write(6,*) "lutfi ",xtphi,ratm2_fi(ifi) >*/
	    s_wsle(&io___426);
	    do_lio(&c__9, &c__1, "lutfi ", 6L);
	    do_lio(&c__5, &c__1, (char *)&xtphi, (ftnlen)sizeof(doublereal));
	    do_lio(&c__5, &c__1, (char *)&ratm2_fi__[ifi - 1], (ftnlen)sizeof(
		    doublereal));
	    e_wsle();
/*< 	  enddo >*/
	}
/*<       endif	   >*/
    }
/* LUT FOR Look up table data */
/*<       if (ilut.eq.1) then >*/
    if (ilut == 1) {
/*<       its=acos(xmus)*180.0/pi >*/
	its = acos(xmus) * 180. / pi;
/*<       open(10,file='rotoa_bs',ACCESS='APPEND') >*/
	o__1.oerr = 0;
	o__1.ounit = 10;
	o__1.ofnmlen = 8;
	o__1.ofnm = "rotoa_bs";
	o__1.orl = 0;
	o__1.osta = 0;
	o__1.oacc = "APPEND";
	o__1.ofm = 0;
	o__1.oblnk = 0;
	f_open(&o__1);
/*<       write(10,2222) "AERO-LUT Lambda min,max ",wlinf,wlsup >*/
	s_wsfe(&io___427);
	do_fio(&c__1, "AERO-LUT Lambda min,max ", 24L);
	do_fio(&c__1, (char *)&sixs_ffu__1.wlinf, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sixs_ffu__1.wlsup, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<  2222 Format(A28,3(F10.7,1X))       >*/
/*<       write(10,2222) "Tau-Lambda,Tau550 asol  ",sodaer,taer55,asol >*/
	s_wsfe(&io___428);
	do_fio(&c__1, "Tau-Lambda,Tau550 asol  ", 24L);
	do_fio(&c__1, (char *)&sodaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&taer55, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&asol, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       aerod=0 >*/
	aerod = 0;
/*<       if (iaer.eq.12) then >*/
	if (iaer == 12) {
/*<       write(10,2223) "aerosol model ",FILE2(1:i2) >*/
	    s_wsfe(&io___430);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, file2, i2);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.1) then >*/
	if (iaer == 1) {
/*<       write(10,2223) "aerosol model ","CONTINENTAL" >*/
	    s_wsfe(&io___431);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "CONTINENTAL", 11L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.2) then >*/
	if (iaer == 2) {
/*<       write(10,2223) "aerosol model ","MARITIME" >*/
	    s_wsfe(&io___432);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "MARITIME", 8L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.3) then >*/
	if (iaer == 3) {
/*<       write(10,2223) "aerosol model ","URBAN" >*/
	    s_wsfe(&io___433);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "URBAN", 5L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.5) then >*/
	if (iaer == 5) {
/*<       write(10,2223) "aerosol model ","DESERTIC" >*/
	    s_wsfe(&io___434);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "DESERTIC", 8L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.6) then >*/
	if (iaer == 6) {
/*<       write(10,2223) "aerosol model ","SMOKE" >*/
	    s_wsfe(&io___435);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "SMOKE", 5L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.7) then >*/
	if (iaer == 7) {
/*<       write(10,2223) "aerosol model ","STRATOSPHERIC" >*/
	    s_wsfe(&io___436);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "STRATOSPHERIC", 13L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (aerod.eq.0) then >*/
	if (aerod == 0) {
/*<       write(10,2223) "aerosol model ","UNDEFINED" >*/
	    s_wsfe(&io___437);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "UNDEFINED", 9L);
	    e_wsfe();
/*<       endif >*/
	}
/*<  2223 format(A24,1X,A80)       >*/
/*<       lutmuv=cos(avis*pi/180.) >*/
	lutmuv = cos(avis * pi / 180.);
/*<    >*/
	cscaa = -xmus * lutmuv - cos(filut[mu - 1] * pi / 180.) * sqrt(1. - 
		xmus * xmus) * sqrt(1. - lutmuv * lutmuv);
/*<       iscama=acos(cscaa)*180./pi >*/
	iscama = acos(cscaa) * 180. / pi;
/*<    >*/
	cscaa = -xmus * lutmuv - cos(filut[mu + nfilut[mu - 1] * 25 - 26] * 
		pi / 180.) * sqrt(1. - xmus * xmus) * sqrt(1. - lutmuv * 
		lutmuv);
/*<       iscami=acos(cscaa)*180./pi >*/
	iscami = acos(cscaa) * 180. / pi;
/*<       write(10,333) its,avis,nfilut(mu),iscama,iscami >*/
	s_wsfe(&io___438);
	do_fio(&c__1, (char *)&its, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&avis, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&nfilut[mu - 1], (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&iscama, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&iscami, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       write(10,'(41(F8.5,1X))')(roluti(mu,j)/seb,j=1,nfilut(mu)) >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(41(F8.5,1X))";
	s_wsfe(&ci__1);
	i__1 = nfilut[mu - 1];
	for (j = 1; j <= i__1; ++j) {
	    d__1 = roluti[mu + j * 25 - 26] / seb;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
/*      write(10,'(41(F8.5,1X))')(rolutiq(mu,j)/seb,j=1,nfilut(mu)) */
/*      write(10,'(41(F8.5,1X))')(rolutiu(mu,j)/seb,j=1,nfilut(mu)) */
/*<       do i=1,mu-1 >*/
	i__1 = mu - 1;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*<       lutmuv=rm(i) >*/
	    lutmuv = rm[i__ + 25];
/*<       luttv=acos(lutmuv)*180./pi >*/
	    luttv = acos(lutmuv) * 180. / pi;
/*<    >*/
	    cscaa = -xmus * lutmuv - cos(filut[i__ - 1] * pi / 180.) * sqrt(
		    1. - xmus * xmus) * sqrt(1. - lutmuv * lutmuv);
/*<       iscama=acos(cscaa)*180./pi >*/
	    iscama = acos(cscaa) * 180. / pi;
/*<    >*/
	    cscaa = -xmus * lutmuv - cos(filut[i__ + nfilut[i__ - 1] * 25 - 
		    26] * pi / 180.) * sqrt(1. - xmus * xmus) * sqrt(1. - 
		    lutmuv * lutmuv);
/*<       iscami=acos(cscaa)*180./pi >*/
	    iscami = acos(cscaa) * 180. / pi;
/*<       write(10,333) its,luttv,nfilut(i),iscama,iscami >*/
	    s_wsfe(&io___439);
	    do_fio(&c__1, (char *)&its, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&luttv, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&nfilut[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&iscama, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&iscami, (ftnlen)sizeof(doublereal));
	    e_wsfe();
/*<  333  Format(F10.5,1X,F10.5,1X,I3,F10.5,F10.5)     >*/
/*<       write(10,'(41(F8.5,1X))')(roluti(i,j)/seb,j=1,nfilut(i)) >*/
	    ci__1.cierr = 0;
	    ci__1.ciunit = 10;
	    ci__1.cifmt = "(41(F8.5,1X))";
	    s_wsfe(&ci__1);
	    i__3 = nfilut[i__ - 1];
	    for (j = 1; j <= i__3; ++j) {
		d__1 = roluti[i__ + j * 25 - 26] / seb;
		do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
/*      write(10,'(41(F8.5,1X))')(rolutiq(i,j)/seb,j=1,nfilut(i)) 
*/
/*      write(10,'(41(F8.5,1X))')(rolutiu(i,j)/seb,j=1,nfilut(i)) 
*/
/*<       enddo >*/
	}
/*<       close(10) >*/
	cl__1.cerr = 0;
	cl__1.cunit = 10;
	cl__1.csta = 0;
	f_clos(&cl__1);
/*<       endif >*/
    }
/* Case a LUT output is desired */
/* Case for an aps LUT */
/*<       if (ilut.eq.3) then >*/
    if (ilut == 3) {
/*<       its=acos(xmus)*180.0/pi >*/
	its = acos(xmus) * 180. / pi;
/*<       open(10,file='rotoa_aps_bs',ACCESS='APPEND') >*/
	o__1.oerr = 0;
	o__1.ounit = 10;
	o__1.ofnmlen = 12;
	o__1.ofnm = "rotoa_aps_bs";
	o__1.orl = 0;
	o__1.osta = 0;
	o__1.oacc = "APPEND";
	o__1.ofm = 0;
	o__1.oblnk = 0;
	f_open(&o__1);
/*<       write(10,2222) "AERO-LUT Lambda min,max ",wlinf,wlsup >*/
	s_wsfe(&io___440);
	do_fio(&c__1, "AERO-LUT Lambda min,max ", 24L);
	do_fio(&c__1, (char *)&sixs_ffu__1.wlinf, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sixs_ffu__1.wlsup, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       write(10,2222) "Tau-Lambda,Tau550 asol  ",sodaer,taer55,asol >*/
	s_wsfe(&io___441);
	do_fio(&c__1, "Tau-Lambda,Tau550 asol  ", 24L);
	do_fio(&c__1, (char *)&sodaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&taer55, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&asol, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       aerod=0 >*/
	aerod = 0;
/*<       if (iaer.eq.12) then >*/
	if (iaer == 12) {
/*<       write(10,2223) "aerosol model ",FILE2(1:i2) >*/
	    s_wsfe(&io___442);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, file2, i2);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.1) then >*/
	if (iaer == 1) {
/*<       write(10,2223) "aerosol model ","CONTINENTAL" >*/
	    s_wsfe(&io___443);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "CONTINENTAL", 11L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.2) then >*/
	if (iaer == 2) {
/*<       write(10,2223) "aerosol model ","MARITIME" >*/
	    s_wsfe(&io___444);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "MARITIME", 8L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.3) then >*/
	if (iaer == 3) {
/*<       write(10,2223) "aerosol model ","URBAN" >*/
	    s_wsfe(&io___445);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "URBAN", 5L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.5) then >*/
	if (iaer == 5) {
/*<       write(10,2223) "aerosol model ","DESERTIC" >*/
	    s_wsfe(&io___446);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "DESERTIC", 8L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.6) then >*/
	if (iaer == 6) {
/*<       write(10,2223) "aerosol model ","SMOKE" >*/
	    s_wsfe(&io___447);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "SMOKE", 5L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (iaer.eq.7) then >*/
	if (iaer == 7) {
/*<       write(10,2223) "aerosol model ","STRATOSPHERIC" >*/
	    s_wsfe(&io___448);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "STRATOSPHERIC", 13L);
	    e_wsfe();
/*<       aerod=1 >*/
	    aerod = 1;
/*<       endif >*/
	}
/*<       if (aerod.eq.0) then >*/
	if (aerod == 0) {
/*<       write(10,2223) "aerosol model ","UNDEFINED" >*/
	    s_wsfe(&io___449);
	    do_fio(&c__1, "aerosol model ", 14L);
	    do_fio(&c__1, "UNDEFINED", 9L);
	    e_wsfe();
/*<       endif >*/
	}

/*<       dtr=atan(1.)*4./180. >*/
	dtr = atan(1.) * 4. / 180.;
/*<    >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(A5,1X,41(F8.4,1X))";
	s_wsfe(&ci__1);
	do_fio(&c__1, "phi", 3L);
	for (i__ = 16; i__ >= 1; --i__) {
	    do_fio(&c__1, (char *)&filut[i__ - 1], (ftnlen)sizeof(doublereal))
		    ;
	}
	do_fio(&c__1, (char *)&filut[mu - 1], (ftnlen)sizeof(doublereal));
	for (i__ = 1; i__ <= 16; ++i__) {
	    do_fio(&c__1, (char *)&filut[i__ + 24], (ftnlen)sizeof(doublereal)
		    );
	}
	e_wsfe();
/*<    >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(A5,1X,41(F8.5,1X))";
	s_wsfe(&ci__1);
	do_fio(&c__1, "tv", 2L);
	for (i__ = 16; i__ >= 1; --i__) {
	    d__1 = acos(rm[i__ + 25]) / dtr;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	}
	d__2 = acos(rm[25]) / dtr;
	do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
	for (k = 1; k <= 16; ++k) {
	    d__3 = acos(rm[k + 25]) / dtr;
	    do_fio(&c__1, (char *)&d__3, (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
/*<    >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(41(F8.5,1X))";
	s_wsfe(&ci__1);
	for (i__ = 16; i__ >= 1; --i__) {
	    d__1 = roluti[i__ - 1] / seb;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	}
	d__2 = roluti[mu - 1] / seb;
	do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
	for (i__ = 1; i__ <= 16; ++i__) {
	    d__3 = roluti[i__ + 24] / seb;
	    do_fio(&c__1, (char *)&d__3, (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
/*<    >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(41(F8.5,1X))";
	s_wsfe(&ci__1);
	for (i__ = 16; i__ >= 1; --i__) {
	    d__1 = rolutiq[i__ - 1] / seb;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	}
	d__2 = rolutiq[mu - 1] / seb;
	do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
	for (i__ = 1; i__ <= 16; ++i__) {
	    d__3 = rolutiq[i__ + 24] / seb;
	    do_fio(&c__1, (char *)&d__3, (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
/*<    >*/
	ci__1.cierr = 0;
	ci__1.ciunit = 10;
	ci__1.cifmt = "(41(F8.5,1X))";
	s_wsfe(&ci__1);
	for (i__ = 16; i__ >= 1; --i__) {
	    d__1 = rolutiu[i__ - 1] / seb;
	    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	}
	d__2 = rolutiu[mu - 1] / seb;
	do_fio(&c__1, (char *)&d__2, (ftnlen)sizeof(doublereal));
	for (i__ = 1; i__ <= 16; ++i__) {
	    d__3 = rolutiu[i__ + 24] / seb;
	    do_fio(&c__1, (char *)&d__3, (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
/*<       close(10) >*/
	cl__1.cerr = 0;
	cl__1.cunit = 10;
	cl__1.csta = 0;
	f_clos(&cl__1);
/*<       endif >*/
    }
/* Case a LUT output is desired */
/*<  160  continue >*/
/* L160: */
/*<         write(iwr, 430 )refet,alumet,tgasm >*/
    io___451.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___451);
    do_fio(&c__1, (char *)&refet, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&alumet, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&tgasm, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<         write(iwr, 431 )refet1,refet2,refet3 >*/
    io___452.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___452);
    do_fio(&c__1, (char *)&refet1, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&refet2, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&refet3, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       if (ipol.eq.1)then >*/
    if (ipol == 1) {
/*<         rpfet=sqrt(rqfet*rqfet+rufet*rufet) >*/
	rpfet = sqrt(rqfet * rqfet + rufet * rufet);
/*< 	plumet=sqrt(qlumet*qlumet+ulumet*ulumet) >*/
	plumet = sqrt(qlumet * qlumet + ulumet * ulumet);
/*< 	xpol=atan2(rufet,rqfet)*180.0/3.14159/2. >*/
	xpol = atan2(rufet, rqfet) * 180. / 3.14159 / 2.;
/*<         write(iwr, 429 )rpfet,plumet,xpol,rpfet/refet >*/
	io___454.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___454);
	do_fio(&c__1, (char *)&rpfet, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&plumet, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xpol, (ftnlen)sizeof(doublereal));
	d__1 = rpfet / refet;
	do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*       write(iwr, 428 )rpfet1,rpfet2,rpfet3 */
/*<       endif >*/
    }
/*<         if(inhomo.ne.0) then >*/
    if (inhomo != 0) {
/*<    >*/
	io___455.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___455);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 2], (ftnlen)sizeof(
		    doublereal));
	}
	do_fio(&c__1, "environment", 11L);
	do_fio(&c__1, "target", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 2], (ftnlen)sizeof(
		    doublereal));
	}
	e_wsfe();
/*<    >*/
	io___456.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___456);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 1], (ftnlen)sizeof(
		    doublereal));
	}
	do_fio(&c__1, "environment", 11L);
	do_fio(&c__1, "target", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 1], (ftnlen)sizeof(
		    doublereal));
	}
	e_wsfe();
/*<         endif >*/
    }
/*<         if(inhomo.eq.0) then >*/
    if (inhomo == 0) {
/*<    >*/
	io___457.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___457);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 2], (ftnlen)sizeof(
		    doublereal));
	}
	do_fio(&c__1, "background ", 11L);
	do_fio(&c__1, "pixel ", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 2], (ftnlen)sizeof(
		    doublereal));
	}
	e_wsfe();
/*<    >*/
	io___458.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___458);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&aini[(j << 1) - 1], (ftnlen)sizeof(
		    doublereal));
	}
	do_fio(&c__1, "background ", 11L);
	do_fio(&c__1, "pixel ", 6L);
	for (j = 1; j <= 3; ++j) {
	    do_fio(&c__1, (char *)&ainr[(j << 1) - 1], (ftnlen)sizeof(
		    doublereal));
	}
	e_wsfe();
/*<         endif >*/
    }
/*<       if (iwave.eq.-1)then >*/
    if (iwave == -1) {
/*<         write(iwr, 436)seb >*/
	io___459.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___459);
	do_fio(&c__1, (char *)&seb, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       else >*/
    } else {
/*<         write(iwr, 437)sb,seb >*/
	io___460.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___460);
	do_fio(&c__1, (char *)&sb, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&seb, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       endif >*/
    }
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                    print of complementary results                    c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<       write(iwr, 929) >*/
    io___461.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___461);
    e_wsfe();
/*<       write(iwr, 930) >*/
    io___462.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___462);
    e_wsfe();
/*<       write(iwr, 931)'global gas. trans. :',dgasm,ugasm,tgasm >*/
    io___463.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___463);
    do_fio(&c__1, "global gas. trans. :", 20L);
    do_fio(&c__1, (char *)&dgasm, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&ugasm, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&tgasm, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'water   "     "    :',sdwava,suwava,stwava >*/
    io___464.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___464);
    do_fio(&c__1, "water   \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdwava, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&suwava, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&stwava, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'ozone   "     "    :',sdozon,suozon,stozon >*/
    io___465.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___465);
    do_fio(&c__1, "ozone   \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdozon, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&suozon, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&stozon, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'co2     "     "    :',sddica,sudica,stdica >*/
    io___466.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___466);
    do_fio(&c__1, "co2     \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sddica, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sudica, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&stdica, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'oxyg    "     "    :',sdoxyg,suoxyg,stoxyg >*/
    io___467.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___467);
    do_fio(&c__1, "oxyg    \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdoxyg, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&suoxyg, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&stoxyg, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'no2     "     "    :',sdniox,suniox,stniox >*/
    io___468.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___468);
    do_fio(&c__1, "no2     \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdniox, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&suniox, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&stniox, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'ch4     "     "    :',sdmeth,sumeth,stmeth >*/
    io___469.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___469);
    do_fio(&c__1, "ch4     \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdmeth, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sumeth, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&stmeth, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'co      "     "    :',sdmoca,sumoca,stmoca >*/
    io___470.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___470);
    do_fio(&c__1, "co      \"     \"    :", 20L);
    do_fio(&c__1, (char *)&sdmoca, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sumoca, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&stmoca, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___471.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___471);
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___472.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___472);
    e_wsfe();
/*<       write(iwr, 931)'rayl.  sca. trans. :',sdtotr,sutotr,sutotr*sdtotr >*/
    io___473.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___473);
    do_fio(&c__1, "rayl.  sca. trans. :", 20L);
    do_fio(&c__1, (char *)&sdtotr, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sutotr, (ftnlen)sizeof(doublereal));
    d__1 = sutotr * sdtotr;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'aeros. sca.   "    :',sdtota,sutota,sutota*sdtota >*/
    io___474.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___474);
    do_fio(&c__1, "aeros. sca.   \"    :", 20L);
    do_fio(&c__1, (char *)&sdtota, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sutota, (ftnlen)sizeof(doublereal));
    d__1 = sutota * sdtota;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'total  sca.   "    :',sdtott,sutott,sutott*sdtott >*/
    io___475.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___475);
    do_fio(&c__1, "total  sca.   \"    :", 20L);
    do_fio(&c__1, (char *)&sdtott, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sutott, (ftnlen)sizeof(doublereal));
    d__1 = sutott * sdtott;
    do_fio(&c__1, (char *)&d__1, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___476.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___476);
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___477.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___477);
    e_wsfe();
/*<       write(iwr, 939) >*/
    io___478.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___478);
    e_wsfe();
/*<       write(iwr, 931)'spherical albedo   :',sasr,sasa,sast >*/
    io___479.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___479);
    do_fio(&c__1, "spherical albedo   :", 20L);
    do_fio(&c__1, (char *)&sasr, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sasa, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sast, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'optical depth total:',sodray,sodaer,sodtot >*/
    io___480.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___480);
    do_fio(&c__1, "optical depth total:", 20L);
    do_fio(&c__1, (char *)&sodray, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sodaer, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sodtot, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 931)'optical depth plane:',sodrayp,sodaerp,sodtotp >*/
    io___481.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___481);
    do_fio(&c__1, "optical depth plane:", 20L);
    do_fio(&c__1, (char *)&sodrayp, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sodaerp, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&sodtotp, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       if (ipol.eq.0) then >*/
    if (ipol == 0) {
/*<         write(iwr, 931)'reflectance        :',sroray,sroaer,srotot >*/
	io___482.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___482);
	do_fio(&c__1, "reflectance        :", 20L);
	do_fio(&c__1, (char *)&sroray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sroaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&srotot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 931)'phase function     :',fophsr,fophsa,fophst >*/
	io___483.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___483);
	do_fio(&c__1, "phase function     :", 20L);
	do_fio(&c__1, (char *)&fophsr, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&fophsa, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&fophst, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       else  >*/
    } else {
/*<         write(iwr, 931)'reflectance I      :',sroray,sroaer,srotot >*/
	io___484.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___484);
	do_fio(&c__1, "reflectance I      :", 20L);
	do_fio(&c__1, (char *)&sroray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sroaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&srotot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 931)'reflectance Q      :',srqray,srqaer,srqtot >*/
	io___485.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___485);
	do_fio(&c__1, "reflectance Q      :", 20L);
	do_fio(&c__1, (char *)&srqray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&srqaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&srqtot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 931)'reflectance U      :',sruray,sruaer,srutot >*/
	io___486.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___486);
	do_fio(&c__1, "reflectance U      :", 20L);
	do_fio(&c__1, (char *)&sruray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sruaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&srutot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 931)'polarized reflect. :',srpray,srpaer,srptot >*/
	io___487.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___487);
	do_fio(&c__1, "polarized reflect. :", 20L);
	do_fio(&c__1, (char *)&srpray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&srpaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&srptot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 932)'degree of polar.   :',sdpray,sdpaer,sdptot >*/
	io___488.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___488);
	do_fio(&c__1, "degree of polar.   :", 20L);
	do_fio(&c__1, (char *)&sdpray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sdpaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sdptot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 932)'dir. plane polar.  :',sdppray,sdppaer,sdpptot >*/
	io___489.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___489);
	do_fio(&c__1, "dir. plane polar.  :", 20L);
	do_fio(&c__1, (char *)&sdppray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sdppaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&sdpptot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/* CC	write(iwr, 931)'instrument app ref.:',zero,zero,refeti */
/*<         write(iwr, 931)'phase function I   :',fophsr,fophsa,fophst >*/
	io___490.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___490);
	do_fio(&c__1, "phase function I   :", 20L);
	do_fio(&c__1, (char *)&fophsr, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&fophsa, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&fophst, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 931)'phase function Q   :',foqhsr,foqhsa,foqhst >*/
	io___491.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___491);
	do_fio(&c__1, "phase function Q   :", 20L);
	do_fio(&c__1, (char *)&foqhsr, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&foqhsa, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&foqhst, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 931)'phase function U   :',fouhsr,fouhsa,fouhst >*/
	io___492.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___492);
	do_fio(&c__1, "phase function U   :", 20L);
	do_fio(&c__1, (char *)&fouhsr, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&fouhsa, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&fouhst, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<         write(iwr, 931)'primary deg. of pol:',spdpray,spdpaer,spdptot >*/
	io___493.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___493);
	do_fio(&c__1, "primary deg. of pol:", 20L);
	do_fio(&c__1, (char *)&spdpray, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&spdpaer, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&spdptot, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<       endif >*/
    }
/*<       write(iwr, 931)'sing. scat. albedo :',pizerr,pizera,pizert >*/
    io___494.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___494);
    do_fio(&c__1, "sing. scat. albedo :", 20L);
    do_fio(&c__1, (char *)&pizerr, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&pizera, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&pizert, (ftnlen)sizeof(doublereal));
    e_wsfe();
/*<       write(iwr, 1401) >*/
    io___495.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___495);
    e_wsfe();
/*<       write(iwr, 1402) >*/
    io___496.ciunit = sixs_ier__1.iwr;
    s_wsfe(&io___496);
    e_wsfe();
/* **********************************************************************c
 */
/*                                                                      c 
*/
/*                    atmospheric correction                            c 
*/
/*                                                                      c 
*/
/* **********************************************************************c
 */
/*<        if (irapp.ge.0) then >*/
    if (irapp >= 0) {
/*< 	 if (rapp.ge.0.) then >*/
	if (rapp >= 0.) {
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
	rog /= rog * sast + 1.;
/*< 	 xa=pi*sb/xmus/seb/tgasm/sutott/sdtott >*/
	xa = pi * sb / xmus / seb / tgasm / sutott / sdtott;
/*< 	 xap=1./tgasm/sutott/sdtott >*/
	xap = 1. / tgasm / sutott / sdtott;
/*< 	 xb=ainr(1,1)/sutott/sdtott/tgasm >*/
	xb = ainr[0] / sutott / sdtott / tgasm;
/*< 	 xb=ainr(1,1)/sutott/sdtott/tgasm >*/
	xb = ainr[0] / sutott / sdtott / tgasm;
/*< 	 xc=sast >*/
	xc = sast;
/*        BRDF coupling correction */
/*<          if (idirec.eq.1) then  >*/
	if (idirec == 1) {
/* compute the coefficient of the 2nd degree equation */
/*  a*(ros^2)+b*ros+c */
/*   compute ratios and transmissions */
/*<          robarstar=(robar1/xnorm1)/brdfints(mu,1) >*/
	    robarstar = robar1 / xnorm1 / brdfints[mu + 25];
/*< 	 robarpstar=(robar2/xnorm2)/brdfints(mu,1) >*/
	    robarpstar = robar2 / xnorm2 / brdfints[mu + 25];
/*< 	 robarbarstar=albbrdf/brdfints(mu,1) >*/
	    robarbarstar = albbrdf / brdfints[mu + 25];
/*< 	 tdd=exp(-sodtot/xmus) >*/
	    tdd = exp(-sodtot / xmus);
/*< 	 tdu=exp(-sodtot/xmuv) >*/
	    tdu = exp(-sodtot / xmuv);
/*< 	 tsd=sdtott-tdd >*/
	    tsd = sdtott - tdd;
/*< 	 tsu=sutott-tdu >*/
	    tsu = sutott - tdu;
/* compute coefficients */
/*< 	 coefc=-(rapp/tgasm-ainr(1,1)/tgasm) >*/
	    coefc = -(rapp / tgasm - ainr[0] / tgasm);
/*< 	 coefb=tdd*tdu+tdu*tsd*robarstar+tsu*tdd*robarpstar >*/
	    coefb = tdd * tdu + tdu * tsd * robarstar + tsu * tdd * 
		    robarpstar;
/*< 	 coefb=coefb+tsu*tsd*robarbarstar >*/
	    coefb += tsu * tsd * robarbarstar;
/*< 	 coefa=sdtott*sutott*sast*robarbarstar*robarbarstar >*/
	    coefa = sdtott * sutott * sast * robarbarstar * robarbarstar;
/*< 	 coefa=coefa/(1-sast*(rog/brdfints(mu,1)*albbrdf)) >*/
	    coefa /= 1 - sast * (rog / brdfints[mu + 25] * albbrdf);
/* solve equations, compute solutions */
/*<          discri=sqrt(coefb*coefb-4*coefa*coefc) >*/
	    discri = sqrt(coefb * coefb - coefa * 4 * coefc);
/*<          rogbrdf=(-coefb+discri)/(2*coefa) >*/
	    rogbrdf = (-coefb + discri) / (coefa * 2);
/* second pass use update value for rog */
/*<          coefa=sdtott*sutott*sast*robarbarstar*robarbarstar   >*/
	    coefa = sdtott * sutott * sast * robarbarstar * robarbarstar;
/*<          coefa=coefa/(1-sast*(rogbrdf/brdfints(mu,1)*albbrdf)) >*/
	    coefa /= 1 - sast * (rogbrdf / brdfints[mu + 25] * albbrdf);
/*<          discri=sqrt(coefb*coefb-4*coefa*coefc) >*/
	    discri = sqrt(coefb * coefb - coefa * 4 * coefc);
/*<          rogbrdf=(-coefb+discri)/(2*coefa) >*/
	    rogbrdf = (-coefb + discri) / (coefa * 2);
/*< 	 else >*/
	} else {
/*< 	 rogbrdf=rog >*/
	    rogbrdf = rog;
/*< 	 endif >*/
	}
/*<          write(iwr, 940) >*/
	io___520.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___520);
	e_wsfe();
/*<          write(iwr, 941)rapp >*/
	io___521.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___521);
	do_fio(&c__1, (char *)&rapp, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*<          write(iwr, 942)xrad >*/
	io___522.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___522);
	do_fio(&c__1, (char *)&xrad, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*< 	 if (irapp.eq.0) then   >*/
	if (irapp == 0) {
/*<          write(iwr, 943)rog >*/
	    io___523.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___523);
	    do_fio(&c__1, (char *)&rog, (ftnlen)sizeof(doublereal));
	    e_wsfe();
/*<          write(iwr, 944)xa,xb,xc >*/
	    io___524.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___524);
	    do_fio(&c__1, (char *)&xa, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&xb, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&xc, (ftnlen)sizeof(doublereal));
	    e_wsfe();
/*< 	 else >*/
	} else {
/*< 	 write(iwr,222)rog,rogbrdf >*/
	    io___525.ciunit = sixs_ier__1.iwr;
	    s_wsfe(&io___525);
	    do_fio(&c__1, (char *)&rog, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&rogbrdf, (ftnlen)sizeof(doublereal));
	    e_wsfe();
/*< 	 endif >*/
	}
/*<          write(iwr, 944)xa,xb,xc >*/
	io___526.ciunit = sixs_ier__1.iwr;
	s_wsfe(&io___526);
	do_fio(&c__1, (char *)&xa, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xb, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&xc, (ftnlen)sizeof(doublereal));
	e_wsfe();
/*         write(iwr, *) "david roy ", xap,xb,xc */
/*<          y=xa*xrad-xb >*/
	y = xa * xrad - xb;
/*        write(6,'(A5,F9.5)') 'rog=', rog */
/*        write(6,'(A5,F9.5,A8,F9.5)') 'y=',y, '  acr=',y/(1.+xc*y) */
/*        write(6,*) 'rogbrdf=',rogbrdf,' rodir=',brdfints(mu,1), */
/*    s            ' diff=',rogbrdf-brdfints(mu,1) */
/*<       endif >*/
    }
/* _otb_adaptation Beginning: */
/*< 9999  return	!Added_for_OTB	 >*/
L9999:
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
/*<  5550 format(1h*,10x,25h aerosols type identity :,t79,1h*) >*/
/*<  5 >*/
/*<  5 >*/
/*<  5553 format(1h*,15x,I2,1x,f10.1,13x,f5.3,15x,A15,t79,1h*) >*/
/*<  5554 format(1h*,15x,20hno aerosols computed,t79,1h*)  >*/
/*<  5555 format(1h*,t79,1h*)  >*/
/*<  132  format(1h*,15x,a30,t79,1h*)          >*/
/*<  1 >*/
/*<  1 >*/
/*<  135  format(1h*,t19,f6.4,T33,f5.3,T47,e8.3,T79,1h*) >*/
/*<  1 >*/
/*<  1 >*/
/*<  1 >*/
/*<  1 >*/
/*<    >*/
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
/* L168: */
/*<    >*/
/*<   170 format(1h*,15x,22h target reflectance : ,t79,1h*) >*/
/*<   171 format(1h*,15x,29h environmental reflectance : ,t79,1h*) >*/
/*<   172 format(1h*,t79,1h*,/,79(1h*),///) >*/
/* L172: */
/*<    >*/
/* L173: */
/*<   174 format(1h*,10x,22h ground pressure  [mb]    ,1x,f7.2,1x,t79,1h*) >*/
/* L174: */
/*<   175 format(1h*,10x,22h ground altitude  [km]    ,f6.3,1x,t79,1h*) >*/
/* L175: */
/*<    >*/
/* L176: */
/*<    >*/
/* L177: */
/*<   220 format(1h*,23x,34h Lambertian assumption  selected  ,t79,1h*) >*/
/* L220: */
/*<   221 format(1h*,23x,34h BRDF coupling correction         ,t79,1h*) >*/
/* L221: */
/*<   185 format(1h*,10x,30h input apparent reflectance : , f6.3,t79,1h*) >*/
/* L185: */
/*<    >*/
/* L186: */
/*<    >*/
/* L187: */
/*<   190 format(1h*,15x,31h brdf from in-situ measurements,t79,1h*) >*/
/* L190: */
/*<    >*/
/* L191: */
/*<    >*/
/* L192: */
/*<    >*/
/* L193: */
/*<    >*/
/* L194: */
/*<    >*/
/* L195: */
/*<    >*/
/* L196: */
/*<    >*/
/* L197: */
/*<    >*/
/* L198: */
/*<    >*/
/* L199: */
/*<    >*/
/* L200: */
/*<    >*/
/* L201: */
/*<    >*/
/* L202: */
/*<    >*/
/* L203: */
/*<    >*/
/* L204: */
/*<    >*/
/* L205: */
/*<    >*/
/* L206: */
/*<   207 format(1h*,15x,A19,t79,1h*) >*/
/* L207: */
/*<   208 format(1h*,15x,A19,1x,f5.2,t79,1h*) >*/
/* L208: */
/*<   209 format(1h*,15x,A31,t79,1h*) >*/
/* L209: */
/*<    >*/
/* L210: */
/*<    >*/
/* L211: */
/* pressure at ground level (174) and altitude (175) */
/*<    >*/
/* L178: */
/*<   179 format(1h*,10x,31h plane  pressure          [mb] ,f7.2,1x,t79,1h*) >*/
/* L179: */
/*<   180 format(1h*,10x,31h plane  altitude absolute [km] ,f6.3,1x,t79,1h*) >*/
/* L180: */
/*<   181 format(1h*,15x,37h atmosphere under plane description: ,t79,1h*) >*/
/* L181: */
/*<   182 format(1h*,15x,26h ozone content            ,f6.3,1x,t79,1h*) >*/
/* L182: */
/*<   183 format(1h*,15x,26h h2o   content            ,f6.3,1x,t79,1h*) >*/
/* L183: */
/*<   184 format(1h*,15x,26haerosol opt. thick. 550nm ,f6.3,1x,t79,1h*) >*/
/* L184: */
/*<    >*/
/* L426: */
/*<    >*/
/* L427: */
/*<    >*/
/* L428: */
/*<    >*/
/*<    >*/
/*<    >*/
/* L500: */
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<   929 format(1h ,////) >*/
/*<    >*/
/*<   931 format(1h*,6x,a20,t32,f8.5,t47,f8.5,t62,f8.5,t79,1h*) >*/
/*<   932 format(1h*,6x,a20,t32,f8.2,t47,f8.2,t62,f8.2,t79,1h*) >*/
/*<    >*/
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
/* L1501: */
/*<  1502 format(1h*,6(F5.3,1X),F6.1,1X,1(F6.4,1X),t79,1h*) >*/
/* L1502: */
/*<  1503 format(1h*,6x,5(F5.3,1X),F6.1,1X,1(F6.4,1X),t79,1h*) >*/
/* L1503: */
/*<       end >*/
    return 0;
} /* otb_6s__ */

#ifdef __cplusplus
	}
#endif
