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
#include "otbNonGaussianRBFKernelFunctor.h"

namespace otb
{

NonGaussianRBFKernelFunctor
::NonGaussianRBFKernelFunctor ()
  : GenericKernelFunctorBase ()
{
  this->SetName("NonGaussianRBF");
  m_Alpha = 1.0;
  m_Beta = 2.0;
  m_Gamma = 1.0;

  SetValue("Alpha", m_Alpha);
  SetValue("Beta", m_Beta);
  SetValue("Gamma", m_Gamma);
}

double
NonGaussianRBFKernelFunctor
::operator ()(const svm_node * x, const svm_node * y,
              const svm_parameter& /*param*/) const
{
  double pr_x, pr_y, diff, sum = 0.0;

  while (x->index != -1 && y->index != -1)
    {
    pr_x = pow(x->value, m_Alpha);
    pr_y = pow(y->value, m_Alpha);

    diff = pow(fabs(pr_x - pr_y), m_Beta);

    sum += diff;

    ++x;
    ++y;
    }

  return exp(-m_Gamma * sum);
}

void
NonGaussianRBFKernelFunctor
::Update()
{
  m_Alpha = GetValue<double>("Alpha");
  m_Beta = GetValue<double>("Beta");
  m_Gamma = GetValue<double>("Gamma");
}

} // end of namespace otb
