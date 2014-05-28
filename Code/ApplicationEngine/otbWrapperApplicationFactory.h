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

#include "itkObjectFactory.h"
#include "itkVersion.h"

namespace otb
{
namespace Wrapper
{

template < class TApplication >
class ITK_ABI_EXPORT ApplicationFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef ApplicationFactory            Self;
  typedef itk::ObjectFactoryBase        Superclass;
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
  itkTypeMacro(ApplicationFactory, itk::ObjectFactoryBase);

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
    const std::string classname("otbWrapperApplication");
    LightObject::Pointer ret;
    if ( classname == itkclassname )
      ret = TApplication::New().GetPointer();

    return ret;
  }

  /** This method creates all the objects with the class overide of
   * itkclass name, which are provide by this object
   */
  virtual std::list<LightObject::Pointer>
  CreateAllObject(const char* itkclassname)
  {
    const std::string classname("otbWrapperApplication");
    std::list<LightObject::Pointer> list;
    if ( classname == itkclassname )
      list.push_back(TApplication::New().GetPointer());

    return list;
  }

private:
  ApplicationFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
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
      return staticFactory;                                                            \
    }                                                                                  \
  }


#endif // __otbWrapperApplication_h_
