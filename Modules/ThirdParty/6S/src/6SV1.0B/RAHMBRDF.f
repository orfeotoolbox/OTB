      Subroutine RAHMBRDF (rho0,af,xk,mu,np,rm,rp,brdfint)
C***********************************************************************
C
C           A semi-empirical bidirectional reflectance model
C
C  Purpose:
C
C  To generate a single bidirectional reflectance factor value for a
C  semi-infinite medium, given the illumination and viewing geometry,
C  the optical and the structural properties of the scatterers.
C
C  Definitions:
c   geometrical conditions
C     mu1          : Illumination zenith angle, in radians
C     mu2          : Observation zenith angle, in radians
C     fi           : Relative azimuth angle, in radians
C   optical characteristics of the scatterers:
C     Rho0         : Intensity of the reflectance of the surface cover.
C                    N/D value greater or equal to 0.0 (Rho_0)
C     af           : Phase function parameter:
C                    Asymmetry factor, N/D value between -1.0 and 1.0
C     xk           : Structural parameter of the medium
C
C  References:
C
C        [1] R. Rahman, Verstraete M., and Pinty B., 1993, `A coupled 
C            surface-atmosphere reflectance (CSAR) model. Part 1: Model
C            Description and Inversion on Synthetic Data', submitted to
C            JGR.
C        [2] R. Rahman, Pinty B., and Verstraete M., 1993, `A coupled 
C            surface-atmosphere reflectance (CSAR) model. Part 2: a 
C            semi-empirical model usable with NOAA/AVHRR data', 
C            submitted to JGR.
C
C***********************************************************************
C 
      integer mu,np,j,k
      real xk,af,rho0
      real rm(-mu:mu),rp(np),brdfint(-mu:mu,np)
      real coef1,coef2,cospha,geofac
      real fi,mu1,mu2,phaang,phafun,pi,tante1,tante2
C
      pi =acos(-1.0)
      mu1=rm(0)
      do 1 k=1,np
      do 2 j=1,mu
      mu2=rm(j)
      if (j.eq.mu)then
	 fi=rm(-mu)
	 else
	 fi=rp(k)+rm(-mu)
      endif
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
      brdfint(j,k)=rho0*coef1*phafun*coef2
  2   continue
  1   continue
C
      return
      end

