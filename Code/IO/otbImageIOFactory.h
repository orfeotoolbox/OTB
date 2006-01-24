/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Création d'instances d'objet ImageIO utilisant les object factory
  $Id$

=========================================================================*/
#ifndef __otbImageIOFactory_h
#define __otbImageIOFactory_h

#include "itkObject.h"
#include "itkImageIOBase.h"
#include "itkImageIOFactory.h"

#include "otbCAIImageIOFactory.h"
//#include "otbGDALImageIOFactory.h"

namespace otb
{
/** \class ImageIOFactory
 * \brief Création d'instances d'objet ImageIO utilisant les object factory.
 */
class ITK_EXPORT ImageIOFactory : public itk::ImageIOFactory
{
public:  
  /** Standard class typedefs. */
  typedef ImageIOFactory   Self;
  typedef itk::ImageIOFactory  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Class Methods used to interface with the registered factories */
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageIOFactory, itk::ImageIOFactory);

  /** Convenient typedefs. */
  typedef ::itk::ImageIOBase::Pointer ImageIOBasePointer;

  /** Mode in which the files is intended to be used */
//  typedef enum { ReadMode, WriteMode } FileModeType;
  
  /** Create the appropriate ImageIO depending on the particulars of the file. */
  static ImageIOBasePointer CreateImageIO(const char* path, FileModeType mode);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  ImageIOFactory();
  ~ImageIOFactory();

private:
  ImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};
  
  
} // end namespace otb

#endif
