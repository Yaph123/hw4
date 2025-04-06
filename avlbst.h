#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);


    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item) override; // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    
    // Add helper functions here
    
    AVLNode<Key,Value>* rightRotate(AVLNode<Key,Value>* node);
    AVLNode<Key,Value>* leftRotate(AVLNode<Key,Value>* node);
    void insertFix(AVLNode<Key,Value>* node,AVLNode<Key,Value>* parent);
    void removeFix(AVLNode<Key,Value>* node, int diff);
    AVLNode<Key, Value>* findAVL(const Key& key) const;
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{ 

    AVLNode<Key,Value>* exist = findAVL(new_item.first);
   if(exist){
      exist->setValue(new_item.second);
      return;
   }

   if(!this->root_){
     this->root_ = new AVLNode<Key,Value>(new_item.first,new_item.second,nullptr);
     return;
   }

   AVLNode<Key,Value>* node = new AVLNode<Key,Value>(new_item.first,new_item.second,nullptr);
   BinarySearchTree<Key,Value>::insertHelp(node); 
   node->setBalance(0);

   AVLNode<Key,Value>* parent = node->getParent();
   if(parent){
     if(node == parent->getLeft()){
      parent->updateBalance(-1);
     }else{
      parent->updateBalance(1);
     }
   }

    insertFix(node,parent);
 }
    // TODO

template<class Key, class Value>
 AVLNode<Key, Value>* AVLTree<Key, Value>::findAVL(const Key& key) const
{


  if(this->root_ == nullptr){
    return nullptr;
  }

   AVLNode<Key, Value>* current = static_cast<AVLNode<Key,Value>*>(this->root_);

  while(current){
    if(key == current->getKey()){
      return current;
    }
    else if(key < current->getKey()){
      current = current->getLeft();
    }
    else{
      current = current->getRight();
    }
  }
  return nullptr;

}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* node,AVLNode<Key,Value>* parent )
{
  if(!parent ||!parent->getParent())return;

  AVLNode<Key,Value>* grandParent = parent->getParent();

  if(grandParent->getLeft() == parent){
    grandParent->updateBalance(-1);
  
  if(grandParent->getBalance() == 0) return;

  if(grandParent->getBalance() == -1){
    insertFix(parent,grandParent);
    return;
  }
  else if(grandParent->getBalance() == -2){
    if(parent->getLeft() == node){        // LL CASE
    grandParent = rightRotate(grandParent);
    if(!grandParent->getParent())this->root_ = grandParent;
    parent->setBalance(0);
    grandParent->setBalance(0);
    }else { // LR CASE
    parent = leftRotate(parent);
    grandParent->setLeft(parent);
    grandParent = rightRotate(grandParent);
    if(!grandParent->getParent())this->root_ = grandParent;

    if(node->getBalance() == 0){
      parent->setBalance(0);
      grandParent->setBalance(0);
    }
    else if( node->getBalance() == -1){
      parent->setBalance(0);
      grandParent->setBalance(1);
    }
    else if(node->getBalance() == 1){
      parent->setBalance(-1);
      grandParent->setBalance(0);
    }
    node->setBalance(0);
  }
}
  else if(grandParent->getBalance() == -1){
    insertFix(parent,grandParent);
  }
}
//////////////// RIGHT CHILD CASE ////////////////////////

  else{
    grandParent->updateBalance(1);

    if(grandParent->getBalance() == 0) return;

    if( grandParent->getBalance() == 1){
    insertFix(parent,grandParent);
    return;
  }
  else if(grandParent->getBalance() == 2){
    if(parent->getRight() == node){        // RR CASE
    grandParent = leftRotate(grandParent);
    if(!grandParent->getParent())this->root_ = grandParent;
    parent->setBalance(0);
    grandParent->setBalance(0);
    } else { // RL CASE
    parent = rightRotate(parent);
    grandParent->setRight(parent);
    grandParent = leftRotate(grandParent);
    if(!grandParent->getParent())this->root_ = grandParent;

    if(node->getBalance() == 0){
      parent->setBalance(0);
      grandParent->setBalance(0);
    }
    else if( node->getBalance() == 1){
      parent->setBalance(0);
      grandParent->setBalance(-1);
    }
    else if(node->getBalance() == -1){
      parent->setBalance(1);
      grandParent->setBalance(0);
    }
    else{
      parent->setBalance(1);
      grandParent->setBalance(0);
    }
    node->setBalance(0);
      }
    }
    else if(grandParent->getBalance() == 1){
      insertFix(parent,grandParent);
    }
  }
}
  
template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key,Value>::leftRotate(AVLNode<Key,Value>* node)
{
   if(node == nullptr || node->getRight() == nullptr){
    return nullptr;
   }
  
   AVLNode<Key,Value>* tmp = node->getRight();
   AVLNode<Key,Value>* originalLeft = tmp->getLeft();
   AVLNode<Key,Value>* parent = node->getParent();

   node->setRight(originalLeft);
   if(originalLeft != nullptr) originalLeft->setParent(node);
   tmp->setLeft(node);
   node->setParent(tmp);

   if(parent == nullptr){
    this->root_ = tmp;
    tmp->setParent(nullptr);
   }else{
   tmp->setParent(parent);
   if(parent->getLeft()== node){
    parent->setLeft(tmp);
   } else{
    parent->setRight(tmp);
   }
}


node->setBalance(node->getBalance() - 1 - std::min(tmp->getBalance(),static_cast<int8_t>(0)));
tmp->setBalance(tmp->getBalance() - 1 + std::max(node->getBalance(),static_cast<int8_t>(0)));


  return tmp; 
}


template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value>* node)
{

  if(node == nullptr || node->getLeft() == nullptr){
    return nullptr;
   }
  
   AVLNode<Key,Value>* tmp = node->getLeft();
   AVLNode<Key,Value>* originalRight = tmp->getRight();
   AVLNode<Key,Value>* parent = node->getParent();

   node->setLeft(originalRight);
   if(originalRight != nullptr) originalRight->setParent(node);
   tmp->setRight(node);
   node->setParent(tmp);

   if(parent == nullptr){
    this->root_ = tmp;
    tmp->setParent(nullptr);
   }else{
   tmp->setParent(parent);
   if(parent->getLeft()== node){
    parent->setLeft(tmp);
   } else{
    parent->setRight(tmp);
   }
}

node->setBalance(node->getBalance() + 1 - std::min(tmp->getBalance(),static_cast<int8_t>(0)));
tmp->setBalance(tmp->getBalance() + 1 + std::max(node->getBalance(),static_cast<int8_t>(0)));

  return tmp; 
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{

 AVLNode<Key,Value>* node = findAVL(key);
 if(!node){
  return;
 }

 AVLNode<Key,Value>* parent = node->getParent();
 int diff = 0;

 if(node->getLeft() && node->getRight()){
  AVLNode<Key,Value>* pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(node));
  this->nodeSwap(node,pred);
  parent = node->getParent();
 }

 if(parent){
  diff = (node == parent->getLeft() ? 1 : -1);
 }

 AVLNode<Key,Value>* child = node->getLeft() ? node->getLeft():node->getRight();

 if(child){
  child->setParent(parent);
 }

 if(!parent){
  this->root_ = child;
 }else if( node == parent->getLeft()){
  parent->setLeft(child);
 } else{
  parent->setRight(child);
 }

 delete node;

 if(parent){
  removeFix(parent,diff);
}else if(child){
  child->setBalance(0);
}
 }


template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key,Value>* node, int diff)
{
  if(!node){
    return;
  }
  node->updateBalance(diff);
  if(node->getBalance() == -2){
    AVLNode<Key,Value>* child = node->getLeft();
    if(child->getBalance() <= 0){   //LL Case
      node = rightRotate(node);
      node->setBalance(0);
      node->getRight()->setBalance(0);
    } else{                     //LR
      //AVLNode<Key,Value>* g = node->getRight();
      node->setLeft(leftRotate(child));
      node = rightRotate(node);

      if(node->getBalance() == 0){
        node->getLeft()->setBalance(0);
        node->getRight()->setBalance(-1);
      }else if(node->getBalance()==-1){
        node->getLeft()->setBalance(0);
        node->getRight()->setBalance(1);
      }else{
        node->getLeft()->setBalance(-1);
        node->getRight()->setBalance(0);
      }
      node->setBalance(0);
    }
    }else if(node->getBalance() ==2){
      AVLNode<Key,Value>* child = node->getRight();
      if(child->getBalance() >= 0){
        node = leftRotate(node);
        node->setBalance(0);
        node->setBalance(0);
      } else {
        child = rightRotate(child);
        node->setRight(child);
        node = leftRotate(node);

      if(node->getBalance() == 0){
        node->getLeft()->setBalance(-1);
        node->getRight()->setBalance(0);
      } else if(node->getBalance() == 1){
        node->getLeft()->setBalance(-1);
        node->getRight()->setBalance(0);
      } else{
        node->getLeft()->setBalance(0);
        node->getRight()->setBalance(1);
      }
      node->setBalance(0);
    }
  } 
  
  if(node->getBalance() != 0||!(node->getParent())){
    return;
  }
  AVLNode<Key,Value>* parent = node->getParent();
    if(parent){
      int ndiff = (node == parent->getLeft() ? +1:-1);
      removeFix(parent,ndiff);
    }
  }

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
