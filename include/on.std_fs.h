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

#ifdef __MINGW32__
//   checking for msys2's boost
#    include <boost/filesystem/operations.hpp>
#define MSYS2017
#elif defined(_MSC_VER) && (_MSC_VER == 1800)
// MSVC2013-express
#define VC2013
#include <filesystem>
#elif defined(_MSC_VER) && (_MSC_VER == 1900)
// MSVC2015-community
#define VC2015
#include <filesystem>
#endif


namespace on {
namespace fs {
    #ifdef MSYS2017
    #define fx boost::filesystem
    #else
    define fx std::tr2::sys
    #endif
  // some work had been done to get some of this working in VC2015-CE

  // rather effective documentation
  // http://www.boost.org/doc/libs/1_43_0/libs/filesystem/doc/index.htm
  // the experimental filesystem related

  // iterate through the directory specified by input directory path fpath.
  static void get_pathinfo(std::string fpath, std::vector<std::string> &FileNames) {

    fx::path directory(fpath);
    fx::directory_iterator iter(directory), ifiles(directory), end;
    std::cout << std::endl << "dirs" << std::endl;
    for (; iter != end; ++iter)
    {
      if (!fx::is_directory(iter->path())) continue;
      FileNames.push_back(std::string(std::string(iter->path().string().c_str())));
      std::cout << "  - " << FileNames.at(FileNames.size() - 1).c_str() << std::endl;
    }
    std::cout << std::endl << "file" << std::endl;
    for (; ifiles != end; ++ifiles)
      //  if (iter->path().extension() == ".txt")
    {
      if (!fx::is_regular_file(ifiles->path())) continue;
      FileNames.push_back((char*)ifiles->path().filename().c_str());
      std::cout << "  - " << FileNames.at(FileNames.size() - 1).c_str() << std::endl;
    }
  }

  static std::string get_pathinfo(std::string fpath, std::string ext, std::vector<std::string> &DirNames, std::vector<std::string> &FileNames) {


    fx::path directory(fpath);
    fx::directory_iterator end;

    std::cout << std::endl << "dir:" << std::endl;
    for (fx::directory_iterator iter(directory); iter != end; ++iter)
    {
      if (!fx::is_directory(iter->path())) continue;
      std::string s = std::string(iter->path().string());
      DirNames.push_back(s);
      std::cout << "  - " << s.c_str() << std::endl;
    }
    std::cout << std::endl << "file:" << std::endl;
    for (fx::directory_iterator ifiles(directory); ifiles != end; ++ifiles)
      if (ifiles->path().extension() == ext)
      {
        if (!fx::is_regular_file(ifiles->path())) continue;
        std::string s = std::string((char*)ifiles->path().filename().c_str());
        FileNames.push_back(s);
        std::cout << "  - " << s.c_str() << std::endl;
      }
  }

  static bool get_filename(std::string file) {
    fx::path filepath = file;
    return filepath.filename().c_str();
  }
  static bool is_directory(std::string file) {
    fx::path filepath = file;
    return fx::is_directory(filepath);
  }
  static bool is_file(std::string file) {
    fx::path filepath = file;
    return fx::is_regular_file(filepath);
  }
  static void examine_path(std::string file) {
    fx::path filepath = file;
    std::cout << (fx::exists(filepath) ? " - path exists" : "path don't exist") << std::endl;


    std::cout << "  - basename=" << filepath.string().c_str() << std::endl;

    // has_branch_path is obsoleted; use has_parent_path
    std::cout << "  - has_branch_path=" << filepath.has_branch_path() << std::endl;
    std::cout << "  - has_parent_path=" << filepath.has_parent_path() << std::endl;
    std::cout << "    -   branch_path=" << filepath.branch_path().string() << std::endl;
    std::cout << "    -   branch_path=" << filepath.parent_path().string() << std::endl;
    std::cout << "  - directory_string=" << filepath.filename().c_str() << std::endl;
    std::cout << "  - filename=" << filepath.filename().c_str() << std::endl;
    std::cout << "  - has_leaf=" << filepath.has_leaf() << std::endl;
    std::cout << "    -   leaf=" << filepath.leaf() << std::endl;
    std::cout << "  - is_directory=" << fx::is_directory(filepath) << std::endl;
    std::cout << "  - is_regular_file=" << fx::is_regular_file(filepath) << std::endl;

    std::vector<std::string> files = std::vector<std::string>();
    get_pathinfo(file, files);
    std::cout << std::endl << std::endl;
  }
};}; // on::fs

#endif
