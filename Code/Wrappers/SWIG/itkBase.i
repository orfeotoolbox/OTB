// kindly stolen from the build files of OTB-Wrapping (Typedefs/itkBase.includes, Typedefs/wrap_ITKCommonBase.i)

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


 class itkLightObject_Pointer {
   public:
     itkLightObject_Pointer();
     itkLightObject_Pointer(itkLightObject_Pointer const & p);
     itkLightObject_Pointer(itkLightObject * p);
     ~itkLightObject_Pointer();
     itkLightObject * operator->() const;
     bool IsNotNull() const;
     bool IsNull() const;
     itkLightObject * GetPointer() const;
     bool operator<(itkLightObject_Pointer const & r) const;
     bool operator>(itkLightObject_Pointer const & r) const;
     bool operator<=(itkLightObject_Pointer const & r) const;
     bool operator>=(itkLightObject_Pointer const & r) const;
     itkLightObject_Pointer & operator=(itkLightObject_Pointer const & r);
     itkLightObject_Pointer & operator=(itkLightObject * r);
     itkLightObject * Print(std::ostream & os) const;
   private:
     void Register();
     void UnRegister();
   protected:
 };

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

 class itkObject_Pointer {
   public:
     itkObject_Pointer();
     itkObject_Pointer(itkObject_Pointer const & p);
     itkObject_Pointer(itkObject * p);
     ~itkObject_Pointer();
     itkObject * operator->() const;
     bool IsNotNull() const;
     bool IsNull() const;
     itkObject * GetPointer() const;
     bool operator<(itkObject_Pointer const & r) const;
     bool operator>(itkObject_Pointer const & r) const;
     bool operator<=(itkObject_Pointer const & r) const;
     bool operator>=(itkObject_Pointer const & r) const;
     itkObject_Pointer & operator=(itkObject_Pointer const & r);
     itkObject_Pointer & operator=(itkObject * r);
     itkObject * Print(std::ostream & os) const;
   private:
     void Register();
     void UnRegister();
   protected:
 };

 

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


 class itkObjectFactoryBase_Pointer {
   public:
     itkObjectFactoryBase_Pointer();
     itkObjectFactoryBase_Pointer(itkObjectFactoryBase_Pointer const & p);
     itkObjectFactoryBase_Pointer(itkObjectFactoryBase * p);
     ~itkObjectFactoryBase_Pointer();
     itkObjectFactoryBase * operator->() const;
     bool IsNotNull() const;
     bool IsNull() const;
     itkObjectFactoryBase * GetPointer() const;
     bool operator<(itkObjectFactoryBase_Pointer const & r) const;
     bool operator>(itkObjectFactoryBase_Pointer const & r) const;
     bool operator<=(itkObjectFactoryBase_Pointer const & r) const;
     bool operator>=(itkObjectFactoryBase_Pointer const & r) const;
     itkObjectFactoryBase_Pointer & operator=(itkObjectFactoryBase_Pointer const & r);
     itkObjectFactoryBase_Pointer & operator=(itkObjectFactoryBase * r);
     itkObjectFactoryBase * Print(std::ostream & os) const;
   private:
     void Register();
     void UnRegister();
   protected:
 };


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

 class itkCommand_Pointer {
   public:
     itkCommand_Pointer();
     itkCommand_Pointer(itkCommand_Pointer const & p);
     itkCommand_Pointer(itkCommand * p);
     ~itkCommand_Pointer();
     itkCommand * operator->() const;
     bool IsNotNull() const;
     bool IsNull() const;
     itkCommand * GetPointer() const;
     bool operator<(itkCommand_Pointer const & r) const;
     bool operator>(itkCommand_Pointer const & r) const;
     bool operator<=(itkCommand_Pointer const & r) const;
     bool operator>=(itkCommand_Pointer const & r) const;
     itkCommand_Pointer & operator=(itkCommand_Pointer const & r);
     itkCommand_Pointer & operator=(itkCommand * r);
     itkCommand * Print(std::ostream & os) const;
   private:
     void Register();
     void UnRegister();
   protected:
 };


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

