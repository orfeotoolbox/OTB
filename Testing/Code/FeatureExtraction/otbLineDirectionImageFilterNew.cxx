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

#include "otbLineDirectionImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"


int otbLineDirectionImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension =2;
  typedef double PixelType;
  typedef otb::Image<PixelType,Dimension>                ImageType;
  typedef otb::VectorImage<PixelType,Dimension> VectorImageType;
  //typedef otb::StreamingImageFileWriter<VectorImageType> WriterType;
  typedef otb::LineDirectionImageFilter<ImageType, VectorImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();

 /*
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::LineDirectionFunctor<VectorImageTypeImageType, ImageType> FunctorType;
  typedef otb::UnaryFunctorNeighborhoodImageFilter<ImageType, FunctorType> FilterType;


  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  filter->SetInput( reader->GetOutput() );
  writer->SetInput( panTex->GetOutput() );

  writer->Update();
  */
  return EXIT_SUCCESS;
}
