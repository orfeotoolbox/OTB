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
#if SWIGPYTHON

%ignore CreateAnother;
%ignore Delete;
%ignore Register;
%ignore UnRegister;
%ignore GetReferenceCount;
%ignore SetReferenceCount;
%ignore DebugOn;
%ignore DebugOff;
%ignore GetDebug;
%ignore SetDebug;
%ignore GetMTime;
%ignore Modified;
%ignore SetGlobalWarningDisplay;
%ignore GetGlobalWarningDisplay;
%ignore GlobalWarningDisplayOn;
%ignore GlobalWarningDisplayOff;



// This macro replaces the use of itk::SmartPointer.
// class_name is class name without namespace qualifiers.
// Reference: http://www.nabble.com/attachment/16653644/0/SwigRefCount.i
%define DECLARE_REF_COUNT_CLASS(class_name)

  class class_name##_Pointer
  {
     public:
       class_name##_Pointer();
       class_name##_Pointer(class_name##_Pointer const & p);
       class_name##_Pointer(class_name * p);
       ~class_name##_Pointer();
       class_name * operator->() const;
       bool IsNotNull() const;
       bool IsNull() const;
       class_name * GetPointer() const;
  };

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

  %extend class_name {
    public:

    // make "deletion" in scripting language just decrement ref. count
    ~class_name()
    {
      self->UnRegister();
    }

    }

%enddef


%typemap(out)  unsigned char &, const unsigned char &, signed char &, const signed char &, unsigned short &, const unsigned short &, signed short &, const signed short &, unsigned int &, const unsigned int &, signed int &, const signed int &, signed long &, const signed long &, unsigned long &, const unsigned long &
  {$result = PyInt_FromLong( *$1 );}

%typemap(out) float &, const float &, double &, const double &
  {$result = PyFloat_FromDouble( *$1 );}

// transform std::vector< std::string > to Python list of Python string
%typemap(out) std::vector< std::string >
  {
  $result = PyList_New( $1.size() );
  Py_ssize_t i = 0;
  for (std::vector< std::string >::const_iterator it = $1.begin();
       it != $1.end();
       ++it, ++i )
    {
    PyList_SetItem( $result, i, PyString_FromString( it->c_str() ) );
    }
  }

 // Add a ToString shortcut to all itkLightObject
 // This will make it easier for string conversion in target languages
 %extend itkLightObject {
    public:
    std::string ToString() {
      std::ostringstream o;
      self->Print(o);
      return o.str();
    }
  }

  %extend itkLightObject {
        %pythoncode %{
          def __str__( self ):
            return self.ToString()
        %}
  }
  
#endif
