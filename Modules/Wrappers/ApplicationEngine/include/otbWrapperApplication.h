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

#ifndef otbWrapperApplication_h
#define otbWrapperApplication_h

#include <string>
#include <set>
#include "otbWrapperTypes.h"
#include "otbWrapperTags.h"
#include "otbWrapperParameterGroup.h"

#include "otbLogger.h"
#include "otbStopwatch.h"
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
class OTBApplicationEngine_EXPORT ApplicationException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro( ApplicationException, ExceptionObject );

  /** Constructor. */
  ApplicationException(const char *file, unsigned int line,
                       const char* message = "Application error.",
                       const char* loc = "Unknown");

  /** Constructor. */
  ApplicationException(const std::string& file, unsigned int line,
                       const char* message = "Application error.",
                       const char* loc = "Unknown");

  ApplicationException(const std::string& file, unsigned int line,
                       const std::string& message = "Application error.",
                       const std::string& loc = "Unknown");
};

namespace Wrapper
{

/** \class Application
 *  \brief This class represent an application
 *  TODO
 *
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

  virtual void SetName(const std::string&);
  virtual const char* GetName() const;

  void SetDescription(const std::string&);
  virtual const char* GetDescription() const;

  /** Initialize the application, instantiating the parameter list */
  void Init();

  /** Check if the application has been initialized */
  bool IsInitialized() const;

  virtual void SetHaveInXML(bool);
  virtual bool GetHaveInXML() const;

  virtual void SetHaveOutXML(bool);
  virtual bool GetHaveOutXML() const;

  /** Update the value of parameters for which no user value has been provided */
  void UpdateParameters();

  /** Run the application.
   *
   * For pipeline ready application, this only wires
   * and configures the pipeline, and provides
   * the output image or vector data parameters as pointers.
   *
   * In other cases, the application must handle
   * the I/O (intermediary results for example)
   *
   * Returns 0 on success, or a non-null integer on error
   */
  int Execute();

  /** write all of the output to disk
   * if they have an associated filename.
   * This is a helper function for wrappers without pipeline support.
   */
  void WriteOutput();

  /** Run the application, then write all of the output to disk
   * if they have an associated filename.
   * This is a helper function for wrappers without pipeline support.
   *
   * Returns 0 on success, or a non-null integer on error
   */
  int ExecuteAndWriteOutput();

  /** Connect input image to an output image in app */
  bool ConnectImage(std::string in, Application* app, std::string out);

  /** Propagate the connection mode : */
  void PropagateConnectMode(bool isMem);
  
  /** Request the application to stop its processing */
  void Stop();

  /* Get the internal application parameters
   *
   * WARNING: this method may disappear from the API */
  ParameterGroup* GetParameterList();

  /* Get the internal application parameter specified
   * if the follow flag is on, the function returns the target of proxy parameters
   * WARNING: this method may disappear from the API */
  Parameter* GetParameterByKey(std::string parameter, bool follow=true);

  /* Get the internal application parameter specified
   *
   * WARNING: this method may disappear from the API */
  const Parameter* GetParameterByKey(std::string parameter, bool follow=true) const;

  /* Returns the description of a parameter */
  std::string GetParameterName(std::string paramKey);

  /* Returns the description of a parameter */
  std::string GetParameterDescription(std::string paramKey);

  /* Set the description of a parameter */
  void SetParameterDescription(std::string paramKey, std::string dec);

  /* Enable the use of an optional parameter. Returns the previous state */
  void EnableParameter(std::string paramKey);

  /* Disable the use of an optional parameter. Returns the previous state  */
  void DisableParameter(std::string paramKey);

  /* Return the enable state of an optional parameter  */
  bool IsParameterEnabled(std::string paramKey, bool recurseParents = false) const;

  /* Return true if the specified parameter is mandatory */
  bool IsMandatory(std::string paramKey) const;

  /* Return true if the specified parameter was set automatically in
   * the application
   */
  bool HasAutomaticValue(std::string paramKey) const;

  /* Returns true if the parameter has an associated value provided externally
   *  (not automatically computed by the application) */
  bool HasUserValue(std::string paramKey) const;

  /* If a user value was provided, clear it and update the other parameters */
  void ClearValue(std::string paramKey);

  /* Returns true if the parameter has an associated value.
   * This value can be an automatically computed value or default value,
   * or a value set externally by user */
  bool HasValue(std::string paramKey) const;

  /** Set HasUserValue flag of parameter with key paramKey
   *  Note that when this function is called from DoInit, DoUpdateParameters
   *  or DoExecute, it will always set this flag to false, because this is
   *  the core behavior of the application.
   */
  void SetParameterUserValue(std::string paramKey, bool value);

  /* Return the user level of access to a parameter */
  UserLevel GetParameterUserLevel(std::string paramKey) const;

  /** Get the role of the parameter */
  Role GetParameterRole(std::string paramKey) const;

  /* Get the parameter type from its name */
  ParameterType GetParameterType(std::string paramKey) const;

  /* Returns the description of a parameter */
  std::vector<std::string> GetChoiceKeys(std::string paramKey);

  /* Returns the description of a parameter */
  std::vector<std::string> GetChoiceNames(std::string paramKey);

  /* Is the application ready to be executed : All the mandatory
   * parameters have to be set
   */
  /* Set the Parameter value and Update the UserFlag. used by xml parameter
   */

  /* Set an integer value
   *
   * Can be called for types :
   * \li ParameterType_Int
   * \li ParameterType_Bool
   * \li ParameterType_Float
   * \li ParameterType_Radius
   * \li ParameterType_Choice
   */
  void SetParameterInt(std::string parameter, int value, bool hasUserValueFlag = true);

  /* Set a floating value
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  void SetParameterFloat(std::string parameter, float value, bool hasUserValueFlag = true);

  /* Set a string value
   *
   * Can be called for types :
   * \li ParameterType_InputImageListParameter
   * \li ParameterType_InputVectorDataListParameter
   * \li ParameterType_InputFilenameListParameter
   * \li ParameterType_StringList
   * \li ParameterType_ListView
   */
  void SetParameterString(std::string parameter, std::string value, bool hasUserValueFlag = true);

  /* Set a string value
   *
   * Can be called for types :
   * \li ParameterType_String
   * \li ParameterType_StringList
   * \li ParameterType_ListView
   * \li ParameterType_InputFilename
   * \li ParameterType_OutputFilename
   * \li ParameterType_Directory
   * \li ParameterType_Choice
   * \li ParameterType_Float
   * \li ParameterType_Int
   * \li ParameterType_Radius
   * \li ParameterType_InputImageParameter
   * \li ParameterType_ComplexInputImageParameter
   * \li ParameterType_InputVectorDataParameter
   * \li ParameterType_OutputImageParameter
   * \li ParameterType_OutputVectorDataParameter
   */
  void SetParameterStringList(std::string parameter, std::vector<std::string> values, bool hasUserValueFlag = true);

  /** Checks if the application is ready to be executed. It checks that there
   *  is no missing parameter
   */
  bool IsApplicationReady();

  /** Checks if a parameter 'key' is missing.
   *
   * A parameter is missing when all the following conditions are true :
   *   - the parameter is mandatory
   *   - the parameter has Role_Input
   *   - the parameter is not a group
   *   - the parameter has no value
   *   - the parameter ancestors are mandatory or enabled.
   */
  bool IsParameterMissing(const std::string &key) const;

  /* Set a default integer value, must be used in the
   * DoInit when setting a value by default
   * for the parameter
   *
   * Can be called for types :
   * \li ParameterType_Int
   * \li ParameterType_Float
   * \li ParameterType_Radius
   * \li ParameterType_Choice
   */
  void SetDefaultParameterInt(std::string parameter, int value);

  /* Get the default integer value of a parameter
   *
   * Can be called for types :
   * \li ParameterType_Int
   * \li ParameterType_Float
   * \li ParameterType_Radius
   * \li ParameterType_Choice
   */
  int GetDefaultParameterInt(std::string parameter);

  /* Set a default floating value, must be used in the
   * DoInit when setting a value by default
   * for the parameter
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  void SetDefaultParameterFloat(std::string parameter, float value);

  /* Get the default floating value of a parameter
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  float GetDefaultParameterFloat(std::string parameter);

  /** Set a default pixel type for an output image parameter
   *
   * \param[in] parameter Name of the output image parameter
   * \param[in] type Default pixel type
   */
  void SetDefaultOutputPixelType(std::string parameter, ImagePixelType type);

  /** Set a default complex pixel type for an output complex image parameter
   *
   * \param[in] parameter Name of the output complex image parameter
   * \param[in] type Default complex pixel type
   */
  void SetDefaultOutputComplexPixelType(std::string parameter, ComplexImagePixelType type);

  /* Set a minimum int value, must used in the
   * DoInit when setting a value by default
   * for the parameter
   *
   * Can be called for types :
   * \li ParameterType_Int
   */
  void SetMinimumParameterIntValue(std::string parameter, int value);

  /* Set a maximum int value, must used in the
   * DoInit when setting a value by default
   * for the parameter
   *
   * Can be called for types :
   * \li ParameterType_Int
   */
  void SetMaximumParameterIntValue(std::string parameter, int value);

  /* Set a minimum int value, must used in the
   * DoInit when setting a value by default
   * for the parameter
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  void SetMinimumParameterFloatValue(std::string parameter, float value);

  /* Set a maximum int value, must used in the
   * DoInit when setting a value by default
   * for the parameter
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  void SetMaximumParameterFloatValue(std::string parameter, float value);


  /**
   * Enable single selection mode for list view if status is true
   * (default is false).
   *
   * Can be called for types:
   * \li ParameterType_ListView
   */
  void SetListViewSingleSelectionMode(std::string parameter, bool status);

  /**
   * True if the parameter is a list view and is in single selection mode
   *
   * Can be called for types:
   * \li ParameterType_ListView
   */
  bool GetListViewSingleSelectionMode(const std::string& parameter);

  /* Set an output image value
   *
   * Can be called for types :
   * \li ParameterType_OutputImage
   */
  void SetParameterOutputImage(std::string parameter, FloatVectorImageType* value);

  /* Set a complex output image value
   *
   * Can be called for types :
   * \li ParameterType_ComplexOutputImage
   */
  void SetParameterComplexOutputImage(std::string parameter, ComplexFloatVectorImageType* value);

  /* Set the pixel type in which the image will be saved
   *
   * Can be called for types :
   * \li ParameterType_OutputImage
   */
  void SetParameterOutputImagePixelType(std::string parameter, ImagePixelType pixelType);

  /* Set the complex pixel type in which the image will be saved
   *
   * Can be called for types :
   * \li ParameterType_ComplexOutputImage
   */
  void SetParameterComplexOutputImagePixelType(std::string parameter, ComplexImagePixelType cpixelType);

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
   * \li ParameterType_Bool
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
   * \li ParameterType_InputFilename
   * \li ParameterType_OutputFilename
   * \li ParameterType_Directory
   * \li ParameterType_InputImage
   * \li ParameterType_ComplexInputImage
   * \li ParameterType_InputVectorData
   * \li ParameterType_OutputImage
   * \li ParameterType_OutputVectorData
   */
  std::string GetParameterString(std::string parameter);

  /* Get a string list parameter value
   *
   * Can be called for types :
   * \li ParameterType_StringList
   * \li ParameterType_InputImageList
   * \li ParameterType_InputFilenameList
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
   * \in parameter The parameter key
   * \in inputImage ImageBase pointer to use as input
   * \throw itk::Exception if parameter is not found or not an
   * InputImageParameter
   */
  void SetParameterInputImage(std::string parameter, ImageBaseType * inputImage);

  /**
   * Get the output image parameter as an ImageBase * instead
   * of writing to disk. Useful to connect pipelines between different
   * application instances.
   * \in parameter The parameter key
   * \return The ImageBase * to the output image
   * \throw itk::Exception if parameter is not found or not an
   * OutputImageParameter
   */
  ImageBaseType * GetParameterOutputImage(std::string parameter);

  /**
   * Set the input complex image parameter as an ImageBase * instead
   * of filename. Useful to connect pipelines between different
   * application instances.
   * \in parameter The parameter key
   * \in inputImage ImageBase pointer to use as input
   * \throw itk::Exception if parameter is not found or not an
   * ComplexInputImageParameter
   */
  void SetParameterComplexInputImage(std::string parameter, ImageBaseType * inputImage);

  /**
   * Get the complex output image parameter as an ImageBase * instead
   * of writing to disk. Useful to connect pipelines between different
   * application instances.
   * \in parameter The parameter key
   * \return The ImageBase * pointer to the output image
   * \throw itk::Exception if parameter is not found or not an
   * ComplexOutputImageParameter
   */
  ImageBaseType * GetParameterComplexOutputImage(std::string parameter);

  /**
   * Add an image to an InputImageList parameter as an ImageBase
   * pointer instead of reading from file. Useful to connect pipelines
   * between different application instances.
   * \in parameter The parameter key
   * \in img The ImageBase * of the image to add
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  void AddImageToParameterInputImageList(std::string parameter, ImageBaseType * img);

  /**
   * Set the nth image of an InputImageList parameter as an ImageBase pointer
   * instead of reading from file. Useful to connect pipelines
   * between different application instances.
   * \in parameter The parameter key
   * \in id Position at which to set the ImageBase pointer
   * \in img The ImageBase * of the image to add
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter or if id is out of bounds
   */
  void SetNthParameterInputImageList(std::string parameter, const unsigned int &id, ImageBaseType * img);

/**
   * Add a value to a parameter list as a string
   *
   * Can be called for parameter types:
   * \li ParameterType_InputImageList
   *
   * \in parameter The parameter key
   * \in str The string
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  void AddParameterStringList(std::string parameter, const std::string & str);

  /**
   * Set the nth value of a parameter list as a string.
   *
   * Can be called for parameter types:
   * \li ParameterType_InputImageList
   *
   * \in parameter The parameter key
   * \in id Position at which to set the ImageBase pointer
   * \in str The string
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter or if id is out of bounds
   */
  void SetNthParameterStringList(std::string parameter, const unsigned int &id, const std::string& str);


  /**
   * Clear all images from an InputImageList parameter.
   *
   * \in parameter The parameter key
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  void ClearParameterInputImageList(std::string parameter);

  /**
   * Get the number of images in an InputImageList parameter.
   * \in parameter The parameter key
   * \return The number of images
   * \throw itk::Exception if parameter is not found or not an
   * InputImageList parameter
   */
  unsigned int GetNumberOfElementsInParameterInputImageList(std::string parameter);


  /* Get an image value
   *
   * Can be called for types :
   * \li ParameterType_InputImage
   */
  FloatVectorImageType* GetParameterImage(std::string parameter);

  UInt8ImageType * GetParameterUInt8Image(std::string);
  UInt16ImageType * GetParameterUInt16Image(std::string);
  Int16ImageType * GetParameterInt16Image(std::string);
  UInt32ImageType * GetParameterUInt32Image(std::string);
  Int32ImageType * GetParameterInt32Image(std::string);
  FloatImageType * GetParameterFloatImage(std::string);
  DoubleImageType * GetParameterDoubleImage(std::string);
  UInt8VectorImageType * GetParameterUInt8VectorImage(std::string);
  UInt16VectorImageType * GetParameterUInt16VectorImage(std::string);
  Int16VectorImageType * GetParameterInt16VectorImage(std::string);
  UInt32VectorImageType * GetParameterUInt32VectorImage(std::string);
  Int32VectorImageType * GetParameterInt32VectorImage(std::string);
  FloatVectorImageType * GetParameterFloatVectorImage(std::string);
  DoubleVectorImageType * GetParameterDoubleVectorImage(std::string);
  UInt8RGBImageType * GetParameterUInt8RGBImage(std::string);
  UInt8RGBAImageType * GetParameterUInt8RGBAImage(std::string);

  // Complex image
  ComplexInt16ImageType * GetParameterComplexInt16Image(std::string);
  ComplexInt32ImageType * GetParameterComplexInt32Image(std::string);
  ComplexFloatImageType * GetParameterComplexFloatImage(std::string);
  ComplexDoubleImageType * GetParameterComplexDoubleImage(std::string);

  ComplexInt16VectorImageType * GetParameterComplexInt16VectorImage(std::string);
  ComplexInt32VectorImageType * GetParameterComplexInt32VectorImage(std::string);
  ComplexFloatVectorImageType * GetParameterComplexFloatVectorImage(std::string);
  ComplexDoubleVectorImageType * GetParameterComplexDoubleVectorImage(std::string);


  /* Get an image list value
   *
   * Can be called for types :
   * \li ParameterType_InputImageList
   */
  FloatVectorImageListType* GetParameterImageList(std::string parameter);

  /* Get a complex image value
   *
   * Can be called for types :
   * \li ParameterType_ComplexInputImage
   */
  ComplexFloatVectorImageType* GetParameterComplexImage(std::string parameter);

  /* GetParameterVectorData
   *
   * Can be called for types :

   * \li ParameterType_InputVectorData
   */
  VectorDataType* GetParameterVectorData(std::string parameter);

  /* GetParameteVetorDataList
     *
     * Can be called for types :

     * \li ParameterType_InputVectorDatalist
     */
  VectorDataListType* GetParameterVectorDataList(std::string parameter);

 /* Get the parameter as a std::string
   *
   * Can be called for types :
   * \li ParameterType_Float
   * \li ParameterType_Int
   * \li ParameterType_Choice
   * \li ParameterType_Radius
   * \li ParameterType_String
   * \li ParameterType_InputFilename
   * \li ParameterType_OutputFilename
   * \li ParameterType_Directory
   * \li ParameterType_InputImage
   * \li ParameterType_ComplexInputImage
   * \li ParameterType_InputVectorData
   * \li ParameterType_OutputImage
   * \li ParameterType_OutputVectorData
   */
  std::string GetParameterAsString(std::string paramKey);

  /* Get the list of all parameters
   */
  std::vector<std::string> GetParametersKeys(bool recursive = true);

  /* Get the pixel type in which the image will be saved
   *
   * Can be called for types :
   * \li ParameterType_OutputImage
   */
  ImagePixelType GetParameterOutputImagePixelType(std::string parameter);

  void SetParameterList(ParameterGroup::Pointer paramGroup)
  {
    m_ParameterList = paramGroup;
  }

  /* Get the pixel type in which the complex image will be saved
   *
   * Can be called for types :
   * \li ParameterType_ComplexOutputImage
   */
  ComplexImagePixelType GetParameterComplexOutputImagePixelType(std::string parameter);

  otb::Logger* GetLogger() const;

  /** Sets the logger instance of the application (use with caution) */
  void SetLogger(otb::Logger *logger);

  itk::ProcessObject* GetProgressSource() const;

  std::string GetProgressDescription() const;

  /** Doc element accessors. */
  virtual void SetDocName(const std::string&);
  virtual const char* GetDocName() const;

  virtual void SetDocLongDescription(const std::string&);
  virtual const char* GetDocLongDescription() const;

  virtual void SetDocAuthors(const std::string&);
  virtual const char* GetDocAuthors() const;

  virtual void SetDocLimitations(const std::string&);
  virtual const char* GetDocLimitations() const;

  virtual void SetDocSeeAlso(const std::string&);
  virtual const char* GetDocSeeAlso() const;

  virtual void SetDocTags(std::vector<std::string>);
  virtual std::vector<std::string> GetDocTags() const;

  void AddDocTag(const std::string&);

  DocExampleStructure::Pointer GetDocExample();
  unsigned int GetNumberOfExamples();
  std::string GetExampleComment(unsigned int id);
  unsigned int GetExampleNumberOfParameters(unsigned int id);
  std::string GetExampleParameterKey(unsigned int exId, unsigned int paramId);
  std::string GetExampleParameterValue(unsigned int exId, unsigned int paramId);
  void SetDocExampleParameterValue( const std::string key, const std::string value, unsigned int exId=0 );
  void SetExampleComment( const std::string & comm, unsigned int i );
  unsigned int AddExample( const std::string & comm="" );
  std::string GetCLExample();
  std::string GetHtmlExample();

  /** Return all parameters which role is Role_Output in a vector of pairs that contains the
  * parameter key and its value.
  */
  std::vector< std::pair<std::string, std::string> > GetOutputParametersSumUp();

   /** If need to force readxml more than once in application */
  void ForceInXMLParseFlag();

  double GetLastExecutionTiming() const;

  virtual void SetDocLink(const std::string & link);
  virtual const std::string& GetDocLink() const;

  void SetOfficialDocLink();

  /** Get the origin of the image parameter 'key'. The optional 'idx' allows
   * to select the image in an InputImageList. */
  ImageBaseType::PointType GetImageOrigin(const std::string & key, unsigned int idx = 0);

  /** Get the spacing of the image parameter 'key'. The optional 'idx' allows to
   *  select the image in an InputImageList. We use the signed spacing convention. */
  ImageBaseType::SpacingType GetImageSpacing(const std::string & key, unsigned int idx = 0);

  /** Get the size of the image parameter 'key'. The optional 'idx' allows to
   * select the image in an InputImageList. It corresponds to the size of LargestPossibleRegion*/
  ImageBaseType::SizeType GetImageSize(const std::string & key, unsigned int idx = 0);

  /** Get the number of bands in the image parameter 'key'. The optional 'idx'
   * allows to select the image in an InputImageList.*/
  unsigned int GetImageNbBands(const std::string & key, unsigned int idx = 0);

  /** Get the projection of the image parameter 'key'. The optional 'idx' allows
   *  to select the image in an InputImageList.*/
  std::string GetImageProjection(const std::string & key, unsigned int idx = 0);

  /** Get the keywordlist of the image parameter 'key'. The optional 'idx'
   * allows to select the image in an InputImageList.*/
  otb::ImageKeywordlist GetImageKeywordlist(const std::string & key, unsigned int idx = 0);

  /** Set the requested region on the image parameter 'key' and propagate it.
   *  The returned value is an estimate of the RAM usage (in Bytes) to process
   *  this region. It should be assumed that the index of the largest possible
   *  region starts at (0,0). The optional 'idx' allows to select the image in
   *  an InputImageList*/
  unsigned long PropagateRequestedRegion(const std::string & key, ImageBaseType::RegionType region, unsigned int idx = 0);

  /** Get the requested region of the image parameter 'key'. The optional 'idx'
   * allows to select the image in an InputImageList. It should be assumed that
   * the index of the largest possible region starts at (0,0).*/
  ImageBaseType::RegionType GetImageRequestedRegion(const std::string & key, unsigned int idx = 0);

  /** Returns a copy of the metadata dictionary of the image */
  itk::MetaDataDictionary GetImageMetaData(const std::string & key, unsigned int idx = 0);

  /** Find out what is the pixel type from an image parameter
   *  This function assumes that the underlying object is either an otb::Image
   *  or an otb::VectorImage. The optional 'idx' allows to access InputImageList.
   */
  ImagePixelType GetImageBasePixelType(const std::string & key, unsigned int idx = 0);

  /** Return the image from parameter 'key' as a base type. The optional 'idx'
   *  allows to access InputImageList.
   *
   *  Works on parameters:
   *  \li ParameterType_InputImage
   *  \li ParameterType_InputImageList
   *  \li ParameterType_OutputImage
   *  \li ParameterType_ComplexInputImage
   *  \li ParameterType_ComplexOutputImage
   */
  ImageBaseType* GetParameterImageBase(const std::string & key, unsigned int idx = 0);

  /** Set the image in parameter 'key' as a base type. The optional 'idx'
   *  allows to access InputImageList.
   *
   *  Works on parameters:
   *  \li ParameterType_InputImage
   *  \li ParameterType_InputImageList
   *  \li ParameterType_ComplexInputImage
   */
  void SetParameterImageBase(const std::string & key, ImageBaseType* img, unsigned int idx = 0);

  /**
  Register all ProcessObject that are linked to parameters : 
    \li ParameterType_OutputImage
    \li ParameterType_OutputVectorData

    Those ProcessObjects are stored in the m_Filters set and are deleted at the 
  end of ExecuteAndWriteOutput (if there are only held by the set)
  This method can be called just before the end of a DoExecute in a derived 
  class of Application.
  */
  void RegisterPipeline();

  /**
  Register all DataObject that are reachable from :
    \li ParameterType_OutputImage
    \li ParameterType_OutputVectorData

  Once registered, the methode ReleaseData is called on each one of them.
  */
  void FreeRessources();

  bool IsExecuteDone();

protected:
  /** Constructor */
  Application();

  /** Destructor */
  ~Application() override;

  /* Register a ProcessObject as a new progress source */
  void AddProcess(itk::ProcessObject* object, std::string description);

  /** Add a new choice value to an existing choice parameter */
  void AddChoice(std::string paramKey, std::string paramName);

  /** Add a new parameter to the parameter group
   * the parent key of paramKey can be the path to a parameter group
   * or the path to a choice value */
  void AddParameter(ParameterType type, std::string paramKey, std::string paramName);

  /** Add a parameterRAM method with no parameter*/
  void AddRAMParameter(std::string paramKey="ram");

  /** Add a parameterRAM method with parameter*/
  void AddRAMParameter(std::string paramKey, std::string paramName, unsigned int defaultValue);

   /** Add a parameterRAND method with no parameter*/
   void AddRANDParameter(std::string paramKey="rand");

  /** Add a parameterRAND method with parameter
   * by default seed initialization is based on time value*/
   void AddRANDParameter(std::string paramKey, std::string paramName, unsigned int defaultValue);

   void AddInXMLParameter()
   {
     GetParameterList()->AddInXMLParameter();
   }

   void AddOutXMLParameter()
   {
     GetParameterList()->AddOutXMLParameter();
   }

  /** Remove the items added to the ListWidget */
  void ClearChoices(std::string key);

  /** Get Items selected in the ListView Parameter*/
  std::vector<int>  GetSelectedItems(std::string paramKey);

  /** Declare a parameter as mandatory */
  void MandatoryOn(std::string paramKey);

  /** Declare a parameter as NOT mandatory (default state) */
  void MandatoryOff(std::string paramKey);

  /* Set the user level of access to a parameter */
  void SetParameterUserLevel(std::string paramKey, UserLevel level);

  /*  Set the parameter role (input/output) */
  void SetParameterRole(std::string paramKey, Role role);

  /* Get an image value
   *
   * Can be called for types :
   * \li ParameterType_InputImage
   */
  template <class TImageType>
    TImageType* GetParameterImage(std::string parameter)
  {
    typename TImageType::Pointer ret;
    Parameter* param = GetParameterByKey(parameter);
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    if (paramDown)
    {
      return paramDown->GetImage<TImageType>();
    }
    else
    {
      itkExceptionMacro(<<parameter << " parameter can't be casted to ImageType");
      return nullptr;
    }
  }

  /** Declare a parameter as having an automatic value */
  void AutomaticValueOn(std::string paramKey);

  /** Declare a parameter as NOT having an automatic value */
  void AutomaticValueOff(std::string paramKey);

  /* Set an output image value
   *
   * Can be called for types :
   * \li ParameterType_OutputImage
   */
  template <class TImageType>
    void SetParameterOutputImage(std::string parameter, TImageType* value)
  {
    Parameter* param = GetParameterByKey(parameter);

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
  /* Implement this method to add parameters */
  virtual void DoInit() = 0;

  /* Implement this method to update non valued parameters */
  virtual void DoUpdateParameters() = 0;

  /* Implement this method to build the output */
  virtual void DoExecute() = 0;

  /* This method will be called after the
   * ExecuteAndWriteOutput() call to allow for cleanup. Default
   * implementation does nothing */
  virtual void AfterExecuteAndWriteOutputs();

  virtual void DoFreeRessources(){};

  Application(const Application &) = delete;
  void operator =(const Application&) = delete;

  std::string                       m_Name;
  std::string                       m_Description;
  ParameterGroup::Pointer           m_ParameterList;
  otb::Logger::Pointer              m_Logger;

  itk::ProcessObject::Pointer       m_ProgressSource;
  std::string                       m_ProgressSourceDescription;

  std::set<itk::ProcessObject::Pointer> m_Filters;

  /** Long name of the application (that can be displayed...) */
  std::string m_DocName;
  /** Long and precise application description . */
  std::string                       m_DocLongDescription;
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
  otb::Stopwatch m_Chrono;

  //rashad:: controls adding of -xml parameter. set to true by default
  bool                              m_HaveInXML;
  bool                              m_HaveOutXML;
  bool                              m_IsInXMLParsed;

  /** Flag is true when executing DoInit, DoUpdateParameters or DoExecute */
  bool m_IsInPrivateDo;

  /** Flag to check if Execute has already been called */
  bool m_ExecuteDone;

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
//#include "otbWrapperApplication.hxx"
//#endif


#endif // otbWrapperApplication_h_
