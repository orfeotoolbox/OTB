/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbParserXPlugins_h
#define otbParserXPlugins_h

#include "itkMacro.h"
#include "otbMath.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"
#include "mpParser.h"
#pragma GCC diagnostic pop
#else
#include "mpParser.h"
#endif
#include <vector>

namespace otb
{


class bands : public mup::ICallback
{
public:
  bands() : ICallback(mup::cmFUNC, "bands", 2)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "bands - A bands selector";
  }

  mup::IToken* Clone() const override
  {
    return new bands(*this);
  }
};


class dotpr : public mup::ICallback
{
public:
  dotpr() : ICallback(mup::cmFUNC, "dotpr", -1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "dotpr(m1,m2) - A vector/matrix dot product";
  }

  mup::IToken* Clone() const override
  {
    return new dotpr(*this);
  }
};

class ElementWiseDivision : public mup::IOprtBin
{
public:
  ElementWiseDivision() : IOprtBin(_T("div"), (int)(mup::prMUL_DIV), mup::oaLEFT)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int) override;

  const mup::char_type* GetDesc() const override
  {
    return _T("x div y - Element-wise division (vectors / matrices)");
  }

  mup::IToken* Clone() const override
  {
    return new ElementWiseDivision(*this);
  }
};


class DivisionByScalar : public mup::IOprtBin
{
public:
  DivisionByScalar() : IOprtBin(_T("dv"), (int)(mup::prMUL_DIV), mup::oaLEFT)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int) override;

  const mup::char_type* GetDesc() const override
  {
    return _T("x dv y - division of vectors / matrices by a scalar");
  }

  mup::IToken* Clone() const override
  {
    return new DivisionByScalar(*this);
  }
};


class ElementWiseMultiplication : public mup::IOprtBin
{
public:
  ElementWiseMultiplication() : IOprtBin(_T("mult"), (int)(mup::prMUL_DIV), mup::oaLEFT)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int) override;

  const mup::char_type* GetDesc() const override
  {
    return _T("x mult y - Element wise multiplication (vectors / matrices)");
  }

  mup::IToken* Clone() const override
  {
    return new ElementWiseMultiplication(*this);
  }
};


class MultiplicationByScalar : public mup::IOprtBin
{
public:
  MultiplicationByScalar() : IOprtBin(_T("mlt"), (int)(mup::prMUL_DIV), mup::oaLEFT)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int) override;

  const mup::char_type* GetDesc() const override
  {
    return _T("x mlt y - multiplication of vectors / matrices by a scalar");
  }

  mup::IToken* Clone() const override
  {
    return new MultiplicationByScalar(*this);
  }
};


class ElementWisePower : public mup::IOprtBin
{
public:
  ElementWisePower() : IOprtBin(_T("pow"), (int)mup::prPOW, mup::oaRIGHT)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return _T("pow - Power for noncomplex vectors & matrices");
  }

  mup::IToken* Clone() const override
  {
    return new ElementWisePower(*this);
  }
};


class PowerByScalar : public mup::IOprtBin
{
public:
  PowerByScalar() : IOprtBin(_T("pw"), (int)(mup::prMUL_DIV), mup::oaLEFT)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int) override;

  const mup::char_type* GetDesc() const override
  {
    return _T("x pw y - power of vectors / matrices by a scalar");
  }

  mup::IToken* Clone() const override
  {
    return new PowerByScalar(*this);
  }
};


class ndvi : public mup::ICallback
{
public:
  ndvi() : ICallback(mup::cmFUNC, "ndvi", 2)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "NDVI - Normalized Difference Vegetation Index";
  }

  mup::IToken* Clone() const override
  {
    return new ndvi(*this);
  }
};


class cat : public mup::ICallback
{
public:
  cat() : ICallback(mup::cmFUNC, "cat", -1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "cat(m1,m2) - Values concatenation";
  }

  mup::IToken* Clone() const override
  {
    return new cat(*this);
  }
};


class mean : public mup::ICallback
{
public:
  mean() : ICallback(mup::cmFUNC, "mean", -1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "mean(m1,m2,..) - mean of each neighborhood";
  }

  mup::IToken* Clone() const override
  {
    return new mean(*this);
  }
};


class var : public mup::ICallback
{
public:
  var() : ICallback(mup::cmFUNC, "var", -1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "var(m1,m2,..) - variance of each neighborhood";
  }

  mup::IToken* Clone() const override
  {
    return new var(*this);
  }
};


class corr : public mup::ICallback
{
public:
  corr() : ICallback(mup::cmFUNC, "corr", 2)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "corr(m1,m2) - variance of two variables m1 and m2";
  }

  mup::IToken* Clone() const override
  {
    return new corr(*this);
  }
};


class median : public mup::ICallback
{
public:
  median() : ICallback(mup::cmFUNC, "median", -1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "median(m1,m2,..) - median value of each neighborhood";
  }

  mup::IToken* Clone() const override
  {
    return new median(*this);
  }
};


class maj : public mup::ICallback
{
public:
  maj() : ICallback(mup::cmFUNC, "maj", -1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "maj(m1,m2,..) - majority value of each neighborhood";
  }

  mup::IToken* Clone() const override
  {
    return new maj(*this);
  }
};

//--------------------------------------------------------------------------------------------------------//
class vnorm : public mup::ICallback
{
public:
  vnorm() : ICallback(mup::cmFUNC, "vnorm", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int a_iArgc) override;

  const mup::char_type* GetDesc() const override
  {
    return "vnorm(v1) - Norm for a vector : sqrt(sum of squared elements); works also with matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vnorm(*this);
  }
};

class vmin : public mup::ICallback
{
public:
  vmin() : ICallback(mup::cmFUNC, "vmin", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vmin(m1) - overall minimum";
  }

  mup::IToken* Clone() const override
  {
    return new vmin(*this);
  }
};


class vmax : public mup::ICallback
{
public:
  vmax() : ICallback(mup::cmFUNC, "vmax", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vmax(m1) - overall maximun";
  }

  mup::IToken* Clone() const override
  {
    return new vmax(*this);
  }
};


class vect2scal : public mup::ICallback
{
public:
  vect2scal() : ICallback(mup::cmFUNC, "vect2scal", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vect2scal - Convert one dimensional vector to scalar";
  }

  mup::IToken* Clone() const override
  {
    return new vect2scal(*this);
  }
};


class vcos : public mup::ICallback
{
public:
  vcos() : ICallback(mup::cmFUNC, "vcos", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vcos - Cosinus for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vcos(*this);
  }
};


class vacos : public mup::ICallback
{
public:
  vacos() : ICallback(mup::cmFUNC, "vacos", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vacos - Arccosinus for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vacos(*this);
  }
};

class vsin : public mup::ICallback
{
public:
  vsin() : ICallback(mup::cmFUNC, "vsin", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vsin - Sinus for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vsin(*this);
  }
};

class vasin : public mup::ICallback
{
public:
  vasin() : ICallback(mup::cmFUNC, "vasin", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vasin - Arcsinus for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vasin(*this);
  }
};


class vtan : public mup::ICallback
{
public:
  vtan() : ICallback(mup::cmFUNC, "vtan", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vtan - Tangent for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vtan(*this);
  }
};


class vatan : public mup::ICallback
{
public:
  vatan() : ICallback(mup::cmFUNC, "vatan", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vatan - Arctangent for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vatan(*this);
  }
};


class vtanh : public mup::ICallback
{
public:
  vtanh() : ICallback(mup::cmFUNC, "vtanh", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vtanh - Hyperbolic tangent for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vtanh(*this);
  }
};


class vsinh : public mup::ICallback
{
public:
  vsinh() : ICallback(mup::cmFUNC, "vsinh", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vsinh - Hyperbolic sinus for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vsinh(*this);
  }
};


class vcosh : public mup::ICallback
{
public:
  vcosh() : ICallback(mup::cmFUNC, "vcosh", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vcosh - Hyperbolic cosinus for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vcosh(*this);
  }
};


class vlog : public mup::ICallback
{
public:
  vlog() : ICallback(mup::cmFUNC, "vlog", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vlog - Log for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vlog(*this);
  }
};


class vlog10 : public mup::ICallback
{
public:
  vlog10() : ICallback(mup::cmFUNC, "vlog10", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vlog10 - Log10 for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vlog10(*this);
  }
};


class vabs : public mup::ICallback
{
public:
  vabs() : ICallback(mup::cmFUNC, "vabs", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vabs - Absolute value for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vabs(*this);
  }
};


class vexp : public mup::ICallback
{
public:
  vexp() : ICallback(mup::cmFUNC, "vexp", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vexp - Exponential for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vexp(*this);
  }
};


class vsqrt : public mup::ICallback
{
public:
  vsqrt() : ICallback(mup::cmFUNC, "vsqrt", 1)
  {
  }

  void Eval(mup::ptr_val_type& ret, const mup::ptr_val_type* a_pArg, int itkNotUsed(a_iArgc)) override;

  const mup::char_type* GetDesc() const override
  {
    return "vsqrt - Sqrt for noncomplex vectors & matrices";
  }

  mup::IToken* Clone() const override
  {
    return new vsqrt(*this);
  }
};


} // end namespace otb

#endif
