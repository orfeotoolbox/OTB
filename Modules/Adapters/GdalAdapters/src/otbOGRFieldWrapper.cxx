/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "ogr_feature.h"
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#include "ogr_feature.h"
#pragma warning(pop)
#else
#include "ogr_feature.h"
#endif
#include "otbOGRFeatureWrapper.h"
#include "otbJoinContainer.h"
#include <boost/array.hpp>
/*===========================================================================*/
/*===========================[ Static Assertions ]===========================*/
/*===========================================================================*/
namespace otb
{
namespace ogr
{
namespace internal
{
BOOST_STATIC_ASSERT(!(boost::is_same<MemberGetterPtr<int, &OGRFeature::GetFieldAsInteger>, MemberGetterPtr<double, &OGRFeature::GetFieldAsDouble>>::value));

BOOST_STATIC_ASSERT(!(boost::is_same<int, float>::value));
BOOST_STATIC_ASSERT(!(boost::is_same<mpl::int_<OFTReal>::type, mpl::int_<OFTString>::type>::value));
BOOST_STATIC_ASSERT(!(boost::is_same<mpl::at<FieldType_Map, float>, mpl::void_>::value));
BOOST_STATIC_ASSERT(!(boost::is_same<mpl::at<FieldType_Map, double>, mpl::int_<OFTReal>>::value));
BOOST_STATIC_ASSERT(!(boost::is_same<mpl::at<FieldType_Map, double>::type, mpl::at<FieldType_Map, int>::type>::value));
BOOST_STATIC_ASSERT((mpl::at<FieldType_Map, double>::type::value != mpl::at<FieldType_Map, int>::type::value));
BOOST_STATIC_ASSERT(!(boost::is_same<mpl::at<FieldGetters_Map, mpl::int_<OFTReal>>, mpl::at<FieldGetters_Map, mpl::int_<OFTString>>>::value));

BOOST_MPL_ASSERT((boost::is_array<int[42]>));
BOOST_MPL_ASSERT_NOT((boost::is_array<boost::array<int, 42>>));
BOOST_MPL_ASSERT_NOT((boost::is_array<std::vector<int>>));

BOOST_STATIC_ASSERT((boost::is_contiguous<int*>::value));
BOOST_STATIC_ASSERT((boost::is_contiguous<int[42]>::value));
BOOST_STATIC_ASSERT((boost::is_contiguous<boost::array<int, 42>>::value));
BOOST_STATIC_ASSERT((boost::is_contiguous<std::vector<int>>::value));

BOOST_MPL_ASSERT((boost::is_same<int, CppToOGRConverter_trait<int>::type>));
BOOST_MPL_ASSERT((boost::is_same<char*, CppToOGRConverter_trait<char[8]>::type>));
BOOST_MPL_ASSERT_NOT((boost::is_same<char*, char[8]>));
}
}
} // end namespace otb::ogr

/*===========================================================================*/
/*===============================[ FieldDefn ]===============================*/
/*===========================================================================*/

std::string otb::ogr::FieldDefn::GetName() const
{
  assert(m_Definition && "No field definition wrapped");
  return m_Definition->GetNameRef();
}

OGRFieldType otb::ogr::FieldDefn::GetType() const
{
  assert(m_Definition && "No field definition wrapped");
  return m_Definition->GetType();
}


std::ostream& otb::ogr::operator<<(std::ostream& os, otb::ogr::FieldDefn const& defn)
{
  return os << defn.GetName() << " (" << OGRFieldDefn::GetFieldTypeName(defn.GetType()) << ")";
}
/*===========================================================================*/
/*=================================[ Field ]=================================*/
/*===========================================================================*/

otb::ogr::Field::Field(otb::ogr::Feature& feature, int index) : m_Definition(*feature.ogr().GetFieldDefnRef(index)), m_Feature(feature.sptr()), m_index(index)
{
  CheckInvariants();
}

std::ostream& otb::ogr::Field::UncheckedPrintSelf(std::ostream& os, itk::Indent indent) const
{
  // const itk::Indent one_indent = itk::Indent().GetNextIndent();
  // os << indent << "|" << one_indent << "+ ";
  os << indent << this->GetName() << ": ";
  switch (this->GetType())
  {
  case OFTInteger:
    os << this->GetValue<int>();
    break;
  case OFTIntegerList:
    os << '{';
    otb::Join(os, this->GetValue<std::vector<int>>(), ", ") << '}';
    break;
  case OFTReal:
    os << this->GetValue<double>();
    break;
  case OFTRealList:
    os << '{';
    otb::Join(os, this->GetValue<std::vector<double>>(), ", ") << '}';
    break;
  case OFTString:
    os << this->GetValue<std::string>();
    break;
  default:
    os << "??? -> " << this->GetType();
    break;
  }
  os << " (" << OGRFieldDefn::GetFieldTypeName(this->GetType()) << ")";
  os << "\n";
  return os;
}

bool otb::ogr::Field::UncheckedHasBeenSet() const
{
  return otb::ogr::IsFieldSetAndNotNull(m_Feature.get(), m_index);
}

void otb::ogr::Field::UncheckedUnset() const
{
  m_Feature->UnsetField(m_index);
}

void otb::ogr::Field::UncheckedAssign(Field const& f)
{
  if (f.HasBeenSet())
  {
    OGRField& of = f.ogr();
    m_Feature->SetField(m_index, &of); // OGR API copies *of
  }
  else // not sure OGR setField handle the case where new fields are unset
  {
    this->Unset();
  }
}
