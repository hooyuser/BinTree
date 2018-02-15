#pragma once

#define BinNodePosi(T) BinNode<T> * //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ��롰�����߶�Ϊ-1����Լ����ͳһ��
typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ


//�������ڵ�ģ����
template <typename T> class BinNode
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
		int h = 0, int l = 1, RBColor c = RB_RED) :s
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