#include"searcher.hpp"
int main(){
	Index index;
	index.Build("../data/raw_input/output");
	const auto list = index.GetCutWords("thread");
	for(const auto& l :*list){
	    const DocInfo* doc = index.GetDocInfo(l.doc_id);
        std::cout<<l.key<<":"<<l.weight<<std::endl;
	    std::cout<<doc->title<<":"<<doc->url<<std::endl;
	  }
	return 0;
}
