/*=========================================
 Copyright (c) Ruben Cardenes Almeida. August 2006
 This work is licensed under a
 Creative Commons Atribution 2.5 License
 http://creativecommons.org/licenses/by/2.5
 ==========================================*/


#ifndef __classifyKNNFilter_h
#define __classifyKNNFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "classifyKNNCore.h"

namespace itk {

template <class TCh1, class TOutput, unsigned int m_ImageDimension>
class ITK_EXPORT classifyKNNFilter :
   public ImageToImageFilter< TCh1, TOutput> 
{
public:
  typedef Image<int,m_ImageDimension> InputType;
  //  typedef Image<float,m_ImageDimension> InputType;
  typedef typename InputType::Pointer InputPointer;
  typedef typename InputType::RegionType InputRegionType; 
  typedef typename InputType::PixelType InputPixelType; 
  typedef typename InputType::SizeType InputSizeType;
  typedef typename InputType::IndexType IndexType;

  typedef Image<int,1> VoronoiImageType;
  //  typedef Image<float,m_ImageDimension> InputType;
  typedef typename VoronoiImageType::Pointer VoronoiPointer;
  typedef typename VoronoiImageType::RegionType VoronoiRegionType; 
  typedef typename VoronoiImageType::PixelType VoronoiPixelType; 
  typedef typename VoronoiImageType::SizeType VoronoiSizeType;
  typedef typename VoronoiImageType::IndexType VoronoiIndexType;

  typedef TOutput OutputType;
  typedef typename OutputType::Pointer OutputPointer;
  typedef typename OutputType::RegionType OutputRegionType; 
  typedef typename OutputType::PixelType OutputPixelType; 
  typedef typename OutputType::SizeType OutputSizeType;
  typedef TCh1 Ch1Type;
  typedef typename Ch1Type::ConstPointer Ch1Pointer;
  typedef typename Ch1Type::RegionType Ch1RegionType; 
  typedef typename Ch1Type::PixelType Ch1PixelType; 
  typedef typename Ch1Type::SizeType Ch1SizeType;

  typedef classifyKNNFilter  Self;
  typedef ImageToImageFilter<TCh1,TOutput>  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(classifyKNNFilter, ImageToImageFilter);

  void SetInput( const TCh1 * input1);
  void SetCh1( const TCh1 * channel1);
  itkSetMacro(K, unsigned short);

protected:

  typedef Image< std::vector<int> , m_ImageDimension> DiagramImageType;//int

  typedef Image<int, m_ImageDimension> IntermediateImageType;//int

  typedef classifyKNNCore<TCh1, DiagramImageType, IntermediateImageType, m_ImageDimension> classifyKNNCoreType;

public: 

  typedef typename  classifyKNNCoreType::NodeType NodeType;
  typedef typename  classifyKNNCoreType::ListType ListType;


 void SetPrototipos (const ListType _arg) 
   { 
    //Later we will put this in Lista 0
     m_Prototipos.insert(m_Prototipos.begin(),_arg.begin(),_arg.end());
       this->Modified(); 
   } 

protected:

 classifyKNNFilter();
 virtual ~classifyKNNFilter() {};
 
 void GenerateData();
// void PrintSelf(std::ostream& os, Indent indent) const;
 
 private:
 classifyKNNFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemented

  unsigned short m_K;
  ListType m_Prototipos;

};



}//end namespace itk



#ifndef ITK_MANUAL_INSTANTIATION
#include "classifyKNNFilter.cxx"
#endif

#endif
