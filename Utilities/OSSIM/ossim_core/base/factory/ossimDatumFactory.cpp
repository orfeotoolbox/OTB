
//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// This holds the class definition of DatumFactory.
//*******************************************************************
//  $Id: ossimDatumFactory.cpp,v 1.8 2004/05/14 13:45:56 gpotts Exp $

#include "ossimDatumFactory.h"
#include "ossimEllipsoidFactory.h"
#include "base/data_types/datum/ossimDatum.h"
#include "base/data_types/datum/ossimThreeParamDatum.h"
#include "base/data_types/datum/ossimSevenParamDatum.h"
#include "base/data_types/datum/ossimWgs84Datum.h"
#include "base/data_types/datum/ossimWgs72Datum.h"
#include "base/data_types/datum/ossimNadconNarDatum.h"
#include "base/data_types/datum/ossimNadconNasDatum.h"
#include "base/common/ossimPreferences.h"
#include <base/factory/ossimDatumFactory.inc>

ossimDatumFactory* ossimDatumFactory::theInstance = 0;
map<ossimString, ossimDatum*> ossimDatumFactory::theDatumTable;

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
      theInstance->theWgs84Datum = theInstance->create("WGE");
      theInstance->theWgs72Datum = theInstance->create("WGD");
   }  
   return theInstance; 
} 


void ossimDatumFactory::writeCStructure(const ossimFilename& file)
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
      map<ossimString, ossimDatum*>::iterator datum;
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


const ossimDatum* ossimDatumFactory::create(const ossimString &code)const
{
   map<ossimString, ossimDatum*>::const_iterator
      datum = theDatumTable.find(code);

   if(datum != theDatumTable.end())
   { 
      return (*datum).second;
   }
   else
   {
     if(code == "NAR")
       {
	 map<ossimString, ossimDatum*>::const_iterator
	   datum = theDatumTable.find("NAR-C");
	 if(datum != theDatumTable.end())
	   {
	     return (*datum).second;
	   }
       }
     else if(code == "NAS")
       {
	 map<ossimString, ossimDatum*>::const_iterator
	   datum = theDatumTable.find("NAS-C");
	 if(datum != theDatumTable.end())
	   {
	     return (*datum).second;
	   }
       }
      return NULL;
   }
}

const ossimDatum* ossimDatumFactory::create(const ossimDatum* aDatum)const
{
   if (aDatum)
      return create (aDatum->code());
   else
      return NULL;
}

list<ossimString> ossimDatumFactory::getList()const
{
   map<ossimString, ossimDatum*>::const_iterator datum = theDatumTable.begin();
   list<ossimString> result;

   while(datum != theDatumTable.end())
   {
      result.push_back((*datum).first);
      ++datum;
   }
   return result;
}

void ossimDatumFactory::deleteAll()
{   
   map<ossimString, ossimDatum*>::iterator datum;
   
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
   theDatumTable.insert(make_pair(ossimString("WGE"), new ossimWgs84Datum));
   theDatumTable.insert(make_pair(ossimString("WGD"), new ossimWgs72Datum));

   ossim_uint32 idx = 0;     

   for(idx = 0; idx < NUMBER_OF_THREE_PARAM_DATUMS; ++idx)
   {
      if((threeParamDatum[idx].theCode != "WGE")&&
         (threeParamDatum[idx].theCode != "WGD"))
      {
         theDatumTable.insert(make_pair(threeParamDatum[idx].theCode,
                                        new ossimThreeParamDatum(threeParamDatum[idx].theCode, threeParamDatum[idx].theName,
                                                                 ossimEllipsoidFactory::instance()->create(ossimString(threeParamDatum[idx].theEllipsoidCode)),
                                                                 threeParamDatum[idx].theSigmaX, threeParamDatum[idx].theSigmaY, threeParamDatum[idx].theSigmaZ, 
                                                                 threeParamDatum[idx].theWestLongitude, threeParamDatum[idx].theEastLongitude, threeParamDatum[idx].theSouthLatitude, 
                                                                 threeParamDatum[idx].theNorthLatitude, threeParamDatum[idx].theParam1, threeParamDatum[idx].theParam2, 
                                                                 threeParamDatum[idx].theParam3)));
      }
   }
   for(idx = 0; idx < NUMBER_OF_SEVEN_PARAM_DATUMS; ++idx)
   {
      if((threeParamDatum[idx].theCode != "WGE")&&
         (threeParamDatum[idx].theCode != "WGD"))
      {
         theDatumTable.insert(make_pair(sevenParamDatum[idx].theCode,
                                        new ossimSevenParamDatum(sevenParamDatum[idx].theCode, sevenParamDatum[idx].theName,
                                                                 ossimEllipsoidFactory::instance()->create(ossimString(sevenParamDatum[idx].theEllipsoidCode)),
                                                                 sevenParamDatum[idx].theSigmaX, sevenParamDatum[idx].theSigmaY, sevenParamDatum[idx].theSigmaZ, 
                                                                 sevenParamDatum[idx].theWestLongitude, sevenParamDatum[idx].theEastLongitude, sevenParamDatum[idx].theSouthLatitude, 
                                                                 sevenParamDatum[idx].theNorthLatitude,
                                                                 sevenParamDatum[idx].theParam1,
                                                                 sevenParamDatum[idx].theParam2, 
                                                                 sevenParamDatum[idx].theParam3,
                                                                 sevenParamDatum[idx].theParam4, 
                                                                 sevenParamDatum[idx].theParam5,
                                                                 sevenParamDatum[idx].theParam6, 
                                                                 sevenParamDatum[idx].theParam7)));
      }
   }
   ossimFilename file = ossimPreferences::instance()->findPreference("datum_grids");
   
   if(file != "")
   {
      if(!file.isDir())
      {
         file = file.path();
      }
      
      ossimFilename fileTest1 = file.dirCat("conus.las");
      ossimFilename fileTest2 = file.dirCat("conus.los");
      
      if(fileTest1.exists()&&
         fileTest2.exists())
      {
         theDatumTable.insert(std::make_pair(ossimString("NAS"),
                                             new ossimNadconNasDatum(file)));
         theDatumTable.insert(std::make_pair(ossimString("NAR"),
                                             new ossimNadconNarDatum(file)));
      }
   }
}
