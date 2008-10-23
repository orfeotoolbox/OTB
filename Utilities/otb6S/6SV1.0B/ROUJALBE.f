      subroutine roujalbe(k0,k1,k2,
     s           brdfalb)
      parameter (nta=24,nfa=48)
      real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa)
      real k0,k1,k2,brdfalb,psi,cpsi,mu1,mu2,si1,si2,ft
      real pi
      real summ,ts,tv,fr,pond,tts,ttv,xmus,xmuv,f2,f1
      integer k,j,l

      pi=atan(1.)*4.
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
      si2=sin(ta(j))
      si1=sin(ta(l))
      mu2=cos(ta(j))
      mu1=cos(ta(l))
      ts=ta(j)
      tv=ta(l)
      fr=acos(cos(fa(k)))
      pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l)
      tts=tan(ts)
      ttv=tan(tv)
      xmus=cos(ts)
      xmuv=cos(tv)
      cpsi=xmus*xmuv+sin(ts)*sin(tv)*cos(fr)
      if (cpsi.lt.1.) then
      psi=acos(cpsi)
      else
      psi=0.
      endif
      f2=4./(3.*pi*(xmus+xmuv))*((pi/2-psi)*cpsi+sin(psi))-1./3.
      ft=tts*tts+ttv*ttv-2*tts*ttv*cos(fr)
      f1=0.5*((pi-fr)*cos(fr)+sin(fr))*tts*ttv-tts-ttv-sqrt(ft)
      f1=f1/pi
      brdfalb=brdfalb+(k0+k1*f1+k2*f2)*pond
      summ=summ+pond
  3   continue
  2   continue
  1   continue
      brdfalb=brdfalb/summ
      return
      end
 
 
