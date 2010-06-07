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

#include "itkPreOrderTreeIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "otbRemoteSensingRegion.h"

namespace otb
{
template<class TImage, class TVectorData>
ListSampleGenerator<TImage, TVectorData>
::ListSampleGenerator() :
  m_MaxTrainingSize(-1), 
  m_MaxValidationSize(-1), 
  m_ValidationTrainingRatio(0.0),//FIXME not used yet
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

template < class TImage, class TVectorData >
void
ListSampleGenerator<TImage,TVectorData>
::SetValidationVectorData( const VectorDataType * vectorData )
{
  this->ProcessObject::SetNthInput(2, const_cast< VectorDataType * >( vectorData ) );
}

template < class TImage, class TVectorData >
const TVectorData *
ListSampleGenerator<TImage,TVectorData>
::GetValidationVectorData( ) const
{
  if (this->GetNumberOfInputs() < 3)
    {
    return 0;
    }

  return static_cast<const VectorDataType * >(this->ProcessObject::GetInput(2) );
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


  
  double minSize = -1;
  for (std::map<int, double>::iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    std::cout << itmap->first << ": " << itmap->second << std::endl;
    if ((minSize < 0) || (minSize > itmap->second))
      {
      minSize = itmap->second;
      }
    }
   std::cout << "MinSize: " << minSize << std::endl;

  //Compute the probability selection for each class
  std::map<int, double> classesProb;
  for (std::map<int, double>::iterator itmap = m_ClassesSize.begin(); itmap != m_ClassesSize.end(); ++itmap)
    {
    classesProb[itmap->first] = minSize/itmap->second;
    }

  m_TrainingListSample->Clear();
  m_TrainingListLabel->Clear();
  m_ValidationListSample->Clear();
  m_ValidationListLabel->Clear();

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGenType;
  RandomGenType::Pointer randomGen = RandomGenType::GetInstance();
  randomGen->SetSeed(1234); //FIXME switch to member

  std::map<int, int> classesSamplesNumber;

  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;
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
      IteratorType it(image,polygonRegion);
      it.GoToBegin();
      while (!it.IsAtEnd())
        {
        itk::ContinuousIndex<double,2 > point;
        image->TransformIndexToPhysicalPoint(it.GetIndex(), point);
        if ( itVector.Get()->GetPolygonExteriorRing()->IsInside(point)
           && (randomGen->GetUniformVariate(0.0,1.0)) < classesProb[itVector.Get()->GetFieldAsInt(m_ClassKey)])
          {
          m_TrainingListSample->PushBack(it.Get());
          m_TrainingListLabel->PushBack(itVector.Get()->GetFieldAsInt(m_ClassKey));
          classesSamplesNumber[itVector.Get()->GetFieldAsInt(m_ClassKey)] += 1;
          }
        ++it;
        }
      }
    ++itVector;
    }

  std::cout << "1: " << classesSamplesNumber[1] << std::endl;
  std::cout << "2: " << classesSamplesNumber[2] << std::endl;
  std::cout << "3: " << classesSamplesNumber[3] << std::endl;
  std::cout << "4: " << classesSamplesNumber[4] << std::endl;
  std::cout << "5: " << classesSamplesNumber[5] << std::endl;


}

template < class TImage, class TVectorData >
void
ListSampleGenerator<TImage,TVectorData>
::GenerateClassStatistics()
{
  typename VectorDataType::ConstPointer vectorData = this->GetInputVectorData();
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;
  TreeIteratorType itVector(vectorData->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {
      m_ClassesSize[itVector.Get()->GetFieldAsInt(m_ClassKey)] +=
          itVector.Get()->GetPolygonExteriorRing()->GetArea()/1000000.;// in km2
      std::cout << itVector.Get()->GetFieldAsInt(m_ClassKey) << std::endl;
      std::cout << itVector.Get()->GetPolygonExteriorRing()->GetArea()/1000000.
          << " km2" << std::endl;
      }
    ++itVector;
    }

  m_NumberOfClasses = m_ClassesSize.size();
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
