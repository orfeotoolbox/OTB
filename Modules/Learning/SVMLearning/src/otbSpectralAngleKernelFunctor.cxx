/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) GET / ENST Bretagne. All rights reserved.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <cmath>
#include <cfloat>

#include "otbSpectralAngleKernelFunctor.h"

namespace otb
{

SpectralAngleKernelFunctor
::SpectralAngleKernelFunctor ()
  : GenericKernelFunctorBase ()
{
  this->SetName("SAMcoeff");
  m_Coef = 2.0;
  SetValue("Coef", m_Coef);
}

void
SpectralAngleKernelFunctor
::Update()
{
  m_Coef = GetValue<double>("Coef");
}

double
SpectralAngleKernelFunctor
::operator ()(const svm_node * x, const svm_node * y,
              const svm_parameter& /*param*/) const
{
  double mq = m_Coef + SAM(x, y);

  if (mq == 0.0) return DBL_MAX;

  return 1.0 / sqrt(mq);
}

double
SpectralAngleKernelFunctor
::SAM(const svm_node * x, const svm_node * y) const
{
  double den = dot(x, x) * dot(y, y);
  if (den <= 0.0) return 0.0;

  double ss = dot(x, y);
  return /*acos*/ (ss / sqrt(den));
}

}
