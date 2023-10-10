(ns project-2.core
  (:gen-class)
  (:require [clojure.set :as set]))

(defn not-elimination [not-prop]
  (when (and (seq? not-prop)
             (= 'not (first not-prop))
             (seq? (second not-prop))
             (= 'not (first (second not-prop))))
    #{(second (second not-prop))}))


(defn and-elimination [and-prop]
  (cond
    (and (list? and-prop) (= (first and-prop) 'and))
    (let [[_ p q] and-prop] #{p q})
    :else #{}))



; Modus ponens: If we have a proposition 'if p q' and we know 'p', infer 'q'
(defn modus-ponens [if-prop kb]
    ; Check if the proposition is of the form 'if p q' and 'p' is known
  (if (and (list? if-prop)
           (= 'if (first if-prop))
           (contains? kb (second if-prop)))
    ; return q
    #{(nth if-prop 2)}
    ; if not, return an empty set
    #{}))

; Modus tollens: If we have a proposition 'if p q' and we know 'not q', infer 'not p'
(defn modus-tollens [if-prop kb]
  ; Check if the proposition is of the form 'if p q' and 'not q' is known
  (if (and (list? if-prop)
           (= 'if (first if-prop))
           (contains? kb (list 'not (nth if-prop 2))))
    ; return 'not p'
    #{(list 'not (second if-prop))}
    ; If not, return empty step
    #{}))

(defn elim-step [prop kb]
  ; determine all possible inferences using each elimination rule
  (let [not-elim (not-elimination prop)
        and-elim (and-elimination prop)
        modus-p (modus-ponens prop kb)
        modus-t (modus-tollens prop kb)]
    ; Combine all infered propositions into a single set
    (clojure.set/union not-elim and-elim modus-p modus-t)))

; recursively infer new propositions until no new inferences can be made
(defn fwd-infer [prop kb]
  ; Add given proposition to the knowledge base
  (let [new-kb (conj kb prop)]
    ; Begin loop with initial kb, comparing it in each iteration to the updated version from the previous iteration
    (loop [current-kb new-kb
           previous-kb #{}]
      ; If no new inferences have been made, return the current kb
      (if (= current-kb previous-kb)
        current-kb
        ; Otherwise, apply elimination rules to each proposition in the knowledge base
        (let [relevant-props (filter list? current-kb)
              inferred-results (map #(elim-step % current-kb) relevant-props)]
          ; Recur with the updated knowledge base and the current one as "previous"
          (recur (reduce clojure.set/union current-kb inferred-results) current-kb))))))

;; (defn print-inference [result-map prop]
;;   (let [derived (result-map :results)
;;         reasons (result-map :reasons)]
;;     (println "Because:" (last (first reasons)))
;;     (println "and:" prop)
;;     (doseq [r (rest (first reasons))]
;;       (println "I derived:" r)
;;       (println (second reasons))
;;       (println))
;;     (println "=> " derived)
;;     (println "--------------------------------")))

(defn -main
  [& args]
  (println (fwd-infer '(if a b) '#{(not b)})) ; Expected: #{(if a b) (not a) (not b)} 
  (println (fwd-infer 'a '#{(if a b) (if b c)})) ; Expected: #{(if a b) a c (if b c) b}
  (println (fwd-infer '(and (not (not (if a b))) a) '#{})) ; Expected: #{(if a b) (not (not (if a b))) a (and (not (not (if a b))) a) b}
  )



