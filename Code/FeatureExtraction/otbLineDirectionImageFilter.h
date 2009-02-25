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

#include "otbLineDirectionFunctor.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"


namespace otb
{
  /** \class LineDirectionImageFilter
   *  \brief This functor computes water, ndvi and spectral index of an image
   */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT LineDirectionImageFilter :
public itk::ImageToImageFilter<TInputImage,TOutputImage>
  /*UnaryFunctorNeighborhoodImageFilter< TInputImage,
				            TOutputImage, 
				            Functor::LineDirectionFunctor< ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>, 
					    ITK_TYPENAME TOutputImage::InternalPixelType> >*/
{
public:
  /** Standard class typedefs. */
  typedef LineDirectionImageFilter                           Self;
  typedef TInputImage                                        InputImageType;
  typedef TOutputImage                                       OutputImageType;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage >  Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorNeighborhoodImageFilter,ImageToImageFilter);
  
  /** Some convenient typedefs. */
  typedef typename InputImageType::ConstPointer InputImagePointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;
  //typedef typename OutputImageType::InternalPixelType   OutputInternalImagePixelType;
  typedef itk::ConstNeighborhoodIterator<TInputImage>     NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType   RadiusType;
  ////////////////////////////////////////////
  // CHANGE THE OUTPUTINTERNAZL INTO OUTPUTPIXE/////////////
  ///////////////////////////////////////////
  typedef Functor::LineDirectionFunctor< NeighborhoodIteratorType,OutputImagePixelType  > FunctorType; 
  typedef typename FunctorType::OutputType FunctorOutputType;
  typedef itk::ProcessObject ProcessObjectType;

  /**Set/Get the radius of neighborhood.*/
  itkGetMacro(Radius,unsigned int);

  /** Functor accessors */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  }

  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  };
  void SetFunctor(const FunctorType& functor)
  {
    m_Functor = functor;
    this->Modified();
  }

  /** Spatial Threshold accessor */
  void SetSpatialThreshold( unsigned int thresh )
    { 
      this->GetFunctor().SetSpatialThreshold( thresh ); 
      m_Radius = thresh;
      this->Modified();
    };
  unsigned int GetSpatialThreshold()
    {
      return this->GetFunctor().GetSpatialThreshold(); 
    };
  /** Spectral Threshold accessor */
  void SetSpectralThreshold( InputImagePixelType thresh )
    { 
      this->GetFunctor().SetSpectralThreshold( thresh ); 
    };
  InputImagePixelType GetSpectralThreshold()
    { 
      return this->GetFunctor().GetSpectralThreshold(); 
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

  /** Texture selection accessors 
   *  1: length
   *  2: width
   *  3: PSI
   *  4: w-mean
   *  5: ratio
   *  6: SD
   *  Set to 1 means the texture will be computed.
   **/
  void SetTextureStatus( unsigned int id, bool isSelected )
    {
      if ( id>this->GetTexturesStatus().size() || id == 0 )
	{
	  itkExceptionMacro(<<"Invalid texture index "<<id<<", must be in [1;"<<this->GetTexturesStatus().size()<<"]");
	}
      else
	{
	  this->GetFunctor().SetTextureStatus( id-1, isSelected );
	}
    }
  std::vector<bool> GetTexturesStatus()
    {
      return this->GetFunctor().GetTexturesStatus();
    }

  /** Return output length image */
  const OutputImageType * GetLengthOutput() const;
  OutputImageType * GetLengthOutput();

   /** Return output width image */
  const OutputImageType * GetWidthOutput() const;
  OutputImageType * GetWidthOutput();

  /** Return output PSI image */
  const OutputImageType * GetPSIOutput() const;
  OutputImageType * GetPSIOutput();
  
  /** Return output WMean image */
  const OutputImageType * GetWMeanOutput() const;
  OutputImageType * GetWMeanOutput();
  
  /** Return output ratio image */
  const OutputImageType * GetRatioOutput() const;
  OutputImageType * GetRatioOutput();
  
  /** Return output SD image */
  const OutputImageType * GetSDOutput() const;
  OutputImageType * GetSDOutput();


 
  virtual void GenerateOutputInformation();
  std::vector<FunctorType> m_FunctorList;

protected:
  LineDirectionImageFilter();
  virtual ~LineDirectionImageFilter(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void BeforeThreadedGenerateData();
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );
  /** Pad the input requested region by radius */
  virtual void GenerateInputRequestedRegion(void);

private:
  LineDirectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_Radius;
  FunctorType m_Functor;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineDirectionImageFilter.txx"
#endif

#endif
