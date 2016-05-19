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

#include "itkDefaultConvertPixelTraits.h"

namespace otb
{
// --------- otb::PersistentImageSampleExtractorFilter ---------------------

template<class TInputImage>
PersistentImageSampleExtractorFilter<TInputImage>
::PersistentImageSampleExtractorFilter() :
  m_LayerIndex(0),
  m_SampleFieldPrefix(std::string("band_"))
{
  this->SetNumberOfRequiredInputs(3);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0,TInputImage::New());
  
  this->SetGeometryType(wkbPoint);
  // TODO : add setter for m_GeometryType in base class
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::SetSamplePositions(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(2, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TInputImage>
const otb::ogr::DataSource*
PersistentImageSampleExtractorFilter<TInputImage>
::GetSamplePositions()
{
  if (this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const ogr::DataSource *>(this->itk::ProcessObject::GetInput(2));
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::Synthetize(void)
{
  ogr::DataSource* vectors = const_cast<ogr::DataSource*>(this->GetSamplePositions());
  vectors->GetLayer(m_LayerIndex).SetSpatialFilter(NULL);
  
  // TODO ?
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::Reset(void)
{
  // initialize output DataSource if copy mode
  const ogr::DataSource* inVectors = this->GetSamplePositions();
  OGRFeatureDefn &inLayerDefn = inVectors->GetLayer(m_LayerIndex).GetLayerDefn();
  ogr::DataSource* outVectors = const_cast<ogr::DataSource*>(this->GetOGRDataSource());
  bool updateMode = bool(inVectors == outVectors);
  ogr::Layer outLayer = inVectors->GetLayer(m_LayerIndex); // has to be initialized with a real layer
  if (updateMode)
    {
    outLayer = outVectors->GetLayer(m_LayerIndex);
    }
  else
    {
    // use the same field type for class label
    int inCFieldIndex = inLayerDefn.GetFieldIndex(this->GetFieldName().c_str());
    this->SetFieldType(inLayerDefn.GetFieldDefn(inCFieldIndex)->GetType());
    // Create layer
    this->Initialize();
    // Get created layer. Handle the case of shapefile, which is a layer and not a datasource.
    //The layer name in a shapefile is the shapefile's name.
    //This is not the case for a database as sqlite or PG.
    if (outVectors->GetLayersCount() == 1)
      {
      outLayer = outVectors->GetLayer(0);
      }
    else
      {
      outLayer = outVectors->GetLayer(this->GetLayerName());
      }
    }

  // initialize fields in output DataSource
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  inputImage->UpdateOutputInformation();
  unsigned int nbBand = inputImage->GetNumberOfComponentsPerPixel();
  this->InitializeFields(outLayer,nbBand);
}

//template<class TInputImage>
//itk::DataObject::Pointer
//PersistentImageSampleExtractorFilter<TInputImage>
//::MakeOutput(DataObjectPointerArraySizeType idx)
//{
  //switch (idx)
    //{
    //case 0:
      //return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      //break;
    //case 1:
      //return static_cast<itk::DataObject*>(otb::ogr::DataSource::New().GetPointer());
      //break;
    //default:
      //// might as well make an image
      //return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      //break;
    //}
//}

template<class TInputImage>
typename PersistentImageSampleExtractorFilter<TInputImage>::OGRDataSourcePointerType
PersistentImageSampleExtractorFilter<TInputImage>
::ProcessTile()
{
  // Retrieve inputs
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  TInputImage* outputImage = this->GetOutput();
  ogr::DataSource* inVectors = const_cast<ogr::DataSource*>(this->GetSamplePositions());
  ogr::DataSource* outVectors = const_cast<ogr::DataSource*>(this->GetOGRDataSource());
  bool updateMode = bool(inVectors == outVectors);
  PointType imgPoint;
  IndexType imgIndex;
  PixelType imgPixel;
  double imgComp;
  RegionType requestedRegion = outputImage->GetRequestedRegion();
  unsigned int nbBand = inputImage->GetNumberOfComponentsPerPixel();
  std::ostringstream oss;
  std::string fieldName;
  ogr::Layer inLayer = inVectors->GetLayer(m_LayerIndex);
  
  // Apply spatial filter on input sample positions
  this->ApplyPolygonsSpatialFilter();
  
  float featCount = static_cast<float>(inLayer.GetFeatureCount(true));
  if (featCount == 0.0) featCount=1.0;
  int currentCount = 0;

  // Prepare temporary output data source
  OGRDataSourcePointerType tmpDS = ogr::DataSource::New();
  OGRSpatialReference * oSRS = NULL;
  if (inLayer.GetSpatialRef()) oSRS = inLayer.GetSpatialRef()->Clone();
  ogr::Layer dstLayer = tmpDS->CreateLayer(
    this->GetLayerName(),
    oSRS,
    this->GetGeometryType(),
    this->GetOGRLayerCreationOptions());
  OGRFieldDefn labelField(this->GetFieldName().c_str(),this->GetFieldType());
  dstLayer.CreateField(labelField, true);
  this->InitializeFields(dstLayer,nbBand);

  // Loop across the features in the layer
  OGRGeometry *geom;
  otb::ogr::Layer::iterator featIt = inLayer.begin(); 
  for(; featIt!=inLayer.end(); ++featIt)
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

        if (updateMode)
          {
          for (unsigned int i=0 ; i<nbBand ; ++i)
            {
            imgComp = static_cast<double>(itk::DefaultConvertPixelTraits<PixelType>::GetNthComponent(i,imgPixel));
            oss.str(std::string(""));
            oss << this->GetSampleFieldPrefix() << i;
            fieldName = oss.str();
            // Set the fields directly in the input features
            (*featIt)[fieldName].SetValue(imgComp);
            }
          inLayer.SetFeature(*featIt);
          }
        else
          {
          typename Superclass::OGRFeatureType dstFeature(dstLayer.GetLayerDefn());
          dstFeature.SetFrom( *featIt, TRUE );
          for (unsigned int i=0 ; i<nbBand ; ++i)
            {
            imgComp = static_cast<double>(itk::DefaultConvertPixelTraits<PixelType>::GetNthComponent(i,imgPixel));
            oss.str(std::string(""));
            oss << this->GetSampleFieldPrefix() << i;
            fieldName = oss.str();
            // Fill the ouptut OGRDataSource
            dstFeature[fieldName].SetValue(imgComp);
            }
          dstLayer.CreateFeature( dstFeature );
          }
        break;
        }
      default:
        {
        otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
        break;
        }
      }
    currentCount++;
    this->UpdateProgress(static_cast<float>(currentCount)/featCount);
    // TODO : multi-points ?
    }

  return tmpDS;
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::ApplyPolygonsSpatialFilter()
{
  TInputImage* outputImage = this->GetOutput();
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetSamplePositions());
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

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::InitializeFields(ogr::Layer &layer, unsigned int size)
{
  OGRFeatureDefn &outFeatureDefn = layer.GetLayerDefn();
  std::ostringstream oss;
  std::string fieldName;
  for (unsigned int i=0 ; i<size ; ++i)
    {
    oss.str(std::string(""));
    oss << this->GetSampleFieldPrefix() << i;
    fieldName = oss.str();
    if (outFeatureDefn.GetFieldIndex(fieldName.c_str()) < 0)
      {
      OGRFieldDefn sampleField(fieldName.c_str(),OFTReal);
      layer.CreateField(sampleField, true);
      }
    }
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
::SetSamplePositions(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetSamplePositions(data);
}

template<class TInputImage>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage>
::GetSamplePositions()
{
  return this->GetFilter()->GetSamplePositions();
}

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetOutputOGRData(OGRDataType::Pointer data)
{
  this->GetFilter()->SetOGRDataSource(data);
}

template<class TInputImage>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage>
::GetOutputOGRData()
{
  return this->GetFilter()->GetOGRDataSource();
}

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetOutputFieldPrefix(const std::string &key)
{
  this->GetFilter()->SetSampleFieldPrefix(key);
}

template<class TInputImage>
std::string
ImageSampleExtractorFilter<TInputImage>
::GetOutputFieldPrefix()
{
  return this->GetFilter()->GetSampleFieldPrefix();
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

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetClassFieldName(const std::string &name)
{
  this->GetFilter()->SetFieldName(name);
}

template<class TInputImage>
std::string
ImageSampleExtractorFilter<TInputImage>
::GetClassFieldName(void)
{
  return this->GetFilter()->GetFieldName();
}

} // end of namespace otb

#endif
