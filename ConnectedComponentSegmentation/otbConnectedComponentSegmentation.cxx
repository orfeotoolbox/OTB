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
  descriptor->AddOption("OBIAExpression", "OBIA Mu Parser expression", "OBIAexpression", 1, true,
                        ApplicationDescriptor::String);
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

  ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::Pointer connected
    = ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::New();
  connected->GetFilter()->SetInput(reader->GetOutput());

  if (parseResult->IsOptionPresent("MaskExpression"))
    connected->GetFilter()->SetMaskExpression(parseResult->GetParameterString("MaskExpression"));

  connected->GetFilter()->SetConnectedComponentExpression(parseResult->GetParameterString("ConnectedComponentExpression"));

  if (parseResult->IsOptionPresent("MinimumObjectSize"))
    connected->GetFilter()->SetMinimumObjectSize(parseResult->GetParameterInt("MinimumObjectSize"));
  else
    connected->GetFilter()->SetMinimumObjectSize(2);

  connected->GetFilter()->SetOBIAExpression(parseResult->GetParameterString("OBIAExpression"));

  connected->GetStreamer()->SetNumberOfDivisionsStrippedStreaming(2);
  connected->Update();

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(connected->GetFilter()->GetOutputVectorData());
  vdwriter->SetFileName(parseResult->GetParameterString("OutputVectorData"));
  vdwriter->Update();

  return EXIT_SUCCESS;
}
}
