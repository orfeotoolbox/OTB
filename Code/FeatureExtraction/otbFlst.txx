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

ShapeType::ShapeType()
{
   this->parent = NULL;
   this->child  = NULL;
   this->next_sibling = NULL;
//   this->parent = new ShapeType;
//   this->child  = new ShapeType;
//   this->next_sibling = new ShapeType;
   this->inferior_type = 0;
   this->value = 0.0;
   this->area = 0;
   this->removed = 0;
}
 
ShapeType::~ShapeType()
{
    delete this->parent;
    delete this->child;
    delete this->next_sibling;
}

/* Insert a new shape and its siblings in the tree, with parent pParent */
void ShapeType::InsertChildren(ShapeType *pParent, ShapeType *pNewChildToInsert)
{
  ShapeType* pSibling = pNewChildToInsert;
  while(pSibling->next_sibling != NULL) {
    pSibling->parent = pParent;
    pSibling = pSibling->next_sibling;
  }
  pSibling->parent = pParent;
  pSibling->next_sibling = pParent->child;
  pParent->child = pNewChildToInsert;
}


ShapeList::ShapeList()
{
   this->the_shapes     = NULL;
   this->smallest_shape = NULL;
//   this->the_shapes     = new ShapeType;
//   this->smallest_shape = new (ShapeType*);
   this->nrow           = 0;
   this->ncol           = 0;
   this->interpolation  = 0;
   this->nb_shapes      = 0;
}

ShapeList::~ShapeList()
{
/*   this->nrow           = 0;
   this->ncol           = 0;
   this->interpolation  = 0;
   this->nb_shapes      = 0;*/
   delete this->the_shapes;
   delete this->smallest_shape;    
}

void ShapeList::Allocate(int nrow,int ncol,float value)
{
 int       size,i;
 ShapeType *root;
 size = nrow*ncol;
  if (size <= 0)
      std::cerr <<"[Allocate] Attempts to alloc shapes with null size"<<std::endl;

  if ((this->the_shapes!=NULL)||(this->smallest_shape!=NULL))
      std::cerr <<"[Allocate] Attempts to alloc root which is already allocated"<<std::endl;

  this->the_shapes    = new ShapeType[size+1];
  root = this->the_shapes;
  
  root->inferior_type = 1;
  root->value         = value;
  root->open          = 1;
  root->area          = size;
  root->removed       = 0;
  root->pixels.clear();
  root->boundary.clear();
  root->parent       = NULL; 
  root->next_sibling = NULL;
  root->child        = NULL;

  this->nb_shapes=1;
  this->nrow     = nrow;
  this->ncol     = ncol;  

  this->smallest_shape = new ShapeType*[size];
  if (this->smallest_shape==NULL)
    {
      std::cerr << "[Allocate] Not enough memory" <<std::endl;
      delete []this->the_shapes;
    }
  for (i=size-1; i>=0; i--) this->smallest_shape[i] = root;
}

void ShapeList::Delete()
{
   for(int i = this->nb_shapes-1; i > 0; i--)
    if(!(this->the_shapes[i].boundary.empty()))
      this->the_shapes[i].boundary.clear();

  if((this->the_shapes != NULL) && (this->nb_shapes > 0))
      this->the_shapes[0].pixels.clear();
  if (this->the_shapes != NULL) delete []this->the_shapes;
  if (this->smallest_shape != NULL) delete [](this->smallest_shape);
}

void ShapeList::Change(int nrow,int ncol,float value)
{
  int size;
  int i;

  /* Deallocate the shapes but the structure itself */
  for(i = nb_shapes-1; i > 0; i--)
    if(!the_shapes[i].boundary.empty())
        the_shapes[i].boundary.clear();

  if((the_shapes != NULL) && (nb_shapes > 0))
      the_shapes[0].pixels.clear();
  if (the_shapes != NULL) delete the_shapes;
  if (smallest_shape != NULL) delete smallest_shape;

  this->Allocate(nrow, ncol, value);
}

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



/* Is pixel (x, y) a local minimum? */
template< class TInputImage, class TOutputTree>
char Flst<TInputImage,TOutputTree>
::Is_local_min(RealImagePointer ou, int x, int y, char b8Connected)
{
  float v[9];
  char n = 0;
  IntImageIndexType Index;
  
  
  Index[1] = y-1 ;   Index[0] = x-1;  v[0] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y-1 ;   Index[0] = x  ;  v[1] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y-1 ;   Index[0] = x+1;  v[2] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x-1;  v[3] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x;    v[4] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x+1;  v[5] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x-1;  v[6] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x;    v[7] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x+1;  v[8] = static_cast<float>( ou->GetPixel(Index) );
   
  return (x==m_Width-1 || (v[2]>v[4] && ++n) || v[5]==v[4]) &&
         (x==0 || (v[3]>v[4] && ++n) || v[3]==v[4]) &&
         (y==m_Height-1 || (v[7]>v[4] && ++n) || v[7]==v[4]) &&
         (y==0 || (v[1]>v[4] && ++n) || v[1]==v[4]) &&
         (b8Connected == 0 ||
         ( (x==m_Width-1 || y==0 || (v[2]>v[4]  && ++n) || v[2]==v[4]) &&
         (x==m_Width-1||y==m_Height-1||(v[8]>v[4] && ++n)|| v[8]==v[4]) &&
         (x==0 || y==m_Height-1 || (v[6]>v[4] && ++n) || v[6]==v[4]) &&
         (x==0 || y==0 || (v[0]>v[4] && ++n) || v[0]==v[4])))	&&
    n != 0;
}


/* Is pixel (x,y) a local maximum? */
template< class TInputImage, class TOutputTree>
char Flst<TInputImage,TOutputTree>
::Is_local_max(RealImagePointer ou, int x, int y, char b8Connected)
{
  float v[9];
  char n = 0;
  RealImageIndexType Index;
  
  
  Index[1] = y-1 ;   Index[0] = x-1;  v[0] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y-1 ;   Index[0] = x  ;  v[1] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y-1 ;   Index[0] = x+1;  v[2] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x-1;  v[3] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x;    v[4] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x+1;  v[5] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x-1;  v[6] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x;    v[7] = static_cast<float>( ou->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x+1;  v[8] = static_cast<float>( ou->GetPixel(Index) );

  return (x==m_Width-1 || (v[5]<v[4] && ++n) || v[5]==v[4]) &&
         (x==0 || (v[3]<v[4] && ++n) || v[3]==v[4]) &&
         (y==m_Height-1 || (v[7]<v[4] && ++n) || v[7]==v[4]) &&
         (y==0 || (v[1]<v[4] && ++n) || v[1]==v[4]) && 
         (b8Connected == 0 ||
         ((x==m_Width-1 || y==0 || (v[2]<v[4]  && ++n) || v[2]==v[4]) &&
         (x==m_Width-1||y==m_Height-1||(v[8]<v[4] && ++n)|| v[8]==v[4]) &&
         (x==0 || y==m_Height-1 || (v[6]<v[4] && ++n) || v[6]==v[4]) &&
         (x==0 || y==0 || (v[0]<v[4] && ++n) || v[0]==v[4]))) &&
          n != 0;
}



/* Set pixels and saddle points in `tabPoints' at level newGray */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::Levelize(PointPlaneListType tabPoints,int iNbPoints, float newGray)
{
  RealImageIndexType Index;
  for(int i = iNbPoints - 1; i >= 0; i--)
    {
    Index[1] = tabPoints[i].y;
    Index[0] = tabPoints[i].x;
    m_PixelOutput->SetPixel(Index,newGray);
    }
}


/* Return, coded in one byte, the local configuration around the pixel (x,y) */
template< class TInputImage, class TOutputTree>
unsigned char 
Flst<TInputImage,TOutputTree>
::Configuration(int x,int y)
{
  short int iMaxX = m_Width-1, iMaxY = m_Height-1;
  unsigned char cPattern = 0;
  int VisitedPixels[3][3];
  IntImageIndexType Index;
  
  Index[1] = y-1 ;   Index[0] = x-1;  VisitedPixels[0][0] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y-1 ;   Index[0] = x  ;  VisitedPixels[0][1] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y-1 ;   Index[0] = x+1;  VisitedPixels[0][2] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x-1;  VisitedPixels[1][0] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x;    VisitedPixels[1][1] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y   ;   Index[0] = x+1;  VisitedPixels[1][2] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x-1;  VisitedPixels[2][0] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x;    VisitedPixels[2][1] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  Index[1] = y+1 ;   Index[0] = x+1;  VisitedPixels[2][2] = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
  
  
  if(x != 0) {
    if(VisitedPixels[1][0] == m_Exploration)
      cPattern = WEST;
    if((y == 0 && m_AtBorder) ||
       (y > 0 && VisitedPixels[0][0] == m_Exploration))
      cPattern |= NORTH_WEST;
    if((y == iMaxY && m_AtBorder) ||
       (y < iMaxY && VisitedPixels[2][0] == m_Exploration))
      cPattern |= SOUTH_WEST;    
  } else if(m_AtBorder)
    cPattern = SOUTH_WEST | WEST | NORTH_WEST;
  
  if(x != iMaxX) {
    if(VisitedPixels[1][2] == m_Exploration)
      cPattern |= EAST;
    if((y == 0 && m_AtBorder) ||
       (y > 0 && VisitedPixels[0][2] == m_Exploration))
      cPattern |= NORTH_EAST;
    if((y == iMaxY && m_AtBorder) ||
       (y < iMaxY && VisitedPixels[2][2] == m_Exploration))
      cPattern |= SOUTH_EAST;
  } else if(m_AtBorder)
    cPattern |= SOUTH_EAST | EAST | NORTH_EAST;
  
  if((y == 0 && m_AtBorder) ||
     (y > 0 && VisitedPixels[0][1] == m_Exploration))
    cPattern |= NORTH;

  if((y == iMaxY && m_AtBorder) ||
     (y < iMaxY && VisitedPixels[2][1] == m_Exploration))
    cPattern |= SOUTH;

  return cPattern;
}

template< class TInputImage, class TOutputTree>
ShapeType* 
Flst<TInputImage,TOutputTree>
::NewShape(int iCurrentArea, float currentGrayLevel, char bOfInferiorType,ShapeType* pChild)
{
  ShapeType* pNewShape = &m_GlobalTree->the_shapes[m_GlobalTree->nb_shapes++];

  pNewShape->inferior_type = bOfInferiorType;
  pNewShape->value = currentGrayLevel;
  pNewShape->open = (char)(m_AtBorder != 0);
  pNewShape->area = iCurrentArea;
  pNewShape->removed = 0;
  pNewShape->pixels.clear();
  pNewShape->boundary.clear();
  pNewShape->parent = NULL;
  pNewShape->next_sibling = NULL;
  pNewShape->child = pChild;
  if(pChild != NULL)
    pChild->parent = pNewShape;
  return pNewShape;
}



/* Knowing that the last extracted shape contains the points, update,
for each one, the smallest shape containing it */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::UpdateSmallestShapes(PointPlaneListType* tabPoints,
			int iLastShapeArea, int iArea)
{
  int i, iIndex;
  int iNbPoints;
  ShapeType* pNewShape;
  ShapeType* pRoot = &m_GlobalTree->the_shapes[0];

  iNbPoints = iArea - iLastShapeArea;
  
  pNewShape = &m_GlobalTree->the_shapes[m_GlobalTree->nb_shapes-1];
  for(i = iNbPoints - 1; i >= iLastShapeArea; i--)
    {
      iIndex = (*tabPoints)[i].y * m_Width + (*tabPoints)[i].x;
      if(m_GlobalTree->smallest_shape[iIndex] == pRoot)
	m_GlobalTree->smallest_shape[iIndex] = pNewShape;
    }
}

/* Find children of the last constructed monotone section, which is composed
of the interval between pSmallestShape and the last extracted shape. That is,
find shapes in other monotone sections whose parent is inside this interval */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::Connect(PointPlaneListType* tabPoints,int iNbPoints,
          ConnectionListType* tabConnections,ShapeType* pSmallestShape)
{
  int i, iIndex;
  ShapeType *pShape;
  ShapeType *pParent;
  float level;

  for(i = iNbPoints-1; i >= 0; i--) {
    iIndex = (*tabPoints)[i].y * m_Width + (*tabPoints)[i].x;
    pShape = (*tabConnections)[iIndex].shape;
    if(pShape != NULL) {
      level = (*tabConnections)[iIndex].level;
      pParent = pSmallestShape;
      while(pParent->value != level) {
	assert(pParent != &m_GlobalTree->the_shapes[m_GlobalTree->nb_shapes-1]);
	pParent = pParent->parent;
      }
      ShapeType::InsertChildren(pParent, pShape);
      (*tabConnections)[iIndex].shape = NULL;
    }
  }
}

/* Make a new connection structure at the given point */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::NewConnection(PointPlaneType* pPoint,float level,ConnectionListType* tabConnections)
{
  int iIndex;
  ShapeType *pSibling;
  ShapeType *pShape = &m_GlobalTree->the_shapes[m_GlobalTree->nb_shapes-1];

  iIndex = pPoint->y*m_Width + pPoint->x;
  if( (*tabConnections)[iIndex].shape == NULL) {
      (*tabConnections)[iIndex].shape = pShape;
      (*tabConnections)[iIndex].level = level;
  } else {
    assert( (*tabConnections)[iIndex].level == level);
    pSibling = (*tabConnections)[iIndex].shape;
    while(pSibling->next_sibling != NULL)
      pSibling = pSibling->next_sibling;
    pSibling->next_sibling = pShape;
  }
}

/* Is the neighbor pixel already stored for this exploration? */
template< class TInputImage, class TOutputTree>
int 
Flst<TInputImage,TOutputTree>
::NEIGHBOR_NOT_STORED(int x,int y)
{ 
  IntImageIndexType Index;

  Index[1] = y;
  Index[0] = x;
  assert( x >= 0 );
  assert( y >= 0 );
  assert( x < m_Width  );
  assert (y < m_Height );
  
  return (m_VisitedPixels->GetPixel(Index) < m_Exploration);   
}


/* Store the 4-neighbors of pixel (x,y) */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::Store_4neighbors(int x,int y,NeighborhoodType* pNeighborhood)
{
  RealImageIndexType Index;
  float             value;
  
  if(x > 0 )
   if(NEIGHBOR_NOT_STORED(x-1,y))
    {
    Index[1] = y  ;
    Index[0] = x-1;
    value =  m_PixelOutput->GetPixel(Index); 
    pNeighborhood->Add( x-1, y, value);
    }
    
  if(x < m_Width-1)
    if(NEIGHBOR_NOT_STORED(x+1,y))
    {
    Index[1] = y  ;
    Index[0] = x+1;
    value =  m_PixelOutput->GetPixel(Index); 
    pNeighborhood->Add( x+1, y, value);
    }
  if(y > 0 )
   if(NEIGHBOR_NOT_STORED(x,y-1))
    {
    Index[1] = y-1;
    Index[0] = x  ;
    value =  m_PixelOutput->GetPixel(Index); 
    pNeighborhood->Add( x, y-1, value);
    }
  if(y < m_Height-1)
   if(NEIGHBOR_NOT_STORED(x,y+1))
    {
    Index[1] = y+1;
    Index[0] = x;
    value =  m_PixelOutput->GetPixel(Index); 
    pNeighborhood->Add( x, y+1, value);
    }
}

/* Store the diagonal neighbors of pixel (x,y) */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::Store_8neighbors(int x,int y,NeighborhoodType* pNeighborhood)
{
  RealImageIndexType Index;
  float             value;

  if(x > 0) {
    if(y > 0)
     if(NEIGHBOR_NOT_STORED(x-1,y-1))
      {
      Index[1] = y-1;
      Index[0] = x-1;
      value =  m_PixelOutput->GetPixel(Index); 
      pNeighborhood->Add( x-1,y-1, value);      
      }
    if(y < m_Height-1)
     if(NEIGHBOR_NOT_STORED(x-1,y+1))
      {
      Index[1] = y+1;
      Index[0] = x-1;
      value =  m_PixelOutput->GetPixel(Index); 
      pNeighborhood->Add( x-1,y+1, value);      
      }
  }
  if(++x < m_Width) {
    if(y > 0 )
     if(NEIGHBOR_NOT_STORED(x,y-1))
      {
      Index[1] = y-1;
      Index[0] = x;
      value =  m_PixelOutput->GetPixel(Index); 
      pNeighborhood->Add( x,y-1, value);      
      }
    if(y < m_Height-1)
     if(NEIGHBOR_NOT_STORED(x,y+1))
      {
      Index[1] = y+1;
      Index[0] = x;
      value =  m_PixelOutput->GetPixel(Index); 
      pNeighborhood->Add( x,y+1, value);      
      }
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
::AddIsoLevel(PointPlaneListType tabPointsInShape,int* pCurrentArea,
	      float currentGrayLevel,NeighborhoodType* pNeighborhood,
	      char* p8Connected,char* pIgnoreHoles)
{
  int                x;
  int                y;
  NeighborPointType* pNeighbor;
  int iCurrentArea,  iNbHoles;
  unsigned char      cPattern;
  RealImageIndexType Index;

  iNbHoles = 0;
  iCurrentArea = *pCurrentArea;
  pNeighbor = &pNeighborhood->m_tabPoints[1];
  do { /* 1) Neighbor is added to the region */
    x = pNeighbor->x; 
    y = pNeighbor->y; 
    tabPointsInShape[iCurrentArea].x = x;
    tabPointsInShape[iCurrentArea++].y = y;
    if(! *pIgnoreHoles) {
      cPattern = this->Configuration( x, y);
      iNbHoles += tabPatterns[*p8Connected][cPattern];
    }
    if(x == 0 || x == m_Width-1 || y == 0 || y == m_Height-1)
      ++ m_AtBorder;
    
    Index[1] = y;
    Index[0] = x;  
    m_VisitedPixels->SetPixel( Index, m_Exploration);
    /* 2) Store new neighbors */
    this->Store_4neighbors(x, y, pNeighborhood);
    if(pNeighborhood->m_type == TreeNeighborhood::MAX) {
      if(! *p8Connected)
	*pIgnoreHoles = *p8Connected = (char)1;
      this->Store_8neighbors(x, y, pNeighborhood);
    }
    pNeighborhood->Free();   // A vérifier
  } while(iCurrentArea <= m_MaxArea &&
	  pNeighbor->value == currentGrayLevel &&  
	  pNeighborhood->m_type != TreeNeighborhood::INVALID);

  if(iCurrentArea <= m_MaxArea &&
     m_AtBorder != m_PerimeterImage &&
     (! m_AtBorder || iCurrentArea <= m_HalfAreaImage) &&
     pNeighborhood->m_type != TreeNeighborhood::INVALID &&
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
::FindTerminalBranch(int x,int y, char b8Connected, 
                     NeighborhoodType*   pNeighborhood, 
		     ConnectionListType* tabConnections)
{
  float     level;
  int       iArea;
  int       iLastShapeArea;
  char      bUnknownHoles;
  ShapeType *pSmallestShape;
  ShapeType *pLastShape;
  RealImageIndexType Index;

 restart_branch:
  ++ m_Exploration;
  iArea = m_AtBorder = 0;
  bUnknownHoles = 0;
  pSmallestShape = pLastShape = NULL;
  Index[1] = y;
  Index[0] = x;
  level = m_PixelOutput->GetPixel(Index);
  pNeighborhood->ReInit(b8Connected ? TreeNeighborhood::MAX: TreeNeighborhood::MIN);
  pNeighborhood->Add( x, y, level);
  while( this->AddIsoLevel(m_PointsInShape, &iArea,
		      level, pNeighborhood,
		      &b8Connected, &bUnknownHoles) != 0) {
    if(bUnknownHoles) {
      assert(iArea != 0);
      if(pLastShape != NULL) {
	iArea = pLastShape->area;
	this->Connect( & m_PointsInShape, iArea, tabConnections, pSmallestShape);
	this->NewConnection(&m_PointsInShape[iArea], level, tabConnections);
      }
      this->Levelize(m_PointsInShape, iArea, level);
      goto restart_branch;
    }
    if(m_MinArea <= iArea) { /* Store new shape? */
      iLastShapeArea = (pLastShape == NULL) ? 0 : pLastShape->area;
      pLastShape = this->NewShape(iArea, level, !b8Connected, pLastShape);
      if(pSmallestShape == NULL) pSmallestShape = pLastShape;
//      this->UpdateSmallestShapes(m_PointsInShape+iLastShapeArea,
//			     iArea-iLastShapeArea);
      this->UpdateSmallestShapes(&m_PointsInShape,iLastShapeArea,
			     iArea);
    }
    if(m_AtBorder && iArea == m_HalfAreaImage)
      break;
    bUnknownHoles = (char)(b8Connected && pNeighborhood->m_type == TreeNeighborhood::AMBIGUOUS);
    if(bUnknownHoles) b8Connected = 0;
    level = pNeighborhood->m_tabPoints[1].value;
  }
  if(pLastShape != NULL) {
    this->Connect( & m_PointsInShape, iArea, tabConnections, pSmallestShape);
    if(m_AtBorder && iArea == m_HalfAreaImage)
      ShapeType::InsertChildren(m_GlobalTree->the_shapes, pLastShape);
    else if(iArea != 0)
      this->NewConnection(&m_PointsInShape[iArea], level, tabConnections);
  }
  this->Levelize(m_PointsInShape, iArea, level);
}

/* Scan the image, calling a procedure to extract terminal branch at each
   (not yet visited) local extremum */
template< class TInputImage, class TOutputTree>
void 
Flst<TInputImage,TOutputTree>
::Scan(NeighborhoodType* pNeighborhood,ConnectionListType* tabConnections)
{
  int i, j;
  char b8Connected = 0;
  int iExplorationInit;
  int iValue;
  IntImageIndexType Index;
  
  iExplorationInit = m_Exploration;
  for(i = 0; i < m_Height; i++)
    for(j = 0; j < m_Width; j++)
      {
      Index[1] = i;
      Index[0] = j;
      iValue = static_cast<int>( m_VisitedPixels->GetPixel(Index) );
      if(iValue <= iExplorationInit &&
	 ( this->Is_local_min(m_PixelOutput, j, i, (char)0) ||
	  (this->Is_local_max(m_PixelOutput, j, i, (char)1) &&(b8Connected=1)==1)))
	{
//	  this->FindTerminalBranch( j, i, b8Connected, pNeighborhood, tabConnections);
	  b8Connected = 0;
	}
      }
}


template<class TInputImage, class TOutputTree>
Flst<TInputImage,TOutputTree>
::Flst()
{
  m_MinArea = 1;                            // Valeur par défaut
  m_VisitedPixels = IntImageType::New();
  m_PixelOutput   = RealImageType::New();
  m_GlobalTree    = new ShapeTreeType();
  m_Connections   = new ConnectionListType();
  m_Neighborhood  = new NeighborhoodType();
  
}

template<class TInputImage, class TOutputTree>
Flst<TInputImage,TOutputTree>
::~Flst()
{
  delete m_GlobalTree;
  delete m_Connections;
  delete m_Neighborhood;
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
  typedef itk::ImageRegionIteratorWithIndex< RealImageType >        OutputIteratorType; 
  typedef itk::ImageRegionConstIteratorWithIndex< InputImageType >  InputIteratorType; 

  std::cout << "Generate data FLST" <<std::endl;

  const InputImageType*  InputImage   = this->GetInput(0);
//  OutputTreePointerType  OutputTree   = this->GetOutput();
//  OutputTreeType        *OutputTree   = this->GetOutput();
  itkDebugMacro(<< "FLST::GenerateData() called");

  typename InputImageType::RegionType region;    
  InputImageSizeType                  Taille;
  int                                 i;
  RealImageIndexType                  Index;
    
  Taille = InputImage->GetLargestPossibleRegion().GetSize();
  m_Width  = static_cast<int>(Taille[0]);
  m_Height = static_cast<int>(Taille[1]);

  m_AreaImage = m_Width * m_Height; 
  m_HalfAreaImage = (m_AreaImage+1) / 2;
  if(m_Width == 1) m_PerimeterImage = m_Height;
  else if(m_Height == 1) m_PerimeterImage = m_Width;
  else m_PerimeterImage = (m_Width+m_Height-2)*2;

  if(m_MinArea > m_AreaImage) std::cerr << "CalculateArea: min area > image"<< std::endl;
  
  Index[0] = 0;
  Index[1] = 0;  
  float grayValue = static_cast<float> (InputImage->GetPixel(Index)); 
  m_GlobalTree->Change(m_Height, m_Width,grayValue );
  m_GlobalTree->interpolation = 0 ;
  m_Connections->resize(m_AreaImage);
  if(m_Connections == NULL)
    std::cerr << "Image of largest shapes: allocation error" <<std::endl;
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
   
  m_Neighborhood->Init(m_AreaImage);

  // Initialisation de l'attribut : m_PointsInShape
  m_PointsInShape.resize(m_AreaImage); 
  
  // Initialisation de l'attribut : m_PointsInShape
  m_Exploration = 0;
  m_MaxArea     = 0;

  std::cout << "Scan" <<std::endl;

  do {
    if(m_MaxArea == 0)
      m_MaxArea = INIT_MAX_AREA;
    else
      m_MaxArea <<= STEP_MAX_AREA;
    if(m_MaxArea == 0 || m_MaxArea >= m_HalfAreaImage) /* iMaxArea==0: overflow */
       m_MaxArea = m_AreaImage-1;
    this->Scan(m_Neighborhood,m_Connections);
  } while(m_MaxArea+1 < m_AreaImage);

  std::cout << "Make connection with root" <<std::endl;

  /* Make connections with root */
  Index[0] = 0;
  Index[1] = 0;
  m_GlobalTree->the_shapes[0].value = static_cast<float>(m_PixelOutput->GetPixel(Index));
  for(i = m_AreaImage-1; i >= 0; i--)
      {
      if( (*m_Connections)[i].shape != NULL) 
        {
        assert( (*m_Connections)[i].level == m_GlobalTree->the_shapes[0].value);
        ShapeType::InsertChildren(m_GlobalTree->the_shapes, (*m_Connections)[i].shape);
        }
     }  
  std::cout << "Generate data FLST: END" <<std::endl;
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
