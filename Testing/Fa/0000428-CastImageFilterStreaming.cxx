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
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

#include "itkCastImageFilter.h"

/**
 * This test reproduces a problem encountered with CastImageFilter and a streaming
 * writer. When the image to write is already loaded in memory and InPlaceOn() is
 * called, only the first stripe is repeated across the output image.
 * Issue number 0000428
 */
int main(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cout << argv[0] << " <input image> <output image>" << std::endl;
    return EXIT_FAILURE;
    }
  
  typedef otb::Image<float, 2>                   ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType>    WriterType;
  typedef itk::CastImageFilter<ImageType, ImageType>   CastFilterType;
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  
  CastFilterType::Pointer caster = CastFilterType::New();
  caster->SetInput( reader->GetOutput() );
  caster->InPlaceOn();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(caster->GetOutput());
  writer->SetFileName(argv[2]);
  //writer->SetAutomaticTiledStreaming(1024);
  writer->SetNumberOfDivisionsStrippedStreaming(10);
  writer->Update();
  
  return EXIT_SUCCESS;
}
