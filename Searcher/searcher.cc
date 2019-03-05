#include"searcher.hpp"
#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include<jsoncpp/json/json.h>
#include"../common/utiil.hpp"


const DocInfo* Index::GetDocInfo(uint64_t doc_id){
	if(doc_id > forward_indedx_.size()){
		std::cout<<"get doc info fail"<<std::endl;
		return nullptr;
	}
	return &forward_index_[doc_id];
}

const WeightList* Index::GetCutWords(const std::string str){
	auto pos = inverted_index_.find(str);
	if(auto == inverted_index_.end()){
		std::cout<<"get cut words fail"<<std::endl;
		return nullptr;
	}

	return &(pos->second);
}

const DocInfo* Index::BuildForward(const std::string line){
	std::vector<std::string> results;
	Util:: Split(line,&results);
	if(results.size() != 3){
		std::cout<<"buildforward error"<<std::endl;
		return NULL;
	}
	DocInfo doc;
	doc.doc_id = forward_index_.size();
	doc.title = result[0];
	doc.url = result[1];
	doc.content = result[2];
	forward_index_.push_back(doc);
	return &forward_index_.back();
}

void Index::BuildInverted(const DocInof& doc){
	std::vector<std::string> content;
	std::vector<std::string> title;
	CutWord(doc.title,&title);
	CutWord(doc.content,&content);

	struct WordCnt{
		size_t title;
		size_t content;
	};

	std::unordered_map<std::string,WordCnt> word_cnt;
	for(const auto& word: title){
		boost::to_lower(word);
		++word_cnt[word].title;
	}

	for(const auto& word: content){
		boost::to_lower(word);
		++word_cnt[word].content;
	}

	for(const auto& word: word_cnt){
		Weight weight;
		weight.doc_id = doc_id;
		weight.key = word.first;
		weight.weight = word.second.title*10 + word.second.content;

		WeightList& list = inverted_index_[word.first];
		list.push_back(weight);
	}
}

static void Index::Help(const int& count,std::string progress){
	double all = 5828.0;
	std::string label = "|/-\\";

	int tmp = count/all*100.0;
	int size = progress.size();
	if(tmp > size){
		progress+='#';
	}

	const char* temp = progress.s_str();
	printf("[%-100s][%d%%][%c]\r",temp,size,label[size%4]);
	fflush(stdout);
}

bool Index::Build(const std::string file){
	std::ifstream input_file(file.c_str());
	if(!input_file.is_open()){
		std::cout<<"build open file error"<<std::endl;
		return  false;
	
	int count = 0;
	std::string progress;
	std::string line;
	while(std::getline(input_file,line)){
		count++;
		Help(count,progress); 
		const DocInfo* doc = BuildForward(line);
		if(doc == NULL)
			continue;
		BuildInverted(*doc);
	}
	input_file.close();
	return true;

}

void Index::CutWord(const std::string content,std::vector<std:string>* result){
	jieba_.CutForSearch(content,*result);
}

void Searcher::Init(const std::string& path){
	index_.Build(path);
}

std::string Searcher::GetDesc(const std::string& content,const std::string& key){
	size_t pos = content.find(key);
	if(pos == std:string::npos){
		if(content.size() < 160)
			return content;
		else
			return content.substr(0,160)+"...";
	}

	size_t begin = pos<60?0:pos-60;
	size_t end = 0;
	if(content.size() < 160){
		return content.substr(begin);
	}else{
		return content.substr(begin,160)+"...";
	}
}


static void Searcher::Sort(std::vector<Weight>& results){
	std::sort(results.begin(),results.end(),[](const Weight& w1,
			const Weight& w2){return w1.doc_id<w2.doc_id;});

	auto w = results.begin();
	auto it = results.begin();
	if(it != results.end()){	
		++it;
	}

	while(it != v.end()){
		if(w->doc_id == it->doc_id){
			w->weight+=it->weight;
			it = earse(it);
		}else{
			w = it;
			++it;
		}
	}

	std::sort(results.begin(),results.end(),[](const Weight& w1,cost Weight& w2
				){return w1.weight>w2.weight;});
}

void Searcher::Search(const std::string& key,std::vector<std::string>* json_results){
	std::vector<std::string> results;
	index_->CutWord(key,&results);

	std::vector<Weight> list_results;
	for(const auto& word:results){
		boost::to_lower(word);
		auto list = index_->GetCutWords(word);
		if(list == nullptr)
			continue;
		list_results.insert(list_results.end(),list.begin(),list.end());
	}
	

	Sort(list_results);
	//sort
	//std::sort(list_results.begin(),list_results.end(),[](const Weight& w1,
	//		const Weight&w2){return w1.weight>w2.weight;});

	Json::Value results;
	for(const auto& w : list_results){
		const DocInfo* doc = index_->GetDocInfo(w.doc_id);
		Json::Value result;
		result["title"] = doc->title;
		result["url"] = doc->url;
		result["desc"] = GetDesc(doc->content,w.key);
		results.append(result);
	}
	Json:;FastWritter writer;
	*json_result = writer.write();
}
