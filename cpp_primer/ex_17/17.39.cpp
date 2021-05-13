#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	string location("E:/code/c++/cppprimerTest.txt");
	fstream inOut(location, fstream::ate | fstream::in | fstream::out); //每次打开文件就定位到文件尾，对文件进行读和写操作

	if (!inOut)
	{
		cerr << "Unable to open file!" << endl;
		return EXIT_FAILURE;
	}
	
	streampos end_mark = inOut.tellg();	//保存文件尾位置
	inOut.seekg(0, fstream::end);		//定位到文件末尾
	inOut << "\n";						//在文件末尾加上换行符
	inOut.seekg(0, fstream::beg);		
	size_t cnt = 0;						//字节数累加器
	string line;						//保存输入中的每行


	while (inOut && inOut.tellg() != end_mark && getline(inOut, line))
	{
		cnt += line.size() + 1;	//计算line的长度， +1表示包括换行符
		auto mark = inOut.tellg();
		inOut.seekg(0, fstream::end);
		inOut << cnt;
		if (mark != end_mark)
		{
			inOut << " ";
		}
		inOut.seekg(mark);
	}
	
	//inOut.seekg(0, fstream::end); 
	//inOut << "\n";	
	return 0;
}