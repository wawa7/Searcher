#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<boost/algorithm/string.hpp>
	
class Util{
public:
	static bool Read(const std::string& file,std::string* html){	 	
		std::ifstream output_file(file.c_str());
		if(!output_file.is_open()){
			std::cout<<"open file error:"<<file<<std::endl;
			return false;
		}

		std::string line;
		while(std::getline(output_file,line)){
			*html+=line+'\n';
		}

		output_file.close();
		return true;
	}
	static bool Split(const std::string& input,std::vector<std::string>* output
					,const std::string& str){
		boost::split(*output,input,boost::is_any_of(str),boost::token_compress_off);
	}
};
