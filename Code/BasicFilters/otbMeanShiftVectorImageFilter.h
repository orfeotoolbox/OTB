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
#ifndef _otbMeanShiftVectorImageFilter_h
#define _otbMeanShiftVectorImageFilter_h

#include "otbMeanShiftImageFilterBase.h"

namespace otb
{

  /** \class MeanShiftVectorImageFilter
   *    
   * This is the implementation of the mean shift edge-preserving smoothing filter adapted to
   * otb::VectorImage or itk::VectorImage. For the full documentation, please refer to the
   * base class otb::MeanShiftImageFilterBase.
   *
   * \sa MeanShiftImageFilterBase
   * \ingroup ImageEnhancement
   * \ingroup Streamed
   * \ingroup Threaded
   */
  template <class TInputImage, class TOutputImage, class TPrecision = double>
    class ITK_EXPORT MeanShiftVectorImageFilter
    : public MeanShiftImageFilterBase<TInputImage,TOutputImage,itk::VariableLengthVector<TPrecision> >
    {
      public:
      /** Standard class typedef */
      typedef MeanShiftVectorImageFilter                        Self;
      typedef MeanShiftImageFilterBase<TInputImage,TOutputImage,
      itk::VariableLengthVector<TPrecision> >                   Superclass;
      typedef itk::SmartPointer<Self>                           Pointer;
      typedef itk::SmartPointer<const Self>                     ConstPointer;

      /** New and Type macros */
      itkNewMacro(Self);
      itkTypeMacro(MeanShiftVectorImageFilter,MeanShiftImageFilterBase);
    
      /** Template parameters typedefs */
      typedef typename Superclass::PrecisionPixelType PrecisionPixelType;

      protected:
      /** Constructor */
      MeanShiftVectorImageFilter(){};

      /** destructor */
      ~MeanShiftVectorImageFilter(){};

      /**PrintSelf method */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

      /** Redefinition of the GetNumberOfComponentsPerPixel() adapted for VectorImages */
      virtual unsigned int GetNumberOfComponentsPerPixel();

      /** Redefinition of the InitValue() method adapted for vector images 
       *  Please be aware that this function modifies the reference to value 
       */
      virtual void InitValue(PrecisionPixelType & value, const unsigned int& nbComponents);

      /** Redefinition of the SquaredNorm() method adapted for vector images */
      virtual double SquaredNorm(const PrecisionPixelType & value);

      private:
      MeanShiftVectorImageFilter(const Self&); //purposely not implemented
      void operator=(const Self&);             //purposely not implemented
    };
}// end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftVectorImageFilter.txx"
#endif

#endif
