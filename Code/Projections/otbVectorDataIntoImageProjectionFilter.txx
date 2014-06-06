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
#ifndef __otbVectorDataIntoImageProjectionFilter_txx
#define __otbVectorDataIntoImageProjectionFilter_txx

#include "otbVectorDataIntoImageProjectionFilter.h"
#include "boost/algorithm/string.hpp"

namespace otb
{
/**
   * Constructor
 */
template <class TInputVectorData, class TInputImage>
VectorDataIntoImageProjectionFilter<TInputVectorData, TInputImage>
::VectorDataIntoImageProjectionFilter()
{
  m_OutputSpacing.Fill(1);
  m_OutputOrigin.Fill(0);
  m_UseOutputSpacingAndOriginFromImage = false;

  m_VdExtractFilter = VectorDataExtractROIType::New();
  m_VdProjFilter    = VectorDataProjectionFilterType::New();

  m_VdProjFilter->SetInput(m_VdExtractFilter->GetOutput());
}

template <class TInputVectorData, class TInputImage>
void
VectorDataIntoImageProjectionFilter<TInputVectorData, TInputImage>
::SetUseOutputSpacingAndOriginFromImage(bool flag)
{
  m_UseOutputSpacingAndOriginFromImage = flag;
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataIntoImageProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputSpacing(const SpacingType& spacing)
{
  itkDebugMacro("setting Spacing to " << spacing);
  if (this->m_OutputSpacing != spacing)
    {
    this->m_OutputSpacing = spacing;
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataIntoImageProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputSpacing(const double spacing[2])
{
  SpacingType s(spacing);
  this->SetOutputSpacing(s);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataIntoImageProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom(sf);
  this->SetOutputSpacing(s);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataIntoImageProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputOrigin(const double origin[2])
{
  OriginType p(origin);
  this->SetOutputOrigin(p);
}

//----------------------------------------------------------------------------
template <class TInputVectorData, class TOutputVectorData>
void
VectorDataIntoImageProjectionFilter<TInputVectorData, TOutputVectorData>
::SetOutputOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom(of);
  this->SetOutputOrigin(p);
}


/**
   * GenerateData Performs the coordinate conversion for each element in the tree
 */
template <class TInputVectorData, class TInputImage>
void
VectorDataIntoImageProjectionFilter<TInputVectorData, TInputImage>
::GenerateData(void)
{
  m_VdExtractFilter->SetInput(this->GetInput());

  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::PointType       PointType;
  typedef typename ImageType::SizeType       SizeType;

  if (m_InputImage.IsNull())
    {
    itkExceptionMacro("Invalid input image.");
    }

  /*std::cout << "Spacing of the input image: "<< m_InputImage->GetSpacing() << std::endl;
  std::cout << "Origin of the input image: "<< m_InputImage->GetOrigin() << std::endl;
  std::cout << "Size of the input image: "<< m_InputImage->GetLargestPossibleRegion() << std::endl;
  std::cout << "ProjRef of the input image: "<< m_InputImage->GetProjectionRef() << std::endl;
  std::cout << "ProjRef of the input vector data: "<< this->GetInput()->GetProjectionRef() << std::endl; */

  // Get the index of the corner of the image
  PointType pul, pur, pll, plr;
  itk::ContinuousIndex<double,2> ul(m_InputImage->GetLargestPossibleRegion().GetIndex());
  ul[0] += -0.5;
  ul[1] += -0.5;

  itk::ContinuousIndex<double,2> ur(ul);
  itk::ContinuousIndex<double,2> ll(ul);
  itk::ContinuousIndex<double,2> lr(ul);

  SizeType size = m_InputImage->GetLargestPossibleRegion().GetSize();
  ur[0] += size[0];
  lr[0] += size[0];
  lr[1] += size[1];
  ll[1] += size[1];

  //std::cout << "bounding box of the input image (pixel): "<< ur << ", " << ul << ", " << lr << ", " << ll << std::endl;

  // Transform to physical point
  m_InputImage->TransformContinuousIndexToPhysicalPoint(ul, pul);
  m_InputImage->TransformContinuousIndexToPhysicalPoint(ur, pur);
  m_InputImage->TransformContinuousIndexToPhysicalPoint(ll, pll);
  m_InputImage->TransformContinuousIndexToPhysicalPoint(lr, plr);
  //std::cout << "bounding box of the input image (physical): "<< pur << ", " << pul << ", " << plr << ", " << pll << std::endl;

  // Build the cartographic region
  RemoteSensingRegionType rsRegion;
  typename RemoteSensingRegionType::IndexType rsOrigin;
  typename RemoteSensingRegionType::SizeType  rsSize;
  rsOrigin[0] = std::min(pul[0], plr[0]);
  rsOrigin[1] = std::min(pul[1], plr[1]);
  rsSize[0] = vcl_abs(pul[0] - plr[0]);
  rsSize[1] = vcl_abs(pul[1] - plr[1]);

  rsRegion.SetOrigin(rsOrigin);
  rsRegion.SetSize(rsSize);

  if (m_InputImage->GetProjectionRef().empty()
      || boost::algorithm::istarts_with(m_InputImage->GetProjectionRef(), "LOCAL_CS") )
    {
    rsRegion.SetKeywordList(m_InputImage->GetImageKeywordlist());
    }
  else
    {
    rsRegion.SetRegionProjection(m_InputImage->GetProjectionRef());
    }

  //std::cout << "remote sensing region origin and size (physical): " << rsOrigin << ", " << rsSize << std::endl;
  //std::cout << "remote sensing region ProjRef: " << rsRegion.GetRegionProjection() << std::endl;

  // Set the cartographic region to the extract roi filter
  m_VdExtractFilter->SetRegion(rsRegion);

  // Reproject VectorData in image projection
  m_VdProjFilter->SetInputProjectionRef(this->GetInput()->GetProjectionRef());

  if (m_InputImage->GetProjectionRef().empty()
      || boost::algorithm::istarts_with(m_InputImage->GetProjectionRef(), "LOCAL_CS") )
    {
    m_VdProjFilter->SetOutputKeywordList(m_InputImage->GetImageKeywordlist());
    }
  else
    {
    m_VdProjFilter->SetOutputProjectionRef(m_InputImage->GetProjectionRef());
    }

  if (m_UseOutputSpacingAndOriginFromImage)
    {
    m_VdProjFilter->SetOutputOrigin(m_InputImage->GetOrigin());
    m_VdProjFilter->SetOutputSpacing(m_InputImage->GetSpacing());
    }
  else
    {
    m_VdProjFilter->SetOutputOrigin(this->GetOutputOrigin());
    m_VdProjFilter->SetOutputSpacing(this->GetOutputSpacing());
    }

  m_VdProjFilter->Update();

  /*
  TreeIteratorType itVector(m_VdProjFilter->GetOutput()->GetDataTree());
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
    }
  */

  if (m_UseOutputSpacingAndOriginFromImage)
    {
    m_VdProjFilter->GetOutput()->SetProjectionRef("");
    }

  this->GraftOutput(m_VdProjFilter->GetOutput());
}


} // end namespace otb

#endif
