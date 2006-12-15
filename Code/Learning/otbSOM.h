/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbSOM_h
#define _otbSOM_h

#include "itkImageToImageFilter.h"
#include "itkEuclideanDistance.h"

namespace otb
{
/**
 * \class SOM
 * \brief
 */
template <class TInputImage,class TMap>  
class ITK_EXPORT SOM  
: public itk::ImageToImageFilter<TInputImage,TMap>
{
  public:
  /** Standard typedefs */
  typedef SOM                           Self;
  typedef itk::ImageToImageFilter<TInputImage,TMap> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime informations macro */
  itkTypeMacro(SOM,ImageToImageFilter);

  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef TMap  MapType;
  typedef typename MapType::PixelType NeuronType;
  typedef typename NeuronType::ValueType ValueType;
  typedef typename MapType::IndexType IndexType;
  typedef typename MapType::SizeType  SizeType;
  typedef typename MapType::RegionType RegionType;
  typedef typename MapType::Pointer MapPointerType;
  
  /** Map dimension */
  itkStaticConstMacro(MapDimension,unsigned int, MapType::ImageDimension);
  
  /** Accessors */
  itkSetMacro(NumberOfIterations,unsigned int);
  itkGetMacro(NumberOfIterations,unsigned int);
  itkSetMacro(BetaInit,double);
  itkGetMacro(BetaInit,double);
  itkSetMacro(BetaEnd,double);
  itkGetMacro(BetaEnd,double);
  itkSetMacro(MinWeight,ValueType);
  itkGetMacro(MinWeight,ValueType);
  itkSetMacro(MaxWeight,ValueType);
  itkGetMacro(MaxWeight,ValueType);
  itkSetMacro(MapSize,SizeType);
  itkGetMacro(MapSize,SizeType);
  itkSetMacro(NeighborhoodSizeInit,SizeType);
  itkGetMacro(NeighborhoodSizeInit,SizeType);
  itkSetMacro(RandomInit,bool);
  itkGetMacro(RandomInit,bool);

  protected:
  /** Constructor */
  SOM();
  /** Destructor */
  virtual ~SOM();
  /** Main computation method */
  void GenerateData(void);
  /**
   * Update the output map with a new sample.
   * \param sample The new sample to learn,
   * \param beta The learning coefficient,
   * \param radius The radius of the nieghbourhood.
   */
  void UpdateMap(const NeuronType& sample, double beta, SizeType& radius);
  /**
   * Step one iteration.
   */
  void Step(unsigned int currentIteration);
  /**
   * Get The index of the winning neuron for a sample.
   * \param the sample.
   * \return The index of the winning neuron.
   */
  IndexType GetWinner(const NeuronType& sample);
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  private:
  SOM(const Self&); // purposely not implemented 
  void operator=(const Self&); // purposely not implemented 
  /** Size of the neurons map */
  SizeType m_MapSize;
  /** Number of iterations */
  unsigned int m_NumberOfIterations;
  /** Initial learning coefficient */
  double m_BetaInit;
  /** Final learning coefficient */
  double m_BetaEnd;
  /** Initial neighborhood size */
  SizeType m_NeighborhoodSizeInit;
  /** Minimum intial neuron weights */
  ValueType m_MinWeight;
  /** Maximum intial neuron weights */
  ValueType m_MaxWeight;
  /** Random initialisation bool */
  bool m_RandomInit;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOM.txx"
#endif

#endif
