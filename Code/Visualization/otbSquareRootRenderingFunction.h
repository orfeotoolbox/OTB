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
#ifndef __otbSquareRootRenderingFunction_h
#define __otbSquareRootRenderingFunction_h

#include <cassert>
#include <iomanip>
#include <vector>

#include "otbMacro.h"
#include "otbI18n.h"
#include "otbChannelSelectorFunctor.h"
#include "otbStandardRenderingFunction.h"
#include "otbViewerConst.h"


namespace otb
{
  namespace Function
  {
    /** \class SquareRootRenderingFunction
     * \brief Square root stretch rendering.
     * If the input image is an Image, the function renders it with R, G and
     * B channels all equals.
     * If it is a VectorImage, the function renders the selected channels.
     * \ingroup Visualization
     */
    template <class TPixel, class TRGBPixel,
              class TPixelRepresentationFunction = ChannelSelectorFunctor<TPixel>,
              class TTransferFunction = Identity<
                typename itk::NumericTraits<typename itk::NumericTraits<TPixel>::ValueType>::RealType,
                typename itk::NumericTraits<typename itk::NumericTraits<TPixel>::ValueType>::RealType
                > >
    class SquareRootRenderingFunction
      : public StandardRenderingFunction<TPixel, TRGBPixel, TPixelRepresentationFunction, TTransferFunction>
    {
      public:
        /** SquareRoot class typedefs */
        typedef SquareRootRenderingFunction                  Self;
        typedef StandardRenderingFunction<TPixel, TRGBPixel> Superclass;
        typedef itk::SmartPointer<Self>                      Pointer;
        typedef itk::SmartPointer<const Self>                ConstPointer;

        /** type macro */
        //itkTypeMacro(SquareRootRenderingFunction, RenderingFunction);
        itkTypeMacro(SquareRootRenderingFunction, StandardRenderingFunction);

        /** new macro */
        itkNewMacro(Self);

        /** PixelType macros */
        typedef TRGBPixel                                         OutputPixelType;
        typedef typename OutputPixelType::ValueType               OutputValueType;
        typedef TPixel                                            PixelType;
        typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
        typedef typename itk::NumericTraits<ScalarType>::RealType RealScalarType;

      protected:
        /**
         * Constructor
         */
        SquareRootRenderingFunction() {}

        /**
         * Destructor
         */
        virtual ~SquareRootRenderingFunction() {}

        /**
         * Perform the computation for a single value (this is done in order
         * to have the same code for vector and scalar version)
         */
        const OutputValueType ClampRescale(RealScalarType input, RealScalarType min, RealScalarType max) const
        {
          if (input > max)
          {
            return SCREEN_COLOR_MAX_VALUE;
          }
          else if (input < min)
          {
            return SCREEN_COLOR_MIN_VALUE;
          }
          else
          {
            double sqrtMin = vcl_sqrt(static_cast<double>(min));
            double sqrtVal = vcl_sqrt(static_cast<double>(input));
            double sqrtMax = vcl_sqrt(static_cast<double>(max));

            return static_cast<OutputValueType>(vcl_floor(SCREEN_COLOR_MAX_VALUE
                                                          * (sqrtVal - sqrtMin) / (sqrtMax - sqrtMin)
                                                          + 0.5));
          }
        }

      private:

        SquareRootRenderingFunction(const Self&); //purposely not implemented
        void operator=(const Self&); //purposely not implemented

    };

  } // end namespace Functor

} // end namespace otb

#endif /* __otbSquareRootRenderingFunction_h */
