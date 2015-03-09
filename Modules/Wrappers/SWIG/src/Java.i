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
#if SWIGJAVA

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
     private:
       void Register();
       void UnRegister();
     protected:
  };
/*
  %extend itkClass {
    public:
    itkClass() {
      typedef ::itk::SmartPointer<itkLightObject> Pointer;
      Pointer smtPtr = itkClass::New().GetPointer();
      itkClass *rawPtr = dynamic_cast<itkClass *>(smtPtr.GetPointer());
      rawPtr->Register();
      return rawPtr;
    }
    ~itkClass() {
      self->UnRegister();
    }
  }

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
  //%ignore itkClass##_Pointer;
*/
%enddef

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary ( "otbApplicationJava" );
  }
%}

%rename(CreateApplicationInternal) Registry::CreateApplication(const std::string& name);

%typemap(javacode) Registry %{
  
  public static Application CreateApplication(String name) {
    Application_Pointer app = CreateApplicationInternal(name);
    return app.__deref__();
  }
%}

/* This tells SWIG to treat char ** as a special case when used as a parameter
   in a function call */
%typemap(in) std::vector< std::string > {
    int i = 0;
    int size = jenv->GetArrayLength($input);
    for (i = 0; i<size; i++) {
        jstring j_string = (jstring)jenv->GetObjectArrayElement($input, i);
        const char * c_string = jenv->GetStringUTFChars(j_string, 0);
        $1.push_back(c_string);
        jenv->ReleaseStringUTFChars(j_string, c_string);
        jenv->DeleteLocalRef(j_string);
    }
    
}

// transform std::vector< std::string > to a more standard String Array
%typemap(out) std::vector< std::string >
  {
    int i;
    int len=$1.size();
    jstring temp_string;
    const jclass clazz = jenv->FindClass("java/lang/String");

    jresult = jenv->NewObjectArray(len, clazz, NULL);
    /* exception checking omitted */

    for (i=0; i<len; i++)
    {
      temp_string = jenv->NewStringUTF($1[i].c_str());
      jenv->SetObjectArrayElement(jresult, i, temp_string);
      jenv->DeleteLocalRef(temp_string);
    }
  }

%typemap(jni) std::vector< std::string > "jobjectArray"
%typemap(jtype) std::vector< std::string > "String[]"
%typemap(jstype) std::vector< std::string > "String[]"

%typemap(javain) std::vector< std::string > "$javainput"
%typemap(javaout) std::vector< std::string > {
    return $jnicall;
  }


#endif
