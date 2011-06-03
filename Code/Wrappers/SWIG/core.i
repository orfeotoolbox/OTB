
%include <exception.i>
%include <typemaps.i>


// This macro replaces the use of itk::SmartPointer.
// class_name is class name without namespace qualifiers.
// Reference: http://www.nabble.com/attachment/16653644/0/SwigRefCount.i
%define DECLARE_REF_COUNT_CLASS(class_name)

  // pointers and references
  %typemap(out) class_name *, class_name & {
    // always tell SWIG_NewPointerObj we're the owner
    $result = SWIG_NewPointerObj((void *) $1, $1_descriptor, 1);
    if ($1) {
      $1->Register();
    }
  }

  // transform smart pointers in raw pointers
  %typemap(out) class_name##_Pointer {
    // get the raw pointer from the smart pointer
    class_name * ptr = $1;
    // always tell SWIG_NewPointerObj we're the owner
    $result = SWIG_NewPointerObj((void *) ptr, $descriptor(class_name *), 1);
    // register the object, it it exists
    if (ptr) {
      ptr->Register();
    }
  }

  // transform smart pointers in raw pointers
  %typemap(out) class_name##_Pointer & {
    // get the raw pointer from the smart pointer
    class_name * ptr = *$1;
    // always tell SWIG_NewPointerObj we're the owner
    $result = SWIG_NewPointerObj((void *) ptr, $descriptor(class_name *), 1);
    // register the object, it it exists
    if (ptr) {
      ptr->Register();
    }
  }

  // make "deletion" in scripting language just decrement ref. count
  %extend class_name {
    public:
    ~class_name() {self->UnRegister();};
  }

  %ignore class_name::~class_name;
  
  %ignore class_name##_Pointer;

  %pythoncode {
    def class_name##_New():
      return class_name.New()
  }
%enddef


 %module core
 %{
#include "otbWrapperSWIGIncludes.h"
 %}


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

  /** Standard class typedef */
  typedef Parameter                     Self;
  typedef itkObject                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  static Parameter_Pointer New(void);

  /** Set the parameter name */
  void SetName (const char*);

  /** Get the parameter name */
  const char* GetName() const;

  /** Set the parameter description */
  void SetDescription (const char*);

  /** Get the parameter description */
  const char* GetDescription() const;

  /** Set the parameter key */
  void SetKey (const char*);

  /** Get the parameter key */
  const char* GetKey() const;

  /** Set the parameter mandatory flag */
  void SetMandatory (bool);

  /** Get the parameter mandatory flag */
  bool GetMandatory();

  /** Toogle the parameter mandatory flag */
  void MandatoryOn ();
  void MandatoryOff ();

  /** Set the default value mode */
  virtual void SetDefaultValueMode (const otb::Wrapper::DefaultValueMode _arg);
  
  /** Get the default value mode */
  otb::Wrapper::DefaultValueMode GetDefaultValueMode() const;

  /** Reset to the the default value. Default implementation does
   * nothing 
   */
  virtual void Reset();
  
 protected:

  Parameter();

  /** Destructor */
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
//     bool operator<=(Parameter_Pointer const & r) const;
//     bool operator>=(Parameter_Pointer const & r) const;
//     Parameter_Pointer & operator=(Parameter_Pointer const & r);
//     Parameter_Pointer & operator=(Parameter * r);
     Parameter * Print(std::ostream & os) const;
   private:
     void Register();
     void UnRegister();
   protected:
};


DECLARE_REF_COUNT_CLASS( Parameter )

