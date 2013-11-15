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
#ifndef __otbPointSetToDisplacementFieldGenerator_h
#define __otbPointSetToDisplacementFieldGenerator_h

#include "itkImageSource.h"

namespace otb
{
namespace Functor
{
/** \class DistanceComparisonFunctor
 *  \brief This functor is used in nearest neighborhood sorting.
 *  \ingroup DisparityMap
 */
class DistanceComparisonFunctor
{
public:
  DistanceComparisonFunctor() {}
  virtual ~DistanceComparisonFunctor() {}
  typedef std::vector<double> DistanceVectorType;

  void SetDistanceVector(DistanceVectorType& vec)
  {
    m_DistanceVector = vec;
  }
  DistanceVectorType GetDistanceVector(void)
  {
    return m_DistanceVector;
  }
  inline bool operator ()(const unsigned int a1, const unsigned int a2)
  {
    return m_DistanceVector[a1] < m_DistanceVector[a2];
  }

private:
  DistanceVectorType m_DistanceVector;
};
}
/** \class PointSetToDisplacementFieldGenerator
 *  \brief Base class for filters generating a displacement field from a point set enriched with displacement and local transform information.
 *  The output of the filters deriving from this base class can be passed to the itk::WarpImageFilter for image resampling.
 *  \sa DisparityMapEstimationMethod
 *   \sa itk::WarpImageFilter
   *  \ingroup DisparityMap
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT PointSetToDisplacementFieldGenerator
  : public itk::ImageSource<TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef PointSetToDisplacementFieldGenerator Self;
  typedef itk::ImageSource<TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PointSetToDisplacementFieldGenerator, ImageSource);

  /** Template parameters typedefs */
  typedef TPointSet                      PointSetType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  typedef TDisplacementField                                DisplacementFieldType;
  typedef typename DisplacementFieldType::Pointer           DisplacementFieldPointerType;
  typedef typename DisplacementFieldType::IndexType         IndexType;
  typedef typename DisplacementFieldType::SizeType          SizeType;
  typedef typename DisplacementFieldType::SpacingType       SpacingType;
  typedef typename DisplacementFieldType::PointType         PointType;
  typedef typename DisplacementFieldType::InternalPixelType ValueType;

  /** More typedefs */
  typedef std::vector<double>       DistanceVectorType;
  typedef std::vector<unsigned int> IndexVectorType;

  /**
   * Set the pointset containing the disparity.
   * \param pointset The pointset containing the disparity.
   */
  void SetPointSet(const TPointSet * pointset);
  /**
   * Get the pointset containing the disparity.
   * \return The pointset containing the disparity.
   */
  const TPointSet * GetPointSet(void);

  itkSetMacro(MetricThreshold, double);
  itkGetMacro(MetricThreshold, double);
  itkSetMacro(DefaultValue, ValueType);
  itkGetMacro(DefaultValue, ValueType);
  itkSetMacro(OutputSize, SizeType);
  itkGetConstReferenceMacro(OutputSize, SizeType);
  itkSetMacro(OutputSpacing, SpacingType);
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);
  itkSetMacro(OutputOrigin, PointType);
  itkGetConstReferenceMacro(OutputOrigin, PointType);

protected:
  /** Constructor */
  PointSetToDisplacementFieldGenerator();
  /** Destructor */
  virtual ~PointSetToDisplacementFieldGenerator() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Generate output information */
  virtual void GenerateOutputInformation(void);
  /**
   * Generate the n nearest valid point in point set, where a valid point has a sufficient metric value.
   *  \param index The index of the pixel to compute.
   *  \param n The number of nearest point to seek.
   *  \return A vector containing the index of the nearest point from nearest to most far.
   */
  IndexVectorType GenerateNearestValidPointsPointSet(IndexType index, unsigned int n = 1);

  /** Euclidean distance of point to index */
  double EuclideanDistance(IndexType index, PointType p);

private:
  PointSetToDisplacementFieldGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /**
   * The threshold of metric value.
   */
  double m_MetricThreshold;
  /** The output size */
  SizeType m_OutputSize;
  /** The output spacing. */
  SpacingType m_OutputSpacing;
  /** The output origin */
  PointType m_OutputOrigin;
  /** Default value */
  ValueType m_DefaultValue;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetToDisplacementFieldGenerator.txx"
#endif

#endif
