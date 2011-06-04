%include "itkMacro.i"
%include "RefCountMacro.i"


 %module otbApplication
 %{
#include "otbWrapperSWIGIncludes.h"
#include "itkBase.includes"
 %}

%include "itkBase.i"

namespace otb
{
namespace Wrapper
{

enum DefaultValueMode
  {
    /** 
     * This parameter has no default behaviour and should be set by
     * the user. 
     */
    UNKNOWN,
    /**
     * The default value of this parameter can be estimated from
     * other parameters.
     */
    RELATIVE,

    /** 
     * Tge default value of this parameter is not depending of any
     * other parameter.
     */
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

  typedef Parameter                     Self;
  typedef itkObject                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  static Parameter_Pointer New(void);

  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

  itkSetStringMacro(Description);
  itkGetStringMacro(Description);

  itkSetStringMacro(Key);
  itkGetStringMacro(Key);

  itkSetMacro(Mandatory,bool);
  itkGetMacro(Mandatory,bool);
  itkBooleanMacro(Mandatory);

  itkSetEnumMacro(DefaultValueMode, otb::Wrapper::DefaultValueMode);
  itkGetEnumMacro(DefaultValueMode, otb::Wrapper::DefaultValueMode);

  virtual void Reset();
  
protected:
  Parameter();
  //~Parameter();
  
private:
    Parameter(const Parameter &);
    void operator =(const Parameter&);

};

class Parameter_Pointer
{
   public:
     Parameter_Pointer();
     Parameter_Pointer(Parameter_Pointer const & p);
     Parameter_Pointer(Parameter * p);
     ~Parameter_Pointer();
     Parameter * operator->() const;
     bool IsNotNull() const;
     bool IsNull() const;
     Parameter * GetPointer() const;
     bool operator<(Parameter_Pointer const & r) const;
     bool operator>(Parameter_Pointer const & r) const;
     bool operator<=(Parameter_Pointer const & r) const;
     bool operator>=(Parameter_Pointer const & r) const;
     Parameter_Pointer & operator=(Parameter_Pointer const & r);
     Parameter_Pointer & operator=(Parameter * r);
     Parameter * Print(std::ostream & os) const;
   private:
     void Register();
     void UnRegister();
   protected:
};


DECLARE_REF_COUNT_CLASS( Parameter )

