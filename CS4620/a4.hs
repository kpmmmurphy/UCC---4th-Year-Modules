-- computeRoot a x:
computeRoot :: Float -> Float -> Float
computeRoot a x = (a + (x/a)) / 2

-- approximate x xs : 
--approx' :: Float -> Float -> [zs]
approx' x y =  computeRoot y x : approx' x (computeRoot y x) 

--approx :: Float -> [xs] -> [ys]
approx x = 1.0 : approx' x 1.0

--squareRoot :: 
squareRoot' ( n1 :n2 : ns ) | abs ((-) n1 n2) > 0.0001 = squareRoot' ns
                            | otherwise = n2

squareRoot x = squareRoot' (approx x)

toRoot  x = [ n | n <- [0 .. squareRoot x]]
toRoot' x = [ n1 | n1 <- [0 .. squareRoot x], n2 <- [1 .. squareRoot x], ((/) n2 n1) /= 0]
--primes = [n \]
