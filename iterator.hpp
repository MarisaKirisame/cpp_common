#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
namespace misc
{
    template< typename IT >
    struct range_container_proxy
    {
        IT b, e;
        IT begin( ) const { return b; }
        IT end( ) const { return e; }
        range_container_proxy( IT b, IT e ) : b( b ), e( e ) { }
    };

    template< typename IT >
    range_container_proxy< IT > make_range_container_proxy( IT begin, IT end )
    { return range_container_proxy< IT >( begin, end ); }

    template< typename IT >
    struct iterator_iterator
    {
        IT current, end;
        using IIT = decltype( std::declval< IT >( )->begin( ) );
        IIT icurrent, iend;
        iterator_iterator & operator ++( )
        {
            ++icurrent;
            while ( icurrent == iend )
            {
                if ( current == end ) { return * this; }
                ++current;
                icurrent = current->begin( );
                iend = current->end( );
            }
            return * this;
        }
        using element = decltype(* icurrent);
        const element & operator * ( ) const { return * icurrent; }
        element & operator * ( ) { return * icurrent; }
        template< typename T >
        iterator_iterator( const T & c, const T & e ) : current( c ), end( e )
        {
            if ( current != end )
            {
                icurrent = current->begin( );
                iend = current->end( );
            }
            while ( icurrent == iend )
            {
                if ( current == end ) { return; }
                icurrent = current->begin( );
                iend = current->end( );
                ++current;
            }
        }
        bool operator ==( const iterator_iterator & cmp ) const
        { return current == cmp.current && end == cmp.end; }
        bool operator !=( const iterator_iterator & cmp ) const
        { return ! ((* this) == cmp); }
    };

    template< typename T >
    std::pair< iterator_iterator< T >, iterator_iterator< T > >
    make_iterator_iterator( const T & b, const T & e )
    {
        using ret_type = iterator_iterator< T >;
        return std::make_pair( ret_type( b, e ), ret_type( e, e ) );
    }
}
template< typename IT >
struct std::iterator_traits< misc::iterator_iterator< IT > >
{
    using iterator_category = std::forward_iterator_tag;
    using reference =
        typename std::iterator_traits< typename misc::iterator_iterator< IT >::IIT >::reference;
    using pointer =
        typename std::iterator_traits< typename misc::iterator_iterator< IT >::IIT >::pointer;
    using value_type =
        typename std::iterator_traits< typename misc::iterator_iterator< IT >::IIT >::value_type;
};

template< typename IT >
struct std::iterator_traits< misc::range_container_proxy< IT > > : std::iterator_traits< IT > { };
#endif // ITERATOR_HPP
