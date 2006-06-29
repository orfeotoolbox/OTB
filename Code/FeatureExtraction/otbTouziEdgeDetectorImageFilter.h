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

#include "itkImageToImageFilter.h"
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
 * one direction \theta_{i} is:
 *	\[R(\theta_{i}) = 1 - \min (\fract{\mu_{1}}{\mu_{2}};\fract{\mu_{2}}{\mu_{1}}) \]
 * where \mu_{1} and  \mu_{2} are the mean of regions 1 and 2.
 *
 * The intensity of contour is calculated in four directions 
 * vertical , diagonal 1, horizontal and diagonal 2.   
 * 
 * The output is an image of intensity of the detection of contour R, the
 * maximum response of the ratio edge detector of R(\theta_{i}:
 *	\[R = \max ( R(\theta_{i}) ) \]
 *
 * An image of the direction of contours can also be determined by this filter:
 * 	\[D = \fract{\sum_{i=1}^{4} s_{i}\theta_{i}R(\theta_{i})}{\sum_{i=1}^{4} R(\theta_{i}}  \]
 * where  \[if \mu_{1}>\mu_{2} s_{i}=+1 \]
 * \[else if \mu_{1}<\mu_{2} s_{i}=-1 \]
 *  
 */

template <class TInputImage, class TOutputImage>
class  ITK_EXPORT TouziEdgeDetectorImageFilter :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
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

  /** "typedef" pour simplifier la définition et la déclaration de variables. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** "typedef" pour les classes standards. */
  typedef TouziEdgeDetectorImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Methode pour la gestion "object factory". */
  itkNewMacro(Self);

  /** Retourne le nom de la classe. */
  itkTypeMacro(TouziEdgeDetectorImageFilter, itk::ImageToImageFilter);
  
  /** Définition des images supportées. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" définissant la taille d'une image. */
  typedef typename InputImageType::SizeType SizeType;

  /** Positionne le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkSetMacro(Radius, SizeType);

  /** Récupère le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkGetConstReferenceMacro(Radius, SizeType);
 

  /** TouziEdgeDetectorImageFilter a besoin d'une zone de traitement plus large en entrée qu'en sortie 
   * afin de permettre une utilisation du filtre par la méthode dite pipeline
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

  const OutputImageType * GetOutputDirections();
  
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
  
  typename OutputImageType::Pointer m_DirectionOuputImage;
  

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTouziEdgeDetectorImageFilter.txx"
#endif

  
#endif
