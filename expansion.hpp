#ifndef EXPANSION_HPP
#define EXPANSION_HPP
#include "can_call.hpp"
namespace common
{
    template< typename T1, typename T2 >
    struct expansion
    {
        T1 first;
        T2 second;
        template< typename ... T, std::enable_if_t< can_call< const T1, T && ... >::value, bool > = true >
        auto operator ( )( T && ... arg ) const { return first( std::forward< T >( arg ) ... ); }
        template< typename ... T, typename = std::enable_if_t< ! can_call< const T1, T && ... >::value > >
        auto operator ( )( T && ... arg ) const { return second( std::forward< T >( arg ) ... ); }
        template< typename ... T, std::enable_if_t< can_call< T1, T && ... >::value, bool > = true >
        auto operator ( )( T && ... arg ) { return first( std::forward< T >( arg ) ... ); }
        template< typename ... T, typename = std::enable_if_t< ! can_call< T1, T && ... >::value > >
        auto operator ( )( T && ... arg ) { return second( std::forward< T >( arg ) ... ); }
        expansion( const T1 & t1, const T2 & t2 ) : first( t1 ), second( t2 ) { }
    };
    template< typename T1, typename T2 >
    expansion< T1, T2 > make_expansion( const T1 & t1, const T2 & t2 )
    { return expansion< T1, T2 >( t1, t2 ); }
}
#endif // EXPANSION_HPP
