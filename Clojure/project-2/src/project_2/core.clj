(ns project-2.core
  (:gen-class)
  ; require, similar to 'import'
  (:require [clojure.set :as set]))

(defn not-elimination [not-prop]
  ; Check if the following conditions are met:
  ; not-prop is a sequence 
  (when (and (seq? not-prop)
             ; The first element of not-prop is the symbol 'not'
             (= 'not (first not-prop))
             ; The second element of not-prop is a sequence
             (seq? (second not-prop))
             ; The first element of the second element of not-prop is the symbol 'not'
             (= 'not (first (second not-prop))))
    ; If the conditions are met, return a set containing 
    ; the second element of the second element of not-prop.
    #{(second (second not-prop))}))


(defn and-elimination [and-prop]
  ; Check if and-prop is a list, and its first element is the symbol 'and'
  (if (and (list? and-prop) (= (first and-prop) 'and))
    ; If they are, return a set containing the second and third elements of and-prop
    #{(nth and-prop 1) (nth and-prop 2)}
    ; Otherwise, return an empty set
    #{}))


; A proposition 'if p q' and we know 'p', infer 'q'
(defn modus-ponens [if-prop kb]
    ; Check if the proposition is a list 
  (if (and (list? if-prop)
           ; Check if the first element is the symbol 'if
           (= 'if (first if-prop))
           ; Check if the knowledge base contains the second element of if-prop
           (contains? kb (second if-prop)))
    ; return a set containing the third element of if-prop
    #{(nth if-prop 2)}
    ; if not, return an empty set
    #{}))


; A proposition 'if p q' and we know 'not q', infer 'not p'
(defn modus-tollens [if-prop kb]
  ; Check if the proposition is a list
  (if (and (list? if-prop)
           ; check if the first element is the symbol 'if
           (= 'if (first if-prop))
           ; check if the negation ('not') of the third element of the if-prop is in the knowledge base
           (contains? kb (list 'not (nth if-prop 2))))
    ; return a set containing the negation of the second element of the if-prop
    #{(list 'not (second if-prop))}
    ; If not, return empty set
    #{}))


; Determines the potential inferences, given a prop and kb
(defn elim-step [prop kb]
  ; Use 'let' to bind results
  ; Apply the not-elimination rule on the proposition and store the result in `not-elim`
  (let [not-elim (not-elimination prop)
        ; Apply the and-elimination rule on the proposition and store the result in `and-elim`
        and-elim (and-elimination prop)
        ; Apply modus-ponus using the prop and kb, then store the result
        modus-p (modus-ponens prop kb)
        ; Apply modus-ponus using the prop and kb, then store the result
        modus-t (modus-tollens prop kb)]
    ; Combine all infered propositions from above, into a single set
    (set/union not-elim and-elim modus-p modus-t)))


; Recursively infer new propositions until no new inferences can be made
(defn fwd-infer [prop kb]
  ; Initialize the knowledge base - the given prop is added (conjoined) to the kb to create 'new-kb' 
  (let [new-kb (conj kb prop)]
    ; Begin recursive loop with initial kb, current-kb is set to new-kb (the kb with added prop), previous-kb is set to an empty set
    (loop [current-kb new-kb
           previous-kb #{}]
      ; Check for new inferences
      ; If current-kb is the same as previous-kb, no new propsitions were infered in last iteration, function returns current state of kb
      (if (= current-kb previous-kb)
        current-kb
        ; Only props that are lists are considered for inference
        ; For each prop in current-kb, the elim-step function is applied to infer new propositions
        ; 'map' function is used to apply elim step to each relevant proposition
        (let [relevant-props (filter list? current-kb)
              inferred-results (map #(elim-step % current-kb) relevant-props)]
          ; Use reduce with the set/union to combine all new infered props with the the current kb
          ; Then the loop recurs with the updated kb as current-kb, and the preivous current-kb becomes previous-kb
          (recur (reduce set/union current-kb inferred-results) current-kb))))))

(defn -main
  [& args]
  (println (fwd-infer '(if a b) '#{(not b)})) ; Expected: #{(if a b) (not a) (not b)} 
  (println (fwd-infer 'a '#{(if a b) (if b c)})) ; Expected: #{(if a b) a c (if b c) b}
  (println (fwd-infer '(and (not (not (if a b))) a) '#{})) ; Expected: #{(if a b) (not (not (if a b))) a (and (not (not (if a b))) a) b}
  )



