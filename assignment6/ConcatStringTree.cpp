#include "ConcatStringTree.h"
#include <string>

ConcatStringTree::ConcatStringTree(const char *s) {
  if (s) {
    int length = 0;
    while (*(s + length) != '\0') {
      length++;
    }

    string str = "";
    for (int i = 0; i < length; i++) {
      str += *(s + i);
    }
    this->root = new ConcatStringTreeNode(str, 0, length);
  } else {
    this->root = nullptr;
  }
}

int ConcatStringTree::length() const { return this->root->length; }

char ConcatStringTree::get(int index) {
  if (index >= this->root->length || index < 0) {
    throw out_of_range("Index of string is invalid!");
  }

  ConcatStringTreeNode *i = this->root;

  while (i->left || i->right) {
    if (index < i->leftLength) {
      i = i->left;
    } else {
      index -= i->leftLength;
      i = i->right;
    }
  }
  return i->data[index];
}

bool ConcatStringTree::checkDataIsNull(ConcatStringTreeNode *node) const {
  return node->right || node->left;
}

int ConcatStringTree::indexOfHelper(ConcatStringTreeNode *root, char c) {
  if (!root) {
    return -1;
  }
  int indexLeft = indexOfHelper(root->left, c);
  int indexRight = indexOfHelper(root->right, c);
  int indexRoot = -1;
  if (checkDataIsNull(root)) {
    indexRoot = root->data.find(c);
  }
  return indexLeft != -1 ? indexLeft : indexRoot != -1 ? indexRoot : indexRight;
}

int ConcatStringTree::indexOf(char c) {
  return this->indexOfHelper(this->root, c);
}

string
ConcatStringTree::toStringPreOrderHelper(ConcatStringTreeNode *root) const {
  if (!root) {
    return "";
  }
  // "ConcatStringTree[(LL=5,L=8,<NULL>);(LL=0,L=5,"Hello");(LL=0,L=3,",_t")]"
  string toStringRoot = "(LL=" + to_string(root->leftLength) +
                        ",L=" + to_string(root->length) + ",";
  if (checkDataIsNull(root)) {
    toStringRoot += "<NULL>);";
  } else {
    toStringRoot = toStringRoot + "\"" + root->data + "\");";
  }
  return toStringRoot + toStringPreOrderHelper(root->left) +
         toStringPreOrderHelper(root->right);
}

string ConcatStringTree::toStringPreOrder() const {
  string preOrderString = toStringPreOrderHelper(root);
  if (!preOrderString.empty()) {
    preOrderString.erase(preOrderString.begin() + preOrderString.length() - 1);
  }
  return "ConcatStringTree[" + preOrderString + "]";
}

string ConcatStringTree::toStringHelper(ConcatStringTreeNode *root) const {
  if (!root) {
    return "";
  }

  return toStringHelper(root->left) + toStringHelper(root->right) +
         (checkDataIsNull(root) ? "" : root->data);
}

string ConcatStringTree::toString() const {
  return "ConcatStringTree[\"" + toStringHelper(root) + "\"]";
}

ConcatStringTree
ConcatStringTree::concat(const ConcatStringTree &otherS) const {
  // return new ConcatStringTree()
}
