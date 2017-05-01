#include <filesystem>

namespace on { namespace fs {
  // rather effective documentation
  // http://www.boost.org/doc/libs/1_43_0/libs/filesystem/doc/index.htm


  static void replace_ext(std::string &infile, std::string &ofile)
  {
    std::tr2::sys::path filep(infile);
    ofile = filep.parent_path().string();
    ofile += "\\" + std::tr2::sys::basename(filep) + ".wav";
  }

  // iterate through the directory specified by input directory path fpath.
  static void get_pathinfo(std::string fpath, std::vector<std::string> &FileNames) {
    namespace fx = std::tr2::sys;
    fx::path directory(fpath);
    fx::directory_iterator iter(directory), ifiles(directory), end;
    std::cout << std::endl << "dirs" << std::endl;
    for (; iter != end; ++iter)
    {
      if (!fx::is_directory(iter->path())) continue;
      FileNames.push_back(iter->path().filename().c_str());
      std::cout << "  - " << FileNames.at(FileNames.size() - 1).c_str() << std::endl;
    }
    std::cout << std::endl << "file" << std::endl;
    for (; ifiles != end; ++ifiles)
      //  if (iter->path().extension() == ".txt")
    {
      if (!fx::is_regular_file(ifiles->path())) continue;
      FileNames.push_back(ifiles->path().filename().c_str());
      std::cout << "  - " << FileNames.at(FileNames.size() - 1).c_str() << std::endl;
    }
  }
  static std::string get_pathinfo(std::string fpath, std::string ext, std::vector<std::string> &DirNames, std::vector<std::string> &FileNames) {
    namespace fx = std::tr2::sys;
    fx::path directory(fpath);
    fx::directory_iterator end;

    std::cout << std::endl << "dir:" << std::endl;
    for (fx::directory_iterator iter(directory); iter != end; ++iter)
    {
      if (!fx::is_directory(iter->path())) continue;
      std::string s = iter->path().filename();
      DirNames.push_back(s);
      std::cout << "  - " << s.c_str() << std::endl;
    }
    std::cout << std::endl << "file" << std::endl;
    for (fx::directory_iterator ifiles(directory); ifiles != end; ++ifiles)
      if (ifiles->path().extension() == ext)
      {
        if (!fx::is_regular_file(ifiles->path())) continue;
        std::string s = ifiles->path().filename();
        FileNames.push_back(s);
        std::cout << "  - " << s.c_str() << std::endl;
      }
  }

  static bool get_filename(std::string file) {
    namespace fx = std::tr2::sys;
    fx::path filepath = file;
    return filepath.filename().c_str();
  }
  static bool is_directory(std::string file) {
    namespace fx = std::tr2::sys;
    fx::path filepath = file;
    return fx::is_directory(filepath);
  }
  static bool is_file(std::string file) {
    namespace fx = std::tr2::sys;
    fx::path filepath = file;
    return fx::is_regular_file(filepath);
  }
  static void examine_path(std::string file) {
    namespace fx = std::tr2::sys;
    fx::path filepath = file;
    std::cout << (fx::exists(filepath) ? " - path exists" : "path don't exist") << std::endl;
    std::cout << "  - basename=" << filepath.basename() << std::endl;
    // has_branch_path is obsoleted; use has_parent_path
    std::cout << "  - has_branch_path=" << filepath.has_branch_path() << std::endl;
    std::cout << "  - has_parent_path=" << filepath.has_parent_path() << std::endl;
    std::cout << "    -   branch_path=" << filepath.branch_path() << std::endl;
    std::cout << "    -   branch_path=" << filepath.parent_path() << std::endl;
    std::cout << "  - directory_string=" << filepath.directory_string() << std::endl;
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
