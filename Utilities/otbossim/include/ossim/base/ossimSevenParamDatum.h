//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
// Description:
//
// SevenParamDatum. This is a base class for all
// seven param datums.
//*******************************************************************
//  $Id: ossimSevenParamDatum.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimSevenParamDatum_HEADER
#define ossimSevenParamDatum_HEADER
#include <ossim/base/ossimDatum.h>
class OSSIMDLLEXPORT ossimEllipsoid;

class OSSIM_DLL ossimSevenParamDatum : public ossimDatum
{
public:
   ossimSevenParamDatum(const ossimString &code, const ossimString &name,
                        const ossimEllipsoid* anEllipsoid,
                        ossim_float64 sigmaX,
                        ossim_float64 sigmaY,
                        ossim_float64 sigmaZ,
                        ossim_float64 westLongitude,
                        ossim_float64 eastLongitude,
                        ossim_float64 southLatitude,
                        ossim_float64 northLatitude,
                        ossim_float64 aParam1,
                        ossim_float64 aParam2,
                        ossim_float64 aParam3,
                        ossim_float64 aParam4,
                        ossim_float64 aParam5,
                        ossim_float64 aParam6,
                        ossim_float64 aParam7)
      : ossimDatum(code, name, anEllipsoid,
                   sigmaX, sigmaY, sigmaZ,
                   westLongitude, eastLongitude,
                   southLatitude, northLatitude),
        theParam1(aParam1),
        theParam2(aParam2),
        theParam3(aParam3),
        theParam4(aParam4),
        theParam5(aParam5),
        theParam6(aParam6),
        theParam7(aParam7)
      {}
   

   /*!
    * Will shift the ground point relative to this datum.
    */
   ossimGpt         shift(const ossimGpt    &aPt)const;
   virtual ossimGpt shiftToWgs84(const ossimGpt &aPt)const;
   virtual ossimGpt shiftFromWgs84(const ossimGpt &aPt)const;
   
   virtual ossim_float64 param1()const{return theParam1;}
   virtual ossim_float64 param2()const{return theParam2;}
   virtual ossim_float64 param3()const{return theParam3;}
   virtual ossim_float64 param4()const{return theParam4;}
   virtual ossim_float64 param5()const{return theParam5;}
   virtual ossim_float64 param6()const{return theParam6;}
   virtual ossim_float64 param7()const{return theParam7;}
   
private:
   ossim_float64 theParam1;
   ossim_float64 theParam2;
   ossim_float64 theParam3;
   ossim_float64 theParam4;
   ossim_float64 theParam5;
   ossim_float64 theParam6;
   ossim_float64 theParam7;
};

#endif
