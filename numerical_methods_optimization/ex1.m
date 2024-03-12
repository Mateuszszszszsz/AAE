clear all;
close all;
clc;
A = [2 -1 0 0; 
    -1 2 -1 0; 
     0 -1 2 -1;
     0 0 -1 2];
b = [0; 0; 0; 5];
B = [ A b ]
for i = 2:size(B)(1)
  k = i - 1;
  B(i,:) = B(i,:) - ( B(i,k)/B(k,k) ) * B(k,:) 
  
end

for i = 0 : 3
  j = 4 - i;
  B(j,5) = B(j,5)/ B(j,j);
  B(j,j) = B(j,j) / B(j,j);
  for k = 1 : j -1
    B(k,5) = B(k,5) - (B(k,j) * B(j,5));
    B(k,j) = 0;
  endfor
endfor
  B