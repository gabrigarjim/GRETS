#include "include/data_structures.h"
#include "include/definitions.h"
#include "include/delta_comp.h"
#include "include/file_reader.h"

#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <cstdlib>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>

using namespace std;


void pin_thread_to_core(int core_id) {
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (rc != 0) {
        perror("pthread_setaffinity_np");
    }
}

void sender_loop(int core_id, int port, double TARGET, vector<whole_message> &data_buffer) {
 
    pin_thread_to_core(core_id);

    double seconds_per_batch = BATCH_SIZE / TARGET; 

    auto delay = std::chrono::duration<double>(seconds_per_batch);

    wvf_message waveform{};
    routing_header header{};

    string packet;
   
    packet.append(reinterpret_cast<const char*>(&header), sizeof(header));
    packet.append(reinterpret_cast<const char*>(&waveform), sizeof(waveform));

    int sockfd;
   
    struct iovec iovecs[BATCH_SIZE]; // Have tried several, 128 seems to be the sweetspot
    struct mmsghdr msgs[BATCH_SIZE];
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    }

    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(HOST_IP);

    memset(msgs, 0, sizeof(msgs));

    for (size_t i = 0; i < BATCH_SIZE; i++) {
            
        msgs[i].msg_hdr.msg_iov = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
        msgs[i].msg_hdr.msg_name = &addr;
        msgs[i].msg_hdr.msg_namelen = sizeof(addr);

    }

    int buf = 64 * 1024 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf));

    size_t counter = 0; 
   

    while (true) {

      if( (counter + BATCH_SIZE) >= data_buffer.size())
       counter = 0;

      
      for (size_t i = 0; i < BATCH_SIZE; i++) {

        iovecs[i].iov_base = reinterpret_cast<void*>(&data_buffer[counter + i]);
        iovecs[i].iov_len  = sizeof(whole_message);
 
      }    


      int sent = sendmmsg(sockfd, msgs, BATCH_SIZE, 0);

      counter += BATCH_SIZE;

      std::this_thread::sleep_for(delay);

      if (sent < 0) {

        perror("Send Message FAILED");
 
      }
    }

    close(sockfd);
}

int main(int argc, char** argv) {


    if(argc != 4){

     cout<<"Usage: ./grets file rate mode "<<endl;

     cout<<"rate: messages/s"<<endl;
     cout<<"mode: 1 - single message 2 - overlapped traces 3 - overlapped and batched (modifies original structure)"<<endl;
     
     return 0 ;

    }


     std::ifstream file(argv[1], std::ios::binary);

     if (!file) 
      throw std::runtime_error("Failed to open file");
    
     vector<whole_message> data_buffer; 

     wvf_custom_message custom_msg{}; 

 
     std::cout << "Size of (custom message) = " << sizeof(custom_msg) << "\n";

     file_reader(data_buffer,file,atoi(argv[3])); 

     cout << "Data buffer size: " << data_buffer.size() << "\n";
   
  
     cout << "Starting data deliverer... goal rate: " << argv[2] << " Hz" <<endl;


     double TARGET_MPS = atof(argv[1]);  // Not precise, aim for higher than required

     // Five cores seems to be enough for now... I can get up to 1.4 M/s (~ 21 GB/s)
     thread t1(sender_loop, 0, OUT_PORT, TARGET_MPS/5, std::ref(data_buffer));
     thread t2(sender_loop, 1, OUT_PORT, TARGET_MPS/5, std::ref(data_buffer));
     thread t3(sender_loop, 2, OUT_PORT, TARGET_MPS/5, std::ref(data_buffer));
     thread t4(sender_loop, 3, OUT_PORT, TARGET_MPS/5, std::ref(data_buffer));
     thread t5(sender_loop, 4, OUT_PORT, TARGET_MPS/5, std::ref(data_buffer));

     cout<<"Running... "<<endl;

     t1.join();
     t2.join();
     t3.join();
     t4.join();
     t5.join();
     
 
     return 0;
}