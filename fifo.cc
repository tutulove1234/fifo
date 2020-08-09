/*
 *
 *
 */

// 思考一下，一个读者一个写者的时候，同一时刻，只会有一个人写，只会有一个人读
// 如果只用一个 size 作为控制，不妥，故读和写时，应该分别操作两个变量才能保证不会并发
// 所以，这里用两个变量，start ， end 分别表示队列的头尾

#include <iostream>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#include "fifo.h"


#define is_power_of_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))

void Fifo::copy_in(const char* src, unsigned int len) noexcept{
	// 鉴于in只有本线程会改动
	unsigned int size = mask_ +1 ;
	unsigned int l ;
	unsigned int off = in_ ;
	// 这里就是为了保留size的剩余位
	// 因为in_ 持续递增
	off &= mask_ ;
	// 后半段可以copy的长度
	l = std::min(len, size - off) ;
	::memcpy(data_ + off, src, l) ;
	// 剩余的copy到前半段
	::memcpy(data_, src + l, len -l) ;

	// 确保当前对data_内容的修改在 in_ 的修改之前完成
	// 对于intel的处理器而言，store，store之间不会重新排序
	// memcpy的话可能使用了String操作进行优化，intel在sdm中写了String之后的store不会重排序到String之前
	__asm__ ("sfence":::"memory") ;
}

void Fifo::copy_out(char *dst, unsigned int len) noexcept {
	// 修改 out_ 这里 out_ 只有本线程修改
	unsigned int size = mask_ +1 ;
	unsigned int l ;
	unsigned int off = out_ ;

	// 这里就是为了保留size的剩余位
	// 因为out_ 持续递增
	off &= mask_ ;
	l = std::min(len, size-off) ;

	// copy 后半部分 
	::memcpy(dst, data_+off, l) ;
	::memcpy(dst+l, data_, len-l) ;

	__asm__ ("sfence":::"memory") ;
}

unsigned int Fifo::push(const char* src, unsigned int data_len) noexcept {
	unsigned int rest_len = unused() ;
	if (data_len > rest_len) {
		// 剩余大小不够的时候，直接返回
		return 0 ;
	}
	copy_in(src, data_len) ;
	in_ += data_len ;
	// 返回实际copy的数据大小
	return data_len ;
}

unsigned int Fifo::pop(char * dst, unsigned int data_size) noexcept {
	unsigned int l ;
	// buffer 剩余大小
	l = in_ - out_ ;

	if ( data_size > l ) {
		// buffer 剩余空间不够需要pop出去的空间时候
		return 0 ;
	}

	copy_out(dst, data_size) ;
	out_ += data_size ;
	return data_size ;
}
