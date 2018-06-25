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


#ifndef otbOGRFieldWrapper_hxx
#define otbOGRFieldWrapper_hxx

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRFieldWrapper.h"
#include "otbConfigure.h"
#include <cassert>
#include <vector>
#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/assert.hpp>
// #include <boost/mpl/print.hpp>

#include <boost/static_assert.hpp>
#include <boost/range/size.hpp>
#include <boost/type_traits/is_same.hpp>
// #include "boost/type_traits/is_array.hpp"
#include "boost/type_traits/is_contiguous.h" // from OTB actually

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "ogr_feature.h"  // OGRFeature::*field_getters
#pragma GCC diagnostic pop
#else
#include "ogr_feature.h"  // OGRFeature::*field_getters
#endif
#include "cpl_string.h" // CSLCount
#include "otbOGRHelpers.h"



/*===========================================================================*/
/*================[ Associations C++ types -> OGR functions ]================*/
/*===========================================================================*/

namespace otb { namespace ogr {
/**\ingroup gGeometry
 * \defgroup GeometryInternals Geometry Internals
 * \since OTB v 3.14.0
 */

/**\ingroup GeometryInternals
 * Namespace used to host internal meta-prog definitions.
 * \since OTB v 3.14.0
 */
namespace internal { // namespace internal
namespace mpl = boost::mpl;

/**\ingroup GeometryInternals
 * Associative map of C++ types to OGR field types (\c OGRFieldType).
 * \internal Relies on Boost.MPL
 * \since OTB v 3.14.0
 * \todo \c OFTBinary, \c OFTDate, \c OFTTime and \c OFTDateTime are not managed
 * yet.
 */
typedef mpl::map
  < mpl::pair<int                     , mpl::int_<OFTInteger> >
  , mpl::pair<std::vector<int>        , mpl::int_<OFTIntegerList> >
  , mpl::pair<double                  , mpl::int_<OFTReal> >
  , mpl::pair<std::vector<double>     , mpl::int_<OFTRealList> >
  , mpl::pair<std::string             , mpl::int_<OFTString> >
  , mpl::pair<char*                   , mpl::int_<OFTString> >
  , mpl::pair<char const*             , mpl::int_<OFTString> >
  , mpl::pair<std::vector<std::string>, mpl::int_<OFTStringList> >
  #ifdef OTB_USE_GDAL_20
    , mpl::pair<GIntBig, mpl::int_<OFTInteger64> >
    , mpl::pair<std::vector<GIntBig>, mpl::int_<OFTInteger64List> >
  #endif
  // OFTBinary
  // OFTDate
  // OFTTime
  // OFTDateTime
  > FieldType_Map;

/**\ingroup GeometryInternals
 * \class MemberGetterPtr
 * Type for hosting simple member-function pointers to field getters.
 * \tparam T type of field according to OGR API.
 * \tparam ptr_to_function member function pointer to a field getter from \c
 *         OGRFeature.
 * \tparam FinalReturnType type of the field according to OTB wrappers (default
 * <tt> = T</tt>)
 *
 * \internal
 * This is a hack to pass a member function pointer as template-parameter.
 * \since OTB v 3.14.0
 */
template
  < typename T
#if GDAL_VERSION_NUM >= 2030000
  , T ( OGRFeature::*ptr_to_function )(int) const
#else
  , T ( OGRFeature::*ptr_to_function )(int)
#endif
  , typename FinalReturnType = T
  > class MemberGetterPtr
    {
  public:
    static FinalReturnType call(OGRFeature &f, int index)
      {
      return (f.*ptr_to_function)(index);
      }
    };

/**\ingroup GeometryInternals
 * \class StringListMemberGetterPtr
 * Type for hosting member-function pointers to string-list field getters.
 * \tparam FinalReturnType type of the field according to OTB wrappers (default
 * <tt> = T</tt>)
 *
 * \internal
 * This override is required because of the particular nature of the
 * <tt>char**</tt> type chosen by OGR API, plus the fact this is the only
 * const-correct getter...
 * \since OTB v 3.14.0
 */
template
  < typename FinalReturnType = std::vector<std::string>
  > class StringListMemberGetterPtr
    {
  public:
    static FinalReturnType call(OGRFeature &f, int index)
      {
      char ** sl = f.GetFieldAsStringList(index);
      FinalReturnType res(sl, sl+CSLCount(sl));
      return res;
      }
    };

/**\ingroup GeometryInternals
 * \class MemberSetterPtr
 * Type for hosting simple member-function pointers to field setters.
 * \tparam T type of field according to OGR API.
 * \tparam ptr_to_function member function pointer to a field setter from \c
 *         OGRFeature.
 * <tt> = T</tt>)
 *
 * \internal
 * This is a hack to pass a member function pointer as template-parameter.
 * \since OTB v 3.14.0
 */
template
  < typename T
  , void ( OGRFeature::*ptr_to_function )(int, T value)
  , typename ActualParamType = T
  > class MemberSetterPtr
    {
  public:
    static void call(OGRFeature &f, int index, T const& value)
      {
      (f.*ptr_to_function)(index, value);
      }
    };

/**\ingroup GeometryInternals
 * \class MemberContainerGetterPtr
 * Type for hosting simple member-function pointers to list-field getters.
 * \tparam T type of field according to OGR API.
 * \tparam ptr_to_function member function pointer to a list-field getter from
 * \c OGRFeature.
 * \tparam FinalReturnType type of the list-field according to OTB wrappers
 * (default <tt> = std::vector<T></tt>)
 *
 * \internal
 * This is a hack to pass a member function pointer as template-parameter.
 * \since OTB v 3.14.0
 */
template
  < typename T
#if GDAL_VERSION_NUM >= 2030000
  , T const* ( OGRFeature::*ptr_to_function )(int, int*) const
#else
  , T const* ( OGRFeature::*ptr_to_function )(int, int*)
#endif
  , typename FinalReturnType = std::vector<T>
  > class MemberContainerGetterPtr
    {
  public:
    static FinalReturnType call(OGRFeature &f, int index)
      {
      int nb = 0;
      T const* raw_container = (f.*ptr_to_function)(index, &nb);
      const FinalReturnType res(raw_container+0, raw_container+nb);
      return res;
      }
    };

/*===========================================================================*/
/**\ingroup GeometryInternals
 * \class TagDispatchMemberContainerSetterPtr
 * \brief Dispatcher function for the Field Setter.
 * The container-field setters from OGR API have a C API. This dispatcher will
 * check whether the parameter container has a contiguous storage. If so it will
 * directly inject the address of the first element of the contiguous container
 * in the OGR C API. If not, the container will be converted into a container
 * with contiguous storage.
 * \since OTB v 3.14.0
 */
template
  < typename T
  , void ( OGRFeature::*ptr_to_function )(int, int, T*) // not const-correct
  , typename ActualParamType = std::vector<T>
  , bool Is_contiguous = boost::is_contiguous<ActualParamType>::value
  > class TagDispatchMemberContainerSetterPtr;

/**\ingroup GeometryInternals
 */
template
  < typename T
  , void ( OGRFeature::*ptr_to_function )(int, int, T*) // not const-correct
  , typename ActualParamType
  > class TagDispatchMemberContainerSetterPtr<T, ptr_to_function, ActualParamType, true>
    {
  public:
    static void call(OGRFeature &f, int index, ActualParamType const& container)
      {
      const int nb = boost::size(container);
      (f.*ptr_to_function)(index, nb, const_cast <T*>(&container[0]));
      }
    };

/**\ingroup GeometryInternals
 */
template
  < typename T
  , void ( OGRFeature::*ptr_to_function )(int, int, T*) // not const-correct
  , typename ActualParamType
  > class TagDispatchMemberContainerSetterPtr<T, ptr_to_function, ActualParamType, false>
    {
  public:
    static void call(OGRFeature &f, int index, ActualParamType const& container)
      {
      const int nb = boost::size(container);
      std::vector<T> v(boost::begin(container), boost::end(container));
      (f.*ptr_to_function)(index, nb, &v[0]);
      }
    };

/**\ingroup GeometryInternals
 * \class StringListMemberSetterPtr
 * Type for hosting member-function pointers to string-list field setters.
 * \tparam ActualParamType type of the field according to OTB wrappers (default
 * <tt> = T</tt>)
 *
 * \internal
 * This override is required because of the particular nature of the
 * <tt>char**</tt> type chosen by OGR API.
 * \since OTB v 3.14.0
 */
template
  < typename ActualParamType
  > class StringListMemberSetterPtr
    {
  public:
    static void call(OGRFeature &f, int index, ActualParamType const& container)
      {
      f.SetField(index, StringListConverter(container).to_ogr());
      }
    };

/**\ingroup GeometryInternals
 * \class MemberContainerSetterPtr
 * Type for hosting simple member-function pointers to list-field setters.
 * \tparam T type of field according to OGR API.
 * \tparam ptr_to_function member function pointer to a list-field setter from
 * \c OGRFeature.
 * \tparam FinalReturnType type of the list-field according to OTB wrappers
 * (default <tt> = std::vector<T></tt>)
 *
 * \internal
 * This is a hack to pass a member function pointer as template-parameter.
 * \since OTB v 3.14.0
 */
template
  < typename T
#if GDAL_VERSION_NUM >= 2030000
  , void ( OGRFeature::*ptr_to_function )(int, int, const T*)
#else
  , void ( OGRFeature::*ptr_to_function )(int, int, T*) // not const-correct
#endif
  , typename ActualParamType = std::vector<T>
  > class MemberContainerSetterPtr
    {
  public:
    static void call(OGRFeature &f, int index, ActualParamType const& container)
      {
      TagDispatchMemberContainerSetterPtr<
        T
        ,ptr_to_function
        ,ActualParamType
        , boost::is_contiguous<ActualParamType>::value
        >::call(f, index, container);
      }
    };

/**\ingroup GeometryInternals
 * Associative map of OGR field types (\c OGRFieldType) to their associated
 * getters.
 * \internal Relies on Boost.MPL
 * \since OTB v 3.14.0
 */
typedef mpl::map
  < mpl::pair<mpl::int_<OFTInteger>,     MemberGetterPtr<int,             &OGRFeature::GetFieldAsInteger> >
  , mpl::pair<mpl::int_<OFTIntegerList>, MemberContainerGetterPtr<int,    &OGRFeature::GetFieldAsIntegerList> >
  , mpl::pair<mpl::int_<OFTReal>,        MemberGetterPtr<double,          &OGRFeature::GetFieldAsDouble> >
  , mpl::pair<mpl::int_<OFTRealList>,    MemberContainerGetterPtr<double, &OGRFeature::GetFieldAsDoubleList> >
  , mpl::pair<mpl::int_<OFTString>,      MemberGetterPtr<char const*,     &OGRFeature::GetFieldAsString, std::string> >
  , mpl::pair<mpl::int_<OFTStringList>,  StringListMemberGetterPtr<std::vector<std::string> > >
  #ifdef OTB_USE_GDAL_20
  , mpl::pair<mpl::int_<OFTInteger64>, MemberGetterPtr<GIntBig, &OGRFeature::GetFieldAsInteger64> >
  , mpl::pair<mpl::int_<OFTInteger64List>, MemberContainerGetterPtr<GIntBig, &OGRFeature::GetFieldAsInteger64List> >
  #endif
  > FieldGetters_Map;

/**\ingroup GeometryInternals
 * Associative map of OGR field types (\c OGRFieldType) to their associated
 * setters.
 * \internal Relies on Boost.MPL
 * \since OTB v 3.14.0
 */
typedef mpl::map
  < mpl::pair<mpl::int_<OFTInteger>,     MemberSetterPtr<int,             &OGRFeature::SetField> >
  , mpl::pair<mpl::int_<OFTIntegerList>, MemberContainerSetterPtr<int,    &OGRFeature::SetField> >
  , mpl::pair<mpl::int_<OFTReal>,        MemberSetterPtr<double,          &OGRFeature::SetField> >
  , mpl::pair<mpl::int_<OFTRealList>,    MemberContainerSetterPtr<double, &OGRFeature::SetField> >
  , mpl::pair<mpl::int_<OFTString>,      MemberSetterPtr<char const*,     &OGRFeature::SetField/*, std::string*/> >
  , mpl::pair<mpl::int_<OFTStringList>,  StringListMemberSetterPtr<std::vector<std::string> > >
  #ifdef OTB_USE_GDAL_20
  , mpl::pair<mpl::int_<OFTInteger64>, MemberSetterPtr<GIntBig, &OGRFeature::SetField> >
  , mpl::pair<mpl::int_<OFTInteger64List>, MemberContainerSetterPtr<const GIntBig, &OGRFeature::SetField> >
  #endif
  > FieldSetters_Map;

/**\ingroup GeometryInternals
 * \class CppToOGRConverter_trait
 * \brief Trait class for converting C++ types into OGR field setter compatible types.
 * \tparam T initial C++ type
 * \return C compatible type for OGR API.
 * \note C static arrays become pointers (in order to support literal strings)
 * \note C++ \c std::string becomes <tt>char const*</tt>
 * \since OTB v 3.14.0
 */
template <typename T> struct CppToOGRConverter_trait
    {
    typedef T type;
    static T const& convert(T const& value) { return value; }
    };

/**\ingroup GeometryInternals
 * \brief Specialisation for C static arrays.
 */
template <typename T, std::size_t N> struct CppToOGRConverter_trait<T[N]>
  {
  typedef T* type;
  static T const* convert(const T value[N]) { return &value[0]; }
  };

/**\ingroup GeometryInternals
 * \brief Specialisation for C++ \c std::string.
 */
template <> struct CppToOGRConverter_trait<std::string>
  {
  typedef char* type;
  static char const* convert(std::string const& value) { return value.c_str(); }
  };

} // namespace internal
} } // end namespace otb::ogr

/*===========================================================================*/
/*=======================[ otb::ogr::Field functions ]=======================*/
/*===========================================================================*/

inline
void otb::ogr::Field::CheckInvariants() const
{
  assert(m_Feature && "OGR Fields must be associated to a valid feature");
  assert(m_index < m_Feature->GetFieldCount() && "Out-of-range index for a OGR field");
  assert(m_Feature->GetFieldDefnRef(m_index) && "No definition available for the OGR field");
}

template <typename T>
inline
void otb::ogr::Field::SetValue(T const& value)
{
  CheckInvariants();
  typedef internal::CppToOGRConverter_trait<T> Converter;
  typedef typename Converter::type             InterfaceType;
  // uncomment the next line to debug the InterfaceType computed
  // boost::mpl::print<typename internal::CppToOGRConverter_trait<T>::type> interface_type; (void) interface_type;
  BOOST_MPL_ASSERT_MSG(!boost::is_array<InterfaceType>::value, InterFaceType_Cant_Be_An_array, (T, InterfaceType));
  typedef typename boost::mpl::at<internal::FieldType_Map, InterfaceType>::type Kind;
  BOOST_MPL_ASSERT_MSG(!(boost::is_same<Kind, boost::mpl::void_>::value), UNEXPECTED_KIND_TYPE, (T, InterfaceType, Kind));
  //const int VALUE = Kind::value;
  assert(m_Definition.GetType() == Kind::value && "OGR field type mismatches the type of new field value");
  typedef typename boost::mpl::at<internal::FieldSetters_Map, Kind>::type SetterType;
  // If you experience a static assertion failure in the line below, it means
  // the type of the parameter is not supported to set a field.
  BOOST_MPL_ASSERT_NOT((boost::is_same<SetterType, boost::mpl::void_>));
  SetterType::call(*m_Feature, m_index, Converter::convert(value));
}

template <typename T>
inline
T otb::ogr::Field::GetValue() const
{
  CheckInvariants();
  assert(HasBeenSet() && "Cannot access the value of a field that hasn't been set");
  typedef typename boost::mpl::at<internal::FieldType_Map, T>::type Kind;
  //const int VALUE = Kind::value;
  BOOST_STATIC_ASSERT(!(boost::is_same<Kind, boost::mpl::void_>::value));
  assert(m_Definition.GetType() == Kind::value && "OGR field type mismatches the type of requested field value");
  typedef typename boost::mpl::at<internal::FieldGetters_Map, Kind>::type GetterType;
  // If you experience a static assertion failure in the line below, it means
  // the field cannot be extracted into the type requested.
  BOOST_STATIC_ASSERT(!(boost::is_same<GetterType, boost::mpl::void_>::value));
  return GetterType::call(*m_Feature, m_index);
}

inline
std::ostream & otb::ogr::Field::PrintSelf(
  std::ostream& os, itk::Indent indent) const
{
  CheckInvariants();
  return UncheckedPrintSelf(os, indent);
}

inline
bool otb::ogr::Field::HasBeenSet() const
{
  CheckInvariants();
  return UncheckedHasBeenSet();
}

inline
void otb::ogr::Field::Unset() const
{
  CheckInvariants();
  UncheckedUnset();
}

inline
void otb::ogr::Field::Assign(Field const& f)
{
  CheckInvariants();
  f.CheckInvariants();
  assert(f.GetType() == this->GetType() && "Cannot assign from a field that doesn't have the same definition");
  // We can't assume the following as sometimes field names are altered by the
  // datasource driver; for instance, shp driver truncates field names to 8
  // characters.
  // assert(f.GetDefinition() == this->GetDefinition() && "Cannot assign from a field that doesn't have the same definition");
  UncheckedAssign(f);
  CheckInvariants();
}

inline
OGRField & otb::ogr::Field::ogr()
{
  CheckInvariants();
  OGRField * f = m_Feature->GetRawFieldRef(m_index);
  assert(f && "The field obtained shall not be null");
  return *f;
}

inline
OGRField & otb::ogr::Field::ogr() const
{
  return const_cast <Field*>(this)->ogr();
}

#endif // otbOGRFieldWrapper_hxx
