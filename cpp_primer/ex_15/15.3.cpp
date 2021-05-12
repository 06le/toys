/*This program contains 15.3，15.5,15.6,15.7,15.11exercises*/

#include <iostream>

using namespace std;

class Quote
{
public:
    Quote() = default;
    Quote(const string& s, double p) : bookNO(s), price(p) {}

    string isbn() const {return bookNO;}
    virtual double net_price(size_t n) const {return n * price;}
    virtual void debug() const {cout << "bookNO: " << bookNO << " " << "price: " << price << '\n';}

    virtual ~Quote() = default;

protected:
    string bookNO;
    double price = 0;
};

class bulk_quote : public Quote
{
public:
    bulk_quote() = default;
    bulk_quote(const string& s, double p, size_t m, double d) : Quote(s, p), min_qty(m), discount(d) {}

    double net_price(size_t n) const override;
    void debug() const override;

protected:
    size_t min_qty = 0;
    double discount = 1; //用小数表示折扣
};

class limite_quote : public bulk_quote
{
public:
    limite_quote() = default;
    limite_quote(const string& s, double p, size_t m, double d, size_t q) : bulk_quote(s, p, m, d), max_qty(q) {}

    virtual double net_price(size_t n) const override;

protected:
    size_t max_qty = 0;
};

double print_total(const Quote& q, size_t n)
{
    double ret = q.net_price(n);
    cout << q.isbn() << "总共" << ret << "元" << '\n';
    return ret;
}

double bulk_quote::net_price(size_t n) const
{
    if(n >= min_qty)
        return n * discount * price;
    else
        return n * price;
}

void bulk_quote::debug() const
{
    cout << "bookNO: " << bookNO << " " << "price: " << price
         << "min_qty: " << min_qty << "discount: " << discount
         << '\n';
}

double limite_quote::net_price(size_t n) const
{
    if(n > max_qty)
        return max_qty * discount * price + (n - max_qty) * price;
    else if(n >= min_qty) return n * discount * price;
    else return n * price;
}

int main()
{
    Quote a("example", 5);
    bulk_quote b("exa2", 5, 3, 0.5);
    print_total(a, 5);
    print_total(b, 5);
    a.debug();
    return 0;
}
