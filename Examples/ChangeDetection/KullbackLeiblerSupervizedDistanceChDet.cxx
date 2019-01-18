/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"

#include "otbKullbackLeiblerSupervizedDistanceImageFilter.h"

int main(int argc, char * argv[])
{
  try
    {
    if( argc < 5 )
      {
      std::cerr << "Usage: " << argv[0];
      std::cerr << "inputNameImage1 inputNameImage2 ROIImageName outputName ";
      std::cerr << "[winSize=35]";
      return EXIT_FAILURE;
      }
    /*
     *  Types declaration
     */

    const unsigned int Dimension = 2;

    typedef double        InputPixelType;
    typedef unsigned char TrainingPixelType;

    typedef otb::Image<InputPixelType, Dimension> ImageType;
    typedef otb::ImageFileReader<ImageType>       ReaderType;

    typedef otb::Image<TrainingPixelType, Dimension> TrainingImageType;
    typedef otb::ImageFileReader<TrainingImageType>  TrainingReaderType;

    /*
     * Extract command line parameters
     */

    std::string inputImageFileName1 = argv[1];
    std::string inputImageFileName2 = argv[2];
    std::string inputTrainingImageFileName = argv[3];
    std::string outputImageFileName = argv[4];

    int winSize = 35;
    if ( argc == 6 )
      winSize = std::stoi(argv[5]);

    /*
     *  JustDoIt
     */

    ImageType::Pointer  img1 = ImageType::New();
    ReaderType::Pointer reader1 = ReaderType::New();
    reader1->SetFileName(inputImageFileName1);
    //reader1->Update();
    img1 = reader1->GetOutput();

    ImageType::Pointer  img2 = ImageType::New();
    ReaderType::Pointer reader2 = ReaderType::New();
    reader2->SetFileName(inputImageFileName2);
    //reader2->Update();
    img2 = reader2->GetOutput();

    TrainingImageType::Pointer  imgRoi = TrainingImageType::New();
    TrainingReaderType::Pointer readerRoi = TrainingReaderType::New();
    readerRoi->SetFileName(inputTrainingImageFileName);
    //readerRoi->Update();
    imgRoi = readerRoi->GetOutput();

    typedef otb::KullbackLeiblerSupervizedDistanceImageFilter<
        ImageType, ImageType, TrainingImageType, ImageType> FilterType;

    FilterType::Pointer changeDetector = FilterType::New();
    changeDetector->SetRadius((winSize - 1) / 2);
    changeDetector->SetInput1(img1);
    changeDetector->SetInput2(img2);

    // Once img1 and img2 are connected, it is time to connect imgRoi
    // to perform the parameters estimation of the non-changed area.

    changeDetector->SetTrainingArea(imgRoi);

    typedef otb::CommandProgressUpdate<FilterType> CommandType;
    CommandType::Pointer observer = CommandType::New();
    changeDetector->AddObserver(itk::ProgressEvent(), observer);

    typedef otb::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputImageFileName);
    writer->SetInput(changeDetector->GetOutput());

    writer->Update();

    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
