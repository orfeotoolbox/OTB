/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingLineSegmentDetector_txx
#define __otbStreamingLineSegmentDetector_txx

#include "otbStreamingLineSegmentDetector.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

template<class TInputImage, class TPrecision>
PersistentLineSegmentDetector<TInputImage, TPrecision>
::PersistentLineSegmentDetector()
  : m_ThreadDistanceThreshold(10.)
{
  this->SetNumberOfRequiredInputs(1);

  // Have 2 outputs : the image created by Superclass, a vector data with lines
  this->SetNumberOfRequiredOutputs(3);
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
}

template<class TInputImage, class TPrecision>
PersistentLineSegmentDetector<TInputImage, TPrecision>
::~PersistentLineSegmentDetector()
{
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //ImagePointerType image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());
    }

  if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
    {
    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }
}

template<class TInputImage, class TPrecision>
typename PersistentLineSegmentDetector<TInputImage, TPrecision>::VectorDataType*
PersistentLineSegmentDetector<TInputImage, TPrecision>
::GetOutputVectorData()
{
  return static_cast<VectorDataType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TPrecision>
itk::DataObject::Pointer
PersistentLineSegmentDetector<TInputImage, TPrecision>
::MakeOutput(unsigned int idx)
{
  itk::DataObject::Pointer output;
  switch (idx)
    {
    case 0:
      output = static_cast<itk::DataObject*>(ImageType::New().GetPointer());
      break;
    case 1:
      {
      output = static_cast<itk::DataObject*>(VectorDataType::New().GetPointer());
      break;
      }
    default:
      output = static_cast<itk::DataObject*>(ImageType::New().GetPointer());
      break;
    }
  return output;
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::Reset()
{
  int nbThread = this->GetNumberOfThreads();

  m_ExtractorList.clear();
  m_RegionList.clear();
  m_LineDetectorList.clear();
  m_VectorDataList.clear();

  m_VectorDataList.resize(nbThread);
  m_RegionList.resize(nbThread);

  for (int p = 0; p < nbThread; ++p)
    {
    m_ExtractorList.push_back(ExtractorType::New());
    m_LineDetectorList.push_back(LineDetectorType::New());
    m_LineDetectorList[p]->SetImageSize(this->GetInput()->GetLargestPossibleRegion().GetSize());
    }

}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::Synthetize()
{
  // merge all lines in a single vector data
  VectorDataType* outputVData = this->GetOutputVectorData();
  
  // Retrieving root node
  DataNodePointerType root = outputVData->GetDataTree()->GetRoot()->Get();
  // Create the document node
  DataNodePointerType document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  DataNodePointerType folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  outputVData->GetDataTree()->Add(document, root);
  outputVData->GetDataTree()->Add(folder, document);

  for (unsigned int threadId = 0; threadId < m_VectorDataList.size(); ++threadId)
    {    
    TreeIteratorType itCurrentVData(m_VectorDataList[threadId]->GetDataTree());
    itCurrentVData.GoToBegin();
    while (!itCurrentVData.IsAtEnd())
      {
      if (itCurrentVData.Get()->IsLineFeature())
        {
        outputVData->GetDataTree()->Add(itCurrentVData.Get(), folder);
        }
      itCurrentVData ++;
      }
    }
  
  this->GetOutputVectorData()->SetMetaDataDictionary(m_VectorDataList[0]->GetMetaDataDictionary());

}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  if (this->GetInput())
    {
    ImagePointerType image = const_cast<ImageType *>(this->GetInput());
    image->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
    }
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  ImagePointerType inputPtr = const_cast<TInputImage *>(this->GetInput());
  
  RegionType region = outputRegionForThread;
  region.PadByRadius((unsigned int)(1));
  region.Crop(inputPtr->GetLargestPossibleRegion());
  
  inputPtr->SetRequestedRegion(region);
  inputPtr->PropagateRequestedRegion();
  inputPtr->UpdateOutputData();

  m_ExtractorList[threadId]->SetInput(this->GetInput());
  m_ExtractorList[threadId]->SetExtractionRegion(region);
  //m_ExtractorList[threadId]->SetExtractionRegion(outputRegionForThread);
  m_ExtractorList[threadId]->UpdateOutputInformation();
  
  m_LineDetectorList[threadId]->SetInput(m_ExtractorList[threadId]->GetOutput());
  m_LineDetectorList[threadId]->Update();
  
  m_RegionList[threadId] = outputRegionForThread;
  m_VectorDataList[threadId] =  m_LineDetectorList[threadId]->GetOutput();
}



// end of class PersistentLineSegmentDetector

/**===========================================================================*/

template<class TInputImage, class TPrecision>
StreamingLineSegmentDetector<TInputImage, TPrecision>
::StreamingLineSegmentDetector()
{
}

template<class TInputImage, class TPrecision>
StreamingLineSegmentDetector<TInputImage, TPrecision>
::~StreamingLineSegmentDetector()
{
}

/**
 * The aim here is to gathered each thread output into a vector data.
 * For that we have to fuse different thread output, in particular for line that throw thread.
 * For that, for each thread and each detected line, we look if one of its extrema is over a thread lower limit (RegionList.GetSize()[1]).
 * If yes, we store the line. If no, we add it to the vector data.
 * At the next loop (ie. next thread), for each line, we check that it doesn't have a point in common with lines that end at the end of
 * the previous thread.
 * If yes we compute the extrema (one point ineach thread).
 * If not, we add it to the vector data, else we store it.
 * And so on.
 * m_ThreadDistanceThreshold allows a tolerance over the commmon point spatial distance when 2 lines are fused.
 */

} // end namespace otb
#endif
