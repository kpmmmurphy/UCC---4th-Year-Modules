-- computeRoot a x:
computeRoot a x = (/) (a + ((/) x a)) 2

-- approximate x xs : 
approximate x xs = if null xs then
                      1 : approximate x (computeRoot 1 x)
                   else
                      x : approximate x (computeRoot (head xs) x)


