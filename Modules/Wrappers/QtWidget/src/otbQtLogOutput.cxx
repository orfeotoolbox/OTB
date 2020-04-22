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


#include <iostream>
#include "otbQtLogOutput.h"


namespace otb
{

/** Constructor */
QtLogOutput::QtLogOutput()
{
}

/** Destructor */
QtLogOutput::~QtLogOutput()
{
}


/** flush a buffer */
void QtLogOutput::Flush()
{
}


/** Write to a buffer */
void QtLogOutput::Write(double timestamp)
{
  std::ostringstream oss;
  oss << timestamp;
  Q_EMIT NewContentLog(QString(oss.str().c_str()));
}


/** Write to a buffer */
void QtLogOutput::Write(std::string const& content)
{
  Q_EMIT NewContentLog(QString(content.c_str()));
}


/** Write to a buffer */
void QtLogOutput::Write(std::string const& content, double timestamp)
{
  std::ostringstream oss;
  oss << timestamp << "  :  " << content;
  Q_EMIT NewContentLog(QString(oss.str().c_str()));
}

void QtLogOutput::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
}
