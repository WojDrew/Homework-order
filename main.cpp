#include <iostream>
#define SIZE 1001

//Przy pisaniu projektu korzystalem z wyk³adow do tego przedmiotu dr in¿. Krzysztofa Manuszewskiego

using namespace std;

class Node
{
public:
	int key, degree, data;
	Node* child, *sibling, *parent;

	Node();

};

Node::Node()
{
	key = 0;
	degree = 0;
	child = nullptr;
	sibling = nullptr;
	parent = nullptr;
}

Node* MaxBNHeap(Node* heap);
void TreeLink(Node** newSon, Node** newParent);
Node* MergeBNHeap(Node** h1, Node** h2);
Node* UnionBNHeap(Node** y, Node** z);
Node* InsertBNHeap(Node** heap, int key, int data);
Node* OdwrocSynow(Node* heap);
void IncKeyBNHeap(Node** node, int newKey);
void RemoveBNHeap(Node** heap, Node** node);
Node* ExtractMaxBNHeap(Node** heap);
Node* Search(Node* heap, int value);

int main()
{
	Node* tab[SIZE];
	for (int i = 0; i < SIZE; i++)
		tab[i] = nullptr;
	int m;
	char n;
	cin >> m;
	for (int i = 0; i < m; i++)
	{
		cin >> n;
		if (n == 'a')
		{
			int group, value;
			cin >> group >> value;
			tab[group] = InsertBNHeap(&tab[group],value,0);
		}
		else if (n == 'e')
		{
			int group;
			cin >> group;
			if (tab[group] != nullptr)
			{
				Node* tmp = ExtractMaxBNHeap(&tab[group]);
				cout << tmp->key << endl;
				delete tmp;
			}
			else
				cout << "na" << endl;
		}
		else if (n == 'm')
		{
			int group1, group2;
			cin >> group1 >> group2;
			tab[group1] = UnionBNHeap(&tab[group1], &tab[group2]);
			delete tab[group2];
			tab[group2] = nullptr;
		}
		else if (n == 'i')
		{
			int group, oldValue, newValue;
			cin >> group >> oldValue >>  newValue;
			Node* tmp = Search(tab[group], oldValue);
			IncKeyBNHeap(&tmp, newValue);
		}
	}
	return 0;
}

Node* MaxBNHeap(Node* heap)
{
	Node* max = heap;
	Node* current = heap;
	while (current != nullptr)
	{
		if (current->key > max->key)
			max = current;
		current = current->sibling;
	}
	return max;
}

void TreeLink(Node** newSon, Node** newParent)
{
	(*newSon)->parent = *newParent;
	(*newSon)->sibling = (*newParent)->child;
	(*newParent)->child = *newSon;
	(*newParent)->degree = (*newParent)->degree + 1;
}

Node* MergeBNHeap(Node** h1, Node** h2)
{
	Node* ret = nullptr;
	Node* current = nullptr;
	Node* next;
	while (*h1 != nullptr && *h2 != nullptr)
	{
		if ((*h1)->degree <= (*h2)->degree)
		{
			next = *h1;
			*h1 = (*h1)->sibling;
		}
		else
		{
			next = *h2;
			*h2 = (*h2)->sibling;
		}
		if (ret == nullptr)
		{
			ret = next;
			current = next;
		}
		else
		{
			current->sibling = next;
			current = next;
		}
	}
	if (*h1 == nullptr)
		swap(*h1,*h2);
	if (ret == nullptr)
		return *h1;
	else
		current->sibling = *h1;
	return ret;
}

Node* UnionBNHeap(Node** y, Node** z)
{
	Node* ret = MergeBNHeap(y, z);
	if (ret == nullptr)
		return nullptr;
	Node* prevX = nullptr;
	Node* X = ret;
	Node* nextX = ret->sibling;
	while (nextX != nullptr)
	{
		if (X->degree != nextX->degree || (nextX->sibling != nullptr && nextX->degree == nextX->sibling->degree))
		{
			prevX = X;
			X = nextX;
			nextX = nextX->sibling;
		}
		else if (X->key >= nextX->key)
		{
			X->sibling = nextX->sibling;
			nextX->sibling = nullptr;
			TreeLink(&nextX, &X);
			nextX = X->sibling;
		}
		else
		{
			if (prevX == nullptr)
				ret = nextX;
			else
				prevX->sibling = nextX;
			X->sibling = nullptr;
			TreeLink(&X,&nextX);
			X = nextX;
			nextX = nextX->sibling;
		}
	}
	return ret;
}

Node* InsertBNHeap(Node** heap, int key, int data)
{
	Node* node = new Node();
	node->key = key;
	node->data = data;
	return UnionBNHeap(heap,&node);
}

Node* ExtractMaxBNHeap(Node** heap)
{
	Node* ret = MaxBNHeap(*heap);
	if(ret->parent != nullptr)
		ret->parent->sibling = ret->sibling;
	if(ret->sibling != nullptr)
		ret->sibling->parent = ret->parent;
	if (ret->child != nullptr)
	{
		Node* tmp = ret->child;
		Node* h1 = OdwrocSynow(tmp);
		if (h1->sibling == ret)
			h1->sibling = nullptr;
		if (ret == *heap)
			*heap = h1;
		else
			*heap = UnionBNHeap(heap, &h1);
	}
	if (ret == *heap)
		*heap = nullptr;
	ret->sibling = nullptr;
	ret->child = nullptr;
	return ret; 
}

Node* OdwrocSynow(Node* heap)
{
	Node* Crawl = heap;
	while (Crawl->parent != nullptr)
	{
		Node* tmp = Crawl->parent;
		Crawl->parent = Crawl->sibling;
		Crawl->sibling = tmp;
		if(Crawl->parent != nullptr)
			Crawl = Crawl->parent;
	}
	return Crawl;
}

void IncKeyBNHeap(Node** node, int newKey)
{
	(*node)->key = newKey;
	while ((*node)->parent != nullptr && (*node)->key > (*node)->parent->key)
	{
		swap((*node)->key, (*node)->parent->key);
		swap((*node)->data, (*node)->parent->data);
		*node = (*node)->parent;
	}
}

void RemoveBNHeap(Node** heap, Node** node)
{
	int tmp = (*node)->key;
	IncKeyBNHeap(node, MaxBNHeap(*heap)->key);
	*heap = ExtractMaxBNHeap(heap);
	*node = ExtractMaxBNHeap(heap);
	(*node)->key = tmp;
}

Node* Search(Node* heap, int value)
{
	bool koniec = false;
	Node* Crawl = heap;
	while (!koniec)
	{
		if (Crawl->key == value)
			koniec = true;
		if (Crawl->key > value)
			Crawl = Crawl->sibling;
		if(Crawl->key < value)
			Crawl = Crawl->child;
	}
	return Crawl;
}