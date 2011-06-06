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
#include "otbAddition.h"
#include "otbWrapperNumericalParameter.h"
#include "itkVersion.h"

namespace otb
{
namespace Wrapper
{

Addition::Addition()
{
  this->SetName("Addition");
  this->SetDescription("This is a simple application which adds two numbers and print the sum on standard output");
}

Addition::~Addition()
{
}

void Addition::DoCreateParameters()
{
  std::cout << "Addition::DoCreateParameters" << std::endl;
  ParameterGroup* params = GetParameterList();

  otb::Wrapper::FloatParameter::Pointer a = otb::Wrapper::FloatParameter::New();
  a->SetKey("a");
  a->SetName("First number");

  otb::Wrapper::FloatParameter::Pointer b = otb::Wrapper::FloatParameter::New();
  b->SetKey("b");
  b->SetName("Second number");

  params->AddParameter(a.GetPointer());
  params->AddParameter(b.GetPointer());
}

void Addition::DoUpdateParameters()
{
  std::cout << "Addition::DoUpdateParameters" << std::endl;
}

void Addition::DoExecute()
{
  std::cout << "Addition::DoExecute" << std::endl;

  ParameterGroup* params = GetParameterList();

  otb::Wrapper::FloatParameter* pa = dynamic_cast<otb::Wrapper::FloatParameter*>(params->GetParameterByKey("a").GetPointer());
  boost::any avalue = pa->GetAnyValue();
  float a = boost::any_cast<float>(avalue);

  otb::Wrapper::FloatParameter* pb = dynamic_cast<otb::Wrapper::FloatParameter*>(params->GetParameterByKey("b").GetPointer());
  boost::any bvalue = pb->GetAnyValue();
  float b = boost::any_cast<float>(bvalue);

  std::cout << a + b << std::endl;
}

class AdditionFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef AdditionFactory              Self;
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
    return "Addition";
    }

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AdditionFactory, itk::ObjectFactoryBase);

protected:
  AdditionFactory()
  {

  }

  virtual ~AdditionFactory()
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
      ret = Addition::New().GetPointer();

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
      list.push_back(Addition::New().GetPointer());

    return list;
  }

private:
  AdditionFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};


}

}

static otb::Wrapper::AdditionFactory::Pointer staticFactory;
itk::ObjectFactoryBase* itkLoad()
{
  staticFactory = otb::Wrapper::AdditionFactory::New();
  return staticFactory;
}
