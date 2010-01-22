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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {srs.las}
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
// This example describes how to read a lidar data file and to display the basic
// information about it. Here, OTB make use of libLAS.
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbPointSetFileReader.h"
#include "itkPointSet.h"
#include <fstream>
int main(int argc, char * argv[])
{
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We need now to declare the data types that we will be using and instanciate the
// reader (which is a \doxygen{otb}{PointSetFileReader}).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::PointSet <double, 2> PointSetType;
  typedef otb::PointSetFileReader<PointSetType> PointSetFileReaderType;
  PointSetFileReaderType::Pointer reader = PointSetFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can already display some interesting information such as the
// data extension and the number of points:
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::cout << "*** Data area *** " << std::endl;
  std::cout << std::setprecision(15);
  std::cout << " - Easting min:  " << reader->GetMinX() << std::endl;
  std::cout << " - Easting max:  " << reader->GetMaxX() << std::endl;
  std::cout << " - Northing min: " << reader->GetMinY() << std::endl;
  std::cout << " - Northing max: " << reader->GetMaxY() << std::endl;
  std::cout << "Data points: " << reader->GetNumberOfPoints() << std::endl;
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
//
// We can also loop on the point to see each point with its coordinates and
// value. Be careful here, data set can have hundred of thousands of points:
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  PointSetType::Pointer data = reader->GetOutput();

  unsigned long nPoints = data->GetNumberOfPoints();

  for (unsigned long i=0; i < nPoints; ++i)
  {
    PointSetType::PointType point;
    data->GetPoint(i,&point);
    std::cout << point << " : ";
    PointSetType::PixelType value = itk::NumericTraits<PointSetType::PixelType>::Zero;
    data->GetPointData(i,&value);
    std::cout << value << std::endl;
  }

  return EXIT_SUCCESS;
}
// Software Guide : EndCodeSnippet


