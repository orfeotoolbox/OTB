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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"

#include "otbListSampleGenerator.h"

//temporary
#include "itkPreOrderTreeIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"


int otbListSampleGenerator(int argc, char* argv[])
{

  if (argc != 7)
    {
    std::cerr << "Usage: " << argv[0] << " inputImage inputVectorData outputTrainingFile" 
        << " maxTrainingSize maxValidationSize validationTrainingRatio"
        << std::endl;
    return EXIT_FAILURE;
    }

  
  std::string imageFilename = argv[1];
  std::string vectorDataFilename = argv[2];
  std::string outputSampleList = argv[3];
  int maxTrainingSize = atoi(argv[4]);
  int maxValidationSize = atoi(argv[4]);
  int validationTrainingRatio = atof(argv[4]);
  
  std::string classKey = "Class";

  typedef double                          PixelType;
  typedef int                             LabeledPixelType;
  typedef otb::VectorImage<PixelType, 2>  ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->UpdateOutputInformation();

  typedef otb::VectorData<float, 2>                 VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;

  VectorDataReaderType::Pointer vectorReader = VectorDataReaderType::New();
  vectorReader->SetFileName(vectorDataFilename);
  vectorReader->Update();

  std::cout << "Number of elements in the tree: " << vectorReader->GetOutput()->Size() << std::endl;

  typedef otb::ListSampleGenerator<ImageType, VectorDataType> ListSampleGeneratorType;
  ListSampleGeneratorType::Pointer generator = ListSampleGeneratorType::New();
  generator->SetMaxTrainingSize(maxTrainingSize);
  generator->SetMaxValidationSize(maxValidationSize);
  generator->SetValidationTrainingRatio(validationTrainingRatio);

  generator->SetInput(reader->GetOutput());
  generator->SetInputVectorData(vectorReader->GetOutput());
  std::cout << generator << std::endl;

  generator->Update();
  std::cout << generator << std::endl;

  std::cout << "Number of classes: " << generator->GetNumberOfClasses() << std::endl;

  ListSampleGeneratorType::ListSamplePointerType samples = generator->GetTrainingListSample();
  ListSampleGeneratorType::ListLabelPointerType  labels  = generator->GetTrainingListLabel();

  typedef ListSampleGeneratorType::ListSampleType::ConstIterator SampleIterator;
  typedef ListSampleGeneratorType::ListLabelType::ConstIterator  LabelIterator;
  
  std::ofstream trainingFile;
  trainingFile.open(outputSampleList.c_str());
  
  SampleIterator sampleIt = samples->Begin();
  LabelIterator labelIt = labels->Begin();
  while (sampleIt != samples->End())
    {
    trainingFile << labelIt.GetMeasurementVector()[0];
    for (unsigned int i = 0; i < sampleIt.GetMeasurementVector().Size(); i++)
      {
      // Careful, the numbering is 1..N in libsvm
      trainingFile << " " << i + 1 << ":" << sampleIt.GetMeasurementVector()[i];
      }
    trainingFile << "\n";
    ++sampleIt;
    ++labelIt;
    }
  
  trainingFile.close();
  
   return EXIT_SUCCESS;
}
