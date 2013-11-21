grammar intent;

COMMA: ',';
PLUS: '+';
MINUS: '-';
TIMES: '*';
DIVIDE: '/';
MOD: '%';
POWER: '**';
AND: '&&';
OR: '||';
BIT_AND: '&';
BIT_OR: '|';
BIT_XOR: '^';
IS_EQUAL: '==';
IS_GT: '>';
IS_GTE: '>=';
IS_LT: '<';
IS_LTE: '<=';
EQUALS: '=';
HASH: '#';
PIPE: '|';
LINE_BREAK: '\n';
COLON: ':';
LPAREN: '(';
RPAREN: ')';    
WORD: [-a-z\'_]+ ;    
MARK: [-+][a-z_]+;
DOT: '.';    
DOT_MAYBE: '.?';

phrase
    : WORD+
    ;

marks
    : MARK+
    ;
    
type
    : phrase
    ;
    
namespace_delim
    : DOT | DOT_MAYBE
    ;
    
namespace
    : phrase DOT
    ;

call
    : namespace* phrase LPAREN arglist RPAREN
    ;
    
arg
    : expr
    ;
    
arglist
    : (arg (COMMA arg)?)?
    ;
    
param
    : named_noun
    ;
    
paramlist
    : (param (COMMA param)?)?
    ;
    
value
    : NUMBER | STR | DATE | REGEX
    ;
    
expr
    : value | unary_expr | binary_expr | ternary_expr | call | group
    ;
    
group
    : LPAREN expr RPAREN
    ;
    
NOT
    : 'not'
    ;
    
IF
    : 'if'
    ;
    
LOOP
    : 'loop'
    ;
    
unary_operator
    : NOT
    ;
    
binary_operator
    : PLUS | MINUS | TIMES | DIVIDE | MOD | POWER
    | AND | OR | XOR | BIT_AND | BIT_OR | BIT_XOR
    | IS_EQUAL | IS_GT | IS_GTE | IS_LT | IS_LTE
    ;
    
binary_expr
    : operand binary_operator operand
    ;
    
loop
    : LOOP prefixed_block_remainder COLON nested_block
    ;
    
conditional
    : IF prefixed_block_remainder COLON nested_block
    ;
    
operand
    : expr
    ;
    
unary_expr
    : unary_operator operand
    ;
    
ternary_expr
    : expr '?' expr ':' expr
    ;

short_form
    : LPAREN ABBREV RPAREN
    ;

named_noun
    : phrase short_form? COLON marks? type? assignment_sans_lhs? nested_block?
    ;
    
assignment_sans_lhs
    : EQUALS expr
    ;
    
comment
    : HASH rest_of_statement
    ;
    
doc_comment
    : PIPE rest_of_statement
    ;

block
    : statement*
    ;
    
rest_of_statement
    : .*? LINE_BREAK
    ;

statement
    : named_noun | loop | conditional | doc_comment | comment
    ;
    
nested_block
    : INDENT block DEDENT
    ;
    
prefixed_block_remainder
    : phrase LPAREN expr RPAREN COLON
    ;