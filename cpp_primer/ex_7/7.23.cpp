#include <iostream>

using namespace std;

class screen
{
    using pos = string::size_type;
    private:
        string content;
        pos curr = 0;
        pos heigh = 0, width = 0;

    public:
        screen() = default;
        screen(char c, pos curr, pos h, pos w) :
            content(h * w, c), curr(curr), heigh(h), width(w) {}

        char get () const ;
        char get(pos h, pos w) const;

        screen& move(pos h, pos w);

        screen& set(pos h, pos w, char c);
        screen& set(char c);

        screen& display(ostream& os);
        const screen& display(ostream& os) const;

        void do_display(ostream& os) const;
};
void screen::do_display(ostream& os) const
{
    unsigned i, k;
    for(i = 0; i < heigh; i++)
    {
        for(k = 0; k < width; k++)
        {
            cout << content[k + i * width];
        }
        cout << endl;
    }
}

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
    screen ex('x', 2, 5, 5);
    ex.set('q');
    ex.do_display(cout);
    return 0;
}
