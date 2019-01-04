#pragma once
#ifndef _INDEX_QRIORITY_QUEUE_H_
#define _INDEX_QRIORITY_QUEUE_H_
#include<vector>
#include<exception>
#include<string>
/*����stl��ֻʵ����priority_queue,ȱ���������ȶ��е�ʵ��
 *����Dijkstra�㷨����Prim�㷨����Ҫ�õ�
 *����ʵ������
 *�����Ͻ�����һ��adaptor
 */
/*������Ϣ*/
class Index_priority_queue_error :public std::runtime_error {
	Index_priority_queue_error(const std::string&info) :runtime_error(info) {}
};
/*����ʵ��*/
template<class T,class Sequence=std::vector<T>,
	class Compare=std::less<typename Sequence::value_type> >
class Index_priority_queue
{
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::difference_type difference_type;
	typedef typename Sequence::const_reference_type const_reference_type
	
protected:
	/*�����������洢��Ŀ�����ڣ��������ȶ��У�������һ��heap����log���������ֵ
	**��Σ�����heap��ı�����λ�ã�����Ҫ��һ���������ⲿ���ƽ���
	**�������������ͨ��index������ϵ���������heap
	*/
	Sequence heap;         //�ײ�����
	Sequence data;         //����
	Sequence index;        //�����ڵײ������е�����
	Compare comp;       //�º���
public:
	Index_priority_queue() : heap(), data(), index() {};
	explicit Index_priority_queue(const Cmpare& x) :heap(), data(), index(), comp(x) {};   //���÷º���������ʽ��ʼ��
    /*��������ʼ��*/
	template<class InputIterator>
	Index_priority_queue(InputIterator first,InputIterator second):data(first,second),heap(),index(){
		;//�ȴ���ӣ����ι����ѽṹ
	}
	/*תһ�½ӿ�*/
	size_type size()const { return data.size(); }
	bool empty()const { return data.empty(); }

	~Index_priority_queue() {};//destroy �Լ�deallocate
private:
	void make_heap() {
		/*��ʼ���ѽṹ*/
		size_type len=this->size();
		size_type iter = 0;
		for (; iter < len; ++iter)
			sink(pos);
	}

	void swim() {
		;
	}

	void sink(size_type pos)[
		;
	]
};






#endif

