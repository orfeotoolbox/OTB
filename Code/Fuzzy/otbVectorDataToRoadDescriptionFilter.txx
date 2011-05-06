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
template <class TVectorData, class TOpticalImage, class TNDVIImage>
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage, TNDVIImage>
::VectorDataToRoadDescriptionFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_NDVIFeatureFunction = NDVIFeatureFunctionType::New();
  m_SpectralAngleFeatureFunction = SpectralAngleFeatureFunctionType::New();
}

template <class TVectorData, class TOpticalImage, class TNDVIImage>
void
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage, TNDVIImage>
::AddOpticalImage(const OpticalImageType * support)
{
  this->AddSupport(const_cast<OpticalImageType *>(support), 0);
}

template <class TVectorData, class TOpticalImage, class TNDVIImage>
const typename VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage, TNDVIImage>
::OpticalImageType *
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage, TNDVIImage>
::GetOpticalImage()
{
  return static_cast<const OpticalImageType *>
    (this->GetSupport(0));
}

template <class TVectorData, class TOpticalImage, class TNDVIImage>
void
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage, TNDVIImage>
::GenerateData()
{
  m_NDVIFeatureFunction->SetInputImage(const_cast<OpticalImageType *>(this->GetOpticalImage()));
  m_SpectralAngleFeatureFunction->SetInputImage(const_cast<OpticalImageType *>(this->GetOpticalImage()));

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
  this->GetOutput(0)->SetProjectionRef(this->GetInput()->GetProjectionRef());

  TreeIteratorType itVector(this->GetInput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
      {
      typename DataNodeType::Pointer currentGeometry = itVector.Get();
      currentGeometry->SetFieldAsDouble("NDVI", (double)(m_NDVIFeatureFunction->Evaluate(*(currentGeometry.GetPointer()))[0]));
      currentGeometry->SetFieldAsDouble("RADIOM", (double)(m_SpectralAngleFeatureFunction->Evaluate(*(currentGeometry.GetPointer()))[0]));
      //currentGeometry->SetFieldAsInt("LSD",  80);
      //currentGeometry->SetFieldAsInt("SHADOW", 80);
      
      this->GetOutput(0)->GetDataTree()->Add(currentGeometry, folder);
      }
    ++itVector;
    }
}

// PrintSelf Method
template <class TVectorData, class TOpticalImage, class TNDVIImage>
void
VectorDataToRoadDescriptionFilter<TVectorData, TOpticalImage, TNDVIImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
