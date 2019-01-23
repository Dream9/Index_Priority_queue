#pragma once
#ifndef _INDEX_QRIORITY_QUEUE_H_
#define _INDEX_QRIORITY_QUEUE_H_
#include<vector>
#include<exception>
#include<string>
#include"source.h"
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
public:
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
	typedef typename Sequence::const_reference const_reference_type;
	typedef typename Sequence::const_iterator const_iterator;
	typedef typename Sequence::const_iterator iterator;

protected:
	/*用三个容器存储的目的在于，索引优先队列，首先有一个heap用于log级别访问最值
	**其次，由于heap会改变序列位置，故需要另一个用于由外部控制交互
	**最后，这两个数据通过index建立联系，方便调整heap
	*/
	Sequence heap;         //底层容器
	Sequence data;         //数据
	Sequence index;        //数据在底层容器中的索引
	Compare comp;          //仿函数
public:
	/*构造*/
	Index_priority_queue() {};
	explicit Index_priority_queue(const Compare& x) :comp(x) {};   //利用仿函数对象显式初始化
	/*使用迭代器初始化*/
	template<class InputIterator>
	Index_priority_queue(InputIterator first, InputIterator second) :data(first, second) {
		heap.resize(data.size());
		index.resize(data.size());
		make_heap();//初次构建堆结构
	}
	template<class InputIterator>
	Index_priority_queue(InputIterator first, InputIterator second, const Compare& x) : data(first, second), comp(x) {
		heap.resize(data.size());
		index.resize(data.size());
		make_heap();//初次构建堆结构
	}
	/*转一下接口*/
	size_type size() const { return heap.size(); }
	bool empty() const { return heap.empty(); }
	const_iterator cbegin() const { return data.cbegin(); }
	const_iterator cend() const { return data.cend(); }
	iterator begin() const { return cbegin(); }
	iterator end() const { return cend(); }

	~Index_priority_queue() {};//destroy 以及deallocate
protected:
	void make_heap() {
		/*初始化堆结构*/
		size_type len = this->size();
		size_type iter = 0;
		for (; iter < len; ++iter) {
			heap[iter] = iter;
			index[iter] = iter;
			swim(iter);
		}
	}

	/*上浮调整*/
	void swim(size_type pos) {
		while (pos > 0 && comp(data[heap[pos]], data[heap[(pos - 1) >> 1]])) {
			//注意这里的结构0元素并没有被舍弃
			size_type tmp = (pos - 1) >> 1;
			swap(pos, tmp);
			pos = tmp;
		}
	}

	/*下沉调整*/
	void sink(size_type pos){
		size_type len = this->size();
		while (pos < len) {
			size_type right = (pos + 1) << 1;
			size_type left = right - 1;
			if (left >= len)
				break;
			if (right >= len)
				right = left;
			left = comp(data[heap[right]], data[heap[left]]) ? right : left;//less时取小者，greater时取大者
			if (comp(data[heap[pos]], data[heap[left]]))//停止下沉
				break;
			swap(pos, left);
			pos = left;
		}
	}

	/*交换*/
	void swap(size_type first, size_type second) {
		/*交换位置，对于索引优先队列而言，要多维护两个容器*/
		//把交换的请求委托给底层
		std::swap(heap[first], heap[second]);
		////////注意此时已经换过来了，看起来比较绕
		index[heap[first]] = first;
		index[heap[second]] = second;
	}

public:
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
		//this->sink(0);
		/*如何要删除操作的话，效率是很低的*/
		//data.erase(data.begin()+heap[len - 1]);
		//index.erase(index.begin()+heap[len - 1]);
		//heap.pop_back();//有问题
		/*主要是因为，data去除一个元素之后，heap原本记录的索引就可能失效，*/
		//因此最终放弃了erase的方法
		index[heap[len - 1]] = T(NULL-1);
		heap.pop_back();
		this->sink(0);
	}

	/*作为queue的push实现*/
	void push(const value_type&x) {
		//有效载荷低于50%时，回收一次空间
		if ((data.size() >> 1) > this->size())
			collect();
		//加入数据并重排
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

	/*通过索引查看值*/
	const_reference_type operator[](const size_type& pos) const {
		return data[pos];/////////注意const
	}

	/*通过索引修改值，并调整堆（两个方向上）*/
	bool replace(const size_type& pos, value_type val) {
		if (pos > data.size())
			throw Index_priority_queue_error("Out of Range");
		data[pos] = val;
		if (index[pos] != T(NULL - 1)) {
			/*此时，value是堆中的一员，并且存在两种调整可能，上浮或者下沉*/
			sink(index[pos]);
			swim(index[pos]);
			return true;
		}
		else {
			return false;//表示修改的值是已经废弃的值
		}
	}

	/*空间回收*/
	void collect() {
		//因为在弹出时仅在heap中pop_back了多余元素，当超过一定阈值时，需要调整底层的空间
		//也可以根据需要，由用户手动回收
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
		//作为局部变量的_new_data将会函数结束时释放掉
	}
};

#endif

