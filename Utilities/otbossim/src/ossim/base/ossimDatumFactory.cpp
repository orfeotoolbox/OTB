//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// This holds the class definition of DatumFactory.
//*******************************************************************
//  $Id: ossimDatumFactory.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimEllipsoidFactory.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimThreeParamDatum.h>
#include <ossim/base/ossimSevenParamDatum.h>
#include <ossim/base/ossimWgs84Datum.h>
#include <ossim/base/ossimWgs72Datum.h>
#include <ossim/base/ossimNadconNarDatum.h>
#include <ossim/base/ossimNadconNasDatum.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimPreferences.h>
#include "ossimDatumFactory.inc"

#include <cstring> /* for strlen */
#include <utility> /* for std::make_pair */

ossimDatumFactory* ossimDatumFactory::theInstance = 0;

static ossimString WGE = "WGE";
static ossimString WGD = "WGD";

ossimDatumFactory::~ossimDatumFactory()
{
   deleteAll();
   theInstance = 0;
}

ossimDatumFactory* ossimDatumFactory::instance()
{
   if(!theInstance) 
   {
      theInstance = new ossimDatumFactory;

      // datum depends on the creation of an ellipsoid factory
      // so make sure an instance exists
      ossimEllipsoidFactory::instance();
      theInstance->initializeDefaults();
      theInstance->theWgs84Datum = theInstance->create(WGE);
      theInstance->theWgs72Datum = theInstance->create(WGD);
   }  
   return theInstance; 
} 

const ossimDatum* ossimDatumFactory::create(const ossimString &code)const
{
   if ( code.size() )
   {
      std::map<std::string, const ossimDatum*>::const_iterator datum = theDatumTable.find(code);
      
      if(datum != theDatumTable.end())
      {
         return (*datum).second;
      }

      if(code == "NAR") // User did not set "datum_grids" so map to NAR-C.
      {
         datum = theDatumTable.find("NAR-C");
         if(datum != theDatumTable.end())
         {
            return (*datum).second;
         }
      }
      else if(code == "NAS") // User did not set "datum_grids" so map to NAS-C."
      {
         datum = theDatumTable.find("NAS-C");
         if(datum != theDatumTable.end())
         {
            return (*datum).second;
         }
      }
   }
   
   return 0;
}

const ossimDatum* ossimDatumFactory::create(const ossimKeywordlist& kwl, const char *prefix) const
{
   ossimString alpha_code = kwl.find(prefix, ossimKeywordNames::DATUM_KW);
   if(!alpha_code.empty())
      return create(alpha_code);
   return 0;
}

const ossimDatum* ossimDatumFactory::create(const ossimDatum* aDatum)const
{
   if (aDatum)
      return create (aDatum->code());
   else
      return 0;
}

std::vector<ossimString> ossimDatumFactory::getList()const
{
   std::map<std::string, const ossimDatum*>::const_iterator datum = theDatumTable.begin();
   std::vector<ossimString> result;

   while(datum != theDatumTable.end())
   {
      result.push_back(ossimString((*datum).first) );
      ++datum;
   }
   return result;
}

void ossimDatumFactory::getList(std::vector<ossimString>& list) const
{
   std::map<std::string, const ossimDatum*>::const_iterator datum =
      theDatumTable.begin();

   while(datum != theDatumTable.end())
   {
      list.push_back( ossimString((*datum).first) );
      ++datum;
   }
}

void ossimDatumFactory::deleteAll()
{   
   std::map<std::string, const ossimDatum*>::iterator datum;

   datum = theDatumTable.begin();
   while(datum != theDatumTable.end())
   {
      delete ((*datum).second);
      ++datum;
   }

   theDatumTable.clear();

}

void ossimDatumFactory::initializeDefaults()
{
   //make the standards
   theDatumTable.insert(std::make_pair(WGE.string(), new ossimWgs84Datum));
   theDatumTable.insert(std::make_pair(WGD.string(), new ossimWgs72Datum));

   ossim_uint32 idx = 0;     
   while( std::strlen(threeParamDatum[idx].theCode) )
   {
      if( (threeParamDatum[idx].theCode != WGE) &&
          (threeParamDatum[idx].theCode != WGD) )
      {
         theDatumTable.insert(std::make_pair(std::string(threeParamDatum[idx].theCode), 
            new ossimThreeParamDatum(threeParamDatum[idx].theCode, 
            threeParamDatum[idx].theName,
            ossimEllipsoidFactory::instance()->create(ossimString(threeParamDatum[idx].theEllipsoidCode)),
            threeParamDatum[idx].theSigmaX, 
            threeParamDatum[idx].theSigmaY, 
            threeParamDatum[idx].theSigmaZ, 
            threeParamDatum[idx].theWestLongitude, 
            threeParamDatum[idx].theEastLongitude, 
            threeParamDatum[idx].theSouthLatitude, 
            threeParamDatum[idx].theNorthLatitude, 
            threeParamDatum[idx].theParam1, 
            threeParamDatum[idx].theParam2, 
            threeParamDatum[idx].theParam3)));
      }

      ++idx;
   }
   idx = 0;
   while( std::strlen(sevenParamDatum[idx].theCode) )
   {
      theDatumTable.insert(std::make_pair(std::string(sevenParamDatum[idx].theCode), 
         new ossimSevenParamDatum(sevenParamDatum[idx].theCode, 
         sevenParamDatum[idx].theName,
         ossimEllipsoidFactory::instance()->create(ossimString(sevenParamDatum[idx].theEllipsoidCode)),
         sevenParamDatum[idx].theSigmaX, 
         sevenParamDatum[idx].theSigmaY, 
         sevenParamDatum[idx].theSigmaZ, 
         sevenParamDatum[idx].theWestLongitude, 
         sevenParamDatum[idx].theEastLongitude, 
         sevenParamDatum[idx].theSouthLatitude, 
         sevenParamDatum[idx].theNorthLatitude,
         sevenParamDatum[idx].theParam1,
         sevenParamDatum[idx].theParam2, 
         sevenParamDatum[idx].theParam3,
         sevenParamDatum[idx].theParam4, 
         sevenParamDatum[idx].theParam5,
         sevenParamDatum[idx].theParam6, 
         sevenParamDatum[idx].theParam7)));
      ++idx;
   }

   // Fetch the HARN grid filenames and add these datums to the table:
   ossimFilename file = ossimPreferences::instance()->findPreference("datum_grids");

   if(file != "")
   {
      if(!file.isDir())
      {
         file = file.path();
      }

      ossimFilename fileTest1 = file.dirCat("conus.las");
      ossimFilename fileTest2 = file.dirCat("conus.los");

      if (fileTest1.exists() && fileTest2.exists())
      {
         theDatumTable.insert(std::make_pair(std::string("NAS"), new ossimNadconNasDatum(file)));
         theDatumTable.insert(std::make_pair(std::string("NAR"), new ossimNadconNarDatum(file)));
      }
   }
}

void ossimDatumFactory::writeCStructure(const ossimFilename& /*file*/)
{
#if 0
   std::ofstream out(file.c_str());

   if(!out) return;

   out << "typedef struct ossimSevenParamDatumType" << std::endl
      << "{" << std::endl
      << "   ossimString theCode;\n"
      << "   ossimString theName;\n"
      << "   ossimString theEllipsoidCode;\n"
      << "   ossim_float64 theSigmaX;\n"
      << "   ossim_float64 theSigmaY;\n"
      << "   ossim_float64 theSigmaZ;\n"
      << "   ossim_float64 theWestLongitude;\n"
      << "   ossim_float64 theEastLongitude;\n"
      << "   ossim_float64 theSouthLatitude;\n"
      << "   ossim_float64 theNorthLatitude;\n"
      << "   ossim_float64 theParam1;\n"
      << "   ossim_float64 theParam2;\n"
      << "   ossim_float64 theParam3;\n"
      << "   ossim_float64 theParam4;\n"
      << "   ossim_float64 theParam5;\n"
      << "   ossim_float64 theParam6;\n"
      << "   ossim_float64 theParam7;\n"
      << "};\n";

   out << "typedef struct ossimThreeParamDatumType" << std::endl
      << "{" << std::endl
      << "   ossimString theCode;\n"
      << "   ossimString theName;\n"
      << "   ossimString theEllipsoidCode;\n"
      << "   ossim_float64 theSigmaX;\n"
      << "   ossim_float64 theSigmaY;\n"
      << "   ossim_float64 theSigmaZ;\n"
      << "   ossim_float64 theWestLongitude;\n"
      << "   ossim_float64 theEastLongitude;\n"
      << "   ossim_float64 theSouthLatitude;\n"
      << "   ossim_float64 theNorthLatitude;\n"
      << "   ossim_float64 theParam1;\n"
      << "   ossim_float64 theParam2;\n"
      << "   ossim_float64 theParam3;\n"
      << "};\n";
   out << "#define NUMBER_OF_SEVEN_PARAM_DATUMS " << 2 << std::endl;
   if(out)
   {
      std::map<std::string, const ossimDatum*>::iterator datum;
      out << "static ossimThreeParamDatumType threeParamDatum[] = {\n";
      datum = theDatumTable.begin();
      ossim_uint32 datumCount = 0;
      while(datum != theDatumTable.end())
      {
         bool written = false;
         if( ((*datum).first != "NAS") &&
            ((*datum).first != "NAR")&&
            ((*datum).first != "EUR-7")&&
            ((*datum).first != "OGB-7"))
         {
            written = true;
            const ossimDatum* d = (*datum).second;

            out << "{\"" << d->code() << "\", "
               << "\"" << d->name() << "\", "
               << "\"" << d->ellipsoid()->code() << "\", "
               << d->sigmaX() << ", "
               << d->sigmaY() << ", "
               << d->sigmaZ() << ", "
               << d->westLongitude()*DEG_PER_RAD << ", "
               << d->eastLongitude()*DEG_PER_RAD << ", "
               << d->southLatitude()*DEG_PER_RAD << ", "
               << d->northLatitude()*DEG_PER_RAD << ", "
               << d->param1() << ", " 
               << d->param2() << ", " 
               << d->param3() << "}";
            ++datumCount;
         }
         ++datum;
         if(datum != theDatumTable.end()&&written)
         {

            out << "," << std::endl;
         }
         else if(datum == theDatumTable.end())
         {
            out << std::endl;
         }
      }
      out << "};" << std::endl;

      out << "#define NUMBER_OF_THREE_PARAM_DATUMS " << datumCount << std::endl;
      out << "static ossimSevenParamDatumType sevenParamDatum[] = {\n";
      const ossimDatum* d = create("EUR-7");
      if(d)
      {
         out << "{\"" << d->code() << "\", "
            << "\"" << d->name() << "\", "
            << "\"" << d->ellipsoid()->code() << "\", "
            << d->sigmaX() << ", "
            << d->sigmaY() << ", "
            << d->sigmaZ() << ", "
            << d->westLongitude()*DEG_PER_RAD << ", "
            << d->eastLongitude()*DEG_PER_RAD << ", "
            << d->southLatitude()*DEG_PER_RAD << ", "
            << d->northLatitude()*DEG_PER_RAD << ", "
            << d->param1() << ", " 
            << d->param2() << ", " 
            << d->param3() << ", "
            << d->param4() << ", "
            << d->param5() << ", "
            << d->param6() << ", "
            << d->param7() << "},\n";
      }
      d = create("OGB-7");
      if(d)
      {
         out << "{\"" << d->code() << "\", "
            << "\"" << d->name() << "\", "
            << "\"" << d->ellipsoid()->code() << "\", "
            << d->sigmaX() << ", "
            << d->sigmaY() << ", "
            << d->sigmaZ() << ", "
            << d->westLongitude()*DEG_PER_RAD << ", "
            << d->eastLongitude()*DEG_PER_RAD << ", "
            << d->southLatitude()*DEG_PER_RAD << ", "
            << d->northLatitude()*DEG_PER_RAD << ", "
            << d->param1() << ", " 
            << d->param2() << ", " 
            << d->param3() << ", "
            << d->param4() << ", "
            << d->param5() << ", "
            << d->param6() << ", "
            << d->param7() << "}\n";
      }
      out << "};" << std::endl;
   }
#endif
}

