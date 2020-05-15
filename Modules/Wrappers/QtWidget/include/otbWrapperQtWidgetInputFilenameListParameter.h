/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperQtWidgetInputFilenameListParameter_h
#define otbWrapperQtWidgetInputFilenameListParameter_h

#include <QtWidgets>

#include "otbWrapperQtWidgetParameterList.h"

namespace otb
{

namespace Wrapper
{

class InputFilenameListParameter;

/** \class QtWidgetInputFilenameListParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetInputFilenameListParameter : public QtWidgetParameterList
{
  Q_OBJECT

  //
  // Public methods.
public:
  QtWidgetInputFilenameListParameter(InputFilenameListParameter*, QtWidgetModel*, QWidget*);
  ~QtWidgetInputFilenameListParameter() override;

  //
  // Private methods.
private:
  QtWidgetInputFilenameListParameter(const QtWidgetInputFilenameListParameter&) = delete;
  void operator=(const QtWidgetInputFilenameListParameter&) = delete;
};
}
}

#endif
