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

int otbLineSegmentDetectorNew(int argc, char * argv[])
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
