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


#include "otbVectorDataToRoadDescriptionFilter.h"

#include "otbVectorData.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataExtractROI.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataProperties.h"
#include "otbRemoteSensingRegion.h"
#include "otbVectorDataIntoImageProjectionFilter.h"




int otbVectorDataToRoadDescriptionFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef double                          PrecisionType;
  typedef otb::VectorData<PrecisionType>  VectorDataType;
  typedef otb::VectorImage<PrecisionType> ImageType;

  typedef otb::VectorDataToRoadDescriptionFilter<VectorDataType, ImageType>
                                          VectorDataToRoadDescriptionFilterType;

  VectorDataToRoadDescriptionFilterType::Pointer filter =
    VectorDataToRoadDescriptionFilterType::New();

  std::cout<<filter<<std::endl;

  return EXIT_SUCCESS;
}

int otbVectorDataToRoadDescriptionFilter(int itkNotUsed(argc), char* argv[])
{
  const char * inputVD  = argv[1];
  const char * inputImg = argv[2];
  const char * inputDB  = argv[3];
  const char * outputVD = argv[4];
  const char * DEMDirectory = argv[5];
  int DisplayWarnings   = atoi(argv[6]);

  typedef double                             PrecisionType;
  typedef otb::VectorData<PrecisionType, 2>  VectorDataType;
  typedef otb::VectorImage<PrecisionType>    ImageType;

  typedef otb::VectorDataToRoadDescriptionFilter<VectorDataType, ImageType>
                                          VectorDataToRoadDescriptionFilterType;

  typedef otb::ImageFileReader<ImageType> ImageReaderType;
  typedef otb::VectorDataFileReader<VectorDataType>
                                          VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>
                                          VectorDataWriterType;
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, ImageType>
                                          VectorDataReProjFilter;


  VectorDataReaderType::Pointer vdReader = VectorDataReaderType::New();
  VectorDataReaderType::Pointer DBReader = VectorDataReaderType::New();
  ImageReaderType::Pointer      reader   = ImageReaderType::New();
  VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  VectorDataReProjFilter::Pointer vdReProjFilter = VectorDataReProjFilter::New();

  VectorDataToRoadDescriptionFilterType::Pointer filter =
    VectorDataToRoadDescriptionFilterType::New();

  if (!DisplayWarnings)
    {
    reader->SetGlobalWarningDisplay(0);
    }
  otb::DEMHandler::Instance()->OpenDEMDirectory(DEMDirectory);

  reader->SetFileName(inputImg);
  reader->UpdateOutputInformation();

  DBReader->SetFileName(inputDB);
  DBReader->Update();

  vdReader->SetFileName(inputVD);
  vdReader->Update();

  vdReProjFilter->SetInputImage(reader->GetOutput());
  vdReProjFilter->SetInputVectorData(vdReader->GetOutput());
  vdReProjFilter->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilter->Update();

  filter->SetInput(vdReProjFilter->GetOutput());
  filter->AddOpticalImage(reader->GetOutput());
  filter->AddBuildingsDB(DBReader->GetOutput());

  vdWriter->SetFileName(outputVD);
  vdWriter->SetInput(filter->GetOutput());
  vdWriter->Update();

  return EXIT_SUCCESS;
}
