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

#include "otbMath.h"
#include "otbMacro.h"
#include "otbParserX.h"
#include "otbParserXPlugins.h"

namespace otb
{

class ITK_EXPORT ParserXImpl : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef ParserXImpl                   Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(ParserXImpl, itk::LightObject);

  /** Convenient type definitions */
  typedef mup::Value       ValueType;
  typedef mup::IValue      IValueType;
  typedef mup::ParserError ExceptionType;

  /** Initialize user defined constants */
  virtual void InitConst()
  {

    m_MuParserX.DefineConst("log2e", CONST_LOG2E);
    m_MuParserX.DefineConst("log10e", CONST_LOG10E);
    m_MuParserX.DefineConst("ln2", CONST_LN2);
    m_MuParserX.DefineConst("ln10", CONST_LN10);
    m_MuParserX.DefineConst("euler", CONST_EULER);
  }

  /** Initialize user defined functions */
  virtual void InitFun()
  {
    m_MuParserX.DefineFun(new ndvi);
    m_MuParserX.DefineFun(new dotpr);
    m_MuParserX.DefineFun(new bands);
    m_MuParserX.DefineFun(new cat);
    m_MuParserX.DefineFun(new mean);
    m_MuParserX.DefineFun(new var);
    m_MuParserX.DefineFun(new median);
    m_MuParserX.DefineFun(new maj);
    m_MuParserX.DefineFun(new corr);

    m_MuParserX.DefineOprt(new ElementWiseDivision);
    m_MuParserX.DefineOprt(new ElementWiseMultiplication);
    m_MuParserX.DefineOprt(new ElementWisePower);
    m_MuParserX.DefineOprt(new DivisionByScalar);
    m_MuParserX.DefineOprt(new MultiplicationByScalar);
    m_MuParserX.DefineOprt(new PowerByScalar);

    m_MuParserX.DefineFun(new vnorm);
    m_MuParserX.DefineFun(new vmin);
    m_MuParserX.DefineFun(new vmax);
    m_MuParserX.DefineFun(new vect2scal);
    m_MuParserX.DefineFun(new vcos);
    m_MuParserX.DefineFun(new vacos);
    m_MuParserX.DefineFun(new vasin);
    m_MuParserX.DefineFun(new vatan);
    m_MuParserX.DefineFun(new vsin);
    m_MuParserX.DefineFun(new vtan);
    m_MuParserX.DefineFun(new vtanh);
    m_MuParserX.DefineFun(new vsinh);
    m_MuParserX.DefineFun(new vcosh);
    m_MuParserX.DefineFun(new vlog);
    m_MuParserX.DefineFun(new vlog10);
    m_MuParserX.DefineFun(new vabs);
    m_MuParserX.DefineFun(new vexp);
    m_MuParserX.DefineFun(new vsqrt);
  }

  /** Set the expression to be parsed */
  virtual void SetExpr(const std::string& Expression)
  {
    m_MuParserX.SetExpr(Expression);
  }

  /** Trigger the parsing */
  ValueType Eval()
  {
    // ParserX::ValueType result = 0.0;
    ValueType result = 0.0;
    try
    {
      result = m_MuParserX.Eval();
    }
    catch (ExceptionType& e)
    {
      ExceptionHandler(e);
    }
    return result;
  }

  const IValueType& EvalRef()
  {
    try
    {
      return m_MuParserX.Eval();
    }
    catch (ExceptionType& e)
    {
      ExceptionHandler(e);
    }
    return m_NullValue;
  }


  /** Define a variable */
  void DefineVar(const std::string& sName, ValueType* fVar)
  {
    try
    {
      m_MuParserX.DefineVar(sName, mup::Variable(fVar));
    }
    catch (ExceptionType& e)
    {
      ExceptionHandler(e);
    }
  }

  /** Define a constant */
  /*void DefineConst(const std::string &sName, ValueType *fVar)
  {
    try
      {
      m_MuParserX.DefineConst(sName, mup::Variable(fVar));
      }
    catch(ExceptionType &e)
      {
      ExceptionHandler(e);
      }
  }*/

  /** Clear all the defined variables */
  void ClearVar()
  {
    m_MuParserX.ClearVar();
  }

  /** Return the expression to be parsed */
  const std::string& GetExpr() const
  {
    return m_MuParserX.GetExpr();
  }

  /** Return the list of variables */
  const mup::var_maptype& GetVar() const
  {
    return m_MuParserX.GetVar();
  }

  /** Return the list of expression variables (only make sense after having set up an expression) */
  const mup::var_maptype& GetExprVar() const
  {
    try
    {
      return m_MuParserX.GetExprVar();
    }
    catch (ExceptionType& e)
    {
      ExceptionHandler(e);
    }
    return m_MuParserX.GetExprVar();
  }

  /**  Check Expression **/
  bool CheckExpr()
  {
    try
    {
      m_MuParserX.Eval();
    }
    catch (ExceptionType& e)
    {
      ExceptionHandler(e);
      return false;
    }

    return true;
  }

  // Get the map with the functions
  /*ParserX::FunctionMapType GetFunList() const chris
  {
    ParserX::FunctionMapType output;
    const mu::funmap_type& funmap = m_MuParserX.GetFunDef();

    mu::funmap_type::const_iterator funItem;

    for (funItem = funmap.begin(); funItem != funmap.end(); ++funItem)
      {
      output[funItem->first] = funItem->second.GetArgc();
      }
    return output;
  }*/

  /** Convert parser specific exception into itk exception */
  virtual void ExceptionHandler(ExceptionType& e) const
  {
    itkExceptionMacro(<< std::endl
                      << "Message:     " << e.GetMsg() << std::endl
                      << "Formula:     " << e.GetExpr() << std::endl
                      << "Token:       " << e.GetToken() << std::endl
                      << "Position:    " << e.GetPos() << std::endl
                      << std::endl);
    //        << "Errc:        "   << e.GetCode()  << std::endl);
  }

  /** Convert parser specific exception into itk debug macro */
  virtual void ExceptionHandlerDebug(ExceptionType& e)
  {
    ExceptionHandler(e);
  }

protected:
  ParserXImpl()
  {
    m_NullValue = ValueType(0.0);
    InitFun();
    InitConst();
  }

  ~ParserXImpl() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }


private:
  ParserXImpl(const Self&) = delete;
  void operator=(const Self&) = delete;

  mup::ParserX m_MuParserX;

  ValueType m_NullValue;


}; // end class


void ParserX::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


ParserX::ParserX() : m_InternalParserX(ParserXImpl::New())
{
}

ParserX::~ParserX()
{
}

void ParserX::SetExpr(const std::string& Expression)
{
  m_InternalParserX->SetExpr(Expression);
}

ParserX::ValueType ParserX::Eval()
{
  return m_InternalParserX->Eval();
}

const ParserX::IValueType& ParserX::EvalRef()
{
  return m_InternalParserX->EvalRef();
}

void ParserX::DefineVar(const std::string& sName, ValueType* fVar)
{
  m_InternalParserX->DefineVar(sName, fVar);
}


void ParserX::ClearVar()
{
  m_InternalParserX->ClearVar();
}

bool ParserX::CheckExpr()
{
  return m_InternalParserX->CheckExpr();
}


const std::string& ParserX::GetExpr() const
{
  return m_InternalParserX->GetExpr();
}

// Get the map with the variables
const mup::var_maptype& ParserX::GetVar() const
{
  return m_InternalParserX->GetVar();
}


/** Return the list of expression variables (only make sense after having set up an expression) */
const mup::var_maptype& ParserX::GetExprVar() const
{
  return m_InternalParserX->GetExprVar();
}


// Get the map with the functions
/*ParserX::FunctionMapType ParserX::GetFunList() const chris
{
  return m_InternalParserX->GetFunList();
}*/

} // end namespace otb
