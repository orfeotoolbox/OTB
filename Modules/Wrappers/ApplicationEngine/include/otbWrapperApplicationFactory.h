/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperApplicationFactory_h
#define otbWrapperApplicationFactory_h

#include "otbWrapperApplicationFactoryBase.h"
#include "itkVersion.h"
#include <string>

namespace otb
{
namespace Wrapper
{

template <class TApplication>
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

  void SetClassName(const char* name)
  {
    // remove namespace, only keep class name
    std::string            tmpName(name);
    std::string::size_type pos = tmpName.rfind("::");
    if (pos != std::string::npos)
    {
      tmpName = tmpName.substr(pos + 2);
    }
    m_ClassName.assign(tmpName);
  }

protected:
  ApplicationFactory()
  {
  }

  ~ApplicationFactory() override
  {
  }

  /** This method is provided by sub-classes of ObjectFactoryBase.
   * It should create the named itk object or return 0 if that object
   * is not supported by the factory implementation. */
  LightObject::Pointer CreateObject(const char* itkclassname) override
  {
    LightObject::Pointer ret;
    if (m_ClassName == itkclassname)
      ret = TApplication::New().GetPointer();

    return ret;
  }

  /** This method creates all the objects with the new class of
   * itkclass name, which are provide by this object
   */
  std::list<LightObject::Pointer> CreateAllObject(const char* itkclassname) override
  {
    const std::string               applicationClass("otbWrapperApplication");
    std::list<LightObject::Pointer> list;
    if (m_ClassName == itkclassname || applicationClass == itkclassname)
      list.push_back(TApplication::New().GetPointer());

    return list;
  }

private:
  ApplicationFactory(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string m_ClassName;
};

} // end namespace Wrapper
} // end namespace otb

#if (defined(WIN32) || defined(_WIN32))
#define OTB_APP_EXPORT __declspec(dllexport)
#else
#define OTB_APP_EXPORT
#endif

#define OTB_APPLICATION_EXPORT(ApplicationType)                                     \
  typedef otb::Wrapper::ApplicationFactory<ApplicationType> ApplicationFactoryType; \
  static ApplicationFactoryType::Pointer                    staticFactory;          \
  extern "C" {                                                                      \
  OTB_APP_EXPORT itk::ObjectFactoryBase* itkLoad()                                  \
  {                                                                                 \
    staticFactory = ApplicationFactoryType::New();                                  \
    staticFactory->SetClassName(#ApplicationType);                                  \
    return staticFactory;                                                           \
  }                                                                                 \
  }


#endif // otbWrapperApplication_h_
