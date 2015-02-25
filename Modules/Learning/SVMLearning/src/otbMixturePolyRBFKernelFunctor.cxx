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
#include "otbMixturePolyRBFKernelFunctor.h"

namespace otb
{

MixturePolyRBFKernelFunctor
::MixturePolyRBFKernelFunctor ()
  : GenericKernelFunctorBase ()
{
  this->SetName("MixturePolyRBF");
  m_Mixture = 0.5;
  m_GammaPoly = 1.0;
  m_CoefPoly = 1.0;
  m_DegreePoly = 2;
  m_GammaRBF = 1.0;

  SetValue("Mixture", m_Mixture);
  SetValue("GammaPoly", m_GammaPoly);
  SetValue("CoefPoly", m_CoefPoly);
  SetValue("DegreePoly", m_DegreePoly);
  SetValue("GammaRBF", m_GammaRBF);
}

double
MixturePolyRBFKernelFunctor
::operator ()(const svm_node * x, const svm_node * y,
              const svm_parameter& /*param*/) const
{
  double kernel_poly = 0.0;
  if (m_Mixture > 0.0)
    kernel_poly = powi(m_GammaPoly * this->dot(x, y) + m_CoefPoly,
                       m_DegreePoly);

  double kernel_rbf = 0.0;
  if (m_Mixture < 1.0)
    {
    double sum = 0.0;
    while (x->index != -1 && y->index != -1)
      {
      if (x->index == y->index)
        {
        double d = x->value - y->value;
        sum += d * d;
        ++x;
        ++y;
        }
      else
        {
        if (x->index > y->index)
          {
          sum += y->value * y->value;
          ++y;
          }
        else
          {
          sum += x->value * x->value;
          ++x;
          }
        }
      }

    while (x->index != -1)
      {
      sum += x->value * x->value;
      ++x;
      }

    while (y->index != -1)
      {
      sum += y->value * y->value;
      ++y;
      }

    kernel_rbf = exp(-m_GammaRBF * sum);
    }

  return m_Mixture * kernel_poly + (1.0 - m_Mixture) * kernel_rbf;
}

void
MixturePolyRBFKernelFunctor
::Update()
{
  m_Mixture = GetValue<double>("Mixture");
  m_GammaPoly = GetValue<double>("GammaPoly");
  m_CoefPoly = GetValue<double>("CoefPoly");
  m_DegreePoly = GetValue<int>("DegreePoly");
  m_GammaRBF = GetValue<double>("GammaRBF");
}

double
MixturePolyRBFKernelFunctor
::powi(double base, int times) const
{
  double tmp = base, ret = 1.0;

  for (int t = times; t > 0; t /= 2)
    {
    if (t % 2 == 1) ret *= tmp;
    tmp = tmp * tmp;
    }
  return ret;
}

} // end of namespace otb
