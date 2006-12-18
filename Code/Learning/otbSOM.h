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
 * \brief This class is responsible for the learning of a self organizing map from a 
 * set of vector represented by the input image (each vector is a pixel of the image).
 *
 * The learning process iteratively select the best-response neuron for each input vector,
 * enhancing its response and the response of its neighbors with respect to a certain radius, 
 * computed from an initial radius, and to a certain learning factor, decreasing at each iteration.
 *
 * The SOMMap produced as output can be either initialized with a constant custom value or randomly 
 * generated following a normal law. The seed for the random intialization can be modified.
 *
 * \sa SOMMap
 * \sa SOMActivationBuilder
 */
template <class TListSample,class TMap>  
class ITK_EXPORT SOM  
: public itk::ImageSource<TMap>
{
  public:
  /** Standard typedefs */
  typedef SOM                           Self;
  typedef itk::ImageSource<TMap>        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime informations macro */
  itkTypeMacro(SOM,ImageSource);

  typedef TListSample                      ListSampleType;
  typedef typename ListSampleType::Pointer ListSamplePointerType;
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
  itkSetMacro(Seed,unsigned int);
  itkGetMacro(Seed,unsigned int);
  itkGetObjectMacro(ListSample,ListSampleType);
  itkSetObjectMacro(ListSample,ListSampleType);

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
  /** Seed for random initialisation */
  unsigned int m_Seed;
  /** The input list sample */
  ListSamplePointerType m_ListSample;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOM.txx"
#endif

#endif
