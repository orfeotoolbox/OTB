//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfAttributes.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <iostream>
 
#include <ossim/support_data/ossimRpfAttributes.h>

std::ostream& operator<<(std::ostream& out,
                    const ossimRpfAttributes& data)
{
   data.print(out);
   
   return out;
}

ossimRpfAttributes::ossimRpfAttributes()
{
   clearFields();
}
ossimRpfAttributes::~ossimRpfAttributes()
{
}

std::ostream& ossimRpfAttributes::print(std::ostream& out,
                                        const std::string& prefix) const
{
   out << prefix << "CurrencyDate: " << theCurrencyDate << "\n"
       << prefix << "ProductionDate: " << theProductionDate << "\n"
       << prefix << "SignificantDate: " << theSignificantDate << "\n"
       << prefix << "ChartSeriesCode: " << theChartSeriesCode << "\n"
       << prefix << "MapDesignationCode: " << theMapDesignationCode << "\n"
       << prefix << "OldHorDatum: " << theOldHorDatum << "\n"
       << prefix << "Edition: " << theEdition << "\n"
       << prefix << "ProjectionCode: " << theProjectionCode << "\n"
       << prefix << "ProjectionA: " << theProjectionA << "\n"
       << prefix << "ProjectionB: " << theProjectionB << "\n"
       << prefix << "ProjectionC: " << theProjectionC << "\n"
       << prefix << "ProjectionD: " << theProjectionD << "\n"
       << prefix << "VertDatumCode: " << theVertDatumCode<< "\n"
       << prefix << "HorDatumCode: " << theHorDatumCode << "\n"
       << prefix << "VertAbsAccuracy: " << theVertAbsAccuracy << "\n"
       << prefix << "VertAbsUnits: " << theVertAbsUnits << "\n"
       << prefix << "HorAbsAccuracy: " << theHorAbsAccuracy << "\n"
       << prefix << "HorAbsUnits: " << theHorAbsUnits << "\n"
       << prefix << "VertRelAccuracy: " << theVertRelAccuracy << "\n"
       << prefix << "VertRelUnits: " << theVertRelUnits << "\n"
       << prefix << "HorRelAccuracy: " << theHorRelAccuracy << "\n"
       << prefix << "HorRelUnits: " << theHorRelUnits << "\n"
       << prefix << "ellipsoidCode: " << ellipsoidCode << "\n"
       << prefix << "SoundingDatumCode: " << theSoundingDatumCode << "\n"
       << prefix << "NavSystemCode: " << theNavSystemCode << "\n"
       << prefix << "GridCode: " << theGridCode << "\n"
       << prefix << "EeasterlyMagChange: " << theEeasterlyMagChange << "\n"
       << prefix << "EasterlyMagChangeUnits: " << theEasterlyMagChangeUnits << "\n"
       << prefix << "WesterlyMagChange: " << theWesterlyMagChange << "\n"
       << prefix << "WesterlyMagChangeUnits: " << theWesterlyMagChangeUnits << "\n"
       << prefix << "MagAngle: " << theMagAngle << "\n"
       << prefix << "GridConver: " << theGridConver << "\n"
       << prefix << "GridConverUnits: " << theGridConverUnits << "\n"
       << prefix << "HighElevation: " << theHighElevation << "\n"
       << prefix << "HighElevationUnits: " << theHighElevationUnits << "\n"
       << prefix << "HighLat: " << theHighLat << "\n"
       << prefix << "HighLon: " << theHighLon << "\n"
       << prefix << "LegendFileName: " << theLegendFileName << "\n"
       << prefix << "DataSource: " << theDataSource << "\n"
       << prefix << "gsd: " << theGsd << "\n"
       << prefix << "DataLevel: " << theDataLevel << std::endl;
   return out;
}

bool ossimRpfAttributes::isEmpty()const
{
   for(std::map<ossim_uint32, bool>::const_iterator iter= theAttributeIdBoolMap.begin();
       iter != theAttributeIdBoolMap.end();++iter)
   {
      if(iter->second!=false)
      {
         return false;
      }
   }
   return true;
}

void ossimRpfAttributes::clearFields()
{
   theCurrencyDate    = "";
   theProductionDate  = "";
   theSignificantDate = "";
   theChartSeriesCode = "";
   theMapDesignationCode = "";
   theOldHorDatum = "";
   theEdition = "";
   theProjectionCode = "";
   theProjectionA = 0.0;
   theProjectionB = 0.0;
   theProjectionC = 0.0;
   theProjectionD = 0.0;
   theVertDatumCode = "";
   theHorDatumCode = "";
   theVertAbsAccuracy = 0;
   theVertAbsUnits = 0;
   theHorAbsAccuracy = 0;
   theHorAbsUnits    = 0;
   theVertRelAccuracy = 0;
   theVertRelUnits = 0;
   theHorRelAccuracy = 0;
   theHorRelUnits = 0;
   ellipsoidCode = "";
   theSoundingDatumCode = "";
   theNavSystemCode = 0;
   theGridCode = "";
   theEeasterlyMagChange = 0.0;
   theEasterlyMagChangeUnits = 0;
   theWesterlyMagChange = 0.0;
   theWesterlyMagChangeUnits = 0;
   theMagAngle = 0.0;
   theMagAngleUnits = 0;
   theGridConver = 0.0;
   theGridConverUnits = 0;
   theHighElevation = 0.0;
   theHighElevationUnits = 0;
   theHighLat = 0.0;
   theHighLon = 0.0;
   theLegendFileName = "";
   theDataSource = "";
   theGsd = 0;
   theDataLevel = 0;

   theAttributeIdBoolMap.clear();
}

void ossimRpfAttributes::setAttributeFlag(ossim_uint32 id,
                                          bool flag)
{
   std::map<ossim_uint32, bool>::iterator iter = theAttributeIdBoolMap.find(id);

   if(iter != theAttributeIdBoolMap.end())
   {
      if(flag)
      {
         iter->second = flag;
      }
      else
      {
         theAttributeIdBoolMap.erase(iter); 
      }
   }
   else if(flag)
   {
	   theAttributeIdBoolMap.insert(std::make_pair(id, flag));
   }
}

bool ossimRpfAttributes::getAttributeFlag(ossim_uint32 id)const
{
   std::map<ossim_uint32, bool>::const_iterator iter = theAttributeIdBoolMap.find(id);

   if(iter != theAttributeIdBoolMap.end())
   {
      return iter->second;
   }

   return false;
}
