/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef otbFastExposeMetaData_h
#define otbFastExposeMetaData_h

#include "itkMetaDataObject.h"
#include <string>

namespace otb
{
/**Fast alternative to `itk::ExposeMetaData`.
 * `FastExposeMetaData` provides a shortcut for pulling a value of type `T` out
 * of a `MetaDataDictionary`.
 * If `Dictionary[key]` isn't set, return the default value `dft`.
 * \param[in] Dictionary  Reference to a dictionary
 * \param[in] key         String identifier for this object
 * \param[in] dft         Default value if key isn't found
 * \return the value associated to the key, or `dft` if none is found.
 * \ingroup OTBMetadata
 * \sa `ExposeMetaData()`
 */
template <typename T>
inline
T const& FastExposeMetaData(
  itk::MetaDataDictionary const& Dictionary,
  std::string             const& key,
  T                       const& dft)
{
  if ( !Dictionary.HasKey(key) )
    {
    return dft;
    }

  const itk::MetaDataObjectBase::ConstPointer baseObjectSmartPointer = Dictionary[key];

  itk::MetaDataObject< T > const * const TempMetaDataObject = dynamic_cast< itk::MetaDataObject< T > const * >( baseObjectSmartPointer.GetPointer() );
  if ( TempMetaDataObject == ITK_NULLPTR )
    {
    return dft;
    }

  return TempMetaDataObject->GetMetaDataObjectValue();
}

} // otb namespace


#endif // otbFastExposeMetaData_h
