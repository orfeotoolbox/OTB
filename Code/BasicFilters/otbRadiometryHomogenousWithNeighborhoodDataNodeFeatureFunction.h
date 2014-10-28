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
#ifndef __otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_h
#define __otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_h

#include "otbDataNodeImageFunction.h"

#include "otbBinarySpectralAngleFunctor.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkLineConstIterator.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction
  * \brief Compute the spectral angle between the radiometry along
  * a line and its neighborhood
  *
  * This function compares the radiometry along a datanode with the
  * radiometry of the neighborhood.
  *
  * It computes the mean of a rectangular area around each line segment,
  * and the mean of two rectangular areas on each side of the line segment.
  * The descriptor is the spectral angle between the two values,
  * scaled by \$f\pi\$f.
  *
  * The width of the area around a line segment can be specified with
  * SetCenterRadius.
  *
  * The neighborhood area can be specified by two radius, using SetNeighborhoodBeginRadius
  * and SetNeighborhoodEndRadius.
  *
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa NDVIDataNodeFeatureFunction
  */

template <
class TImage,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction :
    public DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction                Self;
  typedef DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction, DataNodeImageFunction);

  /** Some typedefs. */
  typedef typename Superclass::DataNodeType           DataNodeType;
  typedef typename DataNodeType::LineType             LineType;
  typedef typename LineType::Pointer                  LinePointer;
  typedef typename LineType::ContinuousIndexType      ContinuousIndexType;
  typedef typename LineType::VertexListConstIteratorType VertexListConstIteratorType;

  typedef TImage                                      InputImageType;
  typedef typename InputImageType::ConstPointer       InputImageConstPointer;
  typedef typename InputImageType::PixelType          PixelType;
  typedef typename InputImageType::InternalPixelType  ScalarRealType;
  typedef typename InputImageType::IndexType          IndexType;
  typedef typename InputImageType::IndexValueType     IndexValueType;

  typedef TCoordRep                                   CoordRepType;

  typedef TPrecision                                  PrecisionType;

  typedef itk::VariableLengthVector<PrecisionType>    ReferencePixelType;

  typedef PolyLineImageConstIterator<InputImageType, LineType> ImageLineIteratorType;
  typedef itk::LineConstIterator<InputImageType>      LineIteratorType;

  typedef Functor::BinarySpectralAngleFunctor<PixelType, ReferencePixelType, PrecisionType>
                                                      SpectralAngleFunctorType;

  typedef std::pair<IndexType, IndexType>             IndexPairType;
  typedef std::vector<PrecisionType>                  OutputType;

  /* Compute the descriptor value along this DataNode */
 virtual OutputType Evaluate( const DataNodeType& node ) const;

  /* Get the radius used to define the area around a line segment.
   * A radius of 0 means that the area is reduced to a line joining
   * the two segments extremities. */
  itkGetConstMacro(CenterRadius, unsigned int);

  /* Set the radius used to define the area around a line segment.
   * A radius of 0 means that the area is reduced to a line joining
   * the two segments extremities. */
  itkSetMacro(CenterRadius, unsigned int);

  /* Get the radius used to define the start of the neighboring area.
   * Typically this value must be greater than CenterRadius */
  itkGetConstMacro(NeighborhoodBeginRadius, unsigned int);

  /* Set the radius used to define the start of the neighboring area.
   * Typically this value must be greater than CenterRadius */
  itkSetMacro(NeighborhoodBeginRadius, unsigned int);

  /* Get the radius used to define the end of the neighboring area.
   * Typically this value must be greater than NeighborhoodBeginRadius */
  itkGetConstMacro(NeighborhoodEndRadius, unsigned int);

  /* Set the radius used to define the end of the neighboring area.
   * Typically this value must be greater than NeighborhoodBeginRadius */
  itkSetMacro(NeighborhoodEndRadius, unsigned int);

protected:
  RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction();
  ~RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Center radius */
  unsigned int m_CenterRadius;

  /** Neighborhood start radius */
  unsigned int m_NeighborhoodBeginRadius;

  /** Neighborhood end radius */
  unsigned int m_NeighborhoodEndRadius;

};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction.txx"
#endif

#endif
