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

#ifndef __otbOGRFieldWrapper_txx
#define __otbOGRFieldWrapper_txx

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRFieldWrapper.h"
#include <cassert>
#include <vector>
#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>

#include <boost/static_assert.hpp>
#include <boost/range/size.hpp>
#include <boost/type_traits/is_same.hpp>
// #include "boost/type_traits/is_array.hpp"
#include "boost/type_traits/is_contiguous.h" // from OTB actually

#include "ogr_feature.h" // OGRFeature::*field_getters


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
using namespace boost::mpl;

/**\ingroup GeometryInternals
 * Associative map of C++ types to OGR field types (\c OGRFieldType).
 * \internal Relies on Boost.MPL
 * \since OTB v 3.14.0
 * \todo \c OFTBinary, \c OFTDate, \c OFTTime and \c OFTDateTime are not managed
 * yet.
 */
typedef boost::mpl::map
  < pair<int                     , int_<OFTInteger> >
  , pair<std::vector<int>        , int_<OFTIntegerList> >
  , pair<double                  , int_<OFTReal> >
  , pair<std::vector<double>     , int_<OFTRealList> >
  , pair<std::string             , int_<OFTString> >
  , pair<char*                   , int_<OFTString> >
  , pair<std::vector<std::string>, int_<OFTStringList> >
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
  , T ( OGRFeature::*ptr_to_function )(int)
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
  , T const* ( OGRFeature::*ptr_to_function )(int, int*)
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
      const int nb = boost::size(container) + 1;
      std::vector<char const*> v; v.reserve(nb);
      for (typename ActualParamType::const_iterator b = container.begin(), e = container.end()
; b != e
; ++b)
        {
        v.push_back(b->c_str());
        }
      v.push_back(0);
      assert(CSLCount(const_cast <char**>(&v[0])) == boost::size(container));
      f.SetField(index, const_cast <char**>(&v[0]));
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
  , void ( OGRFeature::*ptr_to_function )(int, int, T*) // not const-correct
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
typedef map
  < pair<int_<OFTInteger>,     MemberGetterPtr<int,             &OGRFeature::GetFieldAsInteger> >
  , pair<int_<OFTIntegerList>, MemberContainerGetterPtr<int,    &OGRFeature::GetFieldAsIntegerList> >
  , pair<int_<OFTReal>,        MemberGetterPtr<double,          &OGRFeature::GetFieldAsDouble> >
  , pair<int_<OFTRealList>,    MemberContainerGetterPtr<double, &OGRFeature::GetFieldAsDoubleList> >
  , pair<int_<OFTString>,      MemberGetterPtr<char const*,     &OGRFeature::GetFieldAsString, std::string> >
  , pair<int_<OFTStringList>,  StringListMemberGetterPtr<std::vector<std::string> > >
  > FieldGetters_Map;

/**\ingroup GeometryInternals
 * Associative map of OGR field types (\c OGRFieldType) to their associated
 * setters.
 * \internal Relies on Boost.MPL
 * \since OTB v 3.14.0
 */
typedef map
  < pair<int_<OFTInteger>,     MemberSetterPtr<int,             &OGRFeature::SetField> >
  , pair<int_<OFTIntegerList>, MemberContainerSetterPtr<int,    &OGRFeature::SetField> >
  , pair<int_<OFTReal>,        MemberSetterPtr<double,          &OGRFeature::SetField> >
  , pair<int_<OFTRealList>,    MemberContainerSetterPtr<double, &OGRFeature::SetField> >
  , pair<int_<OFTString>,      MemberSetterPtr<char const*,     &OGRFeature::SetField/*, std::string*/> >
  , pair<int_<OFTStringList>,  StringListMemberSetterPtr<std::vector<std::string> > >
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
  assert(int(m_index) < m_Feature->GetFieldCount() && "Out-of-range index for a OGR field");
  assert(m_Feature->GetFieldDefnRef(m_index) && "No definition available for the OGR field");
}

template <typename T>
inline
void otb::ogr::Field::SetValue(T const& value)
{
  CheckInvariants();
  typedef internal::CppToOGRConverter_trait<T> Converter;
  typedef typename Converter::type             InterfaceType;
  // BOOST_STATIC_ASSERT(!(boost::is_array<InterfaceType>::value));
  typedef typename boost::mpl::at<internal::FieldType_Map, InterfaceType>::type Kind;
  const int VALUE = Kind::value;
  BOOST_STATIC_ASSERT(!(boost::is_same<Kind, boost::mpl::void_>::value));
  assert(m_Definition.GetType() == VALUE && "OGR field type mismatches the type of new field value");
  typedef typename boost::mpl::at<internal::FieldSetters_Map, Kind>::type SetterType;
  // If you experience a static assertion failure in the line below, it means
  // the type of the parameter is not supported to set a field.
  BOOST_STATIC_ASSERT(!(boost::is_same<SetterType, boost::mpl::void_>::value));
  SetterType::call(*m_Feature, m_index, Converter::convert(value));
}

template <typename T>
inline
T otb::ogr::Field::GetValue() const
{
  CheckInvariants();
  assert(HasBeenSet() && "Cannot access the value of a field that hasn't been set");
  typedef typename boost::mpl::at<internal::FieldType_Map, T>::type Kind;
  const int VALUE = Kind::value;
  BOOST_STATIC_ASSERT(!(boost::is_same<Kind, boost::mpl::void_>::value));
  assert(m_Definition.GetType() == VALUE && "OGR field type mismatches the type of requested field value");
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

#endif // __otbOGRFieldWrapper_txx
