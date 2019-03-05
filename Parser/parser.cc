#include<iostream>
#include<vector>
#include<unistd.h>
#include<boost/filesystem/operations.hpp>
#include<boost/filesystem/path.hpp>
#include"../common/util.hpp"

struct DocInfo{
	std::string title;
	std::string url;
	std::string content;
};

const std::string g_output_file="../data/raw_input/output";
const std::string g_input_path="../data/input/html/";

bool GetHtmlFile(const std::string& input,std::vector<std::string>* file){
	if(file == NULL)
		return false;
	namespace fs = boost::filesystem;
	fs::path input_path(input);
	if(!fs::exists(input_path)){
		std::cout<<"input directory error"<<std::endl;
		return false;
	}

	fs::recursive_directory_iterator enditer;
	for(fs::recursive_directory_iterator iter(input);iter!=enditer;iter++){
		if(!fs::is_regular_file(*iter)){
			continue;
		}

		if(iter->path().extension()!=".html"){
			continue;
		}
		file->push_back(iter->path().string());
	}
	return true;
}

bool ParseTitle(const std::string& html,std::string* title){
	if(title == NULL)
		return false;
	
	size_t begin;
	size_t end;

	begin=html.find("<title>");
	if(begin == std::string::npos){
		std::cout<<"find <title> error"<<std::endl;
		return false;
	}

	end =html.find("</title>");
	if(end == std::string::npos){
		std::cout<<"find </title> error"<<std::endl;
		return false;
	}
	
	begin+=std::string("<title>").size();
	if(begin>end){
		std::cout<<"begin end error"<<std::endl;
		return false;
	}
	*title = html.substr(begin,end-begin);
	return true;
}

bool ParserUrl(const std::string& file,std::string* url){
	if(url == NULL)
		return false;
	std::string input = "../data/input/";
	std::string prefix("https://www.boost.org/doc/libs/1_53_0/doc/");
	std::string suffix = file.substr(input.size());
	*url = prefix+suffix;
	return true;
}

bool ParserContent(const std::string& html,std::string* content){
	if(content == NULL)
		return false;
	int i = 0;
	bool is_content = true;
	char* str = (char*)html.c_str();
	while(*str){
		if(is_content){
			if(*str == '<'){
				is_content = false;
			}else{
				if(*str == '\n'){
					*str = ' ';
				}
				content->push_back(*str);
			}
		}else{
			if(*str == '>'){
				is_content = true;
			}
		}		
		++str;
	}
	return true;
}

bool ParserFile(const std::string& file,DocInfo* doc){
	if(doc == NULL)
		return false;
	
	std::string html;
	bool ret = Util::Read(file,&html);
	if(!ret){
		std::cout<<"read file error:"<<file<<std::endl;
		return false;
	}
	ret = ParseTitle(html,&doc->title);
	if(!ret){
		std::cout<<"parser title error:"<<file<<std::endl;
		return false;
	}

	ret = ParserUrl(file,&doc->url);
	if(!ret){
		std::cout<<"parser url error:"<<file<<std::endl;
		return false;;
	}

	ret=ParserContent(html,&doc->content);
	if(!ret){
		std::cout<<"parser content error:"<<file<<std::endl;
		return false;
	}
	return true;
}

bool Write(const DocInfo& doc,std::ofstream& output){
	std::string html;
	html=doc.title+'\3'+doc.url+'\3'+doc.content+'\n';
	output.write(html.c_str(),html.size());
	return true;
}

int main(){
	std::vector<std::string> file;
	bool ret = GetHtmlFile(g_input_path,&file);
	if(!ret){
		std::cout<<"get html file error"<<std::endl;
		return 1;
	}
	
	std::ofstream output_file(g_output_file.c_str());
	if(!output_file.is_open()){
		std::cout<<"output file open error"<<std::endl;
		return 1;
	}
	
	const char label[] = "|/-\\";
	std::string progress = "";
	double path_size = file.size();

	printf("path_size=%f\n",path_size);
	double count = 0.0;
	for(const auto& f:file){
		DocInfo doc;
		bool ret = ParserFile(f,&doc);
		if(!ret){
			std::cout<<"parser file error"<<std::endl;
			continue;
		}
		count+=1.0;
	//	std::cout<<doc.title.size()<<doc.url.size()<<doc.content.size()<<std::endl;
		Write(doc,output_file);
		
		double tmp = count/path_size*100;
		const char* str = progress.c_str();
		int size = progress.size();
		if(tmp > progress.size()){
			progress+='#';
		}
	    printf("[%-100s][%d%%][%c]\r",str,size,label[size%4]);
		fflush(stdout);
	}

	output_file.close();
	std::cout<<"Finish"<<std::endl;
	return 0;
}

