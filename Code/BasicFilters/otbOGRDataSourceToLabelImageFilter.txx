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

#include "otbOGRDataSourceToLabelImageFilter.h"
#include "otbOGRIOHelper.h"
#include "otbGdalDataTypeBridge.h"
#include "otbMacro.h"

#include "gdal_alg.h"

namespace otb
{
template< class TOutputImage>
OGRDataSourceToLabelImageFilter<TOutputImage>
::OGRDataSourceToLabelImageFilter() : m_BurnAttribute("DN"), 
                                      m_BackgroundValue(0), 
                                      m_ForegroundValue(255),
                                      m_BurnAttributeMode(true)
{
  this->SetNumberOfRequiredInputs(1);

  // Output parameters initialization
  m_OutputSpacing.Fill(1.0);
  m_OutputSize.Fill(0);
  m_OutputStartIndex.Fill(0);
  m_BandsToBurn.clear();
  m_BandsToBurn.push_back(1);
  
}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::AddOGRDataSource(const OGRDataSourceType* ds)
{
  this->itk::ProcessObject::PushBackInput( ds );
}

template < class TOutputImage>
const typename OGRDataSourceToLabelImageFilter<TOutputImage>::OGRDataSourceType *
OGRDataSourceToLabelImageFilter<TOutputImage>
::GetInput(unsigned int idx)
{
  return static_cast<const OGRDataSourceType *>
           (this->itk::ProcessObject::GetInput(idx));
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputSpacing(const OutputSpacingType& spacing)
{
  if (this->m_OutputSpacing != spacing)
    {
    this->m_OutputSpacing = spacing;
    this->Modified();
    }
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputSpacing(const double spacing[2])
{
  OutputSpacingType s(spacing);
  this->SetOutputSpacing(s);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  OutputSpacingType s;
  s.CastFrom(sf);
  this->SetOutputSpacing(s);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputOrigin(const double origin[2])
{
  OutputOriginType p(origin);
  this->SetOutputOrigin(p);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OutputOriginType p;
  p.CastFrom(of);
  this->SetOutputOrigin(p);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputParametersFromImage(const ImageBaseType * image)
{
  const OutputImageType * src = dynamic_cast<const OutputImageType*>(image);
  
  this->SetOutputOrigin ( src->GetOrigin() );
  this->SetOutputSpacing ( src->GetSpacing() );
  //this->SetOutputStartIndex ( src->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( src->GetLargestPossibleRegion().GetSize() );
  this->SetOutputProjectionRef(src->GetProjectionRef());
  //this->SetOutputKeywordList(src->GetImageKeywordlist());
}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::GenerateOutputInformation()
{
  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();
  if (!outputPtr)
    {
    return;
    }

  // Set the size of the output region
  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_OutputSize);
  //outputLargestPossibleRegion.SetIndex(m_OutputStartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  // Set spacing and origin
  outputPtr->SetSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);

  itk::MetaDataDictionary& dict = outputPtr->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                         static_cast<std::string>(this->GetOutputProjectionRef()));
  
  // Generate the OGRLayers from the input OGRDataSource
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    OGRDataSourcePointerType ogrDS = dynamic_cast<OGRDataSourceType*>(this->itk::ProcessObject::GetInput(idx));
    const unsigned int nbLayers = ogrDS->GetLayersCount();

    for (unsigned int layer = 0; layer < nbLayers; ++layer)
      {
         m_SrcDataSetLayers.push_back( &(ogrDS->GetLayer(layer).ogr()) );
      }
    }
}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>::GenerateData()
{
  // Call Superclass GenerateData
  this->AllocateOutputs();
  
  // Get the buffered region
  OutputImageRegionType bufferedRegion = this->GetOutput()->GetBufferedRegion();

  // nb bands
  const unsigned int & nbBands =  this->GetOutput()->GetNumberOfComponentsPerPixel();

  // register drivers
  GDALAllRegister();
  
  std::ostringstream stream;
  stream << "MEM:::"
         <<  "DATAPOINTER=" << (unsigned long)(this->GetOutput()->GetBufferPointer()) << ","
         <<  "PIXELS=" << bufferedRegion.GetSize()[0] << ","
         <<  "LINES=" << bufferedRegion.GetSize()[1]<< ","
         <<  "BANDS=" << nbBands << ","
         <<  "DATATYPE=" << GDALGetDataTypeName(GdalDataTypeBridge::GetGDALDataType<OutputImageInternalPixelType>()) << ","
         <<  "PIXELOFFSET=" << sizeof(OutputImageInternalPixelType) *  nbBands << ","
         <<  "LINEOFFSET=" << sizeof(OutputImageInternalPixelType)*nbBands*bufferedRegion.GetSize()[0] << ","
         <<  "BANDOFFSET=" << sizeof(OutputImageInternalPixelType);
  
  GDALDatasetH dataset = GDALOpen(stream.str().c_str(), GA_Update);

  // Add the projection ref to the dataset
  GDALSetProjection (dataset, this->GetOutput()->GetProjectionRef().c_str());

  // Set the nodata value
  for(unsigned int band = 0; band < nbBands;++band)
    {
     GDALRasterBandH hBand = GDALGetRasterBand(dataset, band + 1);
     GDALSetRasterNoDataValue(hBand, m_BackgroundValue);
    }
  
  // add the geoTransform to the dataset
  itk::VariableLengthVector<double> geoTransform(6);

  // Reporting origin and spacing of the buffered region
  // the spacing is unchanged, the origin is relative to the buffered region
  OutputIndexType  bufferIndexOrigin = bufferedRegion.GetIndex();
  OutputOriginType bufferOrigin;
  this->GetOutput()->TransformIndexToPhysicalPoint(bufferIndexOrigin, bufferOrigin);
  geoTransform[0] = bufferOrigin[0];
  geoTransform[3] = bufferOrigin[1];
  geoTransform[1] = this->GetOutput()->GetSpacing()[0];
  geoTransform[5] = this->GetOutput()->GetSpacing()[1];

  // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
  geoTransform[2] = 0.;
  geoTransform[4] = 0.;
  GDALSetGeoTransform(dataset,const_cast<double*>(geoTransform.GetDataPointer()));

  // Burn the geometries into the dataset
   if (dataset != NULL)
     {
     std::vector<std::string> options;
     
     std::vector<double> foreground(nbBands,m_ForegroundValue);

     if(m_BurnAttributeMode)
       {
       options.push_back("ATTRIBUTE="+m_BurnAttribute);
       }

     GDALRasterizeLayers( dataset, nbBands,
                          &m_BandsToBurn[0],
                          m_SrcDataSetLayers.size(),
                          &(m_SrcDataSetLayers[0]),
                          NULL, NULL, &foreground[0],
                          ogr::StringListConverter(options).to_ogr(),
                          NULL, NULL );
     // release the dataset
     GDALClose( dataset );
     }
}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

