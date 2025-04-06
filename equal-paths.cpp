#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <cmath>
#include<algorithm>

#endif

#include "equal-paths.h"

using namespace std;


// You may add any prototypes of helper functions here


bool measurePath(Node* root, int depth, int& leafDepth)
{

    if(root == nullptr){
       return true;
    }
    if(root->left == nullptr && root->right == nullptr){
      if(leafDepth == -1){
        leafDepth = depth;
      }
      return leafDepth == depth;
    }

    return measurePath(root->left, depth + 1, leafDepth) && measurePath(root->right, depth + 1, leafDepth);
 
}

bool equalPaths(Node * root)
{
  int leafDepth = -1;
  bool check = measurePath(root, 0, leafDepth);
  return check;
    // Add your code below
}

