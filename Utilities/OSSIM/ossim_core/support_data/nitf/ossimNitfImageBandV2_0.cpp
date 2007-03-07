//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageBandV2_0.cpp,v 1.5 2005/09/27 19:46:45 dburken Exp $
#include <support_data/nitf/ossimNitfImageBandV2_0.h>
#include <base/data_types/ossimString.h>

ossimNitfImageBandV2_0::ossimNitfImageBandV2_0()
{
   clearFields();
}

ossimNitfImageBandV2_0::~ossimNitfImageBandV2_0()
{
}

void ossimNitfImageBandV2_0::parseStream(std::istream& in)
{
   theLookupTables.clear();
   if(in)
   {      
      theLookupTables.clear();
      in.read(theBandRepresentation, 2);
      in.read(theBandSignificance, 6);
      in.read(theBandImageFilterCondition, 1);
      in.read(theBandStandardImageFilterCode, 3);
      in.read(theBandNumberOfLuts, 1);

      long numberOfLuts = ossimString(theBandNumberOfLuts).toLong();

      if(numberOfLuts > 0)
      {
         theLookupTables.resize(numberOfLuts);
         in.read(theBandNumberOfLutEntries, 5);
         
         ossim_uint32 numberOfLutsIndex = numberOfLuts;
         ossim_uint32 numberOfLutEntries = ossimString(theBandNumberOfLutEntries).toLong();
         ossim_uint32 idx = 0;

         while(idx < numberOfLutsIndex)
         {
            theLookupTables[idx] = new ossimNitfImageLutV2_0; 
            theLookupTables[idx]->setNumberOfEntries(numberOfLutEntries);
            theLookupTables[idx]->parseStream(in);
            
            ++idx;
         }
      }
   }   
}

void ossimNitfImageBandV2_0::writeStream(std::ostream& out)
{
   out.write(theBandRepresentation, 2);
   out.write(theBandSignificance, 6);
   out.write(theBandImageFilterCondition, 1);
   out.write(theBandStandardImageFilterCode, 3);
   out.write(theBandNumberOfLuts, 1);

   if(ossimString(theBandNumberOfLuts) > 0)
   {
      // lut output not supported currently
   }
}

std::ostream& ossimNitfImageBandV2_0::print(std::ostream& out)const
{
   ossim_uint32 idx = 0;
   out << "theBandRepresentation:          " << theBandRepresentation
       << "\ntheBandSignificance:            " << theBandSignificance
       << "\ntheBandImageFilterCondition:    " << theBandImageFilterCondition
       << "\ntheBandStandardImageFilterCode: " << theBandStandardImageFilterCode
       << "\ntheBandNumberOfLuts:            " << theBandNumberOfLuts
       << "\ntheBandNumberOfLutEntries:      " << theBandNumberOfLutEntries
       << endl;
   
   for(idx = 0; idx < theLookupTables.size(); ++idx)
   {
      out << *theLookupTables[idx] << std::endl;
   }
   
   return out;
}

ossim_uint32 ossimNitfImageBandV2_0::getNumberOfLuts()const
{
   return theLookupTables.size();
}

const ossimRefPtr<ossimNitfImageLut> ossimNitfImageBandV2_0::getLut(ossim_uint32 idx)const
{
   if(idx < getNumberOfLuts())
   {
      return theLookupTables[idx];
   }
   
   return NULL;
}

ossimRefPtr<ossimNitfImageLut> ossimNitfImageBandV2_0::getLut(ossim_uint32 idx)
{
   if(idx < getNumberOfLuts())
   {
      return theLookupTables[idx];
   }
   
   return NULL;
}

void ossimNitfImageBandV2_0::clearFields()
{
   memset(theBandRepresentation, ' ', 2);
   memset(theBandSignificance, ' ', 6);
   memset(theBandImageFilterCondition, ' ', 1);
   memset(theBandStandardImageFilterCode, ' ', 3);
   memset(theBandNumberOfLuts, '0', 1);
   memset(theBandNumberOfLutEntries, ' ', 5);
   
   theLookupTables.clear();
   theBandRepresentation[2] = '\0';
   theBandSignificance[6] = '\0';
   theBandImageFilterCondition[1] = '\0';
   theBandStandardImageFilterCode[3] = '\0';
   theBandNumberOfLuts[1] = '\0';
   theBandNumberOfLutEntries[5] = '\0';
}

void ossimNitfImageBandV2_0::printLookupTables(std::ostream& out)const
{
   std::vector<ossimRefPtr<ossimNitfImageLut> >::const_iterator tables;

   tables = theLookupTables.begin();
   while(tables != theLookupTables.end())
   {
      if((*tables).valid())
      {
         out << *((*tables).get());
      }
      ++tables;
      if(tables != theLookupTables.end())
      {
         out << std::endl;
      }
   }
}
