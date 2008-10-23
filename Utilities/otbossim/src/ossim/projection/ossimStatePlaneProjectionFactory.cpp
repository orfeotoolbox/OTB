//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionFactory.cpp 12623 2008-04-07 14:10:08Z gpotts $

#include <fstream>
#include <sstream>

#include <ossim/projection/ossimStatePlaneProjectionFactory.h>

#include <ossim/base/ossimDatumFactory.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/projection/ossimProjection.h>
#include <algorithm>

ossimStatePlaneProjectionFactory* ossimStatePlaneProjectionFactory::theInstance = 0;

static const ossimString KEYS[] = { "name", "pcs_code", "projection_code", "param1", "param2", "param3", "param4", "false_easting", "false_northing", "linear_units", "datum_code" };

//---
// Hidden from use constructor.
//---
ossimStatePlaneProjectionFactory::ossimStatePlaneProjectionFactory()
   : ossimProjectionFactoryBase(),
     theCsvFiles(),
     theInfos()
{
   // Get the csv file(s) with the key "state_plane_csv_file":
   ossimString regExpression =  ossimString("^state_plane_csv_file[0-9]+");
   
   std::vector<ossimString> keys = ossimPreferences::instance()->
      preferencesKWL().getSubstringKeyList( regExpression );

   std::vector<ossimString>::const_iterator i = keys.begin();
   while ( i != keys.end() )
   {
      const char* lookup = ossimPreferences::instance()->
         preferencesKWL().find( (*i).c_str() );

      if (lookup)
      {
         ossimFilename f = lookup;
         if (f.exists())
         {
            if (isFileInList(f) == false) // Check for duplicate.
            {
               if ( isValidCsvFile(f) )
               {
                  theCsvFiles.push_back(f);
               }
            }
         }
      }
      ++i;
   }
}

ossimStatePlaneProjectionFactory::~ossimStatePlaneProjectionFactory()
{
   theCsvFiles.clear();

   std::vector<ossimStatePlaneProjectionInfo*>::iterator i = theInfos.begin();
   while ( i != theInfos.end() )
   {
      delete (*i);
      (*i) = 0;
      ++i;
   }
   
   theInstance = 0;
}

ossimStatePlaneProjectionFactory* ossimStatePlaneProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimStatePlaneProjectionFactory();
   }

   return theInstance;
}

ossimProjection* ossimStatePlaneProjectionFactory::createProjection(
   const ossimFilename& filename, ossim_uint32 entryIdx)const
{
   // See if there is an external geomtry.
   return createProjectionFromGeometryFile(filename, entryIdx);
}


ossimProjection* ossimStatePlaneProjectionFactory::createProjection(
   const ossimString& name) const
{
   ossimProjection* result = 0;
   ossimStatePlaneProjectionInfo* info = findInfo(name);
   if (info)
   {
      ossimKeywordlist kwl;
      info->populateProjectionKeywords(kwl);

      result = ossimMapProjectionFactory::instance()->createProjection(kwl);
   }
   
   return result;
}

ossimProjection* ossimStatePlaneProjectionFactory::createProjection(
   const ossimKeywordlist& kwl,
   const char* prefix)const
{
   ossimStatePlaneProjectionInfo* info   = 0;
   ossimProjection*               result = 0;

   // Look for a pcs code first.
   const char* lookup = kwl.find(prefix, ossimKeywordNames::PCS_CODE_KW);
   if(lookup)
   {
      ossim_uint32 code = ossimString(lookup).toUInt32();
      info = findInfo(code);
   }

   if (!info)
   {
      // Pcs lookup failed look for a type name.
      lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
      if (lookup)
      {
         ossimString name = lookup;
         info = findInfo(name);
      }
   }

   if (info)
   {
      // Create a new keyword list using the passed in kwl and the info object.
      ossimKeywordlist kwl2 = kwl;
      info->populateProjectionKeywords(kwl2, prefix);

      // Create projection from the merged keyword list.
      result = ossimMapProjectionFactory::instance()->createProjection(
         kwl2, prefix);
   }
   
   return result;
}

const ossimStatePlaneProjectionInfo* ossimStatePlaneProjectionFactory::getInfo(ossim_int32 pcsCode)const
{
   ossimStatePlaneProjectionInfo* info = findInInfoList(pcsCode);
   if (!info)
   {
      info = findInCsvFile(pcsCode);
   }
   return info;
}

const ossimStatePlaneProjectionInfo* ossimStatePlaneProjectionFactory::getInfo(const ossimString& name)const
{
   ossimStatePlaneProjectionInfo* info = 0;
   if (name.size())
   {
      info = findInInfoList(name);
      if (!info)
      {
         info = findInCsvFile(name);
      }
   }
   return info;
}

ossimObject* ossimStatePlaneProjectionFactory::createObject(
   const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimStatePlaneProjectionFactory::createObject(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   return createProjection(kwl, prefix);
}
   
void ossimStatePlaneProjectionFactory::getTypeNameList(
   std::vector<ossimString>& typeList)const
{
	OpenThreads::ScopedReadLock lock(theMutex);
   // Iterate throught the cvs files to try and get the names.
   std::vector<ossimFilename>::const_iterator i = theCsvFiles.begin();
   while (i != theCsvFiles.end())
   {
      // open file
      std::ifstream in( (*i).c_str() );
      if (!in)
      {
         ++i;
         continue; // Go to next iteration(file) if there is one.
      }

      // used throughout
      std::string line;
      std::string s;
      
      // Eat the first line.
      std::getline(in, line);

      // Iterate through the lines of file.
      while(in.good())
      {
         // Read in a line.
         std::getline(in, line);

         // Split the line between commas stripping quotes.
         std::vector<ossimString> v;
         splitLine(line, v);

         // Check the size
         if (v.size() != KEYS_SIZE)
         {
            continue; // next line
         }

         typeList.push_back(v[NAME_INDEX]);
      }
      ++i; // go to next csv file
      
   } // End "while (i != theCsvFiles.end())"
}

void ossimStatePlaneProjectionFactory::addCsvFile(const ossimFilename csvFile)
{
	OpenThreads::ScopedWriteLock lock(theMutex);
   std::vector<ossimFilename>::iterator iter = std::find(theCsvFiles.begin(),
                                                theCsvFiles.end(),
                                                csvFile);
   if(iter == theCsvFiles.end())
   {
      theCsvFiles.push_back(csvFile);
   }
}

ossimStatePlaneProjectionInfo* ossimStatePlaneProjectionFactory::findInfo(
   ossim_uint32 pcsCode) const
{
   ossimStatePlaneProjectionInfo* info = 0;
   
   // See if it's in the list.
   info = findInInfoList(pcsCode);
   if (!info)
   {
      // See if in a cvs file.
      info = findInCsvFile(pcsCode);
   }

   return info;
}

ossimStatePlaneProjectionInfo* ossimStatePlaneProjectionFactory::findInfo(
   const ossimString& name) const
{
   ossimStatePlaneProjectionInfo* info = 0;
   
   if (name.empty())
   {
      return info;
   }
   
   // See if it's in the list.
   info = findInInfoList(name);
   if (!info)
   {
      // See if in a cvs file.
      info = findInCsvFile(name);
   }

   return info;
}

ossimStatePlaneProjectionInfo*
ossimStatePlaneProjectionFactory::findInInfoList(ossim_uint32 pcsCode)const
{
	OpenThreads::ScopedReadLock lock(theMutex);
   // See if it's in the list.
   std::vector<ossimStatePlaneProjectionInfo*>::const_iterator i =
      theInfos.begin();

   int code = static_cast<int>(pcsCode); // Stored in info object as int.
   
   while ( i != theInfos.end() )
   {
      if ( code == (*i)->code())
      {
         return (*i);
      }
      ++i;
   }
   return 0;
}

ossimStatePlaneProjectionInfo*
ossimStatePlaneProjectionFactory::findInInfoList(const ossimString& name)const
{
	OpenThreads::ScopedReadLock lock(theMutex);
  if (name.size())
   {
      // See if it's in the list.
      std::vector<ossimStatePlaneProjectionInfo*>::const_iterator i =
         theInfos.begin();
      while ( i != theInfos.end() )
      {
         if (name == (*i)->name())
         {
            return (*i);
         }
         ++i;
      }
   }
   return 0;
}

ossimStatePlaneProjectionInfo*
ossimStatePlaneProjectionFactory::findInCsvFile(ossim_uint32 pcsCode) const
{
   ossimStatePlaneProjectionInfo* result = 0;
   std::vector<ossimString> lineArray;
   if (findLine(pcsCode, lineArray))
   {
      result = createInfo(lineArray);
      if (result)
      {
			OpenThreads::ScopedWriteLock lock(theMutex);
         // Store for later use.
         theInfos.push_back(result);
      }
   }
   return result;
}
   
ossimStatePlaneProjectionInfo*
ossimStatePlaneProjectionFactory::findInCsvFile(const ossimString& name) const
{
   ossimStatePlaneProjectionInfo* result = 0;
   std::vector<ossimString> lineArray;
   if (findLine(name, lineArray))
   {
      result = createInfo(lineArray);
      if (result)
      {
			OpenThreads::ScopedWriteLock lock(theMutex);
         // Store for later use.
         theInfos.push_back(result);
      }
   }
   return result;
}

bool ossimStatePlaneProjectionFactory::findLine(
   ossim_uint32 pcsCode, std::vector<ossimString> &result) const
{
	OpenThreads::ScopedReadLock lock(theMutex);

   // Iterate throught the cvs files to try and find pcs code.
   std::vector<ossimFilename>::const_iterator i = theCsvFiles.begin();
   while (i != theCsvFiles.end())
   {
      // open file
      std::ifstream in( (*i).c_str() );
      if (!in)
      {
         continue; // Go to next iteration(file) if there is one.
      }

      // used throughout
      std::string line;
      
      // Eat the first line.
      std::getline(in, line);

      // Iterate through the lines of file.
      while(in.good())
      {
         // Read in a line.
         std::getline(in, line);

         // Split the line between commas stripping quotes.
         splitLine(line, result);

         // Check the size
         if (result.size() != KEYS_SIZE)
         {
            continue; // next line
         }

         if (result[PCS_CODE_INDEX].toUInt32() == pcsCode)
         {
            return true;
         }
      }
      ++i; // go to next csv file
   }
   return false;
}

bool ossimStatePlaneProjectionFactory::findLine(
   const ossimString& name, std::vector<ossimString> &result) const
{
	OpenThreads::ScopedReadLock lock(theMutex);
   // Iterate throught the cvs files to try and find pcs code.
   std::vector<ossimFilename>::const_iterator i = theCsvFiles.begin();
   while (i != theCsvFiles.end())
   {
      // open file
      std::ifstream in( (*i).c_str() );
      if (!in)
      {
         continue; // Go to next iteration(file) if there is one.
      }

      // used throughout
      std::string line;
      
      // Eat the first line.
      std::getline(in, line);

      // Iterate through the lines of file.
      while(in.good())
      {
         // Read in a line.
         std::getline(in, line);

         // Split the line between commas stripping quotes.
         splitLine(line, result);

         // Check the size
         if (result.size() != KEYS_SIZE)
         {
            continue; // next line
         }

         if (result[NAME_INDEX] == name)
         {
            return true;
         }
      }
      ++i; // go to next csv file
   }
   return false;
}

ossimStatePlaneProjectionInfo* ossimStatePlaneProjectionFactory::createInfo(
      const std::vector<ossimString> &lineArray) const
{
   ossimStatePlaneProjectionInfo* result = 0;
   
   if (lineArray.size() != KEYS_SIZE)
   {
      return result;
   }

   // Get the datum from the code.
   const ossimDatum* datum =
      ossimDatumFactory::instance()->create(lineArray[DATUM_CODE_INDEX]);
   if (!datum)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimStatePlaneProjectionFactory::createInfo"
         << "\nUnsupported datum code: " << lineArray[DATUM_CODE_INDEX]
         << std::endl;
      return result;
   }

   result = new ossimStatePlaneProjectionInfo(
      lineArray[NAME_INDEX],
      lineArray[PCS_CODE_INDEX].toInt32(),
      lineArray[PROJECTION_CODE_INDEX],
      lineArray[PARAM1_INDEX],
      lineArray[PARAM2_INDEX],
      lineArray[PARAM3_INDEX],
      lineArray[PARAM4_INDEX],
      lineArray[FALSE_EASTING_INDEX].toDouble(),
      lineArray[FALSE_NORTHING_INDEX].toDouble(),
      lineArray[LINEAR_UNITS_INDEX],
      datum);

   return result;
}

bool ossimStatePlaneProjectionFactory::isFileInList(
   const ossimFilename& file) const
{
	OpenThreads::ScopedReadLock lock(theMutex);
   std::vector<ossimFilename>::const_iterator i = theCsvFiles.begin();
   while (i != theCsvFiles.end())
   {
      if (file == (*i))
      {
         return true;
      }
      ++i;
   }
   return false;
}

bool ossimStatePlaneProjectionFactory::isValidCsvFile(
   const ossimFilename& file) const
{
   // open file
   std::ifstream in( file.c_str() );
   if (!in)
   {
      return false;
   }

   // Grab the first line.
   std::string line;
   std::getline(in, line);
   
   if (!in)
   {
      return false;
   }

   // Split the line between commas stripping quotes.
   std::vector<ossimString> v;
   splitLine(line, v);

   // Check the size
   if (v.size() != KEYS_SIZE)
   {
      return false;
   }

   for (ossim_uint32 i = 0; i < KEYS_SIZE; ++i)
   {
      if (v[i] != KEYS[i])
      {
         return false;
      }
   }
   return true;
}

void ossimStatePlaneProjectionFactory::splitLine(
   const std::string& line, std::vector<ossimString> &result) const
{
   //---
   // Splits line between commas, stripping quotes and making empty string
   // on double comma.
   //
   // Sample line:  ",,foo,you,,,\"too\""
   //---
   
   if (result.size())
   {
      result.clear();
   }

   if (!line.size())
   {
      return;
   }

   // Constants used.
   const char DELIM = ',';
   const std::streamsize SIZE = 64;

   // Buffer for reads.
   char s[SIZE];
   s[SIZE-1] = '\0';

   // tmp
   ossimString os;

   // Open a stream to the line.
   std::istringstream istr(line);
   while (istr.good())
   {
      // We can have lines with empty fields like foo,,,you so peek for it.
      if (istr.peek() != ',')
      {
         istr.get(s, SIZE, DELIM);
         os = s;

         if (istr.peek() == ',')
         {
            istr.ignore();  // Gobble the comma.
         }

         // Get rid of any trailing quote.
         std::string::size_type pos = os.find_last_of('"');
         if (pos != std::string::npos)
         {
            os.erase(pos, 1);
         }

         // Get rid of leading quote if any.
         pos = os.find_first_of('"');
         if (pos != std::string::npos)
         {
            os.erase(pos, 1);
         }

         // Trim leading trailing blanks.
         os.trim();
      }
      else
      {
         // Gobble the comma.
         istr.ignore(); 
         
         // Make an empty string on double comma.
         os = "";
      }

      result.push_back(os);
   }
}

// Hidden from use.
ossimStatePlaneProjectionFactory::ossimStatePlaneProjectionFactory(
   const ossimStatePlaneProjectionFactory&)
   : ossimProjectionFactoryBase()
{
}

// Hidden from use.
const ossimStatePlaneProjectionFactory&
ossimStatePlaneProjectionFactory::operator=(
   const ossimStatePlaneProjectionFactory& /*rhs*/)
{
   return *this;
}
