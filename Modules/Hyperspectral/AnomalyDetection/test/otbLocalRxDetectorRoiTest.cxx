/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbLocalRxDetectorNonThreadFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbExtractROI.h"


int LocalRXDetectorROITest(int itkNotUsed(argc), char * argv[])
{
       typedef double PixelType;
       typedef otb::VectorImage<PixelType, 2> VectorImageType;
       typedef otb::Image<PixelType, 2> ImageType;
       typedef otb::LocalRxDetectorNonThreadFilter<VectorImageType, ImageType> LocalRxDetectorFilterType;
       typedef otb::ImageFileReader<VectorImageType> ReaderType;
       typedef unsigned char WritePixelType;
       typedef otb::Image<WritePixelType> WriteImageType;
       typedef otb::ImageFileWriter<ImageType> WriterType;
       typedef itk::RescaleIntensityImageFilter<ImageType, WriteImageType> RescalerType;
       typedef otb::ExtractROI<ImageType::PixelType, ImageType::PixelType> ExtractRoiType;



       //---------------    Parameters   ------------------//
       const char * filename = argv[1];
       const char * outputFilename = argv[2];
       const unsigned int externalRadius = atoi(argv[3]);
       const unsigned int internalRadius = atoi(argv[4]);

        const unsigned int indexX = atoi(argv[5]);
       const unsigned int indexY = atoi(argv[6]);

       const unsigned int sizeX = atoi(argv[7]);
       const unsigned int sizeY = atoi(argv[8]);

       //---------------   Read hsi file   -----------------//

       ReaderType::Pointer reader = ReaderType::New();
       reader->SetFileName(filename);
       reader->UpdateOutputInformation();

       //-------------   Anomaly detection   ---------------//

       LocalRxDetectorFilterType::Pointer rxDetector = LocalRxDetectorFilterType::New();
       rxDetector->SetExternalRadius(externalRadius);
       rxDetector->SetInternalRadius(internalRadius);
       rxDetector->SetInput(reader->GetOutput());

       //------------   Build extractRoiFilter   -----------//

       ExtractRoiType::Pointer roiExtractor = ExtractRoiType::New();

       roiExtractor->SetStartX(indexX);
        roiExtractor->SetStartY(indexY);
        roiExtractor->SetSizeX(sizeX);
        roiExtractor->SetSizeY(sizeY);
        roiExtractor->SetInput(rxDetector->GetOutput());

       //------------   rescaling and writing   ------------//

       RescalerType::Pointer rescaler = RescalerType::New();
       rescaler->SetOutputMinimum(0);
       rescaler->SetOutputMaximum(255);
       rescaler->SetInput(roiExtractor->GetOutput());

       WriterType::Pointer writer = WriterType::New();
       writer->SetFileName(outputFilename);
       writer->SetInput(roiExtractor->GetOutput());

       //----------------   Launching pipeline   ------------//

       writer->Update();

       return EXIT_SUCCESS;
}
