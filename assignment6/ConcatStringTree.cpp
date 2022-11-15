#include "ConcatStringTree.h"

long long powerFunc(int base, int power) {
  if (power == 0)
    return 1;
  else
    return (base * powerFunc(base, power - 1));
}

// ===================== ConcatStringTree ==========================
long long ConcatStringTree::currentID = 0;

ConcatStringTree::ConcatStringTree(const int &length, const int &leftLength,
                                   ConcatStringTreeNode *left,
                                   ConcatStringTreeNode *right) {
  this->root =
      new ConcatStringTreeNode(nullptr, leftLength, length, left, right);

  // update parentTree its self
  this->root->parentTree->insert(this->root->id);
}

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

    // update parentTree
    this->root->parentTree->insert(this->root->id);
  } else {
    this->root = nullptr;
  }
}

ConcatStringTree::ConcatStringTree(const string &s) {
  if (!s.empty()) {
    this->root = new ConcatStringTreeNode(s, 0, s.length());
    // update parentTree
    this->root->parentTree->insert(this->root->id);
  } else {
    this->root = nullptr;
  }
}

ConcatStringTree::~ConcatStringTree() {
  this->root->parentTree->remove(this->root->id);
  this->root->left->parentTree->remove(this->root->id);
  this->root->right->parentTree->remove(this->root->id);

  if (this->root->left) {
    if (root->left->parentTree->size() <= 0) {
      if (!this->checkDataIsNull(this->root->left)) {
        ((ReducedConcatStringTree *)(this))
            ->litStringHash->remove(this->root->left->data);
      }
      delete this->root->left;
      this->root->left = nullptr;
    }
  }

  if (this->root->right) {
    if (this->root->right->parentTree->size() <= 0) {
      // check right tree has one node
      if (!this->checkDataIsNull(this->root->right)) {
        ((ReducedConcatStringTree *)(this))
            ->litStringHash->remove(this->root->right->data);
      }
      delete this->root->right;
      this->root->right = nullptr;
    }
  }

  if (this->root) {
    if (this->root->parentTree->size() <= 0) {
      // check right tree has one node
      if (!this->checkDataIsNull(this->root)) {
        ((ReducedConcatStringTree *)(this))
            ->litStringHash->remove(this->root->data);
      }
      delete this->root;
      this->root = nullptr;
    }
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
  return "ConcatStringTree[\"" + toStringHelper(this->root) + "\"]";
}

ConcatStringTree
ConcatStringTree::concat(const ConcatStringTree &otherS) const {
  ConcatStringTree concatTree(this->root->length + otherS.root->length,
                              this->root->length, this->root, otherS.root);

  // update parentTree for 2 ConcatStringTree
  this->root->parentTree->insert(concatTree.root->id);
  otherS.root->parentTree->insert(concatTree.root->id);
  return concatTree;
}

ConcatStringTree ConcatStringTree::subStringHelper(ConcatStringTreeNode *root,
                                                   int &from, int &to,
                                                   int &totalLengthStr) const {
  if (!root || totalLengthStr > to) {
    return nullptr;
  }

  ConcatStringTree treeLeft =
      subStringHelper(root->left, from, to, totalLengthStr);
  ConcatStringTree treeRight =
      subStringHelper(root->right, from, to, totalLengthStr);

  // node is not leaf
  if (checkDataIsNull(treeLeft.root) || checkDataIsNull(treeRight.root)) {
    return treeLeft.concat(treeRight);
  }

  // node is leaf
  // s3.subString(5, 6)
  if (from >= root->length) {
    from -= root->length;
    totalLengthStr += root->length;
    return nullptr;
  }

  // s3.subString(1, 10)
  string s = "";
  if (root->length + totalLengthStr <= to) {
    s = s.substr(from);
    from = 0;
    totalLengthStr += root->length;
  } else {
    int numOfCharacters = to - totalLengthStr;
    s = s.substr(from, numOfCharacters);
  }

  if (s.empty()) {
    return nullptr;
  }
  return ConcatStringTree(s);
}

ConcatStringTree ConcatStringTree::subString(int from, int to) const {
  if (from >= this->root->length || to > this->root->length || from < 0 ||
      to < 0) {
    throw out_of_range("Index of string is invalid");
  } else if (from >= to) {
    throw logic_error("Invalid range");
  }

  int totalLengthStr = 0;
  return subStringHelper(root, from, to, totalLengthStr);
}

string ConcatStringTree::reverseStr(const string &s) const {
  string result = "";
  for (int i = s.length() - 1; i >= 0; i++) {
    result += s[i];
  }
  return result;
}

ConcatStringTree
ConcatStringTree::reverseHelper(ConcatStringTreeNode *root) const {
  if (!root) {
    return nullptr;
  }

  ConcatStringTree treeLeft = reverseHelper(root->left);
  ConcatStringTree treeRight = reverseHelper(root->right);

  // node is not leaf
  if (checkDataIsNull(treeLeft.root) || checkDataIsNull(treeRight.root)) {
    return treeRight.concat(treeLeft);
  }

  // node is leaf
  return ConcatStringTree(reverseStr(root->data));
}

ConcatStringTree ConcatStringTree::reverse() const {
  return reverseHelper(root);
}

int ConcatStringTree::getParTreeSize(const string &query) const {
  ConcatStringTreeNode *temp = root;
  for (int i = 0; i < query.length(); i++) {
    if (temp == nullptr) {
      throw runtime_error("Invalid query: reaching NULL");
    }

    if (query[i] == 'l') {
      temp = temp->left;
    } else if (query[i] == 'r') {
      temp = temp->right;
    } else {
      throw runtime_error("Invalid character of query");
    }
  }
  return temp->parentTree->size();
}

string ConcatStringTree::getParTreeStringPreOrder(const string &query) const {
  ConcatStringTreeNode *temp = root;
  for (int i = 0; i < query.length(); i++) {
    if (temp == nullptr) {
      throw runtime_error("Invalid query: reaching NULL");
    }

    if (query[i] == 'l') {
      temp = temp->left;
    } else if (query[i] == 'r') {
      temp = temp->right;
    } else {
      throw runtime_error("Invalid character of query");
    }
  }
  return root->parentTree->toStringPreOrder();
}

// ===================== ParentsTree ==========================
int ParentsTree::getHeightRec(ParentsTreeNode *node) const {
  if (node == nullptr) {
    return 0;
  }
  int lh = getHeightRec(node->left);
  int rh = getHeightRec(node->right);
  return (lh > rh ? lh : rh) + 1;
}

int ParentsTree::getHeight() const { return getHeightRec(root); }

ParentsTree::ParentsTreeNode *ParentsTree::leftRotate(ParentsTreeNode *x) {
  ParentsTreeNode *y = x->right;
  ParentsTreeNode *z = y->left;
  y->left = x;
  x->right = z;
  return y;
}

ParentsTree::ParentsTreeNode *ParentsTree::rightRotate(ParentsTreeNode *x) {
  ParentsTreeNode *y = x->left;
  ParentsTreeNode *z = y->right;
  y->right = x;
  x->left = z;
  return y;
}

int ParentsTree::getBalance(ParentsTreeNode *node) const {
  if (node == nullptr) {
    return 0;
  }
  return getHeightRec(node->left) - getHeightRec(node->right);
}

ParentsTree::ParentsTreeNode *
ParentsTree::insertHelper(ParentsTreeNode *node, const long long &value) {
  if (node == nullptr) {
    return new ParentsTreeNode(value);
  }

  if (value < node->id) {
    node->left = insertHelper(node->left, value);
  } else {
    node->right = insertHelper(node->right, value);
  }

  int balance = getBalance(node);

  if (balance > 1 && value < node->left->id) {
    return rightRotate(node);
  }
  if (balance < -1 && value > node->right->id) {
    return leftRotate(node);
  }

  if (balance > 1 && value > node->left->id) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && value < node->right->id) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

void ParentsTree::insert(const long long &value) {
  this->root = insertHelper(root, value);
  this->nE++;
}

ParentsTree::ParentsTreeNode *
ParentsTree::maxValueNode(ParentsTreeNode *node) const {
  ParentsTreeNode *current = node;
  while (current->right) {
    current = current->right;
  }
  return current;
}

ParentsTree::ParentsTreeNode *
ParentsTree::removeHelper(ParentsTreeNode *root, const long long &value) {
  if (root == NULL)
    return root;

  if (value < root->id)
    root->left = removeHelper(root->left, value);
  else if (value > root->id)
    root->right = removeHelper(root->right, value);
  else {
    if ((root->left == NULL) || (root->right == NULL)) {
      ParentsTreeNode *temp = root->left ? root->left : root->right;

      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else
        *root = *temp;
      free(temp);
    } else {
      ParentsTreeNode *temp = maxValueNode(root->left);

      root->id = temp->id;

      root->left = removeHelper(root->left, temp->id);
    }
  }

  if (root == NULL)
    return root;

  int balance = getBalance(root);

  if (balance > 1 && getBalance(root->left) > 0)
    return rightRotate(root);

  if (balance > 1 && getBalance(root->left) <= 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && getBalance(root->right) <= 0)
    return leftRotate(root);

  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

void ParentsTree::remove(const long long &value) {
  this->root = removeHelper(root, value);
  this->nE--;
}

int ParentsTree::size() const { return this->nE; }

string ParentsTree::toStringPreOrderHelper(ParentsTreeNode *root) const {
  if (!root) {
    return "";
  }
  string toStringRoot = "(id=" + to_string(root->id) + ");";
  return toStringRoot + toStringPreOrderHelper(root->left) +
         toStringPreOrderHelper(root->right);
}

string ParentsTree::toStringPreOrder() const {
  string preOrderString = toStringPreOrderHelper(this->root);
  if (!preOrderString.empty()) {
    preOrderString.erase(preOrderString.begin() + preOrderString.length() - 1);
  }
  return "ParentsTree[" + preOrderString + "]";
}

// =========================== Vector ============================
template <class T> Vector<T>::Vector() {
  this->Size = 0;
  this->capacity = 50;
  this->arr = new T[capacity];
}

template <class T> Vector<T>::Vector(const int &newsize) {
  this->Size = newsize;
  this->capacity = newsize + 10;
  this->arr = new T[newsize];
}

template <class T> Vector<T>::~Vector() {
  if (this->arr) {
    delete[] this->arr;
    this->arr = nullptr;
  }
  this->Size = 0;
  this->capacity = 0;
}

template <class T> void Vector<T>::ensureCap() {
  T *old = this->arr;
  this->arr = new T[this->capacity + 10];
  for (int i = 0; i < this->Size; i++) {
    this->arr[i] = old[i];
  }
  delete[] old;
  this->capacity += 10;
}

template <class T> void Vector<T>::ensureCap(const int &newsize) {
  T *old = this->arr;
  this->arr = new T[newsize];
  for (int i = 0; i < this->Size; i++) {
    this->arr[i] = old[i];
  }
  delete[] old;
  this->capacity = newsize + 10;
}

template <class T> int Vector<T>::size() const { return this->Size; }

template <class T> T &Vector<T>::operator[](int index) {
  return this->arr[index];
}

template <class T> const T &Vector<T>::getAt(int index) const {
  return this->arr[index];
}

template <class T> Vector<T> &Vector<T>::operator=(Vector<T> &new_vec) {
  if (this != &new_vec) {
    delete[] this->arr;
    this->arr = new T[new_vec.Size];
    for (int i = 0; i < new_vec.Size; i++) {
      this->arr[i] = new_vec[i];
    }
    this->Size = new_vec.size();
    this->capacity = new_vec.capacity;
  }
  return *this;
}

template <class T> void Vector<T>::push_back(T newElement) {
  if (this->Size + 1 >= this->capacity) {
    ensureCap();
  }
  this->Size += 1;
  this->arr[Size - 1] = newElement;
}

template <class T> void Vector<T>::insert(T newElement, int idx) {
  this->Size++;
  this->arr[idx] = newElement;
}

// ===================== LitString =========================
string LitString::toString() const {
  if (this->numLinks == 0) {
    return "";
  }
  return "litS=\"" + this->data + "\"";
}

// ===================== ListStringHash ====================
LitStringHash::LitStringHash(const HashConfig &hashConfig)
    : hashConfig(hashConfig), vec(Vector<LitString>(hashConfig.initSize)),
      lastInsertedIndex(-1) {}

long long LitStringHash::hashFunction(const string &s, const int &p,
                                      const int &m) {
  long long total;
  for (int i = 0; i < s.length(); i++) {
    total = (int)s[i] * powerFunc(p, i);
  }
  return total % m;
}

long long LitStringHash::detectionQuadratic(const string &s) {
  long long hf =
      this->hashFunction(s, this->hashConfig.p, this->hashConfig.initSize);
  long long i = 0;
  long long hp;
  bool found = false;
  for (; i < this->hashConfig.initSize && !found; i++) {
    hp = (long long)(hf + this->hashConfig.c1 * i +
                     this->hashConfig.c2 * i * i) %
         this->hashConfig.initSize;
    // LitString element exists in ListStringHash
    if (this->vec[hp].data == s) {
      this->vec[hp].numLinks++;
      found = true;
    }
    // no LitString element exists in ListStringHash
    else if (this->vec[hp].numLinks == 0) {
      this->vec.insert(LitString(s, 1), hp);
      found = true;
    }
  }

  if (!found) {
    throw runtime_error("No possible slot");
  }

  // rehash LitStringHash
  double load_factor = (double)this->vec.size() / this->hashConfig.initSize;
  if (load_factor > this->hashConfig.lambda) {
    this->hashConfig.initSize =
        (int)(this->hashConfig.alpha * this->vec.size());
    this->vec.ensureCap(this->hashConfig.initSize);
  }

  this->lastInsertedIndex = --i;

  return this->lastInsertedIndex;
}

int LitStringHash::getLastInsertedIndex() const {
  return this->lastInsertedIndex;
}

long long LitStringHash::remove(const string &s) {
  bool found = false;
  int i = 0;
  for (; i < this->hashConfig.initSize && !found; i++) {
    if (this->vec[i].data == s) {
      this->vec[i].numLinks--;
      if (this->vec[i].numLinks == 0) {
        this->vec[i].data = nullptr;
        this->lastInsertedIndex = -1;
      }
      found = true;
    }
  }
  if (found) {
    return i;
  }
  return -1;
}

string LitStringHash::toString() const {
  string result = "";
  for (int i = 0; i < this->vec.size(); i++) {
    result = result + "(" + this->vec.getAt(i).toString() + ")";
    if (i != this->vec.size() - 1) {
      result += ";";
    }
  }
  return "LitStringHash[" + result + "]";
}

// ==================== ReducedConcatStringTree =====================
ReducedConcatStringTree::ReducedConcatStringTree(const char *s,
                                                 LitStringHash *litStringHash)
    : ConcatStringTree(s) {
  this->litStringHash = litStringHash;
  // insert into LitStringHash
  this->litStringHash->detectionQuadratic(this->root->data);
}
