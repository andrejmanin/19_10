#include <cstring>
#include <iostream>
#include <queue>

using namespace std;

struct vock {
    char* word;
    char* translate;
    int popularity;

    vock(char* word, char* translate, int popularity) {
        this->word = word;
        this->translate = translate;
        this->popularity = popularity;
    }

    vock(vock& obj) {
        this->word = obj.word;
        this->translate = obj.translate;
        this->popularity = obj.popularity;
    }

    vock& operator=(vock& obj) {
        this->word = obj.word;
        this->translate = obj.translate;
        this->popularity = obj.popularity;
        return *this;
    }
    void display(char* symbol = "") {
        cout << "\n" << symbol<< "Word: " << this->word << "\n" << symbol << "Translate: " << this->translate << "\n" << symbol << "Popularity: " << this->popularity << endl;
    }
};

class node {
public:
    vock* _vock;
    node* left;
    node* right;
    node* parent;

    node(vock* vock, node* left, node* right, node* parent) {
        this->_vock = vock;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }

    node(vock &data) {
        _vock = &data;
    }

    bool operator<(const node& rhs) const {
        return this->_vock->popularity < rhs._vock->popularity;
    }
    bool operator>(const node& rhs) const {
        return this->_vock->popularity > rhs._vock->popularity;
    }
};

struct top3 {
    node* top;
    node* middle;
    node* bottom;
};

struct worst3 {
    node* top;
    node* middle;
    node* bottom;
};


class tree {
    top3 _top3;
    worst3 _worst3;

    static int wordCheck(const char* word, const char* baseWord) {
        if(baseWord == nullptr) {
            return 0;
        }
        int i = 0;
        while(word[i] == baseWord[i] || (i == strlen(baseWord) && i == strlen(word))) {
            i++;
            if(i == strlen(baseWord) || i == strlen(word)) {
                break;
            }
        }
        if(i == strlen(baseWord) && i == strlen(word)) {
            return -1;
        }
        return word[i] < baseWord[i] ? 0 : 1;
    }

    node* findWord(char* word) const {
        node* curr = root;
        while (true) {
            int i = wordCheck(word, curr->_vock->word);
            if(i == -1) {
                curr->_vock->popularity++;
                return curr;
            }
            if(i == 0) {
                curr = curr->left;
            }
            if(i == 1) {
                curr = curr->right;
            }
            if(curr == nullptr) {
                return nullptr;
            }
        }
    }

    node* findTranslate(char* translate) const {
        node* curr = root;
        while (true) {
            int i = wordCheck(translate, curr->_vock->translate);
            if(i == -1) {
                curr->_vock->popularity++;
                return curr;
            }
            if(i == 0) {
                curr = curr->left;
            }
            if(i == 1) {
                curr = curr->right;
            }
            if(curr == nullptr) {
                return nullptr;
            }
        }
    }

    node* findMin(node* curr) {
        if(curr == nullptr) {
            return nullptr;
        } else if(curr->left == nullptr) {
            return curr;
        } else {
            return findMin(curr->left);
        }

    }
public:
    node* root;
    tree() {
        root = new node(nullptr, nullptr, nullptr, nullptr);

        _top3.top = nullptr;
        _top3.middle = nullptr;
        _top3.bottom = nullptr;
        _worst3.top = nullptr;
        _worst3.middle = nullptr;
        _worst3.bottom = nullptr;
    }

    void insert(char* word, char* translate, int popularity = 0) const {
        if (root->_vock == nullptr) {
            root->_vock = new vock(word, translate, popularity);
            return;
        }
        node* curr = root;
        node* prev = nullptr;
        while (true) {
            prev = curr;
            if(wordCheck(word, curr->_vock->word) == 0) {
                curr = curr->left;
                if(curr == nullptr) {
                    prev->left = new node(new vock(word, translate, popularity), nullptr, nullptr, prev);
                    return;
                }
            } else if(wordCheck(word, curr->_vock->word) == 1) {
                curr = curr->right;
                if(curr == nullptr) {
                    prev->right = new node(new vock(word, translate, popularity), nullptr, nullptr, prev);
                    return;
                }
            } else {
                cout << "Error: " << word << " is already exist in the tree" << endl;
                return;
            }
        }
    }

    void showTree(node* curr) {
        if(curr == nullptr) {
            return;
        }

        showTree(curr->left);
        curr->_vock->display();

        showTree(curr->right);
    }

    void showWord(char* word) const {
        node* curr = findWord(word);
        if(curr == nullptr) {
            cout << "Your word isn't founded" << endl;
            return;
        }
        cout << "Word: " << word << "\nTranslate: " << curr->_vock->translate << endl;
    }

    void changeWord(char* oldTranslate, char* newTranslate) {
        node* curr = findTranslate(oldTranslate);
        if(curr == nullptr) {
            return;
        }
        curr->_vock->translate = newTranslate;
    }

    void deleteWord(char* word) {
        node* curr = findWord(word);
        if(curr == nullptr) {
            return;
        }
        if(curr->right == nullptr && curr->left == nullptr) {
            if(curr->parent->left == curr) {
                curr->parent->left = nullptr;
                delete curr;
            } else {
                curr->parent->right = nullptr;
                delete curr;
            }
        } else if(curr->right == nullptr) {
            if(curr->parent->left == curr) {
                curr->parent->left = curr->left;
            } else {
                curr->parent->right = curr->left;
            }
            curr->left->parent = curr->parent;
            delete curr;
        } else if(curr->left == nullptr) {
            if(curr->parent->right == curr) {
                curr->parent->right = curr->right;
            } else {
                curr->parent->left = curr->right;
            }
            curr->right->parent = curr->parent;
            delete curr;
        } else {
            if(curr->parent->left == curr) {
                curr->parent->left = curr->right;
            } else {
                curr->parent->right = curr->right;
            }
            node* temp = findMin(curr->right);
            temp->left = curr->left;
            curr->left->parent = temp;
            delete curr;
        }
    }

    void showPop(node* root) {
        if (root == nullptr) return;

        queue<node*> q;
        q.push(root);


        while (!q.empty()) {
            node* current = q.front();
            q.pop();

            if(_top3.top == nullptr || _top3.top < current) {
                _top3.bottom = _top3.middle;
                _top3.middle = _top3.top;
                _top3.top = current;
            } else if(_top3.middle == nullptr || _top3.middle->_vock->popularity < current->_vock->popularity) {
                _top3.bottom = _top3.middle;
                _top3.middle = current;
            } else if(_top3.bottom == nullptr || _top3.bottom->_vock->popularity < current->_vock->popularity) {
                _top3.bottom = current;
            }

            if(_worst3.bottom > current || _worst3.bottom == nullptr) {
                _worst3.top = _worst3.middle;
                _worst3.middle = _worst3.bottom;
                _worst3.bottom = current;
            } else if(_worst3.middle > current || _worst3.middle == nullptr) {
                _worst3.top = _worst3.middle;
                _worst3.middle = current;
            } else if(_worst3.top > current || _worst3.top == nullptr) {
                _worst3.top = current;
            }

            if (current->left != nullptr) q.push(current->left);
            if (current->right != nullptr) q.push(current->right);
        }
        if(_top3.top != nullptr) {
            cout << "Top: ";
            _top3.top->_vock->display("\t");
            cout << endl;
            if(_top3.middle != nullptr) {
                cout << "Middle: ";
                _top3.middle->_vock->display("\t");
                cout << endl;
                if(_top3.bottom != nullptr) {
                    cout << "Bottom: ";
                    _top3.bottom->_vock->display("\t");
                    cout << endl << endl;
                }
            }
        }
        if (_worst3.top != nullptr) {
            cout << "Worst: ";
            _worst3.top->_vock->display("\t");
            cout << endl;
            if (_worst3.middle != nullptr) {
                cout << "Worse: ";
                _worst3.middle->_vock->display("\t");
                cout << endl;
                if (_worst3.bottom != nullptr) {
                    cout << "Bad: ";
                    _worst3.bottom->_vock->display("\t");
                    cout << endl;
                }
            }
        }
    }
};

int main() {
    tree t;
    t.insert("Hi", "Ahoj", 5);
    t.insert("house", "dům", 4);
    t.insert("dog", "pes", 2);
    t.insert("Good Bye", "Nashledanou", 8);

    // t.showTree(t.root);
    // cout << endl;
    // t.showWord("bye");
    // t.changeWord("ahoj", "cao");
    // t.showWord("hello");
    // cout << endl;
    // t.deleteWord("bye");
    t.showTree(t.root);

    cout << endl << endl;
    t.showPop(t.root);
}
