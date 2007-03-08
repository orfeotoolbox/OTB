//----------------------------------------------------------------------------
// License:  see top level LICENSE.txt file
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// elevation handler for general raster elevation files.
//
//----------------------------------------------------------------------------
// $Id: ossimGeneralRasterElevFactory.h 10042 2006-12-07 16:08:51Z dburken $
#ifndef ossimGeneralRasterElevFactory_HEADER
#define ossimGeneralRasterElevFactory_HEADER

#include <ossim/elevation/ossimElevSourceFactory.h>
#include <ossim/elevation/ossimElevationShapeIdx.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/elevation/ossimGeneralRasterElevHandler.h>

/**
 * @class ossimSrtmFactory Used by the elevation manager, this class returns
 * an ossimSrtmElevSource given a ground point and some directory with srtm
 * files in it.
 */
class OSSIM_DLL ossimGeneralRasterElevFactory : public ossimElevSourceFactory
{
public:

   /** default constructor */
   ossimGeneralRasterElevFactory();

   /** Constructor that takes a directory name. */
   ossimGeneralRasterElevFactory(const ossimFilename& dir);

   /** destructor */
   virtual ~ossimGeneralRasterElevFactory();

   /**
    * Open the appropriate handler that covers given a
    * ground point.
    *
    * @param gpt Ground point that an elevation source is need for.
    *
    * @return Returns a pointer to an ossimElevSource if an srtm file is found
    * that can cover the ground point.  Returns NULL if no cell is found
    * for the point.
    */
   virtual ossimElevSource* getNewElevSource(const ossimGpt& gpt) const;

   /** To satisfy pure virtual. */
   virtual void createIndex();

protected:
   mutable bool theHandlerReturnedFlag;
   
TYPE_DATA
};

#endif /* End of "#ifndef ossimSrtmFactory_HEADER" */
