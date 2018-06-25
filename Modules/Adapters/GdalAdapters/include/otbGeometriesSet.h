/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbGeometriesSet_h
#define otbGeometriesSet_h

#include <boost/variant.hpp>
// #include "otbOGRLayerWrapper.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbImageReference.h"

namespace otb
{
/**\ingroup gGeometry
 * \class GeometriesSet
 * \brief Common ITK type to manipulate any set of geometries: a \c otb::ogr::DataSource, or a \c otb::ogr::Layer.
 * \since OTB v 3.14.0
 *
 * \ingroup OTBGdalAdapters
 */
class OTBGdalAdapters_EXPORT GeometriesSet : public itk::DataObject
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
  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometriesSet, itk::DataObject);
  //@}

  /**\name Factory functions */
  //@{
  /** ITK method for creation through the object factory. */
  itkNewMacro(Self);

  /// Direct builder from an existing \c otb::DataSource.
  static Pointer New(ogr::DataSource::Pointer datasource);
  /// Direct builder from an existing \c otb::Layer.
  static Pointer New(ogr::Layer layer);
  //@}

  /**\name Geometries accessors */
  //@{
  void Set(ogr::DataSource::Pointer datasource);
  void Set(ogr::Layer layer);
  //@}

  /**\name Application of a command to a geometries set, through a visitor
   */
  //@{
  template <typename Visitor>
    typename Visitor::result_type
    /**
     * Unary visiting function to apply a visitor on any kind of geometies set.
     *
     * As a \c GeometriesSet contains either a \c otb::ogr::DataSource, or a \c
     * otb::ogr::Layer, one needs to apply a function on either kind of data.
     * This can be done thanks to a specialization of \c boost::static_visitor<>
     * that has a const \c operator() that takes either one (or two in case of
     * binary visitors) \c otb::DataSource or \c otb::Layer as parameter(s).
     *
     * \throw Whatever is thrown by the actual visitor.
     * \tparam Visitor type of the visitor functor.
     * \param[in] visitor  Functor visitor that shal derive from \c
     * boost::static_visitor.
     */
    apply(Visitor const& visitor)
      {
      return boost::apply_visitor(visitor, m_GeometriesSet);
      }

  /**
   * \copydoc apply(Visitor const& visitor)
   */
  template <typename Visitor>
    typename Visitor::result_type
    apply(Visitor const& visitor) const
      {
      return boost::apply_visitor(visitor, m_GeometriesSet);
      }

  /**
   * Binary visiting function to apply a visitor on any kind of geometies set.
   * \copydetails apply(Visitor const& visitor)
   * \param[in] visitable other geometries set to visit along side with the
   * current geometries set instance.
   */
  template <typename Visitor>
    typename Visitor::result_type
    apply(Visitor const& visitor, Self & visitable)
      {
      return boost::apply_visitor(visitor, m_GeometriesSet, visitable.m_GeometriesSet);
      }

  /**
   * \copydoc apply(Visitor const& visitor, Self& visitable)
   */
  template <typename Visitor>
    typename Visitor::result_type
    apply(Visitor const& visitor, Self /*const*/& visitable) const
      {
      return boost::apply_visitor(visitor, m_GeometriesSet, visitable.m_GeometriesSet);
      }
  //@}


  /** Does it contain a geometries set ?.
   */
  bool IsSet() const;

  /**\name Image reference (spacing + origin) */
  //@{
  typedef ogr::ImageReference<double>                            ImageReference;
  /** \em ImageReference setter. */
  void SetImageReference(ImageReference const& imageReference)
    { m_ImageReference = imageReference; }
  /** \em ImageReference getter. */
  ImageReference const& GetImageReference() const
    { return m_ImageReference; }
  //@}

protected:
  /** Default constructor.
   * This actual geometries set is an in-memory \c otb::ogr::DataSource.
   * It needs to be set to a set of geometries if you expect to serialize it or
   * work on a \c otb::Layer.
   *
   * When using this construction path, you'll likelly need to \c Set() the
   * actual geometries set.
   */
  GeometriesSet();
  /** Init constructor from a \c otb::DataSource. */
  GeometriesSet(ogr::DataSource::Pointer datasource);
  /** Init constructor from a \c otb::Layer. */
  GeometriesSet(ogr::Layer layer);

  /** Destructor. */
  ~GeometriesSet() override;

  /** Prints self to stream. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
private:
  typedef boost::variant<ogr::DataSource::Pointer, ogr::Layer> AnyGeometriesSetType;
  AnyGeometriesSetType m_GeometriesSet;
  ImageReference       m_ImageReference;
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbGeometriesSet.hxx"
#endif

#endif // otbGeometriesSet_h
