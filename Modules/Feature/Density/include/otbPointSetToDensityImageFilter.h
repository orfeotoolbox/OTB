/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbPointSetToDensityImageFilter_h
#define otbPointSetToDensityImageFilter_h

#include "itkProcessObject.h"
#include "itkPointSet.h"
#include "itkPointSetToImageFilter.h"
#include "otbPointSetDensityFunction.h"
#include "itkPoint.h"

namespace otb
{

/** \class PointSetToDensityImageFilter
 *  \brief Draw the density of a point set on an image
 *
 * \ingroup OTBDensity
 */

template <class TInputPointSet, class TOutputImage,
    class TDensityFunction = PointSetDensityFunction<TInputPointSet, typename TOutputImage::PixelType> >
class ITK_EXPORT PointSetToDensityImageFilter
  : public itk::PointSetToImageFilter<TInputPointSet, TOutputImage>
{

public:

  /** Standard class typedefs. */
  typedef PointSetToDensityImageFilter                             Self;
  typedef itk::PointSetToImageFilter<TInputPointSet, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetToDensityImageFilter, itk::PointSetToImageFilter);

  /**   typedefs parameters support */
  typedef TInputPointSet PointSetType;

  typedef TOutputImage                          OutputImageType;
  typedef typename  OutputImageType::PixelType  PixelType;
  typedef typename  OutputImageType::IndexType  IndexType;
  typedef typename  OutputImageType::RegionType OutputImageRegionType;

  /**   typedef filter support*/
  typedef TDensityFunction                                PointSetDensityFunctionType;
  typedef typename PointSetDensityFunctionType::InputType InputType;
  typedef typename PointSetDensityFunctionType::Pointer   PointSetDensityFunctionPointerType;

  /** Set/Get Radius*/
  itkGetMacro(Radius, unsigned int);
  itkSetMacro(Radius, unsigned int);

protected:

  /**
   * Constructor.
   */
  PointSetToDensityImageFilter();
  /**
   * Destructor.
   */
  ~PointSetToDensityImageFilter() ITK_OVERRIDE {}
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /**
   * Call the ImageSource::GenerateData which handle multithreading
   */
  void GenerateData() ITK_OVERRIDE;

  /**
   * Main computation method.
   */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId) ITK_OVERRIDE;

  /**
  * Main computation method.
  */
  void  GenerateOutputInformation() ITK_OVERRIDE;

private:

  PointSetToDensityImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  unsigned int m_Radius;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetToDensityImageFilter.txx"
#endif

#endif
