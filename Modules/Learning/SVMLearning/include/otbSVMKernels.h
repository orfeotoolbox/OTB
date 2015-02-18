/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSVMKernels_h
#define __otbSVMKernels_h

#include <vector>
#include <algorithm> // for std::find
#include <cstring> // for strpbrk

#include "itkNumericTraits.h"
#include "otbMath.h"

// Existing kernels
#include "otbSpectralAngleKernelFunctor.h"
#include "otbChangeProfileKernelFunctor.h"
#include "otbNonGaussianRBFKernelFunctor.h"
#include "otbMixturePolyRBFKernelFunctor.h"

#include "otb_libsvm.h"

namespace otb
{
/** \class CustomKernelFunctor
 * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class CustomKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef CustomKernelFunctor          Self;
  typedef GenericKernelFunctorBase     Superclass;

  CustomKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("custom");
  }
  virtual ~CustomKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& /*param*/) const
  {
    return (dot(x, x) - 2.0 * dot(x, y) + dot(y, y));
  }

protected:
  CustomKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  CustomKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }
};

/** \class InvMultiQuadricKernelFunctor
 * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class InvMultiQuadricKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef InvMultiQuadricKernelFunctor Self;
  typedef GenericKernelFunctorBase     Superclass;

  InvMultiQuadricKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("invMultiQuadric");
    this->SetValue<double>("const_coef", 1);
  }
  virtual ~InvMultiQuadricKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    double mq = this->GetValue<double>("const_coef") + m_Custom(x, y, param);
    if (mq == 0.0)
      {
      return itk::NumericTraits<double>::max();
      }
    return 1.0 / sqrt(mq);
  }

protected:
  InvMultiQuadricKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  InvMultiQuadricKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  CustomKernelFunctor m_Custom;
};

/** \class KModKernelFunctor
 * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class KModKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef KModKernelFunctor            Self;
  typedef GenericKernelFunctorBase     Superclass;

  KModKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("KMod");
    this->SetValue<double>("const_coef", 1);
  }
  virtual ~KModKernelFunctor() {}

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    double              mq = this->GetValue<double>("const_coef") + m_Custom(x, y, param);

    if (mq == 0.0)
      {
      return itk::NumericTraits<double>::max();
      }
    return exp(param.gamma / mq) - 1.0;
  }

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

protected:
  KModKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  KModKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  CustomKernelFunctor m_Custom;
};

/** \class SAMKernelFunctor
 * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class SAMKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef SAMKernelFunctor             Self;
  typedef GenericKernelFunctorBase     Superclass;

  SAMKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("SAM");
  }
  virtual ~SAMKernelFunctor() {}

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& /*param*/) const
  {
    double den = dot(x, x) * dot(y, y);
    if (den <= 0.)
      {
      return 0.0;
      }
    double ss = dot(x, y);
    return vcl_acos(ss / vcl_sqrt(den));
  }

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

protected:
  SAMKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  SAMKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }
};

/** \class RadialSAMKernelFunctor
  * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class RadialSAMKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef RadialSAMKernelFunctor       Self;
  typedef GenericKernelFunctorBase     Superclass;

  RadialSAMKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("RadialSAM");
  }
  virtual ~RadialSAMKernelFunctor() {}

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    return vcl_exp(-param.gamma * m_Sam(x, y, param));
  }

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

protected:
  RadialSAMKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  RadialSAMKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  SAMKernelFunctor m_Sam;
};

/** \class InverseCosSAMKernelFunctor
  * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class InverseCosSAMKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef InverseCosSAMKernelFunctor   Self;
  typedef GenericKernelFunctorBase     Superclass;

  InverseCosSAMKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("InverseCosSAM");
  }
  virtual ~InverseCosSAMKernelFunctor() {}

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    return 1.0 - vcl_cos(m_Sam(x, y, param));
  }

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

protected:
  InverseCosSAMKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  InverseCosSAMKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  SAMKernelFunctor m_Sam;
};

/** \class InvMultiQuadraticSAMKernelFunctor
  * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class InvMultiQuadraticSAMKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef InvMultiQuadraticSAMKernelFunctor   Self;
  typedef GenericKernelFunctorBase            Superclass;

  InvMultiQuadraticSAMKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("invMultiQuadraticSAM");
    this->SetValue<double>("const_coef", 1);
  }
  virtual ~InvMultiQuadraticSAMKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    double mq = this->GetValue<double>("const_coef") + m_Sam(x, y, param);

    if (mq == 0.)
      {
      return itk::NumericTraits<double>::max();
      }
    return 1. / sqrt(mq);
  }

protected:
  InvMultiQuadraticSAMKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  InvMultiQuadraticSAMKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  SAMKernelFunctor m_Sam;
};

/** \class KModSAMKernelFunctor
  * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class KModSAMKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef KModSAMKernelFunctor                Self;
  typedef GenericKernelFunctorBase            Superclass;

  KModSAMKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("KModSAM");
    this->SetValue<double>("const_coef", 1);
  }
  virtual ~KModSAMKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    double mq = this->GetValue<double>("const_coef") + m_Sam(x, y, param);

    if (mq == 0.)
      {
      return itk::NumericTraits<double>::max();
      }
    return vcl_exp(param.gamma / mq) - 1.0;
  }

protected:
  KModSAMKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  KModSAMKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  SAMKernelFunctor m_Sam;
};

/** \class RBFKernelFunctor
  * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class RBFKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef RBFKernelFunctor                    Self;
  typedef GenericKernelFunctorBase            Superclass;

  RBFKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("RBF");
    this->SetValue<double>("gamma_coef", 0.5);
  }
  virtual ~RBFKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    double res = this->GetValue<double>("gamma_coef") * m_Custom(x, y, param);
    return vcl_exp(-res);
  }

  virtual double derivative(const svm_node *x,
                            const svm_node *y,
                            const svm_parameter& param,
                            int degree,
                            int index,
                            bool isAtEnd,
                            double constValue) const
  {
    double gamma = this->GetValue<double>("gamma_coef");
    double kernelValue = 0.;
    double xval = 0.;
    double yval = 0.;
    int    compt = 0;

    const svm_node *xtemp = x;
    const svm_node *ytemp = y;

    bool stop = false;
    while (xtemp->index != -1 && ytemp->index != -1 && stop == false)
      {
      if (xtemp->index == ytemp->index)
        {
        if (compt == index)
          {
          xval = xtemp->value;
          yval = ytemp->value;
          stop = true;
          }
        else
          {
          compt++;
          ++ytemp;
          ++xtemp;
          }
        }
      else
        {
        if (xtemp->index > ytemp->index) ++ytemp;
        else ++xtemp;
        }
      }

    if (isAtEnd == true)
      {
      kernelValue = this->operator ()(x, y, param);
      }
    else
      {
      kernelValue = constValue;
      }

    if (degree < 0)
      {
      return 0;
      }
    switch (degree)
      {
      case 0:
        return kernelValue;
        break;
      case 1:
        return (-2 * gamma * (yval - xval) * kernelValue);
        break;
      default:
        return (-2 * gamma *
                ((degree - 1) *
                 this->derivative(x, y, param, degree - 2, index, isAtEnd, constValue) +
                 (yval - xval) * derivative(x, y, param, degree - 1, index, isAtEnd, constValue)));
        break;
      }

  }

protected:
  RBFKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  RBFKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  CustomKernelFunctor m_Custom;
};

/** \class RBFRBFSAMKernelFunctor
* \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class RBFRBFSAMKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef RBFRBFSAMKernelFunctor              Self;
  typedef GenericKernelFunctorBase            Superclass;

  RBFRBFSAMKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("RBFRBFSAM");
    this->SetValue<double>("lin_coef", 1.);
  }
  virtual ~RBFRBFSAMKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    return (this->GetValue<double>("lin_coef") * vcl_exp(-param.gamma * m_Custom(x, y, param))
            + (1.0 - this->GetValue<double>("lin_coef")) * m_RadialSam(x, y, param));
  }

protected:
  RBFRBFSAMKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  RBFRBFSAMKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  CustomKernelFunctor    m_Custom;
  RadialSAMKernelFunctor m_RadialSam;
};

/** \class PolyRBFSAMKernelFunctor
* \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class PolyRBFSAMKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef PolyRBFSAMKernelFunctor  Self;
  typedef GenericKernelFunctorBase Superclass;

  PolyRBFSAMKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("polyRBFSAM");
    this->SetValue<double>("const_coef", 1);
    this->SetValue<double>("lin_coef", 1);
  }
  virtual ~PolyRBFSAMKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    return this->GetValue<double>("const_lin") * vcl_pow(dot(x, y) + this->GetValue<double>("const_coef"), param.degree)
           + (1.0 - this->GetValue<double>("const_coef"))
           * m_RadialSam(x, y, param);
  }

protected:
  PolyRBFSAMKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  PolyRBFSAMKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  RadialSAMKernelFunctor m_RadialSam;
};

/** \class RBFDiffKernelFunctor
* \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class RBFDiffKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef RBFDiffKernelFunctor     Self;
  typedef GenericKernelFunctorBase Superclass;

  RBFDiffKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("RBFDiff");
  }
  virtual ~RBFDiffKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {

    double total = 0.;

    while (x->index != -1 && y->index != -1)
      {
      if (x->index == y->index)
        {
        total = total +  vcl_exp(-param.gamma * vcl_abs(x->value - y->value));
        ++x;
        ++y;
        }
      else
        {
        if (x->index < y->index) ++x;
        else ++y;
        }
      }
    return total;
  }

protected:
  RBFDiffKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  RBFDiffKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

};

/** \class CustomLinearKernelFunctor
* \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class CustomLinearKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef CustomLinearKernelFunctor           Self;
  typedef GenericKernelFunctorBase            Superclass;

  CustomLinearKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("customLinear");
  }
  virtual ~CustomLinearKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    double value=-param.gamma*m_Custom(x, y, param);
    if(value<-30.0)
      {
        return 0.0;
      }

   return (vcl_exp(value));
  }

protected:
  CustomLinearKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  CustomLinearKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  CustomKernelFunctor m_Custom;
};

/** \class GroupedRBFKernelFunctor
* \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class GroupedRBFKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef GroupedRBFKernelFunctor             Self;
  typedef GenericKernelFunctorBase            Superclass;

  GroupedRBFKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("groupedRBF");
  }
  virtual ~GroupedRBFKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    const char * parameters = param.custom;
    const char * twoPoints = ":";
    const char * position = strpbrk (parameters, twoPoints);
    int          twoPointsPosition = 0;
    double       total = 0.;
    double       value = 0.;
    int          numberOfGroups = atoi(parameters);

    twoPointsPosition = position - parameters;

    parameters = parameters + twoPointsPosition + 1;

    int              i, j;
    std::vector<int> begin;
    begin.resize(numberOfGroups);
    std::vector<int> end;
    end.resize(numberOfGroups);

    for (i = 0; i < numberOfGroups; ++i)
      {
      begin[i] = atoi(parameters);
      position = strpbrk (parameters, twoPoints);
      twoPointsPosition = position - parameters;
      parameters = parameters + twoPointsPosition + 1;

      end[i] = atoi(parameters);
      parameters = parameters + twoPointsPosition + 1;
      }

    const svm_node * xBuff = x;
    const svm_node * yBuff = y;

    int sizeX = 0;
    int sizeY = 0;

    while (xBuff->index != 1)
      {
      ++sizeX;
      ++xBuff;
      }

    while (yBuff->index != 1)
      {
      ++sizeY;
      ++yBuff;
      }

    const svm_node * xTemp = NULL;
    const svm_node * yTemp = NULL;

    int index = 0;

    if (sizeX && sizeY)
      {
      svm_node* xGroup = new svm_node[sizeX];
      svm_node* yGroup = new svm_node[sizeY];
      for (j = 0; j < numberOfGroups; ++j)
        {
        xTemp = x;
        yTemp = y;
        index = 0;

        while (xTemp->index != 1 && yTemp->index != 1)
          {
          xGroup[index].index = xTemp->index;
          yGroup[index].index = yTemp->index;
          if ((xTemp->index < begin[j]) || (xTemp->index > end[j]))
            {
            xGroup[index].value = 0;
            yGroup[index].value = 0;
            }
          else
            {
            xGroup[index].value = xTemp->value;
            yGroup[index].value = yTemp->value;
            }
          ++index;
          ++xTemp;
          ++yTemp;
          }

        // value can have different value according to j
        value =  m_CustomLinear(xGroup, yGroup, param);

        total += value;
        }
      delete[] xGroup;
      delete[] yGroup;
      }

    else if ((sizeX > 0) && (sizeY == 0))
      {
      svm_node* xGroup = new svm_node[sizeX];
      svm_node* yGroup = new svm_node[sizeY];

      for (j = 0; j < numberOfGroups; ++j)
        {
        xTemp = x;
        index = 0;

        while (xTemp->index != -1)
          {
          xGroup[index].index = xTemp->index;
          yGroup[index].index = xTemp->index;

          if ((xTemp->index < begin[j]) || (xTemp->index > end[j]))
            {
            xGroup[index].value = 0;
            yGroup[index].value = 0;
            }
          else
            {
            xGroup[index].value = xTemp->value;
            yGroup[index].value = 0;
            }
          ++index;
          ++xTemp;
          }

        // value can have different value according to j
        value =  m_CustomLinear(xGroup, yGroup, param);

        total += value;
        }
      delete[] xGroup;
      delete[] yGroup;
      }

    else
      {
      total =  static_cast<double>(numberOfGroups) * m_CustomLinear(x, y, param);
      }

    if (xTemp != NULL) delete xTemp;
    if (yTemp != NULL) delete yTemp;

    return total;
  }

protected:
  GroupedRBFKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  GroupedRBFKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

private:
  CustomLinearKernelFunctor m_CustomLinear;
};

/** \class GroupingAdaptiveKernelFunctor
 * \brief Undocumented
 *
 * \ingroup OTBSVMLearning
 */
class GroupingAdaptiveKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef GroupingAdaptiveKernelFunctor       Self;
  typedef GenericKernelFunctorBase            Superclass;

  GroupingAdaptiveKernelFunctor() : GenericKernelFunctorBase()
  {
    this->SetName("groupingAdaptive");
    this->SetValue<double>("lin_coef", 1.);
    this->SetValue<double>("const_coef", 1.);
  }
  virtual ~GroupingAdaptiveKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter& param) const
  {
    const char * parameters = param.custom;
    const char * twoPoints = ":";
    const char * position = strpbrk (parameters, twoPoints);
    int          twoPointsPosition = 0;
    double       total = 0.;
    double       value = 0.;
    int          numberOfGroups = atoi(parameters);

    twoPointsPosition = position - parameters;

    parameters = parameters + twoPointsPosition + 1;

    int i, j;

    std::vector<int> begin;
    begin.resize(numberOfGroups);
    std::vector<int> end;
    end.resize(numberOfGroups);

    for (i = 0; i < numberOfGroups; ++i)
      {
      begin[i] = atoi(parameters);
      position = strpbrk (parameters, twoPoints);
      twoPointsPosition = position - parameters;
      parameters = parameters + twoPointsPosition + 1;

      end[i] = atoi(parameters);
      parameters = parameters + twoPointsPosition + 1;
      }

    const svm_node * xBuff = x;
    const svm_node * yBuff = y;

    int sizeX = 0;
    int sizeY = 0;

    while (xBuff->index != 1)
      {
      ++sizeX;
      ++xBuff;
      }

    while (yBuff->index != 1)
      {
      ++sizeY;
      ++yBuff;
      }

    const svm_node * xTemp = NULL;
    const svm_node * yTemp = NULL;

    int index = 0;

    if (sizeX && sizeY)
      {
      svm_node* xGroup = new svm_node[sizeX];
      svm_node* yGroup = new svm_node[sizeY];

      for (j = 0; j < numberOfGroups; ++j)
        {
        xTemp = x;
        yTemp = y;
        index = 0;

        while (xTemp->index != 1 && yTemp->index != 1)
          {
          xGroup[index].index = xTemp->index;
          yGroup[index].index = yTemp->index;
          if ((xTemp->index < begin[j]) || (xTemp->index > end[j]))
            {
            xGroup[index].value = 0;
            yGroup[index].value = 0;
            }
          else
            {
            xGroup[index].value = xTemp->value;
            yGroup[index].value = yTemp->value;
            }
          ++index;
          ++xTemp;
          ++yTemp;
          }

        // value can have different value according to j
        value =  vcl_pow(this->GetValue<double>("lin_coef") * dot(xGroup, yGroup) + this->GetValue<double>(
                           "const_coef"), static_cast<double>(param.degree));

        total += value;
        }
      delete[] xGroup;
      delete[] yGroup;
      }

    else if ((sizeX > 0) && (sizeY == 0))
      {
      svm_node* xGroup = new svm_node[sizeX];
      svm_node* yGroup = new svm_node[sizeY];

      for (j = 0; j < numberOfGroups; ++j)
        {
        xTemp = x;
        index = 0;

        while (xTemp->index != 1)
          {
          xGroup[index].index = xTemp->index;
          yGroup[index].index = xTemp->index;

          if ((xTemp->index < begin[j]) || (xTemp->index > end[j]))
            {
            xGroup[index].value = 0;
            yGroup[index].value = 0;
            }
          else
            {
            xGroup[index].value = xTemp->value;
            yGroup[index].value = 0;
            }
          ++index;
          ++xTemp;
          }

        // value can have different value according to j
        value = vcl_pow(this->GetValue<double>("lin_coef") * dot(xGroup, yGroup) + this->GetValue<double>(
                          "const_coef"), static_cast<double>(param.degree));

        total += value;
        }
      delete[] xGroup;
      delete[] yGroup;
      }

    else
      {
      CustomKernelFunctor custom;
      total = static_cast<double>(numberOfGroups) * custom(x, y, param);
      }

    return total;
  }


protected:
  GroupingAdaptiveKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  GroupingAdaptiveKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }

};

}
#endif
