//*******************************************************************
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
// $Id: ossimNitfImageBandV2_0.cpp 16035 2009-12-03 21:32:27Z dburken $

#include <sstream>
#include <iostream>
#include <iomanip>
#include <ossim/support_data/ossimNitfImageBandV2_0.h>
#include <ossim/base/ossimString.h>

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

std::ostream& ossimNitfImageBandV2_0::print(std::ostream& out,
                                            const std::string& prefix,
                                            ossim_uint32 band)const
{
   std::ostringstream os;
   os << std::setw(3) << std::setfill('0') << (band+1) << ":";

   ossimString tmpStr = "IREPBAND";
   tmpStr += os.str();
   
   out << prefix << std::setw(24)
       << tmpStr << theBandRepresentation << "\n";

   tmpStr = "ISUBCAT";
   tmpStr += os.str();
   
   out << prefix << std::setw(24)
       << tmpStr << theBandSignificance << "\n";

   tmpStr = "IFC";
   tmpStr += os.str();
   
   out << prefix << std::setw(24)
       << tmpStr << theBandImageFilterCondition << "\n";

   tmpStr = "IMFLT";
   tmpStr += os.str();

   out << prefix << std::setw(24)
       << tmpStr << theBandStandardImageFilterCode << "\n";

   tmpStr = "NLUTS";
   tmpStr += os.str();

   out << prefix << std::setw(24)
       << tmpStr << theBandNumberOfLuts << "\n";

   tmpStr = "NELUTS";
   tmpStr += os.str();

   out << prefix << std::setw(24)
       << tmpStr << theBandNumberOfLutEntries << "\n";
   
   return out;
}

ossim_uint32 ossimNitfImageBandV2_0::getNumberOfLuts()const
{
   return static_cast<ossim_uint32>(theLookupTables.size());
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

ossimString ossimNitfImageBandV2_0::getBandRepresentation()const
{
   return ossimString(theBandRepresentation);
}

void ossimNitfImageBandV2_0::setBandRepresentation(const ossimString& rep)
{
   std::ostringstream out;

   out << std::setw(2)
       << std::setfill(' ')
       << std::setiosflags(std::ios::left)
       << rep.c_str();

   memcpy(theBandRepresentation, out.str().c_str(), 2);
}

ossimString ossimNitfImageBandV2_0::getBandSignificance()const
{
   return ossimString(theBandSignificance);
}

void ossimNitfImageBandV2_0::setBandSignificance(const ossimString& rep)
{
   std::ostringstream out;
   
   out << std::setw(6)
       << std::setfill(' ')
       << rep.c_str();

   memcpy(theBandSignificance, out.str().c_str(), 6);
}

void ossimNitfImageBandV2_0::clearFields()
{
   memset(theBandRepresentation, ' ', 2);
   memset(theBandSignificance, ' ', 6);
   memset(theBandImageFilterCondition, 'N', 1);
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
