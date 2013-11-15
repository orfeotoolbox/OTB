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

namespace otb
{

template <class TInputImage, class TPrecision>
LabelImageToVectorDataFilter<TInputImage, TPrecision>
::LabelImageToVectorDataFilter() : m_FieldName("DN"), m_Use8Connected(false)
{
   this->SetNumberOfRequiredInputs(2);
   this->SetNumberOfRequiredInputs(1);
   this->SetNumberOfRequiredOutputs(1);

   GDALAllRegister();
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
::SetInputMask(const InputImageType *input)
{
  this->Superclass::SetNthInput(1, const_cast<InputImageType *>(input));
}

template <class TInputImage, class TPrecision>
const typename LabelImageToVectorDataFilter<TInputImage, TPrecision>
::InputImageType *
LabelImageToVectorDataFilter<TInputImage, TPrecision>
::GetInputMask(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }

  return static_cast<const InputImageType *>(this->Superclass::GetInput(1));
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
  input->SetRequestedRegionToLargestPossibleRegion();

  typename InputImageType::Pointer mask  =
    const_cast<InputImageType *> (this->GetInputMask());
  if(!mask)
  {
   return;
  }
  // The input is necessarily the largest possible region.
  mask->SetRequestedRegionToLargestPossibleRegion();
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
    // Reporting origin and spacing of the buffered region
    // the spacing is unchanged, the origin is relative to the buffered region
    IndexType  bufferIndexOrigin = this->GetInput()->GetBufferedRegion().GetIndex();
    OriginType  bufferOrigin;
    this->GetInput()->TransformIndexToPhysicalPoint(bufferIndexOrigin, bufferOrigin);
    geoTransform[0] = bufferOrigin[0];
    geoTransform[3] = bufferOrigin[1];
    geoTransform[1] = this->GetInput()->GetSpacing()[0];
    geoTransform[5] = this->GetInput()->GetSpacing()[1];
    // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
    if (projSize == 0)
    {
      geoTransform[2] = 0.;
      geoTransform[4] = 0.;
    }
    else
    {
      geoTransform[2] = this->GetInput()->GetGeoTransform()[2];
      geoTransform[4] = this->GetInput()->GetGeoTransform()[4];
    }
    dataset->SetGeoTransform(geoTransform);

    //Create the output layer for GDALPolygonize().
    ogr::DataSource::Pointer ogrDS = ogr::DataSource::New();

    OGRLayerType outputLayer = ogrDS->CreateLayer("layer",NULL,wkbPolygon);

    OGRFieldDefn field(m_FieldName.c_str(),OFTInteger);
    outputLayer.CreateField(field, true);

    //Call GDALPolygonize()
    char ** options;
    options = NULL;
    char * option[1];
    if (m_Use8Connected == true)
    {
      std::string opt("8CONNECTED:8");
      option[0] = const_cast<char *>(opt.c_str());
      options=option;
    }

    /* Convert the mask input into a GDAL raster needed by GDALPolygonize */
    typename InputImageType::ConstPointer inputMask = this->GetInputMask();
    if (!inputMask.IsNull())
    {
      size = this->GetInputMask()->GetLargestPossibleRegion().GetSize();
      nbBands = this->GetInputMask()->GetNumberOfComponentsPerPixel();
      bytePerPixel = sizeof(InputPixelType);
      // buffer casted in unsigned long cause under Win32 the adress
      // don't begin with 0x, the adress in not interpreted as
      // hexadecimal but alpha numeric value, then the conversion to
      // integer make us pointing to an non allowed memory block => Crash.
      std::ostringstream maskstream;
      maskstream << "MEM:::"
            <<  "DATAPOINTER=" << (unsigned long)(this->GetInputMask()->GetBufferPointer()) << ","
            <<  "PIXELS=" << size[0] << ","
            <<  "LINES=" << size[1] << ","
            <<  "BANDS=" << nbBands << ","
            <<  "DATATYPE=" << GDALGetDataTypeName(GdalDataTypeBridge::GetGDALDataType<InputPixelType>()) << ","
            <<  "PIXELOFFSET=" << bytePerPixel * nbBands << ","
            <<  "LINEOFFSET=" << bytePerPixel * nbBands * size[0] << ","
            <<  "BANDOFFSET=" << bytePerPixel;

      GDALDataset * maskDataset = static_cast<GDALDataset *> (GDALOpen(maskstream.str().c_str(), GA_ReadOnly));

      //Set input Projection ref and Geo transform to the dataset.
      maskDataset->SetProjection(this->GetInputMask()->GetProjectionRef().c_str());

      projSize = this->GetInputMask()->GetGeoTransform().size();

      //Set the geo transform of the input mask image (if any)
      // Reporting origin and spacing of the buffered region
      // the spacing is unchanged, the origin is relative to the buffered region
      bufferIndexOrigin = this->GetInputMask()->GetBufferedRegion().GetIndex();
      this->GetInputMask()->TransformIndexToPhysicalPoint(bufferIndexOrigin, bufferOrigin);
      geoTransform[0] = bufferOrigin[0];
      geoTransform[3] = bufferOrigin[1];
      geoTransform[1] = this->GetInputMask()->GetSpacing()[0];
      geoTransform[5] = this->GetInputMask()->GetSpacing()[1];
      // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
      if (projSize == 0)
      {
         geoTransform[2] = 0.;
         geoTransform[4] = 0.;
      }
      else
      {
         geoTransform[2] = this->GetInputMask()->GetGeoTransform()[2];
         geoTransform[4] = this->GetInputMask()->GetGeoTransform()[4];
      }
      maskDataset->SetGeoTransform(geoTransform);

      GDALPolygonize(dataset->GetRasterBand(1), maskDataset->GetRasterBand(1), &outputLayer.ogr(), 0, options, NULL, NULL);
      GDALClose(maskDataset);
    }
    else
    {
      GDALPolygonize(dataset->GetRasterBand(1), NULL, &outputLayer.ogr(), 0, options, NULL, NULL);
    }


    /** Convert OGR layer into VectorData */

    // Create the document node
    DataNodePointerType document = DataNodeType::New();
    document->SetNodeType(DOCUMENT);
    document->SetNodeId(outputLayer.GetLayerDefn().GetName());

    // Adding the layer to the data tree
    VectorDataPointerType data = dynamic_cast<VectorDataType*>(this->GetOutput());
    data->SetProjectionRef(this->GetInput()->GetProjectionRef());

    DataTreePointerType tree = data->GetDataTree();
    DataNodePointerType root = tree->GetRoot()->Get();
    tree->Add(document, root);

    //This is not good but we do not have the choice if we want to
    //get a hook on the internal structure
    InternalTreeNodeType * documentPtr = const_cast<InternalTreeNodeType *>(tree->GetNode(document));

    OGRIOHelper::Pointer OGRConversion = OGRIOHelper::New();
    OGRConversion->ConvertOGRLayerToDataTreeNode(&outputLayer.ogr(), documentPtr);

    //Clear memory
    GDALClose(dataset);


}


} // end namespace otb

#endif
