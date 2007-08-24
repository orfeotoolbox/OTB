/* MOCA3.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    moca3(a,inu) >*/
/* Subroutine */ int moca3_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)7620.,(float)7630.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    7630.,(float)7640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)7640.,(float)7650.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)7650.,(float)7660.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7660.,
	    (float)7670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7670.,(float)7680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7680.,(float)7690.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7690.,(
	    float)7700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7700.,(float)7710.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7710.,(float)7720.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7720.,(
	    float)7730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7730.,(float)7740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7740.,(float)7750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7750.,(
	    float)7760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7760.,(float)7770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7770.,(float)7780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7780.,(
	    float)7790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7790.,(float)7800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7800.,(float)7810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7810.,(
	    float)7820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7820.,(float)7830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7830.,(float)7840.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7840.,(
	    float)7850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7850.,(float)7860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7860.,(float)7870.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7870.,(
	    float)7880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7880.,(float)7890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7890.,(float)7900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7900.,(
	    float)7910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7910.,(float)7920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7920.,(float)7930.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7930.,(
	    float)7940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7940.,(float)7950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7950.,(float)7960.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7960.,(
	    float)7970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7970.,(float)7980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7980.,(float)7990.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7990.,(
	    float)8e3,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8e3,(float)8010.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8010.,(float)8020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8020.,(
	    float)8030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8030.,(float)8040.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8040.,(float)8050.,(float)
	    1.354e-16,(float).017736,(float).11256,(float)-4.5834e-4,(float)
	    .10977,(float)-4.527e-4,(float)8050.,(float)8060.,(float)
	    4.1572e-16,(float).017736,(float).10803,(float)-4.4019e-4,(float)
	    .10523,(float)-4.3455e-4,(float)8060.,(float)8070.,(float)
	    1.2473e-15,(float).017736,(float).10358,(float)-4.2239e-4,(float)
	    .10078,(float)-4.1676e-4,(float)8070.,(float)8080.,(float)
	    3.6626e-15,(float).017736,(float).099217,(float)-4.0495e-4,(float)
	    .096419,(float)-3.9931e-4,(float)8080.,(float)8090.,(float)
	    1.0521e-14,(float).017736,(float).094944,(float)-3.8786e-4,(float)
	    .092146,(float)-3.8222e-4,(float)8090.,(float)8100.,(float)
	    2.9549e-14,(float).017736,(float).09076,(float)-3.7112e-4,(float)
	    .087962,(float)-3.6548e-4,(float)8100.,(float)8110.,(float)
	    8.1193e-14,(float).017736,(float).086664,(float)-3.5474e-4,(float)
	    .083866,(float)-3.491e-4,(float)8110.,(float)8120.,(float)
	    2.1814e-13,(float).017736,(float).082657,(float)-3.3871e-4,(float)
	    .079859,(float)-3.3307e-4,(float)8120.,(float)8130.,(float)
	    5.7319e-13,(float).017736,(float).078739,(float)-3.2303e-4,(float)
	    .075941,(float)-3.174e-4,(float)8130.,(float)8140.,(float)
	    1.4733e-12,(float).017736,(float).07491,(float)-3.0772e-4,(float)
	    .072112,(float)-3.0208e-4,(float)8140.,(float)8150.,(float)
	    3.7005e-12,(float).017736,(float).07117,(float)-2.9276e-4,(float)
	    .068372,(float)-2.8712e-4,(float)8150.,(float)8160.,(float)
	    9.0898e-12,(float).017736,(float).067519,(float)-2.7816e-4,(float)
	    .064721,(float)-2.7252e-4,(float)8160.,(float)8170.,(float)
	    2.1828e-11,(float).017736,(float).063958,(float)-2.6391e-4,(float)
	    .06116,(float)-2.5828e-4,(float)8170.,(float)8180.,(float)
	    1.6892e-10,(float).034043,(float).058108,(float)-2.3936e-4,(float)
	    .055638,(float)-2.3553e-4,(float)8180.,(float)8190.,(float)
	    2.6399e-10,(float).017736,(float).053812,(float)-2.2333e-4,(float)
	    .051015,(float)-2.177e-4,(float)8190.,(float)8200.,(float)
	    5.7886e-10,(float).017736,(float).050611,(float)-2.1052e-4,(float)
	    .047813,(float)-2.0489e-4,(float)8200.,(float)8210.,(float)
	    1.2407e-9,(float).017957,(float).047499,(float)-1.9807e-4,(float)
	    .044701,(float)-1.9244e-4,(float)8210.,(float)8220.,(float)
	    2.5994e-9,(float).018179,(float).044477,(float)-1.8599e-4,(float)
	    .041679,(float)-1.8035e-4,(float)8220.,(float)8230.,(float)
	    1.596e-8,(float).036001,(float).039635,(float)-1.6576e-4,(float)
	    .03707,(float)-1.6145e-4,(float)8230.,(float)8240.,(float)
	    2.0795e-8,(float).018844,(float).035955,(float)-1.519e-4,(float)
	    .033157,(float)-1.4627e-4,(float)8240.,(float)8250.,(float)
	    3.9708e-8,(float).019066,(float).033295,(float)-1.4126e-4,(float)
	    .030497,(float)-1.3563e-4,(float)8250.,(float)8260.,(float)
	    2.0895e-7,(float).03799,(float).029113,(float)-1.2386e-4,(float)
	    .026495,(float)-1.1926e-4,(float)8260.,(float)8270.,(float)
	    2.3995e-7,(float).019731,(float).025861,(float)-1.1153e-4,(float)
	    .023064,(float)-1.0589e-4,(float)8270.,(float)8280.,(float)
	    4.1681e-7,(float).019953,(float).023566,(float)-1.0234e-4,(float)
	    .020768,(float)-9.6709e-5,(float)8280.,(float)8290.,(float)
	    1.8774e-6,(float).040651,(float).020033,(float)-8.771e-5,(float)
	    .017363,(float)-8.2825e-5,(float)8290.,(float)8300.,(float)
	    1.8914e-6,(float).021283,(float).017225,(float)-7.698e-5,(float)
	    .014427,(float)-7.1347e-5,(float)8300.,(float)8310.,(float)
	    7.568e-6,(float).043989,(float).014172,(float)-6.438e-5,(float)
	    .011463,(float)-5.9288e-5,(float)8310.,(float)8320.,(float)
	    6.8789e-6,(float).023145,(float).011707,(float)-5.4908e-5,(float)
	    .0089094,(float)-4.9275e-5,(float)8320.,(float)8330.,(float)
	    2.4399e-5,(float).046699,(float).0091256,(float)-4.4298e-5,(float)
	    .0063969,(float)-3.9079e-5,(float)8330.,(float)8340.,(float)
	    4.6806e-5,(float).049425,(float).006215,(float)-3.2716e-5,(float)
	    .0034666,(float)-2.739e-5,(float)8340.,(float)8350.,(float)
	    8.0605e-5,(float).050945,(float).0036678,(float)-2.2582e-5,(float)
	    9.0701e-4,(float)-1.718e-5,(float)8350.,(float)8360.,(float)
	    1.2377e-4,(float).051881,(float).0014843,(float)-1.3895e-5,(float)
	    -.0012918,(float)-8.4086e-6,(float)8360.,(float)8370.,(float)
	    1.6751e-4,(float).053029,(float)-3.3537e-4,(float)-6.6561e-6,(
	    float)-.0031202,(float)-1.1154e-6,(float)8370.,(float)8380.,(
	    float)1.9593e-4,(float).054802,(float)-.0017908,(float)-8.6649e-7,
	    (float)-.0045868,(float)4.7351e-6,(float)8380.,(float)8390.,(
	    float)1.8975e-4,(float).058538,(float)-.0028817,(float)3.4723e-6,(
	    float)-.0056846,(float)9.1141e-6,(float)8390.,(float)8400.,(float)
	    1.3472e-4,(float).062039,(float)-.0036068,(float)6.3564e-6,(float)
	    -.006417,(float)1.2035e-5,(float)8400.,(float)8410.,(float)
	    6.4957e-5,(float).06993,(float)-.0039984,(float)7.9148e-6,(float)
	    -.006795,(float)1.3543e-5,(float)8410.,(float)8420.,(float)
	    3.0309e-4,(float).090113,(float)-.0036875,(float)6.6869e-6,(float)
	    -.0065101,(float)1.2442e-5,(float)8420.,(float)8430.,(float)
	    5.1352e-4,(float).08362,(float)-.0026038,(float)2.4173e-6,(float)
	    -.0054145,(float)8.1551e-6,(float)8430.,(float)8440.,(float)
	    8.2971e-4,(float).13049,(float)5.179e-5,(float)-7.5797e-6,(float)
	    -.0026588,(float)-2.2561e-6,(float)8440.,(float)8450.,(float)
	    4.4414e-4,(float).14201,(float).0055167,(float)-2.7842e-5,(float)
	    .0033215,(float)-2.3351e-5,(float)8450.,(float)8460.,(float)
	    8.4089e-5,(float).17964,(float).015226,(float)-6.0171e-5,(float)
	    .015481,(float)-6.3601e-5,(float)8460.,(float)8470.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8470.,(
	    float)8480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8480.,(float)8490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8490.,(float)8500.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8500.,(
	    float)8510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8510.,(float)8520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8520.,(float)8530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8530.,(
	    float)8540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8540.,(float)8550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8550.,(float)8560.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8560.,(
	    float)8570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8570.,(float)8580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8580.,(float)8590.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8590.,(
	    float)8600.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8600.,(float)8610.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8610.,(float)8620.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8620.,(
	    float)8630.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8630.,(float)8640.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8640.,(float)8650.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8650.,(
	    float)8660.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8660.,(float)8670.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8670.,(float)8680.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8680.,(
	    float)8690.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8690.,(float)8700.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8700.,(float)8710.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8710.,(
	    float)8720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8720.,(float)8730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8730.,(float)8740.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8740.,(
	    float)8750.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8750.,(float)8760.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8760.,(float)8770.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8770.,(
	    float)8780.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8780.,(float)8790.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8790.,(float)8800.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8800.,(
	    float)8810.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8810.,(float)8820.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8820.,(float)8830.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8830.,(
	    float)8840.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8840.,(float)8850.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8850.,(float)8860.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8860.,(
	    float)8870.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8870.,(float)8880.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8880.,(float)8890.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8890.,(
	    float)8900.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8900.,(float)8910.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8910.,(float)8920.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8920.,(
	    float)8930.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8930.,(float)8940.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8940.,(float)8950.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8950.,(
	    float)8960.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8960.,(float)8970.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8970.,(float)8980.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8980.,(
	    float)8990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8990.,(float)9e3,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9e3,(float)9010.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9010.,(
	    float)9020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9020.,(float)9030.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9030.,(float)9040.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9040.,(
	    float)9050.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9050.,(float)9060.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9060.,(float)9070.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9070.,(
	    float)9080.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9080.,(float)9090.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9090.,(float)9100.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9100.,(
	    float)9110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9110.,(float)9120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9120.,(float)9130.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9130.,(
	    float)9140.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9140.,(float)9150.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9150.,(float)9160.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9160.,(
	    float)9170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9170.,(float)9180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9180.,(float)9190.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9190.,(
	    float)9200.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9200.,(float)9210.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9210.,(float)9220.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9220.,(
	    float)9230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9230.,(float)9240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9240.,(float)9250.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9250.,(
	    float)9260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9260.,(float)9270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9270.,(float)9280.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9280.,(
	    float)9290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9290.,(float)9300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9300.,(float)9310.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9310.,(
	    float)9320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9320.,(float)9330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9330.,(float)9340.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9340.,(
	    float)9350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9350.,(float)9360.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9360.,(float)9370.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9370.,(
	    float)9380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9380.,(float)9390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9390.,(float)9400.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9400.,(
	    float)9410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9410.,(float)9420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9420.,(float)9430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9430.,(
	    float)9440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9440.,(float)9450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9450.,(float)9460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9460.,(
	    float)9470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9470.,(float)9480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9480.,(float)9490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9490.,(
	    float)9500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9500.,(float)9510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9510.,(float)9520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9520.,(
	    float)9530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9530.,(float)9540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9540.,(float)9550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9550.,(
	    float)9560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9560.,(float)9570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9570.,(float)9580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9580.,(
	    float)9590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9590.,(float)9600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9600.,(float)9610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9610.,(
	    float)9620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9620.,(float)9630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9630.,(float)9640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9640.,(
	    float)9650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9650.,(float)9660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9660.,(float)9670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9670.,(
	    float)9680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9680.,(float)9690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9690.,(float)9700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9700.,(
	    float)9710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9710.,(float)9720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9720.,(float)9730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9730.,(
	    float)9740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9740.,(float)9750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9750.,(float)9760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9760.,(
	    float)9770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9770.,(float)9780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9780.,(float)9790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9790.,(
	    float)9800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9800.,(float)9810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9810.,(float)9820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9820.,(
	    float)9830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9830.,(float)9840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9840.,(float)9850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9850.,(
	    float)9860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9860.,(float)9870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9870.,(float)9880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9880.,(
	    float)9890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9890.,(float)9900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9900.,(float)9910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9910.,(
	    float)9920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9920.,(float)9930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9930.,(float)9940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9940.,(
	    float)9950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9950.,(float)9960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9960.,(float)9970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9970.,(
	    float)9980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9980.,(float)9990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9990.,(float)1e4,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)1e4,(
	    float)10010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)10010.,(float)10020.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)10020.,(float)10030.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10030.,(float)10040.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10040.,(float)10050.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10050.,(float)
	    10060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10060.,(float)10070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10070.,(float)10080.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10080.,(float)10090.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10090.,(float)10100.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10100.,(float)
	    10110.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10110.,(float)10120.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10120.,(float)10130.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10130.,(float)10140.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10140.,(float)10150.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10150.,(float)
	    10160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10160.,(float)10170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10170.,(float)10180. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     carbon monoxide (7600 - 10170 cm-1) */

/*<    >*/
    /* Parameter adjustments */
    --a;

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
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/
/*<    >*/

/*<       do i=1,8 >*/
    for (i__ = 1; i__ <= 8; ++i__) {
/*<       a(i)=acr(i,inu) >*/
	a[i__] = acr[i__ + (*inu << 3) - 9];
/*<       enddo >*/
    }

/*<       return >*/
    return 0;
/*<       end >*/
} /* moca3_ */

#ifdef __cplusplus
	}
#endif
