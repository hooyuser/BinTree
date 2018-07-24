#ifndef BINTREE_H_
#define BINTREE_H_

#include "stdio.h"
#include <algorithm>
#include <typeinfo.h>
#include "stack.h"
//#include "vector.h"
#include "queue.h"


#define BinNodePosi(T) BinNode<T> * //�ڵ�λ�ã���ָ��BinNode�����ָ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ��롰�����߶�Ϊ-1����Լ����ͳһ��
typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ


/******************************************************************************************
* BinNode״̬�����ʵ��ж�
******************************************************************************************/
#define IsRoot(x) (!((x).parent)) //�жϽڵ�x�Ƿ�Ϊ���ڵ�
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc)) //�жϽڵ�x�Ƿ�Ϊĳһ�ڵ������
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc)) //�жϽڵ�x�Ƿ�Ϊĳһ�ڵ���Һ���
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc) //�жϽڵ�x�Ƿ�ӵ������
#define HasRChild(x) ((x).rc) //�жϽڵ�x�Ƿ�ӵ���Һ���
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //�жϽڵ�x�Ƿ�����ӵ��һ������
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //�жϽڵ�x�Ƿ�ͬʱӵ����������
#define IsLeaf(x) (!HasChild(x)) //�жϽڵ�x�Ƿ�ΪҶ�ڵ�

/******************************************************************************************
* ��BinNode�����ض���ϵ�Ľڵ㼰ָ��
******************************************************************************************/
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc ) //����ָ��p��ָ�ڵ���ֵܵ�ָ��
#define uncle(p) sibling((p)->parent) //����ָ��p��ָ�ڵ�������ָ��
#define FromParentTo(x) (IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) ) //���ؽڵ�x�ĸ���ӵ�е�һ��ָ�룬��ָ��ָ��x����
//#define uncle(x) (IsLChild(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc ) //����


using namespace std;


/******************************************************************************************
* �б������Ƚṹ�ڵĽڵ��У����Դ�Ż������ͻ�������
* ���ձ���Լ��������Ч�ʵĿ��ǣ����ں�һ���ͨ��ֻ��������ָ��
* ��ˣ�������ṹ����֮ǰ����Ҫ�����ͷ���Щ��Ա������ռ�Ŀռ�
* �˴�������C++��ƫ�ػ����������������������������Ӧ����
*******************************************************************************************/

template <typename T>
struct Cleaner
{
	static void clean(T x)
	{ //�൱�ڵݹ��
#ifdef _DEBUG
		static int n = 0;
		if (7 > strlen(typeid (T).name()))
		{ //��������һ�ź��ԣ�ֻ�����������
			printf("\t<%s>[%d]=", typeid (T).name(), ++n);
			//print(x);
			printf(" purged\n");
		}
#endif
	}
};

template <typename T>
struct Cleaner<T*>
{
	static void clean(T* x)
	{
		if (x) { delete x; } //������а���ָ�룬�ݹ��ͷ�
#ifdef _DEBUG                
		static int n = 0;
		printf("\t<%s>[%d] released\n", typeid (T*).name(), ++n);
#endif
	}

};

template <typename T>
void release(T x)
{
	Cleaner<T>::clean(x);
}


/*�������ڵ�ģ����*/
template <typename T>
class BinNode
{
public:
	// ��Ա
	T data; //��ֵ
	BinNodePosi(T) parent; //���ڵ�
	BinNodePosi(T) lc; //����
	BinNodePosi(T) rc; //�Һ���
	int height; //�߶ȣ�ͨ�ã�
	int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
	RBColor color; //��ɫ���������

 // ���캯��
	BinNode() :
		parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) { }

	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
		int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }

	// �����ӿ�
	int size(); //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
	BinNodePosi(T) insertAsLC(T const&); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const&); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
	BinNodePosi(T) succ(); //ȡ��ǰ�ڵ��ֱ�Ӻ��
	template <typename VST> void travLevel(VST&); //������α���
	template <typename VST> void travPre(VST&); //�����������
	template <typename VST> void travIn(VST&); //�����������
	template <typename VST> void travPost(VST&); //�����������

 // �Ƚ������е���
	bool operator< (BinNode const& bn) //С��
	{
		return data < bn.data;
	}
	bool operator== (BinNode const& bn) //����
	{
		return data == bn.data;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////
/*�������ڵ�ģ����ʵ��*/
///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
int BinNode<T>::size() //ͳ�Ƶ�ǰ�ڵ���������������Ϊ����������ģ
{
	int s = 1; //���뱾��
	if (lc) s += lc->size(); //�ݹ������������ģ
	if (rc) s += rc->size(); //�ݹ������������ģ
	return s;
}


template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	return lc = new BinNode(e, this); //��e��Ϊ��ǰ�ڵ�����Ӳ�������� 
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) //��e��Ϊ��ǰ�ڵ���Һ��Ӳ��������
{
	return rc = new BinNode(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ()  //��λ�ڵ�v��ֱ�Ӻ��
{
	BinNodePosi(T) s = this; //��¼��̵���ʱ����
	if (rc)
	{ //�����Һ��ӣ���ֱ�Ӻ�̱����������У�����ؾ���
		s = rc; //��������
		while (HasLChild(*s))
			s = s->lc; //�����С���Ľڵ�
	}
	else
	{ //����ֱ�Ӻ��Ӧ�ǡ�����ǰ�ڵ���������������е�������ȡ�������ؾ���
		while (IsRChild(*s))
			s = s->parent; //������������֧�����ϳ����Ϸ��ƶ�
		s = s->parent; //����ٳ����Ϸ��ƶ�һ�������ִ�ֱ�Ӻ�̣�������ڣ�
	}
	return s;
}

template <typename T> template <typename VST> //Ԫ�����͡�������
void BinNode<T>::travLevel(VST& visit)  //��������α����㷨
{
	Queue<BinNodePosi(T)> Q; //��������
	Q.enqueue(this); //���ڵ����
	while (!Q.empty()) //�ڶ����ٴα��֮ǰ����������
	{
		BinNodePosi(T) x = Q.dequeue();
		visit(x->data); //ȡ�����׽ڵ㲢����֮
		if (HasLChild(*x)) Q.enqueue(x->lc); //�������
		if (HasRChild(*x)) Q.enqueue(x->rc); //�Һ������
	}
}

template <typename T> template <typename VST> //Ԫ�����͡�������
void BinNode<T>::travPre(VST& visit)
{ //��������������㷨ͳһ���
	switch (rand() % 3)  //�˴������ѡ���������ԣ�������ѡ��
	{
	case 1: travPre_I1(this, visit); break; //������#1
	case 2: travPre_I2(this, visit); break; //������#2
	default: travPre_R(this, visit); break; //�ݹ��
	}
}

template <typename T, typename VST> //Ԫ�����͡�������
void travPre_I1(BinNodePosi(T) x, VST& visit) //��������������㷨��������#1��
{
	Stack<BinNodePosi(T)> S; //����ջ
	if (x)
		S.push(x); //���ڵ���ջ
	while (!S.empty())
	{ //��ջ���֮ǰ����ѭ��
		x = S.pop();
		visit(x->data); //���������ʵ�ǰ�ڵ㣬��ǿպ��ӵ���ջ����Ϊ���Һ���
		if (HasRChild(*x))
			S.push(x->rc);
		if (HasLChild(*x))
			S.push(x->lc);
	}
}

//�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ㣻��;�ڵ���������������
template <typename T, typename VST> //Ԫ������T��������VST
static void visitAlongVine(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{
		visit(x->data); //���ʵ�ǰ�ڵ�
		S.push(x->rc); //�Һ�����ջ�ݴ棨���Ż���ͨ���жϣ�����յ��Һ�����ջ��
		x = x->lc;  //�����֧����һ��
	}
}

template <typename T, typename VST> //Ԫ�����͡�������
void travPre_I2(BinNodePosi(T) x, VST& visit)  //��������������㷨��������#2��
{
	Stack<BinNodePosi(T)> S; //����ջ
	while (true)
	{
		visitAlongVine(x, visit, S); //�ӵ�ǰ�ڵ��������������
		if (S.empty()) break; //��ջ�գ����˳�ѭ��
		x = S.pop(); //������һ�������
	}
}

template <typename T, typename VST> //Ԫ�����͡�������
void travPre_R(BinNodePosi(T) x, VST& visit) //��������������㷨���ݹ�棩
{
	if (!x) return;
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}

template <typename T> template <typename VST> //Ԫ�����͡�������
void BinNode<T>::travIn(VST& visit)  //��������������㷨ͳһ���
{
	switch (rand() % 5)  //�˴������ѡ���������ԣ�������ѡ��
	{
	case 1: travIn_I1(this, visit); break; //������#1
	case 2: travIn_I2(this, visit); break; //������#2
	case 3: travIn_I3(this, visit); break; //������#3
	case 4: travIn_I4(this, visit); break; //������#4
	default: travIn_R(this, visit); break; //�ݹ��
	}
}

template <typename T> //�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ�
static void goAlongVine(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S) 
{
	while (x) 
	{ 
		S.push(x); 
		x = x->lc; 
	} //��ǰ�ڵ���ջ���漴������֧���룬����ֱ��������
}

template <typename T, typename VST> //Ԫ�����͡�������
void travIn_I1(BinNodePosi(T) x, VST& visit) { //��������������㷨��������#1��
	Stack<BinNodePosi(T)> S; //����ջ
	while (true) {
		goAlongVine(x, S); //�ӵ�ǰ�ڵ������������ջ
		if (S.empty()) break; //ֱ�����нڵ㴦�����
		x = S.pop(); 
		visit(x->data); //����ջ���ڵ㲢����֮
		x = x->rc; //ת��������
	}
}

template <typename T, typename VST> //Ԫ�����͡�������
void travIn_R(BinNodePosi(T) x, VST& visit)
{ //��������������㷨���ݹ�棩
	if (!x) return;
	travIn_R(x->lc, visit);
	visit(x->data);
	travIn_R(x->rc, visit);
}

template <typename T> template <typename VST> //Ԫ�����͡�������
void BinNode<T>::travPost(VST& visit)  //��������������㷨ͳһ���
{
	switch (rand() % 2)  //�˴������ѡ���������ԣ�������ѡ��
	{
	case 1: travPost_I(this, visit); break; //������
	default: travPost_R(this, visit); break; //�ݹ��
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////
/*������ģ����*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class BinTree
{
protected:
	int _size; //��ģ
	BinNodePosi(T) _root; //���ڵ�ָ��
	virtual int updateHeight(BinNodePosi(T) x); //���������߶ȸ��½ڵ�x�ĸ߶�
	void updateHeightAbove(BinNodePosi(T) x); //���½ڵ�x�������ȵĸ߶�
public:
	BinTree() : _size(0), _root(NULL) { } //���캯��

	~BinTree() //��������
	{
		if (0 < _size)
			remove(_root);
	}

	int size() const  //��ģ
	{
		return _size;
	}

	bool empty() const  //�п�
	{
		return !_root;
	}

	BinNodePosi(T) root() const //����
	{
		return _root;
	}

	BinNodePosi(T) insertAsRoot(T const& e); //������ڵ�

	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e��Ϊx�����ӣ�ԭ�ޣ�����

	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e��Ϊx���Һ��ӣ�ԭ�ޣ�����

	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������

	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������

	int remove(BinNodePosi(T) x); //ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ

	BinTree<T>* secede(BinNodePosi(T) x); //������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������

	template <typename VST> //������
	void travLevel(VST& visit)
	{
		if (_root)
			_root->travLevel(visit);
	} //��α���

	template <typename VST> //������
	void travPre(VST& visit)
	{
		if (_root)
			_root->travPre(visit);
	} //�������

	template <typename VST> //������
	void travIn(VST& visit)
	{
		if (_root)
			_root->travIn(visit);
	} //�������

	template <typename VST> //������
	void travPost(VST& visit)
	{
		if (_root)
			_root->travPost(visit);
	} //�������

	bool operator< (BinTree<T> const& t) //�Ƚ������������в��䣩
	{
		return _root && t._root && lt(_root, t._root);
	}
	bool operator== (BinTree<T> const& t) //�е���
	{
		return _root && t._root && (_root == t._root);
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////
/*������ģ����ʵ��*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //���½ڵ�x�߶�
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
} //���������������

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //���¸߶�
{
	while (x)
	{
		updateHeight(x);
		x = x->parent;
	}
} //��x�����������������ȡ����Ż�

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) //��e�������ڵ����յĶ�����
{
	_size = 1;
	return _root = new BinNode<T>(e);
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)  //e����Ϊx������
{
	_size++;
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)  //e����Ϊx���Һ���
{
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
}

template <typename T> //���������������㷨����S�����ڵ�x�����������룬S�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S)
{ //x->lc == NULL
	if (x->lc = S->_root) //����S���ڵ�����Ϊx�����ӡ����S->_root��ΪNULL
		x->lc->parent = x;  //��x����Ϊ��S���ڵ�ĸ���
	_size += S->_size;
	updateHeightAbove(x); //����ȫ����ģ��x�������ȵĸ߶�
	S->_root = NULL;
	S->_size = 0;
	release(S);
	S = NULL;
	return x; //�ͷ�ԭ�������ؽ���λ��
}

template <typename T> //���������������㷨����S�����ڵ�x�����������룬S�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S)
{ //����x->rc == NULL
	if (x->rc = S->_root) //����S���ڵ�����Ϊx���Һ��ӡ����S->_root��ΪNULL
		x->rc->parent = x; //��x����Ϊ��S���ڵ�ĸ���
	_size += S->_size; //����ȫ����ģ
	updateHeightAbove(x); //����x�������ȵĸ߶�
	S->_root = NULL;
	S->_size = 0;
	release(S);
	S = NULL;
	return x; //�ͷ�ԭ�������ؽ���λ��
}

template <typename T> //���������������㷨��������x�ӵ�ǰ����ժ���������װΪһ�ö�����������
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x)
{ //assert: xΪ�������еĺϷ�λ�á���������������������������������������������������������
	FromParentTo(*x) = NULL; //�ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent); //����ԭ�����������ȵĸ߶�
	BinTree<T>* S = new BinTree<T>;
	S->_root = x;
	x->parent = NULL; //������xΪ��
	S->_size = x->size();
	_size -= S->_size;
	return S; //���¹�ģ�����ط������������
}

template <typename T> //ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
int BinTree<T>::remove(BinNodePosi(T) x)
{ //assert: xΪ�������еĺϷ�λ��
	FromParentTo(*x) = NULL; //�ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent); //�������ȸ߶�
	int n = removeAt(x);
	_size -= n;
	return n; //ɾ������x�����¹�ģ������ɾ���ڵ�����
}

template <typename T> //ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
static int removeAt(BinNodePosi(T) x)
{ //assert: xΪ�������еĺϷ�λ��
	if (!x)
		return 0; //�ݹ��������
	int n = 1 + removeAt(x->lc) + removeAt(x->rc); //�ݹ��ͷ���������
	release(x->data);
	release(x);
	return n; //�ͷű�ժ���ڵ㣬������ɾ���ڵ�����
} //release()�����ͷŸ��ӽṹ�����㷨��ֱ�ӹ�ϵ������ʵ����������



#endif 