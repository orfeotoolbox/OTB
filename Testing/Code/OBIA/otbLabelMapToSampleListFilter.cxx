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


#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkBinaryImageToLabelMapFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "itkLabelMap.h"
#include "otbShapeAttributesLabelMapFilter.h"

#include "itkVariableLengthVector.h"
#include "itkListSample.h"

#include "otbLabelMapToSampleListFilter.h"

int otbLabelMapToSampleListFilter(int argc, char* argv[])
{
  const char * infname = argv[1];

 // Labeled image type
  const unsigned int Dimension = 2;
  typedef unsigned short                                                 LabelType;
  typedef otb::Image<LabelType, Dimension>                                LabeledImageType;
  typedef otb::ImageFileReader<LabeledImageType>                         LabeledReaderType;
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>      LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                 LabelMapType;
  typedef itk::BinaryImageToLabelMapFilter<LabeledImageType, LabelMapType> LabelMapFilterType;
  typedef otb::ShapeAttributesLabelMapFilter<LabelMapType>               ShapeLabelMapFilterType;



  typedef itk::VariableLengthVector<double>                      VectorType;
  typedef itk::Statistics::ListSample<VectorType>                ListSampleType;
  typedef otb::LabelMapToSampleListFilter<LabelMapType,
                                          ListSampleType>        LabelMap2ListSampleFilterType;

  // instantiation
   LabeledReaderType::Pointer lreader = LabeledReaderType::New();
   lreader->SetFileName(infname);
   
   LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
   labelMapFilter->SetInput(lreader->GetOutput());
   labelMapFilter->SetInputForegroundValue(255);

   ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
   shapeLabelMapFilter->SetInput(labelMapFilter->GetOutput());
   shapeLabelMapFilter->SetReducedAttributeSet(false);
   shapeLabelMapFilter->SetComputePerimeter(true);
   shapeLabelMapFilter->SetComputeFeretDiameter(true);
   shapeLabelMapFilter->Update();

   LabelMap2ListSampleFilterType::Pointer filter = LabelMap2ListSampleFilterType::New();

   filter->SetInputLabelMap(shapeLabelMapFilter->GetOutput());

   filter->GetMeasurementFunctor().AddAttribute("test");
   filter->GetMeasurementFunctor().RemoveAttribute("test");
   filter->GetMeasurementFunctor().AddAttribute("test");
   filter->GetMeasurementFunctor().ClearAttributes();

   for(int i = 2; i<argc; ++i)
     {
     filter->GetMeasurementFunctor().AddAttribute(argv[i]);
     }
   
   std::cout<<"Number of attributes: "<<filter->GetMeasurementFunctor().GetNumberOfAttributes()<<std::endl;

   filter->Update();

  return EXIT_SUCCESS;
}
