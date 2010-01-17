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

#include "otbPointSetFileReader.h"
#include "itkPointSet.h"
#include "itkNumericTraits.h"
#include <fstream>

int otbPointSetFileReader(int argc, char * argv[])
{
  typedef itk::PointSet <double, 2> PointSetType;
  typedef otb::PointSetFileReader<PointSetType> PointSetFileReaderType;
  PointSetFileReaderType::Pointer reader = PointSetFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->Update();

  PointSetType::Pointer data = reader->GetOutput();


  std::ofstream fout (argv[2]);
  unsigned long nPoints = data->GetNumberOfPoints();
  fout << std::setprecision(15) << "Number of points: " << nPoints << std::endl;

  for (unsigned long i=0; i < nPoints; ++i)
  {
    PointSetType::PointType point;
    data->GetPoint(i,&point);
    fout << point << " : ";
    PointSetType::PixelType value(itk::NumericTraits<PointSetType::PixelType>::Zero);
    data->GetPointData(i,&value);
    fout << value << std::endl;
  }
  fout << std::endl;
  fout.close();
  return EXIT_SUCCESS;
}

