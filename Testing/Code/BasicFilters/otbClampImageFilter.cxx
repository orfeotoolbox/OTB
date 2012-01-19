/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbClampImageFilter.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

/** Pixel typedefs */
typedef double                                      InputPixelType;
typedef unsigned int                                OutputPixelType;

/** Image typedefs */
const unsigned int                            Dimension = 2;
typedef otb::Image<InputPixelType, Dimension>  InputImageType;
typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

typedef otb::ClampImageFilter<InputImageType, OutputImageType>    FilterType;

int otbClampImageFilterNew(int argc, char* argv[])
{
  /** instantiating the filter */
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}

int otbClampImageFilterTest(int argc, char* argv[])
{
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  /** instantiating the filter */
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  filter->SetInput( reader->GetOutput() );
  filter->SetLower( 100 );
  filter->SetUpper( 400 );

  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}
