//
// Created by arthu on 21/09/2022.
//

#ifndef MINEPLUSPLUS_ENDIANNESS_HPP
#define MINEPLUSPLUS_ENDIANNESS_HPP
#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif
#endif //MINEPLUSPLUS_ENDIANNESS_HPP
