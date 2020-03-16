#include "include/EasySocket.h"
#include "include/Events.h"
#include <iostream>

void Print(void* arg){
    EasySocket *At=(EasySocket*)arg;
    cout<<At->getInput()<<endl;
    cout<<"$ ";
}

int main(){
    Events Eventos;
    string ip;
    int porta;
    cout<<"Informe O ip"<<endl;
    cin>>ip;
    cout<<"Informe A porta"<<endl;
    cin>>porta;
    EasySocket Socket(ip,porta,&Print,&Eventos);
    string input;
    Socket.Connect();
    cout<<"$ ";
    while(getline(cin,input) and input!="SaiR"){
        Socket.SendMsg(input);
    }
    return 0;
}