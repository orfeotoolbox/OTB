/* AKTOOL.f -- translated by f2c (version 19970805).
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

struct count_1_ {
    integer jl, jj, lg, jg, lf, nnx, n1, n2;
    doublereal u1[10], u2[10], a1[10], a2[10];
};

#define count_1 (*(struct count_1_ *) &count_)

struct soildata_1_ {
    doublereal phis1[200], phis2[200], phis3[200], phis4[200], rsl1, rsl2, 
	    rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil;
};

#define soildata_1 (*(struct soildata_1_ *) &soildata_)

Extern struct {
    doublereal rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint;
} aaa_;

#define aaa_1 aaa_

Extern struct {
    doublereal gr, gt, g, g1, th, sth, cth, th1, sth1, cth1, phi, sp, cp, 
	    th22, st, ct, st1, ct1, t10, t11, e1, e2, s2, s3, ctg, ctg1, ctt1,
	     stt1, calph, alp2, salp2, calp2, alph, salph, alpp, difmy, 
	    difsig;
} ggg_;

#define ggg_1 ggg_

Extern struct {
    doublereal ee, thm, sthm, cthm;
} ladak_;

#define ladak_1 ladak_

Extern union {
    struct {
	doublereal nnl, vai, kk;
    } _1;
    struct {
	doublereal nn, vai, k;
    } _2;
} leafin_;

#define leafin_1 (leafin_._1)
#define leafin_2 (leafin_._2)

Extern struct {
    doublereal refl, tran;
} leafout_;

#define leafout_1 leafout_

struct dat_1_ {
    doublereal refr[200], ke[200], kab[200], kw[200];
};
struct dat_2_ {
    doublereal ref[200], ke[200], kab[200], kw[200];
};

#define dat_1 (*(struct dat_1_ *) &dat_)
#define dat_2 (*(struct dat_2_ *) &dat_)

Extern struct {
    doublereal rn, rk;
} cfresn_;

#define cfresn_1 cfresn_

Extern struct {
    doublereal th10, rncoef, cab, cw, bq;
} msrmdata_;

#define msrmdata_1 msrmdata_

Extern struct {
    doublereal inex__;
} nagout_;

#define nagout_1 nagout_

Extern struct {
    doublereal teta, ref;
} tauin_;

#define tauin_1 tauin_

Extern struct {
    doublereal tau;
} tauout_;

#define tauout_1 tauout_

/* Initialized data */

struct {
    integer fill_1[4];
    integer e_2;
    integer fill_3[83];
    } count_ = { {0}, 1 };

struct {
    doublereal e_1[800];
    doublereal fill_2[9];
    } soildata_ = { .088, .095, .102, .109, .116, .123, .13, .136, .143, .15, 
	    .157, .164, .171, .178, .185, .192, .199, .206, .213, .22, .227, 
	    .233, .24, .247, .254, .261, .268, .275, .282, .289, .295, .302, 
	    .309, .316, .326, .335, .345, .356, .366, .376, .386, .395, .404, 
	    .412, .421, .429, .436, .443, .45, .457, .464, .47, .476, .483, 
	    .489, .495, .502, .508, .514, .52, .526, .532, .538, .543, .549, 
	    .555, .561, .568, .574, .58, .587, .594, .601, .608, .615, .622, 
	    .629, .637, .644, .652, .659, .667, .674, .681, .689, .696, .702, 
	    .709, .716, .723, .729, .735, .742, .748, .754, .76, .766, .771, 
	    .777, .782, .802, .819, .832, .842, .854, .868, .883, .899, .917, 
	    .935, .954, .974, .993, 1.012, 1.03, 1.047, 1.063, 1.078, 1.091, 
	    1.102, 1.111, 1.118, 1.126, 1.137, 1.15, 1.163, 1.176, 1.187, 
	    1.192, 1.188, 1.177, 1.159, 1.134, 1.09, .979, .83, .764, .744, 
	    .748, .777, .823, .878, .932, .983, 1.026, 1.062, 1.091, 1.115, 
	    1.133, 1.147, 1.156, 1.161, 1.162, 1.158, 1.149, 1.132, 1.109, 
	    1.087, 1.072, 1.056, 1.035, .989, .886, .659, .456, .35, .323, 
	    .335, .361, .396, .438, .484, .53, .576, .622, .664, .705, .74, 
	    .768, .788, .8, .802, .796, .794, .797, .789, .779, .756, .725, 
	    .715, .675, .635, .585, .535, .485, .435, .385, .335, .285, .235, 
	    .249, .245, .241, .237, .232, .228, .222, .217, .211, .205, .199, 
	    .193, .186, .179, .171, .163, .155, .147, .139, .13, .121, .111, 
	    .102, .092, .081, .071, .06, .049, .038, .026, .014, .002, -.011, 
	    -.024, -.037, -.05, -.064, -.078, -.092, -.107, -.121, -.137, 
	    -.152, -.168, -.184, -.2, -.216, -.232, -.246, -.259, -.27, -.28, 
	    -.289, -.297, -.303, -.308, -.313, -.317, -.322, -.325, -.329, 
	    -.332, -.335, -.338, -.34, -.342, -.345, -.347, -.35, -.352, 
	    -.355, -.358, -.36, -.363, -.366, -.369, -.372, -.374, -.377, 
	    -.378, -.38, -.381, -.382, -.382, -.383, -.382, -.382, -.381, 
	    -.38, -.378, -.376, -.373, -.37, -.367, -.363, -.359, -.354, 
	    -.349, -.344, -.338, -.31, -.283, -.258, -.234, -.212, -.19, 
	    -.167, -.143, -.118, -.092, -.066, -.039, -.014, .011, .034, .057,
	     .083, .114, .151, .192, .233, .272, .311, .348, .38, .407, .438, 
	    .476, .521, .57, .624, .674, .708, .766, .824, .853, .854, .852, 
	    .858, .881, .916, .947, .973, .997, 1.017, 1.036, 1.052, 1.067, 
	    1.082, 1.095, 1.107, 1.119, 1.131, 1.142, 1.154, 1.166, 1.175, 
	    1.179, 1.178, 1.172, 1.162, 1.148, 1.083, .9, .678, .538, .499, 
	    .515, .552, .598, .653, .716, .777, .834, .886, .932, .973, 1.007,
	     1.036, 1.058, 1.075, 1.086, 1.091, 1.091, 1.086, 1.076, 1.06, 
	    1.039, 1.012, .98, .943, .9, .852, .799, .74, .676, .606, .532, 
	    .451, .366, -.417, -.384, -.351, -.318, -.285, -.253, -.221, 
	    -.189, -.157, -.126, -.095, -.064, -.033, -.003, .027, .057, .087,
	     .117, .146, .175, .204, .232, .26, .289, .316, .344, .371, .399, 
	    .425, .452, .478, .505, .525, .545, .566, .587, .606, .626, .652, 
	    .676, .699, .722, .744, .764, .784, .804, .822, .839, .856, .872, 
	    .886, .9, .913, .926, .937, .948, .957, .966, .974, .981, .988, 
	    .993, .998, 1.002, 1.006, 1.009, 1.012, 1.014, 1.016, 1.017, 
	    1.018, 1.018, 1.018, 1.017, 1.016, 1.014, 1.012, 1.01, 1.007, 
	    1.003, .999, .995, .99, .984, .978, .972, .965, .957, .949, .941, 
	    .932, .923, .913, .902, .891, .88, .868, .855, .842, .829, .766, 
	    .694, .62, .55, .484, .421, .361, .303, .247, .19, .134, .079, 
	    .023, -.031, -.086, -.14, -.19, -.235, -.275, -.31, -.34, -.367, 
	    -.394, -.422, -.452, -.484, -.513, -.541, -.565, -.578, -.575, 
	    -.556, -.525, -.468, -.323, -.115, -.018, .002, -.003, -.029, 
	    -.076, -.142, -.211, -.274, -.333, -.386, -.432, -.471, -.503, 
	    -.528, -.544, -.551, -.549, -.538, -.517, -.491, -.463, -.436, 
	    -.419, -.417, -.401, -.348, -.216, .014, .16, .203, .209, .21, 
	    .207, .2, .189, .174, .155, .132, .105, .075, .043, .013, -.012, 
	    -.035, -.053, -.068, -.078, -.082, -.08, -.073, -.06, -.041, 
	    -.017, .006, .035, .065, .097, .125, .168, .18, .168, .125, .097, 
	    .065, .067, .077, .086, .094, .102, .111, .118, .126, .133, .14, 
	    .146, .152, .158, .164, .169, .174, .179, .184, .188, .192, .195, 
	    .198, .201, .204, .206, .208, .21, .212, .213, .214, .214, .214, 
	    .214, .214, .213, .212, .211, .21, .21, .209, .207, .205, .202, 
	    .198, .194, .189, .184, .179, .173, .167, .161, .155, .149, .143, 
	    .136, .13, .123, .116, .108, .101, .093, .085, .077, .068, .06, 
	    .051, .043, .034, .026, .018, .01, .002, -.006, -.014, -.022, 
	    -.03, -.037, -.045, -.052, -.06, -.067, -.074, -.081, -.087, 
	    -.093, -.098, -.103, -.108, -.112, -.116, -.12, -.123, -.126, 
	    -.129, -.132, -.134, -.136, -.138, -.14, -.141, -.147, -.152, 
	    -.158, -.166, -.17, -.165, -.157, -.151, -.144, -.128, -.104, 
	    -.078, -.049, -.009, .038, .082, .122, .169, .222, .272, .317, 
	    .364, .413, .469, .532, .591, .642, .694, .748, .79, .81, .817, 
	    .819, .74, .494, .215, .11, .125, .155, .204, .291, .408, .521, 
	    .627, .724, .811, .884, .94, .987, 1.025, 1.053, 1.071, 1.077, 
	    1.072, 1.046, .996, .941, .892, .857, .842, .809, .713, .509, 
	    .055, -.236, -.324, -.336, -.32, -.308, -.294, -.275, -.248, 
	    -.205, -.144, -.094, -.048, .005, .058, .105, .132, .123, .079, 
	    .045, .024, .014, .018, .022, -.01, -.042, -.054, -.055, -.06, 
	    -.06, -.055, -.05, -.046, -.042, -.038, -.034, -.03 };

struct {
    doublereal e_1[800];
    } dat_ = { 1.5123, 1.5094, 1.507, 1.505, 1.5032, 1.5019, 1.5007, 1.4997, 
	    1.4988, 1.498, 1.4969, 1.4959, 1.4951, 1.4943, 1.4937, 1.493, 
	    1.4925, 1.492, 1.4915, 1.491, 1.4904, 1.4899, 1.4893, 1.4887, 
	    1.488, 1.4873, 1.4865, 1.4856, 1.4846, 1.4836, 1.4825, 1.4813, 
	    1.4801, 1.4788, 1.4774, 1.4761, 1.4746, 1.4732, 1.4717, 1.4701, 
	    1.4685, 1.467, 1.4654, 1.4639, 1.4624, 1.4609, 1.4595, 1.4582, 
	    1.457, 1.4559, 1.4548, 1.4538, 1.4528, 1.4519, 1.451, 1.4502, 
	    1.4495, 1.4489, 1.4484, 1.448, 1.4477, 1.4474, 1.4472, 1.447, 
	    1.4468, 1.4467, 1.4465, 1.4463, 1.4461, 1.4458, 1.4456, 1.4453, 
	    1.445, 1.4447, 1.4444, 1.444, 1.4435, 1.443, 1.4423, 1.4417, 
	    1.4409, 1.4402, 1.4394, 1.4387, 1.438, 1.4374, 1.4368, 1.4363, 
	    1.4357, 1.4352, 1.4348, 1.4345, 1.4342, 1.4341, 1.434, 1.434, 
	    1.4341, 1.4342, 1.4343, 1.4345, 1.4347, 1.4348, 1.4347, 1.4345, 
	    1.4341, 1.4336, 1.4331, 1.4324, 1.4317, 1.4308, 1.4297, 1.4284, 
	    1.4269, 1.4253, 1.4235, 1.4216, 1.4196, 1.4176, 1.4156, 1.4137, 
	    1.4118, 1.41, 1.4082, 1.4065, 1.4047, 1.4029, 1.4011, 1.3993, 
	    1.3975, 1.3958, 1.394, 1.3923, 1.3906, 1.3888, 1.387, 1.3851, 
	    1.383, 1.3808, 1.3784, 1.3758, 1.3731, 1.3703, 1.3676, 1.3648, 
	    1.362, 1.3592, 1.3565, 1.3537, 1.351, 1.3484, 1.3458, 1.3433, 
	    1.341, 1.3388, 1.3368, 1.335, 1.3333, 1.3317, 1.3303, 1.3289, 
	    1.3275, 1.3263, 1.3251, 1.3239, 1.3228, 1.3217, 1.3205, 1.3194, 
	    1.3182, 1.3169, 1.3155, 1.314, 1.3123, 1.3105, 1.3086, 1.3066, 
	    1.3046, 1.3026, 1.3005, 1.2985, 1.2964, 1.2944, 1.2923, 1.2902, 
	    1.2882, 1.2863, 1.2844, 1.2826, 1.2808, 1.2793, 1.2781, 1.2765, 
	    1.275, 1.2738, 1.2728, 1.2719, 1.2712, 1.2708, 1.2712, 1.2736, 
	    .1104, .0893, .0714, .0567, .0442, .0348, .0279, .0232, .0197, 
	    .0173, .0154, .0142, .012, .0108, .0093, .0092, .0092, .0092, 
	    .0092, .0092, .0091, .0091, .0091, .0091, .0091, .009, .009, .009,
	     .009, .009, .0089, .0089, .0089, .0089, .0088, .0088, .0088, 
	    .0088, .0088, .0087, .0087, .0087, .0087, .0087, .0086, .0086, 
	    .0086, .0086, .0086, .0085, .0085, .0085, .0085, .0085, .0084, 
	    .0084, .0084, .0084, .0084, .0083, .0083, .0083, .0082, .0082, 
	    .0082, .0082, .0082, .0081, .0081, .0081, .0081, .0081, .008, 
	    .008, .008, .008, .008, .0079, .0079, .0079, .0079, .0079, .0078, 
	    .0078, .0078, .0078, .0078, .0077, .0077, .0077, .0077, .0077, 
	    .0076, .0076, .0076, .0076, .0076, .0075, .0075, .0075, .0074, 
	    .0073, .0072, .0071, .007, .0069, .0068, .0068, .0067, .0066, 
	    .0065, .0064, .0063, .0062, .0062, .0061, .006, .0059, .0058, 
	    .0057, .0056, .0056, .0054, .0053, .0053, .0052, .0051, .005, 
	    .0049, .0048, .0047, .0047, .0046, .0045, .0044, .0043, .0042, 
	    .0041, .004, .0039, .0039, .0037, .0037, .0036, .0035, .0034, 
	    .0033, .0032, .0031, .0031, .003, .0029, .0028, .0027, .0026, 
	    .0025, .0025, .0024, .0023, .0022, .0021, .002, .0019, .0019, 
	    .0018, .0017, .0016, .0015, .0014, .0014, .0013, .0012, .001, 
	    .001, 9e-4, 8e-4, 7e-4, 6e-4, 6e-4, 5e-4, 4e-4, 3e-4, 2e-4, 2e-4, 
	    1e-4, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    .04664, .04684, .04568, .04482, .04344, .04257, .04287, .04189, 
	    .04116, .03847, .03409, .03213, .03096, .03116, .03051, .03061, 
	    .02998, .02965, .02913, .02902, .02769, .02707, .02539, .02409, 
	    .0215, .01807, .01566, .01317, .01095, .00929, .00849, .00803, 
	    .00788, .00757, .00734, .00713, .00692, .00693, .00716, .00758, 
	    .00815, .00877, .00938, .00976, .01041, .01089, .01105, .01127, 
	    .0117, .01222, .0128, .01374, .01441, .01462, .01495, .01499, 
	    .01506, .0158, .01686, .0181, .01961, .02112, .02336, .02702, 
	    .0288, .02992, .03142, .03171, .02961, .02621, .02078, .01518, 
	    .0102, .00718, .00519, .0039, .00298, .00218, .00163, .00116, 
	    8.3e-4, 5.7e-4, 3.9e-4, 2.7e-4, 1.4e-4, 1.1e-4, 9e-5, 5e-5, 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
	    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., .1, .2, 
	    .278, .206, .253, .26, .313, .285, .653, .614, .769, .901, .872, 
	    .812, .733, .724, .855, .9, 1.028, 1.5, 2.026, 2.334, 3.636, 
	    8.942, 14.88, 17.838, 19.497, 19.419, 17.999, 12.024, 10.709, 
	    8.384, 7.081, 6.155, 5.619, 5.112, 4.512, 4.313, 4.064, 3.804, 
	    3.709, 3.877, 4.348, 4.574, 5.029, 5.804, 6.345, 5.823, 5.886, 
	    6.315, 8.432, 15.588, 32.247, 51.05, 58.694, 55.135, 50.454, 
	    42.433, 40.67, 36.03, 29.771, 25.153, 24.378, 22.008, 20.608, 
	    18.576, 17.257, 15.921, 14.864, 12.861, 12.773, 12.426, 13.09, 
	    14.013, 15.066, 15.857, 16.776, 19.113, 21.066, 22.125, 26.438, 
	    28.391, 28.92, 31.754, 36.375, 40.056, 41.019, 45.471, 43.126 };


/* Table of constant values */

static doublereal c_b46 = -1.;
static integer c__9 = 9;
static integer c__1 = 1;

/*<       subroutine msrm >*/
/* Subroutine */ int msrm_()
{

    /* Builtin functions */
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    extern /* Subroutine */ int biz_();
    doublereal rtp, cth10, sth10, rrls;
    extern /* Subroutine */ int difr92_();


/*   MultiSpectral Reflectance Model 93         A.Kuusk   24.03.1993 */

/*<       implicit double precision (a-h, o-z) >*/
/*<       save /count/, /soildata/, /aaa/, /ggg/, /ladak/ >*/

/*<       dimension u1(10), u2(10), a1(10), a2(10) >*/
/*<       common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2 >*/

/*<       double precision nnl, kk >*/
/*<       common /leafin/ nnl, vai, kk >*/
/*<       common /leafout/ refl, tran >*/

/*<       double precision ke, kab, kw >*/
/*<       dimension refr(200), ke(200), kab(200), kw(200) >*/
/*<       common /dat/ refr, ke, kab, kw >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
/*<    >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/
/*<       common /cfresn/ rn, rk >*/
/*<       common /ladak/ ee, thm, sthm, cthm >*/
/*<       common /msrmdata/ th10, rncoef, cab, cw, bq >*/

/*<       data pi12/1.570796326794895d0/, pi/3.141592653589793d0/ >*/
/*<       data eps4/.1d-3/ >*/

/*           print *, 'msrm' */

/*<       sth10 = sin(th10) >*/
    sth10 = sin(msrmdata_1.th10);
/*<       cth10 = cos(th10) >*/
    cth10 = cos(msrmdata_1.th10);

/*<       sp    = sin(phi) >*/
    ggg_1.sp = sin(ggg_1.phi);
/*<       cp    = cos(phi) >*/
    ggg_1.cp = cos(ggg_1.phi);
/*<       th1   = th10 >*/
    ggg_1.th1 = msrmdata_1.th10;
/*<       sth1  = sth10 >*/
    ggg_1.sth1 = sth10;
/*<       cth1  = cth10 >*/
    ggg_1.cth1 = cth10;
/*<       sth   = sin(th) >*/
    ggg_1.sth = sin(ggg_1.th);
/*<       cth   = cos(th) >*/
    ggg_1.cth = cos(ggg_1.th);
/*<       rrls  = rrl >*/
    rrls = aaa_1.rrl;

/*<       call biz >*/
    biz_();

/*<       rrl  = refl >*/
    aaa_1.rrl = leafout_1.refl;
/*<       rtp  = rrl + ttl >*/
    rtp = aaa_1.rrl + aaa_1.ttl;

/*<       call difr92 >*/
    difr92_();

/*< 10    continue >*/
/* L10: */

/*<       rrl = rrls >*/
    aaa_1.rrl = rrls;
/*<       bq  = bi + bd >*/
    msrmdata_1.bq = aaa_1.bi + aaa_1.bd;

/*<       return >*/
    return 0;
/*<       end >*/
} /* msrm_ */


/* ***************************************************************** */

/*<       subroutine akd >*/
/* Subroutine */ int akd_()
{
    /* Initialized data */

    static doublereal eps = .005;
    static doublereal pi = 3.141592653589793;
    static doublereal pi1 = 1.5707963268;

    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    integer i__, j, i1, i2;
    doublereal bd1, bd2, tt2[10], tt3[10];
    extern /* Subroutine */ int biz_();
    doublereal thi, rtp, ctt2[10], ctt3[10], stt2[10], stt3[10], rrls;
    extern /* Subroutine */ int difr92_();

/*  bdz   A.Kuusk    4.03.1988 */

/*<       implicit double precision (a-h, o-z) >*/
/*<       save /count/, /aaa/, /ggg/ >*/

/*<       dimension tt3(10), stt3(10), ctt3(10), tt2(10), stt2(10), ctt2(10) >*/

/*<       dimension u1(10), u2(10), a1(10), a2(10) >*/
/*<       common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2 >*/

/*<       double precision nnl, kk >*/
/*<       common /leafin/ nnl, vai, kk >*/
/*<       common /leafout/ refl, tran >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/

/*<       data pi/3.141592653589793d0/, pi1/1.5707963268d0/, eps/.005d0/ >*/

/*                    print *, 'akd' */
/*<       bqint = 0.d0 >*/
    aaa_1.bqint = 0.;
/*<       if (th .gt. eps) goto 4 >*/
    if (ggg_1.th > eps) {
	goto L4;
    }
/*<       phi = 0.d0 >*/
    ggg_1.phi = 0.;
/*<       sp  = 0.d0 >*/
    ggg_1.sp = 0.;
/*<       cp  = 1.d0 >*/
    ggg_1.cp = 1.;

/*<       do 10 i2 = 1, n2 >*/
    i__1 = count_1.n2;
    for (i2 = 1; i2 <= i__1; ++i2) {
/*<          th1  = (1.d0 - u2(i2))*pi1 >*/
	ggg_1.th1 = (1. - count_1.u2[i2 - 1]) * pi1;
/*<          sth1 = sin(th1) >*/
	ggg_1.sth1 = sin(ggg_1.th1);
/*<          cth1 = cos(th1) >*/
	ggg_1.cth1 = cos(ggg_1.th1);
/*<          rrls = rrl >*/
	rrls = aaa_1.rrl;

/*<          call biz >*/
	biz_();

/*<          rrl = refl >*/
	aaa_1.rrl = leafout_1.refl;
/*<          rtp = rrl + ttl >*/
	rtp = aaa_1.rrl + aaa_1.ttl;

/*<          call difr92 >*/
	difr92_();

/*<          rrl   = rrls >*/
	aaa_1.rrl = rrls;
/*<          bqint = bqint + a2(i2)*(bi + bd)*sth1*cth1 >*/
	aaa_1.bqint += count_1.a2[i2 - 1] * (aaa_1.bi + aaa_1.bd) * 
		ggg_1.sth1 * ggg_1.cth1;
/*< 10    continue >*/
/* L10: */
    }

/*<       bqint = bqint*pi >*/
    aaa_1.bqint *= pi;
/*<       goto 1 >*/
    goto L1;

/*< 4     continue >*/
L4:
/*<       do 14 i = 1, n1 >*/
    i__1 = count_1.n1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          thi     = u1(i)*th >*/
	thi = count_1.u1[i__ - 1] * ggg_1.th;
/*<          tt3(i)  = thi >*/
	tt3[i__ - 1] = thi;
/*<          stt3(i) = sin(thi) >*/
	stt3[i__ - 1] = sin(thi);
/*<          ctt3(i) = cos(thi) >*/
	ctt3[i__ - 1] = cos(thi);
/*< 14    continue >*/
/* L14: */
    }

/*<       do 15 i = 1, n2 >*/
    i__1 = count_1.n2;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          thi     = u2(i)*(th - pi1) + pi1 >*/
	thi = count_1.u2[i__ - 1] * (ggg_1.th - pi1) + pi1;
/*<          tt2(i)  = thi >*/
	tt2[i__ - 1] = thi;
/*<          stt2(i) = sin(thi) >*/
	stt2[i__ - 1] = sin(thi);
/*<          ctt2(i) = cos(thi) >*/
	ctt2[i__ - 1] = cos(thi);
/*< 15    continue >*/
/* L15: */
    }

/*<       do 11 j = 1, n1 >*/
    i__1 = count_1.n1;
    for (j = 1; j <= i__1; ++j) {
/*<          phi  = (1.d0 - u1(j))*pi >*/
	ggg_1.phi = (1. - count_1.u1[j - 1]) * pi;
/*<          sp   = sin(phi) >*/
	ggg_1.sp = sin(ggg_1.phi);
/*<          cp   = cos(phi) >*/
	ggg_1.cp = cos(ggg_1.phi);
/*<          bd1  = 0.d0 >*/
	bd1 = 0.;
/*<          bd2  = 0.d0 >*/
	bd2 = 0.;
/*<          do 12 i1 = 1, n1 >*/
	i__2 = count_1.n1;
	for (i1 = 1; i1 <= i__2; ++i1) {
/*<             th1  = tt3(i1) >*/
	    ggg_1.th1 = tt3[i1 - 1];
/*<             sth1 = stt3(i1) >*/
	    ggg_1.sth1 = stt3[i1 - 1];
/*<             cth1 = ctt3(i1) >*/
	    ggg_1.cth1 = ctt3[i1 - 1];

/*<          rrls = rrl >*/
	    rrls = aaa_1.rrl;

/*<          call biz >*/
	    biz_();

/*<          rrl = refl >*/
	    aaa_1.rrl = leafout_1.refl;
/*<          rtp = rrl + ttl >*/
	    rtp = aaa_1.rrl + aaa_1.ttl;

/*<          call difr92 >*/
	    difr92_();

/*<          rrl = rrls >*/
	    aaa_1.rrl = rrls;

/*<             bd1 = bd1 + a1(i1)*(bi + bd)*sth1*cth1 >*/
	    bd1 += count_1.a1[i1 - 1] * (aaa_1.bi + aaa_1.bd) * ggg_1.sth1 * 
		    ggg_1.cth1;
/*< 12       continue >*/
/* L12: */
	}

/*<          do 13 i2 = 1, n2 >*/
	i__2 = count_1.n2;
	for (i2 = 1; i2 <= i__2; ++i2) {
/*<             th1  = tt2(i2) >*/
	    ggg_1.th1 = tt2[i2 - 1];
/*<             sth1 = stt2(i2) >*/
	    ggg_1.sth1 = stt2[i2 - 1];
/*<             cth1 = ctt2(i2) >*/
	    ggg_1.cth1 = ctt2[i2 - 1];

/*<          rrls = rrl >*/
	    rrls = aaa_1.rrl;

/*<          call biz >*/
	    biz_();

/*<          rrl = refl >*/
	    aaa_1.rrl = leafout_1.refl;
/*<          rtp = rrl + ttl >*/
	    rtp = aaa_1.rrl + aaa_1.ttl;

/*<          call difr92 >*/
	    difr92_();

/*<          rrl = rrls >*/
	    aaa_1.rrl = rrls;

/*<             bd2 = bd2 + a2(i2)*(bi + bd)*sth1*cth1 >*/
	    bd2 += count_1.a2[i2 - 1] * (aaa_1.bi + aaa_1.bd) * ggg_1.sth1 * 
		    ggg_1.cth1;
/*< 13       continue >*/
/* L13: */
	}

/*<          bqint = bqint + ((pi1 - th)*bd2 + th*bd1)*a1(j) >*/
	aaa_1.bqint += ((pi1 - ggg_1.th) * bd2 + ggg_1.th * bd1) * count_1.a1[
		j - 1];
/*< 11    continue >*/
/* L11: */
    }

/*<       bqint = bqint + bqint >*/
    aaa_1.bqint += aaa_1.bqint;

/*< 1     return >*/
L1:
    return 0;
/*<       end >*/
} /* akd_ */


/* ***************************************************************** */

/*<       subroutine biz >*/
/* Subroutine */ int biz_()
{
    /* Initialized data */

    static doublereal pi = 3.14159265358979;
    static doublereal eps = 1e-5;
    static doublereal eps3 = .01;

    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double acos(doublereal), sin(doublereal), cos(doublereal), sqrt(
	    doublereal), exp(doublereal);

    /* Local variables */
    doublereal x, x2, y4, gf, gg, bc1, gg1, bs1, xx1, xx2, bam, gma;
    extern /* Subroutine */ int gmf_(doublereal *);
    doublereal sct, thp, ulg, ths, bc1d, ulg1, th1s;
    extern /* Subroutine */ int glak_(doublereal *, doublereal *);
    doublereal alpd;
    extern /* Subroutine */ int gmd92_();
    doublereal bcsp, cths;
    extern /* Subroutine */ int soil_();
    doublereal sths, bc1hs, cth1s, sth1s, gammd, easte, catmp, glthp, easte2, 
	    easte4;

/*     canopy reflectance of single scattering for direct radiation */
/*     A. Kuusk   6.02.1992 */

/*<       implicit double precision (a-h, o-z) >*/
/*<       double precision integr >*/
/*<       save /count/, /soildata/, /aaa/, /ggg/, /ladak/ >*/

/*     dimension gj(2), g1j(2), grj(2), gtj(2), gfj(2) */

/*<       dimension u1(10), u2(10), a1(10), a2(10) >*/
/*<       common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2 >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
/*<    >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/
/*<       common /ladak/ ee, thm, sthm, cthm >*/

/*<       data pi/3.14159265358979d0/, eps/.1d-4/, eps3/.01d0/ >*/

/*<       integr(xx) = (1.d0 - exp(-xx))/xx >*/
/*           print *, 'biz in' */
/*<       ths   = th >*/
    ths = ggg_1.th;
/*<       sths  = sth >*/
    sths = ggg_1.sth;
/*<       cths  = cth >*/
    cths = ggg_1.cth;
/*<       th1s  = th1 >*/
    th1s = ggg_1.th1;
/*<       sth1s = sth1 >*/
    sth1s = ggg_1.sth1;
/*<       cth1s = cth1 >*/
    cth1s = ggg_1.cth1;
/*     thms  = thm */

/*<       call soil >*/
    soil_();

/*<       if (ul .gt. eps) goto 2 >*/
    if (aaa_1.ul > eps) {
	goto L2;
    }
/*<       bi  = rsoil >*/
    aaa_1.bi = soildata_1.rsoil;
/*<       goto 1 >*/
    goto L1;

/*< 2     continue >*/
L2:
/*<       if (th1 .lt. th) goto 12 >*/
    if (ggg_1.th1 < ggg_1.th) {
	goto L12;
    }
/*<       t11  = th1 >*/
    ggg_1.t11 = ggg_1.th1;
/*<       st   = sth >*/
    ggg_1.st = ggg_1.sth;
/*<       st1  = sth1 >*/
    ggg_1.st1 = ggg_1.sth1;
/*<       ct   = cth >*/
    ggg_1.ct = ggg_1.cth;
/*<       ct1  = cth1 >*/
    ggg_1.ct1 = ggg_1.cth1;
/*<       t10  = th >*/
    ggg_1.t10 = ggg_1.th;
/*<       jj   = 0 >*/
    count_1.jj = 0;
/*<       goto 7 >*/
    goto L7;

/*< 12    t10  = th1 >*/
L12:
    ggg_1.t10 = ggg_1.th1;
/*<       st   = sth1 >*/
    ggg_1.st = ggg_1.sth1;
/*<       st1  = sth >*/
    ggg_1.st1 = ggg_1.sth;
/*<       ct   = cth1 >*/
    ggg_1.ct = ggg_1.cth1;
/*<       ct1  = cth >*/
    ggg_1.ct1 = ggg_1.cth;
/*<       t11  = th >*/
    ggg_1.t11 = ggg_1.th;
/*<       jj   = 1 >*/
    count_1.jj = 1;

/*< 7     continue >*/
L7:
/*<       ctt1  = ct*ct1 >*/
    ggg_1.ctt1 = ggg_1.ct * ggg_1.ct1;
/*<       stt1  = st*st1 >*/
    ggg_1.stt1 = ggg_1.st * ggg_1.st1;
/*<       calph = stt1*cp + ctt1 >*/
    ggg_1.calph = ggg_1.stt1 * ggg_1.cp + ggg_1.ctt1;
/*<       catmp = calph >*/
    catmp = ggg_1.calph;
/*<       alph  = acos(catmp) >*/
    ggg_1.alph = acos(catmp);
/*<       alp2  = alph*.5d0 >*/
    ggg_1.alp2 = ggg_1.alph * .5;
/*     if (lf .ne. 2) then */
/*        if( jg .gt. 2) then */
/*           print *, ' ***  biz3:  jg > 2  ***' */
/*           stop */
/*        endif */
/*<          e1   = st*ct1 >*/
    ggg_1.e1 = ggg_1.st * ggg_1.ct1;
/*<          e2   = ct*st1 >*/
    ggg_1.e2 = ggg_1.ct * ggg_1.st1;
/*<          s2   = e1*cp + e2 >*/
    ggg_1.s2 = ggg_1.e1 * ggg_1.cp + ggg_1.e2;
/*<          s3   = e1*sp >*/
    ggg_1.s3 = ggg_1.e1 * ggg_1.sp;
/*<          ctg  = 1.d30 >*/
    ggg_1.ctg = 1e30;
/*<          ctg1 = 1.d30 >*/
    ggg_1.ctg1 = 1e30;
/*<          if (st .ne. 0.d0) ctg = ct/st >*/
    if (ggg_1.st != 0.) {
	ggg_1.ctg = ggg_1.ct / ggg_1.st;
    }
/*<          if (st1 .ne. 0.d0) ctg1 = ct1/st1 >*/
    if (ggg_1.st1 != 0.) {
	ggg_1.ctg1 = ggg_1.ct1 / ggg_1.st1;
    }
/*<          salph = sin(alph) >*/
    ggg_1.salph = sin(ggg_1.alph);
/*<          alpp  = pi - alph >*/
    ggg_1.alpp = pi - ggg_1.alph;
/*<          salp2 = sin(alp2) >*/
    ggg_1.salp2 = sin(ggg_1.alp2);
/*<          calp2 = cos(alp2) >*/
    ggg_1.calp2 = cos(ggg_1.alp2);

/*<          call gmf(gf) >*/
    gmf_(&gf);

/*<          if (ee .le. eps3) goto 95 >*/
    if (ladak_1.ee <= eps3) {
	goto L95;
    }
/*<          y4  = abs(cth + cth1)*.5d0/calp2 >*/
    y4 = (d__1 = ggg_1.cth + ggg_1.cth1, abs(d__1)) * .5 / ggg_1.calp2;
/*<          if (y4.lt.1.d0) thp = acos(y4) >*/
    if (y4 < 1.) {
	thp = acos(y4);
    }

/*< 95       call glak(glthp, thp) >*/
L95:
    glak_(&glthp, &thp);

/*<          x2 = glthp*.125d0 >*/
    x2 = glthp * .125;
/*<          gf = gf*x2 >*/
    gf *= x2;

/*<          call gmd92 >*/
    gmd92_();

/*<       gammd = gr*rrl + gt*ttl >*/
    gammd = ggg_1.gr * aaa_1.rrl + ggg_1.gt * aaa_1.ttl;

/*<       t11 = th1 >*/
    ggg_1.t11 = ggg_1.th1;
/*<       st  = sth >*/
    ggg_1.st = ggg_1.sth;
/*<       st1 = sth1 >*/
    ggg_1.st1 = ggg_1.sth1;
/*<       ct  = cth >*/
    ggg_1.ct = ggg_1.cth;
/*<       ct1 = cth1 >*/
    ggg_1.ct1 = ggg_1.cth1;
/*<       t10 = th >*/
    ggg_1.t10 = ggg_1.th;
/*<       if (jj .eq. 1) then >*/
    if (count_1.jj == 1) {
/*<          x = g1 >*/
	x = ggg_1.g1;
/*<          g1 = g >*/
	ggg_1.g1 = ggg_1.g;
/*<          g = x >*/
	ggg_1.g = x;
/*<       endif >*/
    }

/*           print *, 'biz:2' */
/*<       gg   = g*g1 >*/
    gg = ggg_1.g * ggg_1.g1;
/*<       g    = g*clmp >*/
    ggg_1.g *= aaa_1.clmp;
/*<       g1   = g1*clmp1 >*/
    ggg_1.g1 *= aaa_1.clmp1;
/*<       gg1  = g*ct1 + g1*ct >*/
    gg1 = ggg_1.g * ggg_1.ct1 + ggg_1.g1 * ggg_1.ct;
/*<       sct  = sqrt(ctt1) >*/
    sct = sqrt(ggg_1.ctt1);
/*<       alpd = alp2/sl >*/
    alpd = ggg_1.alp2 / aaa_1.sl;
/*<       bam  = alpd*sct/ul >*/
    bam = alpd * sct / aaa_1.ul;

/*<       if (ctt1 .gt. eps) then >*/
    if (ggg_1.ctt1 > eps) {
/*<          gma  = alpd/sct >*/
	gma = alpd / sct;
/*<          ulg  = gg1/ctt1*ul >*/
	ulg = gg1 / ggg_1.ctt1 * aaa_1.ul;
/*<       else >*/
    } else {
/*<          gma  = 0.d0 >*/
	gma = 0.;
/*<          ulg  = ul >*/
	ulg = aaa_1.ul;
/*<       endif >*/
    }
/*<       ulg1 = ulg*.5d0 >*/
    ulg1 = ulg * .5;
/*<       xx1  = ulg + gma >*/
    xx1 = ulg + gma;
/*<       if ((xx1 .gt. 30.d0) .or. (ctt1 .le. eps)) then >*/
    if (xx1 > 30. || ggg_1.ctt1 <= eps) {
/*<          easte  = 0.d0 >*/
	easte = 0.;
/*<          easte2 = 0.d0 >*/
	easte2 = 0.;
/*<          easte4 = 0.d0 >*/
	easte4 = 0.;
/*<          bs1    = 0.d0 >*/
	bs1 = 0.;
/*<       else >*/
    } else {
/*<          easte  = exp(-ulg) >*/
	easte = exp(-ulg);
/*<          easte2 = exp(-ulg1 - gma) >*/
	easte2 = exp(-ulg1 - gma);
/*<          easte4 = exp(-ulg - gma) >*/
	easte4 = exp(-ulg - gma);
/*<          bs1    = (easte + easte2 - easte4)*rsoil >*/
	bs1 = (easte + easte2 - easte4) * soildata_1.rsoil;
/*<       endif >*/
    }

/*<       xx1   = (1.d0 - easte)/gg1 >*/
    xx1 = (1. - easte) / gg1;
/*<    >*/
    xx2 = (1. - easte2) / (gg1 * .5 + bam) - (1. - easte4) / (gg1 + bam);
/*<       bc1d  = xx1*gammd >*/
    bc1d = xx1 * gammd;
/*<       bc1hs = xx2*(gammd + gf) >*/
    bc1hs = xx2 * (gammd + gf);
/*<       bcsp  = xx1*gf >*/
    bcsp = xx1 * gf;
/*<       bc1   = bc1d + bcsp + bc1hs >*/
    bc1 = bc1d + bcsp + bc1hs;
/*<       bi    = bc1 + bs1 >*/
    aaa_1.bi = bc1 + bs1;

/*< 1     continue >*/
L1:
/*<       th    = ths >*/
    ggg_1.th = ths;
/*<       sth   = sths >*/
    ggg_1.sth = sths;
/*<       cth   = cths >*/
    ggg_1.cth = cths;
/*<       th1   = th1s >*/
    ggg_1.th1 = th1s;
/*<       sth1  = sth1s >*/
    ggg_1.sth1 = sth1s;
/*<       cth1s = cth1 >*/
    cth1s = ggg_1.cth1;
/*     thm   = thms */

/*<       return >*/
    return 0;
/*<       end >*/
} /* biz_ */


/* ***************************************************************** */

/*<       subroutine difr92 >*/
/* Subroutine */ int difr92_()
{
    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal), exp(doublereal);

    /* Local variables */
    doublereal a, b, c__, d__, m, h1, h2, bf, gg, m11, m12, m21, m22, em, sb, 
	    ko, sf, ks, ub, uf, ep, ek, gp, gm, det, sig, ems, att, rtp, epso,
	     epss, tsun, tanto, tants, tview, rdsoil, rplants;

/*   diffuse fluxes according to SAIL for an elliptical LAD */
/*   A. Kuusk 16.06.1992 */

/*<       implicit double precision (a-h, o-z) >*/
/*<       double precision ks, ko, m, m11, m12, m21, m22, integr >*/
/*<       save /soildata/, /aaa/, /ggg/, /ladak/ >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
/*<    >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/
/*<       common /ladak/ ee, thm, sthm, cthm >*/

/*<       integr(x) = (1.d0 - exp(-x))/x >*/
/*           print *, 'difr92' */

/*<       tsun  = th1 >*/
    tsun = ggg_1.th1;
/*<       tview = th >*/
    tview = ggg_1.th;
/*<       tants = sth1/cth1 >*/
    tants = ggg_1.sth1 / ggg_1.cth1;
/*<       tanto = sth/cth >*/
    tanto = ggg_1.sth / ggg_1.cth;
/*<       rtp   = (rrl + ttl)/2.d0 >*/
    rtp = (aaa_1.rrl + aaa_1.ttl) / 2.;

/*<       ks    = g1*ul/cth1 >*/
    ks = ggg_1.g1 * aaa_1.ul / ggg_1.cth1;
/*<       ko    = g*ul/cth >*/
    ko = ggg_1.g * aaa_1.ul / ggg_1.cth;
/*<    >*/
/* Computing 2nd power */
    d__1 = ladak_1.cthm;
    gg = (ggg_1.difmy * 1.289 - ggg_1.difsig * 1.816) * (d__1 * d__1 - 
	    .33333333333) + .31823;
/*<       bf    = (rrl - ttl)/2.d0*ul*gg >*/
    bf = (aaa_1.rrl - aaa_1.ttl) / 2. * aaa_1.ul * gg;
/*<       att   = (1.d0 - rtp)*ul + bf >*/
    att = (1. - rtp) * aaa_1.ul + bf;
/*<       sig   = rtp*ul + bf >*/
    sig = rtp * aaa_1.ul + bf;
/*<       sb    = ks*rtp + bf >*/
    sb = ks * rtp + bf;
/*<       sf    = ks*rtp - bf >*/
    sf = ks * rtp - bf;
/*<       ub    = ko*rtp + bf >*/
    ub = ko * rtp + bf;
/*<       uf    = ko*rtp - bf >*/
    uf = ko * rtp - bf;
/*<       m     = sqrt(att**2 - sig**2) >*/
/* Computing 2nd power */
    d__1 = att;
/* Computing 2nd power */
    d__2 = sig;
    m = sqrt(d__1 * d__1 - d__2 * d__2);
/*<       h1    = (att + m)/sig >*/
    h1 = (att + m) / sig;
/*<       h2    = 1.d0/h1 >*/
    h2 = 1. / h1;
/*<       c     = (sf*sig - sb*(ks - att))/(m**2 - ks**2) >*/
/* Computing 2nd power */
    d__1 = m;
/* Computing 2nd power */
    d__2 = ks;
    c__ = (sf * sig - sb * (ks - att)) / (d__1 * d__1 - d__2 * d__2);
/*<       d     = (sb*sig + sf*(ks + att))/(m**2 - ks**2) >*/
/* Computing 2nd power */
    d__1 = m;
/* Computing 2nd power */
    d__2 = ks;
    d__ = (sb * sig + sf * (ks + att)) / (d__1 * d__1 - d__2 * d__2);
/*     epso  = skyl - d*sq */
/*<       epso  =  - d >*/
    epso = -d__;
/*     epss  = (rrsoil*(d + 1.d0) - c)*sq*exp(-ks) */
/*<       epss  = (rrsoil*(d + 1.d0) - c)*exp(-ks) >*/
    epss = (soildata_1.rrsoil * (d__ + 1.) - c__) * exp(-ks);
/*<       m11   = h1 >*/
    m11 = h1;
/*<       m12   = h2 >*/
    m12 = h2;
/*<       m21   = (1.d0 - rrsoil*h1)*exp(-m) >*/
    m21 = (1. - soildata_1.rrsoil * h1) * exp(-m);
/*<       m22   = (1.d0 - rrsoil*h2)*exp(m) >*/
    m22 = (1. - soildata_1.rrsoil * h2) * exp(m);
/*<       det   = m11*m22 - m12*m21 >*/
    det = m11 * m22 - m12 * m21;
/*<       a     = (m22*epso - m12*epss)/det >*/
    a = (m22 * epso - m12 * epss) / det;
/*<       b     = (-m21*epso + m11*epss)/det >*/
    b = (-m21 * epso + m11 * epss) / det;
/*<       ep    = integr(ko + m) >*/
    d__1 = ko + m;
    ep = (1. - exp(-d__1)) / d__1;
/*<       em    = integr(ko - m) >*/
    d__1 = ko - m;
    em = (1. - exp(-d__1)) / d__1;
/*<       ek    = integr(ko + ks) >*/
    d__1 = ko + ks;
    ek = (1. - exp(-d__1)) / d__1;
/*     gp    = a*ep + b*em + c*ek*sq */
/*<       gp    = a*ep + b*em + c*ek >*/
    gp = a * ep + b * em + c__ * ek;
/*     gm    = h1*a*ep + h2*b*em + d*ek*sq */
/*<       gm    = h1*a*ep + h2*b*em + d*ek >*/
    gm = h1 * a * ep + h2 * b * em + d__ * ek;
/*     ems   = h1*a*exp(-m) + h2*b*exp(m) + d*sq*exp(-ks) */
/*<       ems   = h1*a*exp(-m) + h2*b*exp(m) + d*exp(-ks) >*/
    ems = h1 * a * exp(-m) + h2 * b * exp(m) + d__ * exp(-ks);
/*<       rplants = uf*gp + ub*gm >*/
    rplants = uf * gp + ub * gm;
/*<       rdsoil  = rrsoil*ems*exp(-ko) >*/
    rdsoil = soildata_1.rrsoil * ems * exp(-ko);
/*<       bd    = rplants + rdsoil >*/
    aaa_1.bd = rplants + rdsoil;

/*<       return >*/
    return 0;
/*<       end >*/
} /* difr92_ */


/* ********************************************************************* */

/*<       subroutine glak(glth, th) >*/
/* Subroutine */ int glak_(doublereal *glth, doublereal *th)
{
    /* Initialized data */

    static doublereal tms = 0.;
    static doublereal eps = .1;
    static doublereal bb = 1.;
    static doublereal es = 0.;

    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal), log(doublereal), atan2(doublereal, doublereal), 
	    cos(doublereal);

    /* Local variables */
    doublereal x, u1, u2, u3, u4, x1, x2;

/*  elliptical distribution */
/*  A.Kuusk   1.03.1988 */

/*<       implicit double precision (a-h, o-z) >*/
/*<       save /aaa/, /ladak/ >*/
/*<       save bb, es, tms >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<       common /ladak/ ee, thm, sthm, cthm >*/

/*<       data bb/1.d0/, es/0.d0/, tms/0.d0/, eps/.1d0/ >*/

/*           print *, 'gl' */

/*<       if (ee .lt. eps) then >*/
    if (ladak_1.ee < eps) {
/*<       glth = 1.d0 >*/
	*glth = 1.;
/*<       return >*/
	return 0;
/*<       endif >*/
    }

/*<       if (ee .eq. 1.d0) ee = .999999d0 >*/
    if (ladak_1.ee == 1.) {
	ladak_1.ee = .999999;
    }
/*<       if ((ee .ne. es) .or. (thm .ne. tms)) then >*/
    if (ladak_1.ee != es || ladak_1.thm != tms) {
/*<         u1  = ee*cthm >*/
	u1 = ladak_1.ee * ladak_1.cthm;
/*<         u3  = ee*sthm >*/
	u3 = ladak_1.ee * ladak_1.sthm;
/*<         u2  = sqrt(1.d0 - u1*u1) >*/
	u2 = sqrt(1. - u1 * u1);
/*<         u4  = sqrt(1.d0 - u3*u3) >*/
	u4 = sqrt(1. - u3 * u3);
/*<         x   = log((u4 + u1)/(u2 - u3)) >*/
	x = log((u4 + u1) / (u2 - u3));
/*<         x1  = atan2(u3, u4) - atan2(u1, u2) >*/
	x1 = atan2(u3, u4) - atan2(u1, u2);
/*<         x2  = sthm*x - cthm*x1 >*/
	x2 = ladak_1.sthm * x - ladak_1.cthm * x1;
/*<         bb  = ee/x2 >*/
	bb = ladak_1.ee / x2;
/*<         es  = ee >*/
	es = ladak_1.ee;
/*<         tms = thm >*/
	tms = ladak_1.thm;
/*<       endif >*/
    }

/*<       glth = bb/sqrt(1.d0 - (ee*cos(thm - th))**2) >*/
/* Computing 2nd power */
    d__1 = ladak_1.ee * cos(ladak_1.thm - *th);
    *glth = bb / sqrt(1. - d__1 * d__1);

/*<       return >*/
    return 0;
/*<       end >*/
} /* glak_ */


/* ***************************************************************** */

/*<       subroutine gmf(gf) >*/
/* Subroutine */ int gmf_(doublereal *gf)
{
    /* Initialized data */

    static doublereal pi12 = 1.570796326794895;

    /* Builtin functions */
    double sqrt(doublereal), exp(doublereal);

    /* Local variables */
    doublereal y, x2, ca, ag, bg, cg, xy, yy, sa2;

/*  Fresnel' reflection                    A.Kuusk 02.01.1991 */
/*  input parameters are ca = cos(th_incident),  rn=refract.ind., */
/*  rk = leaf hair index */

/*<       implicit double precision (a-h, o-z) >*/
/*<       save /aaa/, /ggg/ >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/
/*<       common /cfresn/ rn, rk >*/

/*<       data pi12/1.570796326794895d0/ >*/

/*           print *, 'gmf' */

/*<       ca=calp2 >*/
    ca = ggg_1.calp2;
/*<       x2  = ca*ca >*/
    x2 = ca * ca;
/*<       ag  = x2*2.d0 - 1.d0 + rn*rn >*/
    ag = x2 * 2. - 1. + cfresn_1.rn * cfresn_1.rn;
/*<       bg  = 1.d0 + (ag - 2.d0)*x2 >*/
    bg = (ag - 2.) * x2 + 1.;
/*<       xy  = ag - x2 >*/
    xy = ag - x2;
/*<       cg  = 2.d0*ca*sqrt(xy) >*/
    cg = ca * 2. * sqrt(xy);
/*<       sa2 = 1.d0 - x2 >*/
    sa2 = 1. - x2;
/*<       y   = (bg + sa2*cg)*(ag + cg) >*/
    y = (bg + sa2 * cg) * (ag + cg);
/*<       y   = (ag - cg)*bg/y >*/
    y = (ag - cg) * bg / y;
/*<       yy  = sqrt(sa2)/pi12/ca*rk >*/
    yy = sqrt(sa2) / pi12 / ca * cfresn_1.rk;
/*<       gf  = exp(-yy)*y >*/
    *gf = exp(-yy) * y;

/*<       return >*/
    return 0;
/*<       end >*/
} /* gmf_ */


/* ***************************************************************** */

/*<       subroutine soil >*/
/* Subroutine */ int soil_()
{
    /* Initialized data */

    static doublereal a = .45098;
    static doublereal b = 5.7829;
    static doublereal c__ = 13.7575;
    static doublereal cts = 13.7575;
    static doublereal ths1 = .785398163;
    static doublereal ths2 = .785398163;

    doublereal x, x2;

/*   Soil directional reflectance and reflectance (albedo) */
/*   th, th1, th2 in radianes,  cp = cos(phi) */
/*   A.Kuusk     1.03.1988 */

/*<       implicit double precision (a-h, o-z) >*/
/*<       save a, b, c, cts, ths1, ths2 >*/
/*<       save /soildata/, /aaa/, /ggg/ >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
/*<    >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/

/*<       data a/.45098d0/, b/5.7829d0/, c, cts/2*13.7575d0/ >*/
/*<       data ths1, ths2/2*.785398163d0/ >*/

/*           print *, 'soil' */
/*<       if (th2 .ne. ths2) then >*/
    if (soildata_1.th2 != ths2) {
/*<          cts  = 16.41d0 - th2*th2*4.3d0 >*/
	cts = 16.41 - soildata_1.th2 * soildata_1.th2 * 4.3;
/*<          ths2 = th2 >*/
	ths2 = soildata_1.th2;
/*<       endif >*/
    }
/*<       if (th1 .ne. ths1) then >*/
    if (ggg_1.th1 != ths1) {
/*<          ths1 = th1 >*/
	ths1 = ggg_1.th1;
/*<          x    = th1*th1 >*/
	x = ggg_1.th1 * ggg_1.th1;
/*<          a    = x*7.702d0 - 4.3d0 >*/
	a = x * 7.702 - 4.3;
/*<          b    = th1*7.363d0 >*/
	b = ggg_1.th1 * 7.363;
/*<          c    = 16.41d0 - x*4.3d0 >*/
	c__ = 16.41 - x * 4.3;
/*<       endif >*/
    }
/*<       x2      = rsl/cts >*/
    x2 = soildata_1.rsl / cts;
/*<       rsoil   = ((a*th + b*cp)*th + c)*x2 >*/
    soildata_1.rsoil = ((a * ggg_1.th + b * ggg_1.cp) * ggg_1.th + c__) * x2;
/*<       rr1soil = (.7337d0*a + c)*x2 >*/
    soildata_1.rr1soil = (a * .7337 + c__) * x2;
/*<       rrsoil  = 14.25d0*x2 >*/
    soildata_1.rrsoil = x2 * 14.25;

/*<       return >*/
    return 0;
/*<       end >*/
} /* soil_ */


/* ***************************************************************** */

/*<       subroutine soilspec >*/
/* Subroutine */ int soilspec_()
{

/*   Soil spectral reflectance,  Price,  RSE 33:113 - 121 (1990) */

/*<       implicit double precision (a-h, o-z) >*/
/*<       save /count/, /soildata/ >*/

/*<       dimension u1(10), u2(10), a1(10), a2(10) >*/
/*<       common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2 >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
/*<    >*/

/*<    >*/
    soildata_1.rsl = soildata_1.rsl1 * soildata_1.phis1[count_1.jl - 1] + 
	    soildata_1.rsl2 * soildata_1.phis2[count_1.jl - 1] + 
	    soildata_1.rsl3 * soildata_1.phis3[count_1.jl - 1] + 
	    soildata_1.rsl4 * soildata_1.phis4[count_1.jl - 1];

/*<       return >*/
    return 0;
/*<       end >*/
} /* soilspec_ */


/* ********************************************************************* */

/*<       subroutine gmd92 >*/
/* Subroutine */ int gmd92_()
{
    /* Initialized data */

    static doublereal pi = 3.14159265358979;
    static doublereal pi4 = 6.28318531717958;
    static doublereal pi12 = .159154943;
    static doublereal pi14 = .636619773;
    static doublereal eps5 = .001;
    static doublereal pi13 = .1061032953;

    /* Format strings */
    static char fmt_46[] = "";
    static char fmt_48[] = "";
    static char fmt_51[] = "";
    static char fmt_52[] = "";
    static char fmt_71[] = "";
    static char fmt_72[] = "";
    static char fmt_73[] = "";
    static char fmt_74[] = "";

    /* Builtin functions */
    double sqrt(doublereal), atan2(doublereal, doublereal), sin(doublereal), 
	    cos(doublereal);

    /* Local variables */
    doublereal f[5];
    integer j;
    doublereal x, y;
    integer i1, l2, l4;
    doublereal x1, y1, x2, fa, fb;
    integer ii;
    doublereal sg, pp, gr0, gr1, gt0, sg1, gt1, cfa, cfb, sfa, sfb, sgmr, 
	    sgmt;

    /* Assigned format variables */
    static char *l4_fmt, *l2_fmt;

/*  phase function and G-funktion */
/*  A. Kuusk    22.03.1988 & 16.06.1992 */
/*  0< = th,  th1, th2<=pi/2,  0<=phi<=pi */

/*<       implicit double precision (a-h, o-z) >*/
/*<       dimension f(5) >*/
/*<       save /aaa/, /ggg/, /ladak/ >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/
/*<       common /ladak/ ee, thm, sthm, cthm >*/

/*<    >*/

/*           print *, 'gmd92' */

/*                            ***  gammad,  e = 0.  *** */
/*<       gr0 = (salph + alpp*calph)*pi13 >*/
    gr0 = (ggg_1.salph + ggg_1.alpp * ggg_1.calph) * pi13;
/*<       gt0 = (salph - alph*calph)*pi13 >*/
    gt0 = (ggg_1.salph - ggg_1.alph * ggg_1.calph) * pi13;
/*<       if (ee .lt. .4d0) then >*/
    if (ladak_1.ee < .4) {
/*<          gr = gr0 >*/
	ggg_1.gr = gr0;
/*<          gt = gt0 >*/
	ggg_1.gt = gt0;
/*<          g  = .5d0 >*/
	ggg_1.g = .5;
/*<          g1 = .5d0 >*/
	ggg_1.g1 = .5;
/*<          return >*/
	return 0;
/*<       endif >*/
    }
/*                            ***  gammad,  e = 1.  *** */
/*<       sg   = 0.d0 >*/
    sg = 0.;
/*<       sg1  = 0.d0 >*/
    sg1 = 0.;
/*<       sgmr = 0.d0 >*/
    sgmr = 0.;
/*<       sgmt = 0.d0 >*/
    sgmt = 0.;
/*<       if (th22 .lt. t11) goto 47 >*/
    if (ggg_1.th22 < ggg_1.t11) {
	goto L47;
    }
/*<       assign 46 to l4 >*/
    l4 = 0;
    l4_fmt = fmt_46;
/*<       goto 61 >*/
    goto L61;

/*< 46    continue >*/
L46:
/*<       assign 48 to l4 >*/
    l4 = 1;
    l4_fmt = fmt_48;
/*<       goto 64 >*/
    goto L64;

/*< 47    continue >*/
L47:
/*<       if (th22 .lt. t10) goto 50 >*/
    if (ggg_1.th22 < ggg_1.t10) {
	goto L50;
    }
/*<       assign 51 to l4 >*/
    l4 = 2;
    l4_fmt = fmt_51;
/*<       goto 62 >*/
    goto L62;

/*< 51    continue >*/
L51:
/*<       assign 46 to l4 >*/
    l4 = 0;
/*<       goto 65 >*/
    goto L65;

/*< 50    continue >*/
L50:
/*<       assign 52 to l4 >*/
    l4 = 3;
    l4_fmt = fmt_52;
/*<       goto 63 >*/
    goto L63;

/*< 52    continue >*/
L52:
/*<       assign 48 to l4 >*/
    l4 = 1;
    l4_fmt = fmt_48;
/*<       goto 65 >*/
    goto L65;

/*< 48    continue >*/
L48:

/*<       gr1 = sgmr*pi12 >*/
    gr1 = sgmr * pi12;
/*<       gt1 = sgmt*pi12 >*/
    gt1 = sgmt * pi12;
/*<    >*/
    ggg_1.gr = gr0 - .0102 + (ggg_1.difmy * 1.742 - ggg_1.difsig * .4557) * (
	    gr1 - gr0);
/*<    >*/
    ggg_1.gt = gt0 + .00653 + (ggg_1.difmy * .2693 + ggg_1.difsig * 5.821) * (
	    gt1 - gt0);
/*<       g   = (2.653d0*difmy + 1.432d0*difsig)*(sg  - .5d0) + .50072d0 >*/
    ggg_1.g = (ggg_1.difmy * 2.653 + ggg_1.difsig * 1.432) * (sg - .5) + 
	    .50072;
/*<       g1  = (2.653d0*difmy + 1.432d0*difsig)*(sg1 - .5d0) + .50072d0 >*/
    ggg_1.g1 = (ggg_1.difmy * 2.653 + ggg_1.difsig * 1.432) * (sg1 - .5) + 
	    .50072;

/*< 49    continue >*/
/* L49: */
/*<       return >*/
    return 0;

/*  ******************************    tl1 = 0.,  tl2=pi/2 - th1 */

/*< 61    assign 71 to l2 >*/
L61:
    l2 = 0;
    l2_fmt = fmt_71;
/*<       goto 130 >*/
    goto L130;
/*< 71    y = pp >*/
L71:
    y = pp;
/*<       if (y .gt. 0.d0) sgmr = sgmr + y >*/
    if (y > 0.) {
	sgmr += y;
    }
/*<       if (y .lt. 0.d0) sgmt = sgmt - y >*/
    if (y < 0.) {
	sgmt -= y;
    }
/*<       y1  = ct1*cthm >*/
    y1 = ggg_1.ct1 * ladak_1.cthm;
/*<       sg1 = sg1 + abs(y1) >*/
    sg1 += abs(y1);
/*<       goto l4 >*/
    switch (l4) {
	case 0: goto L46;
	case 1: goto L48;
	case 2: goto L51;
	case 3: goto L52;
    }

/*  ******************************    tl1 = pi/2 - th1,  tl2=pi/2 - th */

/*< 62    continue >*/
L62:
/*<       x2 = cthm/sthm >*/
    x2 = ladak_1.cthm / ladak_1.sthm;
/*<       x  = -ctg1*x2 >*/
    x = -ggg_1.ctg1 * x2;
/*<       x1 = sqrt(1.d0 - x*x) >*/
    x1 = sqrt(1. - x * x);
/*<       fa = atan2(x1, x) >*/
    fa = atan2(x1, x);
/*<       fb = pi4 - fa >*/
    fb = pi4 - fa;
/*<       assign 72 to l2 >*/
    l2 = 1;
    l2_fmt = fmt_72;
/*<       goto 30 >*/
    goto L30;

/*< 72    continue >*/
L72:
/*<       y = pp >*/
    y = pp;
/*<       if (y .gt. 0.d0) sgmr = sgmr + y >*/
    if (y > 0.) {
	sgmr += y;
    }
/*<       if (y .lt. 0.d0) sgmt = sgmt - y >*/
    if (y < 0.) {
	sgmt -= y;
    }
/*<       assign 73 to l2 >*/
    l2 = 2;
    l2_fmt = fmt_73;
/*<       goto 130 >*/
    goto L130;

/*< 73    y = pp - y >*/
L73:
    y = pp - y;
/*<       if (y .gt. 0.d0) sgmr = sgmr + y >*/
    if (y > 0.) {
	sgmr += y;
    }
/*<       if (y .lt. 0.d0) sgmt = sgmt - y >*/
    if (y < 0.) {
	sgmt -= y;
    }
/*<       goto l4 >*/
    switch (l4) {
	case 0: goto L46;
	case 1: goto L48;
	case 2: goto L51;
	case 3: goto L52;
    }

/*  ******************************   tl1 = pi/2 - th,  tl2=pi/2 */

/*< 63    continue >*/
L63:
/*<       x2 = cthm/sthm >*/
    x2 = ladak_1.cthm / ladak_1.sthm;
/*<       x  = -ctg1*x2 >*/
    x = -ggg_1.ctg1 * x2;
/*<       x1 = sqrt(1.d0 - x*x) >*/
    x1 = sqrt(1. - x * x);
/*<       fa = atan2(x1, x) >*/
    fa = atan2(x1, x);
/*<       f(2) = fa >*/
    f[1] = fa;
/*<       f(3) = pi4 - fa >*/
    f[2] = pi4 - fa;
/*<       x  = -ctg*x2 >*/
    x = -ggg_1.ctg * x2;
/*<       x1 = sqrt(1.d0 - x*x) >*/
    x1 = sqrt(1. - x * x);
/*<       fa = atan2(x1, x) >*/
    fa = atan2(x1, x);
/*<       fb = phi - fa >*/
    fb = ggg_1.phi - fa;
/*<       if (fb .lt. 0.d0) fb = fb + pi4 >*/
    if (fb < 0.) {
	fb += pi4;
    }
/*<       f(4) = fb >*/
    f[3] = fb;
/*<       f(5) = phi + fa >*/
    f[4] = ggg_1.phi + fa;
/*<       do 75 ii = 2, 4 >*/
    for (ii = 2; ii <= 4; ++ii) {
/*<          i1 = ii + 1 >*/
	i1 = ii + 1;
/*<          do 75 j = i1, 5 >*/
	for (j = i1; j <= 5; ++j) {
/*<             fa = f(ii) >*/
	    fa = f[ii - 1];
/*<             fb = f(j) >*/
	    fb = f[j - 1];
/*<             if (fb .gt. fa) goto 75 >*/
	    if (fb > fa) {
		goto L75;
	    }
/*<             f(ii) = fb >*/
	    f[ii - 1] = fb;
/*<             f(j)  = fa >*/
	    f[j - 1] = fa;
/*< 75    continue >*/
L75:
	    ;
	}
    }
/*<       f(1) = f(5) - pi4 >*/
    f[0] = f[4] - pi4;
/*<       i1   = 1 >*/
    i1 = 1;
/*< 76    ii   = i1 >*/
L76:
    ii = i1;
/*<       i1   = ii + 1 >*/
    i1 = ii + 1;
/*<       fa   = f(ii) >*/
    fa = f[ii - 1];
/*<       fb   = f(i1) >*/
    fb = f[i1 - 1];
/*<       assign 74 to l2 >*/
    l2 = 3;
    l2_fmt = fmt_74;
/*<       goto 30 >*/
    goto L30;

/*  ******************************   tl1 = pi/2 - th,  tl2=pi/2 */

/*< 74    continue >*/
L74:
/*<       y = pp >*/
    y = pp;
/*<       if (y .gt. 0.d0) sgmr = sgmr + y >*/
    if (y > 0.) {
	sgmr += y;
    }
/*<       if (y .lt. 0.d0) sgmt = sgmt - y >*/
    if (y < 0.) {
	sgmt -= y;
    }
/*<       if (i1 .le. 4) goto 76 >*/
    if (i1 <= 4) {
	goto L76;
    }

/*<       x2 = ct*cthm >*/
    x2 = ggg_1.ct * ladak_1.cthm;
/*<       x1 = st*sthm/x2 >*/
    x1 = ggg_1.st * ladak_1.sthm / x2;
/*<       x1 = sqrt(x1*x1 - 1.d0) >*/
    x1 = sqrt(x1 * x1 - 1.);
/*<       x  = atan2(1.d0, x1) >*/
    x = atan2(1., x1);
/*<       x  = (x + x1)*x2 >*/
    x = (x + x1) * x2;
/*<       y  = x*pi14 >*/
    y = x * pi14;
/*<       sg = sg + abs(y) >*/
    sg += abs(y);
/*<       goto l4 >*/
    switch (l4) {
	case 0: goto L46;
	case 1: goto L48;
	case 2: goto L51;
	case 3: goto L52;
    }

/*  ******************************    tl1 = 0,  tl2=pi/2 - th */

/*< 64    y1 = ct*cthm >*/
L64:
    y1 = ggg_1.ct * ladak_1.cthm;
/*<       sg = sg + abs(y1) >*/
    sg += abs(y1);
/*<       goto l4 >*/
    switch (l4) {
	case 0: goto L46;
	case 1: goto L48;
	case 2: goto L51;
	case 3: goto L52;
    }

/*  ******************************    tl1 = pi/2 - th1,  tl2=pi/2 */

/*< 65    continue >*/
L65:
/*<       x2  = ct1*cthm >*/
    x2 = ggg_1.ct1 * ladak_1.cthm;
/*<       x1  = st1*sthm/x2 >*/
    x1 = ggg_1.st1 * ladak_1.sthm / x2;
/*<       x1  = sqrt(x1*x1 - 1.d0) >*/
    x1 = sqrt(x1 * x1 - 1.);
/*<       x   = atan2(1.d0, x1) >*/
    x = atan2(1., x1);
/*<       x   = (x + x1)*x2 >*/
    x = (x + x1) * x2;
/*<       y   = x*pi14 >*/
    y = x * pi14;
/*<       sg1 = sg1 + abs(y) >*/
    sg1 += abs(y);
/*<       goto l4 >*/
    switch (l4) {
	case 0: goto L46;
	case 1: goto L48;
	case 2: goto L51;
	case 3: goto L52;
    }

/*  ******************************    p(fa, fb) */

/*< 30    x  = fb - fa >*/
L30:
    x = fb - fa;
/*<       if (x .gt. eps5) goto 31 >*/
    if (x > eps5) {
	goto L31;
    }
/*<       pp = 0.d0 >*/
    pp = 0.;
/*<       goto l2 >*/
    switch (l2) {
	case 0: goto L71;
	case 1: goto L72;
	case 2: goto L73;
	case 3: goto L74;
    }
/*< 31    if ((pi4 - x) .lt. eps5) goto 130 >*/
L31:
    if (pi4 - x < eps5) {
	goto L130;
    }
/*<       sfa = sin(fa) >*/
    sfa = sin(fa);
/*<       sfb = sin(fb) >*/
    sfb = sin(fb);
/*<       cfa = cos(fa) >*/
    cfa = cos(fa);
/*<       cfb = cos(fb) >*/
    cfb = cos(fb);
/*<       pp  = x*ctt1*cthm*cthm >*/
    pp = x * ggg_1.ctt1 * ladak_1.cthm * ladak_1.cthm;
/*<       y1  = x + sfb*cfb - sfa*cfa >*/
    y1 = x + sfb * cfb - sfa * cfa;
/*<       x   = cfa - cfb >*/
    x = cfa - cfb;
/*<       y1  = y1*cp + sp*x*(cfa + cfb) >*/
    y1 = y1 * ggg_1.cp + ggg_1.sp * x * (cfa + cfb);
/*<       pp  = pp + stt1*.5d0*y1*sthm*sthm >*/
    pp += ggg_1.stt1 * .5 * y1 * ladak_1.sthm * ladak_1.sthm;
/*<       y1  = s2*(sfb - sfa) + s3*x >*/
    y1 = ggg_1.s2 * (sfb - sfa) + ggg_1.s3 * x;
/*<       pp  = pp + y1*sthm*cthm >*/
    pp += y1 * ladak_1.sthm * ladak_1.cthm;
/*<       goto l2 >*/
    switch (l2) {
	case 0: goto L71;
	case 1: goto L72;
	case 2: goto L73;
	case 3: goto L74;
    }

/*< 130   x  = sthm*sthm >*/
L130:
    x = ladak_1.sthm * ladak_1.sthm;
/*<       pp = calph*x + ctt1*(2.d0 - 3.d0*x) >*/
    pp = ggg_1.calph * x + ggg_1.ctt1 * (2. - x * 3.);
/*<       pp = pp*pi >*/
    pp *= pi;
/*<       goto l2 >*/
    switch (l2) {
	case 0: goto L71;
	case 1: goto L72;
	case 2: goto L73;
	case 3: goto L74;
    }
    return 0;
/*<       end >*/
} /* gmd92_ */


/* ***************************************************************** */


/*     ****************************************************************** */
/*     leaf reflectance and transmittance. */
/*     Input data are refractive index n,  a structure parameter N */
/*     and an absorption coefficient k: */
/*     the PROSPECT model,  Jacquemoud & Baret,  RSE 34:75-91 (1990) */
/*     ****************************************************************** */
/*<       subroutine leaf >*/
/* Subroutine */ int leaf_()
{
    /* System generated locals */
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double exp(doublereal), sqrt(doublereal), pow_dd(doublereal *, doublereal 
	    *);

    /* Local variables */
    doublereal r__, t, s1, t1, t2, s2, s3, x1, x2, x3, x4, x5, x6, ra, ta, va,
	     vb;
    extern /* Subroutine */ int tav_();
    doublereal alfa, beta;
    extern /* Subroutine */ int s13aaf_();
    doublereal delta;


/*<       implicit double precision (a-h, o-z) >*/

/*<       double precision nn, k, inex >*/
/*<       common /leafin/ nn, vai, k >*/
/*<       common /leafout/ refl, tran >*/
/*<       common /nagout/ inex >*/
/*<       common /tauin/ teta, ref >*/
/*<       common /tauout/ tau >*/
/*     ****************************************************************** 
*/
/*     determination of elementary reflectances et transmittances */
/*     ****************************************************************** 
*/
/*     ALLEN et al.,  1969,  Interaction of isotropic ligth with a compact
 */
/*     plant leaf,  J. Opt. Soc. Am.,  Vol.59,  10:1376-1379 */
/*     JACQUEMOUD S. and BARET F.,  1990,  Prospect : a model of leaf */
/*     optical properties spectra,  Remote Sens. Environ.,  34:75-91 */
/*     ****************************************************************** 
*/
/*                     print *, 'leaf' */
/*<       if (k .le. 0.d0) then >*/
    if (leafin_2.k <= 0.) {
/*<          k = 1.d0 >*/
	leafin_2.k = 1.;
/*<       else >*/
    } else {
/*<          call s13aaf >*/
	s13aaf_();
/*<          k = (1.d0 - k)*exp(-k) + k**2*inex >*/
/* Computing 2nd power */
	d__1 = leafin_2.k;
	leafin_2.k = (1. - leafin_2.k) * exp(-leafin_2.k) + d__1 * d__1 * 
		nagout_1.inex__;
/*<       endif >*/
    }
/*<       teta = 90.d0 >*/
    tauin_1.teta = 90.;
/*<       ref  = nn >*/
    tauin_1.ref = leafin_2.nn;

/*<       call tav >*/
    tav_();

/*<       t1   = tau >*/
    t1 = tauout_1.tau;
/*<       teta = 59.d0 >*/
    tauin_1.teta = 59.;

/*<       call tav >*/
    tav_();

/*<       t2 = tau >*/
    t2 = tauout_1.tau;
/*<       x1 = 1.d0 - t1 >*/
    x1 = 1. - t1;
/*<       x2 = t1**2*k**2*(nn**2 - t1) >*/
/* Computing 2nd power */
    d__1 = t1;
/* Computing 2nd power */
    d__2 = leafin_2.k;
/* Computing 2nd power */
    d__3 = leafin_2.nn;
    x2 = d__1 * d__1 * (d__2 * d__2) * (d__3 * d__3 - t1);
/*<       x3 = t1**2*k*nn**2 >*/
/* Computing 2nd power */
    d__1 = t1;
/* Computing 2nd power */
    d__2 = leafin_2.nn;
    x3 = d__1 * d__1 * leafin_2.k * (d__2 * d__2);
/*<       x4 = nn**4 - k**2*(nn**2 - t1)**2 >*/
/* Computing 4th power */
    d__1 = leafin_2.nn, d__1 *= d__1;
/* Computing 2nd power */
    d__2 = leafin_2.k;
/* Computing 2nd power */
    d__4 = leafin_2.nn;
/* Computing 2nd power */
    d__3 = d__4 * d__4 - t1;
    x4 = d__1 * d__1 - d__2 * d__2 * (d__3 * d__3);
/*<       x5 = t2/t1 >*/
    x5 = t2 / t1;
/*<       x6 = x5*(t1 - 1.d0) + 1.d0 - t2 >*/
    x6 = x5 * (t1 - 1.) + 1. - t2;
/*<       r  = x1 + x2/x4 >*/
    r__ = x1 + x2 / x4;
/*<       t  = x3/x4 >*/
    t = x3 / x4;
/*<       ra = x5*r + x6 >*/
    ra = x5 * r__ + x6;
/*<       ta = x5*t >*/
    ta = x5 * t;
/*     ****************************************************************** 
*/
/*     reflectances et transmittances corresponding to N elementary */
/*     layers */
/*     ****************************************************************** 
*/
/*    STOKES G.G.,  1862,  On the intensity of the light reflected from or
*/
/*     transmitted through a pile of plates,  Proceedings of the Royal */
/*     Society of London,  Vol.11,  545-556 */
/*     ****************************************************************** 
*/
/*<       delta = (t**2 - r**2 - 1.d0)**2 - 4.d0*r**2 >*/
/* Computing 2nd power */
    d__2 = t;
/* Computing 2nd power */
    d__3 = r__;
/* Computing 2nd power */
    d__1 = d__2 * d__2 - d__3 * d__3 - 1.;
/* Computing 2nd power */
    d__4 = r__;
    delta = d__1 * d__1 - d__4 * d__4 * 4.;
/*<       alfa  = (1.d0 + r**2 - t**2 + sqrt(delta))/(2.d0*r) >*/
/* Computing 2nd power */
    d__1 = r__;
/* Computing 2nd power */
    d__2 = t;
    alfa = (d__1 * d__1 + 1. - d__2 * d__2 + sqrt(delta)) / (r__ * 2.);
/*<       beta  = (1.d0 + r**2 - t**2 - sqrt(delta))/(2.d0*r) >*/
/* Computing 2nd power */
    d__1 = r__;
/* Computing 2nd power */
    d__2 = t;
    beta = (d__1 * d__1 + 1. - d__2 * d__2 - sqrt(delta)) / (r__ * 2.);
/*<       va    = (1.d0 + r**2 - t**2 + sqrt(delta))/(2.d0*r) >*/
/* Computing 2nd power */
    d__1 = r__;
/* Computing 2nd power */
    d__2 = t;
    va = (d__1 * d__1 + 1. - d__2 * d__2 + sqrt(delta)) / (r__ * 2.);
/*<       vb    = sqrt(beta*(alfa - r)/(alfa*(beta - r))) >*/
    vb = sqrt(beta * (alfa - r__) / (alfa * (beta - r__)));
/*<    >*/
    d__1 = leafin_2.vai - 1.;
    d__2 = -(leafin_2.vai - 1.);
    d__3 = leafin_2.vai - 1.;
    d__4 = -(leafin_2.vai - 1.);
    s1 = ra * (va * pow_dd(&vb, &d__1) - pow_dd(&va, &c_b46) * pow_dd(&vb, &
	    d__2)) + (ta * t - ra * r__) * (pow_dd(&vb, &d__3) - pow_dd(&vb, &
	    d__4));
/*<       s2    = ta*(va - va**(-1.d0)) >*/
    s2 = ta * (va - pow_dd(&va, &c_b46));
/*<    >*/
    d__1 = leafin_2.vai - 1.;
    d__2 = -(leafin_2.vai - 1.);
    d__3 = leafin_2.vai - 1.;
    d__4 = -(leafin_2.vai - 1.);
    s3 = va * pow_dd(&vb, &d__1) - pow_dd(&va, &c_b46) * pow_dd(&vb, &d__2) - 
	    r__ * (pow_dd(&vb, &d__3) - pow_dd(&vb, &d__4));
/*<       refl  = s1/s3 >*/
    leafout_1.refl = s1 / s3;
/*<       tran  = s2/s3 >*/
    leafout_1.tran = s2 / s3;

/*<       return >*/
    return 0;
/*<       end >*/
} /* leaf_ */

/*     ****************************************************************** */
/*     exponential integral: int(exp(-t)/t, t = x..inf) */
/*     ****************************************************************** */
/*<       subroutine s13aaf >*/
/* Subroutine */ int s13aaf_()
{
    /* Builtin functions */
    double log(doublereal), exp(doublereal);

    /* Local variables */
    doublereal x, y;


/*<       implicit double precision (a-h, o-z) >*/

/*<       double precision nn, k, inex >*/
/*<       common /leafin/ nn, vai, k >*/
/*<       common /nagout/ inex >*/
/*                     print *, 's13aafin' */
/*<       if (k .gt. 4.d0) goto 10 >*/
    if (leafin_2.k > 4.) {
	goto L10;
    }
/*<       x  =  0.5d0 * k  -  1.d0 >*/
    x = leafin_2.k * .5 - 1.;
/*<    >*/
    y = (((((((((((((((x * -3.60311230482612224e-13 + 3.46348526554087424e-12)
	     * x - 2.99627399604128973e-11) * x + 2.57747807106988589e-10) * 
	    x - 2.09330568435488303e-9) * x + 1.59501329936987818e-8) * x - 
	    1.13717900285428895e-7) * x + 7.55292885309152956e-7) * x - 
	    4.64980751480619431e-6) * x + 2.63830365675408129e-5) * x - 
	    1.37089870978830576e-4) * x + 6.476865037281034e-4) * x - 
	    .00276060141343627983) * x + .0105306034687449505) * x - 
	    .0357191348753631956) * x + .107774527938978692) * x - 
	    .296997075145080963;
/*<       y  =  (y*x + 8.64664716763387311d-1)*x  +  7.42047691268006429d-1 >*/
    y = (y * x + .864664716763387311) * x + .742047691268006429;
/*<       inex  =  y  -  log(k) >*/
    nagout_1.inex__ = y - log(leafin_2.k);
/*<       goto 30 >*/
    goto L30;
/*< 10    if (k .ge. 85.d0) go to 20 >*/
L10:
    if (leafin_2.k >= 85.) {
	goto L20;
    }
/*<       x  =  14.5d0 / (k + 3.25d0)  -  1.d0 >*/
    x = 14.5 / (leafin_2.k + 3.25) - 1.;
/*<    >*/
    y = (((((((((((((((x * -1.62806570868460749e-12 - 8.95400579318284288e-13)
	     * x - 4.08352702838151578e-12) * x - 1.45132988248537498e-11) * 
	    x - 8.35086918940757852e-11) * x - 2.13638678953766289e-10) * x - 
	    1.1030243146706977e-9) * x - 3.67128915633455484e-9) * x - 
	    1.66980544304104726e-8) * x - 6.11774386401295125e-8) * x - 
	    2.70306163610271497e-7) * x - 1.05565006992891261e-6) * x - 
	    4.72090467203711484e-6) * x - 1.95076375089955937e-5) * x - 
	    9.16450482931221453e-5) * x - 4.05892130452128677e-4) * x - 
	    .00214213055000334718;
/*<    >*/
    y = ((y * x - .0106374875116569657) * x - .0850699154984571871) * x + 
	    .923755307807784058;
/*<       inex  =  exp(-k) * y / k >*/
    nagout_1.inex__ = exp(-leafin_2.k) * y / leafin_2.k;
/*<       goto 30 >*/
    goto L30;
/*< 20    inex  =  0.d0 >*/
L20:
    nagout_1.inex__ = 0.;
/*<       goto 30 >*/
    goto L30;
/*< 30    continue >*/
L30:
/*                     print *, 's13aafout' */
/*<       return >*/
    return 0;
/*<       end >*/
} /* s13aaf_ */

/*     ****************************************************************** */
/*     determination of tav for any solid angle */
/*     ****************************************************************** */
/*     STERN F.,  1964,  Transmission of isotropic radiation across an */
/*     interface between two dielectrics,  Appl.Opt.,  Vol.3,  1:111-113 */
/*     ALLEN W.A.,  1973,  Transmission of isotropic light across a */
/*     dielectric surface in two and three dimensions,  J.Opt.Soc.Am., */
/*     Vol.63,  6:664-666 */
/*     ****************************************************************** */
/*<       subroutine tav >*/
/* Subroutine */ int tav_()
{
    /* Initialized data */

    static doublereal dr = .0174532925199433;
    static doublereal eps = 1e-7;
    static doublereal pi12 = 1.570796326794895;

    /* System generated locals */
    doublereal d__1, d__2, d__3, d__4, d__5, d__6, d__7;

    /* Builtin functions */
    double sin(doublereal), sqrt(doublereal), log(doublereal);

    /* Local variables */
    doublereal a, b, k, b1, b2, r2, ds, rm, rp, tp, ts, tp1, tp2, tp3, tp4, 
	    tp5, xxx;


/*<       implicit double precision (a-h, o-z) >*/
/*<       double precision k >*/

/*<       common /tauin/ teta, ref >*/
/*<       common /tauout/ tau >*/

/*<    >*/
/*                     print *, 'tavin' */
/*<       teta = teta*dr >*/
    tauin_1.teta *= dr;
/*<       r2   = ref**2 >*/
/* Computing 2nd power */
    d__1 = tauin_1.ref;
    r2 = d__1 * d__1;
/*<       rp   = r2 + 1.d0 >*/
    rp = r2 + 1.;
/*<       rm   = r2 - 1.d0 >*/
    rm = r2 - 1.;
/*<       a    = (ref + 1.d0)**2/2.d0 >*/
/* Computing 2nd power */
    d__1 = tauin_1.ref + 1.;
    a = d__1 * d__1 / 2.;
/*<       k    = -(r2 - 1.d0)**2/4.d0 >*/
/* Computing 2nd power */
    d__1 = r2 - 1.;
    k = -(d__1 * d__1) / 4.;
/*<       ds   = sin(teta) >*/
    ds = sin(tauin_1.teta);
/*<       if (abs(teta) .le. eps) then >*/
    if (abs(tauin_1.teta) <= eps) {
/*<          tau = 4.d0*ref/(ref + 1.d0)**2 >*/
/* Computing 2nd power */
	d__1 = tauin_1.ref + 1.;
	tauout_1.tau = tauin_1.ref * 4. / (d__1 * d__1);
/*<       else >*/
    } else {
/*<          if (abs(teta - pi12) .le. eps) then >*/
	if ((d__1 = tauin_1.teta - pi12, abs(d__1)) <= eps) {
/*<             b1 = 0.d0 >*/
	    b1 = 0.;
/*<          else >*/
	} else {
/*<             xxx = (ds**2 - rp/2.d0)**2 + k >*/
/* Computing 2nd power */
	    d__2 = ds;
/* Computing 2nd power */
	    d__1 = d__2 * d__2 - rp / 2.;
	    xxx = d__1 * d__1 + k;
/*<             b1 = sqrt(xxx) >*/
	    b1 = sqrt(xxx);
/*<          endif >*/
	}
/*<          b2 = ds**2 - rp/2.d0 >*/
/* Computing 2nd power */
	d__1 = ds;
	b2 = d__1 * d__1 - rp / 2.;
/*<          b  = b1 - b2 >*/
	b = b1 - b2;
/*<    >*/
/* Computing 2nd power */
	d__1 = k;
/* Computing 3rd power */
	d__2 = b, d__3 = d__2;
/* Computing 2nd power */
	d__4 = k;
/* Computing 3rd power */
	d__5 = a, d__6 = d__5;
	ts = d__1 * d__1 / (d__3 * (d__2 * d__2) * 6.) + k / b - b / 2. - (
		d__4 * d__4 / (d__6 * (d__5 * d__5) * 6.) + k / a - a / 2.);
/*<          tp1 = -2.d0*r2*(b - a)/rp**2 >*/
/* Computing 2nd power */
	d__1 = rp;
	tp1 = r2 * -2. * (b - a) / (d__1 * d__1);
/*<          tp2 = -2.d0*r2*rp*log(b/a)/rm**2 >*/
/* Computing 2nd power */
	d__1 = rm;
	tp2 = r2 * -2. * rp * log(b / a) / (d__1 * d__1);
/*<          tp3 = r2*(1.d0/b - 1.d0/a)/2.d0 >*/
	tp3 = r2 * (1. / b - 1. / a) / 2.;
/*<    >*/
/* Computing 2nd power */
	d__1 = r2;
/* Computing 2nd power */
	d__2 = r2;
/* Computing 2nd power */
	d__3 = rm;
/* Computing 2nd power */
	d__4 = rm;
/* Computing 3rd power */
	d__5 = rp, d__6 = d__5;
/* Computing 2nd power */
	d__7 = rm;
	tp4 = d__1 * d__1 * 16. * (d__2 * d__2 + 1.) * log((rp * 2. * b - 
		d__3 * d__3) / (rp * 2. * a - d__4 * d__4)) / (d__6 * (d__5 * 
		d__5) * (d__7 * d__7));
/*<    >*/
/* Computing 3rd power */
	d__1 = r2, d__2 = d__1;
/* Computing 2nd power */
	d__3 = rm;
/* Computing 2nd power */
	d__4 = rm;
/* Computing 3rd power */
	d__5 = rp, d__6 = d__5;
	tp5 = d__2 * (d__1 * d__1) * 16. * (1. / (rp * 2. * b - d__3 * d__3) 
		- 1. / (rp * 2. * a - d__4 * d__4)) / (d__6 * (d__5 * d__5));
/*<          tp  = tp1 + tp2 + tp3 + tp4 + tp5 >*/
	tp = tp1 + tp2 + tp3 + tp4 + tp5;
/*<          tau = (ts + tp)/(2.d0*ds**2) >*/
/* Computing 2nd power */
	d__1 = ds;
	tauout_1.tau = (ts + tp) / (d__1 * d__1 * 2.);
/*<       endif >*/
    }
/*                     print *, 'tavout' */
/*<       return >*/
    return 0;
/*<       end >*/
} /* tav_ */


/* ***************************************************************** */

/*     constant values: refractive index (ref), albino and dry leaf */
/*     absorption (ke), chlorophyll a+b specific absorption coefficient */
/*     (kab), water specific absorption coefficient (kw), */
/*     and basis functions for soil spectral reflectance phis1, phis2, */
/*     phis3 and phis4 (Price, 1990) */
/*     ****************************************************************** */
/*     JACQUEMOUD S. AND BARET F., 1990, Prospect : a model of leaf */
/*     optical properties spectra, Remote Sens. Environ., 34:75-91 */
/*     JACQUEMOUD S. et al., 1991, Validation d'un modele de reflectance */
/*     spectrale et directionnnelle de sol, 5ieme Colloque International */
/*     Mesures Physiques et Signatures en Teledetection, Courchevel */
/*     (France), 14-18 Janvier 1991 */
/*     ****************************************************************** */
/*<       block data valeur >*/
/* Subroutine */ int valeur_()
{
    return 0;
} /* valeur_ */


/*<       implicit double precision (a-h, o-z) >*/

/*<       double precision ke, kab, kw >*/
/*<       dimension ref(200), ke(200), kab(200), kw(200) >*/
/*<       common /dat/ ref, ke, kab, kw >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
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
/*<        end >*/


/* ***************************************************************** */

/*<       subroutine dakg(u, a, nq) >*/
/* Subroutine */ int dakg_(doublereal *u, doublereal *a, integer *nq)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    integer i__, n, ii, nq1;

    /* Fortran I/O blocks */
    static cilist io___202 = { 0, 6, 0, 0, 0 };


/* Gaussi kvadratuuri sqlmed ja kordajad, nq = 2*n, u=(-1., 1.) */
/*<       implicit double precision (a-h, o-z) >*/
/*<       dimension u(48), a(48) >*/

/*              print *,'dakg' */
/*<       n = nq/2 >*/
    /* Parameter adjustments */
    --a;
    --u;

    /* Function Body */
    n = *nq / 2;
/*<    >*/
    switch (*nq) {
	case 1:  goto L1;
	case 2:  goto L2;
	case 3:  goto L1;
	case 4:  goto L4;
	case 5:  goto L1;
	case 6:  goto L6;
	case 7:  goto L1;
	case 8:  goto L8;
	case 9:  goto L1;
	case 10:  goto L10;
	case 11:  goto L1;
	case 12:  goto L12;
	case 13:  goto L1;
	case 14:  goto L14;
	case 15:  goto L1;
	case 16:  goto L16;
	case 17:  goto L1;
	case 18:  goto L1;
	case 19:  goto L1;
	case 20:  goto L20;
	case 21:  goto L1;
	case 22:  goto L1;
	case 23:  goto L1;
	case 24:  goto L1;
	case 25:  goto L1;
	case 26:  goto L1;
	case 27:  goto L1;
	case 28:  goto L1;
	case 29:  goto L1;
	case 30:  goto L1;
	case 31:  goto L1;
	case 32:  goto L1;
	case 33:  goto L1;
	case 34:  goto L1;
	case 35:  goto L1;
	case 36:  goto L1;
	case 37:  goto L1;
	case 38:  goto L1;
	case 39:  goto L1;
	case 40:  goto L1;
	case 41:  goto L1;
	case 42:  goto L1;
	case 43:  goto L1;
	case 44:  goto L1;
	case 45:  goto L1;
	case 46:  goto L1;
	case 47:  goto L1;
	case 48:  goto L48;
    }
/*< 1     continue >*/
L1:
/*<       print *,  ' ***   dakg - inacceptable nq' >*/
    s_wsle(&io___202);
    do_lio(&c__9, &c__1, " ***   dakg - inacceptable nq", 29L);
    e_wsle();
/*<       stop 'dakg' >*/
    s_stop("dakg", 4L);

/*< 2     continue >*/
L2:
/*<       u(2) = .577350269189626d0 >*/
    u[2] = .577350269189626;
/*<       a(2) = 1.d0 >*/
    a[2] = 1.;
/*<       goto 13 >*/
    goto L13;

/*< 4     continue >*/
L4:
/*<       u(3) = .339981043584856d0 >*/
    u[3] = .339981043584856;
/*<       u(4) = .861136311594053d0 >*/
    u[4] = .861136311594053;
/*<       a(3) = .652145154862546d0 >*/
    a[3] = .652145154862546;
/*<       a(4) = .347854845137454d0 >*/
    a[4] = .347854845137454;
/*<       goto 13 >*/
    goto L13;

/*< 6     continue >*/
L6:
/*<       u(4) = .238619186083197d0 >*/
    u[4] = .238619186083197;
/*<       u(5) = .661209386466265d0 >*/
    u[5] = .661209386466265;
/*<       u(6) = .932469514203152d0 >*/
    u[6] = .932469514203152;
/*<       a(4) = .467913934572691d0 >*/
    a[4] = .467913934572691;
/*<       a(5) = .360761573048139d0 >*/
    a[5] = .360761573048139;
/*<       a(6) = .171324492379170d0 >*/
    a[6] = .17132449237917;
/*<       goto 13 >*/
    goto L13;

/*< 8     continue >*/
L8:
/*<       u(5) = .183434642495650d0 >*/
    u[5] = .18343464249565;
/*<       u(6) = .525532409916329d0 >*/
    u[6] = .525532409916329;
/*<       u(7) = .796666477413627d0 >*/
    u[7] = .796666477413627;
/*<       u(8) = .960289856497536d0 >*/
    u[8] = .960289856497536;
/*<       a(5) = .362683783378362d0 >*/
    a[5] = .362683783378362;
/*<       a(6) = .313706645877887d0 >*/
    a[6] = .313706645877887;
/*<       a(7) = .222381034453374d0 >*/
    a[7] = .222381034453374;
/*<       a(8) = .101228536290376d0 >*/
    a[8] = .101228536290376;
/*<       goto 13 >*/
    goto L13;

/*< 10    continue >*/
L10:
/*<       u(6)  = .148874338981631d0 >*/
    u[6] = .148874338981631;
/*<       u(7)  = .433395394129247d0 >*/
    u[7] = .433395394129247;
/*<       u(8)  = .679409568299024d0 >*/
    u[8] = .679409568299024;
/*<       u(9)  = .865063366688985d0 >*/
    u[9] = .865063366688985;
/*<       u(10) = .973906528517172d0 >*/
    u[10] = .973906528517172;
/*<       a(6)  = .295524224714753d0 >*/
    a[6] = .295524224714753;
/*<       a(7)  = .269266719309996d0 >*/
    a[7] = .269266719309996;
/*<       a(8)  = .219086362515982d0 >*/
    a[8] = .219086362515982;
/*<       a(9)  = .149451349150580d0 >*/
    a[9] = .14945134915058;
/*<       a(10) = .666713443086881d-1 >*/
    a[10] = .0666713443086881;
/*<       goto 13 >*/
    goto L13;

/*< 12    continue >*/
L12:
/*<       u(7)  = .125233408511469d0 >*/
    u[7] = .125233408511469;
/*<       u(8)  = .367831498998180d0 >*/
    u[8] = .36783149899818;
/*<       u(9)  = .587317954286617d0 >*/
    u[9] = .587317954286617;
/*<       u(10) = .769902674194305d0 >*/
    u[10] = .769902674194305;
/*<       u(11) = .904117256370475d0 >*/
    u[11] = .904117256370475;
/*<       u(12) = .981560634246719d0 >*/
    u[12] = .981560634246719;
/*<       a(7)  = .249147045813402d0 >*/
    a[7] = .249147045813402;
/*<       a(8)  = .233492536538355d0 >*/
    a[8] = .233492536538355;
/*<       a(9)  = .203167426723066d0 >*/
    a[9] = .203167426723066;
/*<       a(10) = .160078328543346d0 >*/
    a[10] = .160078328543346;
/*<       a(11) = .106939325995318d0 >*/
    a[11] = .106939325995318;
/*<       a(12) = .471753363865118d-1 >*/
    a[12] = .0471753363865118;
/*<       goto 13 >*/
    goto L13;

/*< 14    continue >*/
L14:
/*<       u( 8) = .108054948707344d0 >*/
    u[8] = .108054948707344;
/*<       u( 9) = .319112368927890d0 >*/
    u[9] = .31911236892789;
/*<       u(10) = .515248636358154d0 >*/
    u[10] = .515248636358154;
/*<       u(11) = .687292904811685d0 >*/
    u[11] = .687292904811685;
/*<       u(12) = .827201315069765d0 >*/
    u[12] = .827201315069765;
/*<       u(13) = .928434883663574d0 >*/
    u[13] = .928434883663574;
/*<       u(14) = .986283808696812d0 >*/
    u[14] = .986283808696812;
/*<       a( 8) = .215263853463158d0 >*/
    a[8] = .215263853463158;
/*<       a( 9) = .205198463721296d0 >*/
    a[9] = .205198463721296;
/*<       a(10) = .185538397477938d0 >*/
    a[10] = .185538397477938;
/*<       a(11) = .157203167158194d0 >*/
    a[11] = .157203167158194;
/*<       a(12) = .121518570687903d0 >*/
    a[12] = .121518570687903;
/*<       a(13) = .801580871597602d-1 >*/
    a[13] = .0801580871597602;
/*<       a(14) = .351194603317519d-1 >*/
    a[14] = .0351194603317519;
/*<       goto 13 >*/
    goto L13;

/*< 16    continue >*/
L16:
/*<       u( 9) = .950125098376374d-1 >*/
    u[9] = .0950125098376374;
/*<       u(10) = .281603550779259d0 >*/
    u[10] = .281603550779259;
/*<       u(11) = .458016777657227d0 >*/
    u[11] = .458016777657227;
/*<       u(12) = .617876244402643d0 >*/
    u[12] = .617876244402643;
/*<       u(13) = .755404408355003d0 >*/
    u[13] = .755404408355003;
/*<       u(14) = .865631202387832d0 >*/
    u[14] = .865631202387832;
/*<       u(15) = .944575023073233d0 >*/
    u[15] = .944575023073233;
/*<       u(16) = .989400934991650d0 >*/
    u[16] = .98940093499165;
/*<       a( 9) = .189450610455068d0 >*/
    a[9] = .189450610455068;
/*<       a(10) = .182603415044924d0 >*/
    a[10] = .182603415044924;
/*<       a(11) = .169156519395003d0 >*/
    a[11] = .169156519395003;
/*<       a(12) = .149595988816577d0 >*/
    a[12] = .149595988816577;
/*<       a(13) = .124628971255534d0 >*/
    a[13] = .124628971255534;
/*<       a(14) = .951585116824928d-1 >*/
    a[14] = .0951585116824928;
/*<       a(15) = .622535239386479d-1 >*/
    a[15] = .0622535239386479;
/*<       a(16) = .271524594117541d-1 >*/
    a[16] = .0271524594117541;
/*<       goto 13 >*/
    goto L13;

/*< 20    continue >*/
L20:
/*<       u(11) = .765265211334973d-1 >*/
    u[11] = .0765265211334973;
/*<       u(12) = .227785851141645d0 >*/
    u[12] = .227785851141645;
/*<       u(13) = .373706088715420d0 >*/
    u[13] = .37370608871542;
/*<       u(14) = .510867001950827d0 >*/
    u[14] = .510867001950827;
/*<       u(15) = .636053680726515d0 >*/
    u[15] = .636053680726515;
/*<       u(16) = .746331906460151d0 >*/
    u[16] = .746331906460151;
/*<       u(17) = .839116971822219d0 >*/
    u[17] = .839116971822219;
/*<       u(18) = .912234428251326d0 >*/
    u[18] = .912234428251326;
/*<       u(19) = .963971927277914d0 >*/
    u[19] = .963971927277914;
/*<       u(20) = .993128599185095d0 >*/
    u[20] = .993128599185095;
/*<       a(11) = .152753387130726d0 >*/
    a[11] = .152753387130726;
/*<       a(12) = .149172986472604d0 >*/
    a[12] = .149172986472604;
/*<       a(13) = .142096109318382d0 >*/
    a[13] = .142096109318382;
/*<       a(14) = .131688638449177d0 >*/
    a[14] = .131688638449177;
/*<       a(15) = .118194531961518d0 >*/
    a[15] = .118194531961518;
/*<       a(16) = .101930119817240d0 >*/
    a[16] = .10193011981724;
/*<       a(17) = .832767415767047d-1 >*/
    a[17] = .0832767415767047;
/*<       a(18) = .626720483341091d-1 >*/
    a[18] = .0626720483341091;
/*<       a(19) = .406014298003869d-1 >*/
    a[19] = .0406014298003869;
/*<       a(20) = .176140071391521d-1 >*/
    a[20] = .0176140071391521;
/*<       goto 13 >*/
    goto L13;

/*< 48    continue >*/
L48:
/*<       u(25) = .323801709628694d-1 >*/
    u[25] = .0323801709628694;
/*<       u(26) = .970046992094627d-1 >*/
    u[26] = .0970046992094627;
/*<       u(27) = .161222356068892d0 >*/
    u[27] = .161222356068892;
/*<       u(28) = .224763790394689d0 >*/
    u[28] = .224763790394689;
/*<       u(29) = .287362487355456d0 >*/
    u[29] = .287362487355456;
/*<       u(30) = .348755886292161d0 >*/
    u[30] = .348755886292161;
/*<       u(31) = .408686481990717d0 >*/
    u[31] = .408686481990717;
/*<       u(32) = .466902904750958d0 >*/
    u[32] = .466902904750958;
/*<       u(33) = .523160974722233d0 >*/
    u[33] = .523160974722233;
/*<       u(34) = .577224726083973d0 >*/
    u[34] = .577224726083973;
/*<       u(35) = .628867396776514d0 >*/
    u[35] = .628867396776514;
/*<       u(36) = .677872379632664d0 >*/
    u[36] = .677872379632664;
/*<       u(37) = .724034130923815d0 >*/
    u[37] = .724034130923815;
/*<       u(38) = .767159032515740d0 >*/
    u[38] = .76715903251574;
/*<       u(39) = .807066204029443d0 >*/
    u[39] = .807066204029443;
/*<       u(40) = .843588261624394d0 >*/
    u[40] = .843588261624394;
/*<       u(41) = .876572020274247d0 >*/
    u[41] = .876572020274247;
/*<       u(42) = .905879136715570d0 >*/
    u[42] = .90587913671557;
/*<       u(43) = .931386690706554d0 >*/
    u[43] = .931386690706554;
/*<       u(44) = .952987703160431d0 >*/
    u[44] = .952987703160431;
/*<       u(45) = .970591592546247d0 >*/
    u[45] = .970591592546247;
/*<       u(46) = .984124583722827d0 >*/
    u[46] = .984124583722827;
/*<       u(47) = .993530172266351d0 >*/
    u[47] = .993530172266351;
/*<       u(48) = .998771007252426d0 >*/
    u[48] = .998771007252426;
/*<       a(25) = .647376968126839d-1 >*/
    a[25] = .0647376968126839;
/*<       a(26) = .644661644359501d-1 >*/
    a[26] = .0644661644359501;
/*<       a(27) = .639242385846482d-1 >*/
    a[27] = .0639242385846482;
/*<       a(28) = .631141922862540d-1 >*/
    a[28] = .063114192286254;
/*<       a(29) = .620394231598927d-1 >*/
    a[29] = .0620394231598927;
/*<       a(30) = .607044391658939d-1 >*/
    a[30] = .0607044391658939;
/*<       a(31) = .591148396983956d-1 >*/
    a[31] = .0591148396983956;
/*<       a(32) = .572772921004032d-1 >*/
    a[32] = .0572772921004032;
/*<       a(33) = .551995036999842d-1 >*/
    a[33] = .0551995036999842;
/*<       a(34) = .528901894851937d-1 >*/
    a[34] = .0528901894851937;
/*<       a(35) = .503590355538545d-1 >*/
    a[35] = .0503590355538545;
/*<       a(36) = .476166584924905d-1 >*/
    a[36] = .0476166584924905;
/*<       a(37) = .446745608566943d-1 >*/
    a[37] = .0446745608566943;
/*<       a(38) = .415450829434647d-1 >*/
    a[38] = .0415450829434647;
/*<       a(39) = .382413510658307d-1 >*/
    a[39] = .0382413510658307;
/*<       a(40) = .347772225647704d-1 >*/
    a[40] = .0347772225647704;
/*<       a(41) = .311672278327981d-1 >*/
    a[41] = .0311672278327981;
/*<       a(42) = .274265097083569d-1 >*/
    a[42] = .0274265097083569;
/*<       a(43) = .235707608393244d-1 >*/
    a[43] = .0235707608393244;
/*<       a(44) = .196161604573555d-1 >*/
    a[44] = .0196161604573555;
/*<       a(45) = .155793157229438d-1 >*/
    a[45] = .0155793157229438;
/*<       a(46) = .114772345792345d-1 >*/
    a[46] = .0114772345792345;
/*<       a(47) = .732755390127626d-2 >*/
    a[47] = .00732755390127626;
/*<       a(48) = .315334605230584d-2 >*/
    a[48] = .00315334605230584;
/*< 13    continue >*/
L13:

/*<       nq1 = nq+1 >*/
    nq1 = *nq + 1;
/*<       do 15 i = 1,n >*/
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          ii   = nq1-i >*/
	ii = nq1 - i__;
/*<          u(i) = -u(ii) >*/
	u[i__] = -u[ii];
/*<          a(i) = a(ii) >*/
	a[i__] = a[ii];
/*< 15    continue >*/
/* L15: */
    }

/*<       return >*/
    return 0;
/*<       end >*/
} /* dakg_ */


/* ***************************************************************** */
/*  akbrdf  -  an interface between 6s and msrm */
/*  MSRM93  -  MultiSpectral Reflectance Model   A. Kuusk   24.03.1993 */
/*                                               Internet:  andres@aai.ee */

/*             A. Kuusk, A multispectral canopy reflectance model, */
/*                       Remote Sens. Environ.,  1994,  50(2):75-82. */

/*<    >*/
/* Subroutine */ int akbrdf_(doublereal *eei, doublereal *thmi, doublereal *
	uli, doublereal *sli, doublereal *rsl1i, doublereal *wlmoy, 
	doublereal *rnci, doublereal *cabi, doublereal *cwi, doublereal *vaii,
	 integer *mu, integer *np, doublereal *rm, doublereal *rp, doublereal 
	*brdfint)
{
    /* Initialized data */

    static doublereal pir = 3.14159265;
    static doublereal pi12 = 1.570796326794895;
    static doublereal dr = .0174532925199433;
    static doublereal eps = 1e-6;
    static doublereal eps4 = 1e-4;

    /* System generated locals */
    integer rm_offset, brdfint_dim1, brdfint_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle();
    /* Subroutine */ int s_stop(char *, ftnlen);
    integer i_dnnt(doublereal *);
    double cos(doublereal), sin(doublereal), log(doublereal), acos(doublereal)
	    , tan(doublereal), exp(doublereal);

    /* Local variables */
    extern /* Subroutine */ int soilspec_();
    integer j, k;
    doublereal fi, xx, mu1, mu2, eln, clx, clz, tgt, tgt1;
    extern /* Subroutine */ int leaf_();
    doublereal cth10, sth10;
    extern /* Subroutine */ int msrm_();
    doublereal rrls, rlambda;

    /* Fortran I/O blocks */
    static cilist io___215 = { 0, 6, 0, 0, 0 };


/*  See on tegelikult juba mcrm,  aga clx ja clz on fikseeritud */

/*<       implicit double precision (a-h, o-z) >*/
/*<       double precision integr >*/
/*<       integer np, mu >*/
/*<       integer k, j >*/
/*<    >*/
/*<       real mu1, mu2, fi >*/
/*<       real rm(-mu:mu), rp(np), brdfint(-mu:mu, np) >*/
/*<       save /count/, /soildata/, /aaa/, /ggg/, /ladak/ >*/

/*<       dimension u1(10), u2(10), a1(10), a2(10) >*/
/*<       common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2 >*/

/*<       double precision nnl, kk >*/
/*<       common /leafin/ nnl, vai, kk >*/
/*<       common /leafout/ refl, tran >*/

/*<       double precision ke, kab, kw >*/
/*<       dimension refr(200), ke(200), kab(200), kw(200) >*/
/*<       common /dat/ refr, ke, kab, kw >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
/*<    >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/
/*<       common /cfresn/ rn, rk >*/
/*<       common /ladak/ ee, thm, sthm, cthm >*/
/*<       common /msrmdata/ th10, rncoef, cab, cw, bq >*/


/*<       data pi/3.141592653589793d0/, pir/3.14159265/ >*/
    /* Parameter adjustments */
    rm_offset = -(*mu);
    rm -= rm_offset;
    brdfint_dim1 = *mu - (-(*mu)) + 1;
    brdfint_offset = -(*mu) + brdfint_dim1;
    brdfint -= brdfint_offset;
    --rp;

    /* Function Body */
/*<       data pi12/1.570796326794895d0/, dr/1.745329251994330d-2/ >*/
/*<       data eps/.1d-5/, eps4/.1d-3/ >*/
/*<       data lf/1/ >*/

/*           print *, 'msrm93' */

/*<       integr(xx) = (1.d0 - exp(-xx))/xx >*/
/*<       jg = 1 >*/
    count_1.jg = 1;
/*     if (lf .eq. 1) then */

/*<         ee    = eei >*/
    ladak_1.ee = *eei;
/*<         thm   = thmi*dr >*/
    ladak_1.thm = *thmi * dr;
/*<         ul    = uli >*/
    aaa_1.ul = *uli;
/*<         sl    = sli >*/
    aaa_1.sl = *sli;
/*<         clz   = .9d0 >*/
    clz = .9;
/*<         clx   = .1d0 >*/
    clx = .1;
/*<         th2   = 45.d0*dr >*/
    soildata_1.th2 = dr * 45.;
/*<         rsl1  = rsl1i >*/
    soildata_1.rsl1 = *rsl1i;
/*<         rsl2  = -.48d0*rsl1 + .0862d0 >*/
    soildata_1.rsl2 = soildata_1.rsl1 * -.48 + .0862;
/*<         rsl3  = 0.d0 >*/
    soildata_1.rsl3 = 0.;
/*<         rsl4  = 0.d0 >*/
    soildata_1.rsl4 = 0.;
/*<         rlambda = wlmoy*1000.d0 >*/
    rlambda = *wlmoy * 1e3;

/*<         if ((rlambda .gt. 2500.d0) .or. (rlambda .lt. 404.d0)) then >*/
    if (rlambda > 2500. || rlambda < 404.) {
/*<            print *, 'AKBRDF: wavelength out of range' >*/
	s_wsle(&io___215);
	do_lio(&c__9, &c__1, "AKBRDF: wavelength out of range", 31L);
	e_wsle();
/*<            stop >*/
	s_stop("", 0L);
/*<         endif >*/
    }

/*<         if (rlambda .le. 800.d0) then >*/
    if (rlambda <= 800.) {
/*<            jl = nint((rlambda - 400.d0)/4.d0) >*/
	d__1 = (rlambda - 400.) / 4.;
	count_1.jl = i_dnnt(&d__1);
/*<         else >*/
    } else {
/*<            jl = nint((rlambda - 800.d0)/17.d0) + 100 >*/
	d__1 = (rlambda - 800.) / 17.;
	count_1.jl = i_dnnt(&d__1) + 100;
/*<         endif >*/
    }

/*<         rncoef = rnci >*/
    msrmdata_1.rncoef = *rnci;
/*<         cab    = cabi >*/
    msrmdata_1.cab = *cabi;
/*<         cw     = cwi >*/
    msrmdata_1.cw = *cwi;
/*<         vai    = vaii >*/
    leafin_1.vai = *vaii;
/*<         nnl    = refr(jl) >*/
    leafin_1.nnl = dat_1.refr[count_1.jl - 1];
/*<         kk     = ke(jl) + cab*kab(jl) + cw*kw(jl) >*/
    leafin_1.kk = dat_1.ke[count_1.jl - 1] + msrmdata_1.cab * dat_1.kab[
	    count_1.jl - 1] + msrmdata_1.cw * dat_1.kw[count_1.jl - 1];
/*<         call leaf >*/
    leaf_();

/*<         rn   = rncoef*nnl >*/
    cfresn_1.rn = msrmdata_1.rncoef * leafin_1.nnl;
/*<         rrl  = refl - ((1.d0 - rn)/(1.d0 + rn))**2 >*/
/* Computing 2nd power */
    d__1 = (1. - cfresn_1.rn) / (cfresn_1.rn + 1.);
    aaa_1.rrl = leafout_1.refl - d__1 * d__1;
/*<         rrls = rrl >*/
    rrls = aaa_1.rrl;
/*<         ttl  = tran >*/
    aaa_1.ttl = leafout_1.tran;

/*<         call soilspec >*/
    soilspec_();

/*<         cthm = cos(thm) >*/
    ladak_1.cthm = cos(ladak_1.thm);
/*<         sthm = sin(thm) >*/
    ladak_1.sthm = sin(ladak_1.thm);

/*<         th22 = pi12 - thm >*/
    ggg_1.th22 = pi12 - ladak_1.thm;
/*<         if (abs(th22) .lt. eps4) th22 = 0.d0 >*/
    if (abs(ggg_1.th22) < eps4) {
	ggg_1.th22 = 0.;
    }
/*<         eln = -log(1.d0 - ee) >*/
    eln = -log(1. - ladak_1.ee);
/*<         difmy  = abs(.059d0*eln*(thm - 1.02d0) + .02d0) >*/
    ggg_1.difmy = (d__1 = eln * .059 * (ladak_1.thm - 1.02) + .02, abs(d__1));
/*<         difsig = abs(.01771d0 - .0216d0*eln*(thm - .256d0)) >*/
    ggg_1.difsig = (d__1 = .01771 - eln * .0216 * (ladak_1.thm - .256), abs(
	    d__1));

/*       lf = 2 */
/*     endif */

/*<       sth10 = sin(th10) >*/
    sth10 = sin(msrmdata_1.th10);
/*<       cth10 = cos(th10) >*/
    cth10 = cos(msrmdata_1.th10);

/*<       mu1 = rm(0) >*/
    mu1 = rm[0];
/*<       do 1 k = 1, np >*/
    i__1 = *np;
    for (k = 1; k <= i__1; ++k) {
/*<       do 2 j = 1, mu >*/
	i__2 = *mu;
	for (j = 1; j <= i__2; ++j) {
/*<         mu2 = rm(j) >*/
	    mu2 = rm[j];
/*<         if (j .eq. mu) then >*/
	    if (j == *mu) {
/*<            fi = rm(-mu) >*/
		fi = rm[-(*mu)];
/*<         else >*/
	    } else {
/*<            fi = rp(k) + rm(-mu) >*/
		fi = rp[k] + rm[-(*mu)];
/*<         endif >*/
	    }
/*<         th10 = acos(mu1) >*/
	    msrmdata_1.th10 = acos(mu1);
/*<         if (fi .lt. 0.) fi = fi + 2.*pir >*/
	    if (fi < 0.) {
		fi += pir * 2.;
	    }
/*<         if (fi .gt. (2.*pir)) fi = fi - 2.*pir >*/
	    if (fi > pir * 2.) {
		fi -= pir * 2.;
	    }
/*<         if (fi .gt. pir) fi = 2.*pir - fi >*/
	    if (fi > pir) {
		fi = pir * 2. - fi;
	    }
/*<         tgt1 = tan(th10) >*/
	    tgt1 = tan(msrmdata_1.th10);
/*<         xx   = tgt1*clx/sl >*/
	    xx = tgt1 * clx / aaa_1.sl;

/*<         if (xx .lt. eps) then >*/
	    if (xx < eps) {
/*<             clmp1 = clz >*/
		aaa_1.clmp1 = clz;
/*<         else >*/
	    } else {
/*<             clmp1 = 1.d0 - (1.d0 - clz)*integr(xx) >*/
		aaa_1.clmp1 = 1. - (1. - clz) * ((1. - exp(-xx)) / xx);
/*<         endif >*/
	    }

/*<         phi = fi >*/
	    ggg_1.phi = fi;
/*<         th1 = th10 >*/
	    ggg_1.th1 = msrmdata_1.th10;
/*<         th  = acos(mu2) >*/
	    ggg_1.th = acos(mu2);
/*<         tgt = tan(th) >*/
	    tgt = tan(ggg_1.th);
/*<         xx  = tgt*clx/sl >*/
	    xx = tgt * clx / aaa_1.sl;

/*<         if (xx .lt. eps) then >*/
	    if (xx < eps) {
/*<             clmp = clz >*/
		aaa_1.clmp = clz;
/*<         else >*/
	    } else {
/*<             clmp = 1.d0 - (1.d0 - clz)*integr(xx) >*/
		aaa_1.clmp = 1. - (1. - clz) * ((1. - exp(-xx)) / xx);
/*<         endif >*/
	    }

/*<         call msrm >*/
	    msrm_();
/*<         brdfint(j, k) = bq >*/
	    brdfint[j + k * brdfint_dim1] = msrmdata_1.bq;

/*<   2   continue >*/
/* L2: */
	}
/*<   1   continue >*/
/* L1: */
    }

/*<       return >*/
    return 0;
/*<       end >*/
} /* akbrdf_ */



/* ***************************************************************** */

/*<    >*/
/* Subroutine */ int akalbe_(doublereal *albbrdf)
{
    /* Initialized data */

    static doublereal pi = 3.141592653589793;
    static doublereal pi1 = 1.5707963268;

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    integer i__, n, i1, i2;
    doublereal aa[20];
    integer ng;
    doublereal uu[20], bdd;
    extern /* Subroutine */ int akd_(), dakg_(doublereal *, doublereal *, 
	    integer *);

/*   & (eei, thmi, uli, sli, rsl1i, wlmoy, rnci, cabi, cwi, vaii, albbrdf)
*/

/*   aa94.f   -  albeedo integrating msrm93 over the hemisphere */
/*   A. Kuusk    23.09.1994 */

/*<       implicit double precision (a-h, o-z) >*/

/*    real eei, thmi, uli, sli, rsl1i, wlmoy, rnci, cabi, cwi, vaii, albbr
df*/
/*<       real albbrdf >*/
/*<       save /count/, /soildata/, /aaa/, /ggg/, /ladak/ >*/

/*<       dimension uu(20), aa(20) >*/

/*<       dimension u1(10), u2(10), a1(10), a2(10) >*/
/*<       common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2 >*/

/*<       dimension phis1(200), phis2(200), phis3(200), phis4(200) >*/
/*<    >*/

/*<       common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint >*/
/*<    >*/
/*<       common /ladak/ ee, thm, sthm, cthm >*/

/*<       data pi/3.141592653589793d0/, pi1/1.5707963268d0/ >*/

/*           print *, 'aa94' */

/*<       n1 = 6 >*/
    count_1.n1 = 6;
/*<       n2 = 8 >*/
    count_1.n2 = 8;

/*<       n  = n2 + n2 >*/
    n = count_1.n2 + count_1.n2;
/*<       ng = n + 1 >*/
    ng = n + 1;
/*<       call dakg(uu, aa, n) >*/
    dakg_(uu, aa, &n);

/*<       do 20 i = 1, n2 >*/
    i__1 = count_1.n2;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          i1    = ng - i >*/
	i1 = ng - i__;
/*<          a2(i) = aa(i) >*/
	count_1.a2[i__ - 1] = aa[i__ - 1];
/*< 20       u2(i) = uu(i1) >*/
/* L20: */
	count_1.u2[i__ - 1] = uu[i1 - 1];
    }

/*<       n  = n1 + n1 >*/
    n = count_1.n1 + count_1.n1;
/*<       ng = n + 1 >*/
    ng = n + 1;
/*<       call dakg(uu, aa, n) >*/
    dakg_(uu, aa, &n);

/*<       do 21 i = 1, n1 >*/
    i__1 = count_1.n1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          i1    = ng - i >*/
	i1 = ng - i__;
/*<          a1(i) = aa(i) >*/
	count_1.a1[i__ - 1] = aa[i__ - 1];
/*< 21       u1(i) = uu(i1) >*/
/* L21: */
	count_1.u1[i__ - 1] = uu[i1 - 1];
    }

/*<       bdd = 0.d0 >*/
    bdd = 0.;
/*<       do 10 i2 = 1, n2 >*/
    i__1 = count_1.n2;
    for (i2 = 1; i2 <= i__1; ++i2) {
/*<          th  = (1.d0 - u2(i2))*pi1 >*/
	ggg_1.th = (1. - count_1.u2[i2 - 1]) * pi1;
/*<          sth = sin(th) >*/
	ggg_1.sth = sin(ggg_1.th);
/*<          cth = cos(th) >*/
	ggg_1.cth = cos(ggg_1.th);

/*<          call akd >*/
	akd_();

/*<          bdd = bdd + a2(i2)*bqint*sth*cth >*/
	bdd += count_1.a2[i2 - 1] * aaa_1.bqint * ggg_1.sth * ggg_1.cth;
/*< 10    continue >*/
/* L10: */
    }

/*<       albbrdf = bdd*pi >*/
    *albbrdf = bdd * pi;

/*<       return >*/
    return 0;
/*<       end >*/
} /* akalbe_ */

#ifdef __cplusplus
	}
#endif
