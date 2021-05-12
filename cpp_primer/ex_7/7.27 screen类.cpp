#include <iostream>
#include <vector>
#include <string>
using namespace std;

class screen
{
    using pos = string::size_type;
    private:
        string content;     //屏幕内容
        pos curr = 0;   //光标位置
        pos high = 0, width = 0;

    public:
        screen() = default;
        screen(char c, pos curr, pos h, pos w) :
            content(h * w, c), curr(curr), high(h), width(w) {}

        char get () const ;
        char get(pos h, pos w) const;

        screen& move(pos h, pos w);

        screen& set(pos h, pos w, char c);
        screen& set(char c);

        screen& display(ostream& os);
        const screen& display(ostream& os) const;

        void do_display(ostream& os) const {os << content;}
};

class window_mgr
{
    private:
        vector<screen> screens;
};

screen& screen::display(ostream& os)
{
    do_display(os);
    return *this;
}
const screen& screen::display(ostream& os) const
{
    do_display(os);
    return *this;
}

char screen::get () const
{
    return content[curr];
}

char screen::get(pos h, pos w) const
{
    pos row = h * width;
    return content[row + w];
}

screen& screen::move(pos h, pos w)
{
    pos row = h * width;
    curr = row + w;
    return *this;
}

screen& screen::set(pos h, pos w, char c)
{
    pos row = h * width;
    row += w;
    content[row] = c;
    return *this;
}

screen& screen::set(char c)
{
    content[curr] = c;
    return *this;
}





int main()
{
    screen ex;
    ex.set('Q');

    return 0;
}
