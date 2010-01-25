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

int otbPointSetFileReaderNew(int argc, char * argv[])
{
  typedef itk::PointSet <double, 2> PointSetType;
  typedef otb::PointSetFileReader<PointSetType> PointSetFileReaderType;
  PointSetFileReaderType::Pointer object = PointSetFileReaderType::New();


  return EXIT_SUCCESS;
}
