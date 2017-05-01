#ifndef __on_std_fs_h__
#define __on_std_fs_h__
// cstd template classes have proven working
// [*] visual studio 2013 express
// [ ] visual studio 2015 community - more strict and not supporting prior methods
// [*] gcc [which versions?]
//       -  crap this isn't working in my favorite subsystem
//       -  https://github.com/Alexpux/MINGW-packages/issues/2292
// using boost see: http://www.boost.org/doc/libs/1_64_0/libs/filesystem/doc/deprecated.html
//                                                ^^^^^^
#if defined(_MSC_VER) && (_MSC_VER == 1800)
#  include "on.std_vc12_fs.h"
#elif defined(_MSC_VER) && (_MSC_VER == 1900)
#  include "on.std_vc14_fs.h"
#else
#  include "on.std_gc_fs.h"
#endif

#endif
