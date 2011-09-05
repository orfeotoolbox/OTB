//*************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description:
// 
// Image handler used when the multi-band image has each band represented by a different disk file.
// This is the case for some Ikonos imagery. 
//
//*************************************************************************************************
//  $Id: ossimBandSeparateHandler.cpp 2644 2011-05-26 15:20:11Z oscar.kramer $

#include <algorithm>

#include <ossim/imaging/ossimBandSeparateHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimStdOutProgress.h>

RTTI_DEF1(ossimBandSeparateHandler, "ossimBandSeparateHandler", ossimImageHandler)

//*************************************************************************************************
//!  Constructor (default):
//*************************************************************************************************
ossimBandSeparateHandler::ossimBandSeparateHandler()
{ }

//*************************************************************************************************
//! Destructor:
//*************************************************************************************************
ossimBandSeparateHandler::~ossimBandSeparateHandler()
{
   close();
}

//*************************************************************************************************
// Opens multiple band-files based on info in metadata file.
//*************************************************************************************************
bool ossimBandSeparateHandler::open()
{
   if (isOpen())
      close();

   // Fetch the list of band filenames:
   vector<ossimFilename> file_names;
   getBandFileNames(file_names);

   // There should be a list of filenames populated:
   if (file_names.size() == 0)
      return false;

   m_mergeSource = new ossimBandMergeSource;
   ossimRefPtr<ossimImageHandler> handler = 0;
   for (ossim_uint32 band=0; band<file_names.size(); ++band)
   {
      // Open input band file:
      handler = ossimImageHandlerRegistry::instance()->open(file_names[band], true, false);
      if (!handler.valid())
      {
         ossimNotify(ossimNotifyLevel_WARN)<<"ossimBandSeparateHandler::open() -- "
            "Expected to find band file at <"<<file_names[band]<<"> but could not open file."
            " Image not opened."<<endl;
         close();
         return false;
      }

      // Add this band file to the merge source:
      m_bandFiles.push_back(handler);
      m_mergeSource->connectMyInputTo(band, handler.get());
   }

   completeOpen();
   return true;
}

//*************************************************************************************************
//! Deletes the overview and clears the valid image vertices.  Derived
//! classes should implement.
//*************************************************************************************************
void ossimBandSeparateHandler::close()
{
   vector<ossimRefPtr<ossimImageHandler> >::iterator iter = m_bandFiles.begin();
   while (iter != m_bandFiles.end())
   {
      (*iter)->close();
      *iter = 0;
      ++iter;
   }
   m_bandFiles.clear();
   m_mergeSource = 0;
}

//*************************************************************************************************
//! Derived classes must implement this method to be concrete.
//! @return true if open, false if not.
//*************************************************************************************************
bool ossimBandSeparateHandler::isOpen()const
{
   if (m_bandFiles.size() > 0)
      return true;

   return false;
}

//*************************************************************************************************
//! Fills the requested tile by pulling pixels from multiple file tiles as needed.
//*************************************************************************************************
ossimRefPtr<ossimImageData> ossimBandSeparateHandler::getTile(const ossimIrect& tile_rect, 
                                                              ossim_uint32 resLevel)
{
   // First verify that there are band-files available:
   if ((m_bandFiles.size() == 0) || !m_mergeSource.valid())
      return ossimRefPtr<ossimImageData>();

   // Check if res level represents data in the overview:
   if (theOverview.valid() && (resLevel > 0))
      return theOverview->getTile(tile_rect, resLevel);

   // Just pass getTile call on to contained bandMergeSource:
   return m_mergeSource->getTile(tile_rect, resLevel);
}

//*************************************************************************************************
//! @param resLevel Reduced resolution level to return lines of.
//! Default = 0
//! @return The number of lines for specified reduced resolution level.
//*************************************************************************************************
ossim_uint32 ossimBandSeparateHandler::getNumberOfLines(ossim_uint32 resLevel) const
{
   if ((m_bandFiles.size() == 0))
      return 0;

   // Using simple decimation by powers of 2:
   ossim_uint32 numlines = m_bandFiles[0]->getNumberOfLines() >> resLevel;
   return numlines;
}

//*************************************************************************************************
//! @param resLevel Reduced resolution level to return samples of.
//! Default = 0
//! @return The number of samples for specified reduced resolution level.
//*************************************************************************************************
ossim_uint32 ossimBandSeparateHandler::getNumberOfSamples(ossim_uint32 resLevel) const
{
   if ((m_bandFiles.size() == 0))
      return 0;

   // Using simple decimation by powers of 2:
   ossim_uint32 numsamps = m_bandFiles[0]->getNumberOfSamples() >> resLevel;
   return numsamps;
}

//*************************************************************************************************
//! Method to save the state of an object to a keyword list.
//! Return true if ok or false on error.
//*************************************************************************************************
bool ossimBandSeparateHandler::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   return ossimImageHandler::saveState(kwl, prefix);
}

//*************************************************************************************************
//! Method to the load (recreate) the state of an object from a keyword
//! list.  Return true if ok or false on error.
//*************************************************************************************************
bool ossimBandSeparateHandler::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   if (!ossimImageHandler::loadState(kwl, prefix))
      return false;

   return open();
}

//*************************************************************************************************
// Returns the number of bands of the first tile since all tiles need to have the same pixel type.
//*************************************************************************************************
ossim_uint32 ossimBandSeparateHandler::getNumberOfInputBands() const
{
   return (ossim_uint32) m_bandFiles.size();
}

//*************************************************************************************************
//! Returns scalar type of first tile (should be the same for all tiles)
//*************************************************************************************************
ossimScalarType ossimBandSeparateHandler::getOutputScalarType() const
{
   if ((m_bandFiles.size() == 0) || (!m_bandFiles[0].valid()))
      return OSSIM_SCALAR_UNKNOWN;

   return m_bandFiles[0]->getOutputScalarType();
}

//*************************************************************************************************
//! Overrides base connection method to connect output of merge source.
// Returns TRUE if connection possible
//*************************************************************************************************
bool ossimBandSeparateHandler::canConnectMyOutputTo(ossim_int32 index, 
                                                    const ossimConnectableObject* obj)
{
   if (!m_mergeSource.valid())
      return false;

   return m_mergeSource->canConnectMyOutputTo(index, obj);
}

//*************************************************************************************************
//! Fetches the list of band file names from the header file. The header file name is contained
//! in ossimImageHandler::theImageFile.
//! NOTE TO PROGRAMMER: This class is presently tailored to the Ikonos metadata file specifying
//! the individual band files. If other "metadata" formats are to be supported, then this class
//! should be derived from. This method would therefore be pure virtual in this base-class and only 
//! this method would need to be implemented in the derived classes.
//*************************************************************************************************
void ossimBandSeparateHandler::getBandFileNames(vector<ossimFilename>& file_names)
{
   file_names.clear();

   // Ikonos expects metadata file with text extension:
   if (theImageFile.ext() != "txt")
      return;

   // Attempt to open the metadata file:
   ifstream is (theImageFile.chars());
   if (is.fail())
   {
      ossimNotify(ossimNotifyLevel_WARN)<<"ossimBandSeparateHandler::open() -- "
         "Cannot open text file at <"<<theImageFile<<">. Image not opened."<<endl;
      return;
   }

   vector<ossimString> fname_list;
   const ossimString separator (" ");
   char line_buf[4096];
   while (!is.eof())
   {
      is.getline(line_buf, 4096);
      ossimString line_str (line_buf);

      // Look for mention of multiple band files. It must indicate "separate files". This comes 
      // before the list of band files:
      if (line_str.contains("Multispectral Files:"))
      {
         if (!line_str.contains("Separate Files"))
            return;
      }

      // Look for list of band files:
      if (line_str.contains("Tile File Name:"))
      {
         ossimString file_list_str = line_str.after(":");
         file_list_str.split(fname_list, separator, true);
         break;
      }
   }
   is.close();

   // Set the path of individual band files to match metadata file path:
   ossimFilename pathName (theImageFile.path());
   for (size_t i=0; i<fname_list.size(); ++i)
   {
      ossimFilename fname (fname_list[i]);
      fname.setPath(pathName);
      file_names.push_back(fname);
   }
}

//*************************************************************************************************
//! Returns the geometry of the first band.
//*************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimBandSeparateHandler::getImageGeometry()
{
   if ((m_bandFiles.size() == 0) || (!m_bandFiles[0].valid()))
      return 0;

   return m_bandFiles[0]->getImageGeometry();
}
