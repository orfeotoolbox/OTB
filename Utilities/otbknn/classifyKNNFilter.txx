/*=========================================
 Copyright (c) Ruben Cardenes Almeida. August 2006
 This work is licensed under a
 Creative Commons Atribution 2.5 License
 http://creativecommons.org/licenses/by/2.5
 ==========================================*/

#ifndef _classifyKNNFilter_txx
#define _classifyKNNFilter_txx

#include <iostream>
#include "classifyKNNFilter.h"
#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "voronoiFilter.h"
#include "classifyKNNCore.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkImageDuplicator.h"


namespace itk {


  /**
   * Constructor
   */
template <class TCh1, class TOutput, unsigned int m_ImageDimension>
  classifyKNNFilter<TCh1, TOutput, m_ImageDimension>
  :: classifyKNNFilter()
  {
    /**
     * Set the number of inputs and outputs for the filter. 
    **/
    this->SetNumberOfRequiredInputs( 1 );
    this->SetNumberOfRequiredOutputs( 1 );
    
    /**
     * A pointer to the output image is created and conected to the output
     **/

    OutputPointer output = OutputType::New();
    this->SetNthOutput( 0, output.GetPointer() );

    m_K=1;
    
  }// End Constructor


template <class TCh1, class TOutput, unsigned int m_ImageDimension>
  void
  classifyKNNFilter<TCh1, TOutput, m_ImageDimension>
  ::SetInput( const TCh1 * input1 ) 
  {
    // Process object is not const-correct so the const casting is required.
    SetNthInput(0, const_cast<TCh1 *>( input1 ));
   
  }


template <class TCh1, class TOutput, unsigned int m_ImageDimension>
 void
  classifyKNNFilter<TCh1, TOutput, m_ImageDimension>
  ::SetCh1( const TCh1 * channel1 ) 
  {
    // Process object is not const-correct so the const casting is required.
    SetInput(channel1);
   
  }

template <class TCh1, class TOutput, unsigned int m_ImageDimension>
 void
  classifyKNNFilter<TCh1, TOutput, m_ImageDimension>
::GenerateData()
{

    Ch1Pointer ch1
      = dynamic_cast<const TCh1*>(ProcessObject::GetInput(0));
    
    OutputPointer output = this->GetOutput(0);
    
    typename itk::MinimumMaximumImageCalculator< Ch1Type >::Pointer Ch1Calculator=itk::MinimumMaximumImageCalculator<  Ch1Type  >::New();

    Ch1Calculator->SetImage(ch1);

    VoronoiIndexType v_Index;
    v_Index.Fill(0);
    InputRegionType inputRegion;
    inputRegion.SetIndex(v_Index);
    InputSizeType regionSize;
    Ch1Calculator->ComputeMaximum();
    regionSize[0]=Ch1Calculator->GetMaximum()+1;
    inputRegion.SetSize(regionSize);
    InputPointer voronoi_input= InputType::New();
    std::cout << "inputRegion=" << inputRegion << std::endl;
    voronoi_input->SetRegions(inputRegion );
    voronoi_input->Allocate();
    voronoi_input->FillBuffer(-1);   

    // int maxClass=0;
    unsigned int i;
    int aux;
    InputSizeType region= (voronoi_input->GetRequestedRegion()).GetSize();

    for (i=0;i<m_Prototipos.size();i++){
      voronoi_input->SetPixel(m_Prototipos[i].index_intensity,i);
    }

    output->SetRegions( ch1->GetRequestedRegion() );

    typedef ImageRegionConstIteratorWithIndex<InputType> InputIterator;
    InputIterator inputIt = InputIterator(voronoi_input,voronoi_input->GetRequestedRegion());

    output->Allocate();
      
    typedef voronoiFilter<InputType, typename IntermediateImageType::PixelType,1> VoronoiFilterType;

    OutputPointer auxOutput=OutputType::New();
    auxOutput->SetRegions(ch1->GetRequestedRegion());
    auxOutput->Allocate();

    typename VoronoiFilterType::Pointer voronoi = VoronoiFilterType::New();
    typename classifyKNNCoreType::Pointer KNNCore = classifyKNNCoreType::New();
    typename DiagramImageType::Pointer diagram= DiagramImageType::New();

    voronoi->SetBackgroundValue(-1);
    voronoi->SetInput(voronoi_input);
    voronoi->SetK(m_K);
    diagram=voronoi->GetOutput();
    voronoi->Update();

    KNNCore->SetDiagram(diagram);
    KNNCore->SetCh1(ch1);
    KNNCore->SetK(m_K);
    KNNCore->SetPrototipos(m_Prototipos);
//    KNNCore->SetMaxClass(maxClass);
    output=KNNCore->GetOutput();
    KNNCore->Update();


}//end GenerateData



}//end namespace itk


#endif
