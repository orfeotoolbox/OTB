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
#include "otbDataNode.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class SpectralAngleDataNodeFeatureFunction
  * \brief .
  *
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

  typedef TImage                                      InputImageType;
  typedef typename InputImageType::ConstPointer       InputImageConstPointer;
  typedef typename InputImageType::PixelType          PixelType;
  typedef typename InputImageType::InternalPixelType  ScalarRealType;
  typedef typename InputImageType::IndexType          IndexType;
  typedef typename InputImageType::IndexValueType     IndexValueType;

  typedef TCoordRep                                   CoordRepType;

  typedef TPrecision                                  PrecisionType;

  typedef itk::VariableLengthVector<PrecisionType>    ReferencePixelType;

  typedef PolyLineImageConstIterator<InputImageType, LineType>
                                                      ImageLineIteratorType;
  typedef Functor::BinarySpectralAngleFunctor<PixelType, ReferencePixelType, PrecisionType>
                                                      SpectralAngleFunctorType;

  typedef std::vector<PrecisionType>                  OutputType;

  virtual OutputType Evaluate( const DataNodeType& node ) const;

  /** Set/Get methods */
  itkGetConstMacro(RefPixel, PixelType);
  itkSetMacro(RefPixel, PixelType);

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
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpectralAngleDataNodeFeatureFunction.txx"
#endif

#endif
