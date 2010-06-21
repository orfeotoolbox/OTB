/*=========================================
 Copyright (c) Ruben Cardenes Almeida. August 2006
 This work is licensed under a
 Creative Commons Atribution 2.5 License
 http://creativecommons.org/licenses/by/2.5
 ==========================================*/


#ifndef _classifyKNNCore_txx
#define _classifyKNNCore_txx

#include <iostream>
#include "classifyKNNCore.h"
#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "voronoiFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

namespace itk {


  /**
   * Constructor
   */
template <class TCh1, class TCh2, class TDiagram, class TOutput, unsigned int m_ImageDimension>
  classifyKNNCore<TCh1, TCh2, TDiagram, TOutput, m_ImageDimension>
  :: classifyKNNCore()
  {
    /**
     * Set the number of inputs and outputs for the filter. 
    **/
    this->SetNumberOfRequiredInputs( 3 );
    this->SetNumberOfRequiredOutputs( 1 );
    
    /**
     * A pointer to the output image is created and conected to the output
     **/
    OutputPointer output = OutputType::New();
    this->SetNthOutput( 0, output.GetPointer() );
    m_K = 1; 
  }// End Constructor


template <class TCh1, class TCh2, class TDiagram, class TOutput, unsigned int m_ImageDimension>
 void
  classifyKNNCore<TCh1, TCh2, TDiagram,  TOutput, m_ImageDimension>
  ::SetCh1( const TCh1 * channel1 ) 
  {
    SetNthInput(0, const_cast<TCh1 *>( channel1 ));
   
  }

template <class TCh1, class TCh2, class TDiagram, class TOutput, unsigned int m_ImageDimension>
 void
  classifyKNNCore<TCh1, TCh2, TDiagram,  TOutput, m_ImageDimension>
  ::SetCh2( const TCh2 * channel2 ) 
  {
    SetNthInput(1, const_cast<TCh2 *>( channel2 ));
   
  }


template <class TCh1, class TCh2, class TDiagram, class TOutput, unsigned int m_ImageDimension>
 void
  classifyKNNCore<TCh1, TCh2, TDiagram, TOutput, m_ImageDimension>
  ::SetDiagram( const TDiagram * diagram ) 
  {
    SetNthInput(2, const_cast<TDiagram *>( diagram ));
   
  }

template <class TCh1, class TCh2, class TDiagram, class TOutput, unsigned int m_ImageDimension>
 void
  classifyKNNCore<TCh1, TCh2, TDiagram, TOutput, m_ImageDimension>
::GenerateData()
{

    Ch1Pointer ch1
      = dynamic_cast<const TCh1*>(ProcessObject::GetInput(0));
    Ch2Pointer ch2
      = dynamic_cast<const TCh2*>(ProcessObject::GetInput(1));

     DiagramPointer voro
      = dynamic_cast<const TDiagram*>(ProcessObject::GetInput(2));

    typename MinimumMaximumImageCalculator<Ch1Type>::Pointer Ch1Calculator=MinimumMaximumImageCalculator<Ch1Type>::New();
    Ch1Calculator->SetImage(ch1);
    Ch1Calculator->ComputeMaximum();
    typename MinimumMaximumImageCalculator<Ch2Type>::Pointer Ch2Calculator=MinimumMaximumImageCalculator<Ch2Type>::New();
    Ch2Calculator->SetImage(ch2);
    Ch2Calculator->ComputeMaximum();

    OutputPointer output = this->GetOutput(0);
    output->SetRegions( ch1->GetRequestedRegion() );
    output->Allocate();

    typedef ImageRegionConstIteratorWithIndex<TCh1> Ch1Iterator;
    Ch1Iterator ch1It = Ch1Iterator(ch1,ch1->GetRequestedRegion());
    typedef ImageRegionConstIteratorWithIndex<TCh2> Ch2Iterator;
    Ch1Iterator ch2It = Ch1Iterator(ch2,ch2->GetRequestedRegion());
    typedef ImageRegionConstIteratorWithIndex<TDiagram> voroIterator;
    voroIterator voroIt = voroIterator(voro,voro->GetRequestedRegion());
    
    DiagramIndexType featureIndex;
    
    //struct NodeType auxNodo;

    //Presuponemos que las clases estan ordenadas, siendo cero el fondo y
    //1,2,3...  las clases.

    typename DiagramType::PixelType featureVector;

    int i,count,clasemax;    
    //    int classCount[m_MaxClass+1];//Le sumo uno, aun a costa de
    //int classCount[m_Prototipos.size()+1];
    int* classCount = new int[m_Prototipos.size()+1];

    //Le sumo uno, aun a costa de
    //tener un elemento mas, para no tener
    //que restar uno cada vez  

    for (ch1It.GoToBegin(),ch2It.GoToBegin(); !ch1It.IsAtEnd(); ++ch1It,++ch2It){

      featureIndex[0]=(unsigned int)ch1It.Get();
      featureIndex[1]=(unsigned int)ch2It.Get();
      featureVector= voro->GetPixel(featureIndex);
       memset(classCount,0,sizeof(int)*(m_Prototipos.size()+1));
       for (i=0;i<m_K;i++) {
	  classCount[m_Prototipos[featureVector[i]].clase]++;
       }

       count=0;
       clasemax=0;
       for (i=0;i<m_K;i++) { 
          if (count < classCount[m_Prototipos[featureVector[i]].clase]) {
	  count = classCount[m_Prototipos[featureVector[i]].clase];
	  clasemax = m_Prototipos[featureVector[i]].clase;
	  }
       }
       output->SetPixel(ch1It.GetIndex(),clasemax); 
    }

    delete[] classCount;

}//end GenerateData



}//end namespace itk


#endif
