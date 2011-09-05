#ifndef ossimNadconNasDatum_HEADER
#define ossimNadconNasDatum_HEADER

#include <ossim/base/ossimNadconGridDatum.h>
#include <ossim/base/ossimNadconGridFile.h>

class ossimNadconNasDatum : public ossimNadconGridDatum
{
public:
   ossimNadconNasDatum(const ossimFilename& nadconDirectory);

   // Argument holds the source point and datum.  Returns another
   // point with this datum.
   //
   virtual ossimGpt shift(const ossimGpt    &aPt)const;
   TYPE_DATA;
};

#endif
