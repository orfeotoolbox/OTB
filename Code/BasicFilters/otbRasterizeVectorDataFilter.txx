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

#include "otbRasterizeVectorDataFilter.h"
#include "otbOGRIOHelper.h"
#include "otbGdalDataTypeBridge.h"
#include "otbMacro.h"

namespace otb
{
template<class TVectorData, class TInputImage, class TOutputImage>
RasterizeVectorDataFilter<TVectorData, TInputImage, TOutputImage>
::RasterizeVectorDataFilter()
 : m_OGRDataSourcePointer(0)
{
  this->SetNumberOfRequiredInputs(1);
}

template<class TVectorData, class TInputImage, class TOutputImage>
void
RasterizeVectorDataFilter<TVectorData, TInputImage, TOutputImage>
::AddVectorData(const VectorDataType* vd)
{
  // Process object is not const-correct so the const_cast is required
  // here
  if (this->GetNumberOfInputs() < 1)
    {
    this->itk::ProcessObject::SetNthInput(1, const_cast<VectorDataType *>(vd) );
    }
  else
    {
    this->itk::ProcessObject::PushBackInput(  vd   );
    }
}


template<class TVectorData, class TInputImage, class TOutputImage>
void
RasterizeVectorDataFilter<TVectorData, TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Generate the OGRLayers from the input VectorDatas
  // iteration begin from 1 cause the 0th input is a image
  for (unsigned int idx = 1; idx < this->GetNumberOfInputs(); ++idx)
    {
    const VectorDataType* vd = dynamic_cast< const VectorDataType*>(this->itk::ProcessObject::GetInput(idx));

    // Get the projection ref of the current VectorData
    std::string projectionRefWkt = vd->GetProjectionRef();
    bool        projectionInformationAvailable = !projectionRefWkt.empty();
    OGRSpatialReference * oSRS = NULL;

    if (projectionInformationAvailable)
      {
      oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
      }
    else
      {
      otbMsgDevMacro(<< "Projection information unavailable");
      }

    // Retrieving root node
    DataTreeConstPointerType tree = vd->GetDataTree();

    // Get the input tree root
    InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(tree->GetRoot());

    // Iterative method to build the layers from a VectorData
    OGRRegisterAll();
    OGRLayer *   ogrCurrentLayer = NULL;
    std::vector<OGRLayer *> ogrLayerVector;
    otb::OGRIOHelper::Pointer IOConversion = otb::OGRIOHelper::New();

    // The method ConvertDataTreeNodeToOGRLayers create the
    // OGRDataSource but don t release it. Destruction is done in the
    // desctructor
    m_OGRDataSourcePointer = NULL;
    ogrLayerVector = IOConversion->ConvertDataTreeNodeToOGRLayers(inputRoot,
                                                                  m_OGRDataSourcePointer,
                                                                  ogrCurrentLayer,
                                                                  oSRS);

    // Cast OGRLayer* to OGRLayerH
    for (unsigned int idx = 0; idx < ogrLayerVector.size(); ++idx)
      {
      m_SrcDataSetLayers.push_back( (OGRLayerH)(ogrLayerVector[idx]) );
      }

    // Destroy the oSRS
    if (oSRS != NULL)
      {
      OSRRelease(oSRS);
      }
    }

  // Some checking : Check the consistency between the band list
  // to burn and the burn values :
  // There should be "m_BandsToBurn.size()" burn values for each layer.
  // If not, burn values vector will be cloned as many time as the number of
  // OGRLayer we have
  if (m_BurnValues.size() !=  m_BandsToBurn.size() * m_SrcDataSetLayers.size())
    {
    std::ostringstream oss;
    oss  <<"Inconsistency detected : expected burn vector size to be equal to( bandToBurn * nb layers = "
         << m_BandsToBurn.size() * m_SrcDataSetLayers.size()
         << " ), got :  "<< m_BurnValues.size()<<std::endl;
    itkWarningMacro(<<oss.str());
    }

  // Clone the burn values to fit the condition
  for (unsigned int idx =0; idx < m_SrcDataSetLayers.size(); ++idx)
    {
    for (unsigned int burnidx = 0; burnidx < m_BurnValues.size(); ++burnidx)
      {
      m_FullBurnValues.push_back(m_BurnValues[burnidx]);
      }
    }
}

template<class TVectorData, class TInputImage, class TOutputImage>
void
RasterizeVectorDataFilter<TVectorData, TInputImage, TOutputImage>::GenerateData()
{
  // Call Superclass GenerateData
  Superclass::GenerateData();

  // Get the buffered region
  OutputImageRegionType bufferedRegion = this->GetOutput()->GetBufferedRegion();

  // nb bands
  unsigned int nbBands =  this->GetOutput()->GetNumberOfComponentsPerPixel();

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

  // add the geoTransform to the dataset
  itk::VariableLengthVector<double> geoTransform(6);

  // Reporting origin and spacing of the buffered region
  // the spacing is unchanged, the origin is relative to the buffered region
  InputIndexType  bufferIndexOrigin = bufferedRegion.GetIndex();
  InputPointType  bufferOrigin;
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
     GDALRasterizeLayers( dataset, m_BandsToBurn.size(),
                          &(m_BandsToBurn[0]),
                          m_SrcDataSetLayers.size(),
                          &(m_SrcDataSetLayers[0]),
                          NULL, NULL, &(m_FullBurnValues[0]),
                          NULL,
                          GDALDummyProgress, NULL );

     // release the dataset
     GDALClose( dataset );
     }
}

template<class TVectorData, class TInputImage, class TOutputImage>
void
RasterizeVectorDataFilter<TVectorData, TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

