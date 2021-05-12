#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <memory>
#include <string>

using namespace std;

int main()
{

		/*17.16  输入的单词只能是三个字母的字符串，多了都会显示输入错误*/
		string pattern("[^c]ie");
		regex r(pattern);
		smatch result;
		cout << "please enter a word: ";
		string word;
		while(cin >> word)
		{
			if(regex_search(word, result, r))
				cout << result.str() << endl;
			else
				cout << "input error!, please re-enter the word" << endl;
		}
		

		/*17.15*/
		string pattern("[^c]ie");
		pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
		regex r(pattern);
		smatch result;

		cout << "please enter a word: ";
		string word;
		while(cin >> word)
		{
			if(regex_search(word, result, r))
				cout << result.str() << endl;
			else
				cout << "input error!, please re-enter the word" << endl;
		}
		

		/* 17.14 */
		try
		{
			regex r("[[:alnum:]] + \\.(cpp|cxx|cc)$", regex::icase);
		}
		catch (regex_error e)
		{
			cout << e.what() << "\ncode: " << e.code() << endl;
		}
		

	return 0;
}
