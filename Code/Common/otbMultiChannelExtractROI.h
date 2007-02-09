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
#ifndef __otbMultiChannelExtractROI_h
#define __otbMultiChannelExtractROI_h

#include "otbExtractROIBase.h"
//#include "itkVectorImage.h"
#include "otbVectorImage.h"

#include "itkMacro.h"
#include <vector>

namespace otb
{

/** \class MultiChannelExtractROI
 * \brief Extrait une partie d'une image d'une image multi-canal.
 *
 * Il est possible d'extraire tous les canaux de l'image ou seulement ceux pr�cis�s par l'utilisateur.
 * La m�thode SetChannel permet de s�lectionner un canal (une liste est incr�ment�e).
 * Les m�thodes SetFirstChannel/SetLastChannel permettent de d�finir une liste de canaux, d�finit par cet intervalle.
 * \note Si aucun canal n'est sp�cifi� par l'utilisateur, alors tous les canaux de l'image d'entr�e sont trait�s
 * \note Ces classe est param�tr�e par le type de pixel des images d'entr�e et de sortie. Les images manipul�es dans cette classe 
 * sont de type "itk::VectorImage".
 *
 */
template <class TInputPixelType, class TOutputPixelType>
class ITK_EXPORT MultiChannelExtractROI:
//    public ExtractROIBase< itk::VectorImage<TInputPixelType,2> , itk::VectorImage<TOutputPixelType,2> >
    public ExtractROIBase< VectorImage<TInputPixelType,2> , VectorImage<TOutputPixelType,2> >
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelExtractROI                Self;
//  typedef ExtractROIBase< itk::VectorImage<TInputPixelType,2> , itk::VectorImage<TOutputPixelType,2> > Superclass;
  typedef ExtractROIBase< VectorImage<TInputPixelType,2> , VectorImage<TOutputPixelType,2> > Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef typename Superclass::InputImageType   InputImageType;
  typedef typename Superclass::OutputImageType  OutputImageType;

  /** Pixel type information */
  typedef typename OutputImageType::ValueType   OutputValueType;

  /** Typedef to describe the output and input image region types. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::RegionType InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename InputImageType::PixelType InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename InputImageType::IndexType InputImageIndexType;
  typedef typename OutputImageType::SizeType OutputImageSizeType;
  typedef typename InputImageType::SizeType InputImageSizeType;

  itkSetMacro(FirstChannel,unsigned int);
  itkGetConstMacro(FirstChannel,unsigned int);
  itkSetMacro(LastChannel,unsigned int);
  itkGetConstMacro(LastChannel,unsigned int);

  /** ExtractImageFilter can produce an image which is a different
   * resolution than its input image.  As such, ExtractImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** Typedef Liste des canaux */
  typedef typename std::vector<unsigned int> ChannelsType;

  /** Renvoie la liste des canaux trait�s */
  ChannelsType GetChannels()const
  {
        return (m_Channels);
  }
  /** Renvoie le nombre de canaux trait�s */
  unsigned int GetNbChannels()const
  {
        return (m_Channels.size());
  }
  
  /** Selectionne un canal a traiter */
  void SetChannel(unsigned int channel);
  /** Annule la selection des canaux */
  void ClearChannels(void);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

protected:
  MultiChannelExtractROI();
  ~MultiChannelExtractROI() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ExtractImageFilter peut etre implementee comme un filtre multithreaded.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  MultiChannelExtractROI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  /** Premier/Dernier canal � traiter [1...] */
  unsigned int  m_FirstChannel;
  unsigned int  m_LastChannel;
  /** Liste des canaux � traiter  [1...] */
  ChannelsType  m_Channels; 
  /** Liste des canaux qui seront r�ellement trait�s [1...] */
  ChannelsType  m_ChannelsWorks;
  /** */
  bool m_ChannelsWorksBool;
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiChannelExtractROI.txx"
#endif
  
#endif
