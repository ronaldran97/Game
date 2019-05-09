/*
 Game created by Ronald Tran
 */

#include <SFML/Graphics.hpp>   //using namespace sf
#include <time.h>   //use to get elapsed time
#include <string>
#include <sstream>
#include <iostream>
//#include <>

//dimensions for window size and background
int num_vertBox = 50, num_horzBox = 40;
int size = 16; //number of pixels usually 16
int w = size * num_horzBox;  //background number of pixels in width
int h = size * num_vertBox;    //background number of pixels in height
int score = 0;  //keeps track of how many ice creams are caught

//Catcher: directions, size
int direction;  //direction that the catcher is moving (left or right
int enemyDirection;  //direction that the bomb will wmove it
int catcher_size = 5; //initial size is
float timer = 0.0f, delay = 0.1f;   //game time

//Actual catcher in game is an array of x,y coordinates of sprite2
struct Catcher
{
    int x, y;        //each sprite of Catcher needs an x,y coordinate
}player_catcher[100];   //player


struct OBTAINABLE_ITEMS
{
    int x, y;    // only need one sprite needed for one enemy item
}enemy;


void PlayerMove() {
    
    //updates body to make sure eveything is in the correct order. player_catcher[0] = head
    for (int i = catcher_size; i > 0; i--)
    {
        player_catcher[i].x = player_catcher[i - 1].x;  //only updates x axis because you only need to move horizontally
    }
    
    //Head of catcher, player_catcher[0] depends on direction user inputs
    //if user presses left
    if (direction == 0)
        player_catcher[0].x -= 1;
    
    //if user presses right
    if (direction == 1)
        player_catcher[0].x += 1;
    
    
    //enemy movement, only goes up (y axis)
    enemyDirection = 2;
    if (enemyDirection == 2) {
        enemy.y -= 1;
    }
    
    
    for (int i = catcher_size; i > 0; i--){
    //check if player = enemy location
        if (((player_catcher[i].x) == enemy.x) && ((player_catcher[i].y) == enemy.y)){
                //increment score
                score++;
                //Randomly place enemy somewhere else on x axis
                enemy.x = rand() % num_horzBox;
                enemy.y = 47; //should be 47
        }
    }
    
    //checks if the enemy has reached or passed your line of defense
    if (enemy.y == 0) {
        enemy.x = rand() & num_horzBox; //spawns the enemy randomly on an x point, the y is fixed at 47
        enemy.y = 47;   //spawns enemies at y - 47
        score--;    //decrements score if enemies have passed
        if (score < 0) {    //makes sure score doesnt go into the negatives
            score = 0;
        }
    }


    /*
     boundary checking for player
     loops player to other side if they reach the edge (left or right)
     */
    if (player_catcher[0].x > num_horzBox)
        player_catcher[0].x = 0;
    if (player_catcher[0].x < 0)
        player_catcher[0].x = num_horzBox;
}
          
int main() {
    // Declare and create a new render-window (Window that we play game in)
    sf::RenderWindow window(sf::VideoMode(w, h), "Catcher Game");
    
    //Textures load an image into the GPU Memory
    sf::Texture background, catcher, enemyTexture;
    background.loadFromFile("/Users/ronaldtran/Documents/2560-misc/CatcherInTheRye/images/sky-maybe.png");
    catcher.loadFromFile("/Users/ronaldtran/Documents/2560-misc/CatcherInTheRye/images/white.png");
    enemyTexture.loadFromFile("/Users/ronaldtran/Documents/2560-misc/CatcherInTheRye/images/enemy.png");
    
    //Sprites
    //Sprite has physical dimmensions that can be set in
    //coordinate system, setPosition(x,y), and drawn on screen
    sf::Sprite sprite1(background);
    sf::Sprite sprite2(catcher);
    sf::Sprite sprite3(enemyTexture);

    //initially place food somewhere on screen
    enemy.x = rand() % num_horzBox;
    enemy.y = 47; //should be 47

    //FONTS
    sf::Font font;  //font for timer
    font.loadFromFile("/Users/ronaldtran/Documents/2560-misc/CatcherInTheRye/8bit_wonder/8-BITWONDER.TTF");
    
    //for time keeping
    sf::Clock clock;
    
    while (window.isOpen()) {
        
        sf::Event event; //too keep track of what/when user is doing
        
        //Check when the window is closed
        while (window.pollEvent(event))
        {
            //If user presses x in the top right, Windows, top left, Mac,  close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    
        //keyboard controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) direction = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) direction = 1;
        
        //to keep track of time
        float time = clock.getElapsedTime().asSeconds();    //tracks time as seconds
        
        clock.restart();
        
        timer += time;
        
        //if timer is running
        if (timer > delay)
        {
            timer = 0; //reset timer
            PlayerMove();    //move Snake one sprite forward
        
        }
        
        //score stuff
        std::string scoreAsString = std::to_string(score); //convert int to string
        sf::Text scoreText; //declaring the text
        scoreText.setFont(font);    //sets font of the score text
        scoreText.setString(scoreAsString); //sets the string to the converted int
        scoreText.setCharacterSize(32); //font size of the score
        scoreText.setFillColor(sf::Color::Yellow);  //sets color
        
        //Title related things
        sf:: Text TitleText;
        TitleText.setFont(font);
        TitleText.setString("Sky Defender");
        TitleText.setCharacterSize(50);
        TitleText.setFillColor(sf::Color::White);
        
        //Draw in Window
        window.clear(); //clear the window so new frame can be drawn in
        
        //Items drawn first will be drawn over
        //backgroud
        for (int i = 0; i < num_horzBox; i++) {
            for (int j = 0; j < num_vertBox; j++) {
                //Set position of sprite1 one at a time
                sprite1.setPosition(i*size, j*size);
                //Draw sprite1 but, do not show it on screen.
                window.draw(sprite1);
            }
        }
        
        //draw score
        scoreText.setPosition(20*size, 47*size);
        window.draw(scoreText);
        
        //writes out sky defender
        TitleText.setPosition(2*size, 21*size);
        window.draw(TitleText);
        
        //third: draw catcher
        for (int i = 0; i < catcher_size; i++) {
            //position sprite2 one at a time
            sprite2.setPosition(player_catcher[i].x*size, player_catcher[i].y*size);  //size    16 pxls
            //Draw sprite2 one at a time by drawing over background
            window.draw(sprite2);
        }
        
        //draws enemy
        sprite3.setPosition(enemy.x*size, enemy.y*size);
        window.draw(sprite3);
    
        //Show everything we have drawn on the screen
        window.display();
    }
    return 0;
}

