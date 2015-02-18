      subroutine possol (month,jday,tu,xlon,xlat,
     a                   asol,phi0)
 
      real    tu,xlon,xlat,asol,phi0
      integer month,jday,ia,nojour

c     solar position (zenithal angle asol,azimuthal angle phi0
c                     in degrees)
c     jday is the number of the day in the month
 
      ia = 0
      call day_number(jday,month,ia,nojour)
 
      call  pos_fft (nojour, tu, xlon, xlat, asol, phi0)
 
      if(asol.gt.90) call print_error(
     s 'The sun is not raised')
      return
      end

      subroutine day_number(jday,month,ia,j)
      integer jday, month, ia, j

      if (month.le.2) then
                      j=31*(month-1)+jday
		      return
		      endif
      if (month.gt.8) then
                      j=31*(month-1)-((month-2)/2)-2+jday
		      else
                      j=31*(month-1)-((month-1)/2)-2+jday
		      endif
      if(ia.ne.0 .and. mod(ia,4).eq.0) j=j+1
      return
      end

      subroutine pos_fft (j,tu,xlon,xlat,asol,phi0)
      real    tu, xlat, asol,phi0, tsm, xlon,xla, xj, tet,
     a	      a1, a2, a3, a4, a5, et, tsv, ah, b1, b2, b3, b4,
     a	      b5, b6, b7, delta, amuzero, elev, az, caz, azim, pi2
      integer j
      parameter (pi=3.14159265,fac=pi/180.)
c     solar position (zenithal angle asol,azimuthal angle phi0
c                     in degrees)
c     j is the day number in the year
c
c    mean solar time (heure decimale)
 
      tsm=tu+xlon/15.
      xla=xlat*fac
      xj=float(j)
      tet=2.*pi*xj/365.
 
c    time equation (in mn.dec)
      a1=.000075
      a2=.001868
      a3=.032077
      a4=.014615
      a5=.040849
      et=a1+a2*cos(tet)-a3*sin(tet)-a4*cos(2.*tet)-a5*sin(2.*tet)
      et=et*12.*60./pi
 
c     true solar time
 
      tsv=tsm+et/60.
      tsv=(tsv-12.)
 
c     hour angle
 
      ah=tsv*15.*fac
 
c     solar declination   (in radian)
 
      b1=.006918
      b2=.399912
      b3=.070257
      b4=.006758
      b5=.000907
      b6=.002697
      b7=.001480
      delta=b1-b2*cos(tet)+b3*sin(tet)-b4*cos(2.*tet)+b5*sin(2.*tet)-
     &b6*cos(3.*tet)+b7*sin(3.*tet)
 
c     elevation,azimuth
 
      amuzero=sin(xla)*sin(delta)+cos(xla)*cos(delta)*cos(ah)
      elev=asin(amuzero)
      az=cos(delta)*sin(ah)/cos(elev)
      if ( (abs(az)-1.000).gt.0.00000) az = sign(1.,az)
      caz=(-cos(xla)*sin(delta)+sin(xla)*cos(delta)*cos(ah))/cos(elev)
      azim=asin(az)
      if(caz.le.0.) azim=pi-azim
      if(caz.gt.0.and.az.le.0) azim=2*pi+azim
      azim=azim+pi
      pi2=2*pi
      if(azim.gt.pi2) azim=azim-pi2
      elev=elev*180./pi
 
c     conversion in degrees
 
      asol=90.-elev
      phi0=azim/fac
      return
      end
