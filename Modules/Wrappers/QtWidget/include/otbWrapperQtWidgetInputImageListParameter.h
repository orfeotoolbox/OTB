/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperQtWidgetInputImageListParameter_h
#define otbWrapperQtWidgetInputImageListParameter_h


#include <QtWidgets>


#include "OTBQtWidgetExport.h"
#include "otbWrapperQtWidgetParameterList.h"


namespace otb
{


namespace Wrapper
{


class InputImageListParameter;


/** \class QtWidgetInputImageListParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetInputImageListParameter :
    public QtWidgetParameterList
{
  Q_OBJECT;

//
// Public methods.
public:
  QtWidgetInputImageListParameter( InputImageListParameter *, QtWidgetModel *, QWidget * );
  ~QtWidgetInputImageListParameter() override;

//
// Private methods.
private:
  QtWidgetInputImageListParameter( const QtWidgetInputImageListParameter & ) = delete;
  void operator = ( const QtWidgetInputImageListParameter & ) = delete;
};


}
}

#endif
