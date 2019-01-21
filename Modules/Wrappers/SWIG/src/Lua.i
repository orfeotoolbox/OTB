/*
 * Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if SWIGLUA


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

#endif

