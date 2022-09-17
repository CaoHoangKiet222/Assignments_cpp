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
  char &operator[](int index) const;
  int indexOf(char c) const;

private:
  int length;
  char *str;
};

// ============= CharALNode ===================
class CharALNode {
public:
  CharALNode();
  CharALNode(const char *s);
  CharArrayList& getCharArrayList();
  CharALNode*& getNext();
  // ~CharALNode();

private:
  CharArrayList arrayList;
  CharALNode *next;
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

  public:
    int size() const;
    int refCountAt(int index) const;
    std::string refCountsString() const;
  };

  class DeleteStringList {
    // TODO: may provide some attributes

  public:
    int size() const;
    std::string totalRefCountsString() const;
  };
};

char* convertStringToCharPointer(std::string s);
char* reverseCharPointer(CharArrayList arrayList);

#endif // __CONCAT_STRING_LIST_H__
