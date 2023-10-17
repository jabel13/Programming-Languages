(* Modified in part from CSMC330 at UMD *)
 
(* Parser for
E -: T '+' E | T
T -: A '*' T | A
A -: [0-9]+ *)
 
(* #load "str.cma" *)
(* open Str; *)
 
(***** Scanner *****)
 
type token = Tok_Num of string (* changed to string for multiple digits *)
  | Tok_Sum 
  | Tok_Mul (* New token type for multiplication *)
  | Tok_END
 
let re_num = Str.regexp "[0-9]+"  (* added a '+' to match one or more digits *)
let re_add = Str.regexp "+"
let re_mul = Str.regexp "*"
 
exception IllegalExpression of string
 
let tokenize str =
  let rec tok pos s =
    if pos >= String.length s then
      [Tok_END]
    else
      if (Str.string_match re_num s pos) then
        (* Tokenize numbers and advance by the length of the matched string *)
        let token = Str.matched_string s in
        (Tok_Num token)::(tok (pos + (String.length token)) s)
      else if (Str.string_match re_add s pos) then
        Tok_Sum::(tok (pos+1) s)
      else if (Str.string_match re_mul s pos) then
        Tok_Mul::(tok (pos+1) s)  (* Tokenize multiplication symbol *)  
      else
        raise (IllegalExpression "tokenize")
    in
    tok 0 str
 
(***** Parser *****)
 
type exp = Num of int
 | Sum of exp * exp
 | Mul of exp * exp (* New type for multiplication expressions *)
 
let rec a_to_str a =
  match a with
    Num n -> string_of_int n 
  | Sum (a1,a2) -> "(" ^ (a_to_str a1) ^ " + " ^ (a_to_str a2) ^ ")"
  (* matches an expression that represents a multiplication *)
  | Mul (a1,a2) -> "(" ^ (a_to_str a1) ^ " * " ^ (a_to_str a2) ^ ")"
;;

 
let tok_list = ref []
 
exception ParseError of string
 
let lookahead () =
 match !tok_list with
   [] -> raise (ParseError "no tokens")
 | (h::t) -> h
 
let match_tok a =
 match !tok_list with
 (* checks the current token in the list (the "lookahead") and 
    consumes the token if it matches the expected token. *)
 | (h::t) when a = h -> tok_list := t
 | _ -> raise (ParseError "bad match")
 
 
 
let rec parse_E () =
  let t1 = parse_T () in
  match lookahead () with
    | Tok_Sum ->
        match_tok Tok_Sum;
        let t2 = parse_E () in
        Sum(t1,t2)
    | _ -> t1 

and parse_T () =
  let a = parse_A () in
  match lookahead () with
  (* If we see a *, then continue parsing after matching *)
    | Tok_Mul ->  
        match_tok Tok_Mul;
        let t = parse_T () in
        Mul(a, t)
    | _ -> a

and parse_A () =
  match lookahead () with
    | Tok_Num n ->
        match_tok (Tok_Num n);
        Num (int_of_string n)
    | _ -> raise (ParseError "parse_A")
;;

 
let parse str =
 tok_list := (tokenize str);
 let exp = parse_E () in
 if !tok_list <> [Tok_END] then
   raise (ParseError "parse_E")
 else
   exp
;;
 
(***** Interpreter ****)
 
let rec eval a =
  match a with
    Num n -> n
  | Sum (a1,a2) -> (eval a1) + (eval a2)
  (* New multiplication logic *)
  | Mul (a1,a2) -> (eval a1) * (eval a2)   
  
let eval_str str =
 print_string str; print_string "\n";
 let e = parse str in
 print_string "AST produced = " ;
 print_endline (a_to_str e) ;
 let v = eval e in
 print_string "Value of AST = " ;
 print_int v ;
 print_endline "";
 v
;;
 
 eval_str "1+2*3+4*5";;
 print_string "------------------"; print_string "\n";
 eval_str "1+2*3+4";;
 print_string "------------------"; print_string "\n";
 eval_str "1*2+3*4*5+6";;
 print_string "------------------"; print_string "\n";
 eval_str "2*2+3+4*5*2+1";;
 print_string "------------------"; print_string "\n";