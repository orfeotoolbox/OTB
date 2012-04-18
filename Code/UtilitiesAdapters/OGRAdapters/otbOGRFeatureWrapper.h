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

  OGRFeature const& ogr() const
    {
    CheckInvariants();
    return *m_Feature;
    }
  OGRFeature & ogr()
    {
    CheckInvariants();
    return *m_Feature;
    }

  friend bool otb::ogr::operator==(Feature const& lhs, Feature const& rhs);
private:
  void CheckInvariants() const
    {
    assert(m_Feature && "OGRFeature can't be null");
    }
  boost::shared_ptr<OGRFeature> m_Feature;

  };


} // ogr namespace
} // otb namespace


#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRFeatureWrapper.txx"
#endif

#endif // __otbOGRFeatureWrapper_h
