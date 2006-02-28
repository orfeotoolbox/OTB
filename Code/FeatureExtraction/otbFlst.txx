/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   21 fevrier 2006
  Role      :   
  $Id$ 

=========================================================================*/
#ifndef __otbFlst_txx
#define __otbFlst_txx

#include "otbFlst.h"

namespace otb
{

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::INIT_MAX_AREA = 10; 

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::STEP_MAX_AREA = 8; 

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::EAST         =  1; 

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::NORTH_EAST   =  2; 

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::NORTH        =  4;

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::NORTH_WEST   =  8;

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::WEST         = 16; 
 
template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::SOUTH_WEST   = 32; 

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::SOUTH        = 64; 

template< class TInputImage, class TOutputTree>
const int FLST<TInputImage,TOutputTree>::SOUTH_EAST   =128; 

template< class TInputImage, class TOutputTree>
const char FLST<TInputImage,TOutputTree>::tabPatterns[2][256] =
{{  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1,
    1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1,
    0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1,
    0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0,
    1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,-1},
 {  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0,
    0, 0, 1, 0, 0,-1, 0,-1, 0, 0, 1, 0, 0,-1, 0,-1,
    0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0,
    0, 0, 1, 0, 0,-1, 0,-1, 0, 0, 1, 0, 0,-1, 0,-1,
    0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0,
    1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0,
    1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
    1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0,
    0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0,
    0, 0, 1, 0, 0,-1, 0,-1, 0, 0, 1, 0, 0,-1, 0,-1,
    0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0,
    0, 0, 1, 0, 0,-1, 0,-1, 0, 0, 1, 0, 0,-1, 0,-1}};

 
template< class TInputImage, class TOutputTree>
FLST<TInputImage,TOutputTree>::FLST()
{
  m_MinArea = 1;
  m_VisitedNeighbors = IntImageType::New();
}

template< class TInputImage, class TOutputTree>
void FLST<TInputImage,TOutputTree>::CalculateArea(int Width, int Height)
{
  int iAreaImage;
  
  m_AreaImage = Width * Height; 
  m_HalfAreaImage = (m_AreaImage+1) / 2;
  if(Width == 1) m_PerimeterImage = Height;
  else if(Height == 1) m_PerimeterImage = Width;
  else m_PerimeterImage = (Width+Height-2)*2;
  
  if(m_MinArea > m_AreaImage) std::cerr << "CalculateArea: min area > image"<< std::endl;


}

template< class TInputImage, class TOutputTree>
void FLST<TInputImage,TOutputTree>::GenerateData(void)
{
  int iWidth, iHeight;
  int iMinArea, iMaxArea, iAreaImage, iHalfAreaImage, iPerimeterImage;
  int iExploration;  /* Used to avoid reinitializing images */
//  static Point_plane tabPointsInShape;
  static int** tabtabVisitedNeighbors; /* Exterior boundary */
//  static Shapes pGlobalTree;
  static int iAtBorder; /* #points meeting at border of image */
  typename InputImageType::SizeType Taille;

  itkDebugMacro(<< "FLST::GenerateData() called");
  // Get the input and output pointers 
  const InputImageType  *InputImage   = this->GetImageInput();
  OutputTreeListType    *OutputTree   = this->GetOutput();
  
  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  iWidth  = Taille[0];
  iHeight = Taille[1];
  
  this->CalculateArea(iWidth, iHeight);
  
  itkDebugMacro(<< "FLST::GenerateData() finished");
}

/**
 * Standard "PrintSelf" method
 */
template< class TInputImage, class TOutputTree>
void FLST<TInputImage,TOutputTree>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "MinArea: " << m_MinArea << std::endl;
}


} // end namespace otb


#endif
 
