// Header file to out keyvalue library , descriptions of each function given  in the lib file

using namespace std;
#include<bits/stdc++.h>
#include<semaphore.h>
class KeyValue {

    public :
    sem_t data_store;
    
    int unique_process_id;
    string path;
    vector<string> keys;
   
    
    KeyValue(void);

    KeyValue(string);

    void Create(string ,string ,int );

    void Read(string);

    void Delete(string);

    inline vector<string> parse(string);

};