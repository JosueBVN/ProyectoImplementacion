#include<stdio.h>
#include <string.h>
#include <fstream>  // Función de biblioteca de flujo de archivos
#include <iostream>
using namespace std;

char reserveWord[32][20]={ // Codificación de palabra reservada 1-32
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

char singleOperator[24][10]={  // Codificación de operador de un solo byte 33-57
    "+", "-", "*", "/", "<", ">",
    ";", "(", ")", "^", ",", "#",
    "|", "%", "~", "[", "]", "{",
    "}", ".", ":", "!", "=", "&"
};

static char mutilOperator[14][10]={  // Codificación de operador de doble byte 57-71
    "++","--","==","<=",">=","<<",
    ">>","&&","||","!=","\\","\?",
    "\"", "\'"
};

// Determinar si es una letra
int isLetter(char ch){
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){return 1;}
    return 0;
}

// Determina si es un número
int isDigit(char ch){
    if((ch>='0'&&ch<='9')){return 1;}
    return 0;
}
// Determinar si es un operador y devolver el valor relativo del código
int isOperater(char ch){
    int i;
    for(i=0;i<sizeof(singleOperator)/10;i++){
        if(ch==singleOperator[i][0]){
            return i;
        }
    }
    return -1;
}
// Determinar si es una palabra reservada y devolver el valor relativo del código
int isReserve(char token[]){
    int i;
    for(i=0;i<sizeof(reserveWord)/20;i++){
        if(strcmp(token,reserveWord[i])==0){
            return i;
        }
    }
    return 100;
}
// Puntero hacia atrás
void Retract(int &pProject){
    pProject--;
}

// Escanea una palabra a la vez
void Scanner(int& syn,int &pProject,char token[],char projectResorce[]){

    char ch,mutil[2]={"\0"};
    int ptoken=0,i=0;
    syn=0;
    ch=projectResorce[pProject++];
    while(ch==' '){ // Limpia el espacio antes de la palabra
        ch=projectResorce[pProject++];
    }
    for (i = 0; i<10; i++){// Limpia el token antes de cada colección
        token[i] = '\0';
    }
    if(isLetter(ch)){  // Identificador o reconocimiento de palabra reservada
        while(isLetter(ch)||isDigit(ch)){
            token[ptoken++]=ch;
            ch=projectResorce[pProject++];
        }
        Retract(pProject);
        syn=isReserve(token);
//        cout<<"token syn " <<token<<syn<<endl;
        if(syn==100){ // La palabra es un identificador
            return ;
        }
        syn+=1;
        return ;  // Devuelve el código de categoría
    }
    else if(isDigit(ch)){ // Reconocimiento constante
        while(isDigit(ch)){
            token[ptoken++]=ch;
            ch=projectResorce[pProject++];
        }
        syn=99; // Devuelve el código de categoría
        Retract(pProject);
        return ;
    }
    else if((syn=isOperater(ch))!=-1){// Después del juicio y obtener el valor relativo del código de tipo de carácter único
        token[ptoken++]=ch;
        mutil[0]=ch;    // Almacena el contenido después del operador
        mutil[1]=projectResorce[pProject++];
        for(i=0;i<sizeof(mutilOperator)/10;i++){// Determine si es un operador de doble byte
            if(strcmp(mutil,mutilOperator[i])==0){
                syn=57+i;
                token[ptoken++]=ch;
                return ;
            }
        }
        Retract(pProject);
        syn=33+syn;
        return ;
    }
    else if(ch=='$'){syn=0;}
    else{syn=-1;}
    return ;
}

int main(){
    int pProject=0,syn=-1;
    char token[10],projectResorce[1000];
    FILE *fp;
    fp=fopen("./test.txt","r");
    fgets(projectResorce,1000,fp);// Obtener el contenido del archivo a la matriz
    cout<<"source code:\n"<<projectResorce<<endl;
    pProject=0; // Juzgar desde cero
    while(syn!=0){ // Salida binaria
        Scanner(syn,pProject,token,projectResorce);
        if(syn==99){
            cout<<"<constante,"<<token<<">"<<endl;
        }else if(syn==100){
            cout<<"<identificador,"<<token<<">"<<endl;
        }else if(syn>=1&&syn<=71){
            cout<<"<"<<token<<",-->  ("<<syn<<")"<<endl;
        }else if(syn==-1){
            cout<<"error code!\n"<<endl;
            break;
        }
        else{ cout<<"compile finished!\n"<<endl; }
        // Salida y escritura de archivos
    }
    return 0;
}