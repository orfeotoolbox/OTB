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
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbSOMActivationBuilder.h"
#include "itkVariableLengthVector.h"
#include "itkEuclideanDistanceMetric.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkListSample.h"
#include "otbVectorImage.h"
#include "itkImageRegionIterator.h"

int otbSOMActivationBuilder(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;
  char *             vectorSetFileName = argv[1];
  char *             mapFileName = argv[2];
  char *             outputFileName = argv[3];

  typedef float                                         ComponentType;
  typedef unsigned char                                 OutputPixelType;
  typedef itk::VariableLengthVector<ComponentType>      PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType> DistanceType;

  typedef otb::SOMMap<PixelType, DistanceType, Dimension> MapType;
  typedef otb::ImageFileReader<MapType>                   MapReaderType;

  typedef otb::VectorImage<ComponentType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>       ReaderType;
  typedef itk::Statistics::ListSample<PixelType>     ListSampleType;

  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;

  typedef otb::SOMActivationBuilder<ListSampleType, MapType, OutputImageType> SOMActivationBuilderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(vectorSetFileName);
  reader->Update();

  ListSampleType::Pointer listSample = ListSampleType::New();
  listSample->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());

  itk::ImageRegionIterator<InputImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    listSample->PushBack(it.Get());
    ++it;
    }

  MapReaderType::Pointer mapReader = MapReaderType::New();
  mapReader->SetFileName(mapFileName);

  SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();
  somAct->SetInput(mapReader->GetOutput());
  somAct->SetListSample(listSample);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(somAct->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
