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

#include "otbScalarVectorImageTextureFunctor.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbUnaryFunctorNeighborhoodImageFilter.h"


int otbScalarVectorImageTextureFunctor(int argc, char * argv[])
{
  typedef double InputPixelType;
  const char * inputfilename(argv[1]);
  const char * outputfilename(argv[2]);
  const int radius(atoi(argv[3]));
//  const int indexfeature(atoi(argv[4]));

  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> InputImageType;
  typedef otb::VectorImage<InputPixelType,Dimension> OutputVectorImageType;
  typedef OutputVectorImageType::PixelType OutputVectorImagePixelType;

  typedef itk::ConstNeighborhoodIterator<InputImageType>   IterType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<OutputVectorImageType> WriterType;

  typedef otb::Functor::ScalarVectorImageTextureFunctor<IterType, InputImageType, OutputVectorImagePixelType>  FunctorType;

  typedef FunctorType::IndexSelectFeaturesType ListIndexType;
  typedef otb::UnaryFunctorNeighborhoodImageFilter<InputImageType, OutputVectorImageType, FunctorType> UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();
  WriterType::Pointer writer = WriterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  ListIndexType listIndexFeature;
  listIndexFeature.push_back(0);
  listIndexFeature.push_back(2);
  listIndexFeature.push_back(4);

  object->SetInput(reader->GetOutput());
  object->SetRadius(radius);
  object->GetFunctor().SetFeatureIndex(listIndexFeature);
  writer->SetInput(object->GetOutput());
  reader->SetFileName(inputfilename);

  writer->SetFileName(outputfilename);


  writer->Update();

  return EXIT_SUCCESS;
}
