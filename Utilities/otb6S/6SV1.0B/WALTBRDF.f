      subroutine waltbrdf(a,ap,b,c,mu,np,rm,rp,
     s           brdfint)
c this model can be found in applied optics vol 24 / no 3/ pp 383-387
c but it has to be modified (slightly) to match reciprocity principle
      integer mu,np,k,j
      real rm(-mu:mu),rp(np),brdfint(-mu:mu,np)
      real a,ap,b,c
      real xmu,ts,view,tv,fi,phi

      xmu=rm(0)
      ts=acos(xmu)
      do 1 k=1,np
      do 2 j=1,mu
      view=rm(j)
      tv=acos(view)
      if (j.eq.mu) then
         fi=rm(-mu)
         else
         fi=rp(k)+rm(-mu)
         endif
      phi=fi
      brdfint(j,k)=a*(ts*ts*tv*tv)+ap*(ts*ts+tv*tv)+b*ts*tv*cos(phi)+c
  2   continue
  1   continue
      return
      end
