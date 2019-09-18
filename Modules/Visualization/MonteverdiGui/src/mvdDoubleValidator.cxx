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

#include "mvdDoubleValidator.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
/*
  TRANSLATOR mvd::DoubleValidator

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DoubleValidator::DoubleValidator(QObject* p) : QDoubleValidator(p)
{
}

/*******************************************************************************/
DoubleValidator::DoubleValidator(double bottomVal, double topVal, int decim, QObject* p) : QDoubleValidator(bottomVal, topVal, decim, p)
{
}

/*******************************************************************************/
DoubleValidator::~DoubleValidator()
{
}

/*******************************************************************************/
void DoubleValidator::fixup(QString& input) const
{
  int pos = input.length();

  if (validate(input, pos) != QValidator::Intermediate)
    return;

  bool   isOk;
  double value = input.toDouble(&isOk);

  if (!isOk)
  {
  }

  if (value < bottom())
    input = QString::number(bottom(), 'g', decimals());

  else if (value > top())
    input = QString::number(top(), 'g', decimals());
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
