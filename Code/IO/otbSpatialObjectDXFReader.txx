/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSpatialObjectDXFReader_txx
#define __otbSpatialObjectDXFReader_txx

#include "otbSpatialObjectDXFReader.h"
#include "otbDXFToSpatialObjectGroupFilter.h"
#include "dl_dxf.h"
#include "dl_creationadapter.h"


namespace otb
{
/**
 * Constructor
 */
template <unsigned int VDimension>
SpatialObjectDXFReader<VDimension>
::SpatialObjectDXFReader()
{
  m_FileName = "";
  m_LayerName="";
}
/**
 * Destructor
 */
template <unsigned int VDimension>
SpatialObjectDXFReader<VDimension>
::~SpatialObjectDXFReader()
{}
/**
 * Main computation method
 */
template <unsigned int VDimension>
void
SpatialObjectDXFReader<VDimension>
::GenerateData()
{
  otbMsgDebugMacro(<<"Entering GenerateData()");
  GroupSpatialObjectType * ptr = this->GetOutput();
  typedef otb::DXFToSpatialObjectGroupFilter<VDimension> CreationFilter;
  typename CreationFilter::Pointer creationClass = CreationFilter::New();
  if (m_LayerName.size()>0)
    {
      creationClass->SetLayer(m_LayerName.c_str());
    }
  DL_Dxf dxf;
  dxf.in(m_FileName, creationClass);
  creationClass->GenerateData();
  otbMsgDebugMacro(<<" from inside class: "<<creationClass->GetOutput()->GetNumberOfChildren());
  this->SetNthOutput(0,static_cast<GroupSpatialObjectType *>(creationClass->GetOutput()));
}
} //namespace otb

#endif
