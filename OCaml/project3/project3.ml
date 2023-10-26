type token =
    | Tok_Word of string
    | Tok_OR
    | Tok_Q
    | Tok_LPAREN
    | Tok_RPAREN
    | Tok_END

type re =
    | C of string  (* Change from char to string *)
    | Concat of re * re
    | Optional of re
    | Alternation of re * re


let parse tokens =
    let rec parse_E tokens =
        let t, rest = parse_T tokens in
        match rest with
        | Tok_OR :: rest' -> 
            let e, final_tokens = parse_E rest' in
            Alternation(t, e), final_tokens
        | _ -> t, rest

    and parse_T tokens =
        let f, rest = parse_F tokens in
        match rest with
        | (Tok_Word _  :: _) | (Tok_LPAREN :: _) -> 
            let t, final_tokens = parse_T rest in
            Concat(f, t), final_tokens
        | _ -> f, rest

    and parse_F tokens =
        let a, rest = parse_A tokens in
        match rest with
        | Tok_Q :: rest' -> Optional(a), rest'
        | _ -> a, rest

    and parse_A tokens =
        match tokens with
        | Tok_LPAREN :: rest -> 
            let e, next_tokens = parse_E rest in
            (match next_tokens with
             | Tok_RPAREN :: rest' -> e, rest'
             | _ -> failwith "Missing closing parenthesis")
        | Tok_Word w :: rest -> C w, rest  (* This line is updated *)
        | _ -> failwith "Unexpected token in parse_A"


    in
    let ast, _ = parse_E tokens in
    ast

let rec match_from_position re str pos =
    if pos > String.length str then false
    else
        match re with
        | C w -> 
            let len = String.length w in
            if pos + len <= String.length str && (String.sub str pos len) = w then true
            else false
        | Concat(a, b) -> 
            let found = ref false in
            for i = pos to String.length str do
                if match_from_position a str pos && match_from_position b str i then
                    found := true
            done;
            !found
        | Optional a -> 
            match_from_position a str pos || true
        | Alternation(a, b) -> 
            match_from_position a str pos || match_from_position b str pos

let match_re re str = 
    match_from_position re str 0




let scan s =
    let len = String.length s in
    let rec scan_aux i acc =
        if i >= len then (if acc = "" then [Tok_END] else [Tok_Word acc; Tok_END])
        else match s.[i] with
            | ' ' -> if acc = "" then scan_aux (i+1) acc else Tok_Word acc :: scan_aux (i+1) ""
            | '0' .. '9' | 'a' .. 'z' | 'A' .. 'Z' | '.' -> scan_aux (i+1) (acc ^ (String.make 1 s.[i]))
            | '|' -> if acc = "" then Tok_OR :: scan_aux (i+1) "" else Tok_Word acc :: Tok_OR :: scan_aux (i+1) ""
            | '?' -> if acc = "" then Tok_Q :: scan_aux (i+1) "" else Tok_Word acc :: Tok_Q :: scan_aux (i+1) ""
            | '(' -> if acc = "" then Tok_LPAREN :: scan_aux (i+1) "" else Tok_Word acc :: Tok_LPAREN :: scan_aux (i+1) ""
            | ')' -> if acc = "" then Tok_RPAREN :: scan_aux (i+1) "" else Tok_Word acc :: Tok_RPAREN :: scan_aux (i+1) ""
            | _ -> failwith ("Unexpected character " ^ String.make 1 s.[i])
    in
    scan_aux 0 ""


let rec pretty_print re =
    match re with
    | C w -> Printf.sprintf "C '%s'" w
    | Concat(a, b) -> Printf.sprintf "Concat(%s, %s)" (pretty_print a) (pretty_print b)
    | Optional a -> Printf.sprintf "Optional(%s)" (pretty_print a)
    | Alternation(a, b) -> Printf.sprintf "Alternation(%s, %s)" (pretty_print a) (pretty_print b)



let main () =
    print_endline "Welcome to the regex matcher!";
    let rec loop () =
        print_string "pattern? ";
        flush stdout; (* Ensure that the output is displayed immediately *)
        let pattern = input_line stdin in
        let tokens = scan pattern in
        let ast = parse tokens in
        print_endline (pretty_print ast);  
        let rec match_strings () =
            print_string "string? ";
            flush stdout; (* Ensure that the output is displayed immediately *)
            let str = input_line stdin in
            if match_re ast str then print_endline "match"
            else print_endline "no match";
            match_strings ()
        in
        match_strings ()
    in
    loop ()
;;


main ()
