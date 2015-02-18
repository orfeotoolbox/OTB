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

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

namespace Functor
{
template <class TInput, class TOutput>
class UnaryFunctorNeighborhoodWithOffseImageFilterTest
{
public:
  UnaryFunctorNeighborhoodWithOffseImageFilterTest() {}
  ~UnaryFunctorNeighborhoodWithOffseImageFilterTest() {}

  typedef TInput                                      InputScalarType;
  typedef TOutput                                     OutputScalarType;
  typedef itk::VariableLengthVector<InputScalarType>  InputVectorType;
  typedef itk::VariableLengthVector<OutputScalarType> OutputVectorType;
  typedef itk::Offset<>                               OffsetType;
  typedef itk::Neighborhood<InputScalarType, 2>       NeighborhoodType;
  typedef itk::Neighborhood<InputVectorType, 2>       NeighborhoodVectorType;

  void SetOffset(OffsetType off)
  {
    m_Offset = off;
  }
  OffsetType GetOffset()
  {
    return m_Offset;
  }

  inline OutputScalarType operator ()(const NeighborhoodType& neigh)
  {
    return (static_cast<OutputScalarType>(neigh.GetCenterValue()));
  }
  inline OutputVectorType operator ()(const NeighborhoodVectorType& neigh)
  {
    return (static_cast<OutputVectorType>(neigh.GetCenterValue()));

  }
private:
  OffsetType m_Offset;
};
}

int otbUnaryFunctorNeighborhoodWithOffsetImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::VectorImage<InputPixelType, Dimension>                                               ImageType;
  typedef ImageType::OffsetType                                                                     OffsetType;
  typedef otb::ImageFileReader<ImageType>                                                           ReaderType;
  typedef otb::ImageFileWriter<ImageType>                                                           WriterType;
  typedef Functor::UnaryFunctorNeighborhoodWithOffseImageFilterTest<InputPixelType, InputPixelType> FunctorType;
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType, ImageType,
      FunctorType>     UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();
  ReaderType::Pointer                              reader  = ReaderType::New();
  WriterType::Pointer                              writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  object->SetInput(reader->GetOutput());
  object->SetRadius(atoi(argv[3]));
  OffsetType offset;
  offset[0] =  atoi(argv[4]);
  offset[1] =  atoi(argv[5]);

  //object->SetOffset(offset);
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
