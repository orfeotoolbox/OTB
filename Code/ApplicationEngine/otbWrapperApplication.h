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
#include "otbWrapperTags.h"
#include "otbWrapperParameterGroup.h"

#include "itkLogger.h"
#include "otbWrapperMacros.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperDocExampleStructure.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

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
  typedef Application                   Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** RTTI support */
  itkTypeMacro(Application, itk::Object);

  /** Set the parameter name */
  //itkSetStringMacro(Name);

  virtual void SetName( const std::string & name )
  {
    m_Name = name;
    GetDocExample()->SetApplicationName(name);
    this->Modified();
  }

  itkGetStringMacro(Name);

  /** Set the parameter description */
  itkSetStringMacro(Description);

  /** Get the parameter description */
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

  /* Set the description of a parameter */
  void SetParameterDescription(std::string paramKey, std::string dec);

  /* Enable the use of an optional parameter. Returns the previous state */
  void EnableParameter(std::string paramKey);

  /* Disable the use of an optional parameter. Returns the previous state  */
  void DisableParameter(std::string paramKey);

  /* Return the enable state of an optional parameter  */
  bool IsParameterEnabled(std::string paramKey) const;

  /* Return true if the specified parameter is mandatory */
  bool IsMandatory(std::string paramKey) const;

  /* Return true if the specified parameter was set automatically in
   * the application
   */
  bool HasAutomaticValue(std::string paramKey) const;

  /* Returns true if the parameter has an associated value provided externally
   *  (not automatically computed by the application) */
  bool HasUserValue(std::string paramKey) const;

  /* If a user value was provided clear it and update the other parameters */
  void ClearValue(std::string paramKey);

  /* Returns true if the parameter has an associated value.
   * This value can be an automatically computed value or default value,
   * or a value set externally by user */
  bool HasValue(std::string paramKey) const;

  /* Activate or deactivate the bool parameter
   */
  void SetParameterEmpty(std::string paramKey, bool active);

  /* Get active flag of parameter with key paramKey
   */
  bool GetParameterEmpty(std::string paramKey);

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
  bool IsApplicationReady();

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

  /* Set an default integer value, must used in the
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

  /* Set a default floating value, must used in the
   * DoInit when setting a value by default
   * for the parameter
   *
   * Can be called for types :
   * \li ParameterType_Float
   */
  void SetDefaultParameterFloat(std::string parameter, float value);

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

  /* Set a string value
   *
   * Can be called for types :
   * \li ParameterType_String
   * \li ParameterType_InputFilename
   * \li ParameterType_OutputFilename
   * \li ParameterType_Directory
   * \li ParameterType_Choice
   * \li ParameterType_Float
   * \li ParameterType_Int
   * \li ParameterType_Radius
   * \li ParameterType_InputImageParameter
   * \li ParameterType_InputComplexImageParameter
   * \li ParameterType_InputVectorDataParameter
   * \li ParameterType_OutputImageParameter
   * \li ParameterType_OutputVectorDataParameter
   */
  void SetParameterString(std::string parameter, std::string value);

  /* Set a string value
   *
   * Can be called for types :
   * \li ParameterType_InputImageListParameter
   * \li ParameterType_InputVectorDataListParameter
   * \li ParameterType_InputFilenameListParameter
   * \li ParameterType_StringList
   */
  void SetParameterStringList(std::string parameter, std::vector<std::string> value);

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
   * \li ParameterType_InputComplexImage
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
  std::vector<std::string> GetParameterStringList(std::string parameter);

  /* Get an image value
   *
   * Can be called for types :
   * \li ParameterType_InputImage
   */
  FloatVectorImageType* GetParameterImage(std::string parameter);

#define otbGetParameterImageMacro( Image )                              \
  Image##Type * GetParameter##Image( std::string parameter )            \
    {                                                                   \
    Image##Type::Pointer ret;                                           \
    Parameter* param = GetParameterByKey(parameter);                    \
    if (dynamic_cast<InputImageParameter*>(param))                      \
      {                                                                 \
      InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param); \
      ret = paramDown->Get##Image();                                    \
      }                                                                 \
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
   * Can be called for types :
   * \li ParameterType_ComplexInputImage
   */

#define otbGetParameterComplexImageMacro( Image )                       \
  Image##Type * GetParameter##Image( std::string parameter )            \
    {                                                                   \
    Image##Type::Pointer ret;                                           \
    Parameter* param = GetParameterByKey(parameter);                    \
    if (dynamic_cast<ComplexInputImageParameter*>(param))               \
      {                                                                 \
      ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param); \
      ret = paramDown->Get##Image();                                    \
      }                                                                 \
    return ret;                                                         \
    }

  otbGetParameterComplexImageMacro(ComplexFloatImage);
  otbGetParameterComplexImageMacro(ComplexDoubleImage);

  otbGetParameterComplexImageMacro(ComplexFloatVectorImage);
  otbGetParameterComplexImageMacro(ComplexDoubleVectorImage);

  /* Get an image list value
   *
   * Can be called for types :
   * \li ParameterType_InputImageList
   */
  FloatVectorImageListType* GetParameterImageList(std::string parameter);

  /* Get a complex image value
   *
   * Can be called for types :
   * \li ParameterType_InputComplexImage
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
   * \li ParameterType_InputComplexImage
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

  itk::Logger* GetLogger();

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
  std::vector<std::string> GetDocTags(){
    return m_DocTags;
  }
  void SetDocTags( std::vector<std::string> val ){
    m_DocTags = val;
    this->Modified();
  }

  void AddDocTag( const std::string & tag )
  {
    for (unsigned int i=0; i<m_DocTags.size(); i++)
      {
      if (m_DocTags[i].compare(tag) == 0) return;
      }
    m_DocTags.push_back( tag );
    this->Modified();
  }

  DocExampleStructure::Pointer GetDocExample()
  {
    if (! IsInitialized())
      {
      Init();
      }

    return m_DocExample;
  }

  unsigned int GetNumberOfExamples()
  {
    return GetDocExample()->GetNbOfExamples();
  }

  std::string GetExampleComment(unsigned int id)
  {
    return GetDocExample()->GetExampleComment(id);
  }

  unsigned int GetExampleNumberOfParameters(unsigned int id)
  {
    return GetDocExample()->GetNumberOfParameters(id);
  }

  std::string GetExampleParameterKey(unsigned int exId, unsigned int paramId)
  {
    return GetDocExample()->GetParameterKey(paramId, exId);
  }

  std::string GetExampleParameterValue(unsigned int exId, unsigned int paramId)
  {
    return GetDocExample()->GetParameterValue(paramId, exId);
  }

  void SetDocExampleParameterValue( const std::string key, const std::string value, unsigned int exId=0 )
  {
    GetDocExample()->AddParameter( key, value, exId );
    this->Modified();
  }

  void SetExampleComment( const std::string & comm, unsigned int i )
  {
    GetDocExample()->SetExampleComment( comm, i );
    this->Modified();
  }

  unsigned int AddExample( const std::string & comm="" )
  {
    unsigned int id = GetDocExample()->AddExample( comm );
    this->Modified();
    return id;
  }

  std::string GetCLExample()
  {
    return GetDocExample()->GenerateCLExample();
  }

  std::string GetHtmlExample()
  {
    return GetDocExample()->GenerateHtmlExample();
  }

  /** Return all parameters which role is Role_Output in a vector of pairs that contains the
  * parameter key and its value.
  */
  std::vector< std::pair<std::string, std::string> > GetOutputParametersSumUp();

   /** If need to force readxml more than once in application */
   void ForceInXMLParseFlag()
   {
     m_IsInXMLParsed = false;
   }

protected:
  /** Constructor */
  Application();

  /** Destructor */
  virtual ~Application();

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
    if (dynamic_cast<InputImageParameter*>(param))
      {
      InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
      ret = paramDown->GetImage<TImageType>();
      }

    //TODO: exception if not found ?
    return ret;
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

  Application(const Application &); //purposely not implemented
  void operator =(const Application&); //purposely not implemented

  std::string                       m_Name;
  std::string                       m_Description;
  ParameterGroup::Pointer           m_ParameterList;
  itk::Logger::Pointer              m_Logger;

  itk::ProcessObject::Pointer       m_ProgressSource;
  std::string                       m_ProgressSourceDescription;

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

  //rashad:: controls adding of -xml parameter. set to true by default
  bool                              m_HaveInXML;
  bool                              m_HaveOutXML;
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

#endif // __otbWrapperApplication_h_
