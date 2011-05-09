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


#include "otbHaralickTexturesImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

typedef unsigned short                                                InputPixelType;
const unsigned int Dimension =                                        2;

typedef otb::Image<InputPixelType,  Dimension>                        InputImageType;
typedef otb::ImageFileReader<InputImageType>                          ReaderType;

typedef otb::HaralickTexturesImageFunction<
        InputImageType, double>                                        HaralickTexturesImageFunctionType;
typedef HaralickTexturesImageFunctionType::PointType                  PointType;
typedef HaralickTexturesImageFunctionType::OutputType                 OutputType;

int otbHaralickTexturesImageFunctionNew(int argc, char * argv[])
{
  HaralickTexturesImageFunctionType::Pointer function = HaralickTexturesImageFunctionType::New();

  return EXIT_SUCCESS;
}

int otbHaralickTexturesImageFunction(int argc, char * argv[])
{
  // Read the input image
  ReaderType::Pointer   reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  std::ofstream outputStream(argv[2]);

  HaralickTexturesImageFunctionType::Pointer haralick = HaralickTexturesImageFunctionType::New();
  haralick->SetInputImage(reader->GetOutput());
  haralick->SetNeighborhoodRadius(10);

  HaralickTexturesImageFunctionType::OffsetType offset;
  offset.Fill(1);
  haralick->SetOffset(offset);

  PointType p;
  p[0] = atof(argv[3]);
  p[1] = atof(argv[4]);

  OutputType output = haralick->Evaluate(p);

  outputStream<<"Evaluate("<<p<<") = "<<output<<std::endl;

  outputStream.close();

  return EXIT_SUCCESS;
}
