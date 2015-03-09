      subroutine poslan (month,jday,tu,xlon,xlat,
     a                   asol,phi0,avis,phiv)
      logical ier
      real tu,xlon,xlat,asol,phi0,avis,phiv
      integer month,jday,iwr
      common/sixs_ier/iwr,ier
 
c     landsat5 definition
c     warning !!!
c     xlon and xlat are the coordinates of the scene center.
 
      avis=0.
      phiv=0.
 
      call possol(month,jday,tu,xlon,xlat,
     a            asol,phi0)
      if(ier)return
 
      return
      end
