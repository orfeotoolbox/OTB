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
#include "otbChangeProfileKernelFunctor.h"

namespace otb
{

ChangeProfileKernelFunctor
::ChangeProfileKernelFunctor ()
  : GenericKernelFunctorBase ()
{
  this->SetName("ChangeProfile");
  m_Coef = 0.0;
  m_Degree = 1.0;
  m_Gamma = 1.0;

  SetValue("Coef", m_Coef);
  SetValue("Degree", m_Degree);
  SetValue("Gamma", m_Gamma);
}

void
ChangeProfileKernelFunctor
::Update()
{
  m_Coef = GetValue<double>("Coef");
  m_Degree = GetValue<double>("Degree");
  m_Gamma = GetValue<double>("Gamma");
}

double
ChangeProfileKernelFunctor
::operator ()(const svm_node * x, const svm_node * y,
              const svm_parameter& /*param*/) const
{
  double theMax(0.);
  double theCur(0.);

  while (x->index != -1 && y->index != -1)
    {
    if (x->index == y->index)
      {
      theCur = fabs(x->value - y->value);
      ++x;
      ++y;
      }
    else
      {
      if (x->index > y->index) ++y;
      else ++x;
      }

    if (theCur > theMax) theMax = theCur;
    }

  return exp(-m_Gamma * pow(theMax, m_Degree) + m_Coef);
}

} // end of namespace otb
