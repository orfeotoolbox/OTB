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
#ifndef __otbOGRFeatureWrapper_h
#define __otbOGRFeatureWrapper_h

// #include <iosfwd> // std::ostream&
#include <cassert>
#include <boost/shared_ptr.hpp>
#include "itkIndent.h"
#include "otbOGRFieldWrapper.h"

class OGRFeature;
class OGRGeometry;
class OGRFeatureDefn;

namespace otb {
namespace ogr {
class Feature;
bool operator==(Feature const& lhs, Feature const& rhs);

/**\ingroup Geometry
 * \class Feature
 * \invariant <tt>m_Feature != 0</tt>
 */
class Feature
  {
public:
  // no default constructor
  Feature(OGRFeatureDefn & definition);
  Feature(OGRFeature * feature);
  ~Feature();

  Feature clone() const;
  void PrintSelf(std::ostream &os, itk::Indent indent) const;

  OGRFeature & ogr() const;
  OGRFeature & ogr();
  boost::shared_ptr<OGRFeature>      & sptr()       {return m_Feature; }
  boost::shared_ptr<OGRFeature> const& sptr() const {return m_Feature; }

  /**\name Fields */
  //@{
  size_t GetSize() const;
  Field       operator[](size_t index);
  Field const operator[](size_t index) const;
  //@}

  friend bool otb::ogr::operator==(Feature const& lhs, Feature const& rhs);
private:
  void CheckInvariants() const;
  boost::shared_ptr<OGRFeature> m_Feature;
  };


} // ogr namespace
} // otb namespace


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRFeatureWrapper.txx"
#endif

#endif // __otbOGRFeatureWrapper_h
