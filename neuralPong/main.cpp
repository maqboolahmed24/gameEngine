//libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>

//global variables, functions and classes
int main()
{
    int xPixels = 1600;
    int yPixels = 1200;
    //creating the window
    sf::RenderWindow window(sf::VideoMode(xPixels, yPixels), "Pong");

    //Setting the frame limit to 120
    window.setFramerateLimit(120);
    window.setKeyRepeatEnabled(false);

    //variable that keeps the game loop runnning
    bool play = true;

    //states
    bool UPpressed = false, DOWNpressed = false, leftPressed = false, rightPressed = false;
    bool leftMouse = false;
    bool gameover = false;

    //speed of the bar
    float barspeed = 3.0f;
    
    bool hitRightBar = false;
    bool intersectingRefLine1 = false;
    bool intersectingRefLine2 = false;


    //variables
    int clicks = 0;
    int mouseX{}, mouseY{};

    int rectRotation = 0;
    int xVelocity1 = 0, yVelocity1 = 0;
    int xVelocity2 = 0, yVelocity2 = 0;
    int xVelocityBall = 10;
    float yVelocityBall = 5;
    

    

    sf::Event event;

    //images
    sf::Texture image1;
    if (image1.loadFromFile("Data/img2.png")==-1) //load the image, if image not found i.e., -1 then close the program
    {
        return 1;
    }

    //fonts
    sf::Font font;
    if (font.loadFromFile("Data/font.ttf") == 0)
    {
        return 1;
    }

    //sounds
    sf::SoundBuffer jumpbuffer;
    if (jumpbuffer.loadFromFile("Data/jump.wav") == 0 )
    {
        return 1;
    }
    
    sf::Sound jump;
    jump.setBuffer(jumpbuffer);
    //jump.play();


    //render shapes

    //lines as border for the game
    //top
    sf::VertexArray topline(sf::Lines, 2);
    topline[0].position = sf::Vector2f(50, 50);
    topline[1].position = sf::Vector2f(xPixels-50, 50);
    //left
    sf::VertexArray leftline(sf::Lines, 2);
    leftline[0].position = sf::Vector2f(50, 50);
    leftline[1].position = sf::Vector2f(50, yPixels-50);
    //right
    sf::VertexArray rightline(sf::Lines, 2);
    rightline[0].position = sf::Vector2f(xPixels-50, 50);
    rightline[1].position = sf::Vector2f(xPixels-50, yPixels-50);
    //bottom
    sf::VertexArray bottomline(sf::Lines, 2);
    bottomline[0].position = sf::Vector2f(50, yPixels-50);
    bottomline[1].position = sf::Vector2f(xPixels-50, yPixels-50);
    
    //reference lines for the intersection detection
    //line1
    sf::VertexArray refline1(sf::Lines, 2);
    refline1[0].position = sf::Vector2f(xPixels - 110, 50);
    refline1[1].position = sf::Vector2f(xPixels - 110, yPixels - 50);
    //line2
    sf::VertexArray refline2(sf::Lines, 2);
    refline2[0].position = sf::Vector2f(xPixels - 180, 50);
    refline2[1].position = sf::Vector2f(xPixels - 180, yPixels - 50);
    

    //rectangle shape
    sf::RectangleShape rect1;
    rect1.setSize(sf::Vector2f(30, 200));
    rect1.setPosition(50, yPixels/2);
    rect1.setFillColor(sf::Color::Cyan);
    //rect1.setTexture(&image1);
    
    sf::RectangleShape rect2;
    rect2.setSize(sf::Vector2f(30, 200));
    rect2.setPosition(xPixels-80, yPixels/2);

    //text
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(30);
    title.setString("king Pong");
    title.setPosition(700, 1);
    //title.setColor(sf::Color::White);

    //circle shape
    sf::CircleShape ball;
    ball.setRadius(30);
    ball.setPosition(500, 500);
    ball.setFillColor(sf::Color::Red);

    //game loop
    while (play == true)
    {
        //Events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                //key conditions for Up and Down button
                if (event.key.code == sf::Keyboard::Up) UPpressed = true;
                if (event.key.code == sf::Keyboard::Down) DOWNpressed = true;
                //key conditions for Left and Right button
                if (event.key.code == sf::Keyboard::Left) leftPressed = true;
                if (event.key.code == sf::Keyboard::Right) rightPressed = true;
            }

            if (event.type == sf::Event::KeyReleased)
            {
                //key conditions for Up and Down button
                if (event.key.code == sf::Keyboard::Up) UPpressed = false;
                if (event.key.code == sf::Keyboard::Down) DOWNpressed = false;
                //key conditions for Left and Right button
                if (event.key.code == sf::Keyboard::Left) leftPressed = false;
                if (event.key.code == sf::Keyboard::Right)  rightPressed = false;
            }
            //mousse events
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                leftMouse = true;
            }
            if (event.type == sf::Event::MouseMoved)
            {
                mouseX = event.mouseMove.x;
                mouseY = event.mouseMove.y;
            }
            //window close event
            if (event.type == sf::Event::Closed)
            {
                play = false;
            }
        }

        //AI controol for the left bar
        float targetY = ball.getPosition().y + ball.getRadius() - rect1.getSize().y / 2;
        float deltaY = targetY - rect1.getPosition().y;
        float damping = 0.1f;

        if (std::abs(deltaY) > 5) { // Adjust the threshold
            rect1.move(0, deltaY * damping);
        }


        //Logic

        //keyboad contrl for tte left bar

        float barspeed = 5;
        {
            if (leftPressed == true) yVelocity1 = -barspeed;
            if (rightPressed == true) yVelocity1 = barspeed;
        }
        //keyboard controls for the right bar
        {
            if (UPpressed == true) yVelocity2 = -barspeed;
            if (DOWNpressed == true) yVelocity2 = barspeed;
        }

  
        //exception
        if (UPpressed == true && DOWNpressed == true)
        {
            yVelocity2 = 0;
        }
        if (UPpressed == false && DOWNpressed == false)
        {
            yVelocity2 = 0;
        }
        if (leftPressed == true && rightPressed == true)
        {
            yVelocity1 = 0;
        }
        if (leftPressed == false && rightPressed == false)
        {
            yVelocity1 = 0;
        }

    //here move is velocity funciton to move the object
        {
            rect1.move(xVelocity1, 0);
            //cheching collision
            if (rect1.getGlobalBounds().intersects(ball.getGlobalBounds()) == true) rect1.move(-xVelocity1, 0);
            //swprationg the x and v axes collision
            rect1.move(0, yVelocity1);
            if (rect1.getGlobalBounds().intersects(ball.getGlobalBounds()) == true) rect1.move(0, -yVelocity1);
        }
        //same for the right bar
        {
            rect2.move(xVelocity2, 0);
            //cheching collision
            if (rect2.getGlobalBounds().intersects(ball.getGlobalBounds()) == true) rect2.move(-xVelocity2, 0);
            //swprationg the x and v axes collision
            rect2.move(0, yVelocity2);
            if (rect2.getGlobalBounds().intersects(ball.getGlobalBounds()) == true) rect2.move(0, -yVelocity2);
        }
        //ball logic 
        ball.move(xVelocityBall, yVelocityBall);
        if (ball.getGlobalBounds().intersects(rect1.getGlobalBounds()) == true)
        {
            xVelocityBall = -xVelocityBall;
            yVelocityBall = yVelocityBall; 
            hitRightBar = false; //ball hits the right bar, thats why reseting the flag so that while ball return the position of the ball calulated to predict the ball
        }
        if (ball.getGlobalBounds().intersects(rect2.getGlobalBounds()) == true)
        {
            xVelocityBall = -xVelocityBall;
            yVelocityBall = yVelocityBall;
            hitRightBar = true;
        }
        

        float ballTop = ball.getPosition().y;
        float ballBottom = ball.getPosition().y + (2 * ball.getRadius());

        float ballleft = ball.getPosition().x;
        float ballright = ball.getPosition().x + (2 * ball.getRadius());


        //for blocking the the bars from the top and bottom line
        float topBound = topline[0].position.y;
        float bottomBound = bottomline[0].position.y;
        {
            float rect1top = rect1.getPosition().y;
            float rect1bottom = rect1.getPosition().y + rect1.getSize().y;
            if (rect1top <= topBound)
            {
                rect1.setPosition(rect1.getPosition().x, topBound);
                yVelocity1 = 0;
            }
            if (rect1bottom >= bottomBound) {
                rect1.setPosition(rect1.getPosition().x, bottomBound - rect1.getSize().y);
                yVelocity1 = 0;
            }
        }

        //for rec2
        {
            float rect2Top = rect2.getPosition().y;
            float rect2Bottom = rect2.getPosition().y + rect2.getSize().y;

            if (rect2Top <= topBound) {
                rect2.setPosition(rect2.getPosition().x, topBound);
                yVelocity2 = 0;
            }

            if (rect2Bottom >= bottomBound) {
                rect2.setPosition(rect2.getPosition().x, bottomBound - rect2.getSize().y);
                yVelocity2 = 0;
            }
        }
        

        //checking for the collision with the bottom and top line 
        if (ballTop <= topline[0].position.y || ballBottom >= bottomline[0].position.y)
        {
            yVelocityBall = -yVelocityBall;
        }

        
        if (ballleft <= leftline[0].position.x || ballright >= rightline[0].position.x &&gameover==false)
        {
            
            std::cout << "Game Over";
            gameover = true;
            play = false;
        }

        //check the intersection 
        // Check intersection with refline1
        if (hitRightBar && ball.getGlobalBounds().left + ball.getGlobalBounds().width >= refline1[0].position.x &&
            ball.getGlobalBounds().left <= refline1[0].position.x) {
            if (!intersectingRefLine1)
            {
                intersectingRefLine1 = true;
            }
        }
        else if (intersectingRefLine1)
        {
            // The ball intersects with refline1
            sf::Vector2f intersectionPoint1;
            intersectionPoint1.x = refline1[0].position.x;
            intersectionPoint1.y = ball.getPosition().y + ball.getRadius();  // center of the ball

            // For debugging purposes, print the intersection point to the console
            std::cout << "Intersection with refline1 at: (" << intersectionPoint1.x << ", " << intersectionPoint1.y << ")" << std::endl;
            intersectingRefLine1 = false;
        }

        //Check intersection with refline2
        if (hitRightBar && ball.getGlobalBounds().left + ball.getGlobalBounds().width >= refline2[0].position.x &&
            ball.getGlobalBounds().left <= refline2[0].position.x) {
            if (!intersectingRefLine2)
            {
                intersectingRefLine2 = true;
            }
        }
        else if (intersectingRefLine2)
        {
            // The ball intersects with refline2
            sf::Vector2f intersectionPoint2;
            intersectionPoint2.x = refline2[0].position.x;
            intersectionPoint2.y = ball.getPosition().y + ball.getRadius();  // center of the ball

            // For debugging purposes, print the intersection point to the console
            std::cout << "Intersection with refline2 at: (" << intersectionPoint2.x << ", " << intersectionPoint2.y << ")" << std::endl;
            intersectingRefLine2 = false;
        }


        //Rendering
        window.clear();

        window.draw(rect1);
        window.draw(rect2);
        window.draw(ball);
        window.draw(title);

        //border
        window.draw(leftline);
        window.draw(rightline);
        window.draw(topline);
        window.draw(bottomline);

        //reflines
        window.draw(refline1);
        window.draw(refline2);

        window.display();
    }

    //cleanup and close the window
    window.close();

    //close the program
    return 0;
}