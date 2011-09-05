//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Wgs72Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs72Ellipsoid and set the initial defaults for
// that are specific to a Wgs72Datum
//*******************************************************************
//  $ID$
#ifndef ossimWgs72Datum_HEADER
#define ossimWgs72Datum_HEADER
#include <ossim/base/ossimThreeParamDatum.h>

class OSSIMDLLEXPORT ossimWgs72Datum : public ossimThreeParamDatum
{
public:
   ossimWgs72Datum();

   virtual ~ossimWgs72Datum(){}
   virtual ossimGpt    shift(const ossimGpt    &aPt)const;
   virtual ossimGpt    shiftToWgs84(const ossimGpt &aPt)const;
   virtual ossimGpt    shiftFromWgs84(const ossimGpt &aPt)const;

   TYPE_DATA;   
};

#endif
