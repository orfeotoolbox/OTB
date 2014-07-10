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
#include "otbStreamingMatrixTransposeMatrixImageFilter.h"

int otbMatrixTransposeMatrixImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname1 = argv[1];
  const char * infname2 = argv[2];
  const char * outfname = argv[3];

  const unsigned int Dimension = 2;
  //typedef double InputPixelType;
  //typedef double OutputPixelType;
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::VectorImage<InputPixelType, Dimension> InputImage1Type;
  typedef otb::VectorImage<InputPixelType, Dimension> InputImage2Type;
  typedef otb::StreamingMatrixTransposeMatrixImageFilter<InputImage1Type,
      InputImage2Type> MatrixTransposeMatrixImageFilterType;
  typedef otb::ImageFileReader<InputImage1Type> ReaderType1;
  typedef otb::ImageFileReader<InputImage2Type> ReaderType2;

  // Instantiation
  MatrixTransposeMatrixImageFilterType::Pointer filter = MatrixTransposeMatrixImageFilterType::New();
  ReaderType1::Pointer                          reader1 = ReaderType1::New();
  ReaderType2::Pointer                          reader2 = ReaderType2::New();

  reader1->SetFileName(infname1);
  reader2->SetFileName(infname2);

  filter->GetStreamer()->SetNumberOfLinesStrippedStreaming( 10 );
  filter->SetFirstInput(reader1->GetOutput());
  filter->SetSecondInput(reader2->GetOutput());
  filter->SetUsePadFirstInput(true);
  filter->SetUsePadSecondInput(true);

  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file << "transpose : " << filter->GetResult() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
