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
ConcatStringList::ConcatStringList() { this->head = this->tail = nullptr; }

ConcatStringList::ConcatStringList(const char *s) {
  if (s) {
    this->head = this->tail = new CharALNode(s);
    this->totalLength += this->head->getCharArrayList().size();
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
  return "ConcatStringList[\"" + s + "\"]";
}

ConcatStringList
ConcatStringList::concat(const ConcatStringList &otherS) const {
  // https://stackoverflow.com/questions/48861494/c-why-can-a-passed-objects-private-variables-be-accessed-directly-in-the-co
  ConcatStringList concatString = ConcatStringList();
  concatString.totalLength = otherS.totalLength + this->totalLength;
  this->tail->getNext() = otherS.head;
  concatString.head = this->head;
  concatString.tail = this->tail;
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
    i = i->getNext();
    from -= i->getCharArrayList().size();
    to -= i->getCharArrayList().size();
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
      CharALNode* temp = new CharALNode(reverseString);
      temp->getNext() = concatString.head;
      concatString.head = temp;
    }
    concatString.totalLength += concatString.head->getCharArrayList().size();
    delete [] reverseString;
    i = i->getNext();
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

ConcatStringList::~ConcatStringList() {
}
