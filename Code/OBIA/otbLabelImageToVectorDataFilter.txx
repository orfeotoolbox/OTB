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
#ifndef __otbLabelImageToVectorDataFilter_txx
#define __otbLabelImageToVectorDataFilter_txx

#include "otbLabelImageToVectorDataFilter.h"
#include "otbOGRIOHelper.h"
#include "itkImageRegionIterator.h"
#include "otbGdalDataTypeBridge.h"

//gdal libraries
#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdal_alg.h"
#include "ogrsf_frmts.h"

#include <typeinfo>
namespace otb
{

template <class TInputImage, class TPrecision>
LabelImageToVectorDataFilter<TInputImage, TPrecision>
::LabelImageToVectorDataFilter()
{
   this->SetNumberOfRequiredInputs(1);
   this->SetNumberOfRequiredOutputs(1);
   
   GDALAllRegister();
   OGRRegisterAll();
}

template <class TInputImage, class TPrecision>
void
LabelImageToVectorDataFilter<TInputImage, TPrecision>
::SetInput(const InputImageType *input)
{
  this->Superclass::SetNthInput(0, const_cast<InputImageType *>(input));
}

template <class TInputImage, class TPrecision>
const typename LabelImageToVectorDataFilter<TInputImage, TPrecision>
::InputImageType *
LabelImageToVectorDataFilter<TInputImage, TPrecision>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputImageType *>(this->Superclass::GetInput(0));
}

template <class TInputImage, class TPrecision>
void
LabelImageToVectorDataFilter<TInputImage, TPrecision>
::GenerateInputRequestedRegion(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  typename InputImageType::Pointer input  =
    const_cast<InputImageType *> (this->GetInput());

  if ( !input )
    {
    return;
    }

  // The input is necessarily the largest possible region.
  // For a streamed implementation, use the StreamingLineSegmentDetector filter
  input->SetRequestedRegionToLargestPossibleRegion();
}


template <class TInputImage, class TPrecision>
void
LabelImageToVectorDataFilter<TInputImage, TPrecision>
::GenerateData(void)
{
   if (this->GetInput()->GetRequestedRegion() != this->GetInput()->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<< "Not streamed filter. ERROR : requested region is not the largest possible region.");
    }
    
    typename InputImageType::Pointer inImage = const_cast<InputImageType *>(this->GetInput());

    SizeType size = this->GetInput()->GetLargestPossibleRegion().GetSize();
    
    unsigned int nbBands = this->GetInput()->GetNumberOfComponentsPerPixel();
    unsigned int bytePerPixel = sizeof(InputPixelType);

    /** Convert Input image into a OGRLayer using GDALPolygonize */
    // buffer casted in unsigned long cause under Win32 the adress
    // don't begin with 0x, the adress in not interpreted as
    // hexadecimal but alpha numeric value, then the conversion to
    // integer make us pointing to an non allowed memory block => Crash.
    std::ostringstream stream;
    stream << "MEM:::"
           <<  "DATAPOINTER=" << (unsigned long)(this->GetInput()->GetBufferPointer()) << ","
           <<  "PIXELS=" << size[0] << ","
           <<  "LINES=" << size[1] << ","
           <<  "BANDS=" << nbBands << ","
           <<  "DATATYPE=" << GDALGetDataTypeName(GdalDataTypeBridge::GetGDALDataType<InputPixelType>()) << ","
           <<  "PIXELOFFSET=" << bytePerPixel * nbBands << ","
           <<  "LINEOFFSET=" << bytePerPixel * nbBands * size[0] << ","
           <<  "BANDOFFSET=" << bytePerPixel;
    
    GDALDataset * dataset = static_cast<GDALDataset *> (GDALOpen(stream.str().c_str(), GA_ReadOnly));
    
    //Set input Projection ref and Geo transform to the dataset.
    dataset->SetProjection(this->GetInput()->GetProjectionRef().c_str());
    
    unsigned int projSize = this->GetInput()->GetGeoTransform().size();
    double geoTransform[6];
    
    //Set the geo transform of the input image (if any)
    if (projSize != 0)
    {
      for(unsigned int i = 0; i<6; i++)
      {
         geoTransform[i] = this->GetInput()->GetGeoTransform()[i];
      }
    }
    //set the identity geo transform,
    //because mem driver set the default geo transform to 0,1,0,0,0,-1
    else
    {
      geoTransform[0] = 0;
      geoTransform[1] = 1;
      geoTransform[2] = 0;
      geoTransform[3] = 0;
      geoTransform[4] = 0;
      geoTransform[5] = 1;
    }
    dataset->SetGeoTransform(geoTransform);
    
    //Create the output layer for GDALPolygonize().
    const char * driverName = "Memory";
    OGRSFDriver * ogrDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driverName);
    OGRDataSource * dataSource = ogrDriver->CreateDataSource("Shape",NULL);
    
    OGRLayer * outputLayer = dataSource->CreateLayer("toto",NULL,wkbMultiPolygon,NULL);

    //Call GDALPolygonize()
    GDALPolygonize(dataset->GetRasterBand(1), NULL, outputLayer, 0, NULL, NULL, NULL);
    
    /** Convert OGR layer into VectorData */
    OGRFeatureDefn * dfn = outputLayer->GetLayerDefn();

    // Create the document node
    DataNodePointerType document = DataNodeType::New();
    document->SetNodeType(DOCUMENT);
    document->SetNodeId(dfn->GetName());

    // Adding the layer to the data tree
    VectorDataPointerType data = dynamic_cast<VectorDataType*>(this->GetOutput());
    DataTreePointerType tree = data->GetDataTree();
    DataNodePointerType root = tree->GetRoot()->Get();
    tree->Add(document, root);

    //This is not good but we do not have the choice if we want to
    //get a hook on the internal structure
    InternalTreeNodeType * documentPtr = const_cast<InternalTreeNodeType *>(tree->GetNode(document));
    
    OGRIOHelper::Pointer OGRConversion = OGRIOHelper::New();
    OGRConversion->ConvertOGRLayerToDataTreeNode(outputLayer, documentPtr);
    
    //Clear memory
    OGRDataSource::DestroyDataSource(dataSource);
    GDALClose(dataset);


}


} // end namespace otb

#endif
