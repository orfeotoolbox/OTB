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

#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

namespace Functor
{
template <class TIter, class TOutput>
class UnaryFunctorNeighborhoodImageFilterTest
{
public:
  UnaryFunctorNeighborhoodImageFilterTest() {}
  ~UnaryFunctorNeighborhoodImageFilterTest() {}

  inline TOutput operator ()(const TIter& it)
  {
    return (static_cast<TOutput>(it.GetCenterPixel()));

  }
};
}

int otbUnaryFunctorNeighborhoodImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef unsigned char InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension> ImageType;
  typedef ImageType::PixelType                  PixelType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  typedef itk::ConstNeighborhoodIterator<ImageType>                             IterType;
  typedef Functor::UnaryFunctorNeighborhoodImageFilterTest<IterType, PixelType> FunctorType;
  typedef otb::UnaryFunctorNeighborhoodImageFilter<ImageType, ImageType,
      FunctorType> UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();
  ReaderType::Pointer                              reader  = ReaderType::New();
  WriterType::Pointer                              writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  object->SetInput(reader->GetOutput());
  object->SetRadius(atoi(argv[3]));
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
