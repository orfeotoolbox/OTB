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

int otbListSampleGeneratorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::VectorImage<float, 2>                          ImageType;
  typedef otb::VectorData<double, 2>                          VectorDataType;
  typedef otb::ListSampleGenerator<ImageType, VectorDataType> ListSampleGeneratorType;
  ListSampleGeneratorType::Pointer generator = ListSampleGeneratorType::New();

  std::cout << generator << std::endl;

  return EXIT_SUCCESS;
}

int otbListSampleGenerator(int argc, char* argv[])
{

  if (argc != 8)
    {
    std::cerr << "Usage: " << argv[0] << " inputImage inputVectorData outputTrainingFile outputValidationFile"
              << " maxTrainingSize maxValidationSize validationTrainingProportion"
              << std::endl;
    return EXIT_FAILURE;
    }

  std::string imageFilename = argv[1];
  std::string vectorDataFilename = argv[2];
  std::string outputSampleList = argv[3];
  std::string outputSampleListValidation = argv[4];
  int         maxTrainingSize = atoi(argv[5]);
  int         maxValidationSize = atoi(argv[6]);
  double      validationTrainingProportion = atof(argv[7]);

  std::string classKey = "Class";

  typedef double                          PixelType;
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
  generator->SetValidationTrainingProportion(validationTrainingProportion);

  generator->SetInput(reader->GetOutput());
  generator->SetInputVectorData(vectorReader->GetOutput());
  std::cout << generator << std::endl;

  generator->Update();
  std::cout << generator << std::endl;

  std::cout << "Number of classes: " << generator->GetNumberOfClasses() << std::endl;

    {
    ListSampleGeneratorType::ListSamplePointerType samples = generator->GetTrainingListSample();
    ListSampleGeneratorType::ListLabelPointerType  labels = generator->GetTrainingListLabel();

    typedef ListSampleGeneratorType::ListSampleType::ConstIterator SampleIterator;
    typedef ListSampleGeneratorType::ListLabelType::ConstIterator  LabelIterator;

    std::ofstream trainingFile;
    trainingFile.open(outputSampleList.c_str());

    SampleIterator sampleIt = samples->Begin();
    LabelIterator  labelIt = labels->Begin();
    while (sampleIt != samples->End())
      {
      trainingFile << labelIt.GetMeasurementVector()[0];
      for (unsigned int i = 0; i < sampleIt.GetMeasurementVector().Size(); ++i)
        {
        // Careful, the numbering is 1..N in libsvm
        trainingFile << " " << i + 1 << ":" << sampleIt.GetMeasurementVector()[i];
        }
      trainingFile << "\n";
      ++sampleIt;
      ++labelIt;
      }

    trainingFile.close();
    }

    {

    ListSampleGeneratorType::ListSamplePointerType samples = generator->GetValidationListSample();
    ListSampleGeneratorType::ListLabelPointerType  labels = generator->GetValidationListLabel();

    typedef ListSampleGeneratorType::ListSampleType::ConstIterator SampleIterator;
    typedef ListSampleGeneratorType::ListLabelType::ConstIterator  LabelIterator;

    std::ofstream validationFile;
    validationFile.open(outputSampleListValidation.c_str());

    SampleIterator sampleIt = samples->Begin();
    LabelIterator  labelIt = labels->Begin();
    while (sampleIt != samples->End())
      {
      validationFile << labelIt.GetMeasurementVector()[0];
      for (unsigned int i = 0; i < sampleIt.GetMeasurementVector().Size(); ++i)
        {
        // Careful, the numbering is 1..N in libsvm
        validationFile << " " << i + 1 << ":" << sampleIt.GetMeasurementVector()[i];
        }
      validationFile << "\n";
      ++sampleIt;
      ++labelIt;
      }

    validationFile.close();
    }

  return EXIT_SUCCESS;
}
