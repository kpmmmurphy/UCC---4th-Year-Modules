insert :: [xs] -> [ys] -> [[zs]]
insert ( xs ) ( ys ) = if null head ys || head xs <= head ys then
                                      insert xs ( head xs : ys ) 
                                   else
                                      insert xs ( ys ++ head xs )
isort xs = insert xs []
