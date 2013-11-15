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
#include "otbSOM.h"
#include "itkRGBPixel.h"
#include "itkEuclideanDistanceMetric.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkListSample.h"
#include "itkImageRegionIterator.h"

int otbSOM(int argc, char* argv[])
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

  typedef double                                          ComponentType;
  typedef itk::VariableLengthVector<ComponentType>        PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType>   DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension> MapType;
  typedef otb::VectorImage<ComponentType, Dimension>      ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef itk::Statistics::ListSample<PixelType>          ListSampleType;

  typedef otb::SOM<ListSampleType, MapType> SOMType;
  typedef otb::ImageFileWriter<MapType>     WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  ListSampleType::Pointer listSample = ListSampleType::New();
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
