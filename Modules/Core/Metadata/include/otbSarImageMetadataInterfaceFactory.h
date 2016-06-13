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
#ifndef otbSarImageMetadataInterfaceFactory_h
#define otbSarImageMetadataInterfaceFactory_h

#include "itkObject.h"
#include "otbSarImageMetadataInterface.h"

namespace otb
{
/** \class SarImageMetadataInterfaceFactory
 * \brief Create instances of SarImageMetadataInterfaceFactory objects using an object factory.
 *
 * \ingroup OTBMetadata
 */
class ITK_EXPORT SarImageMetadataInterfaceFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef SarImageMetadataInterfaceFactory Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarImageMetadataInterfaceFactory, Object);

  /** Convenient typedefs. */
  typedef SarImageMetadataInterface::ImageType                ImageType;
  typedef SarImageMetadataInterface::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef SarImageMetadataInterface::VectorType               VectorType;
  typedef SarImageMetadataInterface::VariableLengthVectorType VariableLengthVectorType;
  typedef SarImageMetadataInterface::ImageKeywordlistType     ImageKeywordlistType;
  typedef SarImageMetadataInterface::Pointer                  SarImageMetadataInterfacePointerType;

  /** Create the appropriate SarImageMetadataInterfaceFactory depending on the particulars of the file. */
  static SarImageMetadataInterfacePointerType CreateIMI(const MetaDataDictionaryType& dict);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  SarImageMetadataInterfaceFactory();
  ~SarImageMetadataInterfaceFactory() ITK_OVERRIDE;

private:
  SarImageMetadataInterfaceFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
