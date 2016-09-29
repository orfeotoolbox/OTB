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
#ifndef otbImageSampleExtractorFilter_txx
#define otbImageSampleExtractorFilter_txx

#include "otbImageSampleExtractorFilter.h"
#include "itkDefaultConvertPixelTraits.h"
#include "itkProgressReporter.h"
#include <type_traits>

namespace otb
{
// --------- otb::PersistentImageSampleExtractorFilter ---------------------

template<class TInputImage, class TFeatureExtractionFunctor>
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::PersistentImageSampleExtractorFilter() :
  m_SampleFieldPrefix(std::string("band_"))
{
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TInputImage::New());
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetOutputSamples(ogr::DataSource* data)
{
  this->SetNthOutput(1,data);
}

template<class TInputImage, class TFeatureExtractionFunctor>
ogr::DataSource*
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetOutputSamples()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<ogr::DataSource *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::Synthetize(void)
{
  // clear temporary outputs
  this->m_InMemoryOutputs.clear();
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::Reset(void)
{
  // Check output field names
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  inputImage->UpdateOutputInformation();
  unsigned int nbBand = inputImage->GetNumberOfComponentsPerPixel();
  if (m_SampleFieldNames.size())
    {
    if ( m_SampleFieldNames.size() != nbBand)
      {
      itkExceptionMacro(<< "Wrong number of field names given, got "
                        <<m_SampleFieldNames.size() << ", expected "<< nbBand);
      }
    }
  else
    {
    // use the prefix to create field names
    std::ostringstream oss;
    for (unsigned int i=0 ; i<nbBand ; ++i)
      {
      oss.str("");
      oss << this->GetSampleFieldPrefix() << i;
      m_SampleFieldNames.push_back(oss.str());
      }
    }
  // initialize additional fields for output
  this->InitializeFields();

  // initialize output DataSource
  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::DataSource* output  = this->GetOutputSamples();
  this->InitializeOutputDataSource(inputDS,output);
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetSampleFieldNames(std::vector<std::string> &names)
{
  m_SampleFieldNames.clear();
  for (unsigned int i=0 ; i<names.size() ; i++)
    {
    m_SampleFieldNames.push_back(names[i]);
    }
}

template<class TInputImage, class TFeatureExtractionFunctor>
const std::vector<std::string> &
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetSampleFieldNames()
{
  return m_SampleFieldNames;
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Check SRS of input image and samples
  std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
  bool projectionInformationAvailable = !projectionRefWkt.empty();
  if(projectionInformationAvailable)
    {
    OGRSpatialReference imgSRS;
    const char *projWktCstr = projectionRefWkt.c_str();
    char **projWktPointer = const_cast<char**>(&projWktCstr);
    OGRErr err = imgSRS.importFromWkt( projWktPointer );
    if (err == OGRERR_NONE)
      {
      // get input layer
      ogr::Layer inLayer = this->GetOGRData()->GetLayer(this->GetLayerIndex());
      if ( !imgSRS.IsSame(inLayer.GetSpatialRef()) )
        {
        char *layerSrsWkt = NULL;
        inLayer.GetSpatialRef()->exportToPrettyWkt(&layerSrsWkt);
        itkExceptionMacro(<< "Spatial reference of input image and samples don't match:  \n" << projectionRefWkt << "\nvs\n"<<  layerSrsWkt);
        }
      }
    }
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GenerateInputRequestedRegion()
{
  InputImageType *input = const_cast<InputImageType*>(this->GetInput());
  RegionType requested = this->GetOutput()->GetRequestedRegion();
  input->SetRequestedRegion(requested);
}


template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::ThreadedGenerateData(const RegionType&, itk::ThreadIdType threadid)
{
  // Retrieve inputs
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  unsigned int nbBand = inputImage->GetNumberOfComponentsPerPixel();

  ogr::Layer layer = this->m_InMemoryInputs[threadid]->GetLayerChecked(0);
  if (! layer)
    {
    return;
    }
  ogr::Layer outputLayer = this->m_InMemoryOutputs[threadid][0]->GetLayerChecked(0);

  itk::ProgressReporter progress( this, threadid, layer.GetFeatureCount(true) );

  // Loop across the features in the layer (filtered by requested region in BeforeTGD already)
  OGRGeometry *geom;
  PointType imgPoint;
  IndexType imgIndex;
  PixelType imgPixel;
  double imgComp;
  ogr::Layer::const_iterator featIt = layer.begin();
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
      imgPixel = m_FeatureExtraction(inputImage->GetPixel(imgIndex));

      ogr::Feature dstFeature(outputLayer.GetLayerDefn());
      dstFeature.SetFrom( *featIt, TRUE );
      dstFeature.SetFID(featIt->GetFID());
      for (unsigned int i=0 ; i<nbBand ; ++i)
        {
        imgComp = static_cast<double>(itk::DefaultConvertPixelTraits<PixelType>::GetNthComponent(i,imgPixel));
        // Fill the output OGRDataSource
        dstFeature[m_SampleFieldNames[i]].SetValue(imgComp);
        }
      outputLayer.CreateFeature( dstFeature );
      break;
      }
      default:
      {
      otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
      break;
      }
      }
    progress.CompletedPixel();
    }
}


template<class TInputImage, class TFeatureExtractionFunctor>
void
PersistentImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::InitializeFields()
{
  this->ClearAdditionalFields();
  for (unsigned int i=0 ; i<m_SampleFieldNames.size() ; ++i)
    {
    this->CreateAdditionalField(m_SampleFieldNames[i],OFTReal,24,15);
    }
}

// -------------- otb::ImageSampleExtractorFilter --------------------------

template<class TInputImage, class TFeatureExtractionFunctor>
void
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TFeatureExtractionFunctor>
const TInputImage*
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetSamplePositions(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TFeatureExtractionFunctor>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetSamplePositions()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetOutputSamples(OGRDataType::Pointer data)
{
  this->GetFilter()->SetOutputSamples(data);
}

template<class TInputImage, class TFeatureExtractionFunctor>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetOutputSamples()
{
  return this->GetFilter()->GetOutputSamples();
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetOutputFieldPrefix(const std::string &key)
{
  this->GetFilter()->SetSampleFieldPrefix(key);
}

template<class TInputImage, class TFeatureExtractionFunctor>
std::string
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetOutputFieldPrefix()
{
  return this->GetFilter()->GetSampleFieldPrefix();
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetOutputFieldNames(std::vector<std::string> &names)
{
  this->GetFilter()->SetSampleFieldNames(names);
}

template<class TInputImage, class TFeatureExtractionFunctor>
const std::vector<std::string> &
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetOutputFieldNames()
{
  return this->GetFilter()->GetSampleFieldNames();
}


template<class TInputImage, class TFeatureExtractionFunctor>
void
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TInputImage, class TFeatureExtractionFunctor>
int
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

template<class TInputImage, class TFeatureExtractionFunctor>
void
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::SetClassFieldName(const std::string &name)
{
  this->GetFilter()->SetFieldName(name);
}

template<class TInputImage, class TFeatureExtractionFunctor>
std::string
ImageSampleExtractorFilter<TInputImage,TFeatureExtractionFunctor>
::GetClassFieldName(void)
{
  return this->GetFilter()->GetFieldName();
}

} // end of namespace otb

#endif
