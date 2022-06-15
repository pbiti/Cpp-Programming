/*
 * Biti Polyxeni 2582
 */

#include "AVL.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <stack>  

 using namespace std;
 
 AVL::Node::Node(const string& e, Node *p, Node *leftp, Node *rightp){
     element = e;
     parent = p;
     left = leftp;
     right = rightp; 
     height = 1;
}
AVL::Node::Node(const AVL::Node* o){
    if(o == NULL){*this = NULL;}
    //else{*this = *o;}
    else{
        this->setParent(o->getParent());
        this->setElement(o->getElement());
        this->setLeft(o->getLeft());
        this->setRight(o->getRight());
        this->height = o->getHeight();
    }
}

AVL::Node* AVL::Node::getParent() const {return parent;}

AVL::Node* AVL::Node::getLeft() const {return left;}

AVL::Node* AVL::Node::getRight() const {return right;}

string AVL::Node::getElement() const {return element;}

int AVL::Node::heightwithNode(const AVL::Node* n) const{
    if(n==NULL){return 0;}
    else{return n->height;}
}

int AVL::Node::getHeight() const {
    
    return heightwithNode(this);
}

void AVL::Node::setLeft(Node *n){left = n;}

void AVL::Node::setRight(Node *n){right = n;}

void AVL::Node::setParent(Node *n){parent = n;}

void AVL::Node::setElement(string s){element = s;}

bool AVL::Node::isLeft() const {
    if(parent == NULL){return false;} 
    if((element.compare(parent->element))<0){return true;}
    else{return false;}
}

bool AVL::Node::isRight() const {
    if(parent == NULL){return false;} 
    if((element.compare(parent->element))>0){return true;}
    else{return false;}
}

int AVL::Node::rightChildHeight() const {
    if(right !=NULL) {return right->height;}
    else{return 0;}
}

int AVL::Node::leftChildHeight() const {
    if(left!=NULL) {return left->height;}
    else{return 0;}
    
}

int AVL::Node::updateHeight(){
    int max;
    int rheight = rightChildHeight();
    int lheight = leftChildHeight();
    max = (rheight > lheight) ? rheight+1 : lheight+1;
    height = max;
    return max;
}

bool AVL::Node::isBalanced(){
    if(abs(leftChildHeight() - rightChildHeight()) <= 1){
        return true;
    }
    else{return false;}
}

AVL::AVL(){
    root = NULL;
    size = 0;
}
AVL::~AVL(){
    EmptyTree(root);
}

AVL::AVL(const AVL& avlpo){
     root = NULL;
     size = 0;
//     CopyTree(avlpo.root);
    AVL::Iterator it = avlpo.begin();
    for(int i=0; i<avlpo.size; i++){
        add(*it);
        ++it;
    }
}

bool AVL::search(AVL::Node* r, string word){    
    AVL::Node* current = r;
    
    if(current == NULL){return false;}

    if(word.compare(current->getElement())==0){return true;}
    
    if(word.compare(current->getElement()) < 0){return search(current->getLeft(), word);}
    else{ return search(current->getRight(), word);}
    
}

bool AVL::contains(string s){
    
    return search(root, s);
    
}

AVL::Node* AVL::rotateLeft(AVL::Node* n){
    AVL::Node* e = n->getRight();
    AVL::Node* temp = e->getLeft();
 
    e->setLeft(n);
    n->setRight(temp);

    n->updateHeight();
    e->updateHeight();
    return e;
}

AVL::Node* AVL::rotateRight(AVL::Node* n){
    AVL::Node* e = n->getLeft();
    AVL::Node* temp = e->getRight();
 
    e->setRight(n);
    n->setLeft(temp);
 
    n->updateHeight();
    e->updateHeight();
    
    return e;
}

AVL::Node* AVL::selfBalance(AVL::Node* n){
    n->updateHeight();
   
    
    int balance = n->leftChildHeight() - n->rightChildHeight();
    
    
    if(balance > 1){
        if(n->getLeft()->getLeft()->getHeight() >= n->getLeft()->getRight()->getHeight()){
            if(n == root){ root = rotateRight(n); return root;}
        
            return rotateRight(n);
        }
        
        else if(n->getLeft()->getLeft()->getHeight() < n->getLeft()->getRight()->getHeight()){
            
            if(n == root){root->setLeft(rotateLeft(root->getLeft())); root = rotateRight(n); return root;}
            
            else {n->setLeft(rotateLeft(n->getLeft())); return rotateRight(n);}
            
        }
    }
    if(balance < -1){

        if(n->getRight()->getRight()->getHeight() >= n->getRight()->getLeft()->getHeight()){			
            
            if(n == root){root = rotateLeft(n); return root;}
            
            
            return rotateLeft(n);
        }
        else if(n->getRight()->getRight()->getHeight() < n->getRight()->getLeft()->getHeight()){
            
            if(n == root){root->setRight(rotateRight(root->getRight())); root = rotateLeft(n); return root;}
            else{n->setRight(rotateRight(n->getRight())); return rotateLeft(n);} 
            
        }
    }
    
    return n;
}
    

AVL::Node* AVL::insertion(AVL::Node* r, string s, AVL::Node* p){
    if(r==NULL){
        
         r = new AVL::Node(s, p, NULL, NULL);
        //AVL::Node * temp = new AVL::Node(s, p, NULL, NULL);
        size++; 
        return r;
    }
    
    if(s.compare(r->getElement())==0){
//         cerr << "Key " << s <<" already exists" << endl;
        return r;
    }
    if(s.compare(r->getElement()) < 0){
        
        r->setLeft(insertion(r->getLeft(), s, r));
        
    }else{
        
        r->setRight(insertion(r->getRight(),s,r));
    }
    
    r = selfBalance(r);
    return r;
}

bool AVL::add(string s){
    if(root == NULL){
        root = insertion(root, s, NULL);
    }else{
        insertion(root, s, NULL);
    }
    if(AVL::contains(s)){return true;}
    return false;
}

AVL::Node* AVL::minRightSubtree(AVL::Node* r){
    
    AVL::Node* current = r;

    while(current->getLeft()!=NULL){
        current = current->getLeft();
    }
    return current;
}

AVL::Node* AVL::deletion(AVL::Node* r, string k){

    if(r == NULL){
//         cerr << "Key " << k << " not found" << endl;
        return r;
    }
    if(k.compare(r->getElement()) < 0){
        r->setLeft(deletion(r->getLeft(), k));
    }else if(k.compare(r->getElement()) > 0){
        r->setRight(deletion(r->getRight(), k));
    }else{
        if(size==1){root = NULL; return root;}
        if(r->getLeft()==NULL || r->getRight()==NULL){
            AVL::Node* temp = r->getRight() ? r->getRight() : r->getLeft();
            
            if(temp==NULL){
                temp = r;
                delete r;
                r = NULL;

            }else{
                *r = *temp;
                delete temp;
            }
        }else{
            
            AVL::Node* temp = minRightSubtree(r->getRight());
            r->setElement(temp->getElement());
            r->setRight(deletion(r->getRight(), temp->getElement()));
            /**r = *temp;
            r->setRight(deletion(r->getRight(), temp->getElement()));*/
        }
    }
    if(r == NULL){return r;}
    return selfBalance(r);
}

bool AVL::rmv(string s){
    if(!AVL::contains(s)){return false;}
    AVL::deletion(root, s);
    
    if(!AVL::contains(s)){size--; return true;}
    return false;
}

void AVL::recurPreorder(const Node* r, std::ostream& out){
    
    if(r !=NULL){
        out << r->getElement() << " ";
        recurPreorder(r->getLeft(), out);
        recurPreorder(r->getRight(), out);
    }
}

void AVL::pre_order(std::ostream& out){
    if(root==NULL){return;}
    recurPreorder(root, out);
}

void AVL::EmptyTree(AVL::Node* r){
    
    if(r==NULL){return;}

    AVL::Iterator it = this->begin();
    rmv(*it);
    EmptyTree(root);
    size = 0;
    return;
}

AVL& AVL::operator=(const AVL& avl){   
    EmptyTree(root);
    AVL::Iterator it = avl.begin();
    for(int i=0; i<avl.size; i++){
        add(*it);
        ++it;
    }
    return *this;
}

AVL AVL::operator+(const AVL& avl){
    AVL newtree;
    
    AVL::Iterator itleft = this->begin();
    
    AVL::Iterator itright = avl.begin();
    
    for(int i=0; i<this->size; i++){
        newtree.add(*itleft);
        ++itleft;
    }
    for(int k=0; k<avl.size; k++){
        newtree.add(*itright);
        ++itright;
    }
    return newtree;
}

AVL& AVL::operator+=(const AVL& avl){
    AVL::Iterator itright = avl.begin();
    
    for(int i=0; i<avl.size; i++){
        add(*itright);
        ++itright;
    }
    return *this;
}

AVL AVL::operator+(const string& e){
    AVL newtree;
    AVL::Iterator itleft = this->begin();
    
    for(int i=0; i<this->size; i++){
        newtree.add(*itleft);
        ++itleft;
    }
    newtree.add(e);
    
    return newtree;
}

AVL AVL::operator-(const string& e){
    AVL::Iterator itleft = this->begin();
    AVL newtree;

    for(int i=0; i<this->size; i++){
        newtree.add(*itleft);
        ++itleft;
    }
    newtree.rmv(e);
    return newtree;
}

AVL& AVL::operator-=(const string& e){
    this->rmv(e);
    return *this;
}

AVL& AVL::operator+=(const string& e){
    this->add(e);
    return *this;
}

AVL::Iterator::Iterator(const Iterator &it){
    node = it.node;
}

AVL::Iterator::Iterator(AVL::Node* n, AVL::Node* p){
    if(n == NULL){node = NULL;}
    else{
        node = n;
        node->setParent(p);
    }
}

AVL::Iterator AVL::begin() const{
    //return root of AVL tree
    return Iterator(root, NULL);
    
}

AVL::Iterator AVL::end() const{
    AVL::Node* r;
    AVL::Node* temp;
    r = root;
    
    while(r->getRight()!=NULL){
        temp = r;
        r = r->getRight();
        r->setParent(temp);
    }
    
//     return Iterator(r, r->getParent());
    return Iterator(NULL, NULL);
}


AVL::Iterator& AVL::Iterator::operator++(){

    if(node->getRight()!=NULL){nodeStack.push(node->getRight());}
    if(node->getLeft()!=NULL){nodeStack.push(node->getLeft());}
    
    if(nodeStack.empty()==false){
        node = nodeStack.top();
        nodeStack.pop();
    }else{node = NULL;}
    
    
    return *this;
}

AVL::Iterator AVL::Iterator::operator++(int a){
    Iterator it (*this);
    
    if(node->getRight()!=NULL){nodeStack.push(node->getRight());}
    if(node->getLeft()!=NULL){nodeStack.push(node->getLeft());}
    
    if(nodeStack.empty()==false){
        node = nodeStack.top();
        nodeStack.pop();
    }else{node = NULL;}
    
    return it;
}

string AVL::Iterator::operator*(){
    return node->getElement();
}

bool AVL::Iterator::operator!=(Iterator it){
    if(this->node == NULL && it.node!=NULL){return true;}
    else if(this->node != NULL && it.node == NULL){return true;}
    else if(this->node == NULL && it.node == NULL){return false;}
    else if(this->node->getElement() != it.node->getElement()){return true;}
    return false;
}

bool AVL::Iterator::operator==(Iterator it){
    if(this->node == NULL && it.node!=NULL){return false;}
    else if(this->node != NULL && it.node == NULL){return false;}
    else if(this->node == NULL && it.node == NULL){return true;}
    if(this->node->getElement() == it.node->getElement()){return true;}
    return false;
}

string AVL::recurdot(string d, AVL::Node* s, string hash){
    char c = '"';
    if(s == root){
        d = d+'\n'+'\t'+s->getElement()+" ";
        d = d+"[label="+c+s->getElement()+c+" ,shape=circle, color=black]";
    }
    else{
        d = d+'\n'+'\t'+hash+" -- "+s->getElement();
        d = d+'\n'+'\t'+s->getElement()+" [label="+c+s->getElement()+c+" ,shape=circle, color=black]";
    }
    if(s->getLeft()!=NULL)
    d = recurdot(d, s->getLeft(), s->getElement());
    if(s->getRight()!=NULL)
    d = recurdot(d, s->getRight(), s->getElement());
    
    
    return d;
}

void AVL::print2DotFile(char* filename){
    ofstream myfile;
     myfile.open(filename);
    
    string final_dot = "graph AVL {";
    
    final_dot = recurdot(final_dot, root, "");
    
    final_dot = final_dot +'\n'+"}";
    
    myfile << final_dot;
    myfile.close();
    
}

std::ostream& operator<<(std::ostream& out, const AVL& tree){
   AVL::Iterator it = tree.begin();
   
   for(int i = 0; i<tree.size; i++){
       out << *it << " ";
       ++it;
    }
    return out;
}

 

