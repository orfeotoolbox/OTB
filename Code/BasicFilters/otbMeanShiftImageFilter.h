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
#ifndef _otbMeanShiftImageFilter_h
#define _otbMeanShiftImageFilter_h

#include "otbMeanShiftImageFilterBase.h"

namespace otb
{
  /** \class MeanShiftImageFilter
   *    
   * This is the implementation of the mean shift edge-preserving smoothing filter adapted to
   * otb::Image or itk::Image. For the full documentation, please refer to the
   * base class otb::MeanShiftImageFilterBase.
   *
   * \sa MeanShiftImageFilterBase
   * \ingroup ImageEnhancement
   * \ingroup Streamed
   * \ingroup Threaded
   */
template <class TInputImage, class TOutputImage, class TPrecision = double>
class ITK_EXPORT MeanShiftImageFilter
: public MeanShiftImageFilterBase<TInputImage,TOutputImage,TPrecision>
  {
    public:
    /** Standard class typedef */
    typedef MeanShiftImageFilter                              Self;
    typedef MeanShiftImageFilterBase<TInputImage,
    TOutputImage,TPrecision>                                  Superclass;
    typedef itk::SmartPointer<Self>                           Pointer;
    typedef itk::SmartPointer<const Self>                     ConstPointer;
   
    // Precision pixel type 
    typedef typename Superclass::InputPixelType     InputPixelType;
    typedef typename Superclass::PrecisionPixelType PrecisionPixelType;
    typedef typename Superclass::OutputPixelType    OutputPixelType;
    

    /** New and Type macros */
    itkNewMacro(Self);
    itkTypeMacro(MeanShiftImageFilter,MeanShiftImageFilterBase);
    
    protected:
    /** Constructor */
    MeanShiftImageFilter(){};
    
    /** destructor */
    ~MeanShiftImageFilter(){};
    
    /**PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
    
    /** Redefinition of the GetNumberOfComponentsPerPixel() adapted for Image */
    inline virtual unsigned int GetNumberOfComponentsPerPixel();
    
    /** Redefinition of the InitValue() method adapted for Image
     *  Please be aware that this function modifies the reference to value 
     */
    inline virtual void InitValue(PrecisionPixelType & value, const unsigned int& nbComponents);
    
    /** Redefinition of the SquaredNorm() method adapted for Image */
    inline virtual double SquaredNorm(const PrecisionPixelType& value);

    /** Redefinition of CastInputPixelToPrecisionPixel() method adapted form Image */
    inline virtual const PrecisionPixelType CastInputPixelToPrecisionPixel(const InputPixelType & pixel);

    /** Redefinition of CastPrecisionPixelToOutputPixel() method adapted form Image */
    inline virtual const OutputPixelType CastPrecisionPixelToOutputPixel(const PrecisionPixelType & pixel);

    private:
    MeanShiftImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&);             //purposely not implemented
  };
}// end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftImageFilter.txx"
#endif

#endif
