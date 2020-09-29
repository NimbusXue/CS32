#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Trie.h"


using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    struct info{
        info(int GenomeP, int fragmentP){
            Genomeposition=GenomeP;
            fragmentposition=fragmentP;
        }
        int Genomeposition;
        int fragmentposition;
    };
    
    int m_minSearchLength;
    Trie<info> trie;
    vector<Genome> VG;
    
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    m_minSearchLength=minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    VG.push_back(genome);
    string fragment;
    for(int i=0;i+m_minSearchLength<=genome.length();i++){
        if(genome.extract(i, m_minSearchLength, fragment)){
            //use trie data structure to insert the fragment of the genome
            trie.insert(fragment, info(VG.size()-1, i));
        }
        
    }
    
    
    
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    
    if(fragment.size()<minimumLength||minimumLength<m_minSearchLength){
        return false;
    }
    bool final=false;
    
    if(exactMatchOnly){
        vector<info> VI=trie.find(fragment.substr(0,m_minSearchLength), exactMatchOnly);
        for(int i=0;i<VI.size();i++){
            
            string f; bool extract; int countmax=0;
            int fp= VI[i].fragmentposition;  int gp=VI[i].Genomeposition;
            //extract fragments of matching genome with fragment length
            if(fp+fragment.size()>VG[gp].length()){
                extract= VG[gp].extract(fp, VG[gp].length()-fp, f);
            }else{
                extract= VG[gp].extract(fp, fragment.size(), f);
            }
            if(extract){
                //counting their amount of matching characters
                for(int i=0;i<f.size();i++){
                    if(f[i]==fragment[i]){
                        countmax++;
                    }else{
                        break;
                    }
                }
            }
            if(countmax>=minimumLength){
                //if matching fulfills,continue to extract their more info
                if(!final){
                    while(!matches.empty()){
                        matches.pop_back();
                    }
                }
                final=true;
                bool exist=false;
                DNAMatch a; a.genomeName=VG[gp].name();a.position=fp;a.length=countmax;
                for(int i=0;i<matches.size();i++){
                    if(matches[i].genomeName==a.genomeName){
                        exist=true;
                        //replace with the one of larger matching length if name exists
                        if(a.length>matches[i].length){
                            matches[i]=a;
                        }
                        break;
                    }
                }
                if(!exist){
                    matches.push_back(a);
                }
            }
            
        }
    }
    if(!exactMatchOnly){
        vector<info> VI=trie.find(fragment.substr(0,m_minSearchLength), exactMatchOnly);
        for(int i=0;i<VI.size();i++){
            string f; bool extract; int countmax=0;int countd=0;
            int fp= VI[i].fragmentposition;  int gp=VI[i].Genomeposition;
            //extract fragments of matching genome with fragment length
            if(fp+fragment.size()>VG[gp].length()){
                extract= VG[gp].extract(fp, VG[gp].length()-fp, f);
            }else{
                extract= VG[gp].extract(fp, fragment.size(), f);
            }
            if(extract){
                for(int i=0;i<f.size();i++){
                    //counting their amount of matching characters
                    if(f[i]==fragment[i]){
                        countmax++;
                    }else{
                        countmax++;
                        countd++;
                        //break the loop if mismatch reaches 2
                        if(countd==2){
                            countmax--;
                            countd--;
                            break;
                        }
                    }
                }
            }
            if(countmax>=minimumLength&&countd<=1){
                    //if matching fulfills,continue to extract their more info
                if(!final){
                    while(!matches.empty()){
                        matches.pop_back();
                    }
                }
                final=true;
                bool exist=false;
                DNAMatch a; a.genomeName=VG[gp].name();a.position=fp;a.length=countmax;
                for(int i=0;i<matches.size();i++){
                    if(matches[i].genomeName==a.genomeName){
                        exist=true;
                        //replace with the one of larger matching length if name exists
                        if(a.length>matches[i].length){
                            matches[i]=a;
                        }
                        break;
                    }
                }
                if(!exist){
                    matches.push_back(a);
                }
            }
            
        }
        
    }
    return final;
    
}

double countHowMany(vector<string> v,string n){
    //count the amount of matching characters
    double count=0;
    for(int i=0;i<v.size();i++){
        if(v[i]==n)
            count++;
    }
    return count;
}
//help with the algorithm function
bool isGreater(GenomeMatch g1,GenomeMatch g2){
    
    if(g1.percentMatch>g2.percentMatch){
        return true;
    }
    if(g1.percentMatch==g2.percentMatch){
        return g1.genomeName<g2.genomeName;
    }
    return false;
}
bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    
    if(fragmentMatchLength<m_minSearchLength){
        return false;
    }
    while(!results.empty()){
        results.pop_back();
    }
    vector<string> store;
    int s=query.length()/fragmentMatchLength;
    
    for(int i=0;i+fragmentMatchLength<=query.length();i+=fragmentMatchLength){
    // extract all the fragments
        string f;
        vector<DNAMatch> m;
        query.extract(i, fragmentMatchLength, f);
        
        if(findGenomesWithThisDNA(f, fragmentMatchLength, exactMatchOnly, m)){
            for(int i=0;i<m.size();i++){
                //push back the potential related Genomes
                store.push_back(m[i].genomeName);
                
            }
            
        }
        
    }
    double number=0;
    for(int i=0;i<VG.size();i++){
        //count the number of matching sequence fragments for each Genome
        number=countHowMany(store,VG[i].name());
        if(number/s*100>=matchPercentThreshold){
            //push back the finally satisfied Genome into results
            GenomeMatch g;g.genomeName=VG[i].name();g.percentMatch=number/s*100;
            results.push_back(g);
        }
    }
    if(results.size()>0){
        //sort with the algorithm function
        sort(results.begin(), results.end(), isGreater);
        return true;
    }
    
    
    
    return false;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
