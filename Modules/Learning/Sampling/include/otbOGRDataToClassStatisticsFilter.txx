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
#ifndef __otbOGRDataToClassStatisticsFilter_txx
#define __otbOGRDataToClassStatisticsFilter_txx

#include "otbMaskedIteratorDecorator.h"
#include "itkImageRegionConstIteratorWithOnlyIndex.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{
// --------- otb::PersistentOGRDataToClassStatisticsFilter ---------------------

template<class TInputImage, class TMaskImage>
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::PersistentOGRDataToClassStatisticsFilter() :
  m_LayerIndex(0)
{
  this->SetNumberOfRequiredOutputs(3);
  this->SetNthOutput(0,TInputImage::New());
  this->SetNthOutput(1,ClassCountObjectType::New());
  this->SetNthOutput(2,PolygonSizeObjectType::New());
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
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
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->SetNthInput(2, const_cast<TMaskImage *>( mask ));
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
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
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::Synthetize(void)
{
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetOGRData());
  vectors->GetLayer(m_LayerIndex).SetSpatialFilter(NULL);
  
  ClassCountMapType &classCount = this->GetClassCountOutput()->Get();
  PolygonSizeMapType &polygonSize = this->GetPolygonSizeOutput()->Get();
  
  // Reset outputs
  classCount.clear();
  polygonSize.clear();
  // Copy temporary stats to outputs
  classCount = m_TemporaryStats->GetClassCountMap();
  polygonSize = m_TemporaryStats->GetPolygonSizeMap();
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
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

  // Reset list of individual containers
  //m_TemporaryStats = std::vector<PolygonClassStatisticsAccumulator::Pointer>(numberOfThreads);
  //std::vector<PolygonClassStatisticsAccumulator::Pointer>::iterator it = m_TemporaryStats.begin();
  //for (; it != m_TemporaryStats.end(); it++)
  //{
    //*it = PolygonClassStatisticsAccumulator::New();
    //(*it)->SetFieldIndex(fieldIndex);
  //}
  m_TemporaryStats = PolygonClassStatisticsAccumulator::New();
  m_TemporaryStats->SetFieldIndex(fieldIndex);
}

template<class TInputImage, class TMaskImage>
const typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput() const
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<const ClassCountObjectType *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TMaskImage>
typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType* 
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<ClassCountObjectType *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TMaskImage>
const typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput() const
{
  if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<const PolygonSizeObjectType *>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TMaskImage>
typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput()
{
    if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<PolygonSizeObjectType *>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TMaskImage>
itk::DataObject::Pointer
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  switch (idx)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(ClassCountObjectType::New().GetPointer());
      break;
    case 2:
      return static_cast<itk::DataObject*>(PolygonSizeObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
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
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
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

//template<class TInputImage, class TMaskImage>
//void
//PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
//::BeforeThreadedGenerateData()
//{
  //this->ApplyPolygonsSpatialFilter();
//}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
//::ThreadedGenerateData(const RegionType& outputRegionForThread,
//                       itk::ThreadIdType threadId)
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
        // For pixels in consideredRegion and not masked
        typedef otb::MaskedIteratorDecorator<
          itk::ImageRegionConstIterator<TInputImage>,
          itk::ImageRegionConstIterator<TMaskImage> > MaskedIteratorType;
        MaskedIteratorType it(mask, inputImage, consideredRegion);
        m_TemporaryStats->Add<MaskedIteratorType>(featIt, it);
      }
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
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
typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::RegionType
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
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

// -------------- otb::OGRDataToClassStatisticsFilter --------------------------

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TMaskImage>
const TInputImage*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetFieldName(std::string &key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TInputImage, class TMaskImage>
int
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

template<class TInputImage, class TMaskImage>
const typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput() const
{
  return this->GetFilter()->GetClassCountOutput();
}

template<class TInputImage, class TMaskImage>
typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput()
{
  return this->GetFilter()->GetClassCountOutput();
}

template<class TInputImage, class TMaskImage>
const typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput() const
{
  return this->GetFilter()->GetPolygonSizeOutput();
}

template<class TInputImage, class TMaskImage>
typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput()
{
  return this->GetFilter()->GetPolygonSizeOutput();
}


} // end of namespace otb

#endif
