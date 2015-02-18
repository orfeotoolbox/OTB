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

#include "otbVectorDataToRightAngleVectorDataFilter.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbImage.h"
#include "otbImageFileWriter.h"


int otbVectorDataToRightAngleVectorDataFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  /** Typedefs */
  typedef otb::VectorData< >                    VectorDataType;
  typedef otb::VectorDataToRightAngleVectorDataFilter<VectorDataType>
                                                RightAngleFilterType;

  RightAngleFilterType::Pointer filter  = RightAngleFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}


int otbVectorDataToRightAngleVectorDataFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname  = argv[1];
  const char * outfname = argv[2];

  /** Typedefs */
  typedef otb::VectorData< >                          VectorDataType;
  typedef otb::VectorDataToRightAngleVectorDataFilter<VectorDataType>
                                                      RightAngleFilterType;

  typedef otb::VectorDataFileReader<VectorDataType>   vDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>   vDataWriterType;

  // Create instances
  vDataReaderType::Pointer      vDataReader       = vDataReaderType::New();
  RightAngleFilterType::Pointer rightAngleFilter  = RightAngleFilterType::New();
  vDataWriterType::Pointer      vDataWriter       = vDataWriterType::New();

  // Begin the process
  vDataReader->SetFileName(infname);
  rightAngleFilter->SetInput(vDataReader->GetOutput());
  rightAngleFilter->SetDistanceThreshold(10.0);
  rightAngleFilter->SetAngleThreshold(3.141592/18.0);
  vDataWriter->SetFileName(outfname);
  vDataWriter->SetInput(rightAngleFilter->GetOutput());
  vDataWriter->Update();

  return EXIT_SUCCESS;
}
