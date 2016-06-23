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
#ifndef otbOpticalImageMetadataInterfaceFactory_h
#define otbOpticalImageMetadataInterfaceFactory_h

#include "itkObject.h"
#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class OpticalImageMetadataInterfaceFactory
 * \brief Create instances of OpticalImageMetadataInterfaceFactory objects using an object factory.
 *
 * \ingroup OTBMetadata
 */
class ITK_EXPORT OpticalImageMetadataInterfaceFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef OpticalImageMetadataInterfaceFactory Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalImageMetadataInterfaceFactory, Object);

  /** Convenient typedefs. */
  typedef OpticalImageMetadataInterface::ImageType                ImageType;
  typedef OpticalImageMetadataInterface::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef OpticalImageMetadataInterface::VectorType               VectorType;
  typedef OpticalImageMetadataInterface::VariableLengthVectorType VariableLengthVectorType;
  typedef OpticalImageMetadataInterface::ImageKeywordlistType     ImageKeywordlistType;
  typedef OpticalImageMetadataInterface::Pointer                  OpticalImageMetadataInterfacePointerType;

  /** Create the appropriate OpticalImageMetadataInterfaceFactory depending on the particulars of the file. */
  static OpticalImageMetadataInterfacePointerType CreateIMI(const MetaDataDictionaryType& dict);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  OpticalImageMetadataInterfaceFactory();
  ~OpticalImageMetadataInterfaceFactory() ITK_OVERRIDE;

private:
  OpticalImageMetadataInterfaceFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
