(* Parser for 
    E  -: T '|' E | T
    T  -: F T | F
    F  -: A '?' | A
    A  -: C | '(' E ')'
    C  -: Alphanumeric characters plus '.' *)

(* Scanner Types *)
type token = Tok_Char of char
    | Tok_OR
    | Tok_Q
    | Tok_LPAREN
    | Tok_RPAREN
    | Tok_END

(* AST Types *)
type re = C of char  
    | Concat of re * re
    | Optional of re
    | Alternation of re * re

(* Scanner *)
let scan str =
    let len = String.length str in
    let rec tokenize i =
        if i >= len then [Tok_END]
        else match str.[i] with
            | ' ' | '0' .. '9' | 'a' .. 'z' | 'A' .. 'Z' | '.'  -> 
                Tok_Char str.[i] :: tokenize (i+1)
            | '|' -> Tok_OR :: tokenize (i+1)
            | '?' -> Tok_Q :: tokenize (i+1)
            | '(' -> Tok_LPAREN :: tokenize (i+1)
            | ')' -> Tok_RPAREN :: tokenize (i+1)
            | _ -> failwith ("Unexpected character " ^ String.make 1 str.[i])
    in
    tokenize 0

let parse tokens =

(* This handles Alternation - '|' *)
    let rec parse_E tokens =
        (* t is the parsed AST node, rest are the tokens remaining *)
        let t, rest = parse_T tokens in
        match rest with
        (* If the next token is Tok_OR - the | operator*)
        | Tok_OR :: rest' -> 
            (* Try to parse another 'E' production, after the '|' *)
            let e, final_tokens = parse_E rest' in
            (* Construct an Alternation AST node and return it with any remaining tokens *)
            Alternation(t, e), final_tokens
        (* If there is no '|' operator, return what's been parsed*)
        | _ -> t, rest
        
(* This handles 'concat' nodes *)
    and parse_T tokens =
        (* f is the parsed AST node, rest are tokens remaining *)
        let f, rest = parse_F tokens in
        match rest with
        (* If the next token is a char, (, or an |,*)
        | (Tok_Char _  :: _) | (Tok_LPAREN :: _) | Tok_Q :: _ -> 
            (* continue parsing another 'T' production *)
            let t, final_tokens = parse_T rest in
            (* Construct a Concat AST node *)
            Concat(f, t), final_tokens
        (* If not, just resturn what's been parsed so far *)
        | _ -> f, rest

(* Handles optional sequences - '?' (Tok_Q) *)
    and parse_F tokens =
        (* Tries to parse an 'A' production first *)
        let a, rest = parse_A tokens in
        match rest with
        (* If the next token is a question mark, construct Optional AST node *)
        | Tok_Q :: rest' -> Optional(a), rest'
        (* Otherwise return what was parsed *)
        | _ -> a, rest

 (* Lowest level in the grammar, regex the parathesized expression *)
    and parse_A tokens =
        match tokens with
        (* If the next token is an opening parenthesis *)
        | Tok_LPAREN :: rest -> 
            (* Parse an 'E' production in the parentheses *)
            let e, next_tokens = parse_E rest in
            (match next_tokens with
            (* there should be a closing parenthesis *)
            | Tok_RPAREN :: rest' -> e, rest'
            (* throw error is parenthesis not found *)
            | _ -> failwith "Missing closing parenthesis")
        (* If next token is a character, construct a 'C' AST node with that character *)
        | Tok_Char c :: rest -> C c, rest  
        (* throw error for unexpected token *)
        | _ -> failwith "Unexpected token in parse_A"

(* at the end of the parse function, start parsing with parse_E *)
in
let ast, _ = parse_E tokens in
(* return the abstract syntax tree *)
ast


let rec matcher regex str pos =
    (* pattern matching on the regex, decide what action to take based on the structure *)
    match regex with
        | C '.' -> 
            if pos < String.length str then
                pos + 1
            else 
                -1
        | C c -> 
            if pos < String.length str && str.[pos] = c then
                pos + 1
            else 
                -1
        | Concat(e1, e2) -> 
            let next_pos = matcher e1 str pos in
            if next_pos <> -1 then 
                matcher e2 str next_pos
            else 
                -1
        | Optional e -> 
            let next_pos = matcher e str pos in
            if next_pos <> -1 then 
                next_pos
            (* Since 'e' is optional, return original position *)
            else 
                pos
        | Alternation(e1, e2) -> 
            let next_pos_a = matcher e1 str pos in
            if next_pos_a <> -1 then 
                (*  return position after the matched sequence *)
                next_pos_a
            else 
                matcher e2 str pos


let main () =
    let rec loop () =
        print_string "pattern? ";
        flush stdout; (* flushes the standard output buffer *)
        let pattern = input_line stdin in   (* read input until newline character*)
        let tokens = scan pattern in    
        let ast = parse tokens in
        let rec match_strings () =
            print_string "string? ";
            flush stdout;
            let str = input_line stdin in
            if str = "" then 
                loop ()    (* if empty go back to the outer loop, ask for pattern *)
            else
                if (matcher ast str 0) = String.length str then 
                    print_endline "match"
                else 
                    print_endline "no match";
                match_strings ()
        in
        match_strings ()
    in
    loop ()
;;


let _ = main ()


(* Test cases
   ((h|j)ell. worl?d)|(42)
   I (like|love|hate)( (cat|dog))? people
   *)
