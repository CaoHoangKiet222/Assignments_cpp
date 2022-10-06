#include "ConcatStringList.h"
#include <iostream>
#include <string>

ConcatStringList::ReferencesList ConcatStringList::refList =
    ConcatStringList::ReferencesList();
ConcatStringList::DeleteStringList ConcatStringList::delStrList =
    ConcatStringList::DeleteStringList();

// ============= CharArrayList ===================
CharArrayList::CharArrayList() {
  this->str = nullptr;
  this->length = 0;
}

CharArrayList::~CharArrayList() {
  if (this->str) {
    delete[] this->str;
    this->str = nullptr;
    this->length = 0;
  }
}

CharArrayList::CharArrayList(const char *s) {
  if (s) {
    this->length = 0;
    while (*(s + length) != '\0') {
      this->length++;
    }
    if (this->length) {
      this->str = new char[this->length + 1];
      for (int i = 0; i < this->length; i++) {
        *(this->str + i) = *(s + i);
      }
      *(this->str + this->length) = '\0';
    } else {
      this->str = nullptr;
    }
  } else {
    this->str = nullptr;
    this->length = 0;
  }
}

int CharArrayList::size() const { return this->length; }

char &CharArrayList::operator[](int index) {
  if (index >= this->length || index < 0) {
    throw out_of_range("Index of string is invalid");
  }
  return *(this->str + index);
}

int CharArrayList::indexOf(char c) const {
  for (int i = 0; i < this->length; i++) {
    if (*(this->str + i) == c) {
      return i;
    }
  }
  return -1;
}

char *&CharArrayList::getStr() { return this->str; }

// ============= CharALNode ===================
CharALNode::CharALNode() {
  this->arrayList = nullptr;
  this->next = nullptr;
}

CharALNode::CharALNode(const char *s) {
  s ? this->arrayList = new CharArrayList(s) : this->arrayList = nullptr;
  this->next = nullptr;
}

CharArrayList &CharALNode::getCharArrayList() { return *this->arrayList; }

CharALNode *&CharALNode::getNext() { return this->next; }

CharALNode::~CharALNode() {
  if (this->arrayList) {
    delete this->arrayList;
    this->arrayList = nullptr;
  }
}

// ============= ConcatStringList ===================
ConcatStringList::ConcatStringList() {
  this->head = this->tail = nullptr;
  this->totalLength = 0;
}

ConcatStringList::ConcatStringList(const char *s) {
  if (s) {
    this->totalLength = 0;
    CharALNode *node = new CharALNode(s);
    this->head = this->tail = node;
    this->totalLength += this->head->getCharArrayList().size();

    // initialize refList
    refList.addReferenceNode(node, 2);
  } else {
    this->head = this->tail = nullptr;
    this->totalLength = 0;
  }
}

int ConcatStringList::length() const { return this->totalLength; }

char ConcatStringList::get(int index) const {
  if (index >= this->totalLength || index < 0) {
    throw out_of_range("Index of string is invalid!");
  }
  CharALNode *i = this->head;
  while (i && index > i->getCharArrayList().size() - 1) {
    index -= i->getCharArrayList().size();
    i = i->getNext();
  }
  return i->getCharArrayList()[index];
}

int ConcatStringList::indexOf(char c) const {
  int count = 0;
  CharALNode *i = this->head;
  while (i) {
    int found_at_index = i->getCharArrayList().indexOf(c);
    if (found_at_index != -1) {
      // found character
      count += found_at_index;
      break;
    } else {
      // not found character
      count += i->getCharArrayList().size();
    }
    i = i->getNext();
  }
  if (count == this->totalLength) {
    return -1;
  }
  return count;
}

std::string ConcatStringList::toString() const {
  std::string s = "";
  CharALNode *i = this->head;
  while (i) {
    for (int j = 0; j < i->getCharArrayList().size(); j++) {
      s += i->getCharArrayList()[j];
    }
    if (i == this->tail) {
      break;
    }
    i = i->getNext();
  }
  return s;
}

ConcatStringList
ConcatStringList::concat(const ConcatStringList &otherS) const {
  // https://stackoverflow.com/questions/48861494/c-why-can-a-passed-objects-private-variables-be-accessed-directly-in-the-co
  ConcatStringList concatString = ConcatStringList();
  concatString.totalLength = otherS.totalLength + this->totalLength;
  this->tail->getNext() = otherS.head;
  concatString.head = this->head;
  concatString.tail = otherS.tail;

  // initialize refList
  refList.incReferenceNode(concatString.head, 1);
  refList.incReferenceNode(concatString.tail, 1);

  return concatString;
}

ConcatStringList ConcatStringList::subString(int from, int to) const {
  if (from >= this->totalLength || to > this->totalLength || from < 0 ||
      to < 0) {
    throw out_of_range("Index of string is invalid");
  } else if (from >= to) {
    throw logic_error("Invalid range");
  }
  ConcatStringList concatString = ConcatStringList();

  CharALNode *i = this->head;
  bool isFirst = true;
  const char *charList;
  while (i && from >= 0 && to >= 0 && from < to) {
    std::string s = "";
    bool found = false;
    while (from < i->getCharArrayList().size() && from < to) {
      s += i->getCharArrayList()[from];
      found = true;
      from++;
    }
    if (found) {
      char *charList = convertStringToCharPointer(s);
      if (isFirst) {
        concatString.head = concatString.tail = new CharALNode(charList);
        isFirst = false;
      } else {
        concatString.tail->getNext() = new CharALNode(charList);
        concatString.tail = concatString.tail->getNext();
      }
      concatString.totalLength += concatString.tail->getCharArrayList().size();
      delete[] charList;
    }
    from -= i->getCharArrayList().size();
    to -= i->getCharArrayList().size();
    i = i->getNext();
  }

  // initialize refList
  if (concatString.head == concatString.tail) {
    refList.addReferenceNode(concatString.tail, 2);
  } else {
    refList.addReferenceNode(concatString.head, 1);
    refList.addReferenceNode(concatString.tail, 1);
  }

  return concatString;
}

char *convertStringToCharPointer(std::string s) {
  char *result = new char[s.length() + 1];
  for (int i = 0; i < s.length(); i++) {
    *(result + i) = s[i];
  }
  *(result + s.length()) = '\0';
  return result;
}

ConcatStringList ConcatStringList::reverse() const {
  ConcatStringList concatString = ConcatStringList();
  CharALNode *i = this->head;
  bool isFirst = true;
  while (i) {
    char *reverseString = reverseCharPointer(i->getCharArrayList());
    if (isFirst) {
      concatString.head = concatString.tail = new CharALNode(reverseString);
      isFirst = false;
    } else {
      CharALNode *temp = new CharALNode(reverseString);
      temp->getNext() = concatString.head;
      concatString.head = temp;
    }
    concatString.totalLength += concatString.head->getCharArrayList().size();
    delete[] reverseString;

    if (i == this->tail) {
      break;
    }
    i = i->getNext();
  }

  // initialize refList
  if (concatString.head == concatString.tail) {
    refList.addReferenceNode(concatString.tail, 2);
  } else {
    refList.addReferenceNode(concatString.head, 1);
    refList.addReferenceNode(concatString.tail, 1);
  }
  return concatString;
}

char *reverseCharPointer(CharArrayList &arrayList) {
  char *result = new char[arrayList.size() + 1];
  for (int i = 0; i < arrayList.size(); i++) {
    *(result + i) = arrayList[arrayList.size() - 1 - i];
  }
  *(result + arrayList.size()) = '\0';
  return result;
}

// need to implement
ConcatStringList::~ConcatStringList() {
  if (this->head == this->tail) {
    refList.decReferenceNode(this->head, 2);
  } else {
    refList.decReferenceNode(this->head, 1);
    refList.decReferenceNode(this->tail, 1);
  }

  ReferencesListNode *refHead = refList.findRefNode(this->head),
                     *refTail = refList.findRefNode(this->tail);
  DeleteStringListNode *delNode = new DeleteStringListNode(refHead, refTail);

  // if (refHead == refTail) {
  //   refHead->setNumRefByDelNode(refHead->getNumRefByDelNode() + 2);
  // } else {
  //   refHead->setNumRefByDelNode(refHead->getNumRefByDelNode() + 1);
  //   refTail->setNumRefByDelNode(refTail->getNumRefByDelNode() + 1);
  // }
  delStrList.addDeleteStringListNode(delNode);

  DeleteStringListNode *i = delStrList.getHead(), *preI = nullptr;
  while (i) {
    if (i->getRefNodeTail()->getNumOfRef() +
            i->getRefNodeHead()->getNumOfRef() ==
        0) {

      // delete charNode in concatString;
      // CharALNode *j = i->getRefNodeHead()->getCharALNode(),
      //            *preJ = i->getRefNodeHead()->getCharALNode();
      // bool foundTailCharALNode = false;
      // while (j && !foundTailCharALNode) {
      //   j = j->getNext();
      //   if (preJ) {
      //     if (preJ == i->getRefNodeTail()->getCharALNode()) {
      //       foundTailCharALNode = true;
      //     }
      //     delete preJ;
      //     preJ = nullptr;
      //   }
      //   preJ = j;
      // }
      // i->getRefNodeHead()->getCharALNode() =
      //     i->getRefNodeTail()->getCharALNode() = nullptr;

      // delete delNode in delStrList;
      DeleteStringListNode *delNode = i;
      if (preI) {
        preI->getNext() = i->getNext();
      } else {
        // change head
        delStrList.getHead() = i->getNext();
      }
      i = i->getNext();

      if (delNode) {
        // delNode->getRefNodeHead()->setNumRefByDelNode(
        //     delNode->getRefNodeHead()->getNumRefByDelNode() - 1);
        // delNode->getRefNodeTail()->setNumRefByDelNode(
        //     delNode->getRefNodeTail()->getNumRefByDelNode() - 1);
        delete delNode;
        delStrList.setSize(delStrList.size() - 1);
      }

      // change tail
      if (preI && preI->getNext() == nullptr) {
        delStrList.getTail() = preI;
      }
    } else {
      preI = i;
      i = i->getNext();
    }
  }

  // delete all nodes in refList;
  if (refList.checkAllRefsAreZero()) {
    ReferencesListNode *i = refList.getHead(), *preI = i;
    while (i) {
      i = i->getNext();
      preI->getNext() = nullptr;
      if (preI) {
        delete preI;
      }
      preI = i;
    }
    refList.getHead() = refList.getTail() = nullptr;
    refList.setNumOfNodes(0);
  }
}

// ============= ReferencesListNode ===================
ReferencesListNode::ReferencesListNode() {
  this->charNode = nullptr;
  this->numOfRef = 0;
  // this->numRefByDelNode = 0;
  this->next = nullptr;
}

ReferencesListNode::ReferencesListNode(CharALNode *&node, const int &numOfRef,
                                       const int &numRefByDelNode) {
  this->charNode = node;
  this->numOfRef = numOfRef;
  // this->numRefByDelNode = numRefByDelNode;
  this->next = nullptr;
}

ReferencesListNode *&ReferencesListNode::getNext() { return this->next; }

CharALNode *&ReferencesListNode::getCharALNode() { return this->charNode; }

int ReferencesListNode::getNumOfRef() const { return this->numOfRef; }

// int ReferencesListNode::getNumRefByDelNode() const {
//   return this->numRefByDelNode;
// }

// void ReferencesListNode::setNumRefByDelNode(const int &numRefByDelNode) {
//   this->numRefByDelNode = numRefByDelNode;
// }

void ReferencesListNode::setNumOfRef(const int &number) {
  this->numOfRef = number;
}

// ============= DeleteStringListNode ===================
DeleteStringListNode::DeleteStringListNode() {
  this->refNodeHead = this->refNodeTail = nullptr;
  this->next = nullptr;
}

DeleteStringListNode::DeleteStringListNode(ReferencesListNode *refHead,
                                           ReferencesListNode *refTail) {
  this->refNodeHead = refHead;
  this->refNodeTail = refTail;
  this->next = nullptr;
}

DeleteStringListNode *&DeleteStringListNode::getNext() { return this->next; }

ReferencesListNode *&DeleteStringListNode::getRefNodeHead() {
  return this->refNodeHead;
}

ReferencesListNode *&DeleteStringListNode::getRefNodeTail() {
  return this->refNodeTail;
}

// ============= ReferencesList ===================
int ConcatStringList::ReferencesList::size() const { return this->numOfNodes; }

int ConcatStringList::ReferencesList::refCountAt(int index) const {
  if (index > this->numOfNodes || index < 0) {
    throw out_of_range("Index of references list is invalid!");
  }
  ReferencesListNode *i = this->head;
  for (int j = 0; j < index; j++) {
    i = i->getNext();
  }
  return i->getNumOfRef();
}

std::string ConcatStringList::ReferencesList::refCountsString() const {
  ReferencesListNode *i = this->head;
  std::string result = "";
  while (i) {
    int numOfRef = i->getNumOfRef();
    result += std::to_string(numOfRef);
    if (i->getNext()) {
      result += ",";
    }
    i = i->getNext();
  }
  return "RefCounts[" + result + "]";
}

void ConcatStringList::ReferencesList::setNumOfNodes(int number) {
  this->numOfNodes = number;
}

ReferencesListNode *&ConcatStringList::ReferencesList::getHead() {
  return this->head;
}

ReferencesListNode *&ConcatStringList::ReferencesList::getTail() {
  return this->tail;
}

void ConcatStringList::ReferencesList::addReferenceNode(CharALNode *&node,
                                                        const int &numOfRef) {
  if (refList.size() == 0) {
    refList.getHead() = refList.getTail() =
        new ReferencesListNode(node, numOfRef, 0);
    refList.setNumOfNodes(refList.size() + 1);
  } else {
    ReferencesListNode *i = ConcatStringList::refList.getHead(),
                       *preI = nullptr;
    while (i && i->getNumOfRef() && i->getNumOfRef() < numOfRef) {
      preI = i;
      i = i->getNext();
    }
    // same address like &(*i) == &(*refList.getHead())
    ReferencesListNode *refNode = new ReferencesListNode(node, numOfRef, 0);
    if (preI == nullptr) {
      // insert before refList.head
      refNode->getNext() = i;
      refList.getHead() = refNode;
    } else {
      preI->getNext() = refNode;
      refNode->getNext() = i;
      if (refNode->getNext() == nullptr) {
        refList.getTail() = refNode;
      }
    }
    // increase size ReferencesList
    ConcatStringList::refList.setNumOfNodes(ConcatStringList::refList.size() +
                                            1);
  }
}

void ConcatStringList::ReferencesList::incReferenceNode(CharALNode *&node,
                                                        const int &numOfRef) {
  ReferencesListNode *i = this->head, *preI = nullptr, *j = nullptr,
                     *preJ = nullptr;
  while (i) {
    if (i->getCharALNode() == node) {
      i->setNumOfRef(i->getNumOfRef() + numOfRef);
      break;
    }
    preI = i;
    i = i->getNext();
  }

  // sort reList after increase numOfRef
  if (i) {
    preJ = i;
    j = i->getNext();

    bool found = false;
    while (j && j->getNumOfRef() && j->getNumOfRef() < i->getNumOfRef()) {
      preJ = j;
      j = j->getNext();
      found = true;
    }

    if (found) {
      if (preI) {
        preI->getNext() = i->getNext();
      } else {
        this->head = i->getNext();
      }

      i->getNext() = j;
      preJ->getNext() = i;

      if (j == nullptr) {
        this->tail = i;
      }
    }
  }
}

void ConcatStringList::ReferencesList::decReferenceNode(CharALNode *&node,
                                                        const int &numOfRef) {
  ReferencesListNode *i = this->head, *preI = nullptr, *j = this->head,
                     *preJ = nullptr;
  while (i) {
    if (i->getCharALNode() == node) {
      i->setNumOfRef(i->getNumOfRef() - numOfRef);
      break;
    }
    preI = i;
    i = i->getNext();
  }

  if (i->getNumOfRef()) {
    // sort after decreasing
    while (j && i && j->getNumOfRef() < i->getNumOfRef()) {
      preJ = j;
      j = j->getNext();
    }

    if (preI) {
      preI->getNext() = i->getNext();
      i->getNext() = j;
      if (preJ == nullptr) {
        // insert before refList.head
        this->head = i;
      } else {
        preJ->getNext() = i;
      }

      if (preI->getNext() == nullptr) {
        this->tail = preI;
      }
    }
  } else {
    // move zero reference to bottom
    if ((i->getNext() && i->getNext()->getNumOfRef()) && i != this->tail) {
      if (preI) {
        preI->getNext() = i->getNext();
      } else {
        this->head = i->getNext();
      }
      this->tail->getNext() = i;
      this->tail = i;
      i->getNext() = nullptr;
    }
  }
}

ReferencesListNode *
ConcatStringList::ReferencesList::findRefNode(CharALNode *&node) {
  // error check here
  ReferencesListNode *i = this->head;
  while (i && i->getCharALNode() != node) {
    i = i->getNext();
  }
  return i;
}

bool ConcatStringList::ReferencesList::checkAllRefsAreZero() const {
  ReferencesListNode *i = this->head;
  while (i) {
    if (i->getNumOfRef()) {
      return false;
    }
    i = i->getNext();
  }
  return true;
}

// ============= DeleteStringList ===================
ConcatStringList::DeleteStringList::DeleteStringList() {
  this->head = this->tail = nullptr;
  this->totalLength = 0;
}

DeleteStringListNode *&ConcatStringList::DeleteStringList::getHead() {
  return this->head;
}

DeleteStringListNode *&ConcatStringList::DeleteStringList::getTail() {
  return this->tail;
}

void ConcatStringList::DeleteStringList::addDeleteStringListNode(
    DeleteStringListNode *&delNode) {
  if (this->totalLength == 0) {
    this->head = this->tail = delNode;
  } else {
    this->tail->getNext() = delNode;
    this->tail = this->tail->getNext();
  }
  this->totalLength++;
}

int ConcatStringList::DeleteStringList::size() const {
  return this->totalLength;
}

void ConcatStringList::DeleteStringList::setSize(int newSize) {
  this->totalLength = newSize;
}

std::string ConcatStringList::DeleteStringList::totalRefCountsString() const {
  DeleteStringListNode *i = this->head;
  std::string s = "";
  while (i) {
    if (i->getRefNodeHead() && i->getRefNodeTail()) {
      int result = i->getRefNodeHead()->getNumOfRef() +
                   i->getRefNodeTail()->getNumOfRef();
      if (result != 0) {
        s += std::to_string(result);

        if (i->getNext()) {
          s += ",";
        }
      }
    }
    i = i->getNext();
  }
  return "TotalRefCounts[" + s + "]";
}
