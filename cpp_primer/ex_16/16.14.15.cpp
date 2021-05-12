#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
using pos = string::size_type;

template <unsigned h, unsigned w>
class screen
{
    public:
        friend ostream& operator<<(ostream& os, const screen& c)
        {
            unsigned i, k;
            for(i = 0; i < c.height; i++)
            {
                for(k = 0; k < c.width; k++)
                {
                    cout << c.content[k + i * c.width];
                }
                cout << endl;
            }
            return os;
        }

        friend istream& operator>>(istream& is, screen& c)
        {
            string temp;
            is >> temp;
            for(auto ch : temp)
            {
                c.set(ch);
            }
            return is;
        }

        screen() = default;
        screen(char c, pos curr) :
            content(h * w, c), curr(curr) {}

        char get () const ;
        char get(pos he, pos wd) const;

        screen<h, w>& move(pos he, pos wd);

        screen<h, w>& set(pos he, pos wd, char c);
        screen<h, w>& set(char c);

    private:
        string content;
        pos curr = 0;
        pos height = h;
        pos width = w;
};

template <unsigned h, unsigned w>
char screen<h, w>::get () const       //template <unsigned h, unsigned w>
{
    return content[curr];
}

template <unsigned h, unsigned w>
char screen<h, w>::get(pos he, pos wd) const
{
    pos row = he * width;
    return content[row + wd];
}

template <unsigned h, unsigned w>
screen<h, w>& screen<h, w>::move(pos he, pos wd)
{
    pos row = he * width;
    curr = row + wd;
    return *this;
}

template <unsigned h, unsigned w>
screen<h, w>& screen<h, w>::set(pos he, pos wd, char c)
{
    content[he * width + wd] = c;
    return *this;
}

template <unsigned h, unsigned w>
screen<h, w>& screen<h, w>::set(char c)
{
    content[curr] = c;
    return *this;
}

int main()
{
    screen<5, 5> ex('x', 2);
    ex.set('q');
    cout << ex;
    return 0;
}
