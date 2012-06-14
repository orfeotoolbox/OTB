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
#ifndef __otbImageReference_h
#define __otbImageReference_h

#include "itkVector.h"
#include "itkPoint.h"
#include "itkMacro.h"

namespace otb
{
namespace ogr
{
/**\ingroup gGeometry
 * \class ImageReference
 * \todo See how mix it with the \c otb::ogr::DataSource wrapper as it was
 * with \c VectorData.
 * \since OTB v 3.14.0
 */
template <typename TPrecision> class ImageReference
{
public:
  typedef TPrecision PrecisionType;
  enum { Dimension = 2 };
  /**\name Standard ITK typedefs */
  //@{
  itkTypeMacro(ImageReference, void);
  bool GetDebug() const
    {
    return m_Holder.GetDebug();
    }
  //@}
  /**\name Template-parameters typedefs */
  //@{
  typedef itk::Vector<PrecisionType, 2> SpacingType;
  typedef itk::Point<PrecisionType, 2>  OriginType;
  typedef itk::Point<PrecisionType, 2>  PointType;
  //@}

  /** Default constructor.
   * \post <tt>m_Spacing = {1,1}</tt>
   * \post <tt>m_Origin = {0,0}</tt>
   */
  ImageReference(itk::Object const& holder)
    : m_Holder(holder)
    {
    m_Spacing.Fill(1);
    m_Origin.Fill(0);
    }
  /** Init constructor.
   * \post <tt>m_Spacing = spacing</tt>
   * \post <tt>m_Origin = origin</tt>
   */
  ImageReference(SpacingType const& spacing, OriginType const& origin, itk::Object const& holder)
    : m_Holder(holder) , m_Spacing(spacing), m_Origin(origin)
    {
    }

  void Modified() const
    {
    m_Holder.Modified();
    }

  /**\name Origin property
   * Represents the origin of the geometries in the image coordinates system.
   */
  //@{
  itkGetConstReferenceMacro(Origin, OriginType);     //!< Origin getter.
  itkSetMacro(Origin, OriginType);                   //!< Origin setter.
  void SetOrigin(const TPrecision origin[Dimension]) //!< Origin setter.
    {
    const OriginType p(origin);
    this->SetOrigin(p);
    }
  //@}

  /**\name Spacing property
   * Spacing of the geometries to put in the corresponding image coordinates.
   */
  //@{
  itkGetConstReferenceMacro(Spacing, SpacingType);         //!< Spacing getter.
  void SetSpacing(const SpacingType& spacing)              //!< Spacing setter.
    {
    // itkDebugMacro("setting Spacing to " << spacing);
    if (this->m_Spacing != spacing)
      {
      this->m_Spacing = spacing;
      this->Modified();
      }
    }
  void SetSpacing(const TPrecision spacing[Dimension]) //!< Spacing setter.
    {
    const SpacingType s(spacing);
    this->SetSpacing(s);
    }
  //@}

  /**
   * Projects a point from the Data Source coordinates system to the image
   * coordinates system.
   * \param[in] point  point in Data Source coordinates system
   * \param[out] physicalPoint  point in the image coordinates system.
   * \throw None
   */
  void TransformPointToPhysicalPoint(const PointType& point, PointType& physicalPoint) const
    {
    for (size_t i=0; i!=Dimension; ++i)
      physicalPoint[i] = point[i] * m_Spacing[i] + m_Origin[i];
    }

  /**
   * Projects a point from the Data Source coordinates system to the image
   * coordinates system.
   * \param[in] point  point in Data Source coordinates system
   * \return the point projected in the image coordinates system.
   * \throw None
   */
  void TransformPointToPhysicalPoint(const PointType& point) const
    {
    // why no loop on VDimension ?
    PointType physicalPoint;
    for (size_t i=0; i!=Dimension; ++i)
      physicalPoint[i] = point[i] * m_Spacing[i] + m_Origin[i];
    return physicalPoint;
    }

  /**
   * Assignment operator.
   * \param[in] rhs source data to be copied.
   * \throw None
   * \internal
   * Required because of the hack to provide a \c GetDebug() function which is
   * required by \c itkTypeMacro.
   */
  ImageReference& operator=(ImageReference const& rhs)
    {
    m_Spacing = rhs.m_Spacing;
    m_Origin  = rhs.m_Origin;
    return *this;
    }

private:
  itk::Object     const& m_Holder;
  SpacingType            m_Spacing;
  OriginType             m_Origin;
};

} // ogr namespace

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbImageReference.txx"
#endif

#endif // __otbImageReference_h
