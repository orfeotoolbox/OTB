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

#ifndef __otbLineDirectionImageFilter_h
#define __otbLineDirectionImageFilter_h

#include "otbMath.h"
#include "otbLineDirectionFunctor.h"
#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "itkConstNeighborhoodIterator.h"


namespace otb
{
  /** \class LineDirectionImageFilter
   *  \brief This functor computes water, ndvi and spectral index of an image
   */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT LineDirectionImageFilter :
public UnaryFunctorNeighborhoodImageFilter< TInputImage,
				            TOutputImage, 
				            Functor::LineDirectionFunctor< ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>, 
						           		   ITK_TYPENAME TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef LineDirectionImageFilter                           Self;
  typedef TInputImage                                        InputImageType;
  typedef TOutputImage                                       OutputImageType;
  typedef typename InputImageType::PixelType                 InputPixelType;
  typedef typename InputImageType::InternalPixelType         InputInternalPixelType;
  typedef typename OutputImageType::PixelType                OutputPixelType;
  typedef typename otb::UnaryFunctorNeighborhoodImageFilter< TInputImage,
                                                             TOutputImage, 
               				                     Functor::LineDirectionFunctor< typename itk::ConstNeighborhoodIterator<TInputImage>, 
                                                                                            OutputPixelType > > Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Spatial Threshold accessor */
  void SetSpatialThreshold( unsigned int thresh )
    { 
      this->GetFunctor().SetSpatialThreshold( thresh ); 
      this->SetRadius(thresh);
      this->Modified();
    };
  unsigned int GetSpatialThreshold()
    {
      return this->GetFunctor().GetSpatialThreshold(); 
    };
  /** Spectral Threshold accessor */
  void SetSpectralThreshold( InputInternalPixelType thresh )
    { 
      this->GetFunctor().SetSpectralThreshold( thresh ); 
    };
  InputInternalPixelType GetSpectralThreshold()
    { 
      return this->GetFunctor().GSetSpectralThreshold(); 
    };
  /** RatioMaxConsiderationNumber accessor */
  void SetRatioMaxConsiderationNumber( unsigned int value )
    { 
      this->GetFunctor().SetRatioMaxConsiderationNumber(value); 
    };
  unsigned int GetRatioMaxConsiderationNumber()
    { 
      return this->GetFunctor().GetRatioMaxConsiderationNumber(); 
    }; 
 
  /** Alpha accessor */
  void SetAlpha( double alpha )
    { 
      this->GetFunctor().SetAlpha(alpha); 
    };
  double GetAlpha()
    { 
      return this->GetFunctor().GetAlpha(); 
    }; 
 
  /** Number Of Directions */
  void SetNumberOfDirections( unsigned int D )
    { 
      this->GetFunctor().SetNumberOfDirections( D );
      double step = 2*M_PI/static_cast<double>(D);
      this->GetFunctor().SetDirectionStep( step );
    };
  unsigned int GetNumberOfDirections()
    { 
      return this->GetFunctor().GetNumberOfDirections(); 
    }; 


 virtual void GenerateOutputInformation()
    {
      Superclass::GenerateOutputInformation();
      this->GetOutput()->SetNumberOfComponentsPerPixel(6);
    }

protected:
  LineDirectionImageFilter()
    {
      this->SetRadius(this->GetSpatialThreshold());
    };
  virtual ~LineDirectionImageFilter(){};

private:
  LineDirectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};

} // end namespace otb


#endif
