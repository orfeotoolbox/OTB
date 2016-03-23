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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbReciprocalPauliDecompImageFilter.h"
#include "otbNRIBandImagesToOneNComplexBandsImage.h"

int otbReciprocalPauliDecompImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const unsigned int Dimension = 2;

  typedef std::complex<double>   PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  typedef otb::ReciprocalPauliDecompImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}


int otbReciprocalPauliDecompImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilenameHH = argv[1];
  const char * inputFilenameHV = argv[2];
  const char * inputFilenameVV = argv[3];
  const char * outputFilename = argv[4];


  typedef std::complex<double>  ComplexPixelType;
  const unsigned int Dimension = 2;


  typedef otb::VectorImage<ComplexPixelType, Dimension>  ComplexVectorImageType;
  typedef otb::VectorImage<double, Dimension>          RealVectorImageType;

  typedef otb::NRIBandImagesToOneNComplexBandsImage<RealVectorImageType, ComplexVectorImageType> NRITOOneCFilterType;

  typedef otb::ImageFileReader<RealVectorImageType>  ReaderType;
  typedef otb::ImageFileWriter<ComplexVectorImageType> WriterType;
  
  typedef otb::ReciprocalPauliDecompImageFilter<ComplexVectorImageType, ComplexVectorImageType> FilterType;
  
  ReaderType::Pointer readerHH = ReaderType::New();
  ReaderType::Pointer readerHV = ReaderType::New();
  ReaderType::Pointer readerVV = ReaderType::New();
  NRITOOneCFilterType::Pointer nriToOneCfilter = NRITOOneCFilterType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer paulifilter = FilterType::New();
        
  readerHH->SetFileName(inputFilenameHH);
  readerHV->SetFileName(inputFilenameHV);
  readerVV->SetFileName(inputFilenameVV);
  
  nriToOneCfilter->SetInput(0,readerHH->GetOutput());
  nriToOneCfilter->SetInput(1,readerHV->GetOutput());
  nriToOneCfilter->SetInput(2,readerVV->GetOutput());
 
  paulifilter->SetInput(nriToOneCfilter->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(paulifilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
