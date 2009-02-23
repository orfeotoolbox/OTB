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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUT: {ADS40RoiSmall.png}
//    OUTPUT: {TextureOutput.tif}, {pretty_TextureOutput.png}
//    2 1 1
//  Software Guide : EndCommandLineArgs

#include "itkExceptionObject.h"
#include "otbImage.h"
#include "itkVector.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"



#include "otbTextureFunctors.h"
#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "otbTextureImageFunction.h"


int main(int argc, char * argv[])
{
  // Parse command line parameters
  
  const char* infname   = argv[1];
  const char* outfname  = argv[2];
  const char* outprettyfname  = argv[3];

  const unsigned int radius  =  static_cast<unsigned int>(atoi(argv[4]));
  const unsigned int xOffset =  static_cast<unsigned int>(atoi(argv[5]));
  const unsigned int yOffset =  static_cast<unsigned int>(atoi(argv[6]));

 
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> ImageType;
  typedef ImageType::PixelType                       PixelType;
  typedef itk::ConstNeighborhoodIterator<ImageType>  IteratorType;
  typedef itk::Vector< PixelType > VectorType;


  typedef otb::Functor::ContrastTextureFunctor<IteratorType, VectorType>
                                                             FunctorType;

  typedef otb::TextureImageFunction<ImageType, FunctorType> FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<ImageType,
                                          ImageType, FunctionType> FilterType;
  typedef ImageType::OffsetType OffsetType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  

  // Instantiating object
  FilterType::Pointer textureFilter = FilterType::New();
  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  reader->SetFileName(infname);
  writer->SetFileName(outfname);

  textureFilter->SetInput(reader->GetOutput());
  ImageType::SizeType tRadius;
  tRadius[0] = radius;
  tRadius[1] = radius;
  textureFilter->SetRadius(tRadius);
  OffsetType offset;
  offset[0] =  xOffset;
  offset[1] =  yOffset;

  textureFilter->SetOffset(offset);
  writer->SetInput(textureFilter->GetOutput());

  writer->Update();

 // Pretty image creation for printing

  typedef otb::Image<unsigned char, Dimension>                                           OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>                                    WriterPrettyOutputType;
  typedef itk::RescaleIntensityImageFilter< ImageType, OutputPrettyImageType>      RescalerOutputType;

  RescalerOutputType::Pointer     outputRescaler     = RescalerOutputType::New();
  WriterPrettyOutputType::Pointer prettyOutputWriter = WriterPrettyOutputType::New();
  outputRescaler->SetInput( textureFilter->GetOutput() );
  outputRescaler->SetOutputMinimum(0);
  outputRescaler->SetOutputMaximum(255);
  prettyOutputWriter->SetFileName( argv[4] );
  prettyOutputWriter->SetInput( outputRescaler->GetOutput() );

  prettyOutputWriter->Update();


  return EXIT_SUCCESS;
}
