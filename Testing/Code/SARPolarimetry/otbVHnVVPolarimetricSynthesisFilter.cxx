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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPolarimetricSynthesisFilter.h"


int otbVHnVVPolarimetricSynthesisFilter( int argc, char * argv[] )
{
  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];

  const char * outputFilename = argv[3];

  double  PsiI = strtod(argv[4],NULL);
  double  KhiI = strtod(argv[5],NULL);
  double  PsiR = strtod(argv[6],NULL);
  double  KhiR = strtod(argv[7],NULL);

  typedef std::complex <double>                   InputPixelType;
  typedef double    	                        OutputPixelType;
  const   unsigned int                            Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >  InputImageType;
  typedef otb::Image< OutputPixelType, Dimension >  OutputImageType;

  typedef otb::ImageFileReader< InputImageType  >   ReaderType;
  typedef otb::ImageFileWriter< OutputImageType >   WriterType;

  typedef otb::PolarimetricSynthesisFilter<  InputImageType,InputImageType,InputImageType,InputImageType,OutputImageType >   FilterType;

  FilterType::Pointer polarimetricSynthesis = FilterType::New();

  polarimetricSynthesis->SetPsiI( PsiI );
  polarimetricSynthesis->SetKhiI( KhiI );
  polarimetricSynthesis->SetPsiR( PsiR );
  polarimetricSynthesis->SetKhiR( KhiR );

  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader1->SetFileName( inputFilename1 );
  reader2->SetFileName( inputFilename2 );
  writer->SetFileName( outputFilename );

  polarimetricSynthesis->SetInputVH( reader1->GetOutput() );
  polarimetricSynthesis->SetInputVV( reader2->GetOutput() );

  std::cout<<polarimetricSynthesis->GetNumberOfValidRequiredInputs()<<std::endl;
  std::cout<<polarimetricSynthesis->GetInputs().size()<<std::endl;

  writer->SetInput( polarimetricSynthesis->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}


