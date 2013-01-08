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

#include "otbVectorImageToColorAnaglyphVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbVectorImageToColorAnaglyphVectorImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  typedef otb::VectorImage<PixelType,
      Dimension>                                       VectorImageType;
  typedef otb::VectorImageToColorAnaglyphVectorImageFilter<VectorImageType, VectorImageType,
      VectorImageType> FilterType;
  typedef otb::ImageFileReader<VectorImageType>
  ReaderType;
  typedef otb::ImageFileWriter<VectorImageType>
  WriterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader1->SetFileName(argv[1]);
  reader2->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);

  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
