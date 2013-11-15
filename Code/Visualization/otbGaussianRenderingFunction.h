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
#ifndef __otbGaussianRenderingFunction_h
#define __otbGaussianRenderingFunction_h

#include <cassert>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "otbMacro.h"
#include "otbI18n.h"
#include "otbChannelSelectorFunctor.h"
#include "otbStandardRenderingFunction.h"
#include "otbViewerConst.h"
#include <itkGaussianDistribution.h>


namespace otb
{
  namespace Function
  {
    /** \class GaussianRenderingFunction
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
    class GaussianRenderingFunction
      : public StandardRenderingFunction<TPixel, TRGBPixel, TPixelRepresentationFunction, TTransferFunction>
    {
      public:
        /** Gaussian class typedefs */
        typedef GaussianRenderingFunction                    Self;
        typedef StandardRenderingFunction<TPixel, TRGBPixel> Superclass;
        typedef itk::SmartPointer<Self>                      Pointer;
        typedef itk::SmartPointer<const Self>                ConstPointer;

        typedef typename Superclass::ParametersType          ParametersType;


        /** type macro */
        itkTypeMacro(GaussianRenderingFunction, StandardRenderingFunction);

        /** new macro */
        itkNewMacro(Self);

        /** PixelType macros */
        typedef TRGBPixel                                         OutputPixelType;
        typedef typename OutputPixelType::ValueType               OutputValueType;
        typedef TPixel                                            PixelType;
        typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
        typedef typename itk::NumericTraits<ScalarType>::RealType RealScalarType;
        typedef itk::VariableLengthVector<RealScalarType>         InternalPixelType;


        typedef typename Superclass::MetaDataDictionaryType       MetaDataDictionaryType;

        /**
         *
         */
        virtual void Initialize(const MetaDataDictionaryType &metadatadictionary)
        {
          Superclass::Initialize(metadatadictionary);
          EvaluateGaussianDistribution();
        }

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

          if (spixel.Size() != this->m_TransferedMinimum.size())
          {
            itkExceptionMacro( << " m_TransferedMinimum and pixel size do not correspond"
                               << " spixel.Size(): " << spixel.Size()
                               << " m_TransferedMinimum.size(): " << this->m_TransferedMinimum.size());
          }

          OutputPixelType output(SCREEN_COLOR_MAX_VALUE);

          if (spixel.Size() == 1)
          {
            OutputValueType value = ClampRescale(this->m_TransferFunction(spixel[0]), 0);
            output[0] = value;
            output[1] = value;
            output[2] = value;
          }
          else
          {
            output[0] = ClampRescale(this->m_TransferFunction(spixel[0]), 0);
            output[1] = ClampRescale(this->m_TransferFunction(spixel[1]), 1);
            output[2] = ClampRescale(this->m_TransferFunction(spixel[2]), 2);
          }

          if ((spixel.Size() == 4) && (output.Size() == 4))
          {
            assert((spixel[3] >= SCREEN_COLOR_MIN_VALUE) && (spixel[3] <= SCREEN_COLOR_MAX_VALUE));
            output[3] = static_cast<OutputValueType>(spixel[3]); //just copy the alpha channel
          }

          return output;
        }

        /**
         * Set the rendering function parameters:
         * p[0]:    mean value of the gaussian curve
         * p[1]:    standard deviation of the gaussian curve
         * p[2..n]: minimum and maximum for the different bands, has to be
         *          provided as [minBand0, maxBand0, minBand1, maxBand1, ...]
         */
        virtual void SetParameters(const ParametersType & p)
        {
          m_Mean              = p[0];
          m_StandardDeviation = p[1];

          ParametersType psup;
          psup.SetSize(p.GetSize() - 2);
          for (unsigned int i = 0; i < psup.GetSize(); ++i)
          {
            psup[i] = p[i + 2];
          }

          Superclass::SetParameters(psup);

          EvaluateGaussianDistribution();

          otbMsgDevMacro(<< "GaussianRenderingFunction::SetParameters: Gaussian curve mean = "
                         << m_Mean << "; standard deviation = " << m_StandardDeviation);

          this->Modified();
        }

        /**
         * Get the rendering function parameters:
         * p[0]:    mean value of the gaussian curve
         * p[1]:    standard deviation of the gaussian curve
         * p[2..n]: minimum and maximum for the different bands, has to be
         *          provided as [minBand0, maxBand0, minBand1, maxBand1, ...]
         */
        virtual ParametersType GetParameters() const
        {
          ParametersType p;
          ParametersType psup = Superclass::GetParameters();

          p.SetSize(psup.GetSize() + 2);
          p[0] = m_Mean;
          p[1] = m_StandardDeviation;

          for (unsigned int i = 0; i < psup.GetSize(); ++i)
          {
            p[i + 2] = psup[i];
          }

          return p;
        }

      protected:
        /**
         * Constructor
         */
        GaussianRenderingFunction()
          : m_Mean(GAUSSIAN_MEAN),
            m_StandardDeviation(GAUSSIAN_STANDARD_DEVIATION)
        {}

        /**
         * Destructor
         */
        virtual ~GaussianRenderingFunction() {}


        /**
         * Gaussian stretch:
         * -------------------------------------------------------------------
         * Data                          => Screen value
         * -------------------------------------------------------------------
         * Mean - 3 x standard deviation => SCREEN_COLOR_MAX_VALUE      (0)
         * Mean                          => SCREEN_COLOR_MIDDLE_VALUE (127)
         * Mean + 3 x standard deviation => SCREEN_COLOR_MAX_VALUE    (255)
         * Intermediate data values      => set to screen values using a
         *                                  gaussian curve
         * -------------------------------------------------------------------
         * NB: the standard deviation is arbitrary chosen by the user
         */
        virtual void EvaluateGaussianDistribution()
        {
          m_BinLowerBounds.clear();

    // MDE : Correction
          unsigned int nbHist = this->m_PixelRepresentationFunction->GetOutputSize();

          for (unsigned int histIndex = 0; histIndex < nbHist; histIndex++)
          {
            std::vector<double> blb;

            double xMin = m_Mean - 3 * m_StandardDeviation;
            double xMax = m_Mean + 3 * m_StandardDeviation;

            for (unsigned int binIndex = 0; binIndex <= SCREEN_COLOR_MAX_VALUE; binIndex++)
            {
              double x = ( xMax - xMin ) * binIndex / ( SCREEN_COLOR_MAX_VALUE + 1 ) + xMin;
              double cdfValue = itk::Statistics::GaussianDistribution::CDF(x, m_Mean, m_StandardDeviation);

              double binMinValue = this->GetHistogramList()->GetNthElement(histIndex)->Quantile(0, cdfValue);

              blb.push_back(binMinValue);
            }

            m_BinLowerBounds.push_back(blb);
          }
        }

        /**
         * Perform the computation for a single value (this is done in order
         * to have the same code for vector and scalar version)
         */
        const OutputValueType ClampRescale(RealScalarType input, unsigned int channel) const
        {
          const std::vector<double> & blb = m_BinLowerBounds[channel];
          std::vector<double>::const_iterator it;
          double output;

          it = std::lower_bound(blb.begin(), blb.end(), static_cast<double>(input));

          if (it == blb.end())
          {
            output = SCREEN_COLOR_MAX_VALUE;
          }
          else if (it != blb.begin())
          {
            output = int(it - blb.begin()) - 1;
          }
          else
          {
            output = SCREEN_COLOR_MIN_VALUE;
          }

          return static_cast<OutputValueType>(output);
        }

        /**
         * Mean value of the gaussian distribution
         */
        double m_Mean;

        /**
         * Standard deviation value of the gaussian distribution, arbitrary
         * chosen by the user
         */
        double m_StandardDeviation;

        /**
         * Output lower bounds of input value correponding to the screen value
         * (index of the array)
         */
        std::vector< std::vector<double> > m_BinLowerBounds;


      private:

        GaussianRenderingFunction(const Self&); //purposely not implemented
        void operator=(const Self&); //purposely not implemented

    };

} // end namespace Functor

} // end namespace otb

#endif /* __otbGaussianRenderingFunction_h */
