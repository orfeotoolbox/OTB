/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CNES Stage - MICHEL J.
  Language  :   C++
  Date      :   2 aout 2006
  Version   :   $Revision$

=========================================================================*/
#ifndef _otbImageToEdgePathFilter_txx
#define _otbImageToEdgePathFilter_txx

#include "otbImageToEdgePathFilter.h"
#include "otbMacro.h"
#include "itkConstantPadImageFilter.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{
/*
 * Constructeur
 */
template <class TInputImage, class TOutputPath>
ImageToEdgePathFilter<TInputImage, TOutputPath>
::ImageToEdgePathFilter()
{
	m_ForegroundValue = PixelType(255);
}
/**
 * Methode impl�mentant le calcul du chemin
 */
template <class TInputImage, class TOutputPath>
void
ImageToEdgePathFilter<TInputImage, TOutputPath>
::GenerateData(void)
{
  
  const InputImageType * inputImage = this->GetInput();
  OutputPathType * outputPath       = this->GetOutput();
 
  otbMsgDebugMacro(<<"Foreground value : "<<m_ForegroundValue);
  
  PixelType initPadConstant(0);
  if( initPadConstant == m_ForegroundValue )
  {
  	initPadConstant = 1;
  }
  
  typedef itk::ConstantPadImageFilter<InputImageType,InputImageType> PadFilterType;
  typedef itk::ConstShapedNeighborhoodIterator<InputImageType> IteratorType;
  typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> LinearIteratorType;

  // Ajout de bords noirs d'�paisseur 1 � l'image pour d�tecter correctement
  // les regions aux bords
  typename PadFilterType::Pointer pad = PadFilterType::New();
  pad->SetInput(inputImage);
  pad->SetConstant(initPadConstant);
  long unsigned int padSize[2] = {1,1};
  pad->SetPadUpperBound(padSize);
  pad->SetPadLowerBound(padSize);
  pad->Update();
  // Parcours de l'image du bord jusqu'� trouver un premier pixel
  // blanc qui sera le point de d�part du Path
  LinearIteratorType linIter(pad->GetOutput(),pad->GetOutput()->GetLargestPossibleRegion());
  linIter.GoToBegin();
  bool flag = true;
  while(flag)
    {     
    if(linIter.Get() == m_ForegroundValue )
      {
      flag=false;
      }
      else
      {
    	++linIter;
      }
    }
  typename InputImageType::IndexType start = linIter.GetIndex();
  outputPath->AddVertex(start);
  
  // D�finition du voisinnage utilis� dans la recherche du chemin
  typename IteratorType::RadiusType radius;
  radius.Fill(1);
  IteratorType it(radius, pad->GetOutput(),pad->GetOutput()->GetLargestPossibleRegion());
  const typename IteratorType::OffsetType LEFT   ={{-1,0}};
  const typename IteratorType::OffsetType RIGHT  ={{1,0}};
  const typename IteratorType::OffsetType UP     ={{0,-1}};
  const typename IteratorType::OffsetType DOWN   ={{0,1}};
  const typename IteratorType::OffsetType LEFTUP   ={{-1,-1}};
  const typename IteratorType::OffsetType RIGHTDOWN ={{1,1}};
  const typename IteratorType::OffsetType RIGHTUP  ={{1,-1}};
  const typename IteratorType::OffsetType LEFTDOWN ={{-1,1}};
  const typename IteratorType::OffsetType CENTER ={{0,0}};
  it.ClearActiveList();
  it.ActivateOffset(LEFT);
  it.ActivateOffset(RIGHT);
  it.ActivateOffset(UP);
  it.ActivateOffset(DOWN);
  it.ActivateOffset(CENTER);
  it.ActivateOffset(RIGHTUP);
  it.ActivateOffset(RIGHTDOWN);
  it.ActivateOffset(LEFTUP);
  it.ActivateOffset(LEFTDOWN);
  // Le vecteur de rotation permet de parcourir le voisinage
  // dans le sens anti-trigonom�trique 
  std::vector<typename IteratorType::OffsetType> rotation;
  rotation.push_back(RIGHT);
  rotation.push_back(RIGHTDOWN);
  rotation.push_back(DOWN);
  rotation.push_back(LEFTDOWN);
  rotation.push_back(LEFT);
  rotation.push_back(LEFTUP);
  rotation.push_back(UP);
  rotation.push_back(RIGHTUP);
  // On initialise le point de d�part de l'it�rateur
  it.SetLocation(start);
  otbMsgDebugMacro(<<"START: "<<start);
  // Flag permet d'arr�ter l'algorithme
  flag = true;
  int nbMove=0;
  // nextStart permet d'indiquer � l'algorithme le point de
  // d�part de la rotation du voisinage pour rechercher le prochain
  // point du contour  
  int nextStart=0;
  // Tant que la recherche n'est pas termin�e
  while(flag)
    {
    // move permet de parcourir le voisinage de mani�re circulaire
    int move = nextStart;
    // edgeFound indique si l'on a rencontr� le contour dans la
    // recherche circulaire
    bool EdgeFound = false;
    // LastWasPositive indique l'�tat du dernier pixel du voisinage examin�
    //Init value
    bool LastWasPositive(false);
    // Tant que l'on n'a pas visit� tout le voisinage et que le
    // contour n'a pas �t� rencontr�
    while((move<nextStart+8)&&(!EdgeFound))
      {
      otbMsgDebugMacro(<<"SEARCH: "<<move%8<<" "<<it.GetPixel(rotation[move%8])<<" LAST: "<<LastWasPositive);
      // Si le dernier pixel parcouru �tait en dehors de la
      // r�gion et que le pixel courant est dans la r�gion
      if((!LastWasPositive) && (it.GetPixel(rotation[move%8]) == m_ForegroundValue) )
	{
	// Alors on a rencontr� le contour
	EdgeFound=true;
	}
      else
	{
	// Sinon on met � jour l'�tat du dernier pixel parcouru
	// et passe au pixel suivant dans la rotation du voisinage
	LastWasPositive=(it.GetPixel(rotation[move%8]) == m_ForegroundValue);
	move++;
	}
      }
    // Une fois la recherche du point suivant termin�
    // Si on a trouv� un nouveau point du contour
    if(EdgeFound)
      {
      // On met � jour le path
      it+=rotation[move%8];
      nextStart=(move+5)%8;
      outputPath->AddVertex(it.GetIndex(CENTER));
      otbMsgDebugMacro(<<it.GetIndex(CENTER));
      // Si ce nouveau point est situ� sur le point de d�part
      // et que l'algorithme a effectu� un nombre suffisant de
      // mouvements (il n'est pas � son point de d�part)
      if((it.GetIndex(CENTER)==start)&&(nbMove>=2))
	{
	// La recherche est termin�e
        flag=false;
	}
      else
	{
	//Sinon
        for(int i=0;i<8;i++)
	  {
	  // Si un des pixels du voisinage du point courant correspond
	  // au point de d�part
	  if((it.GetIndex(rotation[i])==start)&&(nbMove>=2))
	    {
	    // la recherche est termin�e
            flag=false;
	    }
	  }
	}
      }
      //Sinon
      else
      {
      // La recherche est termin�e, aucun pixel ne peut �tre
      // ajout� au contour.
      flag=false;
      }
    nbMove++;
    }
}
template <class TInputImage, class TOutputPath>
void
ImageToEdgePathFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << "Foreground value : "<<m_ForegroundValue<<std::endl;
}
} // Fin de l'espace de nom otb

#endif
