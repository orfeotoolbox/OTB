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
#ifndef __otbNDVIDataNodeFeatureFunction_h
#define __otbNDVIDataNodeFeatureFunction_h

#include "otbDataNodeImageFunction.h"

#include "otbVegetationIndicesFunctor.h"
#include "otbPolyLineImageConstIterator.h"

namespace otb
{
/** \class NDVIDataNodeFeatureFunction
  * \brief Compute a NDVI based feature alongside a
  * datanode.
  *
  * This function compute a feature alongside a datanode.
  * The feature is (NDVI >= threshold) per cent along the
  * tested datanode.
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa SpectralAngleDataNodeFeatureFunction
  */

template <
class TImage,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT NDVIDataNodeFeatureFunction :
    public DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef NDVIDataNodeFeatureFunction                         Self;
  typedef DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NDVIDataNodeFeatureFunction, DataNodeImageFunction);

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

  typedef PolyLineImageConstIterator<InputImageType, LineType>
                                                      ImageLineIteratorType;

  typedef Functor::NDVI<ScalarRealType, ScalarRealType, PrecisionType>
                                                      NDVIFunctorType;

  typedef std::vector<PrecisionType>                  OutputType;

  virtual OutputType Evaluate( const DataNodeType& node ) const;

  /** Set/Get methods */
  itkGetConstMacro(NDVIThreshold, PrecisionType);
  itkSetMacro(NDVIThreshold, PrecisionType);

  //TODO replace by metadata parsing
  unsigned int GetREDChannelIndex() const
  {
    return m_NDVIFunctor.GetRedIndex()+1;
  }

  void SetREDChannelIndex(unsigned int id)
  {
    m_NDVIFunctor.SetRedIndex(id-1);
  }

  unsigned int GetNIRChannelIndex() const
  {
    return m_NDVIFunctor.GetNIRIndex()+1;
  }

  void SetNIRChannelIndex(unsigned int id)
  {
    m_NDVIFunctor.SetNIRIndex(id-1);
  }

protected:
  NDVIDataNodeFeatureFunction();
  ~NDVIDataNodeFeatureFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  NDVIDataNodeFeatureFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** NDVI Threshold & Functor */
  PrecisionType               m_NDVIThreshold;
  NDVIFunctorType             m_NDVIFunctor;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNDVIDataNodeFeatureFunction.txx"
#endif

#endif
