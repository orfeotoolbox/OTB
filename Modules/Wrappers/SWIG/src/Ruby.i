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
#if SWIGRUBY

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

%ignore New;


//######################################################################
// Simulating smart pointers in SWIG
// This gets rid of wrapping ITK smart pointers.

// TODO: always tell swig we're the owner
// TODO: itk classes with no New() must be marked as abstract
%define DECLARE_REF_COUNT_CLASS(itkClass)

  class itkClass##_Pointer
  {
     public:
       itkClass##_Pointer();
       itkClass##_Pointer(itkClass##_Pointer const & p);
       itkClass##_Pointer(itkClass * p);
       ~itkClass##_Pointer();
       itkClass * operator->() const;
       bool IsNotNull() const;
       bool IsNull() const;
       itkClass * GetPointer() const;
       bool operator<(itkClass##_Pointer const & r) const;
       bool operator>(itkClass##_Pointer const & r) const;
       bool operator<=(itkClass##_Pointer const & r) const;
       bool operator>=(itkClass##_Pointer const & r) const;
       itkClass##_Pointer & operator=(itkClass##_Pointer const & r);
       itkClass##_Pointer & operator=(itkClass## * r);
//       itkClass * Print(std::ostream & os) const;
     private:
       void Register();
       void UnRegister();
     protected:
  };


  // Extend the itk classtype defined for wrapping to simulate a smart pointer in SWIG.
  // Also, make the ctor public to make the 'new' operator available in java
  %extend itkClass {
    public:
    itkClass() {
      typedef ::itk::SmartPointer<itkLightObject> Pointer;
      Pointer smtPtr = itkClass::New().GetPointer();
      itkClass *rawPtr = dynamic_cast<itkClass *>(smtPtr.GetPointer());
      rawPtr->Register();
      return rawPtr;
    };
    ~itkClass() {
      self->UnRegister();
    };
  }
/*
  %typemap(out) itkClass * {
    itkClass* ptrRaw = $1;
    if (ptrRaw) {
      ptrRaw->Register();
    }
    *(itkClass **)&$result = ptrRaw;
  }
*/  
  %typemap(out) itkClass##_Pointer {
    itkClass* ptrRaw = $1.GetPointer();
    if (ptrRaw) {
      ptrRaw->Register();
    }
    *(itkClass **)&$result = ptrRaw;
  }
  
  %typemap(out) itkClass##_Pointer & {
    itkClass* ptrRaw = (*$1).GetPointer();
    if (ptrRaw) {
      ptrRaw->Register();
    }
    *(itkClass **)&$result = ptrRaw;
  }

  // Do not wrap the corresponding itkSmartPointer
  %ignore itkClass##_Pointer;
  
%enddef

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary ( "otbApplicationJava" );
  }
%}


#endif
