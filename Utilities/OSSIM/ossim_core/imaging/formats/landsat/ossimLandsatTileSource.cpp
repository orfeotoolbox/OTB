//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class implementaiton for the class "ossim LandsatTileSource".
//
//*******************************************************************
//  $Id: ossimLandsatTileSource.cpp,v 1.27 2005/11/10 15:07:49 gpotts Exp $

#include <imaging/formats/landsat/ossimLandsatTileSource.h>
#include <base/data_types/ossimDirectory.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimKeywordNames.h>
#include <support_data/ff_l7/ossimFfL7.h>
#include <support_data/ff_l7/ossimFfL5.h>
#include <projections/sensor_modeling/landsat/ossimLandSatModel.h>

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

   if (traceDebug()) CLOG << " Entered..." << endl
                          << " trying to open file " << theImageFile << std::endl;

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
   ossimFilename f1 = theImageFile.file();
   vector<ossimFilename> fileList;

   const char* c = f1.c_str();
   bool downcase = false;
   if ((c[0]<='z') && (c[0]>='a')) downcase = true;
   for (ossim_uint32 i=0; i<theFfHdr->getBandCount(); ++i)
   {
      f1 = theFfHdr->theBandFileNames[i];
      if (f1.trim() != "")
      {
         if (downcase) f1.downcase();
         
          // Make the file name.
          ossimFilename f2 = theImageFile.path();
          f2 = f2.dirCat(f1);

          if (traceDebug())
          {
             CLOG << "DEBUG:"
                  << "\nfile[" << i << "]  " << f2.c_str() << endl;
          }

          // See if it exists.
          if (f2.exists())
          {
             if (traceDebug())
             {
                CLOG << "DEBUG:"
                     << "\nAdding file:  " << f2 << endl;
             }
	     fileList.push_back(f2);
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
					    theFfHdr->theLinesPerBand,
					    theFfHdr->thePixelsPerLine,
					    0,
					    ossimGeneralRasterInfo::NONE,
					    0);
   if(fileList.size() == 1)
   {
      generalRasterInfo = ossimGeneralRasterInfo(fileList,
                                                 OSSIM_UINT8,
                                                 OSSIM_BSQ,
                                                 fileList.size(),
                                                 theFfHdr->theLinesPerBand,
                                                 theFfHdr->thePixelsPerLine,
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
      if (traceDebug()) CLOG << " Exited..." << endl;
      return false;
   }
   
   if (traceDebug()) CLOG << " Exited..." << endl;
   
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
         cout << "ossimLandsatTileSource::openHeader DEBUG:"
              << "\nNot a standard landsat 7 file name:  " << hdr << endl;
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
         cout << "ossimLandsatTileSource::openHeader DEBUG:"
              << "\nCould not derive header name from:  " << file
              << endl;
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
         bool result =  ossimImageHandler::open(fileName);
         ossimImageSource::loadState(kwl, prefix);
         return result;
      }
   }
      
   return ossimGeneralRasterTileSource::loadState(kwl, prefix);
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

   std::cout << "Date = " << theFfHdr->theAcquisitionDate << std::endl;
   
   ossimString y = ossimString(theFfHdr->theAcquisitionDate,
                               theFfHdr->theAcquisitionDate+4);
   ossimString m = ossimString(theFfHdr->theAcquisitionDate+4,
                               theFfHdr->theAcquisitionDate+6);
   ossimString d = ossimString(theFfHdr->theAcquisitionDate+6,
                               theFfHdr->theAcquisitionDate+8);

   date = ossimDate(m.toInt(),
                    d.toInt(),
                    y.toInt());
   
   return true;
}

ossimString ossimLandsatTileSource::getSatelliteName()const
{
   if(!theFfHdr) return "";

   return theFfHdr->theSatName;
}

ossimFilename ossimLandsatTileSource::getBandFilename(ossim_uint32 idx)const
{
   ossim_uint32 maxIdx = getNumberOfInputBands();

   if(!theFfHdr||(idx > maxIdx))
   {
      return "";
   }

   ossimFilename path = getFilename().path();
   ossimString filename = theFfHdr->theBandFileNames[idx];
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
   
