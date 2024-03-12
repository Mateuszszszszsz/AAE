clear all;
close all;
clc;
A = [1 1 1; 
    1 1 2; 
     1 2 2];
b = [1; 2; 1];
B = [ A b ]
for i = 2:size(B,1)
  k = i - 1
  if B(k, k) == 0
    for j = i:size(B,1)
      if B(k,j) ~= 0
        temp = B(j, :)
        B(j, :) = B(k, :)
        B(k, :) = temp
      end
    end
  end
  for j = i:size(B,1)
    B(j,:) = B(j,:) - ( B(j,k)/B(k,k) ) * B(k,:) 
  end
end

for i = 1 : 3
  j = 4 - i;
  B(j,4) = B(j,4)/ B(j,j);
  B(j,j) = B(j,j) / B(j,j);
  for k = 1 : j -1
    B(k,4) = B(k,4) - (B(k,j) * B(j,4));
    B(k,j) = 0;
  end
end
  B