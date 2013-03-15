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
#ifndef __otbImageIOFactory_h
#define __otbImageIOFactory_h

#include "itkObject.h"
#include "otbImageIOBase.h"
#include "itkImageIOFactory.h"

namespace otb
{
/** \class ImageIOFactory
 * \brief Creation of object instance using object factory.
 */
class ITK_EXPORT ImageIOFactory : public itk::ImageIOFactory
{
public:
  /** Standard class typedefs. */
  typedef ImageIOFactory                Self;
  typedef itk::ImageIOFactory           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageIOFactory, itk::ImageIOFactory);

  /** Convenient typedefs. */
  typedef::otb::ImageIOBase::Pointer ImageIOBasePointer;

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
  ImageIOFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
