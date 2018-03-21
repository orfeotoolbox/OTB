/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperExpApplicationFactory_h
#define otbWrapperExpApplicationFactory_h

#include "otbWrapperExpApplicationFactoryBase.h"
#include "itkVersion.h"

namespace otb
{
namespace WrapperExp
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
  const char* GetITKSourceVersion(void) const override
    {
    return ITK_SOURCE_VERSION;
    }

  const char* GetDescription(void) const override
    {
    return "ApplicationFactory";
    }

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ApplicationFactory, ApplicationFactoryBase);
  
  void SetClassName( std::string name)
  {
    // remove namespace, only keep class name
    auto pos = name.rfind("::");
    if (pos != std::string::npos)
      {
      name.erase(0 , pos+2 );
      }
    m_ClassName = move(name);
  }

protected:
  ApplicationFactory() = default ;

  ~ApplicationFactory() override = default ;

  /** This method is provided by sub-classes of ObjectFactoryBase.
   * It should create the named itk object or return 0 if that object
   * is not supported by the factory implementation. */
  LightObject::Pointer CreateObject(const char* itkclassname ) override
  {
    LightObject::Pointer ret;
    if ( m_ClassName == itkclassname)
      ret = TApplication::New().GetPointer();

    return ret;
  }

  /** This method creates all the objects with the new class of
   * itkclass name, which are provide by this object
   */
  std::list<LightObject::Pointer>
  CreateAllObject(const char* itkclassname) override
  {
    const std::string applicationClass("otbWrapperExpApplication");
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

} // end namespace WrapperExp
} //end namespace otb

#if (defined(WIN32) || defined(_WIN32))
#  define OTB_APP_EXPORT __declspec(dllexport)
#else
#  define OTB_APP_EXPORT
#endif

#define OTB_APPLICATION_EXPORT( ApplicationType )                                      \
  typedef otb::WrapperExp::ApplicationFactory<ApplicationType> ApplicationFactoryType;    \
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


#endif // otbWrapperExpApplication_h_
