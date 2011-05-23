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
#include "otbDataNode.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkLineConstIterator.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction
  * \brief Compute a spectral angle based feature alongside a
  * datanode.
  *
  * This function compute a spectral angle alongside a datanode.
  * The feature is the mean spectral angle regarding a
  * reference pixel alongside the tested datanode.
  *
  * Furthermore, it compute the spectral in a neighborhood region located between
  * +/- m_StartNeighborhoodRadius and StopNeighborhoodRadius.
  *
  * The output has five elements:
  * - #0: \$f \frac{mean spectral angle alongside the datanode}{mean spectral angle of the neighborhood} \$f
  * - #1: cumulate spectral angle alongside the datanode
  * - #2: number of pixel alongside the datanode
  * - #3: cumulate spectral angle of the neighborhood
  * - #4: number of pixel in the neighborhood
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

  virtual OutputType Evaluate( const DataNodeType& node ) const;

  /** Set/Get methods */
  itkGetConstMacro(RefPixel, PixelType);
  itkSetMacro(RefPixel, PixelType);

  itkGetConstMacro(CenterRadius, unsigned int);
  itkSetMacro(CenterRadius, unsigned int);

  itkGetConstMacro(NeighborhoodBeginRadius, unsigned int);
  itkSetMacro(NeighborhoodBeginRadius, unsigned int);

  itkGetConstMacro(NeighborhoodEndRadius, unsigned int);
  itkSetMacro(NeighborhoodEndRadius, unsigned int);

protected:
  RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction();
  ~RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** SpectralAngle Functor & ReferencePixel*/
  ReferencePixelType          m_RefPixel;
  SpectralAngleFunctorType    m_SpectralAngleFunctor;

  /** Start neighborhod radius */
  unsigned int m_CenterRadius;
  unsigned int m_NeighborhoodBeginRadius;
  unsigned int m_NeighborhoodEndRadius;

};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction.txx"
#endif

#endif
