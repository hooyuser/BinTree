#pragma once

#define BinNodePosi(T) BinNode<T> * //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ��롰�����߶�Ϊ-1����Լ����ͳһ��
typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ


//�������ڵ�ģ����
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


//�������ڵ�ģ����ʵ��
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


//������ģ����
template <typename T> 
class BinTree 
{ 
protected:
	int _size; //��ģ
	BinNodePosi(T) _root; //���ڵ�
	virtual int updateHeight(BinNodePosi(T) x); //���½ڵ�x�ĸ߶�
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
	void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //��α���
	template <typename VST> //������
	void travPre(VST& visit) { if (_root) _root->travPre(visit); } //�������
	template <typename VST> //������
	void travIn(VST& visit) { if (_root) _root->travIn(visit); } //�������
	template <typename VST> //������
	void travPost(VST& visit) { if (_root) _root->travPost(visit); } //�������

	bool operator< (BinTree<T> const& t) //�Ƚ������������в��䣩
	{
		return _root && t._root && lt(_root, t._root);
	}
	bool operator== (BinTree<T> const& t) //�е���
	{
		return _root && t._root && (_root == t._root);
	}
}; 

//������ģ����ʵ��
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