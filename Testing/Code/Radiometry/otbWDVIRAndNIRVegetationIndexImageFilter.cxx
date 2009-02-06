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

#include "otbRAndNIRVegetationIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVegetationIndex.h"


int otbWDVIRAndNIRVegetationIndexImageFilter(int argc, char * argv[])
{
  const unsigned int                      Dimension = 2;
  typedef double                          PixelType;
  typedef otb::Image<PixelType,Dimension> InputRImageType;
  typedef otb::Image<PixelType,Dimension> InputNIRImageType;
  typedef otb::Image<double,Dimension>    OutputImageType;

  typedef otb::ImageFileReader<InputRImageType>    RReaderType;
  typedef otb::ImageFileReader<InputNIRImageType>  NIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>    WriterType;

  typedef otb::Functor::WDVI< InputRImageType::PixelType,
                                InputNIRImageType::PixelType,
                                OutputImageType::PixelType > FunctorType;

  typedef otb::RAndNIRVegetationIndexImageFilter< InputRImageType,
                                                      InputNIRImageType,
                                                      OutputImageType,
                                                      FunctorType > RAndNIRVegetationIndexImageFilterType;

  // Instantiating object
  RAndNIRVegetationIndexImageFilterType::Pointer filter = RAndNIRVegetationIndexImageFilterType::New();
  RReaderType::Pointer readerR = RReaderType::New();
  NIRReaderType::Pointer readerNIR = NIRReaderType::New();
  WriterType::Pointer writer = WriterType::New();


  const char * inputFilenameR  = argv[1];
  const char * inputFilenameNIR  = argv[2];
  const char * outputFilename = argv[3];

  double  g(::atof(argv[4]));

  readerR->SetFileName( inputFilenameR );
  readerNIR->SetFileName( inputFilenameNIR );
  writer->SetFileName( outputFilename  );
  filter->SetInputR( readerR->GetOutput() );
  filter->SetInputNIR( readerNIR->GetOutput() );

  filter->GetFunctor().SetG(g);

  writer->SetInput( filter->GetOutput() );
  writer->Update();


  return EXIT_SUCCESS;
}

