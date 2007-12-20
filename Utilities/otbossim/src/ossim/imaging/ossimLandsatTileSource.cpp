//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class implementaiton for the class "ossim LandsatTileSource".
//
//*******************************************************************
//  $Id: ossimLandsatTileSource.cpp 10752 2007-04-23 16:50:08Z dburken $

#include <ossim/imaging/ossimLandsatTileSource.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/support_data/ossimFfL7.h>
#include <ossim/support_data/ossimFfL5.h>
#include <ossim/projection/ossimLandSatModel.h>

RTTI_DEF1_INST(ossimLandsatTileSource,
               "ossimLandsatTileSource",
               ossimGeneralRasterTileSource)

static ossimTrace traceDebug("ossimLandsatTileSource:debug");


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimLandsatTileSource::ossimLandsatTileSource()
   :
      ossimGeneralRasterTileSource(),
      theFfHdr(NULL)
{
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimLandsatTileSource::ossimLandsatTileSource(const ossimKeywordlist& kwl,
                                               const char* prefix)
   :
      ossimGeneralRasterTileSource(),
      theFfHdr(NULL)
{
   if (loadState(kwl, prefix) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimLandsatTileSource::~ossimLandsatTileSource()
{
   if (theFfHdr)
   {
      delete theFfHdr;
      theFfHdr = NULL;
   }
}

bool ossimLandsatTileSource::open()
{
   static const char MODULE[] = "ossimLandsatTileSource::open";

   if (traceDebug())
   {
      CLOG << " Entered..." << std::endl
           << " trying to open file " << theImageFile << std::endl;
   }
   
   ossimFilename tempFilename = theImageFile;
   // See if the file passed in is a header file.
   
   openHeader(theImageFile);

   if (!theFfHdr) return false;

   // Start building the keyword list for the general raster base class.
   ossimKeywordlist kwl;

   //***
   // There can be up to seven (six for L7) files that belong to the header.
   // Note that it seems the file names in the header are always upper case.
   // So test the file given to us to see if they should be downcased.  This
   // is assuming that all files in the directory have the same case.
   //***
   vector<ossimFilename> fileList;
   
   for (ossim_uint32 i=0; i<theFfHdr->getBandCount(); ++i)
   {
      bool addFile = false;
      ossimFilename f1 = theFfHdr->getBandFilename(i);
      if (f1.trim() != "")
      {
         // Make the file name.
         ossimFilename f2 = theImageFile.path();
         f2 = f2.dirCat(f1);
         
         if (f2.exists())
         {
            addFile = true;
         }
         else
         {
            // Try it downcased...
            f2 = theImageFile.path();
            f1.downcase();
            f2 = f2.dirCat(f1);
            if (f2.exists())
            {
               addFile = true;
            }
            else
            {
               // Try is upcased...
               f2 = theImageFile.path();
               f1.upcase();
               f2 = f2.dirCat(f1);
               if (f2.exists())
               {
                  addFile = true;
               }
            }
         }
         
         if (addFile)
         {
            if (traceDebug())
            {
               CLOG << "\nAdding file:  " << f2 << std::endl;
            }
            fileList.push_back(f2);
         }
         else 
         {
            if (traceDebug())
            {
               f2 = theImageFile.path();
               f1 = theFfHdr->getBandFilename(i);
               f1.trim();
               f2 = f2.dirCat(f1);
               CLOG << "\nCould not find:  " << f2 << std::endl;
            }
         }
      }
   }
   
   if(fileList.size() == 0)
   {
      close();
      return false;
   }
   
   ossimGeneralRasterInfo generalRasterInfo(fileList,
					    OSSIM_UINT8,
					    OSSIM_BSQ_MULTI_FILE,
					    fileList.size(),
					    theFfHdr->getLinesPerBand(),
					    theFfHdr->getPixelsPerLine(),
					    0,
					    ossimGeneralRasterInfo::NONE,
					    0);
   if(fileList.size() == 1)
   {
      generalRasterInfo = ossimGeneralRasterInfo(fileList,
                                                 OSSIM_UINT8,
                                                 OSSIM_BSQ,
                                                 fileList.size(),
                                                 theFfHdr->getLinesPerBand(),
                                                 theFfHdr->getPixelsPerLine(),
                                                 0,
                                                 ossimGeneralRasterInfo::NONE,
                                                 0);
   }
   theMetaData.clear();
   theMetaData.setScalarType(OSSIM_UINT8);
   theMetaData.setNumberOfBands(fileList.size());   
   theImageData = generalRasterInfo;
   if(initializeHandler())
   {
      theImageFile = tempFilename;
      
      completeOpen();
   }
   else
   {
      if (traceDebug()) CLOG << " Exited..." << std::endl;
      return false;
   }
   
   if (traceDebug()) CLOG << " Exited..." << std::endl;
   
   return true;
}
   
void ossimLandsatTileSource::openHeader(const ossimFilename& file)
{
   //***
   // Landsat file name example:  l71024031_03119990929_hpn.fst
   // Three header header file type substrings:
   // HPN = Pan
   // HRF = VNIR/SWIR (visible near infrared/shortwave infrared)
   // HTM = Thermal
   //***
   ossimFilename hdr = file.file();
   hdr.downcase();
   if ( hdr.contains("hpn") || hdr.contains("hrf") || hdr.contains("htm") )
   {
      theFfHdr = new ossimFfL7(file.c_str());      
   } else if (hdr.contains("header.dat"))
   {
      theFfHdr = new ossimFfL5(file.c_str());
   } else {
      theFfHdr = NULL;
      return;
   }
   if (theFfHdr->getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      delete theFfHdr;
      theFfHdr = NULL;
   }
   return;

   // I had to force the open to go with a header since there are duplicate entries when scanning
   // landsat directories.
   //  For now I am commenting this code out.
   //
#if 0
   //***
   // User may have passed in an image file name in which case the header file
   // must be derived from it.
   //***
   if (hdr.size() < 25)
   {
      // file name not long enough...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimLandsatTileSource::openHeader DEBUG:"
            << "\nNot a standard landsat 7 file name:  " << hdr << std::endl;
         return;
      }
   }
   
   char substr[4];
   const char* f = hdr.c_str();
   strncpy(substr, (f+22), 3);
   substr[3] = '\0';
   ossimString s1 = substr;
   ossimString s2;
   s1.downcase();
   if (s1 == "b80")
   {
      s2 = "hpn";
   }
   else if (s1 == "b61" || s1 == "b62")
   {
      s2 = "htm";
   }
   else if (s1 == "b10" || s1 == "b20" || s1 == "b30" ||
            s1 == "b40" || s1 == "b50" || s1 == "b70")
   {
      s2 = "hrf";
   }
   else
   {
      // Not of any format we know of...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimLandsatTileSource::openHeader DEBUG:"
            << "\nCould not derive header name from:  " << file
            << std::endl;
      }

      return;
   }

   // Set the case to be the same as the file passed in.
   if (substr[0] == 0x42) // ascii "B"
   {
      s1.upcase();
      s2.upcase();
      hdr.upcase();

      // Header files alway start with "L71"
      hdr = hdr.substitute(ossimString("L72"), ossimString("L71"));  
   }
   else
   {
      // Header files alway start with "l71"
      hdr = hdr.substitute(ossimString("l72"), ossimString("l71")); 
   }

   // Make the hdr file name.
   hdr = hdr.substitute(s1, s2);

   ossimFilename f1 = file.drive();
   f1 += file.path();
   hdr = f1.dirCat(hdr);
   theFfHdr = new ossimFfL7(hdr.c_str());

   if (theFfHdr->getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      delete theFfHdr;
      theFfHdr = NULL;
   }
#endif
}
   
bool ossimLandsatTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                              const char* prefix)
{
   // Check for override for an external geometry file, or a previous save.
   if(ossimImageHandler::getImageGeometry(kwl, prefix))
   {
      return true;
   }
   
   if (!theFfHdr) return false;

   // Make a model
   ossimLandSatModel model(*theFfHdr);

   if (model.getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
   
      return false;
   }

   bool result = model.saveState(kwl, prefix);
   if (result)
   {
      // Capture for next time...
      setImageGeometry(kwl);
   }
   return result;
}

bool ossimLandsatTileSource::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if (lookup)
   {
      ossimFilename fileName = lookup;

      ossimString ext = fileName.ext();

      if((ext.upcase() == "FST") || (ext.upcase() == "DAT"))
      {
         //---
         // This will call:
         // ossimImageHandler::loadState() the open()
         //---
         if (ossimGeneralRasterTileSource::loadState(kwl, prefix))
         {
            return true;
         }
      }
   }
   return false;
}

ossimRefPtr<ossimProperty> ossimLandsatTileSource::getProperty(
   const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = 0;

   if (theFfHdr)
   {
      result = theFfHdr->getProperty(name);
   }

   if ( result.valid() == false )
   {
      result = ossimGeneralRasterTileSource::getProperty(name);
   }

   return result;
}

void ossimLandsatTileSource::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   if (theFfHdr)
   {
      theFfHdr->getPropertyNames(propertyNames);
   }
   ossimGeneralRasterTileSource::getPropertyNames(propertyNames);
}

ossimString ossimLandsatTileSource::getShortName() const
{
   return ossimString("Landsat");
}
   
ossimString ossimLandsatTileSource::getLongName() const
{
   return ossimString("Landsat reader");
}

ossimString  ossimLandsatTileSource::className() const
{
   return ossimString("ossimLandsatTileSource");
}
 
double ossimLandsatTileSource::getNullPixelValue(ossim_uint32)const
{
   return 0.0;
}

double ossimLandsatTileSource::getMinPixelValue(ossim_uint32)const
{
   return 1.0;
}

double ossimLandsatTileSource::getMaxPixelValue(ossim_uint32)const
{
   return 255.0;
}

ossimScalarType ossimLandsatTileSource::getOutputScalarType() const
{
   return OSSIM_UINT8;
}

bool ossimLandsatTileSource::getAcquisitionDate(ossimDate& date)const
{
   if(!theFfHdr) return false;

   theFfHdr->getAcquisitionDate(date);

   return true;
}

ossimString ossimLandsatTileSource::getSatelliteName()const
{
   if(!theFfHdr) return "";

   return theFfHdr->getSatelliteName();
}

ossimFilename ossimLandsatTileSource::getBandFilename(ossim_uint32 idx)const
{
   ossim_uint32 maxIdx = getNumberOfInputBands();

   if(!theFfHdr||(idx > maxIdx))
   {
      return "";
   }

   ossimFilename path = getFilename().path();
   ossimString filename = theFfHdr->getBandFilename(idx);
   filename = filename.trim();
   ossimFilename file = path.dirCat(filename);

   if (file.exists())
   {
      return file;
   }

   // Try downcased name.
   file = path.dirCat(filename.downcase());
   if (file.exists())
   {
      return file;
   }
   
   // Try upcase name.
   file = path.dirCat(filename.upcase());
   if (file.exists())
   {
      return file;
   }
   
   return ossimFilename();
}

bool ossimLandsatTileSource::isPan()const
{
   return (getNumberOfInputBands() == 1);
}

bool ossimLandsatTileSource::isVir()const
{
   return (getNumberOfInputBands() == 6);
}

bool ossimLandsatTileSource::isTm()const
{
   return (getNumberOfInputBands() == 2);
}
   
