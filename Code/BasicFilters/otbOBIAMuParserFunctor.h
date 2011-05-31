/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.

 Some parts of this code are derived from ITK. See ITKCopyright.txt
 for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#ifndef __otbOBIAMuParserFunctor_h
#define __otbOBIAMuParserFunctor_h

#include "otbParser.h"
#include "otbMacro.h"
#include "itkMacro.h"


#include "itkLightObject.h"
#include "itkObjectFactory.h"


namespace otb
{
/** \class OBIAMuParserFunctor
 * \brief use MuParser criteria to accept/reject LabelObject given his shape and radiometrics statistics.
 * This functor is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 *
 * OTB additional functions:
 * ndvi(r, niri)
 *
 * OTB additional constants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 * \sa Parser
 *
 */

/** \class OBIAMuParserFunctor
 *  \brief This functor use MuParser to evaluate and process mathematical expression
 */
namespace Functor
{

template<class TLabelObject>
//class ITK_EXPORT OBIAMuParserFunctor : public itk::LightObject
class ITK_EXPORT OBIAMuParserFunctor
{
public:
  typedef Parser ParserType;
  typedef OBIAMuParserFunctor Self;

  std::string GetNameOfClass()
  {
    return "OBIAMuParserFunctor";
  }

  inline bool operator()(const TLabelObject &a)
  {

    double value;

    if(a.GetNumberOfAttributes()!=m_NbOfAttributes)
      {
      this->SetAttributes(a);
      }

    // we fill the buffer

    for(unsigned int i=0; i<m_NbOfAttributes; i++)
      {
       m_AAttributes[i]= a.GetAttribute((m_AttributesName.at(i)).c_str());
      }


    // cast
    try
    {
      value = m_Parser->Eval();
    }
    catch(itk::ExceptionObject& err)
    {
      itkExceptionMacro(<< err);
    }
    return static_cast<bool> (value);

  }

  void SetExpression(const std::string expression)
  {
    m_Expression=expression;
    m_Parser->SetExpr(m_Expression);
  }

  /** Return the expression to be parsed */
  std::string GetExpression() const
  {
    return m_Expression;
  }

  void ParseAttributeName(std::string &attributeName)
  {

    for(unsigned int i=0; i<attributeName.size(); i++)
      {
      if(attributeName[i]==':')
        {
        attributeName.erase(i, 1);
        attributeName[i]='_';
        }
      }
    // TODO JGU
    // replace "Band" by "b" for homogeneity with other functors

  }


  void SetAttributes(const TLabelObject &a)
  {

    m_NbOfAttributes=a.GetNumberOfAttributes();

    m_AAttributes.resize(m_NbOfAttributes, 0.0);
    m_AttributesName.resize(m_NbOfAttributes,"");
    m_AttributesName = a.GetAvailableAttributes();
    for(unsigned int i=0; i<m_NbOfAttributes; i++)
      {
      std::string attributeName=m_AttributesName.at(i);
      ParseAttributeName(attributeName); //eliminate '::' from string name
      m_Parser->DefineVar(attributeName, &(m_AAttributes[i]));
      }

  }

  void SetAttributes(std::vector<std::string> shapeAttributes, std::vector<std::string> statAttributes, unsigned int nbOfBands)
  {
    int index=0;
    m_NbOfAttributes=shapeAttributes.size()+statAttributes.size()*nbOfBands;

    m_AAttributes.resize(m_NbOfAttributes, 0.0);
    m_AttributesName.resize(m_NbOfAttributes,"");
    std::ostringstream varName;
    for(unsigned int i=0; i<shapeAttributes.size(); i++)
      {

      varName<<"SHAPE::"<<shapeAttributes.at(i);
      m_AttributesName.at(index)=varName.str();
      varName.str("");
      varName<<"SHAPE_"<<shapeAttributes.at(i);

      m_Parser->DefineVar(varName.str(), &(m_AAttributes[index]));
      varName.str("");
      index++;
      }
    for(unsigned int i=0; i<statAttributes.size(); i++)
      {
      for(unsigned int bandIndex=1; bandIndex<=nbOfBands; bandIndex++)
        {
        varName<<"STATS::Band"<<bandIndex<<"::"<<statAttributes.at(i);
        m_AttributesName.at(index)=varName.str();
        varName.str("");
        varName<<"STATS_Band"<< bandIndex <<"_"<<statAttributes.at(i);
        m_Parser->DefineVar(varName.str(), &(m_AAttributes[index]));
        varName.str("");
        index++;
        }
      }
  }


  /** Check the expression */
  bool CheckExpression()
  {
    return m_Parser->CheckExpr();
  }

 const std::map<std::string, Parser::ValueType*>& GetVar() const
  {
    return this->m_Parser->GetVar();
  }

 const mu::funmap_type& GetFunList() const
  {
    return this->m_Parser->GetFunList();
  }

  OBIAMuParserFunctor()
  {
    m_Parser = ParserType::New();
    m_AAttributes.resize(0);
  };

  ~OBIAMuParserFunctor()
  {};

protected:

private:

  OBIAMuParserFunctor(const Self &); //purposely not implemented
  void operator =(const Self &);    //purposely not implemented

  std::string m_Expression;
  ParserType::Pointer m_Parser;
  std::vector<double> m_AAttributes;
  std::vector<std::string> m_AttributesName;
  unsigned int m_NbOfAttributes;
  double m_ParserResult;

};
} // end of Functor namespace


}//end namespace otb


#endif
