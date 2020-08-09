// 思考一下，一个读者一个写者的时候，同一时刻，只会有一个人写，只会有一个人读
// 如果只用一个 size 作为控制，不妥，故读和写时，应该分别操作两个变量才能保证不会并发
// 所以，这里用两个变量，in_ ， out_ 分别表示队列的头尾

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#define is_power_of_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))

class Fifo {
	public:
		Fifo(unsigned int size): 
			in_(0),
			out_(0),
			mask_(0),
			data_(nullptr) 
	{
			// 这里要2的次幂对齐
			if (! is_power_of_2(size) ) {
				std::cerr << "size is not power of 2" << std::endl ;	
				::abort() ;
			}
			if ( size < 2 ) {
				std::cerr << "Size two small ..." << std::endl ;
				::abort() ;
			}
			data_ = new(std::nothrow) char[size] ;
			if ( nullptr == data_) {
				std::cerr << "Alloc failed, abort ..." << std::endl ;
				::abort() ;
			}
			// 判断是否已经超过size了
			// 那么mask_ +1 就是size了
			mask_ = size - 1 ;
		} 
		~Fifo() {
			if ( nullptr != data_) {
				delete []data_ ;	
			}
		}
		unsigned int push(const char* src, unsigned int data_size) noexcept ; 
		unsigned int pop(char * dst, unsigned int data_size) noexcept ;
		// For multi thread access
		void push_with_spin() noexcept {}
		void pop_with_spin() noexcept {}
	private:
		Fifo(const Fifo&) = delete ;
		Fifo(const Fifo&&) = delete ;
		Fifo& operator=(const Fifo&) = delete ;
		Fifo& operator=(const Fifo&&) = delete ;

		// 剩下的size
		unsigned int unused() {
			return (mask_ + 1) - (in_ - out_ ) ;
		}
		void copy_in(const char * src, unsigned int len) noexcept ;
		void copy_out(char *dst, unsigned int len) noexcept ;
	private:
		unsigned int in_ ;
		unsigned int out_ ;
		unsigned int mask_ ;
		char* data_ ;
} ;

