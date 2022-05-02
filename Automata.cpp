#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Clase Nodo Necesaria para la lista
template <typename D>//T va a almacenar el tipo de dato 
class NodoD{
    private:
        D dato;
        NodoD<D> * siguiente;
        NodoD<D> * anterior;

    public:
    //Getters y Setters
    D getDato(){return this->dato;}
    void setDato(D valor){this->dato=valor;}

    NodoD<D> * getSiguiente(){return this->siguiente;}
    void setSiguiente(NodoD<D> * nodo){this->siguiente=nodo;}

    NodoD<D> * getAnterior(){return this->anterior;}
    void setAnterior(NodoD<D> * nodo){this->anterior=nodo;}

    NodoD<D>(D dato, NodoD<D> * anterior, NodoD<D> * siguiente){
        this->dato=dato;
        this->siguiente=siguiente;
        this->anterior=anterior;
    }
};

//Clase Lista donde guardaremos los caracteres
template<typename T>
class ListaDoble{
    //Atributos
    NodoD<T> * head;
    int tam;

    //Metodos
    public:
        ListaDoble(){
            this->head=nullptr;
            this->tam=0;
        }
        int getTam(){
            return this->tam;
        }

        NodoD<T> * getHead(){
            return this->head;
        }
        
        void agregarInicio(T dato){
            //Crear el nuevo nodo
            NodoD<T> * nuevo = new NodoD<T>(dato, nullptr, this->head);
            //Actualizar los atributos de head
            if(this->head){
                this->head->setAnterior(nuevo);
            }
            //Actualizar el head de la lista al nuevo nodo
            this->head = nuevo;
            //Actualizar el tamaño
            this->tam++;
        }
        void agregarFinal(T dato){
            //Crear el nuevo nodo
            NodoD<T> * nuevo = new NodoD<T>(dato, nullptr, nullptr);
            if(this->head){//La lista tiene valores
                NodoD<T> * nodo = this->head;
                while(nodo->getSiguiente()){
                    nodo=nodo->getSiguiente();
                }
                nodo->setSiguiente(nuevo);
                nuevo->setAnterior(nodo);
            }else{//La lista está vacía
                //El head es igual al nuevo nodo
                this->head=nuevo;
            }
            //Actualizar el tamaño
            this->tam++;
        }
        NodoD<T> * buscarNodoT(T dato){
            NodoD<T> * nodo = this->head;
            while(nodo){//Recorrer la lista
                if(nodo->getDato()==dato){
                    return nodo;
                }
                nodo = nodo->getSiguiente();
            }
            return nullptr;
        }


        void update(T dato, T n_dato){
            NodoD<T> * nodo = buscarNodoT(dato);//Buscamos el nodo
            if(nodo){//El nodo que buscamos sí está en la lista
                //Creamos un nuevo nodo con el valor solicitado
                NodoD<T> * nuevo = new NodoD<T>(n_dato, nodo->getAnterior(), nodo->getSiguiente());
                //Revisamos si la lista tiene más de un elemento
                if(this->tam==1){//Si solo tiene un elemento actualizamos el head
                    this->head = nuevo;
                }else{
                    if(nodo->getAnterior() && nodo->getSiguiente()){//Revisar que el nodo que queremos actualizar no esté en los extremos
                        nodo->getAnterior()->setSiguiente(nuevo);
                        nodo->getSiguiente()->setAnterior(nuevo);
                    }else{
                        if(nodo->getAnterior()){//Es el last
                            nodo->getAnterior()->setSiguiente(nuevo);
                            nuevo->setSiguiente(nullptr);
                        }else{//Es el head
                            nodo->getSiguiente()->setAnterior(nuevo);
                            nuevo->setAnterior(nullptr);
                            this->head=nuevo; //Actualizar el head
                        }
                    }
                }
                nodo=nullptr;
            }
        }

        void del(T dato){
            //Buscamos el nodo en la lista
            NodoD<T> * nodo = buscarNodoT(dato);
            if(nodo){//Lista con valores
                if(this->tam==1){//La lista sólo tiene un elemento
                    this->head=nullptr;
                }else{
                    if(nodo->getAnterior() && nodo->getSiguiente()){//Revisar que el nodo que 
                    //queremos actualizar no esté en los extremos
                        nodo->getAnterior()->setSiguiente(nodo->getSiguiente());
                        nodo->getSiguiente()->setAnterior(nodo->getAnterior());
                    }else{
                        if(nodo->getAnterior()){//Es el last
                            nodo->getAnterior()->setSiguiente(nullptr);
                        }else{//Es el head
                            nodo->getSiguiente()->setAnterior(nullptr);
                            this->head=nodo->getSiguiente();
                        }
                    }
                    nodo=nullptr;
                }
                this->tam--;//Actualizar el tamaño de la lista
            }
        }
        
        void imprimirLista(){
            NodoD<T> * nodo = this->head;
            while(nodo){
                cout<<nodo->getDato()<<" ";
                nodo=nodo->getSiguiente();
            }
            cout<<endl;
        }
};

//Funcion principal que clasifica los caracteres 
void LexerAritmetico(ListaDoble<char> *var, ListaDoble<char> *num,ListaDoble<char> *esp, ListaDoble<char> *op, string line, ListaDoble<string> * Token, ListaDoble<string> * Dato){
    int i;
    bool wasComment;
    bool wasJumpLine = false;

    for(i=0;i<=line.length()+1; i++){ //for para hacer pasar por los caracteres de cada linea del archivo txt
        if(line[i]==' '||!line[i]){ //If para omitir los espacios
            if(!line[i]&&!wasJumpLine){
                Token->agregarFinal("Salto Linea");
                wasJumpLine=true;
                break;
            }
        continue;
        }
        else if(var->buscarNodoT(line[i])){ //Busca el caracter en la lista de variables (A,a..Z,z)
        //cout << "Hola"<<endl;
            string miStrVar;
            miStrVar.push_back(line[i]);
            while(1){          
                if(var->buscarNodoT(line[i+1])||num->buscarNodoT(line[i+1])){
                    miStrVar.push_back(line[i+1]);
                    i++;
                }
                else
                    break;
            }
            cout << miStrVar << "-> Variable" << endl; 
            Token->agregarFinal("Variable");
            Dato->agregarFinal(miStrVar);
            wasComment = false;
            wasJumpLine=false;
        }
        else if(num->buscarNodoT(line[i])||line[i]==esp->getHead()->getSiguiente()->getSiguiente()->getDato()){ //Busca el caracter en la lista de numeros o valida si el caracter es igual a '.'
            
            if(num->buscarNodoT(line[i+1])||line[i+1]==esp->getHead()->getSiguiente()->getSiguiente()->getDato()){ //Valida si existe otro numero en la siguiente iteracion 
                string miStrNum = ""; //String que se llenara con el numero resultante
                string tipoNum; //Tipo de numero Real o Entero
                if(num->buscarNodoT(line[i])){ //Si el primer caracter es numero entonces el tipo es un entero
                    tipoNum="Entero";
                    miStrNum.push_back(line[i]);//Aqui se agrega al string
                }
                else{
                    tipoNum="Real"; //En caso de ser un punto el tipo es un real
                    miStrNum.push_back(line[i]);
                }
                i++;
                while(1){ //Empieza un while infinito en la siguiente iteracion

                    if(num->buscarNodoT(line[i])){//Agrega numeros
                        miStrNum.push_back(line[i]);
                    }
                    if(esp->buscarNodoT(line[i])){//Posibilidad de agregar un punto y cambia el tipo a Real
                        miStrNum.push_back(line[i]);
                        tipoNum="Real";
                    }
                     if(!num->buscarNodoT(line[i+1])&&line[i+1]!=esp->getHead()->getSiguiente()->getSiguiente()->getDato()){
                        break; //Se rompe el ciclo si la siguiente iteracion deja de ser un numero o un punto
                    }
                    i++;
                }
                cout<< miStrNum << "->" <<tipoNum<<endl;
                Token->agregarFinal(tipoNum);
                Dato->agregarFinal(miStrNum);
                wasComment = false;
                wasJumpLine=false;
            }
            else{ //Este codigo solo corre si hay un unico token, si es un punto, el identificador no es valido
                string miStrOp;
                if(num->buscarNodoT(line[i])){
                    cout << line[i]<<"-> Entero"<<endl;
                    string miStrOp;
                    miStrOp.push_back(line[i]);
                    Token->agregarFinal("Entero");
                    Dato->agregarFinal(miStrOp);
                    wasComment = false;
                    wasJumpLine=false;
                }
                else{
                    cout << line[i]<<"-> Identificador no valido"<<endl;
                    string miStrOp;
                    miStrOp.push_back(line[i]);
                    Token->agregarFinal("Identificador no valido");
                    Dato->agregarFinal(miStrOp);
                    wasComment = false;
                    wasJumpLine=false;
                }

            }
        }
        
        else if(op->buscarNodoT(line[i])){//Valida si el caracter existe dentro de la lista de operadores
            switch (op->buscarNodoT(line[i])->getDato()){ //El switch recibe el caracter y los compara en los cases para definir el token
                case '=':
                    cout<< "= -> Asignacion" << endl;
                    Token->agregarFinal("Asignacion");
                    Dato->agregarFinal("=");
                    wasComment = false;
                    wasJumpLine=false;
                    break;
                case '*':
                    cout<< "* -> Multiplicacion" << endl;
                    Token->agregarFinal("Operador");
                    Dato->agregarFinal("*");
                    wasComment = false;
                    wasJumpLine=false;
                    break;
                case '+':
                    cout<< "+ -> Suma"<<endl;
                    Token->agregarFinal("Signo");
                    Dato->agregarFinal("+");
                    wasComment = false;
                    wasJumpLine=false;
                    break;
                case '-':
                    cout<< "- -> Resta"<<endl;
                    Token->agregarFinal("Signo");
                    Dato->agregarFinal("-");
                    wasComment = false;
                    wasJumpLine=false;
                    break;
                case '^':
                    cout<< "^ -> Potencia"<<endl;
                    Token->agregarFinal("Operador");
                    Dato->agregarFinal("^");
                    wasComment = false;
                    wasJumpLine=false;
                    break;
                case '/':
                    if(line[i+1]=='/'){//En caso de ser el simbolo de la division '/' es necesario validar si existe otro simbolo igual para de esta forma definir el token y el resto de la linea como comentario
                        string coment = "";
                        while(line[i]){

                            coment.push_back(line[i]);
                            i++;
                        }
                        cout << coment << "-> Comentario"<<endl;
                        Token->agregarFinal("Comentario");
                        Dato->agregarFinal(coment);
                        wasComment = true;
                        wasJumpLine=false;
                        Token->agregarFinal("Salto Linea");
                        return;

                    }
                    else
                        if(!wasComment){
                            cout << "/ -> Division"<<endl;
                            Token->agregarFinal("Operador");
                            Dato->agregarFinal("/");
                            wasComment = false;
                            wasJumpLine=false;
                        }
                        // cout << "/ -> Division"<<endl;
                        // Token->agregarFinal("Hola");
                        // Dato->agregarFinal("/");
                    break;
            }

        }
        else if(esp->buscarNodoT(line[i])){//Valida si el caracter esta dentro de simbolos especiales para definirlo como parentesis
            switch(esp->buscarNodoT(line[i])->getDato()){
                case '(':
                    cout << line[i]<< "-> Parentesis que abre" << endl;
                    Token->agregarFinal("Parentesis que abre");
                    Dato->agregarFinal("(");
                    wasComment = false;
                    wasJumpLine=false;
                    break;
                case ')':
                    cout << line[i]<< "-> Parentesis que cierra" << endl;
                    Token->agregarFinal("Parentesis que cierra");
                    Dato->agregarFinal(")");
                    wasComment = false;
                    wasJumpLine=false;
                    break;
            }
        }
        else{//Si el caracter no se encontro en ninguna lista, se correrá este codigo
            string miStrInv;
            if(line[i]&&!wasJumpLine)// Necesario para no procesar el salto de linea
                cout << line[i]<<"-> Identificador no valido"<<endl;
                if(line[i]){
                    miStrInv.push_back(line[i]);
                }
                Token->agregarFinal("Identificador no valido");
                Dato->agregarFinal(miStrInv);
                wasComment = false;
                wasJumpLine=false;
        }
    }
}
void marcarError(NodoD<string>* tipoToken,int &contador,ofstream &archivo){
    archivo<<".tok"+to_string(contador)+"{  text-decoration-line: underline; text-decoration-color: red; text-decoration-style: wavy;}\n";
    contador++;
    cout<<tipoToken->getDato()<<endl;
}

void isValue(NodoD<string>* tipoToken,int &contador,ofstream &archivo){
    archivo<<".tok"+to_string(contador)+"{color:blue;}\n";
    contador++;
    cout<<tipoToken->getDato()<<endl;
    // if(tipoToken->getSiguiente()->getDato()=="Asignacion"||tipoToken->getSiguiente()->getDato()=="Parentesis que abre"||tipoToken->getSiguiente()->getDato()=="Token Invalido"||tipoToken->getSiguiente()->getDato()=="Entero"||tipoToken->getSiguiente()->getDato()=="Variable"||tipoToken->getSiguiente()->getDato()=="Real"){
    //     //Llamamos funcion marcar error
    //     marcarError(tipoToken, contador, archivo);
    // }
}
void isParentensis(NodoD<string>* tipoToken,int &contador,ofstream &archivo, int &contPar){
    if(tipoToken->getDato()=="Parentesis que abre"){
        archivo<<".tok"+to_string(contador)+"{color:orange;}\n";
        contador++;
        contPar++;
        cout<<tipoToken->getDato()<<endl;
        // if(tipoToken->getSiguiente()->getDato()=="Parentesis que cierra"||tipoToken->getSiguiente()->getDato()=="Operador"||tipoToken->getSiguiente()->getDato()=="Asignacion"||tipoToken->getSiguiente()->getDato()=="Token Invalido"||tipoToken->getSiguiente()->getDato()=="Comentario"){
        //     //Llamamos funcion marcar error
        //     marcarError(tipoToken, contador, archivo);
        // }
    }
    else if(tipoToken->getDato()=="Parentesis que cierra"){
        archivo<<".tok"+to_string(contador)+"{color:orange;}\n";
        contador++;
        contPar--;
        cout<<tipoToken->getDato()<<endl;
        // if(tipoToken->getSiguiente()->getDato()=="Entero"||tipoToken->getSiguiente()->getDato()=="Real"||tipoToken->getSiguiente()->getDato()=="Variable"||tipoToken->getSiguiente()->getDato()=="Parentesis que abre"||tipoToken->getSiguiente()->getDato()=="Token Invalido"||tipoToken->getSiguiente()->getDato()=="Asignacion"||!(contPar>=0)){
        //     //Llamamos funcion marcar error
        //     marcarError(tipoToken, contador, archivo);
        // }
    }
}
void isOperador(NodoD<string>* tipoToken,int &contador,ofstream &archivo){
    if(tipoToken->getDato()=="Signo"){
        archivo<<".tok"+to_string(contador)+"{color:green;}\n";
        contador++;
        cout<<tipoToken->getDato()<<endl;
    }
    else{
        archivo<<".tok"+to_string(contador)+"{color:green;}\n";
        contador++;
        cout<<tipoToken->getDato()<<endl;
    }

    // if(tipoToken->getSiguiente()->getDato()=="Entero"||tipoToken->getSiguiente()->getDato()=="Real"||tipoToken->getSiguiente()->getDato()=="Variable"){
    //     marcarError(tipoToken, contador, archivo);
    // }

}
void isComentario(NodoD<string>* tipoToken,int &contador,ofstream &archivo){
    archivo<<".tok"+to_string(contador)+"{color:yellow;}\n";
    contador++;
    cout<<tipoToken->getDato()<<endl;
}



void generarVerificador(string archivo,string archivo2, ListaDoble<string>*Token, ListaDoble<string> *Dato){
    ofstream miverificador;
    miverificador.open(archivo);
    miverificador<<"<!DOCTYPE html>\n";
    miverificador<<"<html lang='en'>\n";
    miverificador<<"<head>\n";
    miverificador<<"<meta charset='UTF-8'>\n";
    miverificador<<"<meta http-equiv='X-UA-Compatible' content='IE=edge'>\n";
    miverificador<<"<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    miverificador<<"<link rel='stylesheet' href='estiloVerificador.css'>\n";
    miverificador<<"<title>Verificador</title>\n";
    miverificador<<"</head>\n";
    miverificador<<"<body>\n";
    NodoD<string>*buscandoToken=Token->getHead();
    NodoD<string>*buscandoSimbolo=Dato->getHead();
    int contador = 0;
    int i= 0;
    while(buscandoToken){
        while(buscandoToken->getDato()!="Salto Linea"){
            contador++;
            buscandoToken=buscandoToken->getSiguiente();
            //cout<<buscandoToken->getDato()<<endl;
        }
        buscandoToken=buscandoToken->getSiguiente();
        miverificador<<"<div>\n";
        for(i; i < contador ; i++){
            miverificador<<"<span class='tok"+to_string(i)+"'>"+buscandoSimbolo->getDato()+"</span>\n";
            buscandoSimbolo=buscandoSimbolo->getSiguiente();
        }
        miverificador<<"</div>\n";

    }

    miverificador<<"</html>\n";
    miverificador<<"</body>\n";
    miverificador.close();


    miverificador.open(archivo2);
    NodoD<string>* buscandoToken1=Token->getHead();
    int contadorSimbolo=0;
    int contPar=0;
    bool wasOp=false;
    bool wasPa=false;
    bool wasVal=false;
    bool wasPc=false;
    while(buscandoToken1){
        if(buscandoToken1->getDato()=="Salto Linea"){
            buscandoToken1=buscandoToken1->getSiguiente();
            continue;
        }
        bool wasOp=false;
        bool wasPa=false;
        bool wasVal=false;
        bool wasPc=false;
        if(buscandoToken1->getDato()=="Variable"&&buscandoToken1->getSiguiente()->getDato()=="Asignacion"&&(buscandoToken1->getSiguiente()->getSiguiente()->getDato()=="Entero"||buscandoToken1->getSiguiente()->getSiguiente()->getDato()=="Real"||buscandoToken1->getSiguiente()->getSiguiente()->getDato()=="Variable"||buscandoToken1->getSiguiente()->getSiguiente()->getDato()=="Signo"||buscandoToken1->getSiguiente()->getSiguiente()->getDato()=="Parentesis que abre")){
            miverificador<<".tok"+to_string(contadorSimbolo)+"{color:blue;}\n";
            contadorSimbolo++;
            cout<<buscandoToken1->getDato()<<endl;
            buscandoToken1=buscandoToken1->getSiguiente();
            miverificador<<".tok"+to_string(contadorSimbolo)+"{color:green;}\n";
            contadorSimbolo++;
            cout<<buscandoToken1->getDato()<<endl;
            buscandoToken1=buscandoToken1->getSiguiente();
            if(buscandoToken1->getDato()=="Entero"||buscandoToken1->getDato()=="Real"||buscandoToken1->getDato()=="Variable"){
                miverificador<<".tok"+to_string(contadorSimbolo)+"{color:blue;}\n";
                contadorSimbolo++;
                cout<<buscandoToken1->getDato()<<endl;
                buscandoToken1=buscandoToken1->getSiguiente();
                wasVal=true;
            }
            else if(buscandoToken1->getDato()=="Signo"){
                miverificador<<".tok"+to_string(contadorSimbolo)+"{color:green;}\n";
                contadorSimbolo++;
                cout<<buscandoToken1->getDato()<<endl;
                buscandoToken1=buscandoToken1->getSiguiente();
                wasOp=true;
            }
            else if(buscandoToken1->getDato()=="Parentesis que abre"){
                miverificador<<".tok"+to_string(contadorSimbolo)+"{color:orange;}\n";
                contadorSimbolo++;
                cout<<buscandoToken1->getDato()<<endl;
                buscandoToken1=buscandoToken1->getSiguiente();
                wasPa=true;
            }          
            while(buscandoToken1->getDato()!="Salto Linea"){
                if(!(buscandoToken1->getSiguiente()->getDato()  =="Salto Linea"&&contPar!=0)){
                    if((buscandoToken1->getDato()=="Variable"||buscandoToken1->getDato()=="Entero"||buscandoToken1->getDato()=="Real")&&(wasOp||wasPa)){
                        isValue(buscandoToken1,contadorSimbolo,miverificador);
                        wasVal=true;
                        wasOp=false;
                        wasPa=false;
                    }
                    else if((buscandoToken1->getDato()=="Parentesis que abre"||buscandoToken1->getDato()=="Parentesis que cierra")&&(wasVal||wasOp)){
                        isParentensis(buscandoToken1,contadorSimbolo,miverificador,contPar);
                        if(buscandoToken1->getDato()=="Parentesis que abre")
                            wasPa=true;
                        else
                            wasPc=true;
                        wasVal=false;
                        wasOp=false;
                    }
                    else if((buscandoToken1->getDato()=="Operador"||buscandoToken1->getDato()=="Signo")&&(wasPa||wasPc||wasVal)){
                        isOperador(buscandoToken1,contadorSimbolo,miverificador);
                        wasOp=true;
                        wasPa=false;
                        wasPc=false;
                        wasVal=false;
                    }
                    else if(buscandoToken1->getDato()=="Comentario"){
                        if((buscandoToken1->getAnterior()->getDato()=="Variable")&&(wasVal||wasPc)){
                            isComentario(buscandoToken1,contadorSimbolo,miverificador);
                            wasVal=false;
                            wasPc=false;
                        }
                        else{
                            //Llamamor marcar error
                            while(buscandoToken1->getDato()!="Salto Linea"){
                                marcarError(buscandoToken1, contadorSimbolo,miverificador);
                                buscandoToken1=buscandoToken1->getSiguiente();
                            }
                            break;
                        }
                    }
                    else{
                        while(buscandoToken1->getDato()!="Salto Linea"){
                            marcarError(buscandoToken1, contadorSimbolo,miverificador);
                            buscandoToken1=buscandoToken1->getSiguiente();
                        }
                        break;
                    }

                    buscandoToken1=buscandoToken1->getSiguiente();
                }
                
                else{
                    while(buscandoToken1->getDato()!="Salto Linea"){
                        marcarError(buscandoToken1, contadorSimbolo,miverificador);
                        buscandoToken1=buscandoToken1->getSiguiente();
                    }
                }
            }
        }
        else{
            if(buscandoToken1->getDato()=="Comentario"){
                isComentario(buscandoToken1,contadorSimbolo,miverificador);
                buscandoToken1=buscandoToken1->getSiguiente();                
            }
            else{
                //Llamamos marcar error
                while(buscandoToken1->getDato()!="Salto Linea"){
                    marcarError(buscandoToken1, contadorSimbolo,miverificador);
                    buscandoToken1=buscandoToken1->getSiguiente();
                }    
            }
        }
        
    }
    miverificador.close();
}

void paso(NodoD<string> *hola){
    *hola=*hola->getSiguiente();
    cout<<hola->getDato()<<endl;
}
int main(){
    //Definicion de Listas Dobles apuntadas
    ListaDoble<char> *Variable = new ListaDoble<char>(); //(A,a..Z,z)
    ListaDoble<char> *Operadores = new ListaDoble<char>();//0..9
    ListaDoble<char> *Numeros = new ListaDoble<char>();//= + - * ^ / 
    ListaDoble<char> *SimbolosEspeciales= new ListaDoble<char>();// ( ) .
    ListaDoble<string>*Token=new ListaDoble<string>(); //Lista Tokens
    ListaDoble<string>*Dato=new ListaDoble<string>(); //Lista Datos
    int i;
    //Se llena la lista de variables y numeros con codigo ASCII
    for(i=65; i<=90; i++){
        Variable->agregarFinal(char(i));
    }
    for(i=97; i<=122; i++){
        Variable->agregarFinal(char(i));
    }
    //Variable->imprimirLista();

    for(i=48; i<=57; i++){
        Numeros->agregarFinal(char(i));
    }
    
    //Se llena el resto de las listas con el metodo de la  clase ListaDoble
    Operadores->agregarFinal('=');
    Operadores->agregarFinal('+');
    Operadores->agregarFinal('-');
    Operadores->agregarFinal('^');
    Operadores->agregarFinal('/');
    Operadores->agregarFinal('*');

    SimbolosEspeciales->agregarFinal('(');
    SimbolosEspeciales->agregarFinal(')');
    SimbolosEspeciales->agregarFinal('.');   

    ifstream misDatos; 
    misDatos.open("archivoTokens.txt");//Se abre el archivo txt
    string linea;
    while(misDatos.good()){//While que correra mientras todavia falten elementos por leer de la lista
        getline(misDatos, linea,'\n');//Asigna a la variable linea cada linea del archivo
        cout << linea << endl; //Imprime la linea del archivo completa
        LexerAritmetico(Variable, Numeros, SimbolosEspeciales,Operadores,linea,Token, Dato); //Se corre la funcion principal
        cout<< '\n'<<endl;
    }
    // Token->imprimirLista();
    // NodoD<string> *hola = Token->getHead();
    // cout<<hola->getDato()<<endl;
    // paso(hola);
    // cout<<hola->getDato()<<endl;
    
    Token->imprimirLista();
    Dato->imprimirLista();
    cout<<Token->getTam()<<endl;
    cout<<Dato->getTam()<<endl;
    string archivo = "verificador.html";
    string archivo2="estiloVerificador.css";
    generarVerificador(archivo,archivo2, Token,Dato);
    
    return 0;
}

