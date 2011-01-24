/*
 * InverseFilter.h
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEINVERSEFILTER_H
#define __VAHINEINVERSEFILTER_H

#include <itkImageToImageFilter.h>
#include <vnl/vnl_vector_ref.h>

#include "VahineMacro.h"
#include "VahineInterpol.h"

namespace otb {
template <class TInputImage, class TOutputImage> class ITK_EXPORT VahineInverseFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage > {
public:
  typedef VahineInverseFilter                                Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;
  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineInverseFilter, ImageToImageFilter);

  typedef TInputImage                                              InputImageType;
  typedef typename TInputImage::Pointer                            InputImagePointer;
  typedef TOutputImage                                             OutputImageType;
  typedef typename TOutputImage::Pointer                           OutputImagePointer;
  typedef typename TInputImage::PixelType                          PixelType;
  typedef typename TInputImage::InternalPixelType                  InternalPixelType;
  typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef vnl_vector<InternalPixelType>                            VnlVectorType;
  typedef itk::ImageRegionConstIterator<InputImageType>            ConstVectorIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>                OutIterator;

  typedef typename vahine::VahineInterpol<InternalPixelType> * InterpolPointer;

  itkSetMacro( GrsirAxe, VnlVectorType );
  itkSetMacro( Interpolation, InterpolPointer)
  bool GetDebug(){
    return true;
  };
protected:
  VahineInverseFilter();
  virtual ~VahineInverseFilter(){
  };
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const {
    Superclass::PrintSelf(os, indent);
  };
  void GenerateData();

private:
  VahineInverseFilter( const Self& ); //purposely not implemented
  void operator=( const Self& );      //purposely not implemented

  VnlVectorType   m_GrsirAxe;
  InterpolPointer m_Interpolation;
};
} //end namespace otb

#include "InverseFilter.txx"

#endif /* INVERSEFILTER_H_ */