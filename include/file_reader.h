#include "data_structures.h"

constexpr size_t ROUTING_SIZE = sizeof(routing_header);
constexpr size_t WF_SIZE = sizeof(wvf_message);
constexpr size_t CUSTOM_WF_SIZE = sizeof(wvf_custom_message);

using namespace std;

void file_reader(vector <whole_message> &data, ifstream &f, int mode){

   std::cout << "Size of (routing_header) = " << ROUTING_SIZE << "\n";
   std::cout << "Size of (wvf_message)    = " << WF_SIZE << "\n"; 
   std::cout << "Size of (whole_message)  = " << ROUTING_SIZE + WF_SIZE << "\n";
   std::cout << "Size of (custom_message) = " << CUSTOM_WF_SIZE << "\n";

   cout<<"Reading experimental file..."<<endl; 

   vector<uint8_t> buffer (UDP_SIZE);

   while(f.read(reinterpret_cast<char*>(buffer.data()),UDP_SIZE)){

     whole_message event; 
     
     // First the routing Header
     memcpy(&event.header, buffer.data(), ROUTING_SIZE);
     
     // Now the waveform
     memcpy(&event.message, buffer.data() + ROUTING_SIZE , WF_SIZE); 

     data.push_back(event);

   } 

    if (!f.eof()) {
        throw runtime_error("File read error");
    }

    if (f.gcount() != 0) {
        throw runtime_error("Partial message at end of file");
    }

    cout << "Reached EOF \n";




  if(mode == 1){

    cout<<"Creating traces in mode 1 (single event traces) "<<endl;  
  
   
  }

  if(mode == 2){

    cout<<"Creating traces in mode 2 (overlapped single event traces) "<<endl;  
  
   
  }

  if(mode == 3){

    cout<<"Creating traces in mode 3 (overlapped, continous event traces) "<<endl;  
  
   
  }




}