       Subroutine POLGLIT(xts,xtv,phi,wspd,azw,ropq,ropu)
       real xts,xtv,phi,azw,wspd,ropq,ropu
       
C      compute the polarized components of the surface 
C      to an agitated surface model

C   xts is the sun zenith angle in degrees
C   xtv is the view zenith angle in degrees
C   phi is the relative azimuth between sun and view in degrees
C   wspd is the wind speed for use in ponderating
c       azw=azim. of the sun - azim. of the wind (in deg.)

C   the polarization by the actual glint reflectance
C   ropq and ropu and the stokes parameter (Q,U) of the polarized
C   surface reflectance
        real pi,dtr,csca,sca,alpha,m,rl,rr,factor
C   csca is the cosine of the scattering angle (sca)
C   alpha is the incidence angle used for specular reflection
C   alphap is the refraction angle after specular reflection
C   m is the refractive index of water (fixed to 1.33)
C   rr and rl and parameters used in the reflection mattrix
C   computation.
C   factor is a multiplicative factor to account for agitated surface
 
C following is a set of intermediate variable used in the computation
c  of factor 
      real phw
      real cs,cv,ss,sv,zx,zy,tantilt,tilt,proba,xe,xn,xe2,xn2
      real coef,cos2chi,coschi,sinchi
      real sigmaC,sigmaU,C21,C03,C40,C04,C22
      real mus,muv,sinv,cksi,ksi,r1f
      real nr,ni

        m=1.33	
	nr=1.33
	ni=0.0
        pi=acos(0.0)*2.0
        dtr=pi/180.0
	csca=-cos(xts*dtr)*cos(xtv*dtr)-sin(xts*dtr)
     s      *sin(xtv*dtr)*cos(phi*dtr)
        sca=acos(csca)
	alpha=(pi-sca)/2.0
C Originally from Deuze et al cannot mak it work to be investigated	
 	rl=(sqrt(m*m-sin(alpha)*sin(alpha))-m*m*cos(alpha))/
     s        (sqrt(m*m-sin(alpha)*sin(alpha))+m*m*cos(alpha))
C     
 	rr=(cos(alpha)-sqrt(m*m-sin(alpha)*sin(alpha)))/
     s       (cos(alpha)+sqrt(m*m-sin(alpha)*sin(alpha))) 
C     
        r1=(rl*rl+rr*rr)/2.
       	r2=(rl*rl-rr*rr)/2.
 	r3=rl*rr
 	r3=0.0
	

C       rpsur=sqrt(r2*r2+r3*r3)
C	chi=atan2(r3,r2)     

C adjust with agitated surface 
      phw=azw*dtr
      cs=cos(xts*dtr)
      cv=cos(xtv*dtr)
      ss=sin(xts*dtr)
      sv=sin(xtv*dtr)
      Zx=-sv*sin(phi*dtr)/(cs+cv)
      Zy=(ss+sv*cos(phi*dtr))/(cs+cv)
      tantilt=sqrt(zx*zx+zy*zy)
      tilt=atan(tantilt)
c  Anisotropic Gaussian distribution
c    phw=phi_sun-phi_wind
      sigmaC=0.003+0.00192*wspd
      sigmaU=0.00316*wspd
      C21=0.01-0.0086*wspd
      C03=0.04-0.033*wspd
      C40=0.40
      C22=0.12
      C04=0.23
      xe=(cos(phw)*Zx+sin(phw)*Zy)/sqrt(SigmaC)
      xn=(-sin(phw)*Zx+cos(phw)*Zy)/sqrt(SigmaU)
      xe2=xe*xe
      xn2=xn*xn
      coef=1-C21/2.*(xe2-1)*xn-C03/6.*(xn2-3)*xn
      coef=coef+c40/24.*(xe2*xe2-6*xe2+3)
      coef=coef+C04/24.*(xn2*xn2-6*xn2+3)
      coef=coef+C22/4.*(xe2-1)*(xn2-1)
      proba=coef/2./pi/sqrt(sigmaU)/sqrt(sigmaC)*exp(-(xe2+xn2)/2.)
C      write(6,*) "probapol:",proba
C      write(6,*) "coefpol:",coef
C      write(6,*) "tiltpol:",tilt
C      write(6,*) "phiw pol:",phw
c Compute Fresnel's coefficient R1
C      cos2chi=cv*cs+sv*ss*cos(phi*dtr)
C      if (cos2chi.gt.1.0)cos2chi=0.99999999999
C      if (cos2chi.lt.-1.0)cos2chi=-0.99999999999
C      coschi=sqrt(0.5*(1+cos2chi))
C      sinchi=sqrt(0.5*(1-cos2chi))
C      Call Fresnel(nr,ni,coschi,sinchi,R1f)
C      Call pfresnel(nr,ni,coschi,sinchi,r1f,r2f)
C      write(6,*) "R1 fresnel:",R1f," r1 actual:",r1
C      write(6,*) "R2 fresnel:",R2f," r2 actual:",r2
C Compute Reflectance of the sun glint
C      Rog=pi*R1*proba/4./cs/cv/(cos(tilt)**4)
        factor=pi*proba/4./cs/cv/(cos(tilt)**4)
	
C compute rotation angle for Q and U
        muv=cos(xtv*dtr)
	mus=cos(xts*dtr)
	sinv=sin(xtv*dtr)
        if (xtv.gt.0.5) then
        if (sin(phi*dtr).lt.0) then
        cksi=(muv*csca+mus)/sqrt(1.-csca*csca)/sinv
        else
        cksi=-(muv*csca+mus)/sqrt(1.-csca*csca)/sinv
        endif
        else
        cksi=1.0
        endif
	if (cksi.gt.1.) cksi=1.
	if (cksi.lt.-1.) cksi=-1.
        ksi=acos(cksi)/dtr
C	write(6,*) "KSI=",ksi	

C apply rotation mattrix
	ropq=r2*(2.*cksi*cksi-1.)*factor
	ropu=-r2*2.*cksi*sqrt(1.-cksi*cksi)*factor
	return
	end
	
	Subroutine pfresnel(nr,ni,coschi,sinchi,r1,r2)
C
C to compute the Fresnel's coefficient of reflection (see for
C example M. Born and E. Wolf, Principles of Optics, Pergamon Press, fifth
C edition, 1975, pp 628
C input parameters: nr=index of refraction of the sea water
C                   ni=extinction coefficient of the sea water
C                   coschi & sinchi=cosine and sine of the incident radiation 
C                                   with respect of the wave facet normal.
C output parameter: R1=Fresnel's coefficient for reflection
C
      real nr,ni,a1,a2,u,v,Rr2,Rl2,b1,b2,R1,coschi,sinchi
c absolute value for a1 to get v=0 when ni=0
      a1=abs(nr*nr-ni*ni-sinchi*sinchi)
      a2=sqrt((nr*nr-ni*ni-sinchi*sinchi)**2.+4*nr*nr*ni*ni)
      u=sqrt(0.5*(a1+a2))
      v=sqrt(0.5*(-a1+a2))
      Rr2=((coschi-u)**2+v*v)/((coschi+u)**2+v*v)
      b1=(nr*nr-ni*ni)*coschi
      b2=2*nr*ni*coschi
      Rl2=((b1-u)**2+(b2+v)**2)/((b1+u)**2+(b2-v)**2)
      R1=(Rr2+Rl2)/2.
      R2=(Rl2-Rr2)/2.
      return
      end

	
	
	
       
