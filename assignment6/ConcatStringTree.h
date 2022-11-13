#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

class ConcatStringTree {
protected:
  class ConcatStringTreeNode {
  public:
    ConcatStringTreeNode()
        : data(nullptr), left(nullptr), right(nullptr), leftLength(0),
          length(0) {}
    ConcatStringTreeNode(string data, const int &leftLength = 0,
                         const int &length = 0,
                         ConcatStringTreeNode *left = nullptr,
                         ConcatStringTreeNode *right = nullptr)
        : data(data), left(left), right(right), leftLength(leftLength),
          length(length) {}
    ~ConcatStringTreeNode() {}

  public:
    string data;
    ConcatStringTreeNode *left, *right;
    int leftLength, length;
  };

  ConcatStringTreeNode *root;

public:
  ConcatStringTree(const char *s);
  int length() const;
  char get(int index);
  bool checkDataIsNull(ConcatStringTreeNode *node) const;
  int indexOfHelper(ConcatStringTreeNode *root, char c);
  int indexOf(char c);
  string toStringPreOrderHelper(ConcatStringTreeNode *root) const;
  string toStringPreOrder() const;
  string toStringHelper(ConcatStringTreeNode *root) const;
  string toString() const;
  ConcatStringTree concat(const ConcatStringTree &otherS) const;
  ConcatStringTree subString(int from, int to) const;
  ConcatStringTree reverse() const;

  int getParTreeSize(const string &query) const;
  string getParTreeStringPreOrder(const string &query) const;
};

class ReducedConcatStringTree; // forward declaration

class HashConfig {
private:
  int p;
  double c1, c2;
  double lambda;
  double alpha;
  int initSize;

  friend class ReducedConcatStringTree;
};

class LitStringHash {
public:
  LitStringHash(const HashConfig &hashConfig);
  int getLastInsertedIndex() const;
  string toString() const;
};

class ReducedConcatStringTree /* */ {
public:
  ReducedConcatStringTree(const char *s, LitStringHash *litStringHash);
  LitStringHash *litStringHash;
};

#endif // __CONCAT_STRING_TREE_H__
