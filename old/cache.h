//
// Created by ich on 13.07.23.
//

#ifndef NIX_OPERATOR_CACHE_H
#define NIX_OPERATOR_CACHE_H

void cache_init();
void cache_clean();

const char* cache_get_package(const char* packageName);


#endif //NIX_OPERATOR_CACHE_H
