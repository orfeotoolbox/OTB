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
#ifndef __otbParser_h
#define __otbParser_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"

#include "muParser.h" //TODO a inclure uniquement dans le cxx


namespace otb
{

class ITK_EXPORT Parser : public itk::LightObject
{
public:
  typedef Parser                                   Self;
  typedef itk::LightObject                         Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(Parser, itk::LightObject);

  typedef Parser                                   ParserType;
  typedef double                                   ValueType;

  virtual void InitConst();
  virtual void InitFun();
  virtual void SetExpr(const std::string & Expression);
  ValueType Eval();
  void DefineVar(const std::string &sName, ValueType *fVar);
  void ClearVar();



  typedef mu::Parser::exception_type               ExceptionType;
  virtual void ExceptionHandler(ExceptionType &e);

  
protected:
  Parser();
  virtual ~Parser() {}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  Parser(const Self &);             //purposely not implemented
  void operator =(const Self &);    //purposely not implemented

  mu::Parser                 m_Parser;
  
  //----------  User Defined Functions  ----------//BEGIN

  static ValueType NDVI(ValueType, ValueType); // NDVI(r, niri)

  //----------  User Defined Functions  ----------//END



};// end class

}//end namespace otb

#endif
