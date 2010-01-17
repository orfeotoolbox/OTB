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
// $Id: ossimGeneralRasterElevFactory.h 13269 2008-07-25 14:27:36Z dburken $
#ifndef ossimGeneralRasterElevFactory_HEADER
#define ossimGeneralRasterElevFactory_HEADER

#include <ossim/elevation/ossimElevSourceFactory.h>
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
   typedef std::vector<ossimGeneralRasterElevHandler::GeneralRasterInfo> BoundingRectListType;

   /** Constructor that takes a file name. */

   /** default constructor */
   ossimGeneralRasterElevFactory();

   /** Constructor that takes a directory name. */
   ossimGeneralRasterElevFactory(const ossimFilename& dir);

   /** destructor */
   virtual ~ossimGeneralRasterElevFactory();
   void setDirectory(const ossimFilename& directory);

   /**
    * Open the appropriate handler that covers given a
    * ground point.
    *
    * @param gpt Ground point that an elevation source is need for.
    *
    * @return Returns a pointer to an ossimElevSource if an elevation file is found
    * that can cover the ground point.  Returns NULL if no cell is found
    * for the point.
    */
   virtual ossimElevSource* getNewElevSource(const ossimGpt& gpt) const;

   /** To satisfy pure virtual. */
   virtual void createIndex();

protected:
/*    void initializeList(const ossimFilename& file); */
/*    bool initializeInfo(ossimGeneralRasterElevHandler::GeneralRasterInfo& info, */
/*                        const ossimFilename& file); */
   void addInfo(const ossimGeneralRasterElevHandler::GeneralRasterInfo& info);

   
   std::vector<ossimGeneralRasterElevHandler::GeneralRasterInfo>  theGeneralRasterInfoList;
   ossimDrect            theBoundingRect;
/*    mutable std::ifstream theFileStr;  */
   mutable ossim_int32   theCurrentIdx;
   mutable bool theHandlerReturnedFlag;
   
TYPE_DATA
};

#endif /* End of "#ifndef ossimSrtmFactory_HEADER" */
