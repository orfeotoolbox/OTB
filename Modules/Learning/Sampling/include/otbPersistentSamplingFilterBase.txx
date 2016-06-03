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
#ifndef __otbPersistentSamplingFilterBase_txx
#define __otbPersistentSamplingFilterBase_txx

#include "otbPersistentSamplingFilterBase.h"
#include "otbMaskedIteratorDecorator.h"
#include "itkImageRegionConstIteratorWithOnlyIndex.h"
#include "itkImageRegionConstIterator.h"
#include "otbMacro.h"
#include "itkTimeProbe.h"
#include "itkProgressReporter.h"

namespace otb
{

template<class TInputImage, class TMaskImage>
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::PersistentSamplingFilterBase()
  : m_FieldName(std::string("class"))
  , m_LayerIndex(0)
{
}


template<class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
PersistentSamplingFilterBase<TInputImage,TMaskImage>
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
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->SetNthInput(2, const_cast<TMaskImage *>( mask ));
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
PersistentSamplingFilterBase<TInputImage,TMaskImage>
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
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::SetOGRLayerCreationOptions(const std::vector<std::string> & options)
{
  m_OGRLayerCreationOptions.clear();
  m_OGRLayerCreationOptions = options;
}

template<class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Get OGR field index
  const otb::ogr::DataSource* vectors = this->GetOGRData();
  otb::ogr::Layer::const_iterator featIt = vectors->GetLayer(m_LayerIndex).begin();
  int fieldIndex = featIt->ogr().GetFieldIndex(this->m_FieldName.c_str());
  if (fieldIndex < 0)
    {
    itkGenericExceptionMacro("Field named "<<this->m_FieldName<<" not found!");
    }
  this->m_FieldIndex = fieldIndex;

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
PersistentSamplingFilterBase<TInputImage,TMaskImage>
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


template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::BeforeThreadedGenerateData(void)
{
  this->PrepareInputVectors();

  this->PrepareOutputVectors();
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::AfterThreadedGenerateData(void)
{
  // clean temporary inputs
  this->m_InMemoryInputs.clear();

  // gather temporary outputs and write to output
  itk::TimeProbe chrono;
  chrono.Start();
  unsigned int count = 0;
  for (unsigned int k=0 ; k < this->GetNumberOfOutputs() ; k++)
    {
    ogr::DataSource* realOutput = dynamic_cast<ogr::DataSource *>(
        this->itk::ProcessObject::GetOutput(k));
    if (realOutput)
      {
      ogr::Layer outLayer = realOutput->GetLayersCount() == 1
                            ? realOutput->GetLayer(0)
                            : realOutput->GetLayer(m_LayerName);

      OGRErr err = outLayer.ogr().StartTransaction();
      if (err != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
        }
  
      for (unsigned int thread=0 ; thread < this->GetNumberOfThreads() ; thread++)
        {
        ogr::Layer inLayer = this->m_InMemoryOutputs[thread][count]->GetLayerChecked(0);
        if (!inLayer)
          {
          continue;
          }
  
        ogr::Layer::const_iterator tmpIt = inLayer.begin();
        for(; tmpIt!=inLayer.end(); ++tmpIt)
          {
          ogr::Feature dstFeature(outLayer.GetLayerDefn());
          dstFeature.SetFrom( *tmpIt, TRUE );
          outLayer.CreateFeature( dstFeature );
          }
        }
  
      err = outLayer.ogr().CommitTransaction();
      if (err != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << outLayer.ogr().GetName() << ".");
        }
      count++;
      }
    }
  chrono.Stop();
  otbMsgDebugMacro(<< "write ogr points took " << chrono.GetTotal() << " sec");
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::ThreadedGenerateData(const RegionType&, itk::ThreadIdType threadid)
{
  // Retrieve inputs
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  TInputImage* outputImage = this->GetOutput();
  RegionType requestedRegion = outputImage->GetRequestedRegion();

  ogr::Layer layer = this->m_InMemoryInputs[threadid]->GetLayerChecked(0);
  if (! layer)
    {
    return;
    }

  itk::ProgressReporter progress( this, threadid, layer.GetFeatureCount(true) );

  // Loop across the features in the layer (filtered by requested region in BeforeTGD already)
  ogr::Layer::const_iterator featIt = layer.begin();
  for(; featIt!=layer.end(); ++featIt)
    {
    // Compute the intersection of thread region and polygon bounding region, called "considered region"
    // This need not be done in ThreadedGenerateData and could be pre-processed and cached before filter execution if needed
    RegionType consideredRegion = FeatureBoundingRegion(inputImage, featIt);
    bool regionNotEmpty = consideredRegion.Crop(requestedRegion);
    if (regionNotEmpty)
      {
      this->PrepareFeature(*featIt,threadid);
      this->ExploreGeometry(*featIt, featIt->ogr().GetGeometryRef(),consideredRegion,threadid);
      }
    progress.CompletedPixel();
    }
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::ExploreGeometry(const ogr::Feature& feature,
                  OGRGeometry* geom,
                  RegionType& region,
                  itk::ThreadIdType& threadid)
{
  typename TInputImage::PointType imgPoint;
  typename TInputImage::IndexType imgIndex;
  switch (geom->getGeometryType())
    {
    case wkbPoint:
    case wkbPoint25D:
      {
      OGRPoint* castPoint = dynamic_cast<OGRPoint*>(geom);
      imgPoint[0] = castPoint->getX();
      imgPoint[1] = castPoint->getY();
      const TInputImage* img = this->GetInput();
      const TMaskImage* mask = this->GetMask(); 
      img->TransformPhysicalPointToIndex(imgPoint,imgIndex);
      if ((mask == NULL) || mask->GetPixel(imgIndex))
        {
        this->ProcessSample(feature, imgIndex, imgPoint, threadid);
        }
      break;
      }
    case wkbLineString:
    case wkbLineString25D:
      {
      OGRLineString* castLineString = dynamic_cast<OGRLineString*>(geom);
      if (castLineString == NULL) break;
      this->ProcessLine(feature,castLineString,region,threadid);
      break;
      }
    case wkbPolygon:
    case wkbPolygon25D:
      {
      OGRPolygon* castPolygon = dynamic_cast<OGRPolygon*>(geom);
      if (castPolygon == NULL) break;
      this->ProcessPolygon(feature,castPolygon,region,threadid);
      break;
      }
    case wkbMultiPoint:
    case wkbMultiPoint25D:
    case wkbMultiLineString:
    case wkbMultiLineString25D:
    case wkbMultiPolygon:
    case wkbMultiPolygon25D:
    case wkbGeometryCollection:
    case wkbGeometryCollection25D:
      {
      OGRGeometryCollection *geomCollection = dynamic_cast<OGRGeometryCollection*>(geom);
      if (geomCollection)
        {
        unsigned int nbGeom =  geomCollection->getNumGeometries();
        for (unsigned int i=0 ; i < nbGeom ; ++i)
          {
          this->ExploreGeometry(feature,
                                geomCollection->getGeometryRef(i),
                                region,
                                threadid);
          }
        }
      else
        {
        otbWarningMacro("Geometry not recognized as a collection : " << geom->getGeometryName());
        }
      break;
      }
    default:
      {
      otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
      break;
      }
    }
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::ProcessLine(const ogr::Feature& feature,
              OGRLineString* line,
              RegionType& region,
              itk::ThreadIdType& threadid)
{
  OGRPolygon tmpPolygon;
  OGRLinearRing ring;
  ring.addPoint(0.0,0.0,0.0);
  ring.addPoint(1.0,0.0,0.0);
  ring.addPoint(1.0,1.0,0.0);
  ring.addPoint(0.0,1.0,0.0);
  ring.addPoint(0.0,0.0,0.0);
  tmpPolygon.addRing(&ring);
  const TInputImage* img = this->GetInput();
  TMaskImage* mask = const_cast<TMaskImage*>(this->GetMask());
  typename TInputImage::IndexType imgIndex;
  typename TInputImage::PointType imgPoint;
  typename TInputImage::SpacingType imgAbsSpacing = img->GetSpacing();
  if (imgAbsSpacing[0] < 0) imgAbsSpacing[0] = -imgAbsSpacing[0];
  if (imgAbsSpacing[1] < 0) imgAbsSpacing[1] = -imgAbsSpacing[1];

  if (mask)
    {
    // For pixels in consideredRegion and not masked
    typedef MaskedIteratorDecorator<
      itk::ImageRegionConstIterator<TMaskImage>,
      itk::ImageRegionConstIterator<TMaskImage> > MaskedIteratorType;
    MaskedIteratorType it(mask, mask, region);
    it.GoToBegin();
    while (!it.IsAtEnd())
      {
      imgIndex = it.GetIndex();
      img->TransformIndexToPhysicalPoint(imgIndex,imgPoint);
      bool isInside = this->IsSampleOnLine(line,imgPoint,imgAbsSpacing,tmpPolygon);
      if (isInside)
        {
        this->ProcessSample(feature,imgIndex, imgPoint, threadid);
        }
      ++it;
      }
    }
  else
    {
    typedef itk::ImageRegionConstIteratorWithOnlyIndex<TInputImage> NoValueIteratorType;
    NoValueIteratorType it(img,region);
    it.GoToBegin();
    while (!it.IsAtEnd())
      {
      imgIndex = it.GetIndex();
      img->TransformIndexToPhysicalPoint(imgIndex,imgPoint);
      bool isInside = this->IsSampleOnLine(line,imgPoint,imgAbsSpacing,tmpPolygon);
      if (isInside)
        {
        this->ProcessSample(feature,imgIndex, imgPoint, threadid);
        }
      ++it;
      }
    }
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::ProcessPolygon(const ogr::Feature& feature,
                 OGRPolygon* polygon,
                 RegionType& region,
                 itk::ThreadIdType& threadid)
{
  const TInputImage* img = this->GetInput();
  TMaskImage* mask = const_cast<TMaskImage*>(this->GetMask());
  typename TInputImage::IndexType imgIndex;
  typename TInputImage::PointType imgPoint;
  OGRPoint tmpPoint;

  if (mask)
    {
    // For pixels in consideredRegion and not masked
    typedef MaskedIteratorDecorator<
      itk::ImageRegionConstIterator<TMaskImage>,
      itk::ImageRegionConstIterator<TMaskImage> > MaskedIteratorType;
    MaskedIteratorType it(mask, mask, region);
    it.GoToBegin();
    while (!it.IsAtEnd())
      {
      imgIndex = it.GetIndex();
      img->TransformIndexToPhysicalPoint(imgIndex,imgPoint);
      tmpPoint.setX(imgPoint[0]);
      tmpPoint.setY(imgPoint[1]);
      bool isInside = this->IsSampleInsidePolygon(polygon,&tmpPoint);
      if (isInside)
        {
        this->ProcessSample(feature,imgIndex, imgPoint, threadid);
        }
      ++it;
      }
    }
  else
    {
    typedef itk::ImageRegionConstIteratorWithOnlyIndex<TInputImage> NoValueIteratorType;
    NoValueIteratorType it(img,region);
    it.GoToBegin();
    while (!it.IsAtEnd())
      {
      imgIndex = it.GetIndex();
      img->TransformIndexToPhysicalPoint(imgIndex,imgPoint);
      tmpPoint.setX(imgPoint[0]);
      tmpPoint.setY(imgPoint[1]);
      bool isInside = this->IsSampleInsidePolygon(polygon,&tmpPoint);
      if (isInside)
        {
        this->ProcessSample(feature,imgIndex, imgPoint, threadid);
        }
      ++it;
      }
    }  
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::ProcessSample(const ogr::Feature& ,
                typename TInputImage::IndexType& ,
                typename TInputImage::PointType& ,
                itk::ThreadIdType& )
{
  itkExceptionMacro("Method ProcessSample not implemented !");
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::PrepareFeature(const ogr::Feature&,
                 itk::ThreadIdType&)
{
  // Nothing to do here
}

template <class TInputImage, class TMaskImage>
inline bool
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::IsSampleInsidePolygon(OGRPolygon* poly,
                        OGRPoint* tmpPoint)
{
  bool ret = poly->getExteriorRing()->isPointInRing(tmpPoint);
  if (ret)
    {
    for (unsigned int k=0 ; k<poly->getNumInteriorRings() ; k++)
      {
      if (poly->getInteriorRing(k)->isPointInRing(tmpPoint))
        {
        ret = false;
        break;
        }
      }
    }
  return ret;
}

template <class TInputImage, class TMaskImage>
inline bool
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::IsSampleOnLine(OGRLineString* line,
                 typename TInputImage::PointType& position,
                 typename TInputImage::SpacingType& absSpacing,
                 OGRPolygon& tmpPolygon)
{
  tmpPolygon.getExteriorRing()->setPoint(0
    ,position[0]-0.5*absSpacing[0]
    ,position[1]-0.5*absSpacing[1]
    ,0.0);
  tmpPolygon.getExteriorRing()->setPoint(1
    ,position[0]+0.5*absSpacing[0]
    ,position[1]-0.5*absSpacing[1]
    ,0.0);
  tmpPolygon.getExteriorRing()->setPoint(2
    ,position[0]+0.5*absSpacing[0]
    ,position[1]+0.5*absSpacing[1]
    ,0.0);
  tmpPolygon.getExteriorRing()->setPoint(3
    ,position[0]-0.5*absSpacing[0]
    ,position[1]+0.5*absSpacing[1]
    ,0.0);
  tmpPolygon.getExteriorRing()->setPoint(4
    ,position[0]-0.5*absSpacing[0]
    ,position[1]-0.5*absSpacing[1]
    ,0.0);
  return line->Intersects(&tmpPolygon);
}

template<class TInputImage, class TMaskImage>
typename PersistentSamplingFilterBase<TInputImage,TMaskImage>::RegionType
PersistentSamplingFilterBase<TInputImage,TMaskImage>
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

template<class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::PrepareInputVectors()
{
  TInputImage* outputImage = this->GetOutput();
  ogr::DataSource* vectors = const_cast<ogr::DataSource*>(this->GetOGRData());
  const RegionType& requestedRegion = outputImage->GetRequestedRegion();
  typename TInputImage::IndexType startIndex = requestedRegion.GetIndex();
  typename TInputImage::IndexType endIndex = requestedRegion.GetUpperIndex();

  itk::Point<double, 2> startPoint;
  itk::Point<double, 2> endPoint;

  outputImage->TransformIndexToPhysicalPoint(startIndex, startPoint);
  outputImage->TransformIndexToPhysicalPoint(endIndex, endPoint);

  // create geometric extent
  OGRPolygon tmpPolygon;
  OGRLinearRing ring;
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  ring.addPoint(startPoint[0],endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,startPoint[1],0.0);
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  tmpPolygon.addRing(&ring);

  int numberOfThreads = this->GetNumberOfThreads();

  // prepare temporary input : split input features between available threads
  this->m_InMemoryInputs.clear();
  ogr::Layer inLayer = vectors->GetLayer(m_LayerIndex);
  std::string tmpLayerName("thread");
  OGRSpatialReference * oSRS = NULL;
  if (inLayer.GetSpatialRef())
    {
    oSRS = inLayer.GetSpatialRef()->Clone();
    }
  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  std::vector<ogr::Layer> tmpLayers;
  for (unsigned int i=0 ; i < numberOfThreads ; i++)
    {
    ogr::DataSource::Pointer tmpOgrDS = ogr::DataSource::New();
    ogr::Layer tmpLayer = tmpOgrDS->CreateLayer(
      tmpLayerName,
      oSRS,
      inLayer.GetGeomType());
    // add field definitions
    for (unsigned int k=0 ; k < layerDefn.GetFieldCount() ; k++)
      {
      OGRFieldDefn originDefn(layerDefn.GetFieldDefn(k));
      ogr::FieldDefn fieldDefn(originDefn);
      tmpLayer.CreateField(fieldDefn);
      }
    this->m_InMemoryInputs.push_back(tmpOgrDS);
    tmpLayers.push_back(tmpLayer);
    }

  ogr::Layer::const_iterator featIt = inLayer.begin();
  int counter=0;
  for(; featIt!=inLayer.end(); ++featIt)
    {
    ogr::Feature dstFeature(layerDefn);
    dstFeature.SetFrom( *featIt, TRUE );
    dstFeature.SetFID(featIt->GetFID());
    tmpLayers[counter].CreateFeature( dstFeature );
    counter++;
    if (counter >= numberOfThreads)
      counter = 0;
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::PrepareOutputVectors()
{
  // Prepare in-memory outputs
  this->m_InMemoryOutputs.clear();
  for (unsigned int i=0 ; i < this->GetNumberOfThreads() ; i++)
    {
    std::vector<OGRDataPointer> tmpContainer;
    // iterate over outputs, only process ogr::DataSource
    for (unsigned int k=0 ; k < this->GetNumberOfOutputs() ; k++)
      {
      ogr::DataSource* realOutput = dynamic_cast<ogr::DataSource *>(
        this->itk::ProcessObject::GetOutput(k));
      if (realOutput)
        {
        ogr::Layer realLayer = realOutput->GetLayersCount() == 1
                               ? realOutput->GetLayer(0)
                               : realOutput->GetLayer(m_LayerName);
        OGRSpatialReference * oSRS = NULL;
        if (realLayer.GetSpatialRef())
          {
          oSRS = realLayer.GetSpatialRef()->Clone();
          }
        OGRFeatureDefn &layerDefn = realLayer.GetLayerDefn();
        ogr::DataSource::Pointer tmpOutput = ogr::DataSource::New();
        ogr::Layer tmpLayer = tmpOutput->CreateLayer(
          m_LayerName, oSRS,  realLayer.GetGeomType());
        // add field definitions
        for (unsigned int f=0 ; f < layerDefn.GetFieldCount() ; f++)
          {
          OGRFieldDefn originDefn(layerDefn.GetFieldDefn(f));
          tmpLayer.CreateField(originDefn);
          }
        tmpContainer.push_back(tmpOutput);
        }
      }
    this->m_InMemoryOutputs.push_back(tmpContainer);
    }
}

} // end namespace otb

#endif
