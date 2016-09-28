#ifndef otbWrapperProxyParameter_h
#define otbWrapperProxyParameter_h

#include "itkObject.h"
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

class ProxyParameter : public Parameter
{
public:
  typedef ProxyParameter                       Self;
  typedef Parameter                            Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ProxyParameter, Parameter);

  void SetInternalParameter(Parameter *param)
    {
    m_InternalParam = param;
    }

  Parameter* GetInternalParameter(void)
    {
    return m_InternalParam.GetPointer();
    }

protected:
  ProxyParameter() {}
  ~ProxyParameter() ITK_OVERRIDE {}

private:
  ProxyParameter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename Parameter::Pointer m_InternalParam;
};

}
}

//#ifndef OTB_MANUAL_INSTANTIATION
//#include "otbWrapperProxyParameter.txx"
//#endif

#endif
