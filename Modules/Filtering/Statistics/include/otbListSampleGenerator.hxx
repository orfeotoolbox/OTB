/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbListSampleGenerator_hxx
#define otbListSampleGenerator_hxx

#include "otbListSampleGenerator.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbRemoteSensingRegion.h"
#include "otbVectorDataProjectionFilter.h"

#include "otbMacro.h"

namespace otb
{

/*template <class TVectorData>
void printVectorData(TVectorData * vectorData, string msg = "")
{
  typedef TVectorData VectorDataType;
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;

  TreeIteratorType itVector(vectorData->GetDataTree());
  itVector.GoToBegin();

  if (!msg.empty())
  {
    std::cout<< msg << std::endl;
  }

  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {
      std::cout << itVector.Get()->GetNodeTypeAsString() << std::endl;
      for (unsigned int itPoints = 0; itPoints < itVector.Get()->GetPolygonExteriorRing()->GetVertexList()->Size(); itPoints++)
        {
        std::cout << "vertex[" << itPoints << "]: " << itVector.Get()->GetPolygonExteriorRing()->GetVertexList()->GetElement(itPoints) <<std::endl;
        }
      std::cout << "Polygon bounding region:\n" << itVector.Get()->GetPolygonExteriorRing()->GetBoundingRegion() <<  std::endl;
      }
    ++itVector;
    }
}*/

template<class TImage, class TVectorData>
ListSampleGenerator<TImage, TVectorData>
::ListSampleGenerator() :
  m_MaxTrainingSize(-1),
  m_MaxValidationSize(-1),
  m_ValidationTrainingProportion(0.0),
  m_BoundByMin(true),
  m_PolygonEdgeInclusion(false),
  m_NumberOfClasses(0),
  m_ClassKey("Class"),
  m_ClassMinSize(-1)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(4);

  // Register the outputs
  this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
  this->itk::ProcessObject::SetNthOutput(3, this->MakeOutput(3).GetPointer());

  m_RandomGenerator = RandomGeneratorType::GetInstance();
}

template <class TImage, class TVectorData>
void
ListSampleGenerator<TImage, TVectorData>
::SetInput(const ImageType * image)
{
  this->ProcessObject::SetNthInput(0, const_cast<ImageType *>(image));
}

template <class TImage, class TVectorData>
const TImage *
ListSampleGenerator<TImage, TVectorData>
::GetInput() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }

  return static_cast<const ImageType *>(this->ProcessObject::GetInput(0));
}

template <class TImage, class TVectorData>
void
ListSampleGenerator<TImage, TVectorData>
::SetInputVectorData(const VectorDataType * vectorData)
{
  this->ProcessObject::SetNthInput(1, const_cast<VectorDataType *>(vectorData));

  //printVectorData(vectorData);

}

template <class TImage, class TVectorData>
const TVectorData *
ListSampleGenerator<TImage, TVectorData>
::GetInputVectorData() const
{
  if (this->GetNumberOfInputs() < 2)
    {
    return nullptr;
    }

  return static_cast<const VectorDataType *>(this->ProcessObject::GetInput(1));
}

template <class TImage, class TVectorData>
typename ListSampleGenerator<TImage, TVectorData>::DataObjectPointer
ListSampleGenerator<TImage, TVectorData>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  DataObjectPointer output;
  switch (idx)
    {
    case 0:
      output = static_cast<itk::DataObject*>(ListSampleType::New().GetPointer());
      break;
    case 1:
      output = static_cast<itk::DataObject*>(ListLabelType::New().GetPointer());
      break;
    case 2:
      output = static_cast<itk::DataObject*>(ListSampleType::New().GetPointer());
      break;
    case 3:
      output = static_cast<itk::DataObject*>(ListLabelType::New().GetPointer());
      break;
    default:
      output = static_cast<itk::DataObject*>(ListSampleType::New().GetPointer());
      break;
    }
  return output;
}
// Get the Training ListSample
template <class TImage, class TVectorData>
typename ListSampleGenerator<TImage, TVectorData>::ListSampleType*
ListSampleGenerator<TImage, TVectorData>
::GetTrainingListSample()
{
  return dynamic_cast<ListSampleType*>(this->itk::ProcessObject::GetOutput(0));
}
// Get the Training label ListSample
template <class TImage, class TVectorData>
typename ListSampleGenerator<TImage, TVectorData>::ListLabelType*
ListSampleGenerator<TImage, TVectorData>
::GetTrainingListLabel()
{
  return dynamic_cast<ListLabelType*>(this->itk::ProcessObject::GetOutput(1));
}

// Get the validation ListSample
template <class TImage, class TVectorData>
typename ListSampleGenerator<TImage, TVectorData>::ListSampleType*
ListSampleGenerator<TImage, TVectorData>
::GetValidationListSample()
{
  return dynamic_cast<ListSampleType*>(this->itk::ProcessObject::GetOutput(2));
}


// Get the validation label ListSample
template <class TImage, class TVectorData>
typename ListSampleGenerator<TImage, TVectorData>::ListLabelType*
ListSampleGenerator<TImage, TVectorData>
::GetValidationListLabel()
{
  return dynamic_cast<ListLabelType*>(this->itk::ProcessObject::GetOutput(3));
}

template <class TImage, class TVectorData>
void
ListSampleGenerator<TImage, TVectorData>
::GenerateInputRequestedRegion()
{
  ImagePointerType img = static_cast<ImageType *>(this->ProcessObject::GetInput(0));

  if(img.IsNotNull())
    {

    // Requested regions will be generated during GenerateData
    // call. For now request an empty region so as to avoid requesting
    // the largest possible region (fixes bug #943 )
    typename ImageType::RegionType dummyRegion;
    typename ImageType::SizeType dummySize;
    dummySize.Fill(0);
    dummyRegion.SetSize(dummySize);
    img->SetRequestedRegion(dummyRegion);
    }
}


template <class TImage, class TVectorData>
void
ListSampleGenerator<TImage, TVectorData>
::GenerateData()
{
  // Get the inputs
  ImagePointerType image = const_cast<ImageType*>(this->GetInput());
  VectorDataPointerType vectorData = const_cast<VectorDataType*>(this->GetInputVectorData());

  // Get the outputs
  ListSamplePointerType trainingListSample   = this->GetTrainingListSample();
  ListLabelPointerType  trainingListLabel    = this->GetTrainingListLabel();
  ListSamplePointerType validationListSample = this->GetValidationListSample();
  ListLabelPointerType  validationListLabel  = this->GetValidationListLabel();

  // Gather some information about the relative size of the classes
  // We would like to have the same number of samples per class
  this->GenerateClassStatistics();

  this->ComputeClassSelectionProbability();

  // Clear the sample lists
  trainingListSample->Clear();
  trainingListLabel->Clear();
  validationListSample->Clear();
  validationListLabel->Clear();

  // Set MeasurementVectorSize for each sample list
  trainingListSample->SetMeasurementVectorSize(image->GetNumberOfComponentsPerPixel());
  // stores label as integers,so put the size to 1
  trainingListLabel->SetMeasurementVectorSize(1);
  validationListSample->SetMeasurementVectorSize(image->GetNumberOfComponentsPerPixel());
  // stores label as integers,so put the size to 1
  validationListLabel->SetMeasurementVectorSize(1);

  m_ClassesSamplesNumberTraining.clear();
  m_ClassesSamplesNumberValidation.clear();

  typename ImageType::RegionType imageLargestRegion = image->GetLargestPossibleRegion();

  TreeIteratorType itVector(vectorData->GetDataTree());
  for (itVector.GoToBegin(); !itVector.IsAtEnd(); ++itVector)
    {
    if (itVector.Get()->IsPolygonFeature())
      {
      PolygonPointerType exteriorRing = itVector.Get()->GetPolygonExteriorRing();

      typename ImageType::RegionType polygonRegion =
        otb::TransformPhysicalRegionToIndexRegion(exteriorRing->GetBoundingRegion(),
                                                  image.GetPointer());

      const bool hasIntersection = polygonRegion.Crop(imageLargestRegion);
      if (!hasIntersection)
      {
        continue;
      }

      image->SetRequestedRegion(polygonRegion);
      image->PropagateRequestedRegion();
      image->UpdateOutputData();

      typedef itk::ImageRegionConstIteratorWithIndex<ImageType> IteratorType;
      IteratorType it(image, polygonRegion);

      for (it.GoToBegin(); !it.IsAtEnd(); ++it)
        {
        itk::ContinuousIndex<double, 2> point;
        image->TransformIndexToPhysicalPoint(it.GetIndex(), point);

        if ( exteriorRing->IsInside(point) ||
             (this->GetPolygonEdgeInclusion() && exteriorRing->IsOnEdge(point)) )
          {
          PolygonListPointerType interiorRings = itVector.Get()->GetPolygonInteriorRings();

          bool isInsideInteriorRing = false;
          for (typename PolygonListType::Iterator interiorRing = interiorRings->Begin();
               interiorRing != interiorRings->End();
               ++interiorRing)
            {
            if ( interiorRing.Get()->IsInside(point)
                 || (this->GetPolygonEdgeInclusion() && interiorRing.Get()->IsOnEdge(point)) )
              {
              isInsideInteriorRing = true;
              break;
              }
            }
          if (isInsideInteriorRing)
            {
            continue; // skip this pixel and continue
            }

          double randomValue = m_RandomGenerator->GetUniformVariate(0.0, 1.0);
          if (randomValue < m_ClassesProbTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)])
            {
            //Add the sample to the training list
            trainingListSample->PushBack(it.Get());
            trainingListLabel->PushBack(itVector.Get()->GetFieldAsInt(m_ClassKey));
            m_ClassesSamplesNumberTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)] += 1;
            }
          else if (randomValue < m_ClassesProbTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)]
                   + m_ClassesProbValidation[itVector.Get()->GetFieldAsInt(m_ClassKey)])
            {
            //Add the sample to the validation list
            validationListSample->PushBack(it.Get());
            validationListLabel->PushBack(itVector.Get()->GetFieldAsInt(m_ClassKey));
            m_ClassesSamplesNumberValidation[itVector.Get()->GetFieldAsInt(m_ClassKey)] += 1;
            }
          //Note: some samples may not be used at all
          }
        }
      }
    }

  assert(trainingListSample->Size() == trainingListLabel->Size());
  assert(validationListSample->Size() == validationListLabel->Size());
  this->UpdateProgress(1.0f);
}

template <class TImage, class TVectorData>
void
ListSampleGenerator<TImage, TVectorData>
::GenerateClassStatistics()
{
  m_ClassesSize.clear();

  ImageType* image = const_cast<ImageType*> (this->GetInput());
  typename VectorDataType::ConstPointer vectorData = this->GetInputVectorData();

  // Compute cumulative area of all polygons of each class
  TreeIteratorType itVector(vectorData->GetDataTree());
  for (itVector.GoToBegin(); !itVector.IsAtEnd(); ++itVector)
    {
    DataNodeType* datanode = itVector.Get();
    if (datanode->IsPolygonFeature())
      {
      double area = GetPolygonAreaInPixelsUnits(datanode, image);
      m_ClassesSize[datanode->GetFieldAsInt(m_ClassKey)] += area;
      }
    }
  m_NumberOfClasses = m_ClassesSize.size();
}

template <class TImage, class TVectorData>
void
ListSampleGenerator<TImage, TVectorData>
::ComputeClassSelectionProbability()
{
  m_ClassesProbTraining.clear();
  m_ClassesProbValidation.clear();

  // Sanity check
  if (m_ClassesSize.empty())
    {
    itkGenericExceptionMacro(<< "No training sample found inside image");
    }

  // Go through the classes size to find the smallest one
  double minSize = itk::NumericTraits<double>::max();
  for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesSize.begin();
        itmap != m_ClassesSize.end();
        ++itmap)
    {
    if (minSize > itmap->second)
      {
      minSize = itmap->second;
      }
    }

  // Apply the proportion between training and validation samples (all training by default)
  double minSizeTraining   = minSize * (1.0 - m_ValidationTrainingProportion);
  double minSizeValidation = minSize * m_ValidationTrainingProportion;

  // Apply the limit if specified by the user
  if(m_BoundByMin)
    {
    if ((m_MaxTrainingSize != -1) && (m_MaxTrainingSize < minSizeTraining))
      {
      minSizeTraining = m_MaxTrainingSize;
      }
    if ((m_MaxValidationSize != -1) && (m_MaxValidationSize < minSizeValidation))
      {
      minSizeValidation = m_MaxValidationSize;
      }
    }
  // Compute the probability selection for each class
  for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesSize.begin();
       itmap != m_ClassesSize.end();
       ++itmap)
    {
    if (m_BoundByMin)
      {
      m_ClassesProbTraining[itmap->first] = minSizeTraining / itmap->second;
      m_ClassesProbValidation[itmap->first] = minSizeValidation / itmap->second;
      }
    else
      {
      long int maxSizeT = (itmap->second)*(1.0 - m_ValidationTrainingProportion);
      long int maxSizeV = (itmap->second)*m_ValidationTrainingProportion;

      // Check if max sizes respect the maximum bounds
      double correctionRatioTrain = 1.0;
      if((m_MaxTrainingSize > -1) && (m_MaxTrainingSize < maxSizeT))
        {
        correctionRatioTrain = (double)(m_MaxTrainingSize) / (double)(maxSizeT);
        }
      double correctionRatioValid = 1.0;
      if((m_MaxValidationSize > -1) && (m_MaxValidationSize < maxSizeV))
        {
        correctionRatioValid = (double)(m_MaxValidationSize) / (double)(maxSizeV);
        }
      double correctionRatio = std::min(correctionRatioTrain,correctionRatioValid);
      m_ClassesProbTraining[itmap->first] = correctionRatio*(1.0 - m_ValidationTrainingProportion);
      m_ClassesProbValidation[itmap->first] = correctionRatio*m_ValidationTrainingProportion;
      }
    }
}
template <class TImage, class TVectorData>
double
ListSampleGenerator<TImage, TVectorData>
::GetPolygonAreaInPixelsUnits(DataNodeType* polygonDataNode, ImageType* image)
{
  const double pixelArea = vcl_abs(image->GetSignedSpacing()[0] * image->GetSignedSpacing()[1]);

  // Compute area of exterior ring in pixels
  PolygonPointerType exteriorRing = polygonDataNode->GetPolygonExteriorRing();
  double area = exteriorRing->GetArea() / pixelArea;

  // Remove contribution of all interior rings
  PolygonListPointerType interiorRings = polygonDataNode->GetPolygonInteriorRings();
  for (typename PolygonListType::Iterator interiorRing = interiorRings->Begin();
       interiorRing != interiorRings->End();
       ++interiorRing)
    {
    area -= interiorRing.Get()->GetArea() / pixelArea;
    }

  return area;
}

template <class TImage, class TVectorData>
void
ListSampleGenerator<TImage, TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "* MaxTrainingSize: " << m_MaxTrainingSize << "\n";
  os << indent << "* MaxValidationSize: " << m_MaxValidationSize << "\n";
  os << indent << "* Proportion: " << m_ValidationTrainingProportion << "\n";
  os << indent << "* Input data:\n";
  if (m_ClassesSize.empty())
    {
    os << indent << "Empty\n";
    }
  else
    {
    for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesSize.begin();
         itmap != m_ClassesSize.end(); ++itmap)
      {
      os << indent << itmap->first << ": " << itmap->second << "\n";
      }
    }

  os << "\n" << indent << "* Training set:\n";
  if (m_ClassesProbTraining.empty())
    {
    os << indent << "Not computed\n";
    }
  else
    {
    os << indent << "** Selection probability:\n";
    for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesProbTraining.begin();
         itmap != m_ClassesProbTraining.end(); ++itmap)
      {
      os << indent << itmap->first << ": " << itmap->second << "\n";
      }
    os << indent << "** Number of selected samples:\n";
    for (std::map<ClassLabelType, int>::const_iterator itmap = m_ClassesSamplesNumberTraining.begin();
         itmap != m_ClassesSamplesNumberTraining.end(); ++itmap)
      {
      os << indent << itmap->first << ": " << itmap->second << "\n";
      }
    }

  os << "\n" << indent << "* Validation set:\n";
  if (m_ClassesProbValidation.empty())
    {
    os << indent << "Not computed\n";
    }
  else
    {
    os << indent << "** Selection probability:\n";
    for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesProbValidation.begin();
         itmap != m_ClassesProbValidation.end(); ++itmap)
      {
      os << indent << itmap->first << ": " << itmap->second << "\n";
      }
    os << indent << "** Number of selected samples:\n";
    for (std::map<ClassLabelType, int>::const_iterator itmap = m_ClassesSamplesNumberValidation.begin();
         itmap != m_ClassesSamplesNumberValidation.end(); ++itmap)
      {
      os << indent << itmap->first << ": " << itmap->second << "\n";
      }
    }
}

}

#endif
