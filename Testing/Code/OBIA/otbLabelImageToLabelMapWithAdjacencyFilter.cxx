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

#include "otbLabelImageToLabelMapWithAdjacencyFilter.h"
#include "otbLabelMapWithAdjacency.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkLabelObject.h"
#include "itkNumericTraits.h"

#include <fstream>

typedef unsigned short                              LabelType;
typedef otb::Image<LabelType>                       LabelImageType;
typedef otb::ImageFileReader<LabelImageType>        LabelReaderType;
typedef itk::LabelObject<LabelType, 2>               LabelObjectType;
typedef otb::LabelMapWithAdjacency<LabelObjectType> LabelMapType;
typedef otb::LabelImageToLabelMapWithAdjacencyFilter
< LabelImageType, LabelMapType>                     FilterType;


int otbLabelImageToLabelMapWithAdjacencyFilterNew(int argc, char * argv[])
{
  // Instantiation
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}

int otbLabelImageToLabelMapWithAdjacencyFilter(int argc, char * argv[])
{
  LabelReaderType::Pointer reader = LabelReaderType::New();
  reader->SetFileName(argv[1]);
  
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());
  filter->Update();

  std::ofstream ofs(argv[2]);

  // Retrieve the adjacency map
  LabelMapType::AdjacencyMapType adjMap = filter->GetOutput()->GetAdjacencyMap();

  for(LabelMapType::AdjacencyMapType::const_iterator it = adjMap.begin(); it!=adjMap.end(); ++it)
    {
    ofs<<"Label:\t"<<it->first<<" adjacent with labels";

    LabelMapType::AdjacentLabelsContainerType::const_iterator lit = it->second.begin();

    for(; lit!=it->second.end(); ++lit)
      {

      ofs<<"\t"<<(*lit);
      }
    ofs<<std::endl;
    }

  ofs.close();

  return EXIT_SUCCESS;
}
