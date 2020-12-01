// This is the library file, implementing CRD functions on a local key value data store

#include<bits/stdc++.h>
#include<chrono>
#include<unistd.h>
#include<sys/types.h>
#include "keyvaluestore.h"


using namespace std::chrono;
    

// Default constructor 

KeyValue::KeyValue(void){
    
    // The semaphore is to ensure thread safety

     sem_init(&data_store,0,1);
     unique_process_id = getpid();

     // Every Process has its own unique data store 
     path = std::to_string(unique_process_id) + "_key_value_store.txt";

}

// Parameterised constructor

KeyValue::KeyValue(std::string directory){

        sem_init(&data_store,0,1);
        unique_process_id = getpid();
        path = directory + path = std::to_string(unique_process_id) + "_key_value_store.txt";
    }

//CRD functions 

void KeyValue::Create(std::string key,std::string value,int time_to_live_in_seconds){

       sem_wait(&data_store);

        // semaphore locked

       
        if(std::find(keys.begin(),keys.end(),key) != keys.end()){
            std::cout<<"**ERROR**"<<std::endl<<"key already present"<<std::endl;
            sem_post(&data_store);
            return;
        }
        
        
       
        std::ifstream read(path,std::ios::binary);
        read.seekg(0,std::ios::end);
        
        //Ensuring that the file hasnt exceeded its 1 GB data  limit

        if(read.tellg() >= 1024*1024){
            
            std::cout<<"*ERROR**"<<std::endl<<"exceeded maximum storage capacity"<<std::endl;
            sem_post(&data_store);
            return;
        }

        std::ofstream write;
        write.open(path,ios::app);

        // To enable the time to live functionality, we use the system clock to mark every key's time of creation

        auto current_time = std::chrono::system_clock::now();
        current_time = time_point_cast<std::chrono::seconds>(current_time);
        auto dur = current_time.time_since_epoch();
        dur = duration_cast<seconds>(dur);
        
        
        //string data inserted into file
        std::string text = key + "," + value + "," + std::to_string(time_to_live_in_seconds) + "," + std::to_string(dur.count());

        write<<text<<std::endl;

        write.close();
        
        sem_post(&data_store);

        // other threads free to access the functionality again

        keys.push_back(key);
        
        return;
    }

void KeyValue::Read(std::string key){

        bool found_key = false;

        if(std::find(keys.begin(),keys.end(),key) == keys.end()){

            std::cout<<"**ERROR**"<<std::endl<<"invalid key"<<std::endl;
            return;
        }

        sem_wait(&data_store);
        //ensuring thread safety

        std::ifstream read(path);
        std::string text;
        std::vector<std::string> parsed_text;
        while(read.is_open() && !read.eof()){

            getline(read,text);
            parsed_text = parse(text);

            if(parsed_text[0] == key){
                
                //member function that deconstructs the text data

               
                // checking if time to live is given or not

                if(parsed_text[2] != "0") {

                //calculating the time duration of the  key-value pair

                auto current_time = std::chrono::system_clock::now();
                current_time = time_point_cast<std::chrono::seconds>(current_time);
                auto dur = current_time.time_since_epoch();
                dur = duration_cast<seconds>(dur);
                
                long time_lapse,start_time;
                std::stringstream ss;
                ss<<parsed_text[2];
                ss>>time_lapse;
                
                if(time_lapse <= dur.count() - start_time){
                    std::cout<<"**ERROR**"<<std::endl<<"the key has expired"<<std::endl;
                    read.close();
                    sem_post(&data_store);
                    return;
                }
                
                found_key = true;
                std::cout<<parsed_text[1]<<std::endl;

                }
                else {

                    cout<<parsed_text[1]<<endl;
                }

            }
            
        }

        read.close();
        sem_post(&data_store);

        // other threads free to run again
        return;

    }

/*

 Deleting  the actual entry from the file will require making a copy of the existing 
 file and removing the particular entry, which is  both time and memory ineffecient,
 hence we only remove the key from the list in program memory and not from the actual data store
 
 */
void KeyValue::Delete(std::string key){

        auto find_key  = std::find(keys.begin(),keys.end(),key);

        if(find_key == keys.end()){
            std::cout<<"**ERROR**"<<std::endl<<"invalid key"<<std::endl;
        } 
        keys.erase(find_key);        

        return;
    }

// Member functiont to parse the data from the file

inline std::vector<std::string> KeyValue::parse(std::string text){

        std::vector<std::string> parsed_text;

        int previous = 0;

        for(int i = 0 ; i < text.length();i++){
            if(text[i] == ','){
                parsed_text.push_back(text.substr(previous,i - previous));
                previous = i+1;
            }
        }
        parsed_text.push_back(text.substr(previous,text.length()-previous));

        return parsed_text;

    } 

