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
#ifndef __otbProSail_txx
#define __otbProSail_txx

#include "itkNumericTraits.h"

#include "otbProSail.h"
#include <boost/math/special_functions/expint.hpp>
#include <boost/shared_ptr.hpp>
#include "otbMath.h"

//TODO check EPSILON matlab
#define EPSILON 0.000000000000000000000000000000000000000000001 

namespace otb
{

template <class TSpectralResponse>
ProSail<TSpectralResponse>
::ProSail()
{
   m_ProSailParameters = ProSailParametersType::New();
   m_Resh = SpectralResponseType::New();
   m_Resv = SpectralResponseType::New();
}

template <class TSpectralResponse>
void
ProSail<TSpectralResponse>
::Compute()
{
   // LEAF ANGLE DISTRIBUTION
   double rd = CONST_PI/180;
   VectorType lidf;
   this->Calc_LIDF(m_ProSailParameters->GetAngl(), lidf);

   double cts, cto, ctscto, tants, tanto, cospsi, dso;
   cts = vcl_cos(rd*m_ProSailParameters->GetTTS());
   cto = vcl_cos(rd*m_ProSailParameters->GetTTO());
   ctscto = cts*cto;
   tants = vcl_tan(rd*m_ProSailParameters->GetTTS());
   tanto = vcl_tan(rd*m_ProSailParameters->GetTTO());
   cospsi = vcl_cos(rd*m_ProSailParameters->GetPSI());
   dso = vcl_sqrt(tants*tants+tanto*tanto-2.*tants*tanto*cospsi);

   // angular distance, compensation of shadow length
   // Calculate geometric factors associated with extinction and scattering 
   // Initialise sums
   double ks = 0;
   double ko = 0;
   double bf = 0;
   double sob = 0;
   double sof = 0;
   double ttl,ctl,ksli,koli,sobli,sofli,bfli;
   double chi_s, chi_o, frho, ftau;
   VectorType result(4);
   
   // Weighted sums over LIDF
   for(unsigned int i=0;i<lidf.size();i++)
   {
      ttl = 2.5+5*i;	// leaf inclination discrete values
      ctl = vcl_cos(rd*ttl);
      // SAIL volume scattering phase function gives interception and portions to be 
      // multiplied by rho and tau
      
      this->Volscatt(m_ProSailParameters->GetTTS(),m_ProSailParameters->GetTTO(),m_ProSailParameters->GetPSI(),ttl,result);
      chi_s = result[0];
      chi_o = result[1];
      frho = result[2];
      ftau = result[3];

      //********************************************************************************
      //*                   SUITS SYSTEM COEFFICIENTS 
      //*
      //*	ks  : Extinction coefficient for direct solar flux
      //*	ko  : Extinction coefficient for direct observed flux
      //*	att : Attenuation coefficient for diffuse flux
      //*	sigb : Backscattering coefficient of the diffuse downward flux
      //*	sigf : Forwardscattering coefficient of the diffuse upward flux
      //*	sf  : Scattering coefficient of the direct solar flux for downward diffuse flux
      //*	sb  : Scattering coefficient of the direct solar flux for upward diffuse flux
      //*	vf   : Scattering coefficient of upward diffuse flux in the observed direction
      //*	vb   : Scattering coefficient of downward diffuse flux in the observed direction
      //*	w   : Bidirectional scattering coefficient
      //********************************************************************************
   
      // Extinction coefficients
      ksli = chi_s/cts;
      koli = chi_o/cto;
   
      // Area scattering coefficient fractions
      sobli	= frho*CONST_PI/ctscto;
      sofli	= ftau*CONST_PI/ctscto;
      bfli	= ctl*ctl;
      ks	= ks+ksli*lidf[i];
      ko	= ko+koli*lidf[i];
      bf	= bf+bfli*lidf[i];
      sob	= sob+sobli*lidf[i];
      sof	= sof+sofli*lidf[i];
   }
   
   // Geometric factors to be used later with rho and tau
   double sdb,sdf,dob,dof,ddb,ddf;
   sdb	= 0.5*(ks+bf);
   sdf	= 0.5*(ks-bf);
   dob	= 0.5*(ko+bf);
   dof	= 0.5*(ko-bf);
   ddb	= 0.5*(1.+bf);
   ddf	= 0.5*(1.-bf);

   double lambda,Es,Ed,Rsoil1,Rsoil2,rsoil0,rho,tau,PARdiro,PARdifo;
   double sigb,sigf,att,m2,m,sb,sf,vb,vf,w;
   double tss,too,tsstoo,rdd,tdd,rsd,tsd,rdo,tdo,rso,rsos,rsod;
   double rddt, rsdt,rdot,rsodt,rsost,rsot,dn;
   double e1,e2,rinf,rinf2,re,denom,J1ks,J2ks,J1ko,J2ko;
   double Ps,Qs,Pv,Qv,z,g1,g2,Tv1,Tv2,T1,T2,T3;
   double alf,sumint,fhot,x1,y1,f1,fint,x2,y2,f2;
   LRTType LRT(3);
   double resh,resv;
   

   this->Prospect5B(LRT);

   int nbdata = sizeof(dataSpec_P5B) / sizeof(dataSpec);
   for (int i = 0 ; i < nbdata ; i++)
   {
      lambda = dataSpec_P5B[i].lambda;
      Es = dataSpec_P5B[i].directLight; //8
      Ed = dataSpec_P5B[i].diffuseLight; //9
      Rsoil1 = dataSpec_P5B[i].drySoil; //10
      Rsoil2 = dataSpec_P5B[i].wetSoil; //11
      rho = LRT[1][i];
      tau = LRT[2][i];
      
      // direct/diffuse light
      //Es = direct
      //Ed = diffuse
      PARdiro = (1-m_ProSailParameters->GetSkyl()/100.)*Es;
      PARdifo = (m_ProSailParameters->GetSkyl()/100.)*Ed;
      
      // Soil Reflectance Properties
      //rsoil1 = dry soil
      //rsoil2 = wet soil
      rsoil0 = m_ProSailParameters->GetPSoil()*Rsoil1+(1-m_ProSailParameters->GetPSoil())*Rsoil2;
      
      // Here rho and tau come in
      sigb = ddb*rho+ddf*tau;
      sigf = ddf*rho+ddb*tau;
      att = 1.-sigf;
      m2 = (att+sigb)*(att-sigb);
      if(m2<=0) m2 = 0;
      m = vcl_sqrt(m2);

   
      sb = sdb*rho+sdf*tau;
      sf = sdf*rho+sdb*tau;
      vb = dob*rho+dof*tau;
      vf = dof*rho+dob*tau;
      w = sob*rho+sof*tau;
   
      // Here the LAI comes in
      // Outputs for the case LAI = 0
      if (m_ProSailParameters->GetLAI()<0)
      {
         tss = 1;
         too = 1;
         tsstoo = 1;
         rdd = 0;
         tdd = 1;
         rsd = 0;
         tsd = 0;
         rdo = 0;
         tdo = 0;
         rso = 0;
         rsos = 0;
         rsod = 0;
   
         rddt = rsoil0;
         rsdt = rsoil0;
         rdot = rsoil0;
         rsodt = 0;
         rsost = rsoil0;
         rsot = rsoil0;
      }
      
      // Other cases (LAI > 0)
      e1 = exp(-m*m_ProSailParameters->GetLAI());
      e2 = e1*e1;
      rinf = (att-m)/sigb;
      rinf2 = rinf*rinf;
      re = rinf*e1;
      denom = 1.-rinf2*e2;

      J1ks=Jfunc1(ks,m,m_ProSailParameters->GetLAI());
      J2ks=Jfunc2(ks,m,m_ProSailParameters->GetLAI());
      J1ko=Jfunc1(ko,m,m_ProSailParameters->GetLAI());
      J2ko=Jfunc2(ko,m,m_ProSailParameters->GetLAI());
      
      Ps = (sf+sb*rinf)*J1ks;
      Qs = (sf*rinf+sb)*J2ks;
      Pv = (vf+vb*rinf)*J1ko;
      Qv = (vf*rinf+vb)*J2ko;
      
      rdd = rinf*(1.-e2)/denom;
      tdd = (1.-rinf2)*e1/denom;
      tsd = (Ps-re*Qs)/denom;
      rsd = (Qs-re*Ps)/denom;
      tdo = (Pv-re*Qv)/denom;
      rdo = (Qv-re*Pv)/denom;
      
      tss = exp(-ks*m_ProSailParameters->GetLAI());
      too = exp(-ko*m_ProSailParameters->GetLAI());
      z = Jfunc3(ks,ko,m_ProSailParameters->GetLAI());
      g1 = (z-J1ks*too)/(ko+m);
      g2 = (z-J1ko*tss)/(ks+m);
      
      Tv1 = (vf*rinf+vb)*g1;
      Tv2 = (vf+vb*rinf)*g2;
      T1 = Tv1*(sf+sb*rinf);
      T2 = Tv2*(sf*rinf+sb);
      T3 = (rdo*Qs+tdo*Ps)*rinf;

      // Multiple scattering contribution to bidirectional canopy reflectance
      rsod = (T1+T2-T3)/(1.-rinf2);
      
      // Treatment of the hotspot-effect
      alf=1e6;
      // Apply correction 2/(K+k) suggested by F.-M. Br�on
      if (m_ProSailParameters->GetHSpot()>0) alf=(dso/m_ProSailParameters->GetHSpot())*2./(ks+ko);
      if (alf>200) alf=200;
      if (alf==0)
      {
         // The pure hotspot - no shadow
         tsstoo = tss;
         sumint = (1-tss)/(ks*m_ProSailParameters->GetLAI());
      }
      else
      {
         // Outside the hotspot
         fhot=m_ProSailParameters->GetLAI()*vcl_sqrt(ko*ks);
         // Integrate by exponential Simpson method in 20 steps
         // the steps are arranged according to equal partitioning
         // of the slope of the joint probability function
         x1=0;
         y1=0;
         f1=1;
         fint=(1.-exp(-alf))*0.05;
         sumint=0;
         
         for(unsigned int j=1;j<=20;j++)
         {
            if (j<20) x2 = -vcl_log(1.-j*fint)/alf;
            else x2 = 1;
            y2 = -(ko+ks)*m_ProSailParameters->GetLAI()*x2+fhot*(1.-exp(-alf*x2))/alf;
            f2 = exp(y2);
            sumint = sumint+(f2-f1)*(x2-x1)/(y2-y1);
            x1=x2;
            y1=y2;
            f1=f2;
         }
         tsstoo=f1;
      }

      // Bidirectional reflectance
      // Single scattering contribution
      rsos = w*m_ProSailParameters->GetLAI()*sumint;
      // Total canopy contribution
      rso=rsos+rsod;
      //Interaction with the soil
      dn=1.-rsoil0*rdd;
      
      rddt = rdd+tdd*rsoil0*tdd/dn;
      rsdt = rsd+(tsd+tss)*rsoil0*tdd/dn;
      rdot = rdo+tdd*rsoil0*(tdo+too)/dn;
      
      rsodt = rsod+((tss+tsd)*tdo+(tsd+tss*rsoil0*rdd)*too)*rsoil0/dn;
      rsost = rsos+tsstoo*rsoil0;
      rsot = rsost+rsodt;
      
      resh = (rddt*PARdifo+rsdt*PARdiro)/(PARdiro+PARdifo);
      resv = (rdot*PARdifo+rsot*PARdiro)/(PARdiro+PARdifo);
      
      PairType tmp1,tmp2;
      tmp1.first=lambda;
      tmp1.second=static_cast<ValuePrecisionType>(resh);
      tmp2.first=lambda;
      tmp2.second=static_cast<ValuePrecisionType>(resv);
      
      m_Resh->GetResponse().push_back(tmp1);
      m_Resv->GetResponse().push_back(tmp2);

   }

}




/** Plant Leaf Reflectance and Transmittance computation from 400nm to 2500 nm*/
template <class TSpectralResponse>
void
ProSail<TSpectralResponse>
::Prospect5B( LRTType &LRT )
{

   unsigned int alpha=40;
   double lambda, n, k, trans, t12, temp, t21, r12, r21, x, y, ra, ta, r90, t90;
   double delta, beta, va, vb, vbNN, vbNNinv, vainv, s1, s2, s3, RN, TN;
   double N, Cab, Car, CBrown, Cw, Cm;
   N = m_ProSailParameters->GetN();
   Cab = m_ProSailParameters->GetCab();
   Car = m_ProSailParameters->GetCar();
   CBrown = m_ProSailParameters->GetCBrown();
   Cw = m_ProSailParameters->GetCw();
   Cm = m_ProSailParameters->GetCm();

   
   int nbdata = sizeof(dataSpec_P5B) / sizeof(dataSpec);
   for (int i = 0 ; i < nbdata ; i++)
   {
      lambda = dataSpec_P5B[i].lambda;
      n = dataSpec_P5B[i].refLeafMatInd;
      
      k = Cab*dataSpec_P5B[i].chlAbsCoef+Car*dataSpec_P5B[i].carAbsCoef+CBrown*dataSpec_P5B[i].brownAbsCoef+Cw*dataSpec_P5B[i].waterAbsCoef;
      k = k + Cm*dataSpec_P5B[i].dryAbsCoef;
      k = k / N;
      if(k == itk::NumericTraits<PrecisionType>::ZeroValue() ) k=EPSILON;

      trans=(1.-k)*exp(-k)+k*k*boost::math::expint(1,k);
      
      t12 = this->Tav(alpha,n);
      temp = this->Tav(90,n);
      
      
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
   
      vbNN = vcl_pow(vb,N-1.);
      vbNNinv = 1./vbNN;
      vainv = 1./va;
      s1=ta*t90*(vbNN-vbNNinv);
      s2=ta*(va-vainv);
      s3=va*vbNN-vainv*vbNNinv-r90*(vbNN-vbNNinv);

      RN=ra+s1/s3;
      TN=s2/s3;
      
      LRT[0].push_back(lambda);
      LRT[1].push_back(RN);
      LRT[2].push_back(TN);
   }
   
}


template <class TSpectralResponse>
double
ProSail<TSpectralResponse>
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
      if(theta_rad==CONST_PI/2) b1=itk::NumericTraits<PrecisionType>::ZeroValue();
      else b1=vcl_sqrt((ds*ds-rp/2)*(ds*ds-rp/2)+k);

      b2=ds*ds-rp/2;
      b=b1-b2;
      ts=(k2/(6*vcl_pow(b,3))+k/b-b/2)-(k2/(6*vcl_pow(a,3))+k/a-a/2);
      tp1=-2*r2*(b-a)/(rp*rp);
      tp2=-2*r2*rp*log(b/a)/rm2;
      tp3=r2*(1./b-1./a)/2;
      tp4=16*r2*r2*(r2*r2+1)*log((2*rp*b-rm2)/(2*rp*a-rm2))/(vcl_pow(rp,3)*rm2);
      tp5=16*vcl_pow(r2,3)*(1./(2*rp*b-rm2)-1./(2*rp*a-rm2))/vcl_pow(rp,3);
      tp=tp1+tp2+tp3+tp4+tp5;
      res=(ts+tp)/(2*ds*ds);
   }
   return res;
   
   
}

template <class TSpectralResponse>
void
ProSail<TSpectralResponse>
::Calc_LIDF(const double a, VectorType &lidf)
{
   int ala=a;
   VectorType freq;
   Campbell(ala,freq);
   lidf=freq;

}


template <class TSpectralResponse>
void
ProSail<TSpectralResponse>
::Campbell(const double ala, VectorType &freq)
{
   unsigned int n=18;
   double excent = exp(-1.6184e-5*vcl_pow(ala,3)+2.1145e-3*ala*ala-1.2390e-1*ala+3.2491);
   double sum=0;
   unsigned int tx2, tx1;
   double tl1, tl2, x1, x2, v, alpha, alpha2, x12, x22, alpx1, alpx2, dum, almx1, almx2;
   VectorType temp;

   for(unsigned int i=0;i<n;i++)
   {
      tx2 = 5*i;
      tx1 = 5*(i+1);
      tl1 = tx1*CONST_PI/180;
      tl2 = tx2*CONST_PI/180;


      x1 = excent/sqrt(1.+excent*excent*vcl_tan(tl1)*vcl_tan(tl1));
      x2 = excent/sqrt(1.+excent*excent*vcl_tan(tl2)*vcl_tan(tl2));
      if(excent==1)
      {
         v = abs(cos(tl1)-cos(tl2));
         temp.push_back( v );
         sum = sum + v;
      }
      else
      {
         alpha = excent/vcl_sqrt(vcl_abs(1.-excent*excent));
         alpha2 = alpha*alpha;
         x12 = x1*x1;
         x22 = x2*x2;
         if(excent>1)
         {
            alpx1 = vcl_sqrt(alpha2+x12);
            alpx2 = vcl_sqrt(alpha2+x22);
            dum   = x1*alpx1+alpha2*log(x1+alpx1);
            v = vcl_abs(dum-(x2*alpx2+alpha2*log(x2+alpx2)));
            temp.push_back( v );
            sum = sum + v;
         }
         else
         {
            almx1 = sqrt(alpha2-x12);
            almx2 = sqrt(alpha2-x22);
            dum   = x1*almx1+alpha2*asin(x1/alpha);
            v = vcl_abs(dum-(x2*almx2+alpha2*asin(x2/alpha)));
            temp.push_back( v );
            sum = sum + v;
         }
      }
   }

   for(unsigned int i=0;i<n;i++)
   {
      freq.push_back(temp[i]/sum);
   }

}


template <class TSpectralResponse>
void
ProSail<TSpectralResponse>
::Volscatt(const double tts, const double tto, const double psi, const double ttl, VectorType &result)
{

   double rd = CONST_PI/180;
   double costs = vcl_cos(rd*tts);
   double costo = vcl_cos(rd*tto);
   double sints = vcl_sin(rd*tts);
   double sinto = vcl_sin(rd*tto);
   double cospsi = vcl_cos(rd*psi);
   double psir = rd*psi;
   double costl = vcl_cos(rd*ttl);
   double sintl = vcl_sin(rd*ttl);
   double cs = costl*costs;
   double co = costl*costo;
   double ss = sintl*sints;
   double so = sintl*sinto;
   
   // ..............................................................................
   //     betas -bts- and betao -bto- computation
   //     Transition angles (beta) for solar (betas) and view (betao) directions
   //     if thetav+thetal>pi/2, bottom side of the leaves is observed for leaf azimut 
   //     interval betao+phi<leaf azimut<2pi-betao+phi.
   //     if thetav+thetal<pi/2, top side of the leaves is always observed, betao=pi
   //     same consideration for solar direction to compute betas
   // ..............................................................................
   double cosbts, cosbto, bts,ds,chi_s, bto, doo, chi_o;
   double btran1, btran2, bt1, bt2, bt3, t1, t2 , denom, frho, ftau;
   
   cosbts = 5;
   if (vcl_abs(ss)>1e-6) cosbts = -cs/ss;
   
   cosbto=5;
   if (vcl_abs(so)>1e-6) cosbto = -co/so;
   
   
   if (vcl_abs(cosbts)<1)
   {
      bts = vcl_acos(cosbts);
      ds = ss;
   }
   else
   {
      bts = CONST_PI;
      ds = cs;
   }

   chi_s = 2./CONST_PI*((bts-CONST_PI*0.5)*cs+vcl_sin(bts)*ss);
   
   if (vcl_abs(cosbto)<1)
   {
      bto = vcl_acos(cosbto);
      doo = so;
   }
   else if(tto<90)
   {
      bto = CONST_PI;
      doo = co;
   }
   else
   {
      bto = 0;
      doo = -co;
   }
   chi_o = 2./CONST_PI*((bto-CONST_PI*0.5)*co+vcl_sin(bto)*so);
   
   // ..............................................................................
   //   Computation of auxiliary azimut angles bt1, bt2, bt3 used
   //   for the computation of the bidirectional scattering coefficient w
   // .............................................................................
   
   btran1 = vcl_abs(bts-bto);
   btran2 = CONST_PI - vcl_abs(bts+bto-CONST_PI);
   
   if (psir<=btran1)
   {
      bt1=psir;
      bt2=btran1;
      bt3=btran2;
   }
   else
   {
      bt1=btran1;
      if (psir<=btran2)
      {
         bt2=psir;
         bt3=btran2;
      }
      else
      {
         bt2=btran2;
         bt3=psir;
      }
   }
   
   t1 = 2.*cs*co+ss*so*cospsi;
   t2 = 0;
   if (bt2>0) t2=sin(bt2)*(2.*ds*doo+ss*so*cos(bt1)*cos(bt3));
   denom = 2.*CONST_PI*CONST_PI;
   frho = ((CONST_PI-bt2)*t1+t2)/denom;
   ftau = (-bt2*t1+t2)/denom;
   
   if (frho<0) frho = 0;
   if (ftau<0) ftau = 0;

   result[0] = chi_s;
   result[1] = chi_o;
   result[2] = frho;
   result[3] = ftau;
//    result.push_back(chi_s);
//    result.push_back(chi_o);
//    result.push_back(frho);
//    result.push_back(ftau);
}



template <class TSpectralResponse>
double
ProSail<TSpectralResponse>
::Jfunc1(const double k, const double l, const double t)
{
// function Jout=Jfunc1(k,l,t)
// %	J1 function with avoidance of singularity problem
// del=(k-l)*t;
// Jout(abs(del)>1e-3)=(exp(-l(abs(del)>1e-3)*t)-exp(-k*t))./(k-l(abs(del)>1e-3));
// Jout(abs(del)<=1e-3)=0.5*t*(exp(-k*t)+exp(-l(abs(del)<=1e-3)*t)).*(1-del(abs(del)<=1e-3).*del(abs(del)<=1e-3)/12);
// Jout=transpose(Jout);

   
   //J1 function with avoidance of singularity problem
   double v;
   double del=(k-l)*t;
   if(vcl_abs(del)>1e-3)
   {
      v = (exp(-l*t)-exp(-k*t))/(k-l);
      return v;
   }
   else
   {
      v = 0.5*t*(exp(-k*t)+exp(-l*t))*(1.-del*del/12.);
      return v;
   }
}

template <class TSpectralResponse>
double
ProSail<TSpectralResponse>
::Jfunc2(const double k, const double l, const double t)
{
// function Jout=Jfunc2(k,l,t)
// %	J2 function
// Jout=(1.-exp(-(k+l)*t))./(k+l);
   double v;
   v = (1.-exp(-(k+l)*t))/(k+l);
   return v;
}

template <class TSpectralResponse>
double
ProSail<TSpectralResponse>
::Jfunc3(const double k, const double l, const double t)
{
// function out=Jfunc3(k,l,t)
// out=(1.-exp(-(k+l)*t))/(k+l);
   double v;
   v =  (1.-exp(-(k+l)*t))/(k+l);
   return v;
}


template <class TSpectralResponse>
void
ProSail<TSpectralResponse>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os,indent);
   std::cout<<m_ProSailParameters<<std::endl;

}
} // end namespace otb

#endif
