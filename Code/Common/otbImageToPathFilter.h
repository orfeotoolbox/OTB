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
#ifndef __otbImageToPathFilter_h
#define __otbImageToPathFilter_h

#include "itkPathSource.h"

namespace otb
{
/**
 * \class ImageToPathFilter
 * \brief Classe de base base pour les filtres prenant une entr�e de type
 * \doxygen{itk}{Image} et renvoyant une sortie de type \doxygen{itk}{Path}.
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToPathFilter
  : public itk::PathSource<TOutputPath>
{
public:
  ///Typedefs standards
  typedef ImageToPathFilter             Self;
  typedef itk::PathSource<TOutputPath>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::ProcessObject            ProcessObjectType;
  /// Constructeur "objectFactory"
  itkNewMacro(Self);
  /// Informations � l'ex�cution
  itkTypeMacro(PathToImageFilter,PathSource);
  /// typedefs relatifs aux param�tres du template
  typedef          TInputImage             InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef          TOutputPath             OutputPathType;
  typedef typename OutputPathType::Pointer OutputPathPointerType;
  ///Dimension de l'image d'entr�e
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  ///Entr�e du pipeline
  virtual void SetInput(const InputImageType * Image);
  const InputImageType * GetInput(void);

protected:
  ImageToPathFilter();
  virtual ~ImageToPathFilter(){};
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageToPathFilter(const Self&);
  void operator=(const Self&);
};
}//Fin de l'espace de nom otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathFilter.txx"
#endif

#endif
