#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <memory>
namespace common
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

    template< typename T >
    struct function_output_iterator : std::iterator< std::output_iterator_tag, T >
    {
        explicit function_output_iterator( ) { }
        explicit function_output_iterator( const T & f ) : f( std::shared_ptr< T >( new T( f ) ) ) {}
        struct proxy
        {
            proxy( std::shared_ptr< T > & f ) : f( f ) { }
            template< typename V >
            proxy & operator = ( const V & value )
            {
                (*f)(value);
                return *this;
            }
            const std::shared_ptr< T > & f;
        };
        proxy operator*( ) { return proxy( f ); }
        function_output_iterator & operator ++ ( ) { return *this; }
        function_output_iterator & operator ++ (int) { return *this; }
        function_output_iterator & operator = ( const function_output_iterator & p ) { f = p.f; return * this; }
        std::shared_ptr< T > f;
    };
    template< typename T >
    function_output_iterator< T > make_function_output_iterator( const T & f = T( ) )
    { return function_output_iterator< T >( f ); }
}
template< typename IT >
struct std::iterator_traits< common::iterator_iterator< IT > >
{
    using iterator_category = std::forward_iterator_tag;
    using reference =
        typename std::iterator_traits< typename common::iterator_iterator< IT >::IIT >::reference;
    using pointer =
        typename std::iterator_traits< typename common::iterator_iterator< IT >::IIT >::pointer;
    using value_type =
        typename std::iterator_traits< typename common::iterator_iterator< IT >::IIT >::value_type;
};

template< typename IT >
struct std::iterator_traits< common::range_container_proxy< IT > > : std::iterator_traits< IT > { };
#endif // ITERATOR_HPP
