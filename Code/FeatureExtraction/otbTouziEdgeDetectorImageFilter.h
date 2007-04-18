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
#ifndef __otbTouziEdgeDetectorImageFilter_h
#define __otbTouziEdgeDetectorImageFilter_h

#include "otbImageToModulusAndDirectionImageFilter.h"
#include "otbImage.h"
#include "itkNumericTraits.h"

#define MINI(_A,_B) ((_A) < (_B) ? (_A) : (_B))   
#define MAXI(_A,_B) ((_A) > (_B) ? (_A) : (_B))


namespace otb
{

/** \class TouziEdgeDetectorImageFilter
 * \brief Application of a filter of detection of contours.
 *
 * This class implements the Touzi's ratio edge detector used to detect 
 * contours. 
 * 
 * We define a square region of size 2n+1 that we devided in two regions.
 *
 * The response of the edge detector between two regions 1 and 2 in
 * one direction \f$ \theta_{i} \f$ is:
 *	\f[R(\theta_{i}) = 1 - \min (\frac{\mu_{1}}{\mu_{2}};\frac{\mu_{2}}{\mu_{1}}) \f]
 * where \f$ \mu_{1} \f$ and  \f$ \mu_{2} \f$ are the mean of regions 1 and 2.
 *
 * The intensity of contour is calculated in four directions 
 * vertical , diagonal 1, horizontal and diagonal 2.   
 * 
 * The output is an image of intensity of the detection of contour R, the
 * maximum response of the ratio edge detector of \f$ R(\theta_{i}) \f$ :
 *	\f[R = \max ( R(\theta_{i}) ) \f]
 *
 * An image of the direction of contours can also be determined by this filter:
 * 	\f[D = \frac{\sum_{i=1}^{4} s_{i}\theta_{i}R(\theta_{i})}{\sum_{i=1}^{4} R(\theta_{i}}  \f]
 * where if  \f[ \mu_{1}>\mu_{2} s_{i}=+1 \f]
 * else if \f[ \mu_{1}<\mu_{2} s_{i}=-1 \f]
 *  
 */

template <class TInputImage, class TOutputImage, class TOutputImageDirection = TOutputImage >
class  ITK_EXPORT TouziEdgeDetectorImageFilter :  public ImageToModulusAndDirectionImageFilter< TInputImage, TOutputImage, TOutputImageDirection >
{
public:
  /** 	Extrait les dimensions aussi bien des images 
  	d'entrée (Input) que de sortie (Output). */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef TouziEdgeDetectorImageFilter Self;
  typedef ImageToModulusAndDirectionImageFilter< TInputImage, TOutputImage, TOutputImageDirection> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(TouziEdgeDetectorImageFilter, ImageToModulusAndDirectionImageFilter);

  /** "typedef" pour simplifier la définition et la déclaration de variables. */
  typedef typename Superclass::InputImageType InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::OutputImageDirectionType OutputImageDirectionType;

  typedef typename InputImageType::SizeType SizeType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename OutputImageDirectionType::PixelType OutputPixelDirectionType;
  

  /** Set/Get radius methods */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);
 

  /** TouziEdgeDetectorImageFilter a besoin d'une zone de traitement plus large en entrée qu'en sortie 
   * afin de permettre une utilisation du filtre par la méthode dite pipeline
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);
  
protected:
  TouziEdgeDetectorImageFilter();
  virtual ~TouziEdgeDetectorImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void BeforeThreadedGenerateData();

  /** TouziEdgeDetectorImageFilter peut etre implémentée pour un traitement de filtre multithreaded.
   * Ainsi, cette implémentation fournit la méthode ThreadedGenerateData()
   * qui est appelée pour chaque thread du process. Les données image sont allouées automatiquement 
   * par la classe "mère" en appelant la méthode ThreadedGenerateData(). ThreadedGenerateData peut seulement 
   * écrire la portion de l'image spécifiée par le paramètre "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  TouziEdgeDetectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Déclaration du rayon */
  SizeType m_Radius;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTouziEdgeDetectorImageFilter.txx"
#endif

  
#endif
