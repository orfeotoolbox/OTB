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
#ifndef __otbFourierMellinDescriptorsImageFunction_h
#define __otbFourierMellinDescriptorsImageFunction_h

#include "itkImageFunction.h"

#include "otbImage.h"

#include "otbMath.h"
#include <complex>

namespace otb
{

/**
 * \class FourierMellinDescriptorsImageFunction
 * \brief Calculate the Fourier-Mellin descriptors in the specified
 * neighborhood.
 *
 * Calculate the Fourier-Mellin descriptors over a specified neighborhood.
 * The implemented equation is:
 *
 * 
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 */

template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT FourierMellinDescriptorsImageFunction :
    public itk::ImageFunction <TInputImage,
      std::vector< std::vector< double > >,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef FourierMellinDescriptorsImageFunction                                Self;
  typedef itk::ImageFunction<TInputImage, 
                             std::vector<
                             std::vector<double> >, 
                             TCoordRep>                                        Superclass;
  typedef itk::SmartPointer<Self>                                              Pointer;
  typedef itk::SmartPointer<const Self>                                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FourierMellinDescriptorsImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::OutputType          OutputType;  
  
  typedef double                                   ScalarRealType;
  typedef typename std::complex<ScalarRealType>    ScalarComplexType;
  typedef typename std::vector< std::vector< ScalarComplexType > >  ComplexType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  
  /** Evalulate the function at specified index */
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;
  
  /** Evaluate the function at non-integer positions */
  virtual OutputType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  virtual OutputType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }
  
  /** Get/Set the radius of the neighborhood over which the
   *  statistics are evaluated 
   */
  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );
  
  itkSetMacro(Pmax, unsigned int);
  itkGetConstReferenceMacro(Pmax, unsigned int);
  itkSetMacro(Qmax, unsigned int);
  itkGetConstReferenceMacro(Qmax, unsigned int);

protected:
  FourierMellinDescriptorsImageFunction();
  virtual ~FourierMellinDescriptorsImageFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FourierMellinDescriptorsImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  unsigned int m_Pmax;
  unsigned int m_Qmax;
  unsigned int m_NeighborhoodRadius;
  double       m_Sigma;
  
  
  
  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFourierMellinDescriptorsImageFunction.txx"
#endif

#endif
