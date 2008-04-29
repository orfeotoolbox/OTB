
#ifndef _otbMRFEnergyEdgeFidelity_h
#define _otbMRFEnergyEdgeFidelity_h

#include "otbMRFEnergy.h"
#define M_SQUARE(x) ((x)*(x))

namespace otb
{
  /**
   * \class MRFEnergyEdgeFidelity
   * \brief This is the implementation of an edge preserving model for Markov denoising.
   *
   * This is the implementation of an edge fidelity model for Markov denoising, to be used for
   * regularization. Energy is:
   * \f[  \sum_{t \in \mathcal{V}_s} U(x_s,x_t) = \Phi(x_s-x_t) \f]
   * with
   * - \f$ x_s \f$ the label on site s
   * - \f$ x_t \f$ the label on site t, a neighbor of s
             * - \f$ \Phi \f$ an edge preserving function: 
  \f[ \Phi(u) = \frac{u^2}{1+u^2} \f]
   */
  
  template< class TInput1, class TInput2>    
      class ITK_EXPORT MRFEnergyEdgeFidelity : public MRFEnergy< TInput1, TInput2>
      {
        public:
          typedef MRFEnergyEdgeFidelity Self;
          typedef itk::Object Superclass;
          typedef itk::SmartPointer<Self>  Pointer;
          typedef itk::SmartPointer<const Self>  ConstPointer;
    
          typedef itk::ConstNeighborhoodIterator< TInput1 >  NeighborhoodIterator;
          typedef typename TInput2::PixelType LabelledImagePixelType;
    
          itkNewMacro(Self);
        
//         itkSetMacro(Beta, double);
          itkTypeMacro(MRFEnergy,Object);
     
          double GetSingleValue(const InputImagePixelType & value1,  const LabelledImagePixelType & value2)
          {
            return M_SQUARE((value1 - value2))/double(1+M_SQUARE((value1 - value2)));
          }

    
        protected:
      // The constructor and destructor.
          MRFEnergyEdgeFidelity() {};
          virtual ~MRFEnergyEdgeFidelity() {};
      };
}

#endif
