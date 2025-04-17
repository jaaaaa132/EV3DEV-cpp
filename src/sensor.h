#include<iostream>
#include<fstream>
class Sensor{
  private:

  protected:
    bool connected = false;
    std::string directory;
    
    std::ofstream mode_file;
    std::ifstream value0_file;
    std::ifstream value1_file;
    std::ifstream value2_file;
    std::ifstream value3_file;
    std::ifstream value4_file;
    std::ifstream value5_file;
    std::ifstream value6_file;
    std::ifstream value7_file;
    
    void open_files();

    void debug_output_file(std::string file_name);
    template<typename FileType> bool check_file(FileType& file, std::string debug_name);    
    bool are_files_opened();
    int read_value(std::ifstream& file);
  public:
    Sensor();
    ~Sensor();
    void set_directory(std::string p_directory);
    bool is_connected();
    int get_value(int value_index = 0);
    void set_mode(std::string mode);
  };

