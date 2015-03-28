#ifndef CAN_CALL_HPP
#define CAN_CALL_HPP
#include <boost/mpl/bool.hpp>
namespace misc
{
    template< typename T > struct PrintType;
    template< typename  T > T FakeType( );
    template< typename T, typename ... R >
    struct can_call
    {
        template< typename t >
        static constexpr boost::mpl::true_ SFINAE(
                t,
                std::remove_reference_t
                < decltype( FakeType< t >( )( FakeType< R >( )... ) ) > * = nullptr )
        { return boost::mpl::true_( ); }
        static constexpr boost::mpl::false_ SFINAE( ... ) { return boost::mpl::false_( ); }
        static constexpr bool value = decltype( SFINAE( std::declval< T >( ) ) )::value;
    };
}
#endif // CAN_CALL_HPP
