      subroutine oceabrdffast(pws,paw,xsal,pcl,pwl,mu,np,rm,rp,
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
C
      parameter (nta=24,nfa=48)
      integer np,mu,k,j,m,n,iwl
      real rm(-mu:mu),rp(np),brdfint(-mu:mu,np)
      real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa)
      real Ref(39)
      real pwl,paw,pcl,pws,wl,wspd,C,azw,xsal
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
      n12=sqrt(nr*nr+ni*ni)
C COMPUTE WHITECAPS REFLECTANCE (LAMBERTIAN)
C      W=2.95e-06*(wspd**3.52)
C      iwl=1+int((wl-0.2)/0.1)
C      wlp=0.5+(iwl-1)*0.1
C      Ref_i=ref(iwl+1)+(wl-wlp)/0.1*(ref(iwl)-ref(iwl+1))
C      Rwc=W*Ref_i
C COMPUTE BACKSCATTERED REFLECTANCE FROM THE SEA WATER (LAMBERTIAN)
C  water reflectance below the sea surface
C      call MORCASIWAT(wl,C,Rw)
C call gauss quadrature
C      tds=1.0
C      tdv=1.0
C      if (Rw.gt.0.0001) then
C        teta1=0.
C        teta2=pi/2.
C        call gauss(teta1,teta2,ta,wta,nta)
C        phi1=0.
C        phi2=2.*pi
C        call gauss(phi1,phi2,fa,wfa,nfa)
C COMPUTE DOWNWARD TRANSMISSION FUNCTION
C        tds=0.
C        summ=0.
C        do k=1,nfa
C        do j=1,nta
C          tp=ta(j)/fac
C          fip=fa(k)/fac
C          call sunglint(wspd,nr,ni,azw,tetas,tp,fip,rogp)
C          pond=cos(ta(j))*sin(ta(j))*wfa(k)*wta(j)
C          summ=summ+pond
C          tds=tds+rogp*pond
C        enddo
C        enddo
C        tds=1.-tds/summ
C      endif
C Compute glint contribution,transmission terms and total reflectance
      Do 1 j=1,mu
        tetav=acos(rm(j))/fac
C compute upward transmission 
C        if (Rw.gt.0.0001) then
C          tw=asin(sin(tetav*fac)/nr)/fac
C          tdv=0.
C          summ=0.
C          do n=1,nfa
C          do m=1,nta
C            tp=ta(m)/fac
C            fip=fa(n)/fac
c here we assume no aborption
C            call sunglint(wspd,1./nr,0.0,azw,tw,tp,fip,rogp)
C            pond=cos(ta(m))*sin(ta(m))*wfa(n)*wta(m)
C            summ=summ+pond
C            tdv=tdv+rogp*pond
C          enddo
C          enddo
C          tdv=1.-tdv/summ
C        endif
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
C           Rwb=(1/(n12*n12))*tds*tdv*Rw/(1-a*Rw)
C TOTAL REFLECTANCE OF SEA WATER
           brdfint(j,k)=Rog
 2      continue
 1    continue
      return
      end
