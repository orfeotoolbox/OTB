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
#include "otbDSFuzzyModelEstimation.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorDataToRandomLineGenerator.h"

#include "otbVectorDataProjectionFilter.h"  //Envelope TO WGS84 (OSM friendly)
#include "otbVectorDataIntoImageProjectionFilter.h" //WGS84 to index

#include "otbVectorDataToRoadDescriptionFilter.h"
#include "otbVectorDataToDSValidatedVectorDataFilter.h"


//TESTING
#include "otbVectorDataFileWriter.h"


namespace otb
{

int DSFuzzyModelEstimation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("DSFuzzyModelEstimation");
  descriptor->SetDescription("Estimate feature fuzzy model parameters using an image and an VectorData");
  descriptor->AddOption("InputImage", "Support to estimate the models on",
                        "in", 1, true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("InputVectorData", "Ground Truth Vector Data",
                        "vdin", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Hypothesis", "Dempster Shafer study hypothesis",
                        "hyp", 1, false, ApplicationDescriptor::StringList);
  descriptor->AddOption("Criterion", "Dempster Shafer Criterion (by default (Belief+Plausibility)/2 >= 0.5))",
                          "cri", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("DEMDirectory", "DEM directory",
                          "dem", 1, false, ApplicationDescriptor::DirectoryName);

  //TESTING PURPOSE
  descriptor->AddOption("Output", "OutputVectorData",
                        "out", 1, true, ApplicationDescriptor::FileName);
  return EXIT_SUCCESS;
}


int DSFuzzyModelEstimation::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::VectorData<double>                     VectorDataType;
  typedef VectorDataType::ValuePrecisionType          PrecisionType;
  typedef VectorDataType::PrecisionType               CoordRepType;
  typedef otb::VectorDataFileReader<VectorDataType>   VectorDataReaderType;
  typedef otb::VectorDataToRandomLineGenerator<VectorDataType>
                                                      RandomGeneratorType;

  typedef otb::VectorImage<PrecisionType , 2>         ImageType;
  typedef otb::ImageFileReader<ImageType>             ImageReaderType;

  typedef otb::ImageToEnvelopeVectorDataFilter<ImageType, VectorDataType>
                                                      EnvelopeFilterType;

  typedef otb::VectorDataProjectionFilter<VectorDataType, ImageType>
                                                      VectorDataProjFilter;

  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, ImageType>
                                                      VectorDataReProjFilter;

  typedef otb::VectorDataToRoadDescriptionFilter<VectorDataType, ImageType>
                                                      DescriptionFilterType;

  typedef otb::VectorDataToDSValidatedVectorDataFilter<VectorDataType, PrecisionType>
                                                      ValidationFilterType;


  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataWriterType;


  //Instantiate
  ImageReaderType::Pointer                   imgReader = ImageReaderType::New();
  VectorDataReaderType::Pointer               vdReader = VectorDataReaderType::New();
  EnvelopeFilterType::Pointer           envelopeFilter = EnvelopeFilterType::New();
  RandomGeneratorType::Pointer       vdRandomGenerator = RandomGeneratorType::New();
  VectorDataReProjFilter::Pointer     vdReProjFilterGT = VectorDataReProjFilter::New();
  VectorDataReProjFilter::Pointer     vdReProjFilterRL = VectorDataReProjFilter::New();

  VectorDataWriterType::Pointer               vdWriter = VectorDataWriterType::New();

  //Read the image
  imgReader->SetFileName(parseResult->GetParameterString("InputImage"));
  imgReader->UpdateOutputInformation();

  //Read the vector data
  vdReader->SetFileName(parseResult->GetParameterString("InputVectorData"));
  vdReader->Update();

  //Generate the envelope
  envelopeFilter->SetInput(imgReader->GetOutput()); //->Output in WGS84
  envelopeFilter->Update();

  //Generate Random lines
  vdRandomGenerator->SetInput(envelopeFilter->GetOutput());
  vdRandomGenerator->SetNumberOfOutputLine(vdReader->GetOutput()->Size());
  vdRandomGenerator->SetMinLineSize(4);
  vdRandomGenerator->SetMaxLineSize(20);

  //Reproject into image index coordinates
  vdReProjFilterGT->SetInputImage(imgReader->GetOutput());
  vdReProjFilterGT->SetInputVectorData(vdReader->GetOutput());
  vdReProjFilterGT->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
  vdReProjFilterGT->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilterGT->Update();

  vdReProjFilterRL->SetInputImage(imgReader->GetOutput());
  vdReProjFilterRL->SetInputVectorData(vdRandomGenerator->GetOutput());
  vdReProjFilterRL->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
  vdReProjFilterRL->SetUseOutputSpacingAndOriginFromImage(true);
  vdReProjFilterRL->Update();



  vdWriter->SetInput(vdReProjFilterRL->GetOutput());
  vdWriter->SetFileName(parseResult->GetParameterString("Output"));
  vdWriter->Update();

  return EXIT_SUCCESS;
}

}

