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
#ifndef __otbOGRDataToSamplePositionFilter_txx
#define __otbOGRDataToSamplePositionFilter_txx

#include "otbMaskedIteratorDecorator.h"
#include "itkImageRegionConstIteratorWithOnlyIndex.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{
// --------- otb::PersistentOGRDataToSamplePositionFilter ---------------------

template<class TInputImage, class TMaskImage>
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::PersistentOGRDataToSamplePositionFilter() :
   m_LayerIndex(0),
   m_MaxSamplingVecSize(1000),
   m_OutputVectorDataPath("")
{
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0,TInputImage::New());
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const otb::ogr::DataSource *>(this->itk::ProcessObject::GetInput(1));
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->SetNthInput(2, const_cast<TMaskImage *>( mask ));
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetMask()
{
  if (this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(2));
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::Synthetize(void)
{
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetOGRData());
  vectors->GetLayer(m_LayerIndex).SetSpatialFilter(NULL);
  
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::Reset(void)
{
  // Get OGR field index
  const otb::ogr::DataSource* vectors = this->GetOGRData();
  otb::ogr::Layer::const_iterator featIt = vectors->GetLayer(m_LayerIndex).begin();
  int fieldIndex = featIt->ogr().GetFieldIndex(this->m_FieldName.c_str());
  if (fieldIndex < 0)
    {
    itkGenericExceptionMacro("Field named "<<this->m_FieldName<<" not found!");
    }

   m_TemporaryStats = OGRDataResampler::New();
   m_TemporaryStats->SetFieldName(this->GetFieldName());
   m_TemporaryStats->SetFieldIndex(fieldIndex);
   m_TemporaryStats->SetMaxSamplingVecSize(m_MaxSamplingVecSize);
   m_TemporaryStats->SetInputOGRDataSourcePointer(vectors);
   m_TemporaryStats->SetLayerIndex(this->GetLayerIndex());
   m_TemporaryStats->SetOutputPath(this->GetOutputVectorDataPath());
   m_TemporaryStats->SetMaxSamplingVecSize(m_MaxSamplingVecSize);
   if (!m_OutputSamplingVectorsPath.empty())
      m_TemporaryStats->SetOutputSamplingVectorsPath(m_OutputSamplingVectorsPath);
   if (!m_InputSamplingVectorsPath.empty())
      m_TemporaryStats->SetInputSamplingVectorsPath(m_InputSamplingVectorsPath);


   if (m_RatesByClass.empty())
      {itkGenericExceptionMacro("m_RatesbyClass is empty. Use SetRatesbyClass to provide some vector statistics information.");}
   else
      {
         m_TemporaryStats->SetRatesbyClass(m_RatesByClass); 
         m_TemporaryStats->Prepare();
         m_TemporaryStats->PrepareOutputOGRData();
      }
}




template<class TInputImage, class TMaskImage>
itk::DataObject::Pointer
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  switch (idx)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  
  const MaskImageType *mask = this->GetMask();
  if (mask)
    {
    const InputImageType *input = this->GetInput();
    if (mask->GetLargestPossibleRegion() !=
        input->GetLargestPossibleRegion() )
      {
      itkGenericExceptionMacro("Mask and input image have a different size!");
      }
    if (mask->GetOrigin() != input->GetOrigin())
      {
      itkGenericExceptionMacro("Mask and input image have a different origin!");
      }
    if (mask->GetSpacing() != input->GetSpacing())
      {
      itkGenericExceptionMacro("Mask and input image have a different spacing!");
      }
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GenerateInputRequestedRegion()
{
  InputImageType *input = const_cast<InputImageType*>(this->GetInput());
  MaskImageType *mask = const_cast<MaskImageType*>(this->GetMask());

  RegionType requested = this->GetOutput()->GetRequestedRegion();
  RegionType emptyRegion = input->GetLargestPossibleRegion();
  emptyRegion.SetSize(0,0);
  emptyRegion.SetSize(1,0);

  input->SetRequestedRegion(emptyRegion);

  if (mask)
    {
    mask->SetRequestedRegion(requested);
    }
}


template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GenerateData()
{
  // Retrieve inputs
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  TInputImage* outputImage = this->GetOutput();
  TMaskImage* mask = const_cast<TMaskImage*>(this->GetMask());
  const otb::ogr::DataSource* vectors = this->GetOGRData();
  PointType point;
  RegionType requestedRegion =outputImage->GetRequestedRegion();
  
  this->ApplyPolygonsSpatialFilter();
  
  ogr::Layer layer = vectors->GetLayer(m_LayerIndex);

  // Loop across the features in the layer (filtered by requested region in BeforeTGD already)
  otb::ogr::Layer::const_iterator featIt = layer.begin(); 
  for(; featIt!=layer.end(); ++featIt)
    {
    // Compute the intersection of thread region and polygon bounding region, called "considered region"
    // This need not be done in ThreadedGenerateData and could be pre-processed and cached before filter execution if needed
    RegionType consideredRegion = FeatureBoundingRegion(inputImage, featIt);
    bool regionNotEmpty = consideredRegion.Crop(requestedRegion);
    if (regionNotEmpty)
      {
        if (mask)
          {
          // For pixels in consideredRegion and not masked
          typedef otb::MaskedIteratorDecorator<
            itk::ImageRegionConstIterator<TMaskImage>,
            itk::ImageRegionConstIterator<TMaskImage> > MaskedIteratorType;
          MaskedIteratorType it(mask, mask, consideredRegion);
          m_TemporaryStats->Add<MaskedIteratorType>(featIt, it, mask);
          }
        else
          {
          typedef itk::ImageRegionConstIteratorWithOnlyIndex<TInputImage> NoValueIteratorType;
          NoValueIteratorType it(inputImage,consideredRegion);
          m_TemporaryStats->Add<NoValueIteratorType>(featIt, it, inputImage);
          }
      }
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::ApplyPolygonsSpatialFilter()
{
  TInputImage* outputImage = this->GetOutput();
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetOGRData());
  const RegionType& requestedRegion = outputImage->GetRequestedRegion();
  typename TInputImage::IndexType startIndex = requestedRegion.GetIndex();
  typename TInputImage::IndexType endIndex = requestedRegion.GetUpperIndex();

  itk::Point<double, 2> startPoint;
  itk::Point<double, 2> endPoint;

  outputImage->TransformIndexToPhysicalPoint(startIndex, startPoint);
  outputImage->TransformIndexToPhysicalPoint(endIndex, endPoint);

  vectors->GetLayer(m_LayerIndex).SetSpatialFilterRect(
    std::min(startPoint[0],endPoint[0]),
    std::min(startPoint[1],endPoint[1]),
    std::max(startPoint[0],endPoint[0]),
    std::max(startPoint[1],endPoint[1]));
}

template<class TInputImage, class TMaskImage>
typename PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>::RegionType
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::FeatureBoundingRegion(const TInputImage* image, otb::ogr::Layer::const_iterator& featIt) const
{
  // otb::ogr wrapper is incomplete and leaky abstraction is inevitable here
  OGREnvelope envelope;
  featIt->GetGeometry()->getEnvelope(&envelope);
  itk::Point<double, 2> lowerPoint, upperPoint;
  lowerPoint[0] = envelope.MinX;
  lowerPoint[1] = envelope.MinY;
  upperPoint[0] = envelope.MaxX;
  upperPoint[1] = envelope.MaxY;

  typename TInputImage::IndexType lowerIndex;
  typename TInputImage::IndexType upperIndex;

  image->TransformPhysicalPointToIndex(lowerPoint, lowerIndex);
  image->TransformPhysicalPointToIndex(upperPoint, upperIndex);

  // swap coordinate to keep lowerIndex as start index
  if (lowerIndex[0] > upperIndex[0])
    {
    int tmp = lowerIndex[0];
    lowerIndex[0] = upperIndex[0];
    upperIndex[0] = tmp;
    }
  if (lowerIndex[1] > upperIndex[1])
    {
    int tmp = lowerIndex[1];
    lowerIndex[1] = upperIndex[1];
    upperIndex[1] = tmp;
    }

  RegionType region;
  region.SetIndex(lowerIndex);
  region.SetUpperIndex(upperIndex);

  return region;
}

// -------------- otb::OGRDataToSamplePositionFilter --------------------------

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TMaskImage>
const TInputImage*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetFieldName(std::string key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TInputImage, class TMaskImage>
int
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetOutputSamplingVectorsPath(std::string path)
{
  this->GetFilter()->SetOutputSamplingVectorsPath(path);
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetInputSamplingVectorsPath(std::string path)
{
  this->GetFilter()->SetInputSamplingVectorsPath(path);
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetOutputVectorDataPath(std::string path)
{
  this->GetFilter()->SetOutputVectorDataPath(path);
}


template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetMaxSamplingVecSize(unsigned int max)
{
  this->GetFilter()->SetMaxSamplingVecSize(max);
}


template<class TInputImage, class TMaskImage>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::SetRatesByClass(const SamplingRateCalculator::MapRateType& map)
{
     this->GetFilter()->SetRatesByClass(map);
     
}

template<class TInputImage, class TMaskImage>
unsigned int
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetMaxSamplingVecSize()
{
  return this->GetFilter()->GetMaxSamplingVecSize();
}

template<class TInputImage, class TMaskImage>
const std::string*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetOutputVectorDataPath() const
{
  return this->GetFilter()->GetOutputVectorDataPath();
}

template<class TInputImage, class TMaskImage>
std::string*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage>
::GetOutputVectorDataPath()
{
  return this->GetFilter()->GetOutputVectorDataPath();
}


} // end of namespace otb

#endif
