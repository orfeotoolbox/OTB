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

#ifndef __otbMeanShiftImageFilter_txx
#define __otbMeanShiftImageFilter_txx

#include "otbMeanShiftImageFilter.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

#include "msImageProcessor.h"

namespace otb
{
template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::MeanShiftImageFilter()
{
  m_SpatialRadius      = 3;
  m_RangeRadius        = 10;
  m_MinimumRegionSize  = 10;
  m_Scale              = 100000.;

  this->SetNumberOfOutputs(4);
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, LabeledOutputType::New());
  this->SetNthOutput(3, LabeledOutputType::New());
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
const typename MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>::OutputImageType *
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::GetClusteredOutput() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
typename MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>::OutputImageType *
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::GetClusteredOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
const typename MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>::LabeledOutputType *
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::GetLabeledClusteredOutput() const
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<const LabeledOutputType *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
typename MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>::LabeledOutputType *
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::GetLabeledClusteredOutput()
{
  if (this->GetNumberOfOutputs() < 3)
    {
    return 0;
    }
  return static_cast<LabeledOutputType *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
const typename MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>::LabeledOutputType *
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::GetClusterBoundariesOutput() const
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  return static_cast<const LabeledOutputType *>(this->itk::ProcessObject::GetOutput(3));
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
typename MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>::LabeledOutputType *
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::GetClusterBoundariesOutput()
{
  if (this->GetNumberOfOutputs() < 4)
    {
    return 0;
    }
  return static_cast<LabeledOutputType *>(this->itk::ProcessObject::GetOutput(3));
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
void
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::AllocateOutputs()
{
  typename OutputImageType::Pointer   outputPtr = this->GetOutput();
  typename OutputImageType::Pointer   clusteredOutputPtr = this->GetClusteredOutput();
  typename LabeledOutputType::Pointer labeledClusteredOutputPtr = this->GetLabeledClusteredOutput();
  typename LabeledOutputType::Pointer clusterBoundariesOutputPtr = this->GetClusterBoundariesOutput();

  outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  clusteredOutputPtr->SetBufferedRegion(clusteredOutputPtr->GetRequestedRegion());
  clusteredOutputPtr->Allocate();

  labeledClusteredOutputPtr->SetBufferedRegion(labeledClusteredOutputPtr->GetRequestedRegion());
  labeledClusteredOutputPtr->Allocate();

  clusterBoundariesOutputPtr->SetBufferedRegion(clusterBoundariesOutputPtr->GetRequestedRegion());
  clusterBoundariesOutputPtr->Allocate();
}


template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
void
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>::EnlargeOutputRequestedRegion( itk::DataObject *output )
{

  // This filter requires all of the output images in the buffer.
  for ( unsigned int j = 0; j < this->GetNumberOfOutputs(); j++ )
    {
    if ( this->itk::ProcessObject::GetOutput(j) )
      {
      this->itk::ProcessObject::GetOutput(j)->SetRequestedRegionToLargestPossibleRegion();
      }
    }
}

template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
void
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::GenerateData()
{
  // Allocate the output image
  this->AllocateOutputs();

  // Input and output pointers
  typename InputImageType::ConstPointer inputPtr  = this->GetInput();
  typename OutputImageType::Pointer     outputPtr = this->GetOutput();


  double invScale = 1 / m_Scale;

  RegionType inputRequestedRegion =inputPtr->GetRequestedRegion();
  RegionType outputRequestedRegion = inputPtr->GetRequestedRegion();

  inputRequestedRegion.PadByRadius(m_SpatialRadius);
  if (!inputRequestedRegion.Crop(inputPtr->GetRequestedRegion()))
    {
    itkExceptionMacro(<< "error in requested region cropping");
    }

  // Iterators
  itk::ImageRegionConstIteratorWithIndex<InputImageType> inputIt(inputPtr, inputRequestedRegion);
  itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRequestedRegion);

  //create image processing object
  msImageProcessor edisonProcessor;

  float * data = new float[inputRequestedRegion.GetNumberOfPixels() * inputPtr->GetNumberOfComponentsPerPixel()];

  unsigned int index = 0;

  for (inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt)
    {
    TBufferConverter::PixelToFloatArray(data, index, inputIt.Get(), m_Scale);
    index += inputPtr->GetNumberOfComponentsPerPixel();
    }

  edisonProcessor.DefineLInput(data,
                               inputRequestedRegion.GetSize()[1],
                               inputRequestedRegion.GetSize()[0],
                               inputPtr->GetNumberOfComponentsPerPixel());

  //define default kernel paramerters...
  kernelType k[2] = {Uniform, Uniform};
  int        P[2] = {2, static_cast<int>(inputPtr->GetNumberOfComponentsPerPixel())};
  float      tempH[2] = {1.0, 1.0};

  edisonProcessor.DefineKernel(k, tempH, P, 2);

  edisonProcessor.Filter(m_SpatialRadius, m_RangeRadius * m_Scale, MED_SPEEDUP);

  if (edisonProcessor.ErrorStatus)
    {
    itkExceptionMacro(<< "Error while running edison!");
    }

  typename OutputImageType::Pointer tmpOutput = OutputImageType::New();
  tmpOutput->SetRegions(inputRequestedRegion);
  tmpOutput->SetNumberOfComponentsPerPixel(outputPtr->GetNumberOfComponentsPerPixel());
  tmpOutput->Allocate();
  itk::ImageRegionIterator<OutputImageType> tmpIt(tmpOutput, inputRequestedRegion);
  itk::ImageRegionIterator<OutputImageType> tmp2It(tmpOutput, outputRequestedRegion);

  edisonProcessor.GetRawData(data);

  if (edisonProcessor.ErrorStatus)
    {
    itkExceptionMacro(<< "Error while running edison!");
    }

  index = 0;
  for (tmpIt.GoToBegin(); !tmpIt.IsAtEnd(); ++tmpIt)
    {
    OutputPixelType pixel;

    TBufferConverter::FloatArrayToPixel(data, index, pixel, outputPtr->GetNumberOfComponentsPerPixel(), invScale);
    tmpIt.Set(pixel);
    index += outputPtr->GetNumberOfComponentsPerPixel();
    }

  tmp2It.GoToBegin();
  outputIt.GoToBegin();

  while (!tmp2It.IsAtEnd() && !outputIt.IsAtEnd())
    {
    outputIt.Set(tmp2It.Get());
    ++tmp2It;
    ++outputIt;
    }


 // typename OutputImageType::Pointer   outputPtr = this->GetOutput();
   typename OutputImageType::Pointer   clusteredOutputPtr = this->GetClusteredOutput();
   typename LabeledOutputType::Pointer labeledClusteredOutputPtr = this->GetLabeledClusteredOutput();
   typename LabeledOutputType::Pointer clusterBoudariesOutputPtr = this->GetClusterBoundariesOutput();
   itk::ImageRegionIterator<OutputImageType> clusteredOutputIt(clusteredOutputPtr, outputRequestedRegion);

    index = 0;

   for (outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt)
     {
     TBufferConverter::PixelToFloatArray(data, index, outputIt.Get(), m_Scale);
     index += outputPtr->GetNumberOfComponentsPerPixel();
     }

   edisonProcessor.DefineLInput(data,
                                outputRequestedRegion.GetSize()[1],
                                outputRequestedRegion.GetSize()[0],
                                outputPtr->GetNumberOfComponentsPerPixel());

   // define default kernel paramerters...
   //kernelType k[2] = {Uniform, Uniform};
   //int        P[2] = {2, outputPtr->GetNumberOfComponentsPerPixel()};
   //float      tempH[2] = {1.0, 1.0};

   edisonProcessor.DefineKernel(k, tempH, P, 2);

   edisonProcessor.FuseRegions(m_RangeRadius * m_Scale, m_MinimumRegionSize);

   if (edisonProcessor.ErrorStatus)
     {
     itkExceptionMacro(<< "Error while running edison!");
     }

   edisonProcessor.GetRawData(data);

   if (edisonProcessor.ErrorStatus)
     {
     itkExceptionMacro(<< "Error while running edison!");
     }

   index = 0;
   for (clusteredOutputIt.GoToBegin(); !clusteredOutputIt.IsAtEnd(); ++clusteredOutputIt)
     {
     OutputPixelType pixel;
     TBufferConverter::FloatArrayToPixel(data, index, pixel,
                                         clusteredOutputPtr->GetNumberOfComponentsPerPixel(), invScale);
     clusteredOutputIt.Set(pixel);
     index += clusteredOutputPtr->GetNumberOfComponentsPerPixel();
     }

   int *   labels = NULL;
   float * modes = NULL;
   int *   modesPointsCount = NULL;

   edisonProcessor.GetRegions(&labels, &modes, &modesPointsCount);

   if (edisonProcessor.ErrorStatus)
     {
     itkExceptionMacro(<< "Error while running edison!");
     }

   itk::ImageRegionIteratorWithIndex<LabeledOutputType> lcIt(labeledClusteredOutputPtr,
                                                             labeledClusteredOutputPtr->GetRequestedRegion());

   index = 0;

   labeledClusteredOutputPtr->FillBuffer(0);

   for (lcIt.GoToBegin(); !lcIt.IsAtEnd(); ++lcIt)
     {
     lcIt.Set(static_cast<LabelType>(labels[index]));
     ++index;
     }

   clusterBoudariesOutputPtr->FillBuffer(0);

   //define the boundaries
   RegionList * regionList        = edisonProcessor.GetBoundaries();
   int *        regionIndeces;
   unsigned int numRegions = regionList->GetNumRegions();

   typename LabeledOutputType::IndexType boundIndex;

   for (LabelType label = 0; label < static_cast<LabelType>(numRegions); ++label)
     {
     OutputPixelType pixel;
     TBufferConverter::FloatArrayToPixel(modes,
                                         static_cast<unsigned int>(label *
                                                                   clusteredOutputPtr->GetNumberOfComponentsPerPixel()),
                                         pixel, clusteredOutputPtr->GetNumberOfComponentsPerPixel(), invScale);
     // Filling the modes map
     m_Modes[label] = pixel;
     regionIndeces = regionList->GetRegionIndeces(static_cast<int>(label));
     for (int i = 0; i < regionList->GetRegionCount(static_cast<int>(label)); ++i)
       {
       boundIndex[0] =
         (regionIndeces[i] %
          clusterBoudariesOutputPtr->GetRequestedRegion().GetSize()[0]) +
         clusterBoudariesOutputPtr->GetRequestedRegion().GetIndex()[0];
       boundIndex[1] =
         (regionIndeces[i] /
          clusterBoudariesOutputPtr->GetRequestedRegion().GetSize()[0]) +
         clusterBoudariesOutputPtr->GetRequestedRegion().GetIndex()[1];
       if (clusterBoudariesOutputPtr->GetBufferedRegion().IsInside(boundIndex))
         {
         clusterBoudariesOutputPtr->SetPixel(boundIndex, 1);
         }
       }
     }

   // Free memory
   delete[] labels;
   delete[] modes;
   delete[] modesPointsCount;
   delete[] data;
}


template <class TInputImage, class TOutputImage, class TLabeledOutput, class TBufferConverter>
void
MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, TBufferConverter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Spatial radius: "                << m_SpatialRadius               << std::endl;
  os << indent << "Range radius: "                  << m_RangeRadius                 << std::endl;
  os << indent << "Minimum region size: "           << m_MinimumRegionSize           << std::endl;
  os << indent << "Scale: "                         << m_Scale                       << std::endl;
}

} // end namespace otb

#endif
