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

#include "itkLabelMapToLabelImageFilter.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbSpatialisationFilter.h"
#include "otbImage.h"
#include "otbAttributesMapLabelObject.h"

int otbSpatialisationTest(int itkNotUsed(argc), char * argv[])
{
  const char * outfilename  = argv[1];

  typedef unsigned short LabelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<LabelType, Dimension> ImageType;
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, std::string>    LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                      LabelMapType;
  typedef otb::ImageFileWriter<ImageType>                                     ImageWriterType;
  typedef otb::SpatialisationFilter<LabelMapType>                             FilterType;
  typedef itk::LabelMapToLabelImageFilter<LabelMapType, ImageType>             LabelMapToLabelImageFilterType;


  /** Instantiation of pointer objects*/
  ImageWriterType::Pointer writer = ImageWriterType::New();
  FilterType::Pointer filter = FilterType::New();
  LabelMapToLabelImageFilterType::Pointer labelMapToLabelImageFilter = LabelMapToLabelImageFilterType::New();

  FilterType::SizeType objectSize;
  objectSize[0]=300;
  objectSize[1]=300;

  FilterType::SizeType nbOjects;
  nbOjects[0]=3;
  nbOjects[1]=3;

  std::vector<std::string> pathVector(9);
  pathVector[0]="path1";
  pathVector[1]="path2";
  pathVector[2]="path3";
  pathVector[3]="path4";
  pathVector[4]="path5";
  pathVector[5]="path6";
  pathVector[6]="path7";
  pathVector[7]="path8";
  pathVector[8]="path9";

  std::vector<std::string> areaVector(9);
  areaVector[0]="moutains";
  areaVector[1]="plants";
  areaVector[2]="roads";
  areaVector[3]="forest";
  areaVector[4]="vegetables";
  areaVector[5]="cars";
  areaVector[6]="water";
  areaVector[7]="rocks";
  areaVector[8]="lunar";

  std::vector<LabelType> labels(9);
  labels[0]=1;
  labels[1]=2;
  labels[2]=3;
  labels[3]=4;
  labels[4]=5;
  labels[5]=6;
  labels[6]=7;
  labels[7]=8;
  labels[8]=9;

  filter->SetObjectSize(objectSize);
  filter->SetNumberOfObjects(nbOjects);
  filter->SetPathVector(pathVector);
  filter->SetAreaVector(areaVector);
  filter->SetLabels(labels);
  filter->Update();
//
//
//   std::cout<<"nb labels : "<<filter->GetOutput()->GetNumberOfLabelObjects()<<std::endl;
//   filter->GetOutput()->PrintLabelObjects();
   labelMapToLabelImageFilter->SetInput(filter->GetOutput());
   labelMapToLabelImageFilter->Update();
//
   writer->SetFileName(outfilename);
   writer->SetInput(labelMapToLabelImageFilter->GetOutput());
   writer->Update();

  return EXIT_SUCCESS;
}
