/*
 ********************************************************
 * Author       : windleaves 
 * Create Time  : 2020-08-09 17:21
 * Email        : windleaves@xiyoulinux.org
 * Filename     : main.cpp
 * Description  : 
 * Copyleft by windleaves, all wrongs reserved
 ********************************************************
 */

#include <iostream>

#include "fifo.h"

int main() {
	Fifo q(1024) ;

	for( int i = 0; i < 1024/4 ; i ++ ) {
		if ( 0 == q.push((char*)&i , sizeof(i)) ) {
			std::cerr << "Error, buffer full" << std::endl ;
		}
	}

	int tmp = 2 ;
	if ( 0 == q.push((char*)&tmp , sizeof(tmp)) ) {
		std::cerr << "Error, buffer full" << std::endl ;
	}

	for( int i = 0 ; i < 1024/4 ; i++ ) {
		q.pop((char*)&tmp, sizeof(tmp)) ;
		std::cout << tmp << " " ;
	}
	if ( 0 == q.pop((char*)&tmp, sizeof(tmp))) {
		std::cerr << "Error, buffer is empty" << std::endl ;
	}

	std::cout << std::endl ;

	return 0;
}

