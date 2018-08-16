#include <iostream>
#include <Windows.h>

#include <conio.h>
#include <vector>
using namespace std;

const int NUM_TROPHY = 4;

class point
{
public:
	int x, y;
	point();
	point(int, int);
	void move_right();
	void move_left();
	void move_up();
	void move_down();
	friend bool operator == (point, point);
};
class snake
{
public:
	snake();
	void take_direction();
	void correct_direction();
	void move_snake();
	void grow_snake();
	bool is_snake_intersected() const;
	bool is_snake_at_border() const;
	void make_snake_walkthrough_borders();
	bool is_point_is_of_snake(const point&);
	point get_tail()const;
	point get_head()const;
	const point* pass_snake() const;
	int snake::get_length()const;
private:
	int length;
	int head;
	int tail;
	vector<point> body;
	enum dir{ UP = 'w', DOWN = 's', LEFT = 'a', RIGHT = 'd' };
	dir direction;
	dir exdirection;
};
class trophy
{
	point trph;
	int timer;
	enum state{ ON, OFF };
	state status;
public:
	trophy();
	void change_position();
	point get_coordinates();
	
	virtual int get_value() = 0;
	virtual char sympol() = 0;

	void set_timer(const point& head);
	void run_timer();
	bool is_timer_out();

	void turn_on();
	void turn_off();
	bool is_on();
};
class normal_trophy :public trophy
{
public :
	int get_value();
	char sympol();
};
class golden_trophy :public trophy
{
public:
	int get_value();
	char sympol();
};
class game
{
public:
	game();
	int  get_score();
	void run_level();
private:
	int score;
	int moves;
	void update_score(snake&, trophy*);
	void avoid_intersict_snake_trophy(snake&, trophy*);
	
};
class draw
{
	char map[20][60];
	int mapwidth, mapheight;
	void assign_trophy(trophy *trph);
public :
	draw();
	void assign_snake(const snake& snk);
	void assign_trophy(trophy *trph[]);
	void print()const;
	void erase(point);
	friend class game;
};

int main()
{
	char again = 'y'; int highscore = 0;
	while (again == 'y')
	{
		game x;
		x.run_level();
		if (x.get_score() > highscore)
			highscore = x.get_score();
		cout << "High Score is : " << highscore << endl;
		cout << "Play Again ?(y/n)" << endl;
		again = _getch();
	}
}


bool operator == (point a, point b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	else
		return false;
}
point::point()
{
	x = 0;
	y = 0;
}
point::point(int s, int d)
{
	x = s;
	y = d;
}
void point::move_up()
{
	y--;
}
void point::move_down()
{
	y++;
}
void point::move_right()
{
	x++;
}
void point::move_left()
{
	x--;
}


snake::snake()
{
	length = 4;
	head = 0;
	tail = length - 1;
	
	direction = RIGHT;
	exdirection = RIGHT;
	for (int i = 0; i < length; i++)
	{
		body.push_back(point(30 - i, 9));
	}
}
void snake::take_direction()
{
	if (_kbhit())
	{
		exdirection = direction;
		char x;
		x = _getch();
		if (x == UP)
			direction = UP;
		else if (x == DOWN)
			direction = DOWN;
		else if (x == LEFT)
			direction = LEFT;
		else if (x == RIGHT)
			direction = RIGHT;
	}
	while (_kbhit()) _getch(); // to clear the input buffer 
}
void snake::correct_direction()
{
	if (direction != RIGHT && direction != LEFT && direction != UP && direction != DOWN)
		direction = exdirection;
	else if (exdirection == UP && direction == DOWN)
		direction = exdirection;
	else if (exdirection == DOWN && direction == UP)
		direction = exdirection;
	else if (exdirection == LEFT && direction == RIGHT)
		direction = exdirection;
	else if (exdirection == RIGHT && direction == LEFT)
		direction = exdirection;
}
void snake::move_snake()
{
	for (int i = tail; i > head; i--)
	{
		body[i] = body[i - 1];
	}
	if (direction == 'd') // MOVE RIGHT
	{
		body[head].move_right(); // update the value of x of the head by +1 ,, move point horizontally (y is constant in this specific case while x increases by 1 on scale )
	}
	else if (direction == 's') // MOVE DOWNWARD
	{
		body[head].move_down();
	}
	else if (direction == 'w') //MOVE UPWARD
	{
		body[head].move_up();
	}
	else if (direction == 'a') // MOVE LEFT 
	{
		body[head].move_left();
	}
}
void snake::grow_snake()
{
	point p(body[tail]);
	body.push_back(p);
	length++;
	tail++;
}
bool snake::is_snake_intersected() const
{
	for (int i = 1; i < length; i++)
	{
		if (body[i] == body[head])
		{
			return true;
		}
	}
	return false;
}
bool snake::is_snake_at_border() const
{
	if (body[head].y == 0 || body[head].y == 19)
		return true;
	else if (body[head].x == 0 || body[head].x == 59)
		return true;
	else
		return false;
}
void snake::make_snake_walkthrough_borders()
{
	if (body[head].x == 0)
		body[head].x = 58;
	else if (body[head].x == 59)
		body[head].x = 1;
	else if (body[head].y == 0)
		body[head].y = 18;
	else if (body[head].y == 19)
		body[head].y = 1;
}
bool snake::is_point_is_of_snake(const point &a)
{
	for (int i = 0; i < length; i++)
	{
		if (body[i] == a)
			return true;
	}
	return false;
}
int snake::get_length()const
{
	return length;
}
point snake::get_tail() const
{
	return body[tail];
}
point snake::get_head() const
{
	return body[head];
}
const point* snake::pass_snake() const
{
	const point* p = &body[0];
	return p;
}


trophy::trophy()
{
	/*srand(time(NULL));*/
	trph.x = rand() % 58 + 1;
	trph.y = rand() % 18 + 1;
	timer = 0;
	status = OFF;
}
void  trophy::change_position()
{
	/*srand(time(0));*/
	trph.x = rand() % 58 + 1;
	trph.y = rand() % 18 + 1;
}
point trophy::get_coordinates()
{
	return trph;
}
void  trophy::set_timer(const point& head)
{
	timer = fabs((float)trph.x - head.x) + fabs((float)trph.y - head.y) + (rand() * rand() % 7 + 3);
}
void trophy::run_timer()
{
	timer--;
}
bool trophy::is_timer_out()
{
	if (timer < 0)
		return true;
	else
		return false;
}
void trophy::turn_on()
{
	status = ON;
}
void trophy::turn_off()
{
	status = OFF;
}
bool trophy::is_on()
{
	if (status == ON)
		return true;
	else
		return false;
}



int normal_trophy::get_value()
{
	return 1;
}
char normal_trophy::sympol()
{
	return '$';
}

int golden_trophy::get_value()
{
	return 5;
}
char golden_trophy::sympol()
{
	return 'G';
}


draw::draw()
{
	mapwidth = 60;
	mapheight = 20;
	for (int j = 0; j < mapheight; j++)
	{
		for (int i = 0; i < mapwidth; i++)
		{
			if (j == 0 || j == mapheight - 1)
			{
				map[j][i] = '#';
			}
			else if (i == 0 || i == mapwidth - 1)
			{
				map[j][i] = '#';
			}
			else
				map[j][i] = ' ';
		}
	}
}
void draw::assign_trophy(trophy *trph)
{
	map[trph->get_coordinates().y][trph->get_coordinates().x] = trph->sympol();
}
void draw::assign_snake(const snake& snk)
{
	const point* snakepoint = snk.pass_snake();
	for (int j = 0; j < snk.get_length(); j++)
	{
		if (j==0)
		{
			map[snakepoint[j].y][snakepoint[j].x] = '@';
		}
		else
		{
			map[snakepoint[j].y][snakepoint[j].x] = 'O';
		}
	}
} 
void draw::assign_trophy(trophy *trph[])
{
	for (int i = 0; i < NUM_TROPHY ; i++)
		assign_trophy(trph[i]);
}
void draw::print()const
{
	for (int j = 0; j < mapheight; j++)
	{
		for (int i = 0; i < mapwidth; i++)
		{
			cout << map[j][i];
		}
		cout << endl;
	}
} 
void draw::erase(point p)
{
	map[p.y][p.x] = ' ';
}



game::game()
{
	score = 0;
	moves = 0;
}
void game::avoid_intersict_snake_trophy(snake& snk, trophy* trph)
{
	if (snk.is_point_is_of_snake(trph->get_coordinates()))
	{
		trph->change_position();
		avoid_intersict_snake_trophy(snk, trph);
	}
	trph->set_timer(snk.get_head());
}
int game::get_score()
{
	return score;
}
void game::update_score(snake& snk, trophy *trph)
{
	if (snk.is_point_is_of_snake(trph->get_coordinates()))
	{
		score += trph->get_value();
		avoid_intersict_snake_trophy(snk, trph);
		snk.grow_snake();
	}
}
void game::run_level()
{
	snake snk;
	trophy *trph[NUM_TROPHY];
	for (int i = 0; i < NUM_TROPHY - 1; i++)
		trph[i] = new normal_trophy();
	trph[NUM_TROPHY-1] = new golden_trophy();
	draw map;
	while (1)
	{
		snk.take_direction();
		snk.correct_direction();
		snk.move_snake();
		if (snk.is_snake_at_border())
			snk.make_snake_walkthrough_borders();
		
		trph[0]->turn_on();

		for (int i = 1; i < NUM_TROPHY - 1; i++)
		{
			int n = 20;
			if (moves == 30 + i*n)
				trph[i]->turn_on();
		} 

		for (int i = 0; i < NUM_TROPHY ; i++)
		{
			if (trph[i]->is_on())   // if trophy is switched on
			{
				trph[i]->run_timer();
				int x = score;
				update_score(snk, trph[i]);
				if (score > x && i == NUM_TROPHY - 1)
					trph[NUM_TROPHY - 1]->turn_off();
				if (trph[i]->is_timer_out())    // if trophy timer goes off 
				{
					map.erase(trph[i]->get_coordinates());
					trph[i]->change_position();
					trph[i]->set_timer(snk.get_head());
					if (i == NUM_TROPHY - 1)
						trph[NUM_TROPHY - 1]->turn_off();
				}
			}
			if (trph[i]->is_on())
				map.assign_trophy(trph[i]);
		}
		if ((moves + rand() %4 +2) % 97 == 0)
		{
			trph[NUM_TROPHY - 1]->turn_on();
			trph[NUM_TROPHY - 1]->set_timer(snk.get_head());
		}
		map.assign_snake(snk);
		cout << "score:" << score << endl;
		map.print();
		map.erase(snk.get_tail());
		if (snk.is_snake_intersected())
		{
			cout << "game over!!" << endl;
			Sleep(3000);
			break;
		}
		Sleep(500);
		system("cls");
		moves++;
	}
}

