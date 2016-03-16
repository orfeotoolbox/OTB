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
#ifndef __otbImageSampleExtractorFilter_txx
#define __otbImageSampleExtractorFilter_txx

#include "otbMaskedIteratorDecorator.h"
#include "itkImageRegionConstIteratorWithOnlyIndex.h"
#include "itkImageRegionConstIterator.h"
#include "itkDefaultConvertPixelTraits.h"

namespace otb
{
// --------- otb::PersistentImageSampleExtractorFilter ---------------------

template<class TInputImage>
PersistentImageSampleExtractorFilter<TInputImage>
::PersistentImageSampleExtractorFilter() :
  m_LayerIndex(0)
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TInputImage::New());
  this->SetNthOutput(1,OGRDataType::New());
  this->m_FieldPrefix = std::string("band_");
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::SetInputOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage>
const otb::ogr::DataSource*
PersistentImageSampleExtractorFilter<TInputImage>
::GetInputOGRData()
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const otb::ogr::DataSource *>(this->itk::ProcessObject::GetInput(1));
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::SetOutputOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthOutput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage>
const otb::ogr::DataSource*
PersistentImageSampleExtractorFilter<TInputImage>
::GetOutputOGRData()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<const otb::ogr::DataSource *>(this->itk::ProcessObject::GetOutput(1));
}


template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::Synthetize(void)
{
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetInputOGRData());
  vectors->GetLayer(m_LayerIndex).SetSpatialFilter(NULL);
  
  // TODO ?
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::Reset(void)
{
  // TODO
}

template<class TInputImage>
itk::DataObject::Pointer
PersistentImageSampleExtractorFilter<TInputImage>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  switch (idx)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(otb::ogr::DataSource::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

//template<class TInputImage>
//void
//PersistentImageSampleExtractorFilter<TInputImage>
//::GenerateInputRequestedRegion()
//{
  //InputImageType *input = const_cast<InputImageType*>(this->GetInput());
  //MaskImageType *mask = const_cast<MaskImageType*>(this->GetMask());

  //RegionType requested = this->GetOutput()->GetRequestedRegion();
  //RegionType emptyRegion = input->GetLargestPossibleRegion();
  //emptyRegion.SetSize(0,0);
  //emptyRegion.SetSize(1,0);

  //input->SetRequestedRegion(emptyRegion);

  //if (mask)
    //{
    //mask->SetRequestedRegion(requested);
    //}
//}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::GenerateData()
{
  // Retrieve inputs
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  TInputImage* outputImage = this->GetOutput();
  const otb::ogr::DataSource* inVectors = this->GetInputOGRData();
  otb::ogr::DataSource* outVectors = const_cast<otb::ogr::DataSource*>(this->GetOutputOGRData());
  PointType imgPoint;
  IndexType imgIndex;
  PixelType imgPixel;
  InternalPixelType imgComp;
  RegionType requestedRegion = outputImage->GetRequestedRegion();
  bool updateMode = bool(inVectors == outVectors);
  unsigned int nbBand = inputImage->GetNumberOfComponentsPerPixel();
  
  this->ApplyPolygonsSpatialFilter();
  
  ogr::Layer layer = inVectors->GetLayer(m_LayerIndex);

  // Loop across the features in the layer
  OGRGeometry *geom;
  otb::ogr::Layer::const_iterator featIt = layer.begin(); 
  for(; featIt!=layer.end(); ++featIt)
    {
    geom = featIt->ogr().GetGeometryRef();
    switch (geom->getGeometryType())
      {
      case wkbPoint:
      case wkbPoint25D:
        {
        OGRPoint* castPoint = dynamic_cast<OGRPoint*>(geom);
        if (castPoint == NULL)
          {
          // Wrong Type !
          break;
          }
        imgPoint[0] = castPoint->getX();
        imgPoint[1] = castPoint->getY();
        inputImage->TransformPhysicalPointToIndex(imgPoint,imgIndex);
        imgPixel = inputImage->GetPixel(imgIndex);
        for (unsigned int i=0 ; i<nbBand ; ++i)
          {
          imgComp = itk::DefaultConvertPixelTraits<PixelType>::GetNthComponent(i,imgPixel);
          // TODO
          }
        
        
        break;
        }
      default:
        {
        otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
        break;
        }
      }
    
    // TODO
    }
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::ApplyPolygonsSpatialFilter()
{
  TInputImage* outputImage = this->GetOutput();
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetInputOGRData());
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

// -------------- otb::ImageSampleExtractorFilter --------------------------

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage>
const TInputImage*
ImageSampleExtractorFilter<TInputImage>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetInputOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetInputOGRData(data);
}

template<class TInputImage>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage>
::GetInputOGRData()
{
  return this->GetFilter()->GetInputOGRData();
}

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetOutputOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOutputOGRData(data);
}

template<class TInputImage>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage>
::GetOutputOGRData()
{
  return this->GetFilter()->GetOutputOGRData();
}

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetFieldPrefix(std::string &key)
{
  this->GetFilter()->SetFieldPrefix(key);
}

template<class TInputImage>
std::string
ImageSampleExtractorFilter<TInputImage>
::GetFieldPrefix()
{
  return this->GetFilter()->GetFieldPrefix();
}

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TInputImage>
int
ImageSampleExtractorFilter<TInputImage>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

} // end of namespace otb

#endif
