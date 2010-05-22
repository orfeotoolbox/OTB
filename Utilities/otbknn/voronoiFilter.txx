/*=========================================
 Copyright (c) Ruben Cardenes Almeida. August 2006
 This work is licensed under a
 Creative Commons Atribution 2.5 License
 http://creativecommons.org/licenses/by/2.5
 ==========================================*/


#ifndef _voronoiFilter_txx
#define _voronoiFilter_txx

#include "voronoiFilter.h"
#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkVector.h"
#include "itkConstantBoundaryCondition.h"
#include "itkNumericTraits.h"



#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


namespace itk
{
  
  /**
   * Constructor
   */
  
  template <class TDomain, typename MapElement, unsigned int m_ImageDimension> 
  voronoiFilter<TDomain, MapElement, m_ImageDimension> 
  ::voronoiFilter()
  {
    /**
     * Set the number of inputs and outputs for the filter. 
    **/
    this->SetNumberOfRequiredInputs( 1 );
    this->SetNumberOfRequiredOutputs( 1 );
    
    
    /**
     * A pointer to the output image is created and conected to the output
     **/
    MapPointer mapa = MapType::New();
    this->SetNthOutput( 0, mapa.GetPointer() );
//    m_ImageDimension=itkGetStaticConstMacro(SetDimension);
	m_BackgroundValue=0;
	m_K=1;
  }// End Constructor
  
  
  
  
  /**
   * Connect the Domain
   **/
  
  template <class TDomain, typename MapElement, unsigned int m_ImageDimension> 
  void
  voronoiFilter<TDomain, MapElement, m_ImageDimension> 
  ::SetInput(const  TDomain * domain1 ) 
  {
    // Process object is not const-correct so the const casting is required.
    SetNthInput(0, const_cast<TDomain *>( domain1 ));
   
  }
  

  template <class TDomain, typename MapElement, unsigned int m_ImageDimension> 
  void
  voronoiFilter<TDomain, MapElement, m_ImageDimension> 
  ::indexConversorInit(IndexType& indexConversor,DomainSizeType size) {
    unsigned int i,aux;
    indexConversor[0]=1;
    aux=1;
      for(i=1;i<m_ImageDimension;i++){
         aux=size[i-1]*aux;
         indexConversor[i]=aux;
      }
    
  }


  template <class TDomain, typename MapElement, unsigned int m_ImageDimension> 
  unsigned int
  voronoiFilter<TDomain, MapElement, m_ImageDimension> 
  ::pasarALineal(IndexType index) {
    unsigned int i,aux;
    aux=0;
      for(i=1;i<m_ImageDimension;i++){
         aux=aux+index[i]*m_indexConversor[i];
      }
      return aux+index[0];
  }



  template <class TDomain, typename MapElement, unsigned int m_ImageDimension> 
  float
  voronoiFilter<TDomain, MapElement, m_ImageDimension> 
  ::distance(IndexType mapIndex1, IndexType mapIndex2) {

    float sum=0;
    for (unsigned int j=0; j< m_ImageDimension;j++) {
      sum += (mapIndex1[j] - mapIndex2[j]) * (mapIndex1[j] - mapIndex2[j]);
    }
    return sqrt(sum);    

  }


  
  template <class TDomain, typename MapElement, unsigned int m_ImageDimension> 
  void
  voronoiFilter<TDomain, MapElement, m_ImageDimension> 
  ::GenerateData()
  {
    int j,i,l,d,next,dist,dist_2,asignacion,icurold,icurnew,icuraux,r,aux_j,rechazar,index,newIndex;
    int numrechazos=0;
    float distreal,distreal_2;
    IndexType CurrentIndex, newMapIndex;
    struct NodeType auxNode;
    ListType Lista0;
    ListType Lista1;
    ListType Lista2;
    
    /**
     **We do not use GetInput directly because the ImageToFilter GetInput 
     ** method only returs the instantiated class for the Superclass 
     **ImageToImage Filter. In our case TDomain.
     **/

    DomainPointer domain
      = dynamic_cast<const TDomain*>(ProcessObject::GetInput(0));

    
    MapPointer mapa = this->GetOutput(0);
    
    
    /*
     *We create an auxiliary image to store Element struct defined by Ruben Cardenes
     */
    

    MapPixelType MapAuxValue(m_K);
    
    
    
    /**
     ** Set regions for the output from the domain
     **/
    
    typename MapType::RegionType MapRegion = domain->GetRequestedRegion();



    mapa->SetRegions( MapRegion );
    
    mapa->Allocate();

    mapa->FillBuffer(MapAuxValue);
    
    indexConversorInit(m_indexConversor,MapRegion.GetSize());

    int auxiliar= m_indexConversor[m_ImageDimension-1]*MapRegion.GetSize()[m_ImageDimension-1];

    std::cout << "Auxiliar " << auxiliar << std::endl;
    struct element *Element;
    Element=(struct element*)malloc(sizeof(struct element)*auxiliar);

    float **maps;
    maps = (float**)malloc(sizeof(float*)*auxiliar);
    for (i=0;i<auxiliar;i++) {
      maps[i] = (float*)malloc(sizeof(float)*m_K);
    }


    //DomainPixelType dominio[auxiliar];
    std::vector<DomainPixelType> dominio;
    dominio.reserve(auxiliar);


    //    DomainPixelType dominio[m_indexConversor[m_ImageDimension-1]*MapRegion.GetSize()[m_ImageDimension-1]];
    //  float maps[m_indexConversor[m_ImageDimension-1]*MapRegion.GetSize()[m_ImageDimension-1]][K];
    //memset(maps,-1,sizeof(float)*(m_indexConversor[m_ImageDimension-1])*(MapRegion.GetSize()[m_ImageDimension-1])*K);

    //Definition of the Domain Iterators
    typedef ImageRegionConstIteratorWithIndex<TDomain> DomainIterator;


    DomainIterator domainIt = DomainIterator(domain,domain->GetRequestedRegion());
     
    
    typedef ImageRegionIterator<MapType> MapIterator;
    MapIterator mapIt = MapIterator(mapa,mapa->GetRequestedRegion());
    

   
    //To use in propagate8
    
    Offset<m_ImageDimension> offsetBucle,centro,ultimo;
     centro.Fill(0);
     ultimo.Fill(1);
    //initialize values

    for (index=0, l=0, domainIt.GoToBegin() ; !domainIt.IsAtEnd();  ++domainIt, index++) {

      Element[index].DObj=std::vector<float>(m_K);
      Element[index].PosObj=std::vector<IndexType>(m_K);

      for(i=0;i<m_K;i++) {
	Element[index].DObj[i]=0;  
      }  
      //Insert 1st element properly
      
      if(dominio.size()==0)
      {
		dominio.insert(dominio.begin(),domainIt.Get());
      }    
      else
      {
		dominio.insert(dominio.end(),domainIt.Get());
	  }
	  
      if (dominio[index] != m_BackgroundValue) {
	CurrentIndex=domainIt.GetIndex();
	Element[index].icur=1;
	Element[index].dcur=0;
	Element[index].PosObj[0]=CurrentIndex;
	maps[index][0]=dominio[index];
	auxNode.index=CurrentIndex;
	auxNode.l=dominio[index];
	Lista0.push_back(auxNode);
	l++;
      } else {
	Element[index].icur=0;
	Element[index].dcur=-1;      
      }

    }
    
    d=0;
    while (1) {
      while (!Lista0.empty() ) {
      auxNode=Lista0.back();
      CurrentIndex=auxNode.index;
      index=pasarALineal(CurrentIndex);
      l=auxNode.l;
	Lista0.pop_back();
	//Propagate8
	offsetBucle.Fill(-1);
	icurold=0;
	for (i=Element[index].icur;i>0;i--) {
	  if (maps[index][i-1]==l){
	    icurold=i;
	    break;
	  }
	}
		
	if (icurold==0){
	  itkExceptionMacro(<< "Error with icurold" );
	}    
	
	while (1){
      
	  newMapIndex=CurrentIndex+offsetBucle;
	  newIndex=pasarALineal(newMapIndex);
	  if ( (MapRegion.IsInside(newMapIndex)) && (offsetBucle!=centro) ) {

	    icurnew=Element[newIndex].icur;
	    icuraux=icurnew>0?icurnew-1:0;

	    if ( (maps[newIndex][icuraux]!=l) && (icurnew < m_K) ) { 
	    // if ( (dominio[newIndex]>=0) && (maps[newIndex][icuraux]!=l) && (icurnew < K) ) {

	      rechazar=0;

	      for (j=1;j<=icurnew;j++) {	
		aux_j = j>0?j-1:0;
		if (maps[newIndex][aux_j] == l) { 
		  numrechazos++; 
		  rechazar = 1;
		  break;
		}
	      }
	    
	      if ( (Element[newIndex].dcur<d ) && (rechazar==0) ) {
		distreal=distance(newMapIndex, Element[index].PosObj[icurold-1]) + Element[index].DObj[icurold-1];
		dist=round(distreal);
	      
		if (dist==d+1){
		  maps[newIndex][icurnew]=l;
		  Element[newIndex].icur++;
		  icurnew++;
		  Element[newIndex].PosObj[icurnew-1]=Element[index].PosObj[icurold-1];
		  Element[newIndex].DObj[icurnew-1]=Element[index].DObj[icurnew-1];
		  Element[newIndex].dcur=d;
		  auxNode.index=newMapIndex;
		  auxNode.l=l;
		  Lista1.push_back(auxNode);
		} //end if (dist==d+1)
	      
		else if (dist<=d){
		  maps[newIndex][icurnew]=l;
		  Element[newIndex].icur++;
		  icurnew++;
		  Element[newIndex].PosObj[icurnew-1]=CurrentIndex;
		  Element[newIndex].DObj[icurnew-1]=distreal;
		  Element[index].DObj[icurold-1]=distreal;
		  Element[newIndex].dcur=d;
		  auxNode.index=newMapIndex;
		  auxNode.l=l;
		  Lista2.push_back(auxNode);		  		
		}//end if (dist<=d)	      
	      }	else if ( (Element[newIndex].dcur>=d)   && (rechazar==0) ) {
		j=Element[newIndex].icur;
		distreal=distance(newMapIndex,Element[index].PosObj[icurold-1])+Element[index].DObj[icurold-1];
		dist=round(distreal);
		r=(int)maps[newIndex][j-1];
		distreal_2=distance(newMapIndex,Element[newIndex].PosObj[icurnew-1])+Element[newIndex].DObj[icurnew-1];
		dist_2=round(distreal_2);
		if (distreal<distreal_2){
		  maps[newIndex][j-1]=l;
		  asignacion=r;
		} else {
		  asignacion=l;
		}
		
		if ( (dist==d+1) || (dist<=d) ) {
		  maps[newIndex][icurnew]=asignacion;
		  Element[newIndex].icur++;
		  icurnew++;
		  if (asignacion==l) { 
		    Element[newIndex].PosObj[icurnew-1]=Element[index].PosObj[icurold-1];
		    Element[newIndex].DObj[icurnew-1]=Element[index].DObj[icurold-1];
		  } else {
		    
		    Element[newIndex].PosObj[icurnew-1]=Element[newIndex].PosObj[icurnew-2];
		    Element[newIndex].DObj[icurnew-1]=Element[newIndex].DObj[icurnew-2];

		    Element[newIndex].PosObj[icurnew-2]=Element[index].PosObj[icurold-1];
		    Element[newIndex].DObj[icurnew-2]=Element[index].DObj[icurold-1];

		  }

		  Element[newIndex].dcur=d;		 
		  auxNode.index=newMapIndex;
		  auxNode.l=l;
		  Lista1.push_back(auxNode);
		}
	      }
	    	    
	    }

	  }//end if ((MapRegion.IsInside...

	  if (offsetBucle==ultimo) { break; }
	  offsetBucle[0]=offsetBucle[0]+1;
	  if (offsetBucle[0]!=2) { continue; }
	  offsetBucle[0]=-1;
	  for (i=1;i<m_ImageDimension;i++) {
	    offsetBucle[i]=offsetBucle[i]+1;
	    if (offsetBucle[i]!=2) break;
	    offsetBucle[i]=-1;
	  }

	}//end while (1)
	//end propagate8			
	
      }//end while (!Lista0.empty())

      d++;
      Lista0.swap(Lista1);
      if (!Lista2.empty()) {
        Lista0.insert(Lista0.end(), Lista2.begin(), Lista2.end());
        Lista2.clear();
      }

      if (Lista0.empty() && Lista1.empty()) {break;}

    }//end while (1)


    for (index=0, mapIt.GoToBegin(); !mapIt.IsAtEnd(); ++mapIt, index++) {
      for (i=0; i<m_K; i++) {
	MapAuxValue[i]=(MapElement)maps[index][i];
      }
      mapIt.Set(MapAuxValue);
    }

    // For debugging purposes: 
    //    std::cout << "d= " << d << std::endl;
    //////////////////

    free(Element);

}// End GenerateData


  
  template <class TDomain, typename MapElement, unsigned int m_ImageDimension> 
  void
  voronoiFilter<TDomain, MapElement, m_ImageDimension> 
  ::PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    
  } 
  
}// end namespaceitk



#endif
