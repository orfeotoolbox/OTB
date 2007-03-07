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
//********************************************************************
// $Id: ossimRpfConstants.h,v 1.1 2003/05/08 15:39:23 gpotts Exp $
//
#ifndef ossimRpfComponentId_HEADER
#define ossimRpfComponentId_HEADER

const unsigned long OSSIM_RPF_ULONG_NULL = 0xFFFFFFFF;

enum ossimRpfComponentId
{
   OSSIM_RPF_HEADER_COMPONENT   = 128,
   OSSIM_RPF_LOCATION_COMPONENT = 129,
   OSSIM_RPF_COVERAGE_SECTION_SUBHEADER = 130,
   OSSIM_RPF_COMPRESSION_SECTION_SUBHEADER = 131,
   OSSIM_RPF_COMPRESION_LOOKUP_SUBSECTION = 132,
   OSSIM_RPF_COMPRESION_PARAMETER_SUBSECTION = 133,
   OSSIM_RPF_COLOR_GRAYSCALE_SECTION_SUBHEADER = 134,
   OSSIM_RPF_COLORMAP_SUBSECTION = 135,
   OSSIM_RPF_IMAGE_DESCRIPTION_SUBHEADER = 136,
   OSSIM_RPF_IMAGE_DISPLAY_PARAMETERS_SUBHEADER = 137,
   OSSIM_RPF_MASK_SUBSECTION = 138,
   OSSIM_RPF_COLOR_CONVERTER_SUBSECTION = 139,
   OSSIM_RPF_SPATIAL_DATA_SUBSECTION = 140,
   OSSIM_RPF_ATTRIBUTE_SECTION_SUBHEADER = 141,
   OSSIM_RPF_ATTRIBUTE_SUBSECTION = 142,
   OSSIM_RPF_EXPLICIT_AREAL_COVERAGE_TABLE = 143,
   OSSIM_RPF_RELATED_IMAGES_SECTION_SUBHEADER = 144,
   OSSIM_RPF_RELATED_IMAGES_SUBSECTION = 145,
   OSSIM_RPF_REPLACE_UPDATE_SECTION_SUBHEADER = 146,
   OSSIM_RPF_REPLACE_UPDATE_TABLE = 147,
   OSSIM_RPF_BOUNDARY_RECT_SECTION_SUBHEADER = 148,
   OSSIM_RPF_BOUNDARY_RECT_TABLE = 149,
   OSSIM_RPF_FRAME_FILE_INDEX_SECTION_SUBHEADER = 150,
   OSSIM_RPF_FRAME_FILE_INDEX_SUBSECTION = 151,
   OSSIM_RPF_COLOR_TABLE_INDEX_SECTION_SUBHEADER = 152,
   OSSIM_RPF_COLOR_TABLE_INDEX_RECORD = 153
};

#endif
