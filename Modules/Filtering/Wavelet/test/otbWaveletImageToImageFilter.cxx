/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
/* 
This test code will run a WaveletImageFilter on given input image. Output 
from this filter is then fed to WaveletInverseImageFilter which does 
the inverse transform. Hence output of inverse transform passed to
ImageFileWriter will result in the input image. 

Wavelet operator used is HAAR (otb::Wavelet::HAAR).
This filter works only in single-threaded setup 
*/

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbWaveletImageFilter.h"
#include "otbWaveletInverseImageFilter.h"

int otbWaveletImageToImageFilter(int argc, char * argv[])
{

  const char *       inputFileName = argv[1];
  const char *       outputFileName = argv[argc-1];

  /* Wavelet operator */
  const otb::Wavelet::Wavelet OperatorType = otb::Wavelet::HAAR;
  
  const int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;



  typedef otb::WaveletImageFilter<
	  ImageType,
	  ImageType,
	  OperatorType> FwdFilterType;

  typedef otb::WaveletInverseImageFilter<
	  ImageType,
	  ImageType,
	  OperatorType> InvFilterType;

  typedef otb::ImageFileWriter<ImageType> WriterType;
  
  //  itk::MultiThreader::SetGlobalMaximumNumberOfThreads( 1 );

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  /* Forward Transformation */  
  FwdFilterType::Pointer fwdFilter = FwdFilterType::New();
 
  fwdFilter->SetInput(reader->GetOutput());
  fwdFilter->Update();

  /* Inverse Transformation */  
  
  InvFilterType::Pointer invFilter = InvFilterType::New();
  
  invFilter->SetInput(fwdFilter->GetOutput());
  invFilter->Update();

  /* Writing output */
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(invFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
