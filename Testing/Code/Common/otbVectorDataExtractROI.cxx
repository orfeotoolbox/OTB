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

int otbVectorDataExtractROI( int argc, char * argv[] )
{
  const char * infname = argv[1];

  const unsigned int  startX = atoi(argv[3]);
  const unsigned int  startY = atoi(argv[4]);
  const unsigned int  sizeX = atoi(argv[5]);
  const unsigned int  sizeY = atoi(argv[6]);
  
  typedef otb::VectorData<>                                  VectorDataType;
  typedef otb::VectorDataExtractROI< VectorDataType >        FilterType;
  typedef otb::VectorDataFileReader<VectorDataType>          VectorDataFileReaderType;

  /** */
  FilterType::Pointer filter = FilterType::New();
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  
  /** */
  reader->SetFileName(infname);
  
  filter->SetInput(reader->GetOutput());
  filter->SetSizeX(sizeX);
  filter->SetSizeY(sizeY);
  filter->SetStartX(startX);
  filter->SetStartY(startY);

  filter->Update();
  
  return EXIT_SUCCESS;
}


