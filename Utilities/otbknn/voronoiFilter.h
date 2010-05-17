/*=========================================
 Copyright (c) Ruben Cardenes Almeida. August 2006
 This work is licensed under a
 Creative Commons Atribution 2.5 License
 http://creativecommons.org/licenses/by/2.5
 ==========================================*/

#ifndef __voronoiFilter_h
#define __voronoiFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"

//
//TMap must accept negative pixel values

namespace itk
{


template <class TDomain, typename MapElement, unsigned int m_ImageDimension>
class ITK_EXPORT voronoiFilter :
    public ImageToImageFilter< TDomain, Image<std::vector<MapElement> ,m_ImageDimension> > 
{
 
public:

  /** Some convenient typedefs. */
  typedef TDomain DomainType;
  typedef typename DomainType::ConstPointer DomainPointer;
  typedef typename DomainType::RegionType DomainRegionType; 
  typedef typename DomainType::PixelType DomainPixelType; 
  typedef typename DomainType::SizeType DomainSizeType;
  typedef Image<std::vector<MapElement> ,m_ImageDimension> MapType;
  typedef typename MapType::Pointer MapPointer;
  typedef typename MapType::RegionType MapRegionType;
  typedef typename MapType::PixelType MapPixelType;


  /** Standard class typedefs. */
  typedef voronoiFilter  Self;
  typedef ImageToImageFilter<TDomain,MapType>  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;


  typedef typename DomainType::IndexType IndexType;

//  typedef LevelSetTypeDefault<TDomain> LevelSetType;
//  itkStaticConstMacro(SetDimension, unsigned int, LevelSetType::SetDimension);
  
//It is more common in ITK to use LevelSetNode, but it is more memory consuming and it is not justified
struct NodeType {
    IndexType index;
    int l;
};

  typedef std::vector<NodeType> ListType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

 /** Run-time type information (and related methods). */
  itkTypeMacro(voronoiFilter, ImageToImageFilter);


  /** Connect the domain */
  void SetInput( const TDomain * domain1);
  itkSetMacro(BackgroundValue, int);  
  itkSetMacro(K, unsigned short);  
  
  /** Connect Prototypes */

 protected:

 voronoiFilter();
 virtual ~voronoiFilter() {};
 
 void GenerateData();
 void PrintSelf(std::ostream& os, Indent indent) const;
 

 float distance(IndexType mapIndex1 ,IndexType mapIndex2); 
 
 void indexConversorInit(IndexType& indexConversor,DomainSizeType size);

 unsigned int  pasarALineal(IndexType index);

 private:
 voronoiFilter(const Self&); //purposely not implemented
 void operator=(const Self&); //purposely not implemented

  int m_BackgroundValue;
  IndexType m_indexConversor;
  unsigned short m_K;

struct element {
  //float DObj[K];
  //IndexType PosObj[K];
  std::vector<float> DObj;
  std::vector<IndexType> PosObj;
  int icur;
  int dcur;

};



};

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "voronoiFilter.cxx"
#endif

#endif
