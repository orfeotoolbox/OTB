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
#ifndef otbPersistentSamplingFilterBase_txx
#define otbPersistentSamplingFilterBase_txx

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
  , m_FieldIndex(0)  
  , m_LayerIndex(0)
  , m_OutLayerName(std::string("output"))
  , m_OGRLayerCreationOptions()
  , m_AdditionalFields()
{
  this->SetNthOutput(0,TInputImage::New());
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
const std::vector<std::string>&
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::GetOGRLayerCreationOptions()
{
  return m_OGRLayerCreationOptions;
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
::GenerateData(void)
{
  this->AllocateOutputs();
  this->BeforeThreadedGenerateData();

  // Split the data into in-memory layers
  this->DispatchInputVectors();

  // struct to store filter pointer
  VectorThreadStruct str;
  str.Filter = this;

  // Get the output pointer
  //const InputImageType *outputPtr = this->GetOutput();

  this->GetMultiThreader()->SetNumberOfThreads( this->GetNumberOfThreads() );
  this->GetMultiThreader()->SetSingleMethod(this->VectorThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // gather the data from in-memory output layers
  this->GatherOutputVectors();

  this->AfterThreadedGenerateData();
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::AllocateOutputs(void)
{
  Superclass::AllocateOutputs();

  ogr::DataSource* vectors = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::Layer inLayer = vectors->GetLayer(m_LayerIndex);

  unsigned int numberOfThreads = this->GetNumberOfThreads();

  // Prepare temporary input
  this->m_InMemoryInputs.clear();
  std::string tmpLayerName("thread");
  OGRSpatialReference * oSRS = ITK_NULLPTR;
  if (inLayer.GetSpatialRef())
    {
    oSRS = inLayer.GetSpatialRef()->Clone();
    }
  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  //std::vector<ogr::Layer> tmpLayers;
  for (unsigned int i=0 ; i < numberOfThreads ; i++)
    {
    ogr::DataSource::Pointer tmpOgrDS = ogr::DataSource::New();
    ogr::Layer tmpLayer = tmpOgrDS->CreateLayer(
      tmpLayerName,
      oSRS,
      inLayer.GetGeomType());
    // add field definitions
    for (int k=0 ; k < layerDefn.GetFieldCount() ; k++)
      {
      OGRFieldDefn originDefn(layerDefn.GetFieldDefn(k));
      ogr::FieldDefn fieldDefn(originDefn);
      tmpLayer.CreateField(fieldDefn);
      }
    this->m_InMemoryInputs.push_back(tmpOgrDS);
    //tmpLayers.push_back(tmpLayer);
    }

  // Prepare in-memory outputs
  this->m_InMemoryOutputs.clear();
  tmpLayerName = std::string("threadOut");
  for (unsigned int i=0 ; i < numberOfThreads ; i++)
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
                               : realOutput->GetLayer(m_OutLayerName);
        OGRFeatureDefn &outLayerDefn = realLayer.GetLayerDefn();
        ogr::DataSource::Pointer tmpOutput = ogr::DataSource::New();
        ogr::Layer tmpLayer = tmpOutput->CreateLayer(
          tmpLayerName, oSRS,  realLayer.GetGeomType());
        // add field definitions
        for (int f=0 ; f < outLayerDefn.GetFieldCount() ; f++)
          {
          OGRFieldDefn originDefn(outLayerDefn.GetFieldDefn(f));
          tmpLayer.CreateField(originDefn);
          }
        tmpContainer.push_back(tmpOutput);
        }
      }
    this->m_InMemoryOutputs.push_back(tmpContainer);
    }
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::GatherOutputVectors(void)
{
  // clean temporary inputs
  this->m_InMemoryInputs.clear();

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  
  // gather temporary outputs and write to output
  const otb::ogr::DataSource* vectors = this->GetOGRData();
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
                            : realOutput->GetLayer(m_OutLayerName);

      OGRErr err = outLayer.ogr().StartTransaction();
      if (err != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
        }
  
      for (unsigned int thread=0 ; thread < numberOfThreads ; thread++)
        {
        ogr::Layer inLayer = this->m_InMemoryOutputs[thread][count]->GetLayerChecked(0);
        if (!inLayer)
          {
          continue;
          }
  
        ogr::Layer::const_iterator tmpIt = inLayer.begin();
        // This test only uses 1 input, not compatible with multiple OGRData inputs
        if (vectors == realOutput)
          {
          // Update mode
          for(; tmpIt!=inLayer.end(); ++tmpIt)
            {
            outLayer.SetFeature( *tmpIt );
            }
          }
        else
          {
          // Copy mode
          for(; tmpIt!=inLayer.end(); ++tmpIt)
            {
            ogr::Feature dstFeature(outLayer.GetLayerDefn());
            dstFeature.SetFrom( *tmpIt, TRUE );
            outLayer.CreateFeature( dstFeature );
            }
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
  this->m_InMemoryOutputs.clear();
}

template <class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid)
{
  // Retrieve inputs
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  TInputImage* outputImage = this->GetOutput();
  RegionType requestedRegion = outputImage->GetRequestedRegion();

  itk::ProgressReporter progress( this, threadid, layerForThread.GetFeatureCount(true) );

  // Loop across the features in the layer (filtered by requested region in BeforeTGD already)
  ogr::Layer::const_iterator featIt = layerForThread.begin();
  for(; featIt!=layerForThread.end(); ++featIt)
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
      if (castPoint == ITK_NULLPTR) break;
      
      imgPoint[0] = castPoint->getX();
      imgPoint[1] = castPoint->getY();
      const TInputImage* img = this->GetInput();
      const TMaskImage* mask = this->GetMask(); 
      img->TransformPhysicalPointToIndex(imgPoint,imgIndex);
      if ((mask == ITK_NULLPTR) || mask->GetPixel(imgIndex))
        {
        this->ProcessSample(feature, imgIndex, imgPoint, threadid);
        }
      break;
      }
    case wkbLineString:
    case wkbLineString25D:
      {
      OGRLineString* castLineString = dynamic_cast<OGRLineString*>(geom);

      if (castLineString == ITK_NULLPTR) break;
      this->ProcessLine(feature,castLineString,region,threadid);
      break;
      }
    case wkbPolygon:
    case wkbPolygon25D:
      {
      OGRPolygon* castPolygon = dynamic_cast<OGRPolygon*>(geom);
      if (castPolygon == ITK_NULLPTR) break;
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
    for (int k=0 ; k<poly->getNumInteriorRings() ; k++)
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
::DispatchInputVectors()
{
  TInputImage* outputImage = this->GetOutput();
  ogr::DataSource* vectors = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::Layer inLayer = vectors->GetLayer(m_LayerIndex);

  const RegionType& requestedRegion = outputImage->GetRequestedRegion();
  itk::ContinuousIndex<double> startIndex(requestedRegion.GetIndex());
  itk::ContinuousIndex<double> endIndex(requestedRegion.GetUpperIndex());
  startIndex[0] += -0.5;
  startIndex[1] += -0.5;
  endIndex[0] += 0.5;
  endIndex[1] += 0.5;
  itk::Point<double, 2> startPoint;
  itk::Point<double, 2> endPoint;
  outputImage->TransformContinuousIndexToPhysicalPoint(startIndex, startPoint);
  outputImage->TransformContinuousIndexToPhysicalPoint(endIndex, endPoint);

  // create geometric extent
  OGRPolygon tmpPolygon;
  OGRLinearRing ring;
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  ring.addPoint(startPoint[0],endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,startPoint[1],0.0);
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  tmpPolygon.addRing(&ring);

  inLayer.SetSpatialFilter(&tmpPolygon);

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  std::vector<ogr::Layer> tmpLayers;
  for (unsigned int i=0 ; i<numberOfThreads ; i++)
    {
    tmpLayers.push_back(this->GetInMemoryInput(i));
    }
  
  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  ogr::Layer::const_iterator featIt = inLayer.begin();
  unsigned int counter=0;
  for(; featIt!=inLayer.end(); ++featIt)
    {
    ogr::Feature dstFeature(layerDefn);
    dstFeature.SetFrom( *featIt, TRUE );
    dstFeature.SetFID(featIt->GetFID());
    tmpLayers[counter].CreateFeature( dstFeature );
    counter++;
    if (counter >= tmpLayers.size())
      counter = 0;
    }

  inLayer.SetSpatialFilter(ITK_NULLPTR);
}

template<class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::InitializeOutputDataSource(ogr::DataSource* inputDS, ogr::DataSource* outputDS)
{
  TInputImage *inputImage = const_cast<TInputImage*>(this->GetInput());
  inputImage->UpdateOutputInformation();
  
  ogr::Layer inLayer = inputDS->GetLayer(this->GetLayerIndex());

  bool updateMode = false;
  if (inputDS == outputDS)
    {
    updateMode = true;
    // Check m_OutLayerName is same as input layer name 
    m_OutLayerName = inLayer.GetName();
    }

  // First get list of current fields
  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  std::map<std::string, OGRFieldType> currentFields;
  for (int k=0 ; k<layerDefn.GetFieldCount() ; k++)
    {
    OGRFieldDefn fieldDefn(layerDefn.GetFieldDefn(k));
    std::string currentName(fieldDefn.GetNameRef());
    currentFields[currentName] = fieldDefn.GetType();
    }

  ogr::Layer outLayer = inLayer;
  if (!updateMode)
    {
    std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
    bool projectionInformationAvailable = !projectionRefWkt.empty();
    OGRSpatialReference * oSRS = ITK_NULLPTR;
    if(projectionInformationAvailable)
      {
      oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
      }
    // Create layer
    outLayer = outputDS->CreateLayer(
      this->GetOutLayerName(),
      oSRS,
      wkbPoint,
      this->GetOGRLayerCreationOptions());
    // Copy existing fields
    for (int k=0 ; k<layerDefn.GetFieldCount() ; k++)
      {
      OGRFieldDefn fieldDefn(layerDefn.GetFieldDefn(k));
      outLayer.CreateField(fieldDefn);
      }
    }

  // Add new fields
  for (unsigned int k=0 ; k<m_AdditionalFields.size() ; k++)
    {
    OGRFieldDefn ogrFieldDefinition(m_AdditionalFields[k].Name.c_str(),m_AdditionalFields[k].Type);
    ogrFieldDefinition.SetWidth( m_AdditionalFields[k].Width );
    ogrFieldDefinition.SetPrecision( m_AdditionalFields[k].Precision );
    ogr::FieldDefn fieldDef(ogrFieldDefinition);
    // test if field is already present
    if (currentFields.count(fieldDef.GetName()))
      {
      // test the field type
      if (currentFields[fieldDef.GetName()] != fieldDef.GetType())
        {
        itkExceptionMacro("Field name "<< fieldDef.GetName() << " already exists with a different type!");
        }
      }
    else
      {
      outLayer.CreateField(fieldDef);
      }
    }
}


template<class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::ClearAdditionalFields()
{
  this->m_AdditionalFields.clear();
}

template<class TInputImage, class TMaskImage>
void
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::CreateAdditionalField(std::string name,
                        OGRFieldType type,
                        int width,
                        int precision)
{
  SimpleFieldDefn defn;
  defn.Name = name;
  defn.Type = type;
  defn.Width = width;
  defn.Precision = precision;
  this->m_AdditionalFields.push_back(defn);
}

template<class TInputImage, class TMaskImage>
const std::vector<
  typename PersistentSamplingFilterBase<TInputImage,TMaskImage>
    ::SimpleFieldDefn>&
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::GetAdditionalFields()
{
  return this->m_AdditionalFields;
}

template<class TInputImage, class TMaskImage>
ITK_THREAD_RETURN_TYPE
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::VectorThreaderCallback(void *arg)
{
  VectorThreadStruct *str = (VectorThreadStruct*)(((itk::MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  int threadId = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;
  int threadCount = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->NumberOfThreads;

  ogr::Layer layer = str->Filter->GetInMemoryInput(threadId);

  if (threadId < threadCount)
    {
    str->Filter->ThreadedGenerateVectorData(layer,threadId);
    }

  return ITK_THREAD_RETURN_VALUE;
}

template<class TInputImage, class TMaskImage>
ogr::Layer
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::GetInMemoryInput(unsigned int threadId)
{
  if (threadId >= m_InMemoryInputs.size())
    {
    itkExceptionMacro(<< "Requested in-memory input layer not available " << threadId << " (total size : "<< m_InMemoryInputs.size() <<").");
    }
  return m_InMemoryInputs[threadId]->GetLayerChecked(0);
}

template<class TInputImage, class TMaskImage>
ogr::Layer
PersistentSamplingFilterBase<TInputImage,TMaskImage>
::GetInMemoryOutput(unsigned int threadId, unsigned int index)
{
  if (threadId >= m_InMemoryOutputs.size())
    {
    itkExceptionMacro(<< "Requested in-memory output layer not available " << threadId << " (total size : "<< m_InMemoryOutputs.size() <<").");
    }
  if (index >= m_InMemoryOutputs[threadId].size())
    {
    itkExceptionMacro(<< "Requested output dataset not available " << index << " (available : "<< m_InMemoryOutputs[threadId].size() <<").");
    }
  return m_InMemoryOutputs[threadId][index]->GetLayerChecked(0);
}

} // end namespace otb

#endif
