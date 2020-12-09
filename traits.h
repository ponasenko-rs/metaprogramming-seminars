#pragma once

namespace traits {
    template<typename From, typename To>
    class Convertible {
        using A = char;
        using Yes = A;
        struct No {
            A field[2];
        };

        static Yes Test(const To &);

        static No Test(...);

        static From Mock();

    public:
        const static bool value = sizeof(Test(Mock())) == sizeof(Yes);
    };

    template<typename T, typename U>
    static const bool Convertible_v = Convertible<T, U>::value;


    template<typename T, typename U>
    struct SameTypes {
        const static bool value = false;
    };

    template<typename T>
    struct SameTypes<T, T> {
        const static bool value = true;
    };

    template<typename T, typename U>
    const static bool SameTypes_v = SameTypes<T, U>::value;

} // namespace traits
