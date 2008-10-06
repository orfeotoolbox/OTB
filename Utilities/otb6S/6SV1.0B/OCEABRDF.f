      subroutine oceabrdf(pws,paw,xsal,pcl,pwl,
     s        rfoam,rwat,rglit,
     s        mu,np,rm,rp,
     s           brdfint)
C
C INPUT:  pws=wind speed (in m/s)
C         paw= azim. of sun - azim. of wind (in deg.)
C	  xsal=salinity (in ppt)
C	  pcl=pigment concentration (in mg.m-3)
C         pwl=wavelength of the computation (in micrometer)
C         mu=number of zenith angle
C         np=number of azimuth
C         rm=cosine of Gauss's angles for angles between -PI/2 and PI/2 deg
C         rp=Gauss's angles for angles between 0 and 2*PI
C OUTPUT: brdfint(j,k)=the total reflectance of the sea water
C         rfoam= the effective reflectance of the foam (foam coverage x foam reflectance)
C	  rwat= the reflectance of the water (just above the surface)
C         rglit= the reflectance of the sunglint
	  
C
      parameter (nta=24,nfa=48)
      integer np,mu,k,j,m,n,iwl
      real rm(-mu:mu),rp(np),brdfint(-mu:mu,np)
      real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa)
      real Ref(39)
      real pwl,paw,pcl,pws,wl,wspd,C,azw,xsal
      real rfoam,rwat,rglit
      real pi,fac,nr,ni,n12
      real tetas,w,wlp,ref_i,rwc,rw,tds,summ,tp,fip
      real rogp,pond,tetav,tw,tdv,fi,rog,a,rwb
c effective reflectance of the whitecaps (Koepke, 1984)
      data Ref/
     &0.220,0.220,0.220,0.220,0.220,0.220,0.215,0.210,0.200,0.190,
     &0.175,0.155,0.130,0.080,0.100,0.105,0.100,0.080,0.045,0.055,
     &0.065,0.060,0.055,0.040,0.000,0.000,0.000,0.000,0.000,0.000,
     &0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000/
C conversion of parameter
      C=pcl
      wspd=pws
      azw=paw
      wl=pwl
      pi=atan(1.)*4.
      fac=pi/180.
      tetas=acos(rm(0))/fac
C COMPUTE INDEX OF WATER
      call indwat(wl,xsal,nr,ni)
C      write(6,'(1A,1X,2(F10.5,1X))') "nr,ni",nr,ni
      n12=sqrt(nr*nr+ni*ni)
C COMPUTE WHITECAPS REFLECTANCE (LAMBERTIAN)
      W=2.95e-06*(wspd**3.52)
      iwl=1+int((wl-0.2)/0.1)
      wlp=0.5+(iwl-1)*0.1
      Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1))
      Rwc=W*Ref_i
C COMPUTE BACKSCATTERED REFLECTANCE FROM THE SEA WATER (LAMBERTIAN)
C  water reflectance below the sea surface
      call MORCASIWAT(wl,C,Rw)
C call gauss quadrature
      tds=1.0
      tdv=1.0
      if (Rw.gt.0.0001) then
        teta1=0.
        teta2=pi/2.
        call gauss(teta1,teta2,ta,wta,nta)
        phi1=0.
        phi2=2.*pi
        call gauss(phi1,phi2,fa,wfa,nfa)
C COMPUTE DOWNWARD TRANSMISSION FUNCTION
        tds=0.
        summ=0.
        do k=1,nfa
        do j=1,nta
          tp=ta(j)/fac
          fip=fa(k)/fac
          call sunglint(wspd,nr,ni,azw,tetas,tp,fip,rogp)
          pond=cos(ta(j))*sin(ta(j))*wfa(k)*wta(j)
          summ=summ+pond
          tds=tds+rogp*pond
        enddo
        enddo
        tds=1.-tds/summ
      endif
C Compute glint contribution,transmission terms and total reflectance
      Do 1 j=1,mu
        tetav=acos(rm(j))/fac
C compute upward transmission 
        if (Rw.gt.0.0001) then
          tw=asin(sin(tetav*fac)/nr)/fac
          tdv=0.
          summ=0.
          do n=1,nfa
          do m=1,nta
            tp=ta(m)/fac
            fip=fa(n)/fac
c here we assume no aborption
            call sunglint(wspd,1./nr,0.0,azw,tw,tp,fip,rogp)
            pond=cos(ta(m))*sin(ta(m))*wfa(n)*wta(m)
            summ=summ+pond
            tdv=tdv+rogp*pond
          enddo
          enddo
          tdv=1.-tdv/summ
        endif
        Do 2 k=1,np
           if (j.eq.mu) then
             fi=rm(-mu)
           else
             fi=(rp(k)+rm(-mu))
           endif
           if (fi.lt.0.) fi=fi+2.*pi
           if (fi.gt.(2.*pi)) fi=fi-2.*pi
           fi=fi/fac
C SUNGLINT REFLECTANCE
           call sunglint(wspd,nr,ni,azw,tetas,tetav,fi,rog)
C  water reflectance above the sea surface
C for explanation on value of a see OCEAALBE.f
           a=0.485
C add change in solid angle from under to above to surface
C that account for 1/(n12*n12) decrease in sea water directional
C reflectance
           Rwb=(1/(n12*n12))*tds*tdv*Rw/(1-a*Rw)
C TOTAL REFLECTANCE OF SEA WATER
           brdfint(j,k)=Rwc+(1-W)*Rog+(1-Rwc)*Rwb
	   rfoam=rwc
	   rwat=(1-Rwc)*Rwb
	   rglit=(1-W)*Rog
 2      continue
 1    continue
      return
      end
