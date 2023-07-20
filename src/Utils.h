/**
 * Description of the header file to be included here.
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_UTILS_H
#define NIX_OPERATOR_UTILS_H
#include <json/json.h>

class Utils {
public:
    static const char *asCString(const Json::Value &val);
    static std::string asString(const Json::Value &val);
};

template <std::string_view const &...Strs>
struct JoinExpr {
    // Join all strings into a single std::array of chars
    inline static constexpr auto impl() noexcept
    {
        constexpr std::size_t len = (Strs.size() + ... + 0);
        std::array<char, len + 1> arr {};
        auto append = [i = 0, &arr](auto const &s) mutable {
            for (auto c : s)
                arr[i++] = c;
        };
        (append(Strs), ...);
        arr[len] = 0;
        return arr;
    }

    // Give the joined string static storage
    static constexpr auto arr = impl();
    
    // View as a std::string_view
    static constexpr std::string_view value { arr.data(), arr.size() - 1 };
};

#endif // NIX_OPERATOR_UTILS_H
