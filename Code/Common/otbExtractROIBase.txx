#ifndef _otbExtractROIBase_txx
#define _otbExtractROIBase_txx

#include "otbExtractROIBase.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ExtractROIBase<TInputImage,TOutputImage>
::ExtractROIBase() :        itk::ExtractImageFilter<TInputImage,TOutputImage>(),
                                m_StartX(0),
                                m_StartY(0),
                                m_SizeX(0),
                                m_SizeY(0)
{
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
ExtractROIBase<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}


/** 
 * ExtractROIBase can produce an image which is a different resolution
 * than its input image.  As such, ExtractROIBase needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template <class TInputImage, class TOutputImage>
void 
ExtractROIBase<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
        
        // Determine la zone a extraire
        // Si SizeX(Y) est nulle, alors SizeX(Y) est egale à la SizeX(Y) de l'image
        typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();
        // Recupere Region de l'image d'entree
        const InputImageRegionType& inputRegion = inputPtr->GetRequestedRegion();
        if ( m_SizeX == 0 )
        {
                m_SizeX = inputRegion.GetSize()[1] - m_StartX;
        }
        if ( m_SizeY == 0 )
        {       
                m_SizeY = inputRegion.GetSize()[0] - m_StartY;
        }
  
        InputImageIndexType start;
        start[0] = m_StartX;
        start[1] = m_StartY;
        InputImageSizeType size;
        size[0] = m_SizeX;
        size[1] = m_SizeY;
        InputImageRegionType desiredRegion;
        desiredRegion.SetSize(  size  );
        desiredRegion.SetIndex( start );
        // Appel à la methode de base d'initialisation de la region
        this->SetExtractionRegion( desiredRegion );

        // Appel à la methode de la classe de base
        Superclass::GenerateOutputInformation();

}


} // end namespace otb

#endif
