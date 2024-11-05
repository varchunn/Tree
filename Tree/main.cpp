#include <iostream>
using namespace std;

class Nod 
{
public:
    char* eng;
    char* rus;
    int cnt;
    Nod* lft;
    Nod* rgt;

    Nod(const char* e, const char* r, int c = 0) : cnt(c), lft(nullptr), rgt(nullptr)
    {
        eng = new char[strlen(e) + 1];
        rus = new char[strlen(r) + 1];
        strcpy(eng, e);
        strcpy(rus, r);
    }

    ~Nod() 
    {
        delete[] eng;
        delete[] rus;
    }
};

class Dict 
{
private:
    Nod* root;

    Nod* addNod(Nod* node, const char* e, const char* r, int c = 0) 
    {
        if (!node) 
        {
            return new Nod(e, r, c);
        }
        if (strcmp(e, node->eng) < 0) 
        {
            node->lft = addNod(node->lft, e, r, c);
        } 
        else if
            (strcmp(e, node->eng) > 0) 
        {
            node->rgt = addNod(node->rgt, e, r, c);
        }
        return node;
    }

    Nod* find(Nod* node, const char* e)
    {
        if (!node || strcmp(e, node->eng) == 0)
        {
            return node;
        }
        if (strcmp(e, node->eng) < 0) 
        {
            return find(node->lft, e);
        } 
        else
        {
            return find(node->rgt, e);
        }
    }

    Nod* findMin(Nod* node) 
    {
        while (node && node->lft) node = node->lft;
        return node;
    }

    Nod* delNod(Nod* root, const char* e) 
    {
        if (!root) return nullptr;
        if (strcmp(e, root->eng) < 0)
        {
            root->lft = delNod(root->lft, e);
        } else if (strcmp(e, root->eng) > 0)
        {
            root->rgt = delNod(root->rgt, e);
        } else
        {
            if (!root->lft)
            {
                Nod* tmp = root->rgt;
                delete root;
                return tmp;
            }
            if (!root->rgt) 
            {
                Nod* tmp = root->lft;
                delete root;
                return tmp;
            }
            Nod* tmp = findMin(root->rgt);
            strcpy(root->eng, tmp->eng);
            strcpy(root->rus, tmp->rus);
            root->cnt = tmp->cnt;
            root->rgt = delNod(root->rgt, tmp->eng);
        }
        return root;
    }

    void showInOrder(Nod* node) 
    {
        if (!node) return;
        showInOrder(node->lft);
        cout << node->eng << ": " << node->rus << " (appeals: " << node->cnt << ")" << endl;
        showInOrder(node->rgt);
    }

    void findPopular(Nod* node, Nod*& pop, Nod*& npop) 
    {
        if (!node) return;
        if (!pop || node->cnt > pop->cnt) pop = node;
        if (!npop || node->cnt < npop->cnt) npop = node;
        findPopular(node->lft, pop, npop);
        findPopular(node->rgt, pop, npop);
    }

public:
    Dict() : root(nullptr) {}

    ~Dict() 
    {
        clear(root);
    }

    void addWord(const char* e, const char* r, int c = 0) 
    {
        root = addNod(root, e, r, c);
    }

    void showWord(const char* e) 
    {
        Nod* res = find(root, e);
        if (res) 
        {
            res->cnt++;
            cout << e << ": " << res->rus << " (appeals: " << res->cnt << ")" << endl;
        } 
        else
        {
            cout << "Word not found." << endl;
        }
    }

    void showAll() 
    {
        showInOrder(root);
    }

    void updateWord(const char* e, const char* r) 
    {
        Nod* res = find(root, e);
        if (res) 
        {
            delete[] res->rus;
            res->rus = new char[strlen(r) + 1];
            strcpy(res->rus, r);
            cout << "Update: " << e << " -> " << r << endl;
        } else {
            cout << "Not found." << endl;
        }
    }

    void deleteWord(const char* e) 
    {
        root = delNod(root, e);
    }

    void showPopular() 
    {
        Nod* pop = nullptr;
        Nod* npop = nullptr;
        findPopular(root, pop, npop);
        if (pop) cout << "Most famous: " << pop->eng << " (appeals: " << pop->cnt << ")" << endl;
        if (npop) cout << "Least famous: " << npop->eng << " (appeals: " << npop->cnt << ")" << endl;
    }

private:
    void clear(Nod* node) 
    {
        if (!node) return;
        clear(node->lft);
        clear(node->rgt);
        delete node;
    }
};

int main() 
{
    Dict d;
    d.addWord("hello", "hi", 2);
    d.addWord("world", "universe", 1);
    d.addWord("apple", "fruit", 0);

    d.showWord("hello");
    d.showAll();

    d.updateWord("apple", "fruit");
    d.showAll();

    d.deleteWord("world");
    d.showAll();

    d.showPopular();

    return 0;
}
