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
#ifndef __otbSpectralAngleDataNodeFeatureFunction_h
#define __otbSpectralAngleDataNodeFeatureFunction_h

#include "otbDataNodeImageFunction.h"

#include "otbBinarySpectralAngleFunctor.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkLineConstIterator.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class SpectralAngleDataNodeFeatureFunction
  * \brief Compute a spectral angle based feature alongside a
  * datanode.
  *
  * This function compute a spectral angle alongside a datanode.
  * The feature is the mean spectral angle regarding a
  * reference pixel alongside the tested datanode.
  *
  * The output has three elements:
  * - #0: mean spectral angle of a neighborhood around the datanode, weighted to fit in [0, 1] interval
  * - #1: accumulator of the spectral angle alongside the datanode
  * - #2: number of visited pixel
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
class ITK_EXPORT SpectralAngleDataNodeFeatureFunction :
    public DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef SpectralAngleDataNodeFeatureFunction                Self;
  typedef DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpectralAngleDataNodeFeatureFunction, DataNodeImageFunction);

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

  itkGetConstMacro(Radius, unsigned int);
  itkSetMacro(Radius, unsigned int);

protected:
  SpectralAngleDataNodeFeatureFunction();
  ~SpectralAngleDataNodeFeatureFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SpectralAngleDataNodeFeatureFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** SpectralAngle Functor & ReferencePixel*/
  ReferencePixelType          m_RefPixel;
  SpectralAngleFunctorType    m_SpectralAngleFunctor;

  /** Neighborhood radius */
  unsigned int m_Radius;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpectralAngleDataNodeFeatureFunction.txx"
#endif

#endif
