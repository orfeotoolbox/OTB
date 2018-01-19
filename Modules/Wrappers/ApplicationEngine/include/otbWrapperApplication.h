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

#ifndef otbWrapperApplication_h
#define otbWrapperApplication_h

#include <string>
#include "otbWrapperTypes.h"
#include "otbWrapperTags.h"
#include "otbWrapperParameterGroup.h"

#include "otbLogger.h"
#include "itkTimeProbe.h"
#include "otbWrapperMacros.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperDocExampleStructure.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{

/** \class ApplicationException
 *  \brief Exception for runtime errors in OTB Applications
 *
 *  Usually thrown with the otbAppLogFATAL macro
 *
 * \ingroup OTBApplicationEngine
 */
class ApplicationException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro( ApplicationException, ExceptionObject );

  /** Constructor. */
  ApplicationException(const char *file, unsigned int line,
                       const char* message = "Application error.",
                       const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc)
  {
  }

  /** Constructor. */
  ApplicationException(const std::string &file, unsigned int line,
                       const char* message = "Application error.",
                       const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc)
  {
  }
};

namespace Wrapper
{

/** \class Application
 *  \brief %This class represent an application.
 *  
 * \par
 * %This class is an abstract class that is used to set a framework
 * for the different applications that will derived from it.
 * It defines the basic actions that one needs to create or use an application,
 * mainly through parameter manipulation. See the Parameter class 
 * documentation for more information.
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT Application: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef Application                   Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** RTTI support */
  itkTypeMacro(Application, itk::Object);


  //itkSetStringMacro(Name);
  /** Set the application name */
  virtual void SetName( const std::string & name )
  {
    m_Name = name;
    GetDocExample()->SetApplicationName(name);
    this->Modified();
    m_Logger->SetName(name);
  }

  /** Get the application name */
  itkGetStringMacro(Name);

  /** Set the application description */
  itkSetStringMacro(Description);

  /** Get the application description */
  itkGetStringMacro(Description);

  /** Initialize the application, instantiating the parameter list */
  void Init();

  /** Check if the application has been initialized */
  bool IsInitialized() const;

  /** Set the parameter xml flag */
  itkSetMacro(HaveInXML, bool);

  /** Get the parameter xml flag */
  itkGetConstMacro(HaveInXML, bool);

  /** Set the parameter xml flag */
  itkSetMacro(HaveOutXML, bool);

  /** Get the parameter xml flag */
  itkGetConstMacro(HaveOutXML, bool);

  /** Update the value of parameters for which no user value has been provided */
  void UpdateParameters();

  /** Run the application.
   *
   * For pipeline ready application, this only wire
   * and configure the pipeline, and provides
   * the output image or vector data parameters as pointers.
   *
   * In other cases, the application must handle
   * the I/O (intermediary results for example)
   *
   * Returns 0 on success, or a non-null integer on error
   */
  int Execute();

  /** Run the application, then writes all the output to disk
   * if they have an associated filename.
   * This is a helper function for wrappers without pipeline support.
   *
   * Returns 0 on success, or a non-null integer on error
   */
  int ExecuteAndWriteOutput();

  /** Get the internal application parameters
   *
   * WARNING: this method may disappear from the API */
  ParameterGroup* GetParameterList();

  /** Get the internal application parameter specified.
   * If the follow flag is on, the function returns the target 
   * of proxy parameters, default is on.
   * 
   * WARNING: this method may disappear from the API */
  Parameter* GetParameterByKey(std::string parameter, bool follow=true);

  /** Get the internal application parameter specified.
   *
   * WARNING: this method may disappear from the API */
  const Parameter* GetParameterByKey(std::string parameter, bool follow=true) const;

  /** Returns the name of a parameter */
  std::string GetParameterName(std::string paramKey);

  /** Returns the description of a parameter */
  std::string GetParameterDescription(std::string paramKey);

  /** Set the description of a parameter */
  void SetParameterDescription(std::string paramKey, std::string dec);

  /** Enable the use of an optional parameter.*/
  void EnableParameter(std::string paramKey);

  /** Disable the use of an optional parameter.*/
  void DisableParameter(std::string paramKey);

  /** Return the enable state of an optional parameter  */
  bool IsParameterEnabled(std::string paramKey, bool recurseParents = false) const;

  /** Return true if the specified parameter is mandatory */
  bool IsMandatory(std::string paramKey) const;

  /** Return true if the specified parameter was set automatically in
   * the application
   */
  bool HasAutomaticValue(std::string paramKey) const;

  /** Returns true if the parameter has an associated value provided externally
   *  (not automatically computed by the application) 
   */
  bool HasUserValue(std::string paramKey) const;

  /** If a user value was provided clear it and update the other parameters */
  void ClearValue(std::string paramKey);

  /** Returns true if the parameter has an associated value.
   * This value can be an automatically computed value, default value,
   * or a value set externally by user */
  bool HasValue(std::string paramKey) const;

  /** Get active flag of parameter with key paramKey */
  bool GetParameterEmpty(std::string paramKey);

  /** Set UserValue flag of parameter with key paramKey */
  void SetParameterUserValue(std::string paramKey, bool value);

  /** Return the user level of access to a parameter */
  UserLevel GetParameterUserLevel(std::string paramKey) const;

  /** Get the role of the parameter */
  Role GetParameterRole(std::string paramKey) const;

  /** Get the parameter type */
  ParameterType GetParameterType(std::string paramKey) const;

  /** Returns the list of choices (key) of the ListView parameter
   * or the Choice parameter */
  std::vector<std::string> GetChoiceKeys(std::string paramKey);

  /** Returns the list of choices (name) of the ListView parameter
   * or the Choice parameter */
  std::vector<std::string> GetChoiceNames(std::string paramKey);

  /* Is the application ready to be executed : All the mandatory
   * parameters have to be set
   */
  /* Set the Parameter value and Update the UserFlag. used by xml parameter
   */

  /** Set an integer value.
   *
   * Can be called for types :
   * \li \c ParameterType_Int
   * \li \c ParameterType_Float
   * \li \c ParameterType_Radius
   * \li \c ParameterType_Choice
   */
  void SetParameterInt(std::string parameter, int value, bool hasUserValueFlag = true);

  /** Set a floating value.
   *
   * Can be called for type \c ParameterType_Float
   */
  void SetParameterFloat(std::string parameter, float value, bool hasUserValueFlag = true);

  /** Set a string value.
   *
   * Can be called for types :
   * \li \c ParameterType_Choice
   * \li \c ParameterType_ListView
   * \li \c ParameterType_String
   * \li \c ParameterType_InputFilename
   * \li \c ParameterType_OutputFilename
   * \li \c ParameterType_Directory
   * \li \c ParameterType_RAM
   * \li \c ParameterType_Int (conversion)
   * \li \c ParameterType_Float (conversion)
   * \li \c ParameterType_Radius (conversion)
   * \li \c ParameterType_InputImage (set filename)
   * \li \c ParameterType_ComplexInputImage (set filename)
   * \li \c ParameterType_InputVectorData (set filename)
   * \li \c ParameterType_OutputImage (set filename)
   * \li \c ParameterType_ComplexOutputImage (set filename)
   * \li \c ParameterType_OutputVectorData (set filename)
   * \li \c ParameterType_OutputProcessXML (set filename)
   * \li \c ParameterType_InputProcessXML (set filename)
   */
  void SetParameterString(std::string parameter, std::string value, bool hasUserValueFlag = true);

  /** Set a string value.
   *
   * Can be called for types :
   * \li \c ParameterType_StringList
   * \li \c ParameterType_InputFilenameList
   * \li \c ParameterType_ListView (set selected names)
   * \li \c ParameterType_InputVectorDataList (set filenames)
   * \li \c ParameterType_InputImageList (set filenames)
   */
  void SetParameterStringList(std::string parameter, std::vector<std::string> values, bool hasUserValueFlag = true);

  /** Set Active flag to \a value and UserValue to 
  * \a hasUserValueFlag 
  */
  void SetParameterEmpty(std::string parameter, bool value, bool hasUserValueFlag = true);

  /** Checks if the application is ready to be executed. It checks that there
   *  is no parameter missing
   */
  bool IsApplicationReady();

  /** Checks if a parameter 'key' is missing.
   *
   * A parameter is missing when all the following conditions are true :
   *   - the parameter has no value
   *   - the parameter is mandatory
   *   - the parameter has Role_Input
   *   - the parameter is not a group
   *   - the parameter ancestors are mandatory or enabled.
   */
  bool IsParameterMissing(const std::string &key) const;

  /** Set a default integer value, must be used in the
   * DoInit when setting a value by default for the parameter.
   *
   * Can be called for types :
   * \li \c ParameterType_Int
   * \li \c ParameterType_Float
   * \li \c ParameterType_Radius
   * \li \c ParameterType_RAM
   */
  void SetDefaultParameterInt(std::string parameter, int value);

  /** Set a default floating value, must used in the
   * DoInit when setting a value by default
   * for the parameter.
   *
   * Can be called for type \c ParameterType_Float
   */
  void SetDefaultParameterFloat(std::string parameter, float value);

  /** Set a default pixel type for an output image parameter
   *
   * \param parameter Name of the output image parameter
   * \param type Default pixel type
   */
  void SetDefaultOutputPixelType(std::string parameter, ImagePixelType type);

  /** Set a default complex pixel type for an output complex image parameter
   *
   * \param parameter Name of the output complex image parameter
   * \param type Default complex pixel type
   */
  void SetDefaultOutputComplexPixelType(std::string parameter, ComplexImagePixelType type);

  /** Set a minimum int value, must be used in the
   * DoInit when setting a \a value by default for the 
   * \a parameter.
   *
   * Can be called for type \c ParameterType_Int
   */
  void SetMinimumParameterIntValue(std::string parameter, int value);

  /** Set a maximum int value, must be used in the
   * DoInit when setting a \a value by default for the 
   * \a parameter.
   *
   * Can be called for type \c ParameterType_Int
   */
  void SetMaximumParameterIntValue(std::string parameter, int value);

  /** Set a minimum float value, must be used in the
   * DoInit when setting a \a value by default
   * for the \a parameter
   *
   * Can be called for type \c ParameterType_Float
   */
  void SetMinimumParameterFloatValue(std::string parameter, float value);

  /** Set a maximum float value, must be used in the
   * DoInit when setting a \a value by default
   * for the \a parameter
   *
   * Can be called for type \c ParameterType_Float
   */
  void SetMaximumParameterFloatValue(std::string parameter, float value);


  /** Set single selection mode on \a status.
   *
   * Can be called for type \c ParameterType_ListView
   */
  void SetListViewSingleSelectionMode(std::string parameter, bool status);

  /** Set an output image value.
   *
   * Can be called for type \c ParameterType_OutputImage
   */
  void SetParameterOutputImage(std::string parameter, FloatVectorImageType* value);

  /** Set a complex output image value
   *
   * Can be called for type \c ParameterType_ComplexOutputImage
   */
  void SetParameterComplexOutputImage(std::string parameter, ComplexFloatVectorImageType* value);

  /** Set the pixel type in which the image will be saved.
   *
   * Can be called for type \c ParameterType_OutputImage
   */
  void SetParameterOutputImagePixelType(std::string parameter, ImagePixelType pixelType);

  /** Set the complex pixel type in which the image will be saved
   *
   * Can be called for type \c ParameterType_ComplexOutputImage
   */
  void SetParameterComplexOutputImagePixelType(std::string parameter, ComplexImagePixelType cpixelType);

  /** Set an output vector data value.
   *
   * Can be called for type \c ParameterType_OutputVectorData
   */
  void SetParameterOutputVectorData(std::string parameter, VectorDataType* value);

  /** Get an integer from a \a parameter.
   *
   * Can be called for types :
   * \li \c ParameterType_Int
   * \li \c ParameterType_Float
   * \li \c ParameterType_Radius
   * \li \c ParameterType_Choice
   * \li \c ParameterType_RAM
   */
  int GetParameterInt(std::string parameter);

  /** Get a floating from a \a parameter.
   *
   * Can be called for type \c ParameterType_Float
   */
  float GetParameterFloat(std::string parameter);

  /** Get a string from a \a parameter.
   *
   * Can be called for types :
   * \li \c ParameterType_String
   * \li \c ParameterType_InputFilename
   * \li \c ParameterType_OutputFilename
   * \li \c ParameterType_Directory
   * \li \c ParameterType_InputImage (get the filename)
   * \li \c ParameterType_ComplexInputImage (get the filename)
   * \li \c ParameterType_InputVectorData (get the filename)
   * \li \c ParameterType_OutputImage (get the filename)
   * \li \c ParameterType_OutputVectorData (get the filename)
   * \li \c ParameterType_ListView (get the string list)
   * \li \c ParameterType_Choice (get the string list)
   */
  std::string GetParameterString(std::string parameter);

  /** Get a string list parameter value.
   *
   * Can be called for types :
   * \li \c ParameterType_StringList
   * \li \c ParameterType_ListView (get the filename list)
   * \li \c ParameterType_InputImageList (get the filename list)
   * \li \c ParameterType_InputVectorDataList (get the filename list)
   * \li \c ParameterType_InputFilenameList (get the filename list)
   */
  // TODO: Should be rewritten:
  //
  // std::size_t
  // GetParameterStringList( const std::vector< String > & v,
  //                         const std::string & parameter ) const;
  //
  // to avoid useless memory allocations.
  std::vector< std::string >
    GetParameterStringList( const std::string & parameter );


  /**
   * Set the input image parameter as an ImageBase * instead
   * of filename. Useful to connect pipelines between different
   * application instances.
   * \param parameter The parameter key
   * \param inputImage ImageBase pointer to use as input
   * \throw itk::Exception if parameter is not found or not an
   * InputImageParameter
   */
  void SetParameterInputImage(std::string parameter, InputImageParameter::ImageBaseType * inputImage);

  /**
   * Get the output image parameter as an ImageBase * instead
   * of writing to disk. Useful to connect pipelines between different
   * application instances.
   * \param parameter The parameter key
   * \return The ImageBase * to the output image
   * \throw itk::Exception if parameter is not found or not an
   * OutputImageParameter
   */
  OutputImageParameter::ImageBaseType * GetParameterOutputImage(std::string parameter);

  /**
   * Set the input complex image parameter as an ImageBase * instead
   * of filename. Useful to connect pipelines between different
   * application instances.
   * \param parameter The parameter key
   * \param inputImage ImageBase pointer to use as input
   * \throw itk::Exception if parameter is not found or not an
   * ComplexInputImageParameter
   */
  void SetParameterComplexInputImage(std::string parameter, ComplexInputImageParameter::ImageBaseType * inputImage);

  /**
   * Get the complex output image parameter as an ImageBase * instead
   * of writing to disk. Useful to connect pipelines between different
   * application instances.
   * \param parameter The parameter key
   * \return The ImageBase * pointer to the output image
   * \throw itk::Exception if parameter is not found or not an
   * ComplexOutputImageParameter
   */
  ComplexOutputImageParameter::ImageBaseType * GetParameterComplexOutputImage(std::string parameter);

  /**
   * Add an image to an InputImageList parameter as an ImageBase
   * pointer instead of reading from file. Useful to connect pipelines
   * between different application instances.
   * \param parameter The parameter key
   * \param img The ImageBase * of the image to add
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  void AddImageToParameterInputImageList(std::string parameter, InputImageListParameter::ImageBaseType * img);

  /**
   * Set the nth image of an InputImageList parameter as an ImageBase pointer
   * instead of reading from file. Useful to connect pipelines
   * between different application instances.
   * \param parameter The parameter key
   * \param id Position at which to set the ImageBase pointer
   * \param img The ImageBase * of the image to add
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter or if id is out of bounds
   */
  void SetNthParameterInputImageList(std::string parameter, const unsigned int &id, InputImageListParameter::ImageBaseType * img);

  /**
   * Add a value to a parameter list as a string.
   *
   * Can be called for parameter type \c ParameterType_InputImageList .
   *
   * \param parameter The parameter key
   * \param str The filename of the image to add
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  void AddParameterStringList(std::string parameter, const std::string & str);

  /**
   * Set the nth value of a parameter list as a string.
   *
   * Can be called for parameter type \c ParameterType_InputImageList .
   *
   * \param parameter The parameter key
   * \param id Position at which to set the ImageBase pointer
   * \param str The filename of the image to add
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter or if id is out of bounds
   */
  void SetNthParameterStringList(std::string parameter, const unsigned int &id, const std::string& str);


  /**
   * Clear all images from an InputImageList parameter.
   *
   * \param parameter The parameter key
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  void ClearParameterInputImageList(std::string parameter);

  /**
   * Get the number of images in an InputImageList parameter.
   * \param parameter The parameter key
   * \return The number of images
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  unsigned int GetNumberOfElementsInParameterInputImageList(std::string parameter);


  /** Get an image value as a pointer of \c FloatVectorImageType.
   *
   * Can be called for type \c ParameterType_InputImage
   */
  FloatVectorImageType* GetParameterImage(std::string parameter);

  /** Get an image value as a pointer of \c Image
   * 
   * Can be called for type \c ParameterType_InputImage
   */      
#define otbGetParameterImageMacro( Image )                              \
  Image##Type * GetParameter##Image( std::string parameter )            \
    {                                                                   \
    Image##Type::Pointer ret;                                           \
    Parameter* param = GetParameterByKey(parameter);                    \
    if (dynamic_cast<InputImageParameter*>(param))                      \
      {                                                                 \
      InputImageParameter* paramDown =                                  \
       dynamic_cast<InputImageParameter*>(param);                       \
      ret = paramDown->Get##Image();                                    \
      }                                                                 \
    else                                                                \
    {                                                                   \
    itkExceptionMacro(<<parameter <<                                    \
      " parameter can't be casted to ImageType");                       \
    }                                                                   \
    return ret;                                                         \
    }

  otbGetParameterImageMacro(UInt8Image);
  otbGetParameterImageMacro(UInt16Image);
  otbGetParameterImageMacro(Int16Image);
  otbGetParameterImageMacro(UInt32Image);
  otbGetParameterImageMacro(Int32Image);
  otbGetParameterImageMacro(FloatImage);
  otbGetParameterImageMacro(DoubleImage);

  otbGetParameterImageMacro(UInt8VectorImage);
  otbGetParameterImageMacro(UInt16VectorImage);
  otbGetParameterImageMacro(Int16VectorImage);
  otbGetParameterImageMacro(UInt32VectorImage);
  otbGetParameterImageMacro(Int32VectorImage);
  otbGetParameterImageMacro(FloatVectorImage);
  otbGetParameterImageMacro(DoubleVectorImage);

  otbGetParameterImageMacro(UInt8RGBImage);
  otbGetParameterImageMacro(UInt8RGBAImage);

  /* Get a complex image value
   *
   * Can be called for type \c ParameterType_ComplexInputImage
   */

#define otbGetParameterComplexImageMacro( Image )                       \
  Image##Type * GetParameter##Image( std::string parameter )            \
    {                                                                   \
    Image##Type::Pointer ret;                                           \
    Parameter* param = GetParameterByKey(parameter);                    \
    ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param); \
    if (paramDown)                                                      \
      {                                                                 \
      ret = paramDown->Get##Image();                                    \
      }                                                                 \
    return ret;                                                         \
    }

  otbGetParameterComplexImageMacro(ComplexFloatImage);
  otbGetParameterComplexImageMacro(ComplexDoubleImage);

  otbGetParameterComplexImageMacro(ComplexFloatVectorImage);
  otbGetParameterComplexImageMacro(ComplexDoubleVectorImage);

  /** Get an image list value as a pointer of \c FloatVectorImageListType.
   *
   * Can be called for type \c ParameterType_InputImageList
   */
  FloatVectorImageListType* GetParameterImageList(std::string parameter);

  /** Get a complex image value as a pointer of 
   * \c ComplexFloatVectorImageType.
   *
   * Can be called for type \c ParameterType_ComplexInputImage
   */
  ComplexFloatVectorImageType* GetParameterComplexImage(std::string parameter);

  /** Get a vector data as a pointer of \c VectorDataType.
   *
   * Can be called for type \c ParameterType_InputVectorData
   */
  VectorDataType* GetParameterVectorData(std::string parameter);

  /** Get a vector data list as a pointer of \c VectorDataListType.
  *
  * Can be called for type \c ParameterType_InputVectorDatalist
  */
  VectorDataListType* GetParameterVectorDataList(std::string parameter);

 /** Get the parameter as a std::string
  *
  * Can be called for types :
  * \li \c ParameterType_String 
  * \li \c ParameterType_InputFilename 
  * \li \c ParameterType_OutputFilename
  * \li \c ParameterType_Directory 
  * \li \c ParameterType_InputImage
  * \li \c ParameterType_ComplexInputImage 
  * \li \c ParameterType_InputVectorData
  * \li \c ParameterType_OutputImage 
  * \li \c ParameterType_OutputVectorData
  * \li \c ParameterType_ListView 
  * \li \c ParameterType_Choice
  * \li \c ParameterType_OutputProcessXML 
  * \li \c ParameterType_InputProcessXML
  * \li \c ParameterType_StringList (get parameter string list) 
  * \li \c ParameterType_InputImageList (get parameter string list)
  * \li \c ParameterType_InputVectorDataList (get parameter string list) 
  * \li \c ParameterType_InputFilenameList (get parameter string list)
  * \li \c ParameterType_Int 
  * \li \c ParameterType_Radius
  * \li \c ParameterType_RAM
  */
  std::string GetParameterAsString(std::string paramKey);

  /** Get the list of all parameters as a vector of their key.
   * The \a recursive allows you to get all parameters inside
   * group prameter (default value is \True) 
   */
  std::vector<std::string> GetParametersKeys(bool recursive = true);

  /** Get the pixel type in which the image will be saved.
   *
   * Can be called for type \c ParameterType_OutputImage
   */
  ImagePixelType GetParameterOutputImagePixelType(std::string parameter);

  /** Set the parameter list of the application as \a paramGroup */
  void SetParameterList(ParameterGroup::Pointer paramGroup)
  {
    m_ParameterList = paramGroup;
  }

  /** Get the pixel type in which the complex image will be saved.
   *
   * Can be called for type \c ParameterType_ComplexOutputImage
   */
  ComplexImagePixelType GetParameterComplexOutputImagePixelType(std::string parameter);

  /** Return the logger of the application */
  otb::Logger* GetLogger() const;

  /** Set the logger instance of the application as \a logger
   * (to use with caution) */
  void SetLogger(otb::Logger *logger);

  itk::ProcessObject* GetProgressSource() const;

  std::string GetProgressDescription() const;

  /** Doc element accessors. */
  itkSetStringMacro(DocName);
  itkGetStringMacro(DocName);
  itkSetStringMacro(DocLongDescription);
  itkGetStringMacro(DocLongDescription);
  itkSetStringMacro(DocAuthors);
  itkGetStringMacro(DocAuthors);
  itkSetStringMacro(DocLimitations);
  itkGetStringMacro(DocLimitations);
  itkSetStringMacro(DocSeeAlso);
  itkGetStringMacro(DocSeeAlso);

  /** Get the application doc tag */
  std::vector<std::string> GetDocTags(){
    return m_DocTags;
  }

  /** Get the application doc tag to \a val */
  void SetDocTags( std::vector<std::string> val ){
    m_DocTags = val;
    this->Modified();
  }

  /** Add a \a tag to the application doc tag */
  void AddDocTag( const std::string & tag )
  {
    for (unsigned int i=0; i<m_DocTags.size(); i++)
      {
      if (m_DocTags[i].compare(tag) == 0) return;
      }
    m_DocTags.push_back( tag );
    this->Modified();
  }

  /** Get the application doc example */
  DocExampleStructure::Pointer GetDocExample()
  {
    if (! IsInitialized())
      {
      Init();
      }

    return m_DocExample;
  }

  /** Get the number of example in the application doc example */
  unsigned int GetNumberOfExamples()
  {
    return GetDocExample()->GetNbOfExamples();
  }

  /** Get the comment of the \a id th example 
   * of the application doc example 
   */
  std::string GetExampleComment(unsigned int id)
  {
    return GetDocExample()->GetExampleComment(id);
  }

  /** Get the number of parameter of the \a id th example 
   * of the application doc example 
   */
  unsigned int GetExampleNumberOfParameters(unsigned int id)
  {
    return GetDocExample()->GetNumberOfParameters(id);
  }

  /** Get the parameter key of the \a paramId th parameter
   * of the \a id th example of the application doc example 
   */
  std::string GetExampleParameterKey(unsigned int exId, unsigned int paramId)
  {
    return GetDocExample()->GetParameterKey(paramId, exId);
  }

  /** Get the parameter value of the \a paramId th parameter
   * of the \a id th example of the application doc example 
   */
  std::string GetExampleParameterValue(unsigned int exId, unsigned int paramId)
  {
    return GetDocExample()->GetParameterValue(paramId, exId);
  }

  /** Set the parameter \a key value
   * of the \a id th example of the application doc example to \a val
   */
  void SetDocExampleParameterValue( const std::string key, const std::string value, unsigned int exId=0 )
  {
    GetDocExample()->AddParameter( key, value, exId );
    this->Modified();
  }

  /** Set the \a i th example comment to \a comm  */
  void SetExampleComment( const std::string & comm, unsigned int i )
  {
    GetDocExample()->SetExampleComment( comm, i );
    this->Modified();
  }

  /** Add an example to the application doc example. 
   * Return the id of the added example */
  unsigned int AddExample( const std::string & comm="" )
  {
    unsigned int id = GetDocExample()->AddExample( comm );
    this->Modified();
    return id;
  }

  /** Generate the first command line example */
  std::string GetCLExample()
  {
    return GetDocExample()->GenerateCLExample();
  }

  /** Generate the first command line example (HTML style) */
  std::string GetHtmlExample()
  {
    return GetDocExample()->GenerateHtmlExample();
  }

  /** Return all parameters which role is \c Role_Output and that are enable
   * in a vector of pairs that contains the parameter key (first) 
   * and its value (second).
   */
  std::vector< std::pair<std::string, std::string> > GetOutputParametersSumUp();

   /** If need to force readxml more than once in application */
   void ForceInXMLParseFlag()
   {
     m_IsInXMLParsed = false;
   }


  double GetLastExecutionTiming() const;

  /** documentation link */
  void SetDocLink(const std::string & link)
  {
    if (m_Doclink.compare(link) != 0) {
      m_Doclink = link;
      this->Modified();
    }
  }

  /** Get the doc link */
  const std::string& GetDocLink() const
  {
    return m_Doclink;
  }

  inline void SetOfficialDocLink()
  {
    std::string link = "http://www.orfeo-toolbox.org/Applications/";
    link.append(this->GetName());
    link.append(".html");
    this->SetDocLink(link);
  }

protected:
  /** Constructor */
  Application();

  /** Destructor */
  ~Application() ITK_OVERRIDE;

  /** Register a ProcessObject as a new progress source */
  void AddProcess(itk::ProcessObject* object, std::string description);

  /** Add a new choice value to an existing choice parameter
   * \param paramKey : path.to.choice.NewChoice
   * \param paramName : name of NewChoice
   */
  void AddChoice(std::string paramKey, std::string paramName);

  /** Add a new parameter to a parameter group.
   * The \a paramKey can be the path to a parameter group
   * or the path to a choice value.
   * \param paramKey : path.to.parametergroupe.NewParam or NewParam
   * \param paramName : name of NewParam
   * \param type : parameter type
   */
  void AddParameter(ParameterType type, std::string paramKey, std::string paramName);

  /** Is calling \link <tt> AddRAMParameter(std::string paramKey, std::string paramName, unsigned int defaultValue) </tt>
   * AddRAMParameter( paramKey , "Available RAM (Mb)" , maxRam ) \endlink 
   * where the \a maxRam is computed by 
   * otb::ConfigurationManager::GetMaxRAMHint() . 
   * Default value of \a paramKey is \c "ram" .
   */
  void AddRAMParameter(std::string paramKey="ram");

  /** Add a parameterRAM 
   * \param paramKey : key to the ram parameter
   * \param paramName : name of the ram parameter
   * \param defaultValue : default value that will be used during execution if
   * none is given
   */
  void AddRAMParameter(std::string paramKey, std::string paramName, unsigned int defaultValue);

  /** Add a rand parameter with name : "set user defined seed" 
   * and no default value.
   * \param paramKey : key of the parameter, default value is "rand"
   */
  void AddRANDParameter(std::string paramKey="rand");

  /** Add a parameterRAND method with parameter
   * by default seed initialization is based on time value
   */
  void AddRANDParameter(std::string paramKey, std::string paramName, unsigned int defaultValue);

  /** Add a \c ParameterType_InputProcessXML */
  void AddInXMLParameter()
  {
    GetParameterList()->AddInXMLParameter();
  }

  /** Add a \c ParameterType_OutputProcessXML */
  void AddOutXMLParameter()
  {
    GetParameterList()->AddOutXMLParameter();
  }

  /** Remove the items of a ListView parameter. 
   * Can be called on type \c ParameterType_ListView 
   */
  void ClearChoices(std::string key);

  /** Get Items selected in the ListView parameter.
   * Can be called on type \c ParameterType_ListView 
   */
  std::vector<int>  GetSelectedItems(std::string paramKey);

  /** Set Mandatory flag to \c true */
  void MandatoryOn(std::string paramKey);

  /** Set Mandatory flag to \c false */
  void MandatoryOff(std::string paramKey);

  /** Not implemented */
  void SetParameterUserLevel(std::string paramKey, UserLevel level);

  /**  Set the parameter role (Role_Input / Role_Output ) */
  void SetParameterRole(std::string paramKey, Role role);

  /** Get a TImageType pointer.
   *
   * Can be called for type ParameterType_InputImage
   */
  template <class TImageType>
    TImageType* GetParameterImage(std::string parameter)
  {
    typename TImageType::Pointer ret;
    Parameter* param = GetParameterByKey(parameter);
    if (dynamic_cast<InputImageParameter*>(param))
      {
      InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
      ret = paramDown->GetImage<TImageType>();
      }

    //TODO: exception if not found ?
    return ret;
  }

  /** Set flag AutomaticValue to \c true */
  void AutomaticValueOn(std::string paramKey);

  /** Set flag AutomaticValue to \c false */
  void AutomaticValueOff(std::string paramKey);

  /** Set a TImageType value.
   * \param paramKey : key to a ParameterType_OutputImage
   * \param value : TImageType pointer
   *
   * Can be called for type \c ParameterType_OutputImage
   */
  template <class TImageType>
    void SetParameterOutputImage(std::string paramKey, TImageType* value)
  {
    Parameter* param = GetParameterByKey(paramKey);

    if (dynamic_cast<OutputImageParameter*>(param))
      {
      OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
      paramDown->SetValue(value);
      }
  }

    /* Set a complex output image value
   *
   * Can be called for types :
   * \li ParameterType_ComplexOutputImage
   */
  template <class TImageType>
    void SetParameterComplexOutputImage(std::string parameter, TImageType* value)
  {
    Parameter* param = GetParameterByKey(parameter);

    if (dynamic_cast<ComplexOutputImageParameter*>(param))
      {
      ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
      paramDown->SetValue(value);
      }
  }

private:
  /** Override this method to add parameters */
  virtual void DoInit() = 0;

  /** Override this method to update non valued parameters */
  virtual void DoUpdateParameters() = 0;

  /** Override this method to build the output */
  virtual void DoExecute() = 0;

  /** This method will be called after the
   * ExecuteAndWriteOutput() call to allow for cleanup. Default
   * implementation does nothing */
  virtual void AfterExecuteAndWriteOutputs();

  Application(const Application &); //purposely not implemented
  void operator =(const Application&); //purposely not implemented

  /** %Application name */
  std::string                       m_Name;

  /** %Application description */
  std::string                       m_Description;

  /** Root parameter group for the application */
  ParameterGroup::Pointer           m_ParameterList;

  /** %Application logger */
  otb::Logger::Pointer              m_Logger;

  itk::ProcessObject::Pointer       m_ProgressSource;
  std::string                       m_ProgressSourceDescription;

  /** Long name of the application (that can be displayed...) */
  std::string m_DocName;

  /** Long and precise application description . */
  std::string m_DocLongDescription;

  /** Doc example structure. Use GetDocExample() to access it */
  DocExampleStructure::Pointer m_DocExample;

  /** Author List. Format : "John Doe, Winnie the Pooh".*/
  std::string m_DocAuthors;

  /** Known limitations (threading, invalid pixel type ...) or bugs */
  std::string m_DocLimitations;

  /** Related applications */
  std::string m_DocSeeAlso;

  /** Tags that define the application (ex : segmentation, OBIA).*/
  std::vector<std::string> m_DocTags;

  /** Doc link application */
  std::string m_Doclink;

  /** Chrono to measure execution time */
  itk::TimeProbe m_Chrono;

  //rashad:: controls adding of -xml parameter. set to true by default
  /** Flag for \c ParameterType_InputProcessXML */
  bool                              m_HaveInXML;

  /** Flag for \c ParameterType_OutputProcessXML */
  bool                              m_HaveOutXML;

  /** Flag for the state of the input XML */
  bool                              m_IsInXMLParsed;

  /**
    * Declare the class
    * - Wrapper::MapProjectionParametersHandler
    * - Wrapper::ElevationParametersHandler
    * as friend to be able to access to the protected method of
    * Wrapper::Application class.
    **/
  friend class MapProjectionParametersHandler;
  friend class ElevationParametersHandler;

}; //end class

} // end namespace Wrapper
} //end namespace otb


//#ifndef OTB_MANUAL_INSTANTIATION
//#include "otbWrapperApplication.txx"
//#endif


#endif // otbWrapperApplication_h_
