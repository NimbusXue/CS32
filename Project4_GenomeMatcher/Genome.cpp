#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <cctype>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string m_name;
    string m_sequence;
    static bool checkATCGN(string &s);
    static bool checkName(string &s);
   
    
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
    m_name=nm;
    m_sequence=sequence;
   
    
}
bool GenomeImpl::checkATCGN(string &s){
    if(s.size()==0)
        return false;
    for(int i=0;i<s.size();i++){
        if(toupper(s[i])!='A'&&toupper(s[i])!='T'&&toupper(s[i])!='C'&&toupper(s[i])!='G'&&toupper(s[i])!='N'){
            return false;
        }
    }
    return true;
}

bool GenomeImpl::checkName(string &s){
    if(s.size()<2){
        return false;
    }
    if(s[0]!='>'){
        return false;
    }
    return true;
    
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
     string name1="";
    string sequence1="";
    bool isname=false;
    string s;
    while(getline(genomeSource, s)){
        //continuously read from the text
        bool a=checkName(s);
        bool b=checkATCGN(s);
        if(!a&&!b){
            return false;
        }
        if(b&&name1==""){
            return false;
        }
        //use checkName function to check name
        if(a){
            if(isname){
                return false;
            }else if(!isname&&name1!=""){
               genomes.push_back(Genome(name1,sequence1));
                sequence1="";
            }
            //remove the > sign
           name1=s.substr(1,s.size()-1);
            isname=true;
     
        }
        //use check letter function to check DNA sequence
        if(b){
            sequence1+=s;
             isname=false;
        }
     
    }
    if(sequence1!=""){
        genomes.push_back(Genome(name1,sequence1));
    }else{
        return false;
    }
   
     return true;
    
}

int GenomeImpl::length() const
{
    return m_sequence.size();
}

string GenomeImpl::name() const
{
    return m_name;
}
bool GenomeImpl::extract(int position, int length, string& fragment) const
{
   if(position<0||length<0||position>=m_sequence.size()||position+length>m_sequence.size())
       return false;
    //make sure it won't extract out of bound
   fragment= m_sequence.substr(position,length);
    return true;  // This compiles, but may not be correct
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
