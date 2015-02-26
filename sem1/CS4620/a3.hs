-- iterate f x : Takes the function 'f' and an item, 'x' and returns the 
--               infinite list  x : f x  f ( f x ) : .....
iterate = \f -> \x -> x : iterate f (f x) 

-- powers n : The list of all positive powers of the number 'n'
powers = \x -> if x < 0 then 
                  iterate (\n -> if n > 0 then (n) * (-n) else n * n) x 
               else 
                  iterate (\n -> n * 2) (x) 

-- rep : produces a list of 'y's of length 'x' 
rep = \x -> \y -> if x == 0 then 
                      []
                  else
                      y : rep (x - 1) y 

-- reps_from : produces a list of repeated element lists from 'z'   
reps_from = \z -> rep z z : reps_from (z + 1)

-- reps : The infinite list which has, as its nth item,
--        a list composed of 'n' copies of the number 'n'
reps = reps_from 1

-- sum_elements : The list created by adding adjacent elements in the list 'xs'
sum_elements = \xs -> if null xs || null (tail xs) then
                          []
                      else 
                          (head xs) + (head (tail xs)) : sum_elements (tail xs)

-- append : The list created by appending 'xs' to 'ys', in that order
append = \xs -> \ys -> if null xs then 
                           ys
                       else
                           head xs : append (tail xs) ys                  

-- pascal_from : Produces the inifite list of Pascal's Triangle rows, from row 2 
pascal_from = \xs -> (1: append (sum_elements xs) (1:[])) : 
                      pascal_from (1: append (sum_elements xs) (1:[])) 

-- pascal : The inifite list which has, as its n-th item, a list fromed by 
--          n-th row of Pascal's Triangle, for n = 1,2,3...
pascal = (1:[]) : pascal_from ([]) 
