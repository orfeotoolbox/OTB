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



#include <fstream>
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbSOMClassifier.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkListSample.h"

int otbSOMClassifier(int argc, char* argv[])
{
  if (argc != 4)
    {
    std::cout << "Usage : " << argv[0] << " inputImage modelFile outputImage"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char * imageFilename  = argv[1];
  const char * mapFilename  = argv[2];
  const char * outputFilename = argv[3];

  typedef double InputPixelType;
  typedef int    LabelPixelType;
  const unsigned int Dimension = 2;

  typedef itk::VariableLengthVector<InputPixelType>                  PixelType;
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType>              DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension>            SOMMapType;
  typedef otb::VectorImage<InputPixelType, Dimension>                InputImageType;
  typedef otb::ImageFileReader<InputImageType>                       ReaderType;
  typedef otb::ImageFileReader<SOMMapType>                           SOMReaderType;
  typedef itk::Statistics::ListSample<PixelType>                     SampleType;
  typedef otb::SOMClassifier<SampleType, SOMMapType, LabelPixelType> ClassifierType;
  typedef otb::Image<LabelPixelType, Dimension>                      OutputImageType;
  typedef itk::ImageRegionIterator<OutputImageType>                  OutputIteratorType;
  typedef otb::ImageFileWriter<OutputImageType>                      WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->Update();
  std::cout << "Image read" << std::endl;

  SOMReaderType::Pointer somreader = SOMReaderType::New();
  somreader->SetFileName(mapFilename);
  somreader->Update();
  std::cout << "SOM map read" << std::endl;

  SampleType::Pointer listSample = SampleType::New();
  listSample->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  
  itk::ImageRegionIterator<InputImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    listSample->PushBack(it.Get());
    ++it;
    }

  ClassifierType::Pointer classifier = ClassifierType::New();
  classifier->SetSample(listSample.GetPointer());
  classifier->SetMap(somreader->GetOutput());
  classifier->Update();

  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();

  ClassifierType::OutputType*               membershipSample = classifier->GetOutput();
  ClassifierType::OutputType::ConstIterator m_iter =  membershipSample->Begin();
  ClassifierType::OutputType::ConstIterator m_last =  membershipSample->End();

  OutputIteratorType outIt(outputImage, outputImage->GetLargestPossibleRegion());
  outIt.GoToBegin();

  while (m_iter != m_last && !outIt.IsAtEnd())
    {
    outIt.Set(m_iter.GetClassLabel());
    ++m_iter;
    ++outIt;
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(outputImage);
  writer->Update();

  return EXIT_SUCCESS;
}
