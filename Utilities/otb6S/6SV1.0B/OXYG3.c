/* OXYG3.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/*<       subroutine    oxyg3(a,inu) >*/
/* Subroutine */ int oxyg3_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)7620.,(float)7630.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    7630.,(float)7640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)7640.,(float)7650.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)7650.,(float)7660.,(float)
	    5.4851e-9,(float).018194,(float).043239,(float)-1.7296e-4,(float)
	    .041212,(float)-1.6888e-4,(float)7660.,(float)7670.,(float)
	    1.5945e-8,(float).018194,(float).03852,(float)-1.5408e-4,(float)
	    .036492,(float)-1.5e-4,(float)7670.,(float)7680.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7680.,(float)
	    7690.,(float)4.3251e-8,(float).018194,(float).034069,(float)
	    -1.3628e-4,(float).032042,(float)-1.3219e-4,(float)7690.,(float)
	    7700.,(float)1.0929e-7,(float).018194,(float).029889,(float)
	    -1.1956e-4,(float).027862,(float)-1.1547e-4,(float)7700.,(float)
	    7710.,(float)2.5722e-7,(float).018194,(float).02598,(float)
	    -1.0392e-4,(float).023953,(float)-9.9839e-5,(float)7710.,(float)
	    7720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)7720.,(float)7730.,(float)5.6266e-7,(float).018194,(float)
	    .022343,(float)-8.9371e-5,(float).020315,(float)-8.5289e-5,(float)
	    7730.,(float)7740.,(float)1.1425e-6,(float).01906,(float).018977,(
	    float)-7.591e-5,(float).01695,(float)-7.1828e-5,(float)7740.,(
	    float)7750.,(float)2.1596e-6,(float).022229,(float).016034,(float)
	    -6.2289e-5,(float).015554,(float)-5.5716e-5,(float)7750.,(float)
	    7760.,(float)8.1892e-8,(float).052034,(float).039222,(float)
	    -1.5556e-4,(float).037572,(float)-1.5339e-4,(float)7760.,(float)
	    7770.,(float)3.9209e-6,(float).032273,(float).014036,(float)
	    -4.6954e-5,(float).015654,(float)-4.8384e-5,(float)7770.,(float)
	    7780.,(float)6.4421e-6,(float).036427,(float).011888,(float)
	    -3.5835e-5,(float).013496,(float)-4.0675e-5,(float)7780.,(float)
	    7790.,(float)1.1047e-5,(float).055891,(float).011375,(float)
	    -2.6109e-5,(float).013172,(float)-4.0967e-5,(float)7790.,(float)
	    7800.,(float)1.7987e-5,(float).067558,(float).010748,(float)
	    -2.3562e-5,(float).01137,(float)-3.8223e-5,(float)7800.,(float)
	    7810.,(float)9.8092e-6,(float).038971,(float).015905,(float)
	    -6.3621e-5,(float).013877,(float)-5.9532e-5,(float)7810.,(float)
	    7820.,(float)4.4018e-5,(float).082997,(float).0095499,(float)
	    -3.1911e-5,(float).0077993,(float)-3.2091e-5,(float)7820.,(float)
	    7830.,(float)7.0926e-5,(float).086557,(float).0076614,(float)
	    -2.7697e-5,(float).0055197,(float)-2.4447e-5,(float)7830.,(float)
	    7840.,(float)1.0119e-4,(float).087824,(float).0052157,(float)
	    -1.9889e-5,(float).00297,(float)-1.5277e-5,(float)7840.,(float)
	    7850.,(float)1.2386e-4,(float).090123,(float).0034834,(float)
	    -1.2896e-5,(float).0022642,(float)-3.5587e-6,(float)7850.,(float)
	    7860.,(float)1.3558e-4,(float).14355,(float).0025455,(float)
	    7.5114e-7,(float).0043782,(float)4.2838e-6,(float)7860.,(float)
	    7870.,(float)3.6155e-4,(float).30611,(float).00774,(float)
	    -2.3164e-5,(float).0072019,(float)-2.6289e-5,(float)7870.,(float)
	    7880.,(float)7.4132e-4,(float).38505,(float).0019206,(float)
	    -6.4925e-6,(float)1.9419e-5,(float)-3.3562e-6,(float)7880.,(float)
	    7890.,(float)2.6142e-4,(float).099139,(float)5.6355e-4,(float)
	    -2.1878e-6,(float)-.0015085,(float)2.0396e-6,(float)7890.,(float)
	    7900.,(float)3.3585e-4,(float).12035,(float).0017877,(float)
	    -6.7625e-6,(float)-3.0147e-4,(float)-2.6201e-6,(float)7900.,(
	    float)7910.,(float)2.1797e-4,(float).09124,(float).0044856,(float)
	    -1.7351e-5,(float).0025121,(float)-1.3772e-5,(float)7910.,(float)
	    7920.,(float)1.8805e-4,(float).12809,(float).0061732,(float)
	    -1.8626e-5,(float).0047187,(float)-2.0003e-5,(float)7920.,(float)
	    7930.,(float)8.7414e-5,(float).10246,(float).0071283,(float)
	    -9.5048e-6,(float).0079366,(float)-2.5696e-5,(float)7930.,(float)
	    7940.,(float)5.0284e-5,(float).070001,(float).0058651,(float)
	    -1.5881e-6,(float).0099197,(float)-1.9259e-5,(float)7940.,(float)
	    7950.,(float)3.4484e-5,(float).036136,(float).0050423,(float)
	    -1.3556e-5,(float).0083314,(float)-2.9461e-6,(float)7950.,(float)
	    7960.,(float)2.508e-5,(float).023597,(float).0062798,(float)
	    -2.4587e-5,(float).0054199,(float)-1.4942e-5,(float)7960.,(float)
	    7970.,(float)1.7175e-5,(float).021672,(float).0082429,(float)
	    -3.2972e-5,(float).0062159,(float)-2.8891e-5,(float)7970.,(float)
	    7980.,(float)1.0919e-5,(float).0208,(float).010514,(float)
	    -4.2054e-5,(float).0084864,(float)-3.7973e-5,(float)7980.,(float)
	    7990.,(float)6.453e-6,(float).019927,(float).013058,(float)
	    -5.2234e-5,(float).011031,(float)-4.8151e-5,(float)7990.,(float)
	    8e3,(float)3.5484e-6,(float).019493,(float).015877,(float)
	    -6.3508e-5,(float).01385,(float)-5.9425e-5,(float)8e3,(float)
	    8010.,(float)1.8178e-6,(float).01906,(float).018969,(float)
	    -7.5874e-5,(float).016941,(float)-7.1792e-5,(float)8010.,(float)
	    8020.,(float)8.6808e-7,(float).018194,(float).022333,(float)
	    -8.9332e-5,(float).020306,(float)-8.525e-5,(float)8020.,(float)
	    8030.,(float)5.4731e-7,(float).034766,(float).027088,(float)
	    -1.0685e-4,(float).025457,(float)-1.0498e-4,(float)8030.,(float)
	    8040.,(float)6.2423e-8,(float).018194,(float).034057,(float)
	    -1.3623e-4,(float).03203,(float)-1.3215e-4,(float)8040.,(float)
	    8050.,(float)2.2644e-8,(float).018194,(float).038506,(float)
	    -1.5403e-4,(float).036479,(float)-1.4994e-4,(float)8050.,(float)
	    8060.,(float)7.6809e-9,(float).018194,(float).043225,(float)
	    -1.729e-4,(float).041198,(float)-1.6882e-4,(float)8060.,(float)
	    8070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)8070.,(float)8080.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)8080.,(float)8090.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)8090.,(float)8100.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)8100.,(float)8110.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)8110.,(float)8120.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)8120.,(float)8130.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    8130.,(float)8140.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)8140.,(float)8150.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)8150.,(float)8160.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8160.,
	    (float)8170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8170.,(float)8180.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8180.,(float)8190.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8190.,(
	    float)8200.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8200.,(float)8210.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8210.,(float)8220.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8220.,(
	    float)8230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8230.,(float)8240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8240.,(float)8250.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8250.,(
	    float)8260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8260.,(float)8270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8270.,(float)8280.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8280.,(
	    float)8290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8290.,(float)8300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8300.,(float)8310.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8310.,(
	    float)8320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8320.,(float)8330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8330.,(float)8340.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8340.,(
	    float)8350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8350.,(float)8360.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8360.,(float)8370.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8370.,(
	    float)8380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8380.,(float)8390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8390.,(float)8400.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8400.,(
	    float)8410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8410.,(float)8420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8420.,(float)8430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8430.,(
	    float)8440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8440.,(float)8450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8450.,(float)8460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8460.,(
	    float)8470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8470.,(float)8480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8480.,(float)8490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8490.,(
	    float)8500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8500.,(float)8510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8510.,(float)8520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8520.,(
	    float)8530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8530.,(float)8540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8540.,(float)8550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8550.,(
	    float)8560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8560.,(float)8570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8570.,(float)8580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8580.,(
	    float)8590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8590.,(float)8600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8600.,(float)8610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8610.,(
	    float)8620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8620.,(float)8630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8630.,(float)8640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8640.,(
	    float)8650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8650.,(float)8660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8660.,(float)8670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8670.,(
	    float)8680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8680.,(float)8690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8690.,(float)8700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8700.,(
	    float)8710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8710.,(float)8720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8720.,(float)8730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8730.,(
	    float)8740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8740.,(float)8750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8750.,(float)8760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8760.,(
	    float)8770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8770.,(float)8780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8780.,(float)8790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8790.,(
	    float)8800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8800.,(float)8810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8810.,(float)8820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8820.,(
	    float)8830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8830.,(float)8840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8840.,(float)8850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8850.,(
	    float)8860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8860.,(float)8870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8870.,(float)8880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8880.,(
	    float)8890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8890.,(float)8900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8900.,(float)8910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8910.,(
	    float)8920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8920.,(float)8930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8930.,(float)8940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8940.,(
	    float)8950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8950.,(float)8960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8960.,(float)8970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)8970.,(
	    float)8980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)8980.,(float)8990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)8990.,(float)9e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9e3,(
	    float)9010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9010.,(float)9020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9020.,(float)9030.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9030.,(
	    float)9040.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9040.,(float)9050.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9050.,(float)9060.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9060.,(
	    float)9070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9070.,(float)9080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9080.,(float)9090.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9090.,(
	    float)9100.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9100.,(float)9110.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9110.,(float)9120.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9120.,(
	    float)9130.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9130.,(float)9140.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9140.,(float)9150.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9150.,(
	    float)9160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9160.,(float)9170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9170.,(float)9180.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9180.,(
	    float)9190.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9190.,(float)9200.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9200.,(float)9210.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9210.,(
	    float)9220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9220.,(float)9230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9230.,(float)9240.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9240.,(
	    float)9250.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9250.,(float)9260.,(float)2.9327e-8,(float)
	    .020793,(float).01052,(float)-4.2079e-5,(float).0084925,(float)
	    -3.7997e-5,(float)9260.,(float)9270.,(float)4.284e-8,(float)
	    .021659,(float).0082482,(float)-3.2993e-5,(float).0062209,(float)
	    -2.8911e-5,(float)9270.,(float)9280.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)9280.,(float)9290.,(float)
	    1.4493e-7,(float).078959,(float).011173,(float)-3.6086e-5,(float)
	    .0098558,(float)-3.9242e-5,(float)9290.,(float)9300.,(float)
	    2.5005e-7,(float).083955,(float).0092831,(float)-3.2526e-5,(float)
	    .0073578,(float)-3.1073e-5,(float)9300.,(float)9310.,(float)
	    2.7511e-7,(float).065813,(float).0069254,(float)-2.5251e-5,(float)
	    .0046721,(float)-2.1357e-5,(float)9310.,(float)9320.,(float)
	    3.3929e-7,(float).065167,(float).0055165,(float)-2.0774e-5,(float)
	    .0031412,(float)-1.5731e-5,(float)9320.,(float)9330.,(float)
	    7.3603e-7,(float).092398,(float).0037855,(float)-1.4891e-5,(float)
	    .0017763,(float)-1.1006e-5,(float)9330.,(float)9340.,(float)
	    4.4813e-7,(float).07935,(float).0045127,(float)-2.4202e-6,(float)
	    .0042176,(float)-9.3426e-6,(float)9340.,(float)9350.,(float)
	    1.4773e-6,(float).2629,(float).0066029,(float)-1.8761e-5,(float)
	    .004739,(float)-1.8284e-5,(float)9350.,(float)9360.,(float)
	    3.6258e-6,(float).35403,(float).0024719,(float)-8.6417e-6,(float)
	    5.0685e-4,(float)-5.3198e-6,(float)9360.,(float)9370.,(float)
	    1.0885e-6,(float).098959,(float)2.798e-4,(float)-1.1056e-6,(float)
	    -.0017654,(float)3.0416e-6,(float)9370.,(float)9380.,(float)
	    1.6901e-6,(float).12199,(float).0012891,(float)-4.9557e-6,(float)
	    -7.9138e-4,(float)-7.506e-7,(float)9380.,(float)9390.,(float)
	    1.6816e-6,(float).13899,(float).0035668,(float)-1.2886e-5,(float)
	    .0015543,(float)-9.4945e-6,(float)9390.,(float)9400.,(float)
	    1.0341e-6,(float).16279,(float).0068719,(float)-1.996e-5,(float)
	    .0057191,(float)-2.3184e-5,(float)9400.,(float)9410.,(float)
	    3.1773e-7,(float).06436,(float).0052558,(float)-2.7296e-6,(float)
	    .0065364,(float)-1.6975e-5,(float)9410.,(float)9420.,(float)
	    2.0773e-7,(float).023392,(float).0030789,(float)-1.2316e-5,(float)
	    .0010516,(float)-8.2335e-6,(float)9420.,(float)9430.,(float)
	    1.6613e-7,(float).022959,(float).0045253,(float)-1.8101e-5,(float)
	    .0024979,(float)-1.4019e-5,(float)9430.,(float)9440.,(float)
	    1.2289e-7,(float).022092,(float).0062467,(float)-2.4987e-5,(float)
	    .0042194,(float)-2.0905e-5,(float)9440.,(float)9450.,(float)
	    8.4168e-8,(float).021659,(float).0082429,(float)-3.2972e-5,(float)
	    .0062156,(float)-2.8889e-5,(float)9450.,(float)9460.,(float)
	    8.5119e-8,(float).040142,(float).011444,(float)-4.5054e-5,(float)
	    .0095711,(float)-4.1931e-5,(float)9460.,(float)9470.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9470.,(
	    float)9480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9480.,(float)9490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9490.,(float)9500.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9500.,(
	    float)9510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9510.,(float)9520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9520.,(float)9530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9530.,(
	    float)9540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9540.,(float)9550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9550.,(float)9560.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9560.,(
	    float)9570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9570.,(float)9580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9580.,(float)9590.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9590.,(
	    float)9600.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9600.,(float)9610.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9610.,(float)9620.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9620.,(
	    float)9630.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9630.,(float)9640.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9640.,(float)9650.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9650.,(
	    float)9660.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9660.,(float)9670.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9670.,(float)9680.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9680.,(
	    float)9690.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9690.,(float)9700.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9700.,(float)9710.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9710.,(
	    float)9720.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9720.,(float)9730.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9730.,(float)9740.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9740.,(
	    float)9750.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9750.,(float)9760.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9760.,(float)9770.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9770.,(
	    float)9780.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9780.,(float)9790.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9790.,(float)9800.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9800.,(
	    float)9810.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9810.,(float)9820.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9820.,(float)9830.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9830.,(
	    float)9840.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9840.,(float)9850.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9850.,(float)9860.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9860.,(
	    float)9870.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9870.,(float)9880.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9880.,(float)9890.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9890.,(
	    float)9900.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9900.,(float)9910.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9910.,(float)9920.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9920.,(
	    float)9930.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9930.,(float)9940.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9940.,(float)9950.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9950.,(
	    float)9960.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9960.,(float)9970.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)9970.,(float)9980.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)9980.,(
	    float)9990.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)9990.,(float)1e4,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)1e4,(float)10010.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10010.,(
	    float)10020.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)10020.,(float)10030.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)10030.,(float)10040.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10040.,(float)10050.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10050.,(float)10060.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10060.,(float)
	    10070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10070.,(float)10080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10080.,(float)10090.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10090.,(float)10100.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10100.,(float)10110.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10110.,(float)
	    10120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10120.,(float)10130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)10130.,(float)10140.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    10140.,(float)10150.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)10150.,(float)10160.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)10160.,(float)
	    10170.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)10170.,(float)10180. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     oxygen (7620 - 10170 cm-1) */

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
} /* oxyg3_ */

#ifdef __cplusplus
	}
#endif
