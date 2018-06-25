/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbOGRFieldWrapper_h
#define otbOGRFieldWrapper_h

#include <string>
#include <boost/shared_ptr.hpp>
class OGRFieldDefn;
class OGRFeature;
#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable: 4251 )
#include "ogr_core.h" // OGR enums
#pragma warning ( pop )
#else
#include "ogr_core.h" // OGR enums
#endif
#include "itkIndent.h"

#include "OTBGdalAdaptersExport.h"

namespace otb { namespace ogr {

/*===========================================================================*/
/*===========================[ Field definition ]============================*/
/*===========================================================================*/
/**\ingroup gGeometry
 * \class FieldDefn
 * \brief Encapsulation of \c OGRFieldDefn: field definition.
 * \invariant <tt>m_Definition != 0</tt>
 * \invariant \c m_Definition lifetime is of the responsibility of the owning \c
 * \c OGRFeatureDefn.
 * \sa OGRFieldDefn
 * \since OTB v 3.14.0
 *
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT FieldDefn
  {
public:
  FieldDefn(OGRFieldDefn& definition) : m_Definition(&definition){ }
  std::string  GetName() const; //!< Field name accessor.
  OGRFieldType GetType() const; //!< Field type accessor.

  OGRFieldDefn & ogr()       { return *m_Definition; }
  OGRFieldDefn & ogr() const { return *m_Definition; }

private:
  /**Pointer to the actual definition.
   * \internal pointer in order to support assignments
   */
  OGRFieldDefn * m_Definition;
  };

inline
bool operator==(FieldDefn const& lhs, FieldDefn const& rhs)
  {
  return lhs.GetName() == rhs.GetName()
    &&   lhs.GetType() == rhs.GetType();
  }

OTBGdalAdapters_EXPORT
std::ostream & operator<<(std::ostream & os, FieldDefn const& defn);

/*===========================================================================*/
/*=================================[ Field ]=================================*/
/*===========================================================================*/
class Feature;

/**\ingroup gGeometry
 * \class Field
 * \brief Encapsulation of \c OGRField
 * Instances of \c Field are expected to be built from an existing \c Feature
 * with which they'll share their owning \c OGRFeature.
 *
 * A \c Field instance works as a proxy. Copying a field will only have it share
 * the actual \c OGRField between several instances. In order to copy a field
 * value from another field, use \c Field::assign().
 *
 * \invariant <tt>m_Feature</tt> shall be valid (i.e. not wrapping a null \c
 * OGRFeature).
 * \invariant <tt>m_index < m_Feature->GetFieldCount()</tt>.
 * \invariant <tt>m_Feature->GetFieldDefnRef(m_index) != 0</tt>.
 * \sa OGRField
 * \since OTB v 3.14.0
 *
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT Field
{
public:
  /**
   * Constructor.
   * \param[in,out] feature  \c Feature from which the field is.
   * \param[in] index  Index of the field in the \c Feature.
   * \throw None
   */
  Field(Feature & feature, int index);
  /// %Field definition accessor.
  FieldDefn const& GetDefinition() const
    { return m_Definition; }
  /// %Field type accessor.
  OGRFieldType GetType() const
    { return m_Definition.GetType(); }
  /// %Field name accessor.
  std::string GetName() const
    { return m_Definition.GetName(); }

  /// Tells whether the field value has been set.
  bool HasBeenSet() const;
  /// Unsets the value of the field.
  void Unset() const;

  /**
   * Value setter.
   * \param[in] value  New value for the field.
   * \throw None
   * \pre \c value's kind must match the field's type.
   * \internal
   * This function automagically decodes the type of the parameter in order to
   * use the ad'hoc setter from \c OGRFeature.
   */
  template <typename T> void SetValue(T const& value);

  /**
   * Value getter.
   * \tparam T expected type for the stored value.
   * \throw None
   * \pre \c T must match the field's type.
   * \pre The field must have set.
   * \internal
   * This function automagically deduce the ad'hoc getter (from \c OGRFeature)
   * from \c T.
   */
  template <typename T> T GetValue() const;

  /** Prints self into stream. */
  std::ostream & PrintSelf(std::ostream&os, itk::Indent indent) const;

  /** Copies a field.
   * As \c Field is a proxy type, this function is the only possible way to copy a field.
   *
   * First, the field must be defined with a definition, then it could be set
   * from another field value.
   * \code
   * Field srcField = srcFeature[42];
   * Feature dstFeature(layerDefinition);
   * Field dst(dstFeature, 12);
   * dstField.Assign(srcField);
   * \endcode
   */
  void Assign(Field const& f);

  /** \copydoc Field::ogr() const */
  OGRField & ogr();
  
  /** Access to the raw underlying OGR data.
   * This function provides an abstraction leak in case deeper control on the
   * underlying \c OGRFeature is required.
   * \warning You must under no circumstance try to delete the \c OGRField
   * obtained this way.
   */
  OGRField & ogr() const;

private:
  /**\name Unchecked definitions
   * All the definitions that follow do the real work. However, they are not the
   * exposed public functions. The design of this class follows the principle
   * behind the NVI (<em>Non-Virtual Interface</em>) pattern:
   * - The public functions are inlined and check invariants and preconditions,
   * - While the private functions do the work.
   */
  //@{
  bool           UncheckedHasBeenSet() const;
  void           UncheckedUnset() const;
  std::ostream & UncheckedPrintSelf(std::ostream&os, itk::Indent indent) const;
  void           UncheckedAssign(Field const& f);
  //@}

  /**
   * Internal function to check class invariants.
   * If any invariant is broken, an assertion will be fired.
   * \internal
   * This function is defined inlined in order to be able to inhibit invariants
   * checking depending on the compilation mode (on \c NDEBUG preprocessor
   * define actually).
   */
  void CheckInvariants() const;

  /// Definition of the field.
  FieldDefn                       m_Definition;
  /// Link to the actual \c OGRFeature in charge of the fields.
  boost::shared_ptr<OGRFeature> & m_Feature;
  /** Index of the field according to the feature definition.
   * \internal
   * All the fields decoding is at the wrong place (\c OGRFeature instead of
   * \c OGRField) => need for an index.
   */

  int        m_index;
};

}
} // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRFieldWrapper.hxx"
#endif

#endif // otbOGRFieldWrapper_h
