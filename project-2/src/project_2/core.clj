(ns project-2.core
  (:gen-class))

(defn -main
  "I don't do a whole lot ... yet."
  [& args]
  (println "Hello, World!"))


(defn not-elimination [not-prop]
  (println (not not-prop)))



;; (defn and-elimination [and-prop] ...)

;; (defn modus-ponens [if-prop kb] ...)

;; (defn modus-tollens [if-prop kb] ...)

;; (defn elim-step
;;   "One step of the elimination inference procedure."
;;   [prop kb]
;;   ...)

;; (defn fwd-infer [prop kb] ...)
