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
#ifndef __otbParserXPlugins_h
#define __otbParserXPlugins_h

#include "itkMacro.h"
#include "otbMath.h"
#include "mpParser.h"
#include <vector>

namespace otb
{


class bands : public mup::ICallback
  {
public:
    bands():ICallback(mup::cmFUNC, "bands", 2)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "bands - A bands selector";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new bands(*this);
    }
  };


class dotpr : public mup::ICallback
  {
public:
    dotpr():ICallback(mup::cmFUNC, "dotpr", -1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "dotpr(m1,m2) - A vector/matrix dot product";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new dotpr(*this);
    }
  };

class ElementWiseDivision : public mup::IOprtBin
  {
  public:
    ElementWiseDivision():IOprtBin(_T("div"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return _T("x div y - Element-wise division (vectors / matrices)");
    }
  
    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new ElementWiseDivision(*this);
    }
  };


class DivisionByScalar : public mup::IOprtBin
  {
  public:
    DivisionByScalar():IOprtBin(_T("dv"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return _T("x dv y - division of vectors / matrices by a scalar");
    }
  
    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new DivisionByScalar(*this);
    }
  };


class ElementWiseMultiplication : public mup::IOprtBin
  {
  public:
    ElementWiseMultiplication():IOprtBin(_T("mult"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return _T("x mult y - Element wise multiplication (vectors / matrices)");
    }
  
    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new ElementWiseMultiplication(*this);
    }
  };


class MultiplicationByScalar : public mup::IOprtBin
  {
  public:
    MultiplicationByScalar():IOprtBin(_T("mlt"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return _T("x mlt y - multiplication of vectors / matrices by a scalar");
    }
  
    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new MultiplicationByScalar(*this);
    }
  };


class ElementWisePower : public mup::IOprtBin
  {
public:
    ElementWisePower():IOprtBin(_T("pow"), (int) mup::prPOW, mup::oaRIGHT)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return _T("pow - Power for noncomplex vectors & matrices");
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new ElementWisePower(*this);
    }
  };


class PowerByScalar : public mup::IOprtBin
  {
  public:
    PowerByScalar():IOprtBin(_T("pw"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return _T("x pw y - power of vectors / matrices by a scalar");
    }
  
    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new PowerByScalar(*this);
    }
  };


class ndvi : public mup::ICallback
  {
public:
    ndvi():ICallback(mup::cmFUNC, "ndvi", 2)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "NDVI - Normalized Difference Vegetation Index";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new ndvi(*this);
    }
  };


class cat : public mup::ICallback
  {
public:
    cat():ICallback(mup::cmFUNC, "cat", -1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "cat(m1,m2) - Values concatenation";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new cat(*this);
    }
  };


class mean : public mup::ICallback
  {
public:
    mean():ICallback(mup::cmFUNC, "mean", -1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "mean(m1,m2,..) - mean of each neighborhood";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new mean(*this);
    }
  };


class var : public mup::ICallback
  {
public:
    var():ICallback(mup::cmFUNC, "var", -1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "var(m1,m2,..) - variance of each neighborhood";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new var(*this);
    }
  };


class corr : public mup::ICallback
  {
public:
    corr():ICallback(mup::cmFUNC, "corr", 2)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "corr(m1,m2) - variance of two variables m1 and m2";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new corr(*this);
    }
  };


class median : public mup::ICallback
  {
public:
    median():ICallback(mup::cmFUNC, "median", -1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "median(m1,m2,..) - median value of each neighborhood";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new median(*this);
    }
  };


class maj : public mup::ICallback
  {
public:
    maj():ICallback(mup::cmFUNC, "maj", -1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "maj(m1,m2,..) - majority value of each neighborhood";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new maj(*this);
    }
  };

//--------------------------------------------------------------------------------------------------------//
class vnorm : public mup::ICallback
  {
public:
    vnorm():ICallback(mup::cmFUNC, "vnorm", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vnorm(v1) - Norm for a vector : sqrt(sum of squared elements); works also with matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vnorm(*this);
    }
  };

class vmin : public mup::ICallback
  {
public:
    vmin():ICallback(mup::cmFUNC, "vmin", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vmin(m1) - overall minimun";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vmin(*this);
    }
  };


class vmax : public mup::ICallback
  {
public:
    vmax():ICallback(mup::cmFUNC, "vmax", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vmax(m1) - overall maximun";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vmax(*this);
    }
  };


class vect2scal : public mup::ICallback
  {
public:
    vect2scal():ICallback(mup::cmFUNC, "vect2scal", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vect2scal - Convert one dimensional vector to scalar";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vect2scal(*this);
    }
  };
  
  

class vcos : public mup::ICallback
  {
public:
    vcos():ICallback(mup::cmFUNC, "vcos", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vcos - Cosinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vcos(*this);
    }
  };


class vacos : public mup::ICallback
  {
public:
    vacos():ICallback(mup::cmFUNC, "vacos", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vacos - Arccosinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vacos(*this);
    }
  };

class vsin : public mup::ICallback
  {
public:
    vsin():ICallback(mup::cmFUNC, "vsin", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vsin - Sinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vsin(*this);
    }
  };

class vasin : public mup::ICallback
  {
public:
    vasin():ICallback(mup::cmFUNC, "vasin", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vasin - Arcsinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vasin(*this);
    }
  };


class vtan : public mup::ICallback
  {
public:
    vtan():ICallback(mup::cmFUNC, "vtan", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vtan - Tangent for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vtan(*this);
    }
  };


class vatan : public mup::ICallback
  {
public:
    vatan():ICallback(mup::cmFUNC, "vatan", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vatan - Arctangent for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vatan(*this);
    }
  };
  

class vtanh : public mup::ICallback
  {
public:
    vtanh():ICallback(mup::cmFUNC, "vtanh", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vtanh - Hyperbolic tangent for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vtanh(*this);
    }
  };


class vsinh : public mup::ICallback
  {
public:
    vsinh():ICallback(mup::cmFUNC, "vsinh", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vsinh - Hyperbolic sinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vsinh(*this);
    }
  };


class vcosh : public mup::ICallback
  {
public:
    vcosh():ICallback(mup::cmFUNC, "vcosh", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vcosh - Hyperbolic cosinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vcosh(*this);
    }
  };


class vlog : public mup::ICallback
  {
public:
    vlog():ICallback(mup::cmFUNC, "vlog", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vlog - Log for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vlog(*this);
    }
  };


class vlog10 : public mup::ICallback
  {
public:
    vlog10():ICallback(mup::cmFUNC, "vlog10", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vlog10 - Log10 for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vlog10(*this);
    }
  };


class vabs : public mup::ICallback
  {
public:
    vabs():ICallback(mup::cmFUNC, "vabs", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vabs - Absolute value for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vabs(*this);
    }
  };


class vexp : public mup::ICallback
  {
public:
    vexp():ICallback(mup::cmFUNC, "vexp", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vexp - Exponential for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vexp(*this);
    }
  };


class vsqrt : public mup::ICallback
  {
public:
    vsqrt():ICallback(mup::cmFUNC, "vsqrt", 1)
    {}

    void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc)) ITK_OVERRIDE;

    const mup::char_type* GetDesc() const ITK_OVERRIDE
    {
      return "vsqrt - Sqrt for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const ITK_OVERRIDE
    {
      return new vsqrt(*this);
    }
  };


}//end namespace otb

#endif
