#ifndef EXPANSION_HPP
#define EXPANSION_HPP
#include "can_call.hpp"
namespace misc
{
    template< typename T1, typename T2 >
    struct expansion
    {
        T1 first;
        T2 second;
        template< typename ... T, std::enable_if_t< can_call< const T1, T ... >::value, bool > = true >
        auto operator ( )( const T & ... arg ) const { return first( arg ... ); }
        template< typename ... T, typename = std::enable_if_t< ! can_call< const T1, T ... >::value > >
        auto operator ( )( const T & ... arg ) const { return second( arg ... ); }
        template< typename ... T, std::enable_if_t< can_call< T1, T ... >::value, bool > = true >
        auto operator ( )( const T & ... arg ) { return first( arg ... ); }
        template< typename ... T, typename = std::enable_if_t< ! can_call< T1, T ... >::value > >
        auto operator ( )( const T & ... arg ) { return second( arg ... ); }
        expansion( const T1 & t1, const T2 & t2 ) : first( t1 ), second( t2 ) { }
    };
    template< typename T1, typename T2 >
    expansion< T1, T2 > make_expansion( const T1 & t1, const T2 & t2 )
    { return expansion< T1, T2 >( t1, t2 ); }
}
#endif // EXPANSION_HPP
