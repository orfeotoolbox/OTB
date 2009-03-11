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

int otbCurves2DWidget( int argc, char * argv[] )
{
  
  // typedefs
  typedef otb::Curves2DWidget    WidgetType;
  typedef WidgetType::PointType  PointType;
  typedef WidgetType::VectorType VectorType;

  PointType origin;
  VectorType length;
  PointType gridOrigin;
  VectorType gridSpacing;

  origin[0]=atof(argv[1]);
  origin[1]=atof(argv[2]);
  length[0]=atof(argv[3]);
  length[1]=atof(argv[4]);
  gridOrigin[0]=atof(argv[5]);
  gridOrigin[1]=atof(argv[6]);
  gridSpacing[0]=atof(argv[7]);
  gridSpacing[1]=atof(argv[8]);
  const bool run = atoi(argv[9]);
  
   // Create a widget
  WidgetType::Pointer widget = WidgetType::New();
  widget->SetAxisOrigin(origin);
  widget->SetAxisLength(length);
  widget->SetGridOrigin(gridOrigin);
  widget->SetGridSpacing(gridSpacing);

  // Resize it
  Fl_Window window(500,500);
  window.add(widget);
  window.resizable(widget);
  window.show();
  widget->show();
  widget->resize(0,0,500,500);
  // Show it
  widget->show();
  // Refresh display
  if(run)
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
