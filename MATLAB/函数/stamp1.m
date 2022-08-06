function omega = stamp1(k,n)
%UNTITLED2 k个人，集齐n个星座的情况总数
%   此处提供详细说明
omega=0;

for i=1:n
    omega=omega+(-1)^(i+n)*combine(n,i)*i^k;
end

end