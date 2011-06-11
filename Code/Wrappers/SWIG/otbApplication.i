%include "itkMacro.i"



 %module otbApplication
 %{
#include "itkBase.includes"
#include "otbWrapperSWIGIncludes.h"
 %}

%include "itkBase.i"

namespace otb
{
namespace Wrapper
{

enum DefaultValueMode
  {
    UNKNOWN,
    RELATIVE,
    ABSOLUTE
  };

}
}

/** \class Parameter
 *  \brief This class represent a parameter for the wrapper framework
 *  This class is a high level class representing a parameter for the
 *  wrapper framework. It should be subclassed to represent different
 *  kinds of parameters 
 */
class Parameter : public itkObject
{
public:
  itkNewMacro(Parameter)

  itkSetStringMacro(Name)
  itkGetStringMacro(Name)
  
  itkSetStringMacro(Description)
  itkGetStringMacro(Description)
  
  itkSetStringMacro(Key)
  itkGetStringMacro(Key)

  itkSetMacro(Mandatory,bool)
  itkGetMacro(Mandatory,bool)
  itkBooleanMacro(Mandatory)

  itkSetEnumMacro(DefaultValueMode, otb::Wrapper::DefaultValueMode)
  itkGetEnumMacro(DefaultValueMode, otb::Wrapper::DefaultValueMode)

  virtual void Reset();
  
protected:
  Parameter();
  //~Parameter();
  
private:
    Parameter(const Parameter &);
    void operator =(const Parameter&);

};
DECLARE_REF_COUNT_CLASS( Parameter )

class ParameterGroup : public Parameter
{
public:
  itkNewMacro(ParameterGroup);
  
  void AddParameter(Parameter_Pointer p);
  Parameter_Pointer GetParameterByIndex(unsigned int i);
  Parameter_Pointer GetParameterByKey(std::string name);
  unsigned int GetNumberOfParameters();

protected:
  ParameterGroup();
  virtual ~ParameterGroup();

private:
  ParameterGroup(const ParameterGroup &);
  void operator =(const ParameterGroup&);
};
DECLARE_REF_COUNT_CLASS( ParameterGroup )

class Application: public itkObject
{
public:
  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

  itkSetStringMacro(Description);
  itkGetStringMacro(Description);

  void Init();
  void UpdateParameters();
  void Execute();
  ParameterGroup* GetParameterList();
  
  void SetParameterInt(std::string parameter, int value);
  void SetParameterFloat(std::string parameter, float value);
  void SetParameterString(std::string parameter, std::string value);
  
  int GetParameterInt(std::string parameter);
  float GetParameterFloat(std::string parameter);
  std::string GetParameterString(std::string parameter);
  
protected:
  Application();
  virtual ~Application();

private:
  virtual void DoCreateParameters() = 0;
  virtual void DoUpdateParameters() = 0;
  virtual void DoExecute() = 0;

  Application(const Application &);
  void operator =(const Application&);
};
DECLARE_REF_COUNT_CLASS( Application )


class ApplicationRegistry : public itkObject
{
public:
  static std::list<std::string> GetAvailableApplications();
  static Application_Pointer CreateApplication(const std::string& name);

protected:
  ApplicationRegistry();
  virtual ~ApplicationRegistry();

private:
  ApplicationRegistry(const Self&);
  void operator=(const Self&);
};

