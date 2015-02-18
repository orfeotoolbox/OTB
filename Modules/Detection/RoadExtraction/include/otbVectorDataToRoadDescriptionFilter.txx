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
#ifndef __otbVectorDataToRoadDescriptionFilter_txx
#define __otbVectorDataToRoadDescriptionFilter_txx

#include "otbVectorDataToRoadDescriptionFilter.h"

namespace otb
{

// Constructor
template <class TVectorData, class TOpticalImage>
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::VectorDataToRoadDescriptionFilter()
{
  this->SetNumberOfRequiredInputs(3);

  m_NDVIFeatureFunction = ParserConditionFeatureFunctionType::New();
  m_NDVIFeatureFunction->SetExpression("ndvi(b3, b4) > 0.4");

  m_SpectralAngleFeatureFunction = ParserConditionFeatureFunctionType::New();
  m_SpectralAngleFeatureFunction->SetExpression("spectralAngle > 0.25");

  //Example for QuickBird images (on a specific image)
  typename ParserConditionFeatureFunctionType::PixelType refPixel;
  refPixel.SetSize(4);
  refPixel[0] = 252;
  refPixel[1] = 357;
  refPixel[2] = 232;
  refPixel[3] = 261;
  m_SpectralAngleFeatureFunction->SetSpectralAngleReferencePixel(refPixel);

  m_DBOverlapFeatureFunction = DBOverlapFeatureFunctionType::New();

  m_DescriptorsList.push_back("NONDVI");
  m_DescriptorsList.push_back("ROADSA");
  m_DescriptorsList.push_back("NOBUIL");
}

template <class TVectorData, class TOpticalImage>
void
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::AddOpticalImage(const OpticalImageType * support)
{
  this->AddSupport(const_cast<OpticalImageType *>(support), 0);
}

template <class TVectorData, class TOpticalImage>
const typename VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::OpticalImageType *
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::GetOpticalImage()
{
  return static_cast<const OpticalImageType *>
    (this->GetSupport(0));
}


template <class TVectorData, class TOpticalImage>
void
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::AddBuildingsDB(const VectorDataType * support)
{
  this->AddSupport(const_cast<VectorDataType *>(support), 1);
}

template <class TVectorData, class TOpticalImage>
const typename VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::VectorDataType *
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::GetBuildingsDB()
{
  return static_cast<const VectorDataType *>
    (this->GetSupport(1));
}

template <class TVectorData, class TOpticalImage>
void
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::GenerateData()
{
  m_NDVIFeatureFunction->SetInputImage(const_cast<OpticalImageType *>(this->GetOpticalImage()));
  m_SpectralAngleFeatureFunction->SetInputImage(const_cast<OpticalImageType *>(this->GetOpticalImage()));
  m_DBOverlapFeatureFunction->SetInputVectorData(const_cast<VectorDataType *>(this->GetBuildingsDB()));

  // Retrieving root node
  typename DataNodeType::Pointer root = this->GetOutput(0)->GetDataTree()->GetRoot()->Get();
  // Create the document node
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(document, root);
  // Create the folder node
  typename DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(folder, document);

  TreeIteratorType itVector(this->GetInput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
      {
      typename DataNodeType::Pointer currentGeometry = itVector.Get();
      currentGeometry->SetFieldAsDouble("NONDVI", (double)(m_NDVIFeatureFunction->Evaluate(*(currentGeometry.GetPointer()))[0]));
      currentGeometry->SetFieldAsDouble("ROADSA", (double)(m_SpectralAngleFeatureFunction->Evaluate(*(currentGeometry.GetPointer()))[0]));
      currentGeometry->SetFieldAsDouble("NOBUIL", (double)(m_DBOverlapFeatureFunction->Evaluate(*(currentGeometry.GetPointer()))[0]));

      this->GetOutput(0)->GetDataTree()->Add(currentGeometry, folder);
      }
    ++itVector;
    }
}

// PrintSelf Method
template <class TVectorData, class TOpticalImage>
void
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
