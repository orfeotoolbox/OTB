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

  //Gather some information about the relative size of the classes
  //we would like to have the same number of samples per class
  std::map<int, double> classesSize;

  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType> TreeIteratorType;
  TreeIteratorType itVector(vectorReader->GetOutput()->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {
      classesSize[itVector.Get()->GetFieldAsInt(classKey)] += itVector.Get()->GetPolygonExteriorRing()->GetArea()
          / 1000000.;
      std::cout << itVector.Get()->GetFieldAsInt(classKey) << std::endl;
      std::cout << itVector.Get()->GetPolygonExteriorRing()->GetArea() / 1000000. << " km2" << std::endl;
      }
    ++itVector;
    }

  double minSize = -1;
  for (std::map<int, double>::iterator itmap = classesSize.begin(); itmap != classesSize.end(); ++itmap)
    {
    std::cout << itmap->first << ": " << itmap->second << std::endl;
    if ((minSize < 0) || (minSize > itmap->second))
      {
      minSize = itmap->second;
      }
    }
  std::cout << "MinSize: " << minSize << std::endl;

  //Compute the probability selection for each class
  std::map<int, double> classesProb;
  for (std::map<int, double>::iterator itmap = classesSize.begin(); itmap != classesSize.end(); ++itmap)
    {
    classesProb[itmap->first] = minSize / itmap->second;
    }


  std::ofstream trainingFile;
  trainingFile.open(outputSampleList.c_str());

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGenType;
  RandomGenType::Pointer randomGen = RandomGenType::GetInstance();
  randomGen->SetSeed(1234);

  std::map<int, int> classesSamplesNumber;

  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {

      ImageType::RegionType polygonRegion =
          otb::TransformPhysicalRegionToIndexRegion(itVector.Get()->GetPolygonExteriorRing()->GetBoundingRegion(),
                                                    reader->GetOutput());

      reader->GetOutput()->SetRequestedRegion(polygonRegion);
      reader->GetOutput()->PropagateRequestedRegion();
      reader->GetOutput()->UpdateOutputData();

      typedef itk::ImageRegionConstIteratorWithIndex<ImageType> IteratorType;
      IteratorType it(reader->GetOutput(), polygonRegion);
      it.GoToBegin();
      while (!it.IsAtEnd())
        {
        itk::ContinuousIndex<double, 2> point;
        reader->GetOutput()->TransformIndexToPhysicalPoint(it.GetIndex(), point);
        if (itVector.Get()->GetPolygonExteriorRing()->IsInside(point) && (randomGen->GetUniformVariate(0.0, 1.0))
            < classesProb[itVector.Get()->GetFieldAsInt(classKey)])
          {
          classesSamplesNumber[itVector.Get()->GetFieldAsInt(classKey)] += 1;
          int trainingClass = itVector.Get()->GetFieldAsInt(classKey);
          trainingFile << trainingClass;
          for (unsigned int i = 0; i < it.Get().Size(); i++)
            {
            // Careful, the numbering is 1..N in libsvm
            trainingFile << " " << i + 1 << ":" << it.Get()[i];
            }
          trainingFile << "\n";
          }
        ++it;
        }
      }
    ++itVector;
    }

  std::cout << "1: " << classesSamplesNumber[1] << std::endl;
  std::cout << "2: " << classesSamplesNumber[2] << std::endl;
  std::cout << "3: " << classesSamplesNumber[3] << std::endl;
  std::cout << "4: " << classesSamplesNumber[4] << std::endl;
  std::cout << "5: " << classesSamplesNumber[5] << std::endl;

  trainingFile.close();

  return EXIT_SUCCESS;
}
