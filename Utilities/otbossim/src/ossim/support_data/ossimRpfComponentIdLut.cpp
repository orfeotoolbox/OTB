//----------------------------------------------------------------------------
//
// File:     ossimRpfComponentIdLut.cpp
// 
// License:  See top level LICENSE.txt file.
// 
// Author:   David Burken
//
// Description:
//
//  Lookup table for mapping RPF component ID's to strings.
//
// See MIL-STD-2411-1, section 5.1.1 for detailed information.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfComponentIdLut.cpp 20324 2011-12-06 22:25:23Z dburken $

#include <ossim/support_data/ossimRpfComponentIdLut.h>

ossimRpfComponentIdLut* ossimRpfComponentIdLut::theInstance = 0;

ossimRpfComponentIdLut* ossimRpfComponentIdLut::instance()
{
   if (!theInstance)
   {
      theInstance = new ossimRpfComponentIdLut;
   }
   return theInstance;
}

ossimRpfComponentIdLut::ossimRpfComponentIdLut()
{
   //---
   // Complete initialization of data member "theTable".
   // Note:  ID's defined in ossimRpfConstants.h file.
   //---
   ossimKeyValueMap entry;
   
   entry.init(OSSIM_RPF_HEADER_COMPONENT,
              "RPF_HEADER_COMPONENT");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_LOCATION_COMPONENT,
              "RPF_LOCATION_COMPONENT");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COVERAGE_SECTION_SUBHEADER,
              "RPF_COVERAGE_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COMPRESSION_SECTION_SUBHEADER,
              "RPF_COMPRESSION_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COMPRESION_LOOKUP_SUBSECTION,
              "RPF_COMPRESION_LOOKUP_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COMPRESION_PARAMETER_SUBSECTION,
              "RPF_COMPRESION_PARAMETER_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COLOR_GRAYSCALE_SECTION_SUBHEADER,
              "RPF_COLOR_GRAYSCALE_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COLORMAP_SUBSECTION,
              "RPF_COLORMAP_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_IMAGE_DESCRIPTION_SUBHEADER,
              "RPF_IMAGE_DESCRIPTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_IMAGE_DISPLAY_PARAMETERS_SUBHEADER,
              "RPF_IMAGE_DISPLAY_PARAMETERS_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_MASK_SUBSECTION,
              "RPF_MASK_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COLOR_CONVERTER_SUBSECTION,
              "RPF_COLOR_CONVERTER_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_SPATIAL_DATA_SUBSECTION,
              "RPF_SPATIAL_DATA_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_ATTRIBUTE_SECTION_SUBHEADER,
              "RPF_ATTRIBUTE_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_ATTRIBUTE_SUBSECTION,
              "RPF_ATTRIBUTE_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_EXPLICIT_AREAL_COVERAGE_TABLE,
              "RPF_EXPLICIT_AREAL_COVERAGE_TABLE");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_RELATED_IMAGES_SECTION_SUBHEADER,
              "RPF_RELATED_IMAGES_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_RELATED_IMAGES_SUBSECTION,
              "RPF_RELATED_IMAGES_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_REPLACE_UPDATE_SECTION_SUBHEADER,
              "RPF_REPLACE_UPDATE_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_REPLACE_UPDATE_TABLE,
              "RPF_REPLACE_UPDATE_TABLE");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_BOUNDARY_RECT_SECTION_SUBHEADER,
              "RPF_BOUNDARY_RECT_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_BOUNDARY_RECT_TABLE,
              "RPF_BOUNDARY_RECT_TABLE");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_FRAME_FILE_INDEX_SECTION_SUBHEADER,
              "RPF_FRAME_FILE_INDEX_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_FRAME_FILE_INDEX_SUBSECTION,
              "RPF_FRAME_FILE_INDEX_SUBSECTION");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COLOR_TABLE_INDEX_SECTION_SUBHEADER,
              "RPF_COLOR_TABLE_INDEX_SECTION_SUBHEADER");
   theTable.push_back(entry);

   entry.init(OSSIM_RPF_COLOR_TABLE_INDEX_RECORD,
              "RPF_COLOR_TABLE_INDEX_RECORD");
   theTable.push_back(entry);
}

ossimRpfComponentIdLut::~ossimRpfComponentIdLut()
{
   theInstance = 0;
}

ossimKeyword ossimRpfComponentIdLut::getKeyword() const
{
   return ossimKeyword("rpf_component_id", "");
}
