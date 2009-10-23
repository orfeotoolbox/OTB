#ifndef __otbBasicApplicationControllerInterface_h
#define __otbBasicApplicationControllerInterface_h

#include "itkObject.h"
#include "otbBasicApplicationModel.h"

namespace otb
{
class ITK_EXPORT BasicApplicationControllerInterface
      : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef BasicApplicationControllerInterface Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  typedef BasicApplicationModel           ModelType;

  /** Standard type macros */
  itkTypeMacro(BasicApplicationControllerInterface,Superclass);

  /** Users actions */
  virtual void OpenImage( const char * filename ) = 0;


protected:
  /** Constructor */
  BasicApplicationControllerInterface()
  {
    m_Model = ModelType::GetInstance();
  }
  /** Destructor */
  ~BasicApplicationControllerInterface() {};

  /** The instance of the model (for commodity) */
  ModelType * m_Model;

private:
  BasicApplicationControllerInterface(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
} // end namespace otb

#endif
