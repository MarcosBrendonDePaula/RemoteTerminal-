#include <iostream>
#include "include/EasyMultServer.h"
#include "include/Events.h"
#include "fstream"
Events Eventos;

void Request(void *arg){
    Nsock *Socket=(Nsock*)arg;
    string input = Socket->getInput();
    input+=" > Saida.txt";
    system(input.c_str());
    fstream Arq("Saida.txt");
    string concat="";
    string linha;
    while(getline(Arq,linha))
        concat+=linha+"\n";
    Socket->SendMsg(concat);
    Arq.clear();
    Arq.close();
}
void accept(void *arg){
    Nsock *Socket=(Nsock*)arg;
    Socket->SendMsg("Conectado");
}

int main(){
    EasyMultServer Servidor(25565,&Eventos);
    if(Servidor.Start(&Request,900,&accept)){
        int x;
        cout<<"Iniciado"<<endl;
        cin>>x;
    }
    return 0;
}