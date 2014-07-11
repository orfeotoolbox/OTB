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
#include "otbLineSegmentDetector.h"

#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbImageFileWriter.h"

int otbLineSegmentDetectorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  /** Typedefs */
  typedef otb::Image<InputPixelType,  Dimension>                   InputImageType;
  typedef otb::LineSegmentDetector<InputImageType, InputPixelType> lsdFilterType;

  lsdFilterType::Pointer filter = lsdFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}

int otbLineSegmentDetector(int argc, char * argv[])
{
  const char * infname  = argv[1];
  const char * outfname  = argv[2];

  /** Typedefs */
  typedef unsigned char                                               PixelType;
  typedef otb::Image<PixelType>                                       ImageType;
  typedef otb::ImageFileReader<ImageType>                             ReaderType;
  typedef otb::LineSegmentDetector<ImageType, double>                 LSDFilterType;
  typedef LSDFilterType::VectorDataType                               VectorDataType;
  typedef otb::VectorDataFileWriter<VectorDataType>                   VectorDataWriterType;
  typedef otb::ImageFileWriter<ImageType>                             WriterType;

  /** Instantiation of smart pointer*/
  ReaderType::Pointer             reader         = ReaderType::New();
  LSDFilterType::Pointer          lsdFilter      = LSDFilterType::New();

  //Reade the input image
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  //LSD Detection
  lsdFilter->SetInput(reader->GetOutput());

    VectorDataWriterType::Pointer vdWriter = VectorDataWriterType::New();
  vdWriter->SetFileName(outfname);
  vdWriter->SetInput(lsdFilter->GetOutput());
  vdWriter->Update();

  if (argc > 3)
    {
    //Write the status map
    const char * regfname  = argv[3];
    WriterType::Pointer writer0 = WriterType::New();
    writer0->SetFileName(regfname);
    writer0->SetInput(lsdFilter->GetMap());
    writer0->Update();

    //Write the gradient modulus image
    typedef otb::ImageFileWriter< otb::Image<double> >  WriterDoubleType;
    const char * gradModfname  = argv[4];
    WriterDoubleType::Pointer writer1 = WriterDoubleType::New();
    writer1->SetFileName(gradModfname);
    writer1->SetInput(lsdFilter->GetGradMod());
    writer1->Update();

    //Write the gradient orientation image
    const char * gradOrifname  = argv[5];
    WriterDoubleType::Pointer writer2 = WriterDoubleType::New();
    writer1->SetFileName(gradOrifname);
    writer1->SetInput(lsdFilter->GetGradOri());
    writer1->Update();
    }

  return EXIT_SUCCESS;
}

int otbLineSegmentDetector_8b_16b_compare(int itkNotUsed(argc), char * argv[])
{
  const char * infname8    = argv[1];
  const char * infname16   = argv[2];
  const char * outfname8   = argv[3];
  const char * outfname16  = argv[4];

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
