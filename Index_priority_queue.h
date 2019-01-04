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
	Index_priority_queue(InputIterator first,InputIterator second):data(first,second),heap(),index(){
		;//等待添加，初次构建堆结构
	}
	/*转一下接口*/
	size_type size()const { return data.size(); }
	bool empty()const { return data.empty(); }

	~Index_priority_queue() {};//destroy 以及deallocate
private:
	void make_heap() {
		/*初始化堆结构*/
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

