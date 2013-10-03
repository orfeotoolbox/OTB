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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRFieldWrapper.h"
#include "ogr_feature.h"
#include "otbOGRFeatureWrapper.h"
#include "otbJoinContainer.h"
#include <boost/array.hpp>
/*===========================================================================*/
/*===========================[ Static Assertions ]===========================*/
/*===========================================================================*/
namespace otb { namespace ogr {
namespace internal {
BOOST_STATIC_ASSERT(!(boost::is_same<
  MemberGetterPtr<int,         &OGRFeature::GetFieldAsInteger>,
  MemberGetterPtr<double,      &OGRFeature::GetFieldAsDouble>
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

BOOST_MPL_ASSERT((boost::is_array<int[42]>));
BOOST_MPL_ASSERT_NOT((boost::is_array<boost::array<int, 42> >));
BOOST_MPL_ASSERT_NOT((boost::is_array<std::vector<int> >));

BOOST_STATIC_ASSERT((boost::is_contiguous<int*>::value));
BOOST_STATIC_ASSERT((boost::is_contiguous<int[42]>::value));
BOOST_STATIC_ASSERT((boost::is_contiguous<boost::array<int, 42> >::value));
BOOST_STATIC_ASSERT((boost::is_contiguous<std::vector<int> >::value));

BOOST_MPL_ASSERT((boost::is_same<int, CppToOGRConverter_trait<int>::type >));
BOOST_MPL_ASSERT((boost::is_same<char*, CppToOGRConverter_trait<char[8]>::type >));
BOOST_MPL_ASSERT_NOT((boost::is_same<char*, char[8] >));

}
} } // end namespace otb::ogr

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


std::ostream & otb::ogr::operator<<(std::ostream & os, otb::ogr::FieldDefn const& defn)
{
  return os << defn.GetName()
    << " (" << OGRFieldDefn::GetFieldTypeName(defn.GetType()) << ")";
}
/*===========================================================================*/
/*=================================[ Field ]=================================*/
/*===========================================================================*/

otb::ogr::Field::Field(otb::ogr::Feature & feature, size_t index)
: m_Definition(*feature.ogr().GetFieldDefnRef(index))
, m_Feature(feature.sptr())
, m_index(index)
{
  CheckInvariants();
}

std::ostream & otb::ogr::Field::UncheckedPrintSelf(
  std::ostream& os, itk::Indent indent) const
{
  //const itk::Indent one_indent = itk::Indent().GetNextIndent();
  // os << indent << "|" << one_indent << "+ ";
  os << indent << this->GetName() << ": ";
  switch (this->GetType())
    {
  case OFTInteger:
    os << this->GetValue<int>();
    break;
  case OFTIntegerList:
    os << '{';
    otb::Join(os, this->GetValue<std::vector<int> >(), ", ") << '}';
    break;
  case OFTReal   :
    os << this->GetValue<double>();
    break;
  case OFTRealList:
    os << '{';
    otb::Join(os, this->GetValue<std::vector<double> >(), ", ") << '}';
    break;
  case OFTString :
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
  return m_Feature->IsFieldSet(m_index);
}

void otb::ogr::Field::UncheckedUnset() const
{
  m_Feature->UnsetField(m_index);
}

void otb::ogr::Field::UncheckedAssign(Field const& f)
{
  if (f.HasBeenSet())
    {
    OGRField & of = f.ogr();
    m_Feature->SetField(m_index, &of); // OGR API copies *of
    }
  else // not sure OGR setField handle the case where new fields are unset
    {
    this->Unset();
    }
}
