/* NIOX2.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/*<       subroutine    niox2(a,inu) >*/
/* Subroutine */ int niox2_(real *a, integer *inu)
{
    /* Initialized data */

    static real acr[2048]	/* was [8][256] */ = { (float).072211,(float)
	    .24584,(float).0096738,(float)-5.1958e-5,(float).0067533,(float)
	    -4.7277e-5,(float)5060.,(float)5070.,(float).21388,(float).25456,(
	    float).0043318,(float)-3.1058e-5,(float).0012217,(float)
	    -2.5614e-5,(float)5070.,(float)5080.,(float).57556,(float).33263,(
	    float)-2.6597e-4,(float)-1.2844e-5,(float)-.0033007,(float)
	    -7.3238e-6,(float)5080.,(float)5090.,(float).67723,(float).36014,(
	    float)-.0034018,(float)-8.0539e-7,(float)-.006515,(float)
	    5.4383e-6,(float)5090.,(float)5100.,(float).30093,(float).41799,(
	    float)-.0047912,(float)4.5347e-6,(float)-.0079672,(float)
	    1.0982e-5,(float)5100.,(float)5110.,(float)1.0645,(float).53257,(
	    float)-.003157,(float)-1.4679e-6,(float)-.0062971,(float)
	    4.7135e-6,(float)5110.,(float)5120.,(float).67643,(float).61899,(
	    float).0025029,(float)-2.0398e-5,(float)2.0488e-4,(float)
	    -1.9155e-5,(float)5120.,(float)5130.,(float).018117,(float).12425,
	    (float).013386,(float)-6.7752e-5,(float).010428,(float)-6.2211e-5,
	    (float)5130.,(float)5140.,(float)0.,(float)0.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)5140.,(float)5150.,(float)0.,(float)0.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)5150.,(float)5160.,
	    (float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)5160.,(float)5170.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)5170.,(float)5180.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)5180.,(float)5190.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    5190.,(float)5200.,(float)0.,(float)0.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)5200.,(float)5210.,(float)0.,(float)0.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)5210.,(float)5220.,(float)
	    0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5220.,
	    (float)5230.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5230.,(float)5240.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5240.,(float)5250.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5250.,(
	    float)5260.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5260.,(float)5270.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5270.,(float)5280.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5280.,(
	    float)5290.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5290.,(float)5300.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5300.,(float)5310.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5310.,(
	    float)5320.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5320.,(float)5330.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5330.,(float)5340.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5340.,(
	    float)5350.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5350.,(float)5360.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5360.,(float)5370.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5370.,(
	    float)5380.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5380.,(float)5390.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5390.,(float)5400.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5400.,(
	    float)5410.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5410.,(float)5420.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5420.,(float)5430.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5430.,(
	    float)5440.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5440.,(float)5450.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5450.,(float)5460.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5460.,(
	    float)5470.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5470.,(float)5480.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5480.,(float)5490.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5490.,(
	    float)5500.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5500.,(float)5510.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5510.,(float)5520.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5520.,(
	    float)5530.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5530.,(float)5540.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5540.,(float)5550.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5550.,(
	    float)5560.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5560.,(float)5570.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5570.,(float)5580.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5580.,(
	    float)5590.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5590.,(float)5600.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5600.,(float)5610.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5610.,(
	    float)5620.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5620.,(float)5630.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5630.,(float)5640.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5640.,(
	    float)5650.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5650.,(float)5660.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5660.,(float)5670.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5670.,(
	    float)5680.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5680.,(float)5690.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5690.,(float)5700.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5700.,(
	    float)5710.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5710.,(float)5720.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5720.,(float)5730.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5730.,(
	    float)5740.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5740.,(float)5750.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5750.,(float)5760.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5760.,(
	    float)5770.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5770.,(float)5780.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5780.,(float)5790.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5790.,(
	    float)5800.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5800.,(float)5810.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5810.,(float)5820.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5820.,(
	    float)5830.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5830.,(float)5840.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5840.,(float)5850.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5850.,(
	    float)5860.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5860.,(float)5870.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5870.,(float)5880.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5880.,(
	    float)5890.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5890.,(float)5900.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5900.,(float)5910.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5910.,(
	    float)5920.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5920.,(float)5930.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5930.,(float)5940.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5940.,(
	    float)5950.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5950.,(float)5960.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5960.,(float)5970.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)5970.,(
	    float)5980.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)5980.,(float)5990.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)5990.,(float)6e3,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6e3,(
	    float)6010.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6010.,(float)6020.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6020.,(float)6030.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6030.,(
	    float)6040.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6040.,(float)6050.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6050.,(float)6060.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6060.,(
	    float)6070.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6070.,(float)6080.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6080.,(float)6090.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6090.,(
	    float)6100.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6100.,(float)6110.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6110.,(float)6120.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6120.,(
	    float)6130.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6130.,(float)6140.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6140.,(float)6150.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6150.,(
	    float)6160.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6160.,(float)6170.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6170.,(float)6180.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6180.,(
	    float)6190.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6190.,(float)6200.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6200.,(float)6210.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6210.,(
	    float)6220.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6220.,(float)6230.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6230.,(float)6240.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6240.,(
	    float)6250.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6250.,(float)6260.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6260.,(float)6270.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6270.,(
	    float)6280.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6280.,(float)6290.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6290.,(float)6300.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6300.,(
	    float)6310.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6310.,(float)6320.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6320.,(float)6330.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6330.,(
	    float)6340.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6340.,(float)6350.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6350.,(float)6360.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6360.,(
	    float)6370.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6370.,(float)6380.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6380.,(float)6390.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6390.,(
	    float)6400.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6400.,(float)6410.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6410.,(float)6420.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6420.,(
	    float)6430.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6430.,(float)6440.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6440.,(float)6450.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6450.,(
	    float)6460.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6460.,(float)6470.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6470.,(float)6480.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6480.,(
	    float)6490.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6490.,(float)6500.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6500.,(float)6510.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6510.,(
	    float)6520.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6520.,(float)6530.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6530.,(float)6540.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6540.,(
	    float)6550.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6550.,(float)6560.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6560.,(float)6570.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6570.,(
	    float)6580.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6580.,(float)6590.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6590.,(float)6600.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6600.,(
	    float)6610.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6610.,(float)6620.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6620.,(float)6630.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6630.,(
	    float)6640.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6640.,(float)6650.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6650.,(float)6660.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6660.,(
	    float)6670.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6670.,(float)6680.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6680.,(float)6690.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6690.,(
	    float)6700.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6700.,(float)6710.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6710.,(float)6720.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6720.,(
	    float)6730.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6730.,(float)6740.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6740.,(float)6750.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6750.,(
	    float)6760.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6760.,(float)6770.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6770.,(float)6780.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6780.,(
	    float)6790.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6790.,(float)6800.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6800.,(float)6810.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6810.,(
	    float)6820.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6820.,(float)6830.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6830.,(float)6840.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6840.,(
	    float)6850.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6850.,(float)6860.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6860.,(float)6870.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6870.,(
	    float)6880.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6880.,(float)6890.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6890.,(float)6900.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6900.,(
	    float)6910.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6910.,(float)6920.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6920.,(float)6930.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6930.,(
	    float)6940.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6940.,(float)6950.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6950.,(float)6960.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6960.,(
	    float)6970.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)6970.,(float)6980.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)6980.,(float)6990.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)6990.,(
	    float)7e3,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7e3,(float)7010.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7010.,(float)7020.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7020.,(
	    float)7030.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7030.,(float)7040.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7040.,(float)7050.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7050.,(
	    float)7060.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7060.,(float)7070.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7070.,(float)7080.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7080.,(
	    float)7090.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7090.,(float)7100.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7100.,(float)7110.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7110.,(
	    float)7120.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7120.,(float)7130.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7130.,(float)7140.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7140.,(
	    float)7150.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7150.,(float)7160.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7160.,(float)7170.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7170.,(
	    float)7180.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7180.,(float)7190.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7190.,(float)7200.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7200.,(
	    float)7210.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7210.,(float)7220.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7220.,(float)7230.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7230.,(
	    float)7240.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7240.,(float)7250.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7250.,(float)7260.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7260.,(
	    float)7270.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7270.,(float)7280.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7280.,(float)7290.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7290.,(
	    float)7300.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7300.,(float)7310.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7310.,(float)7320.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7320.,(
	    float)7330.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7330.,(float)7340.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7340.,(float)7350.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7350.,(
	    float)7360.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7360.,(float)7370.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7370.,(float)7380.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7380.,(
	    float)7390.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7390.,(float)7400.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7400.,(float)7410.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7410.,(
	    float)7420.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7420.,(float)7430.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7430.,(float)7440.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7440.,(
	    float)7450.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7450.,(float)7460.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7460.,(float)7470.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7470.,(
	    float)7480.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7480.,(float)7490.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7490.,(float)7500.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7500.,(
	    float)7510.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7510.,(float)7520.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7520.,(float)7530.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7530.,(
	    float)7540.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7540.,(float)7550.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7550.,(float)7560.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7560.,(
	    float)7570.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7570.,(float)7580.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7580.,(float)7590.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(float)7590.,(
	    float)7600.,(float)0.,(float)0.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)7600.,(float)7610.,(float)0.,(float)0.,(float)0.,(
	    float)0.,(float)0.,(float)0.,(float)7610.,(float)7620. };

    integer i__;

/*<        real a(8) >*/
/*<        real acr(8,256) >*/
/*<        integer inu,j,k,i >*/
/*     nitrous oxide (5060 - 7610 cm-1) */

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
} /* niox2_ */

#ifdef __cplusplus
	}
#endif
