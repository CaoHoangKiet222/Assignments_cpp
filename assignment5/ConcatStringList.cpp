#include "ConcatStringList.h"

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

char &CharArrayList::operator[](int index) const {
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

// ============= CharALNode ===================
CharALNode::CharALNode() {
  this->arrayList = nullptr;
  this->next = nullptr;
}

CharALNode::CharALNode(const char *s) {
  s ? this->arrayList = CharArrayList(s) : this->arrayList = nullptr;
  this->next = nullptr;
}

CharArrayList &CharALNode::getCharArrayList() { return this->arrayList; }

CharALNode *&CharALNode::getNext() { return this->next; }

// CharALNode::~CharALNode() {
//   if (this->arrayList) {
//     delete this->arrayList;
//     this->arrayList = nullptr;
//   }
// }

// ============= ConcatStringList ===================
ConcatStringList::ConcatStringList() {
  this->head = this->tail = nullptr;
  this->totalLength = 0;
}

ConcatStringList::ConcatStringList(const char *s) {
  if (s) {
    CharALNode *node = new CharALNode(s);
    this->head = this->tail = node;
    this->totalLength += this->head->getCharArrayList().size();

    // initialize refList
    addReferenceNode(node, 2);
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
    } else {
      // not found character
      count += i->getCharArrayList().size();
    }
    i = i->getNext();
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
    i = i->getNext();
  }
  return "\"ConcatStringList[\"" + s + "\"]\"";
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
  ReferencesListNode *i = refList.getHead(), *preI = nullptr;
  runSwapReferenceNodes(i, preI, concatString.head);

  i = refList.getHead();
  preI = nullptr;
  runSwapReferenceNodes(i, preI, concatString.tail);

  return concatString;
}

ConcatStringList ConcatStringList::subString(int from, int to) const {
  if (from > this->totalLength || to > this->totalLength || from < 0 ||
      to < 0) {
    throw out_of_range("Index of string is invalid");
  } else if (from > to) {
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
      const char *charList = convertStringToCharPointer(s);
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
    addReferenceNode(concatString.tail, 2);
  } else {
    addReferenceNode(concatString.head, 1);
    addReferenceNode(concatString.tail, 1);
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
    i = i->getNext();
  }

  // initialize refList
  if (concatString.head == concatString.tail) {
    addReferenceNode(concatString.tail, 2);
  } else {
    addReferenceNode(concatString.head, 1);
    addReferenceNode(concatString.tail, 1);
  }
  return concatString;
}

char *reverseCharPointer(CharArrayList arrayList) {
  char *result = new char[arrayList.size() + 1];
  for (int i = 0; i < arrayList.size(); i++) {
    *(result + i) = arrayList[arrayList.size() - 1 - i];
  }
  *(result + arrayList.size()) = '\0';
  return result;
}

// need to implement
ConcatStringList::~ConcatStringList() {}

// ============= ReferencesListNode ===================
ReferencesListNode::ReferencesListNode() {
  this->charNode = nullptr;
  this->numOfRef = 0;
  this->next = nullptr;
}

ReferencesListNode::ReferencesListNode(CharALNode *&node, const int &numOfRef) {
  this->charNode = node;
  this->numOfRef = numOfRef;
  this->next = nullptr;
}

ReferencesListNode *&ReferencesListNode::getNext() { return this->next; }

CharALNode *&ReferencesListNode::getCharALNode() { return this->charNode; }

int ReferencesListNode::getNumOfRef() const { return this->numOfRef; }

void ReferencesListNode::setNumOfRef(int number) { this->numOfRef = number; }

// ============= ReferencesList ===================
int ConcatStringList::ReferencesList::size() const { return this->numOfNodes; }

int ConcatStringList::ReferencesList::refCountAt(int index) const {
  if (index > refList.numOfNodes || index < 0) {
    throw out_of_range("Index of references list is invalid!");
  }
  ReferencesListNode *i = refList.head;
  for (int j = 0; j < index; j++) {
    i = i->getNext();
  }
  return i->getNumOfRef();
}

std::string ConcatStringList::ReferencesList::refCountsString() const {
  ReferencesListNode *i = refList.head;
  std::string result = "";
  while (i) {
    int numOfRef = i->getNumOfRef();
    if (numOfRef) {
      result += std::to_string(numOfRef);
    }
    i = i->getNext();
  }
  return "\"TotalRefCounts[" + result + "]\"";
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

void ConcatStringList::ReferencesList::swapReferenceNodes(
    ReferencesListNode *&node1, ReferencesListNode *&node2,
    ReferencesListNode *&preOfNode1) {
  bool isTopList = false;
  if (preOfNode1) {
    preOfNode1->getNext() = node2;

    // change preOfNode1
    preOfNode1 = preOfNode1->getNext();
  } else {
    isTopList = true;
  }

  node1->getNext() = node2->getNext();
  node2->getNext() = node1;

  // change head
  if (isTopList) {
    preOfNode1 = refList.getHead() = node2;
  }
  // change tail
  if (node1->getNext() == nullptr) {
    refList.getTail() = node1;
  }
}

void runSwapReferenceNodes(ReferencesListNode *&i, ReferencesListNode *&preI,
                           CharALNode *&node) {
  bool found = false;
  while (i && !found) {
    if (i->getCharALNode() == node) {
      // increase numOfRef by 1
      i->setNumOfRef(i->getNumOfRef() + 1);
      while (i && i->getNext()) {
        if (i->getNumOfRef() > i->getNext()->getNumOfRef()) {
          ConcatStringList::refList.swapReferenceNodes(i, i->getNext(), preI);
          // i had been change so don't need i = i->getNext() and preI =
          // preI->getNext()
        } else {
          break;
        }
      }
      found = true;
    }

    preI = i;
    i = i->getNext();
  }
}

void addReferenceNode(CharALNode *&node, const int &numOfRef) {
  if (ConcatStringList::refList.size() == 0) {
    ConcatStringList::refList.getHead() = ConcatStringList::refList.getTail() =
        new ReferencesListNode(node, numOfRef);
  } else {
    ReferencesListNode *i = ConcatStringList::refList.getHead(),
                       *preI = nullptr;
    while (i && i->getNumOfRef() < numOfRef) {
      preI = i;
      i = i->getNext();
    }
    // same address like &(*i) == &(*refList.getHead())
    ReferencesListNode *refNode = new ReferencesListNode(node, numOfRef);
    if (preI == nullptr) {
      // insert before element
      refNode->getNext() = i;
    } else {
      preI->getNext() = refNode;
      refNode->getNext() = i;
    }
    // increase size ReferencesList
    ConcatStringList::refList.setNumOfNodes(ConcatStringList::refList.size() +
                                            1);
  }
}
