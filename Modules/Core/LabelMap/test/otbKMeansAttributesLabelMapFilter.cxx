/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbImageFileReader.h"

#include <fstream>
#include <iostream>

#include "otbVectorImage.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbImageToLabelMapWithAttributesFilter.h"
#include "otbKMeansAttributesLabelMapFilter.h"

const unsigned int Dim  = 2;
typedef unsigned short LabelType;
typedef double         PixelType;

typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, Dim, double, LabelType>
                                                                                    LabelObjectType;
typedef itk::LabelMap<LabelObjectType>                                              LabelMapType;
typedef otb::VectorImage<PixelType, Dim>                                      VectorImageType;
typedef otb::Image<unsigned int, 2>                                                 LabeledImageType;

typedef LabelMapType::Iterator                                                      IteratorType;

typedef otb::ImageFileReader<VectorImageType>                                       ReaderType;
typedef otb::ImageFileReader<LabeledImageType>                                      LabeledReaderType;
typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>             LabelMapFilterType;
typedef otb::ShapeAttributesLabelMapFilter<LabelMapType>                            ShapeFilterType;
typedef otb::KMeansAttributesLabelMapFilter<LabelMapType>                           KMeansAttributesLabelMapFilterType;


int otbKMeansAttributesLabelMapFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * lfname  = argv[2];
  const char * outfname = argv[3];

  // SmartPointer instantiation
  ReaderType::Pointer         reader = ReaderType::New();
  LabeledReaderType::Pointer  labeledReader = LabeledReaderType::New();
  LabelMapFilterType::Pointer filter = LabelMapFilterType::New();
  ShapeFilterType::Pointer    shapeFilter = ShapeFilterType::New();
  KMeansAttributesLabelMapFilterType::Pointer kmeansLabelMapFilter = KMeansAttributesLabelMapFilterType::New();

  // Inputs
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();
  labeledReader->SetFileName(lfname);
  labeledReader->UpdateOutputInformation();
  labeledReader->Update();

  // Filter
  filter->SetInput(labeledReader->GetOutput());
  filter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());
  filter->Update();

  shapeFilter->SetInput(filter->GetOutput());
  shapeFilter->Update();

  // Labelize the objects
  LabelMapType::Pointer labelMap = shapeFilter->GetOutput();

  IteratorType loIt = IteratorType( labelMap );

  unsigned int labelObjectID = 0;

  while ( !loIt.IsAtEnd() )
    {
    unsigned int classLabel = labelObjectID % 3;
    loIt.GetLabelObject()->SetClassLabel(classLabel);
    ++labelObjectID;
    ++loIt;
    }

  kmeansLabelMapFilter->SetInputLabelMap(labelMap);
  std::vector<std::string> attributes = labelMap->GetLabelObject(0)->GetAvailableAttributes();
  std::vector<std::string>::const_iterator attrIt;
  for (attrIt = attributes.begin(); attrIt != attributes.end(); ++attrIt)
    {
    kmeansLabelMapFilter->GetMeasurementFunctor().AddAttribute((*attrIt).c_str());
    }

  kmeansLabelMapFilter->SetNumberOfClasses(3);
  kmeansLabelMapFilter->Compute();

  std::ofstream outfile(outfname);
  const KMeansAttributesLabelMapFilterType::CentroidsVectorType& centroids = kmeansLabelMapFilter->GetCentroids();
  for (unsigned int i = 0; i < centroids.size(); ++i)
    {
    outfile << "Centroid " << i << " : " << centroids[i] << std::endl;
    }
  outfile.close();

  return EXIT_SUCCESS;
}
