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
#ifndef __otbListSampleGenerator_txx
#define __otbListSampleGenerator_txx

#include "otbListSampleGenerator.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbRemoteSensingRegion.h"

namespace otb
{
template<class TImage, class TVectorData>
ListSampleGenerator<TImage, TVectorData>
::ListSampleGenerator() :
  m_MaxTrainingSize(-1), 
  m_MaxValidationSize(-1), 
  m_ValidationTrainingProportion(0.0),
  m_NumberOfClasses(0),
  m_ClassKey("Class")
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  //  this->ProcessObject::SetNthOutput( 0, this->MakeOutput(0) );
  //  this->ProcessObject::SetNthOutput( 1, this->MakeOutput(1) );
  m_TrainingListSample = ListSampleType::New();
  m_TrainingListLabel = ListLabelType::New();
  m_ValidationListSample = ListSampleType::New();
  m_ValidationListLabel = ListLabelType::New();
  
  m_RandomGenerator = RandomGeneratorType::GetInstance();
}

template < class TImage, class TVectorData > 
void
ListSampleGenerator<TImage,TVectorData>
::SetInput( const ImageType * image ) 
{
  this->ProcessObject::SetNthInput(0, const_cast< ImageType * >( image ) );
}

template < class TImage, class TVectorData > 
const TImage *
ListSampleGenerator<TImage,TVectorData>
::GetInput( ) const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const ImageType * >(this->ProcessObject::GetInput(0) );
}

template < class TImage, class TVectorData > 
void
ListSampleGenerator<TImage,TVectorData>
::SetInputVectorData( const VectorDataType * vectorData ) 
{
  this->ProcessObject::SetNthInput(1, const_cast< VectorDataType * >( vectorData ) );
}

template < class TImage, class TVectorData > 
const TVectorData *
ListSampleGenerator<TImage,TVectorData>
::GetInputVectorData( ) const
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }

  return static_cast<const VectorDataType * >(this->ProcessObject::GetInput(1) );
}

/**
 *
 */
//template < class TImage, class TVectorData > 
//typename ListSampleGenerator<TImage,TVectorData>::DataObjectPointer
//ListSampleGenerator<TImage,TVectorData>
//::MakeOutput(unsigned int itkNotUsed(idx))
//{
//  typename ListSampleType::Pointer listSample = ListSampleType::New();
//  return static_cast<itk::DataObject*>(listSample);
//}

/**
 *
 */
template < class TImage, class TVectorData > 
void 
ListSampleGenerator<TImage,TVectorData>
::Update()
{
  this->GenerateData();
}

template < class TImage, class TVectorData > 
void
ListSampleGenerator<TImage,TVectorData>
::GenerateData()
{
  typename VectorDataType::ConstPointer vectorData = this->GetInputVectorData();
  std::cout << "******** Number of elements in the tree: " << vectorData->Size() << std::endl;
  
 
  typename ImageType::Pointer image = const_cast<ImageType*>(this->GetInput());
  
  //Gather some information about the relative size of the classes
  //we would like to have the same number of samples per class
  this->GenerateClassStatistics();
  
  this->ComputeClassSelectionProbability();

  
  // Clear the sample lists
  m_TrainingListSample->Clear();
  m_TrainingListLabel->Clear();
  m_ValidationListSample->Clear();
  m_ValidationListLabel->Clear();

  m_RandomGenerator->SetSeed(1234); //FIXME switch to member

  std::map<int, int> classesSamplesNumberTraining; //Just a counter
  std::map<int, int> classesSamplesNumberValidation; //Just a counter


  TreeIteratorType itVector(vectorData->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {

    if (itVector.Get()->IsPolygonFeature())
      {

      typename ImageType::RegionType polygonRegion =
          otb::TransformPhysicalRegionToIndexRegion(itVector.Get()->GetPolygonExteriorRing()->GetBoundingRegion(),
                                                    image.GetPointer());

      image->SetRequestedRegion(polygonRegion);
      image->PropagateRequestedRegion();
      image->UpdateOutputData();

      typedef itk::ImageRegionConstIteratorWithIndex<ImageType> IteratorType;
      IteratorType it(image, polygonRegion);
      it.GoToBegin();
      while (!it.IsAtEnd())
        {
        itk::ContinuousIndex<double, 2> point;
        image->TransformIndexToPhysicalPoint(it.GetIndex(), point);
        if (itVector.Get()->GetPolygonExteriorRing()->IsInside(point))
          {
          double randomValue = m_RandomGenerator->GetUniformVariate(0.0, 1.0);
          if (randomValue < m_ClassesProbTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)])
            {
            //Add the sample to the training list
            m_TrainingListSample->PushBack(it.Get());
            m_TrainingListLabel->PushBack(itVector.Get()->GetFieldAsInt(m_ClassKey));
            classesSamplesNumberTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)] += 1;
            }
          else if (randomValue < m_ClassesProbTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)]
                                + m_ClassesProbValidation[itVector.Get()->GetFieldAsInt(m_ClassKey)])
            {
            //Add the sample to the validation list
            m_ValidationListSample->PushBack(it.Get());
            m_ValidationListLabel->PushBack(itVector.Get()->GetFieldAsInt(m_ClassKey));
            classesSamplesNumberValidation[itVector.Get()->GetFieldAsInt(m_ClassKey)] += 1;
            }
          }
        ++it;
        }
      }
    ++itVector;
    }

  assert(m_TrainingListSample->Size() == m_TrainingListLabel->Size());
  assert(m_ValidationListSample->Size() == m_ValidationListLabel->Size());
  
  std::cout << "Training\n";
  std::cout << "1: " << classesSamplesNumberTraining[1] << "\n";
  std::cout << "2: " << classesSamplesNumberTraining[2] << "\n";
  std::cout << "3: " << classesSamplesNumberTraining[3] << "\n";
  std::cout << "4: " << classesSamplesNumberTraining[4] << "\n";
  std::cout << "5: " << classesSamplesNumberTraining[5] << "\n";
  std::cout << "Validation\n";
  std::cout << "1: " << classesSamplesNumberValidation[1] << "\n";
  std::cout << "2: " << classesSamplesNumberValidation[2] << "\n";
  std::cout << "3: " << classesSamplesNumberValidation[3] << "\n";
  std::cout << "4: " << classesSamplesNumberValidation[4] << "\n";
  std::cout << "5: " << classesSamplesNumberValidation[5] << "\n";

}

template < class TImage, class TVectorData >
void
ListSampleGenerator<TImage,TVectorData>
::GenerateClassStatistics()
{
  //Compute pixel area:
  typename ImageType::Pointer image = const_cast<ImageType*>(this->GetInput());
  double pixelArea = vcl_abs(image->GetSpacing()[0]*image->GetSpacing()[1]);
  
  typename VectorDataType::ConstPointer vectorData = this->GetInputVectorData();
  TreeIteratorType itVector(vectorData->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {
      m_ClassesSize[itVector.Get()->GetFieldAsInt(m_ClassKey)] +=
          itVector.Get()->GetPolygonExteriorRing()->GetArea()/pixelArea;// in pixel
      std::cout << itVector.Get()->GetFieldAsInt(m_ClassKey) << std::endl;
      std::cout << itVector.Get()->GetPolygonExteriorRing()->GetArea()/pixelArea
          << " pixels" << std::endl;
      }
    ++itVector;
    }

  m_NumberOfClasses = m_ClassesSize.size();

}

template < class TImage, class TVectorData >
void
ListSampleGenerator<TImage,TVectorData>
::ComputeClassSelectionProbability()
{
  //Go throught the classes size to find the smallest one
  double minSizeTraining = -1;
  for (std::map<int, double>::iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    std::cout << itmap->first << ": " << itmap->second << std::endl;
    if ((minSizeTraining < 0) || (minSizeTraining > itmap->second))
      {
      minSizeTraining = itmap->second;
      }
    }
  std::cout << "MinSize: " << minSizeTraining << std::endl;

  double minSizeValidation = minSizeTraining;
  
  //Apply the proportion between training and validation samples (all training by default)
  minSizeTraining *= (1.0-m_ValidationTrainingProportion);
  minSizeValidation *= m_ValidationTrainingProportion;

  
  //Apply the limit if specified by the user
  if ((m_MaxTrainingSize != -1) && (m_MaxTrainingSize < minSizeTraining))
    {
    minSizeTraining = m_MaxTrainingSize;
    }
  if ((m_MaxValidationSize != -1) && (m_MaxValidationSize < minSizeValidation))
    {
    minSizeValidation = m_MaxValidationSize;
    }
  
  std::cout << "MinSizeTraining: " << minSizeTraining << std::endl;
  std::cout << "MinSizeValidation: " << minSizeValidation << std::endl;
  
  //Compute the probability selection for each class
  for (std::map<int, double>::iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    m_ClassesProbTraining[itmap->first] = minSizeTraining / itmap->second;
    }
  for (std::map<int, double>::iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    m_ClassesProbValidation[itmap->first] = minSizeValidation / itmap->second;
    }
  
}

template < class TImage, class TVectorData > 
void
ListSampleGenerator<TImage,TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "TODO";
}

}

#endif
