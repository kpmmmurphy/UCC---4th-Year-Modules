-- iterate f x : Takes the function 'f' and an item, 'x' and returns the infinite list
--               x : f x  f ( f x ) : .....
iterate = \f -> \x -> x : iterate f (f x) 

-- powers n : The list of all positive powers of the number 'n'
powers = \x -> iterate (\n -> n * n) (x) 

-- reps : The infinite list which has, as its n-th item, a list formed
--        of 'n' copies of the number 'n'

rep = \x ->  \y -> if x == 0 then 
                      []
                    else
                      y : rep (x - 1) y

repp = iterate ((rep) 1  1) (from 1)

