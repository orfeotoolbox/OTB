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
#ifndef _otbOpeningClosingMorphologicalFilter_txx
#define _otbOpeningClosingMorphologicalFilter_txx

#include "otbOpeningClosingMorphologicalFilter.h"

#include "itkGrayscaleMorphologicalOpeningImageFilter.h"
#include "itkGrayscaleMorphologicalClosingImageFilter.h"
#include "itkProgressAccumulator.h"

namespace otb {
/**
 * Constructor
 */
template <class TInputImage,class TOuputImage, class TKernel>
OpeningClosingMorphologicalFilter<TInputImage, TOuputImage, TKernel>
::OpeningClosingMorphologicalFilter()
  : m_Kernel()
{}



// /// Méthode standard pour la gestion du pipeline d'exécution
// template <class TInputImage,class TOuputImage, class TKernel>
// void
// OpeningClosingMorphologicalFilter<TInputImage, TOuputImage, TKernel>
// ::GenerateInputRequestedRegion()
// {
//   // Appel à l'implémentation de la classe supérieur
//   Superclass::GenerateInputRequestedRegion();
//   // Réccupération de l'image en entrée
//   InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
//   input->SetRequestedRegion( input->GetLargestPossibleRegion());
// }
// /// Méthode standard pour la gestion du pipeline d'exécution
// template <class TInputImage,class TOuputImage, class TKernel>
// void
// OpeningClosingMorphologicalFilter<TInputImage, TOuputImage, TKernel>
// ::EnlargeOutputRequestedRegion(itk::DataObject *)
// {
//   this->GetOutput()->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion());
// }




/**
 * Main computation method
 */
template <class TInputImage,class TOuputImage, class TKernel>
void
OpeningClosingMorphologicalFilter<TInputImage, TOuputImage, TKernel>
::GenerateData()
{
  // Filters Typedefs (this class is actually a composite filter) 
  typedef itk::GrayscaleMorphologicalOpeningImageFilter<InputImageType,OutputImageType, KernelType> OpenFilterType;
  typedef itk::GrayscaleMorphologicalClosingImageFilter<InputImageType,OutputImageType,KernelType> CloseFilterType;
  // Filters initialisation
  typename OpenFilterType::Pointer opening = OpenFilterType::New();
  typename CloseFilterType::Pointer closing = CloseFilterType::New();
  // Set the kernel
  opening->SetKernel(this->GetKernel());
  closing->SetKernel(this->GetKernel());
  // Progression report management
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(opening, .5f);
  progress->RegisterInternalFilter(closing, .5f);
  // Internal Pipeline connexion 
  closing->SetInput(this->GetInput()); 
  opening->SetInput(closing->GetOutput());
  // Output connexion
  opening->GraftOutput(this->GetOutput());
  opening->Update();
  this->GraftOutput(opening->GetOutput());
}
/**
 * PrintSelf method
 */
template <class TInputImage,class TOuputImage, class TKernel>
void
OpeningClosingMorphologicalFilter<TInputImage, TOuputImage, TKernel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Kernel: " << m_Kernel << std::endl;
}
}// End namespace otb
#endif
