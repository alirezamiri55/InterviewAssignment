#include<iostream>
#include <vector>
#include <random>
#include <string>
#include <istream>
#include <fstream>
#include <utility>
#include <chrono>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <array>

//////////////////////////////////////////////////////
// Interview assignment for Qlik  by  ALIREZA MIRI
//////////////////////////////////////////////////////////////////////////
enum  Difficulty {VERYEASY = 0, EASY, MEDIUM, HARD, SAMURAI};
//////////////////////////////////////////////////////////////////////////
class Reader {

public:
  ////////////////////////////////////////////////////////////////////////
  std::vector<char> splitter(std::string string_in) {

    string_in.erase(std::remove(string_in.begin(),string_in.end(),' '), string_in.end());

    std::vector<char> split;

    for (auto i : string_in) {
      split.emplace_back(i);
    }

    return split;
  }
  //////////////////////////////////////////////////////////////////////////
  std::vector<std::vector<char> > readTable(const std::string& file) {

    std::vector<std::vector<char> > table;
    std::string line;
    std::ifstream myfile(file);

    if (myfile.is_open())
      {
        while (getline(myfile, line))
          {
            table.emplace_back(splitter(line));
          }
        myfile.close();
      } else {
      throw std::runtime_error("");
    }

    return table;
  }
  //////////////////////////////////////////////////////////////////////////
  void print(const std::vector<std::vector<char> >& table) {

    std::cout << "*****************************************\n";
    for (auto i = 0; i < 9; ++i) {
       if(i==3 or i==6){
          std::cout << "---------------------\n";
        }
      for (auto j = 0; j < 9; ++j) {

        std::cout << table[i][j] << " ";

        if(j==2 or j==5){
          std::cout << "| ";
        }

      }
      std::cout << std::endl;
    }
    std::cout << "*****************************************\n";

  }
};

//////////////////////////////////////////////////////////////////////////
class Sudoku {

private:

  long int count = 0;
  std::array<char,9> nums = {'1','2','3','4','5','6','7','8','9'};

public:

  inline std::vector<short int> findBlank(const std::vector<std::vector<char> >& tableIn) {

    for (short int i = 0; i < 9; ++i) {
      for (short int j = 0; j < 9; ++j) {

        if (tableIn[i][j] == '.') return { i,j };

      }
    }

    return {};
  }

  ///////////////////////////////////////////////////////////////
  inline bool validityCheck(const std::vector<std::vector<char> >& tableIn,
                            const std::vector<short int>& cell, const char& char_in) {

    for (auto k = 0; k < 9; k++)
      {
        if (tableIn[cell[0]][k] == char_in or tableIn[k][cell[1]] == char_in)
          return false;
      }

    auto i = (cell[0] / 3) * 3;
    auto j = (cell[1] / 3) * 3;

    for (auto n = i; n < i + 3; ++n) {
      for (auto m = j; m < j + 3; ++m) {
        if (tableIn[n][m] == char_in) return false;
      }
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  bool solve(std::vector<std::vector<char> >& table) {

    ++count;

    auto blank = findBlank(table);
    if (blank.empty()) return true;

      for (auto i:nums){

      if (validityCheck(table, blank, i)) {

        table[blank[0]][blank[1]] = i;

        if (solve(table)) return true;
        table[blank[0]][blank[1]] = '.';

      }
    }
    return false;
  }

  //////////////////////////////////////////////////////
  int numSolutions(std::vector<std::vector<char> >& table, const std::vector<short int>& index)
  {

    std::vector<std::vector<char> > temp(table);
    int num = 0;

    for (char n = '1'; n <= '9' and num < 2; ++n)
      {
        if (validityCheck(table, index, n))
          {
            table[index[0]][index[1]] = n;
            if (solve(table)) ++num;

            table = temp;

          }
      }

    return num;
  }

  /////////////////////////////////////////////////////////////
  bool checkUnique(std::vector<std::vector<char> > table) {

    for (short int i = 0; i < 9; ++i) {
      for (short int j = 0; j < 9; ++j) {

        if (table[i][j] == '.') {
          if (numSolutions(table, { i,j }) != 1) {
            return false;
          }
        }
      }
    }

    return true;
  }

  ///////////////////////////////////////////////////////////
  Difficulty difficulty(std::vector<std::vector<char> > table) {

    count = 0;//counting number of recursion calls
    solve(table);

    if (count < 1800) {
      return Difficulty::VERYEASY;
    }
    else if (count < 2854) {
      return Difficulty::EASY;
    }
    else if (count < 21717) {
      return Difficulty::MEDIUM;
    }
    else if (count < 63985) {
      return Difficulty::HARD;
    }
    else {
      return Difficulty::SAMURAI;
    }

  }
  ////////////////////////////////////////////////////////////////////////
  void printDifficulty(const Difficulty& rate) {

    switch (rate) {
      case Difficulty::VERYEASY:
        std::cout << " The difficulty is very easy" << std::endl;
        break;
      case Difficulty::EASY:
        std::cout << " The difficulty is  easy" << std::endl;
        break;
      case Difficulty::MEDIUM:
        std::cout << " The difficulty is  medium" << std::endl;
        break;
      case Difficulty::HARD:
        std::cout << " The difficulty is  hard" << std::endl;
        break;
      case Difficulty::SAMURAI:
        std::cout << " The difficulty is  samurai" << std::endl;
        break;
    }
  }
};
//////////////////////////////////////////////////////////////////////////
class Generator : public Sudoku {

public:

  Generator() = default;

  std::vector<std::vector<char> > randomValidFilled() {

    auto r = []() {

      std::random_device rdevice;
      std::default_random_engine random_engine(rdevice());
      std::uniform_int_distribution<int> distribution_0_8(0, 8);

      auto randomNumber = distribution_0_8(random_engine);

      return '1' + randomNumber;
    };

    std::vector<std::vector<char> > table(9, std::vector<char>(9, '.'));
    std::vector<std::vector<char> > temp(table);

    char n;

    for (short int i = 0; i < 9; ++i) {
      for (short int j = 0; j < 9; ++j) {

        while (table[i][j] == '.') {
          n = r();
          temp = table;

          if (validityCheck(temp, { i,j }, n)) {
            temp[i][j] = n;
            if (solve(temp)) table[i][j] = n;
          }
        }
      }
    }

    return table;
  }
  ///////////////////////////////////////////////////////////////////////
  std::vector<std::vector<char> > randomDelete(std::vector<std::vector<char> > table,
                                               const bool& symm) {

    int symmetry = 9;
    if(symm) symmetry = 5;

    std::vector< std::vector<int> > randomij(9*symmetry, std::vector<int>(2, 0));
    auto it = randomij.begin();

    for (short int i = 0; i < 9; ++i) {
      for (short int j = 0; j < symmetry; ++j) {
        *it = { i,j };
        ++it;
      }
    }

    unsigned seed = std::chrono::system_clock::now()
      .time_since_epoch()
      .count();

    std::shuffle(std::begin(randomij),
                 std::end(randomij),
                 std::default_random_engine(seed));

    std::vector<std::vector<char> > temp(table);

    std::cout << std::endl << "[" << std::setw(randomij.size()+1) << "]"
              << std::string(randomij.size()+1, '\b');
    std::cout.flush();

    for (auto ij : randomij) {
      temp = table;

      std::cout << "."; std::cout.flush();

      table[ij[0]][ij[1]] = '.';
      if(symm) {
        table[8-ij[0]][8-ij[1]] = '.';
        table[ij[0]][8-ij[1]] = '.';
        table[8-ij[0]][ij[1]] = '.';
      }
      if (!checkUnique(table)) {
        table = temp;
      }
    }

    std::cout << "\n\n"<< "\033[0m";

    return table;
  }
  /////////////////////////////////////////////////////////////////////
  std::vector<std::vector<char> > sudokuFactory(const Difficulty& diff, const bool& symm) {

    std::vector<std::vector<char> > table;

    do {
      table = randomValidFilled();
      table = randomDelete(table, symm);
    } while (difficulty(table) != diff);

    return table;
  }

};
//////////////////////////////////////////////////////////////////////////
template <typename ReaderT>
class UI {

private:

  ReaderT read;

public:


  void generateS(Sudoku sIn, Generator gIn) {

    std::string input;
    std::string sym;
    bool isSym;

    std::cout << "\033[1m\033[33m" << "\n\t\t Generating Sudoku\n";
    std::cout << "\n Difficulty of the Sudoku : Easy= 1 , Medium= 2, Hard= 3, Samurai= 4  \n";
    std::cout << "\n Please enter the difficulty number of sudoku: " ;
    getline(std::cin, input);

    std::cout << "\n Do you want sudoku with symmetry =1 or not =2 : "<< "\033[0m";
    getline(std::cin, sym);

    if(stoi(sym)==1) isSym=true;
    else if(stoi(sym)==2) isSym=false;
    else generateS(sIn, gIn);

    std::vector<std::vector<char> > table;

    switch (stoi(input)) {

      case 1:
        std::cout <<"\033[34m" << "****************************************\n" ;
        std::cout << "\n Generating easy Sudoku with unique solution\n "<< "\033[0m";
        table = gIn.sudokuFactory(Difficulty::EASY, isSym);
        break;

      case 2:
        std::cout << "\033[34m" << "****************************************\n";
        std::cout << "\n Generating medium Sudoku with unique solution\n "<< "\033[0m";
        table = gIn.sudokuFactory(Difficulty::MEDIUM, isSym);
        break;

      case 3:
        std::cout << "\033[34m" << "****************************************\n";
        std::cout << "\n Generating hard Sudoku with unique solution\n "<< "\033[0m";
        table = gIn.sudokuFactory(Difficulty::HARD, isSym);
        break;

      case 4:
        std::cout << "\033[34m" << "****************************************\n";
        std::cout << "\n Generating samurai Sudoku with unique solution\n "<< "\033[0m";
        table = gIn.sudokuFactory(Difficulty::SAMURAI, isSym);
        break;

      default:
        generateS(sIn, gIn);
    }

    read.print(table);
    sIn.printDifficulty(sIn.difficulty(table));
    sIn.solve(table);
    std::cout << "*****************************************\n";
    std::cout << "*The solution of the generated Sudoku is*\n";
    read.print(table);
  }
  //////////////////////////////////////////////////////////
  void solveS(Sudoku sIn) {

    std::string input;

    std::cout << "\033[1m\033[32m" << "\n\t\t Solving Sudoku\n";
    std::cout << "\n Please enter the file name of the sudoku file (for example 'medium.txt') \n"
              <<" File must be in the curent directory: " << "\033[0m";

    getline(std::cin, input);

    try {

      std::vector<std::vector<char> > table = read.readTable(input);

      read.print(table);

      sIn.printDifficulty(sIn.difficulty(table));

      std::cout << "*****************************************\n";
      if (sIn.checkUnique(table)) {
        std::cout << " it has unique answer\n";
      }
      else {
        std::cout << " it has not unique answer\n";
      }
      std::cout << "*****************************************\n";

      std::cout << "**********The solution is****************\n";

      if (sIn.solve(table)) {
        read.print(table);
      }
      else {
        std::cout << "*****************************************\n";
        std::cout << "**There is no solution for this problem**\n";
        std::cout << "*****************************************\n";
      }

    }
    catch (...) {
      solveS(sIn);
    }
  }
  //////////////////////////////////////////////////////////
  void start(const Sudoku& sIn, const Generator& gIn) {
    std::cout<<"\n\t********************************************\n";
    std::cout<<"\033[33m"<<"\t\tInterview assignment for QLIK \n";
    std::cout<<"\t\t   Written by ALIREZA MIRI\n";
    std::cout<<"\033[0m"<<"\t********************************************\n";
    std::string userInput;
    std::cout <<"\033[33m"<< "\nDo you want to solve a sudoku = 1 or generate = 2  (1 or 2) :  " << "\033[0m";
    getline(std::cin, userInput);

    try {

      if (userInput == std::to_string(1)) {
        solveS(sIn);
      }
      else if (userInput == std::to_string(2)) {
        generateS(sIn, gIn);
      }
      else {
        throw std::runtime_error("");
      }

    }
    catch (...) {
      start(sIn, gIn);
    }
  }
};
//////////////////////////////////////////////////////////////////////////
int main()
{

  Sudoku s;            // part 2
  Generator g;         // part 3
  UI<Reader> input;    // part 4

  input.start(s, g);

  std::cout << "\033[32m" << "\t\t Completed Successfully  \n \n" << "\033[0m";

  return 0;
}
