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

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/

otb::GeometriesSet::GeometriesSet()
{
}

/*virtual*/  otb::GeometriesSet::~GeometriesSet()
{
}

void otb::GeometriesSet::Set(ogr::Layer layer)
{
  m_geometriesSet = layer;
}

void otb::GeometriesSet::Set(ogr::DataSource::Pointer datasource)
{
  m_geometriesSet = datasource;
}

struct Printer : boost::static_visitor<>
{
  Printer(std::ostream& os, itk::Indent indent)
    : m_os(os), m_indent(indent) {}
  void operator()(otb::ogr::Layer layer) const
    {
    layer.PrintSelf(m_os, m_indent);
    }
  void operator()(otb::ogr::DataSource::Pointer datasource) const
    {
    datasource->Print(m_os, m_indent);
    }
private:
  std::ostream &m_os;
  itk::Indent   m_indent;
};

void otb::GeometriesSet::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->apply(Printer(os, indent));
}
