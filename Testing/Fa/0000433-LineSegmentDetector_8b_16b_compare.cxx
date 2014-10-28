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

#include "otbLineSegmentDetector.h"

#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbImageFileWriter.h"

// Code showing difference between LSD with 8 and 16 bits images.
// http://bugs.orfeo-toolbox.org/view.php?id=433

int main(int argc, char *argv[])
{
  if (argc != 6)
    {
    std::cout << "Usage : <inputImage_8b> <inputImage_16b> <outputImage_8b> <outputImage_16b>" << std::endl;

    return EXIT_FAILURE;
    }

  const char * infname8    = argv[2];
  const char * infname16   = argv[3];
  const char * outfname8   = argv[4];
  const char * outfname16  = argv[5];

  /** Typedefs */
  typedef unsigned char                                               PixelType8;
  typedef otb::Image<PixelType8>                                      ImageType8;
  typedef otb::ImageFileReader<ImageType8>                            ReaderType8;
  typedef otb::LineSegmentDetector<ImageType8, double>                LSDFilterType8;
  typedef unsigned short                                              PixelType16;
  typedef otb::Image<PixelType16>                                     ImageType16;
  typedef otb::ImageFileReader<ImageType16>                           ReaderType16;
  typedef otb::LineSegmentDetector<ImageType16, double>               LSDFilterType16;
  typedef LSDFilterType8::VectorDataType                              VectorDataType;
  typedef otb::VectorDataFileWriter<VectorDataType>                   VectorDataWriterType;

  /** 8bits */
  //Instantiation of smart pointer
  ReaderType8::Pointer             reader8         = ReaderType8::New();
  LSDFilterType8::Pointer          lsdFilter8      = LSDFilterType8::New();
  //Reade the input image
  reader8->SetFileName(infname8);
  reader8->GenerateOutputInformation();
  //LSD Detection
  lsdFilter8->SetInput(reader8->GetOutput());
  VectorDataWriterType::Pointer vdWriter8 = VectorDataWriterType::New();
  vdWriter8->SetFileName(outfname8);
  vdWriter8->SetInput(lsdFilter8->GetOutput());
  vdWriter8->Update();

  /** 16bits */
  //Instantiation of smart pointer
  ReaderType16::Pointer             reader16         = ReaderType16::New();
  LSDFilterType16::Pointer          lsdFilter16      = LSDFilterType16::New();
  //Reade the input image
  reader16->SetFileName(infname16);
  reader16->GenerateOutputInformation();
  //LSD Detection
  lsdFilter16->SetInput(reader16->GetOutput());
  VectorDataWriterType::Pointer vdWriter16 = VectorDataWriterType::New();
  vdWriter16->SetFileName(outfname16);
  vdWriter16->SetInput(lsdFilter16->GetOutput());
  vdWriter16->Update();

  return EXIT_SUCCESS;
}
