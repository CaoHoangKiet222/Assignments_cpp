#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

long long powerFunc(int base, int power);

class ParentsTree {
protected:
  class ParentsTreeNode {
  public:
    ParentsTreeNode() : id(-1), left(nullptr), right(nullptr){};
    ParentsTreeNode(const long long &id, ParentsTreeNode *left = nullptr,
                    ParentsTreeNode *right = nullptr)
        : id(id), left(left), right(right){};

  public:
    long long id;
    ParentsTreeNode *left, *right;
  };

  int getHeightRec(ParentsTreeNode *node) const;

public:
  ParentsTreeNode *root;
  int nE;

  ParentsTree() : root(nullptr){};
  int getHeight() const;
  ParentsTreeNode *leftRotate(ParentsTreeNode *x);
  ParentsTreeNode *rightRotate(ParentsTreeNode *x);
  int getBalance(ParentsTreeNode *node) const;
  ParentsTreeNode *insertHelper(ParentsTreeNode *node, const long long &value);
  void insert(const long long &value);
  ParentsTreeNode *maxValueNode(ParentsTreeNode *node) const;
  ParentsTreeNode *removeHelper(ParentsTreeNode *root, const long long &value);
  void remove(const long long &value);
  int size() const;
  string toStringPreOrderHelper(ParentsTreeNode *root) const;
  string toStringPreOrder() const;
};

class ConcatStringTree {
protected:
  class ConcatStringTreeNode {
  public:
    ConcatStringTreeNode()
        : data(nullptr), left(nullptr), right(nullptr), leftLength(0),
          length(0), id(-1), parentTree(nullptr) {}
    ConcatStringTreeNode(const string &data, const int &leftLength = 0,
                         const int &length = 0,
                         ConcatStringTreeNode *left = nullptr,
                         ConcatStringTreeNode *right = nullptr)
        : data(data), left(left), right(right), leftLength(leftLength),
          length(length) {
      if (currentID > powerFunc(10, 7)) {
        throw overflow_error("Id is overflow!");
      }
      this->id = ++currentID;
    }

  public:
    string data;
    ConcatStringTreeNode *left, *right;
    int leftLength, length;
    long long id;
    ParentsTree *parentTree;
  };

  ConcatStringTreeNode *root;
  static long long currentID;

public:
  ConcatStringTree(const int &length, const int &leftLength,
                   ConcatStringTreeNode *left, ConcatStringTreeNode *right);
  ConcatStringTree(const char *s);
  ConcatStringTree(const string &s);
  ~ConcatStringTree();
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
  ConcatStringTree subStringHelper(ConcatStringTreeNode *root, int &from,
                                   int &to, int &totalLengthStr) const;
  ConcatStringTree subString(int from, int to) const;
  string reverseStr(const string &s) const;
  ConcatStringTree reverseHelper(ConcatStringTreeNode *root) const;
  ConcatStringTree reverse() const;

  int getParTreeSize(const string &query) const;
  string getParTreeStringPreOrder(const string &query) const;
};

// ==========================================================
class ReducedConcatStringTree; // forward declaration

class HashConfig {
private:
  int p;
  double c1, c2;
  double lambda;
  double alpha;
  int initSize;

  friend class ReducedConcatStringTree;
  friend class LitStringHash;
};

template <class T> class Vector {
public:
  Vector();
  Vector(const int &newsize);
  ~Vector();
  int size() const;
  T &operator[](int index); // get value of object[index]
  const T &getAt(int index) const;
  Vector &operator=(Vector &new_vec); // assign Vector for Vector
  void push_back(T newElement);
  void insert(T newElement, int idx);
  void ensureCap(const int &newsize);

private:
  int Size;
  int capacity;
  T *arr;
  void ensureCap();
};

class LitString {
public:
  LitString() : data(nullptr), numLinks(0) {}
  LitString(const string &data, const int &numLinks)
      : data(data), numLinks(numLinks) {}
  string toString() const;

public:
  string data;
  int numLinks;
};

class LitStringHash {
public:
  LitStringHash(const HashConfig &hashConfig);
  long long hashFunction(const string &s, const int &p, const int &m);
  long long detectionQuadratic(const string &s);
  int getLastInsertedIndex() const;
  long long remove(const string &s);
  string toString() const;

private:
  HashConfig hashConfig;
  int lastInsertedIndex;
  Vector<LitString> vec;
};

class ReducedConcatStringTree : public ConcatStringTree {
public:
  ReducedConcatStringTree(const char *s, LitStringHash *litStringHash);
  LitStringHash *litStringHash;
};

#endif // __CONCAT_STRING_TREE_H__
