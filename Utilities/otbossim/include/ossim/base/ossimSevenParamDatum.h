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
//  $Id: ossimSevenParamDatum.h 19795 2011-06-30 15:04:48Z gpotts $

#ifndef ossimSevenParamDatum_HEADER
#define ossimSevenParamDatum_HEADER
#include <ossim/base/ossimDatum.h>

class ossimEllipsoid;

class OSSIM_DLL ossimSevenParamDatum : public ossimDatum
{
public:
   /**
    *   @param code           new datum code.                               (input)
    *   @param name           Name of the new datum                         (input)
    *   @param SigmaX         Standard error in X in meters                 (input)
    *   @param SigmaY         Standard error in Y in meters                 (input)
    *   @param SigmaZ         Standard error in Z in meters                 (input)
    *   @param southLatitude  Southern edge of validity rectangle in radians(input)
    *   @param northLatitude  Northern edge of validity rectangle in radians(input)
    *   @param westLongitude  Western edge of validity rectangle in radians (input)
    *   @param eastLongitude  Eastern edge of validity rectangle in radians (input)
    *   @param param1         X translation
    *   @param param2         Y translation
    *   @param param3         Z translation
    *   @param param4         X Rotation
    *   @param param5         Y Rotation
    *   @param param6         Z Rotation
    *   @param param7         x,y,z Scale
    */
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
   virtual bool isEqualTo(const ossimObject& obj, ossimCompareType compareType=OSSIM_COMPARE_FULL)const;
   
private:
   ossim_float64 theParam1;
   ossim_float64 theParam2;
   ossim_float64 theParam3;
   ossim_float64 theParam4;
   ossim_float64 theParam5;
   ossim_float64 theParam6;
   ossim_float64 theParam7;
   
   TYPE_DATA;
};

#endif
