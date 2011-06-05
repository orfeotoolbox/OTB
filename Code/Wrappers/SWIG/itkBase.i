// kindly stolen from the build files of OTB-Wrapping (Typedefs/itkBase.includes, Typedefs/wrap_ITKCommonBase.i)

%include "RefCountMacro.i"

%include <exception.i>
%include <typemaps.i>

%typemap(out)  unsigned char &, const unsigned char &, signed char &, const signed char &, unsigned short &, const unsigned short &, signed short &, const signed short &, unsigned int &, const unsigned int &, signed int &, const signed int &, signed long &, const signed long &, unsigned long &, const unsigned long &
  {$result = PyInt_FromLong( *$1 );}

%typemap(out) float &, const float &, double &, const double &
  {$result = PyFloat_FromDouble( *$1 );}

// ignore reference count management
%ignore Delete;
%ignore SetReferenceCount;
%ignore Register;
%ignore UnRegister;



%include <std_iostream.i>
%include <std_sstream.i>
%include <std_vector.i>
%include <std_map.i>
%include <std_list.i>
%include <std_set.i>

// some code from stl

%template(vectorstring)   std::vector< std::string >;
%template(liststring)     std::list< std::string >;

%template(vectorB)        std::vector< bool >;
%template(vectorvectorB)  std::vector< std::vector< bool > >;
%template(vectorUC)       std::vector< unsigned char >;
%template(vectorvectorUC) std::vector< std::vector< unsigned char > >;
%template(vectorUS)       std::vector< unsigned short >;
%template(vectorvectorUS) std::vector< std::vector< unsigned short > >;
%template(vectorUL)       std::vector< unsigned long >;
%template(vectorvectorUL) std::vector< std::vector< unsigned long > >;
%template(vectorSC)       std::vector< signed char >;
%template(vectorvectorSC) std::vector< std::vector< signed char > >;
%template(vectorSS)       std::vector< signed short >;
%template(vectorvectorSS) std::vector< std::vector< signed short > >;
%template(vectorSL)       std::vector< signed long >;
%template(vectorvectorSL) std::vector< std::vector< signed long > >;
%template(vectorF)        std::vector< float >;
%template(vectorvectorF)  std::vector< std::vector< float > >;
%template(vectorD)        std::vector< double >;
%template(vectorvectorD)  std::vector< std::vector< double > >;

%template(listB)          std::list< bool >;
%template(listUC)         std::list< unsigned char >;
%template(listUS)         std::list< unsigned short >;
%template(listUL)         std::list< unsigned long >;
%template(listSC)         std::list< signed char >;
%template(listSS)         std::list< signed short >;
%template(listSL)         std::list< signed long >;
%template(listF)          std::list< float >;
%template(listD)          std::list< double >;

 class itkIndent {
   public:
     ~itkIndent();
     itkIndent & operator=(itkIndent const & arg0);
     itkIndent(itkIndent const & arg0);
     static itkIndent * New();
     void Delete();
     itkIndent(int ind = 0);
     static char const * GetNameOfClass();
     itkIndent GetNextIndent();
   private:
   protected:
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
   private:
     itkLightObject(itkLightObject const & arg0);
     void operator=(itkLightObject const & arg0);
   protected:
     itkLightObject();
     ~itkLightObject();
     virtual void PrintSelf(std::ostream & os, itkIndent indent) const;
     virtual void PrintHeader(std::ostream & os, itkIndent indent) const;
     virtual void PrintTrailer(std::ostream & os, itkIndent indent) const;
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
     unsigned long AddObserver(itkEventObject const & event, itkCommand * arg1) const;
     itkCommand * GetCommand(unsigned long tag);
     void InvokeEvent(itkEventObject const & arg0);
     void InvokeEvent(itkEventObject const & arg0) const;
     void RemoveObserver(unsigned long tag);
     void RemoveAllObservers();
     bool HasObserver(itkEventObject const & event) const;
     itkMetaDataDictionary & GetMetaDataDictionary();
     itkMetaDataDictionary const & GetMetaDataDictionary() const;
     void SetMetaDataDictionary(itkMetaDataDictionary const & rhs);
   private:
     itkObject(itkObject const & arg0);
     void operator=(itkObject const & arg0);
   protected:
     itkObject();
     ~itkObject();
     virtual void PrintSelf(std::ostream & os, itkIndent indent) const;
     bool PrintObservers(std::ostream & os, itkIndent indent) const;
 };
 DECLARE_REF_COUNT_CLASS( itkObject )

 

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
   private:
     itkObjectFactoryBase(itkObjectFactoryBase const & arg0);
     void operator=(itkObjectFactoryBase const & arg0);
     static void Initialize();
     static void RegisterDefaults();
     static void LoadDynamicFactories();
     static void LoadLibrariesInPath(char const * arg0);
   protected:
     virtual void PrintSelf(std::ostream & os, itkIndent indent) const;
     void RegisterOverride(char const * classOverride, char const * overrideClassName, char const * description, bool enableFlag, itk::CreateObjectFunctionBase * createFunction);
     virtual itkLightObject_Pointer CreateObject(char const * itkclassname);
     virtual std::list< itkLightObject_Pointer > CreateAllObject(char const * itkclassname);
     itkObjectFactoryBase();
     ~itkObjectFactoryBase();
 };
 DECLARE_REF_COUNT_CLASS( itkObjectFactoryBase )


 class itkMetaDataDictionary {
   public:
     virtual void Print(std::ostream & os) const;
     itkMetaDataDictionary();
     itkMetaDataDictionary(itkMetaDataDictionary const & arg0);
     void operator=(itkMetaDataDictionary const & arg0);
     ~itkMetaDataDictionary();
     std::vector< std::string > GetKeys() const;
     itkMetaDataObjectBase_Pointer & operator[](std::string const & arg0);
     itkMetaDataObjectBase const * operator[](std::string const & arg0) const;
     bool HasKey(std::string const & arg0) const;
   private:
   protected:
 };



 class itkCommand : public itkObject {
   public:
     virtual char const * GetNameOfClass() const;
     virtual void Execute(itkObject * caller, itkEventObject const & event) = 0;
     virtual void Execute(itkObject const * caller, itkEventObject const & event) = 0;
   private:
     itkCommand(itkCommand const & arg0);
     void operator=(itkCommand const & arg0);
   protected:
     itkCommand();
     ~itkCommand();
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
   private:
     void operator=(itkEventObject const & arg0);
   protected:
     virtual void PrintSelf(std::ostream & os, itkIndent indent) const;
     virtual void PrintHeader(std::ostream & os, itkIndent indent) const;
     virtual void PrintTrailer(std::ostream & os, itkIndent indent) const;
 };


 class itkNoEvent : public itkEventObject {
   public:
     itkNoEvent();
     ~itkNoEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkNoEvent(itkNoEvent const & s);
   private:
     void operator=(itkNoEvent const & arg0);
   protected:
 };


 class itkAnyEvent : public itkEventObject {
   public:
     itkAnyEvent();
     ~itkAnyEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkAnyEvent(itkAnyEvent const & s);
   private:
     void operator=(itkAnyEvent const & arg0);
   protected:
 };


 class itkDeleteEvent : public itkAnyEvent {
   public:
     itkDeleteEvent();
     ~itkDeleteEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkDeleteEvent(itkDeleteEvent const & s);
   private:
     void operator=(itkDeleteEvent const & arg0);
   protected:
 };


 class itkEndEvent : public itkAnyEvent {
   public:
     itkEndEvent();
     ~itkEndEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkEndEvent(itkEndEvent const & s);
   private:
     void operator=(itkEndEvent const & arg0);
   protected:
 };


 class itkExitEvent : public itkAnyEvent {
   public:
     itkExitEvent();
     ~itkExitEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkExitEvent(itkExitEvent const & s);
   private:
     void operator=(itkExitEvent const & arg0);
   protected:
 };


 class itkIterationEvent : public itkAnyEvent {
   public:
     itkIterationEvent();
     ~itkIterationEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkIterationEvent(itkIterationEvent const & s);
   private:
     void operator=(itkIterationEvent const & arg0);
   protected:
 };


 class itkModifiedEvent : public itkAnyEvent {
   public:
     itkModifiedEvent();
     ~itkModifiedEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkModifiedEvent(itkModifiedEvent const & s);
   private:
     void operator=(itkModifiedEvent const & arg0);
   protected:
 };


 class itkPickEvent : public itkAnyEvent {
   public:
     itkPickEvent();
     ~itkPickEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkPickEvent(itkPickEvent const & s);
   private:
     void operator=(itkPickEvent const & arg0);
   protected:
 };


 class itkProgressEvent : public itkAnyEvent {
   public:
     itkProgressEvent();
     ~itkProgressEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkProgressEvent(itkProgressEvent const & s);
   private:
     void operator=(itkProgressEvent const & arg0);
   protected:
 };


 class itkStartEvent : public itkAnyEvent {
   public:
     itkStartEvent();
     ~itkStartEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkStartEvent(itkStartEvent const & s);
   private:
     void operator=(itkStartEvent const & arg0);
   protected:
 };


 class itkStartPickEvent : public itkPickEvent {
   public:
     itkStartPickEvent();
     ~itkStartPickEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkStartPickEvent(itkStartPickEvent const & s);
   private:
     void operator=(itkStartPickEvent const & arg0);
   protected:
 };


 class itkUserEvent : public itkAnyEvent {
   public:
     itkUserEvent();
     ~itkUserEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkUserEvent(itkUserEvent const & s);
   private:
     void operator=(itkUserEvent const & arg0);
   protected:
 };


 class itkAbortCheckEvent : public itkPickEvent {
   public:
     itkAbortCheckEvent();
     ~itkAbortCheckEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkAbortCheckEvent(itkAbortCheckEvent const & s);
   private:
     void operator=(itkAbortCheckEvent const & arg0);
   protected:
 };


 class itkEndPickEvent : public itkPickEvent {
   public:
     itkEndPickEvent();
     ~itkEndPickEvent();
     virtual char const * GetEventName() const;
     virtual bool CheckEvent(itkEventObject const * e) const;
     virtual itkEventObject * MakeObject() const;
     itkEndPickEvent(itkEndPickEvent const & s);
   private:
     void operator=(itkEndPickEvent const & arg0);
   protected:
 };

