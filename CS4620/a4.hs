-- computeRoot a x:
computeRoot :: Float -> Float -> Float
computeRoot a x = (a + (x/a)) / 2

-- approximate x xs : 
--approx' :: Float -> Float -> [zs]
approx' x y =  computeRoot y x : approx' x (computeRoot y x) 

--approx :: Float -> [xs] -> [ys]
approx x = 1.0 : approx' x 1.0

--squareRoot :: 
squareRoot' x = [let n3 = n1 - n2 | n1 <- approx x, n2 <- tail (approx x), n3 <= 0.0005]
