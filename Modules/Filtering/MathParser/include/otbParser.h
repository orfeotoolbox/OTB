/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbParser_h
#define otbParser_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"

#include "otb_muparser.h"
#include "OTBMathParserExport.h"

namespace otb
{

class ParserImpl;


/** \class Parser
 * \brief  Definition of the standard floating point parser.
 * Standard implementation of the mathematical expressions parser.
 *
 * \sa BandMathImageFilter
 *
 *
 * \ingroup OTBMathParser
 */
class OTBMathParser_EXPORT Parser : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef Parser                                   Self;
  typedef itk::LightObject                         Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro(Parser, itk::LightObject);

  /** Convenient type definitions */
  typedef Parser                                   ParserType;
  typedef double                                   ValueType;

  /** Type for function/number of arguments map */
  typedef std::map<std::string, int>               FunctionMapType;

  /** Set the expression to be parsed */
  virtual void SetExpr(const std::string & Expression);

  /** Trigger the parsing */
  ValueType Eval();

  /** Define a variable */
  void DefineVar(const std::string &sName, ValueType *fVar);

  /** Clear all the defined variables */
  void ClearVar();

  /** Return the expression to be parsed */
  const std::string& GetExpr() const;

  /** Return the list of variables */
  const std::map<std::string, Parser::ValueType*>& GetVar() const;

  /** Return a map of function names and associated number of arguments */
  FunctionMapType GetFunList() const;

  /**  Check Expression **/
  bool CheckExpr();

protected:
  Parser();
  ~Parser() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  Parser(const Self &) = delete;
  void operator =(const Self &) = delete;

  typedef itk::SmartPointer<ParserImpl> ParserImplPtr;
  ParserImplPtr m_InternalParser;
}; // end class

}//end namespace otb

#endif
