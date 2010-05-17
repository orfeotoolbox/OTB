/*=========================================
 Copyright (c) Ruben Cardenes Almeida. August 2006
 This work is licensed under a
 Creative Commons Atribution 2.5 License
 http://creativecommons.org/licenses/by/2.5
 ==========================================*/

#ifndef __classifyKNNCore_h
#define __classifyKNNCore_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"

namespace itk {

template <class TCh1, class TCh2, class TDiagram, class TOutput, unsigned int m_ImageDimension>
class ITK_EXPORT classifyKNNCore :
   public ImageToImageFilter< TDiagram, TOutput> 
{
public:
  typedef TOutput OutputType;
  typedef typename OutputType::Pointer OutputPointer;
  typedef typename OutputType::RegionType OutputRegionType; 
  typedef typename OutputType::PixelType OutputPixelType; 
  typedef typename OutputType::SizeType OutputSizeType;
  typedef typename OutputType::IndexType IndexType;
  typedef TCh1 Ch1Type;
  typedef typename Ch1Type::ConstPointer Ch1Pointer;
  typedef typename Ch1Type::RegionType Ch1RegionType; 
  typedef typename Ch1Type::PixelType Ch1PixelType; 
  typedef typename Ch1Type::SizeType Ch1SizeType;
  typedef TCh2 Ch2Type;
  typedef typename Ch2Type::ConstPointer Ch2Pointer;
  typedef typename Ch2Type::RegionType Ch2RegionType; 
  typedef typename Ch2Type::PixelType Ch2PixelType; 
  typedef typename Ch2Type::SizeType Ch2SizeType;
  typedef TDiagram DiagramType;
  typedef typename DiagramType::ConstPointer DiagramPointer;
  typedef typename DiagramType::RegionType DiagramRegionType; 
  typedef typename DiagramType::PixelType DiagramPixelType; 
  typedef typename DiagramType::SizeType DiagramSizeType;
  typedef typename DiagramType::IndexType DiagramIndexType;

  typedef classifyKNNCore  Self;
  typedef ImageToImageFilter<TDiagram,TOutput>  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

 /** Run-time type information (and related methods). */
  itkTypeMacro(classifyKNNCore, ImageToImageFilter);

  void SetCh1( const TCh1 * channel1);
  void SetCh2( const TCh2 * channel2);
  void SetDiagram( const TDiagram * diagram);

struct NodeType {
    DiagramIndexType index_intensity;
    IndexType index_space;
    int clase;
};

  typedef  std::vector<NodeType> ListType;

 void SetPrototipos (const ListType _arg) 
   { 
    //Luego lo volvere a volcar en Lista 0, pero es un mal menor.
     m_Prototipos.insert(m_Prototipos.begin(),_arg.begin(),_arg.end());
       this->Modified(); 
   } 
 
 itkSetMacro(K, unsigned short);

 protected:

 classifyKNNCore();
 virtual ~classifyKNNCore() {};
 
 void GenerateData();
// void PrintSelf(std::ostream& os, Indent indent) const;
 
 private:
 classifyKNNCore(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemented

//unsigned short m_MaxClass;
ListType m_Prototipos;
 unsigned short m_K;
};


}//end namespace itk



#ifndef ITK_MANUAL_INSTANTIATION
#include "classifyKNNCore.cxx"
#endif

#endif

