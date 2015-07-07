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
#ifndef __otbWrapperApplicationFactory_h
#define __otbWrapperApplicationFactory_h

#include "otbWrapperApplicationFactoryBase.h"
#include "itkVersion.h"

namespace otb
{
namespace Wrapper
{

template < class TApplication >
class ITK_ABI_EXPORT ApplicationFactory : public ApplicationFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef ApplicationFactory            Self;
  typedef ApplicationFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const
    {
    return ITK_SOURCE_VERSION;
    }

  virtual const char* GetDescription(void) const
    {
    return "ApplicationFactory";
    }

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ApplicationFactory, ApplicationFactoryBase);
  
  void SetClassName(const char* name)
  {
    // remove namespace, only keep class name
    std::string tmpName(name);
    std::string::size_type pos = tmpName.rfind("::");
    if (pos != std::string::npos)
      {
      tmpName = tmpName.substr(pos+2);
      }
    m_ClassName.assign(tmpName);
  }

protected:
  ApplicationFactory()
  {

  }

  virtual ~ApplicationFactory()
  {

  }

  /** This method is provided by sub-classes of ObjectFactoryBase.
   * It should create the named itk object or return 0 if that object
   * is not supported by the factory implementation. */
  virtual LightObject::Pointer CreateObject(const char* itkclassname )
  {
    LightObject::Pointer ret;
    if ( m_ClassName == itkclassname)
      ret = TApplication::New().GetPointer();

    return ret;
  }

  /** This method creates all the objects with the class overide of
   * itkclass name, which are provide by this object
   */
  virtual std::list<LightObject::Pointer>
  CreateAllObject(const char* itkclassname)
  {
    const std::string applicationClass("otbWrapperApplication");
    std::list<LightObject::Pointer> list;
    if ( m_ClassName == itkclassname ||
         applicationClass == itkclassname )
      list.push_back(TApplication::New().GetPointer());

    return list;
  }

private:
  ApplicationFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  
  std::string m_ClassName;
};

} // end namespace Wrapper
} //end namespace otb

#if (defined(WIN32) || defined(_WIN32))
#  define OTB_APP_EXPORT __declspec(dllexport)
#else
#  define OTB_APP_EXPORT
#endif

#define OTB_APPLICATION_EXPORT( ApplicationType )                                      \
  typedef otb::Wrapper::ApplicationFactory<ApplicationType> ApplicationFactoryType;    \
  static ApplicationFactoryType::Pointer staticFactory;                                \
  extern "C"                                                                           \
  {                                                                                    \
    OTB_APP_EXPORT itk::ObjectFactoryBase* itkLoad()                                   \
    {                                                                                  \
      staticFactory = ApplicationFactoryType::New();                                   \
      staticFactory->SetClassName(#ApplicationType);                                   \
      return staticFactory;                                                            \
    }                                                                                  \
  }


#endif // __otbWrapperApplication_h_
