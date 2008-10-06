      subroutine splie2(x2a,ya,m,n,y2a)

      parameter (nn=100)
      integer m,n,j,k
      real x2a(n),ya(m,n),y2a(m,n),ytmp(nn),y2tmp(nn)
      do 13 j=1,m
        do 11 k=1,n
          ytmp(k)=ya(j,k)
11      continue
        call spline(x2a,ytmp,n,1.e30,1.e30,y2tmp)
        do 12 k=1,n
          y2a(j,k)=y2tmp(k)
12      continue
13    continue
      return
      end
