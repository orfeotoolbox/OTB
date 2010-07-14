//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimGcsCodeProjectionFactory.cpp 709 2010-04-01 03:02:03Z oscar.kramer $

#include <fstream>
#include <sstream>

#include <ossim/projection/ossimGcsCodeProjectionFactory.h>

#include <ossim/base/ossimDatumFactory.h>
#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <algorithm>

ossimGcsCodeProjectionFactory* ossimGcsCodeProjectionFactory::theInstance = 0;

static const ossimString KEYS[] = { "COORD_REF_SYS_CODE","COORD_REF_SYS_NAME","DATUM_CODE","DATUM_NAME","GREENWICH_DATUM","UOM_CODE","ELLIPSOID_CODE","PRIME_MERIDIAN_CODE","SHOW_CRS","DEPRECATED","COORD_SYS_CODE","COORD_OP_METHOD_CODE","DX","DY","DZ","RX","RY","RZ","DS" };

//---
// Hidden from use constructor.
//---
ossimGcsCodeProjectionFactory::ossimGcsCodeProjectionFactory()
   : ossimProjectionFactoryBase(),
     theCsvFiles()
{
   // Get the csv file(s) with the key "state_plane_csv_file":
   ossimString regExpression =  ossimString("ossim_gcs");
   
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
               if (isValidCsvFile(f))
               {
                  theCsvFiles.push_back(f);
               }
            }
         }
      }
      ++i;
   }
}

ossimGcsCodeProjectionFactory::~ossimGcsCodeProjectionFactory()
{
   theCsvFiles.clear();
   
   theInstance = 0;
}

ossimGcsCodeProjectionFactory* ossimGcsCodeProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimGcsCodeProjectionFactory();
   }
   
   return theInstance;
}

ossimProjection* ossimGcsCodeProjectionFactory::createProjection(
   const ossimFilename& filename, ossim_uint32 entryIdx)const
{
   // See if there is an external geomtry.
   return createProjectionFromGeometryFile(filename, entryIdx);
}


ossimProjection* ossimGcsCodeProjectionFactory::createProjection(const ossimString& name) const
{
   ossimProjection* result = 0;

   ossim_int32 code = name.toInt32();
  
   switch (code)
   {
      case 4322:
      {
         ossimEquDistCylProjection* proj =
            new ossimEquDistCylProjection(*(ossimDatumFactory::instance()->wgs72()->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->wgs72());
         proj->setGcsCode(code);
         result = proj;
         break;
      }
      case 4326:
      {
         ossimEquDistCylProjection* proj =
            new ossimEquDistCylProjection(*(ossimDatumFactory::instance()->wgs84()->ellipsoid()));
         proj->setDatum(ossimDatumFactory::instance()->wgs84());
         proj->setGcsCode(code);
         result = proj;
         break;
      }
   }

   return result;
}

ossimProjection* ossimGcsCodeProjectionFactory::createProjection(
   const ossimKeywordlist& kwl, const char* prefix) const
{
   ossimProjection* result = 0;

#if 0 /* TODO */
   // Look for a gcs code first.
   const char* lookup = kwl.find(prefix, ossimKeywordNames::GCS_CODE_KW);
   if(lookup)
   {
      ossim_uint32 code = ossimString(lookup).toUInt32();
   }

   // Pcs lookup failed look for a type name.
   lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      ossimString name = lookup;
   }
#endif

   const char *lookup = kwl.find(prefix, ossimKeywordNames::GCS_CODE_KW);
   if(lookup)
   {
     result = createProjection(ossimString(lookup).trim());
     if(result)
     {
       result->loadState(kwl, prefix);
     }
   }
   return result;
}

ossimObject* ossimGcsCodeProjectionFactory::createObject(
   const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimGcsCodeProjectionFactory::createObject(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimGcsCodeProjectionFactory::getTypeNameList(
   std::vector<ossimString>& /* typeList */)const
{
}

bool ossimGcsCodeProjectionFactory::findLine(const ossimString& name, 
                                             ossim_uint16& gcsCode) const
{
   OpenThreads::ScopedReadLock lock(theMutex);
   std::string savedLine;
   // bool bSavedLine = false;
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
      std::vector<ossimString> result;

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
         if (result.size() != OSSIM_KEYS_SIZE)
         {
            continue; // next line
         }

         if (result[OSSIM_DATUM_NAME] == name)
         {
            gcsCode = result[OSSIM_COORD_REF_SYS_CODE].toInt16();
            return true;
         }
         else if (result[OSSIM_COORD_REF_SYS_NAME] == name)
         {
           gcsCode = result[OSSIM_COORD_REF_SYS_CODE].toInt16();
           return true;
         }
      }
      ++i; // go to next csv file
   }

   return false;
}

bool ossimGcsCodeProjectionFactory::isFileInList(
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

bool ossimGcsCodeProjectionFactory::isValidCsvFile(
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
   if (v.size() != OSSIM_KEYS_SIZE)
   {
      return false;
   }

   for (ossim_uint32 i = 0; i < OSSIM_KEYS_SIZE; ++i)
   {
      if (v[i] != KEYS[i])
      {
         return false;
      }
   }
   return true;
}

void ossimGcsCodeProjectionFactory::splitLine(
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
ossimGcsCodeProjectionFactory::ossimGcsCodeProjectionFactory(
   const ossimGcsCodeProjectionFactory&)
   : ossimProjectionFactoryBase()
{
}

// Hidden from use.
const ossimGcsCodeProjectionFactory&
ossimGcsCodeProjectionFactory::operator=(
   const ossimGcsCodeProjectionFactory& /*rhs*/)
{
   return *this;
}

ossim_uint16 ossimGcsCodeProjectionFactory::getGcsCodeFromProjection(const ossimMapProjection* proj) const
{
   ossim_uint16 gcsCode = 4326; // default datumCode=="WGE"

   if (!proj)
   {
      return 0;
   }

   ossimEquDistCylProjection* equDistCyl = PTR_CAST(ossimEquDistCylProjection, proj);
   if (equDistCyl)
   {
      ossimString datumCode = proj->getDatum()->code();
      ossimString datumName = proj->getDatum()->name();

      if ( datumCode != "WGE" )
      {
         if ( findLine( datumName, gcsCode ) == false )
         {
            gcsCode = 0;
         }
      }
   }

   return gcsCode;
}

ossim_uint16 ossimGcsCodeProjectionFactory::getGcsCodeFromProjectionName(const ossimString projName) const
{
  ossim_uint16 gcsCode = 0;

  if (findLine(projName, gcsCode))
  {
    return gcsCode;
  }

  return gcsCode;
}
