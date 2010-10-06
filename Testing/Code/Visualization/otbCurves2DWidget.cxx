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

int otbCurves2DWidget(int argc, char * argv[])
{

  // typedefs
  typedef otb::Curves2DWidget    WidgetType;
  typedef WidgetType::PointType  PointType;
  typedef WidgetType::VectorType VectorType;

  PointType  origin;
  VectorType length;
  PointType  gridOrigin;
  VectorType gridSpacing;

  origin[0] = atof(argv[1]);
  origin[1] = atof(argv[2]);
  length[0] = atof(argv[3]);
  length[1] = atof(argv[4]);
  gridOrigin[0] = atof(argv[5]);
  gridOrigin[1] = atof(argv[6]);
  gridSpacing[0] = atof(argv[7]);
  gridSpacing[1] = atof(argv[8]);
  const bool run = atoi(argv[9]);

  // Create a widget
  WidgetType::Pointer widget = WidgetType::New();
  widget->SetAxisOrigin(origin);
  if( origin != widget->GetAxisOrigin())
    {
      std::cout << "error with GetAxisOrigin() method ! " << std::endl;
      return EXIT_FAILURE;
    }

  widget->SetAxisLength(length);
  if( length != widget->GetAxisLength())
    {
      std::cout << "error with GetAxisLength() method ! " << std::endl;
      return EXIT_FAILURE;
    }

  widget->SetGridOrigin(gridOrigin);
  if( gridOrigin != widget->GetGridOrigin())
    {
      std::cout << "error with GetGridOrigin() method ! " << std::endl;
      return EXIT_FAILURE;
    }

  widget->SetGridSpacing(gridSpacing);
  if( gridSpacing != widget->GetGridSpacing())
    {
      std::cout << "error with GetGridSpacing() method ! " << std::endl;
      return EXIT_FAILURE;
    }


  WidgetType::ColorType white;
  white.Fill(1);
  if(widget->GetBackgroundColor() != white)
    {
    std::cout << "error with GetBackgroundColor() method ! " << std::endl;
    return EXIT_FAILURE;
    }
  widget->SetBackgroundColor(white);

  // Colors
  WidgetType::ColorType axisColor;
  WidgetType::ColorType gridColor;

  axisColor.Fill(0.);
  gridColor.Fill(0.9);

  if(widget->GetAxisColor() != axisColor)
    {
    std::cout << "error with GetAxisColor() method ! " << std::endl;
    return EXIT_FAILURE;
    }
  widget->SetAxisColor(axisColor);

  if(widget->GetGridColor() != gridColor)
    {
    std::cout << "error with GetGridColor() method ! " << std::endl;
    return EXIT_FAILURE;
    }
  widget->SetGridColor(gridColor);

  // Default margin
  WidgetType::SizeType margins;
  margins.Fill(20);
  if(widget->GetMargins() != margins)
    {
    std::cout << "error with GetMargins() method ! " << std::endl;
    return EXIT_FAILURE;
    }
  widget->SetMargins(margins);

  widget->AutoScaleOn();
  if(widget->GetAutoScale() != true)
    {
    std::cout << "error with m_AutoScale value ! " << std::endl;
    return EXIT_FAILURE;
    }

  widget->ZeroCrossingAxisOn();
  if(widget->GetZeroCrossingAxis() != true)
    {
    std::cout << "error with m_ZeroCrossingAxis value ! " << std::endl;
    return EXIT_FAILURE;
    }


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
