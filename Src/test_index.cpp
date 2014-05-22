/*************************************************************************
	> File Name: test_index.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 00时22分53秒
 ************************************************************************/

#include <iostream>
#include "InvertIndex.h"
#include "Config.h"
#include "MixSegment.hpp"

using namespace std;

int main()
{	
	InvertIndex ix;
	ix.build_invert_index();
	ix.write_index();
	ix.count_word_weight();
	ix.write_word_weight();
	return 0;
}
