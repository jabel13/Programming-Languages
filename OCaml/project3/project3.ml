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
        let t, rest = parse_T tokens in
        (* If the OR token is found, continue and construct an alternation node *)
        match rest with
        | Tok_OR :: rest' -> 
            let e, final_tokens = parse_E rest' in
            Alternation(t, e), final_tokens
        | _ -> t, rest

(* This handles 'concat' nodes *)
    and parse_T tokens =
        let f, rest = parse_F tokens in
    (* If next token is a word or an opening parenthesis, 
       continue parsing and construct concat node*)
        match rest with
        | (Tok_Char _  :: _) | (Tok_LPAREN :: _) | Tok_Q :: _ -> 
            let t, final_tokens = parse_T rest in
            Concat(f, t), final_tokens
        | _ -> f, rest

    (* Handles optional sequences - '?' (Tok_Q) *)
    and parse_F tokens =
        let a, rest = parse_A tokens in
        match rest with
        | Tok_Q :: rest' -> Optional(a), rest'
        | _ -> a, rest

 (* Lowest level in the grammar, regex the parathesized expression *)
    and parse_A tokens =
        match tokens with
        | Tok_LPAREN :: rest -> 
            let e, next_tokens = parse_E rest in
            (match next_tokens with
            | Tok_RPAREN :: rest' -> e, rest'
            | _ -> failwith "Missing closing parenthesis")
        | Tok_Char c :: rest -> C c, rest  
        | _ -> failwith "Unexpected token in parse_A"


    in
    let ast, _ = parse_E tokens in
    ast

    let rec match_from_position re str pos =
        match re with
        | C '.' ->  (* Handling the dot character *)
            if pos < String.length str then
                Some (pos + 1)
            else None
        | C c -> 
            if pos < String.length str && str.[pos] = c then
                Some (pos + 1)
            else None
        | Concat(a, b) -> 
            (match match_from_position a str pos with
             | Some next_pos -> match_from_position b str next_pos
             | None -> None)
        | Optional a -> 
            (match match_from_position a str pos with
            | Some next_pos -> Some next_pos
            | None -> Some pos)
        | Alternation(a, b) -> 
            (match match_from_position a str pos with
            | Some next_pos -> Some next_pos
            | None -> match_from_position b str pos)


let check_pattern re str =
    match match_from_position re str 0 with
    | Some pos when pos = String.length str -> true
    | _ -> false

let main () =
    print_endline "Welcome to the regex matcher!";
    let rec loop () =
        print_string "pattern? ";
        flush stdout; (* Ensure that the output is displayed immediately *)
        let pattern = input_line stdin in
        let tokens = scan pattern in
        let ast = parse tokens in
        (* print_endline (pretty_print ast);   *)
        let rec match_strings () =
            print_string "string? ";
            flush stdout; (* Ensure that the output is displayed immediately *)
            let str = input_line stdin in
            if str = "" then loop ()
            else begin
                if check_pattern ast str then print_endline "match"
                else print_endline "no match";
                match_strings ()
            end
        in
        match_strings ()
    in
    loop ()
;;


let _ = main ()
