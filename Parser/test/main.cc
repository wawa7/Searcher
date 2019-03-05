#include<iostream>
#include<stdio.h>
#include<unistd.h>
int main(){
	double all = 5000.0;
	std::string line = "";
	const char* label = "|/-\\";
	for(int i=1; i<=5000; i++){
		int tmp = i/all*100;
		if(tmp>line.size())
			line+='#';
		const char* temp = line.c_str();
		int size = line.size();
		printf("[%-100s][%d%%][%c]\r",temp,size,label[i%4]);
		fflush(stdout);
		usleep(10000);
	}

	return 0;
}
