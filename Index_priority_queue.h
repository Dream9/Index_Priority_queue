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

/*
*���������ʾ����
*data:7,8,1  heap:2,0,1��Ԫ����data�е�λ�ã�   index:1,2,0  ����heap�е�λ�ã�
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
	Index_priority_queue(InputIterator first, InputIterator second) :data(first, second), heap(), index() {
		;//�ȴ���ӣ����ι����ѽṹ
	}
	/*תһ�½ӿ�*/
	size_type size()const { return data.size(); }
	bool empty()const { return data.empty(); }

	~Index_priority_queue() {};//destroy �Լ�deallocate
private:
	void make_heap() {
		/*��ʼ���ѽṹ*/
		size_type len = this->size();
		size_type iter = 0;
		for (; iter < len; ++iter) {
			heap[iter] = iter;
			index[iter] = iter;
			sink(pos);
		}
	}

	void swim(size_type pos) {
		while (pos > 0 && comp(data[heap[pos]], data[heap[(pos - 1) << 1]])) {
			//ע������Ľṹ0Ԫ�ز�û�б�����
			tmp = (pos - 1) << 1;
			swap(pos, tmp);
			pos = tmp;
		}
	}

	void sink(size_type pos){
		size_type len = this->size();
		while (pos < len) {
			size_type right = (pos + 1) << 1;
			size_type left = right - 1;
			left = comp(data[heap[right]], data[heap[left]]) ? right : left;//ȡ��С��
			if (comp(data[heap[pos]], data[heap[left]]))//ֹͣ�³�
				break;
			swap(pos, left);
			pos = left;
		}
	}

public:
	void swap(size_type first, size_type second) {
		/*����λ�ã������������ȶ��ж��ԣ�Ҫ��ά����������*/
		//�ѽ���������ί�и��ײ�
		heap.swap(first, second);
		////////ע���ʱ�Ѿ��������ˣ��������Ƚ���
		index[heap[first]] = first;
		index[heap[second]] = second;
	}

	value_type top() {
		//������Ԫ��
		if (empty())
			throw Index_priority_queue_error("Without data!");
		return data[heap[0]];
	}

	void pop() {
		//������Ԫ��
		if (empty())
			throw Index_priority_queue_error("Without data!");
		size_type len = this->size();
		swap(0, len-1);
		this->sink(0);
		/*���Ҫɾ�������Ļ���Ч���Ǻܵ͵�*/
		data.erase(heap[len - 1]);
		index.erase(heap[len - 1]);
		heap.pop_back();
	}
};






#endif

