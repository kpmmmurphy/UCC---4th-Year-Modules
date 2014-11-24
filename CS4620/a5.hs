
--positives ::
positives :: [Int]
positives = [ n1 | n1 <- [ 1.. ]]
--negatives :: 
negatives :: [Int]
negatives = [ n1 | n1 <- [-1, -2.. ]]
--zipeList :: 
zipLists :: [Int] -> [Int] -> [Int] 
zipLists ( n1 : ns ) ( n2 : ns' ) = [n1] ++ [n2] ++ (zipLists ns ns')

--integers :: 
integers :: [Int]
integers = 0 : zipLists positives negatives

--squared ::
squared n = (*) n n

--pytrips ::
pytrips :: [(Int,Int,Int)]
pytrips = [( (-) (squared n2) (squared n1), (*) (2) ( (*) n1 n2 ), (+) (squared n1) (squared n2) ) | 
            n1 <- [1..], n2 <- [2..], gcd n2 n1 == 1, mod ((-) n2 n1) 2 /= 0]

--runs xs ::
runs' [_] acc = acc + 1
runs' ( x1 : x2 : xs ) acc | x1 == x2  = runs' ( x2 : xs ) acc
                           | otherwise = runs' ( x2 : xs ) (acc + 1)

runs [] = 0
runs xs = runs' xs 0 
