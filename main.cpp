#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#define PI 3.14159

using namespace std;

enum Team
{
    orange = 1,
    purple = -1
};

class Position
{
public:
  Position(int x = 0, int y = 0, int vx = 0, int vy = 0)
  {
    m_x = x;
    m_y = y;
    m_vx = vx;
    m_vy = vy;
  }

  ~Position()
  {

  }

  void set_pos(int x, int y, int vx = 0, int vy = 0)
  {
    m_x = x;
    m_y = y;
    m_vx = vx;
    m_vy = vy;
  }

  void set_x(int x)
  {
    m_x = x;
  }

  void set_y(int y)
  {
    m_y = y;
  }

  int get_x()
  {
      return m_x;
  }

  int get_y()
  {
      return m_y;
  }

  int get_vx()
  {
    return m_vx;
  }

  int get_vy()
  {
    return m_vy;
  }

  // Return the estimate position in N steps
  Position get_n_step_pos(int n)
  {
    Position pos;
    //cerr << "get n step pos res :" << m_x + n*m_vx << " " << m_y + n*m_vy << endl;
    pos.set_pos(m_x + n*m_vx, m_y + n*m_vy, m_vx, m_vy);
    //cerr << "get n step pos res :" << pos.m_x << " " << pos.m_y << endl;
    return pos;
  }

  // Return the distance between two point in N steps
  float n_step_distance_from(int n, Position pos)
  {
    return sqrt(pow(m_x + n*m_vx - n*pos.get_vx() - pos.get_x(), 2) + pow(m_y + n*m_vy - n*pos.get_vy() - pos.get_y(), 2));
  }

  // Return the closest Position in N steps
  Position get_n_step_closest(int n, Position a, Position b)
  {
    if(this->n_step_distance_from(n, a) < this->n_step_distance_from(n, b))
      return a;
    else
      return b;
  }
protected:
  int m_x;
  int m_y;
  int m_vx;
  int m_vy;
};

class Flag : public Position
{
public:
    Flag()
    {
        m_team = "none";
    }

    ~Flag()
    {

    }

    void set_info()
    {
        int x, y;
        cin >> m_x >> m_y;
        cin.ignore();
        if(m_team == "none")
            if(m_x <= 1000)
                m_team = "orange";
            else
                m_team = "purple";

        if(m_x ==-1 && m_y == -1)
            m_in_base = false;
        else
            m_in_base = true;
    }

    string get_team()
    {
        return m_team;
    }

    bool is_in_base()
    {
        return m_in_base;
    }

protected:
    string m_team;

    bool m_in_base;
};

class Ovni : public Position
{
public:
    Ovni()
    {
        m_team = "none";
        m_power = "0";
    }

    ~Ovni()
    {

    }

    void set_info()
    {
        cin >> m_x >> m_y >> m_vx >> m_vy >> m_flag;
        
        m_busy = false;

        cin.ignore();
    }

    void set_team(string team)
    {
        if(m_team == "none")
            m_team = team;
    }

    bool has_flag()
    {
        return m_flag;
    }
    
    bool is_busy()
    {
        return m_busy;
    }

    void command()
    {
        cout << m_target.get_x() << ' ' << m_target.get_y() << ' ' << m_power << endl;
    }

    // Regulate the position of the ovni on its target
    void regulate_speed()
    {
        int speed = 100;

        float d = this->n_step_distance_from(0, m_target);
        float d_1s = this->n_step_distance_from(1,m_target);

        if(d < 100 && m_vx < 10 && m_vy << 10)
            speed = 0;
        else
            if(d_1s < 200)
            {
                m_target.set_pos(m_target.get_x() - 100*m_vx, m_target.get_y() - 100*m_vy);
                speed = 100;
            }
            else
            {
                if(d < 1000)
                {
                    speed = d/100;
                }
                if(speed < 50)
                    speed = 50;
            }
        m_power = to_string(speed);
    }

    void chase(Position enemy)
    {
        m_busy = true;
        if(m_team == "orange")
        {
            //if(m_x + 500< enemy.get_x())
              m_target = enemy.get_n_step_pos(2);
            //else
            //  m_target = enemy.get_n_step_pos(0);

            // TODO Bounce on border
        }
        else
        {
          //if(m_x  - 500 > enemy.get_x())
            m_target = enemy.get_n_step_pos(2);
          //else
            //m_target = enemy.get_n_step_pos(0);

            // TODO Bounce on border
        }

        m_power = "BOOST";
    }

    void return_flag(Ovni enemy_1, Ovni enemy_2)
    {
        m_busy = true;
        float theta;
        Position enemy;

        if(m_team == "orange")
        {
            // If we are going in enemy home, bounce on the wall
            if(m_vx > 0)
            {
              m_target = this->get_n_step_pos(10);
            }
            // Else go back to home
            else
              m_target.set_x(0);
        }
        else
        {
              // If we are going in enemy home, bounce on the wall
             if(m_vx < 0)
            {
                m_target = this->get_n_step_pos(10);
            }
            // Else go back to home
            else
                m_target.set_x(10000);
        }

        enemy = this->get_n_step_closest(1, enemy_1, enemy_2);

        // If there is an enemy close, go in the other way
        if(this->n_step_distance_from(1, enemy) < 500)
          m_target.set_y(m_y - 4*enemy.get_vy());
        // Else follow the same way
        else
          m_target.set_y(m_y + 4*m_vy);

        m_power = "100";
    }

    void attack_flag(Flag enemy_flag)
    {
        m_busy = true;
      // Go catch the flag
      m_target = enemy_flag;
      if(this->n_step_distance_from(1, enemy_flag) > 8000)
        m_power = "100";
      else
        m_power = "BOOST";
    }

    void defend_flag(Flag ally_flag)
    {
        m_busy = true;
        m_target = ally_flag;
        this->regulate_speed();
    }

    void block_flag(Flag ally_flag, Ovni enemy)
    {
        m_busy = true;
        m_target.set_pos(m_x + 5*(enemy.get_x() - m_x), m_y + 5*(enemy.get_y() - m_y));
        m_power = "100";
    }

    void debug()
    {
        cerr << "x: " << m_x << ", y: " << m_y << ", vx: " << m_vx << ", vy: " << m_vy << endl;
    }

protected:
 bool m_flag;

 bool m_busy;

 string m_team;

 Position m_target;
 string m_power;

};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    Ovni ally_1, ally_2, enemy_1, enemy_2, enemy;
    Ovni* ally;

    Flag ally_flag, enemy_flag;

// game loop
    while (1) {
// Get the data

        // Ally flag
        ally_flag.set_info();

        // Enemy flag
        enemy_flag.set_info();

        // Ally 1
        ally_1.set_info();
        ally_1.set_team(ally_flag.get_team());

        // Ally 2
        ally_2.set_info();
        ally_2.set_team(ally_flag.get_team());

        // Enemy 1
        enemy_1.set_info();
        enemy_1.set_team(enemy_flag.get_team());

        // Enemy 2
        enemy_2.set_info();
        enemy_2.set_team(enemy_flag.get_team());

// Process game

        if(enemy_flag.is_in_base())
        {
            ally = (ally_1.n_step_distance_from(0, enemy_flag) < ally_2.n_step_distance_from(0, enemy_flag)) ? &ally_1 : &ally_2;
            ally->attack_flag(enemy_flag);
        }

        if(ally_flag.is_in_base())
        {
            enemy = (ally_flag.n_step_distance_from(0, enemy_1) < ally_flag.n_step_distance_from(0, enemy_2)) ? enemy_1 : enemy_2;
            ally = (ally_1.n_step_distance_from(0, ally_flag) < ally_2.n_step_distance_from(0, ally_flag)) ? &ally_1 : &ally_2;

            if(enemy.n_step_distance_from(1, ally_flag) > 1000)
                ally->defend_flag(ally_flag);
            else
                ally->block_flag(ally_flag, enemy);
        }
        else
        {
            enemy = (enemy_1.has_flag()) ? enemy_1 : enemy_2;
            ally = (ally_1.n_step_distance_from(0, enemy) < ally_2.n_step_distance_from(0, enemy)) ? &ally_1 : &ally_2;

            ally->chase(enemy);
        }
        
        if(ally_1.has_flag())
                ally_1.return_flag(enemy_1, enemy_2);
        if(ally_2.has_flag())
                ally_2.return_flag(enemy_1, enemy_2);
        
        if(!ally_1.is_busy())
            if(ally_flag.is_in_base())
                ally_1.defend_flag(ally_flag);
            else
            {
                enemy = (enemy_1.has_flag()) ? enemy_1 : enemy_2;
                ally_1.chase(enemy);
            }
            
        if(!ally_2.is_busy())
            if(ally_flag.is_in_base())
                ally_2.defend_flag(ally_flag);
            else
            {
                enemy = (enemy_1.has_flag()) ? enemy_1 : enemy_2;
                ally_2.chase(enemy);
            }
        
// Ouptut command
        // Command ovni 1
        ally_1.command();

        // Command ovni 2
        ally_2.command();
    }
}
