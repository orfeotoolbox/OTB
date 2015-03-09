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
#include "otbImageFileWriter.h"

#include "itkCastImageFilter.h"

/**
 * This test reproduces a problem encountered with CentOS5.5 using 3.8.2 libTIFF version
 * Issue number 0000437
 */
int main(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cout << argv[0] << " <input image> <output image>" << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Image<unsigned short, 2>                   ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::ImageFileWriter<ImageType>    WriterType;
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
  writer->SetTileDimensionTiledStreaming(256);
  writer->Update();

  return EXIT_SUCCESS;
}
