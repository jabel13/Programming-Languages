(ns project-2.core
  (:gen-class))


(defn not-elimination [not-prop]
  ; Check if it's a list
  (if (list? not-prop)
    ; Check if it starts with 'not'
    (if (= 'not (first not-prop))
      ; Check if the second element is a list starting with 'not'
      (if (list? (second not-prop))
        ; If all conditions are met, return a singleton set
        #{(second (second not-prop))}
        ; If the second element is not a valid nested 'not', return an empty set
        #{})
      ; If it doesn't start with 'not', return an empty set
      #{})
    ; If it's not a list, return an empty set
    #{}))


;; (defn and-elimination [and-prop] ...)

;; (defn modus-ponens [if-prop kb] ...)

;; (defn modus-tollens [if-prop kb] ...)

;; (defn elim-step
;;   "One step of the elimination inference procedure."
;;   [prop kb]
;;   ...)

;; (defn fwd-infer [prop kb] ...)

(defn -main
  [& args])

