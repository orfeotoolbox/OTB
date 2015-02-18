      subroutine splin2(x1a,x2a,ya,y2a,m,n,x1,x2,y)
      parameter (nn=100)
      integer m,n,j,k
      real x1,x2,y
      real x1a(m),x2a(n),ya(m,n),y2a(m,n),ytmp(nn),y2tmp(nn)
      real yytmp(nn)
      do 12 j=1,m
        do 11 k=1,n
          ytmp(k)=ya(j,k)
          y2tmp(k)=y2a(j,k)
11      continue
        call splint(x2a,ytmp,y2tmp,n,x2,yytmp(j))
12    continue
      call spline(x1a,yytmp,m,1.e30,1.e30,y2tmp)
      call splint(x1a,yytmp,y2tmp,m,x1,y)
      return
      end
