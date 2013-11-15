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
#ifndef __otbDescriptorsListSampleGenerator_txx
#define __otbDescriptorsListSampleGenerator_txx

#include "otbDescriptorsListSampleGenerator.h"

#include "itkContinuousIndex.h"

namespace otb
{

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::PersistentDescriptorsListSampleGenerator()
  : m_NeighborhoodRadius(0)
{
  // Need 2 inputs : a vector image and a vectordata
  this->SetNumberOfRequiredInputs(2);

  // Have 4 outputs : the image created by Superclass,
  // the sample list, the label sample list, the positions of the samples
  this->SetNumberOfRequiredOutputs(3);
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
  this->itk::ProcessObject::SetNthOutput(3, this->MakeOutput(3).GetPointer());
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::~PersistentDescriptorsListSampleGenerator()
{
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

    if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
      {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
      }
    }
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::SetSamplesLocations(VectorDataType* location)
{
  this->SetNthInput(1, location);
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
typename PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>::VectorDataType*
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::GetSamplesLocations()
{

  return static_cast<VectorDataType*>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
typename PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>::ListSampleType*
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::GetListSample()
{
  return dynamic_cast<ListSampleType*>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
typename PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>::LabelListSampleType*
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::GetLabelListSample()
{
  return dynamic_cast<LabelListSampleType*>( this->itk::ProcessObject::GetOutput(2) );
}


template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
typename PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>::SamplesPositionType&
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::GetSamplesPositions()
{
  return this->GetSamplesPositionsObject()->Get();
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
typename PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>::SamplesPositionObjectType*
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::GetSamplesPositionsObject()
{
  return dynamic_cast<SamplesPositionObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
itk::DataObject::Pointer
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::MakeOutput(unsigned int idx)
{
  itk::DataObject::Pointer output;
  switch (idx)
    {
    case 0:
      output = static_cast<itk::DataObject*>(InputImageType::New().GetPointer());
      break;
    case 1:
      {
      ListSamplePointerType listSample = ListSampleType::New();
      output = static_cast<itk::DataObject*>(listSample.GetPointer());
      break;
      }
    case 2:
      {
      LabelListSamplePointerType labelListSample = LabelListSampleType::New();
      output = static_cast<itk::DataObject*>(labelListSample.GetPointer());
      break;
      }
    case 3:
      {
      SamplesPositionObjectPointerType samplesPositions = SamplesPositionObjectType::New();
      output = static_cast<itk::DataObject*>(samplesPositions.GetPointer());
      break;
      }
    default:
      output = static_cast<itk::DataObject*>(InputImageType::New().GetPointer());
      break;
    }
  return output;
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::Reset()
{
  m_ThreadListSample = ListSampleArray(this->GetNumberOfThreads());
  for (unsigned int i = 0; i < this->GetNumberOfThreads(); ++i)
    {
    m_ThreadListSample[i] = ListSampleType::New();
    }

  m_ThreadLabelListSample = LabelListSampleArray(this->GetNumberOfThreads());
  for (unsigned int i = 0; i < this->GetNumberOfThreads(); ++i)
    {
    m_ThreadLabelListSample[i] = LabelListSampleType::New();
    }

  m_ThreadSamplesPosition = SamplesPositionArray(this->GetNumberOfThreads());

  this->GetListSample()->Clear();
  this->GetLabelListSample()->Clear();
  this->GetSamplesPositions().clear();
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::Synthetize()
{
  // Merge the ListSample from all the threads
  ListSampleType* listSample = this->GetListSample();
  LabelListSampleType* labelListSample = this->GetLabelListSample();
  SamplesPositionType& samplesPosition = this->GetSamplesPositions();
  VectorDataPointType ref;
  
  // Get the output list measurement vector sample sizes once
  listSample->SetMeasurementVectorSize(m_ThreadListSample[0]->GetMeasurementVectorSize());
  labelListSample->SetMeasurementVectorSize(m_ThreadLabelListSample[0]->GetMeasurementVectorSize());

  // Copy the first thread elements into lists
  if( this->GetNumberOfThreads() > 1 )
    {
      ListSampleType* threadListSample = m_ThreadListSample[0];
      LabelListSampleType* threadLabelListSample = m_ThreadLabelListSample[0];
      SamplesPositionType& threadSamplesPosition = m_ThreadSamplesPosition[0];
      
      for (unsigned int i = 0; i < threadListSample->Size(); ++i)
      {
        listSample->PushBack( threadListSample->GetMeasurementVector(i) );
        labelListSample->PushBack( threadLabelListSample->GetMeasurementVector(i) );
        samplesPosition.push_back( threadSamplesPosition[i] );
      }
    }

  // Add the other thread element checking if the point dosn't already exist
  for (itk::ThreadIdType threadId = 1; threadId < this->GetNumberOfThreads(); ++threadId )
    {
    ListSampleType* threadListSample = m_ThreadListSample[threadId];
    LabelListSampleType* threadLabelListSample = m_ThreadLabelListSample[threadId];
    SamplesPositionType& threadSamplesPosition = m_ThreadSamplesPosition[threadId];

    for (unsigned int i = 0; i < threadListSample->Size(); ++i)
      {
        const DescriptorsFunctionPointType & curSamplesPosition = threadSamplesPosition[i];
        if( std::find(samplesPosition.begin(), samplesPosition.end(), curSamplesPosition) == samplesPosition.end() )
          {
            listSample->PushBack( threadListSample->GetMeasurementVector(i) );
            labelListSample->PushBack( threadLabelListSample->GetMeasurementVector(i) );
            samplesPosition.push_back( curSamplesPosition );
          }
      }
    }
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr =
    const_cast< TInputImage * >( this->GetInput() );
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_NeighborhoodRadius + 5 );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    }
}

template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::BeforeThreadedGenerateData()
{}


template <class TInputImage, class TVectorData, class TFunctionType, class TListSample, class TLabelListSample>
void
PersistentDescriptorsListSampleGenerator<TInputImage, TVectorData, TFunctionType, TListSample, TLabelListSample>
::ThreadedGenerateData(const RegionType&  outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  ListSampleType* listSample = m_ThreadListSample[threadId];
  LabelListSampleType* labelListSample = m_ThreadLabelListSample[threadId];
  SamplesPositionType& samplesPosition = m_ThreadSamplesPosition[threadId];
  
  VectorDataTreeIteratorType vectorDataIt(this->GetSamplesLocations()->GetDataTree());
  bool isInitialized = false;
  
  for (vectorDataIt.GoToBegin(); !vectorDataIt.IsAtEnd(); ++vectorDataIt)
    {
    if (vectorDataIt.Get()->IsPointFeature())
      {
      VectorDataPointType point = vectorDataIt.Get()->GetPoint();
      ContinuousIndexType cidx;
      this->GetInput()->TransformPhysicalPointToContinuousIndex(point, cidx);

      // OGR convention : vector data are recorded with a 0.5 shift
      cidx[0] -= 0.5;
      cidx[1] -= 0.5;

      RegionType paddedRegion = outputRegionForThread;
      paddedRegion.PadByRadius(m_NeighborhoodRadius);
      if (this->IsInsideWithNeighborhoodRadius(paddedRegion, cidx))
        {
        // Set the Measurement Vector Size of the samplelists, once
        if(!isInitialized)
          {
          // output list sample
          listSample->SetMeasurementVectorSize(m_DescriptorsFunction->Evaluate(point).Size());
          labelListSample->SetMeasurementVectorSize(LabelMeasurementVectorType::Length);
          isInitialized = true;
          }

        SampleMeasurementVectorType sample(m_DescriptorsFunction->Evaluate(point));
        listSample->PushBack( sample );

        LabelMeasurementVectorType label;
        label[0] = static_cast<LabelMeasurementType>(vectorDataIt.Get()->GetFieldAsInt("Class"));
        labelListSample->PushBack( label );

        samplesPosition.push_back(point);
        }
      }
    }
}


template <class TInputImage, class TVectorData, class TListSample, class TLabelListSample, class TOutputPrecision, class TCoordRep>
DescriptorsListSampleGenerator<TInputImage, TVectorData, TListSample, TLabelListSample, TOutputPrecision, TCoordRep>
::DescriptorsListSampleGenerator()
{
  

}

template <class TInputImage, class TVectorData, class TListSample, class TLabelListSample, class TOutputPrecision, class TCoordRep>
DescriptorsListSampleGenerator<TInputImage, TVectorData, TListSample, TLabelListSample, TOutputPrecision, TCoordRep>
::~DescriptorsListSampleGenerator()
{

}


} // end namespace otb

#endif
