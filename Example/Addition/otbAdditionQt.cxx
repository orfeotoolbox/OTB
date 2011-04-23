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
#include <QApplication>
#include "otbWrapperQtWidgetView.h"

#include "otbAddition.h"
typedef otb::Wrapper::Addition ApplicationType;

typedef otb::Wrapper::QtWidgetView WidgetType;

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  WidgetType* gui = new WidgetType(ApplicationType::New());
  gui->CreateGui();
  gui->show();
  return app.exec();
}
