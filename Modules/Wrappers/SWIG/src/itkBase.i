/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


// kindly stolen from the build files of OTB-Wrapping (Typedefs/itkBase.includes, Typedefs/wrap_ITKCommonBase.i)

%include <exception.i>
%include <typemaps.i>


// Customize exception handling
%exception {
  try {
    $action
  } catch(otb::ImageFileReaderException& err) {
    std::ostringstream oss;
    oss << "Cannot open image " << err.m_Filename + ". " + err.GetDescription();
    SWIG_exception( SWIG_RuntimeError, oss.str().c_str() );
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

// BASIC ITK TYPES WRAPPING
%include "itkFloatTypes.h"
%include "itkIntTypes.h"

// Some code from STL
// Do not wrap if not necessary as it really slows down compilation

%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>
#if SWIGPYTHON
%include <std_complex.i>
#endif

%template(vectorstring)     std::vector< std::string >;
%template(mapstringstring)  std::map< std::string, std::string >;
%template(vectorbool)       std::vector<bool>;
%template(vectordouble)     std::vector<double>;

#if SWIGPYTHON
%extend std::map< std::string, std::string >
{
  %pythoncode
  {
    def __str__(self):
      ret = "{"
      for key in self:
        ret += str(key)+":"+str(self[key])+", "
      if len(ret) == 1:
        ret += ", "
      return ret[:-2]+"}"
  }
};
#endif

//---------------- ITK classes ------------------------------------

namespace itk
{

template <unsigned int VDim = 2>
class Size
{
public:
  Size();
  ~Size();
  void Fill(unsigned long val);
  SizeValueType GetElement(unsigned long element) const;
  void SetElement(unsigned long element, SizeValueType val);
  static unsigned int GetSizeDimension();
};

template <unsigned int VDim = 2>
class Index
{
public:
  Index();
  ~Index();
  void Fill(signed long val);
  IndexValueType GetElement(unsigned long element) const;
  void SetElement(unsigned long element, IndexValueType val);
  static unsigned int GetIndexDimension();
};

template <unsigned int VDim>
class ImageRegion
{
public:
  ImageRegion();
  ImageRegion(const Index<VDim> &index, const Size<VDim> &size);
  ~ImageRegion();
  void SetIndex(const Index<VDim> &index);
  void SetSize(const Size<VDim> &size);
  void SetUpperIndex(const Index<VDim> &idx);
  Index<VDim> GetUpperIndex() const;
  const Index<VDim> & GetIndex() const;
  const Size<VDim> & GetSize() const;
  bool IsInside(const Index<VDim> & index) const;
  void SetSize(unsigned int i, SizeValueType val);
  SizeValueType GetSize(unsigned int i) const;
  void SetIndex(unsigned int i, IndexValueType val);
  IndexValueType GetIndex(unsigned int i) const;
};

template <typename TValue, unsigned int VLength = 3>
class FixedArray
{
public:
  FixedArray();
  ~FixedArray();
  unsigned int Size();
  void SetElement(unsigned short idx, const TValue &val);
  const TValue & GetElement(unsigned short idx);
};

template <typename TValue, unsigned int NDim = 3>
class Vector: public FixedArray<TValue,NDim>
{
public: 
  Vector();
  ~Vector();
  typedef NumericTraits<TValue>::RealType RealValueType;
  RealValueType GetNorm() const;
  RealValueType GetSquaredNorm() const;
  RealValueType Normalize();
};

template <typename TCoord, unsigned int NDim = 3>
class Point: public FixedArray<TCoord,NDim>
{
public:
  Point();
  ~Point();
};

// Instanciate the needed templates
%template(itkSize) Size<2>;
%template(itkIndex) Index<2>;
%template(itkRegion) ImageRegion<2>;
%template(itkFixedArray) FixedArray<SpacePrecisionType,2>;
%template(itkVector) Vector<SpacePrecisionType,2>;
%template(itkPoint) Point<SpacePrecisionType,2>;

} // end of namespace itk

#if SWIGPYTHON

%define WRAP_AS_LIST(N, T...)
%extend T
  {
  %pythoncode
    {
    def __str__(self):
      ret = "["
      for index in range(N):
        ret += str(self.GetElement(index))+","
      ret = ret[:-1] + "]"
      return ret
    def __len__(self):
      return N
    def __getitem__(self,idx):
      if idx >= N or idx < 0:
        raise IndexError('Index outside [0,'+str(N-1)+']')
      return self.GetElement(idx)
    def __setitem__(self,idx,val):
      if idx >= N or idx < 0:
        raise IndexError('Index outside [0,'+str(N-1)+']')
      return self.SetElement(idx,val)
    }
  };
%enddef

namespace itk
{
WRAP_AS_LIST(2, Size<2>)
WRAP_AS_LIST(2, Index<2>)
WRAP_AS_LIST(2, FixedArray<SpacePrecisionType,2>)
WRAP_AS_LIST(2, Vector<SpacePrecisionType,2>)
WRAP_AS_LIST(2, Point<SpacePrecisionType,2>)

%extend ImageRegion<2>
{
  %pythoncode
    {
    def __str__(self):
      return "{index:"+str(self.GetIndex())+", size:"+str(self.GetSize())+"}"
    def __len__(self):
      return 2
    def keys(self):
      return ['index', 'size']
    def __getitem__(self,key):
      if key == 'index':
        return self.GetIndex()
      elif key == 'size':
        return self.GetSize()
      else:
        raise IndexError('Key not in ["index","size"]')
    def __setitem__(self,key,val):
      if key == 'index':
        self.SetIndex(val)
      elif key == 'size':
        self.SetSize(val)
      else:
        raise IndexError('Key not in ["index","size"]')
    }
};
} // end of namespace itk
#endif

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

class itkMetaDataObjectBase : public itkLightObject
{
public:
  virtual const std::type_info & GetMetaDataObjectTypeInfo() const;
  virtual const char * GetMetaDataObjectTypeName() const;
  virtual const char * GetNameOfClass() const;
  virtual void Print(std::ostream &os) const;
protected:
  itkMetaDataObjectBase();
#if SWIGJAVA
  ~itkMetaDataObjectBase();
#endif
};
DECLARE_REF_COUNT_CLASS(itkMetaDataObjectBase)

 class itkMetaDataDictionary {
   public:
     virtual void Print(std::ostream & os) const;
     itkMetaDataDictionary();
     itkMetaDataDictionary(itkMetaDataDictionary const & arg0);
     ~itkMetaDataDictionary();
     std::vector< std::string > GetKeys() const;
     bool HasKey(std::string const & arg0) const;
     const itkMetaDataObjectBase* Get(const std::string &) const;
     void Set(const std::string &, itkMetaDataObjectBase *);
 };

namespace itk
{

template <typename T>
inline bool ExposeMetaData(const itkMetaDataDictionary & Dictionary, const std::string key, T & outval);

template <typename T>
inline void EncapsulateMetaData(itkMetaDataDictionary & Dictionary, const std::string & key, const T & invalue);

}
 
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

