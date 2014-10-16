-- foldr_apply : Template helper function which returns 'z' if 'fs' is null, or applies all the functions in 'fs' to the item 'x'
foldr_apply = \z -> \fs -> \x -> if null fs then z else (head fs) x : foldr_apply z (tail fs) x 
 
-- applyAll : Returns the list formed by calling each function in the function list 'fs' on item 'x'
applyAll = foldr_apply [] 


-- foldr_remove : Template helper function which checks if each item in 'xs' satisfies a predicate 'p', 
--               and includes the value in the returned list if true
foldr_remove = \z -> \p -> \xs -> if null xs then 
                                     z
                                  else
                                     if p (head xs) then
                                        head xs : foldr_remove z p (tail xs)
                                     else
                                        foldr_remove z p (tail xs) 

-- remove : The list formed by those components of list 'xs' which do not satisfy 'p'                             
--remove = foldr_remove [] 
remove = \p -> foldr (\h -> \t -> if p (h) then  t else h : t) []


-- foldr_count : Template helper method 
foldr_count = \z -> \x -> \xs -> if null xs then 
                                   z
                                 else
                                   ( if x == head xs then 1 else 0 ) + foldr_count z x (tail xs)

-- count : the number of times the item 'x' occurs in 'xs'
count = foldr_count 0 


