
clear

wmin=9999;
wmax=0;

load /users/cabot/Spectral/Solar/nl.dat

nom=[
'01_amb_1pct.det'
'02_amb_1pct.det'
'03_amb_1pct.det'
'04_amb_1pct.det'
'05_tv_1pct.det '
'06_tv_1pct.det '
'07_tv_1pct.det '];

for b=1:7

  clear t r w ri w0ch w1ch w0 w1 

  [B c w r]=textread(sprintf('%s',deblank(nom(b,:))),'%d %d %f %f','commentstyle','shell');
  t=[B c w r];
  
  if (max(t(:,3)<200))
    t(:,3)=t(:,3)*1000;
  end
  
  nch=max(t(:,2));
  nwl=length(t(:,2))/nch;
  
  for ch=1:nch
    id=find(t(:,2)==ch);
    w0ch(ch) = t(id(1),3);
    w1ch(ch) = t(id(max(find(t(id,4)>=0.01))),3);
  end
  
  w0 = ceil (10*max(w0ch))/10 ;
  w1 = floor(10*min(w1ch))/10 ;
  
  wi=w0:0.1:w1;
  
  for ch=1:nch
   id=find(t(:,2)==ch);
   ri(:,ch)=interp1(t(id,3),t(id,4),wi');
  end

   
  cmd=sprintf('rep%d(:,1) = wi'';',b);
  eval(cmd);
  cmd=sprintf('rep%d(:,2) = mean(ri'')'';',b);
  eval(cmd);
  
  if (w0<wmin)
    wmin=w0;
  end
  if (w1>wmax)
    wmax=w1;
  end
  
end

W=(wmin:2.5:wmax)';
rep6S=zeros(length(W),9);

rep6S(:,1) = W/1000;
rep6S(:,2) = interp1(nl(:,1),nl(:,2),rep6S(:,1));
id=find(isnan(rep6S(:,2)));
rep6S(id,2) = planck(rep6S(id,1),5800)/15000;

for b=1:7
  cmd=sprintf('rep6S(:,b+2) = interp1(rep%d(:,1),rep%d(:,2),rep6S(:,1)*1000);',b,b);
  eval(cmd);
  id=find(isnan(rep6S(:,b+2)));
  rep6S(id,b+2)=0;
  rep6S(:,b+2)=rep6S(:,b+2)/max(rep6S(:,b+2));
end

save rep6S.dat rep6S /ascii



