#pragma once
#ifndef _INDEX_QRIORITY_QUEUE_H_
#define _INDEX_QRIORITY_QUEUE_H_
#include<vector>
#include<exception>
#include<string>
#include"source.h"
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
public:
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
	typedef typename Sequence::const_reference const_reference_type;
	typedef typename Sequence::const_iterator const_iterator;
	typedef typename Sequence::const_iterator iterator;

protected:
	/*�����������洢��Ŀ�����ڣ��������ȶ��У�������һ��heap����log���������ֵ
	**��Σ�����heap��ı�����λ�ã�����Ҫ��һ���������ⲿ���ƽ���
	**�������������ͨ��index������ϵ���������heap
	*/
	Sequence heap;         //�ײ�����
	Sequence data;         //����
	Sequence index;        //�����ڵײ������е�����
	Compare comp;          //�º���
public:
	/*����*/
	Index_priority_queue() {};
	explicit Index_priority_queue(const Compare& x) :comp(x) {};   //���÷º���������ʽ��ʼ��
	/*ʹ�õ�������ʼ��*/
	template<class InputIterator>
	Index_priority_queue(InputIterator first, InputIterator second) :data(first, second) {
		heap.resize(data.size());
		index.resize(data.size());
		make_heap();//���ι����ѽṹ
	}
	template<class InputIterator>
	Index_priority_queue(InputIterator first, InputIterator second, const Compare& x) : data(first, second), comp(x) {
		heap.resize(data.size());
		index.resize(data.size());
		make_heap();//���ι����ѽṹ
	}
	/*תһ�½ӿ�*/
	size_type size() const { return heap.size(); }
	bool empty() const { return heap.empty(); }
	const_iterator cbegin() const { return data.cbegin(); }
	const_iterator cend() const { return data.cend(); }
	iterator begin() const { return cbegin(); }
	iterator end() const { return cend(); }

	~Index_priority_queue() {};//destroy �Լ�deallocate
protected:
	void make_heap() {
		/*��ʼ���ѽṹ*/
		size_type len = this->size();
		size_type iter = 0;
		for (; iter < len; ++iter) {
			heap[iter] = iter;
			index[iter] = iter;
			swim(iter);
		}
	}

	/*�ϸ�����*/
	void swim(size_type pos) {
		while (pos > 0 && comp(data[heap[pos]], data[heap[(pos - 1) >> 1]])) {
			//ע������Ľṹ0Ԫ�ز�û�б�����
			size_type tmp = (pos - 1) >> 1;
			swap(pos, tmp);
			pos = tmp;
		}
	}

	/*�³�����*/
	void sink(size_type pos){
		size_type len = this->size();
		while (pos < len) {
			size_type right = (pos + 1) << 1;
			size_type left = right - 1;
			if (left >= len)
				break;
			if (right >= len)
				right = left;
			left = comp(data[heap[right]], data[heap[left]]) ? right : left;//lessʱȡС�ߣ�greaterʱȡ����
			if (comp(data[heap[pos]], data[heap[left]]))//ֹͣ�³�
				break;
			swap(pos, left);
			pos = left;
		}
	}

	/*����*/
	void swap(size_type first, size_type second) {
		/*����λ�ã������������ȶ��ж��ԣ�Ҫ��ά����������*/
		//�ѽ���������ί�и��ײ�
		std::swap(heap[first], heap[second]);
		////////ע���ʱ�Ѿ��������ˣ��������Ƚ���
		index[heap[first]] = first;
		index[heap[second]] = second;
	}

public:
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
		//this->sink(0);
		/*���Ҫɾ�������Ļ���Ч���Ǻܵ͵�*/
		//data.erase(data.begin()+heap[len - 1]);
		//index.erase(index.begin()+heap[len - 1]);
		//heap.pop_back();//������
		/*��Ҫ����Ϊ��dataȥ��һ��Ԫ��֮��heapԭ����¼�������Ϳ���ʧЧ��*/
		//������շ�����erase�ķ���
		index[heap[len - 1]] = T(NULL-1);
		heap.pop_back();
		this->sink(0);
	}

	/*��Ϊqueue��pushʵ��*/
	void push(const value_type&x) {
		//��Ч�غɵ���50%ʱ������һ�οռ�
		if ((data.size() >> 1) > this->size())
			collect();
		//�������ݲ�����
		__STL_TRY
		data.push_back(x);
		heap.push_back(std::distance(data.begin(), data.last()));
		index.push_back(heap.size());
		__STL_TRY_END
		__CATCH_ALL{
			throw Index_priority_queue_error(to_string(__LINE__)+"Error happening when push data");
		};
		swim(heap.size());
	}

	/*ͨ�������鿴ֵ*/
	const_reference_type operator[](const size_type& pos) const {
		return data[pos];/////////ע��const
	}

	/*ͨ�������޸�ֵ���������ѣ����������ϣ�*/
	bool replace(const size_type& pos, value_type val) {
		if (pos > data.size())
			throw Index_priority_queue_error("Out of Range");
		data[pos] = val;
		if (index[pos] != T(NULL - 1)) {
			/*��ʱ��value�Ƕ��е�һԱ�����Ҵ������ֵ������ܣ��ϸ������³�*/
			sink(index[pos]);
			swim(index[pos]);
			return true;
		}
		else {
			return false;//��ʾ�޸ĵ�ֵ���Ѿ�������ֵ
		}
	}

	/*�ռ����*/
	void collect() {
		//��Ϊ�ڵ���ʱ����heap��pop_back�˶���Ԫ�أ�������һ����ֵʱ����Ҫ�����ײ�Ŀռ�
		//Ҳ���Ը�����Ҫ�����û��ֶ�����
		size_type len = data.size();
		Sequence _new_data;
		Sequence _new_index;

		size_type pos = 0;
		size_type accumulation = 0;
		while(pos<len){
			if (index[pos] == T(NULL - 1)) {
				++pos;
				continue;
			}
			_new_data.push_back(data[pos]);
			_new_index.push_back(index[pos]);
			heap[index[pos]] = accumulation++;	
			++pos;
		}
		data.swap(_new_data);
		index.swap(_new_index);
		//��Ϊ�ֲ�������_new_data���ắ������ʱ�ͷŵ�
	}
};

#endif

