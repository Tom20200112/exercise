
function c = combine(n,k)
%UNTITLED3 此处提供此函数的摘要
%   此处提供详细说明
c=1;
for i=0:(k-1)
    c=c*((n-i)/(i+1));
end

end