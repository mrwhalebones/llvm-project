//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator, class Allocator, class charT,
//           class traits>
//   bool regex_match(BidirectionalIterator first, BidirectionalIterator last,
//                    match_results<BidirectionalIterator, Allocator>& m,
//                    const basic_regex<charT, traits>& e,
//                    regex_constants::match_flag_type flags
//                                            = regex_constants::match_default);

// TODO: investigation needed
// TODO(netbsd): incomplete support for locales
// XFAIL: target={{.*}}-linux-gnu{{.*}}, netbsd, freebsd
// XFAIL: target={{.*}}-amazon-linux{{.*}}
// REQUIRES: locale.cs_CZ.ISO8859-2

#include <regex>
#include <cassert>
#include "test_macros.h"
#include "test_iterators.h"

#include "platform_support.h" // locale name macros

int main(int, char**)
{
    std::locale::global(std::locale(LOCALE_cs_CZ_ISO8859_2));
    {
        std::cmatch m;
        const char s[] = "m";
// AIX supports character equivalence classes. What the contents of the class are depends
// on the locale and the standards do not specify any locale other than C/POSIX.
#if defined(_AIX)
        assert(std::regex_match(s, m,
                      std::regex("[a[=m=]z]", std::regex_constants::awk)));
#else
        assert(std::regex_match(s, m,
                      std::regex("[a[=M=]z]", std::regex_constants::awk)));
#endif
        assert(m.size() == 1);
        assert(!m.prefix().matched);
        assert(m.prefix().first == s);
        assert(m.prefix().second == m[0].first);
        assert(!m.suffix().matched);
        assert(m.suffix().first == m[0].second);
        assert(m.suffix().second == m[0].second);
        assert((std::size_t)m.length(0) == std::char_traits<char>::length(s));
        assert(m.position(0) == 0);
        assert(m.str(0) == s);
    }
    {
        std::cmatch m;
        const char s[] = "Ch";
        assert(std::regex_match(s, m, std::regex("[a[.ch.]z]",
                   std::regex_constants::awk | std::regex_constants::icase)));
        assert(m.size() == 1);
        assert(!m.prefix().matched);
        assert(m.prefix().first == s);
        assert(m.prefix().second == m[0].first);
        assert(!m.suffix().matched);
        assert(m.suffix().first == m[0].second);
        assert(m.suffix().second == m[0].second);
        assert((std::size_t)m.length(0) == std::char_traits<char>::length(s));
        assert(m.position(0) == 0);
        assert(m.str(0) == s);
    }
    std::locale::global(std::locale("C"));
    {
        std::cmatch m;
        const char s[] = "m";
        assert(!std::regex_match(s, m, std::regex("[a[=M=]z]",
                                                 std::regex_constants::awk)));
        assert(m.size() == 0);
    }

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    std::locale::global(std::locale(LOCALE_cs_CZ_ISO8859_2));
    {
        std::wcmatch m;
        const wchar_t s[] = L"m";
#if defined(_AIX)
        assert(std::regex_match(s, m, std::wregex(L"[a[=m=]z]",
                                                 std::regex_constants::awk)));
#else
        assert(std::regex_match(s, m, std::wregex(L"[a[=M=]z]",
                                                 std::regex_constants::awk)));
#endif
        assert(m.size() == 1);
        assert(!m.prefix().matched);
        assert(m.prefix().first == s);
        assert(m.prefix().second == m[0].first);
        assert(!m.suffix().matched);
        assert(m.suffix().first == m[0].second);
        assert(m.suffix().second == m[0].second);
        assert((std::size_t)m.length(0) == std::char_traits<wchar_t>::length(s));
        assert(m.position(0) == 0);
        assert(m.str(0) == s);
    }
//TODO: Need to be investigated for AIX OS
#if !defined(_AIX)
    {
        std::wcmatch m;
        const wchar_t s[] = L"Ch";
        assert(std::regex_match(s, m, std::wregex(L"[a[.ch.]z]",
                   std::regex_constants::awk | std::regex_constants::icase)));
        assert(m.size() == 1);
        assert(!m.prefix().matched);
        assert(m.prefix().first == s);
        assert(m.prefix().second == m[0].first);
        assert(!m.suffix().matched);
        assert(m.suffix().first == m[0].second);
        assert(m.suffix().second == m[0].second);
        assert((std::size_t)m.length(0) == std::char_traits<wchar_t>::length(s));
        assert(m.position(0) == 0);
        assert(m.str(0) == s);
    }
#endif
    std::locale::global(std::locale("C"));
    {
        std::wcmatch m;
        const wchar_t s[] = L"m";
        assert(!std::regex_match(s, m, std::wregex(L"[a[=M=]z]",
                                                 std::regex_constants::awk)));
        assert(m.size() == 0);
    }
#endif // TEST_HAS_NO_WIDE_CHARACTERS
    return 0;
}
