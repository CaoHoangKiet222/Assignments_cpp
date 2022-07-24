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
bool isNumber(string s);

struct Levitation {
  int times_resist;
  bool isReal;
  bool isWearing;
  int need_to_increase;

  Levitation()
      : times_resist(0), need_to_increase(2), isReal(false), isWearing(false) {}

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
  bool negotiation;
  int TS;

  Wanda()
      : deprive_levitation(false), chances_to_kill(false), negotiation(false){};

  void activate(int TS) {
    this->deprive_levitation = true;
    this->chances_to_kill = true;
    this->negotiation = true;
    this->TS = TS;
  }

  void inactivate() {
    this->deprive_levitation = false;
    this->chances_to_kill = false;
    this->negotiation = false;
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
    this->array = new int[(int)s.size()];
    // cout << "s: " << s << "\n";

    while (s.find(" ", start) != -1) {
      string value = anotherTokenize(s, start);
      if (value == "" || !isNumber(value)) {
        return false;
      }
      this->array[this->size++] = stoi(value);
    }

    string value = s.substr(start);
    if (value == "" || !isNumber(value)) {
      return false;
    }
    this->array[this->size++] = stoi(value);

    if (this->size == 0) {
      return false;
    }

    return true;
  }

  bool areDecrease() {
    for (int i = 0; i < this->size - 1; i++) {
      if (this->array[i] < this->array[i + 1]) {
        return false;
      }
    }

    return true;
  }

  ~Gates() {
    delete[] this->array;
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

struct KamarTaj {
  int **matrix;

  bool initMatrix(string s, int &start) {
    this->matrix = new int *[ROW] { nullptr };
    int pre_start = -1, size = 0;

    for (int i = 0; i < ROW; i++) {
      this->matrix[i] = new int[COLUMN];
      for (int j = 0; j < COLUMN; j++) {
        pre_start = start;
        string num = anotherTokenize(s, start);
        if (num == "" || !isNumber(num) ||
            (pre_start == start && size != ROW * COLUMN - 1)) {
          return false;
        }
        this->matrix[i][j] = stoi(num);
        size++;
      }
    }

    // matrix oversize
    if ((int)s.find(" ", pre_start) != -1) {
      return false;
    }

    return true;
  }

  bool matrixIncrease(int m, int index_row, int index_column) {
    for (int i = index_column; i < index_column + m; i++) {
      for (int j = index_row; j < index_row + m - 1; j++) {
        if (this->matrix[j][i] < this->matrix[j + 1][i]) {
          return false;
        }
      }
    }
    return true;
  }

  ~KamarTaj() {
    for (int i = 0; i < ROW; i++) {
      delete[] this->matrix[i];
      this->matrix[i] = nullptr;
    }
    delete[] this->matrix;
    this->matrix = nullptr;
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

void reverse(string &s, int start, int end) {
  for (int i = start; i < (end + start) / 2; i++) {
    char temp = s[i];
    s[i] = s[end - (i - start)];
    s[end - (i - start)] = temp;
  }
}

double long Fibonacci(int n) {
  if (n == 1 || n == 2) {
    return 1;
  }

  int i = 3;
  double long F_1 = 1, F_2 = 1, F_i;

  while (i != n + 1) {
    F_i = F_2 + F_1;
    F_1 = F_2;
    F_2 = F_i;
    i++;
  }
  return F_i;
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

bool isNumber(string s) {
  for (int i = 0; i < (int)s.size(); i++) {
    if (s[i] > '9' || s[i] < '0') {
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
        // std::cout << "nearestPrime: " << nearestPrime(doctor.HP) <<
        // std::endl; std::cout << "G_y: " << G_y << std::endl; std::cout <<
        // "LVo: " << LVo << std::endl; std::cout << "baseDamage: " <<
        // baseDamage << std::endl; std::cout << "damage: "
        //           << (int)(baseDamage * LVo * 10) * (100 - G_y) / 100
        //           << std::endl;
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
  if ((doctor.HP > 999 || doctor.HP <= 0) ||
      (doctor.LV > 10 || doctor.LV < 1) ||
      (doctor.EXP > 100 || doctor.EXP < 0) || (doctor.TS > 5 || doctor.TS < 0))
    return -1;

  if (events[0] == '!' && events[(int)events.size() - 1] == '!') {
    int start = 1, ith_event = 0;

    while (start != (int)events.size()) {
      if (!doctor.throwback.isActivate) {
        // save event index for time throwback
        doctor.throwback.return_at_event = start;
      }

      if (!doctor.throwback.isActivate) {
        if (doctor.throwback.maxHP <= doctor.HP) {
          doctor.throwback.maxHP = doctor.HP;
          doctor.throwback.event_has_maxHP = doctor.throwback.return_at_event;
        } else {
          doctor.throwback.return_at_event = doctor.throwback.event_has_maxHP;
        }
      }

      string event = Tokenize(events, start, "#");
      cout << "event: " << event << "\n";
      string event_num_str;

      if ((event_num_str = event.substr(0, (int)event.find(" ", 0))) == "" ||
          !isNumber(event_num_str)) {
        return -1;
      }

      int event_num = stoi(event_num_str);
      ith_event += 1;

      int b = ith_event % 10;
      int LVo = ith_event > 6 ? (b > 5 ? b : 5) : b;

      if (doctor.mushroom.isEating) {
        if (doctor.mushroom.times_poisoned >= 1) {
          doctor.mushroom.times_poisoned--;
        } else {
          doctor.LV += 2;
          if (doctor.LV > 10) {
            doctor.LV = 10;
          }
          doctor.mushroom.isEating = false;
        }
      }

      if (doctor.levitation.isWearing && doctor.levitation.isReal) {
        if (doctor.levitation.times_resist <= 0) {
          //  the cloak is incapacitated
          if (doctor.LV < 3) {
            doctor.LV = 1;
          } else {
            doctor.LV -= doctor.levitation.need_to_increase;
          }
        }
      }

      switch (event_num) {
      case 1: {
        if ((int)event.find(" ") != -1)
          return -1;

        practice(doctor, ith_event, LVo, 10, 1.5);
        break;
      }
      case 2: {
        if ((int)event.find(" ") != -1)
          return -1;

        practice(doctor, ith_event, LVo, 20, 2.5);
        break;
      }
      case 3: {
        if ((int)event.find(" ") != -1)
          return -1;

        practice(doctor, ith_event, LVo, 40, 4.5);
        break;
      }
      case 4: {
        if ((int)event.find(" ") != -1)
          return -1;

        practice(doctor, ith_event, LVo, 50, 7.5);
        break;
      }
      case 5: {
        if ((int)event.find(" ") != -1)
          return -1;

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
            // std::cout << "G_y: " << G_y << std::endl;
            winning_rate += G_y;
            blood_loss_reduction_rate += G_y;
            isActivate = true;
          }
        }

        // std::cout << "winning_rate: " << winning_rate << std::endl;
        // std::cout << "blood_loss_reduction_rate: " <<
        // blood_loss_reduction_rate
        //           << std::endl;

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
        if ((int)event.find(" ") != -1)
          return -1;

        if (doctor.wanda.deprive_levitation) {
          break;
        }

        if (!doctor.levitation.isWearing) {
          if (doctor.LV == 9) {
            doctor.levitation.need_to_increase = 1;
          } else if (doctor.LV == 10) {
            doctor.levitation.need_to_increase = 0;
          } else {
            doctor.levitation.need_to_increase = 2;
          }

          doctor.LV += doctor.levitation.need_to_increase;
          if (doctor.LV > 10) {
            doctor.LV = 10;
          }

          doctor.wong.kill();

          doctor.levitation.summon();
        } else if (!doctor.levitation.isReal) {
          doctor.levitation.isReal = true;
          doctor.LV += doctor.levitation.need_to_increase;
        }

        break;
      }
      case 8: {
        if ((int)event.find(" ") != -1)
          return -1;

        if (!doctor.wong.isCalled) {
          doctor.wong.init();
        } else {
          // init fake wong
          if (doctor.wong.times_help < 1) {
            doctor.wong.initFake();

            if (doctor.LV >= 7) {
              doctor.wong.kill();
              break;
            }

            if (doctor.levitation.isWearing) {
              doctor.levitation.isReal = false;

              if (doctor.LV < 3) {
                doctor.LV = 1;
              } else {
                doctor.LV -= doctor.levitation.need_to_increase;
              }

              doctor.wong.times_help--;
            }

            // if (doctor.LV >= 7 && doctor.wong.isCalled &&
            // !doctor.wong.isReal) {
            //   doctor.wong.kill();
            //     doctor.LV -= doctor.levitation.need_to_increase;
            // }

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
        if ((int)event.find(" ") != -1)
          return -1;

        doctor.HP = doctor.maxHP;
        doctor.wong.kill();
        doctor.levitation.summon();
        doctor.mushroom.inactivate();
        break;
      }
      case 10: {
        if ((int)event.find(" ") != -1)
          return -1;

        // std::cout << "Fibonacci: " << Fibonacci(doctor.HP - 1) << std::endl;
        doctor.HP += Fibonacci(doctor.HP - 1);
        if (doctor.HP > doctor.maxHP) {
          doctor.HP = doctor.maxHP;
        }
        break;
      }
      case 11: {
        if ((int)event.find(" ") != -1)
          return -1;

        // real Wong helps doctor
        if (!doctor.wong.returnToKamarTaj(doctor.wong.isReal)) {
          doctor.wong.times_help--;
          break;
        }

        if (!doctor.mushroom.isEating) {
          doctor.mushroom.activate();

          if (doctor.LV < 3) {
            doctor.LV = 1;
          } else {
            doctor.LV -= 2;
          }
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
          doctor.wanda.negotiation = true;
          levelUp(doctor, 30);
        } else {
          // the decryption is fail
          doctor.wanda.activate(doctor.TS);
          doctor.levitation.isWearing = false;

          if (doctor.LV < 3) {
            doctor.LV = 1;
          } else {
            doctor.LV -= doctor.levitation.need_to_increase;
          }

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

        // unreal wong need to be removed
        if (doctor.wong.isCalled && !doctor.wong.isReal) {
          doctor.wong.kill();
        }

        KamarTaj kamar_taj;
        if (kamar_taj.initMatrix(event, st)) {
          // for (int i = 0; i < ROW; i++) {
          //   for (int j = 0; j < COLUMN; j++) {
          //     cout << kamar_taj.matrix[i][j] << " ";
          //   }
          //   cout << "\n";
          // }
          int min_sum = INT16_MIN, min_index_row = -1, min_index_column = -1;
          int m = (ith_event % 7) > 2 ? (ith_event % 7) : 2;
          // std::cout << "m: " << m << std::endl;
          for (int i = ROW - m; i >= 0; i--) {
            for (int j = COLUMN - m; j >= 0; j--) {
              int sum = 0;
              for (int q = i; q < i + m; q++) {
                for (int p = j; p < j + m; p++) {
                  sum += kamar_taj.matrix[q][p];
                }
              }
              if (min_sum == INT16_MIN || min_sum >= sum) {
                min_sum = sum;
                min_index_column = j;
                min_index_row = i;
              }
            }
          }

          // std::cout << "min_sum: " << min_sum << "\n"
          //           << "index_row: " << min_index_row << "\n"
          //           << "index_column: " << min_index_column << std::endl;
          if (kamar_taj.matrixIncrease(m, min_index_row, min_index_column)) {
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
              if (doctor.wanda.negotiation) {
                if (!doctor.wanda.chances_to_kill) {
                  doctor.HP = 1;
                }
              }
            }
          }
        } else {
          return -1;
        }
        break;
      }
      case 14: {
        int st = (int)event.find(" ", 0);
        if (st == -1 || (int)event.find(" ", ++st) == -1) {
          return -1;
        }

        string key = anotherTokenize(event, st);
        // std::cout << "key: " << key << std::endl;
        if (!isNumber(key)) {
          return -1;
        }

        // unreal wong need to be removed
        if (doctor.wong.isCalled && !doctor.wong.isReal) {
          doctor.wong.kill();
        }

        if (doctor.gates.initGates(event, st)) {
          if (!doctor.gates.areDecrease()) {
            return -1;
          }
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
            // cout << "total_moves: " << total_moves << "\n";
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
        if ((int)event.find(" ") != -1)
          return -1;

        if (!doctor.throwback.isActivate && doctor.TS >= 1) {
          if (doctor.throwback.maxHP > doctor.HP) {
            doctor.throwback.activate(start);
            doctor.initThrowBack(doctor.throwback.maxHP, 10, 100,
                                 doctor.TS - 1);
          }
        }
        break;
      }
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

      cout << "doctor: " << doctor.maxHP << " " << doctor.HP << " " << doctor.LV
           << " " << doctor.EXP << " " << doctor.TS << "\n";
      cout << "wong: " << doctor.wong.isCalled << " " << doctor.wong.isReal
           << " " << doctor.wong.times_help << "\n";
      cout << "levitation: " << doctor.levitation.isWearing << " "
           << doctor.levitation.isReal << " " << doctor.levitation.times_resist
           << " " << doctor.levitation.need_to_increase << '\n';
      cout << "mushroom: " << doctor.mushroom.isEating << " "
           << doctor.mushroom.times_poisoned << '\n';
      cout << "wanda: " << doctor.wanda.deprive_levitation << " "
           << doctor.wanda.chances_to_kill << " " << doctor.wanda.negotiation
           << " " << doctor.wanda.TS << '\n';
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
