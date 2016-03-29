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
#include "itkMeanImageFilter.h"
#include "otbReciprocalBarnesDecompImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbSinclairReciprocalImageFilter.h"
#include "otbSinclairToReciprocalCoherencyMatrixFunctor.h"

int otbReciprocalBarnesDecompImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const unsigned int Dimension = 2;

  typedef std::complex<double>   PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  typedef otb::ReciprocalBarnesDecompImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}


int otbReciprocalBarnesDecompImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilenameHH = argv[1];
  const char * inputFilenameHV = argv[2];
  const char * inputFilenameVV = argv[3];
  int size = atoi(argv[4]);
  const char * outputFilename = argv[5];


  typedef std::complex<double>  ComplexPixelType;
  const unsigned int Dimension = 2;


  typedef otb::Image<ComplexPixelType, Dimension>  	   ComplexImageType;
  typedef otb::VectorImage<ComplexPixelType, Dimension>  ComplexVectorImageType;


  typedef otb::ImageFileReader<ComplexImageType>  ReaderType;
  typedef otb::ImageFileWriter<ComplexVectorImageType> WriterType;
  
  
  typedef otb::SinclairReciprocalImageFilter<ComplexImageType, ComplexImageType, ComplexImageType, ComplexVectorImageType, 
  otb::Functor::SinclairToReciprocalCovarianceMatrixFunctor<ComplexImageType::PixelType,
                                    ComplexImageType::PixelType,
                                    ComplexImageType::PixelType,
                                    ComplexVectorImageType::PixelType> > SinclairToCovFilterType;
  
  
  typedef itk::MeanImageFilter<ComplexImageType, ComplexImageType>         MeanFilterType;
  typedef otb::PerBandVectorImageFilter<ComplexVectorImageType, ComplexVectorImageType, MeanFilterType> PerBandMeanFilterType;
  
  
  typedef otb::ReciprocalBarnesDecompImageFilter<ComplexVectorImageType, ComplexVectorImageType> FilterType;
  
  

  ReaderType::Pointer readerHH = ReaderType::New();
  ReaderType::Pointer readerHV = ReaderType::New();
  ReaderType::Pointer readerVV = ReaderType::New();
  
  WriterType::Pointer writer = WriterType::New();

  SinclairToCovFilterType::Pointer sinclairtocov = SinclairToCovFilterType::New();
  PerBandMeanFilterType::Pointer perBand = PerBandMeanFilterType::New();
  FilterType::Pointer barnesfilter = FilterType::New();
        
  
  MeanFilterType::InputSizeType radius;
  radius.Fill( size );
  perBand->GetFilter()->SetRadius(radius);
 
 
  readerHH->SetFileName(inputFilenameHH);
  readerHV->SetFileName(inputFilenameHV);
  readerVV->SetFileName(inputFilenameVV);
  
  sinclairtocov->SetInputHH(readerHH->GetOutput());
  sinclairtocov->SetInputHV_VH(readerHV->GetOutput());
  sinclairtocov->SetInputVV(readerVV->GetOutput());
 
  perBand->SetInput(sinclairtocov->GetOutput());
  
  barnesfilter->SetInput(perBand->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(barnesfilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
