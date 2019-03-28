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

#ifndef mvdApplicationLauncher_h
#define mvdApplicationLauncher_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


#include <QtWidgets>
#include "OTBMonteverdiGUIExport.h"
#include "otbWrapperApplication.h"

namespace mvd
{

/**
 * \class ApplicationLauncher
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class OTBMonteverdiGUI_EXPORT ApplicationLauncher : public QObject
{
  Q_OBJECT;

public:
  /** \brief Constructor. */
  ApplicationLauncher(QObject* p = nullptr);

  /** \brief Destructor. */
  ~ApplicationLauncher() override;

  otb::Wrapper::Application::Pointer PrepareApplication(const QString& appName, bool isStandalone = false) const;

  QWidget* NewOtbApplicationWindow(const QString& appName, bool isStandalone = false, QWidget* p = nullptr) const;
};

} // namespace mvd

#endif // mvdApplicationLauncher_h
