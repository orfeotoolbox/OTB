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

#include "otbWrapperTypes.h"
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

  /** Initialize the application, instanciating the parameter list */
  void Init();

  /** Update the value of parameters for which no user value has been provided */
  void UpdateParameters();

  /** Run the application.
   *
   * For pipeline ready application, this only wire
   * and configure the pipeline, and provides
   * the output image or vector data parameters as pointers.
   *
   * In other cases, the application must handle
   * the I/O (intermediary results for example) */
  void Execute();

  /* Get the internal application parameters
   *
   * WARNING: this method may disappear from the API */
  ParameterGroup* GetParameterList();

  /* Get the internal application parameter specified
   *
   * WARNING: this method may disappear from the API */
  Parameter* GetParameterByKey(std::string parameter);

  /* Enable the use of an optional parameter. Returns the previous state */
  bool EnableParameter(std::string paramKey);

  /* Disable the use of an optional parameter. Returns the previous state  */
  bool DisableParameter(std::string paramKey);

  /* Return the enable state of an optional parameter  */
  bool IsParameterEnabled(std::string paramKey) const;

  /* Return true if the specified parameter is mandatory */
  bool IsMandatory(std::string paramKey) const;

  /* Returns true if the parameter has an associated value provided externally
   *  (not automatically computed by the application) */
  bool HasUserValue(std::string paramKey) const;

  /* Returns true if the parameter has an associated value.
   * This value can be an automatically computed value or default value,
   * or a value set externally by user */
  bool HasValue(std::string paramKey) const;

  /* Return the user level of access to a parameter */
  UserLevel GetParameterUserLevel(std::string paramKey) const;

  /* Get the parameter type from its name */
  ParameterType GetParameterType(std::string paramKey) const;

  void SetParameterInt(std::string parameter, int value);
  void SetParameterFloat(std::string parameter, float value);
  void SetParameterString(std::string parameter, std::string value);
  void SetParameterOutputImage(std::string parameter, VectorImageType* value);
  void SetParameterOutputVectorData(std::string parameter, VectorDataType* value);

  int GetParameterInt(std::string parameter);
  float GetParameterFloat(std::string parameter);
  std::string GetParameterString(std::string parameter);
  VectorImageType* GetParameterImage(std::string parameter);
  ComplexVectorImageType* GetParameterComplexImage(std::string parameter);
  VectorDataType* GetParameterVectorData(std::string parameter);


protected:
  /** Constructor */
  Application();

  /** Destructor */
  virtual ~Application();

  /** Add a new parameter to the parameter group */
  void AddParameter(ParameterType type, std::string paramKey, std::string paramName);

  /** Add a new subgroup to an existing group */
  void AddParameterGroup(std::string paramKey, std::string paramName);

  /** Declare a parameter as mandatory */
  void MandatoryOn(std::string paramKey);

  /** Declare a parameter as NOT mandatory (default state) */
  void MandatoryOff(std::string paramKey);

  /* Set the user level of access to a parameter */
  void SetParameterUserLevel(std::string paramKey, UserLevel level);

private:
  /* Implement this method to add parameters */
  virtual void DoCreateParameters() = 0;

  /* Implement this method to update non valued parameters */
  virtual void DoUpdateParameters() = 0;

  /* Implement this method to build the output */
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
