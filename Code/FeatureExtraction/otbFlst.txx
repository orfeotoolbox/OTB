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
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::INIT_MAX_AREA = 10; 

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::STEP_MAX_AREA = 8; 

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::EAST         =  1; 

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::NORTH_EAST   =  2; 

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::NORTH        =  4;

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::NORTH_WEST   =  8;

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::WEST         = 16; 
 
template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::SOUTH_WEST   = 32; 

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::SOUTH        = 64; 

template< class TInputImage, class TOutputTree>
const int Flst<TInputImage,TOutputTree>::SOUTH_EAST   =128; 

template< class TInputImage, class TOutputTree>
const char Flst<TInputImage,TOutputTree>::tabPatterns[2][256] =
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



template<class TInputImage, class TOutputTree>
Flst<TInputImage,TOutputTree>
::Flst()
{
  m_MinArea = 1;                            // Valeur par défaut
  m_VisitedPixels = IntImageType::New();
  m_PixelOutput   = RealImageType::New();
}


template< class TInputImage, class TOutputTree>
char 
Flst<TInputImage,TOutputTree>::
is_local_min(RealImagePointerType ou, int x, int y, char b8Connected)
{
#if 0
  float v;
  char n = 0;

  v = ou[y][x];
  return (x==m_Width-1 || (ou[y][x+1]>v && ++n) || ou[y][x+1]==v) &&
    (x==0 || (ou[y][x-1]>v && ++n) || ou[y][x-1]==v) &&
    (y==m_Height-1 || (ou[y+1][x]>v && ++n) || ou[y+1][x]==v) &&
    (y==0 || (ou[y-1][x]>v && ++n) || ou[y-1][x]==v) &&
    (b8Connected == 0 ||
     ((x==m_Width-1 || y==0 || (ou[y-1][x+1]>v  && ++n) || ou[y-1][x+1]==v) &&
      (x==m_Width-1||y==m_Height-1||(ou[y+1][x+1]>v && ++n)|| ou[y+1][x+1]==v) &&
      (x==0 || y==m_Height-1 || (ou[y+1][x-1]>v && ++n) || ou[y+1][x-1]==v) &&
      (x==0 || y==0 || (ou[y-1][x-1]>v && ++n) || ou[y-1][x-1]==v)))	&&
    n != 0;
#endif
}

template< class TInputImage, class TOutputTree>
char 
Flst<TInputImage,TOutputTree>::
is_local_max(RealImagePointerType ou, int x, int y, char b8Connected)
{
#if 0
  float v;
  char n = 0;

  v = ou[y][x];
  return (x==m_Width-1 || (ou[y][x+1]<v && ++n) || ou[y][x+1]==v) &&
    (x==0 || (ou[y][x-1]<v && ++n) || ou[y][x-1]==v) &&
    (y==m_Height-1 || (ou[y+1][x]<v && ++n) || ou[y+1][x]==v) &&
    (y==0 || (ou[y-1][x]<v && ++n) || ou[y-1][x]==v) && 
    (b8Connected == 0 ||
     ((x==m_Width-1 || y==0 || (ou[y-1][x+1]<v  && ++n) || ou[y-1][x+1]==v) &&
      (x==m_Width-1||y==m_Height-1||(ou[y+1][x+1]<v && ++n)|| ou[y+1][x+1]==v) &&
      (x==0 || y==m_Height-1 || (ou[y+1][x-1]<v && ++n) || ou[y+1][x-1]==v) &&
      (x==0 || y==0 || (ou[y-1][x-1]<v && ++n) || ou[y-1][x-1]==v))) &&
    n != 0;
#endif
}

template< class TInputImage, class TOutputTree>
unsigned char 
Flst<TInputImage,TOutputTree>
::configuration(IntImagePointerType VisitedPixels, int x, int y)
{
#if 0
  short int iMaxX = m_Width-1, iMaxY = m_Height-1;
  unsigned char cPattern = 0;

  if(x != 0) {
    if(VisitedPixels[y][x-1] == m_Exploration)
      cPattern = WEST;
    if((y == 0 && m_AtBorder) ||
       (y > 0 && VisitedPixels[y-1][x-1] == m_Exploration))
      cPattern |= NORTH_WEST;
    if((y == iMaxY && m_AtBorder) ||
       (y < iMaxY && VisitedPixels[y+1][x-1] == m_Exploration))
      cPattern |= SOUTH_WEST;    
  } else if(m_AtBorder)
    cPattern = SOUTH_WEST | WEST | NORTH_WEST;
  
  if(x != iMaxX) {
    if(VisitedPixels[y][x+1] == m_Exploration)
      cPattern |= EAST;
    if((y == 0 && m_AtBorder) ||
       (y > 0 && VisitedPixels[y-1][x+1] == m_Exploration))
      cPattern |= NORTH_EAST;
    if((y == iMaxY && m_AtBorder) ||
       (y < iMaxY && VisitedPixels[y+1][x+1] == m_Exploration))
      cPattern |= SOUTH_EAST;
  } else if(m_AtBorder)
    cPattern |= SOUTH_EAST | EAST | NORTH_EAST;
  
  if((y == 0 && m_AtBorder) ||
     (y > 0 && VisitedPixels[y-1][x] == m_Exploration))
    cPattern |= NORTH;

  if((y == iMaxY && m_AtBorder) ||
     (y < iMaxY && VisitedPixels[y+1][x] == m_Exploration))
    cPattern |= SOUTH;

  return cPattern;
#endif
}

template< class TInputImage, class TOutputTree>
void  
Flst<TInputImage,TOutputTree>
::Store4Neighbors(RealImagePointerType ou, int x,int y,NeighborhoodType *pNeighborhood)
{
#if 0
   if(x > 0         && NEIGHBOR_NOT_STORED(x-1,y))
      pNeighborhood->Add( x-1, y, ou[y][x-1]);
   if(x < iWidth-1  && NEIGHBOR_NOT_STORED(x+1,y))
      pNeighborhood->Add( x+1, y, ou[y][x+1]);
   if(y > 0         && NEIGHBOR_NOT_STORED(x,y-1))
      pNeighborhood->Add( x, y-1, ou[y-1][x]);
   if(y < iHeight-1 && NEIGHBOR_NOT_STORED(x,y+1))
      pNeighborhood->Add( x, y+1, ou[y+1][x]);
#endif
}

template< class TInputImage, class TOutputTree>
void  
Flst<TInputImage,TOutputTree>
::Store8Neighbors(RealImagePointerType ou,int x,int y,NeighborhoodType *pNeighborhood)
{
#if 0
  if(x > 0) {
    if(y > 0         && NEIGHBOR_NOT_STORED(x-1,y-1))
      pNeighborhood->Add(x-1, y-1, ou[y-1][x-1]);
    if(y < iHeight-1 && NEIGHBOR_NOT_STORED(x-1,y+1))
      pNeighborhood->Add( x-1, y+1, ou[y+1][x-1]);
  }
  if(++x < iWidth) {
    if(y > 0         && NEIGHBOR_NOT_STORED(x,y-1))
      pNeighborhood->Add( x, y-1, ou[y-1][x]);
    if(y < iHeight-1 && NEIGHBOR_NOT_STORED(x,y+1))
      pNeighborhood->Add( x, y+1, ou[y+1][x]);
  }
#endif
}

template< class TInputImage, class TOutputTree>
void  
Flst<TInputImage,TOutputTree>
::AddIsoLevel(     PointPlaneType    tabPointsInShape, 
                   int              *pCurrentArea,
		   float            currentGrayLevel, 
		   NeighborhoodType *pNeighborhood, 
		   char             *p8Connected, 
		   char             *pIgnoreHoles)
{
#if 0
  int              x, y;
  NeighborhoodType pNeighbor;
  int              iCurrentArea;
  int              iNbHoles;
  unsigned char    cPattern;

  iNbHoles = 0;
  iCurrentArea = *pCurrentArea;
  pNeighbor = &pNeighborhood->tabPoints[1];
  do { /* 1) Neighbor is added to the region */
    x = pNeighbor->point.x;
    y = pNeighbor->point.y;
    tabPointsInShape[iCurrentArea].x = x;
    tabPointsInShape[iCurrentArea++].y = y;
    if(! *pIgnoreHoles) {
      cPattern = configuration(tabtabVisitedPixels, x, y);
      iNbHoles += tabPatterns[*p8Connected][cPattern];
    }
    if(x == 0 || x == iWidth-1 || y == 0 || y == iHeight-1)
      ++ iAtBorder;
    tabtabVisitedPixels[y][x] = iExploration;
    /* 2) Store new neighbors */
    store_4neighbors(ou, x, y, pNeighborhood);
    if(pNeighborhood->type == MAX) {
      if(! *p8Connected)
	*pIgnoreHoles = *p8Connected = (char)1;
      store_8neighbors(ou, x, y, pNeighborhood);
    }
    remove_neighbor(pNeighborhood);
  } while(iCurrentArea <= iMaxArea &&
	  pNeighbor->value == currentGrayLevel &&
	  pNeighborhood->type != INVALID);

  if(iCurrentArea <= iMaxArea &&
     iAtBorder != iPerimeterImage &&
     (! iAtBorder || iCurrentArea <= iHalfAreaImage) &&
     pNeighborhood->type != INVALID &&
     (*pIgnoreHoles || iNbHoles == 0)) {
    *pCurrentArea = iCurrentArea;
    return (char)1;
  }
  return 0;
#endif
}

template< class TInputImage, class TOutputTree>
void  
Flst<TInputImage,TOutputTree>
::FindTerminalBranch(RealImagePointerType ou,int x,int y,char b8Connected,
                          NeighborhoodType *pNeighborhood, 
			  ConnectionType *tabConnections)
{
#if 0
  float     level;
  int       iArea;
  int       iLastShapeArea;
  char      bUnknownHoles;
  ShapeType pSmallestShape;
  ShapeType pLastShape;

  restart_branch:

    ++ m_Exploration;
    iArea = iAtBorder = 0;
    bUnknownHoles = 0;
    pSmallestShape = pLastShape = NULL;
    level = ou[y][x];
    pNeighborhood->ReInit(b8Connected ? MAX: MIN);
    pNeighborhood->Add( x, y, level);
    while( this->AddIsoLevel(tabPointsInShape, &iArea,
		             level, pNeighborhood, 
			     ou, &b8Connected, &bUnknownHoles) != 0) {
    if(bUnknownHoles) {
      assert(iArea != 0);
      if(pLastShape != NULL) {
	iArea = pLastShape->area;
	connect(tabPointsInShape, iArea, tabConnections, pSmallestShape);
	new_connection(&tabPointsInShape[iArea], level, tabConnections);
      }
      levelize(ou, tabPointsInShape, iArea, level);
      goto restart_branch;
    }
    if(iMinArea <= iArea) { /* Store new shape? */
      iLastShapeArea = (pLastShape == NULL) ? 0 : pLastShape->area;
      pLastShape = new_shape(iArea, level, !b8Connected, pLastShape);
      if(pSmallestShape == NULL) pSmallestShape = pLastShape;
      update_smallest_shapes(tabPointsInShape+iLastShapeArea,
			     iArea-iLastShapeArea);
    }
    if(iAtBorder && iArea == iHalfAreaImage)
      break;
    bUnknownHoles = (char)(b8Connected && pNeighborhood->type == AMBIGUOUS);
    if(bUnknownHoles) b8Connected = 0;
    level = pNeighborhood->tabPoints[1].value;
  }
  if(pLastShape != NULL) {
    connect(tabPointsInShape, iArea, tabConnections, pSmallestShape);
    if(iAtBorder && iArea == iHalfAreaImage)
      insert_children(pGlobalTree->the_shapes, pLastShape);
    else if(iArea != 0)
      new_connection(&tabPointsInShape[iArea], level, tabConnections);
  }
  this->levelize(ou, tabPointsInShape, iArea, level);
#endif
}			  

template< class TInputImage, class TOutputTree>
void  
Flst<TInputImage,TOutputTree>
::Scan(RealImagePointerType ou,NeighborhoodType *pNeighborhood,
            ConnectionType *tabConnections)
{
#if 0
  short int i, j;
  char b8Connected = 0;
  int iExplorationInit;

  iExplorationInit = m_Exploration;
  for(i = 0; i < m_Height; i++)
    for(j = 0; j < m_Width; j++)
      if(m_VisitedPixels[i][j] <= iExplorationInit &&
	 (this->is_local_min(ou, j, i, (char)0) ||
	 (this->is_local_max(,j,i,(char)1) &&(b8Connected=1)==1)))
	{
	  this->FindTerminalBranch(ou, j, i, b8Connected, pNeighborhood, tabConnections);
	  b8Connected = 0;
	}
#endif
}	    


template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::GenerateData(void)
{
  typedef itk::ImageRegionIteratorWithIndex< RealImageType >        OutputIteratorType; 
  typedef itk::ImageRegionConstIteratorWithIndex< InputImageType >  InputIteratorType; 
  std::cout << "Generate data FLST" <<std::endl;
  const InputImageType  *InputImage   = this->GetInput();
//  OutputTreePointerType  OutputTree   = this->GetOutput();
//  OutputTreeType        *OutputTree   = this->GetOutput();
  itkDebugMacro(<< "FLST::GenerateData() called");

  typename InputImageType::RegionType region;
    
  InputImageSizeType Taille;
  int i;
  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  m_Width  = Taille[0];
  m_Height = Taille[1];

  m_AreaImage = m_Width * m_Height; 
  m_HalfAreaImage = (m_AreaImage+1) / 2;
  if(m_Width == 1) m_PerimeterImage = m_Height;
  else if(m_Height == 1) m_PerimeterImage = m_Width;
  else m_PerimeterImage = (m_Width+m_Height-2)*2;

  if(m_MinArea > m_AreaImage) std::cerr << "CalculateArea: min area > image"<< std::endl;
  
  m_GlobalTree->interpolation = 0 ;
  m_Connections->resize(m_AreaImage);
  
  for(i = m_AreaImage-1; i >= 0; i--)
    (*m_Connections)[i].shape = NULL;
  
  // Initialise la variable de travail m_PixelOutput
      
  region.SetSize(InputImage->GetLargestPossibleRegion().GetSize());
  region.SetIndex(InputImage->GetLargestPossibleRegion().GetIndex());
  m_PixelOutput->SetRegions( region );
  m_PixelOutput->SetOrigin(InputImage->GetOrigin());
  m_PixelOutput->SetSpacing(InputImage->GetSpacing());
  m_PixelOutput->Allocate();
  
  OutputIteratorType    outputIt( m_PixelOutput, m_PixelOutput->GetRequestedRegion() );
  InputIteratorType     inputIt(  InputImage,    m_PixelOutput->GetRequestedRegion() );

  outputIt.GoToBegin();
  inputIt.GoToBegin();

  for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt,++inputIt)
    {
     outputIt.Set( inputIt.Get() );
    }

  // Initialise l'attribut : m_VisitedPixels:
  
  m_VisitedPixels->SetRegions( region );
  m_VisitedPixels->SetOrigin(InputImage->GetOrigin());
  m_VisitedPixels->SetSpacing(InputImage->GetSpacing());
  m_VisitedPixels->Allocate();
  
  // Initialisation de l'attribut : m_Neighborhood
   
//  m_Neighborhood->Init(m_AreaImage);

  // Initialisation de l'attribut : m_PointsInShape
  m_PointsInShape.resize(m_AreaImage); 
  
  // Initialisation de l'attribut : m_PointsInShape
  m_Exploration = 0;
  m_MaxArea     = 0;

  do {
    if(m_MaxArea == 0)
      m_MaxArea = INIT_MAX_AREA;
    else
      m_MaxArea <<= STEP_MAX_AREA;
    if(m_MaxArea == 0 || m_MaxArea >= m_HalfAreaImage) /* iMaxArea==0: overflow */
       m_MaxArea = m_AreaImage-1;
//    scan(tabtabPixelsOutput, tabtabVisitedPixels,&neighborhood,tabConnections);
  } while(m_MaxArea+1 < m_AreaImage);

  /* Make connections with root */
//  pTree->the_shapes[0].value = tabtabPixelsOutput[0][0];
  for(i = m_AreaImage-1; i >= 0; i--)
      {
//    if(tabConnections[i].shape != NULL) {
//      assert(tabConnections[i].level == pTree->the_shapes[0].value);
//      insert_children(pTree->the_shapes, tabConnections[i].shape);
//    }
     }  
  

}


/**
 * Standard "PrintSelf" method
 */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "MinArea: " << m_MinArea << std::endl;
}


} // end namespace otb



 
#endif
