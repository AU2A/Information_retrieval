#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<cmath>

using namespace std;

struct myDoc
{
    string docName;
    vector<int> wordFreq;
    vector<float> weight1,
                  weight2,
                  weight3,
                  relevance1,
                  relevance2,
                  relevance3;
    vector<string> fileName;
};

fstream file;
int idx,chk,docSize;
myDoc temp;
ofstream oFile;
string str;
stringstream ss;
vector<int> wordList,
            wordZero,
            wordTotal,
            wordContain,
            queryWordContain;
vector<myDoc> doc,query;
char sp='.';

int main(){
    //Variable

    //Read Document list
    if(true){
        cout<<"Read Document list"<<endl;
        file.open("Documents_and_Query\\doc_list.txt",ios::in);
        while(getline(file,str)){
        // for(int i=0;i<40;i++){
            getline(file,str);
            temp.docName=str;
            doc.push_back(temp);
        }
        file.close();
    }

    //Read Query list
    if(true){
        cout<<"Read Query list"<<endl;
        file.open("Documents_and_Query\\query_list.txt",ios::in);
        while(getline(file,str)){
            temp.docName=str;
            query.push_back(temp);
        }
        file.close();
    }

    //Read Document content
    if(true){
        cout<<"Read Document content ";
        for(int index=0;index<doc.size();index++){
            file.open("Documents_and_Query\\Document\\"+doc[index].docName,ios::in);
            doc[index].wordFreq=wordZero;
            getline(file,str);//Read first line,unused
            getline(file,str);//Read second line,unused
            getline(file,str);//Read third line,unused
            while(getline(file,str)){
                ss<<str<<endl;
                while(ss>>idx){
                    if(idx!=-1){
                        chk=0;
                        for(int i=0;i<wordList.size();i++){
                            if(wordList[i]==idx){
                                doc[index].wordFreq[i]++;
                                chk=1;
                            }
                        }
                        if(chk==0){
                            doc[index].wordFreq.push_back(1);
                            wordList.push_back(idx);
                            wordZero.push_back(0);
                        }
                    }
                }
                ss.clear();
            }
            file.close();
            if((index+1) % (doc.size()/10) == 0) cout<<sp;
        }
    }
    
    //Read Query content
    if(true){
        cout<<endl<<"Read Query content";
        for(int index=0;index<query.size();index++){
            file.open("Documents_and_Query\\Query\\"+query[index].docName,ios::in);
            query[index].wordFreq=wordZero;
            while(getline(file,str)){
                ss<<str<<endl;
                while(ss>>idx){
                    if(idx!=-1){
                        chk=0;
                        for(int i=0;i<wordList.size();i++){
                            if(wordList[i]==idx){
                                query[index].wordFreq[i]++;
                                chk=1;
                            }
                        }
                        if(chk==0){
                            query[index].wordFreq.push_back(1);
                            wordList.push_back(idx);
                            wordZero.push_back(0);
                        }
                    }
                }
                ss.clear();
            }
            file.close();
            if((index+1)%(query.size()/10)==0)cout<<sp;
        }
    }

    //Push zero to vector
    if(true){
        cout<<endl<<"Push zero to vector";
        for(int i=0;i<doc.size();i++){
            for(int j=doc[i].wordFreq.size();j<wordZero.size();j++){
                doc[i].wordFreq.push_back(0);
            }
        }
        for(int i=0;i<query.size();i++){
            for(int j=query[i].wordFreq.size();j<wordZero.size();j++){
                query[i].wordFreq.push_back(0);
            }
        }
        //cout<<"push Zero Done"<<endl;
    }

    //Count the frequency of Document term
    if(true){
        cout<<endl<<"Count the frequency of Document term";
        wordTotal = wordZero;
        wordContain = wordZero;
        int sum;
        for(int i=0;i<wordList.size();i++){
            sum=0;
            for(int j=0;j<doc.size();j++){
                sum+=doc[j].wordFreq[i];
                if(doc[j].wordFreq[i]>0){
                    wordContain[i]++;
                }
            }
            wordTotal[i]=sum;
            if((i+1)%(wordList.size()/10)==0)cout<<sp;
        }
    }

    //Count the frequency of Query term
    if(true){
        cout<<endl<<"Count the frequency of Query term";
        queryWordContain = wordZero;
        int sum;
        for(int i=0;i<wordList.size();i++){
            for(int j=0;j<query.size();j++){
                if(query[j].wordFreq[i]>0){
                    queryWordContain[i]++;
                }
            }
            if((i+1)%(wordList.size()/10)==0)cout<<sp;
        }
    }

    //Calculate the weight of Document term
    if(true){
        cout<<endl<<"Calculate the weight of Document term";
        for(int i=0;i<doc.size();i++){
            for(int j=0;j<wordList.size();j++){
                if(wordContain[j]!=0){
                    doc[i].weight1.push_back(doc[i].wordFreq[j]*log(doc.size()/float(wordContain[j])));
                    doc[i].weight2.push_back(1+doc[i].wordFreq[j]);
                    doc[i].weight3.push_back((1+doc[i].wordFreq[j])*log(doc.size()/float(wordContain[j])));
                }
                else{
                    doc[i].weight1.push_back(0);
                    doc[i].weight2.push_back(0);
                    doc[i].weight3.push_back(0);
                }
            }
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
    }

    //Calculate the weight of Query term
    if(true){
        cout<<endl<<"Calculate the weight of Query term";
        float maxt=0;
        for(int i=0;i<query.size();i++){
            for(int j=0;j<wordList.size();j++){
                maxt=0;
                for(int k=0;k<query.size();k++){
                    if(query[k].wordFreq[j]>maxt){
                        maxt = query[k].wordFreq[j];
                    }
                }
                if(wordContain[j]!=0&&maxt!=0){
                    query[i].weight1.push_back((0.5+0.5*(query[i].wordFreq[j]/maxt))*log(doc.size()/float(wordContain[j])));
                    query[i].weight2.push_back((0.5+0.5*(query[i].wordFreq[j]/maxt))*log(1.0+doc.size()/float(wordContain[j])));
                    query[i].weight3.push_back((1+query[i].wordFreq[j])*log(doc.size()/float(wordContain[j])));
                }
                else{
                    query[i].weight1.push_back(0);
                    query[i].weight2.push_back(0);
                    query[i].weight3.push_back(0);
                }
            }
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
    }
    
    //Caculate the Relevance
    if(true){
        cout<<endl<<"Caculate the Relevance1";
        float numerator,queryWeight,docWeight;
        for(int i=0;i<query.size();i++){
            for(int j=0;j<doc.size();j++){
                numerator=0;
                queryWeight=0;
                docWeight=0;
                for(int k=0;k<wordList.size();k++){
                    numerator+=query[i].weight1[k]*doc[j].weight1[k];
                    queryWeight+=query[i].weight1[k]*query[i].weight1[k];
                    docWeight+=doc[j].weight1[k]*doc[j].weight1[k];
                }
                query[i].relevance1.push_back(numerator/(sqrt(queryWeight)*sqrt(docWeight)));
                query[i].fileName.push_back(doc[j].docName);
            }
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
        cout<<endl<<"Caculate the Relevance2";
        for(int i=0;i<query.size();i++){
            for(int j=0;j<doc.size();j++){
                numerator=0;
                queryWeight=0;
                docWeight=0;
                for(int k=0;k<wordList.size();k++){
                    numerator+=query[i].weight2[k]*doc[j].weight2[k];
                    queryWeight+=query[i].weight2[k]*query[i].weight2[k];
                    docWeight+=doc[j].weight2[k]*doc[j].weight2[k];
                }
                query[i].relevance2.push_back(numerator/(sqrt(queryWeight)*sqrt(docWeight)));
                query[i].fileName.push_back(doc[j].docName);
            }
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
        cout<<endl<<"Caculate the Relevance3";
        for(int i=0;i<query.size();i++){
            for(int j=0;j<doc.size();j++){
                numerator=0;
                queryWeight=0;
                docWeight=0;
                for(int k=0;k<wordList.size();k++){
                    numerator+=query[i].weight3[k]*doc[j].weight3[k];
                    queryWeight+=query[i].weight3[k]*query[i].weight3[k];
                    docWeight+=doc[j].weight3[k]*doc[j].weight3[k];
                }
                query[i].relevance3.push_back(numerator/(sqrt(queryWeight)*sqrt(docWeight)));
                query[i].fileName.push_back(doc[j].docName);
            }
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
    }

    //Save Document Info
    if(true){
        cout<<endl<<"Save Document Info";
        oFile.open("document_output.csv", ios::out | ios::trunc);
        for(int i=0;i<wordList.size();i++){
            oFile<<wordList[i];
            if(i!=wordList.size()-1){
                oFile<<",";
            }
        }
        oFile<<endl;
        for(int i=0;i<doc.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<doc[i].wordFreq[j];
                if(i!=wordZero.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
        
        oFile.close();
    }

    //Save Query Info
    if(true){
        cout<<endl<<"Save Query Info";
        oFile.open("query_output.csv", ios::out | ios::trunc);
        for(int i=0;i<wordList.size();i++){
            oFile<<wordList[i];
            if(i!=wordList.size()-1){
                oFile<<",";
            }
        }
        oFile<<endl;
        for(int i=0;i<query.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<query[i].wordFreq[j];
                if(i!=wordList.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
        oFile.close();
    }

    //Save Document Term Weight 
    if(true){
        cout<<endl<<"Save Document Term Weight1";
        //Scheme-1
        oFile.open("doc_weight1.csv", ios::out | ios::trunc);
        for(int i=0;i<doc.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<doc[i].weight1[j];
                if(i!=wordZero.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
        oFile.close();
        //Scheme-2
        cout<<endl<<"Save Document Term Weight2";
        oFile.open("doc_weight2.csv", ios::out | ios::trunc);
        for(int i=0;i<doc.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<doc[i].weight2[j];
                if(i!=wordZero.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
        oFile.close();
        //Scheme-3
        cout<<endl<<"Save Document Term Weight3";
        oFile.open("doc_weight3.csv", ios::out | ios::trunc);
        for(int i=0;i<doc.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<doc[i].weight3[j];
                if(i!=wordZero.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
        oFile.close();
    }

    //Save Query Term Weight 
    if(true){
        //Scheme-1
        cout<<endl<<"Save Query Term Weight1";
        oFile.open("query_weight1.csv", ios::out | ios::trunc);
        for(int i=0;i<query.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<query[i].weight1[j];
                if(i!=wordZero.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
        oFile.close();
        //Scheme-2
        cout<<endl<<"Save Query Term Weight2";
        oFile.open("query_weight2.csv", ios::out | ios::trunc);
        for(int i=0;i<query.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<query[i].weight2[j];
                if(i!=wordZero.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
        oFile.close();
        //Scheme-3
        cout<<endl<<"Save Query Term Weight3";
        oFile.open("query_weight3.csv", ios::out | ios::trunc);
        for(int i=0;i<query.size();i++){
            for(int j=0;j<wordList.size();j++){
                oFile<<query[i].weight3[j];
                if(i!=wordZero.size()-1){
                    oFile<<",";
                }
            }
            oFile<<endl;
            if((i+1)%(query.size()/10)==0)cout<<sp;
        }
        oFile.close();
    }
    
    //Save Query Relevance
    if(true){
        //Scheme-1
        cout<<endl<<"Save Query Relevance1";
        oFile.open("query_Relevance1.csv", ios::out | ios::trunc);
        oFile<<" ,";
        for(int j=0;j<query.size();j++){
            oFile<<"Query "<<j;
            if(query.size()-1!=j) oFile<<",";
        }
        oFile<<endl;
        for(int i=0;i<doc.size();i++){
            oFile<<doc[i].docName<<",";
            for(int j=0;j<query.size();j++){
                oFile<<query[j].relevance1[i];
                if(query.size()-1!=j) oFile<<",";
            }
            oFile<<endl;
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
        oFile.close();
        //Scheme-2
        cout<<endl<<"Save Query Relevance2";
        oFile.open("query_Relevance2.csv", ios::out | ios::trunc);
        oFile<<" ,";
        for(int j=0;j<query.size();j++){
            oFile<<"Query "<<j;
            if(query.size()-1!=j) oFile<<",";
        }
        oFile<<endl;
        for(int i=0;i<doc.size();i++){
            oFile<<doc[i].docName<<",";
            for(int j=0;j<query.size();j++){
                oFile<<query[j].relevance2[i];
                if(query.size()-1!=j) oFile<<",";
            }
            oFile<<endl;
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
        oFile.close();
        //Scheme-3
        cout<<endl<<"Save Query Relevance3";
        oFile.open("query_Relevance3.csv", ios::out | ios::trunc);
        oFile<<" ,";
        for(int j=0;j<query.size();j++){
            oFile<<"Query "<<j;
            if(query.size()-1!=j) oFile<<",";
        }
        oFile<<endl;
        for(int i=0;i<doc.size();i++){
            oFile<<doc[i].docName<<",";
            for(int j=0;j<query.size();j++){
                oFile<<query[j].relevance3[i];
                if(query.size()-1!=j) oFile<<",";
            }
            oFile<<endl;
            if((i+1)%(doc.size()/10)==0)cout<<sp;
        }
        oFile.close();
    }


    system("Pause");
    return 0;
}