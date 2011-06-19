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

  /** Run the application, then writes all the output to disk
   * if they have an associated filename.
   * This is a helper function for wrappers without pipeline support.
   */
  void ExecuteAndWriteOutput();

  /* Get the internal application parameters
   *
   * WARNING: this method may disappear from the API */
  ParameterGroup* GetParameterList();

  /* Get the internal application parameter specified
   *
   * WARNING: this method may disappear from the API */
  Parameter* GetParameterByKey(std::string parameter);

  /* Get the internal application parameter specified
   *
   * WARNING: this method may disappear from the API */
  const Parameter* GetParameterByKey(std::string parameter) const;

  /* Returns the description of a parameter */
  std::string GetParameterName(std::string paramKey);

  /* Returns the description of a parameter */
  std::string GetParameterDescription(std::string paramKey);

  /* Enable the use of an optional parameter. Returns the previous state */
  void EnableParameter(std::string paramKey);

  /* Disable the use of an optional parameter. Returns the previous state  */
  void DisableParameter(std::string paramKey);

  /* Return the enable state of an optional parameter  */
  bool IsParameterEnabled(std::string paramKey) const;

  /* Return true if the specified parameter is mandatory */
  bool IsMandatory(std::string paramKey) const;

  /* Returns true if the parameter has an associated value provided externally
   *  (not automatically computed by the application) */
  bool HasUserValue(std::string paramKey) const;

  /* If a user value was provided clear it and update the other parameters */
  void ClearValue(std::string paramKey);

  /* Returns true if the parameter has an associated value.
   * This value can be an automatically computed value or default value,
   * or a value set externally by user */
  bool HasValue(std::string paramKey) const;

  /* Return the user level of access to a parameter */
  UserLevel GetParameterUserLevel(std::string paramKey) const;

  /* Get the parameter type from its name */
  ParameterType GetParameterType(std::string paramKey) const;

  /* Set an integer value
   *
   * Can be called for types :
   * \li ParameterType_Int
   * \li ParameterType_Float
   * \li ParameterType_Radius
   * \li ParameterType_Choice
   */
  void SetParameterInt(std::string parameter, int value);

  /* Set a floating value
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  void SetParameterFloat(std::string parameter, float value);

  /* Set a string value
   *
   * Can be called for types :
   * \li ParameterType_String
   * \li ParameterType_Filename
   * \li ParameterType_Directory
   * \li ParameterType_Choice
   * \li ParameterType_InputImageParameter
   * \li ParameterType_InputComplexImageParameter
   * \li ParameterType_InputVectorDataParameter
   * \li ParameterType_OutputImageParameter
   * \li ParameterType_OutputVectorDataParameter
   */
  void SetParameterString(std::string parameter, std::string value);

  /* Set an output image value
   *
   * Can be called for types :
   * \li ParameterType_OutputImage
   */
  void SetParameterOutputImage(std::string parameter, VectorImageType* value);

  /* Set an output vector data value
   *
   * Can be called for types :
   * \li ParameterType_OutputVectorData
   */
  void SetParameterOutputVectorData(std::string parameter, VectorDataType* value);

  /* Get an integer parameter value
   *
   * Can be called for types :
   * \li ParameterType_Int
   * \li ParameterType_Float
   * \li ParameterType_Radius
   * \li ParameterType_Choice
   */
  int GetParameterInt(std::string parameter);

  /* Get a floating parameter value
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  float GetParameterFloat(std::string parameter);

  /* Get a string parameter value
   *
   * Can be called for types :
   * \li ParameterType_String
   * \li ParameterType_Filename
   * \li ParameterType_Directory
   * \li ParameterType_InputImage
   * \li ParameterType_InputComplexImage
   * \li ParameterType_InputVectorData
   * \li ParameterType_OutputImage
   * \li ParameterType_OutputVectorData
   */
  std::string GetParameterString(std::string parameter);

  /* Get an image value
   *
   * Can be called for types :
   * \li ParameterType_InputImage
   */
  VectorImageType* GetParameterImage(std::string parameter);

  /* Get a complex image value
   *
   * Can be called for types :
   * \li ParameterType_InputComplexImage
   */
  ComplexVectorImageType* GetParameterComplexImage(std::string parameter);

  /* GetParameterInt
   *
   * Can be called for types :
   * \li ParameterType_InputVectorData
   */
  VectorDataType* GetParameterVectorData(std::string parameter);

  /* Get the list of all parameters
   */
  std::vector<std::string> GetParametersKeys(bool recursive = true);

protected:
  /** Constructor */
  Application();

  /** Destructor */
  virtual ~Application();

  /** Add a new choice value to an existing choice parameter */
  void AddChoice(std::string paramKey, std::string paramName);

  /** Add a new parameter to the parameter group
   * the parent key of paramKey can be the path to a parameter group
   * or the path to a choice value */
  void AddParameter(ParameterType type, std::string paramKey, std::string paramName);

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
