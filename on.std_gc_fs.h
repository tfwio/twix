// cstd template classes have proven working
// [*] visual studio 2013 express
// [ ] visual studio 2015 community - more strict and not supporting prior methods
// [*] gcc [which versions?]
//       -  crap this isn't working in my favorite subsystem
//       -  https://github.com/Alexpux/MINGW-packages/issues/2292
// using boost see: http://www.boost.org/doc/libs/1_64_0/libs/filesystem/doc/deprecated.html
//                                                ^^^^^^

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace fx = boost::filesystem;

namespace on { namespace fs {
  // rather effective documentation
  // http://www.boost.org/doc/libs/1_43_0/libs/filesystem/doc/index.htm

  static void replace_ext(std::string &infile, std::string &ofile)
  {
    fx::path filep(infile);
    ofile = fx::change_extension(filep,".wav").string();
  }

  // iterate through the directory specified by input directory path fpath.
  static void get_pathinfo(std::string fpath, std::vector<std::string> &FileNames) {
    fx::path directory(fpath);
    fx::directory_iterator iter(directory), ifiles(directory), end;
    std::cout << std::endl << "dirs" << std::endl;
    for (; iter != end; ++iter)
    {
      if (!fx::is_directory(iter->path())) continue;
      FileNames.push_back(iter->path().filename().string());
      std::cout << "  - " << FileNames.at(FileNames.size() - 1).c_str() << std::endl;
    }
    std::cout << std::endl << "file" << std::endl;
    for (; ifiles != end; ++ifiles)
      //  if (iter->path().extension() == ".txt")
    {
      if (!fx::is_regular_file(ifiles->path())) continue;
      FileNames.push_back(ifiles->path().filename().string());
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
      std::string s = iter->path().filename().string();
      DirNames.push_back(s);
      std::cout << "  - " << s.c_str() << std::endl;
    }
    std::cout << std::endl << "file" << std::endl;
    for (fx::directory_iterator ifiles(directory); ifiles != end; ++ifiles)
      if (ifiles->path().extension() == ext)
      {
        if (!fx::is_regular_file(ifiles->path())) continue;
        std::string s = ifiles->path().filename().string();
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
    std::cout << "  - basename=" << fx::basename(filepath) << std::endl;
    // has_branch_path is obsoleted; use has_parent_path
    std::cout << "  - has_branch_path=" << filepath.has_branch_path() << std::endl;
    std::cout << "  - has_parent_path=" << filepath.has_parent_path() << std::endl;
    std::cout << "    -   branch_path=" << filepath.branch_path() << std::endl;
    std::cout << "    -   branch_path=" << filepath.parent_path() << std::endl;
    std::cout << "  - directory_string=" << filepath.parent_path() << std::endl; // directory_path()
    std::cout << "  - filename=" << filepath.filename() << std::endl;
    std::cout << "  - has_leaf=" << filepath.has_leaf() << std::endl;
    std::cout << "    -   leaf=" << filepath.leaf() << std::endl;
    std::cout << "  - is_directory=" << fx::is_directory(filepath) << std::endl;
    std::cout << "  - is_regular_file=" << fx::is_regular_file(filepath) << std::endl;

    std::vector<std::string> files = std::vector<std::string>();
    get_pathinfo(file, files);
    std::cout << std::endl << std::endl;
  }
};};
