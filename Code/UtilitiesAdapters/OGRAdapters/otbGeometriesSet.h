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
#ifndef __otbGeometriesSet_h
#define __otbGeometriesSet_h

#include <boost/variant.hpp>
#include "itkDataObject.h"
// #include "otbOGRLayerWrapper.h"
#include "otbOGRDataSourceWrapper.h"

namespace otb
{
class ITK_EXPORT GeometriesSet : public itk::DataObject
  {
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesSet                 Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  /**\name Standard macros */
  //@{
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometriesSet, itk::DataObject);
  //@}

  /**\name Geometries accessors */
  //@{
  void Set(ogr::Layer layer);
  void Set(ogr::DataSource::Pointer datasource);
  //@}

  template <typename Visitor>
    void apply(Visitor const& visitor)
      {
      boost::apply_visitor(visitor, m_geometriesSet);
      }

  template <typename Visitor>
    void apply(Visitor const& visitor) const
      {
      boost::apply_visitor(visitor, m_geometriesSet);
      }

  template <typename Visitor>
    void apply(Visitor const& visitor, Self & visitable)
      {
      boost::apply_visitor(visitor, m_geometriesSet, visitable.m_geometriesSet);
      }

  template <typename Visitor>
    void apply(Visitor const& visitor, Self /*const*/& visitable) const
      {
      boost::apply_visitor(visitor, m_geometriesSet, visitable.m_geometriesSet);
      }


protected:
  /** Default constructor. */
  GeometriesSet();
  /** Destructor. */
  virtual ~GeometriesSet();

  /** Prints self to stream. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
private:
  typedef boost::variant<ogr::DataSource::Pointer, ogr::Layer> AnyGeometriesSetType;
  AnyGeometriesSetType m_geometriesSet;
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbGeometriesSet.txx"
#endif

#endif // __otbGeometriesSet_h
