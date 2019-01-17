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

#ifndef otbWrapperQtWidgetFactory_h
#define otbWrapperQtWidgetFactory_h

#include <QtWidgets>
#include "itkObject.h"
#include "itkObjectFactory.h"

#include "OTBQtWidgetExport.h"

namespace otb
{
namespace Wrapper
{

class Parameter;
class QtWidgetModel;
class QtWidgetParameterBase;

/** \class QtWidgetParameterFactory
 * \brief
 *
 * \ingroup OTBQtWidget
 *
 */
class OTBQtWidget_EXPORT QtWidgetParameterFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef QtWidgetParameterFactory     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** New() method */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QtWidgetParameterFactory, Object);

  /** Create the appropriate QtWidget depending on the particulars of the parameter */
  static QtWidgetParameterBase* CreateQtWidget( Parameter* param, QtWidgetModel* model, QWidget * parent );

protected:
  QtWidgetParameterFactory();
  ~QtWidgetParameterFactory() override;

private:
  QtWidgetParameterFactory(const Self&) = delete;
  void operator=(const Self&) = delete;

};

}
}

#endif
