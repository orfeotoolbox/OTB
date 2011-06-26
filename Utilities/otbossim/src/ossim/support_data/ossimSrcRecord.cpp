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
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>

//*************************************************************************************************
// Default constructor
//*************************************************************************************************
ossimSrcRecord::ossimSrcRecord() 
:  m_entryIndex(-1),
   m_weight(0.0),
   m_isVectorData(false),
   m_isRgbData(false)
{}

//*************************************************************************************************
// Constructs given an in-memory KWL and entry index.
//*************************************************************************************************
ossimSrcRecord::ossimSrcRecord(const ossimKeywordlist& src_kwl, ossim_uint32 index, ossimString prefix_str)
:  m_entryIndex(-1),
   m_weight(0.0),
   m_isVectorData(false),
   m_isRgbData(false)
{
   prefix_str += ossimString::toString(index) + ".";
   const char* prefix = prefix_str.chars();

   loadState(src_kwl, prefix);
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
bool ossimSrcRecord::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Read image filename entry (required):
   m_rgbFilenames.clear();
   m_rgbHistogramPaths.clear();
   m_rgbHistogramOps.clear();
   m_rgbOverviewPaths.clear();
   ossimString lookup = kwl.find(prefix, "r");
   if (!lookup.empty())
   {
      m_filename = lookup;
      m_isRgbData = true;
      m_rgbFilenames.push_back(lookup);

      lookup = kwl.find(prefix, "r.hist");
      m_rgbHistogramPaths.push_back(lookup.downcase());
     
      lookup = kwl.find(prefix, "r.hist-op");
      m_rgbHistogramOps.push_back(lookup.downcase());

      lookup = kwl.find(prefix, "r.ovr");
      m_rgbOverviewPaths.push_back(lookup);
     
      lookup = kwl.find(prefix, "g");
      m_rgbFilenames.push_back(lookup);
      
      lookup = kwl.find(prefix, "g.hist");
      m_rgbHistogramPaths.push_back(lookup.downcase());
      
      lookup = kwl.find(prefix, "g.ovr");
      m_rgbOverviewPaths.push_back(lookup);
      
      lookup = kwl.find(prefix, "g.hist-op");
      m_rgbHistogramOps.push_back(lookup.downcase());
      
      lookup = kwl.find(prefix, "b");
      m_rgbFilenames.push_back(lookup);
      
      lookup = kwl.find(prefix, "b.hist");
      m_rgbHistogramPaths.push_back(lookup.downcase());
      
      lookup = kwl.find(prefix, "b.hist-op");
      m_rgbHistogramOps.push_back(lookup.downcase());
      
      lookup = kwl.find(prefix, "b.ovr");
      m_rgbOverviewPaths.push_back(lookup);
      
      return true;
   }
      
   lookup = kwl.find(prefix, "file");
   if (!lookup.empty())
   {
      m_filename = lookup;
      m_attributesKwl.add(ossimKeywordNames::FILENAME_KW, m_filename.chars());
   }
   else
   {
      m_filename.clear();
      return false;
   }

   // Read image entry index:
   lookup = kwl.find(prefix, "entry");
   if (!lookup.empty()) 
   {
      m_entryIndex = lookup.toInt32();
      m_attributesKwl.add(ossimKeywordNames::ENTRY_KW, m_entryIndex);
   }
   else
   {
      m_entryIndex = -1;
   }

   // Establish supplementary directory containing overview:
   lookup = kwl.find(prefix, "ovr");
   if (!lookup.empty()) 
   {
      m_overviewPath = ossimFilename(lookup);
      m_attributesKwl.add(ossimKeywordNames::OVERVIEW_FILE_KW, m_overviewPath.chars());
   }
   else
   {
      m_overviewPath.clear();
   }

   lookup = kwl.find(prefix, "mask");
   if (!lookup.empty()) 
   {
      m_maskPath = ossimFilename(lookup);
   }
   else
   {
      m_maskPath.clear();
   }

   // Histogram operation for this image:
   lookup = kwl.find(prefix, "hist");
   if (!lookup.empty()) 
   {
      m_histogramPath = lookup.downcase();
   }
   else
   {
      m_histogramPath.clear();
   }
   
   // Histogram operation for this image:
   lookup = kwl.find(prefix, "hist-op");
   if (!lookup.empty()) 
   {
      m_histogramOp = lookup.downcase();
   }
   else
   {
      m_histogramOp.clear();
   }
  
   // Newer more generic spec of supp dir since more than just ovrs may reside there:
   lookup = kwl.find(prefix, "support");
   if (!lookup.empty()) 
   {
      setSupportDir(lookup);
   }
   else
   {
      m_supportDir.clear();
   }
         
   // Establish selected bands:
   lookup = kwl.find(prefix, "rgb");
   if (!lookup.empty())
   {
      m_bandList.clear();

      //---
      // Multiple bands delimited by comma:
      //
      // NOTE:  Keyword list ONE based.
      // ossimBandSelector list ZERO based.
      //---
      std::vector<ossimString> bandsStr = lookup.split(",");
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

   lookup = kwl.find(prefix, "opacity");
   if (!lookup.empty())
      m_weight =  lookup.toDouble();

   // Look for vector data info:
   m_isVectorData = false;
   ossimString vector_prefix (prefix);
   vector_prefix += "vector.";
   ossimKeywordlist vectorKwl;

#define DIRECT_KW_MAPPING false
   if (DIRECT_KW_MAPPING)
   {
      // This method of stuffing the attributes KWL is presented here as an example of the way we 
      // should do it: 
      m_attributesKwl.add(kwl, vector_prefix.chars()); 
   }
   else
   {
      // Need to translate SRC keywords to KWs expected by OSSIM:
      lookup = kwl.find(vector_prefix, "line.color");
      if (!lookup.empty())
         vectorKwl.add(ossimKeywordNames::PEN_COLOR_KW, lookup);
      lookup = kwl.find(vector_prefix, "line.width");
      if (!lookup.empty())
         vectorKwl.add(ossimKeywordNames::THICKNESS_KW, lookup);
      lookup = kwl.find(vector_prefix, "fill.color");
      if (!lookup.empty())
      {
         vectorKwl.add(ossimKeywordNames::FILL_FLAG_KW, true);
         vectorKwl.add(ossimKeywordNames::BRUSH_COLOR_KW, lookup);
      }
      lookup = kwl.find(vector_prefix, "query");
      if (!lookup.empty())
        vectorKwl.add(ossimKeywordNames::QUERY_KW, lookup);
   }

   if (vectorKwl.getSize())
   {
      m_isVectorData = true;
      m_attributesKwl.add(0, vectorKwl);
   }

   // Read keywords associated with special pixel remapping (pixel flipping, clamping, and 
   // clipping):
   lookup = kwl.find(prefix, "replacement_mode");
   if (!lookup.empty())
      m_pixelFlipParams.replacementMode = lookup;
   lookup = kwl.find(prefix, "clamp.min");
   if (!lookup.empty())
      m_pixelFlipParams.clampMin = lookup.toDouble();
   lookup = kwl.find(prefix, "clamp.max");
   if (!lookup.empty())
      m_pixelFlipParams.clampMax = lookup.toDouble();
   lookup = kwl.find(prefix, "clip.min");
   if (!lookup.empty())
      m_pixelFlipParams.clipMin = lookup.toDouble();
   lookup = kwl.find(prefix, "clip.max");
   if (!lookup.empty())
      m_pixelFlipParams.clipMax = lookup.toDouble();

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
      setOverview(m_supportDir);
   if (m_histogramPath.empty())
      m_histogramPath = m_supportDir;
   if (m_maskPath.empty())
      m_maskPath = m_supportDir;
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
void ossimSrcRecord::setFilename(const ossimFilename& f)          
{ 
   m_filename = f; 
   m_attributesKwl.add(ossimKeywordNames::FILENAME_KW, m_filename.chars());
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
void ossimSrcRecord::setEntryIndex(ossim_int32 i)                 
{ 
   m_entryIndex = i; 
   m_attributesKwl.add(ossimKeywordNames::ENTRY_KW, m_entryIndex);
}

//*************************************************************************************************
// METHOD
//*************************************************************************************************
void ossimSrcRecord::setOverview(const ossimFilename& f)          
{ 
   m_overviewPath = f; 
   m_attributesKwl.add(ossimKeywordNames::OVERVIEW_FILE_KW, m_overviewPath.chars());
}

