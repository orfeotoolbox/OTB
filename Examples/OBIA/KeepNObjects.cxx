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
//    OUTPUTS: {OBIAShapeAttribute1.txt}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example shows the
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
// Software Guide : EndCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

//#include "itkLabelStatisticsKeepNObjectsImageFilter.h"

#include "itkLabelImageToLabelMapFilter.h"
#include "itkStatisticsLabelObject.h"
#include "itkStatisticsLabelMapFilter.h"
#include "itkStatisticsKeepNObjectsLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"

int main(int argc, char * argv[])
{

  if (argc != 8)
    {
    std::cerr << "usage: " << argv[0] <<
    " input input output background nb reverseOrdering attribute" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 3;

  typedef unsigned char              PixelType;
  typedef otb::Image<PixelType, dim> IType;

  typedef otb::ImageFileReader<IType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(argv[2]);

  typedef  itk::StatisticsLabelObject<PixelType, dim>           LabelObjectType;
  typedef  itk::LabelMap<LabelObjectType>                       LabelMapType;
  typedef  itk::LabelImageToLabelMapFilter<IType, LabelMapType> LabelizerType;

  typedef itk::StatisticsLabelMapFilter<LabelMapType,
      IType>    LabelObjectValuatorType;
  typedef  itk::StatisticsKeepNObjectsLabelMapFilter<LabelMapType>
  KeepNObjectsType;
  typedef  itk::LabelMapToLabelImageFilter<LabelMapType,
      IType> BinarizerType;

  LabelizerType::Pointer labelizer = LabelizerType::New();
  labelizer->SetInput(reader->GetOutput());
  labelizer->SetBackgroundValue(atoi(argv[4]));

  LabelObjectValuatorType::Pointer valuator = LabelObjectValuatorType::New();
  valuator->SetInput(labelizer->GetOutput());
  valuator->SetFeatureImage(reader2->GetOutput());
  valuator->SetLabelImage(reader->GetOutput());
  valuator->SetComputeHistogram(false);

  KeepNObjectsType::Pointer opening = KeepNObjectsType::New();
  opening->SetInput(valuator->GetOutput());
  opening->SetNumberOfObjects(atoi(argv[5]));
  opening->SetReverseOrdering(atoi(argv[6]));
  opening->SetAttribute(argv[7]);

  BinarizerType::Pointer binarizer = BinarizerType::New();
  binarizer->SetInput(opening->GetOutput());

  /*
  typedef itk::LabelStatisticsKeepNObjectsImageFilter< IType, IType > LabelOpeningType;
  LabelOpeningType::Pointer opening = LabelOpeningType::New();
  opening->SetInput( reader->GetOutput() );
  opening->SetFeatureImage( reader2->GetOutput() );
  opening->SetBackgroundValue( atoi(argv[4]) );
  opening->SetNumberOfObjects( atoi(argv[5]) );
  opening->SetReverseOrdering( atoi(argv[6]) );
  opening->SetAttribute( argv[7] );
  itk::SimpleFilterWatcher watcher(opening, "filter");
*/
  typedef otb::ImageFileWriter<IType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(binarizer->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();
  return 0;
}
