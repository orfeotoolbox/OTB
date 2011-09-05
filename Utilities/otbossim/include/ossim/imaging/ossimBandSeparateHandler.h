//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Oscar Kramer
// 
// Description:
//
// Image handler used when the multi-band image has each band represented by a different disk file.
// This is the case for some Ikonos imagery. 
//
//**************************************************************************************************
// $Id: ossimBandSeparateHandler.h 2644 2011-05-26 15:20:11Z oscar.kramer $
#ifndef ossimBandSeparateHandler_HEADER
#define ossimBandSeparateHandler_HEADER

#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimBandMergeSource.h>

/**
 *  This class defines an abstract Handler which all image handlers(loaders)
 *  should derive from.
 */
class OSSIMDLLEXPORT ossimBandSeparateHandler : public ossimImageHandler
{
public:
   //!  Constructor (default):
   ossimBandSeparateHandler();
   
   //! Destructor:
   virtual ~ossimBandSeparateHandler();
   
   //! Opens multiple band-files based on info in metadata file.
   //! @return Returns true on success, false on error.
   virtual bool open();

   //! Closes all band-files and clears input handler list.
   virtual void close();

   //! @return true if open, false if not.
   virtual bool isOpen()const;

   virtual ossim_uint32 getNumberOfInputBands() const;
   
   //! @param resLevel Reduced resolution level to return lines of.
   //! Default = 0
   //! @return The number of lines for specified reduced resolution level.
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel = 0) const;

   //! @param resLevel Reduced resolution level to return samples of.
   //! Default = 0
   //! @return The number of samples for specified reduced resolution level.
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel = 0) const;
   
   //! Method to save the state of an object to a keyword list.
   //! Return true if ok or false on error.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   //! Method to the load (recreate) the state of an object from a keyword
   //! list.  Return true if ok or false on error.
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   //! Fills the requested tile by pulling pixels from multiple file tiles as needed.
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect, ossim_uint32 resLevel);

   //! Returns scalar type of first band (should be the same for all bands)
   virtual ossimScalarType getOutputScalarType() const;

   //! Overrides base connection method to connect output of merge source.
   virtual bool canConnectMyOutputTo(ossim_int32 index, const ossimConnectableObject* obj);

   //! Returns the geometry of the first band:
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   //! These images are not tiled
   virtual ossim_uint32 getImageTileWidth()  const { return 0; }
   virtual ossim_uint32 getImageTileHeight() const { return 0; }

protected:
   //! Fetches the list of band file names from the header file. The header file name is contained
   //! in ossimImageHandler::theImageFile.
   //! NOTE TO PROGRAMMER: This class is presently tailored to the Ikonos metadata file specifying
   //! the individual band files. If other "metadata" formats are to be supported, then this class
   //! should be derived from. This method would therefore be pure virtual in this base-class and 
   //! only this method would need to be implemented in the derived classes.
   virtual void getBandFileNames(vector<ossimFilename>& file_names);

  vector<ossimRefPtr<ossimImageHandler> > m_bandFiles;
  ossimRefPtr<ossimBandMergeSource>       m_mergeSource;

TYPE_DATA
};

#endif /* #ifndef ossimBandSeparateHandler_HEADER */
