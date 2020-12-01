//driver program

#include<bits/stdc++.h>
#include<pthread.h>
#include<unistd.h>
#include "keyvaluestore.h"
using namespace std;

KeyValue *thread_example = new KeyValue();

void* thread1(void* arg){

    cout<<" Entered thread 1 "<<endl;

    thread_example->Create("thread_example","this is thread safe",0);

    cout<<"Exiting thread 1"<<endl;

}

void* thread2(void *arg){

    cout<<"Entered thread 2"<<endl;

    thread_example->Read("thread_example");

    cout<<"Exiting thread 2"<<endl;
}

int main(){
    
    /*

    //testing file capacity

    KeyValue *a = new KeyValue();
    
    
    int i =0;
    while(1) {
        a->Create(to_string(i),"unit_testing",0);
        cout<<i<<endl;
        i++;
    }
    */

  /*  

  // testing TTL

   KeyValue *b = new KeyValue();

   //testing without time to live

   b->Create("first","unit_test",0);
   b->Read("first");

   // testing with time to live as 1 second

   b->Create("second","unit_test",1);
   b->Read("second");

*/

/*

// Testing Delete
KeyValue *c = new KeyValue();

    c->Create("India","New Delhi",0);
    c->Create("America","Washington DC",0);
    
    c->Read("India");
    c->Read("America");
    
    c->Delete("America");
    
    c->Read("India");
    c->Read("America");

*/

/*

//testing thread safety

    pthread_t t1,t2;
    pthread_create(&t1,NULL,&thread1,NULL);
    sleep(1);
    pthread_create(&t2,NULL,&thread2,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
 
    return 0;
*/
}



