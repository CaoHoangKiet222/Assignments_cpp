#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

// ============= CharArrayList ===================
class CharArrayList {
public:
  CharArrayList();
  CharArrayList(const char *s);
  ~CharArrayList();
  int size() const;
  char &operator[](int index);
  int indexOf(char c) const;
  char *&getStr();

private:
  int length;
  char *str;
};

// ============= CharALNode ===================
class CharALNode {
public:
  CharALNode();
  CharALNode(const char *s);
  CharArrayList &getCharArrayList();
  CharALNode *&getNext();
  ~CharALNode();

private:
  CharArrayList *arrayList;
  CharALNode *next;
};

// ============= ReferencesListNode ===================
class ReferencesListNode {
public:
  ReferencesListNode();
  ReferencesListNode(CharALNode *&node, const int &numOfRef,
                     const int &numRefByDelNode);
  ReferencesListNode *&getNext();
  CharALNode *&getCharALNode();
  int getNumOfRef() const;
  void setNumOfRef(const int &number);
  // int getNumRefByDelNode() const;
  // void setNumRefByDelNode(const int &numRefByDelNode);

private:
  CharALNode *charNode;
  int numOfRef;
  // int numRefByDelNode;
  ReferencesListNode *next;
};

// ============= DeleteStringListNode ===================
class DeleteStringListNode {
public:
  DeleteStringListNode();
  DeleteStringListNode(ReferencesListNode *refHead,
                       ReferencesListNode *refTail);
  DeleteStringListNode *&getNext();
  ReferencesListNode *&getRefNodeHead();
  ReferencesListNode *&getRefNodeTail();

private:
  ReferencesListNode *refNodeHead, *refNodeTail;
  DeleteStringListNode *next;
};

// ============= ConcatStringList ===================
class ConcatStringList {
public:
  class ReferencesList;   // forward declaration
  class DeleteStringList; // forward declaration

public:
  static ReferencesList refList;
  static DeleteStringList delStrList;

  // TODO: may provide some attributes
private:
  CharALNode *head, *tail;
  int totalLength;

public:
  ConcatStringList();
  ConcatStringList(const char *s);
  int length() const;
  char get(int index) const;
  int indexOf(char c) const;
  std::string toString() const;
  ConcatStringList concat(const ConcatStringList &otherS) const;
  ConcatStringList subString(int from, int to) const;
  ConcatStringList reverse() const;
  ~ConcatStringList();

public:
  class ReferencesList {
    // TODO: may provide some attributes
  private:
    ReferencesListNode *head, *tail;
    int numOfNodes;

  public:
    int size() const;
    int refCountAt(int index) const;
    std::string refCountsString() const;
    void setNumOfNodes(int number);
    ReferencesListNode *&getHead();
    ReferencesListNode *&getTail();
    void addReferenceNode(CharALNode *&node, const int &numOfRef);
    void incReferenceNode(CharALNode *&node, const int &numOfRef);
    void decReferenceNode(CharALNode *&node, const int &numOfRef);
    ReferencesListNode *findRefNode(CharALNode *&node);
    bool checkAllRefsAreZero() const;
  };

  class DeleteStringList {
    // TODO: may provide some attributes
    DeleteStringListNode *head, *tail;
    int totalLength;

  public:
    DeleteStringList();
    DeleteStringListNode *&getHead();
    DeleteStringListNode *&getTail();
    void addDeleteStringListNode(DeleteStringListNode *&delNode);
    int size() const;
    void setSize(int newSize);
    std::string totalRefCountsString() const;
  };
};

char *convertStringToCharPointer(std::string s);
char *reverseCharPointer(CharArrayList &arrayList);

#endif // __CONCAT_STRING_LIST_H__
