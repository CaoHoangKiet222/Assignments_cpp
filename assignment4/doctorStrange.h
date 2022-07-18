//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef doctorStrange_h
#define doctorStrange_h

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

/// STUDENT'S ANSWER BEGINS HERE
/// STUDENTS CAN ADD HELPER FUNCTIONS
int ROW = 7;
int COLUMN = 7;

string anotherTokenize(string s, int &start, string del = " ");

struct Levitation {
  int times_resist;
  bool isReal;
  bool isWearing;

  Levitation() : times_resist(0), isReal(false), isWearing(false) {}

  void summon() {
    this->times_resist = 3;
    this->isReal = true;
    this->isWearing = true;
  }
};

struct Wong {
  int times_help;
  bool isCalled;
  bool isReal;

  Wong() : times_help(0), isCalled(false), isReal(false){};

  void init() {
    this->times_help = 3;
    this->isCalled = true;
    this->isReal = true;
  }

  void initFake() {
    this->times_help = 3;
    this->isCalled = true;
    this->isReal = false;
  }

  bool returnToKamarTaj(bool isReal) {
    if (this->isCalled && isReal) {
      if (this->times_help >= 1) {
        return false;
      }
    }
    return true;
  }

  void kill() {
    if (this->isCalled && !this->isReal) {
      this->times_help = 0;
      this->isCalled = true;
      this->isReal = false;
    }
  }
};

struct Mushroom {
  int times_poisoned;
  bool isEating;
  Mushroom() : times_poisoned(0), isEating(false){};

  void activate() {
    this->times_poisoned = 3;
    this->isEating = true;
  }

  void inactivate() {
    this->times_poisoned = 0;
    this->isEating = false;
  }
};

struct Wanda {
  bool deprive_levitation;
  bool chances_to_kill;
  bool is_negotiation;
  int TS;

  Wanda()
      : deprive_levitation(false), chances_to_kill(false),
        is_negotiation(false){};

  void activate(int TS) {
    this->deprive_levitation = true;
    this->chances_to_kill = true;
    this->is_negotiation = true;
    this->TS = TS;
  }

  void inactivate() {
    this->deprive_levitation = false;
    this->chances_to_kill = false;
    this->is_negotiation = false;
    this->TS = 0;
  }

  bool findExitWay(int *arr, int size, int key, int moves, int &total_moves) {
    int left = 0, right = size - 1;

    while (left <= right && moves > 0) {
      total_moves++;
      int mid = (right + left) / 2;

      if (arr[mid] == key) {
        return true;
      }

      if (arr[mid] > key) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }

      moves--;
    }

    return false;
  }
};

struct Gates {
  int *array;
  int size;

  Gates() : array(nullptr), size(0) {}

  bool initGates(string s, int &start) {
    this->array = new int;

    while (s.find(" ", start) != -1) {
      string value = anotherTokenize(s, start);
      if (value == "") {
        return false;
      }
      array[this->size++] = stoi(value);
    }

    if (start >= (int)s.size() - 1) {
      string value = s.substr(start);
      if (value == "") {
        return false;
      }
      array[this->size++] = stoi(value);
    }

    if (this->size == 0) {
      return false;
    }

    return true;
  }

  ~Gates() {
    delete this->array;
    this->size = 0;
    this->array = nullptr;
  }
};

struct TimeThrowback {
  int return_at_event;
  int maxHP;
  int event_has_maxHP;
  bool isActivate;

  TimeThrowback() : return_at_event(-1), maxHP(INT16_MIN), isActivate(false){};

  void activate(int &start) {
    this->isActivate = true;
    start = this->event_has_maxHP;
  }
};

struct Doctor {
  int HP;
  int maxHP;
  int LV;
  int EXP;
  int TS;
  Levitation levitation;
  Wong wong;
  Mushroom mushroom;
  Wanda wanda;
  Gates gates;
  TimeThrowback throwback;

  Doctor(int hp, int maxHp, int lv, int exp, int ts)
      : HP(hp), maxHP(maxHp), LV(lv), EXP(exp), TS(ts){};

  void initThrowBack(int hp, int lv, int exp, int ts) {
    this->HP = hp;
    this->LV = lv;
    this->EXP = exp;
    this->TS = ts;
  }
};

string Tokenize(string s, int &start, string del = " ") {
  string result;
  int end = (int)s.find(del, start);
  if (end == -1) {
    result = s.substr(start, (int)s.find("!", start) - start);
    start = (int)s.size();
  } else {
    result = s.substr(start, end - start);
    start = (int)end + (int)del.size();
  }
  return result;
}

string anotherTokenize(string s, int &start, string del) {
  string result;
  int end = (int)s.find(del, start);
  if (end == -1) {
    result = s.substr(start);
  } else {
    result = s.substr(start, end - start);
    start = (int)end + (int)del.size();
  }
  return result;
}

bool initMatrix(int **&matrix, string s, int &start) {
  for (int i = 0; i < ROW; i++) {
    matrix[i] = new int[COLUMN];
    for (int j = 0; j < COLUMN; j++) {
      string num = anotherTokenize(s, start);
      if (num == "") {
        return false;
      }
      matrix[i][j] = stoi(num);
    }
  }
  return true;
}

bool matrixIncrease(int **matrix, int m, int index_row, int index_column) {
  for (int i = index_column; i < index_column + m; i++) {
    for (int j = index_row; j < index_row + m - 1; j++) {
      if (matrix[j][i] < matrix[j + 1][i]) {
        return false;
      }
    }
  }
  return true;
}

void deleteMatrix(int **&matrix) {
  for (int i = 0; i < ROW; i++) {
    delete[] matrix[i];
    matrix[i] = nullptr;
  }
  delete[] matrix;
  matrix = nullptr;
}

void reverse(string &s, int start, int end) {
  for (int i = start; i < (end + start) / 2; i++) {
    char temp = s[i];
    s[i] = s[end - (i - start)];
    s[end - (i - start)] = temp;
  }
}

long long Fibonacci(int n) {
  if (n == 1 || n == 2) {
    return 1;
  }
  return Fibonacci(n - 1) + Fibonacci(n - 2);
}

bool isPrime(int n) {
  if (n <= 1) {
    return false;
  }
  for (int i = 2; i <= sqrt(n); i++) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}

int nearestPrime(int n) {
  while (!isPrime(++n))
    ;
  return n;
}

bool isAlphabet(string s) {
  for (int i = 0; i < (int)s.size(); i++) {
    if (tolower(s[i]) < 'a' || tolower(s[i]) > 'z') {
      return false;
    }
  }
  return true;
}

void levelUp(Doctor &doctor, int exp) {
  doctor.EXP += exp;
  while (doctor.EXP >= 100) {
    if (doctor.LV == 10) {
      doctor.EXP = 100;
      break;
    } else {
      doctor.LV += 1;
      doctor.EXP -= 100;

      if (doctor.maxHP + 50 >= 999) {
        doctor.maxHP = 999;
      } else {
        doctor.maxHP += 50;
      }

      if (doctor.HP + 10 >= 999) {
        doctor.HP = 999;
      } else {
        doctor.HP += 10;
      }
    }
  }
}

void practice(Doctor &doctor, int ith_event, int LVo, int exp,
              double baseDamage) {
  if (doctor.wong.isCalled) {
    if (doctor.wong.isReal && doctor.wong.times_help >= 1) {
      doctor.wong.times_help--;
      return levelUp(doctor, exp);
    } else if (!doctor.wong.isReal && doctor.wong.times_help >= 1) {
      doctor.HP -= (int)(baseDamage * LVo * 10);
      doctor.wong.times_help--;
      return;
    }
  }

  if (doctor.LV > LVo) {
    levelUp(doctor, exp);
  } else if (doctor.LV == LVo) {
    levelUp(doctor, exp / 2);
  } else {
    if (doctor.levitation.isWearing && doctor.levitation.isReal) {
      if (doctor.levitation.times_resist >= 1) {
        int G_y = (ith_event + nearestPrime(doctor.HP)) % 100;
        std::cout << "nearestPrime: " << nearestPrime(doctor.HP) << std::endl;
        std::cout << "G_y: " << G_y << std::endl;
        std::cout << "LVo: " << LVo << std::endl;
        std::cout << "baseDamage: " << baseDamage << std::endl;
        std::cout << "damage: "
                  << (int)(baseDamage * LVo * 10) * (100 - G_y) / 100
                  << std::endl;
        doctor.HP -= (int)((baseDamage * LVo * 10) * (100 - G_y) / 100);
        doctor.levitation.times_resist--;
      } else {
        doctor.HP -= (int)(baseDamage * LVo * 10);
      }
    } else {
      doctor.HP -= (int)(baseDamage * LVo * 10);
    }
  }
}

void countWitchcraft(string spell, string witchcraft, int index_spell,
                     int index_witchcraft, int &result) {
  if (index_witchcraft == (int)witchcraft.size()) {
    result += 10;
    return;
  }
  for (int i = index_spell; i < (int)spell.size(); i++) {
    if (tolower(spell[i]) == tolower(witchcraft[index_witchcraft])) {
      countWitchcraft(spell, witchcraft, i + 1, index_witchcraft + 1, result);
    }
  }
}

int handleEvents(string &HP, string &LV, string &EXP, string &TS,
                 string &events) {
  /// Students have to complete this function and DO NOT modify any parameters
  /// in this function.
  Doctor doctor(stoi(HP), stoi(HP), stoi(LV), stoi(EXP), stoi(TS));

  if (events[0] == '!') {
    int start = 1;

    int ith_event = 0;
    while (start != (int)events.size()) {

      if (!doctor.throwback.isActivate) {
        // save event index for time throwback
        doctor.throwback.return_at_event = start;
      }

      string event = Tokenize(events, start, "#");
      cout << "event: " << event << "\n";
      int event_num = stoi(event.substr(0, (int)event.find(" ", 0)));
      ith_event += 1;

      int b = ith_event % 10;
      int LVo = ith_event > 6 ? (b > 5 ? b : 5) : b;

      if (doctor.mushroom.isEating) {
        if (doctor.mushroom.times_poisoned >= 1) {
          doctor.LV -= 2;

          if (doctor.LV < 3) {
            doctor.LV = 1;
          }
          doctor.mushroom.times_poisoned--;
        }
      }

      if (doctor.levitation.isWearing && doctor.levitation.isReal) {
        if (doctor.levitation.times_resist <= 0) {
          //  the cloak is incapacitated
          if (doctor.LV < 3) {
            doctor.LV = 1;
          }
        }
      }

      switch (event_num) {
      case 1: {
        practice(doctor, ith_event, LVo, 10, 1.5);
        break;
      }
      case 2: {
        practice(doctor, ith_event, LVo, 20, 2.5);
        break;
      }
      case 3: {
        practice(doctor, ith_event, LVo, 40, 4.5);
        break;
      }
      case 4: {
        practice(doctor, ith_event, LVo, 50, 7.5);
        break;
      }
      case 5: {
        practice(doctor, ith_event, LVo, 70, 9.5);
        break;
      }
      case 6: {
        int index = (int)event.find(" ", 0) + 1;

        if ((int)event.find(" ", index) != -1) {
          return -1;
        }

        string spells = event.substr(index);

        if (!isAlphabet(spells)) {
          return -1;
        }

        bool isActivate = false;

        int F_x = ith_event + (int)spells.size();

        int winning_rate = 0, blood_loss_reduction_rate = 0;
        countWitchcraft(spells, "attack", 0, 0, winning_rate);
        countWitchcraft(spells, "defense", 0, 0, blood_loss_reduction_rate);

        if (doctor.levitation.isWearing && doctor.levitation.isReal) {
          if (doctor.levitation.times_resist >= 1) {
            int G_y = (ith_event + nearestPrime(doctor.HP)) % 100;
            // std::cout << "nearestPrime: " << nearestPrime(doctor.HP) <<
            // std::endl;
            std::cout << "G_y: " << G_y << std::endl;
            winning_rate += G_y;
            blood_loss_reduction_rate += G_y;
            isActivate = true;
          }
        }

        std::cout << "winning_rate: " << winning_rate << std::endl;
        std::cout << "blood_loss_reduction_rate: " << blood_loss_reduction_rate
                  << std::endl;

        if (winning_rate > F_x) {
          levelUp(doctor, 200);
          doctor.TS += 1;
          if (doctor.TS > 5) {
            doctor.TS = 5;
          }
        } else {
          if (blood_loss_reduction_rate < 100) {
            if (doctor.HP < 100) {
              doctor.HP = 0;
              return -1;
            }
            doctor.HP -= doctor.HP * (100 - blood_loss_reduction_rate) / 100;

            if (isActivate) {
              doctor.levitation.times_resist--;
            }
          }
        }

        break;
      }
      case 7: {
        if (doctor.wanda.deprive_levitation) {
          break;
        }

        if (!doctor.levitation.isWearing) {
          doctor.LV += 2;
          if (doctor.LV > 10) {
            doctor.LV = 10;
          }

          doctor.wong.kill();

          doctor.levitation.summon();
        } else if (!doctor.levitation.isReal) {
          doctor.levitation.isReal = true;
          doctor.wong.kill();
        }
        break;
      }
      case 8: {
        if (!doctor.wong.isCalled) {
          doctor.wong.init();
        } else {
          // init fake wong
          if (doctor.wong.times_help < 1) {
            doctor.wong.initFake();

            if (doctor.levitation.isWearing) {
              doctor.levitation.isReal = false;
              doctor.wong.times_help--;
            }

            break;
          }

          // kill fake Wong even if he is fake wong
          if (doctor.wong.times_help >= 1) {
            doctor.wong.times_help--;
          }
        }
        break;
      }
      case 9: {
        doctor.HP = doctor.maxHP;
        doctor.wong.kill();
        doctor.levitation.summon();
        doctor.mushroom.inactivate();
        break;
      }
      case 10: {
        doctor.HP += Fibonacci(doctor.HP - 1);
        if (doctor.HP > doctor.maxHP) {
          doctor.HP = doctor.maxHP;
        }
        break;
      }
      case 11: {
        // real Wong helps doctor
        if (!doctor.wong.returnToKamarTaj(doctor.wong.isReal)) {
          doctor.wong.times_help--;
          break;
        }

        if (!doctor.mushroom.isEating) {
          doctor.mushroom.activate();
        }

        // unreal Wong ignores doctor
        if (!doctor.wong.returnToKamarTaj(!doctor.wong.isReal)) {
          doctor.wong.times_help--;
        }

        doctor.HP -= 50;
        break;
      }
      case 12: {
        int st = (int)event.find(" ", 0);
        if (st == -1) {
          return -1;
        }
        st += 1;

        string code_1 = anotherTokenize(event, st);
        string code_2 = anotherTokenize(event, st);

        if (!isAlphabet(code_1) || !isAlphabet(code_2)) {
          return -1;
        }

        if (code_1 == "" || code_2 == "" || code_1 == event ||
            code_2 == event || code_2 == code_1) {
          return -1;
        }

        if ((int)code_1.size() <= 2) {
          break;
        }

        reverse(code_1, 0, (int)code_1.size() / 2 - 1);
        reverse(code_1, (int)code_1.size() / 2 + 1, (int)code_1.size() - 1);

        int ith = (int)code_2.size() % 10;
        for (int i = 0; i < (int)code_1.size(); i++) {
          if (('a' <= code_1[i] && code_1[i] <= 'z' &&
               (int)code_1[i] + ith > 122) ||
              ('A' <= code_1[i] && code_1[i] <= 'Z' &&
               (int)code_1[i] + ith > 90)) {
            code_1[i] = (char)((int)code_1[i] + ith - 26);
          } else {
            code_1[i] = (char)((int)code_1[i] + ith);
          }
        }

        if (code_1.find(code_2) != -1) {
          // the decryption is successful
          doctor.HP -= doctor.HP * 0.1;
          doctor.maxHP -= doctor.maxHP * 0.1;
          doctor.wanda.is_negotiation = true;
          levelUp(doctor, 30);
        } else {
          // the decryption is fail
          doctor.wanda.activate(doctor.TS);
          doctor.levitation.isWearing = false;
          doctor.TS = 0;
          levelUp(doctor, 15);
        }
        break;
      }
      case 13: {
        int st = (int)event.find(" ", 0);
        if (st == -1) {
          return -1;
        }
        st += 1;

        int **matrix = new int *[ROW] { nullptr };
        if (initMatrix(matrix, event, st)) {
          // for (int i = 0; i < ROW; i++) {
          //   for (int j = 0; j < COLUMN; j++) {
          //     cout << matrix[i][j] << " ";
          //   }
          //   cout << "\n";
          // }
          int min_sum = INT16_MIN, min_index_row = -1, min_index_column = -1;
          int m = (ith_event % 7) > 2 ? (ith_event % 7) : 2;
          std::cout << "m: " << m << std::endl;
          for (int i = ROW - m; i >= 0; i--) {
            for (int j = COLUMN - m; j >= 0; j--) {
              int sum = 0;
              for (int q = i; q < i + m; q++) {
                for (int p = j; p < j + m; p++) {
                  sum += matrix[q][p];
                }
              }
              if (min_sum == INT16_MIN || min_sum >= sum) {
                min_sum = sum;
                min_index_column = j;
                min_index_row = i;
              }
            }
          }

          std::cout << "min_sum: " << min_sum << " " << min_index_row << " "
                    << min_index_column << std::endl;
          if (matrixIncrease(matrix, m, min_index_row, min_index_column)) {
            // the defense against Wanda’s attack is successful
            doctor.HP += min_sum * (min_index_row + 1 + min_index_column + 1);
            if (doctor.HP > doctor.maxHP) {
              doctor.HP = doctor.maxHP;
            }

            // need to fixed Wanda will turn to using gaslighting
          } else {
            // the defense against Wanda’s attack is fail
            doctor.HP -= min_sum * (min_index_row + 1 + min_index_column + 1);
            if (doctor.HP <= 0) {
              if (doctor.wanda.is_negotiation) {
                if (!doctor.wanda.chances_to_kill) {
                  doctor.HP = 1;
                }
              }
            }
          }
          deleteMatrix(matrix);
        } else {
          deleteMatrix(matrix);
          return -1;
        }
        break;
      }
      case 14: {
        int st = (int)event.find(" ", 0);
        if (st == -1) {
          return -1;
        }
        st += 1;

        string key = anotherTokenize(event, st);
        std::cout << "key: " << key << std::endl;

        if (doctor.gates.initGates(event, st)) {
          // for (int i = 0; i < doctor.gates.size; i++) {
          //   std::cout << doctor.gates.array[i] << " ";
          // }
          // cout << "size: " << doctor.gates.size << "\n";
          int wanda_moves = log2(doctor.gates.size);
          // cout << "wanda_moves: " << wanda_moves << "\n";
          int total_moves = 0;

          if (doctor.wanda.findExitWay(doctor.gates.array, doctor.gates.size,
                                       stoi(key), wanda_moves, total_moves)) {
            // Wanda can find the exit way
            cout << "total_moves: " << total_moves << "\n";
            doctor.LV = 1;
            doctor.HP = doctor.HP - total_moves * (ith_event % 10) * 7;
          } else {
            // Wanda cannot find the exit way
            levelUp(doctor, 150);
            doctor.TS = doctor.wanda.TS;
            doctor.wanda.inactivate();
          }
        } else {
          return -1;
        }
        break;
      }
      case 15: {
        if (!doctor.throwback.isActivate && doctor.TS >= 1) {
          doctor.throwback.activate(start);
          doctor.initThrowBack(doctor.throwback.maxHP, 10, 100, doctor.TS - 1);
        }
        break;
      }
      }

      if (doctor.LV >= 7) {
        doctor.wong.kill();
      }

      // save HP after doing event for time throwback
      if (doctor.HP <= 0) {
        if (doctor.TS == 0) {
          return -1;
        }

        doctor.wong.kill();
        doctor.mushroom.inactivate();

        doctor.TS -= 1;
        doctor.HP = doctor.maxHP;
      }

      if (!doctor.throwback.isActivate) {
        if (doctor.throwback.maxHP <= doctor.HP) {
          doctor.throwback.maxHP = doctor.HP;
          doctor.throwback.event_has_maxHP = doctor.throwback.return_at_event;
        } else {
          doctor.throwback.return_at_event = doctor.throwback.event_has_maxHP;
        }
      }


      cout << "doctor: " << doctor.maxHP << " " << doctor.HP << " " << doctor.LV
           << " " << doctor.EXP << " " << doctor.TS << "\n";
      cout << "wong: " << doctor.wong.isCalled << " " << doctor.wong.isReal
           << " " << doctor.wong.times_help << "\n";
      cout << "levitation: " << doctor.levitation.isWearing << " "
           << doctor.levitation.isReal << " " << doctor.levitation.times_resist
           << '\n';
      cout << "mushroom: " << doctor.mushroom.isEating << " "
           << doctor.mushroom.times_poisoned << '\n';
      cout << "wanda: " << doctor.wanda.deprive_levitation << " "
           << doctor.wanda.chances_to_kill << '\n';
      cout << "timethrowback: " << doctor.throwback.event_has_maxHP << " "
           << doctor.throwback.maxHP << " " << doctor.throwback.isActivate
           << '\n'
           << '\n';
    }
    return doctor.HP + doctor.LV + doctor.EXP + doctor.TS;
  }
  return -1;
}

/// END OF STUDENT'S ANSWER

#endif /* studyInPink_h */
