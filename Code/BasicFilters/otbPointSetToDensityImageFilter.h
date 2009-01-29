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
#ifndef __otbPointSetToDensityImageFilter_h
#define __otbPointSetToDensityImageFilter_h

#include "itkProcessObject.h"
#include "itkPointSet.h"
#include "itkPointSetToImageFilter.h"


/** \class PointSetToDensityImageFilter
 *  \brief This class extracts key points from an image through a pyramidal gaussian based decomposition
 *
 * This class implements the SURF Key point detector proposed by Tuytelaars and Vangool from the university
 * of Leuven, 2005
 *
 * \li Gaussian Second order derivative Hessian images are computed in each
 *     level and each octave for the input image.
 * \li For each octave, an extremum search is launched on each 3 adjacent scales.
 * \li The Key points detected are the ones extremum in the current , previous and
 *     next scale of reserach. 3 scales are needed for the computation (NumberOfScales >=3).
 * \li Orientation and descriptors are computed for each key point detected.
 *
 * Selected Key Points are stored in an itk::PointSet structure.
 * Points contains the coordinate of the detected point.
 * DataPoints contain the values of the 64 element descriptor for each key point
 * detected through the pyramidal analysis.
 *
 * Orientation is expressed in degree in the range of [0,360]
 *
 *  \sa otb::ImageToDeterminantHessianImage
 */

namespace otb
{
  template <class TInputPointSet , class TOutputImage>
    class ITK_EXPORT PointSetToDensityImageFilter
    : public itk::PointSetToImageFilter<TInputPointSet, TOutputImage >
    {

    public:

    /** Standard class typedefs. */
      typedef PointSetToDensityImageFilter                                Self;
      typedef itk::PointSetToImageFilter<TInputPointSet, TOutputImage>   Superclass;
      typedef itk::SmartPointer<Self>                                     Pointer;
      typedef itk::SmartPointer<const Self>                               ConstPointer;

      /** Method for creation through the object factory. */
      itkNewMacro(Self);

      /** Run-time type information (and related methods). */
      itkTypeMacro(PointSetToDensityImageFilter,itk::PointSetToImageFilter);


      /** Template parameters typedefs*/



    protected:

      /**
       * Constructor.
       */
      PointSetToDensityImageFilter();
      /**
       * Destructor.
       */
      virtual ~PointSetToDensityImageFilter();
      /**
       * Standard PrintSelf method.
       */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
      /**
       * Main computation method.
       */
      virtual void  GenerateData();

  private:

      PointSetToDensityImageFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

    };
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetToDensityImageFilter.txx"
#endif

#endif


