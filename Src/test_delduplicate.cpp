/*************************************************************************
	> File Name: test_delduplicate.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 23时37分07秒
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include "Config.h"
#include "DelDuplicate.h"

using namespace std;

int main()
{
	DelDuplicate del;
	del.build_page_vector();
	del.delete_duplicate_page();
	del.write_to_file();
	return 0;
}

