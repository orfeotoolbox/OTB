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

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "bands - A bands selector";
    }

    mup::IToken* Clone() const
    {
      return new bands(*this);
    }
  };


class conv : public mup::ICallback
  {
public:
    conv():ICallback(mup::cmFUNC, "conv", -1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "conv(m1,m2) - A matrix convolution";
    }

    mup::IToken* Clone() const
    {
      return new conv(*this);
    }
  };


class ElementWiseDivision : public mup::IOprtBin
  {
  public:
    ElementWiseDivision():IOprtBin(_T("div"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int);

    virtual const mup::char_type* GetDesc() const
    {
      return _T("x div y - Element-wise division (vectors / matrices)");
    }
  
    virtual mup::IToken* Clone() const
    {
      return new ElementWiseDivision(*this);
    }
  };


class DivisionByScalar : public mup::IOprtBin
  {
  public:
    DivisionByScalar():IOprtBin(_T("dv"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int);

    virtual const mup::char_type* GetDesc() const
    {
      return _T("x dv y - division of vectors / matrices by a scalar");
    }
  
    virtual mup::IToken* Clone() const
    {
      return new DivisionByScalar(*this);
    }
  };


class ElementWiseMultiplication : public mup::IOprtBin
  {
  public:
    ElementWiseMultiplication():IOprtBin(_T("mult"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int);

    virtual const mup::char_type* GetDesc() const
    {
      return _T("x mult y - Element wise multiplication (vectors / matrices)");
    }
  
    virtual mup::IToken* Clone() const
    {
      return new ElementWiseMultiplication(*this);
    }
  };


class MultiplicationByScalar : public mup::IOprtBin
  {
  public:
    MultiplicationByScalar():IOprtBin(_T("mlt"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int);

    virtual const mup::char_type* GetDesc() const
    {
      return _T("x mlt y - multiplication of vectors / matrices by a scalar");
    }
  
    virtual mup::IToken* Clone() const
    {
      return new MultiplicationByScalar(*this);
    }
  };


class ElementWisePower : public mup::IOprtBin
  {
public:
    ElementWisePower():IOprtBin(_T("pow"), (int) mup::prPOW, mup::oaRIGHT)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return _T("pow - Power for noncomplex vectors & matrices");
    }

    virtual mup::IToken* Clone() const
    {
      return new ElementWisePower(*this);
    }
  };


class PowerByScalar : public mup::IOprtBin
  {
  public:
    PowerByScalar():IOprtBin(_T("pw"), (int)(mup::prMUL_DIV), mup::oaLEFT)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int);

    virtual const mup::char_type* GetDesc() const
    {
      return _T("x pw y - power of vectors / matrices by a scalar");
    }
  
    virtual mup::IToken* Clone() const
    {
      return new PowerByScalar(*this);
    }
  };


class ndvi : public mup::ICallback
  {
public:
    ndvi():ICallback(mup::cmFUNC, "ndvi", 2)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "NDVI - Normalized Difference Vegetation Index";
    }

    mup::IToken* Clone() const
    {
      return new ndvi(*this);
    }
  };


class cat : public mup::ICallback
  {
public:
    cat():ICallback(mup::cmFUNC, "cat", -1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "cat(m1,m2) - Values concatenation";
    }

    mup::IToken* Clone() const
    {
      return new cat(*this);
    }
  };


class mean : public mup::ICallback
  {
public:
    mean():ICallback(mup::cmFUNC, "mean", -1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "mean(m1,m2,..) - Mean";
    }

    mup::IToken* Clone() const
    {
      return new mean(*this);
    }
  };


class var : public mup::ICallback
  {
public:
    var():ICallback(mup::cmFUNC, "var", -1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "var(m1,m2,..) - Variance";
    }

    mup::IToken* Clone() const
    {
      return new var(*this);
    }
  };


class median : public mup::ICallback
  {
public:
    median():ICallback(mup::cmFUNC, "median", -1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "median(m1,m2,..) - median";
    }

    mup::IToken* Clone() const
    {
      return new median(*this);
    }
  };


class vmin : public mup::ICallback
  {
public:
    vmin():ICallback(mup::cmFUNC, "vmin", -1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vmin(m1,m2,..) - overall minimun";
    }

    mup::IToken* Clone() const
    {
      return new vmin(*this);
    }
  };


class vmax : public mup::ICallback
  {
public:
    vmax():ICallback(mup::cmFUNC, "vmax", -1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vmax(m1,m2,..) - overall maximun";
    }

    mup::IToken* Clone() const
    {
      return new vmax(*this);
    }
  };

//--------------------------------------------------------------------------------------------------------//
class vcos : public mup::ICallback
  {
public:
    vcos():ICallback(mup::cmFUNC, "vcos", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vcos - Cosinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vcos(*this);
    }
  };


class vsin : public mup::ICallback
  {
public:
    vsin():ICallback(mup::cmFUNC, "vsin", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vsin - Sinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vsin(*this);
    }
  };


class vtan : public mup::ICallback
  {
public:
    vtan():ICallback(mup::cmFUNC, "vtan", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vtan - Tangent for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vtan(*this);
    }
  };


class vtanh : public mup::ICallback
  {
public:
    vtanh():ICallback(mup::cmFUNC, "vtanh", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vtanh - Hyperbolic tangent for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vtanh(*this);
    }
  };


class vsinh : public mup::ICallback
  {
public:
    vsinh():ICallback(mup::cmFUNC, "vsinh", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vsinh - Hyperbolic sinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vsinh(*this);
    }
  };


class vcosh : public mup::ICallback
  {
public:
    vcosh():ICallback(mup::cmFUNC, "vcosh", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vcosh - Hyperbolic cosinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vcosh(*this);
    }
  };


class vlog : public mup::ICallback
  {
public:
    vlog():ICallback(mup::cmFUNC, "vlog", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vlog - Log for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vlog(*this);
    }
  };


class vlog10 : public mup::ICallback
  {
public:
    vlog10():ICallback(mup::cmFUNC, "vlog10", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vlog10 - Log10 for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vlog10(*this);
    }
  };


class vabs : public mup::ICallback
  {
public:
    vabs():ICallback(mup::cmFUNC, "vabs", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vabs - Absolute value for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vabs(*this);
    }
  };


class vexp : public mup::ICallback
  {
public:
    vexp():ICallback(mup::cmFUNC, "vexp", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vexp - Exponential for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vexp(*this);
    }
  };


class vsqrt : public mup::ICallback
  {
public:
    vsqrt():ICallback(mup::cmFUNC, "vsqrt", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc);

    const mup::char_type* GetDesc() const
    {
      return "vsqrt - Sqrt for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vsqrt(*this);
    }
  };


}//end namespace otb

#endif
