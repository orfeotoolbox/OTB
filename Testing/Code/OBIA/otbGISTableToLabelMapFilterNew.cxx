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

#include "otbGISTableToLabelMapFilter.h"
#include "otbPostGISConnectionImplementation.h"
#include "otbGISTable.h"
#include "itkAttributeLabelObject.h"

int otbGISTableToLabelMapFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const int Dimension = 2;
  typedef double PType;

  typedef otb::PostGISConnectionImplementation          BdConnection;
  typedef otb::GISTable<BdConnection, PType, Dimension> GISTableType;

  typedef itk::AttributeLabelObject<unsigned long, Dimension, double> LabelObjectType;

  typedef itk::LabelMap<LabelObjectType> LabelMapType;

  typedef otb::GISTableToLabelMapFilter<GISTableType, LabelMapType> GISTableToLabelMapFilterType;

  GISTableToLabelMapFilterType::Pointer filter = GISTableToLabelMapFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
