#ifndef MISC_HPP
#define MISC_HPP
namespace common
{
    template< typename T > void supress_unused(const T &){}
    struct unit{unit(){}};
}
#define AUTO_INIT(NAME, CODE) \
    static struct AUTO_INIT_ ## NAME{ \
        AUTO_INIT_ ## NAME( ){ \
            static ::common::unit auto_init_inner ## NAME { [](){CODE;return unit{};}() }; \
            ::common::supress_unused(auto_init_inner ## NAME); } } \
    auto_init_ ## NAME;
#endif // MISC_HPP

