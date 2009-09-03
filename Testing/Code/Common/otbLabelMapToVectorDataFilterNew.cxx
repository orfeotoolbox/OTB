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

#include "otbLabelMapToVectorDataFilter.h"
#include "otbVectorData.h"
#include "itkAttributeLabelObject.h"

int otbLabelMapToVectorDataFilterNew(int argc, char * argv[])
{
  const int dim = 2;
  typedef unsigned char PType;

  typedef otb::VectorData<> VectorDataType;
  
  typedef itk::AttributeLabelObject< unsigned long, dim, double > LabelObjectType;

  typedef itk::LabelMap< LabelObjectType > LabelMapType;

  typedef otb::LabelMapToVectorDataFilter< LabelMapType , VectorDataType > LabelMapToVectorDataFilterType;

  LabelMapToVectorDataFilterType::Pointer MyFilter = LabelMapToVectorDataFilterType::New();
  
  return EXIT_SUCCESS;
}


