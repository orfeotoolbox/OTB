//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// ossimThreeParamDatum. This is a base class for all
// three param datums.
//*******************************************************************
//  $Id: ossimThreeParamDatum.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimThreeParamDatum_HEADER
#define ossimThreeParamDatum_HEADER
#include <ossim/base/ossimDatum.h>

class OSSIMDLLEXPORT ossimThreeParamDatum : public ossimDatum
{
public:
   ossimThreeParamDatum(const ossimString &code, const ossimString &name,
                        const ossimEllipsoid* anEllipsoid,
                        ossim_float64 sigmaX,
                        ossim_float64 sigmaY,
                        ossim_float64 sigmaZ,
                        ossim_float64 westLongitude,
                        ossim_float64 eastLongitude,
                        ossim_float64 southLatitude,
                        ossim_float64 northLatitude,
                        ossim_float64 param1,
                        ossim_float64 param2,
                        ossim_float64 param3)
      : ossimDatum(code, name, anEllipsoid,
              sigmaX, sigmaY, sigmaZ,
              westLongitude, eastLongitude,
              southLatitude, northLatitude),
        theParam1(param1),
        theParam2(param2),
        theParam3(param3)
      {}
   virtual ~ossimThreeParamDatum(){}

   /*!
    * Will shift a ground point to this datum.  If they are the same
    * datum then nothing happens.
    */
   virtual ossimGpt    shift(const ossimGpt    &aPt)const;
   
   virtual ossimGpt    shiftToWgs84(const ossimGpt &aPt)const;
   virtual ossimGpt    shiftFromWgs84(const ossimGpt &aPt)const;
   virtual ossim_float64 param1()const{return theParam1;}
   virtual ossim_float64 param2()const{return theParam2;}
   virtual ossim_float64 param3()const{return theParam3;}
   virtual ossim_float64 param4()const{return 0.0;}
   virtual ossim_float64 param5()const{return 0.0;}
   virtual ossim_float64 param6()const{return 0.0;}
   virtual ossim_float64 param7()const{return 1.0;}

protected:
   
   ossim_float64 theParam1;
   ossim_float64 theParam2;
   ossim_float64 theParam3;
};


#endif
