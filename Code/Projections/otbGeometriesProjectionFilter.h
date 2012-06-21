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
#ifndef __otbGeometriesProjectionFilter_h
#define __otbGeometriesProjectionFilter_h

#include "otbGeometriesToGeometriesFilter.h"
#include "otbOGRGeometryWrapper.h"
#include "otbImageReference.h"
#include "itkTransform.h"
#include "otbGenericRSTransform.h"

class OGRCoordinateTransformation;

namespace otb
{

/**\ingroup Projection GeometriesFilters
 * Internal functor used to reproject a \c OGRGeometry.
 *
 * \internal
 * As \c OGRGeometry isn't open to new functions through a \em Visitor design
 * pattern, this class use a nasty hack: it try to downcast to any possible
 * subtype of \c OGRGeometry.
 *
 * \since OTB v 3.14.0
 * \todo Move into an \c internal namespace.
 */
struct ReprojectTransformationFunctor
{
  typedef OGRGeometry TransformedElementType;

  /**\ingroup Projection GeometriesFilters
   * Internal functor used to reproject a \c OGRGeometry: \em By-Copy transformation policy.
   * \since OTB v 3.14.0
   */
  struct ByCopy
    {
    ByCopy(ReprojectTransformationFunctor const& reprojector) : m_Reprojector(reprojector){}
    template <typename TGeometry>
      ogr::UniqueGeometryPtr operator()(TGeometry const* in) const;
  private:
    ReprojectTransformationFunctor const& m_Reprojector;
    };

  /**\ingroup Projection GeometriesFilters
   * Internal functor used to reproject a \c OGRGeometry: \em In-Place transformation policy.
   * \since OTB v 3.14.0
   */
  struct InPlace
    {
    InPlace(ReprojectTransformationFunctor const& reprojector) : m_Reprojector(reprojector){}
    template <typename TGeometry>
      void operator()(TGeometry * inout) const;
  private:
    ReprojectTransformationFunctor const& m_Reprojector;
    };


  ogr::UniqueGeometryPtr operator()(OGRGeometry const* in) const;
  // ogr::UniqueGeometryPtr apply(OGRGeometry const* in) const;
  void apply_inplace          (OGRGeometry * inout) const;

  typedef otb::GenericRSTransform<double, 2, 2>   InternalTransformType;
  typedef InternalTransformType::Pointer          InternalTransformPointerType;
  void SetOnePointTransformation(InternalTransformPointerType transform)
    {
    m_Transform = transform;
    }
private:
  void do_transform(OGRPoint              & g) const;
  // void do_transform(OGRLinearRing         & g) const;
  void do_transform(OGRLineString         & g) const;
  // void do_transform(OGRCurve              & g) const;
  void do_transform(OGRPolygon            & g) const;
#if 0
  void do_transform(OGRSurface            & g) const;
  void do_transform(OGRMultiLineString    & g) const;
  void do_transform(OGRMultiPoint         & g) const;
  void do_transform(OGRMultiPolygon       & g) const;
#endif
  void do_transform(OGRGeometryCollection & g) const;

  InternalTransformPointerType m_Transform;
};


/**\ingroup Projection GeometriesFilters
 * \class GeometriesProjectionFilter
 * Projection filter for OGR geometries sets.
 * \since OTB v 3.14.0
 *
 * \param[in] InputGeometriesSet
 * \param[in] InputKeywordList if the \em InputGeometriesSet doesn't have a
 * projection reference (i.e. a \c OGRSpatialReference), this filter will use
 * the \em InputKeywordList to describe the positionning of the geometries set.
 *
 * \param[in,out] OutputGeometriesSet This set of geometries needs to be given to
 * the filter (in order to set the exact output file/OGR driver). However the
 * filter is in charge of filling the geometries set.
 * \param[in] OutputProjectionRef wkt description of the \c OGRSpatialReference
 * to project the \em InputGeometriesSet into.
 * \param[in] OutputKeywordList if no \em OutputProjectionRef is set, the
 * projection will be done according to the \em OutputKeywordList.
 *
 * \note Unlike \c VectorDataProjectionFilter, we have to explicitly set which
 * to use between projection reference or keyword list. There is no \em
 * MetaDataDictionary property.
 */
class ITK_EXPORT GeometriesProjectionFilter : public GeometriesToGeometriesFilter
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesProjectionFilter    Self;
  typedef GeometriesToGeometriesFilter  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  /**\name Standard macros */
  //@{
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometriesProjectionFilter, GeometriesToGeometriesFilter);
  //@}

  /**\name I/O typedefs */
  //@{
  typedef Superclass::InputGeometriesType        InputGeometriesType;
  // typedef Superclass::InputGeometriesPointer  InputGeometriesPointer;
  typedef Superclass::OutputGeometriesType       OutputGeometriesType;
  // typedef Superclass::OutputGeometriesPointer OutputGeometriesPointer;
  typedef ogr::ImageReference<double>            ImageReference;

  //@}

private:
  virtual OGRSpatialReference* DoDefineNewLayerSpatialReference(ogr::Layer const& source) const;
  virtual void DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination) const;
  virtual void DoFinalizeInitialisation();
  virtual void DoDefineNewLayerFields(ogr::Layer const& source, ogr::Layer & dest) const;

protected:
  /** Default constructor. */
  GeometriesProjectionFilter();
  /** Destructor. */
  virtual ~GeometriesProjectionFilter();

  virtual void GenerateOutputInformation(void);

public:
  /**\name Image Reference (origin, spacing) */
  //@{
  void SetInputSpacing(ImageReference::SpacingType const& spacing)
    {
    m_InputImageReference.SetSpacing(spacing);
    }
  void SetOutputSpacing(ImageReference::SpacingType const& spacing)
    {
    m_OutputImageReference.SetSpacing(spacing);
    }
  void SetInputOrigin(ImageReference::OriginType const& origin)
    {
    m_InputImageReference.SetOrigin(origin);
    }
  void SetOutputOrigin(ImageReference::OriginType const& origin)
    {
    m_OutputImageReference.SetOrigin(origin);
    }
  //@}
  /**\name Keywords lists accessors and mutators */
  //@{
  itkGetMacro(InputKeywordList, ImageKeywordlist);
  void SetInputKeywordList(const ImageKeywordlist& kwl)
    {
    this->m_InputKeywordList = kwl;
    this->Modified();
    }

  itkGetMacro(OutputKeywordList, ImageKeywordlist);
  void SetOutputKeywordList(const ImageKeywordlist& kwl)
    {
    this->m_OutputKeywordList = kwl;
    this->Modified();
    }
  //@}

  /**\name Projection references accessors and mutators
   * Projection references accessors and mutators.
   * As the Input Projection Reference comes from the Input Geometries set, it
   * can't be set independently.
   */
  //@{
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);
  //@}
private:
  /**\name Functor definition */
  //@{
  typedef ReprojectTransformationFunctor                          TransformationFunctorType;
  typedef TransformationFunctorType::TransformedElementType       TransformedElementType;
  typedef TransformationFunctorDispatcher<TransformationFunctorType, TransformedElementType, FieldCopyTransformation>
                                                                  TransformationFunctorDispatcherType;

  TransformationFunctorDispatcherType             m_TransformationFunctor;
  //@}

  /**\name 1 Point Transformation definition */
  //@{
  typedef TransformationFunctorType::InternalTransformType        InternalTransformType;
  typedef TransformationFunctorType::InternalTransformPointerType InternalTransformPointerType;

  InternalTransformPointerType                    m_Transform;
  //@}

  /**\name Image Reference (origin, spacing) */
  //@{
  ImageReference                                  m_InputImageReference;
  ImageReference                                  m_OutputImageReference;
  //@}

  std::string                                     m_OutputProjectionRef; // in WKT format!
  ImageKeywordlist                                m_InputKeywordList;
  ImageKeywordlist                                m_OutputKeywordList;

  std::string                                     m_DEMDirectory;
  std::string                                     m_GeoidFile;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbGeometriesProjectionFilter.txx"
#endif

#endif // __otbGeometriesProjectionFilter_h
