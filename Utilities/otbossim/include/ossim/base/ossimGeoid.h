//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of class ossimGeoid. Maintains a grid of elevation
//   offsets for the geoid (mean sea level) relative to WGS-84 ellipsoid.
//
// SOFTWARE HISTORY:
//>
//   17Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************

#ifndef ossimGeoid_HEADER
#define ossimGeoid_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>

class OSSIMDLLEXPORT ossimGpt;
class OSSIMDLLEXPORT ossimFilename;

class OSSIMDLLEXPORT ossimGeoid : public ossimObject,
				  public ossimErrorStatusInterface
{
public:
   ossimGeoid();
   virtual ~ossimGeoid();

   virtual bool open(const ossimFilename& dir, ossimByteOrder byteOrder=OSSIM_BIG_ENDIAN) = 0;

   /**
    *  @return The offset from the ellipsoid to the geoid.  Returns
    *  ossim::nan() if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt& gpt) const = 0;

protected:
   
   TYPE_DATA
};
#endif
