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

#ifndef otbWrapperQtWidgetView_h
#define otbWrapperQtWidgetView_h

#include <QtWidgets>
#include "otbWrapperApplication.h"
#include "otbWrapperQtWidgetModel.h"
#include "OTBQtWidgetExport.h"
#include <string>

namespace otb
{
namespace Wrapper
{

/**
 * \class QtWidgetView
 *
 * \ingroup OTBQtWidget
 *
 * \brief WIP.
 */
class OTBQtWidget_EXPORT QtWidgetView :
    public QWidget
{
  Q_OBJECT

public:

  static char const * const OBJECT_NAME;

  /** \brief Constructor. */
  QtWidgetView( const otb::Wrapper::Application::Pointer & otbApp,
		QWidget* parent=nullptr,
		Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  ~QtWidgetView() override;

  /** \brief Gui Creation. */
  void CreateGui();

  /** \brief Model Accessor */
  QtWidgetModel* GetModel() const;

  virtual bool BeforeExecuteButtonClicked()
  {
    return true;
  }

  void Disable();
  void Enable();

signals:
  void ExecuteAndWriteOutput();
  void Stop();
  void OTBApplicationOutputImageChanged( const QString &, const QString &);
  void ExecutionDone( int nbOutputs );

protected:

  bool IsRunning() const;

  virtual QWidget* CreateInputWidgets();

protected slots:

  virtual void OnExecButtonClicked()
  {
  }

private:

  QtWidgetView(const QtWidgetView&) = delete;
  void operator=(const QtWidgetView&) = delete;

  QWidget* CreateFooter();

private:
  otb::Wrapper::QtWidgetModel* m_Model;

  bool m_IsRunning;
};

} // end namespace 'Wrapper'

} // end namespace 'otb'

#endif
