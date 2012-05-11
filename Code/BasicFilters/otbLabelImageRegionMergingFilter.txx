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

#ifndef __otbLabelImageRegionMergingFilter_txx
#define __otbLabelImageRegionMergingFilter_txx

#include "otbLabelImageRegionMergingFilter.h"
#include "itkProgressReporter.h"


namespace otb
{
template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::LabelImageRegionMergingFilter()
{

  this->SetNumberOfRequiredInputs( 2 );

}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::SetInputLabelImage( const TInputLabelImage * labelImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputLabelImage *>( labelImage ));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::SetInputSpectralImage( const TInputSpectralImage * spectralImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputSpectralImage *>( spectralImage ));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
TInputLabelImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetInputLabelImage()
{
  return dynamic_cast<TInputLabelImage*>(itk::ProcessObject::GetInput(0));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
TInputSpectralImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetInputSpectralImage()
{
  return dynamic_cast<TInputSpectralImage*>(itk::ProcessObject::GetInput(1));
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::~LabelImageRegionMergingFilter()
{

}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
TOutputLabelImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetLabelOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
      return 0;
    }
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
const TOutputLabelImage *
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::GetLabelOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
      return 0;
    }
  return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::BeforeThreadedGenerateData()
{
  typename InputSpectralImageType::Pointer spectralImage = this->GetInputSpectralImage();
  typename InputLabelImageType::Pointer inputLabelImage = this->GetInputLabelImage();

  m_NumberOfComponentsPerPixel = spectralImage->GetNumberOfComponentsPerPixel();

  // Convert to label map with adjacency
  typename LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(inputLabelImage);
  labelMapFilter->Update();

  // Associate each label to a spectral value
  m_LabelMap = labelMapFilter->GetOutput();
  typename LabelMapType::LabelObjectContainerType & labelObjectContainer = m_LabelMap->GetLabelObjectContainer();
  typename LabelMapType::LabelObjectContainerType::iterator labelIt = labelObjectContainer.begin();
  while ( labelIt != labelObjectContainer.end() )
    {
    LabelType l;
    l = labelIt->first;
    typename AttributeLabelObjectType::Pointer labelObject = labelIt->second;

    labelObject->SetAttribute(
      // spectralImage->GetPixel(        labelObject->GetLine(0).GetIndex() )
      AttributeType( spectralImage->GetPixel( labelObject->GetLine(0).GetIndex() ) )
    );
    ++labelIt;
    }

  // Region Merging

  //Define a vector of the pairs to merge
  //typename LabelMapType::LabelPairVectorType pairs;
  //typedef typename LabelMapType::LabelPairType LabelPairType;
  bool finishedMerging = false;
  unsigned int mergeIterations = 0;

  std::cout << "Start merging" << std::endl;
  while(!finishedMerging)
    {
    bool restartFromFirstRegion = false;
    bool mergedOnce = false;
    labelObjectContainer = m_LabelMap->GetLabelObjectContainer();
    labelIt = labelObjectContainer.begin();
    while (labelIt != labelObjectContainer.end())
      {
      typename AttributeLabelObjectType::Pointer labelObject;
      labelObject = m_LabelMap->GetLabelObject(labelIt->first);
      //const OutputPixelType spectral = labelObject->GetAttribute().minValue;
      const AttributeType &currentObjectAttributes = labelObject->GetAttribute();
      //std::cout << "label " << labelIt->first << ": " << labelObject->Size() << "pixels" << std::endl;
      const typename LabelMapType::AdjacentLabelsContainerType & adjacentLabelsContainer = m_LabelMap->GetAdjacentLabels(labelIt->first);
      typename LabelMapType::AdjacentLabelsContainerType::iterator adjIt = adjacentLabelsContainer.begin();
      while (adjIt != adjacentLabelsContainer.end())
        {
        LabelType adjLabel;
        adjLabel = *adjIt;
        typename AttributeLabelObjectType::Pointer adjLabelObject = m_LabelMap->GetLabelObject(adjLabel);
        //const OutputPixelType adjSpectral = adjLabelObject->GetAttribute().minValue;
        const AttributeType &adjacentObjectAttributes = adjLabelObject->GetAttribute();
        //std::cout << "neighbour " << adjLabel << " = " << adjacentObjectAttributes << std::endl;

        // Check condition to fuse regions
        bool doMerge = true;
        for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel && doMerge; comp++)
          {
          if (std::min(currentObjectAttributes.minValue[comp], adjacentObjectAttributes.minValue[comp]) + m_RangeBandwidth < std::max(currentObjectAttributes.maxValue[comp], adjacentObjectAttributes.maxValue[comp]) )
            {
            doMerge = false;
            }
          }
        if(doMerge)
          //if( (adjSpectral - spectral).GetNorm() < m_RangeBandwidth )
          {
          // Define output attribute
          AttributeType att;
          att.minValue.SetSize(m_NumberOfComponentsPerPixel);
          att.maxValue.SetSize(m_NumberOfComponentsPerPixel);
          //std::cout << "merge: " << currentObjectAttributes << " and " << adjacentObjectAttributes << std::endl;
          for(unsigned int comp = 0; comp < m_NumberOfComponentsPerPixel && doMerge; comp++)
            {
            att.minValue[comp] = std::min(currentObjectAttributes.minValue[comp], adjacentObjectAttributes.minValue[comp]);
            att.maxValue[comp] = std::max(currentObjectAttributes.maxValue[comp], adjacentObjectAttributes.maxValue[comp]);
            }
          // Merge objects
          if(labelIt->second->Size() >= adjLabelObject->Size())
            {
            m_LabelMap->MergeLabels(labelIt->first, adjLabel);
            m_LabelMap->GetLabelObject(labelIt->first)->SetAttribute(att);
            }
          else
            {
            m_LabelMap->MergeLabels(adjLabel, labelIt->first);
            m_LabelMap->GetLabelObject(adjLabel)->SetAttribute(att);
            restartFromFirstRegion = true;
            }
          //pairs.push_back(LabelPairType(labelIt->first, adjLabel));
          mergedOnce = true;
          // std::cout << "after merge: " << att << std::endl;
          }
        ++adjIt;
        if(restartFromFirstRegion) break;
        } // end of loop over adjacent labels

      if(restartFromFirstRegion) break;
      ++labelIt;
      } // end of loop over labels
    if ( !mergedOnce   // finished iterating over all regions without merging once
         //   || mergeIterations >= 10 // TEMPORARY !!!
    ) finishedMerging = true;
    mergeIterations++;
     std::cout << "mergeIterations: " << mergeIterations << "(" <<  m_LabelMap->GetNumberOfLabelObjects() << ") objects" << std::endl;
    } // end of main iteration loop
  std::cout << "merge iterations: " << mergeIterations << std::endl;
  std::cout << "number of label objects: " << m_LabelMap->GetNumberOfLabelObjects() << std::endl;
  // Merge pairs listed in the vector
  //m_LabelMap->MergeLabels(pairs);
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::ThreadedGenerateData(const OutputRegionType& outputRegionForThread, int threadId)
{
}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::AfterThreadedGenerateData()
{
  typename OutputLabelImageType::Pointer outputLabelImage = this->GetOutput();
  typedef itk::ImageRegionIterator<OutputLabelImageType> OutputLabelIteratorType;
  OutputLabelIteratorType outputLabelIt(outputLabelImage, outputLabelImage->GetRequestedRegion());

  // copy labelMap to output label image
  typename LabelMapToLabelImageFilterType::Pointer mapToImageFilter = LabelMapToLabelImageFilterType::New();
  mapToImageFilter->SetInput(m_LabelMap);
  mapToImageFilter->Update();

  OutputLabelIteratorType mapToImageIt(mapToImageFilter->GetOutput(), outputLabelImage->GetRequestedRegion());

  outputLabelIt.GoToBegin();
  mapToImageIt.GoToBegin();

  while(!outputLabelIt.IsAtEnd())
    {
    outputLabelIt.Set(mapToImageIt.Get());
    ++outputLabelIt;
    ++mapToImageIt;
    }

}


template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage>
void
LabelImageRegionMergingFilter<TInputLabelImage, TInputSpectralImage, TOutputLabelImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Range bandwidth: "                  << m_RangeBandwidth                 << std::endl;
 }

} // end namespace otb

#endif
