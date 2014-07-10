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
#ifndef __otbGeometriesToGeometriesFilter_h
#define __otbGeometriesToGeometriesFilter_h

/**\ingroup GeometriesFilters
 * \file    otbGeometriesToGeometriesFilter.h
 * \since   OTB v 3.14.0
 * Commons definitions for geometries filter definition.
 * This file contains all the main classes required to define new geometries
 * transformations.
 */

#include "otbGeometriesSource.h"
#include <map>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include "otbOGRFeatureWrapper.h"
#include "otbOGRLayerWrapper.h"

// forward declarations
namespace otb { namespace internal {
struct ProcessVisitor;
} } // otb::internal namespace

namespace otb
{
/**\ingroup GeometriesFilters
 * \class GeometriesToGeometriesFilter
 * Root abstract class for all geometries to geometries filters.
 * \since OTB v 3.14.0
 */
class ITK_EXPORT GeometriesToGeometriesFilter : public GeometriesSource
  {
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesToGeometriesFilter         Self;
  typedef itk::ProcessObject                   Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  //@}

  /**\name I/O typedefs */
  //@{
  typedef GeometriesSet                        InputGeometriesType;
  typedef GeometriesSet                        OutputGeometriesType;
  // typedef GeometriesSet::Pointer               InputGeometriesPointer;
  // typedef GeometriesSet::Pointer               OutputGeometriesPointer;
  //@}

  /**\name Standard ITK macros */
  //@{
  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometriesToGeometriesFilter, GeometriesSource);
  //@}

  /**\name Single input GeometriesSet property */
  //@{
  virtual void SetInput(const InputGeometriesType *input);
  const InputGeometriesType * GetInput(void);
  //@}

protected:
  /** Default constructor.
   * Does nothing.
   */
  GeometriesToGeometriesFilter();
  /** Destructor.
   * Does nothing.
   */
  virtual ~GeometriesToGeometriesFilter();

  /** Processes the input to fill the output.
   * This is the main processing function. It either works \em in-place or by
   * \em copying the transformed input \c Feature s into the output.
   */
  virtual void GenerateData(void);

private:
  /** \e In-place processing function.
   * \param[in,out] inout the geometries set that'll be modified by the filter.
   * \pre The filter must have been initialized
   */
  void Process(OutputGeometriesType &inout);
  /** \e By-copy processing function.
   * \param[in] source the geometries set to transform
   * \param[out] destination the resulting geometries set.
   * \pre The filter must have been initialized
   * \pre The \c destination must be ready to receive a result.
   */
  void Process(InputGeometriesType const& source, OutputGeometriesType &destination);

  /**\name Filter specialization hooks.
   * Filter specialization hooks.
   * These functions are meant to be specialized by the actual filter.
   */
  //@{
  /**
   * Hook that actually filters an OGR \c Layer.
   * \param[in]     source      Input layer
   * \param[in,out] destination Output layer
   *
   * \note When <tt>source == destination</tt>, it means this is an inplace
   * filter.
   */
  virtual void                     DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination) const=0;

  /**
   * Hook used to determine the \c OGRSpatialReference when creating a new layer.
   * \param[in] source  source \c Layer for reference (in case it has relevant
   * information).
   * \return a new \c OGRSpatialReference that the client code shall release
   * with the appropiate OGR function.
   * \return 0 by default.
   * \todo Return a <tt>unique_ptr<OGRSpatialReference></tt>.
   */
  virtual OGRSpatialReference*     DoDefineNewLayerSpatialReference(ogr::Layer const& source) const;

  /**
   * Hook used to determine the type of the new layer.
   * \param[in] source  source \c Layer.
   * \return by default the same type as the one from the \c source \c Layer.
   */
  virtual OGRwkbGeometryType       DoDefineNewLayerGeometryType(ogr::Layer const& source) const;

  /**
   * Hook used to determine the options for  creating a new layer.
   * \param[in] source  source \c Layer for reference (in case it has relevant
   * information).
   * \return nothing by default.
   */
  virtual std::vector<std::string> DoDefineNewLayerOptions(ogr::Layer const& source) const;

  /**
   * Hook used to define the fields of the new layer.
   * \param[in] source  source \c Layer -- for reference
   * \param[in,out] dest  destination \c Layer
   */
  virtual void DoDefineNewLayerFields(ogr::Layer const& source, ogr::Layer & dest) const = 0;

  /** Hook used to conclude the initialization phase.
   * As ITK doesn't follow a <em>the constructor set the object in a final, and
   * ready to be used, state</em>, this step is required to do that once all
   * filter attributes have been set.
   *
   * The default implementation does nothing.
   */
  virtual void                     DoFinalizeInitialisation() {}
  //@}
  friend struct otb::internal::ProcessVisitor;
  };

/**\ingroup GeometriesFilters
 * Helper class to operate an exact copy of the fields from a source layer.
 * \since OTB v 3.14.0
 */
struct FieldCopyTransformation
  {
  OGRFeatureDefn & getDefinition(ogr::Layer & outLayer) const
    {
    return outLayer.GetLayerDefn();
    }
  /**
   * In-place transformation: does nothing.
   * \param[in] inoutFeature  \c Feature to change.
   * \throw Nothing
   */
    void fieldsTransform(ogr::Feature const& itkNotUsed(inoutFeature)) const
    {
    // default => do nothing for in-place transformation
    }
  /**
   * By-Copy transformation: copies all fields.
   * \param[in] inFeature  input \c Feature
   * \param[in,out] outFeature  output \c Feature
   *
   * \throw itk::ExceptionObject if the fields cannot be copied.
   */
  void fieldsTransform(ogr::Feature const& inFeature, ogr::Feature & outFeature) const;

  /**
   * Defines the fields in the destination layer.
   * The default action is to copy all fieds from one layer to another.
   * \param[in] source  source \c Layer
   * \param[in,out] dest  destination \c Layer
   * \throw itk::ExceptionObject in case the operation can't succeed.
   */
  void DefineFields(ogr::Layer const& source, ogr::Layer & dest) const;
private:
  /** Associative table to know how fields are mapped from one layer to another.
   * This table is necesary as:
   * - some data source drivers add their own fields (as a consequence, the
   * number of fields differ between two layers).
   * - other data source drivers truncate the name of the fields (as a
   * consequence, we can't rely on field names).
   * \todo \c std::map may not be the fastest structure available => use a
   * (sorted?) vector of pairs of ints, and search with a simple \c std::find
   * (as we can expect the number of fields to be quite low).
   */
  mutable std::map<int,int> m_SourceToDestFieldIndicesMap;
  };


/**\ingroup GeometriesFilters
 * \class TransformationFunctorDispatcher
 * Policy to call the transformation functor on \c OGRGeometry (even when the
 * actual transformed element is a \c ogr::Layer).
 * \tparam TransformationFunctor actual transformation functor
 * \tparam TransformedElementType geometries to transform.
 * This policy-class is specialized for \c ogr::Layer and \c OGRGeometry.
 *
 * This functor dispatcher is meant to work:
 * - on a single \c ogr::Layer (in that case, it acts as a in-place filter).
 * - or on a pair of \c ogr::Layer s (in that case, it acts as a copy filter).
 *
 * Specializations are expected to provide:
 * - a typedef \c TransformedElementType which is expected to match \c
 * TransformationFunctor::TransformedElementType;
 * - one overload of \c operator() one that takes a reference to a \c ogr::Layer
 * for \em in-place transformation;
 * - another overload of \c operator() one that takes a const reference, and a
 * reference to \c ogr::Layer for \em by-copy transformation;
 * - and one overload to <tt>operator->()</tt> that returns a pointer to the
 * actual \c TransformationFunctor.
 * \since OTB v 3.14.0
 * \todo Add a specialization for \c ogr::Feature.
 */
template <class TransformationFunctor, class TransformedElementType, class FieldTransformationPolicy = FieldCopyTransformation>
struct TransformationFunctorDispatcher
  {
  };

/**\ingroup GeometriesFilters
 * Specialization for \c ogr::Layer.
 * It simply forwards the call to the internal functor that'll have to do all
 * the transformation work.
 *
 * If you need to change the number of elements in a layer, use a \c
 * TransformationFunctor that works on layers. The dispatching mecanism will
 * automatically end up here.
 * \tparam TransformationFunctor actual transformation functor
 * \since OTB v 3.14.0
 */
template <class TransformationFunctor, class FieldTransformationPolicy>
struct TransformationFunctorDispatcher<TransformationFunctor, ogr::Layer, FieldTransformationPolicy>
: FieldTransformationPolicy
  {
  typedef typename TransformationFunctor::TransformedElementType TransformedElementType;
  BOOST_MPL_ASSERT((boost::is_same<ogr::Layer, TransformedElementType>));
  TransformationFunctorDispatcher() { }
  TransformationFunctorDispatcher(TransformationFunctor functor) : m_functor(functor){ }
  void operator()(ogr::Layer const& in, ogr::Layer & out) const;
  void operator()(ogr::Layer & inout) const;
  TransformationFunctor * operator->() { return &m_functor; }
private:
  TransformationFunctor m_functor;
  };

/**\ingroup GeometriesFilters
 * Specialization for \c OGRGeometry.
 * \tparam TransformationFunctor actual transformation functor
 *
 * In this case, the \c TransformationFunctor works on \c OGRGeometry (instead
 * of \c ogr::Layer). The default behaviour provided is to loop on all the \c
 * ogr::Feature s of the original layer, and for each feature:
 * - build a new one (in case of the \emm by-copy policy),
 * - build a new geometry thanks to the \c TransformationFunctor, which will be
 * assigned to the output feature,
 * - transform the fields from the input layer,
 * - and update the feature in the output layer.
 *
 * \note In the case of the \em by-copy policy, fields transformation cannot
 * change the number of fields nor their nature. Only field values may be
 * modified.
 * \since OTB v 3.14.0
 */
template <class TransformationFunctor, class FieldTransformationPolicy>
struct TransformationFunctorDispatcher<TransformationFunctor, OGRGeometry, FieldTransformationPolicy>
: FieldTransformationPolicy
  {
  typedef typename TransformationFunctor::TransformedElementType TransformedElementType;
  BOOST_MPL_ASSERT((boost::is_same<OGRGeometry, TransformedElementType>));
  TransformationFunctorDispatcher() { }
  TransformationFunctorDispatcher(TransformationFunctor functor) : m_functor(functor){}

  void operator()(ogr::Layer const& in, ogr::Layer & out) const;
  void operator()(ogr::Layer & inout) const;
  TransformationFunctor * operator->() { return &m_functor; }
private:
  TransformationFunctor m_functor;
  };


/**\ingroup GeometriesFilters
 * \class DefaultGeometriesToGeometriesFilter
 * Generic helper class to filter geometries sets given a tranformation functor.
 * This generic class provides the default behaviour for most filters that we
 * may need to implement.
 *
 * Thanks to it, we just need to provide a \c TransformationFunctor, and a \c
 * FieldTransformationPolicy (if the default no-transformation policy is not
 * what we need).
 * \since OTB v 3.14.0
 * \todo Find a better name
 */
template <class TransformationFunctor, class FieldTransformationPolicy = FieldCopyTransformation>
class ITK_EXPORT DefaultGeometriesToGeometriesFilter
: public GeometriesToGeometriesFilter
, public TransformationFunctorDispatcher<TransformationFunctor, typename TransformationFunctor::TransformedElementType, FieldTransformationPolicy>
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef DefaultGeometriesToGeometriesFilter                    Self;
  typedef GeometriesToGeometriesFilter                           Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;
  //@}
  /**\name Class typedefs */
  //@{
  typedef TransformationFunctor                                  TransformationFunctorType;
  typedef typename TransformationFunctor::TransformedElementType TransformedElementType;
  typedef TransformationFunctorDispatcher<TransformationFunctorType, TransformedElementType, FieldTransformationPolicy>
                                                                 TransformationFunctorDispatcherType;
  //@}

  /**\name Standard macros */
  //@{
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DefaultGeometriesToGeometriesFilter, GeometriesToGeometriesFilter);
  //@}

protected:
  /** Default constructor. */
  DefaultGeometriesToGeometriesFilter();
  /** Destructor. */
  virtual ~DefaultGeometriesToGeometriesFilter();

  /**
   * Hook that actually filters an OGR \c Layer.
   * \param[in]     source      Input layer
   * \param[in,out] destination Output layer
   *
   * This specialization just forwards the transformation to the \c
   * m_TransformationFunctor.
   * \note When <tt>source == destination</tt>, it means this is an \em in-place
   * filter.
   */
  virtual void DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination) const;
  /**
   * Hook used to define the fields of the new layer.
   * \param[in] source  source \c Layer -- for reference
   * \param[in,out] dest  destination \c Layer
   *
   * Just forwards the fields definition to the \c FieldTransformationPolicy
   * inherited from the \c TransformationFunctorDispatcherType.
   */
  virtual void DoDefineNewLayerFields(ogr::Layer const& source, ogr::Layer & dest) const
    {
    this->DefineFields(source, dest);
    }
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeometriesToGeometriesFilter.txx"
#endif

#endif // __otbGeometriesToGeometriesFilter_h
