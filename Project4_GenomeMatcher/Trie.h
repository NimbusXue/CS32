#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <list>
using namespace std;
template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;
    
    // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    struct Node
    {
        char label;
        list<ValueType> values;
        vector<Node*> children;
        Node* head;
    };
    int count;
    Node* root;
 
    
};

template<typename ValueType>
Trie<ValueType>::Trie(){
    root=new Node;
    root->label=' ';
    root->head=root;
    count=1;
}

template<typename ValueType>
Trie<ValueType>::~Trie(){
    reset();
    delete root;
}

template<typename ValueType>
void Trie<ValueType>::reset(){
   if(count!=0){
       // store the children pointers
        vector<Node*> copy;
        for(int i=0;i<root->children.size();i++){
            Node* temp=root->children[i];
            copy.push_back(temp);
        }
        delete root;
        count--;
       //end reset if all nodes are deleted
       if(count==0){
           root=new Node;
           count++;
           root->label=' ';
           root->head=root;
       }else{
           //recursively reset to delete nodes
        for(int i=0;i<copy.size();i++){
            root=copy[i];
            reset();
        }
       }
    }
    
    
}
template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value){
    Node* head=root;
    int index=0;
    if(root!=nullptr){
        for(int j=0;j<head->children.size()&&(head->children[j]!=nullptr);j++){
            //loop to find matching characters
            if((head->children[j])->label==key[index]){
                head=head->children[j];
                j=-1;
                index++;
                if(index==key.size()){
                    //push if exactly matches
                    head->values.push_back(value);
                    return;
                }
            }
        }
    }else{
        head=new Node;
        count++;
        for(int i=0;i<key.size();i++){
            head->children.push_back(new Node);
            count++;
            head->children[0]->label=key[i];
            head=head->children[0];
        }
        head->values.push_back(value);
        return;
    }
    for(int i=index;i<key.size();i++){
        //insert new node to store previouslt non exist characters
        head->children.push_back(new Node);
        count++;
        head->children[head->children.size()-1]->label=key[i];
        head=head->children[head->children.size()-1];
    }
    head->values.push_back(value);
    return;
    
    
    
}
template<typename ValueType>
vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const{
    
    
    bool first=false;
    vector<ValueType> v;
    if(root->head==nullptr){
     return v;
    }
    if(key.size()==0){
      return v;
    }
    if(key.size()==1){
        //compare single character and push back value it key size is only one
        for(int i=0;i<root->head->children.size()&&(root->head->children[i]!=nullptr);i++){
            if((root->head->children[i])->label==key[0]){
                for(typename list<ValueType>::iterator p=(root->head->children[i])->values.begin();p!=(root->head->children[i])->values.end();p++){
                    v.push_back(*p);
                }
                 root->head=root;
                return v;
            }
        }
    }
 
    if(exactMatchOnly){
    for(int i=0;i<root->head->children.size()&&(root->head->children[i]!=nullptr);i++){
        first=false;
        //test the first character
        if((root->head->children[i])->label==key[0]){
            first=true;
            root->head=root->head->children[i];
            break;
         }
        
    }
        if(first){
           //recursively find key
     return find(key.substr(1,key.size()-1),true);
        } else{
             root->head=root;
   return v;
        }
        
    }else{
        for(int i=0;i<root->head->children.size()&&(root->head->children[i]!=nullptr);i++){
            Node* temp=root->head;
            first=false;
            if((root->head->children[i])->label==key[0]){
                first=true;
            }
            if(!first&&root->head->label==' '){
                continue;
            }
            if(first){
                root->head=root->head->children[i];
                // if exactmatch is false, key size of 2 is guaranteed to be effective
                if(key.size()==2){
                    for(int i=0;i<root->head->children.size()&&(root->head->children[i]!=nullptr);i++){
                        for(typename list<ValueType>::iterator p=(root->head->children[i])->values.begin();p!=(root->head->children[i])->values.end();p++){
                            v.push_back(*p);
                        }
                    }
                    
                }
                if(key.size()>2){
                    //recursively find new characters if key size is still greater than 2
                vector<ValueType> t=find(key.substr(1,key.size()-1),false);
                for(int i=0;i<t.size();i++){
                    v.push_back(t[i]);
                }
                }
            }else{
                //no more mismatch is allowed
                    root->head=root->head->children[i];
                    vector<ValueType> t=find(key.substr(1,key.size()-1),true);
                    for(int i=0;i<t.size();i++){
                        v.push_back(t[i]);
                    }
                    
                
            }
            root->head=temp;
            
        }
        root->head=root;
     return v;
    }
    
    
}
#endif // TRIE_INCLUDED
