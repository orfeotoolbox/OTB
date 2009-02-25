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
#include "otbVectorData.h"
#include "otbVectorDataExtractROI.h"

#include "otbVectorDataFileReader.h"

int otbVectorDataExtractROINew( int argc, char * argv[] )
{
  typedef otb::VectorData<>                                  VectorDataType;
  typedef otb::VectorDataExtractROI< VectorDataType >        FilterType;
  typedef otb::VectorDataFileReader<VectorDataType>          VectorDataFileReaderType;

  
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}


