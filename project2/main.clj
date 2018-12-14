(defn orr [name]
(let[x (flatten(distinct(rest (remove false? name))))]
  
  (cond
      (<= (count x) 0) false 
      (=  (some true? x)     true)true 
      (= (count  x ) 1)  (first x)
      :else ( x)  
  )
  )
  )

  (defn andd [input]
    
    (let [iny (flatten(distinct(rest input)))]
    ;;(print iny)
    (cond
      (=(some false? iny) true) false
      (=(every? true? iny)true) true 
      (>= (count(remove true? iny)) 1)(first(rest(remove true? input)))
      :else (flatten(distinct input)) ;;more than one symbol
    )
  ))




  (defn nott[input]
      
    (let [xx (rest input)]
      (cond      
      (= ( every? false? xx ) true) true
      (= (every? true? xx) true) false
      :else input
      )))

      
      


      
   
(defn deep-substitute [m l]
  ;;(print "hh")
  
  (map (fn [i]
         ;;(print i)
         (if (seq? i)
           (deep-substitute m i)
           (m i i)
           ) 
           )
        l))
;;have a list that contains all of the symbs and call it after recur
(defn main[m l]
   (let[xx (into [] (deep-substitute m l))] ;;inefficient constantly calling
   ;;possible fix just add a higher level function
  
   (evalRes(remove nil?(map (fn [i]

          (if(seq? i)
          (do

            (let[ince  (main m i) ]
        
            ince)
            )
            i) ;;we check for or not and and eval return it 
               )   
                xx)
                ))    
          )
          )
    
   (defn evalRes [msg]
    (cond
    (= 'and (first msg)) 
          (do      
              (andd (distinct msg))
              )
    (= 'or (first msg)) (do               
              (orr (distinct msg)))
                
    (= 'not (first msg)) (do 
              (nott   msg)
                )
                :else msg))