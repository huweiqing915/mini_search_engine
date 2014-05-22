/*************************************************************************
	> File Name: test_remove_page.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 19时21分38秒
 ************************************************************************/

//测试网页去重类RemoveRepeat
#include "RemoveRepeat.h"

using namespace std;

int main(int argc, char *argv[])
{
	RemoveRepeat rm;

	rm.build_document_vec();
	rm.remove_repeat_page();
	rm.write_to_file();
	
	return 0;
}
