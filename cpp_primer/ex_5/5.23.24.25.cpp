

#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    /*5.23*/
    //cout << "please enter two numbers: " << "\n";
    //
    //double num1, num2;
    //cin >> num1 >> num2;

    //cout << "num1 / num2 = ";
    //cout << num1 / num2;

    /*5.24*/
    //cout << "please enter two numbers: " << "\n";

    //double num1, num2;
    //cin >> num1 >> num2;

    //if (num2 == 0)
    //    throw runtime_error("The divisor cannot be 0");

    //cout << "num1 / num2 = ";
    //cout << num1 / num2;

    /*5.25*/
    while (true)
    {
        try
        {
            cout << "please enter two numbers: " << "\n";

            double num1, num2;
            cin >> num1 >> num2;

            if (num2 == 0)
                throw runtime_error("The divisor cannot be 0");

            cout << "num1 / num2 = ";
            cout << num1 / num2;
        }
        catch (runtime_error err)  //const std::exception&
        {
            cout << err.what() << '\n' << "Try again? Enter Y or N" << endl;
            char c;
            cin >> c;
            if (!c || c == 'N')
                break;
        }
    }



    


}

