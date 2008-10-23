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

#include "otbScalarImageTextureFunctor.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbUnaryFunctorNeighborhoodImageFilter.h"


int otbScalarImageTextureFunctor(int argc, char * argv[])
{
  typedef double InputPixelType;
  const char * inputfilename(argv[1]);
  const char * outputfilename(argv[2]);
  const int radius(atoi(argv[3]));
  const int indexfeature(atoi(argv[4]));

  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> InputImageType;
  
  typedef itk::ConstNeighborhoodIterator<InputImageType>   IterType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<InputImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputfilename);

  writer->SetFileName(outputfilename);



  typedef otb::Functor::ScalarImageTextureFunctor<IterType, InputImageType, InputPixelType>  FunctorType;
  
  typedef otb::UnaryFunctorNeighborhoodImageFilter<InputImageType, InputImageType, FunctorType> UnaryFunctorNeighborhoodImageFilterType;
  
  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();
  object->SetInput(reader->GetOutput());
  object->SetRadius(radius);
  object->GetFunctor().SetFeatureIndex(indexfeature);
  writer->SetInput(object->GetOutput());
  
  writer->Update();
      
  return EXIT_SUCCESS;
}
