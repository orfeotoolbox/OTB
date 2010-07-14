//*************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
//  LICENSE: See top level LICENSE.txt file.
//   AUTHOR: Oscar Kramer
//
//  CLASS DESCRIPTION: 
//! Class used for parsing the command line *.src files. This is a scheme for providing input
//! file information to am ossim app such as ossim-orthoigen.
//*************************************************************************************************
//  $Id$

#include <ossim/support_data/ossimSrcRecord.h>
#include <ossim/base/ossimKeywordlist.h>

//*************************************************************************************************
// Default constructor
//*************************************************************************************************
ossimSrcRecord::ossimSrcRecord() 
:  m_entryIndex(-1)
{}

//*************************************************************************************************
// Constructs given an in-memory KWL and entry index.
//*************************************************************************************************
ossimSrcRecord::ossimSrcRecord(const ossimKeywordlist& src_kwl, ossim_uint32 index)
:  m_entryIndex(-1)
{
   ossimString prefix_str = "image";
   prefix_str += ossimString::toString(index) + ".";
   const char* prefix = prefix_str.chars();

   loadState(src_kwl, prefix);
}

bool ossimSrcRecord::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Read image filename entry (required):
   const char* lookup = kwl.find(prefix, "file");
   if (lookup)
   {
      m_filename = lookup;
   }
   else
   {
      m_filename.clear();
      return false;
   }

   // Read image entry index:
   lookup = kwl.find(prefix, "entry");
   if (lookup) 
   {
      m_entryIndex = ossimString(lookup).toInt32();
   }
   else
   {
      m_entryIndex = -1;
   }

   // Establish supplementary directory containing overview:
   lookup = kwl.find(prefix, "ovr");
   if (lookup) 
   {
      m_overviewPath = ossimFilename(lookup);
   }
   else
   {
      m_overviewPath.clear();
   }

   // Histogram operation for this image:
   lookup = kwl.find(prefix, "hist");
   if (lookup) 
   {
      m_histogramPath = ossimString(lookup).downcase();
   }
   else
   {
      m_histogramPath.clear();
   }
   
   // Histogram operation for this image:
   lookup = kwl.find(prefix, "hist-op");
   if (lookup) 
   {
      m_histogramOp = ossimString(lookup).downcase();
   }
   else
   {
      m_histogramOp.clear();
   }
  
   // Newer more generic spec of supp dir since more than just ovrs may reside there:
   lookup = kwl.find(prefix, "support");
   if (lookup) 
   {
      setSupportDir(ossimFilename(lookup));
   }
   else
   {
      m_supportDir.clear();
   }
         
   // Establish selected bands:
   lookup = kwl.find(prefix, "rgb");
   if (lookup)
   {
      m_bandList.clear();

      //---
      // Multiple bands delimited by comma:
      //
      // NOTE:  Keyword list ONE based.
      // ossimBandSelector list ZERO based.
      //---
      std::vector<ossimString> bandsStr = ossimString(lookup).split(",");
      for (ossim_uint32 i = 0; i < bandsStr.size(); i++)
      {
         int band = bandsStr[i].toInt32() - 1;
         if (band >= 0) m_bandList.push_back((ossim_uint32) band);
      }
   }
   else
   {
      m_bandList.clear();
   }

   return true;
}

//*************************************************************************************************
// Sets supplementary data files dir. If the OVR and/or hist dirs are undefined, they are also
// set to this path.
//*************************************************************************************************
void ossimSrcRecord::setSupportDir(const ossimFilename& f)
{
   m_supportDir = f;
   if (m_overviewPath.empty()) 
      m_overviewPath = m_supportDir;
   if (m_histogramPath.empty())
      m_histogramPath = m_supportDir;
}
