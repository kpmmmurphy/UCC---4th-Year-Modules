
--positives :: Produces the infinite list of postivie integers from 1
positives :: [Int]
positives = [ n1 | n1 <- [ 1.. ]]

--negatives :: Produces the infinite list of negative integers from -1
negatives :: [Int]
negatives = [ n1 | n1 <- [-1, -2.. ]]

--zipeList :: Combines two lists together by taking the head of each and
--            combining them in sequence
zipLists :: [Int] -> [Int] -> [Int] 
zipLists ( n1 : ns ) ( n2 : ns' ) = [n1] ++ [n2] ++ (zipLists ns ns')

--integers :: The infinite list of integers, in the order -> [0,1,-1,2,-2]
integers :: [Int]
integers = 0 : zipLists positives negatives

--squared :: Squares the number n
squared :: Int -> Int
squared n = (*) n n

--pytrips :: The infite list of Pythagorean Triples
pytrips :: [(Int,Int,Int)]
pytrips = [( (squared n2) - (squared n1), 
             (2) * (n1 * n2),
             (squared n2) + (squared n1) ) 
           | n1 <- [1..], n2 <- [2..], gcd n2 n1 == 1, mod (n2 - n1) 2 /= 0]

--runs' xs :: Counts the number of adjacent equals blocks using 'acc' paramater
runs' :: (Eq a) => [a] -> Int -> Int
runs' [_] acc = acc + 1
runs' ( x1 : x2 : xs ) acc | x1 == x2  = runs' ( x2 : xs ) acc
                           | otherwise = runs' ( x2 : xs ) (acc + 1)


--runs' xs :: The number of blocks of adjacent equal items in 
--            the finite list 'xs'
runs ::(Eq a) => [a] -> Int
runs [] = 0
runs xs = runs' xs 0 
