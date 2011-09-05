//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// ossimWgs84Datum.  Special hardcoded datum.  It will create a static
// instance of a Wgs84Ellipsoid and set the initial defaults for
// that are specific to a ossimWgs84Datum
//*******************************************************************
//  $Id: ossimWgs84Datum.h 19795 2011-06-30 15:04:48Z gpotts $

#ifndef ossimWgs84Datum_HEADER
#define ossimWgs84Datum_HEADER
#include <ossim/base/ossimThreeParamDatum.h>

class OSSIMDLLEXPORT ossimWgs84Datum : public ossimThreeParamDatum
{
public:
   ossimWgs84Datum();
   virtual ~ossimWgs84Datum(){}
   
   virtual ossimGpt    shift(const ossimGpt    &aPt)const;

   virtual ossimGpt    shiftToWgs84(const ossimGpt &aPt)const;
   virtual ossimGpt    shiftFromWgs84(const ossimGpt&aPt)const;

protected:
 
   TYPE_DATA;
};

#endif
