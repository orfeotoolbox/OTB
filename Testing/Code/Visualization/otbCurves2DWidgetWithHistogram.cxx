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
#include "otbCurves2DWidget.h"
#include <FL/Fl.H>

#include "otbVectorImage.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"
#include "otbImageFileReader.h"
#include "itkImageRegionIterator.h"

#include "otbHistogramCurve.h"

int otbCurves2DWidgetWithHistogram(int argc, char * argv[])
{
  const char * infname = argv[1];
  const bool   run = atoi(argv[2]);

  // typedefs
  typedef otb::Curves2DWidget    WidgetType;
  typedef WidgetType::PointType  PointType;
  typedef WidgetType::VectorType VectorType;

  // Histogram generator
  typedef double                                       PixelType;
  typedef otb::VectorImage<PixelType>                  VectorImageType;
  typedef VectorImageType::PixelType                   VectorPixelType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef otb::ListSampleToHistogramListGenerator
  <ListSampleType, PixelType>                         HistogramGeneratorType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef HistogramGeneratorType::HistogramType HistogramType;

  // Histogram Widget
  typedef otb::HistogramCurve<HistogramType> HistogramCurveType;

  // Instantiation
  ReaderType::Pointer             reader    = ReaderType::New();
  HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();
  ListSampleType::Pointer         ls                = ListSampleType::New();

  // histogram generation
  reader->SetFileName(infname);
  reader->Update();

  itk::ImageRegionConstIterator<VectorImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());
  ls->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    ls->PushBack(it.Get());
    }

  generator->SetListSample(ls);
  generator->SetNumberOfBins(255);
  generator->Update();

  // Create a widget
  WidgetType::Pointer widget = WidgetType::New();

  HistogramCurveType::ColorType red, green, blue;
  red.Fill(0);
  red[0] = 1.;
  red[3] = 0.5;

  green.Fill(0);
  green[1] = 1.;
  green[3] = 0.5;

  blue.Fill(0);
  blue[2] = 1.;
  blue[3] = 0.5;

  HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
  rhistogram->SetHistogram(generator->GetOutput()->GetNthElement(0));
  rhistogram->SetHistogramColor(red);
  rhistogram->SetLabelColor(red);

  HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
  ghistogram->SetHistogram(generator->GetOutput()->GetNthElement(1));
  ghistogram->SetHistogramColor(green);
  ghistogram->SetLabelColor(green);

  HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
  bhistogram->SetHistogram(generator->GetOutput()->GetNthElement(2));
  bhistogram->SetHistogramColor(blue);
  bhistogram->SetLabelColor(blue);

  widget->AddCurve(rhistogram);
  widget->AddCurve(ghistogram);
  widget->AddCurve(bhistogram);

  // Resize it
  Fl_Window window(500, 500);
  window.add(widget);
  window.resizable(widget);
  window.show();
  widget->show();
  widget->resize(0, 0, 500, 500);
  // Show it
  widget->show();
  // Refresh display
  if (run)
    {
    Fl::run();
    }
  else
    {
    Fl::check();
    }

  window.remove(widget);

  return EXIT_SUCCESS;
}
