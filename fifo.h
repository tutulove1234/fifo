/*
 *
 *
 */

// 思考一下，一个读者一个写者的时候，同一时刻，只会有一个人写，只会有一个人读
// 如果只用一个 size 作为控制，不妥，故读和写时，应该分别操作两个变量才能保证不会并发
// 所以，这里用两个变量，start ， end 分别表示队列的头尾

#include <iostream>
#include <cstdlib>

#include <unistd.h>

#define is_power_of_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))

class Fifo {
	public:
		Fifo(int size) : size_(size) {
			// 这里要2的倍数对齐, 8字节对齐
			buf_ = new (std::nothrow) char[size] ;
			if ( nullptr == buf_) {
				::exit(-1) ;
			}
		} ;
		~Fifo() {
			if ( nullptr != buf_) {
				delete []buf_ ;	
			}
		}
		int push(const char* data, const int data_size) noexcept ; 
		int pop(char * const data, const int data_size) noexcept ;
	private:
		Fifo(const Fifo&) = delete ;
		Fifo(const Fifo&&) = delete ;
		Fifo& operator=(const Fifo&) = delete ;
		Fifo& operator=(const Fifo&&) = delete ;
	private:
		int size_ ;
		int in_ ;
		int out_ ;
		char* buf_ ;
} ;

int Fifo::push(const char* data, const int data_size) noexcept {
	// 这里要读取 size_ 并且要修改，所以这里需要
}

int Fifo::pop(char * const data, const int data_size) noexcept {

}
