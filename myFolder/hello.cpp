#include<iostream>
#include <thread> // Thu vi?n cho lu?ng (threads)
#include <chrono> // Thu vi?n cho th?i gian (time)
using namespace std;

int main(){
	int i = 0;
	while(true){
		std::cout << ++i <<  " . hello world\n";
    	this_thread::sleep_for(chrono::milliseconds(1000));
	}
    
    return 0;
}
