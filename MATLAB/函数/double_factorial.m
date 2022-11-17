function result = double_factorial(n) 
    result=1;
    if (n == 1 || n==0) 
        return
    else 
        if (mod(n,2) == 0) % the number is even 
            total=2;
            while total <= n
               result=result*total;
               total=total+2;
            end
            return
        else % the number is odd 
            total=1;
            while total <= n
                result=result*total;
                total=total+2;
            end
        end 
    end
    
    return 
end

