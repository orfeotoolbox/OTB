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

  // Set the number of threads to 1 by default to avoid oversegmentation
  // There also seem to be a bug in the underlying LineSegmentDetector
  // for small regions (probably when a stream does not contain segment)
  this->SetNumberOfThreads(1);
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

  m_VectorDataList.clear();
  m_VectorDataList.resize(nbThread);

  // merge all lines in a single vector data
  VectorDataType* outputVData = this->GetOutputVectorData();

  // Retrieving root node
  DataNodePointerType root = outputVData->GetDataTree()->GetRoot()->Get();
  // Create the document node
  DataNodePointerType document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  DataNodePointerType folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  m_Folder = folder;
  // Adding the layer to the data tree
  outputVData->GetDataTree()->Add(document, root);
  outputVData->GetDataTree()->Add(folder, document);


}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::Synthetize()
{
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
    ImagePointerType input = const_cast<ImageType *>(this->GetInput());

    RegionType region = this->GetOutput()->GetRequestedRegion();
    region.PadByRadius(1);
    region.Crop(input->GetLargestPossibleRegion());
    input->SetRequestedRegion(region);
    }
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::BeforeThreadedGenerateData()
{
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  try
  {
    ImagePointerType inputPtr = const_cast<TInputImage *>(this->GetInput());

    RegionType region = outputRegionForThread;
    region.PadByRadius((unsigned int)(1));
    region.Crop(inputPtr->GetLargestPossibleRegion());

    inputPtr->SetRequestedRegion(region);
    inputPtr->PropagateRequestedRegion();
    inputPtr->UpdateOutputData();

    typename ExtractorType::Pointer extractFilter = ExtractorType::New();
    extractFilter->SetInput(this->GetInput());
    extractFilter->SetExtractionRegion(region);
    extractFilter->Update();

    typename LineDetectorType::Pointer lineDetector = LineDetectorType::New();
    lineDetector->SetInput(extractFilter->GetOutput());
    lineDetector->Update();

    m_VectorDataList[threadId] = lineDetector->GetOutput();
  }
  catch (itk::ExceptionObject& e)
  {
    std::cout << "Exception : " << e;
    throw;
  }
}

template<class TInputImage, class TPrecision>
void
PersistentLineSegmentDetector<TInputImage, TPrecision>
::AfterThreadedGenerateData()
{
  // merge all lines in a single vector data
  VectorDataType* outputVData = this->GetOutputVectorData();
  outputVData->GetDataTree();

  for (unsigned int threadId = 0; threadId < m_VectorDataList.size(); ++threadId)
    {
    TreeIteratorType itCurrentVData(m_VectorDataList[threadId]->GetDataTree());
    itCurrentVData.GoToBegin();
    while (!itCurrentVData.IsAtEnd())
      {
      if (itCurrentVData.Get()->IsLineFeature())
        {
        outputVData->GetDataTree()->Add(itCurrentVData.Get(), m_Folder);
        }
      itCurrentVData ++;
      }
    }

  this->GetOutputVectorData()->SetMetaDataDictionary(m_VectorDataList[0]->GetMetaDataDictionary());
}

// end of class PersistentLineSegmentDetector

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

} // end namespace otb
#endif
