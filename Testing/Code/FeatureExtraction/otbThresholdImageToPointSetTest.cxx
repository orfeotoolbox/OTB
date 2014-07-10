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
#include "otbImageFileReader.h"
#include "otbThresholdImageToPointSetFilter.h"
#include "itkPointSet.h"

int otbThresholdImageToPointSetTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension>                            InputImageType;
  typedef otb::ImageFileReader<InputImageType>                              ReaderType;
  typedef itk::PointSet<InputPixelType, Dimension>                          PointSetType;
  typedef otb::ThresholdImageToPointSetFilter<InputImageType, PointSetType> FunctionType;
  typedef PointSetType::PointType                                           PointType;
  typedef FunctionType::InputPixelType                                      InputPixelType;
  typedef FunctionType::OutputPointSetType                                  OutputPointSetType;

  InputPixelType LowerThreshold((InputPixelType) ::atoi(argv[3]));
  InputPixelType UpperThreshold((InputPixelType) ::atoi(argv[4]));

  ReaderType::Pointer   reader         = ReaderType::New();
  FunctionType::Pointer function       = FunctionType::New();

  reader->SetFileName(inputFilename);

  function->SetInput(0, reader->GetOutput());
  function->SetLowerThreshold(LowerThreshold);
  function->SetUpperThreshold(UpperThreshold);
  function->Update();
  OutputPointSetType * pointList = function->GetOutput();

  std::ofstream file;
  file.open(outputFilename);

  unsigned long NbPoints  = pointList->GetNumberOfPoints();
  file << "NbPoints : " << NbPoints << std::endl;

  PointType CoordPoint;

  //Initialization
  CoordPoint.Fill(0);

  for (unsigned long i = 0; i < NbPoints; ++i)
    {
    pointList->GetPoint(i, &CoordPoint);
    file << i + 1 << "/" << NbPoints << " : " << CoordPoint[0] << " , " << CoordPoint[1] << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
