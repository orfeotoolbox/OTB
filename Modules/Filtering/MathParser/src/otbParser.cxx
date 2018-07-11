/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include "otbParser.h"

#include "otb_muparser.h"

namespace otb
{

class ITK_EXPORT ParserImpl : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef ParserImpl                               Self;
  typedef itk::LightObject                         Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(ParserImpl, itk::LightObject);

  /** Convenient type definitions */
  typedef double                                   ValueType;
  typedef mu::Parser::exception_type               ExceptionType;

  /** Initialize user defined constants */
  virtual void InitConst()
  {
    m_MuParser.DefineConst( "e",      CONST_E );
    m_MuParser.DefineConst( "log2e",  CONST_LOG2E );
    m_MuParser.DefineConst( "log10e", CONST_LOG10E );
    m_MuParser.DefineConst( "ln2",    CONST_LN2 );
    m_MuParser.DefineConst( "ln10",   CONST_LN10 );
    m_MuParser.DefineConst( "pi",     CONST_PI );
    m_MuParser.DefineConst( "euler",  CONST_EULER );
  }

  /** Initialize user defined functions */
  virtual void InitFun()
  {
    m_MuParser.DefineFun("ndvi", Self::NDVI);
    m_MuParser.DefineFun("NDVI", Self::NDVI);
    m_MuParser.DefineFun("atan2", Self::ATAN2);

#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
    /* Starting with muParser 2.0.0, logical operators have been
       renamed to "&&" and "||", instead of "and" and "or".
       Reintroducing pre-2.0.0 operators, so formulas can be
       defined with "and" and "or" even after 2.0.0
     */
    m_MuParser.DefineOprt("and", Self::AND);
    m_MuParser.DefineOprt("or",  Self::OR);
#endif
  }

  /** Set the expression to be parsed */
  virtual void SetExpr(const std::string & Expression)
  {
    m_MuParser.SetExpr(Expression);
  }

  /** Trigger the parsing */
  ValueType Eval()
  {
    Parser::ValueType result = 0.0;
    try
      {
      result = m_MuParser.Eval();
      }
    catch(ExceptionType &e)
      {
      ExceptionHandler(e);
      }
    return result;
  }


  /** Define a variable */
  void DefineVar(const std::string &sName, ValueType *fVar)
  {
    try
      {
      m_MuParser.DefineVar(sName, fVar);
      }
    catch(ExceptionType &e)
      {
      ExceptionHandler(e);
      }
  }

  /** Clear all the defined variables */
  void ClearVar()
  {
    m_MuParser.ClearVar();
  }

  /** Return the expression to be parsed */
  const std::string& GetExpr() const
  {
    return m_MuParser.GetExpr();
  }

  /** Return the list of variables */
  const std::map<std::string, ValueType*>& GetVar() const
  {
    return m_MuParser.GetVar();
  }

  /**  Check Expression **/
  bool CheckExpr()
  {
    try
    {
      m_MuParser.Eval();
    }
   catch(ExceptionType &e)
    {
     ExceptionHandler(e);
     return false;
    }

    return true;
  }

  // Get the map with the functions
  Parser::FunctionMapType GetFunList() const
  {
    Parser::FunctionMapType output;
    const mu::funmap_type& funmap = m_MuParser.GetFunDef();

    mu::funmap_type::const_iterator funItem;

    for (funItem = funmap.begin(); funItem != funmap.end(); ++funItem)
      {
      output[funItem->first] = funItem->second.GetArgc();
      }
    return output;
  }

  /** Convert parser specific exception into itk exception */
  virtual void ExceptionHandler(ExceptionType &e)
  {
    itkExceptionMacro(                                     << std::endl
          << "Message:     "   << e.GetMsg()   << std::endl
          << "Formula:     "   << e.GetExpr()  << std::endl
          << "Token:       "   << e.GetToken() << std::endl
          << "Position:    "   << e.GetPos()   << std::endl
                 << std::endl);
  //        << "Errc:        "   << e.GetCode()  << std::endl);
  }

  /** Convert parser specific exception into itk debug macro */
   virtual void ExceptionHandlerDebug(ExceptionType &e)
   {
     ExceptionHandler(e);
   }

protected:
  ParserImpl()
  {
    InitFun();
    InitConst();
  }

  ~ParserImpl() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }


private:
  ParserImpl(const Self &) = delete;
  void operator =(const Self &) = delete;

  mu::Parser m_MuParser;

  //----------  User Defined Functions  ----------//BEGIN
  static ValueType NDVI(ValueType r, ValueType niri)
  {
    if ( std::abs(r + niri) < 1E-6 )
      {
      return 0.;
      }
    return (niri-r)/(niri+r);
  }
  
  static ValueType ATAN2(ValueType y, ValueType x)
  {
    return std::atan2(y,x);
  }

#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
  static ValueType AND(ValueType left, ValueType right)
  {
    return static_cast<int>(left) && static_cast<int>(right);
  }

  static ValueType OR(ValueType left, ValueType right)
  {
    return static_cast<int>(left) || static_cast<int>(right);
  }
#endif
  //----------  User Defined Functions  ----------//END
}; // end class

void Parser::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


Parser::Parser()
: m_InternalParser( ParserImpl::New() )
{
}

Parser::~Parser()
{
}

void Parser::SetExpr(const std::string & Expression)
{
  m_InternalParser->SetExpr(Expression);
}

Parser::ValueType Parser::Eval()
{
  return m_InternalParser->Eval();
}

void Parser::DefineVar(const std::string &sName, Parser::ValueType *fVar)
{
  m_InternalParser->DefineVar(sName, fVar);
}

void Parser::ClearVar()
{
  m_InternalParser->ClearVar();
}

bool Parser::CheckExpr()
{
  return m_InternalParser->CheckExpr();
}


const std::string& Parser::GetExpr() const
{
  return m_InternalParser->GetExpr();
}

// Get the map with the variables
const std::map<std::string, Parser::ValueType*>& Parser::GetVar() const
{
  return m_InternalParser->GetVar();
}

// Get the map with the functions
Parser::FunctionMapType Parser::GetFunList() const
{
  return m_InternalParser->GetFunList();
}

}//end namespace otb
