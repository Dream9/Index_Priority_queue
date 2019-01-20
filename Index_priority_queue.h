#pragma once
#ifndef _INDEX_QRIORITY_QUEUE_H_
#define _INDEX_QRIORITY_QUEUE_H_
#include<vector>
#include<exception>
#include<string>
/*由于stl中只实现了priority_queue,缺少索引优先队列的实现
 *而且Dijkstra算法，和Prim算法都需要用到
 *单独实现如下
 *本质上讲，是一个adaptor
 */

/*
*三个数组的示例：
*data:7,8,1  heap:2,0,1（元素在data中的位置）   index:1,2,0  （在heap中的位置）
*/
/*错误信息*/
class Index_priority_queue_error :public std::runtime_error {
	Index_priority_queue_error(const std::string&info) :runtime_error(info) {}
};
/*核心实现*/
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
	/*用三个容器存储的目的在于，索引优先队列，首先有一个heap用于log级别访问最值
	**其次，由于heap会改变序列位置，故需要另一个用于由外部控制交互
	**最后，这两个数据通过index建立联系，方便调整heap
	*/
	Sequence heap;         //底层容器
	Sequence data;         //数据
	Sequence index;        //数据在底层容器中的索引
	Compare comp;       //仿函数
public:
	Index_priority_queue() : heap(), data(), index() {};
	explicit Index_priority_queue(const Cmpare& x) :heap(), data(), index(), comp(x) {};   //利用仿函数对象显式初始化
	/*迭代器初始化*/
	template<class InputIterator>
	Index_priority_queue(InputIterator first, InputIterator second) :data(first, second), heap(), index() {
		;//等待添加，初次构建堆结构
	}
	/*转一下接口*/
	size_type size()const { return data.size(); }
	bool empty()const { return data.empty(); }

	~Index_priority_queue() {};//destroy 以及deallocate
private:
	void make_heap() {
		/*初始化堆结构*/
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
			//注意这里的结构0元素并没有被舍弃
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
			left = comp(data[heap[right]], data[heap[left]]) ? right : left;//取较小者
			if (comp(data[heap[pos]], data[heap[left]]))//停止下沉
				break;
			swap(pos, left);
			pos = left;
		}
	}

public:
	void swap(size_type first, size_type second) {
		/*交换位置，对于索引优先队列而言，要多维护两个容器*/
		//把交换的请求委托给底层
		heap.swap(first, second);
		////////注意此时已经换过来了，看起来比较绕
		index[heap[first]] = first;
		index[heap[second]] = second;
	}

	value_type top() {
		//访问首元素
		if (empty())
			throw Index_priority_queue_error("Without data!");
		return data[heap[0]];
	}

	void pop() {
		//弹出首元素
		if (empty())
			throw Index_priority_queue_error("Without data!");
		size_type len = this->size();
		swap(0, len-1);
		this->sink(0);
		/*如何要删除操作的话，效率是很低的*/
		data.erase(heap[len - 1]);
		index.erase(heap[len - 1]);
		heap.pop_back();
	}
};






#endif

