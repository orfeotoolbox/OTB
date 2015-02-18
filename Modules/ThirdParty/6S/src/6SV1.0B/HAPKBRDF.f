      subroutine hapkbrdf(om,af,s0,h,mu,np,rm,rp,
     s           brdfint)
      integer mu,np,k,j
      real rm(-mu:mu),rp(np),brdfint(-mu:mu,np)
      real f,h1
      real h2,h1h2,pg,p0,g,bg
      real mu1,mu2,fi,cg
      real om,af,s0,h
c here the notation are taken according to j.g.r.,vol 95,no d8,pp 11767,
c geometrical conditions: mu1 cosine of sun  zenith angle
c                         mu2 cosine of view zenith angle
c                         fi: fi1-fi2 [radians]
c                         cg: cos(g)
c canopy parameters     : om  (albedo)
c			: af assymetry parameter for the phase function
c			: s0 amplitude of hot spot
c			: h width of the hot spot
      mu1=rm(0)
      do 1 k=1,np
      do 2 j=1,mu
      mu2=rm(j)
      if (j.eq.mu) then
         fi=rm(-mu)
         else
         fi=rp(k)+rm(-mu)
         endif
      cg=mu1*mu2+sqrt(1.-mu1*mu1)*sqrt(1.-mu2*mu2)*cos(fi)
      f=om/4./(mu2+mu1)
      h1=(1.+2*mu1)/(1.+2.*sqrt(1.-om)*mu1)
      h2=(1.+2*mu2)/(1.+2.*sqrt(1.-om)*mu2)
      h1h2=h1*h2
      pg=(1-af*af)/((1+af*af+2*af*cg)**1.5)
      p0=(1-af*af)/((1+af*af+2*af)**1.5)
      g=acos(cg)
      bg=(s0/(om*p0))/(1+tan(g/2.)/h)
      brdfint(j,k)=f*((1.+bg)*pg+h1h2-1.)
  2   continue
  1   continue
      return
      end
 
 
 
 
