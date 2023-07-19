/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 19.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_CACHE_H
#define NIX_OPERATOR_CACHE_H

#define NIXOP_CACHE_DIR "/tmp/nixop"

class Cache {
public:
    static void ensureCacheFolderExists();
};

#endif // NIX_OPERATOR_CACHE_H
