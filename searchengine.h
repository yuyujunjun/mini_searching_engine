
#ifndef SEARCHENGINE_SEARCHENGINE_H
#define SEARCHENGINE_SEARCHENGINE_H
#include<fstream>
#include <iostream>
#include<map>


#include<io.h>


#include "stemming/english_stem.h"
#include "stemming/french_stem.h"
#include "stemming/german_stem.h"
#include "stemming/finnish_stem.h"
#include "stemming/swedish_stem.h"
#include "stemming/spanish_stem.h"
#include "stemming/dutch_stem.h"
#include "stemming/danish_stem.h"
#include "stemming/italian_stem.h"
#include "stemming/norwegian_stem.h"
#include "stemming/portuguese_stem.h"
#include "stemming/russian_stem.h"
#include<algorithm>
#define MAX_LETTER 26//字母总数
#define MIN_LEVEL 0//定义关键词的优先级最小级别
using namespace std;
//dict 用以存储term和term的相关信息
struct dict{
    int term_frequency;//涉及到这个term的文章数量
    map<string,int> article_frequency;//文章与文章中出现这个term的次数
};
//建立一个term_index表
struct term_index{
    map<string,dict *> index;
};
//建立一个返回stem的函数
string getstemmedword(string word);
//为了方便查出stopwords，为stopwords建立了一个字典结构
struct stopwords{
public:
    struct stopwords  *next[MAX_LETTER];
    int num;//表示存储的孩子的个数
};
struct keywords{//存储判断用户输入的搜索关键词
    int level;
    string s;
    int num;
};
struct document_by_weight{
    string title;
    int level;
    //当前文章数
};


stopwords *createNewlist();//创建stopwords表
void Insert_str(string str,stopwords *head);//插入新的单词
int Search_str(string str,stopwords *head);//搜索单词
stopwords* get_stopwords_file(string,stopwords*);
term_index* addfile();//将网站的文件添加进字典
stopwords *stopwords_list=NULL;//初始化stopwords列表的值
term_index* ReadingfromFile(term_index*,int);//从已经建好的文件中建立term表
void WriteintoFile(term_index* term);
void searching(term_index *term,int thresholding);
bool cmp(const document_by_weight &t1, const document_by_weight &t2)
{
    if(t1.level > t2.level)
        return true;
    return false;
}
/****************************************************************************/
term_index* ReadingfromFile(term_index *term,int thresholding) {//从已经建好的文件中建立term表
    string file1 = "..\\term_index_all";//定义文件路径
    string file2 = "..\\term_index400";
    string t;//定义存储term的临时变量
    if(thresholding==0) {
        ifstream in(file1);
        if (!in) {
            cout << "open inversion failure" << endl;//如果文件打开失败则显示
        } else {
            cout << "open inversion successed" << endl;//打开文件成功后显示
            while (in >> t) {
                if (term == NULL) {//如果之前内存中还没有term_index，新建一个
                    term = new term_index;
                }
                //读取三次，分别是term_frequency，title
                int term_frequency = 0;
                int article_frequency = 0;
                in >> term_frequency;//将文件中下一个数据作为放入这个term的频率
                dict *temp = NULL;//定义一个临时变量存储term的字典
                temp = new dict;
                temp->term_frequency = term_frequency;
                for (int i = 0; i < term_frequency; i += article_frequency)//读取term_frequency
                {
                    string title;
                    in >> title;//将文件下一个数据作为文章的标题
                    in >> article_frequency;//下一个数据作为文章中出现这个term的次数
                    temp->article_frequency[title] = article_frequency;//保存入最后需要返回的地方
                    term->index[t] = temp;
                }
            }
        }
        return term;
    }
    else {
        ifstream ink(file2);
        if (!ink) {
            cout << "open inversion failure" << endl;//如果文件打开失败则显示
        } else {
            cout << "open inversion successed" << endl;//打开文件成功后显示
            while (ink >> t) {
                if (term == NULL) {//如果之前内存中还没有term_index，新建一个
                    term = new term_index;
                }
                //读取三次，分别是term_frequency，title
                int term_frequency = 0;
                int article_frequency = 0;
                ink >> term_frequency;//将文件中下一个数据作为放入这个term的频率
                dict *temp = NULL;//定义一个临时变量存储term的字典
                temp = new dict;
                temp->term_frequency = term_frequency;
                for (int i = 0; i < term_frequency; i += article_frequency)//读取term_frequency
                {
                    string title;
                    ink >> title;//将文件下一个数据作为文章的标题
                    ink >> article_frequency;//下一个数据作为文章中出现这个term的次数
                    temp->article_frequency[title] = article_frequency;//保存入最后需要返回的地方
                    term->index[t] = temp;
                }
            }
        }
        return term;
    }

}

stopwords *createNewlist()//创建stopwords表
{
    stopwords *p=new stopwords;//新建一个stopwords
    for(int i=0;i<MAX_LETTER;i++)//将它所指的26个字母的位置都初始化
    {
        p->next[i]=NULL;
    }
    p->num=0;
    return p;//返回表
}
void Insert_str(string str,stopwords *head)//插入新的单词
{
    int len=(int)str.length();//获取单词长度
    stopwords *t,*p=head;
    for(int i=0;i<len;i++)//存储进stopwords表中
    {
        int current_position=str[i]-'a';//判断存储位置
        if(p->next[current_position]==NULL)//判断位置是否已经有元素，如果没有，初始化
        {
            t=createNewlist();
            p->next[current_position]=t;
            p->num++;
            p=p->next[current_position];//让p指向下一个位置
        }
        else//如果有元素了，指向下一个位置
        {
            p=p->next[current_position];
        }
    }
}
int Search_str(string str,stopwords *head)//搜索单词
{
    stopwords *p=head;
    int len=(int)str.length();//获取单词长度
    for(int i=0;i<len;i++)
    {
        int c=str[i]-'a';
        if(p->next[c]==NULL)//如果表格中下个位置没有元素，则证明单词不存在表中，返回0
        {
            return 0;
        }
        else//继续往下直到找到整个单词
        {
            p=p->next[c];
        }
    }
    return 1;//成功则返回1
}
string getstemmedword(string word)//建立一个返回stem的函数
{
    //以下步骤用于返回确切的英语单词的stem，并转换为string 类型，reference：https://github.com/OleanderSoftware/OleanderStemmingLibrary
    stemming::english_stem<> StemEnglish;
    wstring w(L"");
    string ANSIWord(word);
    wchar_t* UnicodeTextBuffer = new wchar_t[ANSIWord.length()+1];
    wmemset(UnicodeTextBuffer, 0, ANSIWord.length()+1);
    mbstowcs(UnicodeTextBuffer, ANSIWord.c_str(), ANSIWord.length());
    w = UnicodeTextBuffer;
    StemEnglish(w);
    string str(w.length(), ' ');
    std::copy(w.begin(), w.end(), str.begin());
    return str;
}
stopwords* get_stopwords_file(string str,stopwords* list)//建立stopwords的表格
{
    ifstream in;
    in.open(str);//打开stopwords文件
    if(!in){
        cout<<"open stopwords failure"<<endl;
    }
    string s;
    list=createNewlist();//创建新的表格
    while(in>>s)
    {
        Insert_str(s,list);//不断从文件中插入单词
    }
    in.close();
    return list;
}

void searching(term_index *term,int thresholding)//搜索引擎的搜索部分
{
    string file_stopwords="..\\stopwords";//指定stopwords的位置
    stopwords_list=get_stopwords_file(file_stopwords,stopwords_list);//获取stopwords
    cout<<"stopwords get completed"<<endl;
    keywords key[10];//建立变量存储用户输入的关键词，系统自动截取前十个关键词
    int keynum=0;
    for(int i=0;i<10;i++) {
        key[i].level = 0;
    }//初始化key的优先级
    string keywords;

    for(int i=0;i<10;i++)//分离keywords，使其能够存储在string 数组里
    {
        cin>>keywords;
        if(keywords=="-1")break;
        else {
            key[i].s=keywords;
            keynum++;//统计用户输入了几个单词
        }
    }

        for (int i = 0; i < keynum; i++) {//对用户输入的每个关键词进行操作
            key[i].s = getstemmedword(key[i].s);
            transform(key[i].s.begin(), key[i].s.end(), key[i].s.begin(), ::tolower);//转换为全小写的stem 形式
            int flag = 0;
            for (int j = 0; j < key[i].s.length(); j++) {
                if ((key[i].s[j] < 'a' || key[i].s[j] > 'z'))flag = 1;//如果单词包含其他符号，不处理
            }
            if (flag == 1 || Search_str(key[i].s, stopwords_list)) {//判断它是否在stopwords_list里，如果在，则为stopwords，将优先级置为最低，暂时不管它
                key[i].level = MIN_LEVEL;
            } else {
                map<string, dict *>::iterator it = term->index.find(key[i].s);
                if (it == term->index.end()) {//如果所有文章都没有这个关键词
                    if (i == keynum)//            且所有关键词都是这样，提示用户关键词无效
                    {
                        cout << "some keywords is invaild" << endl;
                    }
                } else {
                    map<string, int>::const_iterator jt = term->index[it->first]->article_frequency.begin();
                    if (thresholding == 0) {//使用document
                        document_by_weight temp[100];//最多显示100篇文章
                        for (int i = 0; i < 100; i++) {
                            temp[i].level = 0;

                            temp[i].title = " ";
                        }
                        int documentNUM=0;
                        while (jt != term->index[it->first]->article_frequency.end())//列举有该关键词的所有文章和出现关键词的次数
                        {
                            temp[documentNUM].title = jt->first;//将文章标题存入临时变量的title，将文章的词频存入当前文章的等级
                            temp[documentNUM].level = jt->second;
                            if (documentNUM < 100)documentNUM++;
                            else break;
                            jt++;
                        }
                        /**************************************************************/
                        ;//储存文章数以防sort后改变
                        sort(temp, &temp[documentNUM], cmp);
                        for (int i = 0; i <documentNUM; i++) {
                            cout << temp[i].title << "\t" << temp[i].level << endl;//输出文章
                        }
                    } else {//使用sort方法
                        while (jt != term->index[it->first]->article_frequency.end())//列举有该关键词的所有文章和出现关键词的次数
                        {
                            cout<<jt->first<<"\t"<<jt->second<<endl;
                            jt++;
                        }
                    }
                }
            }
        }
}
term_index* addfile()//建立倒排变量
{
    term_index *term=NULL;
    _finddata_t fileDir;
    string dir="..\\all_articals\\*.txt";//打开所有文章
    long Dir;
    if((Dir = _findfirst(dir.c_str(),&fileDir))==-1l)
        cout<<"No file is found"<<endl;
    else{
        do{
            /**********************************************************************************************/
            ifstream in;
            string d="..\\all_articals\\";
            string filedir=d+fileDir.name;//添加路径
            in.open(filedir);//打开具体文件
            if(!in){
                cout<<"open txt failure"<<endl;//如果打开失败
            }
            string s;
            while(in>>s){//保存文章中每一个单词直到保存完毕
                s=getstemmedword(s);
                transform(s.begin(), s.end(), s.begin(), ::tolower);//将单词转换为小写的stem的形式
                int flag=0;
                for(int i=0;i<s.length();i++){
                    if((s[i]<'a'||s[i]>'z'))flag=1;//如果单词包含其他符号，不处理
                }
                if(flag!=1&&!Search_str(s,stopwords_list))//如果该单词的原始形式不是stopwords或包含其他奇怪字符
                {
                    if(term==NULL){
                        term=new term_index;//新建一个term
                    }
                    map<string,dict*>::iterator it= term->index.find(s);
                    if(it == term->index.end()) {//没找到相应数据
                        dict* temp=new dict;
                        temp->term_frequency =0;//将这个term的出现次数置为0
                        temp->article_frequency[fileDir.name]=1;//将文章中出现这个term的次数置为1
                        term->index[s]=temp;
                    }
                    else {//找到了，已经有了term，现在判断这篇文章之前是否出现了这个term
                        /********************************************************/
                        map<string,int>title=term->index[s]->article_frequency;
                        map<string,int>::iterator it= title.find(fileDir.name);//建立好迭代的变量

                        if(it == title.end()) {
                            term->index[s]->article_frequency[fileDir.name]=1;//如果文章之前还没出现过，则现在出现了，置为1
                        }
                        else {
                            term->index[s]->article_frequency[fileDir.name]++;//否则出现的次数加1
                        }
                        /**************************************************************/
                    }
                }
            }
            /***************************************************************************************************/
        }while( _findnext( Dir, &fileDir ) == 0 );
    }
    _findclose(Dir);//关闭文件
    if(term!=NULL){//所有文章都读取完毕后，再来统计每个term有多少文章出现，且每篇文章出现多少次
        map<string,dict *>::const_iterator it=term->index.begin() ;//做一个迭代器
        while(it!=term->index.end())
        {
            map<string,int>::const_iterator jt=term->index[it->first]->article_frequency.begin();
            while(jt!=term->index[it->first]->article_frequency.end())
            {
                term->index[it->first]->term_frequency += term->index[it->first]->article_frequency[jt->first];//每出现一篇文章，就加上那篇文章出现的该term的次数
                jt++;
            }
            it++;
        }}

    return term;
};
void WriteintoFile(term_index* term)//将做好的term目录写入文件
{

    ofstream SaveFile1("..\\term_index400");//用来存储term频率为400以下的
    ofstream SaveFile2("..\\term_index800");//400到800
    ofstream SaveFile3("..\\term_index800+");//800以上的
    ofstream SaveFile4("..\\term_index_all");//存储所有的term
    map<string,dict *>::const_iterator it=term->index.begin() ;//做一个迭代器
    while(it!=term->index.end())
    {
        string file;
        ofstream *SaveFile;
        if(it->second->term_frequency<=400)SaveFile=&SaveFile1;//如果小于400，存储在第一个文件

        else if(it->second->term_frequency>400&&it->second->term_frequency<800)SaveFile=&SaveFile2;//400到800存储第二个文件
        else if(it->second->term_frequency>=800)SaveFile=&SaveFile3;//800以上第三个文件

        *SaveFile<<it->first<<"\t"<<term->index[it->first]->term_frequency<<"\t";
        SaveFile4<<it->first<<"\t"<<term->index[it->first]->term_frequency<<"\t";
        map<string,int>::const_iterator jt=term->index[it->first]->article_frequency.begin();
        //再写入每个term的后面写入文章信息
        while(jt!=term->index[it->first]->article_frequency.end())
        {
            *SaveFile<<jt->first<<"\t"<<jt->second<<endl;
            SaveFile4<<jt->first<<"\t"<<jt->second<<endl;
            jt++;
        }
        it++;

    }
    //关闭文件
    SaveFile1.close();
    SaveFile2.close();
    SaveFile3.close();
    SaveFile4.close();
}
#endif //SEARCHENGINE_SEARCHENGINE_H
