//
// Created by ich on 16.07.23.
//

#ifndef NIX_OPERATOR_COMMONENTITY_H
#define NIX_OPERATOR_COMMONENTITY_H
#include <string>
#include <array>

/**
 * Defines what a type should contain
 */
struct Type
{
    int type;
    const char* name;

    constexpr Type(int TYPE, const char* NAME) : type(TYPE), name(NAME) {}
};

/**
 * The "sealed class" of the type
 */
class _Type
{
public:
    inline static constexpr Type UNKNOWN = Type(0, "unknown");
    inline static constexpr Type PACKAGE = Type(1, "package");

public:
    inline static constexpr std::array<Type, 2> ALL = {
        _Type::UNKNOWN,
        _Type::PACKAGE
    };
};

/**
 * Defines what a Platform should contain
 */
struct Platform
{
    int type;
    const char* name;

    constexpr Platform(int TYPE, const char* NAME) : type(TYPE), name(NAME) {}
};

/**
 * List of all available Platforms. It is meant as a hacky "sealed class" replacement
 */
class _Platform
{
public:
    inline static constexpr Platform UNKNOWN = Platform(0, "unknown");
    inline static constexpr Platform I686_LINUX = Platform(1, "i686-linux");
    inline static constexpr Platform AARCH64_LINUX = Platform(2, "aarch64-linux");
    inline static constexpr Platform ARMV6L_LINUX = Platform(3, "armv6l-linux");
    inline static constexpr Platform RISCV64_LINUX = Platform(4, "riscv64-linux");
    inline static constexpr Platform S390X_LINUX = Platform(5, "s390x-linux");
    inline static constexpr Platform X86_64_DARWIN = Platform(6, "x86_64-darwin");
    inline static constexpr Platform MIPS64EL_LINUX = Platform(7, "mips64el-linux");
    inline static constexpr Platform POWERPC64_LINUX = Platform(8, "powerpc64-linux");
    inline static constexpr Platform M68K_LINUX = Platform(9, "m68k-linux");
    inline static constexpr Platform ARMV7L_LINUX = Platform(10, "armv7l-linux");
    inline static constexpr Platform MIPSEL_LINUX = Platform(11, "mipsel-linux");
    inline static constexpr Platform ARMV7A_LINUX = Platform(12, "armv7a-linux");
    inline static constexpr Platform RISCV32_LINUX = Platform(13, "riscv32-linux");
    inline static constexpr Platform POWERPC64LE_LINUX = Platform(14, "powerpc64le-linux");
    inline static constexpr Platform LOONGARCH64_LINUX = Platform(15, "loongarch64-linux");
    inline static constexpr Platform I686_DARWIN = Platform(16, "i686-darwin");
    inline static constexpr Platform X86_64_LINUX = Platform(17, "x86_64-linux");
    inline static constexpr Platform MICROBLAZEEL_LINUX = Platform(18, "microblazeel-linux");
    inline static constexpr Platform ARM7A_DARWIN = Platform(19, "armv7a-darwin");
    inline static constexpr Platform ARMV5TEL_LINUX = Platform(20, "armv5tel-linux");
    inline static constexpr Platform AARCH64_DARWIN = Platform(21, "aarch64-darwin");
    inline static constexpr Platform S390_LINUX = Platform(22, "s390-linux");
    inline static constexpr Platform MICROBLAZE_LINUX = Platform(23, "microblaze-linux");

public:
    // Makes the whole "sealed class" iterable
    inline static constexpr std::array<Platform, 24> ALL = {
        _Platform::UNKNOWN,
        _Platform::I686_LINUX,
        _Platform::AARCH64_LINUX,
        _Platform::ARMV6L_LINUX,
        _Platform::RISCV64_LINUX,
        _Platform::S390X_LINUX,
        _Platform::X86_64_DARWIN,
        _Platform::MIPS64EL_LINUX,
        _Platform::POWERPC64_LINUX,
        _Platform::M68K_LINUX,
        _Platform::ARMV7L_LINUX,
        _Platform::MIPSEL_LINUX,
        _Platform::ARMV7A_LINUX,
        _Platform::RISCV32_LINUX,
        _Platform::POWERPC64LE_LINUX,
        _Platform::LOONGARCH64_LINUX,
        _Platform::I686_DARWIN,
        _Platform::X86_64_LINUX,
        _Platform::MICROBLAZEEL_LINUX,
        _Platform::ARM7A_DARWIN,
        _Platform::ARMV5TEL_LINUX,
        _Platform::AARCH64_DARWIN,
        _Platform::S390_LINUX,
        _Platform::MICROBLAZE_LINUX
    };
};

struct License
{
    std::string url;
    std::string full_name;

    License(const char* URL, const char* FULLNAME) : url(URL), full_name(FULLNAME) {}
};

struct Maintainer
{
    std::string name;
    std::string github;
    std::string email;

    Maintainer(const char* NAME, const char* GITHUB, const char* EMAIL) : name(NAME), github(GITHUB), email(EMAIL) {}
};

#endif //NIX_OPERATOR_COMMONENTITY_H
