#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;


class String{
public:
	String()
		:_str(new char[16]) // 16加上 '\0'
		, _size(0)
		, _capacity(0){
		_str[_size] = '\0';
		_capacity = 15;
	}


	// 用一个字符串创建一个字符串对象
	/*
	String(char* str)
	:_str(str)
	, _size(strlen(str))
	, _capacity(strlen(str)){

	}
	*/
	String(const char* str){
		_size = strlen(str);
		// 自身拥有一份资源
		// 此时就是释放堆上的空间，与代码段就无关系
		_str = new char[_size + 1];
		strcpy(_str, str);
		_capacity = _size;
		cout << "String(const char* str)" << endl;
	}
	const char* c_str(){
		return _str;
	}

	String(const String& str)
		:_str(new char[str._capacity + 1])
		, _size(str._size)
		, _capacity(str._capacity){
		// 深拷贝：资源拷贝
		strcpy(_str, str._str);
		// 申请一块空间：存放各自的内容
		cout << "String(const String&)" << endl;
	}
	// 拷贝构造：现代写法
	// 拷贝构造的现代写法:代码复用
	/*
	String(const String& str)
		// 此处赋的初始值就用来放在 tmp 中释放资源
		// 不赋值则为随机值，将内容交换过来占为己有
		:_str(nullptr)
		, _size(0)
		, _capacity(0)
	{
		// 调用构造函数
		// 创建临时对象：拷贝新的内容
		String tmp(str._str);
		Swap(tmp); 
	}
	*/
	// 两个对象的资源不变：只是相对指向发生了改变
	void Swap(String& str){
		swap(_str, str._str);
		swap(_size, str._size);
		swap(_capacity, str._capacity);
	}
	// 赋值运算符
	String& operator=(const String& str){
		// 自身给自身赋值：就直接返回自身
		if (this != &str){
			// 开空间
			char* tmp = new char[str._capacity + 1];
			// 内容拷贝
			strcpy(tmp, str._str);
			// 释放原有空间
			delete[] _str;

			_str = tmp;
			_size = str._size;
			_capacity = str._capacity;
		}
		return *this;
	}
	~String() // 常量字符串的指针赋值给 _str , 常量字符串在代码段不可被释放
	{
		if (_str){
			// delete 只能用于堆上的空间释放
			delete[] _str;
			_size = _capacity = 0;
			_str = nullptr;
		}
		cout << "~String()" << endl;
	}
	// 深拷贝交换：效率低
	// 通用的交换函数：会创建临时对象，效率非常低
	/*
	void Swap(String& str){
		// 拷贝构造
		string tmp = str;
		// 赋值运算符
		str = *this;
		// 赋值运算符
		*this = tmp;
	}
	*/
	// 赋值运算符：现代写法，代码复用
	//				拷贝构造复用：创建临时对象
	//					传参的时候进行拷贝构造
	/*
	String& operator=(const String& str){
		String tmp(str);
		Swap(tmp);
		return *this;
	}
	*/
	/*
	String& operator=(String str){
		// 交换局部对象的资源
		Swap(str);
		// 调用局部对象的析构函数，释放资源
		return *this;
	}
	*/
	/*
	String& operator=(String str){
		Swap(str);
		return *this;
	}
	*/

	// 现代拷贝构造
	/*
	String(const String& str){
		String tmp(str._str);
		Swap(tmp);
	}
	*/
	// 完成了各个资源所指向空间的交换
	/*
	void Swap( String& tmp){
		swap(_str,tmp._str);
		swap(tmp._size, _size);
		swap(tmp._capacity, _capacity);
	}
	*/

	//操作相关函数
	
	void pushBack(const char& ch){
		// 检查容量
		if (_size == _capacity){
			size_t newC = _capacity==0?15:2*_capacity;
			reserve(newC);
		}
		// 尾插
		_str[_size] = ch;
		// 更新
		++_size;
		// 注意结尾 加上 '\0'
		_str[_size] = '\0';
	}

	void reserve(size_t n){
		if (n >= _capacity){
			// 开空间 +1 --> 存放'\0'
			char* tmp = new char[n + 1];
			// 拷贝
			strcpy(tmp, _str);
			//释放原有空间
			delete[] _str;
			_str = tmp;
			// 更新容量
			_capacity = n;
		}
	}

	// 遍历 String 内容：c_str() 调用
	// 迭代器遍历每个字符
	// 容器中的元素访问机制：使用方式类似于指针
	// 迭代器：一种访问容器元素机制
	// 体现封装的特性，不需要关注容器的实现细节，就可以直接访问（可读可写）元素
	// 使用方式类似于指针
	// 类似于指针的操作：1、支持解引用：获取指向位置的内容
	//					 2、位置移动：（指针就是一个迭代器），指向其他位置
	// string迭代器：通过指针实现

	typedef char* iterator;
	typedef const char* const_iterator;

	// 可读可写的对象
	iterator begin(){
		// 第一个元素的位置
		return _str;
	}
	iterator end(){
		// 最后一个元素的下一个位置
		return _str + _size;
	}

	// 只读迭代器：保护的作用，只可访问，不可修改
	const_iterator begin() const{
		// 第一个元素位置
		return _str;
	}
	const_iterator end() const{
		return _str + _size;
	}

	char& operator[](size_t pos){
		if (pos < _size)
			return _str[pos];
	}

	const char& operator[](size_t pos) const{
		if (pos < _size)
			return _str[pos];
	}

	size_t size() const{
		return _size;
	}

	void Append(const char* str){
		int len = strlen(str);
		// 检查容量
		if (_size + len > _capacity){
			reserve(_size + len);
		}
		// 尾插
		strcpy(_str + _size, str);
		// 修改 size
		_size += len;
	}

	// += 操作
	String& operator+=(const char* str){
		Append(str);
		return *this;
	}

	void insert(size_t pos, const char& ch){
		// 判断是否为有效区间
		if (pos > _size) return;// 顺序表，元素需要连续存放
		// 检查容量
		if (_size == _capacity){
			size_t newC = _capacity == 0 ? 15 : 2 * _capacity;
			reserve(newC);
		}
		// 移动元素[pos,_size]：从后向前移动
		// 首先移动最右端的字符，防止覆盖
		// 无符号的错误插入位置 0 的错误
//		size_t end = _size;

		// 解决头插操作：无符号的-1情况，变为正数
		// 非法访问的情况
		// end >= pos : 当pos==0时，可能会产生死循环，且可能产生访问越界
		size_t end = _size+1;
		while (end > pos){
			// 首先移动 '\0' 
			_str[end] = _str[end-1];
			--end;
		}

		// 插入
		_str[pos] = ch;
		++_size;// 此处就不需要+'\0'
	}

	void insert(size_t pos, const char* str){
		if (pos > _size)
			return;
		int len = strlen(str);
		if (_size + len > _capacity){
			reserve(_size + len);
		}
		// 移动元素
		// [pos,_size)
		size_t end = _size + len;
		while (end > pos + len - 1){
			_str[end] = _str[end - len];
			--end;
		}

		// 插入
		for (int i = 0; i < len; ++i){
			_str[i + pos] = str[i];
		}

		_size += len;
	}
private:
	char* _str;
	size_t _size;
	size_t _capacity;
};


void printString(const String& str){
	String::const_iterator cit = str.begin();
	while (cit != str.end()){
		cout << *cit << " ";
		++cit;
	}
	cout << endl;
}

void t4(){
	String s;
	s.Append("zhenghong");
	cout << s.c_str() << endl;
	s.insert(0, '1');
	//s.Append("is a hanhan!");
	cout << s.c_str() << endl;
	//s += "kaizaizheer";
	printString(s);

	s.insert(2, "wszdatm");
	printString(s);
}
void test3(){
	String s = "";
	s.pushBack('a');
	cout << s.c_str() << endl;
	s.pushBack('b');
	cout << s.c_str() << endl;
	s.pushBack('c');
	cout << s.c_str() << endl;

	// 迭代器的遍历
	String::iterator it = s.begin();
	while (it != s.end()){
		cout << *it << " "; it++;
	}
	cout << endl;

	// begin迭代器获取起始接口
	// end迭代器获取结束接口
	for (char& ch : s){
		cout << ch << " ";
		ch = '1';
	}
	cout << endl;

	for (const char& ch : s){
		cout << ch << " ";
	}
	cout << endl;

	for (char ch : s){
		ch = 1;// 修改局部变量
	}
	cout << endl;
	for (char ch : s){
		cout << ch << " ";
	}
	cout << endl;

	cout << "[]" << endl;
	for (int i = 0; i < s.size(); ++i){
		cout << s[i] << " ";
		s[i] = '0'; // [] 也可以实现写和读的操作
	}
	cout << endl;
	for (int i = 0; i < s.size(); ++i){
		cout << s[i] << " ";
	}
	cout << endl;
}
void test(){
	string s; // "" 0 15
	// 字符串打印
	// 打印的是解引用之后的内容：并不是打印的指针
	cout << s.c_str() << endl;
}

void test2(){
	String s("123");
	cout << s.c_str() << endl;
	// 调用拷贝构造：浅拷贝，所有的对象就只有一份资源
	String copy = s;
	cout << copy.c_str() << endl;
	String copy2(s);
	cout << copy2.c_str() << endl;

	String s2("456");
	copy = s2;
	cout << s2.c_str() << endl;

	// 迭代器的遍历
	String::iterator it = s.begin();
	while (it != s.end()){
		cout << *it << " "; it++;
	}
	cout << endl;
}


int main(){
	t4();

	system("pause");
	return 0;
}

