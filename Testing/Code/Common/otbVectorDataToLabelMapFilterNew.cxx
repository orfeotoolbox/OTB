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
#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbVectorDataToLabelMapFilter.h"
#include "otbAttributesMapLabelObject.h"

int otbVectorDataToLabelMapFilterNew(int argc, char * argv[])
{
  typedef unsigned short                         LabelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorData<double,Dimension> VectorDataType;
  typedef otb::AttributesMapLabelObject<LabelType,Dimension,double> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType> OutputLabelMapType;
  
  typedef otb::VectorDataToLabelMapFilter< VectorDataType , OutputLabelMapType > VectorDataToLabelMapFilterType;

  

  //Instantiation
  VectorDataToLabelMapFilterType::Pointer MyFilter = VectorDataToLabelMapFilterType::New();

  return EXIT_SUCCESS;
}
