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
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{



/****************************************************************************************************/
/*******     FLST                                                                                   */
/****************************************************************************************************/

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


/* ------------------------------------------------------------------------
   --------- 
   ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------
   --------- Allocations of structures used in the algorithm --------------
   ------------------------------------------------------------------------ */

/* Allocate image of the tags for visited pixels and the visited neighbors.
Do not be afraid about the parameters: pointers to 2-D arrays. */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::init_image_of_visited_pixels(int*** ptabtabVisitedPixels)
{
  int i;
  *ptabtabVisitedPixels   = new int*[iHeight];
  tabtabVisitedNeighbors  = new int*[iHeight];
  for (i = 0 ; i < iHeight ; i++)
    {
    	(*ptabtabVisitedPixels)[i]   = new int[iWidth];
	tabtabVisitedNeighbors[i]    = new int[iWidth];
    }   
}

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::print_visited_pixels(int** ptabtabVisitedPixels)
{
  for (int i = 0 ; i < iHeight ; i++)
  for (int j = 0 ; j < iWidth ; j++)
    {
      std::cout << " VisitedPixel["   <<i << "][" << j << "]=" << ptabtabVisitedPixels[i][j];
      std::cout << " VisitedNeighbor["<<i << "][" << j << "]=" << tabtabVisitedNeighbors[i][j];
      std::cout << std::endl;
    }
}

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::free_image_of_visited_pixels(int** tabtabVisitedPixels)
{
  int i;
  for (i = 0 ; i < iHeight ; i++)
    {
    	delete[] tabtabVisitedPixels[i];
	delete[] tabtabVisitedNeighbors[i];
    }  
  delete[] tabtabVisitedPixels;
  delete[] tabtabVisitedNeighbors;

}

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::init_region(int iMaxArea)
{
  tabPointsInShape = new Point_plane[iMaxArea];
  if(tabPointsInShape == NULL)
    std::cerr << "init_region --> impossible to allocate the array" << std::endl;
}

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::print_PointsInShape()
{
  for(int i = 0 ; i <iMaxArea ;i++)
    {
     std::cout << "tabPointsInShape["<<i<<"]= ("<< tabPointsInShape[i].x ;
     std::cout << " , " << tabPointsInShape[i].y<<" )" << std::endl;
    } 
}

/* Initialize the output image */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::init_output_image(const InputImageType* image,float***  ptabtabPixelsOutput)
{
  int                  i;
  int                  j;
  RealImageIndexType   Index;


  *ptabtabPixelsOutput   = new float*[iHeight];
  for (i = 0 ; i < iHeight ; i++)
	(*ptabtabPixelsOutput)[i] = new float[iWidth];

}

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::free_output_image(float** tabtabPixelsOutput)
{
  int i;
  for (i = 0 ; i < iHeight ; i++)
    	delete[] tabtabPixelsOutput[i];

  delete[] tabtabPixelsOutput;
}

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::free_region()
{
  delete[] tabPointsInShape;
}


/* Add the pixel (x,y), of gray-level VALUE, to the neighbor pixels */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::add_neighbor(Neighborhood* pNeighborhood,short int x,
                           short int y,float value)
{
  Neighbor* pNewNeighbor;

  std::cout << " add_neighbor() " << std::endl;
  std::cout << "   NeighborPosition (" <<x<<" , "<<y<<" )= "<<value << std::endl;
  std::cout << "   Before Adding: " << std::endl;
  
  pNeighborhood->print_neighborhood();
  /* 0) Tag the pixel as 'visited neighbor' */
  tabtabVisitedNeighbors[y][x] = iExploration; 
  /* 1) Update the fields TYPE and OTHERBOUND of PNEIGHBORHOOD */
  if(pNeighborhood->iNbPoints == 0)
    pNeighborhood->otherBound = value;
  else
    switch(pNeighborhood->type)
      {
      case Neighborhood::MIN:
	if(value > pNeighborhood->otherBound)
	  pNeighborhood->otherBound = value;
	else if(value < pNeighborhood->tabPoints[1].value)
	  pNeighborhood->type = Neighborhood::INVALID;
	break;
      case Neighborhood::MAX:
	if(value < pNeighborhood->otherBound)
	  pNeighborhood->otherBound = value;
	else if(value > pNeighborhood->tabPoints[1].value)
	  pNeighborhood->type = Neighborhood::INVALID;
	break;
      case Neighborhood::AMBIGUOUS:
	if(value != pNeighborhood->tabPoints[1].value) {
	  pNeighborhood->type =
	    (value < pNeighborhood->tabPoints[1].value)? Neighborhood::MAX: Neighborhood::MIN;
	  pNeighborhood->otherBound = value;
	}
	break;
      }
  if(pNeighborhood->type == Neighborhood::INVALID)
    return;
  /* 2) Add the point in the heap and update it */
  pNewNeighbor = &pNeighborhood->tabPoints[++pNeighborhood->iNbPoints];
  pNewNeighbor->point.x = x; /* Initialise the new neighbor point */
  pNewNeighbor->point.y = y;
  pNewNeighbor->value = value;
  pNeighborhood->fix_up(); /* Update the heap of neighbors */

  std::cout << "   After Adding: " << std::endl;  
  pNeighborhood->print_neighborhood();
  std::cout << " add_neighbor() END" << std::endl;

}

/* Remove the neighbor at the top of the heap, that is the root of the tree. */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::remove_neighbor(Neighborhood* pNeighborhood)
{
  Neighbor* pTop = &pNeighborhood->tabPoints[1];
  float value = pTop->value;

  std::cout << " remove_neighbor() " << std::endl;
  std::cout << "   Before Removing : " << std::endl;  
  pNeighborhood->print_neighborhood();

  if(pNeighborhood->type == Neighborhood::INVALID)
    return;
  *pTop = pNeighborhood->tabPoints[pNeighborhood->iNbPoints--];
  if(pNeighborhood->iNbPoints == 0)
    return;
  pNeighborhood->fix_down();
  if(value != pTop->value && pTop->value == pNeighborhood->otherBound)
    pNeighborhood->type = Neighborhood::AMBIGUOUS;

  std::cout << "   After Removing : " << std::endl;  
  pNeighborhood->print_neighborhood();
  std::cout << " add_neighbor() END" << std::endl;

}

template< class TInputImage, class TOutputTree>
char 
Flst<TInputImage,TOutputTree>
::is_local_min(float** ou,short int x,short int y,char b8Connected)
{
  float v;
  char n = 0;

  v = ou[y][x];
  return (x==iWidth-1 || (ou[y][x+1]>v && ++n) || ou[y][x+1]==v) &&
    (x==0 || (ou[y][x-1]>v && ++n) || ou[y][x-1]==v) &&
    (y==iHeight-1 || (ou[y+1][x]>v && ++n) || ou[y+1][x]==v) &&
    (y==0 || (ou[y-1][x]>v && ++n) || ou[y-1][x]==v) &&
    (b8Connected == 0 ||
     ((x==iWidth-1 || y==0 || (ou[y-1][x+1]>v  && ++n) || ou[y-1][x+1]==v) &&
      (x==iWidth-1||y==iHeight-1||(ou[y+1][x+1]>v && ++n)|| ou[y+1][x+1]==v) &&
      (x==0 || y==iHeight-1 || (ou[y+1][x-1]>v && ++n) || ou[y+1][x-1]==v) &&
      (x==0 || y==0 || (ou[y-1][x-1]>v && ++n) || ou[y-1][x-1]==v)))	&&
    n != 0;

}

/* Is pixel (x,y) a local maximum? */
template< class TInputImage, class TOutputTree>
char 
Flst<TInputImage,TOutputTree>
::is_local_max(float** ou,short int x,short int y,char b8Connected)
{
  float v;
  char n = 0;

  v = ou[y][x];
  return (x==iWidth-1 || (ou[y][x+1]<v && ++n) || ou[y][x+1]==v) &&
    (x==0 || (ou[y][x-1]<v && ++n) || ou[y][x-1]==v) &&
    (y==iHeight-1 || (ou[y+1][x]<v && ++n) || ou[y+1][x]==v) &&
    (y==0 || (ou[y-1][x]<v && ++n) || ou[y-1][x]==v) && 
    (b8Connected == 0 ||
     ((x==iWidth-1 || y==0 || (ou[y-1][x+1]<v  && ++n) || ou[y-1][x+1]==v) &&
      (x==iWidth-1||y==iHeight-1||(ou[y+1][x+1]<v && ++n)|| ou[y+1][x+1]==v) &&
      (x==0 || y==iHeight-1 || (ou[y+1][x-1]<v && ++n) || ou[y+1][x-1]==v) &&
      (x==0 || y==0 || (ou[y-1][x-1]<v && ++n) || ou[y-1][x-1]==v))) &&
    n != 0;
}

/* Set pixels and saddle points in `tabPoints' at level newGray */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::levelize(float** tabtabPixelsOutput,Point_plane* tabPoints,
           int iNbPoints,float newGray)
{
  std::cout << "Levelize()" << std::endl;

  std::cout << " iNbPoints : " << iNbPoints;
  std::cout << " NewGray   : " << newGray;
  std::cout << std::endl;

  PrintPixelOutput(tabtabPixelsOutput);
  print_PointsInShape();
  
  for(int i = iNbPoints - 1; i >= 0; i--)
    tabtabPixelsOutput[tabPoints[i].y][tabPoints[i].x] = newGray;

  PrintPixelOutput(tabtabPixelsOutput);
  
  std::cout << "Levelize() END" << std::endl;

}

/* Return, coded in one byte, the local configuration around the pixel (x,y) */
template< class TInputImage, class TOutputTree>
unsigned char 
Flst<TInputImage,TOutputTree>
::configuration(int** tabtabVisitedPixels,short int x,short int y)
{
  short int iMaxX = iWidth-1, iMaxY = iHeight-1;
  unsigned char cPattern = 0;

  if(x != 0) {
    if(tabtabVisitedPixels[y][x-1] == iExploration)
      cPattern = WEST;
    if((y == 0 && iAtBorder) ||
       (y > 0 && tabtabVisitedPixels[y-1][x-1] == iExploration))
      cPattern |= NORTH_WEST;
    if((y == iMaxY && iAtBorder) ||
       (y < iMaxY && tabtabVisitedPixels[y+1][x-1] == iExploration))
      cPattern |= SOUTH_WEST;    
  } else if(iAtBorder)
    cPattern = SOUTH_WEST | WEST | NORTH_WEST;
  
  if(x != iMaxX) {
    if(tabtabVisitedPixels[y][x+1] == iExploration)
      cPattern |= EAST;
    if((y == 0 && iAtBorder) ||
       (y > 0 && tabtabVisitedPixels[y-1][x+1] == iExploration))
      cPattern |= NORTH_EAST;
    if((y == iMaxY && iAtBorder) ||
       (y < iMaxY && tabtabVisitedPixels[y+1][x+1] == iExploration))
      cPattern |= SOUTH_EAST;
  } else if(iAtBorder)
    cPattern |= SOUTH_EAST | EAST | NORTH_EAST;
  
  if((y == 0 && iAtBorder) ||
     (y > 0 && tabtabVisitedPixels[y-1][x] == iExploration))
    cPattern |= NORTH;

  if((y == iMaxY && iAtBorder) ||
     (y < iMaxY && tabtabVisitedPixels[y+1][x] == iExploration))
    cPattern |= SOUTH;

  return cPattern;
}

template< class TInputImage, class TOutputTree>
Shape* 
Flst<TInputImage,TOutputTree>
::new_shape(int iCurrentArea,float currentGrayLevel, 
            char bOfInferiorType,Shape* pChild)
{
  std::cout << "Flst::new_shape() "   << std::endl;
  std::cout << " iCurrentArea : "     << iCurrentArea;
  std::cout << " currentGrayLevel : " << currentGrayLevel;
  std::cout << " pChild : "           << pChild ;
  std::cout << std::endl;
  std::cout << " NbShapes: " << pGlobalTree->nb_shapes << std::endl;

  PrintTree(pGlobalTree);
  
  Shape* pNewShape = &pGlobalTree->the_shapes[pGlobalTree->nb_shapes++];
 
  pNewShape->inferior_type = bOfInferiorType;
  pNewShape->value         = currentGrayLevel;
  pNewShape->open          = (char)(iAtBorder != 0);
  pNewShape->area          = iCurrentArea;
  pNewShape->removed       = 0;
  pNewShape->pixels        = NULL;
  /* Make links */
  pNewShape->parent       = NULL;
  pNewShape->next_sibling = NULL;
  pNewShape->child        = pChild;
  if(pChild != NULL)
    pChild->parent = pNewShape;
  return pNewShape;
}

/* Knowing that the last extracted shape contains the points, update,
for each one, the smallest shape containing it */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::update_smallest_shapes(Point_plane* tabPoints,int iNbPoints)
{
  int i, iIndex;
  Shape* pNewShape;
  Shape* pRoot = &pGlobalTree->the_shapes[0];

  pNewShape = &pGlobalTree->the_shapes[pGlobalTree->nb_shapes-1];
  for(i = iNbPoints - 1; i >= 0; i--)
    {
      iIndex = tabPoints[i].y * iWidth + tabPoints[i].x;
      if(pGlobalTree->smallest_shape[iIndex] == pRoot)
	pGlobalTree->smallest_shape[iIndex] = pNewShape;
    }
}

/* Find children of the last constructed monotone section, which is composed
of the interval between pSmallestShape and the last extracted shape. That is,
find shapes in other monotone sections whose parent is inside this interval */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::connect(Point_plane* tabPoints,int iNbPoints,
          Connection* tabConnections,Shape* pSmallestShape)
{
  int i, iIndex;
  Shape* pShape;
  Shape* pParent;
  float level;


  for(i = iNbPoints-1; i >= 0; i--) {
    iIndex = tabPoints[i].y * iWidth + tabPoints[i].x;
    pShape = tabConnections[iIndex].shape;
    if(pShape != NULL) {
      level = tabConnections[iIndex].level;
      pParent = pSmallestShape;
      while(pParent->value != level) {
	assert(pParent != &pGlobalTree->the_shapes[pGlobalTree->nb_shapes-1]);
	pParent = pParent->parent;
      }
      insert_children(pParent, pShape);
      tabConnections[iIndex].shape = NULL;
    }
  }
}

/* Make a new connection structure at the given point */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::new_connection(Point_plane* pPoint,float level,Connection* tabConnections)
{
  int iIndex;
  Shape* pSibling;
  Shape* pShape = &pGlobalTree->the_shapes[pGlobalTree->nb_shapes-1];
  
  std::cout << "New Connection()" << std::endl;
  
  iIndex = pPoint->y*iWidth + pPoint->x;
  if(tabConnections[iIndex].shape == NULL) {
    tabConnections[iIndex].shape = pShape;
    tabConnections[iIndex].level = level;
  } else {
    assert(tabConnections[iIndex].level == level);
    pSibling = tabConnections[iIndex].shape;
    while(pSibling->next_sibling != NULL)
      pSibling = pSibling->next_sibling;
    pSibling->next_sibling = pShape;
  }
}

/* Store the 4-neighbors of pixel (x,y) */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::store_4neighbors(float** ou,short int x,short int y,Neighborhood* pNeighborhood)
{
  if(x > 0         && NEIGHBOR_NOT_STORED(x-1,y))
    add_neighbor(pNeighborhood, x-1, y, ou[y][x-1]);
  if(x < iWidth-1  && NEIGHBOR_NOT_STORED(x+1,y))
    add_neighbor(pNeighborhood, x+1, y, ou[y][x+1]);
  if(y > 0         && NEIGHBOR_NOT_STORED(x,y-1))
    add_neighbor(pNeighborhood, x, y-1, ou[y-1][x]);
  if(y < iHeight-1 && NEIGHBOR_NOT_STORED(x,y+1))
    add_neighbor(pNeighborhood, x, y+1, ou[y+1][x]);
}

/* Store the diagonal neighbors of pixel (x,y) */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::store_8neighbors(float** ou,short int x,short int y,Neighborhood* pNeighborhood)
{
  if(x > 0) {
    if(y > 0         && NEIGHBOR_NOT_STORED(x-1,y-1))
      add_neighbor(pNeighborhood, x-1, y-1, ou[y-1][x-1]);
    if(y < iHeight-1 && NEIGHBOR_NOT_STORED(x-1,y+1))
      add_neighbor(pNeighborhood, x-1, y+1, ou[y+1][x-1]);
  }
  if(++x < iWidth) {
    if(y > 0         && NEIGHBOR_NOT_STORED(x,y-1))
      add_neighbor(pNeighborhood, x, y-1, ou[y-1][x]);
    if(y < iHeight-1 && NEIGHBOR_NOT_STORED(x,y+1))
      add_neighbor(pNeighborhood, x, y+1, ou[y+1][x]);
  }
}


/* Add the points in the neighborhood of gray level currentGrayLevel to the
region tabPointsInShape and return 1 if a new shape is (maybe) detected.
This "maybe" is linked to `pIgnoreHoles', indicating if we can count the
holes. New points are added to `tabPointsInShape' from position `pCurrentArea'.
This value is changed at exit in case of success. */
template< class TInputImage, class TOutputTree>
char 
Flst<TInputImage,TOutputTree>
::add_iso_level(Point_plane* tabPointsInShape,int* pCurrentArea,
		float currentGrayLevel, Neighborhood* pNeighborhood, 
		float** ou,int** tabtabVisitedPixels, 
		char* p8Connected, char* pIgnoreHoles)
{
  short int x, y;
  Neighbor* pNeighbor;
  int iCurrentArea, iNbHoles;
  unsigned char cPattern;

  std::cout << "AddIsoLevel()" << std::endl;
  std::cout << " pCurrentArea :" << *pCurrentArea;
  std::cout << " currentGrayLevel :" << currentGrayLevel;
  std::cout << std::endl;
 
  pNeighborhood->print_neighborhood();
    
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
    if(pNeighborhood->type == Neighborhood::MAX) {
      if(! *p8Connected)
	*pIgnoreHoles = *p8Connected = (char)1;
      store_8neighbors(ou, x, y, pNeighborhood);
    }
    remove_neighbor(pNeighborhood);
  } while(iCurrentArea <= iMaxArea &&
	  pNeighbor->value == currentGrayLevel &&
	  pNeighborhood->type != Neighborhood::INVALID);

  if(iCurrentArea <= iMaxArea &&
     iAtBorder != iPerimeterImage &&
     (! iAtBorder || iCurrentArea <= iHalfAreaImage) &&
     pNeighborhood->type != Neighborhood::INVALID &&
     (*pIgnoreHoles || iNbHoles == 0)) {
    *pCurrentArea = iCurrentArea;
    return (char)1;
  }

  return 0;
}


/* Extract the terminal branch containing the point (x,y) */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::find_terminal_branch(float** ou,int** tabtabVisitedPixels,
                       short int x,short int y,
		       char b8Connected, Neighborhood* pNeighborhood, 
		       Connection* tabConnections)
{
  float level;
  int iArea, iLastShapeArea;
  char bUnknownHoles;
  Shape* pSmallestShape;
  Shape* pLastShape;

  std::cout << "find terminal branch() ";
  std::cout << " Location : ("<<x<<" , "<< y <<" )";
  std::cout << std::endl;

 restart_branch:
  ++ iExploration;
  iArea = iAtBorder = 0;
  bUnknownHoles = 0;
  pSmallestShape = pLastShape = NULL;
  level = ou[y][x];
  std::cout << "before reinit_neighbor()"<<std::endl;
  pNeighborhood->print_neighborhood(); 

  pNeighborhood->reinit_neighborhood(b8Connected ? Neighborhood::MAX : Neighborhood::MIN);

  std::cout << "after reinit_neighbor()"<<std::endl;
  pNeighborhood->print_neighborhood(); 

  add_neighbor(pNeighborhood, x, y, level);

  std::cout << "after add_neighbor()"<<std::endl;
  pNeighborhood->print_neighborhood(); 
  
  while(add_iso_level(tabPointsInShape, &iArea,
		      level, pNeighborhood, ou, tabtabVisitedPixels,
		      &b8Connected, &bUnknownHoles) != 0) 
    {
    std::cout << "AddIsoLevel --- while " << std::endl;

    if(bUnknownHoles) {
      assert(iArea != 0);
      if(pLastShape != NULL) {
	iArea = pLastShape->area;
	std::cout << "Connect--------------->>>"<<iArea << std::endl;
	
	connect(tabPointsInShape, iArea, tabConnections, pSmallestShape);
	new_connection(&tabPointsInShape[iArea], level, tabConnections);
      }
      levelize(ou, tabPointsInShape, iArea, level);
      goto restart_branch;
    }
    std::cout << "End iso-level" <<std::endl;
    if(iMinArea <= iArea) 
      { /* Store new shape? */
      std::cout << "Store New shape " << std::endl;
      iLastShapeArea = (pLastShape == NULL) ? 0 : pLastShape->area;
      pLastShape = new_shape(iArea, level, !b8Connected, pLastShape);
      if(pSmallestShape == NULL) pSmallestShape = pLastShape;
      update_smallest_shapes(tabPointsInShape+iLastShapeArea,
			     iArea-iLastShapeArea);
    }
    if(iAtBorder && iArea == iHalfAreaImage)
      break;
    bUnknownHoles = (char)(b8Connected && pNeighborhood->type == Neighborhood::AMBIGUOUS);
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
  levelize(ou, tabPointsInShape, iArea, level);

  std::cout << "find terminal branch() END "<< std::endl;

}

/* Scan the image, calling a procedure to extract terminal branch at each
   (not yet visited) local extremum */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::scan(float** tabtabPixelsOutput,int** tabtabVisitedPixels,
       Neighborhood* pNeighborhood,Connection* tabConnections)
{
  short int i, j;
  char b8Connected = 0;
  int iExplorationInit;

  std::cout << "scan" <<std::endl;

  iExplorationInit = iExploration;
  for(i = 0; i < iHeight; i++)
    for(j = 0; j < iWidth; j++)
      {
 
      
      if(tabtabVisitedPixels[i][j] <= iExplorationInit &&
	 (is_local_min(tabtabPixelsOutput, j, i, (char)0) ||
	  (is_local_max(tabtabPixelsOutput,j,i,(char)1) &&(b8Connected=1)==1)))
	{
	  find_terminal_branch(tabtabPixelsOutput, tabtabVisitedPixels, j, i,
			       b8Connected, pNeighborhood, tabConnections);
	  b8Connected = 0;
	}
       }	
}

template<class TInputImage, class TOutputTree>
Flst<TInputImage,TOutputTree>
::Flst()
{
  pGlobalTree = new Shapes;
  iMinArea = 1;
}

template<class TInputImage, class TOutputTree>
Flst<TInputImage,TOutputTree>
::~Flst()
{
  
}  


template<class TInputImage, class TOutputTree>
void
Flst<TInputImage,TOutputTree>
::PrintTree(Shapes* pTree)
{
  std::cout <<std::endl;
  std::cout << "NbShapes : " << pTree->nb_shapes;
  std::cout << " Nb col   : " << pTree->ncol;
  std::cout << " Nb lig   : " << pTree->nrow << std::endl;

  for(int lig = 0 ; lig < pTree->nrow;lig++)
  for(int col = 0 ; col < pTree->ncol;col++)
    {
    std::cout <<" smallest_shape("<< lig << " , "<< col <<" ) = " ;
    std::cout << pTree->smallest_shape[lig*pTree->ncol+col] <<std::endl;
    }

  for (int NoShape = 0 ; NoShape <  pTree->nb_shapes ;  NoShape++ )
    {
    std::cout <<" NoShape :" <<NoShape;
    std::cout << " Shape Adress :" << &pTree->the_shapes[NoShape];
    std::cout <<" Parent : "    << pTree->the_shapes[NoShape].parent;
    std::cout <<" Child : "     << pTree->the_shapes[NoShape].child;
    std::cout <<" next_sibling "<< pTree->the_shapes[NoShape].next_sibling;
    std::cout <<" pixels : "    << pTree->the_shapes[NoShape].pixels;
    std::cout <<" value : "     << pTree->the_shapes[NoShape].value;
    std::cout <<" open : "      << int(pTree->the_shapes[NoShape].open);
    std::cout <<" area : "      << pTree->the_shapes[NoShape].area;
    std::cout <<" removed : "   << int(pTree->the_shapes[NoShape].removed);
    
    std::cout << std::endl;
    }
}  

template<class TInputImage, class TOutputTree>
void
Flst<TInputImage,TOutputTree>
::PrintPixelOutput(float **tabtabPixelsOutput)
{
  for (int i = 0 ; i < iHeight ; i++)
  for (int j = 0 ; j < iWidth ; j++)
    {
      std::cout << " tabtabPixelsOutput[" << i << "][" << j << "]=" << tabtabPixelsOutput[i][j];
      std::cout << std::endl;
    }

}


/* The "Fast Level Set Transform" gives the tree of interiors of level lines
(named 'shapes') representing the image.
Only shapes of area >= *pMinArea are put in the tree. pMinArea==NULL means 1.
Output: *pTree is filled */

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::GenerateData()
{

  typename InputImageType::RegionType region;    
  InputImageSizeType                  Taille;
  int                                 i;
  int                                 j;
  RealImageIndexType                  Index;

  float **tabtabPixelsOutput;       /* Array accessing pixels of output image */
  Neighborhood neighborhood;        /* The neighborhood of the current region */
  int** tabtabVisitedPixels;        /* Image of last visit for each pixel */
  Connection* tabConnections;
  Shapes*    pTree = NULL;
  
//  pGlobalTree = pTree;


  std::cout << "Generate data FLST" <<std::endl;
  const InputImageType*  InputImage   = this->GetInput();
  
  
  
  itkDebugMacro(<< "FLST::GenerateData() called");

  pTree = new Shapes;
//  pGlobalTree = new Shapes;
  
  pGlobalTree = pTree;

  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  iWidth  = static_cast<int>(Taille[0]);  
  iHeight = static_cast<int>(Taille[1]);

  iAreaImage = iWidth * iHeight; 
  iHalfAreaImage = (iAreaImage+1) / 2;
  if(iWidth == 1) iPerimeterImage = iHeight;
  else if(iHeight == 1) iPerimeterImage = iWidth;
  else iPerimeterImage = (iWidth+iHeight-2)*2;
  if(iMinArea > iAreaImage) std::cerr << "min area > image" << std::endl;

  std::cout << "Width   : " << iWidth    << std::endl;
  std::cout << "Height  : " << iHeight   << std::endl;
  std::cout << "MinArea : " << iMinArea  << std::endl;
  

  Index[0] = 0;
  Index[1] = 0;
  float gray = static_cast<float>( InputImage->GetPixel(Index) );
  pGlobalTree->mw_change_shapes( iHeight, iWidth, gray);
  pGlobalTree->interpolation = 0;

  PrintTree(pGlobalTree);
  PrintTree(pTree);
  
  
  tabConnections = new Connection[iAreaImage];
  if(tabConnections == NULL)
    std::cerr << "Image of largest shapes: allocation error" << std::endl;
  for(i = iAreaImage-1; i >= 0; i--)
    tabConnections[i].shape = NULL;


  init_output_image(InputImage, &tabtabPixelsOutput);

/*
  tabtabPixelsOutput[0][0] = 255.;
  tabtabPixelsOutput[0][1] = 255.;
  tabtabPixelsOutput[0][2] = 255.;
  tabtabPixelsOutput[0][3] = 255.;

  tabtabPixelsOutput[1][0] = 255.;
  tabtabPixelsOutput[1][1] = 0.;
  tabtabPixelsOutput[1][2] = 0.;
  tabtabPixelsOutput[1][3] = 255.;

  tabtabPixelsOutput[2][0] = 255.;
  tabtabPixelsOutput[2][1] = 255.;
  tabtabPixelsOutput[2][2] = 255.;
  tabtabPixelsOutput[2][3] = 255.;
*/

  for (i = 0 ; i < iHeight ; i++)
    for	(j = 0 ; j < iWidth ; j++)
        {
	Index[0] = j;
	Index[1] = i;
	
        tabtabPixelsOutput[i][j] = ( static_cast<float>(InputImage->GetPixel( Index )) );

	std::cout << "PixelOut [" << i << "][" << j << "] = " << tabtabPixelsOutput[i][j] << std::endl;
	}


  init_image_of_visited_pixels(&tabtabVisitedPixels);
  neighborhood.init_neighborhood(iAreaImage,iWidth,iHeight);
  init_region(iAreaImage);

  iExploration = 0;
  /* Loop with increasing iMaxArea: speed optimization. Result correct
  with only one call to `scan' and iMaxArea = iAreaImage */
  iMaxArea = 0;
  do {
    if(iMaxArea == 0)
      iMaxArea = INIT_MAX_AREA;
    else
      iMaxArea <<= STEP_MAX_AREA;
    if(iMaxArea == 0 || iMaxArea >= iHalfAreaImage) /* iMaxArea==0: overflow */
      iMaxArea = iAreaImage-1;
    scan(tabtabPixelsOutput, tabtabVisitedPixels,&neighborhood,tabConnections);
    std::cout << " ScanResult:" <<std::endl;
    PrintTree(pGlobalTree);
     
  } while(iMaxArea+1 < iAreaImage);


	

  /* Make connections with root */
  std::cout << " NbShapes : " << pTree->nb_shapes << std::endl;
  pTree->the_shapes[0].value = tabtabPixelsOutput[0][0];
  std::cout << "iAreaImage = " << iAreaImage <<std::endl;
  for(i = iAreaImage-1; i >= 0; i--)
    {
     std::cout << " Connections["<<i<<"]: " << tabConnections[i].shape<< std::endl;
    
    if(tabConnections[i].shape != NULL) 
      {
      std::cout << "i : " << i << " Connections INSERT CHILDREN: " << std::endl;
	
      assert(tabConnections[i].level == pTree->the_shapes[0].value);
      insert_children(pTree->the_shapes, tabConnections[i].shape);
      }
    }



//  pGlobalTree->flst_pixels();
  
  TreePointer TreePath = TreeType::New();
   
  for (int NoShape = 0 ; NoShape <=pTree->nb_shapes ;  NoShape++ )
    {
    std::cout << "Shape Area : "<<NoShape <<" -->" << pTree->the_shapes[NoShape].area << std::endl;
    std::cout << "Shape value: "<<NoShape <<" -->" << pTree->the_shapes[NoShape].value << std::endl;
    std::cout << "Shape open : "<<NoShape <<" -->" << pTree->the_shapes[NoShape].open << std::endl;
    std::cout << "Shape pixel: "<<NoShape <<" -->" << pTree->the_shapes[NoShape].pixels<< std::endl;

    PathPointer pathList = PathType::New();
    pathList = pTree->flst_shape_boundary(&pTree->the_shapes[NoShape]);
    
    
    
    
    }

  delete[] tabConnections;
  neighborhood.free_neighborhood();
  free_image_of_visited_pixels(tabtabVisitedPixels);
  free_region(); 
  free_output_image(tabtabPixelsOutput);
  
  std::cout << "GenerateData() FINI !!" << std::endl;
//  delete pGlobalTree;
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
//  os << indent << "MinArea: " << m_MinArea << std::endl;
}
/* Insert a new shape and its siblings in the tree, with parent pParent */

template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree> 
::insert_children(Shape* pParent, Shape* pNewChildToInsert)
{
  Shape* pSibling = pNewChildToInsert;
  while(pSibling->next_sibling != NULL) {
    pSibling->parent = pParent;
    pSibling = pSibling->next_sibling;
  }
  pSibling->parent = pParent;
  pSibling->next_sibling = pParent->child;
  pParent->child = pNewChildToInsert;
}


}; // end namespace otb

 
#endif
