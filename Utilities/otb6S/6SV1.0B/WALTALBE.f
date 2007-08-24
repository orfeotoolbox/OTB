      subroutine waltalbe(a,ap,b,c,
     s           brdfalb)
      parameter (nta=24,nfa=48)
      real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa)
      real a,ap,b,c
      real pi
      real brdfalb,summ,si2,si1,ts,tv,phi,pond,brdfint,mu1,mu2
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
      phi=fa(k)
      pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l)
      brdfint=a*ts*ts*tv*tv+ap*(ts*ts+tv*tv)+b*ts*tv*cos(phi)+c
      brdfalb=brdfalb+brdfint*pond
      summ=summ+pond
  3   continue
  2   continue
  1   continue
      brdfalb=brdfalb/summ
      return
      end
