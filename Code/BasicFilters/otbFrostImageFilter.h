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
#ifndef __otbFrostImageFilter_h
#define __otbFrostImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class FrostImageFilter
 * \brief Applique un filtre de débruitage sur une image.
 *
 * Il utilise un noyau de convolution ayant une forme exponetielle décroissante.
 * 
 * La sortie du filtre pour le pixel p est de la forme:
 *      \f$ \hat I_{s}=\sum_{p\in\eta_{p}} m_{p}I_{p} \f$ 
 *
 * où :   \f$ m_{p}=\frac{KC_{s}^{2}\exp(-KC_{s}^{2}d_{s,p})}{\sum_{p\in\eta_{p}} KC_{s}^{2}\exp(-KC_{s}^{2}d_{s,p})} \f$
 *    et  \f$ d_{s,p}=\sqrt{(i-i_{p})^2+(j-j_{p})^2} \f$
 *
 * \f$ K \f$     : le facteur de décroissance du filtre
 * \f$ (i,j)\f$ : les coordonnées du point à considérer et compris dans la zone définie par \f$ \eta_{s} \f$ 
 * \f$ (i_{p},j_{p})\f$ :les coordonnées des point appartenant à la zone \f$ \eta_{p} \subset \eta_{s} \f$
 * \f$ C_{s}\f$ : le coefficient de varation calculé sur \f$ \eta_{p}\f$ 
 * 
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT FrostImageFilter :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:

 /** Some Image related typedefs. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef FrostImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Methode pour la gestion "object factory". */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FrostImageFilter, ImageToImageFilter);
  
  /** Définition des images supportées. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  /** "typedef" définissant un réel dans cette classe. */
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" définissant la taille d'une image. */
  typedef typename InputImageType::SizeType SizeType;

 /** ImageDimension constants */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);


  /** Positionne le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkSetMacro(Radius, SizeType);

  /** Récupère le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkGetConstReferenceMacro(Radius, SizeType);
  
  /** Positionne le nombre de vues utilisé pour le calcul du filtre. */
  itkSetMacro(Deramp, double);
  /** Récupère le nombre de vues (référencé constant) utilisé pour le calcul du filtre. */
  itkGetConstReferenceMacro(Deramp, double);

  /** FrostImageFilter a besoin d'une zone de traitement plus large en entrée qu'en sortie 
   * afin de permettre une utilisation du filtre par la méthode dite pipeline
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

protected:
  FrostImageFilter();
  virtual ~FrostImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** FrostImageFilter peut etre implémentée pour un traitement de filtre multithreaded.
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
  FrostImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Déclaration du rayon */
  SizeType m_Radius;
  /** Déclaration du facteur de décroissance du filtre */
  double m_Deramp;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFrostImageFilter.txx"
#endif

  
#endif
