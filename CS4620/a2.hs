-- foldr_apply : Template helper function which returns 'z' if 'fs' is null, or applies all the functions in 'fs' to the item 'x'
foldr_apply = \z -> \fs -> \x -> if null fs then z else (head fs) x : foldr_apply z (tail fs) x 
 
-- applyAll : Returns the list formed by calling each function in the function list 'fs' on item 'x'
applyAll = foldr_apply [] 

-- remove : The list formed by those components of list 'xs' which do not satisfy 'p'                             
remove = \p -> foldr (\h -> \t -> if p (h) then  t else h : t) []

-- count : the number of times the item 'x' occurs in 'xs'
count = \x -> foldr (\a -> \b -> (if a == x then 1 else 0) + b) 0 

-- foldr_append : Generic helper function
foldr_append = \f -> \xs -> \ys -> f xs ys (foldr_append f (tail xs) ys)

-- append : The list formed by joining lists 'xs' and 'ys' in that order
append = foldr_append (\xs -> \ys -> \c -> if null xs then ys else  head xs : c) 
