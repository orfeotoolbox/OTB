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

// kindly stolen from the build files of OTB-Wrapping (Typedefs/itkBase.includes, Typedefs/wrap_ITKCommonBase.i)

%include <exception.i>
%include <typemaps.i>


// Customize exception handling
%exception {
  try {
    $action
  } catch( itk::ExceptionObject &ex ) {
    std::ostringstream oss;
    oss << "Exception thrown in otbApplication $symname: " << ex.what();
    SWIG_exception( SWIG_RuntimeError, oss.str().c_str() );
  } catch( const std::exception &ex ) {
    SWIG_exception( SWIG_RuntimeError, ex.what() );
  } catch( ... ) {
    SWIG_exception( SWIG_UnknownError, "Unknown exception thrown in otbApplication $symname" );
  }
}

// Some code from STL
// Do not wrap if not necessary as it really slows down compilation

%include <std_string.i>
%include <std_vector.i>

%template(vectorstring)   std::vector< std::string >;

class itkIndent {
   public:
     static itkIndent * New();
     void Delete();
     itkIndent(int ind = 0);
     static char const * GetNameOfClass();
     itkIndent GetNextIndent();
 };

 class itkLightObject {
   public:
     static itkLightObject_Pointer New();
     virtual itkLightObject_Pointer CreateAnother() const;
     virtual void Delete();
     virtual char const * GetNameOfClass() const;
     void Print(std::ostream & os, itkIndent indent = 0) const;
     static void BreakOnError();
     virtual void Register() const;
     virtual void UnRegister() const;
     virtual int GetReferenceCount() const;
     virtual void SetReferenceCount(int arg0);
   protected:
     itkLightObject();
#if SWIGJAVA
     ~itkLightObject();
#endif
 };
 DECLARE_REF_COUNT_CLASS( itkLightObject )
 
 class itkObject : public itkLightObject {
   public:
     static itkObject_Pointer New();
     virtual itkLightObject_Pointer CreateAnother() const;
     virtual char const * GetNameOfClass() const;
     virtual void DebugOn() const;
     virtual void DebugOff() const;
     bool GetDebug() const;
     void SetDebug(bool debugFlag) const;
     virtual unsigned long GetMTime() const;
     virtual void Modified() const;
     virtual void Register() const;
     virtual void UnRegister() const;
     virtual void SetReferenceCount(int arg0);
     static void SetGlobalWarningDisplay(bool flag);
     static bool GetGlobalWarningDisplay();
     static void GlobalWarningDisplayOn();
     static void GlobalWarningDisplayOff();
     unsigned long AddObserver(itkEventObject const & event, itkCommand * arg1);
     itkCommand * GetCommand(unsigned long tag);
     void InvokeEvent(itkEventObject const & arg0);
     void RemoveObserver(unsigned long tag);
     void RemoveAllObservers();
     bool HasObserver(itkEventObject const & event) const;
     itkMetaDataDictionary & GetMetaDataDictionary();
     void SetMetaDataDictionary(itkMetaDataDictionary const & rhs);
   protected:
     itkObject();
#if SWIGJAVA
     ~itkObject();
#endif
 };
 DECLARE_REF_COUNT_CLASS( itkObject )

 class itkProcessObject : public itkObject {
   public:
     virtual itkLightObject_Pointer CreateAnother() const;
     const float& GetProgress();
   protected:
     itkProcessObject();
#if SWIGJAVA
     ~itkProcessObject();
#endif
 };
 DECLARE_REF_COUNT_CLASS( itkProcessObject )

 class itkObjectFactoryBase : public itkObject {
   public:
     virtual char const * GetNameOfClass() const;
     static itkLightObject_Pointer CreateInstance(char const * itkclassname);
     static std::list< itkLightObject_Pointer > CreateAllInstance(char const * itkclassname);
     static void ReHash();
     static void RegisterFactory(itkObjectFactoryBase * arg0);
     static void UnRegisterFactory(itkObjectFactoryBase * arg0);
     static void UnRegisterAllFactories();
     static std::list< itkObjectFactoryBase* > GetRegisteredFactories();
     virtual char const * GetITKSourceVersion() const = 0;
     virtual char const * GetDescription() const = 0;
     virtual std::list< std::string > GetClassOverrideNames();
     virtual std::list< std::string > GetClassOverrideWithNames();
     virtual std::list< std::string > GetClassOverrideDescriptions();
     virtual std::list< bool > GetEnableFlags();
     virtual void SetEnableFlag(bool flag, char const * className, char const * subclassName);
     virtual bool GetEnableFlag(char const * className, char const * subclassName);
     virtual void Disable(char const * className);
     char const * GetLibraryPath();
   protected:
     itkObjectFactoryBase();
#if SWIGJAVA
     ~itkObjectFactoryBase();
#endif
 };
 DECLARE_REF_COUNT_CLASS( itkObjectFactoryBase )

 class itkMetaDataDictionary {
   public:
     virtual void Print(std::ostream & os) const;
     itkMetaDataDictionary();
     itkMetaDataDictionary(itkMetaDataDictionary const & arg0);
     ~itkMetaDataDictionary();
     std::vector< std::string > GetKeys() const;
     bool HasKey(std::string const & arg0) const;
 };
 
 class itkCommand : public itkObject {
   public:
     virtual char const * GetNameOfClass() const;
     virtual void Execute(itkObject * caller, itkEventObject const & event) = 0;
   protected:
     itkCommand();
#if SWIGJAVA
     ~itkCommand();
#endif
 };
 DECLARE_REF_COUNT_CLASS( itkCommand )

 class itkEventObject {
   public:
     itkEventObject();
     itkEventObject(itkEventObject const & arg0);
     ~itkEventObject();
     virtual itkEventObject * MakeObject() const = 0;
     virtual void Print(std::ostream & os) const;
     virtual char const * GetEventName() const = 0;
     virtual bool CheckEvent(itkEventObject const * arg0) const = 0;
 };

%define DECLARE_itkEventObject_CLASS(class_name, superclass_name)

 class class_name : public superclass_name {
   public:
     class_name();
     ~class_name();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     class_name(class_name const & s);
 };

%enddef

DECLARE_itkEventObject_CLASS(itkNoEvent,         itkEventObject)
DECLARE_itkEventObject_CLASS(itkAnyEvent,        itkEventObject)
DECLARE_itkEventObject_CLASS(itkDeleteEvent,     itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkEndEvent,        itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkExitEvent,       itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkIterationEvent,  itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkModifiedEvent,   itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkPickEvent,       itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkProgressEvent,   itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkStartEvent,      itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkStartPickEvent,  itkPickEvent)
DECLARE_itkEventObject_CLASS(itkUserEvent,       itkAnyEvent)
DECLARE_itkEventObject_CLASS(itkAbortCheckEvent, itkPickEvent)
DECLARE_itkEventObject_CLASS(itkEndPickEvent,    itkPickEvent)

