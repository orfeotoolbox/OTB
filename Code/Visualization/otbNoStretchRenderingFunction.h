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
#ifndef __otbNoStretchRenderingFunction_h
#define __otbNoStretchRenderingFunction_h

#include <cassert>
#include <iomanip>
#include <vector>

#include "otbMacro.h"
#include "otbI18n.h"
#include "otbChannelSelectorFunctor.h"
#include "otbStandardRenderingFunction.h"
#include "otbViewerConst.h"

#include "otbRenderingFunction.h"


namespace otb
{
  namespace Function
  {
    // Identity class template defined in otbStandardRenderingFunction.h
    // template <class TInputPixel, class TOutputPixel> class Identity;

    /** \class NoStretchRenderingFunction
     * \brief No stretch rendering (0-255 clipping).
     * If the input image is an Image, the function renders it with R, G and
     * B channels all equals.
     * If it is a VectorImage, the function renders the selected channels.
     *  \ingroup Visualization
     */
    template <class TPixel, class TRGBPixel,
              class TPixelRepresentationFunction = ChannelSelectorFunctor<TPixel>,
              class TTransferFunction = Identity<
                typename itk::NumericTraits<typename itk::NumericTraits<TPixel>::ValueType>::RealType,
                typename itk::NumericTraits<typename itk::NumericTraits<TPixel>::ValueType>::RealType
                > >
    class NoStretchRenderingFunction
      : public StandardRenderingFunction<TPixel, TRGBPixel, TPixelRepresentationFunction, TTransferFunction>
    {
      public:
        /** Standard class typedefs */
        typedef NoStretchRenderingFunction                   Self;
        typedef StandardRenderingFunction<TPixel, TRGBPixel> Superclass;
        typedef itk::SmartPointer<Self>                      Pointer;
        typedef itk::SmartPointer<const Self>                ConstPointer;

        /** type macro */
        //itkTypeMacro(NoStretchRenderingFunction, RenderingFunction);
        itkTypeMacro(NoStretchRenderingFunction, StandardRenderingFunction);

        /** new macro */
        itkNewMacro(Self);

        /** PixelType macros */
        typedef TRGBPixel                                         OutputPixelType;
        typedef typename OutputPixelType::ValueType               OutputValueType;
        typedef TPixel                                            PixelType;
        typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
        typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
        typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
        typedef typename itk::NumericTraits<ScalarType>::RealType RealScalarType;
        typedef itk::VariableLengthVector<RealScalarType>         InternalPixelType;
        typedef typename Superclass::ParametersType               ParametersType;

        // MDE
        typedef TTransferFunction                                 TransferFunctionType;
        typedef TPixelRepresentationFunction                      PixelRepresentationFunctionType;

        typedef typename PixelRepresentationFunctionType::Pointer         PixelRepresentationFunctionPointerType;
        typedef typename PixelRepresentationFunctionType::ChannelListType ChannelListType;
        // \MDE

        /**
         * Convert the output of the pixel representation to a RGB pixel on
         * unsigned char to be displayed on screen. Values are contrained to
         * 0-255 with a transfer function and a clamping operation.
         * If the pixel representation gives one band, the image is displayed
         * on grey level
         * If it gives 3 bands, it is interpreted as Red, Green and Blue
         * If there is a fourth band, it is considered as the alpha channel
         * and is not scaled.
         */
        virtual OutputPixelType EvaluateTransferFunction(const InternalPixelType &  spixel) const
        {
          if ((spixel.Size() != 1) && (spixel.Size() != 3) && (spixel.Size() != 4))
          {
            itkExceptionMacro( << "the PixelRepresentation function should give an output of "
                               << "size 1, 3 or 4 otherwise I don't know how to make an RGB of it !" );
          }

          OutputPixelType output(SCREEN_COLOR_MAX_VALUE);

          if (spixel.Size() == 1)
          {
            OutputValueType value = ClippedValue(this->m_TransferFunction(spixel[0]));
            output[0] = value;
            output[1] = value;
            output[2] = value;
          }
          else
          {
            output[0] = ClippedValue(this->m_TransferFunction(spixel[0]));
            output[1] = ClippedValue(this->m_TransferFunction(spixel[1]));
            output[2] = ClippedValue(this->m_TransferFunction(spixel[2]));
          }

          if ((spixel.Size() == 4) && (output.Size() == 4))
          {
            assert((spixel[3] >= SCREEN_COLOR_MIN_VALUE) && (spixel[3] <= SCREEN_COLOR_MAX_VALUE));
            output[3] = static_cast<OutputValueType>(spixel[3]); //just copy the alpha channel
          }

          return output;
        }

        virtual void Initialize() //FIXME should disappear and be automatic (IsModified())
        {
          if ((this->GetMTime() > this->m_UTime)
              || (this->GetPixelRepresentationFunction()->GetMTime() > this->m_UTime))
            //NOTE: we assume that Transfer function have no parameters
          {
            if ((this->GetListSample()).IsNotNull())
            {
              // The size of the Vector was unknow at construction time for
              // the m_PixelRepresentationFunction, now, we may get a better
              // default
              if (this->m_PixelRepresentationFunction->IsUsingDefaultParameters())
              {
                // Case of image with 4 bands or more : Display in the
                // B, G, R, NIR channel order
                if (this->GetListSample()->GetMeasurementVectorSize() >= 4)
                {
                  this->m_PixelRepresentationFunction->SetRedChannelIndex(2);
                  this->m_PixelRepresentationFunction->SetGreenChannelIndex(1);
                  this->m_PixelRepresentationFunction->SetBlueChannelIndex(0);
                }

                // Classic case
                if (this->GetListSample()->GetMeasurementVectorSize() == 3)
                {
                  this->m_PixelRepresentationFunction->SetRedChannelIndex(0);
                  this->m_PixelRepresentationFunction->SetGreenChannelIndex(1);
                  this->m_PixelRepresentationFunction->SetBlueChannelIndex(2);
                }

              }
            }

            this->RenderHistogram();

            this->m_UTime.Modified();
          }
        }

        /**
         * Overloaded method of the RenderingFunction class
         */
        virtual void SetParameters(const ParametersType & /* parameters */)
        {
          otbMsgDevMacro(<< "NoStretchRenderingFunction::SetParameters: no parameters needed");
        }

        /**
         * Overloaded method of the RenderingFunction class
         */
        virtual ParametersType GetParameters() const
        {
          unsigned int   nbBands = this->m_PixelRepresentationFunction->GetOutputSize();
          ParametersType param;
          param.SetSize(2 * nbBands);

          for (unsigned int i = 0; i < nbBands; ++i)
          {
            // Min Band
            param.SetElement(2 * i, SCREEN_COLOR_MIN_VALUE);
            // Max Band
            param.SetElement(2 * i + 1, SCREEN_COLOR_MAX_VALUE);
          }

          return param;
        }

        /**
         * Overloaded method of the RenderingFunction class
         */
        virtual void SetAutoMinMax(bool /*val*/)
        {
        }

      protected:
        /**
         * Constructor
         */
        NoStretchRenderingFunction() {}

        /**
         * Destructor
         */
        virtual ~NoStretchRenderingFunction() {}

        /**
         * Perform the computation for a single value (this is done in
         * order to have the same code for vector and scalar version)
         */
        const OutputValueType ClippedValue(RealScalarType input) const
        {
          if (input > SCREEN_COLOR_MAX_VALUE)
          {
            return SCREEN_COLOR_MAX_VALUE;
          }
          else if (input < SCREEN_COLOR_MIN_VALUE)
          {
            return SCREEN_COLOR_MIN_VALUE;
          }
          else
          {
            return static_cast<OutputValueType>(input);
          }
        }

      private:

        NoStretchRenderingFunction(const Self&); //purposely not implemented
        void operator=(const Self&); //purposely not implemented

    };

  } // end namespace Functor

} // end namespace otb

#endif /* __otbNoStretchRenderingFunction_h */
