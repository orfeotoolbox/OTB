//*************************************************************************************************
// 
//                          OSSIM -- Open Source Software Image Map
//
// License:  See top level LICENSE.txt file.
//
// Author: Oscar Kramer
//
// Description: Factory for creating datums from EPSG datum codes (a.k.a. GCS codes)
// 
//*************************************************************************************************
//  $Id$

#include <cstring> /* for strlen */
#include <utility> /* for std::make_pair */

#include <ossim/base/ossimEpsgDatumFactory.h>
#include <ossim/base/ossimEllipsoidFactory.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

ossimEpsgDatumFactory* ossimEpsgDatumFactory::m_instance = 0;

//*************************************************************************************************
//! Singleton implementation.
//*************************************************************************************************
ossimEpsgDatumFactory* ossimEpsgDatumFactory::instance()
{
   if(!m_instance) 
      m_instance = new ossimEpsgDatumFactory;
   return m_instance; 
} 

//*************************************************************************************************
// Hidden constructor part of singleton implementation.
// Intializes the map from EPSG numeric to OSSIM/Geotrans alpha codes
// Datums marked questionable because of ambiguities in the name. Need to check the actual 
// datum definition to make sure these are correct. (most are fairly obscure)
//*************************************************************************************************
ossimEpsgDatumFactory::ossimEpsgDatumFactory()
{
   m_epsgToAlphaMap.insert(std::make_pair(4135, ossimString("OHI-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4139, ossimString("PUR"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4152, ossimString("NAR"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4154, ossimString("EUR-M"))); // QUESTIONABLE  
   m_epsgToAlphaMap.insert(std::make_pair(4155, ossimString("DAL"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4156, ossimString("CCD"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4158, ossimString("NAP"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4165, ossimString("BID"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4169, ossimString("AMA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4175, ossimString("SRL")));   // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4201, ossimString("ADI-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4202, ossimString("AUA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4203, ossimString("AUG"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4204, ossimString("AIN-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4205, ossimString("AFG"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4209, ossimString("ARF-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4210, ossimString("ARS-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4216, ossimString("BER"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4218, ossimString("BOO")));   // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4219, ossimString("BUR"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4221, ossimString("CAI"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4222, ossimString("CAP"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4223, ossimString("CGE"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4224, ossimString("CHU"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4236, ossimString("HTN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4238, ossimString("IDN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4239, ossimString("INF-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4240, ossimString("INH-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4244, ossimString("KAN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4245, ossimString("KEA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4248, ossimString("PRP-M"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4250, ossimString("LEH"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4251, ossimString("LIB"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4253, ossimString("LUZ-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4255, ossimString("HEN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4256, ossimString("MIK"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4261, ossimString("MER"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4262, ossimString("MAS"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4263, ossimString("MIN-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4266, ossimString("MPO"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4267, ossimString("NAS-C"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4269, ossimString("NAR-C"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4270, ossimString("NAH-C"))); // QUESTIONABLE
   m_epsgToAlphaMap.insert(std::make_pair(4282, ossimString("PTN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4284, ossimString("PUK"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4285, ossimString("QAT")));   // QUESTIONABLE
   m_epsgToAlphaMap.insert(std::make_pair(4287, ossimString("QUO"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4292, ossimString("SAP"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4293, ossimString("SCK"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4297, ossimString("TAN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4298, ossimString("TIL"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4301, ossimString("TOY-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4304, ossimString("VOI")));   // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(4307, ossimString("NSD"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4309, ossimString("YAC"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4311, ossimString("ZAN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4322, ossimString("WGD"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4326, ossimString("WGE"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4601, ossimString("AIA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4604, ossimString("ASM"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4611, ossimString("HKD")));   // QUESTIONABLE
   m_epsgToAlphaMap.insert(std::make_pair(4613, ossimString("GSE"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4615, ossimString("POS"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4616, ossimString("SGM"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4618, ossimString("SAN-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4658, ossimString("HJO"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4668, ossimString("EUS"))); 
   m_epsgToAlphaMap.insert(std::make_pair(4807, ossimString("NTF"))); 
} 

//*************************************************************************************************
//! Destructor
//*************************************************************************************************
ossimEpsgDatumFactory::~ossimEpsgDatumFactory()
{
   m_instance = 0;
}

//*************************************************************************************************
//! Creates given an EPSG spec in the form "EPSG:<datum_code>".
//*************************************************************************************************
const ossimDatum* ossimEpsgDatumFactory::create(const ossimString &epsg_spec) const
{
   if ( epsg_spec.size() )
   {
      ossim_uint32 gcs_code;
      ossimString group; // assume we are working with EPSG codes
      if (epsg_spec.contains(":"))
      {
         group = epsg_spec.before(":");
         gcs_code = epsg_spec.after(":").toUInt32();
      }
      else
      {
         group = "EPSG"; // No group spec provided, assuming EPSG:
         gcs_code = epsg_spec.toUInt32();
      }
      if ((group != "EPSG") || (gcs_code == 0))
         return 0;

      return create(gcs_code);
   }
   return 0;
}

//*************************************************************************************************
//! Creates a datum instance given a KWL and prefix
//*************************************************************************************************
const ossimDatum* 
ossimEpsgDatumFactory::create(const ossimKeywordlist& kwl, const char *prefix) const
{
   ossimString lookup = kwl.find(prefix, ossimKeywordNames::GCS_CODE_KW);
   if(!lookup.empty())
      return create(lookup);
   return 0;

}
//*************************************************************************************************
//! Creates given an EPSG spec in the form "EPSG:<datum_code>".
//*************************************************************************************************
const ossimDatum* ossimEpsgDatumFactory::create(ossim_uint32 gcs_code) const
{
   // Find the datum code in the map in order to determine the corresponding OSSIM/Geotrans alpha
   // code. Then use the datum factory to create the datum.
   ossimString alphaCode = findAlphaCode(gcs_code);
   if (!alphaCode.empty())
      return ossimDatumFactory::instance()->create(alphaCode);

   return 0;
}

//*************************************************************************************************
//! Returns list of all datums supported, in the form of "EPSG:<code>, <datum_name>".
//*************************************************************************************************
void ossimEpsgDatumFactory::getList(std::vector<ossimString>& list) const
{
   ossimDatumFactory* df = ossimDatumFactory::instance();
   const ossimDatum* datum;
   std::map<ossim_uint32, ossimString>::const_iterator iter = m_epsgToAlphaMap.begin();

   // Loop over all handled datum codes and instantiate the corresponding datum in order to
   // get the corresponding OSSIM/Geotrans name. Eventually the datum info (including its name as
   // it appears in the EPSG database) will be read directly from the Db without using
   // ossimDatumFactory.
   while(iter != m_epsgToAlphaMap.end())
   {
      datum = df->create(iter->second);
      if (datum)
      {
         ossimString entry ("EPSG:");
         entry += ossimString::toString(iter->first);
         entry += ",";
         entry += datum->name();
         list.push_back(entry);
      }
      ++iter;
   }
}

//*************************************************************************************************
//! Specific to this factory only. Given an alpha-code, returns equivalent EPSG datum code or
//! 0 if not found.
//*************************************************************************************************
ossim_uint32 ossimEpsgDatumFactory::findEpsgCode(const ossimString& alpha_code) const
{
   std::map<ossim_uint32, ossimString>::const_iterator iter = m_epsgToAlphaMap.begin();
   while (iter != m_epsgToAlphaMap.end())
   {
      if (iter->second == alpha_code)
         return iter->first;
      ++iter;
   }
   return 0;
}

//*************************************************************************************************
//! Specific to this factory only. Given an EPSG, returns equivalent alpha-code datum code or
//! empty string if not found.
//*************************************************************************************************
ossimString ossimEpsgDatumFactory::findAlphaCode(ossim_uint32 epsg_code) const
{
   std::map<ossim_uint32, ossimString>::const_iterator iter = m_epsgToAlphaMap.begin();
   while (iter != m_epsgToAlphaMap.end())
   {
      if (iter->first == epsg_code)
         return iter->second;
      ++iter;
   }
   return ossimString("");
}

