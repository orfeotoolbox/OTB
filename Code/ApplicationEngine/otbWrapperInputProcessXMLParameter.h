#ifndef __otbWrapperInputProcessXMLParameter_h
#define __otbWrapperInputProcessXMLParameter_h
#include "otbWrapperApplication.h"
#include "otbTinyXML.h"

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
  
  const std::string GetChildNodeTextOf(TiXmlElement *parentElement, std::string key);

  std::string pixelTypeToString(ImagePixelType pixType);

  ParameterType GetParameterType(const Parameter* param) const;

  void Read(Application::Pointer application);
  
protected:

  InputProcessXMLParameter();

  /** Destructor */
  virtual ~InputProcessXMLParameter();

private:

  std::string    m_FileName;

  InputProcessXMLParameter(const InputProcessXMLParameter &); //purposely not implemented
  void operator =(const InputProcessXMLParameter&); //purposely not implemented


}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
