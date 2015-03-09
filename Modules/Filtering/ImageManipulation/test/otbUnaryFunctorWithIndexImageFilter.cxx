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

#include "otbUnaryFunctorWithIndexImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

namespace Functor
{
template <class TInput, class TOutput>
class UnaryFunctorWithIndexImageFilterFunctorTest
{
public:
  UnaryFunctorWithIndexImageFilterFunctorTest() {}
  ~UnaryFunctorWithIndexImageFilterFunctorTest() {}

  typedef itk::Index<2> IndexType;

  inline TOutput operator ()(const TInput& inPix, IndexType itkNotUsed(index))
  {
    return (static_cast<TOutput>(inPix));
  }
};
}

int otbUnaryFunctorWithIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef double                              InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> ImageType;
  typedef ImageType::PixelType                PixelType;
  typedef otb::ImageFileReader<ImageType>     ReaderType;
  typedef otb::ImageFileWriter<ImageType>     WriterType;

  typedef Functor::UnaryFunctorWithIndexImageFilterFunctorTest<PixelType, PixelType> FunctorType;
  typedef otb::UnaryFunctorWithIndexImageFilter<ImageType, ImageType,
      FunctorType>   UnaryFunctorWithIndexImageFilterType;

  // Instantiating object
  UnaryFunctorWithIndexImageFilterType::Pointer object = UnaryFunctorWithIndexImageFilterType::New();
  ReaderType::Pointer                           reader  = ReaderType::New();
  WriterType::Pointer                           writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  object->SetInput(reader->GetOutput());
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
