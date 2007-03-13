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
#ifndef __otbImageToEdgePathFilter_h
#define __otbImageToEdgePathFilter_h

#include "otbImageToPathFilter.h"

namespace otb {
/**
 * \class ImageToEdgePathFilter
 * \brief Calcule au niveau pixel le contour d'une image binaire sous
 *la forme d'un \doxygen{itk}{Path}.
 *
 *L'image en entr�e est surmont�e d'une bordure noire en
 *utilisant le filtre \doxygen{itk}{ConstantPadImageFilter}, afin de
 *d�tecter correctment les contours d'une r�gion situ�e au bord de
 *l'image.
 *
 *Le filtrage se fait en deux �tapes : tout d'abord, une recherche
 *lin�aire est faite pour obtenir un pixel du contour qui servira
 *de point de d�part. A partir de ce pixel, le voisinage
 *(d�fini par un \doxygen{itk}{ConstShapedNeighborhoodIterator}) de
 *ce pixel est explor� dans le sens anti-trigonom�trique, en
 *partant du pixel suivant directement le dernier pixel ajout� au
 *contour. Une fois un pixel noir d�tect�, le prochain pixel
 *blanc dans cette recherche appartiendra n�cessairement � la
 *fronti�re, et sera ajout� au contour.
 *
 *Ce processus est it�r� jusqu'� ce que l'on revienne dans le
 *voisinage du point de d�part apr�s un nombre suffisant de
 *mouvements (ici 2). Dans le cas o� l'on ne trouve pas de
 *pixel permettant de terminer la cha�ne, l'algorithme termine.
 *
 *En cas d'objet lin�aire (contour ouvert) dans l'image binaire,
 *l'algorithme fera un aller-retour le long de l'objet jusqu'�
 *retomber sur son point de d�part. Il est alors possible que
 *certains pixels appartiennent deux fois au contour.
 *
 * \sa ImageToPathFilter
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToEdgePathFilter
  : public ImageToPathFilter<TInputImage, TOutputPath>
{
public:
  /** typedefs standards */
  typedef ImageToEdgePathFilter                 Self;
  typedef ImageToPathFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  /** Cr�ation */
  itkNewMacro(Self);
  itkTypeMacro(ImageToEdgePathFilter,ImageToPathFilter);

  /** Definition des types parametres */
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointerType InputImagePointerType;
  typedef typename Superclass::OutputPathType        OutputPathType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;
  
  typedef typename InputImageType::PixelType		 PixelType;

  /** Set and Get foreground value */
  itkSetMacro(ForegroundValue,PixelType);
  itkGetConstMacro(ForegroundValue,PixelType);
  
protected:
  ImageToEdgePathFilter();
  virtual ~ImageToEdgePathFilter(){};
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  virtual void GenerateOutputInformation() {}; // Ne fait rien
  virtual void GenerateData();

private:
  ImageToEdgePathFilter(const Self&); // purposely not implemented
  void operator=(const Self&); // purposely not implemented
  
  PixelType m_ForegroundValue; 
};

} //Fin de l'espace de nom otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToEdgePathFilter.txx"
#endif

#endif
