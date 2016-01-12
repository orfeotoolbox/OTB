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
#ifndef __otbChangeInformationImageFilter_h
#define __otbChangeInformationImageFilter_h

#include "itkChangeInformationImageFilter.h"
#include "otbMetaDataKey.h"

namespace otb
{

/**
 * \class ChangeInformationImageFilter
 * \brief Filter to modify image metadata
 *
 * The base class is itk::ChangeInformationImageFilter that allows
 * modifying origin, spacing, direction and buffered region. This deriving
 * filter adds the support of MetaDataDictionary.
 *
 * \ingroup OTBImageManipulation
 */
template< typename TInputImage >
class ChangeInformationImageFilter:
  public itk::ChangeInformationImageFilter< TInputImage >
{
public:
  /** Standard class typedefs. */
  typedef ChangeInformationImageFilter                   Self;
  typedef itk::ChangeInformationImageFilter<TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ChangeInformationImageFilter, itk::ChangeInformationImageFilter);

  /** Set key names to change */
  void SetChangeMetaData(const char *keyname, bool flag);

  /** Ask if a metadata will be changed */
  bool GetChangeMetaData(const char *keyname);

  /** Set output values for metadata, passing a NULL value will remove the
   *  metadata from output. If not set for a key name in the change list,
   *  the metadata will also be set.
   */
  template<typename T>
  void SetOutputMetaData(const char *keyname, const T * value);

protected:
  ChangeInformationImageFilter() {}
  virtual ~ChangeInformationImageFilter() {}

  /** Apply changes to the output image metadata. */
  virtual void GenerateOutputInformation();

private:
  ChangeInformationImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Removes a field from a metadata dictionary
   *  After ITK 4.6, an Erase() method has been added to
   *  itk::MetaDataDictionary, so this function could be tagged as deprecated */
  bool RemoveKeyFromDictionary(itk::MetaDataDictionary & dict, const std::string & key);

  /** List of metadata keys to change */
  std::set<std::string> m_ChangedKeys;

};

} // End of namespace OTB

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbChangeInformationImageFilter.txx"
#endif

#endif
