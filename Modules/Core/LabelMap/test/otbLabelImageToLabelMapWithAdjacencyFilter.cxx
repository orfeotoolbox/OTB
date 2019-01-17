/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbLabelImageToLabelMapWithAdjacencyFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkNumericTraits.h"

#include <fstream>

typedef unsigned short                              LabelType;
typedef otb::Image<LabelType>                       LabelImageType;
typedef otb::ImageFileReader<LabelImageType>        LabelReaderType;
typedef itk::LabelObject<LabelType, 2>              LabelObjectType;
typedef otb::LabelMapWithAdjacency<LabelObjectType> LabelMapType;
typedef LabelMapType::ConstIterator        ConstIteratorType;
typedef LabelObjectType::ConstLineIterator ConstLineIteratorType;
typedef otb::LabelImageToLabelMapWithAdjacencyFilter
< LabelImageType, LabelMapType>                     FilterType;



int otbLabelImageToLabelMapWithAdjacencyFilter(int itkNotUsed(argc), char * argv[])
{
  LabelReaderType::Pointer reader = LabelReaderType::New();
  reader->SetFileName(argv[1]);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());
  filter->Update();

  std::ofstream ofs(argv[2]);

  // Retrieve the label map
  ConstIteratorType  lIt = ConstIteratorType( filter->GetOutput() );

  ofs<<"Label map: "<<std::endl;

  while ( !lIt.IsAtEnd() )
    {
    // Retrieve the label object
    const LabelObjectType * lo = lIt.GetLabelObject();

    // Retrieve the line container
    ConstLineIteratorType lineIt = ConstLineIteratorType( lo );

    ofs<<"Label: "<<lo->GetLabel()<<", lines: ";

    while ( !lineIt.IsAtEnd() )
      {
      ofs<<" { "<<lineIt.GetLine().GetIndex()<<", "<<lineIt.GetLine().GetLength()<<"}";
      ++lineIt;
      }
    ofs<<std::endl;
    ++lIt;
    }

  ofs<<"Adjacency map: "<<std::endl;

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
