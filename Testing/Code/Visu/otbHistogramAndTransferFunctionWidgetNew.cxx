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
#include "itkScalarImageToHistogramGenerator.h"
#include "otbHistogramAndTransferFunctionWidget.h"
#include "otbImage.h"

int otbHistogramAndTransferFunctionWidgetNew(int argc, char * argv[])
{
  typedef unsigned char PixelType;
  const unsigned int Dimension =2;
  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef itk::Statistics::ScalarImageToHistogramGenerator<ImageType> GeneratorType;
  typedef GeneratorType::HistogramType HistogramType;

  typedef otb::HistogramAndTransferFunctionWidget<HistogramType,PixelType> WidgetType;

  WidgetType::Pointer widget = WidgetType::New();
  
  return EXIT_SUCCESS;
}
