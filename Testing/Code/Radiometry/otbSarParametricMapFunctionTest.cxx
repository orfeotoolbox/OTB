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

#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbSarParametricMapFunction.h"
#include "itkPointSet.h"

int otbSarParametricMapFunctionTest(int itkNotUsed(argc), char* argv[])
{

  const char * infname  = argv[1];
  const char * outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef otb::Image<PixelType, Dimension>                InputImageType;
  typedef InputImageType::IndexType                       IndexType;
  typedef InputImageType::PixelType                       InputPixelType;
  typedef otb::ImageFileReader<InputImageType>            ReaderType;
  typedef otb::SarParametricMapFunction<InputImageType>   FunctionType;
  typedef FunctionType::PointSetType                      PointSetType;

  /**Instantiation ofa Smart Pointer*/
  FunctionType::Pointer filter = FunctionType::New();
  ReaderType::Pointer   reader = ReaderType::New();
  PointSetType::Pointer points = PointSetType::New();
  PointSetType::Pointer coefValue = PointSetType::New();

  std::ofstream outfile(outfname);

  /** Input Image*/
  reader->SetFileName(infname);
  reader->Update();
  filter->SetInputImage(reader->GetOutput());

  /** Test on some indexes and some physical coordinates*/
  InputImageType::SizeType size = reader->GetOutput()->GetRequestedRegion().GetSize();
  FunctionType::PointType  p0;
  IndexType                index;
  InputPixelType           value;
  FunctionType::PointType  pDst;


  p0[0] = static_cast<unsigned int>(size[0] / 2.);
  p0[1] = static_cast<unsigned int>(size[1] / 4.);

  value = 10.0;

  points->SetPoint(0, p0);
  points->SetPointData(0, value);
  filter->SetPointSet(points);

  filter->EvaluateParametricCoefficient();
  std::cout << filter << std::endl;

  p0[0] = static_cast<unsigned int>(size[0] / 4.);
  p0[1] = static_cast<unsigned int>(size[1] / 8.);

  value = 20.0;

  points->SetPoint(1, p0);
  points->SetPointData(1, value);
  filter->SetPointSet(points);

  IndexType       polynomalSize;
  polynomalSize[0] = 1;
  polynomalSize[1] = 0;
  filter->SetPolynomalSize(polynomalSize);
  filter->EvaluateParametricCoefficient();
  std::cout << filter << std::endl;

  index[0] = 0;
  index[1] = 0;
  outfile << "Sar Radiometric Calibration computed for the point : DN[" << index <<"] = " << reader->GetOutput()->GetPixel(index) << " is " << filter->EvaluateAtIndex(index) << std::endl;

  index[0] = static_cast<unsigned int>(size[0] / 2.);
  index[1] = static_cast<unsigned int>(size[1] / 4.);
  outfile << "Sar Radiometric Calibration computed for the point : DN[" << index <<"] = " << reader->GetOutput()->GetPixel(index) << " is " << filter->EvaluateAtIndex(index) << std::endl;

  index[0] = static_cast<unsigned int>(size[0] / 2.);
  index[1] = static_cast<unsigned int>(size[1] / 8.);
  outfile << "Sar Radiometric Calibration computed for the point : DN[" << index <<"] = " << reader->GetOutput()->GetPixel(index) << " is " << filter->EvaluateAtIndex(index) << std::endl;

  outfile.close();


  return EXIT_SUCCESS;
}
