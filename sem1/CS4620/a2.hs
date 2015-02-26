-- foldr_apply : Returns 'z' if 'fs' is null, or applies the functions 'f' to 
--               the three params 'fs', 'x', and the recursive call 
--               of this function
foldr_apply = \f -> \z -> \fs -> \x -> if null fs then 
                                          z 
                                       else 
                                          f fs x (foldr_apply f z (tail fs) x)
 
-- applyAll : Returns the list formed by calling each function 
--            in the function list 'fs' on item 'x'
applyAll = foldr_apply (\zs -> \a -> \b -> (head zs) a : b) [] 

-- remove : The list formed by those components of list 'xs' 
--          which do not satisfy 'p'                             
remove = \p -> foldr (\h -> \t -> if p (h) then  t else h : t) []

-- count : the number of times the item 'x' occurs in 'xs'
count = \x -> foldr (\a -> \b -> (if a == x then 1 else 0) + b) 0

-- xor : Does the boolean list contain an odd number of True components?
xor = \bs -> mod (length (filter (\n -> n) bs )) 2  == 1  

-- foldr_append : Applies the function 'f' to two params 'xs' and 'ys' 
--                of type List
foldr_append = \f -> \xs -> \ys -> f xs ys (foldr_append f (tail xs) ys)

-- append : The list formed by joining lists 'xs' and 'ys' in that order
append = foldr_append (\xs -> \ys -> \c -> if null xs then 
                                              ys 
                                           else  
                                              head xs : c) 
