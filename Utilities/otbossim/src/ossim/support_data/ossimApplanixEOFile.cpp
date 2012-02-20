//*******************************************************************
//
// LGPL
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixEOFile.cpp 20483 2012-01-21 15:42:22Z dburken $

#include <ossim/support_data/ossimApplanixEOFile.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimCommon.h>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

static ossimTrace traceDebug("ossimApplanixEOFile:debug");

static std::istream& applanix_skipws(std::istream& in)
{
   int c = in.peek();
   while(((c == ' ') ||
          (c == '\r') ||
          (c == '\t') ||
          (c == '\n'))&&
         (in.good()))
   {
      in.ignore(1);
      c = in.peek();
   }
   
   return in;
}

std::ostream& operator <<(std::ostream& out, const ossimApplanixEORecord& src)
{
   if(src.theField.size())
   {
      std::copy(src.theField.begin(),
                src.theField.end(),
                std::ostream_iterator<ossimString>(out, " "));
   }

   return out;
}

std::ostream& operator <<(std::ostream& out, const ossimApplanixEOFile& src)
{
   ossim_uint32 idx = 0;
   out << src.theHeader << std::endl << std::endl;

   if(src.theRecordFormat.size())
   {
      std::copy(src.theRecordFormat.begin(),
                src.theRecordFormat.end()-1,
                std::ostream_iterator<ossimString>(out, ", "));
      out << *(src.theRecordFormat.end()-1);
      out << std::endl;
   }

   out << "Kappa Cardinal Rotation = " << src.theKappaCardinal << std::endl;
   out << "Bore Sight tx = " << src.theBoreSightTx << std::endl
       << "Bore Sight ty = " << src.theBoreSightTy << std::endl
       << "Bore Sight tz = " << src.theBoreSightTz << std::endl;
   
   out << "Lever arm lx = " << src.theLeverArmLx << std::endl
       << "Lever arm ly = " << src.theLeverArmLy << std::endl
       << "Lever arm lz = " << src.theLeverArmLz << std::endl;
   
   out << "Shift Value X = " << src.theShiftValuesX << std::endl
       << "Shift Value Y = " << src.theShiftValuesY << std::endl
       << "Shift Value Z = " << src.theShiftValuesZ << std::endl;
   
   out << "Mapping Frame Datum      = " << src.theMappingFrameDatum      << std::endl
       << "Mapping Frame Projection = " << src.theMappingFrameProjection << std::endl
       << "Central Meridian         = " << src.theCentralMeridian << std::endl
       << "Origin of Latitude       = " << src.theOriginLatitude  << std::endl
       << "Grid Scale Factor        = " << src.theGridScaleFactor << std::endl
       << "False easting            = " << src.theFalseEasting    << std::endl
       << "False northing           = " << src.theFalseNorthing   << std::endl;

   out << "theUtmZone               = " << src.theUtmZone << std::endl;
   out << "theUtmHemisphere         = " << src.theUtmHemisphere << std::endl;
   
   if(src.theApplanixRecordList.size())
   {
      out << std::endl;
      for(idx = 0; idx < src.theApplanixRecordList.size()-1; ++idx)
      {
         out << *(src.theApplanixRecordList[idx]) << std::endl;
      }
      out << *(src.theApplanixRecordList[idx]);
      
   }

   return out;
}

ossimApplanixEOFile::ossimApplanixEOFile()
{
   theMinLat = ossim::nan();
   theMinLon = ossim::nan();
   theMaxLat = ossim::nan();
   theMaxLon = ossim::nan();
}

bool ossimApplanixEOFile::parseFile(const ossimFilename& file)
{
   std::ifstream in(file.c_str());

   return parseStream(in);
}

bool ossimApplanixEOFile::isEOFile(const ossimFilename& file)const
{
   std::ifstream in(file.c_str());

   return isEOFile(in);
}

bool ossimApplanixEOFile::isEOFile(std::istream& in)const
{
   ossimString header;

   return parseHeader(in, header);
}

bool ossimApplanixEOFile::parseStream(std::istream& in)
{
   theRecordIdMap.clear();
   ossimString line;
   int c = '\0';
   if(!parseHeader(in, theHeader))
   {
      return false;
   }

   // now parse parameters
   in>>applanix_skipws;
   line = "";
   while(in.good()&&
         !line.contains("RECORD FORMAT"))
   {
      std::getline(in, line.string());
      line = line.upcase();
      line = line.substitute("\r","\n", true);
      if(line.contains("KAPPA CARDINAL"))
      {
         theKappaCardinal = line;
         theKappaCardinal = theKappaCardinal.substitute("KAPPA CARDINAL ROTATION","");
         theKappaCardinal = theKappaCardinal.substitute(":","");
         theKappaCardinal = theKappaCardinal.substitute("\n","");
      }
      else if(line.contains("LEVER ARM"))
      {
         ossimKeywordlist kwl('=');
         line = line.substitute("LEVER ARM VALUES:",
                                "");
         line = line.substitute(",",
                                "\n",
                                true);
         std::istringstream in(line);

         kwl.parseStream(in);

         theLeverArmLx = kwl.find("LX");
         theLeverArmLy = kwl.find("LY");
         theLeverArmLz = kwl.find("LZ");
      }
      else if(line.contains("BORESIGHT VALUES"))
      {
         ossimKeywordlist kwl('=');
         line = line.substitute("BORESIGHT VALUES:",
                                "");
         line = line.substitute(",",
                                "\n",
                                true);

         std::istringstream in(line);

         kwl.parseStream(in);

         
         theBoreSightTx = kwl.find("TX");
         theBoreSightTy = kwl.find("TY");
         theBoreSightTz = kwl.find("TZ");
      }
      else if(line.contains("SHIFT VALUES:"))
      {
         ossimKeywordlist kwl('=');
         line = line.substitute("SHIFT VALUES:","");
         line = line.substitute(",",
                                "\n",
                                true);

         std::istringstream in(line);
         kwl.parseStream(in);

         
         theShiftValuesX = kwl.find("X");
         theShiftValuesY = kwl.find("Y");
         theShiftValuesZ = kwl.find("Z");
         
      }
      else if(line.contains("GRID:"))
      { 
         ossimKeywordlist kwl(':');
         line = line.substitute(";",
                                "\n",
                                true);
         std::istringstream in(line);
         kwl.parseStream(in);
         theUtmZone = kwl.find("ZONE");

         if(theUtmZone.contains("NORTH"))
         {
            theUtmHemisphere = "North";
         }
         else
         {
            theUtmHemisphere = "South";
         }
          theUtmZone = theUtmZone.replaceAllThatMatch("UTM|\\(.*\\)|NORTH|SOUTH","");
          theUtmZone = theUtmZone.trim();
      }
      else if(line.contains("FRAME DATUM"))
      {
         ossimKeywordlist kwl(':');
         line = line.substitute(";",
                                "\n",
                                true);
         std::istringstream in(line);
         kwl.parseStream(in);

         theMappingFrameDatum = kwl.find("MAPPING FRAME DATUM");
         theMappingFrameProjection = kwl.find("MAPPING FRAME PROJECTION");
         theMappingFrameDatum = theMappingFrameDatum.trim();
         theMappingFrameProjection = theMappingFrameProjection.trim();
      }
      else if(line.contains("POSPROC SBET"))
      {
         theSbetField = line.after(":");
         theSbetField = theSbetField.trim();
      }
      else if(line.contains("CENTRAL MERIDIAN"))
      {
         theCentralMeridian = line;
         theCentralMeridian = theCentralMeridian.substitute("CENTRAL MERIDIAN","");
         theCentralMeridian = theCentralMeridian.substitute("=","");
         theCentralMeridian = theCentralMeridian.substitute("DEG","");
         theCentralMeridian = theCentralMeridian.substitute(";","");
      }
      else if(line.contains("LATITUDE OF THE GRID ORIGIN"))
      {
         ossimKeywordlist kwl('=');
         line = line.substitute(";",
                                "\n",
                                true);
         std::istringstream in(line);
         kwl.parseStream(in);

         theOriginLatitude  = kwl.find("LATITUDE OF THE GRID ORIGIN");
         theGridScaleFactor = kwl.find("GRID SCALE FACTOR");
      }
      else if(line.contains("FALSE EASTING"))
      {
         ossimKeywordlist kwl('=');
         line = line.substitute(";",
                                "\n",
                                true);
         std::istringstream in(line);
         kwl.parseStream(in);

         theFalseEasting  = kwl.find("FALSE EASTING");
         theFalseNorthing = kwl.find("FALSE NORTHING");
      }
   }

   in>>applanix_skipws;

   c = in.get();

   std::vector<ossimString> fieldArray;
   ossimString field;

   while(in.good()&&
         (c!='\n')&&
         (c!='\r'))
   {
      field = "";
      while((c != ',')&&
            (c != '\n')&&
            (c != '\r'))
      {
         field += (char)c;
         c = in.get();
      }
      if((c!='\n')&&
         (c!='\r'))
      {
         c = in.get();
      }
      field = field.trim();
      if(field != "")
      {
         theRecordFormat.push_back(field);
      }
   }
   in>>applanix_skipws;

   if(in.peek() == '(')
   {
      std::getline(in, line.string());
   }
   in>>applanix_skipws;
   ossimRefPtr<ossimApplanixEORecord> record = new ossimApplanixEORecord((ossim_uint32)theRecordFormat.size());
   ossim_int32 latIdx = getFieldIdx("LAT");
   ossim_int32 lonIdx = getFieldIdx("LONG");;
   bool hasLatLon = (latIdx >=0)&&(lonIdx >= 0);

   
   if(hasLatLon)
   {
      theMinLat = 90.0;
      theMaxLat = -90.0;
      theMinLon = 180.0;
      theMaxLon = -180.0;
   }
   else
   {
      theMinLat = ossim::nan();
      theMaxLat = ossim::nan();
      theMinLon = ossim::nan();
      theMaxLon = ossim::nan();
   }
   
   while(in.good()&&theRecordFormat.size())
   {
      std::getline(in, line.string());
      line = line.trim();
      if(line != "")
      {
         std::istringstream inStr(line);
         ossim_uint32 idx;
         ossimString value;

         for(idx = 0; idx < theRecordFormat.size(); ++idx)
         {
            inStr >> (*record)[idx];
         }
         if(hasLatLon)
         {
            double lat = (*record)[latIdx].toDouble();
            double lon = (*record)[lonIdx].toDouble();

            if(lat<theMinLat) theMinLat = lat;
            if(lat>theMaxLat) theMaxLat = lat;
            if(lon<theMinLon) theMinLon = lon;
            if(lon>theMaxLon) theMaxLon = lon;
            
         }
         theApplanixRecordList.push_back(new ossimApplanixEORecord(*record));
      }
   }
   indexRecordIds();

   if(traceDebug())
   {
      if(hasLatLon)
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "min lat: " << theMinLat << std::endl
                                             << "min lon: " << theMinLon << std::endl
                                             << "max lat: " << theMaxLat << std::endl
                                             << "max lon: " << theMaxLon << std::endl;
      }
   }
   return true;
}

bool ossimApplanixEOFile::parseHeader(const ossimFilename& file,
                                      ossimString& header)const
{
   std::ifstream in(file.c_str());

   return parseHeader(in, header);
}

bool ossimApplanixEOFile::parseHeader(std::istream& in,
                                      ossimString& header)const
{
   header = "";
   in >> applanix_skipws;

   int c = in.peek();
   
   if((c!='*')||
      (!in.good()))
   {
      return false;
   }
   ossimString line;

   std::getline(in, line.string());
   line = line .substitute("\r",
                           "\n");
   if(!line.contains("************"))
   {
      return false;
   }

   header = line;
   header += "\n";

   in>>applanix_skipws;
   std::getline(in, line.string());
   line = line .substitute("\r",
                           "\n");
   
   while((in.good())&&
         (!line.contains("***********")))
   {
      header += line;
      header += "\n";
      std::getline(in, line.string());
      line = line .substitute("\r",
                              "\n");
   }
   header += line;
   header += "\n";

   if(!header.contains("Applanix"))
   {
      return false;
   }
   
   return true;
}

void ossimApplanixEOFile::indexRecordIds()
{
   ossimString id;
   ossim_uint32 idx;
   ossim_int32 idIdx = getFieldIdxLike("ID");
   ossim_uint32 size = (ossim_uint32)theApplanixRecordList.size();
   theRecordIdMap.clear();

   if(idIdx < 0) return;

   for(idx = 0; idx < size; ++idx)
   {
      id = (*theApplanixRecordList[idx])[idIdx];
      theRecordIdMap.insert(std::make_pair(id, theApplanixRecordList[idx]));
   }
}

ossim_int32 ossimApplanixEOFile::getFieldIdxLike(const ossimString& searchKey)const
{
   ossimString key = searchKey;
   ossim_uint32 idx = 0;
   key = key.upcase();

   for(idx = 0; idx < theRecordFormat.size(); ++idx)
   {
      if(ossimString::upcase(theRecordFormat[idx]).contains(key))
      {
         return (int)idx;
      }
   }

   return -1;
}

ossim_int32 ossimApplanixEOFile::getFieldIdx(const ossimString& searchKey)const
{
   ossimString key = searchKey;
   ossim_uint32 idx = 0;
   
   key = key.upcase();

   for(idx = 0; idx < theRecordFormat.size(); ++idx)
   {
      if(ossimString::upcase(theRecordFormat[idx]) == key)
      {
         return (int)idx;
      }
   }
   
   return -1;
}


const ossimRefPtr<ossimApplanixEORecord> ossimApplanixEOFile::getRecordGivenId(const ossimString& id)const
{

   if(theRecordIdMap.size())
   {
      std::map<ossimString, ossimRefPtr<ossimApplanixEORecord>,
         ossimStringLtstr >::const_iterator iter = theRecordIdMap.find(id);
      
      if(iter!=theRecordIdMap.end())
      {
         return iter->second;
      }
   }
   else
   {
      ossim_uint32 idx;
      ossim_int32 idxId = getFieldIdx("ID");
      
      if(idxId >= 0)
      {
         for(idx = 0; idx < theApplanixRecordList.size(); ++idx)
         {
            if((*theApplanixRecordList[idx])[idxId] == id)
            {
               return theApplanixRecordList[idx];
            }
         }
      }
   }
   
   return 0;
}

ossim_uint32 ossimApplanixEOFile::getNumberOfRecords()const
{
   return (ossim_uint32)theApplanixRecordList.size();
}

const ossimRefPtr<ossimApplanixEORecord> ossimApplanixEOFile::getRecord(ossim_uint32 idx)const
{
   if(idx < theApplanixRecordList.size())
   {
      return theApplanixRecordList[idx];
   }
   
   return 0;
}

bool ossimApplanixEOFile::isHeightAboveMSL()const
{
   return (getFieldIdx("ORTHOMETRIC HEIGHT") >= 0);
}

bool ossimApplanixEOFile::isEcefModel()const
{
   ossim_int32 xIdx = getFieldIdx("X");
   ossim_int32 yIdx = getFieldIdx("Y");
   ossim_int32 zIdx = getFieldIdx("Z");
   ossim_int32 rollIdx = getFieldIdx("ROLL");
   ossim_int32 pitchIdx = getFieldIdx("PITCH");
   ossim_int32 headingIdx = getFieldIdx("HEADING");

   return ((xIdx >=0)&&
           (yIdx >=0)&&
           (zIdx >=0)&&
           (rollIdx>=0)&&
           (pitchIdx>=0)&&
           (headingIdx>=0));
}


bool ossimApplanixEOFile::isUtmFrame()const
{
   return ((getFieldIdxLike("EASTING")>=0)&&
            (getFieldIdxLike("NORTHING") >=0)&&
            (getFieldIdxLike("HEIGHT") >= 0)&&
            (getFieldIdxLike("OMEGA") >=0)&&
            (getFieldIdxLike("PHI") >=0)&&
            (getFieldIdxLike("KAPPA") >=0));
            
   return (getUtmZone() >= 0.0);
}

ossim_int32 ossimApplanixEOFile::getUtmZone()const
{
   return theUtmZone.toInt32();
}

ossimString ossimApplanixEOFile::getUtmHemisphere()const
{
   return theUtmHemisphere;
}

ossimString ossimApplanixEOFile::getSbetField()const
{
   return theSbetField;
}

double ossimApplanixEOFile::getBoreSightTx()const
{
   return theBoreSightTx.toDouble();
}

double ossimApplanixEOFile::getBoreSightTy()const
{
   return theBoreSightTy.toDouble();
}

double ossimApplanixEOFile::getBoreSightTz()const
{
   return theBoreSightTz.toDouble();
}

double ossimApplanixEOFile::getLeverArmLx()const
{
   return theLeverArmLx.toDouble();
}

double ossimApplanixEOFile::getLeverArmLy()const
{
   return theLeverArmLy.toDouble();
}

double ossimApplanixEOFile::getLeverArmLz()const
{
   return theLeverArmLz.toDouble();
}

double ossimApplanixEOFile::getKardinalKappa()const
{
   return theKappaCardinal.toDouble();
}

double ossimApplanixEOFile::getShiftValuesX()const
{
   return theShiftValuesX.toDouble();
}

double ossimApplanixEOFile::getShiftValuesY()const
{
   return theShiftValuesY.toDouble();
}

double ossimApplanixEOFile::getShiftValuesZ()const
{
   return theShiftValuesZ.toDouble();
}

ossimString ossimApplanixEOFile::getMappingFrameDatum()const
{
   return theMappingFrameDatum;
}

ossimString ossimApplanixEOFile::getMappingFrameProjection()const
{
   return theMappingFrameProjection;
}

double ossimApplanixEOFile::getMappingFrameCentralMeridian()const
{
   return theCentralMeridian.toDouble();
}

double ossimApplanixEOFile::getMappingFrameOriginLatitude()const
{
   return theOriginLatitude.toDouble();
}

double ossimApplanixEOFile::getMappingFrameGridScaleFactor()const
{
   return theGridScaleFactor.toDouble();
}

double ossimApplanixEOFile::getMappingFrameFalseEasting()const
{
   return theFalseEasting.toDouble();
}

double ossimApplanixEOFile::getMappingFrameFalseNorthing()const
{
   return theFalseNorthing.toDouble();
}

ossimString ossimApplanixEOFile::convertToOssimDatumCode(const ossimString& datum)const
{
   ossimString result = "WGE";
   ossimString temp = datum;
   temp = temp.upcase();

   if(temp == "NAD83")
   {
      result = "NAR-C";
   }

   return result;
}
