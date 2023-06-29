#ifndef MCC_DEFINE_PUNCTUATOR
#define MCC_DEFINE_PUNCTUATOR(ENUM, STRING, VALUE)
#endif  // MCC_DEFINE_PUNCTUATOR
MCC_DEFINE_PUNCTUATOR(Less          , "<"    , '<'                        )
MCC_DEFINE_PUNCTUATOR(Greater       , ">"    , '>'                        )
MCC_DEFINE_PUNCTUATOR(Not           , "!"    , '!'                        )
MCC_DEFINE_PUNCTUATOR(Add           , "+"    , '+'                        )
MCC_DEFINE_PUNCTUATOR(Sub           , "-"    , '-'                        )
MCC_DEFINE_PUNCTUATOR(Mul           , "*"    , '*'                        )
MCC_DEFINE_PUNCTUATOR(Div           , "/"    , '/'                        )
MCC_DEFINE_PUNCTUATOR(Mod           , "%"    , '%'                        )
MCC_DEFINE_PUNCTUATOR(Dot           , "."    , '.'                        )
MCC_DEFINE_PUNCTUATOR(Ques          , "?"    , '?'                        )
MCC_DEFINE_PUNCTUATOR(Colon         , ":"    , ':'                        )
MCC_DEFINE_PUNCTUATOR(Comma         , ","    , ','                        )
MCC_DEFINE_PUNCTUATOR(Sharp         , "#"    , '#'                        )
MCC_DEFINE_PUNCTUATOR(Assign        , "="    , '='                        )
MCC_DEFINE_PUNCTUATOR(LParen        , "("    , '('                        )
MCC_DEFINE_PUNCTUATOR(RParen        , ")"    , ')'                        )
MCC_DEFINE_PUNCTUATOR(LBrace        , "{"    , '{'                        )
MCC_DEFINE_PUNCTUATOR(RBrace        , "}"    , '}'                        )
MCC_DEFINE_PUNCTUATOR(BitOr         , "|"    , '|'                        )
MCC_DEFINE_PUNCTUATOR(BitNot        , "~"    , '~'                        )
MCC_DEFINE_PUNCTUATOR(LBracket      , "["    , '['                        )
MCC_DEFINE_PUNCTUATOR(RBracket      , "]"    , ']'                        )
MCC_DEFINE_PUNCTUATOR(BitAnd        , "&"    , '&'                        )
MCC_DEFINE_PUNCTUATOR(BitXor        , "^"    , '^'                        )
MCC_DEFINE_PUNCTUATOR(Semicolon     , ";"    , ';'                        )
MCC_DEFINE_PUNCTUATOR(Equal         , "=="   , '=' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(NotEqual      , "!="   , '!' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(LessEqual     , "<="   , '<' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(GreaterEqual  , ">="   , '>' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(Or            , "||"   , '|' | '|' << 8             )
MCC_DEFINE_PUNCTUATOR(And           , "&&"   , '&' | '&' << 8             )
MCC_DEFINE_PUNCTUATOR(Arrow         , "->"   , '-' | '>' << 8             )
MCC_DEFINE_PUNCTUATOR(DSharp        , "##"   , '#' | '#' << 8             )
MCC_DEFINE_PUNCTUATOR(BitShl        , "<<"   , '<' | '<' << 8             )
MCC_DEFINE_PUNCTUATOR(BitShr        , ">>"   , '>' | '>' << 8             )
MCC_DEFINE_PUNCTUATOR(Increase      , "++"   , '+' | '+' << 8             )
MCC_DEFINE_PUNCTUATOR(Decrease      , "--"   , '-' | '-' << 8             )
MCC_DEFINE_PUNCTUATOR(OrAssign      , "|="   , '|' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(AddAssign     , "+="   , '+' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(SubAssign     , "-="   , '-' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(MulAssign     , "*="   , '*' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(DivAssign     , "/="   , '/' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(ModAssign     , "%="   , '%' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(AndAssign     , "&="   , '&' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(XorAssign     , "^="   , '^' | '=' << 8             )
MCC_DEFINE_PUNCTUATOR(ShlAssign     , "<<="  , '<' | '<' << 8 | '=' << 16 )
MCC_DEFINE_PUNCTUATOR(ShrAssign     , ">>="  , '>' | '>' << 8 | '=' << 16 )
MCC_DEFINE_PUNCTUATOR(Ellipsis      , "..."  , '.' | '.' << 8 | '.' << 16 )