#ifndef LIBNDGPP_NETWORK_BYTE_ORDER_OPS_HPP
#define LIBNDGPP_NETWORK_BYTE_ORDER_OPS_HPP

namespace ndgpp
{
    template <class T>
    class network_byte_order;

    template <class T>
    bool operator== (const network_byte_order<T>, const network_byte_order<T>);

    template <class T>
    bool operator!= (const network_byte_order<T>, const network_byte_order<T>);

    template <class T>
    bool operator< (const network_byte_order<T>, const network_byte_order<T>);

    template <class T>
    bool operator> (const network_byte_order<T>, const network_byte_order<T>);

    template <class T>
    bool operator<= (const network_byte_order<T>, const network_byte_order<T>);

    template <class T>
    bool operator>= (const network_byte_order<T>, const network_byte_order<T>);
}

#endif
