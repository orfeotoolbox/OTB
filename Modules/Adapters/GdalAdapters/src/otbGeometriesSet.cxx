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
#include "otbGeometriesSet.h"
#include <cassert>

/*===========================================================================*/
/*================================[ Helpers ]================================*/
/*===========================================================================*/
namespace otb { namespace ogr { namespace internal {

/**\ingroup GeometryInternals
 * Access shim: itk smart-pointer to reference.
 * \param[in] ds  itk smart pointer
 * \return a reference to the pointee.
 * \throw None
 * \pre <tt>ds != 0</tt>
 * \since OTB v 3.14.0
 */
template <typename DataType> inline
  DataType & getRef(typename itk::SmartPointer<DataType> const& ds)
    {
    assert(ds && "unexpected nil datasource");
    return *ds;
    }

/**\ingroup GeometryInternals
 * Access shim: const layer to reference.
 * \param[in] layer layer proxy
 * \return the layer proxy
 * \throw None
 * \since OTB v 3.14.0
 */
inline
otb::ogr::Layer const& getRef(otb::ogr::Layer const& layer)
  {
  return layer;
  }

/**\ingroup GeometryInternals
 * Access shim: layer to reference.
 * \param[in] layer layer proxy
 * \return the layer proxy
 * \throw None
 * \since OTB v 3.14.0
 */
inline
otb::ogr::Layer & getRef(otb::ogr::Layer & layer)
  {
  return layer;
  }
} } } // otb::ogr::internal namespaces


/*===========================================================================*/
/*=======================[ Setting and construction ]========================*/
/*===========================================================================*/
otb::GeometriesSet::GeometriesSet()
: m_GeometriesSet(otb::ogr::DataSource::New())
, m_ImageReference(*this)
{
}

otb::GeometriesSet::GeometriesSet(ogr::DataSource::Pointer datasource)
: m_GeometriesSet(datasource)
, m_ImageReference(*this)
{
  assert(datasource && "unexpected nil datasource");
}

otb::GeometriesSet::GeometriesSet(ogr::Layer layer)
: m_GeometriesSet(layer)
, m_ImageReference(*this)
{
}

/*static*/
otb::GeometriesSet::Pointer otb::GeometriesSet::New(ogr::DataSource::Pointer datasource)
{
  Pointer res = new Self(datasource);
  res->UnRegister();
  return res;
}

/*static*/
otb::GeometriesSet::Pointer otb::GeometriesSet::New(ogr::Layer layer)
{
  Pointer res = new Self(layer);
  res->UnRegister();
  return res;
}

/*virtual*/
otb::GeometriesSet::~GeometriesSet()
{
}

void otb::GeometriesSet::Set(ogr::Layer layer)
{
  assert(layer && "unexpected nil layer");
  m_GeometriesSet = layer;
}

void otb::GeometriesSet::Set(ogr::DataSource::Pointer datasource)
{
  assert(datasource && "unexpected nil datasource");
  m_GeometriesSet = datasource;
}

/*===========================================================================*/
/*=================================[ IsSet ]=================================*/
/*===========================================================================*/
namespace otb { namespace ogr { namespace internal {
/**\ingroup GeometryInternals
 * Visitor (functor) to test whether a geometries set is non-null.
 * \param[in] gs geometries set
 * \throw None
 * \since OTB v 3.14.0
 */
struct IsSetTester : boost::static_visitor<bool>
{
  template <typename T>
    bool operator()(T const& gs) const
      {
      return getRef(gs);
      }
};
} } } // otb::ogr::internal namespaces

bool otb::GeometriesSet::IsSet() const
{
  return this->apply(ogr::internal::IsSetTester());
}

/*===========================================================================*/
/*===============================[ Printing ]================================*/
/*===========================================================================*/
namespace otb { namespace ogr { namespace internal {
/**\ingroup GeometryInternals
 * Visitor (functor) to print a geometries set.
 * \param[in] gs geometries set
 * \throw None
 * \since OTB v 3.14.0
 */
struct Printer : boost::static_visitor<>
{
  Printer(std::ostream& os, itk::Indent indent)
    : m_os(os), m_indent(indent) {}
  void operator()(otb::ogr::Layer layer) const
    {
    assert(layer && "unexpected nil layer...");
    layer.PrintSelf(m_os, m_indent);
    }
  void operator()(otb::ogr::DataSource::Pointer datasource) const
    {
    assert(datasource && "unexpected nil datasource...");
    datasource->Print(m_os, m_indent);
    }
private:
  std::ostream &m_os;
  itk::Indent   m_indent;
};
} } } // otb::ogr::internal namespaces

void otb::GeometriesSet::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->apply(ogr::internal::Printer(os, indent));
}
