#include <iostream>
//#define BOOST_MPL_LIMIT_STRING_SIZE 128
#include <boost/mpl/string.hpp>

template <unsigned N, char C>
struct cfill
{
    typedef typename boost::mpl::push_back<
        typename cfill<N-1, C>::type
            , boost::mpl::char_<C>
        >::type type;
};

template <char C>
struct cfill<0, C>
{
    typedef boost::mpl::string<> type;
};

template <unsigned N, char C>
struct carr
{
    typedef typename boost::mpl::c_str< typename cfill< N, C >::type >::type type;
};


int main (void)
{
    std::cout << carr<20, ':'>::type::value << std::endl;
    return 0;
}


