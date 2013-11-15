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
#include "itkMacro.h"
#include "otbVectorImage.h"
#include "otbSOMMap.h"
#include "itkRGBPixel.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkListSample.h"
#include "itkImageRegionIterator.h"

#include "itkVariableLengthVector.h"
#include "otbSOMWithMissingValue.h"
#include "otbFlexibleDistanceWithMissingValue.h"



int otbSOMWithMissingValueTest(int argc, char* argv[])
{
  const unsigned int Dimension = 2;
  char *             inputFileName = argv[1];
  char *             outputFileName = argv[2];
  unsigned int       sizeX = atoi(argv[3]);
  unsigned int       sizeY = atoi(argv[4]);
  unsigned int       neighInitX = atoi(argv[5]);
  unsigned int       neighInitY = atoi(argv[6]);
  unsigned int       nbIterations = atoi(argv[7]);
  double             betaInit = atof(argv[8]);
  double             betaEnd = atof(argv[9]);
  double             initValue = atof(argv[10]);


  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef ImageType::PixelType                   VectorType;

  typedef otb::Statistics::FlexibleDistanceWithMissingValue<VectorType> DistanceType;
  typedef otb::SOMMap<VectorType, DistanceType, Dimension>  MapType;
  typedef itk::Statistics::ListSample<VectorType>           SampleListType;
  typedef otb::Functor::CzihoSOMLearningBehaviorFunctor     LearningBehaviorFunctorType;
  typedef otb::Functor::CzihoSOMNeighborhoodBehaviorFunctor NeighborhoodBehaviorFunctorType;
  typedef otb::SOMWithMissingValue<SampleListType, MapType,
      LearningBehaviorFunctorType, NeighborhoodBehaviorFunctorType> SOMType;

  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::ImageFileWriter<MapType>                   WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  SampleListType::Pointer listSample = SampleListType::New();
  listSample->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  
  itk::ImageRegionIterator<ImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    listSample->PushBack(it.Get());
    ++it;
    }

  std::cout << "LIST SAMPLE SIZE: " << listSample->GetMeasurementVectorSize() << std::endl;

  // Instantiation
  SOMType::Pointer som = SOMType::New();
  som->SetListSample(listSample);
  SOMType::SizeType size;
  size[0] = sizeX;
  size[1] = sizeY;
  som->SetMapSize(size);
  SOMType::SizeType radius;
  radius[0] = neighInitX;
  radius[1] = neighInitY;
  som->SetNeighborhoodSizeInit(radius);
  som->SetNumberOfIterations(nbIterations);
  som->SetBetaInit(betaInit);
  som->SetBetaEnd(betaEnd);
  som->SetMaxWeight(initValue);
  som->SetRandomInit(false);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(som->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
