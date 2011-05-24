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
#include "otbConnectedComponentSegmentation.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"
#include "otbStandardFilterWatcher.h"

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

namespace otb
{

int ConnectedComponentSegmentation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Connected Component Segmentation");
  descriptor->SetDescription(
                             "Connected Component Segmentation, which take mathematical formula as an Neighborhood thresholding criteria.");
  descriptor->AddInputImage();
  descriptor->AddOption("OutputVectorData", "Output Shape file name", "outshape", 1, true,
                        ApplicationDescriptor::FileName);
  descriptor->AddOption("MaskExpression", "Mask expression (only if support image is given)", "maskexpression", 1,
                        false, ApplicationDescriptor::String);
  descriptor->AddOption("ConnectedComponentExpression", "Formula used for connected component segmentation", "expression", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("MinimumObjectSize", "Min object size (area in pixel)", "minsize", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("OBIAExpression", "OBIA Mu Parser expression", "OBIAexpression", 1, false,
                        ApplicationDescriptor::String);
  descriptor->AddOption("DEMDirectory", "DEM directory (used to reproject output in WGS84 if input image is in sensor model geometry)",
                        "dem", 1, false, ApplicationDescriptor::DirectoryName);
  return EXIT_SUCCESS;
}


int ConnectedComponentSegmentation::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
  typedef otb::Image<unsigned int, Dimension>         LabelImageType;
  typedef otb::Image<unsigned int, Dimension>         MaskImageType;
  typedef otb::ImageFileReader<InputVectorImageType>  ReaderType;

  typedef otb::VectorData<double, Dimension>          VectorDataType;
  typedef VectorDataType::Pointer                     VectorDataPointerType;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataFileWriterType;
  typedef VectorDataFileWriterType::Pointer           VectorDataFileWriterPointerType;

  typedef otb::StreamingConnectedComponentSegmentationOBIAToVectorDataFilter
    < InputVectorImageType,
      LabelImageType,
      MaskImageType,
      VectorDataType >  ConnectedComponentSegmentationOBIAToVectorDataFilterType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage());
  reader->UpdateOutputInformation();

  InputVectorImageType::Pointer inputImage = reader->GetOutput();

  ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::Pointer connected
    = ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::New();
  connected->GetFilter()->SetInput(inputImage);

  if (parseResult->IsOptionPresent("MaskExpression"))
    connected->GetFilter()->SetMaskExpression(parseResult->GetParameterString("MaskExpression"));

  connected->GetFilter()->SetConnectedComponentExpression(parseResult->GetParameterString("ConnectedComponentExpression"));

  if (parseResult->IsOptionPresent("MinimumObjectSize"))
    connected->GetFilter()->SetMinimumObjectSize(parseResult->GetParameterInt("MinimumObjectSize"));
  else
    connected->GetFilter()->SetMinimumObjectSize(2);

  if (parseResult->IsOptionPresent("OBIAExpression"))
    connected->GetFilter()->SetOBIAExpression(parseResult->GetParameterString("OBIAExpression"));

  otb::StandardFilterWatcher watcher(connected->GetStreamer(),"Segmentation");
  connected->Update();

  /*
   * Reprojection of the output VectorData
   *
   * The output of the filter is in image physical coordinates,
   * projection WKT applied if the input image has one
   *
   * We need to reproject in WGS84 if the input image is in sensor model geometry
   */

  std::string projRef = inputImage->GetProjectionRef();
  ImageKeywordlist kwl = inputImage->GetImageKeywordlist();

  VectorDataType::Pointer vd = connected->GetFilter()->GetOutputVectorData();
  VectorDataType::Pointer projectedVD = vd;

  if ( projRef.empty() && kwl.GetSize() > 0 )
    {
    // image is in sensor model geometry

    // Reproject VectorData in image projection
    typedef otb::VectorDataProjectionFilter
      <VectorDataType, VectorDataType>                     VectorDataProjectionFilterType;

    VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
    vproj->SetInput(vd);
    vproj->SetInputKeywordList(inputImage->GetImageKeywordlist());
    vproj->SetInputOrigin(inputImage->GetOrigin());
    vproj->SetInputSpacing(inputImage->GetSpacing());

    if( parseResult->IsOptionPresent("DEMDirectory") )
      {
      vproj->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
      }

    vproj->Update();

    projectedVD = vproj->GetOutput();
    }

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(projectedVD);
  vdwriter->SetFileName(parseResult->GetParameterString("OutputVectorData"));

  vdwriter->Update();

  return EXIT_SUCCESS;
}
}
