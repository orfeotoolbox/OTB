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
#include "otbImageFileReader.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "otbLabelObjectToPolygonFunctor.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataFileWriter.h"
#include "itkTimeProbe.h"
#include "itkMinimumMaximumImageCalculator.h"

#include "otbCorrectPolygonFunctor.h"

int otbLabelObjectMapVectorizer(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  // Labeled image type
  const unsigned int Dimension                 = 2;
  typedef unsigned short                         LabelType;
  typedef otb::Image<LabelType, Dimension>       LabeledImageType;
  typedef otb::ImageFileReader<LabeledImageType> LabeledReaderType;

  // Label map typedef
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>             LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                          LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>         LabelMapFilterType;
  typedef otb::Polygon<double>                                                    PolygonType;
  typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType, PolygonType> FunctorType;
  typedef otb::VectorData<double, 2>                                              VectorDataType;
  typedef VectorDataType::DataNodeType                                            DataNodeType;
  typedef otb::VectorDataFileWriter<VectorDataType>                               WriterType;
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType>         VectorDataFilterType;
  typedef itk::MinimumMaximumImageCalculator<LabeledImageType>                    MinMaxCalculatorType;

  typedef otb::CorrectPolygonFunctor <PolygonType> CorrectPolygonFunctorType;

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  lreader->SetFileName(infname);

  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(lreader->GetOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());
  std::cout << "min: " << itk::NumericTraits<LabelType>::min() << std::endl;
  std::cout << "max: " << itk::NumericTraits<LabelType>::max() << std::endl;
  //labelMapFilter->SetBackgroundValue(0);
  labelMapFilter->Update();

  FunctorType functor;

  //correct
  CorrectPolygonFunctorType correctPolygon;

  WriterType::Pointer     writer = WriterType::New();
  VectorDataType::Pointer data = VectorDataType::New();

  DataNodeType::Pointer document = DataNodeType::New();
  DataNodeType::Pointer folder1 = DataNodeType::New();

  document->SetNodeType(otb::DOCUMENT);
  folder1->SetNodeType(otb::FOLDER);

  DataNodeType::Pointer root = data->GetDataTree()->GetRoot()->Get();

  data->GetDataTree()->Add(document, root);
  data->GetDataTree()->Add(folder1, document);
  data->SetProjectionRef(lreader->GetOutput()->GetProjectionRef());

  itk::TimeProbe chrono;

  // If a label is given, extract only this label
  if (argc == 4)
    {
    std::cout << "Label is given; Vectorizing object " << atoi(argv[3]) << std::endl;
    chrono.Start();
    PolygonType::Pointer polygon = functor(labelMapFilter->GetOutput()->GetLabelObject(atoi(argv[3])));
    chrono.Stop();

    //correct polygon
    PolygonType::Pointer correct_polygon = correctPolygon(polygon);

    DataNodeType::Pointer node = DataNodeType::New();
    node->SetNodeType(otb::FEATURE_POLYGON);
    node->SetPolygonExteriorRing(correct_polygon);
    data->GetDataTree()->Add(node, folder1);
    }
  // Else extract all labels
  else
    {
    MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
    minMax->SetImage(lreader->GetOutput());
    minMax->Compute();

    for (LabelType label = minMax->GetMinimum(); label <= minMax->GetMaximum(); ++label)
      {
      if (labelMapFilter->GetOutput()->HasLabel(label) && label != labelMapFilter->GetOutput()->GetBackgroundValue())
        {
        std::cout << "Vectorizing object " << label << std::endl;
        chrono.Start();
        PolygonType::Pointer polygon = functor(labelMapFilter->GetOutput()->GetLabelObject(label));
        chrono.Stop();

        //correct polygon
        PolygonType::Pointer correct_polygon = correctPolygon(polygon);

        DataNodeType::Pointer node = DataNodeType::New();
        node->SetNodeType(otb::FEATURE_POLYGON);
        node->SetPolygonExteriorRing(correct_polygon);
        data->GetDataTree()->Add(node, folder1);
        }
      }
    }
  std::cout << "Average vectorization time: " << chrono.GetMean() << " s." << std::endl;

  VectorDataFilterType::Pointer vectorDataProjection = VectorDataFilterType::New();
  vectorDataProjection->SetInputOrigin(lreader->GetOutput()->GetOrigin());
  vectorDataProjection->SetInputSpacing(lreader->GetOutput()->GetSpacing());
  vectorDataProjection->SetInput(data);

  writer->SetFileName(outfname);
  writer->SetInput(vectorDataProjection->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
