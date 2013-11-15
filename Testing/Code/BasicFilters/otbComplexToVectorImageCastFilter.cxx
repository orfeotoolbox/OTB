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
#include "itkMacro.h"

#include "otbComplexToVectorImageCastFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbComplexToVectorImageCastFilterNew(int argc, char * argv[])
{
  typedef std::complex<float>        ComplexType;
  typedef otb::VectorImage<ComplexType, 2> CplxImageType;
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
  typedef otb::VectorImage<ComplexType, 2> VCplxImageType;
  typedef otb::VectorImage<float, 2> VectorImageType;

  typedef otb::ComplexToVectorImageCastFilter<VCplxImageType, VectorImageType> VFilterType;
  typedef otb::ComplexToVectorImageCastFilter<CplxImageType, VectorImageType>  FilterType;

  typedef otb::ImageFileReader<CplxImageType>                                 ReaderType;
  typedef otb::ImageFileReader<VCplxImageType>                                VReaderType;

  typedef otb::ImageFileWriter<VectorImageType>                               WriterType;


   // Instantiating objects
  ReaderType::Pointer reader = ReaderType::New();
  VReaderType::Pointer vreader = VReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  WriterType::Pointer vwriter = WriterType::New();
  FilterType::Pointer caster = FilterType::New();
  VFilterType::Pointer vcaster = VFilterType::New();

  reader->SetFileName(argv[1]);
  vreader->SetFileName(argv[1]);

  caster->SetInput(reader->GetOutput());
  vcaster->SetInput(vreader->GetOutput());

  writer->SetFileName(argv[2]);
  writer->SetInput( caster->GetOutput());
  vwriter->SetFileName(argv[3]);
  vwriter->SetInput( vcaster->GetOutput());

  writer->Update();
  vwriter->Update();

  return EXIT_SUCCESS;
}
