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

#include "otbImageFileReader.h"

#include <fstream>
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbAttributesMapLabelObjectWithClassLabel.h"
#include "itkLabelMap.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbImageToLabelMapWithAttributesFilter.h"
#include "otbKMeansAttributesLabelMapFilter.h"
#include "otbShapeAttributesLabelMapFilter.h"

const unsigned int Dimension = 2;
typedef unsigned short LabelType;
typedef double         PixelType;

typedef otb::AttributesMapLabelObjectWithClassLabel<LabelType, Dimension, double, LabelType>
                                                                                    LabelObjectType;
typedef itk::LabelMap<LabelObjectType>                                              LabelMapType;
typedef otb::VectorImage<PixelType, Dimension>                                      VectorImageType;
typedef otb::Image<unsigned int, 2>                                                 LabeledImageType;

typedef LabelMapType::Iterator                                                      IteratorType;

typedef otb::ImageFileReader<VectorImageType>                                       ReaderType;
typedef otb::ImageFileReader<LabeledImageType>                                      LabeledReaderType;
typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>             LabelMapFilterType;
typedef otb::ShapeAttributesLabelMapFilter<LabelMapType>                            ShapeFilterType;
typedef otb::KMeansAttributesLabelMapFilter<LabelMapType>                           KMeansAttributesLabelMapFilterType;

int otbKMeansAttributesLabelMapFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  KMeansAttributesLabelMapFilterType::Pointer radiometricLabelMapFilter = KMeansAttributesLabelMapFilterType::New();
  return EXIT_SUCCESS;
}

int otbKMeansAttributesLabelMapFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * lfname  = argv[2];
  const char * outfname = argv[3];

  // SmartPointer instanciation
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
