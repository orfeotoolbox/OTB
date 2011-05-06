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
#include "otbConnectedComponentSegmentation.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"


#include <iostream>
#include <iomanip>
#include <fstream>
#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbMath.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


#include "otbParser.h"

#include "itkRelabelComponentImageFilter.h"
#include <itkLabelMap.h>
#include <otbLabelMapWithAdjacency.h>


#include <otbLabelImageToLabelMapWithAdjacencyFilter.h>

#include <itkScalarToRGBPixelFunctor.h>
#include <itkUnaryFunctorImageFilter.h>
#include "otbConnectedComponentMuParserFunctor.h"
#include <itkConnectedComponentFunctorImageFilter.h>

#include "otbMaskMuParserFilter.h"

#include <otbAttributesMapLabelObject.h>
#include <itkAttributeLabelObject.h>
#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include <itkStatisticsLabelObject.h>
#include <itkShapeLabelMapFilter.h>
#include <otbShapeAttributesLabelMapFilter.h>
#include <itkLabelObject.h>
#include "itkLabelImageToShapeLabelMapFilter.h"
#include <iostream>
#include "otbLabelObjectOpeningMuParserFilter.h"

#include <itkLabelMapToLabelImageFilter.h>

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"

#include <otbLabelMapToVectorDataFilter.h>

namespace otb
{

typedef float InputPixelType;
const unsigned int Dimension = 2;

typedef itk::RGBPixel<unsigned char> RGBPixelType;
typedef otb::Image<RGBPixelType, 2>  RGBImageType;


typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
typedef otb::ImageFileReader<InputVectorImageType> ReaderType;

typedef otb::Image<unsigned int, Dimension> InputMaskImageType;

typedef otb::ImageFileReader<InputMaskImageType> MaskReaderType;

typedef otb::Image<unsigned int, Dimension> OutputImageType;
typedef otb::Image<unsigned int, Dimension> LabelImageType;

typedef otb::ImageFileWriter<OutputImageType> WriterType;
typedef otb::ImageFileWriter<RGBImageType>    RGBWriterType;

typedef Functor::ConnectedComponentMuParserFunctor<InputVectorImageType::PixelType> FunctorType;

typedef itk::ConnectedComponentFunctorImageFilter<InputVectorImageType, LabelImageType, FunctorType, InputMaskImageType>
    ConnectedComponentFilterType;

// Labelization
typedef itk::RelabelComponentImageFilter<LabelImageType, LabelImageType> RelabelComponentFilterType;
typedef otb::AttributesMapLabelObject<unsigned int, Dimension, double>   AttributesMapLabelObjectType;
typedef itk::AttributeLabelObject<unsigned int, Dimension, double>       AttributesLabelObjectType;

typedef otb::LabelMapWithAdjacency<AttributesMapLabelObjectType> AttributesLabelMapType;
typedef otb::LabelImageToLabelMapWithAdjacencyFilter<LabelImageType, AttributesLabelMapType>
    LabelImageToLabelMapFilterType;

typedef otb::BandsStatisticsAttributesLabelMapFilter<AttributesLabelMapType, InputVectorImageType>
    RadiometricLabelMapFilterType;
typedef otb::ShapeAttributesLabelMapFilter<AttributesLabelMapType> ShapeLabelMapFilterType;
typedef itk::ShapeLabelObject<unsigned int, Dimension>             ShapeLabelObjectType;
typedef itk::LabelObject<unsigned int, Dimension>                  LabelObjectType;
typedef itk::LabelMap<ShapeLabelObjectType>                        ShapeLabelMapType;

typedef itk::LabelImageToShapeLabelMapFilter<OutputImageType, ShapeLabelMapType> LabelImageToShapeLabelMapFilterType;

// colored label image typedef
typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long> ColorMapFunctorType;
typedef itk::UnaryFunctorImageFilter<LabelImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;

// mask typedef
typedef otb::MaskMuParserFilter<InputVectorImageType, OutputImageType> MaskMuParserFilterType;


typedef otb::LabelObjectOpeningMuParserFilter<AttributesLabelMapType>            LabelObjectOpeningFilterType;
typedef itk::LabelMapToLabelImageFilter<AttributesLabelMapType, OutputImageType> LabelMapToLabelImageFilterType;

/** Vector data handling */
typedef otb::VectorData<double, Dimension>          VectorDataType;
typedef VectorDataType::Pointer                     VectorDataPointerType;
typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataFileWriterType;
typedef VectorDataFileWriterType::Pointer           VectorDataFileWriterPointerType;

typedef otb::LabelMapToVectorDataFilter<AttributesLabelMapType, VectorDataType> LabelMapToVectorDataFilterType;

int ConnectedComponentSegmentation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Connected Component Segmentation");
  descriptor->SetDescription(
                             "Connected Component Segmentation, which take mathematical formula as an Neighborhood thresholding criteria.");
  descriptor->AddOption("InputImageFileName", "Input frame file name", "in", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputFileName", "Output file name", "out", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputShapeFileName", "Output Shape file name", "outshape", 1, true,
                        ApplicationDescriptor::FileName);
  descriptor->AddOption("Expression", "Formula", "expression", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("MinSize", "Min object size (area in pixel)", "minsize", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("InputImageMaskFileName", "Image for mask computation", "inmask", 1, false,
                        ApplicationDescriptor::FileName);
  descriptor->AddOption("MaskExpression", "Mask expression (only if support image is given)", "maskexpression", 1,
                        false, ApplicationDescriptor::String);
  descriptor->AddOption("OBIAExpression", "OBIA Mu Parser expression", "OBIAexpression", 1, true,
                        ApplicationDescriptor::String);
  descriptor->AddOption("IntermediateOutput", "Save intermediate output", "dbg", 0, false,
                        ApplicationDescriptor::Boolean);

  return EXIT_SUCCESS;
}

int ConnectedComponentSegmentation::Execute(otb::ApplicationOptionsResult* parseResult)
{
  ConnectedComponentFilterType::Pointer connected = ConnectedComponentFilterType::New();

  // Read the input image
  std::string inFileName = parseResult->GetParameterString("InputImageFileName");

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inFileName);
  reader->UpdateOutputInformation();
  connected->SetInput(reader->GetOutput());

  MaskReaderType::Pointer maskReader;
  ReaderType::Pointer maskImageReader;
  MaskMuParserFilterType::Pointer maskFilter;

  WriterType::Pointer writer = WriterType::New();
  // Read the mask image
  if (parseResult->IsOptionPresent("MaskExpression"))
    {
    std::string maskFileName;
    if (parseResult->IsOptionPresent("InputImageMaskFileName"))
      {
      maskFileName = parseResult->GetParameterString("InputImageMaskFileName");
      }
    else // use image input
      {
      maskFileName = inFileName;
      }

    maskImageReader = ReaderType::New();
    maskImageReader->SetFileName(maskFileName);

    maskImageReader->UpdateOutputInformation();

    maskFilter = MaskMuParserFilterType::New();
    maskFilter->SetInput(maskImageReader->GetOutput());

    std::string maskexpression = parseResult->GetParameterString("MaskExpression");

    maskFilter->SetExpression(maskexpression);
    maskFilter->UpdateOutputInformation();
    if (parseResult->IsOptionPresent("IntermediateOutput"))
      {

      writer->SetInput(maskFilter->GetOutput());
      writer->SetFileName("binaryMask.tif");
      writer->Update();
      }

    connected->SetMaskImage(maskFilter->GetOutput());
    }
  else
    if (parseResult->IsOptionPresent("InputImageMaskFileName"))
      {
      std::string maskFileName = parseResult->GetParameterString("InputImageMaskFileName");

      maskReader = MaskReaderType::New();
      maskReader->SetFileName(maskFileName);
      maskReader->UpdateOutputInformation();
      connected->SetMaskImage(maskReader->GetOutput());

      }

  // set up formula for connected component segmentation
  std::string expression = parseResult->GetParameterString("Expression");

  connected->GetFunctor().SetExpression(expression);
  connected->Update();

  if (parseResult->IsOptionPresent("IntermediateOutput"))
    {
    writer->SetInput(connected->GetOutput());
    writer->SetFileName("connectedComponentLabelImage.tif");
    writer->Update();
    }

  // relabel component
  double minObjectSize;
  if (parseResult->IsOptionPresent("MinSize"))
    minObjectSize = parseResult->GetParameterDouble("MinSize");
  else minObjectSize = 2;

  // relabel connected component output
  RelabelComponentFilterType::Pointer relabel = RelabelComponentFilterType::New();
  relabel->SetInput(connected->GetOutput());
  relabel->SetMinimumObjectSize(minObjectSize);

  // debug output
  if (parseResult->IsOptionPresent("IntermediateOutput"))
    {
    ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
    colormapper->SetInput(relabel->GetOutput());
    RGBWriterType::Pointer RGBwriter = RGBWriterType::New();
    RGBwriter->SetInput(colormapper->GetOutput());
    RGBwriter->SetFileName("RGBLabeledImage.tif");
    RGBwriter->Update();
    }

  // step 3 transformation into labelmap and object characterization

  //Attributes computation
  // LabelImage to Label Map transformation
  LabelImageToLabelMapFilterType::Pointer labelImageToLabelMap = LabelImageToLabelMapFilterType::New();
  labelImageToLabelMap->SetInput(relabel->GetOutput());
  labelImageToLabelMap->SetBackgroundValue(0);

  // intermediate step : Fusion
  // TBD

  // shape attributes computation
  ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(labelImageToLabelMap->GetOutput());
  shapeLabelMapFilter->SetReducedAttributeSet(false);

  // band stat attributes computation
  RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter = RadiometricLabelMapFilterType::New();

  radiometricLabelMapFilter->SetInput(shapeLabelMapFilter->GetOutput());
  radiometricLabelMapFilter->SetFeatureImage(reader->GetOutput());
  radiometricLabelMapFilter->SetReducedAttributeSet(false);
  radiometricLabelMapFilter->Update();

  AttributesLabelMapType::Pointer AttributesLabelMap;
  AttributesLabelMap = radiometricLabelMapFilter->GetOutput();
  //AttributesLabelMap->SetAdjacencyMap(labelImageToLabelMap->GetOutput()->GetAdjacencyMap());

  // step 4 OBIA Filtering using shape and radiometric object characteristics

  std::string OBIAexpression = parseResult->GetParameterString("OBIAExpression");

  LabelObjectOpeningFilterType::Pointer opening = LabelObjectOpeningFilterType::New();
  opening->SetExpression(OBIAexpression);
  opening->SetInput(radiometricLabelMapFilter->GetOutput());
  opening->Update();

  LabelMapToLabelImageFilterType::Pointer labelMapToLabelImage = LabelMapToLabelImageFilterType::New();
  labelMapToLabelImage->SetInput(opening->GetOutput());
  labelMapToLabelImage->Update();

  if (parseResult->IsOptionPresent("IntermediateOutput"))
    {
    ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
    colormapper->SetInput(labelMapToLabelImage->GetOutput());

    RGBWriterType::Pointer RGBwriter = RGBWriterType::New();
    RGBwriter->SetInput(colormapper->GetOutput());
    RGBwriter->SetFileName("RGBOpeningOutput.tif");
    RGBwriter->Update();

    // removed object dbg output
    LabelMapToLabelImageFilterType::Pointer labelMapToLabelImageRemoved = LabelMapToLabelImageFilterType::New();

    labelMapToLabelImageRemoved->SetInput(opening->GetOutput(1));
    labelMapToLabelImageRemoved->Update();
    //ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
    colormapper->SetInput(labelMapToLabelImageRemoved->GetOutput());

    //RGBWriterType::Pointer RGBwriter = RGBWriterType::New();
    RGBwriter->SetInput(colormapper->GetOutput());
    RGBwriter->SetFileName("RGBOpeningRemovedObjects.tif");
    RGBwriter->Update();

    }

  //step 5 : Vectorization

  LabelMapToVectorDataFilterType::Pointer labelMapToVectorDataFilter = LabelMapToVectorDataFilterType::New();
  labelMapToVectorDataFilter->SetInput(opening->GetOutput());
  labelMapToVectorDataFilter->Update();

  VectorDataPointerType vectorData = labelMapToVectorDataFilter->GetOutput();

  // Instantiate the vdwriter
  std::string vdoutFilename = parseResult->GetParameterString("OutputShapeFileName");

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(vectorData);
  vdwriter->SetFileName(vdoutFilename);
  vdwriter->Update();

  // Instantiate the writer
  std::string outFilename = parseResult->GetParameterString("OutputFileName");

  writer->SetInput(labelMapToLabelImage->GetOutput());
  writer->SetFileName(outFilename);
  writer->Update();

  return EXIT_SUCCESS;
}
}








