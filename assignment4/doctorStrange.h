//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef doctorStrange_h
#define doctorStrange_h

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

/// STUDENT'S ANSWER BEGINS HERE
/// STUDENTS CAN ADD HELPER FUNCTIONS
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

struct Doctor {
  int HP;
  int maxHP;
  int LV;
  int EXP;
  int TS;
  Levitation levitation;

  Doctor(int hp, int maxHp, int lv, int exp, int ts)
      : HP(hp), maxHP(maxHp), LV(lv), EXP(exp), TS(ts){};
};

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
  if (doctor.LV > LVo) {
    levelUp(doctor, exp);
  } else if (doctor.LV == LVo) {
    levelUp(doctor, exp / 2);
  } else {
    if (doctor.levitation.isWearing) {
      cout << "isWearing" << doctor.levitation.isWearing << '\n';
      cout << "times_resist: " << doctor.levitation.times_resist << '\n';
      if (doctor.levitation.times_resist >= 1) {
        int G_y = (ith_event + nearestPrime(doctor.HP)) % 100;
        // cout << "G_y" << G_y << "\n";
        doctor.HP -= (baseDamage * LVo * 10) * (100 - G_y) / 100;
        doctor.levitation.times_resist--;
      } else {
        //  the cloak is incapacitated
        if (doctor.LV < 3) {
          doctor.LV = 1;
        }
      }
    } else {
      doctor.HP -= baseDamage * LVo * 10;
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
      string event = Tokenize(events, start, "#");
      int event_num = stoi(event.substr(0, (int)event.find(" ", 0)));
      ith_event += 1;

      int b = ith_event % 10;
      int LVo = ith_event > 6 ? (b > 5 ? b : 5) : b;

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
        string spells = event.substr(index);

        int F_x = ith_event + (int)spells.size();

        int winning_rate = 0, blood_loss_reduction_rate = 0;
        countWitchcraft(spells, "attack", 0, 0, winning_rate);
        countWitchcraft(spells, "defense", 0, 0, blood_loss_reduction_rate);

        if (doctor.levitation.isWearing) {
          if (doctor.levitation.times_resist >= 1) {
            int G_y = (ith_event + nearestPrime(doctor.HP)) % 100;
            winning_rate += G_y;
            blood_loss_reduction_rate += G_y;
          }
        }

        if (winning_rate > F_x) {
          levelUp(doctor, 200);
          doctor.TS += 1;
        } else {
          if (blood_loss_reduction_rate < 100) {
            if (doctor.HP < 100) {
              doctor.HP = 0;
              return -1;
            }
            doctor.HP -= doctor.HP * (100 - blood_loss_reduction_rate) / 100;
            doctor.levitation.times_resist--;
            // need to fixed
          }
        }

        break;
      }
      case 7: {
        if (!doctor.levitation.isWearing) {
          doctor.LV += 2;
          if (doctor.LV > 10) {
            doctor.LV = 10;
          }
          doctor.levitation.summon();
          break;
        }
      }
      }
      cout << doctor.HP << " " << doctor.maxHP << " " << doctor.LV << " "
           << doctor.EXP << " " << doctor.TS << "\n";

      if (doctor.HP <= 0) {
        if (doctor.TS == 0) {
          return -1;
        }
        doctor.TS -= 1;
        doctor.HP = doctor.maxHP;
      }
    }
    return doctor.HP + doctor.LV + doctor.EXP + doctor.TS;
  }
  return -1;
}

/// END OF STUDENT'S ANSWER

#endif /* studyInPink_h */
