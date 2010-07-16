//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Doug Shibla (dshibla@imagelinks.com)
//
// DESCRIPTION: 
//
// LIMITATIONS: 
//
//*****************************************************************************

#include <cstdio>
#include <cstdlib>
#include <ossim/projection/ossimNitfMapModel.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimNitfMapModel, "ossimNitfMapModel", ossimSensorModel);

//***
// Define Trace flags for use within this file:
//***
static ossimTrace traceExec  ("ossimNitfMapModel:exec");
static ossimTrace traceDebug ("ossimNitfMapModel:debug");

static const char* PIAIMC_TAG  = "PIAIMC";
static const char* USE26A_TAG  = "USE26A";
static const char* USE00A_TAG  = "USE00A";
static const char* IMAGE_ID_KW = "image_id";

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimNitfMapModel()
//  
//*****************************************************************************
ossimNitfMapModel::ossimNitfMapModel()
   : ossimSensorModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::ossimNitfMapModel: entering..." << std::endl;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::ossimNitfMapModel: returning..." << std::endl;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimNitfMapModel(filename)
//  
//  Constructs model from a filename. The file can be either a FF header file
//  or a KWL file.
//  
//*****************************************************************************
ossimNitfMapModel::ossimNitfMapModel(const ossimFilename& init_file)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::ossimNitfMapModel(init_file): entering..." << std::endl;

   //***
   // Open the NITF file:
   //***
   FILE* fptr = fopen (init_file.chars(), "r");
   if (!fptr)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimNitfMapModel::ossimNitfMapModel(init_file): Could not open NITF file at: <" << init_file << ">."
                                             << " Aborting with error..." << std::endl;
      }
      theErrorStatus = 1;
      return;
   }

   //***
   // Read a sufficient number of bytes to include USE tag:
   //***
   char* header = new char [6000];
   fread(header, 6000, 1, fptr);
   fclose(fptr);
   fptr = 0;
   //***
   // Validate correct NITF version:
   //***
   if (strncmp(header, "NITF02.00", 9))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::ossimNitfMapModel(init_file):"
                                             << "\n\tFailed attempt to open <" << init_file
                                             << "> as NITF2.00."<<endl;
      }
      theErrorStatus = 1;
      delete [] header;
      return;
   }

   //***
   // Fetch Image ID:
   //***
   char charbuf[80];
   strncpy(charbuf, &header[447], 28);
   charbuf[28] = '\0';
   theImageID = charbuf;

   //***
   // Fetch Image Size:
   //***
   strncpy(charbuf, &header[737], 8);
   charbuf[8] = '\0';
   theImageSize.line = atoi(charbuf);
   strncpy(charbuf, &header[745], 8);
   charbuf[8] = '\0';
   theImageSize.samp = atoi(charbuf);

   
   // Fetch Image Size:
   ossimString coord;
   strncpy(charbuf, &header[775], 1);
   charbuf[1] = '\0';
   coord = charbuf;
   if (coord == "G")
   {
      double degreeLat;
      double degreeLon;
      double minute;
      double second;
      ossimString hemis;
      
      strncpy(charbuf, &header[780], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[778], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[776], 2);
      charbuf[2] = '\0';
      degreeLat = atof(charbuf);
      degreeLat = degreeLat + minute;
      strncpy(charbuf, &header[782], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "S")
         degreeLat = 0 - degreeLat;
      strncpy(charbuf, &header[788], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[786], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[783], 3);
      charbuf[3] = '\0';
      degreeLon = atof(charbuf);
      degreeLon = degreeLon + minute;
      strncpy(charbuf, &header[790], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "W")
         degreeLon = 0 - degreeLon;
      ossimDpt ip0 (degreeLon, degreeLat);
      strncpy(charbuf, &header[795], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[793], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[791], 2);
      charbuf[2] = '\0';
      degreeLat = atof(charbuf);
      degreeLat = degreeLat + minute;
      strncpy(charbuf, &header[797], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "S")
         degreeLat = 0 - degreeLat;
      strncpy(charbuf, &header[803], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[801], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[798], 3);
      charbuf[3] = '\0';
      degreeLon = atof(charbuf);
      degreeLon = degreeLon + minute;
      strncpy(charbuf, &header[805], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "W")
         degreeLon = 0 - degreeLon;
      ossimDpt ip1 (degreeLon, degreeLat);
      strncpy(charbuf, &header[810], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[808], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[806], 2);
      charbuf[2] = '\0';
      degreeLat = atof(charbuf);
      degreeLat = degreeLat + minute;
      strncpy(charbuf, &header[812], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "S")
         degreeLat = 0 - degreeLat;
      strncpy(charbuf, &header[818], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[816], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[813], 3);
      charbuf[3] = '\0';
      degreeLon = atof(charbuf);
      degreeLon = degreeLon + minute;
      strncpy(charbuf, &header[820], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "W")
         degreeLon = 0 - degreeLon;
      ossimDpt ip2 (degreeLon, degreeLat);
      strncpy(charbuf, &header[825], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[823], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[821], 2);
      charbuf[2] = '\0';
      degreeLat = atof(charbuf);
      degreeLat = degreeLat + minute;
      strncpy(charbuf, &header[827], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "S")
         degreeLat = 0 - degreeLat;
      strncpy(charbuf, &header[833], 2);
      charbuf[2] = '\0';
      second = atof(charbuf);
      second = second / 60;
      strncpy(charbuf, &header[831], 2);
      charbuf[2] = '\0';
      minute = atof(charbuf);
      minute = (minute + second) / 60;
      strncpy(charbuf, &header[828], 3);
      charbuf[3] = '\0';
      degreeLon = atof(charbuf);
      degreeLon = degreeLon + minute;
      strncpy(charbuf, &header[835], 1);
      charbuf[1] = '\0';
      hemis = charbuf;
      if (hemis == "W")
         degreeLon = 0 - degreeLon;
      ossimDpt ip3 (degreeLon, degreeLat);
      
      theBoundGndPolygon
         = ossimPolygon (ip0, ip1, ip2, ip3);
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfMapModel::ossimNitfMapModel(init_file): Could not find the corner coordinates!! "
                                            << " Aborting with error..." << std::endl;
      }
      theErrorStatus = 1;
      delete [] header;
      return;
   }
   
   
   // Search for the PIAIMC Tag to fetch GSD:
   const char* tag_ptr = strstr(header, PIAIMC_TAG);
   if (tag_ptr)
   {
      //***
      // Parse the tag for GSD (in inches):
      //***
      strncpy(charbuf, &tag_ptr[348], 7);
      charbuf[7] = '\0';
      theGSD.line = atof(charbuf)/12.0 * MTRS_PER_FT;
      theGSD.samp = theGSD.line;
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfMapModel::ossimNitfMapModel(init_file): Could not find the <" << PIAIMC_TAG
                                            << "> tag in the NITF file at: <" << init_file << "> to extract GSD."
                                            << " Searching for alternate <" << USE26A_TAG << ">... "<< std::endl;
      }
      

      //***
      // Search USE26A tag:
      //***
      const char* tag_ptr = strstr(header, USE26A_TAG);
      if (tag_ptr)
      {
         //***
         // Parse the tag for GSD (in inches):
         //***
         strncpy(charbuf, &tag_ptr[15], 5);
         charbuf[6] = '\0';
         theGSD.line = atof(charbuf)/12.0 * MTRS_PER_FT;
         theGSD.samp = theGSD.line;
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimNitfMapModel::ossimNitfMapModel(init_file): Could not find the <" << USE26A_TAG
                                               << "> tag in the NITF file at: <" << init_file << "> to extract GSD."
                                               << " Searching for alternate <" << USE00A_TAG << ">... "<< std::endl;
         }

         //***
         // Search USE00A tag:
         //***
         const char* tag_ptr = strstr(header, USE00A_TAG);
         if (!tag_ptr)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "WARNING ossimNitfMapModel::ossimNitfMapModel(init_file): Could not find the <" << USE00A_TAG
                                                   << "> tag in the NITF file at: <"
                                                   << init_file << ">." << " Aborting with error..." << std::endl;
            }
            theErrorStatus = 1;
            delete [] header;
            return;
         }

         //***
         // Parse the tag for GSD (in inches):
         //***
         strncpy(charbuf, &tag_ptr[15], 5);
         charbuf[6] = '\0';
         theGSD.line = atof(charbuf)/12.0 * MTRS_PER_FT;
         theGSD.samp = theGSD.line;
      }
   }

   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;
   init_file.split(drivePart,
                  pathPart,
                  filePart,
                  extPart);
   //***
   // Save current state:
   //***
   ossimFilename geom_filename;
   geom_filename.merge(drivePart,
                       pathPart,
                       filePart,
                       "geom");
   ossimKeywordlist geom_kwl;
   saveState(geom_kwl);
   geom_kwl.write(geom_filename.chars());

   delete [] header;

   if (traceExec()) ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::ossimNitfMapModel: Exited..." << std::endl;
}



//*****************************************************************************
//  CONSTRUCTOR: ossimNitfMapModel(kwl)
//  
//  Constructs model from keywordlist geometry file
//  
//*****************************************************************************
ossimNitfMapModel::ossimNitfMapModel(const ossimKeywordlist& geom_kwl)
   : ossimSensorModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::ossimNitfMapModel(geom_file): entering..." << std::endl;

   //***
   // Parse keywordlist for geometry:
   //***
   loadState(geom_kwl);

   if (traceExec()) ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::ossimNitfMapModel(geom_file): Exited..." << std::endl;
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimNitfMapModel()
//  
//*****************************************************************************
ossimNitfMapModel::~ossimNitfMapModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::~ossimNitfMapModel: entering..." << std::endl;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::~ossimNitfMapModel: returning..." << std::endl;
}

//*****************************************************************************
//  METHOD: ossimNitfMapModel::lineSampleHeightToWorld()
//  
//  Performs reverse projection of image line/sample to ground point.
//  The imaging ray is intersected with a level plane at height above ellipsoid.
//
//*****************************************************************************
void ossimNitfMapModel::lineSampleHeightToWorld(const ossimDpt& /* image_point */,
                                                const double&   /* height */,
                                                ossimGpt&       /* gpt */) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::lineSampleHeightToWorld: entering..." << std::endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::lineSampleHeightToWorld: exiting..." << std::endl;
}

//*****************************************************************************
//  METHOD: ossimNitfMapModel::print()
//  
//  Formatted dump of data members.
//  
//*****************************************************************************
std::ostream& ossimNitfMapModel::print(std::ostream& out) const
{
   out << "\nDump of ossimNitfMapModel object at " << hex << this << ":"
       << endl;

   return ossimSensorModel::print(out);
}

//*****************************************************************************
//  METHOD: ossimNitfMapModel::saveState()
//  
//  Saves the model state to the KWL. This KWL also serves as a geometry file.
//  
//*****************************************************************************
bool ossimNitfMapModel::saveState(ossimKeywordlist& kwl,
                              const char* prefix) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::saveState: entering..." << std::endl;


   // Hand off to base class for common stuff:
   ossimSensorModel::saveState(kwl, prefix);

      kwl.add(prefix, 
              IMAGE_ID_KW,
              theImageID.chars());

      kwl.add(prefix,
              ossimKeywordNames::NUMBER_LINES_KW,
              theImageSize.line,
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::NUMBER_SAMPLES_KW,
              theImageSize.samp,
              true);

      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              theGSD.line,
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              theGSD.samp,
              true);

      kwl.add(prefix,
              ossimKeywordNames::UL_Y_KW,
              0,
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::UL_X_KW,
              0,
              true);

      kwl.add(prefix,
              ossimKeywordNames::LR_Y_KW,
              theImageSize.line - 1,
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::LR_X_KW,
              theImageSize.samp - 1,
              true);

      ossimDpt ulcorner;
      if(!theBoundGndPolygon.vertex(0, ulcorner))
      {
         ulcorner = ossimDpt(0,0);
      }
      
      kwl.add(prefix,
              ossimKeywordNames::UL_LAT_KW,
              ulcorner.lat,
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::UL_LON_KW,
              ulcorner.lon,
              true);

      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LAT_KW,
              ulcorner.lat,
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LON_KW,
              ulcorner.lon,
              true);

      ossimDpt corner;
      if(!theBoundGndPolygon.nextVertex(corner))
      {
         corner = ossimDpt(0,0);
      }
      kwl.add(prefix,
              ossimKeywordNames::UR_LAT_KW,
              corner.lat,
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::UR_LON_KW,
              corner.lon,
              true);

      ossimDpt lrcorner;
      if(!theBoundGndPolygon.nextVertex(lrcorner))
      {
         lrcorner = ossimDpt(0,0);
      }
      kwl.add(prefix,
              ossimKeywordNames::LR_LAT_KW,
              lrcorner.lat,
              true);
      kwl.add(prefix,
              ossimKeywordNames::LR_LON_KW,
              lrcorner.lon,
              true);
      

      if(!theBoundGndPolygon.nextVertex(corner))
      {
         corner = ossimDpt(0,0);
      }
      kwl.add(prefix,
              ossimKeywordNames::LL_LAT_KW,
              corner.lat,
              true);
      kwl.add(prefix,
              ossimKeywordNames::LL_LON_KW,
              corner.lon,
              true);

      kwl.add(prefix,
              ossimKeywordNames::ORIGIN_LATITUDE_KW,
              (ulcorner.lat + lrcorner.lat)/2,
              true);
      kwl.add(prefix,
              ossimKeywordNames::CENTRAL_MERIDIAN_KW,
              (lrcorner.lon + ulcorner.lon)/2,
              true);

      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
              fabs((ulcorner.lat - lrcorner.lat))/(theImageSize.line - 1),
              true);
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
              fabs((lrcorner.lon - ulcorner.lon))/(theImageSize.samp - 1),
              true);

      kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimEquDistCylProjection");
//   kwl.add(prefix, ossimKeywordNames::TYPE_KW, TYPE_NAME(this));

      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::saveState: returning..." << std::endl;
      return true;
}

//*****************************************************************************
//  METHOD: ossimNitfMapModel::loadState()
//  
//  Restores the model's state from the KWL. This KWL also serves as a
//  geometry file.
//  
//*****************************************************************************
bool ossimNitfMapModel::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix) 
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::loadState: entering..." << std::endl;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::loadState:"
                                          << "\nInput kwl:  " << kwl
                                          << std::endl;
   }

   const char* value = 0;
   const char* keyword = 0;
   bool success = false;

   //***
   // Assure this keywordlist contains correct type info:
   //***
   value = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (!value || (strcmp(value, TYPE_NAME(this))))
      goto loadStateError;
       
   //***
   // Pass on to the base-class for parsing first:
   //***
   success = ossimSensorModel::loadState(kwl, prefix);
   if (!success) goto loadStateError;
 
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::loadState: returning..." << std::endl;
   return true;
 
   //***
   // Local error handling:
   //***
 loadStateError:
   theErrorStatus++;
   ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimNitfMapModel::loadState: encountered parsing the following required keyword: "
                                       << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                       << std::endl;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::loadState: returning with error..." << std::endl;
   return false;   
}

//*****************************************************************************
// STATIC METHOD: ossimNitfMapModel::writeGeomTemplate
//  
//  Writes a sample kwl to output stream.
//  
//*****************************************************************************
void ossimNitfMapModel::writeGeomTemplate(ostream& os)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::writeGeomTemplate: entering..." << std::endl;

   os <<
      "//**************************************************************\n"
      "// Template for LandSat model keywordlist\n"
      "//**************************************************************\n"
      << ossimKeywordNames::TYPE_KW << ": " << "ossimNitfMapModel" << endl;

   ossimSensorModel::writeGeomTemplate(os);
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNitfMapModel::writeGeomTemplate: returning..." << std::endl;
   return;
}

