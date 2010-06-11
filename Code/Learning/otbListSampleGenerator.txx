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
  
  m_ClassesSamplesNumberTraining.clear();
  m_ClassesSamplesNumberValidation.clear();

  m_RandomGenerator->SetSeed(1234); //FIXME switch to member

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
            m_ClassesSamplesNumberTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)] += 1;
            }
          else if (randomValue < m_ClassesProbTraining[itVector.Get()->GetFieldAsInt(m_ClassKey)]
                                + m_ClassesProbValidation[itVector.Get()->GetFieldAsInt(m_ClassKey)])
            {
            //Add the sample to the validation list
            m_ValidationListSample->PushBack(it.Get());
            m_ValidationListLabel->PushBack(itVector.Get()->GetFieldAsInt(m_ClassKey));
            m_ClassesSamplesNumberValidation[itVector.Get()->GetFieldAsInt(m_ClassKey)] += 1;
            }
            //Note: some samples may not be used at all
          }
        ++it;
        }
      }
    ++itVector;
    }

  assert(m_TrainingListSample->Size() == m_TrainingListLabel->Size());
  assert(m_ValidationListSample->Size() == m_ValidationListLabel->Size());
}

template < class TImage, class TVectorData >
void
ListSampleGenerator<TImage,TVectorData>
::GenerateClassStatistics()
{
  m_ClassesSize.clear();
  
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
  m_ClassesProbTraining.clear();
  m_ClassesProbValidation.clear();
  
  //Go throught the classes size to find the smallest one
  double minSizeTraining = -1;
  for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    if ((minSizeTraining < 0) || (minSizeTraining > itmap->second))
      {
      minSizeTraining = itmap->second;
      }
    }

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
  
  //Compute the probability selection for each class
  for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    m_ClassesProbTraining[itmap->first] = minSizeTraining / itmap->second;
    }
  for (std::map<ClassLabelType, double>::const_iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    m_ClassesProbValidation[itmap->first] = minSizeValidation / itmap->second;
    }
  
}

template < class TImage, class TVectorData > 
void
ListSampleGenerator<TImage,TVectorData>
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
  
  os  << "\n" << indent << "* Validation set:\n";
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
