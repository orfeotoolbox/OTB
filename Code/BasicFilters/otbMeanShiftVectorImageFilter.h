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
#ifndef __otbMeanShiftVectorImageFilter_h
#define __otbMeanShiftVectorImageFilter_h

#include "vcl_deprecated_header.h"
#include "otbMeanShiftImageFilter.h"

namespace otb
{
namespace MeanShift
{
/** \class ScalarBufferConverter
 *  \brief Small utilities class used for buffer conversion with EDISON standard.
 *   This class handles buffer conversion when pixel type is scalar.
 **/
class VectorBufferConverter
{
public:
  /**
   * Fill the pixel with the float array values at index, using nbComp values scaled by scale.
   */
  template <class TPixel> static inline void FloatArrayToPixel(const float * data,
                                                               unsigned int index,
                                                               TPixel& pixel,
                                                               const unsigned int nbComp,
                                                               double scale)
  {
    pixel.SetSize(nbComp);
    for (unsigned int i = 0; i < pixel.Size(); ++i)
      {
      pixel[i] = static_cast<typename TPixel::ValueType> (scale * data[index + i]);
      }
  }
  /**
   * Fill the float array data with the pixel values at index, using nbComp values scaled by scale.
   */
  template <class TPixel> static inline void PixelToFloatArray(float * data,
                                                               unsigned int index,
                                                               const TPixel& pixel,
                                                               double scale)
  {
    for (unsigned int i = 0; i < pixel.Size(); ++i)
      {
      data[index + i] = static_cast<float>(scale * pixel[i]);
      }
  }
};
}

/** \class MeanShiftVectorImageFilter
 *
 * This is the implementation of the mean shift clustering and edge-preserving smoothing filter adapted to
 * otb::VectorImage or itk::VectorImage. For the full documentation, please refer to the
 * base class otb::MeanShiftImageFilter.
 *
 * \sa MeanShiftImageFilter
 * \ingroup ImageEnhancement
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage, class TLabeledOutput = otb::Image<unsigned short, 2> >
class MeanShiftVectorImageFilter
  : public MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput, MeanShift::VectorBufferConverter>
{
public:

  /** Standard class typedef */
  typedef MeanShiftVectorImageFilter Self;
  typedef MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledOutput,
      MeanShift::VectorBufferConverter>                       Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(MeanShiftVectorImageFilter, MeanShiftImageFilter);
  itkNewMacro(Self);

protected:
  /** Constructor */
  MeanShiftVectorImageFilter() {}
  /** destructor */
  virtual ~MeanShiftVectorImageFilter() {}

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  MeanShiftVectorImageFilter(const Self&); //purposely not implemented
  void operator =(const Self&);             //purposely not implemented
};
} // end namespace otb
#endif
