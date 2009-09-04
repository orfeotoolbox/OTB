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
#include "otbStatisticsAttributesLabelMapFilter.h"
#include "itkLabelMap.h"
#include "otbImage.h"
#include "otbAttributesMapLabelObject.h"

int otbStatisticsAttributesLabelMapFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned short                         LabelType;
  typedef double                         PixelType;
  
  typedef otb::AttributesMapLabelObject<LabelType,Dimension,double>      LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                 LabelMapType;
  typedef otb::Image<PixelType,Dimension>                                ImageType;
  typedef otb::StatisticsAttributesLabelMapFilter<LabelMapType,ImageType> StatisticsLabelMapFilterType;
  
  // Instantiation
  StatisticsLabelMapFilterType::Pointer StatisticsAttributesLabelMapFilter = StatisticsLabelMapFilterType::New();

  return EXIT_SUCCESS;
}
