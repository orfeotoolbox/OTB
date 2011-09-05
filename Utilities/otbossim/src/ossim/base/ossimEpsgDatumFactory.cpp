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
   m_epsgToAlphaMap.insert(std::make_pair(6135, std::string("OHI-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6139, std::string("PUR"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6152, std::string("NAR"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6154, std::string("EUR-M"))); // QUESTIONABLE  
   m_epsgToAlphaMap.insert(std::make_pair(6155, std::string("DAL"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6156, std::string("CCD"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6158, std::string("NAP"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6165, std::string("BID"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6169, std::string("AMA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6175, std::string("SRL")));   // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6201, std::string("ADI-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6202, std::string("AUA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6203, std::string("AUG"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6204, std::string("AIN-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6205, std::string("AFG"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6209, std::string("ARF-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6210, std::string("ARS-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6216, std::string("BER"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6218, std::string("BOO")));   // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6219, std::string("BUR"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6221, std::string("CAI"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6222, std::string("CAP"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6223, std::string("CGE"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6224, std::string("CHU"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6236, std::string("HTN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6238, std::string("IDN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6239, std::string("INF-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6240, std::string("INH-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6244, std::string("KAN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6245, std::string("KEA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6248, std::string("PRP-M"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6250, std::string("LEH"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6251, std::string("LIB"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6253, std::string("LUZ-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6255, std::string("HEN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6256, std::string("MIK"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6261, std::string("MER"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6262, std::string("MAS"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6263, std::string("MIN-A"))); // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6266, std::string("MPO"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6267, std::string("NAS-C"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6269, std::string("NAR-C"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6270, std::string("NAH-C"))); // QUESTIONABLE
   m_epsgToAlphaMap.insert(std::make_pair(6282, std::string("PTN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6284, std::string("PUK"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6285, std::string("QAT")));   // QUESTIONABLE
   m_epsgToAlphaMap.insert(std::make_pair(6287, std::string("QUO"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6292, std::string("SAP"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6293, std::string("SCK"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6297, std::string("TAN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6298, std::string("TIL"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6301, std::string("TOY-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6304, std::string("VOI")));   // QUESTIONABLE 
   m_epsgToAlphaMap.insert(std::make_pair(6307, std::string("NSD"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6309, std::string("YAC"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6311, std::string("ZAN"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6322, std::string("WGD"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6326, std::string("WGE"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6601, std::string("AIA"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6604, std::string("ASM"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6611, std::string("HKD")));   // QUESTIONABLE
   m_epsgToAlphaMap.insert(std::make_pair(6613, std::string("GSE"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6615, std::string("POS"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6616, std::string("SGM"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6618, std::string("SAN-M"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6658, std::string("HJO"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6668, std::string("EUS"))); 
   m_epsgToAlphaMap.insert(std::make_pair(6807, std::string("NTF"))); 
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
      ossim_uint32 code;
      ossimString group; // assume we are working with EPSG codes
      if (epsg_spec.contains(":"))
      {
         group = epsg_spec.before(":");
         code = epsg_spec.after(":").toUInt32();
      }
      else
      {
         group = "EPSG"; // No group spec provided, assuming EPSG:
         code = epsg_spec.toUInt32();
      }
      if ((group != "EPSG") || (code == 0))
         return 0;

      return create(code);
   }
   return 0;
}

//*************************************************************************************************
//! Creates a datum instance given a KWL and prefix
//*************************************************************************************************
const ossimDatum* 
ossimEpsgDatumFactory::create(const ossimKeywordlist& kwl, const char *prefix) const
{
   ossimString lookup = kwl.find(prefix, ossimKeywordNames::DATUM_KW);
   if(!lookup.empty())
      return create(lookup);
   return 0;

}
//*************************************************************************************************
//! Creates given an EPSG spec in the form "EPSG:<datum_code>".
//*************************************************************************************************
const ossimDatum* ossimEpsgDatumFactory::create(ossim_uint32 datum_code) const
{
   // Geographic coordinate system codes (4XXX) are sometimes used in place of their corresponding
   // datum code (6XXX). Check for this possibility and transpose the code if so:
   if ((datum_code >= 4135) && (datum_code <= 4807))
      datum_code += 2000;

   // Find the datum code in the map in order to determine the corresponding OSSIM/Geotrans alpha
   // code. Then use the datum factory to create the datum.
   ossimString alphaCode = findAlphaCode(datum_code);
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
   std::map<ossim_uint32, std::string>::const_iterator iter = m_epsgToAlphaMap.begin();

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
   std::map<ossim_uint32, std::string>::const_iterator iter = m_epsgToAlphaMap.begin();
   while (iter != m_epsgToAlphaMap.end())
   {
      if (iter->second == alpha_code.string())
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
   ossimString result("");
   std::map<ossim_uint32, std::string>::const_iterator iter = m_epsgToAlphaMap.find(epsg_code);
   if (iter != m_epsgToAlphaMap.end())
   {
      result = iter->second;
   }
   return result;
}
