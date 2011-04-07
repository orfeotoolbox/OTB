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

#include <itkScalarToRGBPixelFunctor.h>
#include <itkUnaryFunctorImageFilter.h>
#include "otbConnectedComponentMuParserFunctor.h"
#include <itkConnectedComponentFunctorImageFilter.h>

#include "otbMaskMuParserFilter.h"

namespace otb
{


typedef float InputPixelType;
const unsigned int     Dimension = 2;

typedef itk::RGBPixel<unsigned char>   RGBPixelType;
typedef otb::Image<RGBPixelType, 2>    RGBImageType;
typedef otb::ImageFileWriter<RGBImageType>                RGBWriterType;
//conected component typedef
typedef otb::VectorImage<InputPixelType,  Dimension>         InputVectorImageType;
typedef otb::ImageFileReader<InputVectorImageType>           ReaderType;

typedef otb::Image<unsigned int,Dimension>                   InputMaskImageType;

typedef otb::ImageFileReader<InputMaskImageType>             MaskReaderType;

typedef otb::Image<unsigned int,Dimension>                   OutputImageType;
typedef otb::ImageFileWriter<OutputImageType> 							 WriterType;
typedef Functor::ConnectedComponentMuParserFunctor<InputVectorImageType::PixelType>  FunctorType;

typedef itk::ConnectedComponentFunctorImageFilter<InputVectorImageType,OutputImageType,FunctorType,InputMaskImageType> ConnectedComponentFilterType;

// Labelization
typedef itk::RelabelComponentImageFilter<OutputImageType,OutputImageType> RelabelComponentFilterType;
//typedef itk::ShapeLabelObject<unsigned int,2>                                   ShapeLabelObjectType;
//typedef itk::LabelMap<ShapeLabelObjectType>                                     ShapeLabelMapType;

// colred label image typedef
typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>  ColorMapFunctorType;
typedef itk::UnaryFunctorImageFilter<OutputImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;

// mask typedef
typedef otb::MaskMuParserFilter<InputVectorImageType,OutputImageType>  MaskMuParserFilterType;


int ConnectedComponentSegmentation::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Connected Component Segmentation");
  descriptor->SetDescription("Connected Component Segmentation, which take mathematical formula as an Neighborhood thresholding criteria.");
  descriptor->AddOption("InputImageFileName", "Input frame file name",
                        "in",  1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputFileName", "Output file name",
                        "out", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Expression", "Formula",
                        "expression", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("MinSize", "Min object size (area in pixel)",
                        "minsize", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("InputImageMaskFileName", "Image for mask computation",
                        "inmask", 1, false, ApplicationDescriptor::FileName);
  descriptor->AddOption("MaskExpression", "Mask expression (only if support image is given)",
                        "maskexpression", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("IntermediateOutput", "Save intermediate output",
                        "dbg", 0, false, ApplicationDescriptor::Boolean);

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
  if(parseResult->IsOptionPresent("MaskExpression"))
    {
    std::string maskFileName;
    if(parseResult->IsOptionPresent("InputImageMaskFileName"))
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

    maskFilter= MaskMuParserFilterType::New();
    maskFilter->SetInput(maskImageReader->GetOutput());

    std::string maskexpression = parseResult->GetParameterString("MaskExpression");

    maskFilter->SetExpression(maskexpression);
    maskFilter->UpdateOutputInformation();
    if(parseResult->IsOptionPresent("IntermediateOutput"))
      {

      writer->SetInput(maskFilter->GetOutput());
      writer->SetFileName("binaryMask.tif");
      writer->Update();
      }

    connected->SetMaskImage(maskFilter->GetOutput());
    }
  else
    if(parseResult->IsOptionPresent("InputImageMaskFileName"))
      {
      std::string maskFileName = parseResult->GetParameterString("InputImageMaskFileName");

      maskReader = MaskReaderType::New();
      maskReader->SetFileName(maskFileName);
      maskReader->UpdateOutputInformation();
      connected->SetMaskImage(maskReader->GetOutput());

      }

  // set formula for connected component segmentation
  std::string expression = parseResult->GetParameterString("Expression");

  connected->GetFunctor().SetExpression(expression);
  connected->Update();

  if(parseResult->IsOptionPresent("IntermediateOutput"))
    {
    writer->SetInput(connected->GetOutput());
    writer->SetFileName("connectedComponentLabelMap.tif");
    writer->Update();
    }

  // relabel component
  double minObjectSize;
  if(parseResult->IsOptionPresent("MinSize"))
    minObjectSize = parseResult->GetParameterDouble("MinSize");
  else
    minObjectSize=2;

  RelabelComponentFilterType::Pointer relabel = RelabelComponentFilterType::New();
  relabel->SetInput(connected->GetOutput());
  relabel->SetMinimumObjectSize(minObjectSize);

  // TBD JG : AttributesMapLabelObjects
  // LabelImage to Label Map transformation
  //  LabelImageToLabelMapFilterType::Pointer labelImageToLabelMap = LabelImageToLabelMapFilterType::New();
  //  labelImageToLabelMap->SetInput(relabel->GetOutput());
  //  labelImageToLabelMap->SetBackgroundValue(0);

  // LabelMapShapeAttribute
  // LabelMapStatisticAttributes


  // Instantiate the writer
  if(parseResult->IsOptionPresent("IntermediateOutput"))
    {


    ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
    colormapper->SetInput(relabel->GetOutput());
    RGBWriterType::Pointer RGBwriter = RGBWriterType::New();
    RGBwriter->SetInput(colormapper->GetOutput());
    RGBwriter->SetFileName("RGBLabeledImage.tif");
    RGBwriter->Update();
    }




  std::string outFilename = parseResult->GetParameterString("OutputFileName");


  writer->SetInput(relabel->GetOutput());
  writer->SetFileName(outFilename);
  writer->Update();

  return EXIT_SUCCESS;
}
}



















