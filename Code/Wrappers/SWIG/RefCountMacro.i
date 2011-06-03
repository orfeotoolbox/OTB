
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
