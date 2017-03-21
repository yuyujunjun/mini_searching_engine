#include"searchengine.h"







int main() {
    term_index *term=NULL;

    string file_stopwords="..\\stopwords";//指定stopwords的位置
    stopwords_list=get_stopwords_file(file_stopwords,stopwords_list);//获取stopwords
    term=addfile();
    cout<<term->index.size();
    WriteintoFile(term);
/*****************************************************************************************************/




    //return 0;
}