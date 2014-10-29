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

namespace otb
{


class bands : public mup::ICallback
  {
public:
    bands():ICallback(mup::cmFUNC, "bands", 2)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      // Get the argument from the argument input vector
      mup::matrix_type a = a_pArg[0]->GetArray();      
      mup::matrix_type b = a_pArg[1]->GetArray();


      int nbrows = b.GetRows();
      int nbcols = b.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        res.At(0,k)=a.At(b.At(0,k).GetInteger()-1); //-1 : to make first band have rank #1 (and not 0)

      // The return value is passed by writing it to the reference ret
      *ret = res;

    }

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

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      mup::matrix_type m1 = a_pArg[0]->GetArray();
      

      int nbrows = m1.GetRows();
      int nbcols = m1.GetCols();

      //matrix_type m3(nbrows,nbcols,0);

      mup::matrix_type res(a_iArgc-1,1,0);

      for (int k=1; k<a_iArgc; ++k)
      {
  
        float sum=0.0;

        mup::matrix_type m2 = a_pArg[k]->GetArray();

        for (int i=0; i<nbrows; i++)
          for (int j=0; j<nbcols; j++)
            sum += m1.At(i,j).GetFloat() * m2.At(i,j).GetFloat();

        
        res.At(k-1,0)=sum;  
      }


      // The return value is passed by writing it to the reference ret
      *ret = res;
    }

    const mup::char_type* GetDesc() const
    {
      return "conv(m1,m2) - A matrix convolution";
    }

    mup::IToken* Clone() const
    {
      return new conv(*this);
    }
  };



class MatDiv : public mup::IOprtBin    
  {
  public:
    MatDiv():IOprtBin(_T("div"), (int)(mup::prMUL_DIV), mup::oaLEFT) 
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int)
    {
      const mup::matrix_type a = a_pArg[0]->GetArray();
      const mup::matrix_type b = a_pArg[1]->GetArray();

      /*if (!arg1->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg1->GetType(), 'f', 1)); 

      if (!arg2->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg2->GetType(), 'f', 2));*/ 
      
      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = a.At(p,k).GetFloat() / b.At(p,k).GetFloat();
          

      // The return value is passed by writing it to the reference ret
      *ret = res; 
    }

    virtual const mup::char_type* GetDesc() const 
    { 
      return _T("x ./ y - Division for noncomplex vectors & matrices"); 
    }
  
    virtual mup::IToken* Clone() const
    { 
      return new MatDiv(*this); 
    }
  };



class MatConv : public mup::IOprtBin    
  {
  public:
    MatConv():IOprtBin(_T("mult"), (int)(mup::prMUL_DIV), mup::oaLEFT) 
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int)
    {
      const mup::matrix_type a = a_pArg[0]->GetArray();
      const mup::matrix_type b = a_pArg[1]->GetArray();

      /*if (!arg1->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg1->GetType(), 'f', 1)); 

      if (!arg2->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg2->GetType(), 'f', 2));*/ 
      
      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = a.At(p,k).GetFloat() * b.At(p,k).GetFloat();
        
      // The return value is passed by writing it to the reference ret
      *ret = res; 
    }

    virtual const mup::char_type* GetDesc() const 
    { 
      return _T("x mult y - Convolution for noncomplex vectors & matrices"); 
    }
  
    virtual mup::IToken* Clone() const
    { 
      return new MatConv(*this); 
    }
  };



class ndvi : public mup::ICallback
  {
public:
    ndvi():ICallback(mup::cmFUNC, "ndvi", 2)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      mup::float_type r = a_pArg[0]->GetFloat();
      
      mup::float_type niri = a_pArg[1]->GetFloat();


     // The return value is passed by writing it to the reference ret
      if ( vcl_abs(r + niri) < 1E-6 )
          *ret = 0.;  
      else
          *ret = (niri-r)/(niri+r);
    }

    const mup::char_type* GetDesc() const
    {
      return "NDVI - Normalized Difference Vegetation Index";
    }

    mup::IToken* Clone() const
    {
      return new ndvi(*this);
    }
  };


//--------------------------------------------------------------------------------------------------------//
class vcos : public mup::ICallback
  {
public:
    vcos():ICallback(mup::cmFUNC, "vcos", 1)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_cos(a.At(p,k).GetFloat());
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }

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

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_sin(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }

    const mup::char_type* GetDesc() const
    {
      return "vsin - Sinus for noncomplex vectors & matrices";
    }

    mup::IToken* Clone() const
    {
      return new vsin(*this);
    }
  };




}//end namespace otb

#endif
