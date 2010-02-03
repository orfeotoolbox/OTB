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

//#include "otbVectorDataFileReader.h"
//#include "otbImageFileWriter.h"
#include "otbVectorData.h"
//#include "otbVectorDataProjectionFilter.h"
//#include "otbVectorDataExtractROI.h"
#include <fstream>
#include <iostream>

//#include "itkRGBAPixel.h"
//#include "otbImage.h"
#include "otbGISTableToVectorDataFilter.h"
//#include "itkAttributeLabelObject.h"
#include "otbPostGISConnectionImplementation.h"

int otbGISTableToVectorDataFilterNew(int argc, char * argv[])
{
  
  const int dim = 2;
  typedef unsigned char PType;

  typedef otb::VectorData<> VectorDataType;
  
  typedef otb::PostGISConnectionImplementation GISConnectionType;

  typedef otb::GISTable<GISConnectionType, double, 2> GISTableType;

  //Instantiation
  GISTableType::Pointer data = GISTableType::New();

  typedef otb::GISTableToVectorDataFilter< GISTableType , VectorDataType > GISTableToVectorDataFilter;

  GISTableToVectorDataFilter::Pointer MyFilter = GISTableToVectorDataFilter::New();
  
  //data->itkGetObjectMacro ( )
  
  return EXIT_SUCCESS;
}
