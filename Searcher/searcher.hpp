#include<iostream>
#include<unordered_map>
#include<vector>
#include<fstream>
#include<boost/algorithm/string.hpp>
#include"cppjieba/Jieba.hpp"

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";
struct DocInfo{
	uint64_t doc_id;
	std::string title;
	std::string url;
	std::string content;
};

struct Weight{
	std::string key;
	uint64_t doc_id;
	size_t weight;
};

class Index{
	typedef std::vector<Weight> WeightList;
public:
	Index():jieba_(DICT_PATH,
  		      HMM_PATH,
  		      USER_DICT_PATH,
  		      IDF_PATH,
  		      STOP_WORD_PATH){
	}
	const DocInfo* GetDocInfo(uint64_t doc_id) const;
	const WeightList* GetCutWords(const std::string str) const;
	bool Build(const std::string input_path);
	void CutWord(std::string& line,std::vector<std::string>* result);
private:
	void Help(const int& count,const std::string& progress);
	const DocInfo* BuildForward(const std::string& line);
	void BuildInverted(const DocInfo& doc);
private:
	std::vector<DocInfo> forward_index_;
	std::unordered_map<std::string,WeightList> invertes_index_;
	cppjieba::Jieba jieba_;
};

class Searcher{
public:
	Searcher():index_(new Index()){
	}
	
	~Searcher(){
		if(index_) 
			delete index_;
	}
	void Init(const std::string& path);
	void Search(const std::string key,std::string* result);
private:
	static void Sort(std::vector<Weight> & results);
	std::string GetDesc(const std::string& content,const std::string& key);
private:
	Index* index_;
};
