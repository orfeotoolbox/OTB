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
#include "itkTimeProbe.h"
#include "otbOGRIOHelper.h"
#include "otbPolygon.h"
#include "itkProgressReporter.h"

namespace otb
{
// --------- otb::PersistentOGRDataToSamplePositionFilter ---------------------

template<class TInputImage, class TMaskImage, class TSampler>
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::PersistentOGRDataToSamplePositionFilter() :
   m_FieldIndex(0),
   m_LayerIndex(0)
{
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TInputImage::New());
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage, class TMaskImage, class TSampler>
const otb::ogr::DataSource*
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOGRData()
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const otb::ogr::DataSource *>(this->itk::ProcessObject::GetInput(1));
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetMask(const TMaskImage* mask)
{
  this->SetNthInput(2, const_cast<TMaskImage *>( mask ));
}

template<class TInputImage, class TMaskImage, class TSampler>
const TMaskImage*
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetMask()
{
  if (this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(2));
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::Synthetize(void)
{
  // clear temporary outputs
  this->m_InMemoryOutputs.clear();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::Reset(void)
{
  // Reset samplers
  for (unsigned int i=0 ; i < m_Samplers.size() ; ++i)
    {
    for (typename SamplerMapType::iterator iter = m_Samplers[i].begin();
         iter != m_Samplers[i].end();
         ++iter)
      {
      iter->second->Reset();
      }
    }

  // compute label mapping
  this->ComputeClassPartition();

  // Get OGR field index
  const otb::ogr::DataSource* vectors = this->GetOGRData();
  m_LayerName = vectors->GetLayer(m_LayerIndex).GetName();
  otb::ogr::Layer::const_iterator featIt = vectors->GetLayer(m_LayerIndex).begin();
  m_FieldIndex = featIt->ogr().GetFieldIndex(this->m_FieldName.c_str());
  if (m_FieldIndex < 0)
    {
    itkGenericExceptionMacro("Field named "<<this->m_FieldName<<" not found!");
    }

  // Prepare outputs
  for (unsigned int k=0 ; k < this->GetNumberOfLevels() ; k++)
    {
    otb::ogr::DataSource* output = this->GetOutputPositionContainer(k);
    if (output)
      {
      std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
      bool projectionInformationAvailable = !projectionRefWkt.empty();
      OGRSpatialReference * oSRS = NULL;
      if(projectionInformationAvailable)
        {
        oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
        }

      output->CreateLayer(m_LayerName, oSRS ,wkbPoint, m_OGRLayerCreationOptions);
      OGRFieldDefn field(m_FieldName.c_str(),OFTString);

      //Handle the case of shapefile. A shapefile is a layer and not a datasource.
      //The layer name in a shapefile is the shapefile's name.
      //This is not the case for a database as sqlite or PG.
      if (output->GetLayersCount() == 1)
        {
        output->GetLayer(0).CreateField(field, true);
        }
      else
        {
        output->GetLayer(m_LayerName).CreateField(field, true);
        }
      }
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOGRLayerCreationOptions(const std::vector<std::string> & options)
{
  m_OGRLayerCreationOptions.clear();
  m_OGRLayerCreationOptions = options;
}

template<class TInputImage, class TMaskImage, class TSampler>
typename PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>::SamplerMapType&
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetSamplers(unsigned int level)
{
  if (m_Samplers.size() < (level + 1))
    {
    itkExceptionMacro("The sampler level "<< level << " doesn't exist !");
    }
  return m_Samplers[level];
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOutputPositionContainerAndRates(
    otb::ogr::DataSource* data,
    const SamplingRateCalculator::MapRateType& map,
    unsigned int level)
{
  // set the output dataset
  this->SetNthOutput(1+level,data);
  // instanciate the corresponding samplers
  if (m_Samplers.size() < (level + 1))
    {
    m_Samplers.resize(level + 1);
    }
  m_Samplers[level].clear();
  for (SamplingRateCalculator::MapRateType::const_iterator it = map.begin() ;
       it != map.end();
       ++it)
    {
    SamplerPointerType sampler = SamplerType::New();
    sampler->SetNumberOfElements(it->second.Required,it->second.Tot);
    m_Samplers[level][it->first] = sampler;
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
otb::ogr::DataSource*
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOutputPositionContainer(unsigned int level)
{
  if (this->GetNumberOfOutputs() < (level + 2))
    {
    return 0;
    }
  return static_cast<otb::ogr::DataSource *>(this->itk::ProcessObject::GetOutput(level + 1));
}

template<class TInputImage, class TMaskImage, class TSampler>
unsigned int
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetNumberOfLevels()
{
  return (this->GetNumberOfOutputs() - 1);
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::ClearOutputs()
{
  // remove output position containers
  for (unsigned int i = (this->GetNumberOfOutputs() - 1) ; i > 0 ; --i)
    {
    this->itk::ProcessObject::RemoveOutput(i);
    }
  // remove samplers
  m_Samplers.clear();
}

template<class TInputImage, class TMaskImage, class TSampler>
itk::DataObject::Pointer
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  if (idx == 0)
    {
    // the output image
    return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
    }
  else if (idx > 0)
    {
    // sample position containers
    return static_cast<itk::DataObject*>(otb::ogr::DataSource::New().GetPointer());
    }
  // might as well make an image
  return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
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

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
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

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::BeforeThreadedGenerateData(void)
{
  // apply spatial filter and fill InMemoryInput
  this->ApplyPolygonsSpatialFilter();

  // Prepare in-memory outputs
  this->m_InMemoryOutputs.clear();
  for (unsigned int i=0 ; i < this->GetNumberOfThreads() ; i++)
    {
    std::vector<OGRDataPointer> tmpContainer;
    for (unsigned int k=0 ; k < this->GetNumberOfLevels() ; k++)
      {
      ogr::DataSource::Pointer tmpOutput = ogr::DataSource::New();
      ogr::DataSource *realOutput = this->GetOutputPositionContainer(k);
      ogr::Layer realLayer = realOutput->GetLayersCount() == 1
                             ? realOutput->GetLayer(0)
                             : realOutput->GetLayer(m_LayerName);
      OGRSpatialReference * oSRS = NULL;
      if (realLayer.GetSpatialRef())
        {
        oSRS = realLayer.GetSpatialRef()->Clone();
        }

      tmpOutput->CreateLayer(m_LayerName, oSRS ,wkbPoint, m_OGRLayerCreationOptions);
      OGRFieldDefn field(m_FieldName.c_str(),OFTString);
      tmpOutput->GetLayer(0).CreateField(field, true);
      tmpContainer.push_back(tmpOutput);
      }
    this->m_InMemoryOutputs.push_back(tmpContainer);
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::AfterThreadedGenerateData(void)
{
  // clean temporary inputs
  this->m_InMemoryInputs.clear();

  // gather temporary outputs and write to output
  itk::TimeProbe chrono;
  chrono.Start();
  for (unsigned int k=0 ; k < this->GetNumberOfLevels() ; k++)
    {
    ogr::DataSource *realOutput = this->GetOutputPositionContainer(k);
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
      ogr::Layer inLayer = this->m_InMemoryOutputs[thread][k]->GetLayerChecked(0);
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
    }
  chrono.Stop();
  otbMsgDebugMacro(<< "write ogr points took " << chrono.GetTotal() << " sec");
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::ThreadedGenerateData(const RegionType&, itk::ThreadIdType threadid)
{
  // Retrieve inputs
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  TInputImage* outputImage = this->GetOutput();
  TMaskImage* mask = const_cast<TMaskImage*>(this->GetMask());
  PointType point;
  RegionType requestedRegion =outputImage->GetRequestedRegion();

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
        if (mask)
          {
          // For pixels in consideredRegion and not masked
          typedef MaskedIteratorDecorator<
            itk::ImageRegionConstIterator<TMaskImage>,
            itk::ImageRegionConstIterator<TMaskImage> > MaskedIteratorType;
          MaskedIteratorType it(mask, mask, consideredRegion);
          this->Add<MaskedIteratorType>(featIt, it, mask, threadid);
          }
        else
          {
          typedef itk::ImageRegionConstIteratorWithOnlyIndex<TInputImage> NoValueIteratorType;
          NoValueIteratorType it(inputImage,consideredRegion);
          this->Add<NoValueIteratorType>(featIt, it, inputImage, threadid);
          }
      }
    progress.CompletedPixel();
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::ApplyPolygonsSpatialFilter()
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

  // prepare temporary input
  this->m_InMemoryInputs.clear();
  std::ostringstream oss;
  for (unsigned int i=0 ; i < numberOfThreads ; i++)
    {
    oss.str("");
    oss << "SELECT * FROM \"" << m_LayerName << "\" ";
    oss << "WHERE "<<m_FieldName<<" IN (";
    bool isFirst = true;
    for (ClassPartitionType::iterator iter = this->m_ClassPartition.begin() ;
         iter != this->m_ClassPartition.end() ;
         ++iter)
      {
      if (iter->second == i)
        {
        if (isFirst)
          {
          isFirst = false;
          }
        else
          {
          oss << ",";
          }
        oss <<"'"<<iter->first<<"'";
        }
      }
    oss << ")";
    OGRDataPointer tmpInput = ogr::DataSource::New();
    std::string newLayerName("thread");
    if (isFirst)
      {
      // no class to process for this thread : store an empty layer
      tmpInput->CreateLayer(newLayerName);
      }
    else
      {
      // Extract classes to process in this thread
      ogr::Layer search = vectors->ExecuteSQL(oss.str().c_str(), &tmpPolygon, NULL);
      tmpInput->CopyLayer(search,newLayerName);
      }
    this->m_InMemoryInputs.push_back(tmpInput);
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
typename PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>::RegionType
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
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

template<class TInputImage, class TMaskImage, class TSampler>
template <typename TIterator>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::Add(otb::ogr::Layer::const_iterator& featIt,
      TIterator& imgIt,
      const typename TIterator::ImageType *img,
      itk::ThreadIdType& threadid)
{
  // Get class name
  std::string className(featIt->ogr().GetFieldAsString(this->m_FieldIndex));

  // Get Feature Id
  unsigned long featureId = featIt->ogr().GetFID();
  this->AddGeometry(featIt->ogr().GetGeometryRef(),
                    imgIt,
                    img,
                    featureId,
                    className,
                    threadid);
}

template<class TInputImage, class TMaskImage, class TSampler>
template <typename TIterator>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::AddGeometry(OGRGeometry *geom,
              TIterator& imgIt,
              const typename TIterator::ImageType *img,
              unsigned long &fId,
              std::string &className,
              itk::ThreadIdType& threadid)
{
  typename TIterator::ImageType::PointType imgPoint;
  typename TIterator::IndexType imgIndex;
  imgIt.GoToBegin();
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
      img->TransformPhysicalPointToIndex(imgPoint,imgIndex);
      while (!imgIt.IsAtEnd())
        {
        if (imgIndex == imgIt.GetIndex())
          {
          this->CallSamplers(imgPoint, className, threadid);
          break;
          }
        ++imgIt;
        }
      break;
      }
    case wkbLineString:
    case wkbLineString25D:
      {
      OGRPolygon tmpPolygon;
      OGRLinearRing ring;
      ring.addPoint(0.0,0.0,0.0);
      ring.addPoint(1.0,0.0,0.0);
      ring.addPoint(1.0,1.0,0.0);
      ring.addPoint(0.0,1.0,0.0);
      ring.addPoint(0.0,0.0,0.0);
      tmpPolygon.addRing(&ring);
      typename TIterator::ImageType::SpacingType imgAbsSpacing = img->GetSpacing();
      if (imgAbsSpacing[0] < 0) imgAbsSpacing[0] = -imgAbsSpacing[0];
      if (imgAbsSpacing[1] < 0) imgAbsSpacing[1] = -imgAbsSpacing[1];
      while (!imgIt.IsAtEnd())
        {
        img->TransformIndexToPhysicalPoint(imgIt.GetIndex(),imgPoint);
        tmpPolygon.getExteriorRing()->setPoint(0
          ,imgPoint[0]-0.5*imgAbsSpacing[0]
          ,imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(1
          ,imgPoint[0]+0.5*imgAbsSpacing[0]
          ,imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(2
          ,imgPoint[0]+0.5*imgAbsSpacing[0]
          ,imgPoint[1]+0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(3
          ,imgPoint[0]-0.5*imgAbsSpacing[0]
          ,imgPoint[1]+0.5*imgAbsSpacing[1]
          ,0.0);
        tmpPolygon.getExteriorRing()->setPoint(4
          ,imgPoint[0]-0.5*imgAbsSpacing[0]
          ,imgPoint[1]-0.5*imgAbsSpacing[1]
          ,0.0);
        if (geom->Intersects(&tmpPolygon))
          this->CallSamplers(imgPoint, className, threadid);
        ++imgIt;
        }
      break;
      }
    case wkbPolygon:
    case wkbPolygon25D:
      {
      OGRIOHelper::Pointer OGRConversion = OGRIOHelper::New();
      typedef otb::VectorData<>::DataNodeType   DataNodeType;
      typedef DataNodeType::PolygonType         PolygonType;
      DataNodeType::Pointer dataNode = DataNodeType::New();
      OGRConversion->ConvertGeometryToPolygonNode(geom,dataNode);
      PolygonType::VertexType vertex;

      while (!imgIt.IsAtEnd())
        {
        img->TransformIndexToPhysicalPoint(imgIt.GetIndex(),imgPoint);
        vertex[0] = imgPoint[0];
        vertex[1] = imgPoint[1];
        bool isInside = dataNode->GetPolygonExteriorRing()->IsInside(vertex);
        if (isInside)
          {
          for (unsigned int k=0 ;
               k < dataNode->GetPolygonInteriorRings()->Size() ;
               k++)
            {
            if (dataNode->GetPolygonInteriorRings()->GetNthElement(k)->IsInside(vertex))
              {
              isInside = false;
              break;
              }
            }
          }
        if (isInside)
          {
          this->CallSamplers(imgPoint, className, threadid);
          }
        ++imgIt;
        }
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
          this->AddGeometry(geomCollection->getGeometryRef(i),
                            imgIt,
                            img,
                            fId,
                            className,
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


template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::CallSamplers(const PointType &point,
               const std::string &className,
               itk::ThreadIdType& threadid)
{
  for (unsigned int i=0 ; i<this->GetNumberOfLevels() ; ++i)
    {
    if (m_Samplers[i][className]->TakeSample())
      {
      OGRPoint ogrTmpPoint;
      ogrTmpPoint.setX(point[0]);
      ogrTmpPoint.setY(point[1]);

      ogr::Layer outputLayer = this->m_InMemoryOutputs[threadid][i]->GetLayerChecked(0);
      ogr::Feature feat(outputLayer.GetLayerDefn());
      feat[m_FieldName].SetValue(className);
      feat.SetGeometry(&ogrTmpPoint);
      outputLayer.CreateFeature(feat);
      break;
      }
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::ComputeClassPartition(void)
{
  m_ClassPartition.clear();
  typedef std::map<std::string, unsigned long> ClassCountMapType;
  ClassCountMapType classCounts;
  for (typename SamplerMapType::const_iterator it = m_Samplers[0].begin() ;
       it != m_Samplers[0].end();
       ++it)
    {
    classCounts[it->first] = it->second->GetTotalElements();
    }

  int numberOfThreads = this->GetNumberOfThreads();
  typedef std::vector<unsigned long> LoadVectorType;
  LoadVectorType currentLoad;
  currentLoad.resize(numberOfThreads, 0UL);
  
  ClassCountMapType::iterator largestClass;
  unsigned long minLoad;
  unsigned int destThread;
  while (classCounts.size())
    {
    // find class with highest number of total elements
    largestClass = classCounts.begin();
    for (ClassCountMapType::iterator iter = classCounts.begin() ;
         iter != classCounts.end() ;
         ++iter)
      {
      if (iter->second > largestClass->second)
        {
        largestClass = iter;
        }
      }

    // affect this class to the thread with lowest load
    minLoad = currentLoad[0];
    destThread = 0;
    for (unsigned int k = 0 ; k < currentLoad.size() ; ++k)
      {
      if (currentLoad[k] < minLoad)
        {
        minLoad = currentLoad[k];
        destThread = k;
        }
      }
    m_ClassPartition[largestClass->first] = destThread;
    currentLoad[destThread] += largestClass->second;

    // remove class from classCounts
    classCounts.erase(largestClass);
    }
}

// -------------- otb::OGRDataToSamplePositionFilter --------------------------

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TMaskImage, class TSampler>
const TInputImage*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TMaskImage, class TSampler>
const otb::ogr::DataSource*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TInputImage, class TMaskImage, class TSampler>
const TMaskImage*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetFieldName(std::string key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TInputImage, class TMaskImage, class TSampler>
std::string
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TInputImage, class TMaskImage, class TSampler>
int
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetSamplerParameters(SamplerParameterType param, unsigned int level)
{
  SamplerMapType &samplers = this->GetSamplers(level);
  for (typename SamplerMapType::iterator it = samplers.begin() ; it != samplers.end() ; ++it)
    {
    it->second->SetParameters(param);
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
typename OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>::SamplerMapType&
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetSamplers(unsigned int level)
{
  return this->GetFilter()->GetSamplers(level);
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOutputPositionContainerAndRates(
  otb::ogr::DataSource* data,
  const SamplingRateCalculator::MapRateType& map,
  unsigned int level)
{
  this->GetFilter()->SetOutputPositionContainerAndRates(data,map,level);
}

template<class TInputImage, class TMaskImage, class TSampler>
otb::ogr::DataSource*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOutputPositionContainer(unsigned int level)
{
  return this->GetFilter()->GetOutputPositionContainer(level);
}



} // end of namespace otb

#endif
