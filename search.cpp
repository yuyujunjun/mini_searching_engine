//
// Created by fcsx1 on 3/19/2017.
//
#include"searchengine.h"



int main()
{
    term_index *term=NULL;
    int thresholding = 1;//定义thresholding 变量，为0使用document，为1使用query
    term=ReadingfromFile(term,thresholding);//从文章中建立关键词表
    searching(term,thresholding);

}
