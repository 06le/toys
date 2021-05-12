/*This program contains 15.15,15.16,15.17 exercises*/
#include <iostream>

using namespace std;

class Quote
{
public:
    Quote() = default;
    Quote(const string& s, double p) : bookNO(s), price(p) {}

    string isbn() const
    {
        return bookNO;
    }
    virtual double net_price(size_t n) const
    {
        return n * price;
    }
    virtual void debug() const
    {
        cout << "bookNO: " << bookNO << " " << "price: " << price << '\n';
    }

    virtual ~Quote() = default;

protected:
    string bookNO;
    double price = 0;
};

class Disc_quote : public Quote
{
public:
    Disc_quote() = default;
    Disc_quote(const string& s, double p, size_t q, double d) : Quote(s, p), quantity(q), discount(d) {}

    double net_price(size_t n) const = 0;

protected:
    size_t quantity = 0;
    double discount = 0;
};



class bulk_quote : public Disc_quote
{
public:
    bulk_quote() = default;
    bulk_quote(const string& s, double p, size_t q, double d) : Disc_quote(s, p, q, d) {}

    double net_price(size_t n) const override;
    void debug() const override;
};

class limite_quote : public Disc_quote
{
public:
    limite_quote() = default;
    limite_quote(const string& s, double p, size_t q, double d, size_t m) : Disc_quote(s, p, q, d), max_qty(m) {}

    double net_price(size_t n) const override;

protected:
    size_t max_qty = 0;
};
/**********************************************************************************/
double print_total(const Quote& q, size_t n)
{
    double ret = q.net_price(n);
    cout << q.isbn() << "×Ü¹²" << ret << "Ôª" << '\n';
    return ret;
}

double bulk_quote::net_price(size_t n) const
{
    if(n >= quantity)
        return n * discount * price;
    else
        return n * price;
}

void bulk_quote::debug() const
{
    cout << "bookNO: " << bookNO << " " << "price: " << price
         << "quantity: " << quantity << "discount: " << discount
         << '\n';
}

double limite_quote::net_price(size_t n) const
{
    if(n > max_qty)
        return max_qty * discount * price + (n - max_qty) * price;
    else if(n >= quantity) return n * discount * price;
    else return n * price;
}

int main()
{

    return 0;
}
