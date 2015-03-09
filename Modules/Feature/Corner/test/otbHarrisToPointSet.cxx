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
#include "otbImageFileWriter.h"
#include "otbHarrisImageToPointSetFilter.h"
#include <fstream>

int otbHarrisToPointSet(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  double SigmaD((double) ::atof(argv[3]));
  double SigmaI((double) ::atof(argv[4]));
  double AlphaValue((double) ::atof(argv[5]));

  typedef unsigned char PixelType;
  const unsigned int Dimension = 2;

  PixelType LowerThreshold((PixelType) ::atoi(argv[6]));
  PixelType UpperThreshold((PixelType) ::atoi(argv[7]));

  typedef otb::Image<PixelType,  Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::HarrisImageToPointSetFilter<ImageType> FunctionType;
  typedef FunctionType::OutputPointSetType            OutputPointSetType;
  typedef OutputPointSetType::PointType               OutputPointType;

  ReaderType::Pointer   reader    = ReaderType::New();
  FunctionType::Pointer harris    = FunctionType::New();
  OutputPointType       CoordPoint;

  //Initialization
  CoordPoint.Fill(0);

  reader->SetFileName(inputFilename);

  harris->SetInput(0, reader->GetOutput());
  harris->SetSigmaD(SigmaD);
  harris->SetSigmaI(SigmaI);
  harris->SetAlpha(AlphaValue);
  harris->SetLowerThreshold(LowerThreshold);
  harris->SetUpperThreshold(UpperThreshold);
  OutputPointSetType * pointList = harris->GetOutput();

  harris->Update();

  std::cout << "Threshold Lower/Upper : " << harris->GetLowerThreshold() << "/" << harris->GetUpperThreshold() <<
  std::endl;
  std::ofstream file;
  file.open(outputFilename);

  unsigned long NbPoints  = pointList->GetNumberOfPoints();
  file << "NbPoints : " << NbPoints << std::endl;

  for (unsigned long i = 0; i < NbPoints; ++i)
    {
    pointList->GetPoint(i, &CoordPoint);
    file << i + 1 << " / " << NbPoints << " : ";
    file << CoordPoint[0] << " , " << CoordPoint[1] << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
