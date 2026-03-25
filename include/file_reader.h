#include "data_structures.h"

constexpr size_t ROUTING_SIZE = sizeof(routing_header);
constexpr size_t WF_SIZE = sizeof(wvf_message);

using namespace std;

void file_reader(vector <whole_message> &data, ifstream &f){

  cout<<"Reading file... \n";  

  std::cout << "sizeof(routing_header) = " << sizeof(routing_header) << "\n";
  std::cout << "sizeof(wvf_message)    = " << sizeof(wvf_message) << "\n"; 
  std::cout << "sizeof(whole_message)  = " << sizeof(whole_message) << "\n";

  vector<uint8_t> buffer (UDP_SIZE);


  while(f.read(reinterpret_cast<char*>(buffer.data()),UDP_SIZE)){


     whole_message event; 
     
     // First the routing Header, Eric's simulation produces two types of messages!
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


}