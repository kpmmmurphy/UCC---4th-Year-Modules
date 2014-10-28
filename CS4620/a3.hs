-- iterate f x : Takes the function 'f' and an item, 'x' and returns the infinite list
--               x : f x  f ( f x ) : .....
iterate = \f -> \x -> x : iterate f (f x) 

-- powers n : The list of all positive powers of the number 'n'
powers = \n -> n : powers (n * 2)

-- reps : The infinite list which has, as its n-th item, a list formed
--        of 'n' copies of the number 'n'


rep = \x -> \n -> (if n == 0 then 
                     [] : (rep (x + 1) (n + 1)) 
                  else
                     x : rep x (n - 1))

reps = rep 1 1
