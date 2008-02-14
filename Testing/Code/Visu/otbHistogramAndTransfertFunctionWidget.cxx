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
#include "otbHistogramAndTransfertFunctionWidget.h"
#include "otbImageFileReader.h"
#include "itkScalarImageToHistogramGenerator.h"
#include "otbImage.h"
#include <FL/Fl.H>

int otbHistogramAndTransfertFunctionWidget(int argc, char * argv[])
{
  typedef unsigned char PixelType;
  const unsigned int Dimension =2;
  
  const char * infname = argv[1];

  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::Statistics::ScalarImageToHistogramGenerator<ImageType> GeneratorType;
  typedef GeneratorType::HistogramType HistogramType;
  
  typedef otb::HistogramAndTransfertFunctionWidget<HistogramType> WidgetType;

  ReaderType::Pointer reader = ReaderType::New();
  GeneratorType::Pointer generator  = GeneratorType::New();

  reader->SetFileName(infname);
  reader->Update();

  generator->SetInput(reader->GetOutput());
  generator->SetNumberOfBins(255);
  generator->Compute();
  std::cout<<generator->GetOutput()->GetSize()<<std::endl;

  Fl_Window window(300,200);
  WidgetType::Pointer widget = WidgetType::New();
  widget->SetHistogram(generator->GetOutput());
  widget->resize(0,0,300,200);
  window.resizable(widget.GetPointer());
  window.end();
  window.show();
  widget->show();
  widget->redraw();
  Fl::run();

  return EXIT_SUCCESS;
}
