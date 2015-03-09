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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"

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

int otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::Pointer connected
    = ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::New();
  std::cout << connected << std::endl;

  return EXIT_SUCCESS;
}

int otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename          = argv[1];
  const char * outputFilename         = argv[2];
  const char * maskexpression         = argv[3];
  const char * segmentationexpression = argv[4];
  unsigned int minobjectsize          = atoi(argv[5]);
  const char * obiaexpression         = argv[6];
  unsigned int nbstreams              = atoi(argv[7]);

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::Pointer connected
    = ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::New();
  connected->GetFilter()->SetInput(reader->GetOutput());

  connected->GetFilter()->SetMaskExpression(maskexpression);
  connected->GetFilter()->SetConnectedComponentExpression(segmentationexpression);
  connected->GetFilter()->SetMinimumObjectSize(minobjectsize);
  connected->GetFilter()->SetOBIAExpression(obiaexpression);

  connected->GetStreamer()->SetNumberOfDivisionsStrippedStreaming(nbstreams);
  connected->Update();

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(connected->GetFilter()->GetOutputVectorData());
  vdwriter->SetFileName(outputFilename);
  vdwriter->Update();

  return EXIT_SUCCESS;
}
