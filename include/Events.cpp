#include "Events.h"
/*
    Construtor Eventos
*/
Events::Events(){
    //this->Debug=Debug;
    this->rodando=0;
    this->Paralel=false;
    #ifdef __linux__
        pthread_create(&this->executor,0,Events::ExecutorF,this);
    #elif _WIN32
        _beginthread(Events::ExecutorF,0,this);
    #endif
}

void Events::addEvent(Event *E){
    this->Leventos[E->ID]=E;
}

void Events::sendSignal(int ID){
    if(this->Leventos[ID]==NULL){
        return;
    }
    this->LEDP.push_back(ID);
    this->rodando=true;
    
}

#ifdef __linux__
void* Events::callFunction(void* arg)
{
    Event *eventoAtual = (Event*)arg;
    eventoAtual->funcao(eventoAtual->parametros);
}

void* Events::ExecutorF(void* arg)
#elif _WIN32
void Events::ExecutorF(void* arg)
#endif
{
    Events *This=(Events*)arg;
    while(true){
        lim:
        while(!This->rodando){
            #ifdef __linux__
                sleep(1);
            #elif _WIN32
                Sleep(500);
            #endif
            
        }
        if(This->LEDP.empty()){
            This->rodando=false;
            goto lim;
        }
        This->rodando=true;
        Event *Atual = This->Leventos[This->LEDP.front()];
        //single process
        if(!This->Paralel)
            Atual->funcao(Atual->parametros);
        else{
            #ifdef __linux__
                pthread_t Nova;
                pthread_create(&Nova,0,Events::callFunction,Atual);
            #elif _WIN32
                _beginthread(Atual->funcao,0,Atual->parametros);
            #endif
        }
        This->LEDP.pop_front();
    }
}

#ifdef _WIN32
void Events::KeyMonitor(void* arg){
    Events *This=(Events*)arg;
    while (true) {						
        Sleep(100);
        for (int i = 8; i <= 255; i++) {
            if (GetAsyncKeyState(i) == -32767) {
                int *x = (int*)malloc(sizeof(int));
                *x = i; 
                This->getEvent(159753)->parametros = x;
                This->sendSignal(159753);
            }					
        }
    }
}


void Events::OnKeyMonitor(void(*funcao)(void*)){
    if(!this->KeySMonitor){
        this->addEvent(new Event(159753,funcao));
        _beginthread(Events::KeyMonitor,0,this);
    }
}
#endif

Event* Events::getEvent(int id){
    return this->Leventos[id];
}
/*
    Construtor Evento
*/
Event::Event(int ID,void(*funcao)(void*),void *parametro){
    this->funcao=funcao;
    this->ID=ID;
    this->parametros=parametro;
}
Event::Event(int ID,void(*funcao)(void*)){
    this->funcao=funcao;
    this->ID=ID;
    this->parametros=NULL;
}
