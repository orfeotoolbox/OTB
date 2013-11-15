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
#ifndef __otbInverseLogPolarTransform_h
#define __otbInverseLogPolarTransform_h

#include "otbTransform.h"

namespace otb
{
/** \class InverseLogPolarTransform
 * \brief This transform is the back transform of the LogPolarTransform.
 *
 * Given (x, y) the coordinates of a point in cartesian system, the corresponding
 * log-polar coordinates are :
 * \f$ \rho = 1/2*log((x-xc)^2+(y+yc)^2) \f$
 * \f$ \theta = asin(y-yc)/(\sqrt{(x-xc)^2+(y+yc)^2}) \f$
 *
 * In this implemenatation, theta is expressed in degree, and the result of the asin function
 * is clamped to the [0, 360] range. Please note that since the transform of the center has no meaning
 * it is rejected to the point [400, 0], which does not belong to this range.  This is done to provide
 * a coordinate not likely to belong to a log-polar image buffer.
 *
 * \sa LogPolarTransform
 * \ingroup  Transform
 */

template <class TScalarType>
class ITK_EXPORT InverseLogPolarTransform
  : public Transform<TScalarType, 2, 2>
{
public:
  /** Standard typedef */
  typedef InverseLogPolarTransform          Self;
  typedef Transform<TScalarType, 2, 2> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  /** Creation through object factory */
  itkNewMacro(Self);
  /** Runtime information */
  itkTypeMacro(InverseLogPolarTransform, Transform);

  /** Template related typedefs */
  typedef TScalarType ScalarType;

  /** Superclass related typedefs */
  typedef typename Superclass::InputPointType      InputPointType;
  typedef typename Superclass::OutputPointType     OutputPointType;
  typedef typename Superclass::InputVectorType     InputVectorType;
  typedef typename Superclass::OutputVectorType    OutputVectorType;
  typedef typename Superclass::InputVnlVectorType  InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType OutputVnlVectorType;
  typedef typename Superclass::ParametersType      ParametersType;
  typedef itk::FixedArray<TScalarType, 2>          ScaleType;

  /** Set/Get the origin */
  itkSetMacro(Center, InputPointType);
  itkGetConstReferenceMacro(Center, InputPointType);

  /** Set/Get the scales */
  itkSetMacro(Scale, ScaleType);
  itkGetConstReferenceMacro(Scale, ScaleType);
  /**
   * Set the transform parameters through the standard interface.
   * \param parameters The parameters of the transform.
   */
  void SetParameters(const ParametersType& parameters);
  /**
   * Get the transform parameters through the standard interface.
   * \return The parameters of the transform.
   */
  virtual ParametersType& GetParameters(void) const;
    /**
   * Set the Fixed Parameters
   * \param The Fixed parameters of the transform.
   */
  virtual void SetFixedParameters( const ParametersType & param)
    { this->m_FixedParameters = param;}
  /**
   * Get the Fixed Parameters
   * \return The Fixed parameters of the transform.
   */
  virtual const ParametersType& GetFixedParameters(void) const{return this->m_FixedParameters;}
  /**
   * Transform a point.
   * \param point The point to transform.
   * \return The transformed point.
   */
  OutputPointType TransformPoint(const InputPointType& point) const;
  /**
   * Transform a vector representing a point.
   * \param vector The point to transform.
   * \return The transformed point.
   */
  OutputVectorType TransformVector(const InputVectorType& vector) const;
  /**
   * Transform a vnl vector representing a point.
   * \param vector The point to transform.
   * \return The transformed point.
   */
  OutputVnlVectorType TransformVector(const InputVnlVectorType& vector) const;

protected:
  /** Constructor */
  InverseLogPolarTransform();
  /** Destructor */
  virtual ~InverseLogPolarTransform();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  InverseLogPolarTransform(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  InputPointType m_Center;
  ScaleType      m_Scale;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInverseLogPolarTransform.txx"
#endif

#endif
