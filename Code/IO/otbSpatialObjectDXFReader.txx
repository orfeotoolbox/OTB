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
template <class TSpatialObject>
SpatialObjectDXFReader<TSpatialObject>
::SpatialObjectDXFReader()
{
  m_FileName = "";
  m_LayerName="";
  // this->itk::ProcessObject::ReleaseDataBeforeUpdateFlagOff();
  this->GetOutput()->ReleaseData();
  this->ResetPipeline();
}
/**
 * Destructor
 */
template <class TSpatialObject>
SpatialObjectDXFReader<TSpatialObject>
::~SpatialObjectDXFReader()
{}

template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
::SetFileName(char * filename)
{
  m_FileName = filename;
  this->Modified();
}
/**
 * Main computation method
 */
template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
::GenerateData()
{
  std::cout<<"Entering GenerateData()"<<std::endl;
  GroupSpatialObjectType * ptr = this->GetOutput();
  typedef otb::DXFToSpatialObjectGroupFilter<TSpatialObject> CreationFilter;
  typename CreationFilter::Pointer creationClass = CreationFilter::New();
  if (m_LayerName.size()>0)
    {
      creationClass->SetLayer(m_LayerName.c_str());
    }
  DL_Dxf dxf;
  dxf.in(m_FileName, creationClass);
  GroupSpatialObjectType * group = creationClass->GetOutput();
  group->Update();
  otbMsgDebugMacro(<<" from inside class: "<<group->GetNumberOfChildren());
  this->SetNthOutput(0,group);
}

template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
::Update(void)
{
  this->GenerateData();
}

} //namespace otb

#endif
