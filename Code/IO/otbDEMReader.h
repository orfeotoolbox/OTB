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
#ifndef __otbDEMReader_h
#define __otbDEMReader_h

#include "itkIndent.h"
#include "itkImageSource.h"
#include "otbImage.h"
#include <iostream>
#include <stdio.h>
#include "elevation/ossimElevManager.h"
#include "base/ossimFilename.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkIndent.h"

namespace otb
{
/**OtbDEMReader header**/
/**Le but de cette classe est de pouvoir créer une image représentant
*  un MNT pour une région spécifiée (coin haut gauche + coin bas droit + spacing).
*  Cette classe s'appuie sur ossimElevManager. 
*  MNT supporté: DTED et SRTM. et pour les autres plus personnalisés???
*  
*  Il est à noter que si la résolution demandée est supérieure à celle des fichiers MNT utilisés, 
*  ossimElevManager fait une interpolation bilinéaire.
**/
template <class TDEMImage>
class ITK_EXPORT DEMReader: 
public itk::ImageSource<Image<typename TDEMImage::PixelType,2, 0> >
{
public :
//Déclaration des types utilisés:
  typedef itk::Indent														Indent;
  typedef TDEMImage					      									DEMImageType;
  typedef typename DEMImageType::Pointer									DEMImagePointerType;
  typedef typename DEMImageType::PixelType                 					PixelType;
  
  typedef DEMReader                                	                       	Self;
  typedef itk::ImageSource<Image<typename DEMImageType::PixelType,2, 0> >	Superclass;
  typedef itk::SmartPointer<Self>                    	      				Pointer;
  typedef itk::SmartPointer<const Self>              	      				ConstPointer;
  typedef Image<PixelType,2>           						   				OutputImageType; 

  typedef typename Superclass::Pointer    					    			OutputImagePointer;
  typedef typename OutputImageType::SpacingType 							SpacingType;
  typedef typename OutputImageType::SizeType 								SizeType;
  typedef typename OutputImageType::PointType 								PointType;
  typedef typename OutputImageType::IndexType 								IndexType;
  typedef typename Superclass::OutputImageRegionType 				        OutputImageRegionType;
  typedef itk::ImageRegionIteratorWithIndex< Image<PixelType,2, 0> >      	ImageIteratorType;
  
  itkNewMacro(Self);
  
  itkTypeMacro(DEMReader,ImageSource);
  	                                      
//Déclaration des méthodes:
itkSetMacro(Spacing,SpacingType);    //Pour fixer le spacing

itkGetConstReferenceMacro(Spacing,SpacingType);

itkSetMacro(Ul,PointType);           //Pour donner les coord géo du coin haut gauche

itkGetConstReferenceMacro(Ul,PointType);

itkSetMacro(Lr,PointType);           //Pour donner les coord géo du coin bas droit

itkGetConstReferenceMacro(Lr,PointType);

void SetSpacing(const double* spacing);       //Pour fixer le spacing

bool OpenDEMDirectory(char* &DEMDirectory);         //Pour lui donner le nom du repertoire contenant les fichiers MNT.

virtual double GetHeightAboveMSL(const PointType& worldPoint);      //Pour avoir l'élévation par rapport au niveau moyen de la mer.

protected:
DEMReader();
~DEMReader();

//Méthodes pour assurer la pipeline
void PrintSelf(std::ostream& os, Indent indent) const;
void GenerateData();
virtual void GenerateOutputInformation();

ossimElevManager* m_ElevManager;
//DEMImagePointerType  m_DEMImage;
SpacingType m_Spacing;
PointType m_Ul;
PointType m_Lr;
};
}//Fin header

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDEMReader.txx"
#endif

#endif
