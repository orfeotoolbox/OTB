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

#include "itkNumericTraits.h"

#include "otbProspectModel.h"
#include <boost/math/special_functions/expint.hpp>
#include <boost/shared_ptr.hpp>
#include "otbMath.h"

//TODO check EPSILON matlab
#define EPSILON 0.0000000000000000000000001

namespace otb
{

/** Constructor */
ProspectModel
::ProspectModel()
{
   this->ProcessObject::SetNumberOfRequiredInputs(1);
   this->ProcessObject::SetNumberOfRequiredOutputs(2);

   SpectralResponseType::Pointer outputRefl = static_cast<SpectralResponseType *>(this->MakeOutput(0).GetPointer());
   this->itk::ProcessObject::SetNthOutput(0, outputRefl.GetPointer());

   SpectralResponseType::Pointer outputTrans = static_cast<SpectralResponseType *>(this->MakeOutput(1).GetPointer());
   this->itk::ProcessObject::SetNthOutput(1, outputTrans.GetPointer());
}

/** Destructor */
ProspectModel
::~ProspectModel()
{}

/** Set Input */
void
ProspectModel
::SetInput(const LeafParametersType * object)
{
   this->itk::ProcessObject::SetNthInput(0, const_cast<LeafParametersType *>(object));
}

/** Get Input */
ProspectModel::LeafParametersType *
ProspectModel
::GetInput()
{
   if(this->GetNumberOfInputs() != 1)
   {
      //exit
      return 0;
   }
   return static_cast<LeafParametersType *>(this->itk::ProcessObject::GetInput(0));
}

/** Make outputs */
ProspectModel::DataObjectPointer
ProspectModel
::MakeOutput(unsigned int)
{
   return static_cast<itk::DataObject *>(SpectralResponseType::New().GetPointer());
}

/** Get Reflectance */
ProspectModel::SpectralResponseType *
ProspectModel
::GetReflectance()
{
   if(this->GetNumberOfOutputs() < 2)
   {
      //exit
      return 0;
   }
   return static_cast<SpectralResponseType *>(this->itk::ProcessObject::GetOutput(0));
}

/** Get Transmittance */
ProspectModel::SpectralResponseType *
ProspectModel
::GetTransmittance()
{
   if(this->GetNumberOfOutputs() < 2)
   {
      //exit
      return 0;
   }
   return static_cast<SpectralResponseType *>(this->itk::ProcessObject::GetOutput(1));
}


/** Set Parameters */
void
ProspectModel
::SetInput(const ParametersType & params)
{
//    m_Parameters = params;
   if(params.Size()!=6) itkExceptionMacro( << "Must have 6 parameters in that order : Cab, Car, CBrown, Cw, Cm, N" );
   LeafParametersType::Pointer leafParams = LeafParametersType::New();
   leafParams->SetCab(params[0]);
   leafParams->SetCar(params[1]);
   leafParams->SetCBrown(params[2]);
   leafParams->SetCw(params[3]);
   leafParams->SetCm(params[4]);
   leafParams->SetN(params[5]);

   this->itk::ProcessObject::SetNthInput(0, leafParams);
}


/** Plant Leaf Reflectance and Transmittance computation from 400nm to 2500 nm*/
void
ProspectModel
::GenerateData()
{

   LeafParametersType::Pointer leafParameters = this->GetInput();
   SpectralResponseType::Pointer outRefl = this->GetReflectance();
   SpectralResponseType::Pointer outTrans = this->GetTransmittance();

   unsigned int alpha=40;
   double lambda, n, k, trans, t12, temp, t21, r12, r21, x, y, ra, ta, r90, t90;
   double delta, beta, va, vb, vbNN, vbNNinv, vainv, s1, s2, s3, RN, TN;
   double N, Cab, Car, CBrown, Cw, Cm;

   N = leafParameters->GetN();
   Cab = leafParameters->GetCab();
   Car = leafParameters->GetCar();
   CBrown = leafParameters->GetCBrown();
   Cw = leafParameters->GetCw();
   Cm = leafParameters->GetCm();

   int nbdata = sizeof(DataSpecP5B) / sizeof(DataSpec);
   for (int i = 0; i < nbdata; ++i)
   {
      lambda = DataSpecP5B[i].lambda;
      n = DataSpecP5B[i].refLeafMatInd;

      k = Cab*DataSpecP5B[i].chlAbsCoef+Car*DataSpecP5B[i].carAbsCoef+CBrown*DataSpecP5B[i].brownAbsCoef+Cw*DataSpecP5B[i].waterAbsCoef;
      k = k + Cm*DataSpecP5B[i].dryAbsCoef;
      k = k / N;
      if(k == itk::NumericTraits<double>::ZeroValue() ) k=EPSILON;

      trans=(1.-k)*exp(-k)+k*k*boost::math::expint(1, k);

      t12 = this->Tav(alpha, n);
      temp = this->Tav(90, n);


      t21 = temp/(n*n);
      r12 = 1.-t12;
      r21 = 1.-t21;
      x = t12/temp;
      y = x*(temp-1)+1-t12;

      ra = r12+(t12*t21*r21*(trans*trans))/(1.-r21*r21*trans*trans);
      ta = (t12*t21*trans)/(1.-r21*r21*trans*trans);
      r90 = (ra-y)/x;
      t90 = ta/x;

      delta = (t90*t90-r90*r90-1.)*(t90*t90-r90*r90-1.) - 4.*r90*r90;
      if(delta < 0) delta = EPSILON;
      else delta=vcl_sqrt(delta);

      beta = (1.+r90*r90-t90*t90-delta)/(2.*r90);
      va=(1.+r90*r90-t90*t90+delta)/(2.*r90);
      if ((beta-r90)<=0)
         vb=vcl_sqrt(beta*(va-r90)/(va*EPSILON));
      else
         vb=vcl_sqrt(beta*(va-r90)/(va*(beta-r90)));

      vbNN = vcl_pow(vb, N-1.);
      vbNNinv = 1./vbNN;
      vainv = 1./va;
      s1=ta*t90*(vbNN-vbNNinv);
      s2=ta*(va-vainv);
      s3=va*vbNN-vainv*vbNNinv-r90*(vbNN-vbNNinv);

      RN=ra+s1/s3;
      TN=s2/s3;


      SpectralResponseType::PairType refl;
      SpectralResponseType::PairType trans;
      refl.first=lambda/1000.0;
      refl.second=RN;
      trans.first=lambda/1000.0;
      trans.second=TN;
      outRefl->GetResponse().push_back(refl);
      outTrans->GetResponse().push_back(trans);
   }
}


double
ProspectModel
::Tav(const int theta, double ref)
{

   double theta_rad = theta*CONST_PI/180;
   double r2, rp, rm, a, k, ds, k2, rm2, res, b1, b2, b;
   double ts, tp1, tp2, tp3, tp4, tp5, tp;

   r2=ref*ref;
   rp=r2+1;
   rm=r2-1;
   a=(ref+1)*(ref+1)/2;
   k=-(r2-1)*(r2-1)/4;
   ds=sin(theta_rad);

   k2=k*k;
   rm2=rm*rm;

   if(theta_rad==0) res=4*ref/((ref+1)*(ref+1));
   else
   {
      if(theta_rad==CONST_PI/2) b1=itk::NumericTraits<double>::ZeroValue();
      else b1=vcl_sqrt((ds*ds-rp/2)*(ds*ds-rp/2)+k);

      b2=ds*ds-rp/2;
      b=b1-b2;
      ts=(k2/(6*vcl_pow(b, 3))+k/b-b/2)-(k2/(6*vcl_pow(a, 3))+k/a-a/2);
      tp1=-2*r2*(b-a)/(rp*rp);
      tp2=-2*r2*rp*log(b/a)/rm2;
      tp3=r2*(1./b-1./a)/2;
      tp4=16*r2*r2*(r2*r2+1)*log((2*rp*b-rm2)/(2*rp*a-rm2))/(vcl_pow(rp, 3)*rm2);
      tp5=16*vcl_pow(r2, 3)*(1./(2*rp*b-rm2)-1./(2*rp*a-rm2))/vcl_pow(rp, 3);
      tp=tp1+tp2+tp3+tp4+tp5;
      res=(ts+tp)/(2*ds*ds);
   }
   return res;


}

void
ProspectModel
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os, indent);

}
} // end namespace otb

