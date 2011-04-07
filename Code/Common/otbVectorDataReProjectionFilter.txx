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
#ifndef __otbVectorDataReProjectionFilter_txx
#define __otbVectorDataReProjectionFilter_txx

#include "otbVectorDataReProjectionFilter.h"

namespace otb
{
/**
   * Constructor
 */
template <class TInputVectorData, class TInputImage>
VectorDataReProjectionFilter<TInputVectorData, TInputImage>
::VectorDataReProjectionFilter() : m_DEMDirectory("")
{
  m_VdExtractFilter = VectorDataExtractROIType::New();
  m_VdProjFilter    = VectorDataProjectionFilterType::New();

  m_VdProjFilter->SetInput(m_VdExtractFilter->GetOutput());
}


/**
   * GenerateData Performs the coordinate conversion for each element in the tree
 */
template <class TInputVectorData, class TInputImage>
void
VectorDataReProjectionFilter<TInputVectorData, TInputImage>
::GenerateData(void)
{
  //this->AllocateOutputs();
  //InputVectorDataPointer  inputPtr = this->GetInput();

  //InputVectorDataPointer outputPtr = this->GetOutput();

  m_VdExtractFilter->SetInput(this->GetInput());

  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::PointType       PointType;

  if (m_InputImage.IsNull())
    {
    itkExceptionMacro("Invalid input image.");
    }

  std::cout << "Spacing of the input image: "<< m_InputImage->GetSpacing() << std::endl;
  std::cout << "Origin of the input image: "<< m_InputImage->GetOrigin() << std::endl;
  std::cout << "Size of the input image: "<< m_InputImage->GetLargestPossibleRegion() << std::endl;
  std::cout << "ProjRef of the input image: "<< m_InputImage->GetProjectionRef() << std::endl;
  std::cout << "ProjRef of the input vector data: "<< this->GetInput()->GetProjectionRef() << std::endl;

  // Get the index of the corner of the image
  IndexType ul, ur, ll, lr;
  PointType pul, pur, pll, plr;
  ul = m_InputImage->GetLargestPossibleRegion().GetIndex();
  ur = ul;
  ll = ul;
  lr = ul;
  ur[0] += m_InputImage->GetLargestPossibleRegion().GetSize()[0];
  lr[0] += m_InputImage->GetLargestPossibleRegion().GetSize()[0];
  lr[1] += m_InputImage->GetLargestPossibleRegion().GetSize()[1];
  ll[1] += m_InputImage->GetLargestPossibleRegion().GetSize()[1];

  std::cout << "bounding box of the input image (pixel): "<< ur << ", " << ul << ", " << lr << ", " << ll << std::endl;

  // Transform to physical point
  m_InputImage->TransformIndexToPhysicalPoint(ul, pul);
  m_InputImage->TransformIndexToPhysicalPoint(ur, pur);
  m_InputImage->TransformIndexToPhysicalPoint(ll, pll);
  m_InputImage->TransformIndexToPhysicalPoint(lr, plr);
  std::cout << "bounding box of the input image (physical): "<< pur << ", " << pul << ", " << plr << ", " << pll << std::endl;

  // Build the cartographic region
  RemoteSensingRegionType rsRegion;
  typename RemoteSensingRegionType::IndexType rsOrigin;
  typename RemoteSensingRegionType::SizeType  rsSize;
  rsOrigin[0] = min(pul[0], plr[0]);
  rsOrigin[1] = min(pul[1], plr[1]);
  rsSize[0] = vcl_abs(pul[0] - plr[0]);
  rsSize[1] = vcl_abs(pul[1] - plr[1]);

  rsRegion.SetOrigin(rsOrigin);
  rsRegion.SetSize(rsSize);
  rsRegion.SetRegionProjection(m_InputImage->GetProjectionRef());
  rsRegion.SetKeywordList(m_InputImage->GetImageKeywordlist());

  std::cout << "remote sensing region origin and size (physical): " << rsOrigin << ", " << rsSize << std::endl;
  std::cout << "remote sensing region ProjRef: " << rsRegion.GetRegionProjection() << std::endl;

  // Set the cartographic region to the extract roi filter
  m_VdExtractFilter->SetRegion(rsRegion);

  if (!m_DEMDirectory.empty())
    {
    m_VdExtractFilter->SetDEMDirectory(m_DEMDirectory);
    }


  // Reproject VectorData in image projection
  m_VdProjFilter->SetInputProjectionRef(this->GetInput()->GetProjectionRef());
  m_VdProjFilter->SetOutputKeywordList(m_InputImage->GetImageKeywordlist());
  m_VdProjFilter->SetOutputProjectionRef(m_InputImage->GetProjectionRef());
  m_VdProjFilter->SetOutputOrigin(m_InputImage->GetOrigin());
  m_VdProjFilter->SetOutputSpacing(m_InputImage->GetSpacing());

  if (!m_DEMDirectory.empty())
    {
    m_VdProjFilter->SetDEMDirectory(m_DEMDirectory);
    }

  m_VdProjFilter->Update();


/*  TreeIteratorType itVector(m_VdProjFilter->GetOutput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {
      std::cout << "----------\n" <<itVector.Get()->GetNodeTypeAsString() << std::endl;
      std::cout << "List of vertices: " << ": " <<std::endl;
      for (unsigned int itPoints = 0; itPoints < itVector.Get()->GetPolygonExteriorRing()->GetVertexList()->Size(); itPoints++)
        {
        std::cout << "vertex[" << itPoints << "]: " << itVector.Get()->GetPolygonExteriorRing()->GetVertexList()->GetElement(itPoints) <<std::endl;
        }
      std::cout << "Polygon region: " << itVector.Get()->GetPolygonExteriorRing()->GetBoundingRegion() <<  std::endl;
      }
    ++itVector;
    }*/

  this->GraftOutput(m_VdProjFilter->GetOutput());
}


} // end namespace otb

#endif
