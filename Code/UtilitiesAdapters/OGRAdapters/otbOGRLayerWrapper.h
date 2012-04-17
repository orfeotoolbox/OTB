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
#ifndef __otbLayer_h
#define __otbLayer_h

// #include <iosfwd> // std::ostream&
#include <boost/shared_ptr.hpp>
// #include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>
#include "itkIndent.h"
#include "otbOGRFeatureWrapper.h"
class OGRLayer; // fwd declarations
class OGRDataSource;
class OGRGeometry;
class OGRFeatureDefn;
class OGRFieldDefn;

namespace otb { namespace ogr {
/**\ingroup Geometry
 * \class Layer
 * \brief %Layer of geometric objets.
 *
 * It provides an encapsulation of OGR classes. In that particular case, it's an
 * encapsulation of \c OGRLayer.
 *
 * \note this class is a proxy class on top of an \c OGRLayer.
 * \note It can be copied.
 * \note when created from a \c otb::ogr::DataSource::ExecuteSQL, it will
 * automatically manage the release of the underlying \c OGRLayer.
 * \note the default constructor is disabled on purpose
 * \note the destructor automatically generated does everything that is
 * expected.
 *
 * \todo find a way to tell whether the related \c OGRDataSource was released
 */
class Layer
  // : public itk::DataObject
  {
public:
  /**\name Standard class typedefs */
  //@{
  typedef Layer                         Self;
  // typedef itk::DataObject               Superclass;
  // typedef itk::SmartPointer<Self>       Pointer;
  // typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  /**\name Standard macros */
  //@{
  // itkNewMacro(Self);
  // itkTypeMacro(Layer, DataObject);
  //@}

  /**\name Construction */
  //@{
  explicit Layer(OGRLayer* layer);
  Layer(OGRLayer* layer, OGRDataSource* sourceInChargeOfLifeTime);
  //@}

  /**\name Features collection */
  //@{
  /** Returns the number of elements in the layer.
   * \param[in] doForceCompuation  indicates whether the size shall be computed
   * even so it's expensive to do so.
   *
   * \return the number of features in the layer, -1 if count is unknown
   * \throw None
   * \sa OGRLayer::GetFeatureCount
   */
  int GetFeatureCount(bool doForceComputation) const;

  void CreateFeature(Feature feature);
  void DeleteFeature(long nFID);
  Feature GetFeature(long nFID);
  void SetFeature(Feature feature);
  //@}

  /** Returns the name given to the layer, if any.
  */
  std::string GetName() const;

  /** Prints self into stream. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;


  /**\copydoc operator int boolean ::* () const
  */
  struct boolean{ int i; };
  /** Can the layer be used (ie not null).
   *
   * Hack to provide a boolean operator that is convertible only to a
   * boolean expression to be used in \c if tests.
   * @see <em>Imperfect C++</em>, Matthew Wilson, Addisson-Welsey, par 24.6
   */
  operator int boolean ::* () const
    {
    return m_Layer ? &boolean::i : 0;
    }

  /** Access to raw \c OGRLayer.
   * This function provides an abstraction leak in case deeper control on the
   * underlying \c OGRLayer is required.
   * \pre the underlying \c OGRLayer must be valid, i.e.
   * <tt>m_Layer != 0</tt>, an assertion is fired otherwise.
   * \warning you must under no circonstance try to delete the \c OGRLayer
   * obtained this way.
   */
  OGRLayer & ogr();

  /**\name Spatial filter property
   * \internal the I/O geometry is an undeletable pointer, can be may be null.
   * \todo we'll see later if a Geometry capsule is defined, or a
   * \c nondeletable<> pointer type.
   */
  //@{
  OGRGeometry const* GetSpatialFilter() const;
  void SetSpatialFilter(OGRGeometry const* spatialFilter);
  //@}

  /**\name Iteration */
  //@{
  /**\ingroup Geometry
   * \class feature_iter
   * Implementation class for \c Feature iterator.
   * \internal
   * \sa otb::ogr::Layer::iterator
   * \sa otb::ogr::Layer::const_iterator
   * \note Naming policy is compliant with C++ standard as the iterator are as
   * well. This will permit transparent integration with all standard and boost
   * algorithms, and C++11 <em>for-range loops</em> for instance.
   * \see http://www.boost.org/doc/libs/1_49_0/libs/iterator/doc/iterator_facade.html#tutorial-example
   */
  template <class Value> class feature_iter
    : public boost::iterator_facade<feature_iter<Value>, Value, boost::single_pass_traversal_tag>
    {
    struct enabler {};
  public:
    feature_iter()
      : m_Layer(0), m_Crt(0) {}
    explicit feature_iter(otb::ogr::Layer & layer)
      : m_Layer(&layer), m_Crt(layer.GetNextFeature()) {}
    template <class OtherValue> feature_iter(
      feature_iter<OtherValue> const& other,
      typename boost::enable_if<boost::is_convertible<OtherValue*,Value*>
      , enabler
      >::type = enabler()
    )
      : m_Layer(other.m_Layer), m_Crt(other.m_Crt)
      {}
  private:
    friend class boost::iterator_core_access;
    template <class> friend class feature_iter;

    template <class OtherValue> bool equal(feature_iter<OtherValue> const& other) const
      { return other.m_Crt == m_Crt; }
    void increment()
      { 
      assert(m_Layer && "cannot increment end()");
      m_Crt = m_Layer->GetNextFeature();
      }
    Value & dereference() const 
      { return m_Crt; }

    otb::ogr::Layer * m_Layer;
    otb::ogr::Feature m_Crt;
    };

  template <class> friend class feature_iter;
  typedef feature_iter<Feature      > iterator;
  typedef feature_iter<Feature const> const_iterator;

  const_iterator begin() const { return cbegin(); }
  const_iterator end  () const { return cend  (); }
  const_iterator cbegin() const ;
  const_iterator cend() const { return iterator(); }
  iterator       begin() ;
  iterator       end() { return iterator(); }
  //@}

  /**\name Features definition */
  //@{
  OGRFeatureDefn & GetLayerDefn() const;

  void CreateField(OGRFieldDefn const& field, bool bApproxOK = true);
  void DeleteField(size_t fieldIndex);
  void AlterFieldDefn(size_t fieldIndex, OGRFieldDefn& newFieldDefn, int nFlags);
  void ReorderField(size_t oldPos, size_t newPos);
  void ReorderFields(int *map);
  void SetIgnoredFields(int *);
  //@}
private:
  /**
   * Internal encapsulation of \c OGRLayer::GetNextFeature().
   *
   * \return the next \c OGRFeature of the layer, encapsulated in a \c Feature.
   * \throw None
   * \internal
   * Required to implement iterators without exposing/including \c OGRFeature
   * class definition.
   */
  Feature GetNextFeature();

  /** Data implementation.
   * \internal
   * The actual %layer implementation belongs to the \c otb::Layer object,
   * unless this is the result of \c ExecuteSQL. In that case a deleter is set
   * to correctly release the layer.
   */
  boost::shared_ptr<OGRLayer> m_Layer;
  };

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbLayer.txx"
#endif

#endif // __otbLayer_h
