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
#ifndef __otbOGRFieldWrapper_h
#define __otbOGRFieldWrapper_h

#include <vector>
#include <string>
#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>

// #include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/range/size.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/shared_ptr.hpp>
class OGRFieldDefn;
// class OGRFeature;
#include "ogr_feature.h" // OGRFeature::*field_getters
#include "ogr_core.h" // OGR enums
#include "itkIndent.h"
#include <cassert>

#include <boost/type_traits/is_array.hpp>
namespace boost {
template <typename Range> struct is_contiguous
  { enum {value = is_array<Range>::value}; };

// todo: other vector params
template <typename T> struct is_contiguous<std::vector<T> >
  { enum {value = true}; };
template <typename T, std::size_t N> class array;
template <typename T, std::size_t N> struct is_contiguous<boost::array<T, N> >
  { enum {value = true}; };
} // boost namespace


namespace otb { namespace ogr {

/**\ingroup Geometry
 * \defgroup GeometryInternals Geometry Internals
 */

/**\ingroup GeometryInternals
 * Namespace used to host internal meta-prog definitions.
 */
namespace metaprog { // namespace metaprog
using namespace boost::mpl;

/**\ingroup GeometryInternals
 * Associative map of C++ types to OGR field types (\c OGRFieldType).
 * \internal Relies on Boost.MPL
 */
typedef boost::mpl::map
  < pair<int, int_<OFTInteger> >
  , pair<std::vector<int>, int_<OFTIntegerList> >
  , pair<double, int_<OFTReal> >
  , pair<std::vector<double>, int_<OFTRealList> >
  , pair<std::string, int_<OFTString> >
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
 * \class MemberSetterPtr
 * Type for hosting simple member-function pointers to field setters.
 * \tparam T type of field according to OGR API.
 * \tparam ptr_to_function member function pointer to a field setter from \c
 *         OGRFeature.
 * <tt> = T</tt>)
 *
 * \internal
 * This is a hack to pass a member function pointer as template-parameter.
 */
template
  < typename T
  , void ( OGRFeature::*ptr_to_function )(int, T value)
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
template
  < typename T
  , void ( OGRFeature::*ptr_to_function )(int, int, T*) // not const-correct
  , typename ActualParamType = std::vector<T>
  , bool Is_contiguous = boost::is_contiguous<ActualParamType>::value
  > class TagDispatchMemberContainerSetterPtr;

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
      (f.*ptr_to_function)(index, nb, &container[0]);
      }
    };

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
 */
typedef map
  < pair<int_<OFTInteger>,     MemberGetterPtr<int,             &OGRFeature::GetFieldAsInteger> >
  , pair<int_<OFTIntegerList>, MemberContainerGetterPtr<int,    &OGRFeature::GetFieldAsIntegerList> >
  , pair<int_<OFTReal>,        MemberGetterPtr<double,          &OGRFeature::GetFieldAsDouble> >
  , pair<int_<OFTRealList>,    MemberContainerGetterPtr<double, &OGRFeature::GetFieldAsDoubleList> >
  , pair<int_<OFTString>,      MemberGetterPtr<char const*,     &OGRFeature::GetFieldAsString, std::string> >
  // , pair<int_<OFTStringList>,  MemberGetterPtr<char const*,     &OGRFeature::GetFieldAsString, std::string> >
  > FieldGetters_Map;

/**\ingroup GeometryInternals
 * Associative map of OGR field types (\c OGRFieldType) to their associated
 * setters.
 * \internal Relies on Boost.MPL
 */
typedef map
  < pair<int_<OFTInteger>,     MemberSetterPtr<int,             &OGRFeature::SetField> >
  , pair<int_<OFTIntegerList>, MemberContainerSetterPtr<int,    &OGRFeature::SetField> >
  , pair<int_<OFTReal>,        MemberSetterPtr<double,          &OGRFeature::SetField> >
  , pair<int_<OFTRealList>,    MemberContainerSetterPtr<double, &OGRFeature::SetField> >
  // , pair<int_<OFTString>,      MemberSetterPtr<char const*,     &OGRFeature::SetField, std::string> >
  // , pair<int_<OFTStringList>,  MemberSetterPtr<char const*,     &OGRFeature::GetFieldAsString, std::string> >
  > FieldSetters_Map;

} // namespace metaprog

/**\ingroup Geometry
 * \class FieldDefn
 * \brief Encapsulation of \c OGRFieldDefn
 * \invariant <tt>m_definition != 0</tt>
 * \invariant \c m_definition lifetime is of the responsability of the owning \c
 * \c OGRFeatureDefn.
 * \sa OGRFieldDefn
 */
class FieldDefn
  {
public:
  FieldDefn(OGRFieldDefn& definition) : m_definition(&definition){ }
  std::string  GetName() const;
  OGRFieldType GetType() const;

private:
  OGRFieldDefn * m_definition; // pointer in order to support assignments
  };

#if 0
template <typename FieldType> struct FieldDecodingTraitsGetter {};
template <typename FieldType> struct FieldDecodingTraits
  {
  // good for int, double, char const*, list of strings
  typedef FieldType Type;
  static void Set(OGRFeature &f, int index, Type const& value)
    {
    f.SetField(index, value);
    }
  };

template <> struct FieldDecodingTraitsGetter<int>
  {
  typedef int Type;
  static Type Get(OGRFeature &f, size_t index)
    {
    return f.GetFieldAsInteger(index);
    }
  };

template <> struct FieldDecodingTraitsGetter<double>
  {
  typedef double Type;
  static Type Get(OGRFeature &f, size_t index)
    {
    return f.GetFieldAsDouble(index);
    }
  };

template <> struct FieldDecodingTraitsGetter<std::string>
  {
  typedef std::string Type;
  static Type Get(OGRFeature &f, size_t index)
    {
    return f.GetFieldAsString(index);
    }
  };
#endif

/*===========================================================================*/
/*=================================[ Field ]=================================*/
/*===========================================================================*/
class Feature;
/**\ingroup Geometry
 * \class Field
 * \brief Encapsulation of \c OGRField
 * Instances of \c Field are expected to be built from an existing \c Feature
 * with which they'll share their owning \c OGRFeature.
 * \sa OGRField
 */
class Field
  {
public:
  Field(Feature & feature, size_t index);
  // Field(OGRFeature& field, FieldDefn const& definition);
  FieldDefn const& GetDefinition() const;
  OGRFieldType GetType() const
    { return m_Definition.GetType(); }
  std::string GetName() const
    { return m_Definition.GetName(); }

  bool HasBeenSet() const;
  void Unset() const;

#if 0
  template <typename T> void SetValue(T const& v)
    {
    assert(m_Definition.GetType() == FieldTraits<T>::kind);
    FieldDecodingTraits<T>::Set(m_Feature.ogr(), m_index, v);
    }
#else
  template <typename T> void SetValue(T const& value)
    {
    const int VALUE = boost::mpl::at<metaprog::FieldType_Map, T>::type::value;
    typedef typename boost::mpl::at<metaprog::FieldType_Map, T>::type Kind;
    BOOST_STATIC_ASSERT(!(boost::is_same<Kind, boost::mpl::void_>::value));
    assert(m_Definition.GetType() == Kind::value);
    typedef typename boost::mpl::at<metaprog::FieldSetters_Map, Kind>::type SetterType;
    BOOST_STATIC_ASSERT(!(boost::is_same<SetterType, boost::mpl::void_>::value));
    assert(m_index >= 0 && int(m_index) < m_Feature->GetFieldCount());
    SetterType::call(*m_Feature, m_index, value);
    }
#endif
  template <typename T> T GetValue() const
    {
    const int VALUE = boost::mpl::at<metaprog::FieldType_Map, T>::type::value;
    typedef typename boost::mpl::at<metaprog::FieldType_Map, T>::type Kind;
    BOOST_STATIC_ASSERT(!(boost::is_same<Kind, boost::mpl::void_>::value));
    assert(m_Definition.GetType() == Kind::value);
    typedef typename boost::mpl::at<metaprog::FieldGetters_Map, Kind>::type GetterType;
    BOOST_STATIC_ASSERT(!(boost::is_same<GetterType, boost::mpl::void_>::value));
    assert(m_index >= 0 && int(m_index) < m_Feature->GetFieldCount());
    return GetterType::call(*m_Feature, m_index);
    }

  std::ostream & PrintSelf(std::ostream&os, itk::Indent indent) const;
private:
  FieldDefn                       m_Definition;
  boost::shared_ptr<OGRFeature> & m_Feature;
  // all the fields decoding is at the wrong place (OGRFeature instead of
  // OGRField) => need for an index
  size_t                          m_index;
  };

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRFieldWrapper.txx"
#endif

#endif // __otbOGRFieldWrapper_h
