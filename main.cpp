#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#define PI 3.14159

using namespace std;

class Flag
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
    
    int get_x()
    {
        return m_x;
    }
    
    int get_y()
    {
        return m_y;
    }
    
    string get_team()
    {
        return m_team;
    }
    
    bool is_in_base()
    {
        return m_in_base;
    }
    
private:
    int m_x;
    int m_y;
    string m_team;
    
    bool m_in_base;
};

class Ovni
{
public:
    Ovni()
    {
        m_team = "none";
        m_v_err = 0;
    }
    
    ~Ovni()
    {
        
    }
    
    void set_info()
    {
        cin >> m_x >> m_y >> m_vx >> m_vy >> m_flag;
        cin.ignore();
    }
    
    void set_team(string team)
    {
        if(m_team == "none")
            m_team = team;
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
    
    bool has_flag()
    {
        return m_flag;
    }
    
    void set_target(int x, int y, string power)
    {
        if(x >= 0)
            m_x_target = x;
        if(y >= 0)
            m_y_target = y;
        m_power = power;
    }
    
    void command()
    {
        cout << m_x_target << ' ' << m_y_target << ' ' << m_power << endl;
    }
    
    float distance_from(int x, int y)
    {
        return sqrt(pow(m_x - x, 2) + pow(m_y - y, 2));
    }
    
    float n_step_distance_from(int n, int x, int y)
    {
        return sqrt(pow(m_x + n*m_vx- x, 2) + pow(m_y + n*m_vy - y, 2));
    }
    
    float distance_from(Ovni ovni)
    {
        return sqrt(pow(m_x - ovni.get_x(), 2) + pow(m_y - ovni.get_y(), 2));
    }
    
    float n_step_distance_from(int n, Ovni ovni)
    {
        return sqrt(pow(m_x + n*m_vx - n*ovni.get_vx() - ovni.get_x(), 2) + pow(m_y + n*m_vy - n*ovni.get_vy() - ovni.get_y(), 2));
    }
    
    float distance_from(Flag flag)
    {
        return sqrt(pow(m_x - flag.get_x(), 2) + pow(m_y - flag.get_y(), 2));
    }
    
    void regulate_speed()
    {
        int speed = 100;
        
        float d = this->n_step_distance_from(0,m_x_target, m_y_target);
        float d_1s = this->n_step_distance_from(1,m_x_target, m_y_target);
        float d_2s = this->n_step_distance_from(2,m_x_target, m_y_target);
        
        if(d < 100 && m_vx < 10 && m_vy << 10)
            speed = 0;
        else
            if(d_1s < 200)
            {
                m_x_target -= 100*m_vx;
                m_y_target -= 100*m_vy;
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
    
    void chase(Ovni enemy)
    {
        if(m_team == "orange")
        {
            if(m_x < enemy.get_x())
                this->set_target(enemy.get_x() + enemy.get_vx(), enemy.get_y() + enemy.get_vy()*2, "BOOST");
            else
                this->set_target(enemy.get_x(), enemy.get_y(), "BOOST");
            
            // Bounce on border
            if(m_x < 2000 && m_vx < 0)
                this->set_target(m_x + 5*m_vx, m_y +5*m_vy, "BOOST");
        }
        else
        {
            if(m_x > enemy.get_x())
                this->set_target(enemy.get_x() + enemy.get_vx(), enemy.get_y() + enemy.get_vy()*2, "BOOST");
            else
                this->set_target(enemy.get_x(), enemy.get_y(), "BOOST");
                
            // Bounce on border
            if(m_x > 8000 && m_vx > 0)
                this->set_target(m_x + 5*m_vx, m_y +5*m_vy, "BOOST");
        }
    }
    
    void return_flag(Ovni enemy_1, Ovni enemy_2)
    {
        int x_target;
        int y_target = -1;
        float theta;
        
        if(m_team == "orange")
        {
            if(m_vx > 0)
            {
                x_target = m_x + m_vx*10;
                y_target = m_y + m_vy*10;
            }
            else
                x_target = 0;
        }
        else
        {
             if(m_vx < 0)
            {
                x_target = m_x + m_vx*10;
                y_target = m_y + m_vy*10;
            }
            else
                x_target = 10000;
        }
        
        if(y_target == -1)
        {
            float dist_enemy_1, dist_enemy_2, dist_enemy;
            Ovni enemy;
            
            dist_enemy_1 = this->n_step_distance_from(1, enemy_1);
            dist_enemy_2 = this->n_step_distance_from(1, enemy_2);
            
            if(dist_enemy_1 < dist_enemy_2)
            {
                enemy = enemy_1;
                dist_enemy = dist_enemy_1;
            }
            else
            {
                enemy = enemy_2;
                dist_enemy = dist_enemy_2;
            }
            
            if(dist_enemy < 500 && m_vy*enemy.get_y() < 0)
            {
                y_target = m_y - m_vy;
            }
            else
            {
                /*theta = atan2(m_vx, m_vy);
                if(theta > PI/2 && theta < 5*PI/6)
                    theta = 5*PI/6;
                if(theta < -PI/2 && theta > -5*PI/6)
                    theta = -5*PI/6;
                if(theta < PI/2 && theta > PI/6)
                    theta = PI/6;
                if(theta > -PI/2 && theta < -PI/6)
                    theta = -PI/6;
                    
                y_target = m_y - (m_x - x_target)/tan(theta);*/
                x_target = m_x + m_vx*10;
                y_target = m_y + m_vy*7;
            }            
        }
        
        this->set_target(x_target, y_target, "100");
    }
    
    void attack_flag(Flag ally_flag, Flag enemy_flag, Ovni enemy_1, Ovni enemy_2)
    {
        // Check if it has to defend
        if(!ally_flag.is_in_base())
        {
            Ovni enemy;
            // If it is closer to the enemy target the enemy else target the flag
            if(enemy_1.has_flag())
                enemy = enemy_1;
            else
                enemy = enemy_2;
                
            if(this->distance_from(enemy) < this->distance_from(enemy_flag))
                    this->chase(enemy);
                else
                    this->set_target(enemy_flag.get_x(), enemy_flag.get_y(), "BOOST");
        }
        // Atack
        else
        {
            if(this->distance_from(enemy_flag) > 6000)
                this->set_target(enemy_flag.get_x(), enemy_flag.get_y(), "BOOST");
            else
                this->set_target(enemy_flag.get_x(), enemy_flag.get_y(), "100");
        }
    }
    
    void defend_flag(Flag ally_flag, Flag enemy_flag, Ovni enemy_1, Ovni enemy_2)
    {
        // Go on the enemy which has the flag
        Ovni enemy;
        if(!ally_flag.is_in_base())
        {
            if(enemy_1.has_flag())
                enemy = enemy_1;
            else
                enemy = enemy_2;
            
            this->chase(enemy);
        }
        else
        // Go between our flag and the enemy
        {
            Ovni enemy;
            int x_target;
            int y_target;
            
            float dist_enemy_1, dist_enemy_2, dist_enemy;

            x_target = ally_flag.get_x();
            y_target = ally_flag.get_y();
            
            this->set_target(x_target, y_target, "100");
            this->regulate_speed();
            
            dist_enemy_1 = enemy_1.distance_from(ally_flag);
            dist_enemy_2 = enemy_2.distance_from(ally_flag);
            
            if(dist_enemy_1 < dist_enemy_2)
            {
                enemy = enemy_1;
                dist_enemy = dist_enemy_1;
            }
            else
            {
                enemy = enemy_2;
                dist_enemy = dist_enemy_2;
            }
            
            
            if(dist_enemy < 2000)
            {
                //x_target = enemy.get_x() - enemy.get_vx();
                //y_target = enemy.get_y() - enemy.get_vy();
                x_target = m_x + 2*(enemy.get_x() - m_x);
                y_target = m_y + 2*(enemy.get_y() - m_y);
                this->set_target(x_target, y_target, "100");
            }
        }
    }
    
    void debug()
    {
        cerr << "x: " << m_x << ", y: " << m_y << ", vx: " << m_vx << ", vy: " << m_vy << endl;
    }
    
private:
 int m_x;
 int m_y;
 int m_vx;
 int m_vy;
 bool m_flag;
 
 float m_v_err;
 
 string m_team;
 
 int m_x_target;
 int m_y_target;
 string m_power;
    
};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    Ovni ally_1, ally_2, enemy_1, enemy_2;
    
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
            if(ally_1.distance_from(enemy_flag) < ally_2.distance_from(enemy_flag))
            {
                ally_1.attack_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
                ally_2.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
            }
            else
            {
                ally_1.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
                ally_2.attack_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
            }
        }
        else
        {
            if(ally_1.has_flag())
            {
                ally_1.return_flag(enemy_1, enemy_2);
                ally_2.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
            }
            else
            {
                ally_1.defend_flag(ally_flag, enemy_flag, enemy_1, enemy_2);
                ally_2.return_flag(enemy_1, enemy_2);
            }
        }
                
        ally_1.debug();
        
// Ouptut command
        // Command ovni 1
        ally_1.command();
        
        // Command ovni 2
        ally_2.command();
    }
}
