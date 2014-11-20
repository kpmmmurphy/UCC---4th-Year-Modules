-- computeRoot a x: Formula to compute the root of 'x' using previously 
--                  computed approximate square root value 'a' 
computeRoot :: Float -> Float -> Float
computeRoot a x = (a + (x/a)) / 2
 
--approx' x y :: Helper function to compute the infinite list of square root
--               approximations of a given value 'x', using 'y' to increase
--               precision of approximation
approx' :: Float -> Float -> [Float]
approx' x y = computeRoot y x : approx' x (computeRoot y x) 

--approx x :: An infinite list of approximations which coverage to the square 
--            root of 'x'
approx :: Float -> [Float]
approx 0 = [0.0]
approx x = approx' x 1.0

--squareRoot' :: Helper function to estimate the square root of 'x'
squareRoot' :: [Float] -> Float
squareRoot' (n1:n2:ns) | abs ((-) n1 n2) <= 0.0001 = n2
                       | otherwise = squareRoot' ns

--squareRoot :: Gives an extimation of the square root of 'x'
squareRoot :: Float -> Float
squareRoot 0 = 0.0
squareRoot x = squareRoot' (approx x)




