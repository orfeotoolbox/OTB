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
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbComplexMomentImageFunction.h"

int otbComplexMomentImage( int argc, char * argv[] )
{
  const char * inputFilename  = argv[1];
  unsigned int  p((unsigned int)::atoi(argv[2]));
  unsigned int  q((unsigned int)::atoi(argv[3]));

  typedef unsigned char                                   InputPixelType;
  const   unsigned int                                  Dimension = 2;

  typedef itk::Image< InputPixelType,  Dimension >                InputImageType;
  typedef otb::ImageFileReader< InputImageType  >                       ReaderType;

  typedef std::complex<float>                                           ComplexType;
  typedef otb::ComplexMomentImageFunction<InputImageType,ComplexType>   CMType;


  ReaderType::Pointer reader         = ReaderType::New();
  CMType::Pointer function =CMType::New();

  reader->SetFileName( inputFilename  );

  reader->Update();
  function->SetInputImage( reader->GetOutput() );

  function->SetQ(q);
  function->SetP(p);

  InputImageType::IndexType index;
  index[0]=10;
  index[1]=10;

  ComplexType Result;

  Result = function->EvaluateAtIndex( index );
  std::cout << "function->EvaluateAtIndex( index ): " << Result << std::endl;

  std::cout << " Traitement local:" << std::endl;

  function->SetNeighborhoodRadius(3);
  Result = function->EvaluateAtIndex( index );
  std::cout << "function->EvaluateAtIndex( index ): " << Result << std::endl;



  return EXIT_SUCCESS;
}

