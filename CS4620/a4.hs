-- computeRoot a x:
computeRoot :: Float -> Float -> Float
computeRoot a x = (/) (a + ((/) x a)) 2.0

-- approximate x xs : 
approximate :: Float -> [xs] -> [ys]
approximate x xs = if null xs then
                      1.0 : approximate x [(computeRoot 1.0 x)]
                   else
                     head xs : approximate x [(computeRoot (head xs) x)]


