#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>

using namespace sf;

int N = 30, M = 20;
int size = 16;
int w = size * N;
int h = size * M;
int s_color = 3; // цвет змейки, по умолчанию змейка красная

int dir; // направление
int num = 4; // длина змейки

struct Snake
{
    int x, y;
}

s[100];

struct Fruit
{
    int x, y;
}

f;

void Tick()
{

    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i - 1].x; s[i].y = s[i - 1].y;
    }

    if (dir == 0) s[0].y += 1;
    if (dir == 1) s[0].x -= 1;
    if (dir == 2) s[0].x += 1;
    if (dir == 3) s[0].y -= 1;

    if ((s[0].x == f.x) && (s[0].y == f.y))
    {
        num++;
        f.x = rand() % N;
        f.y = rand() % M;
    }

}

void menu (RenderWindow & menu_window) {
    Texture title, start, choose, color_blue, color_green, color_red, author;
    title.loadFromFile("images/menu/title.png");
    start.loadFromFile("images/menu/start.png");
    choose.loadFromFile("images/menu/choose.png");
    color_blue.loadFromFile("images/menu/blank.png");
    color_green.loadFromFile("images/menu/blank.png");
    color_red.loadFromFile("images/menu/blank.png");
    author.loadFromFile("images/menu/me.png");

    Sprite menu_title(title), menu_start(start), menu_choose(choose), color1(color_blue), color2(color_green), color3(color_red), me(author);
    bool isMenu = 1;
    int menuNum = 0;

    menu_title.setPosition(140, 30);
    menu_start.setPosition(120, 100);
    menu_choose.setPosition(100, 150);
    color1.setPosition(120, 200);
    color2.setPosition(220, 200);
    color3.setPosition(320, 200);
    me.setPosition(50, 300);

    while (isMenu) {

        Event event;
        while (menu_window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                menu_window.close();
        }

        menu_title.setColor(Color::Black);
        menu_start.setColor(Color::Black);
        menu_choose.setColor(Color::Black);
        color1.setColor(Color(24, 220, 255));
        color2.setColor(Color(50, 255, 126));
        color3.setColor(Color(255, 77, 77));

        menuNum = 0;
        menu_window.clear(Color(247, 241, 227));

        if (IntRect(120, 100, 250, 50).contains(Mouse::getPosition(menu_window))) {menu_start.setColor(Color(189, 195, 199)); menuNum = 1; }
        if (IntRect(120, 200, 60, 50).contains(Mouse::getPosition(menu_window))) {color1.setColor(Color::White); menuNum = 2; }
        if (IntRect(220, 200, 60, 50).contains(Mouse::getPosition(menu_window))) {color2.setColor(Color::White); menuNum = 3; }
        if (IntRect(320, 200, 60, 50).contains(Mouse::getPosition(menu_window))) {color3.setColor(Color::White); menuNum = 4; }
        
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (menuNum == 1) isMenu = false; // если нажали кнопку "начать", то выходим из меню -> начинаем игру
            if (menuNum == 2) s_color = 1;
            if (menuNum == 3) s_color = 2;
            if (menuNum == 4) s_color = 3;
        }

        menu_window.draw(menu_title);
        menu_window.draw(menu_start);
        menu_window.draw(menu_choose);
        menu_window.draw(color1);
        menu_window.draw(color2);
        menu_window.draw(color3);
        menu_window.draw(me);

        menu_window.display();
    }
}

int main()
{
    RenderWindow menu_window(VideoMode(w, h), "Menu");
    menu(menu_window);

    srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snake");

    Font font;
    font.loadFromFile("Gilroy-Black.ttf");
    Text text("", font, 30);
    text.setFillColor(Color::Black);

    Texture t1, t2;
    t1.loadFromFile("images/pink.png"); // цвет фона
    if (s_color == 1) {
        t2.loadFromFile("images/blue.png"); // цвет змейки, который выбирали в меню
    };
    if (s_color == 2) {
        t2.loadFromFile("images/green.png");
    };
    if (s_color == 3) {
        t2.loadFromFile("images/red.png");
    };
    
    Sprite background(t1);
    Sprite snake_color(t2);

    Clock clock;
    float timer = 0, delay = 0.1; // скорость змейки

    f.x = 10;
    f.y = 10;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up)) dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down)) dir = 0;

        if (timer > delay) { timer = 0; Tick(); }

//отрисовка

        window.clear();

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
            {
                background.setPosition(i * size, j * size);  window.draw(background);
            }

        for (int i = 0; i < num; i++)
        {
            snake_color.setPosition(s[i].x * size, s[i].y * size);  window.draw(snake_color);
        }

        snake_color.setPosition(f.x * size, f.y * size);  window.draw(snake_color);

        
        text.setString("Score: " + std::to_string(num));
        text.setPosition(10, 10); 
        window.draw(text); 

        Texture lose, win;
        lose.loadFromFile("images/game over.png");
        win.loadFromFile("images/win.png");
        Sprite game_over(lose), game_win(win);;

        for (int i = 1; i < num; i++) // проигрыш при столкновении с хвостом
            if (s[0].x == s[i].x && s[0].y == s[i].y && num > 4) {
                window.draw(game_over);
                timer = 0;
            };

        if ((s[0].x > N) || (s[0].x < 0) || (s[0].y > M) || (s[0].y < 0))  { // проигрыш при столкновении с границей
            window.draw(game_over);
            timer = 0;
        }

        if (num > 99) { // победа
            window.draw(game_win);
            timer = 0;
        }

        window.display();

    }

    return 0;
}
