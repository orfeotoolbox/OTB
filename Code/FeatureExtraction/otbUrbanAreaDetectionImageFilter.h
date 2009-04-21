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
#ifndef __otbRadiometryNonWaterNonVegetationDetectionImageFilter_h
#define __otbRadiometryNonWaterNonVegetationDetectionImageFilter_h

#include "otbMath.h"
#include "otbImage.h"
#include "otbVegetationIndicesFunctor.h"
#include "otbWaterIndicesFunctor.h"
#include "otbMultiChannelRAndGAndNIRIndexImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageToImageFilter.h"

namespace otb
{

namespace Functor
{

/** TODO A Mettre dans BuiltUpIndices */

  /** \class RadiometryNonWaterNonVegetationIndexFunctor
   *  \brief This functor computes ndwi and ndvi of an image
   *  \brief in order to find urban areas and generate a grayscale image
   */
template<class TInput,class TOutput>
class RadiometryNonWaterNonVegetationIndexFunctor
{
 public:
  typedef Functor::NDVI< double, double, double>            VegetationFunctorType;
  typedef Functor::NDWI2< double, double, double>           WaterFunctorType;
  typedef TOutput                                           ValueType;

  VegetationFunctorType GetVegetationFunctor(){ return m_VegetationFunctor; };
  WaterFunctorType GetWaterFunctor(){ return m_WaterFunctor; };

  void SetRedIndex(int id){ m_VegetationFunctor.SetRedIndex(id); };
  void SetGreenIndex(int id){ m_WaterFunctor.SetGIndex(id); };
  void SetNIRIndex(int id){ m_VegetationFunctor.SetNIRIndex(id);
                            m_WaterFunctor.SetNIRIndex(id); };
  unsigned int GetGreenIndex(){ return m_WaterFunctor.GetGIndex(); };

  RadiometryNonWaterNonVegetationIndexFunctor(){}
  ~RadiometryNonWaterNonVegetationIndexFunctor(){};
  
  inline TOutput operator()(const TInput& pInPix)
    {
      TOutput lVIval,lWIval,lOutPix;
/*
      lVIval = static_cast<ValueType>(m_VegetationFunctor(pInPix));

      lWIval = static_cast<ValueType>(m_WaterFunctor(pInPix));

      lOutPix = 1. - ((1+lVIval)/2. * (1+lWIval)/2);
*/
      lVIval = static_cast<ValueType>(std::max(0.,m_VegetationFunctor(pInPix)));

      lWIval = static_cast<ValueType>(std::max(0.,m_WaterFunctor(pInPix)));

      //lOutPix = vcl_sqrt((1-lVIval)*(1-lWIval)); //sqrt : PAS MAL DU TOUT 
      //lOutPix = vcl_sqrt(vcl_abs(1-lVIval)*vcl_abs(1-lWIval)); //sqrt2 : pareil que sqrt 
      //lOutPix = vcl_sqrt((1-lVIval)*(1-lVIval)+(1-lWIval)*(1-lWIval)); // eucl : bof bof
      //lOutPix = 1-vcl_sqrt((lVIval)*(lVIval)+(lWIval)*(lWIval)); // eucl2 : pas mal, un peu effet de flou par rappart a sqrt
      //lOutPix = 1-vcl_sqrt(vcl_abs(lVIval)*vcl_abs(lWIval)); //sqrt2 : NUUUUUUL
      //lOutPix = 1. - (vcl_abs(lVIval)+vcl_abs(lWIval)/2.);//Div : PAS MAL
      //lOutPix = 1 - std::max(lVIval,lWIval); //MAX : PAS MAL DU TOUT

      lOutPix = 1 / ( lVIval*lWIval + 1 ); //Jordy's formula

      return lOutPix;
    }

 protected :
  VegetationFunctorType m_VegetationFunctor;
  WaterFunctorType      m_WaterFunctor;

};



  /** \class RadiometryNonWaterNonVegetationDetectionFunctor
   *  \brief This functor uses computes RadiometryNonWaterNonVegetationIndexFunctor
   *  \brief and appli a threshold to produce a binary image
   */
template<class TInput,class TOutput>
class RadiometryNonWaterNonVegetationDetectionFunctor
{
public:
  typedef RadiometryNonWaterNonVegetationIndexFunctor<TInput, TOutput> RadiometryNonWaterNonVegetationIndexFunctorType;

  RadiometryNonWaterNonVegetationDetectionFunctor()
  {
    m_LowerThreshold = 0.0;
    m_UpperThreshold = 1.0;
  };

  ~RadiometryNonWaterNonVegetationDetectionFunctor() {};
  inline TOutput operator()(const TInput& inPix)
  {
    if ( (m_RadiometryNonWaterNonVegetationIndexFunctor(inPix) > m_LowerThreshold) && 
         (m_RadiometryNonWaterNonVegetationIndexFunctor(inPix) <= m_UpperThreshold)  )
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  void SetLowerThreshold(double threshold){ m_LowerThreshold = threshold; };
  void SetUpperThreshold(double threshold){ m_UpperThreshold = threshold; };
  double GetLowerThreshold(){ return m_LowerThreshold; };
  double GetUpperThreshold(){ return m_UpperThreshold; };

  void SetRedIndex(int id){ m_RadiometryNonWaterNonVegetationIndexFunctor.SetRedIndex(id); };
  void SetGreenIndex(int id){ m_RadiometryNonWaterNonVegetationIndexFunctor.SetGreenIndex(id); };
  void SetNIRIndex(int id){ m_RadiometryNonWaterNonVegetationIndexFunctor.SetNIRIndex(id); };


protected:
  RadiometryNonWaterNonVegetationIndexFunctorType m_RadiometryNonWaterNonVegetationIndexFunctor;
  double m_LowerThreshold;
  double m_UpperThreshold;

};

} //end namespace functor



  /** \class UrbanAreaDetectionImageFilter
   *  \brief This filter uses associated functors to produce a binary or grayscale image
   *  \brief where the white areas represents the urban areas.
   */
template <class TInputImage, class TOutputImage,
          class TFunction = Functor::RadiometryNonWaterNonVegetationDetectionFunctor< 
                                                                 typename TInputImage::PixelType,
                                                                 typename TOutputImage::PixelType> >
class ITK_EXPORT UrbanAreaDetectionImageFilter :
        public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef UrbanAreaDetectionImageFilter                         Self;
  typedef itk::ImageToImageFilter< TInputImage,TOutputImage >   Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(  UrbanAreaDetectionImageFilter, itk::ImageToImageFilter);

  /** Images typedefs */
  typedef TInputImage                                           VectorImageType;
  typedef TOutputImage                                          OutputImageType;
  typedef double                                                SingleImagePixelType;
  typedef Image<SingleImagePixelType, 2>                        SingleImageType;
  typedef unsigned char                                         BinaryImagePixelType;
  typedef Image<BinaryImagePixelType, 2>                        BinaryImageType;
  typedef typename VectorImageType::PixelType                   VectorImagePixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;

  /** Filters typedefs */
  typedef Functor::RadiometryNonWaterNonVegetationIndexFunctor< VectorImagePixelType, SingleImagePixelType > FunctorType;
  //typedef Functor::RadiometryNonWaterNonVegetationDetectionFunctor< VectorImagePixelType, BinaryImageType > FunctorType;
  typedef MultiChannelRAndGAndNIRIndexImageFilter < VectorImageType,SingleImageType, FunctorType >
                                                                                UrbanAreaExtractionFilterType;
  typedef typename UrbanAreaExtractionFilterType::Pointer                       UrbanAreaExtrationFilterPointerType;



  /** Get/Set indices */
  void SetRedIndex(int id){ m_UrbanAreaExtractionFilter->SetRedIndex( id ); }; /*GetFunctor().*/
  void SetGreenIndex(int id){ m_UrbanAreaExtractionFilter->SetGreenIndex( id ); };
  void SetNIRIndex(int id){ m_UrbanAreaExtractionFilter->SetNIRIndex( id ); };
  unsigned int GetRedIndex(){ return m_UrbanAreaExtractionFilter->GetRedIndex(); };
  unsigned int GetGreenIndex(){ return m_UrbanAreaExtractionFilter->GetGreenIndex(); };
  unsigned int GetNIRIndex(){ return m_UrbanAreaExtractionFilter->GetNIRIndex(); };
  /** Get/Set threshold value */
  itkGetMacro(ThresholdValue, double);
  void SetThreshold(double pThreshold)
  {
    m_ThresholdValue = pThreshold;
  //  m_UrbanAreaExtractionFilter->GetFunctor().SetUpperThreshold( m_ThresholdValue );
  }

  /** Methods */
  virtual void GenerateData();

protected:
  UrbanAreaDetectionImageFilter();
  virtual ~UrbanAreaDetectionImageFilter(){};

private:
  UrbanAreaDetectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Filters */
  UrbanAreaExtrationFilterPointerType m_UrbanAreaExtractionFilter;

  /** MaxThreshold */
  double m_ThresholdValue;

}; // end class RadiometryNonWaterNonVegetationDetectionImageFilter

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUrbanAreaDetectionImageFilter.txx"
#endif

#endif

