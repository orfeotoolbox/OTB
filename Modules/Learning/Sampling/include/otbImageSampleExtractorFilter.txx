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
#include "itkProgressReporter.h"

namespace otb
{
// --------- otb::PersistentImageSampleExtractorFilter ---------------------

template<class TInputImage>
PersistentImageSampleExtractorFilter<TInputImage>
::PersistentImageSampleExtractorFilter() :
  m_SampleFieldPrefix(std::string("band_"))
{
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TInputImage::New());
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::SetOutputSamples(ogr::DataSource* data)
{
  this->SetNthOutput(1,data);
}

template<class TInputImage>
ogr::DataSource*
PersistentImageSampleExtractorFilter<TInputImage>
::GetOutputSamples()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<ogr::DataSource *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::Synthetize(void)
{
  // clear temporary outputs
  this->m_InMemoryOutputs.clear();
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::Reset(void)
{
  // initialize additional fields for output
  this->InitializeFields();

  // initialize output DataSource
  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::DataSource* output  = this->GetOutputSamples();
  this->InitializeOutputDataSource(inputDS,output);
}

template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::GenerateInputRequestedRegion()
{
  InputImageType *input = const_cast<InputImageType*>(this->GetInput());
  RegionType requested = this->GetOutput()->GetRequestedRegion();
  input->SetRequestedRegion(requested);
}


template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
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
  std::ostringstream oss;
  std::string fieldName;
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
        imgPixel = inputImage->GetPixel(imgIndex);

        ogr::Feature dstFeature(outputLayer.GetLayerDefn());
        dstFeature.SetFrom( *featIt, TRUE );
        dstFeature.SetFID(featIt->GetFID());
        for (unsigned int i=0 ; i<nbBand ; ++i)
          {
          imgComp = static_cast<double>(itk::DefaultConvertPixelTraits<PixelType>::GetNthComponent(i,imgPixel));
          oss.str(std::string(""));
          oss << this->GetSampleFieldPrefix() << i;
          fieldName = oss.str();
          // Fill the ouptut OGRDataSource
          dstFeature[fieldName].SetValue(imgComp);
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


template<class TInputImage>
void
PersistentImageSampleExtractorFilter<TInputImage>
::InitializeFields()
{
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  inputImage->UpdateOutputInformation();
  unsigned int nbBand = inputImage->GetNumberOfComponentsPerPixel();
  
  this->m_AdditionalFields.clear();
  std::ostringstream oss;
  std::string fieldName;
  for (unsigned int i=0 ; i<nbBand ; ++i)
    {
    oss.str(std::string(""));
    oss << this->GetSampleFieldPrefix() << i;
    fieldName = oss.str();
    // DEBUG
    std::cout << "new field "<< fieldName << std::endl;
    OGRFieldDefn sampleField(fieldName.c_str(),OFTReal);
    sampleField.SetWidth(12);
    sampleField.SetPrecision(10);
    this->m_AdditionalFields.push_back(ogr::FieldDefn(sampleField));
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
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage>
::GetSamplePositions()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage>
void
ImageSampleExtractorFilter<TInputImage>
::SetOutputSamples(OGRDataType::Pointer data)
{
  this->GetFilter()->SetOutputSamples(data);
}

template<class TInputImage>
const otb::ogr::DataSource*
ImageSampleExtractorFilter<TInputImage>
::GetOutputSamples()
{
  return this->GetFilter()->GetOutputSamples();
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
