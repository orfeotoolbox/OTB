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

#include "otbComplexToVectorImageCastFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbComplexToVectorImageCastFilterNew(int argc, char * argv[])
{
  typedef std::complex<float>        ComplexType;
  typedef otb::Image<ComplexType, 2> CplxImageType;
  typedef otb::VectorImage<float, 2> VectorImageType;

  typedef otb::ComplexToVectorImageCastFilter<CplxImageType, VectorImageType> FilterType;

  // Instantiating object
  FilterType::Pointer caster = FilterType::New();


  return EXIT_SUCCESS;
}

int otbComplexToVectorImageCastFilterTest(int argc, char * argv[])
{
  typedef std::complex<float>        ComplexType;
  typedef otb::Image<ComplexType, 2> CplxImageType;
  typedef otb::VectorImage<float, 2> VectorImageType;

  typedef otb::ComplexToVectorImageCastFilter<CplxImageType, VectorImageType> FilterType;
  typedef otb::ImageFileReader<CplxImageType>                                 ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>                               WriterType;
 

   // Instantiating objects
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer caster = FilterType::New();

  reader->SetFileName(argv[1]);
  caster->SetInput(reader->GetOutput());
  writer->SetFileName(argv[2]);
  writer->SetInput( caster->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
