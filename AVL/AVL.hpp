/*
 * Biti Polyxeni, 2582
 */

#ifndef __AVL_HPP_
#define __AVL_HPP_

#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

class AVL {
private:
  class Node {
    Node *parent, *left, *right;
    int height;
    string element;

  public:
    Node(const string& e, Node *parent, Node *left, Node *right);
    Node(const Node* o);

    Node*  getParent() const;
    Node*  getLeft() const;
    Node*  getRight() const;
    string getElement() const;
    int    getHeight() const;
    int heightwithNode(const Node *) const;
    void setLeft(Node *);
    void setRight(Node *);
    void setParent(Node *);
    void setElement(string e);
    bool isLeft() const;
    bool isRight() const;
    int  rightChildHeight() const;
    int  leftChildHeight() const;
    int  updateHeight();
    bool isBalanced();
  };
private:
  
  int   size;
  Node* root;
  
public:
    
    class Iterator {
        AVL::Node* node;
        stack<AVL::Node*> nodeStack;
      
    public:
      Iterator(AVL::Node* n, AVL::Node* p);
      Iterator(const Iterator &it);
      Iterator& operator++();
      Iterator operator++(int a);
      string operator*(); 
      bool operator!=(Iterator it);
      bool operator==(Iterator it);
    };
    
  Iterator begin() const;  
  Iterator end() const;
  
  static const int MAX_HEIGHT_DIFF = 1;
  AVL();
  AVL(const AVL& );
  ~AVL();
  bool contains(string e);
  bool add(string e);
  bool rmv(string e);
  void print2DotFile(char* filename);
  void pre_order(std::ostream& out);


  friend std::ostream& operator<<(std::ostream& out, const AVL& tree);  
  AVL& operator  =(const AVL& avl);
  AVL  operator  +(const AVL& avl);
  AVL& operator +=(const AVL& avl);
  AVL& operator +=(const string& e);
  AVL& operator -=(const string& e);
  AVL  operator  +(const string& e);
  AVL  operator  -(const string& e);

private: 
  Node* insertion(Node* n, string s, Node* p);
  Node* rotateLeft(Node* n);
  Node* rotateRight(Node* n);
  Node* deletion(Node* r, string k);
  Node* minRightSubtree(Node* r);
  void recurPreorder(const Node* r, std::ostream& out);
  bool search(Node *r, string word);
  Node* selfBalance(Node* n);
  void EmptyTree(Node* r);
  string recurdot(string dot, Node* start, string hash);
};

#endif
