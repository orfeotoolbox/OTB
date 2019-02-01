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

#ifndef otbWrapperQtWidgetParameterLabel_h
#define otbWrapperQtWidgetParameterLabel_h

#include <QtWidgets>
#include "otbWrapperParameter.h"

#include "OTBQtWidgetExport.h"

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetParameterLabel
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetParameterLabel : public QWidget
{
  Q_OBJECT
public:
  QtWidgetParameterLabel(Parameter*, QWidget*);
  ~QtWidgetParameterLabel() override;

private:
  QtWidgetParameterLabel(const QtWidgetParameterLabel&) = delete;
  void operator=(const QtWidgetParameterLabel&) = delete;
};


}
}

#endif
