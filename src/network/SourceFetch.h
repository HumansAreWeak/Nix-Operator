/**
 * Description of the header file to be included here.
 *
 * @author Maik Steiger
 * @date 18.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_SOURCEFETCH_H
#define NIX_OPERATOR_SOURCEFETCH_H
#include <future>

class SourceFetch {
private:
    std::string mUrl;

public:
    explicit SourceFetch(const char *url);
    std::future<bool> fetchFromUrl();
};

#endif // NIX_OPERATOR_SOURCEFETCH_H
