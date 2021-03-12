#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <string>
#include <cctype>
#include <cstring>

using namespace std;

///fil
ifstream myfile;
bool endfile=false;

//////////
struct table {
    string var ;
    int type;
    bool isFunc;
    int numOfParams;
    vector<table> list ;
};

////
struct exprVal {
    string tok ;
    int type;
};

// token
bool drop=true;
string token="";
string T;

//count main
int count_main=0;

//count main
int count;

//int numofparams for each function
int nparams=0;

// index indentifier
int index_iden;

//type
int type_num=1;
int type_list=2;
int type_nil=3;

///
bool return_func=false;

/// functions
void MFile();
bool is_number(string s);
bool is_word(string w);
bool is_signes(string s);
bool isKey(string word);
void ungetcharr();
char getcharr();
string NextToken();
bool is_identifier_char(char ch);
void droptoken();
string gettoken();
void prog();
void func();
void body(int type);
void stmt(int type);
void defvar();
int expr();
int or_expr();
int and_expr();
int equality_expr();
int relation_expr();
int add_expr();
int mul_expr();
int unary_expr();
int postfix_expr();
int primary_expr();
int clist(int indexFunc , int countp);
bool searchExistIden(string T);
int Type(string c);
int Type_function(string c);
bool searchIsFunc(string T );
int searchIsFunc_index(string T);
int flist(vector<table> &LISTT);

/////
int line=1;
//count of error
int count_error=0;
/////
bool Main(string word);
bool IF(string word);
bool While(string word);
bool For(string word);
bool Else(string word);
bool In(string word);
bool Num(string word);
bool Return(string word);
bool numread(string word);
bool numprint(string word);
bool List(string word);
bool Listlen(string word);
bool Exit(string word);


// symbolTable
vector<table> symbolTable;
/// main
int main(){
    
    MFile();
    
    return 0;
}

/// gettoken;
string gettoken(){
      string o;
      if(myfile.good() && drop==true && !endfile){
        o=NextToken() ;
        while(o==""){
            o=gettoken();
        }
        drop=false;
        token=o;
        return o;
      }else if(!myfile.good()){
          drop=false;
          endfile=true;
      }
      return token;
}

///droptoken
void droptoken(){
    drop=true;
}

///file
void MFile(){
    
    myfile.open("file3.txt")   ;
    if(myfile.good()){
        
        //
        table numRead;
        numRead.var="numread";
        numRead.type=type_num;
        numRead.isFunc=true;
        numRead.numOfParams=0;
        //
        table numPrint;
        numPrint.var="numprint";
        numPrint.type=0;
        numPrint.isFunc=true;
        numPrint.numOfParams=1;
        //
        table input;
        input.var="input";
        input.type=type_num;
        input.isFunc=false;
        input.numOfParams=0;

        numPrint.list.push_back(input);
        //
        table makelist;
        makelist.var="makelist";
        makelist.type=type_list;
        makelist.isFunc=true;
        makelist.numOfParams=1;
        //
        table number;
        number.var="number";
        number.type=type_num;
        number.isFunc=false;
        number.numOfParams=0;

        makelist.list.push_back(number);
        //
        table listlen;
        listlen.var="listlen";
        listlen.type=type_num;
        listlen.isFunc=true;
        listlen.numOfParams=1;
        //
        table listt;
        listt.var="list";
        listt.type=type_list;
        listt.isFunc=false;
        listt.numOfParams=0;

        listlen.list.push_back(listt);
        //
        table Exit;
        listt.var="exit";
        listt.type=0;
        listt.isFunc=true;
        listt.numOfParams=1;

        Exit.list.push_back(input);

        symbolTable.push_back(numRead);
        symbolTable.push_back(numPrint);
        symbolTable.push_back(makelist);
        symbolTable.push_back(listlen);
        symbolTable.push_back(Exit);

        if(myfile.good()){
             T=gettoken();
             prog();
        }    
        cout<<"count of errors : "<<count_error<<endl;
        myfile.close();
    }else{
        cout<<"file is not open!";
    }
    
}

//prog
void prog(){
     func();
     if(myfile.good()){
        droptoken();
        T=gettoken();
        if(is_word(T) || Main(T)){
             droptoken();
             prog();
        }else if(isKey(T)){
             droptoken();
             cout<<"line "<<line <<" :  error\n";
             count_error++;
             ////////////////////////////
             prog();
        }
     }
}
//func
void func(){
     if(is_word(T)){
         int T_type=Type_function(T);
         if(T_type!=type_num && T_type!=type_list && T_type!=type_nil){
            cout<<"line "<<line <<" : type  "<<T<<" not exist \n";
            count_error++;
         }
         if(myfile.good()){
             droptoken();
             T=gettoken();
          if(is_word(T) || Main(T)){
            string funcName=T;  
            
            bool exist = searchIsFunc(T );
            int index = searchIsFunc_index(T);
            if(Main(T) && count_main==0){
                count_main++;
                exist=false;
            }
            int symbolSize;
            if(exist){
                symbolSize= symbolTable.size();
                cout<<"line "<<line <<" : "<<T<<"  function declared before or function name same as if , while , for .... \n";
                count_error++;
            }   
            table s;
            s.var=T;
            s.isFunc=true;
            s.type=T_type;
            if(Main(T) && T_type!=type_num){
                cout<<"line "<<line <<" : "<<"type of main function should be num \n";
                count_error++;
            }
                if(myfile.good()){
                    droptoken();
                    T=gettoken();
                    if(T=="("){
                        if(myfile.good()){
                            droptoken();
                            T=gettoken();
                            nparams=0;
                            s.numOfParams=flist(s.list);  
                            if(Main(funcName) && nparams>0){
                                cout<<"line "<<line<<" : main function should'nt have input parametrs\n";
                                count_error++;
                            }
                            if(T==")"){
                                symbolTable.push_back(s);
                                if(!exist){
                                    symbolSize= symbolTable.size();
                                }
                                for(int j=0 ;j<s.list.size(); j++){
                                    symbolTable.push_back(s.list[j]);
                                }

                                if(myfile.good()){
                                    droptoken();
                                    T=gettoken();
                                    if(T=="{"){
                                        if(myfile.good()){
                                            droptoken();
                                            T=gettoken();
                                            droptoken();
                                            return_func==false;
                                            
                                            body(T_type);
                                            if(T!="}"){
                                                cout<<"line "<<line <<"  :} is missing \n";
                                                count_error++;
                                                droptoken();
                                            }

                                            if((T_type==type_list || T_type==type_num) && return_func==false){
                                                cout<< funcName<<" function  should have return value\n";
                                                count_error++;
                                            }

                                            while(symbolSize != symbolTable.size()){
                                                symbolTable.erase(symbolTable.end());
                                            }
                                        }
                                    }else{
                                        cout<<"line "<<line <<" :  body should be declear in {}\n";
                                        count_error++;
                                    }
                                }
                            }else{
                                cout<<"line "<<line <<" :  not execpt )\n";        
                                count_error++;
                            }
                        }
                    }else{
                        cout<<"line "<<line <<" : not execpt (\n";
                        count_error++;
                    }
                }
         }else if(isKey(T)){
             droptoken();
             cout<<"line "<<line <<" : key word can be redfined \n";
             count_error++;
         }else{
             cout<<"line "<<line <<" : is ambiguse\n";
             count_error++;
         }     
     }

     }else{
         cout<<"line "<<line <<" :name of type of function  shoulde be word \n";
         count_error++;
     }
}

//body
void body(int type){
    if(T!="}" && myfile.good()){
       stmt(type);
    }
    
    while( myfile.good()  &&  T!="}" ){
        T=gettoken();
        droptoken();
        stmt(type);
    }
     
}

//stmt
void stmt(int type){
 
    if(IF(T)){
        if(myfile.good()){
            droptoken();
            T=gettoken();
            if(T=="("){
                if(myfile.good()){
                    droptoken();
                    T=gettoken();
                    expr();
                    if(T==")"){
                        if(myfile.good()){
                            droptoken();
                            T=gettoken();
                            stmt(type);
                            if(Else(T)){
                                if(myfile.good()){
                                    droptoken();
                                    T=gettoken();
                                    stmt(type);
                                }
                            }
                        }
                    }else{
                        cout<<"line "<<line << " : error )\n";    
                        count_error++;
                    }
                }
            }else{
                cout<<"line "<<line << " : error (\n";
                count_error++;
            }
        }
    }else if(While(T)){
        if(myfile.good()){
            droptoken();
            T=gettoken();
            if(T=="("){
                if(myfile.good()){
                    droptoken();
                    T=gettoken();
                    expr();
                    ////////////////////////////////////////
                    if(T==")"){
                        if(myfile.good()){
                            droptoken();
                            T=gettoken();
                            stmt(type);
                        }
                    }else{
                        cout<<"line "<<line << " : error )\n";    
                        count_error++;
                    }
                }
            }else{
                cout<<"line "<<line << " : error (\n";
                count_error++;
            }
        }
    }else if(For(T)){
        if(myfile.good()){
            droptoken();
            T=gettoken();
     
            if(T=="("){
                if(myfile.good()){
                    droptoken();
                    T=gettoken();
                
                    if(is_word(T)){
                        bool isIden=searchExistIden(T);
                        if(!isIden){
                            cout<<"line "<<line<<" : identifier "<< T<<" is not exist \n";
                            count_error++;
                        }
                        if(symbolTable[index_iden].type!=type_num){
                            cout<<"line "<<line<<" : type of identifier "<< T<<" shoulde be num \n";
                            count_error++;
                        }
                        if(myfile.good()){
                            droptoken();
                            T=gettoken();
                            if(In(T)){
                                if(myfile.good()){
                                    droptoken();
                                    T=gettoken();
                                    expr();
                                    if(T==")"){
                                        if(myfile.good()){
                                            droptoken();
                                            T=gettoken();
                                            stmt(type);
                                        }
                                    }else{
                                        cout<<"line "<<line << " : error )\n";
                                        count_error++;
                                    }
                                }
                            }else{
                               cout<<"line "<<line << " : error in for \n";
                               count_error++;
                            }
                        }
                    }else{
                        cout<<"line "<<line << " : rong  iden for iden in for\n";
                        count_error++;
                    }
                }      
            }else{
                cout<<"line "<<line << " : error (\n";
                count_error++;
            }
        }
    }else if(Return(T)){
      if(type==type_nil){
            cout<<"line "<<line << " : this function shouldent have return value \n";                                
            count_error++;
      }  
  
        return_func=true;
        if(myfile.good()){
            droptoken();
            T=gettoken();

            int type_ret=expr();
            if(type!=type_ret && type!=type_nil){
                cout<<"line "<<line << " : the return type isnt same as function type \n";
                count_error++;
            }
            if(T!=";"){
                cout<<"line "<<line << " : error ; \n";
                count_error++;
            }
            if(myfile.good()){
                droptoken();
                T=gettoken();
            }
        }
    }else if(T=="{"){
   
        int count_iden_F_I_W=symbolTable.size();
        if(myfile.good()){
            droptoken();
            T=gettoken();
            body(type);
            //////////////////////////////////////////////////
        }
        if(T!="}"){
            cout<<"line "<<line << " : error } \n";
            count_error++;
        }
        while(count_iden_F_I_W != symbolTable.size()){
            symbolTable.erase(symbolTable.end());
        }

        if(myfile.good()){
            droptoken();
            T=gettoken();
        }
    }else{
        int y =Type(T); 
        if(y==1 || y==2){
            defvar();
            if(myfile.good()){
                T=gettoken();
                if(T!=";"){
                    cout<<"line "<<line << " : error ; \n";
                    count_error++;
                }else{
                    if(myfile.good()){
                        droptoken();
                        T=gettoken();
                    }
                }
            }
        }else{
         ///////////////////////////////////////////////
           int t= expr();
      
            if(T!=";"){
                cout<<" line "<<line << " : error ; \n";
                count_error++;
            }else{
                if(myfile.good()){
                    droptoken();
                    T=gettoken();
                }
            }    
        } 
    }    
}

//defvar
void defvar(){
    int T_type=Type(T);
    if(T_type!=type_num && T_type!=type_list){
        cout<<"line "<<line <<" : type  "<<T<<" not exist \n";
        count_error++;
    }
    if(myfile.good()){
        droptoken();
        T=gettoken();
        droptoken();

        if(!is_word(T)){
            cout<<"line "<<line<<" : identifire name shoulde be word\n";
            count_error++;
        }
        if(isKey(T)){
            cout<<"line "<<line<<" :  identifire name sholudent be key value\n";
            count_error++;
        }

        if(!searchExistIden(T)){
            table h;
            h.isFunc=false;
            h.var=T;
            h.type=T_type;
            h.numOfParams=0;
            symbolTable.push_back(h);
        }else{
            cout<<"line "<<line<<" : identifier  "<< T<<"   declared before\n";
            count_error++;
        }
    }
}
//////////////////////////////////////////////
//expr
int expr(){

    int t1=or_expr();

    int y=t1;
    while(myfile.good()  &&  T=="="){
        droptoken();
        T=gettoken();
        int t2=or_expr();
        if(t2!=y && y!=-1 && t2!=-1 ){
        
            cout<<"line "<<line<<" : illegal assignment!  = \n";
            count_error++;
        }
        y=t2;

    }
    return t1;
}

int or_expr(){
     int t1=and_expr();
     int y=t1;
     while(myfile.good()  &&  T=="||"){
        droptoken();
        T=gettoken();
        int t2=and_expr();
        if(t2!=y && y!=-1  && t2!=-1){
            
            cout<<"line "<<line<<" : illegal assignment!  = \n";
            count_error++;
        }
        y=t2;
     }
     return t1;
}
int and_expr(){

     int t1=equality_expr();
     int y=t1;
     while(myfile.good()  &&  T=="&&"){
        droptoken();
        T=gettoken();
        int t2=equality_expr();
        if(t2!=y  && y!=-1  && t2!=-1){
            //t1=-1;
            cout<<"line : "<<line<<"  illegal assignment!  = \n";
            count_error++;
        }
        y=t2;
     }
     return t1;
}

int equality_expr(){
    
     int t1=relation_expr();
     int y=t1;
     while(myfile.good()  &&  (T=="=="  ||  T=="!=")){
        droptoken();
        T=gettoken();
        int t2=relation_expr();
        if(y!=t2  &&  y!=-1  && t2!=-1){
            t1=-1;
            cout<<"line "<<line<<" : illegal assignment!  == or != \n";
            count_error++;
        }
        y=t2;
     }
     return t1;
}

int relation_expr(){
     int t1=add_expr();
     int y=t1;
     while(myfile.good()  &&  (T=="<"  ||  T==">"  ||  T=="<=" || T==">=")){
        droptoken();
        T=gettoken(); 
        int t2=add_expr();
        if(y!=t2  && y!=-1  && t2!=-1){
            cout<<"line "<<line<<" : illegal assignment!  < or > or <= or >= \n";
            t1=-1;
            count_error++;
        }
        y=t2;
     }
     return t1;
}

int add_expr(){
     int t1=mul_expr();

     if(t1!=type_num && t1!=-1 &&  (T=="-"  ||  T=="+")){
            cout<<"line "<<line<<" : incompatible operands! + - \n";
            t1=-1;
            count_error++;
     }
     while(myfile.good()  &&  (T=="-"  ||  T=="+")){
        droptoken();
        T=gettoken(); 
        int t2=mul_expr();
        if(t2!=type_num && t2!=-1){
          cout<<"line : "<<line<<"   incompatible operands! + - \n";
          count_error++;
        }
     }
     return t1;
}

int mul_expr(){
   
     int t1=unary_expr();
   
     if(t1!=type_num  && t1!=-1 && (T=="*"  ||  T=="/"  ||  T=="%")){
        cout<<"line : "<<line<<"   incompatible operands! / * % \n";
        t1=-1;
        count_error++;
     }
     while(myfile.good()  &&  (T=="*"  ||  T=="/"  ||  T=="%")){
        droptoken();
        T=gettoken();
        int t2=unary_expr();
        if(t2!=type_num && t2!=-1){
          cout<<"line : "<<line<<"   incompatible operands! / * % \n";
          count_error++;
        }
     }
     return t1;
}

int unary_expr(){
   
     if(T=="!"  ||  T=="+"  ||  T=="-"){
         while(myfile.good()  &&  (T=="!"  ||  T=="+"  ||  T=="-")){
           droptoken();
           T=gettoken();  
           int t1=postfix_expr();
           if(t1!=type_num && t1!=-1){
             t1=-1;
             cout<<"line "<<line<<" : incompatible operands! \n";
             count_error++;
           }  
           return t1;
        }
     }else{
         ////////////////////////////////////////////
         int t2=postfix_expr();
         return t2;
     }
}
////l
int postfix_expr(){
   
     int t1=primary_expr();

     if(t1==type_num && T=="["){
           cout<<"line "<<line<<" : incompatible operands! name list\n";
           count_error++;
     }

     while(myfile.good()  &&  T=="["){
        droptoken();
        T=gettoken();
        ///////////////////////////////////////////////////////
        int t1= expr();
        ///////////////////////////////////////////////////////
        
        if (T=="]") {
           droptoken();
           T=gettoken();
           
           //////////////////////////////////////////////////////////
           return type_num;
        }else{
           cout<<"line "<<line<<" : error  ] \n";
           count_error++;
           droptoken(); 
           T=gettoken();
           ///////////////////////////////////////////////
           return -1;

        }
     }
     return t1;

}
///l
int primary_expr(){

      if(is_word(T)){
        if(myfile.good()){
            droptoken();
            string T1=T;
            T=gettoken();
            if(T=="("){

                bool isfunc=searchIsFunc(T1 );
                int index;
                index = searchIsFunc_index(T1);
                if(isfunc){
                    if(myfile.good()){
                        droptoken();
                        T=gettoken();

                        int  callFunctionsParams=0;

                        if(T!=")"){
                            callFunctionsParams=clist(index,callFunctionsParams);
                        }

                        if(callFunctionsParams!=symbolTable[index].numOfParams ){
                           cout<<"line "<<line<<" : number of parametrs is not same as "<<T1<<"  function \n";        
                           count_error++;
                        }
                        if(myfile.good()){
                           droptoken();
                           T=gettoken();
                        }
                    }
                    return symbolTable[index].type;
                }else{
                    cout<<"line "<<line<<" : function  "<< T1<<"  is not declared \n";
                    count_error++;
                    if(myfile.good()){
                        droptoken();
                        T=gettoken();

                        int callFunctionsParams=0;
                        
                        if(T!=")"){
                            callFunctionsParams=clist(-1, callFunctionsParams);
                        }
                        if(myfile.good()){
                           droptoken();
                           T=gettoken();
                        }
                    }
                    return -1;
                }
            }else{
                bool exist=searchExistIden(T1);
                if(!exist){
                    cout<<"line "<<line<<" : identifier  "<< T1<<"  is not declared \n";
                    count_error++;
                    return -1;
                }    
                return symbolTable[index_iden].type;
            }
        }
    }else if(is_number(T)){ 
        if(myfile.good()){
            droptoken();
            T=gettoken();
        }

        return type_num;
    }else if(T=="("){
         if(myfile.good()){
             droptoken();
             T=gettoken();
             ////////////////////////////////////////////
             int t1=expr();
             ///////////////////////////////////////////
             if(T!=")"){
                 cout<<"line "<<line <<" : error )\n";
                 count_error++;
             }
             if(myfile.good()){
                droptoken();
                T=gettoken();
             }
             return t1;
         }
    }else{
         cout<<"line "<<line <<" : syntax error\n";
         count_error++;
         if(myfile.good()){
                droptoken();
                T=gettoken();
         }

    }
}

////flist
int flist(vector<table> &LISTT){
   
    if(T!=")"){
        int T_type=Type(T);
        if(T_type!=type_num && T_type!=type_list){
            cout<<"line "<<line <<" : type  "<<T<<" not exist \n";
            count_error++;
        }
        if(myfile.good()){
            droptoken();
            T=gettoken();

            if(!is_word(T)){
                cout<<"line "<<line <<" : identifire name sholude be word  \n";
                count_error++;
            }
            if(isKey(T)){
               cout<<"line "<<line<<" :  identifire name sholude'nt be key value\n";
               count_error++;
            }
                nparams++;
                table s;
                s.type=T_type;
                s.isFunc=false;
                s.var=T;
                s.numOfParams=0;
                /////////////////////////////
                LISTT.push_back(s);
                ////////////////////////////
                if(myfile.good()){
                  droptoken();
                  T=gettoken();
                    if(T==","){
                        if(myfile.good()){
                          droptoken();
                          T=gettoken();
                          nparams=flist(LISTT);
                        }
                    }else if(T==")"){
                        nparams=flist(LISTT);
                    }else{
                        cout<<"line "<<line <<" : error  , \n";
                        count_error++;
                        if(myfile.good()){
                          droptoken();
                          T=gettoken();
                          nparams=flist(LISTT);
                        }
                    }
                }
        }
    }
    return nparams;
}

//clist
int clist(int indexFunc , int countp){
        countp++;
        string TT=T;
        int t1=expr();
         if(indexFunc!=-1 && t1!=-1){
             if(t1!=symbolTable[indexFunc].list[countp-1].type){
                 cout<<"line "<<line<<" : type of input params of function is not same as function you call "<<endl;
                 count_error++;
             }
         }
        if(T==","){
            if(myfile.good()){
                droptoken();
                T=gettoken();
                countp=clist(indexFunc , countp);
            }
        }else if(T==")"){
            return countp;
        }else{
            cout<<"line "<<line <<" : clist error )\n";
            count_error++;
            T=gettoken();
            droptoken();
            return countp;
        }

    
    return countp;
}

//Type
int Type(string c){
    if(c=="num"){
        return 1;
    }else if(c=="list"){
        return 2;
    }
   
    return -1;
}

//Type function
int Type_function(string c){
    if(c=="num"){
        return 1;
    }else if(c=="list"){
        return 2;
    }else if(c=="nil"){
        return 3;
    }

    return -1;
}


// iden // word
bool is_word(string w){
    if(w.size()==0) return false;
    if(w[0]=='_' || ( (int)w[0]>=65 && (int)w[0]<=90 ) || ((int)w[0]>=97 && (int)w[0]<=122) ){
        for(int i=1;i<w.size();i++) {
            if((w[i]=='_' || ( (int)w[i]>=65 && (int)w[i]<=90 ) || ((int)w[i]>=97 && (int)w[i]<=122) || ((int)w[i]>=48 && (int)w[i]<=57) )==false)  {
                return false ;
            }
        }
    }else{
        return false;
    }
    return true;
}

int searchIsFunc_index(string T){
    for( int i=0 ; i<symbolTable.size() ; i++){
         if(symbolTable[i].isFunc){
             if(symbolTable[i].var==T){
                 return i;
             }
         }
     }
     return -1;
}
//search
bool searchIsFunc(string T){
     for( int i=0 ; i<symbolTable.size() ; i++){
         if(symbolTable[i].isFunc){
             if(symbolTable[i].var==T){
                 return true;
             }
         }
     }
     /////////////////////////////////////////////
     return isKey(T);
     
}

bool searchExistIden(string T){
     for( int i=0 ; i<symbolTable.size() ; i++){
         if(!symbolTable[i].isFunc){
             if(symbolTable[i].var==T){
                 index_iden=i;
                 return true;
             }
         }
     }
     /////////////////////////////////
     return isKey(T);
     
}

bool Main(string word){
    if (word=="main") {
        return true;
    }
    return false;
}

bool IF(string word){
    if (word=="if") {
        return true;
    }
    return false;
}
bool While(string word){
    if (word=="while") {
        return true;
    }
    return false;
}
bool For(string word){
    if (word=="for") {
        return true;
    }
    return false;
}
bool Else(string word){
    if (word=="else") {
        return true;
    }
    return false;
}
bool In(string word){
    if (word=="in") {
        return true;
    }
    return false;
}
bool Num(string word){
    if (word=="num") {
        return true;
    }
    return false;
}
bool Return(string word){
    if (word=="return") {
        return true;
    }
    return false;
}
bool numread(string word){
    if (word=="numread") {
        return true;
    }
    return false;
}
bool numprint(string word){
    if (word=="numprint") {
        return true;
    }
    return false;
}
bool MakeList(string word){
    if (word=="makelist") {
        return true;
    }
    return false;
}
bool Listlen(string word){
    if (word=="listlen") {
        return true;
    }
    return false;
}
bool Exit(string word){
    if (word=="exit") {
        return true;
    }
    return false;
}
bool isKey(string word){
    return Main(word) || IF(word) || While(word) || For(word) || Else(word) || In(word)
        || Num(word) || Return(word) || numread(word) || numprint(word) || MakeList(word)
        || Listlen(word) || Exit(word);
}

///////////////////////


/// is_identifier_char
bool is_identifier_char(char ch){
    int asci = (int)ch;
    if( (ch == '_') || (asci>=65 && asci<=90) || (asci>=97 && asci<=122) ){
        return true;
    }else{
        return false;
    }
}

char getcharr(){
    char c1;
    c1=myfile.get();
    return c1;
}

void ungetcharr(){
    myfile.unget();
}

///nextToken
string NextToken(){
    char c;
    if(myfile.good()){
       c=getcharr();
    }

    while( (c==' ' || c=='\n' || c=='\r') && myfile.good()){
        if(c=='\n'){
            line++;
        }
        c=getcharr();
    }
    
    /// comments
    if(c=='#'){
        string g;
        getline(myfile,g);
        if(myfile.good()){
            c=getcharr();
        }else{
            return "";
        }
    }
    ////////////////
    /////////////////////////////////////////////////
    if(c==':'){
       if(myfile.good()){
            c=getcharr();
            if(c=='='){
                return ":=";
            }
            ungetcharr();
            return ":";
        }    
    }else if(c=='^'){
       if(myfile.good()){
           c=getcharr();
           if(c==';'){
               return "^;";
           }
           ungetcharr();
           return "^";
       }
       return "^" ;
    }else if(c=='~'){
       return "~";
    }else if(c==';'){
        return ";";
    }else if(c==','){
        return ",";
    }else if(c=='('){
        return "(";
    }else if(c==')'){
        return ")";
    }else if(c=='{'){
        return "{";
    }else if(c=='}'){
        return "}";
    }else if(c=='['){
        return "[";
    }else if(c==']'){
        return "]";
    }else if(c=='|'){
        if(myfile.good()){
            c=getcharr();
            if(c=='|'){
                return "||";
            }else{
                ungetcharr();
            }
        }    
        return "";
    }else if(c=='&'){
        if(myfile.good()){
            c=getcharr();
            if(c=='&'){
                return "&&";
            }
        }
        ungetcharr();

    }else if(c=='!'){
        if(myfile.good()){
            c=getcharr();
            if(c=='='){
                return "!=";
            }
        }    
        ungetcharr();
        return "!";
    }else if(c=='<'){
        if(myfile.good()){
            c=getcharr();
            if(c=='='){
                return "<=";
            }
        }
        ungetcharr();
        return "<";
    }else if(c=='>'){
        if(myfile.good()){
            c=getcharr();
            if(c=='='){
                return ">=";
            }
        }    
        ungetcharr();
        return ">";
    }else if(c=='='){
        if(myfile.good()){
            c=getcharr();
            if(c=='='){
                return "==";
            }
        }    
        ungetcharr();
        return "=";    
    }else if(c=='+'){
        return "+";    
    }else if(c=='-'){
        return "-";
    }else if(c=='*'){
        return "*";
    }else if(c=='/'){
        return "/";
    }else if(c=='%'){
        return "%";
    }else if(isdigit(c) && myfile.good()){        
        
        string tok="";
        tok += c;
        if(myfile.good()){    
            c=getcharr();
            while( (isdigit(c) || is_identifier_char(c) || c=='.' )  && myfile.good()){
                 tok += c;
                c=getcharr();
            }
            ungetcharr();
        }
        return tok;
    }else if(is_identifier_char(c) && myfile.good()){
         
         string tok="";
         tok+=c;
         if(myfile.good()){ 
             c=getcharr();
             while((is_identifier_char(c)  ||  isdigit(c) || c=='.') && myfile.good()){
                 tok+=c;
                 c=getcharr();
             } 
             ungetcharr();
         } 
         return tok;
    }
   return "";
}



///signes
bool is_signes(string s){
    if(s.size()==0) return false;
    if(s.size()==1 && ( s==" " ||  s=="\n"  || s=="\t" || s=="\r")) return false ;
    for(int i=0;i<s.size();i++) {
         if( (s[i]>='0' && s[i]<='9') || ((int)s[i]>=65 && (int)s[i]<=90 ) || ((int)s[i]>=97 && (int)s[i]<=122) ){
             return false;
         }
    }
    return true;
}

// number
bool is_number(string s) {
    if(s.size()==0) return false;
    for(int i=0;i<s.size();i++) {
        if((s[i]>='0' && s[i]<='9')==false) {
            return false;
        }
    }
    return true;
}