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
#ifndef otbParserConditionDataNodeFeatureFunction_h
#define otbParserConditionDataNodeFeatureFunction_h

#include "otbDataNodeImageFunction.h"
#include "otbPolyLineImageConstIterator.h"
#include "otbMaskMuParserFunctor.h"

namespace otb
{
/** \class ParserConditionDataNodeFeatureFunction
  * \brief Compute a ParserCondition based feature alongside a
  * datanode.
  *
  * This function evaluate a condition alongside a datanode.
  * The descriptor is the percentage of pixels iterated for which
  * the condition is true.
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa SpectralAngleDataNodeFeatureFunction
 *
 * \ingroup OTBMathParser
  */

template <
class TImage,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT ParserConditionDataNodeFeatureFunction :
    public DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef ParserConditionDataNodeFeatureFunction                         Self;
  typedef DataNodeImageFunction<TImage, std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ParserConditionDataNodeFeatureFunction, DataNodeImageFunction);

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

  typedef Functor::MaskMuParserFunctor<PixelType>      ParserConditionFunctorType;
  typedef typename ParserConditionFunctorType::Pointer ParserConditionFunctorPointer;

  typedef std::vector<PrecisionType>                  OutputType;

  OutputType Evaluate( const DataNodeType& node ) const ITK_OVERRIDE;

  std::string GetExpression() const
  {
    return m_ParserConditionFunctor->GetExpression();
  }

  void SetExpression(std::string expression)
  {
    m_ParserConditionFunctor->SetExpression(expression);
  }

  void SetSpectralAngleReferencePixel(const PixelType& pixel)
  {
    m_ParserConditionFunctor->SetSpectralAngleReferencePixel(pixel);
  }

protected:
  ParserConditionDataNodeFeatureFunction();
  ~ParserConditionDataNodeFeatureFunction() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  ParserConditionDataNodeFeatureFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** ParserCondition Functor */
  ParserConditionFunctorPointer  m_ParserConditionFunctor;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbParserConditionDataNodeFeatureFunction.txx"
#endif

#endif
