/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif


#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"

#include "otbPrintableImageFilter.h"

int main( int argc, char* argv[] )
{
  
  if( argc < 4 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputPanchromatiqueImage inputMultiSpectralImage outputImage outputImagePrinted msPrinted msiPrinted panchroPrinted " << std::endl;
    return 1;
  }
  
  typedef otb::Image<double, 2>     ImageType;
  typedef otb::VectorImage<double, 2>     VectorImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileReader<VectorImageType>  ReaderVectorType;
  
  typedef otb::VectorImage<unsigned int, 2>     VectorIntImageType;


					
  ReaderVectorType::Pointer     	readerXS=ReaderVectorType::New();
  ReaderType::Pointer     	readerPAN=ReaderType::New();

  
  readerPAN->SetFileName(argv[1]);
  readerXS->SetFileName(argv[2]);
  
  typedef otb::SimpleRcsPanSharpeningFusionImageFilter
      <ImageType,VectorImageType,VectorIntImageType> FusionFilterType;
  FusionFilterType::Pointer fusion = FusionFilterType::New();
  fusion->SetPanInput(readerPAN->GetOutput());
  fusion->SetXsInput(readerXS->GetOutput());
      
      
  typedef otb::StreamingImageFileWriter<VectorIntImageType>  WriterType;
  WriterType::Pointer	    	writer=WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(fusion->GetOutput());
  writer->Update();
  
  typedef otb::PrintableImageFilter<VectorIntImageType> PrintableImageType;
  PrintableImageType::Pointer printable = PrintableImageType::New();
  printable->SetInput(fusion->GetOutput());
//   printable->SetInput(readerXS->GetOutput());
//   printable->SetChannel(1);
  printable->SetChannel(3);
  printable->SetChannel(2);
  printable->SetChannel(1); 
  
  typedef otb::VectorImage<unsigned char, 2>     VectorCharImageType;
  typedef otb::StreamingImageFileWriter<VectorCharImageType>  PNGWriterType;
  PNGWriterType::Pointer pngwriter = PNGWriterType::New();
  pngwriter->SetFileName(argv[4]);
  pngwriter->SetInput(printable->GetOutput());
  pngwriter->Update();
  
  return 0;
  
}


