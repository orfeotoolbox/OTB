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
#include "otbSarBrightnessFunction.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include <iostream>

int otbSarBrightnessFunctionWithoutNoise(int itkNotUsed(argc), char* argv[])
{

  const char * infname  = argv[1];
  const char * outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef otb::Image<PixelType, Dimension>                InputImageType;
  typedef InputImageType::IndexType                       IndexType;
  typedef otb::ImageFileReader<InputImageType>            ReaderType;
  typedef otb::SarBrightnessFunction<InputImageType>      FunctionType;

  /**Instantiation ofa Smart Pointer*/
  FunctionType::Pointer filter = FunctionType::New();
  ReaderType::Pointer   reader = ReaderType::New();

  std::ofstream outfile(outfname);

  /** Input Image*/
  reader->SetFileName(infname);
  reader->Update();

  /** Computing the density around a pixel  */
  filter->SetInputImage(reader->GetOutput());
  filter->SetEnableNoise(false);

  /** Test on some indexes and some physical coordinates*/
  InputImageType::SizeType size = reader->GetOutput()->GetRequestedRegion().GetSize();
  FunctionType::PointType  pDst;
  IndexType                index;

  index[0] = 0;
  index[1] = 0;
  outfile << "Sar Brightness value computed for the point : " << index << " is " << filter->EvaluateAtIndex(index) << std::endl;

  index[0] = static_cast<unsigned int>(size[0] / 2.);
  index[1] = static_cast<unsigned int>(size[1] / 4.);
  outfile << "Sar Brightness value computed for the point : " << index << " is " << filter->EvaluateAtIndex(index) << std::endl;

  pDst[0] = static_cast<unsigned int>(size[0] / 4.);
  pDst[1] = static_cast<unsigned int>(size[1] / 8.);
  outfile << "Sar Brightness value computed for the point : " <<  pDst << " is " << filter->Evaluate(pDst) << std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
