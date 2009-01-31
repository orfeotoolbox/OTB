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
#include "itkExceptionObject.h"

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

namespace Functor
{
template <class TIter1, class TIter2, class TOutput>
class UnaryFunctorNeighborhoodWithOffseImageFilterTest
{
public:
  UnaryFunctorNeighborhoodWithOffseImageFilterTest() {};
  ~UnaryFunctorNeighborhoodWithOffseImageFilterTest() {};

  typedef TIter1 IterType1;
  typedef typename IterType1::OffsetType OffsetType;
  typedef TIter2 IterType2;


  void SetOffset(OffsetType off)
  {
    m_Offset=off;
  };
  OffsetType GetOffset()
  {
    return m_Offset;
  };

  inline TOutput operator() (const TIter1 & it1, const TIter2 & it2)
  {
    return(static_cast<TOutput>(it1.GetCenterPixel()));

  }
private:
  OffsetType m_Offset;
};
}


int otbUnaryFunctorNeighborhoodWithOffsetImageFilter(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef double                                     InputPixelType;
  const int                                          Dimension = 2;
  typedef otb::VectorImage<InputPixelType,Dimension> ImageType;
  typedef ImageType::OffsetType                      OffsetType;
  typedef ImageType::PixelType                       PixelType;
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageFileWriter<ImageType>            WriterType;
  typedef itk::ConstNeighborhoodIterator<ImageType>  IterType;;
  typedef Functor::UnaryFunctorNeighborhoodWithOffseImageFilterTest<IterType, IterType, PixelType>  FunctorType;
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType, ImageType, FunctorType>     UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();
  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
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
