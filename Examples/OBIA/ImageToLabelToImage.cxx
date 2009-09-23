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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {MSLabeledOutput.tif}
//    OUTPUTS: {OBIAI2L2IOutput.tif}, {OBIAI2L2IInputPretty.tif}, {OBIAI2L2IOutputPretty.tif}
//    1 1108 0
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example shows the basic approach for the transformation of a
//  segmenred (labeled) image into a LabelObjectMap and then back to
//  an image. For this matter we will need the following header files
//  which contain the basic classes.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"

// Software Guide : EndCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 8 )
    {
    std::cerr << "usage: " << argv[0] << " input output inPretty outPretty conn fg bg" << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef unsigned short PixelType;

  typedef otb::Image< PixelType, dim > ImageType;

  typedef itk::LabelObject< PixelType, dim > LabelObjectType;
  typedef itk::LabelMap< LabelObjectType > LabelMapType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::BinaryImageToLabelMapFilter< ImageType, LabelMapType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );
  i2l->SetFullyConnected( atoi(argv[5]) );
  i2l->SetInputForegroundValue( atoi(argv[6]) );
  i2l->SetOutputBackgroundValue( atoi(argv[7]) );

  typedef itk::LabelMapToLabelImageFilter< LabelMapType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( i2l->GetOutput() );

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  // Pretty image creation for the printing
  typedef otb::Image<unsigned char, dim>                                        OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>                                 WriterPrettyType;
  typedef itk::RescaleIntensityImageFilter< ImageType, OutputPrettyImageType>   RescalerType;


  RescalerType::Pointer     rescaler     = RescalerType::New();
  WriterPrettyType::Pointer prettyWriter = WriterPrettyType::New();
  rescaler->SetInput( reader->GetOutput() );
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  prettyWriter->SetFileName( argv[3] );
  prettyWriter->SetInput( rescaler->GetOutput() );
  prettyWriter->Update();

  rescaler->SetInput( l2i->GetOutput() );
  prettyWriter->SetFileName( argv[4] );
  prettyWriter->SetInput( rescaler->GetOutput() );
  prettyWriter->Update();

  return 0;
}







