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
#ifndef __otbImageMetadataInterface_h
#define __otbImageMetadataInterface_h

#include "itkObject.h"
#include "otbImageMetadataInterfaceBase.h"


namespace otb
{
/** \class ImageMetadataInterface
 * \brief Create instances of ImageMetadataInterface objects using an object factory.
 */
class ITK_EXPORT ImageMetadataInterface : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ImageMetadataInterface           Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMetadataInterface, Object);

  /** Convenient typedefs. */
  typedef ImageMetadataInterfaceBase::ImageType                ImageType;
  typedef ImageMetadataInterfaceBase::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef ImageMetadataInterfaceBase::VectorType               VectorType;
  typedef ImageMetadataInterfaceBase::VariableLengthVectorType VariableLengthVectorType;
  typedef ImageMetadataInterfaceBase::ImageKeywordlistType     ImageKeywordlistType;
  typedef ImageMetadataInterfaceBase::Pointer 	               ImageMetadataInterfaceBasePointerType;
 
  /** Create the appropriate ImageMetadataInterface depending on the particulars of the file. */
  static ImageMetadataInterfaceBasePointerType CreateIMI( const MetaDataDictionaryType & dict );

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  ImageMetadataInterface();
  ~ImageMetadataInterface();

private:
  ImageMetadataInterface(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


} // end namespace otb


#endif
