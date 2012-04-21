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

#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
// class OGRFeature;
#include "ogr_feature.h"
class OGRFieldDefn;



#include <iomanip>
template <typename RT, typename CT> void print_adress(RT (CT::*p)(int))
{
  unsigned char const * first = reinterpret_cast<unsigned char *>(&p);
  unsigned char const * last = reinterpret_cast<unsigned char *>(&p + 1);

  for (; first != last; ++first)
    {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
      << (int)*first << ' ';
    }
  std::cout << "\n";
}


namespace types_ { // Anonymous namespace
using namespace boost::mpl;
typedef boost::mpl::map
  < pair<int, int_<OFTInteger> >
  , pair<std::vector<int>, int_<OFTIntegerList> >
  , pair<double, int_<OFTReal> >
  , pair<std::vector<double>, int_<OFTRealList> >
  , pair<std::string, int_<OFTString> >
  , pair<std::vector<std::string>, int_<OFTStringList> >
  > FieldType_Map;

template
  < typename T
  , T ( OGRFeature::*ptr_to_function )(int)
  , typename FinalReturnType = T
  > class member_function_get_ptr
    {
  public:
    static FinalReturnType call(OGRFeature &f, int index)
      {
      // print_adress(ptr_to_function);
      // print_adress(&OGRFeature::GetFieldAsString);
      return (f.*ptr_to_function)(index);
      }
    };

typedef map
  < pair<int_<OFTInteger>, member_function_get_ptr<int,         &OGRFeature::GetFieldAsInteger> >
  , pair<int_<OFTReal>,    member_function_get_ptr<double,      &OGRFeature::GetFieldAsDouble> >
  , pair<int_<OFTString>,  member_function_get_ptr<char const*, &OGRFeature::GetFieldAsString, std::string> >
  > FieldGetters_Map;

BOOST_STATIC_ASSERT(!(boost::is_same<
  member_function_get_ptr<int,         &OGRFeature::GetFieldAsInteger>,
  member_function_get_ptr<double,      &OGRFeature::GetFieldAsDouble>
  >::value
  ));

BOOST_STATIC_ASSERT(!(boost::is_same< int, float >::value));
BOOST_STATIC_ASSERT(!(boost::is_same<
  int_<OFTReal  >::type,
  int_<OFTString>::type
  >::value
  ));
BOOST_STATIC_ASSERT(!(boost::is_same<
  at<FieldType_Map,  float>,
  void_
  >::value
  ));
BOOST_STATIC_ASSERT(!(boost::is_same<
  at<FieldType_Map,  double>,
  int_<OFTReal>
  >::value
  ));
BOOST_STATIC_ASSERT(!(boost::is_same<
  at<FieldType_Map,  double >::type,
  at<FieldType_Map,  int >::type
  >::value
  ));
BOOST_STATIC_ASSERT((
  at<FieldType_Map,  double>::type::value !=
  at<FieldType_Map,  int   >::type::value
  ));
BOOST_STATIC_ASSERT(!(boost::is_same<
  at<FieldGetters_Map, int_<OFTReal> >,
  at<FieldGetters_Map, int_<OFTString> >
  >::value
  ));
} // Anonymous namespace

namespace otb { namespace ogr {
class FieldDefn
  {
public:
  FieldDefn(OGRFieldDefn& definition) : m_definition(&definition){ }
  std::string  GetName() const
    {
    assert(m_definition);
    return m_definition->GetNameRef();
    }
  OGRFieldType GetType() const
    {
    assert(m_definition);
    return m_definition->GetType();
    }

private:
  OGRFieldDefn * m_definition;
  };

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
  static Type Get(OGRFeature &f, int index)
    {
    return f.GetFieldAsInteger(index);
    }
  };

template <> struct FieldDecodingTraitsGetter<double>
  {
  typedef double Type;
  static Type Get(OGRFeature &f, int index)
    {
    return f.GetFieldAsDouble(index);
    }
  };

template <> struct FieldDecodingTraitsGetter<std::string>
  {
  typedef std::string Type;
  static Type Get(OGRFeature &f, int index)
    {
    return f.GetFieldAsString(index);
    }
  };

/*===========================================================================*/
/*=================================[ Field ]=================================*/
/*===========================================================================*/
class Field
  {
public:
  Field(Feature const& feature, int index)
    :   m_Definition(*feature.ogr().GetFieldDefnRef(index))
      , m_Feature(feature)
      , m_index(index)
  {
  assert(feature.ogr().GetFieldDefnRef(index));
  }
  // Field(OGRFeature& field, FieldDefn const& definition);
  FieldDefn const& GetDefinition() const;
  OGRFieldType GetType() const
    { return m_Definition.GetType(); }
  std::string GetName() const
    { return m_Definition.GetName(); }

#if 0
  template <typename T> void SetValue(T const& v)
    {
    assert(m_Definition.GetType() == FieldTraits<T>::kind);
    FieldDecodingTraits<T>::Set(m_Feature.ogr(), m_index, v);
    }
#endif
  template <typename T> T GetValue() const
    {
    const int VALUE = boost::mpl::at<types_::FieldType_Map, T>::type::value;
    typedef typename boost::mpl::at<types_::FieldType_Map, T>::type Kind;
    BOOST_STATIC_ASSERT(!(boost::is_same< Kind, boost::mpl::void_ >::value));
    assert(m_Definition.GetType() == Kind::value);
    typedef typename boost::mpl::at< types_::FieldGetters_Map, Kind >::type function_;
    BOOST_STATIC_ASSERT(!(boost::is_same< function_, boost::mpl::void_ >::value));
    assert(m_index >= 0 && m_index < m_Feature.GetSize());
    return function_::call(m_Feature.ogr(), m_index);
    // return FieldDecodingTraitsGetter<T>::Get(m_Feature.ogr(), m_index);
    }
#if 0
  std::ostream & PrintSelf(std::ostream&os, itk::Indent indent) {
    os << indent;
    switch (GetType)
      {
machin_int:
      os << GetValue<int>();
      break;
    default:

      }
    return os;
  }
#endif
private:
  FieldDefn  m_Definition;
  Feature    m_Feature;
  int        m_index; // all the fields decoding is at the wrong place (OGRFeature instead of OGRField)
  };

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRFieldWrapper.txx"
#endif

#endif // __otbOGRFieldWrapper_h
