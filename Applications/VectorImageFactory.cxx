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
#include "itkObjectFactory.h"
#include "itkVersion.h"
#include "otbWrapperTypes.h"


//template <class TPixel, unsigned int VImageDimension = 2>
class ITK_EXPORT MyVectorImage : public otb::VectorImage<float, 2>
{
public:
  /** Standard class typedefs. */
  typedef MyVectorImage                       Self;
//  typedef otb::VectorImage<TPixel, VImageDimension> Superclass;
  typedef otb::VectorImage<float, 2>          Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  typedef itk::WeakPointer<const Self>        ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MyVectorImage, otb::VectorImage);

};

namespace otb
{
namespace Wrapper
{

class ImageFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef ImageFactory                  Self;
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
    return "ImageFactory";
    }

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ApplicationFactory, itk::ObjectFactoryBase);

protected:
  ImageFactory()
  {
  }

  virtual ~ImageFactory()
  {
  }

  /** This method is provided by sub-classes of ObjectFactoryBase.
   * It should create the named itk object or return 0 if that object
   * is not supported by the factory implementation. */
  virtual LightObject::Pointer CreateObject(const char* itkclassname )
  {
    std::cout << "CreateObject Testing " << itkclassname << std::endl;

    VectorImageType::Pointer image;
    if ( std::string( typeid(VectorImageType).name() ) == itkclassname )
      {
      std::cout << "HIT" << std::endl;
      image = MyVectorImage::New().GetPointer();
      }

    return image.GetPointer();
  }

  /** This method creates all the objects with the class overide of
   * itkclass name, which are provide by this object
   */
  virtual std::list<LightObject::Pointer>
  CreateAllObject(const char* itkclassname)
  {
    std::list<LightObject::Pointer> list;
    std::cout << "CreateAllObject Testing " << itkclassname << std::endl;

    return list;
  }

private:
  ImageFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace Wrapper
} //end namespace otb

#if (defined(WIN32) || defined(_WIN32))
#  define OTB_APP_EXPORT __declspec(dllexport)
#else
#  define OTB_APP_EXPORT
#endif

typedef otb::Wrapper::ImageFactory ImageFactoryType;
static ImageFactoryType::Pointer staticFactory;

extern "C"
  {
    OTB_APP_EXPORT itk::ObjectFactoryBase* itkLoad()
    {
      staticFactory = ImageFactoryType::New();
      return staticFactory;
    }
  }
