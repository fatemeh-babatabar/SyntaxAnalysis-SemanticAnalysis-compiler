# BNF

The bnf structure of the Tslang language (This grammar is ambiguous) :
         
      prog ::=    func | func prog
      func ::=    type iden ( flist ) { body } |
      body ::=    stmt | stmt body
      stmt ::=    expr ; | 
                  defvar ; |
                  if ( expr ) stmt |
                  if ( expr ) stmt else stmt |
                  while ( expr ) stmt |
                  for ( iden in expr ) stmt |
                  return expr ; |
                  { body }
      defvar ::=  type iden
      expr ::=    iden ( clist ) |
                  expr [ expr ] |
                  expr = expr |
                  expr + expr |
                  expr - expr |
                  expr * expr |
                  expr / expr |
                  expr % expr |
                  expr < expr |
                  expr > expr |
                  expr == expr |
                  expr != expr |
                  expr <= expr |
                  expr >= expr |
                  expr || expr |
                  expr && expr |
                  ! expr |
                  - expr |
                  + expr |
                  ( expr ) |
                  iden |
                  num
      flist ::=     |
                  type iden |
                  type iden , flist
      clist ::=     |
                  expr |
                  expr , clist
      type ::=    num |
                  list
      num ::=     [0-9]+
      iden ::=    [a-zA-Z_][a-zA-Z_0-9]*

      
  # example 
  
  input ( file3.txt) :
      
         num main(){
             num b;
             b=a+9;
         }
         
         nil func(){
             list A;
             A=1;
         }
   
  output:
  
         line 3 : identifier  a  is not declared 
                  main function  should have return value
         line 8 : illegal assignment!  = 
         count of errors : 3
  

 
