      Subroutine RAHMALBE (rho0,af,xk,brdfalb)
      parameter (nta=24,nfa=48)
C
C see RAHMBRDF.f for description
C
      integer j,k,l
      real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa)
      real xk,af,rho0
      real brdfalb,summ,si1,si2,pond
      real coef1,coef2,cospha,geofac
      real fi,mu1,mu2,phaang,phafun,pi,tante1,tante2
C
      pi =acos(-1.0)
      teta1=0.
      teta2=pi/2.
      call gauss(teta1,teta2,ta,wta,nta)
      phi1=0.
      phi2=2.*pi
      call gauss(phi1,phi2,fa,wfa,nfa)
      brdfalb=0.
      summ=0.
      do 1 k=1,nfa
      do 2 j=1,nta
      do 3 l=1,nta
      mu2=cos(ta(j))
      mu1=cos(ta(l))
      si2=sin(ta(j))
      si1=sin(ta(l))
      fi=fa(k)
C Compute various trigonometric expressions:
      cospha=mu1*mu2+sqrt(1.-mu1*mu1)*sqrt(1.-mu2*mu2)*cos(fi)
      phaang=acos(cospha)
      tante1=sqrt(1.-mu1*mu1)/mu1
      tante2=sqrt(1.-mu2*mu2)/mu2
      geofac=sqrt(tante1*tante1+tante2*tante2-2.0*tante1*tante2*cos(fi))
C Compute the first term
      coef1=(mu1**(xk-1.))*(mu2**(xk-1.))/((mu1+mu2)**(1.-xk))
C Compute the phase function:
      phafun=(1.0-af*af)/((1.0+af*af-2.0*af*cos(pi-phaang))**1.5)
C Compute the opposition (hot spot) function:
      coef2=1.+(1.-rho0)/(1.+geofac)
C Compute the bidirectional reflectance factor:
      pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l)
      brdfalb=brdfalb+rho0*coef1*phafun*coef2*pond
      summ=summ+pond
  3   continue
  2   continue
  1   continue
      brdfalb=brdfalb/summ
      return
      end

