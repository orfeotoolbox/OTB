/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Role      :  Definition de la classe TreeSource 
  $Id$ 

=========================================================================*/
 
#ifndef _otbTreeSource_txx
#define _otbTreeSource_txx

#include "otbTreeSource.h"

namespace otb
{

/**
 *
 */
template<class TOutputTree>
TreeSource<TOutputTree>
::TreeSource()
{
  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  m_GlobalTree = new ShapeTreeType;

}

template<class TOutputTree>
void
TreeSource<TOutputTree>
::TranslateTreeShapeToOutputTree( ShapeTreeConstPointerType  Shapes, 
		                  OutputTreePointerType      TreeContainer )
{
  // A définir
}				  
				  

#if 0
template<class TOutputTree>
typename ShapeType
TreeSource<TOutputTree>
::NewShape(	int iCurrentArea, float currentGrayLevel, char bOfInferiorType, 
		ShapeType *pChild)
{

  ShapePointerType pNewShape;
  pNewShape = &(m_GlobalTree->the_shapes[m_GlobalTree->nb_shapes++]);
  pNewShape->inferior_type = bOfInferiorType;
  pNewShape->value = currentGrayLevel;
  pNewShape->open = (char)(m_AtBorder != 0);
  pNewShape->area = m_CurrentArea;
  pNewShape->removed = 0;
  pNewShape->pixels = PointListType::New();
  pNewShape->parent = NULL;
  
  return pNewShape;
}
#endif

#if 0
template<class TOutputTree>
void
TreeSource<TOutputTree>
::UpdateSmallestShapes(PointListPointerType tabPoints, int NbPoints)
{

  int i, iIndex;
  ShapeType::Pointer pNewShape, pRoot;
   
  pRoot     = &m_GlobalTree->the_shapes[0];
  pNewShape = &m_GlobalTree->the_shapes[m_GlobalTree->nb_shapes-1];
  for(i = iNbPoints - 1; i >= 0; i--)
    {
      iIndex = tabPoints[i].y * iWidth + tabPoints[i].x;
      if(pGlobalTree->smallest_shape[iIndex] == pRoot)
	pGlobalTree->smallest_shape[iIndex] = pNewShape;
    }
//   itk::LevelOrderTreeIterator<TreeType> levelIt(tree,10,tree->GetNode(2));
//   levelIt.GoToBegin();
//   while(!levelIt.IsAtEnd())
//   {
//   std::cout << levelIt.Get() << " ("<< levelIt.GetLevel() << ")" << std::endl;;
//   ++levelIt;
//   }

}
#endif

/**
 *
 */
template<class TOutputTree>
void 
TreeSource<TOutputTree>
::AllocateShapeTree(int nrow,int ncol,float value)     
{

  int       size;
  int       i;
  ShapePointerType root;

  if (m_GlobalTree == NULL) 
    {
      std::cerr << "cannot alloc root : shapes structure is NULL" 
                << std::endl;
    }
  
  size = nrow*ncol;
  if (size <= 0)
    {
      std::cerr << "Attempts to alloc shapes with null size"
                << std::endl;
    }

  root = m_GlobalTree->the_shapes = new ShapeType[size+1];
  if (root==NULL)
    {
      std::cerr << " Not enough memory "<< std::endl;
    }
  root->inferior_type = 1;
  root->value         = value;
  root->open          = 1;
  root->area          = size;
  root->removed       = 0;
  root->pixels        = NULL;
  root->boundary      = PathType::New();
  root->parent        = NULL;
  root->next_sibling  = NULL;
  root->child         = NULL;

  m_GlobalTree->nb_shapes = 1;
  m_GlobalTree->nrow      = nrow;
  m_GlobalTree->ncol      = ncol;  

  m_GlobalTree->smallest_shape = new ShapePointerType[size];
  if (m_GlobalTree->smallest_shape==NULL)
    {
      std::cerr << " Not enough memory"<< std::endl;
      delete root;
    }
  for (i=size-1; i>=0; i--) m_GlobalTree->smallest_shape[i] = root;


}
 
 
template<class TOutputTree>
void 
TreeSource<TOutputTree>
::DeAllocateShapeTree()
{
  if (m_GlobalTree == NULL) 
    {
      std::cerr << "cannot delete: shapes structure is NULL" 
                << std::endl;
    }

  if( (m_GlobalTree->the_shapes != NULL) && 
        (m_GlobalTree->nb_shapes > 0)           )
     delete [](m_GlobalTree->the_shapes[0].pixels);
     
  if (m_GlobalTree->the_shapes != NULL) delete [](m_GlobalTree->the_shapes);
  if (m_GlobalTree->smallest_shape != NULL) delete [](m_GlobalTree->smallest_shape);

  delete m_GlobalTree;

}

template<class TOutputTree>
void
TreeSource<TOutputTree>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
