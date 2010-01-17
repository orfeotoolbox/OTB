      subroutine brdfgrid(mu,np,rm,rp,brdfdat,angmu,angphi,
     s           brdfint)
      integer mu,np
      real rp(np),brdfint(-mu:mu,np),rm(-mu:mu)
     s    ,angmu(10),angphi(13),brdfdat(10,13)
      real brdftemp(10,13)
      real gaussmu,gaussphi,y
      integer j,k
      do 10 j=1,np
      do 10 k=1,mu
   10 brdfint(k,j)=0.
      call splie2(angphi,brdfdat,10,13,brdftemp)
      do 1 j=1,np
      do 2 k=1,mu
      gaussmu=rm(k)
      gaussphi=rp(j)
      call splin2(angmu,angphi,brdfdat,brdftemp,10,13,
     s      gaussmu,gaussphi,
     s      y)
      brdfint(k,j)=y
   2  continue
   1  continue
      return
      end
