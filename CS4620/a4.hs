-- computeRoot a x:
computeRoot :: Float -> Float -> Float
computeRoot a x = (a + (x/a)) / 2

-- approximate x xs : 
--approx' :: Float -> Float -> [zs]
--approx' _ [_] = 1.0
approx' x y = computeRoot y x : approx' x (computeRoot y x) 

--approx :: Float -> [xs] -> [ys]
approx x = approx' x 1.0
