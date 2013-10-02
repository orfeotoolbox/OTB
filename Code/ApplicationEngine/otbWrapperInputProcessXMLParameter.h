#ifndef __otbWrapperInputProcessXMLParameter_h
#define __otbWrapperInputProcessXMLParameter_h
#include "otbWrapperApplication.h"
#include "otbTinyXML.h"
#include <sstream>

namespace otb
{
namespace Wrapper
{

/** \class InputProcessXMLParameter
 *  \brief This class represent a xml filename parameter for the wrapper framework to load otb application.
 */
class InputProcessXMLParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputProcessXMLParameter      Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef std::string KeyType;
  typedef std::string ValueType;
  typedef std::map<KeyType,ValueType> MetaDataType;
  typedef MetaDataType::iterator MetaDataTypeIterator;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputProcessXMLParameter, Parameter);

  itkGetStringMacro(FileName);

  bool HasValue() const
  {
    if(m_FileName.empty())
      return false;
    else
      return true;
  }

  // Get Value
  //TODO otbGetObjectMemberMacro(StringParam, Value , std::string);

  void SetFileName(std::string value)
  {
    this->SetValue(value);
  }

  // Set Value
  virtual void SetValue(const std::string value)
    {
    itkDebugMacro("setting member m_FileName to " << value);
    this->m_FileName = value;
    SetActive(true);
    this->Modified();
    }

  void UpdateParameterList(Application::Pointer application);

  std::string pixelTypeToString(ImagePixelType pixType);

  ParameterType GetParameterType(const Parameter* param) const;

  int ReadMetaData();

  void PrintMetaData();

  MetaDataType GetMetaData(bool force = true);

  void UpdateMetaData(Application::Pointer application);

  int Read(Application::Pointer application);

//  void ReloadApplication(Application::Pointer application, std::string moduleName);

  void otbAppLogInfo(Application::Pointer app, std::string info);

  ValueType GetMetaDataByKey(KeyType key);

  void AddMetaData(KeyType key, ValueType value);

  std::vector<std::string> GetDocTagList()
  {
    return m_DocTagList;
  }

/* copied from Utilities/tinyXMLlib/tinyxml.cpp. Must have a FIX inside tinyxml.cpp */
  FILE* TiXmlFOpen( const char* filename, const char* mode );

protected:

  InputProcessXMLParameter();

  /** Destructor */
  virtual ~InputProcessXMLParameter();

private:

  std::string    m_FileName;
  MetaDataType m_MetaData;
  std::vector<std::string> m_DocTagList;

  InputProcessXMLParameter(const InputProcessXMLParameter &); //purposely not implemented
  void operator =(const InputProcessXMLParameter&); //purposely not implemented


}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
