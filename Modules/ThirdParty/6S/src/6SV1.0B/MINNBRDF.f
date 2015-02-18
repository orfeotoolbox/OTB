      subroutine minnbrdf(par1,par2,mu,np,rm,
     s           brdfint)
      real par1,par2,xmu,view
      integer mu,np,k,j
      real rm(-mu:mu),brdfint(-mu:mu,np)
      xmu=rm(0)
      do 1 k=1,np
      do 2 j=1,mu
      view=rm(j)
      brdfint(j,k)=0.5*par2*(par1+1.)*((xmu*view)**(par1-1))
  2   continue
  1   continue
      return
      end
