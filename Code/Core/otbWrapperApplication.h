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
#ifndef __otbWrapperApplication_h
#define __otbWrapperApplication_h

#include <string>
#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

#include "otbWrapperParameterGroup.h"

// include all parameters type for easy use when defining the application
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperFilenameParameter.h"
#include "otbWrapperInputComplexImageParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class Application
 *  \brief This class represent an application
 *  TODO
 *
 */
class ITK_EXPORT Application: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef Application Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  //itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(Application,itk::Object);

  /** Set the parameter name */
  itkSetStringMacro(Name);

  /** Get the parameter name */
  itkGetStringMacro(Name);

  /** Set the parameter description */
  itkSetStringMacro(Description);

  /** Get the parameter description */
  itkGetStringMacro(Description);

  void Init();

  void UpdateParameters();

  void Execute();

  ParameterGroup* GetParameterList();

  Parameter* GetParameterByKey(std::string parameter);

  //void SetParameterBool(std::string parameter, bool value);
  void SetParameterInt(std::string parameter, int value);
  void SetParameterFloat(std::string parameter, float value);
  void SetParameterString(std::string parameter, std::string value);
  void SetParameterOutputImage(std::string parameter, OutputImageParameter::VectorImageType* value);
  void SetParameterOutputVectorData(std::string parameter, OutputVectorDataParameter::VectorDataType* value);

  int GetParameterInt(std::string parameter);
  float GetParameterFloat(std::string parameter);
  std::string GetParameterString(std::string parameter);
  InputImageParameter::VectorImageType* GetParameterImage(std::string parameter);
  InputComplexImageParameter::VectorImageType* GetParameterComplexImage(std::string parameter);
  InputVectorDataParameter::VectorDataType* GetParameterVectorData(std::string parameter);

protected:
  /** Constructor */
  Application();

  virtual ~Application();

private:
  virtual void DoCreateParameters() = 0;

  virtual void DoUpdateParameters() = 0;

  virtual void DoExecute() = 0;

  Application(const Application &); //purposely not implemented
  void operator =(const Application&); //purposely not implemented

  std::string m_Name;
  std::string m_Description;
  ParameterGroup::Pointer m_ParameterList;

}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperApplication_h_
