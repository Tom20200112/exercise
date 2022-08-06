function [r,k,b] = regress1(x,y)
%UNTITLED 此处提供此函数的摘要
%   线性回归
plot(x,y,'r*');

Lxx=sum((x-mean(x)).^2);
Lxy=sum((x-mean(x)).*(y-mean(y)));

k=Lxy/Lxx;
b=mean(y)-k*mean(x);
y1=k*x+b;
hold on;

plot(x,y1,'linewidth',1);
r=corrcoef(x,y);
end