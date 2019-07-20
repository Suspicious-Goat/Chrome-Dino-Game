class DinoGame{
private:
  int w,h;
  Dino* dino;
  bool gameover;
  int gravity;
  float speed;
  int score;
  int bestScore;
  vector<Obstacle*> obstacles;

  vector<Cloud*> clouds;

  vector<Bird*> birds;

  int obstacleTimer, randomAdditionObstacle, cloudTimer, randomAdditionCloud, birdTimer, randomAdditionBird;

  Texture cactiTexture;
  Texture smallCactusTexture;
  Texture bigCactusTexture;

  //for test only
  bool death;


public:
  DinoGame(int height,int width){
    srand(time(NULL));
    //initialize random distances to 0
    randomAdditionObstacle = 0;
    randomAdditionCloud = 0;
    randomAdditionBird = 0;

    //set dimensions
    h=height;w=width;

    //create player
    dino = new Dino(100,600);

    //reset game if true
    gameover = false;

    //physics constants
    gravity = 1;
    speed = 10;

    //times
    obstacleTimer = 0;
    cloudTimer = 0;
    birdTimer = 0;

    //scores
    score = 0;
    bestScore = 0;

    //textures
    cactiTexture.loadFromFile("art/cacti.png");
    smallCactusTexture.loadFromFile("art/cactusSmall.png");
    bigCactusTexture.loadFromFile("art/cactusBig.png");

    //for test only
    death = true;
  }

  ~DinoGame(){
    delete dino;
    for (int i=0;i<obstacles.size();i++){
      delete obstacles[i];
    }
    for (int i=0;i<clouds.size();i++){
      delete clouds[i];
    }
    for (int i=0;i<birds.size();i++){
      delete birds[i];
    }
  }


  void Run(){
    RenderWindow window(VideoMode(h,w), "Dino Game", Style::Close | Style::Resize);
    window.setFramerateLimit(60);

    Font font;
    font.loadFromFile("art/font.ttf");


    while(window.isOpen()){

        //input
        Event evnt;
        while(window.pollEvent(evnt)){
          switch(evnt.type){
            case Event::Closed:
              window.close();
              break;

            case Event::KeyPressed:
              if(gameover){//reset game
                dino->Reset();

                obstacles.clear();
                clouds.clear();
                birds.clear();

                speed = 10;
                score = 0;

                gameover = false;
              }
            }
        }

        //check is game is still running
        if (!gameover){

          Input();

          window.clear(sf::Color(255, 255, 255));

          DrawAll(window, font);

          dino->Update(gravity, false);

          updateObstacles();
          updateClouds();
          updateBirds();

          if(death)
            Logic();
          //speed up game
          speed += .002;

          //increment score
          score ++;
        }else{//game over
          if(score > bestScore){
            bestScore = score;
          }

          window.clear(sf::Color(255, 255, 255));

          dino->unduck();

          DrawAll(window, font);

          dino->Update(gravity,true);
        }
        //display window
        window.display();


      }
  }
private:
  string getScoreString(int score){
    switch(to_string(score/10).length()){
      case 1:
        return "0000"+to_string(score/10);
        break;
      case 2:
        return "000"+to_string(score/10);
        break;
      case 3:
        return "00"+to_string(score/10);
        break;
      case 4:
        return "0"+to_string(score/10);
        break;
      case 5:
        return to_string(score/10);
        break;
      default:
        return "99999";
        break;
    }
  }
  void DrawAll(RenderWindow& window, Font& font){

    RectangleShape ground(Vector2f(1500.0f,125.0f));
    ground.setOrigin(750.0f,62.5f);
    ground.setFillColor(Color::White);
    ground.setPosition(750,700);
    ground.setOutlineColor(Color::Black);
    ground.setOutlineThickness(2.0f);
    window.draw(ground);

    for(int i=0;i<obstacles.size(); i++){
      obstacles[i]->Draw(window);
    }
    for(int i=0;i<clouds.size(); i++){
      clouds[i]->Draw(window);
    }
    for(int i=0;i<birds.size(); i++){
      birds[i]->Draw(window);
    }

    dino->Draw(window);

    //score
    Text scoreText(getScoreString(score), font, 30);
    scoreText.setOrigin(0,0);
    scoreText.setPosition(w+600, 30);
    scoreText.setFillColor(Color::Black);
    window.draw(scoreText);

    if (bestScore != 0){
      //high score
      Text bestScoreText("Hi "+getScoreString(bestScore), font, 30);
      bestScoreText.setOrigin(0,0);
      bestScoreText.setPosition(w+350, 30);
      bestScoreText.setFillColor(Color::Black);
      window.draw(bestScoreText);
    }
  }

  //----------------------------------------------------------------------------
  //                                   Vectors
  //----------------------------------------------------------------------------
  void addCloud(){
    Cloud* cloud = new Cloud(w);
    clouds.push_back(cloud);

    randomAdditionCloud = rand()%75;
    cloudTimer = 0;
  }


  void updateClouds(){

    cloudTimer ++;

    //move clouds
    for(int i=0;i<clouds.size(); i++){
      clouds[i]->Update(speed);
      if(clouds[i]->getX() < 5){
        clouds.erase(clouds.begin()+i);
      }
    }

    //add clouds at random intervals
    if(randomAdditionCloud+200 == cloudTimer){
      addCloud();
    }

  }

  void addBird(){
    Bird* bird = new Bird();
    birds.push_back(bird);

    randomAdditionBird = rand()%100;
    birdTimer = 0;

    obstacleTimer = 0;
  }

  void updateBirds(){
        birdTimer ++;

        //move birds
        for(int i=0;i<birds.size(); i++){
          birds[i]->Update(speed);
          if(birds[i]->getX() < 5){
            birds.erase(birds.begin()+i);
          }
        }

        //add birds at random intervals
        if(randomAdditionBird+200 <= birdTimer && obstacleTimer > 55 && score/10 > 300){
          addBird();
        }
  }
  void addObstacle(){
    int type = rand()%3;
    switch(type){
      case 0://big cactus
        {Obstacle* obstacle = new Obstacle(1500,type,bigCactusTexture);
        obstacles.push_back(obstacle);}
        break;
      case 1://small cactus
        {Obstacle* obstacle = new Obstacle(1500,type,smallCactusTexture);
        obstacles.push_back(obstacle);}
        break;
      case 2://cacti
        {Obstacle* obstacle = new Obstacle(1500,type,cactiTexture);
        obstacles.push_back(obstacle);}
        break;
    }


    randomAdditionObstacle = (int)(rand()%(50/(int)((float)speed*.1)));

    obstacleTimer = 0;
  }

  void updateObstacles(){

    obstacleTimer ++;

    //move obstacles
    for(int i=0;i<obstacles.size(); i++){
      obstacles[i]->Update(speed);
      if(obstacles[i]->getX() < 5){
        obstacles.erase(obstacles.begin()+i);
      }
    }

    //add obstacles at random intervals with minimum of 60
    if(randomAdditionObstacle+60 == obstacleTimer){
      addObstacle();
    }

  }
  //----------------------------------------------------------------------------
  //                              Input & Logic
  //----------------------------------------------------------------------------

  void Input(){
    //keyboard input
    if(Keyboard::isKeyPressed(Keyboard::Key::Space)){
      dino->Jump();
    }
    if(Keyboard::isKeyPressed(Keyboard::Key::Down)){
      dino->Duck();
    }else{
      dino->unduck();
    }
  }
  bool checkCollision(int x, int y, int oWidth, int oHeight, int xTwo, int yTwo, int oTwoWidth, int oTwoHeight){
    // AABB 1
    int x1Min = x;
    int x1Max = x+oWidth;
    int y1Max = y+oHeight;
    int y1Min = y;

    // AABB 2
    int x2Min = xTwo;
    int x2Max = xTwo+oTwoWidth;
    int y2Max = yTwo+oTwoHeight;
    int y2Min = yTwo;

    // Collision tests
    if( x1Max < x2Min || x1Min > x2Max ){
      return false;
    }
    if( y1Max < y2Min || y1Min > y2Max ){
      return false;
    }

    return true;
  }

  void Logic(){
    int dinoX = 100;
    int dinoY = dino->getY();
    int dinoW = 80;
    int dinoH = 100;

    if(dino->isDuck()){
      dinoW = 140;
      dinoH = 44;
    }

    for(int i=0;i<obstacles.size();i++){
      int obstacleX = obstacles[i]->getX();
      int obstacleY = obstacles[i]->getY();
      int obstacleW = obstacles[i]->getW();
      int obstacleH = obstacles[i]->getH();

      if(checkCollision(dinoX,dinoY,dinoW,dinoH,obstacleX,obstacleY,obstacleW,obstacleH)){
        gameover = true;
      }

    }

    int birdW = 90;
    int birdH = 100;

    for (int i=0;i < birds.size(); i++){
      int birdX = birds[i]->getX();
      int birdY = birds[i]->getY();

      if((birdY == 510 || birdY == 550) && dino->isDuck()){

      }else{
        if(checkCollision(dinoX,dinoY,dinoW,dinoH,birdX,birdY,birdW,birdH)){
          gameover = true;
        }
      }


    }
  }
};
