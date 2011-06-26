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

class ossimGpt;
class ossimFilename;

class OSSIMDLLEXPORT ossimGeoid : public ossimObject,
				  public ossimErrorStatusInterface
{
public:
   ossimGeoid();

   virtual bool open(const ossimFilename& dir, ossimByteOrder byteOrder=OSSIM_BIG_ENDIAN) = 0;

   /**
    *  @return The offset from the ellipsoid to the geoid.  Returns
    *  ossim::nan() if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt& gpt) const = 0;

protected:
   virtual ~ossimGeoid();
   
   TYPE_DATA
};

/**
 * Identity geoid.
 */
class OSSIM_DLL ossimIdentityGeoid : public ossimGeoid
{
public:
   virtual ossimString getShortName()const
   {
      return ossimString("identity");
   }
   virtual bool open(const ossimFilename& /*dir*/, ossimByteOrder)
   {
      return false; // can't be opened 
   }
   virtual double offsetFromEllipsoid(const ossimGpt& /*gpt*/) const
   {
      return 0.0;
   }
   
   TYPE_DATA
};
#endif
