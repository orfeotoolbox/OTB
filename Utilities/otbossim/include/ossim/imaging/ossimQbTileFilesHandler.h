//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Oscar Kramer
// 
//**************************************************************************************************
// $Id: ossimQbTileFilesHandler.h 2669 2011-06-06 12:28:20Z oscar.kramer $
#ifndef ossimQbTileFilesHandler_HEADER
#define ossimQbTileFilesHandler_HEADER

#include <ossim/imaging/ossimTiledImageHandler.h>

// Enable this flag to utilize the system of separate overviews for each tile-file
#define USING_SUB_OVRS 0

//! Image handler used for tiled Quickbird imagery. 
class OSSIMDLLEXPORT ossimQbTileFilesHandler : public ossimTiledImageHandler
{
public:
   //!  Constructor (default):
   ossimQbTileFilesHandler();
   
   //! Destructor:
   virtual ~ossimQbTileFilesHandler();
   
   //! @return Returns true on success, false on error.
   //! @note This method relies on the data member ossimImageData::theImageFile being set.  
   virtual bool open();

   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();
   
protected:

   //! Initializes tile image rects by considering adjacent row/col offsets. Called when TIL
   //! doesn't contain the info. Returns true if successful.
   bool computeImageRects();

TYPE_DATA
};

#endif /* #ifndef ossimQbTileFilesHandler_HEADER */
