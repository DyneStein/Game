
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cfloat>
#include<cmath>
#include<fstream>

//Muhammad Dyen Asif. Project CS . Buzz Bomber. 24I-0608

using namespace std;
using namespace sf;

const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

// Function Prototypes
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, RectangleShape& health , int& K);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void drawMenu(RenderWindow& window, Font& font, int& selectedMenuIndex); 
void drawLevelMenu(RenderWindow& window, Font& font, int& selectedLevelIndex);
void handleMenuInput(bool& isNameInputActive, bool& isMenuActive, bool& isLevelMenuActive, 
                     int& selectedMenuIndex, int& selectedLevelIndex, 
                     int& currentLevel, string& inputName, bool& isScoresMenuActive);
bool isObstacleNearby(float x, int index, const int max, bool exsistence[], float obsX[] );
bool isCollidingWithObstacle(float playerX, float playerY, const int maxObstacles, bool obstacleExists[], float obstacleX[], float obstacleY[]);
void drawGameOverScreen(RenderWindow& window, Font& font);
void drawNameInputMenu(RenderWindow& window, Font& font, string& inputName, bool& isNameInputActive);
bool isPlayerCompletlyBlocked(float playerX, const int maxObstacles, bool obstacleExists[], float obstacleX[], float obstacleY[]);

void readScores(string names[], int scores[], int& count);
void drawScoresMenu(RenderWindow& window, Font& font, string names[], int scores[], int count);
void updateScores(string names[], int scores[], int& count, string inputName, int score);




int main() {

    srand(time(0));

// Game State Variables
RectangleShape mainHealth(Vector2f(20.f,30.f));
int currentLevel = 1;              
bool isMenuActive = true;          
int selectedMenuIndex = 0;      
int selectedLevelIndex = 0;        
bool isLevelMenuActive = false; 
bool isScoresMenuActive = false;
const int menuItemsCount = 4;      
const int levelItemsCount = 4;     
string menuItems[menuItemsCount] = {"Start Game", "Select Level","View Scores","Exit"};  
string levelItems[levelItemsCount] = {"Level 1", "Level 2", "Level 3","Fight Boss"};    
const Time MENU_TRANSITION_DELAY = milliseconds(200);
bool isGameOver = false;
Font font;
const int fixedStep = 32;
const int playerWidth = 32;
const int maxBullets = 56;
string inputName = "";    
bool isNameInputActive = true;



const int maxScores = 5; // Maximum number of scores
string names[maxScores];  // Array to store player names
int scores[maxScores];     // Array to store player scores
int scoreCount = 0;       // Counter for the number of scores


int score = 0;

// Bee-related variables
const int maxBees = 20;
float beePositionsX[maxBees];
float beePositionsY[maxBees];
bool beeExists[maxBees] = {false};
bool beeMovingRight[maxBees] = {true}; //Track direction (true: right, false: left)
Texture beeTexture;
Sprite beeSprites[maxBees];
Clock beeMovementClock;
int beesSpawned = 0;
const float beeSpeed = 0.7f; //Speed of bee movement
const float descentStep = 50.0f; //Amount bees descend when changing direction
Clock beeSpawnClock;
const int beeSpawnInterval = 1500; //1.5 seconds
bool beeStopped[maxBees] = {false};
Clock beeStopClock[maxBees];
const float stopDuration = 3.0f;




//Hunter bees related variables

int huntermaxHoneyCombs = 23;
Texture hunterhoneycombTexture;
bool hunterhoneycombCreated = false; 
float hunterhoneycombX[huntermaxHoneyCombs] = {0}; // in order to store position
float hunterhoneycombY[huntermaxHoneyCombs] = {0};

// Add new variables for honeycomb tracking
bool hunterhoneycombExists[huntermaxHoneyCombs] = {false}; // Tracks if a honeycomb exists
Sprite hunterhoneycombSprites[huntermaxHoneyCombs];
Clock hunterbeeSpawnClock;
Clock hunterbeeMovementClock;
Clock hunterbeeClock;
const int maxhunterBees = 10;
float hunterbeePositionsX[maxhunterBees];
float hunterbeePositionsY[maxhunterBees];
bool hunterbeeExists[maxhunterBees] = {false};
bool hunterbeeMovingRight[maxhunterBees] = {true}; //Track direction (true: right, false: left)
Texture hunterbeeTexture;
Sprite hunterbeeSprites[maxhunterBees];
int hunterbeesSpawned = 0;
const float hunterbeeSpeed = 3.f; //Speed of bee movement
const float hunterdescentStep = 60.0f; //Amount bees descend when changing direction
const int hunterbeeSpawnInterval = 1800; //1.8 seconds
//Load textures
if (!hunterbeeTexture.loadFromFile("Textures/Fast_bee.png") || !hunterhoneycombTexture.loadFromFile("Textures/honeycomb_red.png")) {
    cout << "Error: Could not load bee or honeycomb texture!" << endl;
    return -1;
}





//Bird handling
Texture birdTexture;
if (!birdTexture.loadFromFile("Textures/bird.png")) {
    cout << "Error loading bird texture!" << endl;
    return -1;
}
Sprite bird(birdTexture);
float birdX = -100, birdY = 100;
bool birdActive = true;
bird.setScale(2.3f, 2.3f);
Clock birdClock;
Clock EatingTime;
Clock birdRoutingClock;
bool isBirdRerouting = false;
int birdHitCount = 0;
bool isBirdMovingOffScreen = false;
Clock birdOffScreenClock;



// Emoji-related variables
const int maxEmojis = maxBees;
bool emojiExists[maxEmojis] = {false};
float emojiX[maxEmojis] = {0};
float emojiY[maxEmojis] = {0};
Sprite emojiSprites[maxEmojis];
Texture emojiTexture;


// Load emoji texture
if (!emojiTexture.loadFromFile("Textures/hive.png")) {
    cout << "Error: Could not load emoji texture!" << endl;
    return -1;
}


// Initialize emoji sprites
for (int i = 0; i < maxEmojis; ++i) {
    emojiSprites[i].setTexture(emojiTexture);
    emojiSprites[i].setScale(0.8f, 0.8f);
}


// Add new obstacle-related variables
const int maxObstacles = 20; // Max number of obstacles because in loops they are inherently dependent on the bees so giving a big number just in case.
float obstacleX[maxObstacles] = {0}; // X-positions of obstacles
float obstacleY[maxObstacles] = {resolutionY - 100}; // All obstacles are at the same Y
bool obstacleExists[maxObstacles] = {false}; // Track obstacle existence
Sprite obstacleSprites[maxObstacles]; // Sprites for obstacles
bool firstpollination = true;  
bool firstpollinationObstacle = false;
// Add this with other global variables
Clock obstacleResetClock;
const float obstacleResetCooldown = 5.0f;  // 5 seconds between resets


// Clocks for smooth movement
Clock playerClock;  // Clock for player movement
Clock beeClock;     // Clock for bee movement// Clocks for smooth movement


// Load obstacle texture
Texture obstacleTexture;
if (!obstacleTexture.loadFromFile("Textures/obstacles.png")) {
    cout << "Error: Could not load obstacle texture!" << endl;
    return -1;
}


// Initialize obstacle sprites
for (int i = 0; i < maxObstacles; i++) {
    obstacleSprites[i].setTexture(obstacleTexture);
    obstacleSprites[i].setScale(2.f, 2.f);
}



// Honeycomb-related variables
int maxHoneyCombs = 23;
Texture honeycombTexture;
bool honeycombCreated = false; 
float honeycombX[maxHoneyCombs] = {0}; // in order to store position
float honeycombY[maxHoneyCombs] = {0};




// Add new variables for honeycomb tracking
bool honeycombExists[maxHoneyCombs] = {false}; // Tracks if a honeycomb exists
Sprite honeycombSprites[maxHoneyCombs];


//Load textures
if (!beeTexture.loadFromFile("Textures/Regular_bee.png") || !honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
    cout << "Error: Could not load bee or honeycomb texture!" << endl;
    return -1;
}



//Initialize honeycomb sprites
for (int i = 0; i < maxBees; i++) {
    honeycombSprites[i].setTexture(honeycombTexture);
    honeycombSprites[i].setScale(0.8f, 0.8f);
}

//Initialize hunter honeycomb sprites
for (int i = 0; i < maxhunterBees; i++) {
    hunterhoneycombSprites[i].setTexture(hunterhoneycombTexture);
    hunterhoneycombSprites[i].setScale(0.8f, 0.8f);
}




   
    //Game Mechanics Variables
    int j = 0;
    int k = 0;
    int tempVar = 0; // used to show the value of the main health bar of the current spray can because the K value causes conflicts with its previous value and the value with which it gets updated afterwards i.e k = 0 (re-initialization).
    int bulletCounter = 0;
    int sprayCanBullets[3] = {maxBullets-1, maxBullets, maxBullets};
    int gameGrid[gameRows][gameColumns] = {};

    // Create the RenderWindow
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(500, 200));

    // Font selection for the menu
    if (!font.loadFromFile("RuslanDisplay-Regular.ttf")) {
        cout << "Error: Could not load font!" << endl;
        return -1;
    }


    // Background Music
    Music bgMusic;
    if (!bgMusic.openFromFile("Music/Music3.ogg")) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(50);
    bgMusic.setLoop(true);
    bgMusic.play();


    // Player Initialization
    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;
    Texture playerTexture;
    Sprite playerSprite;
    if (!playerTexture.loadFromFile("Textures/spray.png")) {
        cout << "Error: Could not load player texture!" << endl;
    }
    playerSprite.setTexture(playerTexture);


    // Spray Cans Initialization
    Texture sprayCanTexture;
    if (!sprayCanTexture.loadFromFile("Textures/spray.png")) {
        cout << "Error: Could not load spray can texture!" << endl;
    }


    Sprite sprayCanSprites[3];
    for (int i = 0; i < 3; i++) {
        sprayCanSprites[i].setTexture(sprayCanTexture);
        sprayCanSprites[i].setPosition(32 * i + 32*i, resolutionY - 64);
    }


    // Spray cans animation rectangles
    RectangleShape sprayCanRectangles[3];
    for (int i = 0; i < 3; i++) {
        sprayCanRectangles[i].setSize(Vector2f(20.f, 28.f));
        sprayCanRectangles[i].setFillColor(Color::White);
        sprayCanRectangles[i].setPosition(22 + 32 * i + 32*i , resolutionY - 30);
    }
    

// Bullet Initialization
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;
    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    if (!bulletTexture.loadFromFile("Textures/bullet.png")) {
        cout << "Error: Could not load bullet texture!" << endl;
    }
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(3, 3);
    //bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));



    //Ground Initialization
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color(196, 164, 132));



//Main Game Loop
while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            //Input handling with local variables
            handleMenuInput(isNameInputActive, isMenuActive, isLevelMenuActive, 
                            selectedMenuIndex, selectedLevelIndex, 
                            currentLevel, inputName, isScoresMenuActive);


            readScores(names, scores, scoreCount); 

            //Game input handling
            if (!isMenuActive && !isLevelMenuActive && !isGameOver) {
                if (e.type == Event::Closed) {
                    return 0;
                }

//to return to the main menu IN-GAME
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        isMenuActive = true;
        updateScores(names, scores, scoreCount, inputName, score);

    }

//----------------------------------------------------------------------------------




if (Keyboard::isKeyPressed(Keyboard::Left)) {
                  
        float newPlayerX = player_x - fixedStep;
    if (newPlayerX >= 0 && !isCollidingWithObstacle(newPlayerX, player_y, maxObstacles, obstacleExists, obstacleX, obstacleY)) {
        player_x = newPlayerX; // Move left if no collision
    }          
                    
                }

                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    
        float newPlayerX = player_x + fixedStep;
    if (newPlayerX < resolutionX - boxPixelsX && !isCollidingWithObstacle(newPlayerX, player_y, maxObstacles, obstacleExists, obstacleX, obstacleY)) {
        player_x = newPlayerX; // Move right if no collision
    }
                }

                if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists && sprayCanBullets[0] > 0) {
             
        // Game over logic            
        if (sprayCanBullets[0] == 1 && j == 2) {
            isGameOver = true;
            
        }
       
          bullet_x = player_x;
                    bullet_y = player_y;
                    bullet_exists = true;
                    sprayCanBullets[0]--;
                    bulletCounter++; 
                    
                    
                    if(bulletCounter%8==0){ 
                        k++;
                        tempVar = k;
                        
                        if(28.f - 3.f*tempVar>=0)
                            sprayCanRectangles[j].setSize(Vector2f(20.f, 28.f - 3.f*tempVar));
                        else
                            sprayCanRectangles[j].setSize(Vector2f(20.f, 0.f));
                        
                        sprayCanRectangles[j].setPosition(22 + 32 * j + 32*j, resolutionY - 30 + 3.f*tempVar);
                        
                        mainHealth.setSize(Vector2f(20.f,30.f-3.f*tempVar));
                        
                        
                    }		
                    
                    // Spray can bullet management
                    if (sprayCanBullets[0] == 0) { 
                        
                        k = 0;
                        if(j+1<3)
                            j++;
                        
                        sprayCanRectangles[j-1].setPosition(-100.f, -100.f);
                        
                        for (int i = 0; i < 2; i++) {
                            sprayCanBullets[i] = sprayCanBullets[i + 1];
                            sprayCanSprites[i] = sprayCanSprites[i + 1];
                        }
                        sprayCanBullets[2] = maxBullets;
                    }
       

                    
               
                }
            }
        }

        // Clear the window
        window.clear();

        //Drawing logic


    if(isNameInputActive) {
            drawNameInputMenu(window, font, inputName, isNameInputActive);
    }
    else if (isMenuActive) {
    drawMenu(window, font, selectedMenuIndex);
    }
    else if (isScoresMenuActive){

    drawScoresMenu(window, font, names, scores, scoreCount);

    }
    else if (isLevelMenuActive) {
            drawLevelMenu(window, font, selectedLevelIndex);
            
        score = 0;          
        // Reset game state
        player_x = (gameRows / 2) * boxPixelsX;
        player_y = (gameColumns - 4) * boxPixelsY;
        
        // Reset spray cans
        j = 0;
        k = 0;
        tempVar = 0;
        sprayCanBullets[0] = maxBullets-1;
        sprayCanBullets[1] = maxBullets;
        sprayCanBullets[2] = maxBullets;
        bulletCounter = 0;


        firstpollinationObstacle = false;
        firstpollination = true;

if(selectedLevelIndex==0){      


        for (int i = 0; i < maxBees; ++i) {
            beeExists[i] = false;
            honeycombExists[i] = false;
            emojiExists[i] = false;
            obstacleExists[i] = false;
        }

    for(int i = 0;i<maxObstacles;i++)
    {

        obstacleExists[i] = false;
        obstacleX[i]=0;
        obstacleY[i]=resolutionY-100;
    }


    beesSpawned=0;
         
    for(int i=0;i<3;i++)
    { honeycombX[i] = rand()%800;
    honeycombY[i] = rand()%300;
    honeycombExists[i] = true;
    honeycombSprites[i].setPosition(honeycombX[i],honeycombY[i]);

    }



        }
else if(selectedLevelIndex==1)
{


for (int i = 0; i < maxBees-5; ++i) {
            beeExists[i] = false;
            honeycombExists[i] = false;
            emojiExists[i] = false;
            obstacleExists[i] = false;
        }


    for(int i = 0;i<maxObstacles;i++)
    {

        obstacleExists[i] = false;
        obstacleX[i]=0;
        obstacleY[i]=resolutionY-100;
    }


beesSpawned=0;
hunterbeesSpawned=0;



 for (int i = 0; i < maxhunterBees-5; ++i) {
            hunterbeeExists[i] = false;
            hunterhoneycombExists[i] = false;
        }


for(int i=0;i<9;i++)
            { honeycombX[i] = rand()%800;
            honeycombY[i] = rand()%300;
            honeycombExists[i] = true;
            honeycombSprites[i].setPosition(honeycombX[i],honeycombY[i]);
         
            }



        }
else if(selectedLevelIndex==2)
{


    for (int i = 0; i < maxBees; ++i) {
            beeExists[i] = false;
            honeycombExists[i] = false;
            emojiExists[i] = false;
            
        }



   
    for(int i = 0;i<maxObstacles;i++)
    {

        obstacleExists[i] = false;
        obstacleX[i]=0;
        obstacleY[i]=resolutionY-100;
    }




beesSpawned=0;
hunterbeesSpawned=0;



 for (int i = 0; i < maxhunterBees; ++i) {
            hunterbeeExists[i] = false;
            hunterhoneycombExists[i] = false;
        }


for(int i=0;i<15;i++)
            { honeycombX[i] = rand()%800;
            honeycombY[i] = rand()%300;
            honeycombExists[i] = true;
            honeycombSprites[i].setPosition(honeycombX[i],honeycombY[i]);
         
            }


   

}
else if(selectedLevelIndex==3)
{    }

        		
					
	   for (int i = 0; i < 3; i++) {
						sprayCanSprites[i].setTexture(sprayCanTexture);
						sprayCanSprites[i].setPosition(32 * i + 32*i, resolutionY - 64);
					    }					
	
	
        birdX = -200, birdY = 200;
        
        // Reset bullet
        bullet_exists = false;
        beesSpawned = 0;

        // Reset health bar
        mainHealth.setSize(Vector2f(20.f,30.f));
        
        // Reset spray can rectangles
        for (int i = 0; i < 3; i++) {
            sprayCanRectangles[i].setSize(Vector2f(20.f, 28.f));
            sprayCanRectangles[i].setPosition(22 + 32 * i + 32*i, resolutionY - 30);
        }
          
           


            }// is level menu closer if
   else if (isGameOver){

 drawGameOverScreen(window, font);


 if (Keyboard::isKeyPressed(Keyboard::Escape)) {
score = 0;
updateScores(names, scores, scoreCount, inputName, score);
isGameOver = false;
isMenuActive = true;					

 }  
          
          }
else  {
if (currentLevel == 1) {
    // Draw ground
    window.draw(groundRectangle);

    // Draw spray cans and their rectangles
    for (int i = 0; i < 3; i++) {
        window.draw(sprayCanSprites[i]);      // Draw spray can sprites
        window.draw(sprayCanRectangles[i]);  // Draw spray can health bars
    }

    // Spawn bees if not all are generated
    if (beesSpawned < maxBees && beeSpawnClock.getElapsedTime().asMilliseconds() > beeSpawnInterval) {
        if (rand() % 100 < 2) { // Small chance to spawn a bee on each frame
            for (int i = 0; i < maxBees; ++i) {
                if (!beeExists[i] && !honeycombExists[i]) {
                    beePositionsX[i] = (rand() % 2 == 0) ? 0 : resolutionX - 32; // Spawn left or right
                    beePositionsY[i] = 0;
                    beeExists[i] = true;
                    beeSprites[i].setTexture(beeTexture);
                    beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);
                    beeMovingRight[i] = (beePositionsX[i] == 0); // Start moving right if spawned on the left
                    beesSpawned++;
                    break;
                }
            }
            
            beeSpawnClock.restart(); // Reset the spawn clock
        }
    }


float beeDeltaTime = beeClock.restart().asSeconds();


// Move bees in a zigzag pattern
for (int i = 0; i < maxBees; ++i) 
{

if (beeExists[i]) {
        
//------------------------------------------stopped bees getting shot---------------------------------------------
        
         if (bullet_exists &&
            bullet_x + 16 >= beePositionsX[i] &&
            bullet_x <= beePositionsX[i] + 32 &&
            bullet_y + 16 >= beePositionsY[i] &&
            bullet_y <= beePositionsY[i] + 32) {
            // Remove the bee
            beeExists[i] = false;
            score+=100;
            // Create a honeycomb at the bee's position
            honeycombExists[i] = true;
            honeycombX[i] = beePositionsX[i];
            honeycombY[i] = beePositionsY[i];
            honeycombSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
        }
        
 
//----------------------------------------------------------------------------------------------------------------------      
        
     // random bees stopping logic   
        if (beeStopped[i]) {
            // Check if stop duration has elapsed
            if (beeStopClock[i].getElapsedTime().asSeconds() >= stopDuration) {
                beeStopped[i] = false;
            }
            continue; // Skip movement while stopped
        }

        // Occasionally stop the bee
        if (rand() % 5000 == 0) {  //probability
            beeStopped[i] = true;
            beeStopClock[i].restart();
            continue;
        }
        
//-----------------------------------------------------------------------------------------------------------------------   
        
        
            // Move horizontally
            if (beeMovingRight[i]) {
                beePositionsX[i] += beeSpeed * beeDeltaTime * 200;
                if (beePositionsX[i] >= resolutionX - 32) { // Reached right edge
                    beeMovingRight[i] = false; // Change direction
                    beePositionsY[i] += descentStep; // Descend
                    
                    
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            // Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                 
                }
           
          beeExists[i] = false;
            break; // Exit honeycomb loop
        }
    }
}
       
                }
            } else {
                beePositionsX[i] -= beeSpeed * beeDeltaTime * 200;
                if (beePositionsX[i] <= 0) { // Reached left edge
                    beeMovingRight[i] = true; // Change direction
                    beePositionsY[i] += descentStep; // Descend
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            //Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                 
                }
           
          beeExists[i] = false;
            break; //Exit honeycomb loop
        }
    }
}
                    
                    
                    
                }
            }
		
	//Check for collision with honeycombs
        for (int j = 0; j < maxBees; ++j) {
            if (honeycombExists[j]) {
                //Check if bee's bounds intersect honeycomb's bounds
                if (beePositionsX[i] + 32 > honeycombX[j] &&
                    beePositionsX[i] < honeycombX[j] + 32 &&
                    beePositionsY[i] + 16 > honeycombY[j] &&
                    beePositionsY[i] < honeycombY[j] + 16) {
                    // On collision, descend and change direction
                    beeMovingRight[i] = !beeMovingRight[i];
                    beePositionsY[i] += descentStep;
                
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            // Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                  beeExists[i] = false;
            	  break; //Exit honeycomb loop
                }
           
          
        }
    }
} 
                    break; //Exit honeycomb loop for this bee
                }
            }     
        }	
        
        
        
        if (beeExists[i]) {
        // Move bees as before (zigzag logic)

        // Check if bee has crossed the specified height
        if (beePositionsY[i] > resolutionY - 160) {
            // Remove the bee
            beeExists[i] = false;
            // Place an obstacle at or near the bee's X-position
            float newObstacleX = beePositionsX[i];
            bool placed = false;
            
        
// Adjusted Obstacle Placement Logic
for (int j = 0; j < maxObstacles; j++) {
    
        // First pollination logic
        if (firstpollination) {
            // Place two obstacles at fixed positions
            if (j < 2) {
            
                obstacleExists[j] = true;
                obstacleX[j] = (j == 0) ? (resolutionX - 64) : 0; 
                obstacleY[j] = resolutionY - 120;
                obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
                
                if(j==0)
                continue; // only for the first flower, because we have to set the pollination to false for the rest of as well. 
                // the rest will follow conventional rules.
                
                if (j == 1) {  // Only set these flags when the second obstacle is placed
                    firstpollinationObstacle = true;
                    firstpollination = false;
                }
            } 
            
             break; // only to print 2 flowers not 3   
        } 
        
        
     if(!obstacleExists[j] && j>1){
        float newObstacleX = beePositionsX[i];
      

        while (isObstacleNearby(newObstacleX, j, maxObstacles, obstacleExists, obstacleX)) {
            if (player_x < beePositionsX[i]) {
                newObstacleX -= 3; // Move left
            } else {
                newObstacleX += 3; // Move right
            }

            // Wrap around the screen if necessary
            if (newObstacleX < 0) {
                newObstacleX += resolutionX;
            } else if (newObstacleX >= resolutionX) {
                newObstacleX -= resolutionX;
            }
        }
        
            obstacleExists[j] = true;
            obstacleX[j] = newObstacleX;
            obstacleY[j] = resolutionY - 120;
            obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
        
        break;
        
        
        
    }
}


        }
    }
    
    
    
// Check for collision with emojis
for (int i = 0; i < maxEmojis; ++i) {
    if (emojiExists[i]) {
        // Check if bullet collides with emoji
        if (bullet_exists &&
            bullet_x + 32 >= emojiX[i] &&
            bullet_x <= emojiX[i] + 32 &&
            bullet_y + 16 >= emojiY[i] &&
            bullet_y <= emojiY[i] + 16) {
            // Remove the emoji upon collision with the bullet
            emojiExists[i] = false;
            bullet_exists = false; // Bullet is destroyed on hit
            break;
        }
    }
}
	
            beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

		}
			
         if (beeExists[i]) {
        // Check if the bullet hits a bee
        if (bullet_exists &&
            bullet_x + 16 >= beePositionsX[i] &&
            bullet_x <= beePositionsX[i] + 32 &&
            bullet_y + 16 >= beePositionsY[i] &&
            bullet_y <= beePositionsY[i] + 32) {
            // Remove the bee
            beeExists[i] = false;
            score+=100;



            // Create a honeycomb at the bee's position
            honeycombExists[i] = true;
            honeycombX[i] = beePositionsX[i];
    	    honeycombY[i] = beePositionsY[i];
            honeycombSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
               
               
               
            }
        }
        
        
        
// Check for collision with honeycombs
        for (int j = 0; j < maxBees; ++j) {
            if (honeycombExists[j]) {
                // Check if bullet collides with honeycomb
                if (bullet_exists &&
                    bullet_x + 32 >= honeycombX[j] &&
                    bullet_x <= honeycombX[j] + 32 &&
                    bullet_y + 16 >= honeycombY[j] &&
                    bullet_y <= honeycombY[j] + 16) {
                    // Remove the honeycomb upon collision with the bullet
                    honeycombExists[j] = false;
                    bullet_exists = false;  // Bullet is also destroyed on hit
                }
            } 
}
            
          
    }// main bee loop ender 




int activeBeeCount = 0;
for(int i = 0; i < maxBees; i++) {
    if(beeExists[i]) {
        activeBeeCount++;
    }
}

// Check if no active bees remain
if(activeBeeCount == 0 && beesSpawned > 0) {


// Reset spray cans
        j = 0;
        k = 0;
        tempVar = 0;
        sprayCanBullets[0] = maxBullets-1;
        sprayCanBullets[1] = maxBullets;
        sprayCanBullets[2] = maxBullets;
        bulletCounter = 0;


        firstpollinationObstacle = false;
        firstpollination = true;


    for (int i = 0; i < 3; i++) {
        sprayCanSprites[i].setTexture(sprayCanTexture);
        sprayCanSprites[i].setPosition(32 * i + 32*i, resolutionY - 64);
    }



    
        for (int i = 0; i < maxBees; ++i) {
            beeExists[i] = false;
            honeycombExists[i] = false;
            emojiExists[i] = false;
            
        }



   
    for(int i = 0;i<maxObstacles;i++)
    {

        obstacleExists[i] = false;
        obstacleX[i]=0;
        obstacleY[i]=resolutionY-100;
    }




beesSpawned=0;
hunterbeesSpawned=0;



 for (int i = 0; i < maxhunterBees; ++i) {
            hunterbeeExists[i] = false;
            hunterhoneycombExists[i] = false;
        }


for(int i=0;i<9;i++)
            { honeycombX[i] = rand()%800;
            honeycombY[i] = rand()%300;
            honeycombExists[i] = true;
            honeycombSprites[i].setPosition(honeycombX[i],honeycombY[i]);
         
            }




for (int i = 0; i < 3; i++) {
						sprayCanSprites[i].setTexture(sprayCanTexture);
						sprayCanSprites[i].setPosition(32 * i + 32*i, resolutionY - 64);
					    }					
	
	
        birdX = -200, birdY = 200;
        
        // Reset bullet
        bullet_exists = false;

        // Reset health bar
        mainHealth.setSize(Vector2f(20.f,30.f));
        
        // Reset spray can rectangles
        for (int i = 0; i < 3; i++) {
            sprayCanRectangles[i].setSize(Vector2f(20.f, 28.f));
            sprayCanRectangles[i].setPosition(22 + 32 * i + 32*i, resolutionY - 30);
        }
          



        for(int j = 0;j<maxEmojis;j++)
        {
            if(emojiExists[j])
            {
                if(emojiY[j]>=0 && emojiY[j]<=100)
                {score+=2000;}
                else if(emojiY[j]>100 && emojiY[j]<=250)
                {score+=1600;}
                else if(emojiY[j]>250)
                {score+=1000;}

            }
        }







    
    
    currentLevel=2;
}
        
  



//-----------------------------------------------------------------------------------------------------------------------------------

 if (birdClock.getElapsedTime().asSeconds() > 0.025) {


   bool exist = false;
    if (birdActive) {
        // Bird hit detection
        if (bullet_exists &&
            bullet_x + 16 >= birdX &&
            bullet_x <= birdX + 64 && // Assuming bird is larger due to scaling
            bullet_y + 16 >= birdY &&
            bullet_y <= birdY + 64) {
            birdHitCount++;
            bullet_exists = false; // Remove bullet

            // If bird is hit 3 times, move off screen
            if (birdHitCount >= 3) {
                isBirdMovingOffScreen = true;
                birdOffScreenClock.restart();
                birdHitCount = 0; // Reset hit count
            }
        }

        // Bird off-screen movement logic
        if (isBirdMovingOffScreen) {
            // Smoothly move off screen
            if (birdX > -200) {
                birdX -= 3; // Gradually move left
            }

if(birdX<=-200){
            if (birdOffScreenClock.getElapsedTime().asSeconds() >= 3.0) {
                isBirdMovingOffScreen = false;
            }}
        }
        else {
            
            
            //If bird is currently rerouting, check pause duration
        if (isBirdRerouting) {
            if (birdRoutingClock.getElapsedTime().asSeconds() >= 2.0) {
                //Pause is over, allow bird to move again
                isBirdRerouting = false;
        } }
           
           
           
                // Find nearest honeycomb (existing code)
                int nearestHoneycombIndex = -1;
                float minDistance = FLT_MAX;


                for (int i = 0; i < maxBees; ++i) {
                    if (honeycombExists[i]) {
                        float dx = honeycombX[i] - birdX;
                        float dy = honeycombY[i] - birdY;
                        float distance = sqrt(dx * dx + dy * dy);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestHoneycombIndex = i;
                        }
                    }
                }
                
                
             bool temp = false;   
                if (nearestHoneycombIndex != -1){
            // Check if bird has changed target honeycomb
            static int previousTarget = -1;
            if (previousTarget != -1 && previousTarget != nearestHoneycombIndex) {
                //Bird is rerouting to a new honeycomb
                isBirdRerouting = true;
                birdRoutingClock.restart();
            }
            
            previousTarget = nearestHoneycombIndex;
            temp = true;
             }

                
                

                // Move towards nearest honeycomb
                if (!isBirdRerouting) {
                    if (birdX < honeycombX[nearestHoneycombIndex]) birdX += 2;   
                    if (birdX > honeycombX[nearestHoneycombIndex]) birdX -= 2;
                    if (birdY < honeycombY[nearestHoneycombIndex]) birdY += 2;
                    if (birdY > honeycombY[nearestHoneycombIndex]) birdY -= 2;
            

                    // Collect honeycomb logic remains the same
                    if (sqrt(pow(birdX - honeycombX[nearestHoneycombIndex], 2) +
                             pow(birdY - honeycombY[nearestHoneycombIndex], 2)) < 16 && 
                        EatingTime.getElapsedTime().asSeconds() > 5) {

                      honeycombExists[nearestHoneycombIndex] = false;

if(temp){
                         if(honeycombY[nearestHoneycombIndex]>=0.f && honeycombY[nearestHoneycombIndex]<=100.f) //first tier
                         score+=1000;
                         else if(honeycombY[nearestHoneycombIndex]>100.f && honeycombY[nearestHoneycombIndex]<=250.f)
                         score+=800;
                         else if(honeycombY[nearestHoneycombIndex]>250.f)
                         score+=500;}


  

                        EatingTime.restart(); 
                    }
                }
            }
	  bird.setPosition(birdX, birdY);
        birdClock.restart();
    }
}



//-------------------------------------------------------------------------------------------------------------------------------








    // Draw bees
    for (int i = 0; i < maxBees; ++i) {
        if (beeExists[i]) {
            window.draw(beeSprites[i]);
        }
    }
    
    
   // Draw honeycombs
for (int i = 0; i < maxBees; ++i) {
    if (honeycombExists[i]) {
        window.draw(honeycombSprites[i]);
    }
}
        
 
// Draw emojis
for (int i = 0; i < maxEmojis; ++i) {
    if (emojiExists[i]) {
        window.draw(emojiSprites[i]);
    }
}
 
 
  // Draw bird
        if (birdActive) {
            window.draw(bird);
        }
 
 

if (isPlayerCompletlyBlocked(player_x, maxObstacles, obstacleExists, obstacleX, obstacleY) &&
    obstacleResetClock.getElapsedTime().asSeconds() >= obstacleResetCooldown) {
    // Reset all obstacles to their initial state
    for (int i = 0; i < maxObstacles; ++i) {
        obstacleExists[i] = false;  // Clear all existing obstacles
    }
    
    // Restart the reset clock
    obstacleResetClock.restart();



    // Ensure we have spray cans to remove
                        k = 0;
                        if(j+1<3)
                            j++;
                        
                        sprayCanRectangles[j-1].setPosition(-100.f, -100.f);
                        
                        for (int i = 0; i < 2; i++) {
                            sprayCanBullets[i] = sprayCanBullets[i + 1];
                            sprayCanSprites[i] = sprayCanSprites[i + 1];
                        }
                        sprayCanBullets[2] = maxBullets; 



}



// Draw obstacles
for (int i = 0; i < maxObstacles; ++i) {
if(obstacleExists[i]) {
    window.draw(obstacleSprites[i]);
 }
}
    
// Update and move the bullet// new addition
            if (bullet_exists) {
                moveBullet(bullet_y, bullet_exists, bulletClock);
            }




    // Draw player sprite and health bar
    drawPlayer(window, player_x, player_y, playerSprite, mainHealth, tempVar);



    // Draw bullet if it exists
    if (bullet_exists) {
        drawBullet(window, bullet_x, bullet_y, bulletSprite);
    }



}
else if(currentLevel==2)// MAIN LEVEL 2 with hunter bee and pre-generated difference
{


    // Draw ground
    window.draw(groundRectangle);

    // Draw spray cans and their rectangles
    for (int i = 0; i < 3; i++) {
        window.draw(sprayCanSprites[i]);      // Draw spray can sprites
        window.draw(sprayCanRectangles[i]);  // Draw spray can health bars
    }


 // Spawn bees if not all are generated
    if (beesSpawned < maxBees-5 && beeSpawnClock.getElapsedTime().asMilliseconds() > beeSpawnInterval) {
        if (rand() % 100 < 2) { // Small chance to spawn a bee on each frame
            for (int i = 0; i < maxBees-5; ++i) {
                if (!beeExists[i] && !honeycombExists[i]) {
                    beePositionsX[i] = (rand() % 2 == 0) ? 0 : resolutionX - 32; // Spawn left or right
                    beePositionsY[i] = 0;
                    beeExists[i] = true;
                    beeSprites[i].setTexture(beeTexture);
                    beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);
                    beeMovingRight[i] = (beePositionsX[i] == 0); // Start moving right if spawned on the left
                    beesSpawned++;
                    break;
                }
            }
            
            beeSpawnClock.restart(); // Reset the spawn clock
        }
    }

float beeDeltaTime = beeClock.restart().asSeconds();


// Move bees in a zigzag pattern
for (int i = 0; i < maxBees-5; ++i) 
{

if (beeExists[i]) {
        
//------------------------------------------stopped bees getting shot---------------------------------------------
        
         if (bullet_exists &&
            bullet_x + 16 >= beePositionsX[i] &&
            bullet_x <= beePositionsX[i] + 32 &&
            bullet_y + 16 >= beePositionsY[i] &&
            bullet_y <= beePositionsY[i] + 32) {
            // Remove the bee
            beeExists[i] = false;
            score+=100;
            // Create a honeycomb at the bee's position
            honeycombExists[i] = true;
            honeycombX[i] = beePositionsX[i];
            honeycombY[i] = beePositionsY[i];
            honeycombSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
        }
        
 
//----------------------------------------------------------------------------------------------------------------------      
        
     // random bees stopping logic   
        if (beeStopped[i]) {
            // Check if stop duration has elapsed
            if (beeStopClock[i].getElapsedTime().asSeconds() >= stopDuration) {
                beeStopped[i] = false;
            }
            continue; // Skip movement while stopped
        }

        // Occasionally stop the bee
        if (rand() % 5000 == 0) {  //probability
            beeStopped[i] = true;
            beeStopClock[i].restart();
            continue;
        }
        
//-----------------------------------------------------------------------------------------------------------------------   
        
        
            // Move horizontally
            if (beeMovingRight[i]) {
                beePositionsX[i] += beeSpeed * beeDeltaTime * 200;
                if (beePositionsX[i] >= resolutionX - 32) { // Reached right edge
                    beeMovingRight[i] = false; // Change direction
                    beePositionsY[i] += descentStep; // Descend
                    
                    
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees-5; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            // Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                 
                }
           
          beeExists[i] = false;
            break; // Exit honeycomb loop
        }
    }
}
       
                }
            } else {
                beePositionsX[i] -= beeSpeed * beeDeltaTime * 200;
                if (beePositionsX[i] <= 0) { // Reached left edge
                    beeMovingRight[i] = true; // Change direction
                    beePositionsY[i] += descentStep; // Descend
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees-5; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            //Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                 
                }
           
          beeExists[i] = false;
            break; //Exit honeycomb loop
        }
    }
}
                    
                    
                    
                }
            }
		
	//Check for collision with honeycombs
        for (int j = 0; j < maxBees-5; ++j) {
            if (honeycombExists[j]) {
                //Check if bee's bounds intersect honeycomb's bounds
                if (beePositionsX[i] + 32 > honeycombX[j] &&
                    beePositionsX[i] < honeycombX[j] + 32 &&
                    beePositionsY[i] + 16 > honeycombY[j] &&
                    beePositionsY[i] < honeycombY[j] + 16) {
                    // On collision, descend and change direction
                    beeMovingRight[i] = !beeMovingRight[i];
                    beePositionsY[i] += descentStep;
                
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees-5; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            // Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                  beeExists[i] = false;
            	  break; //Exit honeycomb loop
                }
           
          
        }
    }
} 
                    break; //Exit honeycomb loop for this bee
                }
            }     
        }	
        
        
        
        if (beeExists[i]) {

        // Check if bee has crossed the specified height
        if (beePositionsY[i] > resolutionY - 160) {
            // Remove the bee
            beeExists[i] = false;
            // Place an obstacle at or near the bee's X-position
            float newObstacleX = beePositionsX[i];
            bool placed = false;
            
        
// Adjusted Obstacle Placement Logic
for (int j = 0; j < maxObstacles; j++) {
    
        // First pollination logic
        if (firstpollination) {
            // Place two obstacles at fixed positions
            if (j < 2) {
            
                obstacleExists[j] = true;
                obstacleX[j] = (j == 0) ? (resolutionX - 64) : 0; 
                obstacleY[j] = resolutionY - 120;
                obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
                
                if(j==0)
                continue; // only for the first flower, because we have to set the pollination to false for the rest of as well. 
                // the rest will follow conventional rules.
                
                if (j == 1) {  // Only set these flags when the second obstacle is placed
                    firstpollinationObstacle = true;
                    firstpollination = false;
                }
            } 
            
             break; // only to print 2 flowers not 3   
        } 
        
        
     if(!obstacleExists[j] && j>1){
        float newObstacleX = beePositionsX[i];
      

        while (isObstacleNearby(newObstacleX, j, maxObstacles, obstacleExists, obstacleX)) {
            if (player_x < beePositionsX[i]) {
                newObstacleX -= 3; // Move left
            } else {
                newObstacleX += 3; // Move right
            }

            // Wrap around the screen if necessary
            if (newObstacleX < 0) {
                newObstacleX += resolutionX;
            } else if (newObstacleX >= resolutionX) {
                newObstacleX -= resolutionX;
            }
        }
        
            obstacleExists[j] = true;
            obstacleX[j] = newObstacleX;
            obstacleY[j] = resolutionY - 120;
            obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
        
        break;
        
        
        
    }
}


        }
    }
    
    
    
// Check for collision with emojis
for (int i = 0; i < maxEmojis-5; ++i) {
    if (emojiExists[i]) {
        // Check if bullet collides with emoji
        if (bullet_exists &&
            bullet_x + 32 >= emojiX[i] &&
            bullet_x <= emojiX[i] + 32 &&
            bullet_y + 16 >= emojiY[i] &&
            bullet_y <= emojiY[i] + 16) {
            // Remove the emoji upon collision with the bullet
            emojiExists[i] = false;
            bullet_exists = false; // Bullet is destroyed on hit
            break;
        }
    }
}
	
            beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

		}
			
         if (beeExists[i]) {
        // Check if the bullet hits a bee
        if (bullet_exists &&
            bullet_x + 16 >= beePositionsX[i] &&
            bullet_x <= beePositionsX[i] + 32 &&
            bullet_y + 16 >= beePositionsY[i] &&
            bullet_y <= beePositionsY[i] + 32) {
            // Remove the bee
            beeExists[i] = false;
            score+=100;

            // Create a honeycomb at the bee's position
            honeycombExists[i] = true;
            honeycombX[i] = beePositionsX[i];
    	    honeycombY[i] = beePositionsY[i];
            honeycombSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
               
               
               
            }
        }


        // Check for collision with honeycombs
        for (int j = 0; j < maxBees-5; ++j) {
            if (honeycombExists[j]) {
                // Check if bullet collides with honeycomb
                if (bullet_exists &&
                    bullet_x + 32 >= honeycombX[j] &&
                    bullet_x <= honeycombX[j] + 32 &&
                    bullet_y + 16 >= honeycombY[j] &&
                    bullet_y <= honeycombY[j] + 16) {
                    // Remove the honeycomb upon collision with the bullet
                    honeycombExists[j] = false;
                    bullet_exists = false;  // Bullet is also destroyed on hit
                }
            } 
            
            }
            
          
    }// main bee loop ender 


     // Spawn hunter bees if not all are generated
    if (hunterbeesSpawned < maxhunterBees-5 && hunterbeeSpawnClock.getElapsedTime().asMilliseconds() > hunterbeeSpawnInterval) {
        if (rand() % 100 < 2) { // Small chance to spawn a bee on each frame
            for (int i = 0; i < maxhunterBees-5; ++i) {
                if (!hunterbeeExists[i] && !hunterhoneycombExists[i]) {
                    hunterbeePositionsX[i] = (rand() % 2 == 0) ? 0 : resolutionX - 32; // Spawn left or right
                    hunterbeePositionsY[i] = 0;
                    hunterbeeExists[i] = true;
                    hunterbeeSprites[i].setTexture(hunterbeeTexture);
                    hunterbeeSprites[i].setPosition(hunterbeePositionsX[i], hunterbeePositionsY[i]);
                    hunterbeeMovingRight[i] = (hunterbeePositionsX[i] == 0); // Start moving right if spawned on the left
                    hunterbeesSpawned++;
                    break;
                }
            }
            
            hunterbeeSpawnClock.restart(); // Reset the spawn clock
        }
    }

float hunterbeeDeltaTime = hunterbeeClock.restart().asSeconds();


// Move hunter bees in a zigzag pattern
for (int i = 0; i < maxhunterBees-5; ++i) 
{

if (hunterbeeExists[i]) {
        
//-----------------------------------------------------------------------------------------------------------------------   
        
        
            // Move horizontally
            if (hunterbeeMovingRight[i]) {
                hunterbeePositionsX[i] += hunterbeeSpeed * hunterbeeDeltaTime * 200;
                if (hunterbeePositionsX[i] >= resolutionX - 32) { // Reached right edge
                    hunterbeeMovingRight[i] = false; // Change direction
                    hunterbeePositionsY[i] += descentStep; // Descend
                           
                }
            } else {
                hunterbeePositionsX[i] -= hunterbeeSpeed * hunterbeeDeltaTime * 200;
                if (hunterbeePositionsX[i] <= 0) { // Reached left edge
                    hunterbeeMovingRight[i] = true; // Change direction
                    hunterbeePositionsY[i] += descentStep; // Descend                 
                                        
                }
            }
	
        
        
        if (hunterbeeExists[i]) {

        // Check if bee has crossed the specified height
        if (hunterbeePositionsY[i] > resolutionY - 160) {
            // Remove the bee
            hunterbeeExists[i] = false;
            // Place an obstacle at or near the bee's X-position
            float newObstacleX = hunterbeePositionsX[i];
            bool placed = false;

        
// Adjusted Obstacle Placement Logic
for (int j = 0; j < maxhunterBees-5; j++) {
    
        // First pollination logic
        if (firstpollination) {
            // Place two obstacles at fixed positions
            if (j < 2) {
            
                obstacleExists[j] = true;
                obstacleX[j] = (j == 0) ? (resolutionX - 64) : 0; 
                obstacleY[j] = resolutionY - 120;
                obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
                
                if(j==0)
                continue; // only for the first flower, because we have to set the pollination to false for the rest of as well. 
                // the rest will follow conventional rules.
                
                if (j == 1) {  // Only set these flags when the second obstacle is placed
                    firstpollinationObstacle = true;
                    firstpollination = false;
                }
            } 
            
             break; // only to print 2 flowers not 3   
        } 
        
        
     if(!obstacleExists[j] && j>1){
        float newObstacleX = hunterbeePositionsX[i];
      

        while (isObstacleNearby(newObstacleX, j, maxObstacles, obstacleExists, obstacleX)) {
            if (player_x < hunterbeePositionsX[i]) {
                newObstacleX -= 3; // Move left
            } else {
                newObstacleX += 3; // Move right
            }

            // Wrap around the screen if necessary
            if (newObstacleX < 0) {
                newObstacleX += resolutionX;
            } else if (newObstacleX >= resolutionX) {
                newObstacleX -= resolutionX;
            }
        }
        
            obstacleExists[j] = true;
            obstacleX[j] = newObstacleX;
            obstacleY[j] = resolutionY - 120;
            obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
        
        break;
        
        
        
    }
}
    
        }
    }
    	
            hunterbeeSprites[i].setPosition(hunterbeePositionsX[i], hunterbeePositionsY[i]);

		}
			
         if (hunterbeeExists[i]) {
        // Check if the bullet hits a bee
        if (bullet_exists &&
            bullet_x + 16 >= hunterbeePositionsX[i] &&
            bullet_x <= hunterbeePositionsX[i] + 32 &&
            bullet_y + 16 >= hunterbeePositionsY[i] &&
            bullet_y <= hunterbeePositionsY[i] + 32) {
            // Remove the bee
            hunterbeeExists[i] = false;
            score+=1000;

            // Create a honeycomb at the bee's position
            hunterhoneycombExists[i] = true;
            hunterhoneycombX[i] = hunterbeePositionsX[i];
    	    hunterhoneycombY[i] = hunterbeePositionsY[i];
            hunterhoneycombSprites[i].setPosition(hunterbeePositionsX[i], hunterbeePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
               
               
               
            }
        }




// Check for collision with hunter honeycombs
        for (int j = 0; j < maxhunterBees-5; ++j) {
            if (hunterhoneycombExists[j]) {
                // Check if bullet collides with honeycomb
                if (bullet_exists &&
                    bullet_x + 32 >= hunterhoneycombX[j] &&
                    bullet_x <= hunterhoneycombX[j] + 32 &&
                    bullet_y + 16 >= hunterhoneycombY[j] &&
                    bullet_y <= hunterhoneycombY[j] + 16) {
                    // Remove the honeycomb upon collision with the bullet
                    hunterhoneycombExists[j] = false;
                    bullet_exists = false;  // Bullet is also destroyed on hit
                }
            } 
}




          
    }// main hunter bee loop ender 



	    int activeBeeCount = 0;
for(int i = 0; i < maxBees-5; i++) {
    if(beeExists[i]) {
        activeBeeCount++;
    }
}



	    int hunteractiveBeeCount = 0;
for(int i = 0; i < maxhunterBees-5 ; i++) {
    if(hunterbeeExists[i]) {
        hunteractiveBeeCount++;
    }
}



// Check if no active bees remain
if(activeBeeCount==0 && hunteractiveBeeCount==0 && beesSpawned>0 && hunterbeesSpawned>0) {
    
    

// Reset spray cans
        j = 0;
        k = 0;
        tempVar = 0;
        sprayCanBullets[0] = maxBullets-1;
        sprayCanBullets[1] = maxBullets;
        sprayCanBullets[2] = maxBullets;
        bulletCounter = 0;


        firstpollinationObstacle = false;
        firstpollination = true;

    for (int i = 0; i < 3; i++) {
        sprayCanSprites[i].setTexture(sprayCanTexture);
        sprayCanSprites[i].setPosition(32 * i + 32*i, resolutionY - 64);
    }



    for (int i = 0; i < maxBees; ++i) {
            beeExists[i] = false;
            honeycombExists[i] = false;
            emojiExists[i] = false;
            
        }



   
    for(int i = 0;i<maxObstacles;i++)
    {

        obstacleExists[i] = false;
        obstacleX[i]=0;
        obstacleY[i]=resolutionY-100;
    }




beesSpawned=0;
hunterbeesSpawned=0;



 for (int i = 0; i < maxhunterBees; ++i) {
            hunterbeeExists[i] = false;
            hunterhoneycombExists[i] = false;
        }


for(int i=0;i<15;i++)
            { honeycombX[i] = rand()%800;
            honeycombY[i] = rand()%300;
            honeycombExists[i] = true;
            honeycombSprites[i].setPosition(honeycombX[i],honeycombY[i]);
         
            }

	
        birdX = -200, birdY = 200;
        
        // Reset bullet
        bullet_exists = false;

        // Reset health bar
        mainHealth.setSize(Vector2f(20.f,30.f));
        
        // Reset spray can rectangles
        for (int i = 0; i < 3; i++) {
            sprayCanRectangles[i].setSize(Vector2f(20.f, 28.f));
            sprayCanRectangles[i].setPosition(22 + 32 * i + 32*i, resolutionY - 30);
        }
          



        for(int j = 0;j<maxEmojis;j++)
        {
            if(emojiExists[j])
            {
                if(emojiY[j]>=0 && emojiY[j]<=100)
                {score+=2000;}
                else if(emojiY[j]>100 && emojiY[j]<=250)
                {score+=1600;}
                else if(emojiY[j]>250)
                {score+=1000;}

            }
        }




    
    currentLevel=3; // Set game over if all bees are dead
}
       
//-----------------------------------------------------------------------------------------------------------------------------------

 if (birdClock.getElapsedTime().asSeconds() > 0.025) {
    if (birdActive) {
        // Bird hit detection
        if (bullet_exists &&
            bullet_x + 16 >= birdX &&
            bullet_x <= birdX + 64 && // Assuming bird is larger due to scaling
            bullet_y + 16 >= birdY &&
            bullet_y <= birdY + 64) {
            birdHitCount++;
            bullet_exists = false; // Remove bullet

            // If bird is hit 3 times, move off screen
            if (birdHitCount >= 3) {
                isBirdMovingOffScreen = true;
                birdOffScreenClock.restart();
                birdHitCount = 0; // Reset hit count
            }
        }

        // Bird off-screen movement logic
        if (isBirdMovingOffScreen) {
            // Smoothly move off screen
            if (birdX > -200) {
                birdX -= 3; // Gradually move left
            }

if(birdX<=-200){
            if (birdOffScreenClock.getElapsedTime().asSeconds() >= 3.0) {
                isBirdMovingOffScreen = false;
            }}
        }
        else {
            
            
            //If bird is currently rerouting, check pause duration
        if (isBirdRerouting) {
            if (birdRoutingClock.getElapsedTime().asSeconds() >= 2.0) {
                //Pause is over, allow bird to move again
                isBirdRerouting = false;
        } }
           
           
           
                // Find nearest honeycomb (existing code)
                int nearestHoneycombIndex = -1;
                float minDistance = FLT_MAX;

                for (int i = 0; i < maxBees-5; ++i) {
                    if (honeycombExists[i] ) {
                        float dx = honeycombX[i] - birdX;
                        float dy = honeycombY[i] - birdY;
                        float distance = sqrt(dx * dx + dy * dy);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestHoneycombIndex = i;
                        }
                    }
                    else if (hunterhoneycombExists[i])
                    {

                        float dx = hunterhoneycombX[i] - birdX;
                        float dy = hunterhoneycombY[i] - birdY;
                        float distance = sqrt(dx * dx + dy * dy);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestHoneycombIndex = i;
                        }

                    }
                }
                
                 bool temp = false; 
                
                if (nearestHoneycombIndex != -1){
            // Check if bird has changed target honeycomb
            static int previousTarget = -1;
            if (previousTarget != -1 && previousTarget != nearestHoneycombIndex) {
                //Bird is rerouting to a new honeycomb
                isBirdRerouting = true;
                birdRoutingClock.restart();
            }
            temp = true;
            previousTarget = nearestHoneycombIndex; }

                
                

                // Move towards nearest honeycomb
                if (!isBirdRerouting) {

                    if(honeycombExists[nearestHoneycombIndex]){

                    if (birdX < honeycombX[nearestHoneycombIndex]) birdX += 2;
                    if (birdX > honeycombX[nearestHoneycombIndex]) birdX -= 2;
                    if (birdY < honeycombY[nearestHoneycombIndex]) birdY += 2;
                    if (birdY > honeycombY[nearestHoneycombIndex]) birdY -= 2;
                    
                    }
                    else if(hunterhoneycombExists[nearestHoneycombIndex]){

                    if (birdX < hunterhoneycombX[nearestHoneycombIndex]) birdX += 2;
                    if (birdX > hunterhoneycombX[nearestHoneycombIndex]) birdX -= 2;
                    if (birdY < hunterhoneycombY[nearestHoneycombIndex]) birdY += 2;
                    if (birdY > hunterhoneycombY[nearestHoneycombIndex]) birdY -= 2;

                    }


                    // Collect honeycomb logic remains the same
                    if (sqrt(pow(birdX - honeycombX[nearestHoneycombIndex], 2) +
                             pow(birdY - honeycombY[nearestHoneycombIndex], 2)) < 16 && 
                        EatingTime.getElapsedTime().asSeconds() > 5) {
                        honeycombExists[nearestHoneycombIndex] = false;

if(temp){
                         if(honeycombY[nearestHoneycombIndex]>=0.f && honeycombY[nearestHoneycombIndex]<=100.f) //first tier
                         score+=1000;
                         else if(honeycombY[nearestHoneycombIndex]>100.f && honeycombY[nearestHoneycombIndex]<=250.f)
                         score+=800;
                         else if(honeycombY[nearestHoneycombIndex]>250.f)
                         score+=500;}


                    }
                    else if(sqrt(pow(birdX - hunterhoneycombX[nearestHoneycombIndex], 2) +
                             pow(birdY - hunterhoneycombY[nearestHoneycombIndex], 2)) < 16 && 
                        EatingTime.getElapsedTime().asSeconds() > 5){
                        hunterhoneycombExists[nearestHoneycombIndex] = false;


if(temp){
                         if(honeycombY[nearestHoneycombIndex]>=0.f && honeycombY[nearestHoneycombIndex]<=100.f) //first tier
                         score+=2000;
                         else if(honeycombY[nearestHoneycombIndex]>100.f && honeycombY[nearestHoneycombIndex]<=250.f)
                         score+=1800;
                         else if(honeycombY[nearestHoneycombIndex]>250.f)
                         score+=1500;}



                        }

                }
            }
	  bird.setPosition(birdX, birdY);
        birdClock.restart();
    }
}



//-------------------------------------------------------------------------------------------------------------------------------


    // Draw bees
    for (int i = 0; i < maxBees-5; ++i) {
        if (beeExists[i]) {
            window.draw(beeSprites[i]);
        }
    }
    
    
   // Draw honeycombs
for (int i = 0; i < maxBees-5; ++i) {
    if (honeycombExists[i]) {
        window.draw(honeycombSprites[i]);
    }
}





    // Draw  hunter bees
    for (int i = 0; i < maxhunterBees-5; ++i) {
        if (hunterbeeExists[i]) {
            window.draw(hunterbeeSprites[i]);
        }
    }
    
    
   // Draw hunter honeycombs
for (int i = 0; i < maxhunterBees-5; ++i) {
    if (hunterhoneycombExists[i]) {
        window.draw(hunterhoneycombSprites[i]);
    }
}
        
 
// Draw emojis
for (int i = 0; i < maxBees-5; ++i) {
    if (emojiExists[i]) {
        window.draw(emojiSprites[i]);
    }
}
 
 
  // Draw bird
        if (birdActive) {
            window.draw(bird);
        }
 


if (isPlayerCompletlyBlocked(player_x, maxObstacles, obstacleExists, obstacleX, obstacleY) &&
    obstacleResetClock.getElapsedTime().asSeconds() >= obstacleResetCooldown) {
    // Reset all obstacles to their initial state
    for (int i = 0; i < maxObstacles; ++i) {
        obstacleExists[i] = false;  // Clear all existing obstacles
    }
    
    // Restart the reset clock
    obstacleResetClock.restart();



    // Ensure we have spray cans to remove
                        k = 0;
                        if(j+1<3)
                            j++;
                        
                        sprayCanRectangles[j-1].setPosition(-100.f, -100.f);
                        
                        for (int i = 0; i < 2; i++) {
                            sprayCanBullets[i] = sprayCanBullets[i + 1];
                            sprayCanSprites[i] = sprayCanSprites[i + 1];
                        }
                        sprayCanBullets[2] = maxBullets; 



}

 

    // Draw obstacles
for (int i = 0; i < maxBees-5; ++i) {
if(obstacleExists[i]) {
    window.draw(obstacleSprites[i]);
 }
}
    
    
    

	// Update and move the bullet// new addition
            if (bullet_exists) {
                moveBullet(bullet_y, bullet_exists, bulletClock);
            }




    // Draw player sprite and health bar
    drawPlayer(window, player_x, player_y, playerSprite, mainHealth, tempVar);



    // Draw bullet if it exists
    if (bullet_exists) {
        drawBullet(window, bullet_x, bullet_y, bulletSprite);
    }


}
else if(currentLevel==3)
{                                   



                            // Draw ground
    window.draw(groundRectangle);

    // Draw spray cans and their rectangles
    for (int i = 0; i < 3; i++) {
        window.draw(sprayCanSprites[i]);      // Draw spray can sprites
        window.draw(sprayCanRectangles[i]);  // Draw spray can health bars
    }


 // Spawn bees if not all are generated
    if (beesSpawned < maxBees && beeSpawnClock.getElapsedTime().asMilliseconds() > beeSpawnInterval) {
        if (rand() % 100 < 2) { // Small chance to spawn a bee on each frame
            for (int i = 0; i < maxBees-5; ++i) {
                if (!beeExists[i] && !honeycombExists[i]) {
                    beePositionsX[i] = (rand() % 2 == 0) ? 0 : resolutionX - 32; // Spawn left or right
                    beePositionsY[i] = 0;
                    beeExists[i] = true;
                    beeSprites[i].setTexture(beeTexture);
                    beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);
                    beeMovingRight[i] = (beePositionsX[i] == 0); // Start moving right if spawned on the left
                    beesSpawned++;
                    break;
                }
            }
            
            beeSpawnClock.restart(); // Reset the spawn clock
        }
    }

float beeDeltaTime = beeClock.restart().asSeconds();


// Move bees in a zigzag pattern
for (int i = 0; i < maxBees; ++i) 
{

if (beeExists[i]) {
        
//------------------------------------------stopped bees getting shot---------------------------------------------
        
         if (bullet_exists &&
            bullet_x + 16 >= beePositionsX[i] &&
            bullet_x <= beePositionsX[i] + 32 &&
            bullet_y + 16 >= beePositionsY[i] &&
            bullet_y <= beePositionsY[i] + 32) {
            // Remove the bee
            beeExists[i] = false;
            score+=100;

            // Create a honeycomb at the bee's position
            honeycombExists[i] = true;
            honeycombX[i] = beePositionsX[i];
            honeycombY[i] = beePositionsY[i];
            honeycombSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
        }
        
 
//----------------------------------------------------------------------------------------------------------------------      
        
     // random bees stopping logic   
        if (beeStopped[i]) {
            // Check if stop duration has elapsed
            if (beeStopClock[i].getElapsedTime().asSeconds() >= stopDuration) {
                beeStopped[i] = false;
            }
            continue; // Skip movement while stopped
        }

        // Occasionally stop the bee
        if (rand() % 5000 == 0) {  //probability
            beeStopped[i] = true;
            beeStopClock[i].restart();
            continue;
        }
        
//-----------------------------------------------------------------------------------------------------------------------   
        
        
            // Move horizontally
            if (beeMovingRight[i]) {
                beePositionsX[i] += beeSpeed * beeDeltaTime * 200;
                if (beePositionsX[i] >= resolutionX - 32) { // Reached right edge
                    beeMovingRight[i] = false; // Change direction
                    beePositionsY[i] += descentStep; // Descend
                    
                    
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            // Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                 
                }
           
          beeExists[i] = false;
            break; // Exit honeycomb loop
        }
    }
}
       
                }
            } else {
                beePositionsX[i] -= beeSpeed * beeDeltaTime * 200;
                if (beePositionsX[i] <= 0) { // Reached left edge
                    beeMovingRight[i] = true; // Change direction
                    beePositionsY[i] += descentStep; // Descend
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            //Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                 
                }
           
          beeExists[i] = false;
            break; //Exit honeycomb loop
        }
    }
}
                    
                    
                    
                }
            }
		
	//Check for collision with honeycombs
        for (int j = 0; j < maxBees; ++j) {
            if (honeycombExists[j]) {
                //Check if bee's bounds intersect honeycomb's bounds
                if (beePositionsX[i] + 32 > honeycombX[j] &&
                    beePositionsX[i] < honeycombX[j] + 32 &&
                    beePositionsY[i] + 16 > honeycombY[j] &&
                    beePositionsY[i] < honeycombY[j] + 16) {
                    // On collision, descend and change direction
                    beeMovingRight[i] = !beeMovingRight[i];
                    beePositionsY[i] += descentStep;
                
//------------------------------------------------------------BEEHIVE LOGIC------------------------------------------------

if (beeExists[i] && beePositionsY[i] > 0) {
    for (int j = 0; j < maxBees; ++j) {
        if (honeycombExists[j] &&
            beePositionsX[i] + 32 > honeycombX[j] &&
            beePositionsX[i] < honeycombX[j] + 32 &&
            beePositionsY[i] + 32 > honeycombY[j] &&
            beePositionsY[i] < honeycombY[j] + 32) {
            
            // Create emoji at bee's position
           
                if (!emojiExists[j]) {
                    emojiExists[j] = true;
                    emojiX[j] = beePositionsX[i];
                    emojiY[j] = beePositionsY[i];
                    emojiSprites[j].setPosition(emojiX[j], emojiY[j]-descentStep);
                  beeExists[i] = false;
            	  break; //Exit honeycomb loop
                }
           
          
        }
    }
} 
                    break; //Exit honeycomb loop for this bee
                }
            }     
        }	
        
        
        
        if (beeExists[i]) {
        // Move bees as before (zigzag logic)

        // Check if bee has crossed the specified height
        if (beePositionsY[i] > resolutionY - 160) {
            // Remove the bee
            beeExists[i] = false;
            // Place an obstacle at or near the bee's X-position
            float newObstacleX = beePositionsX[i];
            bool placed = false;
            
        
// Adjusted Obstacle Placement Logic
for (int j = 0; j < maxObstacles; j++) {
    
        // First pollination logic
        if (firstpollination) {
            // Place two obstacles at fixed positions
            if (j < 2) {
            
                obstacleExists[j] = true;
                obstacleX[j] = (j == 0) ? (resolutionX - 64) : 0; 
                obstacleY[j] = resolutionY - 120;
                obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
                
                if(j==0)
                continue; // only for the first flower, because we have to set the pollination to false for the rest of as well. 
                // the rest will follow conventional rules.
                
                if (j == 1) {  // Only set these flags when the second obstacle is placed
                    firstpollinationObstacle = true;
                    firstpollination = false;
                }
            } 
            
             break; // only to print 2 flowers not 3   
        } 
        
        
     if(!obstacleExists[j] && j>1){
        float newObstacleX = beePositionsX[i];
      

        while (isObstacleNearby(newObstacleX, j, maxObstacles, obstacleExists, obstacleX)) {
            if (player_x < beePositionsX[i]) {
                newObstacleX -= 3; // Move left
            } else {
                newObstacleX += 3; // Move right
            }

            // Wrap around the screen if necessary
            if (newObstacleX < 0) {
                newObstacleX += resolutionX;
            } else if (newObstacleX >= resolutionX) {
                newObstacleX -= resolutionX;
            }
        }
        
            obstacleExists[j] = true;
            obstacleX[j] = newObstacleX;
            obstacleY[j] = resolutionY - 120;
            obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
        
        break;
        
        
        
    }
}


        }
    }
    
    
    
// Check for collision with emojis
for (int i = 0; i < maxEmojis; ++i) {
    if (emojiExists[i]) {
        // Check if bullet collides with emoji
        if (bullet_exists &&
            bullet_x + 32 >= emojiX[i] &&
            bullet_x <= emojiX[i] + 32 &&
            bullet_y + 16 >= emojiY[i] &&
            bullet_y <= emojiY[i] + 16) {
            // Remove the emoji upon collision with the bullet
            emojiExists[i] = false;
            bullet_exists = false; // Bullet is destroyed on hit
            break;
        }
    }
}
	
            beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

		}
			
         if (beeExists[i]) {
        // Check if the bullet hits a bee
        if (bullet_exists &&
            bullet_x + 16 >= beePositionsX[i] &&
            bullet_x <= beePositionsX[i] + 32 &&
            bullet_y + 16 >= beePositionsY[i] &&
            bullet_y <= beePositionsY[i] + 32) {
            // Remove the bee
            beeExists[i] = false;
            score+=100;

            // Create a honeycomb at the bee's position
            honeycombExists[i] = true;
            honeycombX[i] = beePositionsX[i];
    	    honeycombY[i] = beePositionsY[i];
            honeycombSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
               
               
               
            }
        }


        // Check for collision with honeycombs
        for (int j = 0; j < maxBees; ++j) {
            if (honeycombExists[j]) {
                // Check if bullet collides with honeycomb
                if (bullet_exists &&
                    bullet_x + 32 >= honeycombX[j] &&
                    bullet_x <= honeycombX[j] + 32 &&
                    bullet_y + 16 >= honeycombY[j] &&
                    bullet_y <= honeycombY[j] + 16) {
                    // Remove the honeycomb upon collision with the bullet
                    honeycombExists[j] = false;
                    bullet_exists = false;  // Bullet is also destroyed on hit
                }
            } 
            
            }
            
          
    }// main bee loop ender 


     // Spawn hunter bees if not all are generated
    if (hunterbeesSpawned < maxhunterBees && hunterbeeSpawnClock.getElapsedTime().asMilliseconds() > hunterbeeSpawnInterval) {
        if (rand() % 100 < 2) { // Small chance to spawn a bee on each frame
            for (int i = 0; i < maxhunterBees; ++i) {
                if (!hunterbeeExists[i] && !hunterhoneycombExists[i]) {
                    hunterbeePositionsX[i] = (rand() % 2 == 0) ? 0 : resolutionX - 32; // Spawn left or right
                    hunterbeePositionsY[i] = 0;
                    hunterbeeExists[i] = true;
                    hunterbeeSprites[i].setTexture(hunterbeeTexture);
                    hunterbeeSprites[i].setPosition(hunterbeePositionsX[i], hunterbeePositionsY[i]);
                    hunterbeeMovingRight[i] = (hunterbeePositionsX[i] == 0); // Start moving right if spawned on the left
                    hunterbeesSpawned++;
                    break;
                }
            }
            
            hunterbeeSpawnClock.restart(); // Reset the spawn clock
        }
    }

float hunterbeeDeltaTime = hunterbeeClock.restart().asSeconds();


// Move hunter bees in a zigzag pattern
for (int i = 0; i < maxhunterBees; ++i) 
{

if (hunterbeeExists[i]) {
        
//-----------------------------------------------------------------------------------------------------------------------   
        
        
            // Move horizontally
            if (hunterbeeMovingRight[i]) {
                hunterbeePositionsX[i] += hunterbeeSpeed * hunterbeeDeltaTime * 200;
                if (hunterbeePositionsX[i] >= resolutionX - 32) { // Reached right edge
                    hunterbeeMovingRight[i] = false; // Change direction
                    hunterbeePositionsY[i] += descentStep; // Descend
                           
                }
            } else {
                hunterbeePositionsX[i] -= hunterbeeSpeed * hunterbeeDeltaTime * 200;
                if (hunterbeePositionsX[i] <= 0) { // Reached left edge
                    hunterbeeMovingRight[i] = true; // Change direction
                    hunterbeePositionsY[i] += descentStep; // Descend                 
                                        
                }
            }
	
        
        
        if (hunterbeeExists[i]) {

        // Check if bee has crossed the specified height
        if (hunterbeePositionsY[i] > resolutionY - 160) {
            // Remove the bee
            hunterbeeExists[i] = false;
            // Place an obstacle at or near the bee's X-position
            float newObstacleX = hunterbeePositionsX[i];
            bool placed = false;  
        
// Adjusted Obstacle Placement Logic
for (int j = 0; j < maxhunterBees; j++) {
    
        // First pollination logic
        if (firstpollination) {
            // Place two obstacles at fixed positions
            if (j < 2) {
            
                obstacleExists[j] = true;
                obstacleX[j] = (j == 0) ? (resolutionX - 64) : 0; 
                obstacleY[j] = resolutionY - 120;
                obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
                
                if(j==0)
                continue; // only for the first flower, because we have to set the pollination to false for the rest of as well. 
                // the rest will follow conventional rules.
                
                if (j == 1) {  // Only set these flags when the second obstacle is placed
                    firstpollinationObstacle = true;
                    firstpollination = false;
                }
            } 
            
             break; // only to print 2 flowers not 3   
        } 
        
        
     if(!obstacleExists[j] && j>1){
        float newObstacleX = hunterbeePositionsX[i];
      

        while (isObstacleNearby(newObstacleX, j, maxObstacles, obstacleExists, obstacleX)) {
            if (player_x < hunterbeePositionsX[i]) {
                newObstacleX -= 3; // Move left
            } else {
                newObstacleX += 3; // Move right
            }

            // Wrap around the screen if necessary
            if (newObstacleX < 0) {
                newObstacleX += resolutionX;
            } else if (newObstacleX >= resolutionX) {
                newObstacleX -= resolutionX;
            }
        }
        
            obstacleExists[j] = true;
            obstacleX[j] = newObstacleX;
            obstacleY[j] = resolutionY - 120;
            obstacleSprites[j].setPosition(obstacleX[j], obstacleY[j]);
        
        break;
      
    }
}


        

        }
    }
    	
            hunterbeeSprites[i].setPosition(hunterbeePositionsX[i], hunterbeePositionsY[i]);

		}
			
         if (hunterbeeExists[i]) {
        // Check if the bullet hits a bee
        if (bullet_exists &&
            bullet_x + 16 >= hunterbeePositionsX[i] &&
            bullet_x <= hunterbeePositionsX[i] + 32 &&
            bullet_y + 16 >= hunterbeePositionsY[i] &&
            bullet_y <= hunterbeePositionsY[i] + 32) {
            // Remove the bee
            hunterbeeExists[i] = false;
            score+=1000;

            // Create a honeycomb at the bee's position
            hunterhoneycombExists[i] = true;
            hunterhoneycombX[i] = hunterbeePositionsX[i];
    	    hunterhoneycombY[i] = hunterbeePositionsY[i];
            hunterhoneycombSprites[i].setPosition(hunterbeePositionsX[i], hunterbeePositionsY[i]);

            // Remove the bullet after hitting a bee
            bullet_exists = false;
               
               
               
            }
        }




// Check for collision with hunter honeycombs
        for (int j = 0; j < maxhunterBees; ++j) {
            if (hunterhoneycombExists[j]) {
                // Check if bullet collides with honeycomb
                if (bullet_exists &&
                    bullet_x + 32 >= hunterhoneycombX[j] &&
                    bullet_x <= hunterhoneycombX[j] + 32 &&
                    bullet_y + 16 >= hunterhoneycombY[j] &&
                    bullet_y <= hunterhoneycombY[j] + 16) {
                    // Remove the honeycomb upon collision with the bullet
                    hunterhoneycombExists[j] = false;
                    bullet_exists = false;  // Bullet is also destroyed on hit
                }
            } 
}




          
    }// main hunter bee loop ender 



	    int activeBeeCount = 0;
for(int i = 0; i < maxBees; i++) {
    if(beeExists[i]) {
        activeBeeCount++;
    }
}



	    int hunteractiveBeeCount = 0;
for(int i = 0; i < maxhunterBees ; i++) {
    if(hunterbeeExists[i]) {
        hunteractiveBeeCount++;
    }
}



// Check if no active bees remain
if(activeBeeCount==0 && hunteractiveBeeCount==0 && beesSpawned>0 && hunterbeesSpawned>0) {
    isGameOver = true; // Set game over if all bees are dead
}
       
//-----------------------------------------------------------------------------------------------------------------------------------

 if (birdClock.getElapsedTime().asSeconds() > 0.025) {
    if (birdActive) {
        // Bird hit detection
        if (bullet_exists &&
            bullet_x + 16 >= birdX &&
            bullet_x <= birdX + 64 && // Assuming bird is larger due to scaling
            bullet_y + 16 >= birdY &&
            bullet_y <= birdY + 64) {
            birdHitCount++;
            bullet_exists = false; // Remove bullet

            // If bird is hit 3 times, move off screen
            if (birdHitCount >= 3) {
                isBirdMovingOffScreen = true;
                birdOffScreenClock.restart();
                birdHitCount = 0; // Reset hit count
            }
        }

        // Bird off-screen movement logic
        if (isBirdMovingOffScreen) {
            // Smoothly move off screen
            if (birdX > -200) {
                birdX -= 3; // Gradually move left
            }

if(birdX<=-200){
            if (birdOffScreenClock.getElapsedTime().asSeconds() >= 3.0) {
                isBirdMovingOffScreen = false;
            }}
        }
        else {
            
            
            //If bird is currently rerouting, check pause duration
        if (isBirdRerouting) {
            if (birdRoutingClock.getElapsedTime().asSeconds() >= 2.0) {
                //Pause is over, allow bird to move again
                isBirdRerouting = false;
        } }
           
           
           
                // Find nearest honeycomb (existing code)
                int nearestHoneycombIndex = -1;
                float minDistance = FLT_MAX;

                for (int i = 0; i < maxBees; ++i) {
                    if (honeycombExists[i] ) {
                        float dx = honeycombX[i] - birdX;
                        float dy = honeycombY[i] - birdY;
                        float distance = sqrt(dx * dx + dy * dy);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestHoneycombIndex = i;
                        }
                    }
                    else if (hunterhoneycombExists[i])//idhr masla ho sakta hai out of bounds wala
                    {

                        float dx = hunterhoneycombX[i] - birdX;
                        float dy = hunterhoneycombY[i] - birdY;
                        float distance = sqrt(dx * dx + dy * dy);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestHoneycombIndex = i;
                        }

                    }
                }
                
              bool temp = false;                    
                if (nearestHoneycombIndex != -1){
            // Check if bird has changed target honeycomb
            static int previousTarget = -1;
            if (previousTarget != -1 && previousTarget != nearestHoneycombIndex) {
                //Bird is rerouting to a new honeycomb
                isBirdRerouting = true;
                birdRoutingClock.restart();
            }
            
            previousTarget = nearestHoneycombIndex;
            temp = true;    }

                
                

                // Move towards nearest honeycomb
                if (!isBirdRerouting) {

                    if(honeycombExists[nearestHoneycombIndex]){

                    if (birdX < honeycombX[nearestHoneycombIndex]) birdX += 2;
                    if (birdX > honeycombX[nearestHoneycombIndex]) birdX -= 2;
                    if (birdY < honeycombY[nearestHoneycombIndex]) birdY += 2;
                    if (birdY > honeycombY[nearestHoneycombIndex]) birdY -= 2;
                    
                    }
                    else if(hunterhoneycombExists[nearestHoneycombIndex]){

                    if (birdX < hunterhoneycombX[nearestHoneycombIndex]) birdX += 2;
                    if (birdX > hunterhoneycombX[nearestHoneycombIndex]) birdX -= 2;
                    if (birdY < hunterhoneycombY[nearestHoneycombIndex]) birdY += 2;
                    if (birdY > hunterhoneycombY[nearestHoneycombIndex]) birdY -= 2;

                    }


                                      // Collect honeycomb logic remains the same
                    if (sqrt(pow(birdX - honeycombX[nearestHoneycombIndex], 2) +
                             pow(birdY - honeycombY[nearestHoneycombIndex], 2)) < 16 && 
                        EatingTime.getElapsedTime().asSeconds() > 5) {
                        honeycombExists[nearestHoneycombIndex] = false;

if(temp){
                         if(honeycombY[nearestHoneycombIndex]>=0.f && honeycombY[nearestHoneycombIndex]<=100.f) //first tier
                         score+=1000;
                         else if(honeycombY[nearestHoneycombIndex]>100.f && honeycombY[nearestHoneycombIndex]<=250.f)
                         score+=800;
                         else if(honeycombY[nearestHoneycombIndex]>250.f)
                         score+=500;}


                    }
                    else if(sqrt(pow(birdX - hunterhoneycombX[nearestHoneycombIndex], 2) +
                             pow(birdY - hunterhoneycombY[nearestHoneycombIndex], 2)) < 16 && 
                        EatingTime.getElapsedTime().asSeconds() > 5){
                        hunterhoneycombExists[nearestHoneycombIndex] = false;


if(temp){
                         if(honeycombY[nearestHoneycombIndex]>=0.f && honeycombY[nearestHoneycombIndex]<=100.f) //first tier
                         score+=2000;
                         else if(honeycombY[nearestHoneycombIndex]>100.f && honeycombY[nearestHoneycombIndex]<=250.f)
                         score+=1800;
                         else if(honeycombY[nearestHoneycombIndex]>250.f)
                         score+=1500;}



                        }

                }
            }
	  bird.setPosition(birdX, birdY);
        birdClock.restart();
    }
}



//-------------------------------------------------------------------------------------------------------------------------------


    // Draw bees
    for (int i = 0; i < maxBees; ++i) {
        if (beeExists[i]) {
            window.draw(beeSprites[i]);
        }
    }
    
    
   // Draw honeycombs
for (int i = 0; i < maxBees; ++i) {
    if (honeycombExists[i]) {
        window.draw(honeycombSprites[i]);
    }
}

    // Draw  hunter bees
    for (int i = 0; i < maxhunterBees; ++i) {
        if (hunterbeeExists[i]) {
            window.draw(hunterbeeSprites[i]);
        }
    }
    
    
   // Draw hunter honeycombs
for (int i = 0; i < maxhunterBees; ++i) {
    if (hunterhoneycombExists[i]) {
        window.draw(hunterhoneycombSprites[i]);
    }
}
        
 
// Draw emojis
for (int i = 0; i < maxBees; ++i) {
    if (emojiExists[i]) {
        window.draw(emojiSprites[i]);
    }
}
 
 
  // Draw bird
        if (birdActive) {
            window.draw(bird);
        }
 
 



if (isPlayerCompletlyBlocked(player_x, maxObstacles, obstacleExists, obstacleX, obstacleY) &&
    obstacleResetClock.getElapsedTime().asSeconds() >= obstacleResetCooldown) {
    // Reset all obstacles to their initial state
    for (int i = 0; i < maxObstacles; ++i) {
        obstacleExists[i] = false;  // Clear all existing obstacles
    }
    
    // Restart the reset clock
    obstacleResetClock.restart();



    // Ensure we have spray cans to remove
                        k = 0;
                        if(j+1<3)
                            j++;
                        
                        sprayCanRectangles[j-1].setPosition(-100.f, -100.f);
                        
                        for (int i = 0; i < 2; i++) {
                            sprayCanBullets[i] = sprayCanBullets[i + 1];
                            sprayCanSprites[i] = sprayCanSprites[i + 1];
                        }
                        sprayCanBullets[2] = maxBullets; 



}



    // Draw obstacles
for (int i = 0; i < maxBees; ++i) {
if(obstacleExists[i]) {
    window.draw(obstacleSprites[i]);
 }
}
    
    
    

	// Update and move the bullet// new addition
            if (bullet_exists) {
                moveBullet(bullet_y, bullet_exists, bulletClock);
            }


    // Draw player sprite and health bar
    drawPlayer(window, player_x, player_y, playerSprite, mainHealth, tempVar);


    // Draw bullet if it exists
    if (bullet_exists) {
        drawBullet(window, bullet_x, bullet_y, bulletSprite);
    }

}
else if(currentLevel==4)//boss level
{}


    
        } 
Text scoreText;
scoreText.setFont(font);
scoreText.setString("Score: " + to_string(score)); // Convert score to string
scoreText.setCharacterSize(30); // Set the character size
scoreText.setFillColor(Color::Black); // Set the text color
scoreText.setPosition(resolutionX - 200, resolutionY - 50); // Position it at the bottom right corner

// Draw the score text
window.draw(scoreText);

        window.display(); 
    }
    
    return 0;
}

// Function to handle menu input with passed references
void handleMenuInput(bool& isNameInputActive, bool& isMenuActive, bool& isLevelMenuActive, 
                     int& selectedMenuIndex, int& selectedLevelIndex, 
                     int& currentLevel, string& inputName, bool& isScoresMenuActive) {

    const int menuItemsCount = 4;
    const int levelItemsCount = 4;
    const Time MENU_TRANSITION_DELAY = milliseconds(200);

// Name input menu handling
    if (isNameInputActive) {
        // Handle text input for name
        if (Keyboard::isKeyPressed(Keyboard::BackSpace) && !inputName.empty()) {
            inputName.pop_back();
            sleep(milliseconds(150));  // Prevent rapid deletion
        }
        
        // Handle text input
        for (int i = 0; i < 26; ++i) {
            if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(Keyboard::A + i))) {
                // Add letter (uppercase)
                inputName += static_cast<char>('A' + i);
                sleep(milliseconds(150));  // Prevent rapid typing
            }
        }

        // Confirm name input
        if (Keyboard::isKeyPressed(Keyboard::Enter) && !inputName.empty()) {
            isNameInputActive = false;
            isMenuActive = true;
            sleep(MENU_TRANSITION_DELAY);
        }

        //In order to exit initially
        if (Keyboard::isKeyPressed(Keyboard::Escape) && !inputName.empty()) {
          exit(0);
        }

    }
    else if (isMenuActive) {
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (selectedMenuIndex > 0) selectedMenuIndex--;
        } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (selectedMenuIndex < menuItemsCount - 1) selectedMenuIndex++;
        } else if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            if (selectedMenuIndex == 0) {
                isMenuActive = false;
            } else if (selectedMenuIndex == 1) {
                isLevelMenuActive = true;
                isMenuActive = false;
                selectedLevelIndex = 0;
                sleep(MENU_TRANSITION_DELAY);
            } else if (selectedMenuIndex == 2) {
                isScoresMenuActive = true;
                isMenuActive = false;
            }
            else if (selectedMenuIndex == 3){
             exit(0);  
            }
        }
    } else if (isLevelMenuActive) {
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (selectedLevelIndex > 0) selectedLevelIndex--;
        } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (selectedLevelIndex < levelItemsCount - 1) selectedLevelIndex++;
        } else if (Keyboard::isKeyPressed(Keyboard::Escape)) { 
            isLevelMenuActive = false;
            isMenuActive = true;
        } else if (Keyboard::isKeyPressed(Keyboard::Tab)) {
            currentLevel = selectedLevelIndex + 1;
            isLevelMenuActive = false;
            isMenuActive = false;
            sleep(MENU_TRANSITION_DELAY);
        }
    }
    else if(isScoresMenuActive)
    {

                if (Keyboard::isKeyPressed(Keyboard::Escape)) { 
            isScoresMenuActive = false;
            isMenuActive = true;
        }


    }
}

// Existing function definitions remain the same
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, RectangleShape& health, int& K) {
    playerSprite.setPosition(player_x, player_y);
    window.draw(playerSprite);
    health.setPosition(player_x+23, player_y+33 + 3.f*K);
    window.draw(health);
}

void drawMenu(RenderWindow& window, Font& font, int& selectedMenuIndex) {
    const int menuItemsCount = 4;
    string menuItems[menuItemsCount] = {"Continue Game", "Select Level","View Scores", "Exit"};

    for (int i = 0; i < menuItemsCount; i++) {
        Text menuText;
        menuText.setFont(font);
        menuText.setString(menuItems[i]);
        menuText.setCharacterSize(50);
        menuText.setPosition(300, 200 + i * 50 + i * 35);
        if (i == selectedMenuIndex) {
            menuText.setFillColor(Color::Yellow);
        } else {
            menuText.setFillColor(Color::White);
        }
        window.draw(menuText);
    }
}

void drawLevelMenu(RenderWindow& window, Font& font, int& selectedLevelIndex) {
    const int levelItemsCount = 4;
    string levelItems[levelItemsCount] = {"Level 1", "Level 2", "Level 3","Fight Boss"};

    for (int i = 0; i < levelItemsCount; i++) {
        Text levelText;
        levelText.setFont(font);
        levelText.setString(levelItems[i]);
        levelText.setCharacterSize(50);
        levelText.setPosition(360, 200 + i * 50 + i * 35);
        if (i == selectedLevelIndex) {
            levelText.setFillColor(Color::Red);
        } else {
            levelText.setFillColor(Color::White);
        }
        window.draw(levelText);
    }
}

void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 1)
        return;

    bulletClock.restart();
    bullet_y -= 10;
    if (bullet_y < -32)
        bullet_exists = false;
}


void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}


bool isObstacleNearby(float x, int index, const int max, bool existence[], float obsX[]) {
    for (int i = 0; i < max; i++) {
        if (i != index && existence[i]) {
            float diff = obsX[i] - x;
            if (diff < 0) diff = -diff; // absolute value calculation
            if (diff < 50) {
                return true;
            }
        }
    }
    return false;
}


bool isCollidingWithObstacle(float playerX, float playerY, const int maxObstacles, bool obstacleExists[], float obstacleX[], float obstacleY[]) {
    for (int i = 0; i < maxObstacles; ++i) {
        if (obstacleExists[i]) {
            // Basic bounding box collision check
            if (playerX + boxPixelsX > obstacleX[i] && playerX < obstacleX[i] + boxPixelsX &&
                playerY + boxPixelsY > obstacleY[i] && playerY < obstacleY[i] + boxPixelsY) {
                return true;
            }
        }
    }
    return false;
}
void drawGameOverScreen(RenderWindow& window, Font& font) {
    // Create a darkened background rectangle
    RectangleShape background;
    background.setSize(Vector2f(resolutionX, resolutionY));
    background.setFillColor(Color(0, 0, 0, 200)); // Dark semi-transparent black
    window.draw(background);

    // Game Over text
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(Color::Red);
    
    // Center the text manually without using any .get methods
    gameOverText.setPosition(
        resolutionX / 2.0f - 300, 
        resolutionY / 2.0f - 50
    );
    
    window.draw(gameOverText);

    // Restart instructions
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Press Escape to return to Main Menu");
    instructionText.setCharacterSize(30);
    instructionText.setFillColor(Color::White);
    
    instructionText.setPosition(
        resolutionX / 2.0f - 300, 
        resolutionY / 2.0f + 100
    );
    
    window.draw(instructionText);
}

void drawNameInputMenu(RenderWindow& window, Font& font, string& inputName, bool& isNameInputActive) {
 
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Buzz Bombers");
    titleText.setCharacterSize(70);
    titleText.setFillColor(sf::Color(139, 128, 0));
    titleText.setPosition(210, 100);


    Text promptText;
    promptText.setFont(font);
    promptText.setString("Enter Your Name:");
    promptText.setCharacterSize(40);
    promptText.setFillColor(Color::White);
    promptText.setPosition(250, 250);

    
    Text nameText;
    nameText.setFont(font);
    nameText.setString(inputName + "_");  // Add cursor
    nameText.setCharacterSize(50);
    nameText.setFillColor(Color::Yellow);
    nameText.setPosition(250, 320);

    //Instructions for tha user
    Text instructText;
    instructText.setFont(font);
    instructText.setString("Press Enter to Continue");
    instructText.setCharacterSize(40);
    instructText.setFillColor(Color::White);
    instructText.setPosition(190, 400);


    Text ExitText;
    ExitText.setFont(font);
    ExitText.setString("Press Escape to Exit");
    ExitText.setCharacterSize(30);
    ExitText.setFillColor(Color::Red);
    ExitText.setPosition(280, 470);


    window.draw(titleText);
    window.draw(promptText);
    window.draw(nameText);
    window.draw(instructText);
    window.draw(ExitText);
}

bool isPlayerCompletlyBlocked(float playerX, const int maxObstacles, bool obstacleExists[], float obstacleX[], float obstacleY[]) {
    // Check if there are obstacles on both left and right sides within 64 pixels
    bool leftObstacle = false;
    bool rightObstacle = false;

    for (int i = 0; i < maxObstacles; ++i) {
        if (obstacleExists[i]) {
            // Check left side
            if (obstacleX[i] < playerX && playerX - obstacleX[i] <= 84) {
                leftObstacle = true;
            }
            
            // Check right side
            if (obstacleX[i] > playerX && obstacleX[i] - playerX <= 84) {
                rightObstacle = true;
            }
        }
    }

    // If both left and right sides have obstacles within 64 pixels, return true
    return leftObstacle && rightObstacle;
}
void drawScoresMenu(RenderWindow& window, Font& font, string names[], int scores[], int count) {
    // Title for Scores Menu
    Text titleText;
    titleText.setFont(font);
    titleText.setString("High Scores");
    titleText.setCharacterSize(60);
    titleText.setFillColor(Color::Yellow);
    titleText.setPosition(300, 100);

    // Display scores
    string scoreString;
    for (int i = 0; i < count; i++) {
        scoreString += to_string(i + 1) + ". " + names[i] + " - " + to_string(scores[i]) + "\n";
    }

    Text scoresText;
    scoresText.setFont(font);
    scoresText.setString(scoreString);
    scoresText.setCharacterSize(40);
    scoresText.setFillColor(Color::White);
    scoresText.setPosition(350, 250);

    // Back instruction
    Text backText;
    backText.setFont(font);
    backText.setString("Press Escape to Go Back");
    backText.setCharacterSize(30);
    backText.setFillColor(Color::Red);
    backText.setPosition(280, 500);

    window.draw(titleText);
    window.draw(scoresText);
    window.draw(backText);
}

void updateScores(string names[], int scores[], int& count, string inputName, int score) {
    // Update the fifth score
    if (count < 5) {
        names[count] = inputName;
        scores[count] = score;
        count++;
    } else {
        names[4] = inputName; // Replace the fifth score
        scores[4] = score;
    }

    // Write updated scores back to the file
    ofstream file("scores.txt");
    for (int i = 0; i < count; i++) {
        file << names[i] << " " << scores[i] << endl;
    }
    file.close();
}

void readScores(string names[], int scores[], int& count) {
    ifstream file("scores.txt");
    count = 0;
    while (file >> names[count] >> scores[count]) {
        count++;
    }
    file.close();
}




